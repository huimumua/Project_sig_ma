/**
@file   zip_event_gen.c - Z-wave High Level API event generation facility implementation.

@author David Chow

@version    1.0 10-10-16  Initial release

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
#include "../include/zip_event_gen.h"


/**
@defgroup If_Evt_Gen Event Generation APIs
Used to generate event to interface
@{
*/

/**
zwevtg_cb - Create a callback request to the callback thread
@param[in]	ifd         interface descriptor
@param[in]	rpt         report
@param[in]	rpt_len     report length
@return
*/
static void zwevtg_cb(zwifd_p ifd, uint8_t *rpt, uint16_t rpt_len)
{
    zwnet_p         nw;
    zwnet_cb_req_t  cb_req = {0};

    nw = ifd->net;

    //Send request to callback thread
    cb_req.type = CB_TYPE_SIMULATED_REPORT;
    cb_req.cb = NULL;
    //cb_req.param.sim_rpt.ifd = *ifd;
    memcpy(&cb_req.param.sim_rpt.ifd, ifd, sizeof(zwifd_t));

    cb_req.param.sim_rpt.rpt_len = rpt_len;
    memcpy(cb_req.param.sim_rpt.rpt, rpt, rpt_len);
    util_list_add(nw->cb_mtx, &nw->cb_req_hd,
                  (uint8_t *)&cb_req, sizeof(zwnet_cb_req_t));
    plt_sem_post(nw->cb_sem);
}


/**
zwevtg_find_ent - Find an entry
@param[in]	evtg_ctx	context
@param[in]	ifd	        interface
@param[in]	rpt         report for callback when event generation time out occurs
@param[in]	rpt_len     report length
@return the entry if found; otherwise NULL.
*/
static evtg_q_ent_t *zwevtg_find_ent(zwevtg_ctx_t *evtg_ctx, zwifd_p ifd, uint8_t *rpt, uint16_t rpt_len)
{
    evtg_q_ent_t *ent;

    ent = evtg_ctx->evtg_lst_hd;

    while (ent)
    {
        if (ent->ifd.cls == ifd->cls && ent->ifd.epid == ifd->epid
            && ent->ifd.nodeid == ifd->nodeid)
        {   //Found interface
            if ((ent->rpt_len == rpt_len) && (memcmp(ent->rpt, rpt, rpt_len) == 0))
            {
                return ent;
            }
        }

        //Next
        ent = ent->next;
    }
    return NULL;
}


/**
zwevtg_find_ent_by_node - Find an entry by node id
@param[in]	evtg_ctx	context
@param[in]	node_id	    Node id
@return the entry if found; otherwise NULL.
*/
static evtg_q_ent_t *zwevtg_find_ent_by_node(zwevtg_ctx_t *evtg_ctx, uint8_t node_id)
{
    evtg_q_ent_t *ent;

    ent = evtg_ctx->evtg_lst_hd;

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
zwevtg_add_ent_endlist - Add entry to end of list
@param[in]	evtg_ctx		Context
@param[in]	ent		        New entry
@return     1 if the ent is the only entry in the list after adding; else return 0.
*/
static int zwevtg_add_ent_endlist(zwevtg_ctx_t *evtg_ctx, evtg_q_ent_t *ent)
{
    //The list is empty
    if (!evtg_ctx->evtg_lst_hd && !evtg_ctx->evtg_lst_tail)
    {
        evtg_ctx->evtg_lst_hd = evtg_ctx->evtg_lst_tail = ent;
        ent->next = ent->prev = NULL;
        return 1;
    }

    //Adjust the former tail
    evtg_ctx->evtg_lst_tail->next = ent;

    ent->prev = evtg_ctx->evtg_lst_tail;
    ent->next = NULL;

    //Assign the entry as tail
    evtg_ctx->evtg_lst_tail = ent;

    return 0;

}


/**
zwevtg_rm_ent - Remove entry from list
@param[in]	evtg_ctx		Context
@param[in]	ent		        Poll entry
@return
@pre Caller must lock evtg_mtx
@post ent should not be used as it is freed
*/
static void zwevtg_rm_ent(zwevtg_ctx_t *evtg_ctx, evtg_q_ent_t *ent)
{
    //There is only 1 entry in the list
    if (evtg_ctx->evtg_lst_hd == evtg_ctx->evtg_lst_tail)
    {
        evtg_ctx->evtg_lst_hd = evtg_ctx->evtg_lst_tail = NULL;
        free(ent);
        return;
    }

    //The entry is the head of the list
    if (evtg_ctx->evtg_lst_hd == ent)
    {
        evtg_ctx->evtg_lst_hd = ent->next;
        evtg_ctx->evtg_lst_hd->prev = NULL;
    }
    //The entry is the tail of the list
    else if (evtg_ctx->evtg_lst_tail == ent)
    {
        evtg_ctx->evtg_lst_tail = ent->prev;
        evtg_ctx->evtg_lst_tail->next = NULL;
    }
    else
    {
        ent->prev->next = ent->next;
        ent->next->prev = ent->prev;
    }

    free(ent);
}


/**
zwevtg_rm_ent_by_node - Remove poll entries of a node
@param[in]	evtg_ctx	Poll context
@param[in]	node_id	    Node id
@return
*/
static void zwevtg_rm_ent_by_node(zwevtg_ctx_t *evtg_ctx, uint8_t node_id)
{
    evtg_q_ent_t *evtg_ent;

    while ((evtg_ent = zwevtg_find_ent_by_node(evtg_ctx, node_id)) != NULL)
    {   //Found.
        zwevtg_rm_ent(evtg_ctx, evtg_ent);
    }
}


/**
zwevtg_tmr_chk - thread to process timer tick event
@param[in]	data		Context
@return
*/
static void zwevtg_tmr_chk(zwevtg_ctx_t *evtg_ctx)
{
    evtg_q_ent_t *evtg_ent;
    evtg_q_ent_t *prev_ent;      //Pointer to previous list entry

    evtg_ent = evtg_ctx->evtg_lst_hd;

    while (evtg_ent)
    {
        //Check entry for expiry
        if (util_tmr_exp_chk(evtg_ctx->tmr_tick, evtg_ent->tmout))
        {
            //Expired. Generate event
            zwevtg_cb(&evtg_ent->ifd, evtg_ent->rpt, evtg_ent->rpt_len);

            //Save previous entry since the current entry will be removed
            prev_ent = evtg_ent->prev;

            //Remove the entry
            zwevtg_rm_ent(evtg_ctx, evtg_ent);
            evtg_ent = prev_ent;
        }

        //Next
        if (!evtg_ent)
        {
            evtg_ent = evtg_ctx->evtg_lst_hd;
        }
        else
        {
            evtg_ent = evtg_ent->next;
        }
    }
}


/**
zwevtg_tmr_tick_cb - Timer tick timeout callback
@param[in] data     Pointer to poll context
@return
*/
static void    zwevtg_tmr_tick_cb(void *data)
{
    zwevtg_ctx_t   *evtg_ctx = (zwevtg_ctx_t *)data;
    evtg_evt_msg_t  msg;

    if (evtg_ctx->run)
    {
        //Increment timer tick
        evtg_ctx->tmr_tick++;

        //Send timer tick event
        msg.type = EVTG_EVT_TYPE_TIMER;

        util_msg_loop_send(evtg_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(evtg_evt_msg_t));
    }
}


/**
zwevtg_rm_all_ent - remove all event generation requests
@param[in]	evtg_ctx   Context
@return
*/
static void zwevtg_rm_all_ent(zwevtg_ctx_t *evtg_ctx)
{
    evtg_q_ent_t *ent;
    evtg_q_ent_t *rm_ent;      //Entry to be removed

    if (!evtg_ctx->evtg_lst_hd)
    {
        return;
    }

    ent = evtg_ctx->evtg_lst_hd;

    while (ent)
    {
        rm_ent = ent;
        ent = ent->next;

        free(rm_ent);
    }

    evtg_ctx->evtg_lst_hd = evtg_ctx->evtg_lst_tail = NULL;
}


/**
zwevtg_add_ent - Add an entry to end of list
@param[in]	evtg_ctx		Context
@param[in]	add_msg		    Add new entry message
@return
*/
static void zwevtg_add_ent(zwevtg_ctx_t *evtg_ctx, evtg_ev_msg_add_t  *add_msg)
{
    evtg_q_ent_t *evtg_ent;
    int           new_entry;

    //Find if there is an entry in the queue
    if ((evtg_ent = zwevtg_find_ent(evtg_ctx, &add_msg->ifd, add_msg->rpt, add_msg->rpt_len)) == NULL)
    {   //Create new entry
        evtg_ent = (evtg_q_ent_t *)calloc(1, sizeof(evtg_q_ent_t));

        if (!evtg_ent)
        {
            return;
        }

        new_entry = 1;

        //evtg_ent->ifd = add_msg->ifd;
        memcpy(&evtg_ent->ifd, &add_msg->ifd, sizeof(zwifd_t));

        evtg_ent->rpt_len = add_msg->rpt_len;
        evtg_ent->tmout = add_msg->tmout;
        memcpy(evtg_ent->rpt, add_msg->rpt, add_msg->rpt_len);
    }
    else
    {   //Found existing entry, update timeout
        new_entry = 0;
        evtg_ent->tmout = add_msg->tmout;
    }

    //Add current time to the timeout
    evtg_ent->tmout += evtg_ctx->tmr_tick;

    //Add new entry to the list
    if (new_entry)
    {
        zwevtg_add_ent_endlist(evtg_ctx, evtg_ent);
    }

}


/**
zwevtg_add - add an event generation request into event generation queue
@param[in]	ifd	    interface
@param[in]	rpt     report for callback when event generation time out occurs
@param[in]	rpt_len report length
@param[in]	tmout   time out in seconds (max: 3600 s)
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/
int zwevtg_add(zwifd_p ifd, uint8_t *rpt, uint16_t rpt_len, uint16_t tmout_s)
{
    zwevtg_ctx_t   *evtg_ctx = ifd->net->evtg_ctx;
    evtg_evt_msg_t msg;

    if ((tmout_s > 3600) || (rpt_len > EVTG_MAX_RPT_LEN))
    {
        return ZW_ERR_TOO_LARGE;
    }

    if (!rpt || (tmout_s == 0))
    {
        return ZW_ERR_VALUE;
    }

    //Send add event generation event
    msg.type = EVTG_EVT_TYPE_ADD;
    //msg.ev_msg.add.ifd = *ifd;
    memcpy(&msg.ev_msg.add.ifd, ifd, sizeof(zwifd_t));
    msg.ev_msg.add.rpt_len = rpt_len;
    msg.ev_msg.add.tmout = tmout_s * EVTG_TICK_PER_SEC;
    memcpy(msg.ev_msg.add.rpt, rpt, rpt_len);

    if (util_msg_loop_send(evtg_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(evtg_evt_msg_t)) == 0)
    {
        return ZW_ERR_NONE;
    }

    return ZW_ERR_MEMORY;

}


/**
zwevtg_rm - remove all event generation requests of a node
@param[in]	evtg_ctx	Event generation context
@param[in]	node_id	    Node id
@return
*/
void zwevtg_rm(zwevtg_ctx_t *evtg_ctx, uint8_t node_id)
{
    evtg_evt_msg_t msg;

    //Send event
    msg.type = EVTG_EVT_TYPE_RMV;
    msg.ev_msg.rm_node_id = node_id;

    util_msg_loop_send(evtg_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(evtg_evt_msg_t));

}


/**
zwevtg_rm_all - remove all event generation requests
@param[in]	evtg_ctx	Event generation context
@param[in]	node_id	    Node id
@return
*/
void zwevtg_rm_all(zwevtg_ctx_t *evtg_ctx)
{
    evtg_evt_msg_t msg;

    //Send event
    msg.type = EVTG_EVT_TYPE_RMV_ALL;

    util_msg_loop_send(evtg_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(evtg_evt_msg_t));

}


/**
zwevtg_evt_msg_hdlr - Process message loop on message arrival
@param[in]	usr_prm	    user parameter supplied during initialization
@param[in]	msg	        message
return      1 to exit message loop; else return 0 to continue the message loop
*/
static int zwevtg_evt_msg_hdlr(void *usr_prm, void *msg)
{
    zwevtg_ctx_t    *evtg_ctx = (zwevtg_ctx_t *)usr_prm;
    evtg_evt_msg_t  *evt_msg = (evtg_evt_msg_t *)msg;

    if (evtg_ctx->run)
    {
        switch (evt_msg->type)
        {
            case EVTG_EVT_TYPE_TIMER:
                zwevtg_tmr_chk(evtg_ctx);
                break;

            case EVTG_EVT_TYPE_ADD:
                zwevtg_add_ent(evtg_ctx, &evt_msg->ev_msg.add);
                break;

            case EVTG_EVT_TYPE_RMV:
                zwevtg_rm_ent_by_node(evtg_ctx, evt_msg->ev_msg.rm_node_id);
                break;

            case EVTG_EVT_TYPE_RMV_ALL:
                zwevtg_rm_all_ent(evtg_ctx);
                break;

        }
    }

    return 0;
}


/**
zwevtg_init - Initialize the event generation facility
@param[in]	evtg_ctx	    Event generation context
@return  0 on success; negative error number on failure
@note  Must call zwevtg_shutdown followed by zwevtg_exit to shutdown and clean up the event generation facility
*/
int zwevtg_init(zwevtg_ctx_t *evtg_ctx)
{
    int     res;

    evtg_ctx->run = 1;
    evtg_ctx->tmr_tick = 0;
    evtg_ctx->evtg_lst_hd = NULL;
    evtg_ctx->evtg_lst_tail = NULL;

    res = util_msg_loop_init(zwevtg_evt_msg_hdlr, evtg_ctx, NULL, 8, &evtg_ctx->evt_msg_loop_ctx);

    if (res != 0)
    {
        goto l_EVTG_INIT_ERROR1;
    }

    evtg_ctx->tick_tmr_ctx = plt_periodic_start(&evtg_ctx->net->plt_ctx, EVTG_TIMER_TICK, zwevtg_tmr_tick_cb, evtg_ctx);
    if (!evtg_ctx->tick_tmr_ctx)
        goto l_EVTG_INIT_ERROR2;

    return ZW_ERR_NONE;

l_EVTG_INIT_ERROR2:
    util_msg_loop_shutdown(evtg_ctx->evt_msg_loop_ctx, 0);
    util_msg_loop_exit(evtg_ctx->evt_msg_loop_ctx);
l_EVTG_INIT_ERROR1:
    return ZW_ERR_NO_RES;

}


/**
zwevtg_shutdown - Shutdown the event generation facility
@param[in]	evtg_ctx	    Event generation context
@return
*/
void zwevtg_shutdown(zwevtg_ctx_t *evtg_ctx)
{
    evtg_ctx->run = 0;

    //Stop timer
    plt_tmr_stop(&evtg_ctx->net->plt_ctx, evtg_ctx->tick_tmr_ctx);

    //Stop message loop
    util_msg_loop_shutdown(evtg_ctx->evt_msg_loop_ctx, 0);
}


/**
zwevtg_exit - Clean up
@param[in]	evtg_ctx	    Event generation context
@return
*/
void zwevtg_exit(zwevtg_ctx_t *evtg_ctx)
{
    util_msg_loop_exit(evtg_ctx->evt_msg_loop_ctx);

    //Flush the poll queue
    zwevtg_rm_all_ent(evtg_ctx);
}


/**
@}
*/















