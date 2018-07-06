/**
@file   zip_node_sts.c - Z-wave High Level API sleeping node status polling implementation.

@author David Chow

@version    1.0 20-2-17  Initial release

@copyright © 2017 SIGMA DESIGNS, INC. THIS IS AN UNPUBLISHED WORK PROTECTED BY SIGMA DESIGNS, INC.
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
#include "../include/zip_node_sts.h"

//#undef  NODSTS_DEFAULT_WKUP_INTV    //testing
//#define NODSTS_DEFAULT_WKUP_INTV      (3 * 60)   //testing

#if 0
#define     debug_nsts_ts_msg      plt_msg_ts_show     ///< Print debug message with time stamp
static const char *nodests_state[] = {  "Node is alive",
                                        "Node is down",
                                        "Node is sleeping",
                                        "Waiting transmit status"
                                     };
#else
#define     debug_nsts_ts_msg(...)
#endif


/**
@defgroup NodeStsPoll  Sleeping Node Status Polling APIs
Used to create and delete sleeping node status polling commands to a device
@{
*/


/**
nodests_wkup_rpt_cb - wake up capabilities report callback
@param[in]	ifd	    interface
@param[in]	cap		capabilities report, null for notification
@return
*/
static void nodests_wkup_rpt_cb(zwifd_p ifd, zwif_wakeup_p cap)
{
    nodests_ctx_t       *nodests_ctx = ifd->net->nsts_ctx;
    nodests_evt_msg_t   msg;

    msg.type = NODSTS_EVT_TYPE_WKUP_INTV;
    msg.ev_msg.wk_intv.node_id = ifd->nodeid;
    msg.ev_msg.wk_intv.wkup_intv = cap->cur;

    if (nodests_ctx->run)
    {
        util_msg_loop_send(nodests_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(nodests_evt_msg_t));
    }
}


/**
nodests_wkup_intv_get - Get wakeup interval
@param[in]	nw		    Network
@param[in]	wkup_intf   Wake up interface
@return  0 on successfully sending WAKE_UP_INTERVAL_GET; negative error number on failure
*/
static int nodests_wkup_intv_get(zwnet_p nw, zwif_p wkup_intf)
{
    int         result;
    zwifd_t     ifd;

    zwif_get_desc(wkup_intf, &ifd);

    result = zwif_set_report(&ifd, nodests_wkup_rpt_cb, WAKE_UP_INTERVAL_REPORT);
    if (result != 0)
    {
        return result;
    }

    if (wkup_intf->real_ver >= 2)
    {
        result = zwif_set_report(&ifd, nodests_wkup_rpt_cb, WAKE_UP_INTERVAL_CAPABILITIES_REPORT_V2);
        if (result != 0)
        {
            return result;
        }
    }

    //Get wake up interval
    zwif_get_report(&ifd, NULL, 0, WAKE_UP_INTERVAL_GET, zwif_tx_sts_cb);

    return 0;

}


/**
nodests_probe_cb - node probing callback function
@param[in]	appl_ctx    The application layer context
@param[in]	tx_sts		The transmit complete status
@param[in]	user_prm    The user specific parameter
@param[in]	node_id     Node id
@param[in]	param       Additional parameters
@return
*/
static void  nodests_probe_cb(appl_layer_ctx_t *appl_ctx, int8_t tx_sts, void *user_prm, uint8_t node_id, zw_tx_cb_prm_t *param)
{
    nodests_ctx_t        *nodests_ctx = (nodests_ctx_t *)user_prm;
    nodests_evt_msg_t    msg;

    if (tx_sts != TRANSMIT_COMPLETE_OK)
    {
        debug_nsts_ts_msg(appl_ctx->plt_ctx, "nodests_probe_cb:%u, NOP tx status:%d", node_id, tx_sts);
    }

    //Send NOP poll status event
    msg.type = NODSTS_EVT_TYPE_TX_STS;
    msg.ev_msg.status.node_id = node_id;
    msg.ev_msg.status.tx_sts = tx_sts;

    util_msg_loop_send(nodests_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(nodests_evt_msg_t));
}


/**
nodests_probe - send a "no operation" command to a node to test if it's reachable and update the state
@param[in]	nodests_ctx	context
@param[in]	nodests_ent  wake up poll entry
@return 0 if probe was sent successfully; else return negative number
*/
static int nodests_probe(nodests_ctx_t *nodests_ctx,  nodests_q_ent_t *nodests_ent)
{
    int                 result;
    appl_snd_data_t     prm = {0};
    uint8_t             cmd = COMMAND_CLASS_NO_OPERATION;
    zwnet_p             nw = nodests_ctx->net;

    prm.dat_len = 1;
    prm.dat_buf = &cmd;
    prm.node_id = nodests_ent->node_id;
    //prm.wkup_poll = 1;
    prm.flag = ZIP_FLAG_MORE_INFO;

    if (nw->use_encap_hdr)
    {   //Deprecate "secure origin" bit in Z/IP header. Alwasy set "secure origin" bit to 1
        prm.flag |= (ZIP_FLAG_SECURE | ZIP_FLAG_ENCAP_INFO);
    }

    result = zw_send_data(&nw->appl_ctx, &prm, nodests_probe_cb, nodests_ctx);

    debug_nsts_ts_msg(&nw->plt_ctx, "nodests_probe: node id=%d, state=%u '%s', res=%d",
                       nodests_ent->node_id, nodests_ent->state, nodests_state[nodests_ent->state], result);


    if (result < 0)
    {
        nodests_ent->state_tmout = nodests_ctx->tmr_tick + NODSTS_CHK_SHORT_INTV;
//      if ((result == SESSION_ERROR_PREVIOUS_COMMAND_UNCOMPLETED) || (result == APPL_ERROR_WAIT_CB))
//      {   //Busy, need retrying
//          nodests_ent->state_tmout = nodests_ctx->tmr_tick + NODSTS_CHK_LONG_INTV;
//      }
//      else
//      {   //Failed
//          nodests_ent->state = NODSTS_STA_NODE_DOWN;
//          nodests_ent->state_tmout = nodests_ctx->tmr_tick + NODSTS_CHK_DOWN_INTV;
//      }
    }
    else
    {   //Send NOP o.k.
        nodests_ent->state = NODSTS_STA_WAIT_TX_STS;
        nodests_ent->state_tmout = nodests_ctx->tmr_tick + NODSTS_TX_STATUS_TMOUT;
        debug_nsts_ts_msg(&nw->plt_ctx, "Change state to STA_WAIT_TX_STS");
    }

//  debug_nsts_ts_msg(&nw->plt_ctx, "nodests_probe: node id=%d, changed state=%u '%s', res=%d",
//                     nodests_ent->node_id, nodests_ent->state, nodests_state[nodests_ent->state], result);

    return result;
}


/**
nodests_find_ent - Find a poll entry
@param[in]	nodests_ctx	context
@param[in]	node_id     node id
@return poll entry if found; otherwise NULL.
*/
static nodests_q_ent_t *nodests_find_ent(nodests_ctx_t *nodests_ctx, uint8_t node_id)
{
    nodests_q_ent_t *ent;

    ent = nodests_ctx->nodests_lst_hd;

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
nodests_append_ent - Append entry to end of list
@param[in]	nodests_ctx		Context
@param[in]	ent		        New poll entry
@return     1 if the ent is the only entry in the list after adding; else return 0.
*/
static int nodests_append_ent(nodests_ctx_t *nodests_ctx, nodests_q_ent_t *ent)
{
    //The list is empty
    if (!nodests_ctx->nodests_lst_hd && !nodests_ctx->nodests_lst_tail)
    {
        nodests_ctx->nodests_lst_hd = nodests_ctx->nodests_lst_tail = ent;
        ent->next = ent->prev = NULL;
        return 1;
    }

    //Adjust the former tail
    nodests_ctx->nodests_lst_tail->next = ent;

    ent->prev = nodests_ctx->nodests_lst_tail;
    ent->next = NULL;

    //Assign the entry as tail
    nodests_ctx->nodests_lst_tail = ent;

    return 0;

}


/**
nodests_rm_ent - Remove entry from list
@param[in]	nodests_ctx		Context
@param[in]	ent		        Poll entry
@return
@pre Caller must lock nodests_mtx
@post ent should not be used as it is freed
*/
static void nodests_rm_ent(nodests_ctx_t *nodests_ctx, nodests_q_ent_t *ent)
{
    //There is only 1 entry in the list
    if (nodests_ctx->nodests_lst_hd == nodests_ctx->nodests_lst_tail)
    {
        nodests_ctx->nodests_lst_hd = nodests_ctx->nodests_lst_tail = NULL;
        free(ent);
        return;
    }

    //The entry is the head of the list
    if (nodests_ctx->nodests_lst_hd == ent)
    {
        nodests_ctx->nodests_lst_hd = ent->next;
        nodests_ctx->nodests_lst_hd->prev = NULL;
    }
    //The entry is the tail of the list
    else if (nodests_ctx->nodests_lst_tail == ent)
    {
        nodests_ctx->nodests_lst_tail = ent->prev;
        nodests_ctx->nodests_lst_tail->next = NULL;
    }
    else
    {
        ent->prev->next = ent->next;
        ent->next->prev = ent->prev;
    }

    free(ent);
}


/**
nodests_tx_status_hdlr - Transmit status handler
@param[in]	nodests_ctx	Poll context
@param[in]	msg	        Message
@return
*/
static void nodests_tx_status_hdlr(nodests_ctx_t *nodests_ctx, nodests_ev_msg_sts_t *msg)
{
    nodests_q_ent_t *nodests_ent;

    if ((nodests_ent = nodests_find_ent(nodests_ctx, msg->node_id)) == NULL)
    {   //Not found, the report is unexpected
        return;
    }

    if (nodests_ent->state != NODSTS_STA_WAIT_TX_STS)
    {
        return;
    }

    if (msg->tx_sts == ZWHCI_NO_ERROR)
    {   //Node is alive
        nodests_ent->state = NODSTS_STA_UP;
        nodests_ent->state_tmout = nodests_ctx->tmr_tick + NODSTS_CHK_LONG_INTV;
    }
    else
    {   //Node is sleeping
        nodests_ent->state = NODSTS_STA_SLEEP;

        debug_nsts_ts_msg(&nodests_ctx->net->plt_ctx, "Now:%u, Expected wkup:%u, wkup interval:%d", nodests_ctx->tmr_tick, nodests_ent->exp_wkup, nodests_ent->wkup_intv);

        if (nodests_ent->exp_wkup == 0)
        {
            if (nodests_ent->wkup_intv > 0)
            {
                nodests_ent->exp_wkup =  (uint32_t)nodests_ent->wkup_intv;
            }
            else
            {   //Unknown wakeup interval or manual activated wake up notification
                nodests_ent->exp_wkup = NODSTS_DEFAULT_WKUP_INTV;
            }

            nodests_ent->exp_wkup += nodests_ctx->tmr_tick + NODSTS_CHK_LONG_INTV;
        }

        debug_nsts_ts_msg(&nodests_ctx->net->plt_ctx, "Expected wkup:%u, wkup interval:%d", nodests_ent->exp_wkup, nodests_ent->wkup_intv);

        //Update node status
        appl_updt_node_sts(&nodests_ctx->net->appl_ctx, msg->node_id, APPL_NODE_STS_SLEEP, 1);

    }

    debug_nsts_ts_msg(&nodests_ctx->net->plt_ctx, "nodests_tx_status_hdlr: node id=%d, new state=%u '%s'",
                       nodests_ent->node_id, nodests_ent->state, nodests_state[nodests_ent->state]);

}


/**
nodests_tmr_chk - thread to process timer tick event
@param[in]	data    Context
@return
*/
static void nodests_tmr_chk(nodests_ctx_t *nodests_ctx)
{
    nodests_q_ent_t *nodests_ent;

    nodests_ent = nodests_ctx->nodests_lst_hd;
    while (nodests_ent)
    {
        switch (nodests_ent->state)
        {
            case NODSTS_STA_UP:
                if (util_tmr_exp_chk32(nodests_ctx->tmr_tick, nodests_ent->state_tmout))
                {
                    //nodests_probe(nodests_ctx, nodests_ent);
		    ALOGI("Close nodests_probe (NOP), avoid reset issue after adding battery device. Check here if any issue!!");
                }
                break;

            case NODSTS_STA_DOWN:
                break;

            case NODSTS_STA_SLEEP:
                if (util_tmr_exp_chk32(nodests_ctx->tmr_tick, nodests_ent->exp_wkup + NODSTS_WK_NOTIF_MARGIN))
                {
                    debug_nsts_ts_msg(&nodests_ctx->net->plt_ctx, "Expired. Now:%u, Expected wkup:%u", nodests_ctx->tmr_tick, nodests_ent->exp_wkup);

                    if (nodests_ent->wkup_intv > 0)
                    {
                        //Node is down
                        nodests_ent->state = NODSTS_STA_DOWN;

                        //Update node status
                        appl_updt_node_sts(&nodests_ctx->net->appl_ctx, nodests_ent->node_id, APPL_NODE_STS_FORCE_DOWN, 1);
                    }
                    else
                    {   //Unknown wakeup interval or manual activated wake up notification, , reload expected wakeup interval
                        nodests_ent->exp_wkup = nodests_ctx->tmr_tick + NODSTS_DEFAULT_WKUP_INTV;
                    }
                }
                break;

            case NODSTS_STA_WAIT_TX_STS:
                if (util_tmr_exp_chk32(nodests_ctx->tmr_tick, nodests_ent->state_tmout))
                {
                    nodests_ent->state = NODSTS_STA_UP;
                    nodests_ent->state_tmout = nodests_ctx->tmr_tick + NODSTS_CHK_SHORT_INTV;
                    debug_nsts_ts_msg(&nodests_ctx->net->plt_ctx, "Wait tx status timeout");
                }
                break;
        }

        //Next
        nodests_ent = nodests_ent->next;
    }
}


/**
nodests_tmr_tick_cb - Timer tick timeout callback
@param[in] data     Pointer to poll context
@return
*/
static void    nodests_tmr_tick_cb(void *data)
{
    nodests_ctx_t        *nodests_ctx = (nodests_ctx_t *)data;
    nodests_evt_msg_t    msg;

    //Increment timer tick
    nodests_ctx->tmr_tick++;

    //Send timer tick event
    msg.type = NODSTS_EVT_TYPE_TIMER;

    if (nodests_ctx->run)
    {
        util_msg_loop_send(nodests_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(nodests_evt_msg_t));
    }
}


/**
nodests_rm_all - remove all polling requests
@param[in]	nodests_ctx    Context
@return
*/
void nodests_rm_all(nodests_ctx_t *nodests_ctx)
{
    nodests_evt_msg_t    msg;

    msg.type = NODSTS_EVT_TYPE_RM_ALL;

    if (nodests_ctx->run)
    {
        util_msg_loop_send(nodests_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(nodests_evt_msg_t));
    }

}


/**
nodests_rm_all_ent - remove all polling entries
@param[in]	nodests_ctx    Context
@return
*/
static void nodests_rm_all_ent(nodests_ctx_t *nodests_ctx)
{
    nodests_q_ent_t *ent;
    nodests_q_ent_t *rm_ent;      //Entry to be removed

    if (!nodests_ctx->nodests_lst_hd)
    {
        return;
    }

    ent = nodests_ctx->nodests_lst_hd;

    while (ent)
    {
        rm_ent = ent;
        ent = ent->next;
        free(rm_ent);
    }

    nodests_ctx->nodests_lst_hd = nodests_ctx->nodests_lst_tail = NULL;
}


/**
nodests_add_poll - Add poll entry to end of list
@param[in]	nodests_ctx		Context
@param[in]	add_msg		    Add new poll entry message
@return
*/
static void nodests_add_poll(nodests_ctx_t *nodests_ctx, nodests_ev_msg_add_t *add_msg)
{
    nodests_q_ent_t  *nodests_ent;

    nodests_ent = nodests_find_ent(nodests_ctx, add_msg->node_id);

    if (nodests_ent)
    {   //Entry is in the list, do nothing.
        return;
    }

    //Create new entry
    nodests_ent = (nodests_q_ent_t *)calloc(1, sizeof(nodests_q_ent_t));

    if (nodests_ent)
    {
        nodests_ent->node_id = add_msg->node_id;
        nodests_ent->state = add_msg->state;
        nodests_ent->wkup_intv = add_msg->wkup_intv;

        if (add_msg->state == NODSTS_STA_UP)
        {
            nodests_ent->state_tmout = nodests_ctx->tmr_tick + NODSTS_CHK_SHORT_INTV;
        }
        else if (add_msg->state == NODSTS_STA_SLEEP)
        {
            if (add_msg->wkup_intv > 0)
            {
                nodests_ent->exp_wkup =  (uint32_t)add_msg->wkup_intv;
            }
            else
            {   //Unknown wakeup interval or manual activated wake up notification
                nodests_ent->exp_wkup = NODSTS_DEFAULT_WKUP_INTV;
            }

            nodests_ent->exp_wkup += nodests_ctx->tmr_tick;

            debug_nsts_ts_msg(&nodests_ctx->net->plt_ctx, "Now:%u, Expected wkup:%u, wkup interval:%d", nodests_ctx->tmr_tick, nodests_ent->exp_wkup, nodests_ent->wkup_intv);
        }
        else
        {
            free(nodests_ent);
            return;
        }

        //Add new entry to the list
        nodests_append_ent(nodests_ctx, nodests_ent);
    }
}


/**
nodests_rm_poll - Remove poll entry
@param[in]	nodests_ctx		Context
@param[in]	rm_msg		    Remove poll entry message
@return
*/
static void nodests_rm_poll(nodests_ctx_t *nodests_ctx, nodests_ev_msg_rm_t *rm_msg)
{
    nodests_q_ent_t  *nodests_ent;

    //Find if there is an entry in the queue
    if ((nodests_ent = nodests_find_ent(nodests_ctx, rm_msg->node_id)) != NULL)
    {   //Found the entry
        nodests_rm_ent(nodests_ctx, nodests_ent);
    }
}


/**
nodests_wkup_hdlr - Wakeup notification handler
@param[in]	nodests_ctx		Context
@param[in]	node	        Node
@return
*/
static void nodests_wkup_hdlr(nodests_ctx_t *nodests_ctx, zwnoded_p node)
{
    nodests_q_ent_t  *nodests_ent;
    int              get_wkup_intv = 0;

    nodests_ent = nodests_find_ent(nodests_ctx, node->nodeid);

    if (!nodests_ent)
    {
        return;
    }

    switch (nodests_ent->state)
    {
        case NODSTS_STA_SLEEP:
        case NODSTS_STA_UP:
        case NODSTS_STA_WAIT_TX_STS:
            if ((nodests_ent->wkup_intv < 0)
                || (util_time_diff32(nodests_ent->exp_wkup, nodests_ctx->tmr_tick) > NODSTS_WK_NOTIF_TOO_EARLY))
            {
                get_wkup_intv = 1;
            }

            if (nodests_ent->wkup_intv == 0)
            {   //Manual activated wake up notification
                get_wkup_intv = 0;
            }
            //no break

        case NODSTS_STA_DOWN:
            if (nodests_ent->state == NODSTS_STA_DOWN)
            {
                get_wkup_intv = 1;
            }

            if (nodests_ent->wkup_intv > 0)
            {
                nodests_ent->exp_wkup =  (uint32_t)nodests_ent->wkup_intv;
            }
            else
            {   //Unknown wakeup interval or manual activated wake up notification
                nodests_ent->exp_wkup = NODSTS_DEFAULT_WKUP_INTV;
            }

            nodests_ent->exp_wkup += nodests_ctx->tmr_tick;

            debug_nsts_ts_msg(&nodests_ctx->net->plt_ctx, "Now:%u, Expected wkup:%u, wkup interval:%d", nodests_ctx->tmr_tick, nodests_ent->exp_wkup, nodests_ent->wkup_intv);

            //Change state
            nodests_ent->state = NODSTS_STA_UP;
            nodests_ent->state_tmout = nodests_ctx->tmr_tick + NODSTS_CHK_SHORT_INTV;

            if (get_wkup_intv)
            {
                zwif_p      intf;
                zwnode_p    slp_node;

                plt_mtx_lck(nodests_ctx->net->mtx);

                slp_node = zwnode_find(&nodests_ctx->net->ctl, node->nodeid);

                if (slp_node)
                {
                    intf = zwif_find_cls(slp_node->ep.intf, COMMAND_CLASS_WAKE_UP);
                    if (intf)
                    {
                        //tiny
                        //nodests_wkup_intv_get(nodests_ctx->net, intf);
                    }
                }
                plt_mtx_ulck(nodests_ctx->net->mtx);
            }

            //Update node status
            appl_updt_node_sts(&nodests_ctx->net->appl_ctx, node->nodeid, APPL_NODE_STS_UP, 1);

            break;

    }
}


/**
nodests_wkup_intv_hdlr - Wakeup interval handler
@param[in]	nodests_ctx		Context
@param[in]	wk_intv	        wakeup interval info
@return
*/
static void nodests_wkup_intv_hdlr(nodests_ctx_t *nodests_ctx, nodests_ev_msg_intv_t *wk_intv)
{
    nodests_q_ent_t *nodests_ent;
    int32_t         wkup_interval;

    nodests_ent = nodests_find_ent(nodests_ctx, wk_intv->node_id);

    if (!nodests_ent)
    {
        return;
    }

    wkup_interval = (int32_t)(wk_intv->wkup_intv & 0x00FFFFFF);

    if (wkup_interval != nodests_ent->wkup_intv)
    {   //Different wakeup interval, update the node wakeup interval

        nodests_ent->wkup_intv = wkup_interval;

        //Re-calculate expected wakeup notification time
        if (wkup_interval > 0)
        {
            nodests_ent->exp_wkup = (uint32_t)wkup_interval;
        }
        else
        {   //Manual activated wake up notification
            nodests_ent->exp_wkup = NODSTS_DEFAULT_WKUP_INTV;
        }

        nodests_ent->exp_wkup += nodests_ctx->tmr_tick;

    }
}


/**
nodests_on_wkup - Callback when a sleeping node sends wake up notification
@param[in]	dpoll_ctx   Polling context
@param[in]	noded	    Node
@return
*/
void nodests_on_wkup(nodests_ctx_t *nodests_ctx, zwnoded_p noded)
{
    nodests_evt_msg_t msg;

    msg.type = NODSTS_EVT_TYPE_WAKEUP;
    msg.ev_msg.node = *noded;

    if (nodests_ctx->run)
    {
        util_msg_loop_send(nodests_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(nodests_evt_msg_t));
    }
}


/**
nodests_add - Add anode status polling request into polling queue
@param[in]	nodests_ctx	context
@param[in]	node_id	    node id
@param[in]	state	    current state of the node, see NODSTS_STA_XXX. Only NODSTS_STA_UP and NODSTS_STA_SLEEP are supported
@param[in]	wkup_intv	Wake up interval in seconds. Negative value = invalid or unknown
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/
int nodests_add(nodests_ctx_t *nodests_ctx, uint8_t node_id, uint8_t state, int32_t wkup_intv)
{
    nodests_evt_msg_t msg;

    if ((state != NODSTS_STA_UP) && (state != NODSTS_STA_SLEEP))
    {
        return ZW_ERR_VALUE;
    }

    //Update node sleeping capability
    appl_updt_node_slp_cap(&nodests_ctx->net->appl_ctx, node_id, 1);

    //Send add polling event
    msg.type = NODSTS_EVT_TYPE_ADD;
    msg.ev_msg.add.node_id = node_id;
    msg.ev_msg.add.wkup_intv = wkup_intv;
    msg.ev_msg.add.state = state;

    if (util_msg_loop_send(nodests_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(nodests_evt_msg_t)) == 0)
    {
        return ZW_ERR_NONE;
    }

    return ZW_ERR_MEMORY;
}


/**
nodests_add_all - Add all sleeping nodes for polling
@param[in]	nodests_ctx	context
@return
*/
void nodests_add_all(nodests_ctx_t *nodests_ctx)
{
    zwnode_p        node;
    zwnet_p         nw = nodests_ctx->net;
    nodests_evt_msg_t msg;

    if (!nodests_ctx->run)
    {
        return;
    }

    //Send add polling event
    msg.type = NODSTS_EVT_TYPE_ADD;

    plt_mtx_lck(nw->mtx);

    if ((node = (zwnode_p)nw->ctl.obj.next) != NULL)
    {
        while (node)
        {
            if (node->sleep_cap)
            {   //Sleeping node

                //Update node status
                appl_updt_node_slp_cap(&nw->appl_ctx, node->nodeid, 1);
                appl_updt_node_sts(&nw->appl_ctx, node->nodeid, APPL_NODE_STS_SLEEP, 0);

                msg.ev_msg.add.node_id = node->nodeid;
                msg.ev_msg.add.wkup_intv = node->wkup_intv;
                msg.ev_msg.add.state = NODSTS_STA_SLEEP;

                util_msg_loop_send(nodests_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(nodests_evt_msg_t));
            }
            node = (zwnode_p)node->obj.next;
        }
    }
    plt_mtx_ulck(nw->mtx);

}


/**
nodests_rm - remove a wake-up notification polling entry
@param[in]	nodests_ctx	context
@param[in]	node_id	    node id
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/
int nodests_rm(nodests_ctx_t *nodests_ctx, uint8_t node_id)
{
    nodests_evt_msg_t msg;

    //Send remove polling event
    msg.type = NODSTS_EVT_TYPE_RM;
    msg.ev_msg.rm.node_id = node_id;

    if (util_msg_loop_send(nodests_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(nodests_evt_msg_t)) == 0)
    {
        return ZW_ERR_NONE;
    }

    return ZW_ERR_MEMORY;
}


/**
nodests_evt_msg_hdlr - Process message loop on message arrival
@param[in]	usr_prm	    user parameter supplied during initialization
@param[in]	msg	        message
return      1 to exit message loop; else return 0 to continue the message loop
*/
static int nodests_evt_msg_hdlr(void *usr_prm, void *msg)
{
    nodests_ctx_t      *nodests_ctx = (nodests_ctx_t *)usr_prm;
    nodests_evt_msg_t  *evt_msg = (nodests_evt_msg_t *)msg;

    if (!nodests_ctx->run)
        return 0;

    switch (evt_msg->type)
    {
        case NODSTS_EVT_TYPE_TIMER:
            nodests_tmr_chk(nodests_ctx);
            break;

        case NODSTS_EVT_TYPE_ADD:
            nodests_add_poll(nodests_ctx, &evt_msg->ev_msg.add);
            break;

        case NODSTS_EVT_TYPE_RM:
            nodests_rm_poll(nodests_ctx, &evt_msg->ev_msg.rm);
            break;

        case NODSTS_EVT_TYPE_TX_STS:
            nodests_tx_status_hdlr(nodests_ctx, &evt_msg->ev_msg.status);
            break;

        case NODSTS_EVT_TYPE_RM_ALL:
            nodests_rm_all_ent(nodests_ctx);
            break;

        case NODSTS_EVT_TYPE_WAKEUP:
            nodests_wkup_hdlr(nodests_ctx, &evt_msg->ev_msg.node);
            break;

        case NODSTS_EVT_TYPE_WKUP_INTV:
            nodests_wkup_intv_hdlr(nodests_ctx, &evt_msg->ev_msg.wk_intv);
            break;

    }

    return 0;
}


/**
nodests_init - Initialize the polling facility
@param[in]	nodests_ctx	    Polling context
@return  0 on success; negative error number on failure
@note  Must call nodests_shutdown followed by nodests_exit to shutdown and clean up the polling facility
*/
int nodests_init(nodests_ctx_t *nodests_ctx)
{
    int     res;

    nodests_ctx->run = 1;
    nodests_ctx->tmr_tick = 0xFFFFFF00;
    nodests_ctx->nodests_lst_hd = NULL;
    nodests_ctx->nodests_lst_tail = NULL;

    res = util_msg_loop_init(nodests_evt_msg_hdlr, nodests_ctx, NULL, 9, &nodests_ctx->evt_msg_loop_ctx);

    if (res != 0)
    {
        goto l_NODSTS_INIT_ERROR1;
    }

    nodests_ctx->tick_tmr_ctx = plt_periodic_start(&nodests_ctx->net->plt_ctx, NODSTS_TIMER_TICK, nodests_tmr_tick_cb, nodests_ctx);
    if (!nodests_ctx->tick_tmr_ctx)
        goto l_NODSTS_INIT_ERROR2;

    return ZW_ERR_NONE;

l_NODSTS_INIT_ERROR2:
    util_msg_loop_shutdown(nodests_ctx->evt_msg_loop_ctx, 0);
    util_msg_loop_exit(nodests_ctx->evt_msg_loop_ctx);
l_NODSTS_INIT_ERROR1:
    return ZW_ERR_NO_RES;

}


/**
nodests_shutdown - Shutdown the polling facility
@param[in]	nodests_ctx	    Polling context
@return
*/
void nodests_shutdown(nodests_ctx_t *nodests_ctx)
{
    nodests_ctx->run = 0;

    //Stop timer
    plt_tmr_stop(&nodests_ctx->net->plt_ctx, nodests_ctx->tick_tmr_ctx);

    //Stop message loop
    util_msg_loop_shutdown(nodests_ctx->evt_msg_loop_ctx, 0);
}


/**
nodests_exit - Clean up
@param[in]	nodests_ctx	    Polling context
@return
*/
void nodests_exit(nodests_ctx_t *nodests_ctx)
{
    util_msg_loop_exit(nodests_ctx->evt_msg_loop_ctx);

    //Flush the poll queue
    nodests_rm_all_ent(nodests_ctx);
}


/**
@}
*/















