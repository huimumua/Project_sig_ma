/**
@file   zip_supervision.c - Set and poll supervision facility implementation.

@author David Chow

@version    1.0 1-9-16  Initial release

@copyright © 2016 SIGMA DESIGNS, INC. THIS IS AN UNPUBLISHED WORK PROTECTED BY SIGMA DESIGNS, INC.
AS A TRADE SECRET, AND IS NOT TO BE USED OR DISCLOSED EXCEPT AS PROVIDED Z-WAVE CONTROLLER DEVELOPMENT KIT
LIMITED LICENSE AGREEMENT. ALL RIGHTS RESERVED.

NOTICE: ALL INFORMATION CONTAINED HEREIN IS CONFIDENTIAL AND/OR PROPRIETARY TO SIGMA DESIGNS
AND MAY BE COVERED BY U.S. AND FOREIGN PATENTS, PATENTS IN PROCESS, AND ARE PROTECTED BY TRADE SECRET
OR COPYRIGHT LAW. DISSEMINATION OR REPRODUCTION OF THE SOURCE CODE CONTAINED HEREIN IS EXPRESSLY FORBIDDEN
TO ANYONE EXCEPT LICENSEES OF SIGMA DESIGNS  WHO HAVE EXECUTED A SIGMA DESIGNS' Z-WAVE CONTROLLER DEVELOPMENT KIT
LIMITED LICENSE AGREEMENT. THE COPYRIGHT NOTICE ABOVE IS NOT EVIDENCE OF ANY ACTUAL OR INTENDED PUBLICATION OF
THE SOURCE CODE. THE RECEIPT OR POSSESSION OF  THIS SOURCE CODE AND/OR RELATED INFORMATION DOES NOT CONVEY OR
IMPLY ANY RIGHTS  TO REPRODUCE, DISCLOSE OR DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE, USE, OR SELL A PRODUCT
THAT IT  MAY DESCRIBE.


THE SIGMA PROGRAM AND ANY RELATED DOCUMENTATION OR TOOLS IS PROVIDED TO COMPANY "AS IS" AND "WITH ALL FAULTS",
WITHOUT WARRANTY OF ANY KIND FROM SIGMA. COMPANY ASSUMES ALL RISKS THAT LICENSED MATERIALS ARE SUITABLE OR ACCURATE
FOR COMPANY'S NEEDS AND COMPANY'S USE OF THE SIGMA PROGRAM IS AT COMPANY'S OWN DISCRETION AND RISK. SIGMA DOES NOT
GUARANTEE THAT THE USE OF THE SIGMA PROGRAM IN A THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICES ENVIRONMENT WILL BE:
(A) PERFORMED ERROR-FREE OR UNINTERRUPTED; (B) THAT SIGMA WILL CORRECT ANY THIRD PARTY SERVICE ENVIRONMENT OR
CLOUD SERVICE ENVIRONMENT ERRORS; (C) THE THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICE ENVIRONMENT WILL
OPERATE IN COMBINATION WITH COMPANY'S CONTENT OR COMPANY APPLICATIONS THAT UTILIZE THE SIGMA PROGRAM;
(D) OR WITH ANY OTHER HARDWARE, SOFTWARE, SYSTEMS, SERVICES OR DATA NOT PROVIDED BY SIGMA. COMPANY ACKNOWLEDGES
THAT SIGMA DOES NOT CONTROL THE TRANSFER OF DATA OVER COMMUNICATIONS FACILITIES, INCLUDING THE INTERNET, AND THAT
THE SERVICES MAY BE SUBJECT TO LIMITATIONS, DELAYS, AND OTHER PROBLEMS INHERENT IN THE USE OF SUCH COMMUNICATIONS
FACILITIES. SIGMA IS NOT RESPONSIBLE FOR ANY DELAYS, DELIVERY FAILURES, OR OTHER DAMAGE RESULTING FROM SUCH ISSUES.
SIGMA IS NOT RESPONSIBLE FOR ANY ISSUES RELATED TO THE PERFORMANCE, OPERATION OR SECURITY OF THE THIRD PARTY SERVICE
ENVIRONMENT OR CLOUD SERVICES ENVIRONMENT THAT ARISE FROM COMPANY CONTENT, COMPANY APPLICATIONS OR THIRD PARTY CONTENT.
SIGMA DOES NOT MAKE ANY REPRESENTATION OR WARRANTY REGARDING THE RELIABILITY, ACCURACY, COMPLETENESS, CORRECTNESS, OR
USEFULNESS OF THIRD PARTY CONTENT OR SERVICE OR THE SIGMA PROGRAM, AND DISCLAIMS ALL LIABILITIES ARISING FROM OR RELATED
TO THE SIGMA PROGRAM OR THIRD PARTY CONTENT OR SERVICES. TO THE EXTENT NOT PROHIBITED BY LAW, THESE WARRANTIES ARE EXCLUSIVE.
SIGMA OFFERS NO WARRANTY OF NON-INFRINGEMENT, TITLE, OR QUIET ENJOYMENT. NEITHER SIGMA NOR ITS SUPPLIERS OR LICENSORS
SHALL BE LIABLE FOR ANY INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES OR LOSS (INCLUDING DAMAGES FOR LOSS OF
BUSINESS, LOSS OF PROFITS, OR THE LIKE), ARISING OUT OF THIS AGREEMENT WHETHER BASED ON BREACH OF CONTRACT,
INTELLECTUAL PROPERTY INFRINGEMENT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY, PRODUCT LIABILITY OR OTHERWISE,
EVEN IF SIGMA OR ITS REPRESENTATIVES HAVE BEEN ADVISED OF OR OTHERWISE SHOULD KNOW ABOUT THE POSSIBILITY OF SUCH DAMAGES.
THERE ARE NO OTHER EXPRESS OR IMPLIED WARRANTIES OR CONDITIONS INCLUDING FOR SOFTWARE, HARDWARE, SYSTEMS, NETWORKS OR
ENVIRONMENTS OR FOR MERCHANTABILITY, NONINFRINGEMENT, SATISFACTORY QUALITY AND FITNESS FOR A PARTICULAR PURPOSE.

The Sigma Program  is not fault-tolerant and is not designed, manufactured or intended for use or resale as on-line control
equipment in hazardous environments requiring fail-safe performance, such as in the operation of nuclear facilities,
aircraft navigation or communication systems, air traffic control, direct life support machines, or weapons systems,
in which the failure of the Sigma Program, or Company Applications created using the Sigma Program, could lead directly
to death, personal injury, or severe physical or environmental damage ("High Risk Activities").  Sigma and its suppliers
specifically disclaim any express or implied warranty of fitness for High Risk Activities.Without limiting Sigma's obligation
of confidentiality as further described in the Z-Wave Controller Development Kit Limited License Agreement, Sigma has no
obligation to establish and maintain a data privacy and information security program with regard to Company's use of any
Third Party Service Environment or Cloud Service Environment. For the avoidance of doubt, Sigma shall not be responsible
for physical, technical, security, administrative, and/or organizational safeguards that are designed to ensure the
security and confidentiality of the Company Content or Company Application in any Third Party Service Environment or
Cloud Service Environment that Company chooses to utilize.
*/

#include <stdlib.h>
#ifndef OS_MAC_X
#include <malloc.h>
#endif
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/zip_api_pte.h"
#include "../include/zip_api_util.h"
#include "../include/zip_supervision.h"


/**
@defgroup If_Spv Supervision Interface APIs
Used to create and delete set and poll supervision commands to a device
@{
*/

/**
zwspv_cb - Create a callback request to the callback thread
@param[in]	ifd         interface descriptor
@param[in]	cb		    post-set polling callback function.  NULL if no callback required.
@param[in]	usr_param   user-defined parameter passed in callback.
@param[in]  reason      reason of the callback, see ZWPSET_REASON_XXX
@return
*/
void zwspv_cb(zwifd_p ifd, zw_postset_fn cb, void *usr_param, int reason)
{
    zwnet_p         nw;
    zwnet_cb_req_t  cb_req = {0};

    if (!ifd || !cb)
    {
        return;
    }

    nw = ifd->net;

    //Send request to callback thread
    cb_req.type = CB_TYPE_POST_SET;
    cb_req.cb = cb;
    //cb_req.param.post_set.ifd = *ifd;
    memcpy(&cb_req.param.post_set.ifd, ifd, sizeof(zwifd_t));

    cb_req.param.post_set.user = usr_param;
    cb_req.param.post_set.reason = reason;

    util_list_add(nw->cb_mtx, &nw->cb_req_hd, (uint8_t *)&cb_req, sizeof(zwnet_cb_req_t));

    plt_sem_post(nw->cb_sem);
}


/**
zwspv_find_ent - Find a poll entry by node id and session id
@param[in]	spv_ctx	    context
@param[in]	node_id     node id
@param[in]	ssn_id      session id
@return poll entry if found; otherwise NULL.
*/
static spv_q_ent_t *zwspv_find_ent(zwspv_ctx_t *spv_ctx, uint8_t node_id, uint8_t ssn_id)
{
    spv_q_ent_t *ent;

    ent = spv_ctx->spv_lst_hd;

    while (ent)
    {
        if ((ent->ifd.nodeid == node_id) && (ent->ssn_id == ssn_id))
        {   //Found
            return ent;
        }

        //Next
        ent = ent->next;
    }
    return NULL;
}


/**
zwspv_find_ent_by_node - Find a poll entry by node id
@param[in]	spv_ctx	context
@param[in]	node_id	    Node id
@return poll entry if found; otherwise NULL.
*/
static spv_q_ent_t *zwspv_find_ent_by_node(zwspv_ctx_t *spv_ctx, uint8_t node_id)
{
    spv_q_ent_t *ent;

    ent = spv_ctx->spv_lst_hd;

    while (ent)
    {
        if (ent->ifd.nodeid == node_id)
        {   //Found
            return ent;
        }

        //Next
        ent = ent->next;
    }
    return NULL;
}


/**
zwspv_find_ent_by_intf - Find a poll entry by interface
@param[in]	spv_ctx	    context
@param[in]	ifd	        interface
@return poll entry if found; otherwise NULL.
*/
static spv_q_ent_t *zwspv_find_ent_by_intf(zwspv_ctx_t *spv_ctx, zwifd_p ifd)
{
    spv_q_ent_t *ent;

    ent = spv_ctx->spv_lst_hd;

    while (ent)
    {
        if ((ent->ifd.nodeid == ifd->nodeid) && (ent->ifd.epid == ifd->epid)
            && (ent->ifd.cls == ifd->cls))
        {   //Found
            return ent;
        }

        //Next
        ent = ent->next;
    }
    return NULL;
}


/**
zwspv_add_ent_endlist - Add entry to end of list
@param[in]	spv_ctx		Context
@param[in]	ent		        New poll entry
@return     1 if the ent is the only entry in the list after adding; else return 0.
*/
static int zwspv_add_ent_endlist(zwspv_ctx_t *spv_ctx, spv_q_ent_t *ent)
{
    //The list is empty
    if (!spv_ctx->spv_lst_hd && !spv_ctx->spv_lst_tail)
    {
        spv_ctx->spv_lst_hd = spv_ctx->spv_lst_tail = ent;
        ent->next = ent->prev = NULL;
        return 1;
    }

    //Adjust the former tail
    spv_ctx->spv_lst_tail->next = ent;

    ent->prev = spv_ctx->spv_lst_tail;
    ent->next = NULL;

    //Assign the entry as tail
    spv_ctx->spv_lst_tail = ent;

    return 0;

}


/**
zwspv_rm_ent - Remove entry from list
@param[in]	spv_ctx		Context
@param[in]	ent		        Poll entry
@return
@pre Caller must lock spv_mtx
@post ent should not be used as it is freed
*/
static void zwspv_rm_ent(zwspv_ctx_t *spv_ctx, spv_q_ent_t *ent)
{
    //There is only 1 entry in the list
    if (spv_ctx->spv_lst_hd == spv_ctx->spv_lst_tail)
    {
        spv_ctx->spv_lst_hd = spv_ctx->spv_lst_tail = NULL;
        free(ent);
        return;
    }

    //The entry is the head of the list
    if (spv_ctx->spv_lst_hd == ent)
    {
        spv_ctx->spv_lst_hd = ent->next;
        spv_ctx->spv_lst_hd->prev = NULL;
    }
    //The entry is the tail of the list
    else if (spv_ctx->spv_lst_tail == ent)
    {
        spv_ctx->spv_lst_tail = ent->prev;
        spv_ctx->spv_lst_tail->next = NULL;
    }
    else
    {
        ent->prev->next = ent->next;
        ent->next->prev = ent->prev;
    }

    free(ent);
}


/**
zwspv_gen_rpt_cb - Generate report callback to simulate live report received from device
@param[in]	ifd         interface descriptor
@param[in]	rpt_cmd_buf	command buffer which contains report command and parameters, used when target is hit
@param[in]	rpt_buf_len	length of rpt_cmd_buf in bytes
@return
*/
static void zwspv_gen_rpt_cb(zwifd_p ifd, uint8_t *rpt_cmd_buf, int rpt_buf_len)
{
    zwnet_p         nw = (zwnet_p)ifd->net;
    zwnet_cb_req_t  cb_req;

    //Send request to callback thread
    cb_req.type = CB_TYPE_SIMULATED_REPORT;
    cb_req.cb = NULL;
    memcpy(&cb_req.param.sim_rpt.ifd, ifd, sizeof(zwifd_t));
    cb_req.param.sim_rpt.rpt_len = rpt_buf_len;
    memcpy(cb_req.param.sim_rpt.rpt, rpt_cmd_buf,rpt_buf_len);

    util_list_add(nw->cb_mtx, &nw->cb_req_hd,
                  (uint8_t *)&cb_req, sizeof(zwnet_cb_req_t));
    plt_sem_post(nw->cb_sem);

}


/**
zwspv_rpt_hdlr - Supervision report handler
@param[in]	spv_ctx	    Supervision context
@param[in]	msg	        Message
@return
*/
static void zwspv_rpt_hdlr(zwspv_ctx_t *spv_ctx, spv_ev_msg_rpt_t *rpt_msg)
{
    spv_q_ent_t *spv_ent;
    int         cb_reason = -1;

    if ((spv_ent = zwspv_find_ent(spv_ctx, rpt_msg->ifd.nodeid, rpt_msg->ssn_id)) == NULL)
    {   //Not found, the report is unexpected
        return;
    }


    switch (rpt_msg->status)
    {
        case SPV_STATUS_NO_SUPPORT:
            cb_reason = ZWPSET_REASON_UNSUPPORTED;
            break;

        case SPV_STATUS_WORKING:
            {
                uint16_t    tmout = rpt_msg->duration;
                //Recalculate the next timeout
                if (tmout == 0)
                {   //Invalid
                    cb_reason = ZWPSET_REASON_FAILED;
                    break;
                }
                if (tmout < 0x80)
                {
                    spv_ent->tmout = (tmout * SPV_TICK_PER_SEC) + SPV_ADDTNL_TIMEOUT + spv_ctx->tmr_tick;
                    break;
                }
                if (tmout < 0xFE)
                {
                    spv_ent->tmout = ((tmout - 0x7F) * 60 * SPV_TICK_PER_SEC) + SPV_ADDTNL_TIMEOUT + spv_ctx->tmr_tick;
                    break;
                }
                if (tmout == 0xFE)
                {   //Unknown duration, give it a default timeout
                    spv_ent->tmout = SPV_DEFAULT_TIMEOUT + spv_ctx->tmr_tick;
                    break;
                }

                //Invalid
                cb_reason = ZWPSET_REASON_FAILED;
            }
            break;

        case SPV_STATUS_FAIL:
            cb_reason = ZWPSET_REASON_FAILED;
            break;

        case SPV_STATUS_BUSY:
            cb_reason = ZWPSET_REASON_BUSY;
            break;

        case SPV_STATUS_SUCCESS:
            cb_reason = ZWPSET_REASON_TGT_HIT;
            break;

    }

    if (cb_reason >= 0)
    {
        if (cb_reason == ZWPSET_REASON_TGT_HIT)
        {   //Generate report callback if target hit
            if (spv_ent->rpt_cmd_buf_type == SUPERVSN_RPT_TYPE_RPT)
            {
                zwspv_gen_rpt_cb(&spv_ent->ifd, spv_ent->rpt_cmd_buf, spv_ent->rpt_buf_len);
            }
            else
            {
                //Send the report get
                zwif_exec_ex(&spv_ent->ifd, spv_ent->rpt_cmd_buf, spv_ent->rpt_buf_len,
                             NULL, NULL, ZWIF_OPT_POLL, NULL);

            }
        }

        zwspv_cb(&spv_ent->ifd, spv_ent->cb, spv_ent->usr_param, cb_reason);

        //Remove the entry
        zwspv_rm_ent(spv_ctx, spv_ent);
    }
}


/**
zwspv_rm_poll - Remove poll entries of a node
@param[in]	spv_ctx	Poll context
@param[in]	node_id	    Node id
@return
*/
static void zwspv_rm_poll(zwspv_ctx_t *spv_ctx, uint8_t node_id)
{
    spv_q_ent_t *spv_ent;

    while ((spv_ent = zwspv_find_ent_by_node(spv_ctx, node_id)) != NULL)
    {   //Found.
        zwspv_cb(&spv_ent->ifd, spv_ent->cb, spv_ent->usr_param, ZWPSET_REASON_DEVICE_RMV);
        zwspv_rm_ent(spv_ctx, spv_ent);
    }
}


/**
zwspv_tmr_chk - thread to process timer tick event
@param[in]	data		Context
@return
*/
static void zwspv_tmr_chk(zwspv_ctx_t *spv_ctx)
{
    spv_q_ent_t *spv_ent;
    spv_q_ent_t *prev_ent;      //Pointer to previous list entry

    spv_ent = spv_ctx->spv_lst_hd;

    while (spv_ent)
    {
        //Check entry for expiry
        if (util_tmr_exp_chk(spv_ctx->tmr_tick, spv_ent->tmout))
        {
            //Timeout
            zwspv_cb(&spv_ent->ifd, spv_ent->cb, spv_ent->usr_param, ZWPSET_REASON_TIMEOUT);

            //Save previous entry since the current entry will be removed
            prev_ent = spv_ent->prev;
            //Remove the entry
            zwspv_rm_ent(spv_ctx, spv_ent);
            spv_ent = prev_ent;
        }

        //Next
        if (!spv_ent)
        {
            spv_ent = spv_ctx->spv_lst_hd;
        }
        else
        {
            spv_ent = spv_ent->next;
        }
    }
}


/**
zwspv_tmr_tick_cb - Timer tick timeout callback
@param[in] data     Pointer to poll context
@return
*/
static void    zwspv_tmr_tick_cb(void *data)
{
    zwspv_ctx_t   *spv_ctx = (zwspv_ctx_t *)data;
    spv_evt_msg_t  msg;

    if (spv_ctx->run)
    {
        //Increment timer tick
        spv_ctx->tmr_tick++;

        //Send timer tick event
        msg.type = SPV_EVT_TYPE_TIMER;

        util_msg_loop_send(spv_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(spv_evt_msg_t));
    }
}


/**
zwspv_rm_all_ent - remove all polling requests
@param[in]	spv_ctx   Context
@param[in]	cb_ena      Flag to indicate whether to callback to user application
@return
*/
static void zwspv_rm_all_ent(zwspv_ctx_t *spv_ctx, int cb_ena)
{
    spv_q_ent_t *ent;
    spv_q_ent_t *rm_ent;      //Entry to be removed

    if (!spv_ctx->spv_lst_hd)
    {
        return;
    }

    ent = spv_ctx->spv_lst_hd;

    while (ent)
    {
        rm_ent = ent;
        ent = ent->next;

        if (cb_ena)
        {
            zwspv_cb(&rm_ent->ifd, rm_ent->cb, rm_ent->usr_param, ZWPSET_REASON_DEVICE_RMV);
        }

        free(rm_ent);
    }

    spv_ctx->spv_lst_hd = spv_ctx->spv_lst_tail = NULL;
}


/**
zwspv_on_rpt_rcv - Callback when a report is received
@param[in]	intf	Interface
@param[in]	cmd_buf	The report command received
@param[in]	cmd_len	The length of cmd_buf
@return  Non-zero if the report is processed; else return zero
*/
int zwspv_on_rpt_rcv(zwif_p intf, uint8_t *cmd_buf, uint16_t cmd_len)
{
    if (cmd_len >= 5)
    {
        if ((cmd_buf[0] == COMMAND_CLASS_SUPERVISION) && (cmd_buf[1] == SUPERVISION_REPORT))
        {
            zwspv_ctx_t     *spv_ctx;
            zwifd_t         ifd;
            spv_evt_msg_t   msg;

            zwif_get_desc(intf, &ifd);

            spv_ctx = ifd.net->spv_ctx;

            //Send report arrival event
            msg.type = SPV_EVT_TYPE_REPORT;
            //msg.ev_msg.rpt.ifd = ifd;
            memcpy(&msg.ev_msg.rpt.ifd, &ifd, sizeof(zwifd_t));

            msg.ev_msg.rpt.ssn_id = cmd_buf[2] & 0x3F;
            msg.ev_msg.rpt.more_rpt = cmd_buf[2] & 0x80;
            msg.ev_msg.rpt.status = cmd_buf[3];
            msg.ev_msg.rpt.duration = cmd_buf[4];

            util_msg_loop_send(spv_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(spv_evt_msg_t));

            return 1;
        }
    }
    return 0;
}


/**
zwspv_add_poll - Send set command and add poll entry to end of list
@param[in]	spv_ctx		Context
@param[in]	add_msg		Add new poll entry message
@return
*/
static void zwspv_add_poll(zwspv_ctx_t *spv_ctx, spv_ev_msg_add_t *add_msg)
{
    spv_q_ent_t *spv_ent;
    int         new_entry;
    uint8_t     cmd_buf[SPV_MAX_CMD_LEN + 8];

    //Find if there is an entry in the queue
    if ((spv_ent = zwspv_find_ent_by_intf(spv_ctx, &add_msg->ifd)) == NULL)
    {   //Create new entry
        size_t ent_size = sizeof(spv_q_ent_t);
        spv_ent = (spv_q_ent_t *)calloc(1, ent_size);

        if (!spv_ent)
        {
            return;
        }

        new_entry = 1;

        //spv_ent->ifd = add_msg->ifd;
        memcpy(&spv_ent->ifd, &add_msg->ifd, sizeof(zwifd_t));

    }
    else
    {
        new_entry = 0;
        //debug_zwapi_msg(&spv_ctx->net->plt_ctx, "Found entry with session id:%u", (unsigned)spv_ent->ssn_id);
    }
    //debug_zwapi_msg(&spv_ctx->net->plt_ctx, "Entry new session id:%u", (unsigned)add_msg->ssn_id);

    spv_ent->ssn_id = add_msg->ssn_id;
    spv_ent->cb = add_msg->cb;
    spv_ent->usr_param = add_msg->usr_param;
    spv_ent->rpt_buf_len = add_msg->rpt_buf_len;
    spv_ent->rpt_cmd_buf_type = add_msg->rpt_cmd_buf_type;
    memcpy(spv_ent->rpt_cmd_buf, add_msg->rpt_cmd_buf, add_msg->rpt_buf_len);

    //Create and send supervision get command
    cmd_buf[0] = COMMAND_CLASS_SUPERVISION;
    cmd_buf[1] = SUPERVISION_GET;
    cmd_buf[2] = 0x80 | add_msg->ssn_id;
    cmd_buf[3] = add_msg->set_buf_len;
    memcpy(cmd_buf + 4, add_msg->set_cmd_buf, add_msg->set_buf_len);

    if (zwif_exec_ex(&spv_ent->ifd, cmd_buf, add_msg->set_buf_len + 4, NULL, NULL, ZWIF_OPT_POLL, NULL) >= 0)
    {   //O.k.
        spv_ent->tmout = SPV_RESP_TIMEOUT + spv_ctx->tmr_tick;

        //Add new entry to the list
        if (new_entry)
        {
            zwspv_add_ent_endlist(spv_ctx, spv_ent);
        }
    }
    else
    {   //Failed to send. Callback to report error.
        zwspv_cb(&spv_ent->ifd, spv_ent->cb, spv_ent->usr_param, ZWPSET_REASON_SEND_FAILED);

        if (new_entry)
        {
            free(spv_ent);
        }
        else
        {
            zwspv_rm_ent(spv_ctx, spv_ent);
        }
    }
}


/**
zwspv_add - add a supervision set and polling request into supervision queue
@param[in]	ifd	        interface
@param[in]	set_cmd_buf	command buffer which contains set command and parameters
@param[in]	set_buf_len	length of set_cmd_buf in bytes
@param[in]	rpt_cmd_buf	command buffer which contains report command and parameters, used when target is hit
@param[in]	rpt_buf_len	length of rpt_cmd_buf in bytes
@param[in]	rpt_cmd_buf_type	The content type in rpt_cmd_buf, SUPERVSN_RPT_TYPE_XXX
@param[in]	ssn_id	    session id
@param[in]	cb		    Optioanl post-set supervision callback function.
@param[in]	usr_param   Optional user-defined parameter passed in callback.
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/
int zwspv_add(zwifd_p ifd, uint8_t *set_cmd_buf, int set_buf_len, uint8_t *rpt_cmd_buf, int rpt_buf_len, int rpt_cmd_buf_type,
              uint8_t ssn_id, zw_postset_fn cb, void *usr_param)
{
    zwspv_ctx_t   *spv_ctx = ifd->net->spv_ctx;
    spv_evt_msg_t msg;

    if ((set_buf_len > SPV_MAX_CMD_LEN) || (rpt_buf_len > SPV_MAX_CMD_LEN))
    {
        return ZW_ERR_TOO_LARGE;
    }

    if ((set_buf_len < 3) || (rpt_buf_len < 2))
    {
        return ZW_ERR_TOO_SMALL;
    }

    //Send add polling event
    msg.type = SPV_EVT_TYPE_ADD;
    //msg.ev_msg.add.ifd = *ifd;
    memcpy(&msg.ev_msg.add.ifd, ifd, sizeof(zwifd_t));

    msg.ev_msg.add.ssn_id = ssn_id & 0x3F;
    msg.ev_msg.add.set_buf_len = set_buf_len;
    msg.ev_msg.add.rpt_buf_len = rpt_buf_len;
    msg.ev_msg.add.rpt_cmd_buf_type = rpt_cmd_buf_type;
    msg.ev_msg.add.cb = cb;
    msg.ev_msg.add.usr_param = usr_param;
    memcpy(msg.ev_msg.add.set_cmd_buf, set_cmd_buf, set_buf_len);
    memcpy(msg.ev_msg.add.rpt_cmd_buf, rpt_cmd_buf, rpt_buf_len);

    if (util_msg_loop_send(spv_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(spv_evt_msg_t)) == 0)
    {
        return ZW_ERR_NONE;
    }

    return ZW_ERR_MEMORY;

}


/**
zwspv_rm - remove all post-set polling requests of a node
@param[in]	spv_ctx	Polling context
@param[in]	node_id	    Node id
@return
*/
void zwspv_rm(zwspv_ctx_t *spv_ctx, uint8_t node_id)
{
    spv_evt_msg_t msg;

    //Send event
    msg.type = SPV_EVT_TYPE_RMV;
    msg.ev_msg.rm_node_id = node_id;

    util_msg_loop_send(spv_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(spv_evt_msg_t));

}


/**
zwspv_rm_all - remove all post-set polling requests
@param[in]	spv_ctx	Polling context
@param[in]	node_id	    Node id
@return
*/
void zwspv_rm_all(zwspv_ctx_t *spv_ctx)
{
    spv_evt_msg_t msg;

    //Send event
    msg.type = SPV_EVT_TYPE_RMV_ALL;

    util_msg_loop_send(spv_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(spv_evt_msg_t));

}


/**
zwspv_evt_msg_hdlr - Process message loop on message arrival
@param[in]	usr_prm	    user parameter supplied during initialization
@param[in]	msg	        message
return      1 to exit message loop; else return 0 to continue the message loop
*/
static int zwspv_evt_msg_hdlr(void *usr_prm, void *msg)
{
    zwspv_ctx_t    *spv_ctx = (zwspv_ctx_t *)usr_prm;
    spv_evt_msg_t  *evt_msg = (spv_evt_msg_t *)msg;

    if (spv_ctx->run)
    {
        switch(evt_msg->type)
        {
            case SPV_EVT_TYPE_TIMER:
                zwspv_tmr_chk(spv_ctx);
                break;

            case SPV_EVT_TYPE_ADD:
                zwspv_add_poll(spv_ctx, &evt_msg->ev_msg.add);
                break;

            case SPV_EVT_TYPE_REPORT:
                zwspv_rpt_hdlr(spv_ctx, &evt_msg->ev_msg.rpt);
                break;

            case SPV_EVT_TYPE_RMV:
                zwspv_rm_poll(spv_ctx, evt_msg->ev_msg.rm_node_id);
                break;

            case SPV_EVT_TYPE_RMV_ALL:
                zwspv_rm_all_ent(spv_ctx, 1);
                break;

        }
    }

    return 0;
}


/**
zwspv_init - Initialize the supervision facility
@param[in]	spv_ctx	    supervision context
@return  0 on success; negative error number on failure
@note  Must call zwspv_shutdown followed by zwspv_exit to shutdown and clean up the facility
*/
int zwspv_init(zwspv_ctx_t *spv_ctx)
{
    int     res;

    spv_ctx->run = 1;
    spv_ctx->tmr_tick = 0;
    spv_ctx->spv_lst_hd = NULL;
    spv_ctx->spv_lst_tail = NULL;

    res = util_msg_loop_init(zwspv_evt_msg_hdlr, spv_ctx, NULL, 7, &spv_ctx->evt_msg_loop_ctx);

    if (res != 0)
    {
        goto l_SPV_INIT_ERROR1;
    }

    spv_ctx->tick_tmr_ctx = plt_periodic_start(&spv_ctx->net->plt_ctx, SPV_TIMER_TICK, zwspv_tmr_tick_cb, spv_ctx);
    if (!spv_ctx->tick_tmr_ctx)
        goto l_SPV_INIT_ERROR2;

    return ZW_ERR_NONE;

l_SPV_INIT_ERROR2:
    util_msg_loop_shutdown(spv_ctx->evt_msg_loop_ctx, 0);
    util_msg_loop_exit(spv_ctx->evt_msg_loop_ctx);
l_SPV_INIT_ERROR1:
    return ZW_ERR_NO_RES;

}


/**
zwspv_shutdown - Shutdown the supervision facility
@param[in]	spv_ctx	    supervision context
@return
*/
void zwspv_shutdown(zwspv_ctx_t *spv_ctx)
{
    spv_ctx->run = 0;

    //Stop timer
    plt_tmr_stop(&spv_ctx->net->plt_ctx, spv_ctx->tick_tmr_ctx);

    //Stop message loop
    util_msg_loop_shutdown(spv_ctx->evt_msg_loop_ctx, 0);
}


/**
zwspv_exit - Clean up
@param[in]	spv_ctx	    supervision context
@return
*/
void zwspv_exit(zwspv_ctx_t *spv_ctx)
{
    util_msg_loop_exit(spv_ctx->evt_msg_loop_ctx);

    //Flush the poll queue
    zwspv_rm_all_ent(spv_ctx, 0);
}


/**
@}
*/















