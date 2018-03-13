/**
@file   zip_wkup_poll.c - Z-wave High Level API wake-up notification poll implementation.

@author David Chow

@version    1.0 4-1-16  Initial release

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
#include "../include/zip_wkup_poll.h"

#if 0
#define     debug_wkup_ts_msg      plt_msg_ts_show     ///< Print debug message with time
static const char *wkuppoll_state[] = { "Polling is inactive",
                                        "Node is down",
                                        "Waiting NOP transmit status",
                                        "Polling is active"
                                      };
#else
#define     debug_wkup_ts_msg(...)
#endif


/**
@defgroup If_WkupPoll  Wake-up Notification Polling Interface APIs
Used to create and delete wake-up notification polling commands to a device
@{
*/


/**
wkuppoll_old_sts_cb - Old send status callback function
@param[in]	appl_ctx    The application layer context
@param[in]	tx_sts		The transmit complete status
@param[in]	user_prm    The user specific parameter
@param[in]	node_id     Node id
@param[in]	param       Additional parameters
@return
*/
static void  wkuppoll_old_sts_cb(appl_layer_ctx_t *appl_ctx, int8_t tx_sts, void *user_prm, uint8_t node_id, zw_tx_cb_prm_t *param)
{
    wupoll_ctx_t        *wupoll_ctx = (wupoll_ctx_t *)user_prm;
    wupoll_evt_msg_t    msg;

    if (tx_sts == TRANSMIT_COMPLETE_OK)
    {   //Mailbox message delivered means sleeping node has awaken
        debug_wkup_ts_msg(appl_ctx->plt_ctx, "Wakeup notification: node id = %u", node_id);
        msg.ev_msg.status.tx_sts = WUPOLL_WAKEUP_NOTIFICATION;
    }
    else
    {
        debug_wkup_ts_msg(appl_ctx->plt_ctx, "Wakeup poll old tx status:%d, node id = %u", tx_sts, node_id);
        msg.ev_msg.status.tx_sts = tx_sts;
    }

    msg.type = WUPOLL_EVT_TYPE_STATUS;
    msg.ev_msg.status.node_id = node_id;

    util_msg_loop_send(wupoll_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(wupoll_evt_msg_t));
}


/**
wkuppoll_probe_cb - send NOP callback function
@param[in]	appl_ctx    The application layer context
@param[in]	tx_sts		The transmit complete status
@param[in]	user_prm    The user specific parameter
@param[in]	node_id     Node id
@param[in]	param       Additional parameters
@return
*/
static void  wkuppoll_probe_cb(appl_layer_ctx_t *appl_ctx, int8_t tx_sts, void *user_prm, uint8_t node_id, zw_tx_cb_prm_t *param)
{
    wupoll_ctx_t        *wupoll_ctx = (wupoll_ctx_t *)user_prm;
    wupoll_evt_msg_t    msg;

    if (tx_sts != TRANSMIT_COMPLETE_OK)
    {
        debug_wkup_ts_msg(appl_ctx->plt_ctx, "Wakeup poll node:%u, NOP tx status:%d", node_id, tx_sts);
    }

    //Send NOP poll status event
    msg.type = WUPOLL_EVT_TYPE_STATUS;
    msg.ev_msg.status.node_id = node_id;
    msg.ev_msg.status.tx_sts = tx_sts;

    util_msg_loop_send(wupoll_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(wupoll_evt_msg_t));
}


/**
wkuppoll_probe_updt_state - send a "no operation" command to a node to test if it's reachable and update the state
@param[in]	wupoll_ctx	context
@param[in]	wupoll_ent  wake up poll entry
@return 0 if probe was sent successfully; else return negative number
*/
static int wkuppoll_probe_updt_state(wupoll_ctx_t *wupoll_ctx,  wupoll_q_ent_t *wupoll_ent)
{
    int                 result;
    appl_snd_data_t     prm = {0};
    uint8_t             cmd = COMMAND_CLASS_NO_OPERATION;
    zwnet_p             nw = wupoll_ctx->net;

    prm.dat_len = 1;
    prm.dat_buf = &cmd;
    prm.node_id = wupoll_ent->node_id;
    prm.wkup_poll = 1;
    prm.flag = ZIP_FLAG_MORE_INFO;

    if (nw->use_encap_hdr)
    {   //Deprecate "secure origin" bit in Z/IP header. Alwasy set "secure origin" bit to 1
        prm.flag |= (ZIP_FLAG_SECURE | ZIP_FLAG_ENCAP_INFO);
    }

    result = zw_send_data(&nw->appl_ctx, &prm, wkuppoll_probe_cb, wupoll_ctx);

//  debug_wkup_ts_msg(&nw->plt_ctx, "wkuppoll_probe_updt_state: node id=%d, state=%u, res=%d",
//                     wupoll_ent->node_id, wupoll_ent->state, result);


    if (result < 0)
    {
        if ((result == SESSION_ERROR_PREVIOUS_COMMAND_UNCOMPLETED) || (result == APPL_ERROR_WAIT_CB))
        {   //Busy, need retrying
            wupoll_ent->state = WUPOLL_STA_INACTIVE;
            wupoll_ent->state_tmout = wupoll_ctx->tmr_tick + WUPOLL_CHK_INACTIVE_INTV;
        }
        else
        {   //Failed
            wupoll_ent->state = WUPOLL_STA_NODE_DOWN;
            wupoll_ent->state_tmout = wupoll_ctx->tmr_tick + WUPOLL_CHK_DOWN_INTV;
        }
    }
    else
    {
        wupoll_ent->state = WUPOLL_STA_WAIT_TX_STS;
        wupoll_ent->state_tmout = wupoll_ctx->tmr_tick + WUPOLL_TX_STATUS_TMOUT;
    }

    debug_wkup_ts_msg(&nw->plt_ctx, "wkuppoll_probe_updt_state: node id=%d, changed state=%u '%s', res=%d",
                       wupoll_ent->node_id, wupoll_ent->state, wkuppoll_state[wupoll_ent->state], result);

    return result;
}


/**
wkuppoll_find_ent - Find a poll entry
@param[in]	wupoll_ctx	context
@param[in]	node_id     node id
@return poll entry if found; otherwise NULL.
*/
static wupoll_q_ent_t *wkuppoll_find_ent(wupoll_ctx_t *wupoll_ctx, uint8_t node_id)
{
    wupoll_q_ent_t *ent;

    ent = wupoll_ctx->wupoll_lst_hd;

    while (ent)
    {
        if (ent->node_id == node_id)
        {   //Found
            return ent;
        }

        //Next
        ent = ent->next;
    }
    return NULL;
}


/**
wkuppoll_add_ent_endlist - Add entry to end of list
@param[in]	wupoll_ctx		Context
@param[in]	ent		        New poll entry
@return     1 if the ent is the only entry in the list after adding; else return 0.
*/
static int wkuppoll_add_ent_endlist(wupoll_ctx_t *wupoll_ctx, wupoll_q_ent_t *ent)
{
    //The list is empty
    if (!wupoll_ctx->wupoll_lst_hd && !wupoll_ctx->wupoll_lst_tail)
    {
        wupoll_ctx->wupoll_lst_hd = wupoll_ctx->wupoll_lst_tail = ent;
        ent->next = ent->prev = NULL;
        return 1;
    }

    //Adjust the former tail
    wupoll_ctx->wupoll_lst_tail->next = ent;

    ent->prev = wupoll_ctx->wupoll_lst_tail;
    ent->next = NULL;

    //Assign the entry as tail
    wupoll_ctx->wupoll_lst_tail = ent;

    return 0;

}


/**
wkuppoll_rm_ent - Remove entry from list
@param[in]	wupoll_ctx		Context
@param[in]	ent		        Poll entry
@return
@pre Caller must lock wupoll_mtx
@post ent should not be used as it is freed
*/
static void wkuppoll_rm_ent(wupoll_ctx_t *wupoll_ctx, wupoll_q_ent_t *ent)
{
    //There is only 1 entry in the list
    if (wupoll_ctx->wupoll_lst_hd == wupoll_ctx->wupoll_lst_tail)
    {
        wupoll_ctx->wupoll_lst_hd = wupoll_ctx->wupoll_lst_tail = NULL;
        free(ent);
        return;
    }

    //The entry is the head of the list
    if (wupoll_ctx->wupoll_lst_hd == ent)
    {
        wupoll_ctx->wupoll_lst_hd = ent->next;
        wupoll_ctx->wupoll_lst_hd->prev = NULL;
    }
    //The entry is the tail of the list
    else if (wupoll_ctx->wupoll_lst_tail == ent)
    {
        wupoll_ctx->wupoll_lst_tail = ent->prev;
        wupoll_ctx->wupoll_lst_tail->next = NULL;
    }
    else
    {
        ent->prev->next = ent->next;
        ent->next->prev = ent->prev;
    }

    free(ent);
}


/**
wkuppoll_set_timeout - Set state timeout based on wakeup interval value
@param[in]	wupoll_ctx	Poll context
@param[in]	wupoll_ent	Poll entry
@return
*/
static void wkuppoll_set_timeout(wupoll_ctx_t *wupoll_ctx, wupoll_q_ent_t *wupoll_ent)
{
    if (wupoll_ent->wkup_intv > 0)
    {
        wupoll_ent->state_tmout = WUPOLL_CHK_DOWN_INTV;
    }
    else if (wupoll_ent->wkup_intv == 0)
    {
        wupoll_ent->state_tmout = WUPOLL_CHK_DOWN_0_INTERVAL;
    }
    else
    {
        zwnode_p    node;
        zwnet_p     nw = wupoll_ctx->net;

        plt_mtx_lck(nw->mtx);

        node = zwnode_find(&nw->ctl, wupoll_ent->node_id);
        if (node)
        {
            wupoll_ent->wkup_intv = node->wkup_intv;
        }

        plt_mtx_ulck(nw->mtx);

        wupoll_ent->state_tmout = (wupoll_ent->wkup_intv == 0)? WUPOLL_CHK_DOWN_0_INTERVAL : WUPOLL_CHK_DOWN_INTV;
    }

    wupoll_ent->state_tmout += wupoll_ctx->tmr_tick;
}


/**
wkuppoll_tx_status_hdlr - Transmit status handler
@param[in]	wupoll_ctx	Poll context
@param[in]	msg	        Message
@return
*/
static void wkuppoll_tx_status_hdlr(wupoll_ctx_t *wupoll_ctx, wupoll_ev_msg_sts_t *msg)
{
    wupoll_q_ent_t *wupoll_ent;

    if ((wupoll_ent = wkuppoll_find_ent(wupoll_ctx, msg->node_id)) == NULL)
    {   //Not found, the report is unexpected
        return;
    }

    switch (msg->tx_sts)
    {
        case ZWHCI_NO_ERROR:
            //Node is awake
            wupoll_ent->state = WUPOLL_STA_INACTIVE;
            wupoll_ent->state_tmout = wupoll_ctx->tmr_tick + WUPOLL_CHK_INACTIVE_INTV;
            break;

        case WUPOLL_WAKEUP_NOTIFICATION:
            debug_wkup_ts_msg(&wupoll_ctx->net->plt_ctx, "wkuppoll_tx_status_hdlr: tx_status:wkup_notif, node id=%d, state=%u '%s'",
                               wupoll_ent->node_id, wupoll_ent->state, wkuppoll_state[wupoll_ent->state]);
            if ((wupoll_ent->state == WUPOLL_STA_ACTIVE) || (wupoll_ent->state == WUPOLL_STA_NODE_DOWN))
            {   //Wakeup Notification received
                const static uint8_t  cmd_buf[] = {COMMAND_CLASS_WAKE_UP, WAKE_UP_NOTIFICATION};
                zwnode_p    node;
                zwif_p      wkup_intf = NULL;
                zwnet_p     nw = wupoll_ctx->net;

                plt_mtx_lck(nw->mtx);

                node = zwnode_find(&nw->ctl, msg->node_id);
                if (node && (node->ep.intf))
                {
                    wkup_intf = zwif_find_cls(node->ep.intf, COMMAND_CLASS_WAKE_UP);
                }

                plt_mtx_ulck(nw->mtx);

                if (wkup_intf)
                {   //Call back report handler to handle wake up notification
                    zwif_rep_hdlr(wkup_intf, (uint8_t *)cmd_buf, 2, 0);
                }
                else
                {
                    debug_wkup_ts_msg(&wupoll_ctx->net->plt_ctx, "no wakeup interface");
                }

                //Change status to inactive
                wupoll_ent->state = WUPOLL_STA_INACTIVE;
                wupoll_ent->state_tmout = wupoll_ctx->tmr_tick + WUPOLL_CHK_INACTIVE_INTV;

            }
            break;

        case SESSION_ERROR_UNREACHABLE:
            //Node is down (Z/IP gateway didn't receive Wakeup Notification as expected)
            wupoll_ent->state = WUPOLL_STA_NODE_DOWN;
            wkuppoll_set_timeout(wupoll_ctx, wupoll_ent);
            break;

        case SESSION_ERROR_DEST_BUSY:
            //Node is sleeping
            wupoll_ent->state = WUPOLL_STA_ACTIVE;
            wupoll_ent->state_tmout = wupoll_ctx->tmr_tick + WUPOLL_CHK_ACTIVE_INTV;
            break;

        default:
            wupoll_ent->state = WUPOLL_STA_NODE_DOWN;
            wkuppoll_set_timeout(wupoll_ctx, wupoll_ent);
            break;
    }
//  debug_wkup_ts_msg(&wupoll_ctx->net->plt_ctx, "wkuppoll_tx_status_hdlr: node id=%d, new state=%u '%s'",
//                     wupoll_ent->node_id, wupoll_ent->state, wkuppoll_state[wupoll_ent->state]);

}


/**
wkuppoll_tmr_chk - thread to process timer tick event
@param[in]	data    Context
@return
*/
static void wkuppoll_tmr_chk(wupoll_ctx_t *wupoll_ctx)
{
    wupoll_q_ent_t *wupoll_ent;

    wupoll_ent = wupoll_ctx->wupoll_lst_hd;
    while (wupoll_ent)
    {   //Check for timeout
        if (util_tmr_exp_chk32(wupoll_ctx->tmr_tick, wupoll_ent->state_tmout))
        {
            switch (wupoll_ent->state)
            {
                case WUPOLL_STA_INACTIVE:
                case WUPOLL_STA_NODE_DOWN:
                    wkuppoll_probe_updt_state(wupoll_ctx, wupoll_ent);
                    break;

                case WUPOLL_STA_WAIT_TX_STS:
                    //Trasmit timeout, change state to inactive
                    wupoll_ent->state = WUPOLL_STA_INACTIVE;
                    wupoll_ent->state_tmout = wupoll_ctx->tmr_tick + WUPOLL_CHK_INACTIVE_INTV;
                    break;

                case WUPOLL_STA_ACTIVE:
                    //Haven't waken up for a long time, try to poll again
                    debug_wkup_ts_msg(&wupoll_ctx->net->plt_ctx, "WUPOLL_STA_ACTIVE timeout, node id=%d", wupoll_ent->node_id);
                    wupoll_ent->state = WUPOLL_STA_INACTIVE;
                    wupoll_ent->state_tmout = wupoll_ctx->tmr_tick + WUPOLL_ACTIVE_TMOUT_RESEND;
                    break;

            }
        }

        //Next
        wupoll_ent = wupoll_ent->next;
    }
}


/**
wkuppoll_tmr_tick_cb - Timer tick timeout callback
@param[in] data     Pointer to poll context
@return
*/
static void    wkuppoll_tmr_tick_cb(void *data)
{
    wupoll_ctx_t        *wupoll_ctx = (wupoll_ctx_t *)data;
    wupoll_evt_msg_t    msg;

    //Increment timer tick
    wupoll_ctx->tmr_tick++;

    //Send timer tick event
    msg.type = WUPOLL_EVT_TYPE_TIMER;

    if (wupoll_ctx->run)
    {
        util_msg_loop_send(wupoll_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(wupoll_evt_msg_t));
    }
}


/**
wkuppoll_rm_all - remove all polling requests
@param[in]	wupoll_ctx    Context
@return
*/
void wkuppoll_rm_all(wupoll_ctx_t *wupoll_ctx)
{
    wupoll_evt_msg_t    msg;

    msg.type = WUPOLL_EVT_TYPE_RM_ALL;

    if (wupoll_ctx->run)
    {
        util_msg_loop_send(wupoll_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(wupoll_evt_msg_t));
    }

}


/**
wkuppoll_rm_all_ent - remove all polling entries
@param[in]	wupoll_ctx    Context
@return
*/
static void wkuppoll_rm_all_ent(wupoll_ctx_t *wupoll_ctx)
{
    wupoll_q_ent_t *ent;
    wupoll_q_ent_t *rm_ent;      //Entry to be removed

    if (!wupoll_ctx->wupoll_lst_hd)
    {
        return;
    }

    ent = wupoll_ctx->wupoll_lst_hd;

    while (ent)
    {
        rm_ent = ent;
        ent = ent->next;
        free(rm_ent);
    }

    wupoll_ctx->wupoll_lst_hd = wupoll_ctx->wupoll_lst_tail = NULL;
}


/**
wkuppoll_add_poll - Add poll entry to end of list
@param[in]	wupoll_ctx		Context
@param[in]	add_msg		    Add new poll entry message
@return
*/
static void wkuppoll_add_poll(wupoll_ctx_t *wupoll_ctx, wupoll_ev_msg_add_t *add_msg)
{
    wupoll_q_ent_t  *wupoll_ent;

    wupoll_ent = wkuppoll_find_ent(wupoll_ctx, add_msg->node_id);

    if (wupoll_ent)
    {   //Entry is in the list, do nothing.
        return;
    }

     //Create new entry
    wupoll_ent = (wupoll_q_ent_t *)calloc(1, sizeof(wupoll_q_ent_t));

    if (wupoll_ent)
    {
        wupoll_ent->node_id = add_msg->node_id;
        wupoll_ent->wkup_intv = add_msg->wkup_intv;
        wupoll_ent->state = WUPOLL_STA_INACTIVE;
        wupoll_ent->state_tmout = wupoll_ctx->tmr_tick + WUPOLL_CHK_INIT_INTV;

        //Add new entry to the list
        wkuppoll_add_ent_endlist(wupoll_ctx, wupoll_ent);
    }
}


/**
wkuppoll_rm_poll - Remove poll entry
@param[in]	wupoll_ctx		Context
@param[in]	rm_msg		    Remove poll entry message
@return
*/
static void wkuppoll_rm_poll(wupoll_ctx_t *wupoll_ctx, wupoll_ev_msg_rm_t *rm_msg)
{
    wupoll_q_ent_t  *wupoll_ent;

    //Find if there is an entry in the queue
    if ((wupoll_ent = wkuppoll_find_ent(wupoll_ctx, rm_msg->node_id)) != NULL)
    {   //Found the entry
        wkuppoll_rm_ent(wupoll_ctx, wupoll_ent);
    }
}


/**
wkuppoll_add - add a wake-up notification polling request into polling queue
@param[in]	wupoll_ctx	context
@param[in]	node_id	    node id
@param[in]	wkup_intv	Wake up interval in seconds. Negative value = invalid or unknown
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/
int wkuppoll_add(wupoll_ctx_t *wupoll_ctx, uint8_t node_id, int32_t wkup_intv)
{
    wupoll_evt_msg_t msg;

#ifdef  RM_0_WKUP_INTV_ENTRY
    if (wkup_intv == 0)
    {
        return ZW_ERR_NONE;
    }
#endif
    //Send add polling event
    msg.type = WUPOLL_EVT_TYPE_ADD;
    msg.ev_msg.add.node_id = node_id;
    msg.ev_msg.add.wkup_intv = wkup_intv;

    if (util_msg_loop_send(wupoll_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(wupoll_evt_msg_t)) == 0)
    {
        return ZW_ERR_NONE;
    }

    return ZW_ERR_MEMORY;
}


/**
wkuppoll_add_all - Add all sleeping nodes for polling
@param[in]	wupoll_ctx	context
@return
*/
void wkuppoll_add_all(wupoll_ctx_t *wupoll_ctx)
{
    zwnode_p        node;
    zwnet_p         nw = wupoll_ctx->net;
    wupoll_evt_msg_t msg;

    if (!wupoll_ctx->run)
    {
        return;
    }

    //Send add polling event
    msg.type = WUPOLL_EVT_TYPE_ADD;

    plt_mtx_lck(nw->mtx);

    if ((node = (zwnode_p)nw->ctl.obj.next) != NULL)
    {
        while (node)
        {
            if (node->sleep_cap)
            {   //Sleeping node
                msg.ev_msg.add.node_id = node->nodeid;
                msg.ev_msg.add.wkup_intv = node->wkup_intv;
                util_msg_loop_send(wupoll_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(wupoll_evt_msg_t));
            }
            node = (zwnode_p)node->obj.next;
        }
    }
    plt_mtx_ulck(nw->mtx);

}


/**
wkuppoll_rm - remove a wake-up notification polling entry
@param[in]	wupoll_ctx	context
@param[in]	node_id	    node id
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/
int wkuppoll_rm(wupoll_ctx_t *wupoll_ctx, uint8_t node_id)
{
    wupoll_evt_msg_t msg;

    //Send remove polling event
    msg.type = WUPOLL_EVT_TYPE_RM;
    msg.ev_msg.rm.node_id = node_id;

    if (util_msg_loop_send(wupoll_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(wupoll_evt_msg_t)) == 0)
    {
        return ZW_ERR_NONE;
    }

    return ZW_ERR_MEMORY;
}


/**
wkuppoll_evt_msg_hdlr - Process message loop on message arrival
@param[in]	usr_prm	    user parameter supplied during initialization
@param[in]	msg	        message
return      1 to exit message loop; else return 0 to continue the message loop
*/
static int wkuppoll_evt_msg_hdlr(void *usr_prm, void *msg)
{
    wupoll_ctx_t      *wupoll_ctx = (wupoll_ctx_t *)usr_prm;
    wupoll_evt_msg_t  *evt_msg = (wupoll_evt_msg_t *)msg;

    if (!wupoll_ctx->run)
        return 0;

    switch (evt_msg->type)
    {
        case WUPOLL_EVT_TYPE_TIMER:
            wkuppoll_tmr_chk(wupoll_ctx);
            break;

        case WUPOLL_EVT_TYPE_ADD:
            wkuppoll_add_poll(wupoll_ctx, &evt_msg->ev_msg.add);
            break;

        case WUPOLL_EVT_TYPE_RM:
            wkuppoll_rm_poll(wupoll_ctx, &evt_msg->ev_msg.rm);
            break;

        case WUPOLL_EVT_TYPE_STATUS:
            wkuppoll_tx_status_hdlr(wupoll_ctx, &evt_msg->ev_msg.status);
            break;

        case WUPOLL_EVT_TYPE_RM_ALL:
            wkuppoll_rm_all_ent(wupoll_ctx);
            break;

    }

    return 0;
}


/**
wkuppoll_init - Initialize the polling facility
@param[in]	wupoll_ctx	    Polling context
@return  0 on success; negative error number on failure
@note  Must call wkuppoll_shutdown followed by wkuppoll_exit to shutdown and clean up the polling facility
*/
int wkuppoll_init(wupoll_ctx_t *wupoll_ctx)
{
    int     res;

    wupoll_ctx->run = 1;
    wupoll_ctx->tmr_tick = 0xFFFFFF00;
    wupoll_ctx->wupoll_lst_hd = NULL;
    wupoll_ctx->wupoll_lst_tail = NULL;

    res = util_msg_loop_init(wkuppoll_evt_msg_hdlr, wupoll_ctx, NULL, 6, &wupoll_ctx->evt_msg_loop_ctx);

    if (res != 0)
    {
        goto l_WUPOLL_INIT_ERROR1;
    }

    wupoll_ctx->tick_tmr_ctx = plt_periodic_start(&wupoll_ctx->net->plt_ctx, WUPOLL_TIMER_TICK, wkuppoll_tmr_tick_cb, wupoll_ctx);
    if (!wupoll_ctx->tick_tmr_ctx)
        goto l_WUPOLL_INIT_ERROR2;

    zip_old_status_cb_set(&wupoll_ctx->net->appl_ctx, wkuppoll_old_sts_cb, wupoll_ctx);

    return ZW_ERR_NONE;

l_WUPOLL_INIT_ERROR2:
    util_msg_loop_shutdown(wupoll_ctx->evt_msg_loop_ctx, 0);
    util_msg_loop_exit(wupoll_ctx->evt_msg_loop_ctx);
l_WUPOLL_INIT_ERROR1:
    return ZW_ERR_NO_RES;

}


/**
wkuppoll_shutdown - Shutdown the polling facility
@param[in]	wupoll_ctx	    Polling context
@return
*/
void wkuppoll_shutdown(wupoll_ctx_t *wupoll_ctx)
{
    wupoll_ctx->run = 0;

    //Stop timer
    plt_tmr_stop(&wupoll_ctx->net->plt_ctx, wupoll_ctx->tick_tmr_ctx);

    //Stop message loop
    util_msg_loop_shutdown(wupoll_ctx->evt_msg_loop_ctx, 0);
}


/**
wkuppoll_exit - Clean up
@param[in]	wupoll_ctx	    Polling context
@return
*/
void wkuppoll_exit(wupoll_ctx_t *wupoll_ctx)
{
    util_msg_loop_exit(wupoll_ctx->evt_msg_loop_ctx);

    //Flush the poll queue
    wkuppoll_rm_all_ent(wupoll_ctx);
}


/**
@}
*/















