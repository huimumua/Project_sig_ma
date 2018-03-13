/**
@file   zip_set_poll.c - Z-wave High Level API post-set poll facility implementation.

@author David Chow

@version    1.0 9-9-15  Initial release

@copyright © 2015 SIGMA DESIGNS, INC. THIS IS AN UNPUBLISHED WORK PROTECTED BY SIGMA DESIGNS, INC.
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
#include "../include/zip_set_poll.h"


/**
@defgroup If_SPoll Post-set Polling Interface APIs
Used to create and delete post-set polling commands to a device
@{
*/

/**
zwspoll_cb - Create a callback request to the callback thread
@param[in]	ifd         interface descriptor
@param[in]	cb		    post-set polling callback function.  NULL if no callback required.
@param[in]	usr_param   user-defined parameter passed in callback.
@param[in]  reason      reason of the callback, see ZWPSET_REASON_XXX
@return
*/
void zwspoll_cb(zwifd_p ifd, zw_postset_fn cb, void *usr_param, int reason)
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

    util_list_add(nw->cb_mtx, &nw->cb_req_hd,
                  (uint8_t *)&cb_req, sizeof(zwnet_cb_req_t));
    plt_sem_post(nw->cb_sem);
}


/**
zwspoll_find_ent - Find a poll entry
@param[in]	spoll_ctx	context
@param[in]	ifd	        interface
@return poll entry if found; otherwise NULL.
*/
static spoll_q_ent_t *zwspoll_find_ent(zwspoll_ctx_t *spoll_ctx, zwifd_p ifd)
{
    spoll_q_ent_t *ent;

    ent = spoll_ctx->spoll_lst_hd;

    while (ent)
    {
        if (ent->ifd.cls == ifd->cls && ent->ifd.epid == ifd->epid
            && ent->ifd.nodeid == ifd->nodeid)
        {   //Found
            return ent;
        }

        //Next
        ent = ent->next;
    }
    return NULL;
}


/**
zwspoll_find_ent_by_node - Find a poll entry by node id
@param[in]	spoll_ctx	context
@param[in]	node_id	    Node id
@return poll entry if found; otherwise NULL.
*/
static spoll_q_ent_t *zwspoll_find_ent_by_node(zwspoll_ctx_t *spoll_ctx, uint8_t node_id)
{
    spoll_q_ent_t *ent;

    ent = spoll_ctx->spoll_lst_hd;

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
zwspoll_add_ent_endlist - Add entry to end of list
@param[in]	spoll_ctx		Context
@param[in]	ent		        New poll entry
@return     1 if the ent is the only entry in the list after adding; else return 0.
*/
static int zwspoll_add_ent_endlist(zwspoll_ctx_t *spoll_ctx, spoll_q_ent_t *ent)
{
    //The list is empty
    if (!spoll_ctx->spoll_lst_hd && !spoll_ctx->spoll_lst_tail)
    {
        spoll_ctx->spoll_lst_hd = spoll_ctx->spoll_lst_tail = ent;
        ent->next = ent->prev = NULL;
        return 1;
    }

    //Adjust the former tail
    spoll_ctx->spoll_lst_tail->next = ent;

    ent->prev = spoll_ctx->spoll_lst_tail;
    ent->next = NULL;

    //Assign the entry as tail
    spoll_ctx->spoll_lst_tail = ent;

    return 0;

}


/**
zwspoll_rm_ent - Remove entry from list
@param[in]	spoll_ctx		Context
@param[in]	ent		        Poll entry
@return
@pre Caller must lock spoll_mtx
@post ent should not be used as it is freed
*/
static void zwspoll_rm_ent(zwspoll_ctx_t *spoll_ctx, spoll_q_ent_t *ent)
{
    //There is only 1 entry in the list
    if (spoll_ctx->spoll_lst_hd == spoll_ctx->spoll_lst_tail)
    {
        spoll_ctx->spoll_lst_hd = spoll_ctx->spoll_lst_tail = NULL;
        free(ent);
        return;
    }

    //The entry is the head of the list
    if (spoll_ctx->spoll_lst_hd == ent)
    {
        spoll_ctx->spoll_lst_hd = ent->next;
        spoll_ctx->spoll_lst_hd->prev = NULL;
    }
    //The entry is the tail of the list
    else if (spoll_ctx->spoll_lst_tail == ent)
    {
        spoll_ctx->spoll_lst_tail = ent->prev;
        spoll_ctx->spoll_lst_tail->next = NULL;
    }
    else
    {
        ent->prev->next = ent->next;
        ent->next->prev = ent->prev;
    }

    free(ent);
}


/**
zwspoll_rpt_rcv_chk - Callback when a report is received
@param[in]	spoll_ctx	Poll context
@param[in]	msg	        Message
@return
*/
static void zwspoll_rpt_rcv_chk(zwspoll_ctx_t *spoll_ctx, spoll_ev_msg_rpt_t *rpt_msg)
{
    spoll_q_ent_t *spoll_ent;
    int           target_hit = 0;
    int           cb_reason = -1;

    if ((spoll_ent = zwspoll_find_ent(spoll_ctx, &rpt_msg->ifd)) == NULL)
    {   //Not found, the report is unexpected
        return;
    }

    if (rpt_msg->value == spoll_ent->target)
    {
        target_hit = 1;
    }
    else if (rpt_msg->ifd.cls == COMMAND_CLASS_SWITCH_MULTILEVEL)
    {
        if ((spoll_ent->target == 0xFF) && (rpt_msg->value > 0))
        {
            target_hit = 1;
        }
    }
    else if (rpt_msg->ifd.cls == COMMAND_CLASS_DOOR_LOCK)
    {
        if ((rpt_msg->value != 0xFE) && (rpt_msg->value != 0xFF))
        {
            if ((spoll_ent->target != 0xFF) && (rpt_msg->value == 0))
            {   //Other target value means door unsecured is sufficient to meet the desired target
                target_hit = 1;
            }
        }
    }

    if (target_hit)
    {
        cb_reason = ZWPSET_REASON_TGT_HIT;
    }
    else if (rpt_msg->dur_valid)
    {   //Device supports estimated duration
        if ((rpt_msg->dur == 0) && (spoll_ent->max_spoll_cnt == SPOLL_INVALID_MAX_COUNT))
        {   //Fail to hit the user desired target
            cb_reason = ZWPSET_REASON_FAILED;
        }
        else
        {
            if (spoll_ent->last_rpt_dur)
            {
                if (spoll_ent->last_rpt_dur > rpt_msg->dur)
                {   //Estimated duration has been reduced, readjust expiry time
                    spoll_ent->next_spoll_tm = (rpt_msg->dur + SPOLL_TIMEOUT_CHECK) * SPOLL_TICK_PER_SEC + spoll_ctx->tmr_tick;
                    spoll_ent->final_tmout = 0;

                }
            }
            else
            {   //First report of estimated duration. Readjust expiry time
                spoll_ent->next_spoll_tm = (rpt_msg->dur + SPOLL_TIMEOUT_CHECK) * SPOLL_TICK_PER_SEC + spoll_ctx->tmr_tick;
                spoll_ent->final_tmout = 0;
            }

            spoll_ent->last_rpt_dur = rpt_msg->dur;
            spoll_ent->max_spoll_cnt = SPOLL_INVALID_MAX_COUNT;
        }
    }
    else
    {   //Check for timeout
        if ((spoll_ent->max_spoll_cnt != SPOLL_INVALID_MAX_COUNT)
            && (spoll_ent->cur_spoll_cnt > spoll_ent->max_spoll_cnt))
        {
            cb_reason = ZWPSET_REASON_TIMEOUT;
        }
    }

    if (cb_reason >= 0)
    {
        if (spoll_ent->cb)
        {
            zwspoll_cb(&spoll_ent->ifd, spoll_ent->cb, spoll_ent->usr_param, cb_reason);
        }
        zwspoll_rm_ent(spoll_ctx, spoll_ent);
    }
}


/**
zwspoll_rm_poll - Remove poll entries of a node
@param[in]	spoll_ctx	Poll context
@param[in]	node_id	    Node id
@return
*/
static void zwspoll_rm_poll(zwspoll_ctx_t *spoll_ctx, uint8_t node_id)
{
    spoll_q_ent_t *spoll_ent;

    while ((spoll_ent = zwspoll_find_ent_by_node(spoll_ctx, node_id)) != NULL)
    {   //Found.
        if (spoll_ent->cb)
        {
            zwspoll_cb(&spoll_ent->ifd, spoll_ent->cb, spoll_ent->usr_param, ZWPSET_REASON_DEVICE_RMV);
        }
        zwspoll_rm_ent(spoll_ctx, spoll_ent);
    }
}


/**
zwspoll_tmr_chk - thread to process timer tick event
@param[in]	data		Context
@return
*/
static void zwspoll_tmr_chk(zwspoll_ctx_t *spoll_ctx)
{
    spoll_q_ent_t *spoll_ent;
    spoll_q_ent_t *prev_ent;      //Pointer to previous list entry

    spoll_ent = spoll_ctx->spoll_lst_hd;

    while (spoll_ent)
    {
        //Check entry for expiry
        if (util_tmr_exp_chk(spoll_ctx->tmr_tick, spoll_ent->next_spoll_tm))
        {
            //Expired
            if (spoll_ent->max_spoll_cnt == SPOLL_INVALID_MAX_COUNT)
            {   //Device supports estimated duration
                if (!spoll_ent->final_tmout)
                {    //Send the report get
                    zwif_exec_ex(&spoll_ent->ifd, spoll_ent->rpt_get_buf, spoll_ent->rpt_get_len,
                                 NULL, NULL, ZWIF_OPT_POLL, NULL);

                    spoll_ent->final_tmout = 1;
                    //The last report_get has just been sent, give the device some time to respond
                    spoll_ent->next_spoll_tm = SPOLL_WAIT_REPORT_TIME + spoll_ctx->tmr_tick;

                }
                else
                {   //Timeout
                    if (spoll_ent->cb)
                    {
                        zwspoll_cb(&spoll_ent->ifd, spoll_ent->cb, spoll_ent->usr_param, ZWPSET_REASON_TIMEOUT);
                    }
                    //Save previous entry since the current entry will be removed
                    prev_ent = spoll_ent->prev;
                    //Remove the entry
                    zwspoll_rm_ent(spoll_ctx, spoll_ent);
                    spoll_ent = prev_ent;
                }
            }
            else
            {
                //Send the report get
                if (spoll_ent->cur_spoll_cnt <= spoll_ent->max_spoll_cnt)
                {
                    zwif_exec_ex(&spoll_ent->ifd, spoll_ent->rpt_get_buf, spoll_ent->rpt_get_len,
                                 NULL, NULL, ZWIF_OPT_POLL, NULL);
                }

                spoll_ent->cur_spoll_cnt++;

                if (spoll_ent->cur_spoll_cnt > (spoll_ent->max_spoll_cnt + 1))
                {   //Timeout
                    if (spoll_ent->cb)
                    {
                        zwspoll_cb(&spoll_ent->ifd, spoll_ent->cb, spoll_ent->usr_param, ZWPSET_REASON_TIMEOUT);
                    }
                    //Save previous entry since the current entry will be removed
                    prev_ent = spoll_ent->prev;
                    //Remove the entry
                    zwspoll_rm_ent(spoll_ctx, spoll_ent);
                    spoll_ent = prev_ent;

                }
                else if (spoll_ent->cur_spoll_cnt > spoll_ent->max_spoll_cnt)
                {   //The last report_get has just been sent, give the device some time to respond
                    spoll_ent->next_spoll_tm = SPOLL_WAIT_REPORT_TIME + spoll_ctx->tmr_tick;
                }
                else
                {   //Calculate the next expiry time
                    spoll_ent->next_spoll_tm = spoll_ent->cur_spoll_cnt * SPOLL_TICK_PER_SEC + spoll_ctx->tmr_tick;
                }
            }
        }

        //Next
        if (!spoll_ent)
        {
            spoll_ent = spoll_ctx->spoll_lst_hd;
        }
        else
        {
            spoll_ent = spoll_ent->next;
        }
    }
}


/**
zwspoll_tmr_tick_cb - Timer tick timeout callback
@param[in] data     Pointer to poll context
@return
*/
static void    zwspoll_tmr_tick_cb(void *data)
{
    zwspoll_ctx_t   *spoll_ctx = (zwspoll_ctx_t *)data;
    spoll_evt_msg_t  msg;

    if (spoll_ctx->run)
    {
        //Increment timer tick
        spoll_ctx->tmr_tick++;

        //Send timer tick event
        msg.type = SPOLL_EVT_TYPE_TIMER;

        util_msg_loop_send(spoll_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(spoll_evt_msg_t));
    }
}


/**
zwspoll_rm_all_ent - remove all polling requests
@param[in]	spoll_ctx   Context
@param[in]	cb_ena      Flag to indicate whether to callback to user application
@return
*/
static void zwspoll_rm_all_ent(zwspoll_ctx_t *spoll_ctx, int cb_ena)
{
    spoll_q_ent_t *ent;
    spoll_q_ent_t *rm_ent;      //Entry to be removed

    if (!spoll_ctx->spoll_lst_hd)
    {
        return;
    }

    ent = spoll_ctx->spoll_lst_hd;

    while (ent)
    {
        rm_ent = ent;
        ent = ent->next;

        if (cb_ena && rm_ent->cb)
        {
            zwspoll_cb(&rm_ent->ifd, rm_ent->cb, rm_ent->usr_param, ZWPSET_REASON_DEVICE_RMV);
        }

        free(rm_ent);
    }

    spoll_ctx->spoll_lst_hd = spoll_ctx->spoll_lst_tail = NULL;
}


/**
zwspoll_rpt_rcv - inform the polling system of report arrival
@param[in]	ifd	        interface
@param[in]	value	    report value
@param[in]	dur	        expected time to reach the target. 0 = already at the target; 0x01~0x7F = seconds; 0x80~0xFD = 1~126 minutes;
                        0xFE = Unknown duration; 0xFF = reserved
@param[in]	dur_valid	flag to indicate whether the "dur" is valid
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/
static int zwspoll_rpt_rcv(zwifd_p ifd, uint8_t value, uint16_t dur, uint8_t dur_valid)
{
    zwspoll_ctx_t   *spoll_ctx = ifd->net->spoll_ctx;
    spoll_evt_msg_t msg;

    //Send report arrival event
    msg.type = SPOLL_EVT_TYPE_REPORT;
    //msg.ev_msg.rpt.ifd = *ifd;
    memcpy(&msg.ev_msg.rpt.ifd, ifd, sizeof(zwifd_t));

    msg.ev_msg.rpt.value = value;
    msg.ev_msg.rpt.dur_valid = dur_valid;

    if (dur_valid)
    {
        if (dur < 0x80)
        {
            msg.ev_msg.rpt.dur = dur;
        }
        else if (dur < 0xFE)
        {
            msg.ev_msg.rpt.dur = (dur - 0x7F) * 60;
        }
        else
        {   //Invalid or unknown
            msg.ev_msg.rpt.dur_valid = 0;
        }
    }

    if (util_msg_loop_send(spoll_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(spoll_evt_msg_t)) == 0)
    {
        return ZW_ERR_NONE;
    }

    return ZW_ERR_MEMORY;
}


/**
zwspoll_on_rpt_rcv - Callback when a report is received
@param[in]	intf	Interface
@param[in]	cmd_buf	The report command received
@param[in]	cmd_len	The length of cmd_buf
@return
*/
void zwspoll_on_rpt_rcv(zwif_p intf, uint8_t *cmd_buf, uint16_t cmd_len)
{
    uint16_t    cmd_cls;
    uint8_t     rpt_cmd;
    uint8_t     dur_valid = 0;
    uint16_t    dur = 0;
    zwifd_t     ifd;

    //Get command class and the report command
    if (!zwif_cls_cmd_get(cmd_buf, cmd_len, &cmd_cls, &rpt_cmd))
    {
        return;
    }

    switch (cmd_cls)
    {
        case COMMAND_CLASS_SWITCH_MULTILEVEL:
            if ((rpt_cmd == SWITCH_MULTILEVEL_REPORT) && (cmd_len >= 3))
            {
                zwif_get_desc(intf, &ifd);
                if (cmd_len >= 5)
                {   //version 4
                    dur_valid = 1;
                    dur = cmd_buf[4];
                }
                zwspoll_rpt_rcv(&ifd, cmd_buf[2], dur, dur_valid);
            }
            break;

        case COMMAND_CLASS_DOOR_LOCK:
            if ((rpt_cmd == DOOR_LOCK_OPERATION_REPORT) && (cmd_len >= 7))
            {
                zwif_get_desc(intf, &ifd);
                if (cmd_len >= 9)
                {   //version 3
                    dur_valid = 1;
                    dur = cmd_buf[8];
                }
                zwspoll_rpt_rcv(&ifd, cmd_buf[2], dur, dur_valid);
            }
            break;

        case COMMAND_CLASS_BARRIER_OPERATOR:
            if ((rpt_cmd == BARRIER_OPERATOR_REPORT) && (cmd_len >= 3))
            {
                zwif_get_desc(intf, &ifd);
                zwspoll_rpt_rcv(&ifd, cmd_buf[2], dur, dur_valid);
            }
            break;

        case COMMAND_CLASS_SWITCH_BINARY:
            if ((rpt_cmd == SWITCH_BINARY_REPORT) && (cmd_len >= 3))
            {
                zwif_get_desc(intf, &ifd);
                if (cmd_len >= 5)
                {   //version 2
                    dur_valid = 1;
                    dur = cmd_buf[4];
                }
                zwspoll_rpt_rcv(&ifd, cmd_buf[2], dur, dur_valid);
            }
            break;
    }
}


/**
zwspoll_add_poll - Add poll entry to end of list and get the first report
@param[in]	spoll_ctx		Context
@param[in]	add_msg		    Add new poll entry message
@return
*/
static void zwspoll_add_poll(zwspoll_ctx_t *spoll_ctx, spoll_ev_msg_add_t  *add_msg)
{
    spoll_q_ent_t *spoll_ent;
    int           new_entry;

    //Find if there is an entry in the queue
    if ((spoll_ent = zwspoll_find_ent(spoll_ctx, &add_msg->ifd)) == NULL)
    {   //Create new entry
        size_t ent_size = sizeof(spoll_q_ent_t) + add_msg->rpt_get_len;
        spoll_ent = (spoll_q_ent_t *)calloc(1, ent_size);

        if (!spoll_ent)
        {
            return;
        }

        new_entry = 1;

        //spoll_ent->ifd = add_msg->ifd;
        memcpy(&spoll_ent->ifd, &add_msg->ifd, sizeof(zwifd_t));

        spoll_ent->rpt_get_len = add_msg->rpt_get_len;
        memcpy(spoll_ent->rpt_get_buf, add_msg->rpt_get_buf, add_msg->rpt_get_len);
    }
    else
    {
        new_entry = 0;
    }

    spoll_ent->target = add_msg->target;
    spoll_ent->cb = add_msg->cb;
    spoll_ent->usr_param = add_msg->usr_param;

    if (add_msg->ifd.cls == COMMAND_CLASS_SWITCH_MULTILEVEL)
    {
        spoll_ent->max_spoll_cnt = SPOLL_MOTOR_MAX_COUNT;
    }
    else if (add_msg->ifd.cls == COMMAND_CLASS_BARRIER_OPERATOR)
    {
        spoll_ent->max_spoll_cnt = SPOLL_BARRIER_OPRT_MAX_COUNT;
    }
    else
    {
        spoll_ent->max_spoll_cnt = SPOLL_DOORLOCK_MAX_COUNT;
    }

    spoll_ent->last_rpt_dur = spoll_ent->final_tmout = 0;

    spoll_ent->cur_spoll_cnt = 1;

    spoll_ent->next_spoll_tm = SPOLL_TICK_PER_SEC + spoll_ctx->tmr_tick;

    //Add new entry to the list
    if (new_entry)
    {
        zwspoll_add_ent_endlist(spoll_ctx, spoll_ent);
    }

}


/**
zwspoll_add - add a post-set polling request into polling queue
@param[in]	ifd	        interface
@param[in]	target	    expected report target value
@param[in]	rpt_get_buf	buffer to store the command for report get and its parameters
@param[in]	rpt_get_len	length of the rpt_get_buf field
@param[in]	cb		    Optional post-set polling callback function.  NULL if no callback required.
@param[in]	usr_param   Optional user-defined parameter passed in callback.
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/
int zwspoll_add(zwifd_p ifd, uint8_t target, uint8_t *rpt_get_buf, uint8_t rpt_get_len, zw_postset_fn cb, void *usr_param)
{
    zwspoll_ctx_t   *spoll_ctx = ifd->net->spoll_ctx;
    spoll_evt_msg_t msg;

    if (rpt_get_len > 8)
    {
        return ZW_ERR_TOO_LARGE;
    }

    if (rpt_get_len < 2)
    {
        return ZW_ERR_TOO_SMALL;
    }

    //Send add polling event
    msg.type = SPOLL_EVT_TYPE_ADD;
    //msg.ev_msg.add.ifd = *ifd;
    memcpy(&msg.ev_msg.add.ifd, ifd, sizeof(zwifd_t));

    msg.ev_msg.add.target = target;
    msg.ev_msg.add.rpt_get_len = rpt_get_len;
    msg.ev_msg.add.cb = cb;
    msg.ev_msg.add.usr_param = usr_param;
    memcpy(msg.ev_msg.add.rpt_get_buf, rpt_get_buf, rpt_get_len);

    if (util_msg_loop_send(spoll_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(spoll_evt_msg_t)) == 0)
    {
        return ZW_ERR_NONE;
    }

    return ZW_ERR_MEMORY;

}


/**
zwspoll_rm - remove all post-set polling requests of a node
@param[in]	spoll_ctx	Polling context
@param[in]	node_id	    Node id
@return
*/
void zwspoll_rm(zwspoll_ctx_t *spoll_ctx, uint8_t node_id)
{
    spoll_evt_msg_t msg;

    //Send event
    msg.type = SPOLL_EVT_TYPE_RMV;
    msg.ev_msg.rm_node_id = node_id;

    util_msg_loop_send(spoll_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(spoll_evt_msg_t));

}


/**
zwspoll_rm_all - remove all post-set polling requests
@param[in]	spoll_ctx	Polling context
@param[in]	node_id	    Node id
@return
*/
void zwspoll_rm_all(zwspoll_ctx_t *spoll_ctx)
{
    spoll_evt_msg_t msg;

    //Send event
    msg.type = SPOLL_EVT_TYPE_RMV_ALL;

    util_msg_loop_send(spoll_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(spoll_evt_msg_t));

}


/**
zwspoll_evt_msg_hdlr - Process message loop on message arrival
@param[in]	usr_prm	    user parameter supplied during initialization
@param[in]	msg	        message
return      1 to exit message loop; else return 0 to continue the message loop
*/
static int zwspoll_evt_msg_hdlr(void *usr_prm, void *msg)
{
    zwspoll_ctx_t    *spoll_ctx = (zwspoll_ctx_t *)usr_prm;
    spoll_evt_msg_t  *evt_msg = (spoll_evt_msg_t *)msg;

    if (spoll_ctx->run)
    {
        switch(evt_msg->type)
        {
            case SPOLL_EVT_TYPE_TIMER:
                zwspoll_tmr_chk(spoll_ctx);
                break;

            case SPOLL_EVT_TYPE_ADD:
                zwspoll_add_poll(spoll_ctx, &evt_msg->ev_msg.add);
                break;

            case SPOLL_EVT_TYPE_REPORT:
                zwspoll_rpt_rcv_chk(spoll_ctx, &evt_msg->ev_msg.rpt);
                break;

            case SPOLL_EVT_TYPE_RMV:
                zwspoll_rm_poll(spoll_ctx, evt_msg->ev_msg.rm_node_id);
                break;

            case SPOLL_EVT_TYPE_RMV_ALL:
                zwspoll_rm_all_ent(spoll_ctx, 1);
                break;

        }
    }

    return 0;
}


/**
zwspoll_init - Initialize the polling facility
@param[in]	spoll_ctx	    Polling context
@return  0 on success; negative error number on failure
@note  Must call zwspoll_shutdown followed by zwspoll_exit to shutdown and clean up the polling facility
*/
int zwspoll_init(zwspoll_ctx_t *spoll_ctx)
{
    int     res;

    spoll_ctx->run = 1;
    spoll_ctx->tmr_tick = 0;
    spoll_ctx->spoll_lst_hd = NULL;
    spoll_ctx->spoll_lst_tail = NULL;

    res = util_msg_loop_init(zwspoll_evt_msg_hdlr, spoll_ctx, NULL, 5, &spoll_ctx->evt_msg_loop_ctx);

    if (res != 0)
    {
        goto l_SPOLL_INIT_ERROR1;
    }

    spoll_ctx->tick_tmr_ctx = plt_periodic_start(&spoll_ctx->net->plt_ctx, SPOLL_TIMER_TICK, zwspoll_tmr_tick_cb, spoll_ctx);
    if (!spoll_ctx->tick_tmr_ctx)
        goto l_SPOLL_INIT_ERROR2;

    return ZW_ERR_NONE;

l_SPOLL_INIT_ERROR2:
    util_msg_loop_shutdown(spoll_ctx->evt_msg_loop_ctx, 0);
    util_msg_loop_exit(spoll_ctx->evt_msg_loop_ctx);
l_SPOLL_INIT_ERROR1:
    return ZW_ERR_NO_RES;

}


/**
zwspoll_shutdown - Shutdown the polling facility
@param[in]	spoll_ctx	    Polling context
@return
*/
void zwspoll_shutdown(zwspoll_ctx_t *spoll_ctx)
{
    spoll_ctx->run = 0;

    //Stop timer
    plt_tmr_stop(&spoll_ctx->net->plt_ctx, spoll_ctx->tick_tmr_ctx);

    //Stop message loop
    util_msg_loop_shutdown(spoll_ctx->evt_msg_loop_ctx, 0);
}


/**
zwspoll_exit - Clean up
@param[in]	spoll_ctx	    Polling context
@return
*/
void zwspoll_exit(zwspoll_ctx_t *spoll_ctx)
{
    util_msg_loop_exit(spoll_ctx->evt_msg_loop_ctx);

    //Flush the poll queue
    zwspoll_rm_all_ent(spoll_ctx, 0);
}


/**
@}
*/















