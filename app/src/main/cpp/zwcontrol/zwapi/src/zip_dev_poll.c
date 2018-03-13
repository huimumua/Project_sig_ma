/**
@file   zip_dev_poll.c - Z-wave High Level API device poll facility implementation.

@author David Chow

@version    1.0 29-9-15  Initial release

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
#include "../include/zip_dev_poll.h"
#include "../include/zip_poll.h"

//#define DISABLE_DEV_POLLING ///< Disable device polling (for debugging purposes)


//External function declaration
int zwnet_1_ep_info_sm(zwnet_p nw, zwnet_1_ep_evt_t evt, uint8_t *data);
void zwnet_send_wkup_no_more_info(zwnet_p nw, uint8_t node_id);

/**
@defgroup If_DevPoll Device Polling Interface APIs
Used to create and delete device polling
@{
*/


/**
zwdpoll_flirs_node_cmp - Compare two FLIRS node id
@param[in]	dpoll_ctx   Polling context
@param[in]	s1	        flirs entry
@param[in]	s2	        other flirs entry
return 0 if s1 is same as s2; else return non-zero
*/
static int zwdpoll_flirs_node_cmp(uint8_t *s1, uint8_t *s2)
{
    dpoll_flirs_t   *ent1 = (dpoll_flirs_t *)s1;
    dpoll_flirs_t   *ent2 = (dpoll_flirs_t *)s2;

    return (ent1->noded.nodeid - ent2->noded.nodeid);
}


/**
zwdpoll_ao_node_cmp - Compare two always-on node id
@param[in]	dpoll_ctx   Polling context
@param[in]	s1	        always-on entry
@param[in]	s2	        other always-on entry
return 0 if s1 is same as s2; else return non-zero
*/
static int zwdpoll_ao_node_cmp(uint8_t *s1, uint8_t *s2)
{
    zwnoded_p   ent1 = (zwnoded_p)s1;
    zwnoded_p   ent2 = (zwnoded_p)s2;

    return (ent1->nodeid - ent2->nodeid);
}


/**
zwdpoll_on_wkup - Callback when a sleeping node sends wake up notification and there are commands in the queue
@param[in]	dpoll_ctx   Polling context
@param[in]	noded	    Node
@return
*/
void zwdpoll_on_wkup(zwdpoll_ctx_t *dpoll_ctx, zwnoded_p noded)
{
    dpoll_evt_msg_t msg;

    msg.type = DEV_POLL_EVT_WAKEUP;
    msg.noded = *noded;

    if (dpoll_ctx->run)
    {
        util_msg_loop_send(dpoll_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(dpoll_evt_msg_t));
    }
}


/**
zwdpoll_ep_sm_next - Process next endpoint or terminate the state-machine if no more endpoint.
@param[in]	nw		        Network
@param[in]	first_node_id   The first node id
@return
*/
static void zwdpoll_ep_sm_next(zwnet_p nw, uint8_t first_node_id)
{
    zwep_p      ep;
    zwif_p      first_intf; //first interface of the first endpoint
    zwif_p      intf;
    zwnoded_t   noded;
    ep_sm_job_t sm_job = {0};


    //Stop timer
    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
    nw->sm_tmr_ctx = NULL;    //make sure timer context is null, else restart timer will crash

    //Get next endpoint
    nw->ep_sm_job.cur_ep_id++;
    ep = zwep_get_by_id(nw, nw->ep_sm_job.node_id, nw->ep_sm_job.cur_ep_id);

    while (ep)
    {
        sm_job.cb = zwdpoll_ep_sm_next;
        sm_job.node_id = ep->node->nodeid;
        sm_job.cur_ep_id = ep->epid;
        sm_job.use_cache = 1;

        if (zwnet_1_ep_info_sm(nw, EVT_S_EP_SM_START, (uint8_t *)&sm_job))
        {
            //Change state
            //nw->ep_sm_sta = EP_STA_GET_EP_INFO;
            return;
        }
        else
        {
            if (nw->single_ep_sta != S_EP_STA_IDLE)
            {
                debug_zwapi_msg(&nw->plt_ctx,
                                "zwdpoll_ep_sm_next: single endpoint state-machine is in wrong state: %d",
                                nw->single_ep_sta);
            }
        }

        //Point to next endpoint
        ep = (zwep_p)ep->obj.next;
        nw->ep_sm_job.cur_ep_id++;
    }

    //Done with all endpoints
    ep = zwep_get_by_id(nw, nw->ep_sm_job.node_id, 0);

    first_intf = (ep)? ep->intf : NULL;

    intf = zwif_find_cls(first_intf, COMMAND_CLASS_WAKE_UP);

    if (intf)
    {   //This is a sleeping node

        //Callback other sub-system
        zwpoll_on_wkup(intf->ep->node);

        if (!nw->multi_client)
        {
            zwnet_send_wkup_no_more_info(nw, intf->ep->node->nodeid);
        }
    }

    //Reset network operation
    nw->curr_op = ZWNET_OP_NONE;

    if (ep && zwnode_get_desc(ep->node, &noded) == ZW_ERR_NONE)
    {
        zwdpoll_on_complt(nw->dpoll_ctx, &noded);
    }

}


/**
zwdpoll_node_updt_sm_start - Start state-machine to update node status and information using cache if available
@param[in]	noded	Node
@return		ZW_ERR_xxx
*/
static int zwdpoll_node_updt_sm_start(zwnoded_p noded)
{
    zwnet_p     net = noded->net;
    zwnode_p    node;
    ep_sm_job_t sm_job = {0};

    plt_mtx_lck(net->mtx);
    if (net->curr_op != ZWNET_OP_NONE)
    {
        debug_zwapi_msg(&net->plt_ctx, "zwdpoll_node_updt_sm_start failed. Other operation not completed yet, try again later");
        plt_mtx_ulck(net->mtx);
        return ZW_ERR_LAST_OP_NOT_DONE;
    }

    //Block node update of controller itself
    if (net->ctl.nodeid == noded->nodeid)
    {
        plt_mtx_ulck(net->mtx);
        return  ZW_ERR_UNSUPPORTED;
    }

    node = zwnode_find(&net->ctl, noded->nodeid);
    if (!node)
    {
        plt_mtx_ulck(net->mtx);
        return ZW_ERR_NODE_NOT_FOUND;
    }

    net->curr_op = ZWNET_OP_NODE_CACHE_UPT;

    sm_job.cb = zwdpoll_ep_sm_next;
    sm_job.node_id = net->ep_sm_job.node_id = node->nodeid;
    sm_job.cur_ep_id = net->ep_sm_job.cur_ep_id = 0;
    sm_job.use_cache = 1;

    //Stop timer
    plt_tmr_stop(&net->plt_ctx, net->sm_tmr_ctx);
    net->sm_tmr_ctx = NULL;

    if (zwnet_1_ep_info_sm(net, EVT_S_EP_SM_START, (uint8_t *)&sm_job))
    {   //o.k.
        plt_mtx_ulck(net->mtx);
        return 0;
    }

    if (net->single_ep_sta != S_EP_STA_IDLE)
    {
        debug_zwapi_msg(&net->plt_ctx,
                        "zwnet_ep_info_sm: single endpoint state-machine is in wrong state: %d",
                        net->single_ep_sta);
    }
    net->curr_op = ZWNET_OP_NONE;
    plt_mtx_ulck(net->mtx);
    return ZW_ERR_FAILED;
}


/**
zwdpoll_tmr_tick_cb - Timer tick timeout callback
@param[in] data     Pointer to poll context
@return
*/
static void    zwdpoll_tmr_tick_cb(void *data)
{
    zwdpoll_ctx_t   *dpoll_ctx = (zwdpoll_ctx_t *)data;

    //Increment timer tick
    dpoll_ctx->tmr_tick++;

    //Send timer tick event
    plt_sem_post(dpoll_ctx->tmr_sem);
}


/**
zwdpoll_node_cache_updt - Check for any pending poll that has expired and then perform node cache update
@param[in]	dpoll_ctx   Polling context
@return		1 if node cache update has been started, 0 if no action has been performed; else ZW_ERR_XXX on error
*/
static int zwdpoll_node_cache_updt(zwdpoll_ctx_t *dpoll_ctx)
{
    zwnet_p         nw = dpoll_ctx->net;
    util_lst_t      *lst_ent;
    dpoll_flirs_t   *flirs_ent;
    zwnoded_p       noded;
    int             res = 0;
    int             found_cur_ao = 0;

    if (!dpoll_ctx->run)
    {
        return ZW_ERR_SHUTDOWN;
    }

    //------------------------------------------------------
    //Check for priority queue
    //------------------------------------------------------
    if (dpoll_ctx->priority_hd)
    {
        plt_mtx_lck(nw->mtx);
        if (nw->curr_op != ZWNET_OP_NONE)
        {   //Other operation is in progress
            plt_mtx_ulck(nw->mtx);
            return 0;
        }

        if ((lst_ent = util_list_get(NULL, &dpoll_ctx->priority_hd)) != NULL)
        {
            noded = (zwnoded_p)lst_ent->wr_buf;

            res = zwdpoll_node_updt_sm_start(noded);
            if (res == 0)
            {   //Only sleeping device is allowed in priority queue
                dpoll_ctx->cur_device_type = DEV_POLL_DEV_SLEEPING;
                res = 1;
            }
            else
            {
                debug_zwapi_msg(&nw->plt_ctx, "zwdpoll_node_updt_sm_start failed: %d", res);
            }

            free(lst_ent);
        }
        plt_mtx_ulck(nw->mtx);
        return res;
    }

    //------------------------------------------------------
    //Check for FLIRS queue
    //------------------------------------------------------
    lst_ent = dpoll_ctx->flirs_hd;

    while (lst_ent)
    {
        flirs_ent = (dpoll_flirs_t *)lst_ent->wr_buf;
        //Check for expired entry
        if (util_tmr_exp_chk32(dpoll_ctx->tmr_tick, flirs_ent->next_poll_tm))
        {
            plt_mtx_lck(nw->mtx);
            if (nw->curr_op != ZWNET_OP_NONE)
            {   //Other operation is in progress
                plt_mtx_ulck(nw->mtx);
                return 0;
            }
            res = zwdpoll_node_updt_sm_start(&flirs_ent->noded);
            if (res == 0)
            {
                dpoll_ctx->cur_device_type = DEV_POLL_DEV_FLIRS;
                res = 1;
            }
            else
            {
                debug_zwapi_msg(&nw->plt_ctx, "zwdpoll_node_updt_sm_start flirs failed: %d", res);

                //Remove this poll entry if the node is invalid
                if (res == ZW_ERR_NODE_NOT_FOUND)
                {
                    dpoll_flirs_t   tmp_ent;
                    tmp_ent.noded = flirs_ent->noded;
                    util_list_rm(NULL, &dpoll_ctx->flirs_hd, (uint8_t *)&tmp_ent, zwdpoll_flirs_node_cmp);
                }
            }
            plt_mtx_ulck(nw->mtx);
            return res;
        }

        lst_ent = lst_ent->next;
    }

    //------------------------------------------------------
    //Check for Always-on device queue
    //------------------------------------------------------
    if (!dpoll_ctx->ao_hd
        || !util_tmr_exp_chk32(dpoll_ctx->tmr_tick, dpoll_ctx->next_poll_tm))
    {
        return 0;
    }

    lst_ent = dpoll_ctx->ao_hd;

    while (lst_ent)
    {
        noded = (zwnoded_p)lst_ent->wr_buf;

        if (noded->nodeid == dpoll_ctx->cur_ao_nodeid)
        {
            found_cur_ao = 1;
            break;
        }

        lst_ent = lst_ent->next;
    }

    if (!found_cur_ao)
    {   //The current always-on device has been removed. Assigned to the first one.
        lst_ent = dpoll_ctx->ao_hd;
    }

    noded = (zwnoded_p)lst_ent->wr_buf;

    //Node cached update
    plt_mtx_lck(nw->mtx);
    if (nw->curr_op != ZWNET_OP_NONE)
    {   //Other operation is in progress
        plt_mtx_ulck(nw->mtx);
        return 0;
    }

    res = zwdpoll_node_updt_sm_start(noded);
    if (res == 0)
    {
        dpoll_ctx->cur_device_type = DEV_POLL_DEV_AO;
        dpoll_ctx->cur_ao_nodeid = noded->nodeid;
        //Assume current operation takes some time to finish.  Next poll time will be updated
        //when the cache update operation has completed.
        dpoll_ctx->next_poll_tm = dpoll_ctx->tmr_tick + (60 * DEV_POLL_TICK_PER_SEC);
        res = 1;
    }
    else
    {
        debug_zwapi_msg(&nw->plt_ctx, "zwdpoll_node_updt_sm_start ao failed: %d", res);

        //Remove this poll entry if the node is invalid
        if (res == ZW_ERR_NODE_NOT_FOUND)
        {
            zwnoded_t   tmp_noded;

            tmp_noded = *noded;
            util_list_rm(NULL, &dpoll_ctx->ao_hd, (uint8_t *)&tmp_noded, zwdpoll_ao_node_cmp);
        }

    }
    plt_mtx_ulck(nw->mtx);
    return res;
}


/**
zwdpoll_tmr_chk_thrd - thread to process timer tick event
@param[in]	data	Context
@return
*/
static void zwdpoll_tmr_chk_thrd(void *data)
{
    zwdpoll_ctx_t   *dpoll_ctx = (zwdpoll_ctx_t *)data;

    dpoll_ctx->tmr_chk_thrd_sts = 1;

    while (1)
    {
        //Wait for timer tick event
        plt_sem_wait(dpoll_ctx->tmr_sem);

        //Check whether to exit the thread
        if (!dpoll_ctx->run || dpoll_ctx->tmr_chk_thrd_run == 0)
        {
            dpoll_ctx->tmr_chk_thrd_sts = 0;
            return;
        }

        if (dpoll_ctx->net->curr_op == ZWNET_OP_NONE)
        {
            plt_mtx_lck(dpoll_ctx->poll_mtx);
            if (dpoll_ctx->priority_hd
                || ((dpoll_ctx->tmr_tick % DEV_POLL_TICK_PER_SEC) == 0))
            {   //There is an entry in priority queue or 1 second has elapsed
                zwdpoll_node_cache_updt(dpoll_ctx);
            }
            plt_mtx_ulck(dpoll_ctx->poll_mtx);
        }
    }
}


/**
zwdpoll_add_node_evt_hdlr - Add node event handler
@param[in]	dpoll_ctx   Polling context
@param[in]	noded	    Node
return
*/
static void zwdpoll_add_node_evt_hdlr(zwdpoll_ctx_t *dpoll_ctx, zwnoded_p noded)
{
    int     res;

    if (noded->sensor)
    {   //FLIRS device
         dpoll_flirs_t   flirs_ent;

         flirs_ent.noded = *noded;
         flirs_ent.next_poll_tm = dpoll_ctx->tmr_tick + FLIRS_DEV_POLL_TIME;
         //flirs_ent.next_poll_tm = dpoll_ctx->tmr_tick + DEV_POLL_TICK_PER_SEC * 60;//Testing
         res = util_list_find(NULL, dpoll_ctx->flirs_hd, (uint8_t *)&flirs_ent, zwdpoll_flirs_node_cmp);
         if (res == 0)
         {  //There isn't any FLIRS with the same node id found
             util_list_add(NULL, &dpoll_ctx->flirs_hd, (uint8_t *)&flirs_ent, sizeof(dpoll_flirs_t));
         }
    }
    else if (noded->listen)
    {   //Always-on device
        res = util_list_find(NULL, dpoll_ctx->ao_hd, (uint8_t *)noded, zwdpoll_ao_node_cmp);
        if (res == 0)
        {  //There isn't any node with the same node id found
            util_list_add(NULL, &dpoll_ctx->ao_hd, (uint8_t *)noded, sizeof(zwnoded_t));
        }
    }
//  else if (noded->sleep_cap)
//  {   //Sleeping node should be added to priority queue
//  }
}


/**
zwdpoll_rm_node_evt_hdlr - Remove node event handler
@param[in]	dpoll_ctx   Polling context
@param[in]	noded	    Node
return
*/
static void zwdpoll_rm_node_evt_hdlr(zwdpoll_ctx_t *dpoll_ctx, zwnoded_p noded)
{
    dpoll_flirs_t   flirs_ent;

    //When node is removed, only the the "nodeid" and "net" members in noded are valid

    //Try to remove from FLIRS queue
    flirs_ent.noded = *noded;
    if (!util_list_rm(NULL, &dpoll_ctx->flirs_hd, (uint8_t *)&flirs_ent, zwdpoll_flirs_node_cmp))
    {
        //Try to remove from always-on queue
        util_list_rm(NULL, &dpoll_ctx->ao_hd, (uint8_t *)noded, zwdpoll_ao_node_cmp);
    }
}


/**
zwdpoll_rm_all_evt_hdlr - Remove all nodes event handler
@param[in]	dpoll_ctx   Polling context
return
*/
static void zwdpoll_rm_all_evt_hdlr(zwdpoll_ctx_t *dpoll_ctx)
{
    //Flush poll queues
    util_list_flush(NULL, &dpoll_ctx->flirs_hd);
    util_list_flush(NULL, &dpoll_ctx->ao_hd);
    util_list_flush(NULL, &dpoll_ctx->priority_hd);

}


/**
zwdpoll_wakeup_evt_hdlr - Wakeup event handler
@param[in]	dpoll_ctx   Polling context
@param[in]	noded	    Node
return
*/
static void zwdpoll_wakeup_evt_hdlr(zwdpoll_ctx_t *dpoll_ctx, zwnoded_p noded)
{
    zwnet_p nw = dpoll_ctx->net;
    int     res;
    int     updt_cache = 0;

    if (!noded->sleep_cap)
    {
        return;
    }

    plt_mtx_lck(nw->mtx);
    if (nw->curr_op == ZWNET_OP_NONE)
    {
        updt_cache = 1;
    }
    else if ((nw->curr_op == ZWNET_OP_NODE_CACHE_UPT)
             && (dpoll_ctx->cur_device_type != DEV_POLL_DEV_SLEEPING))
    {   //Cancel lower priority polling
        zwnet_abort(nw);
        updt_cache = 1;
        debug_zwapi_msg(&nw->plt_ctx, "Wake up, cancelled lower priority polling!");
    }
    else
    {   //Queue it
        res = util_list_find(NULL, dpoll_ctx->priority_hd, (uint8_t *)noded, zwdpoll_ao_node_cmp);
        if (res == 0)
        {  //There isn't any node with the same node id found
            util_list_add(NULL, &dpoll_ctx->priority_hd, (uint8_t *)noded, sizeof(zwnoded_t));
        }
        debug_zwapi_msg(&nw->plt_ctx, "Wake up, queued for polling!");

    }

    if (updt_cache)
    {
        res = zwdpoll_node_updt_sm_start(noded);
        if (res == 0)
        {
            dpoll_ctx->cur_device_type = DEV_POLL_DEV_SLEEPING;
        }
        else
        {
            debug_zwapi_msg(&nw->plt_ctx, "zwdpoll_node_updt_sm_start failed: %d", res);
        }
    }
    plt_mtx_ulck(nw->mtx);
}


/**
zwdpoll_complt_evt_hdlr - Node update cache completion event handler
@param[in]	dpoll_ctx   Polling context
@param[in]	noded	    Node
return
*/
static void zwdpoll_complt_evt_hdlr(zwdpoll_ctx_t *dpoll_ctx, zwnoded_p noded)
{
    zwnet_p         nw = dpoll_ctx->net;
    util_lst_t      *lst_ent;
    zwnoded_p       tmp_noded;
    int             found_cur_ao = 0;

    if (noded->sensor)
    {   //FLIRS device
         dpoll_flirs_t   flirs_ent;

         flirs_ent.noded = *noded;
         //Update next poll time
         flirs_ent.next_poll_tm = dpoll_ctx->tmr_tick + FLIRS_DEV_POLL_TIME;
         //flirs_ent.next_poll_tm = dpoll_ctx->tmr_tick + DEV_POLL_TICK_PER_SEC * 60;//Testing
         util_list_updt(NULL, dpoll_ctx->flirs_hd, (uint8_t *)&flirs_ent, zwdpoll_flirs_node_cmp);
    }
    else if (noded->sleep_cap)
    {   //Sleeping node
        if (dpoll_ctx->cur_device_type != DEV_POLL_DEV_SLEEPING)
        {   //Error, the device may be inserted in the wrong queue. Try to remove it.
            if (util_list_rm(NULL, &dpoll_ctx->ao_hd, (uint8_t *)noded, zwdpoll_ao_node_cmp))
            {
                debug_zwapi_msg(&dpoll_ctx->net->plt_ctx, "Warning: node %u was wrongly inserted to ao queue!", noded->nodeid);
            }
        }
        // Update next always-on device poll time
        dpoll_ctx->next_poll_tm = dpoll_ctx->tmr_tick + AO_DEV_POLL_TIME;

    }
    else
    {   //Always-on device
        plt_mtx_lck(nw->mtx);
        if ((nw->curr_op == ZWNET_OP_NODE_CACHE_UPT)
            && (dpoll_ctx->cur_device_type == DEV_POLL_DEV_AO))
        {   //Another always-on node update cache operation is in progress, ignore this completion event

            plt_mtx_ulck(nw->mtx);
        }
        else
        {
            plt_mtx_ulck(nw->mtx);

            //Point to next entry
            if (dpoll_ctx->ao_hd)
            {
                lst_ent = dpoll_ctx->ao_hd;

                while (lst_ent)
                {
                    tmp_noded = (zwnoded_p)lst_ent->wr_buf;

                    if (tmp_noded->nodeid == dpoll_ctx->cur_ao_nodeid)
                    {
                        found_cur_ao = 1;
                        break;
                    }

                    lst_ent = lst_ent->next;
                }

                if (!found_cur_ao)
                {   //The current always-on device has been removed. Assigned to the first one.
                    lst_ent = dpoll_ctx->ao_hd;
                }

                if (lst_ent->next)
                {
                    lst_ent = lst_ent->next;
                }
                else
                {
                    lst_ent = dpoll_ctx->ao_hd;
                }

                tmp_noded = (zwnoded_p)lst_ent->wr_buf;
                dpoll_ctx->cur_ao_nodeid = tmp_noded->nodeid;

            }
            // Update next poll time
            dpoll_ctx->next_poll_tm = dpoll_ctx->tmr_tick + AO_DEV_POLL_TIME;
        }
    }

    if (zwdpoll_node_cache_updt(dpoll_ctx) != 1)
    {   //No cache update operation being carried out
        dpoll_ctx->cur_device_type = DEV_POLL_DEV_NONE;
    }
}


/**
zwdpoll_evt_msg_hdlr - Process message loop on message arrival
@param[in]	usr_prm	    user parameter supplied during initialization
@param[in]	msg	        message
return      1 to exit message loop; else return 0 to continue the message loop
*/
static int zwdpoll_evt_msg_hdlr(void *usr_prm, void *msg)
{
    zwdpoll_ctx_t    *dpoll_ctx = (zwdpoll_ctx_t *)usr_prm;
    dpoll_evt_msg_t  *evt_msg = (dpoll_evt_msg_t *)msg;

    if (dpoll_ctx->run)
    {
        plt_mtx_lck(dpoll_ctx->poll_mtx);
        switch(evt_msg->type)
        {
            case DEV_POLL_EVT_ADD_NODE:
#ifndef DISABLE_DEV_POLLING
                zwdpoll_add_node_evt_hdlr(dpoll_ctx, &evt_msg->noded);
#endif
                break;

            case DEV_POLL_EVT_RM_NODE:
                zwdpoll_rm_node_evt_hdlr(dpoll_ctx, &evt_msg->noded);
                break;

            case DEV_POLL_EVT_RM_ALL:
                zwdpoll_rm_all_evt_hdlr(dpoll_ctx);
                break;

            case DEV_POLL_EVT_WAKEUP:
                zwdpoll_wakeup_evt_hdlr(dpoll_ctx, &evt_msg->noded);
                break;

            case DEV_POLL_EVT_OP_CMPLT:
                zwdpoll_complt_evt_hdlr(dpoll_ctx, &evt_msg->noded);
                break;
        }
        plt_mtx_ulck(dpoll_ctx->poll_mtx);
    }

    return 0;
}


/**
zwdpoll_init - Initialize the polling facility
@param[in]	dpoll_ctx	    Polling context
@return  0 on success; negative error number on failure
@note  Must call zwdpoll_shutdown followed by zwdpoll_exit to shutdown and clean up the polling facility
*/
int zwdpoll_init(zwdpoll_ctx_t *dpoll_ctx)
{
    int     res;

    dpoll_ctx->run = 1;

    dpoll_ctx->next_poll_tm = AO_DEV_POLL_TIME;

    res = util_msg_loop_init(zwdpoll_evt_msg_hdlr, dpoll_ctx, NULL, 1, &dpoll_ctx->evt_msg_loop_ctx);

    if (res != 0)
    {
        goto l_DEV_POLL_INIT_ERR1;
    }

    if (!plt_mtx_init(&dpoll_ctx->poll_mtx))
    {
        goto l_DEV_POLL_INIT_ERR2;
    }

    if (!plt_sem_init(&dpoll_ctx->tmr_sem))
    {
        goto l_DEV_POLL_INIT_ERR3;
    }

    dpoll_ctx->tick_tmr_ctx = plt_periodic_start(&dpoll_ctx->net->plt_ctx, DEV_POLL_TIMER_TICK,
                                                 zwdpoll_tmr_tick_cb, dpoll_ctx);
    if (!dpoll_ctx->tick_tmr_ctx)
        goto l_DEV_POLL_INIT_ERR4;

    //Start timer check thread
    dpoll_ctx->tmr_chk_thrd_run = 1;
    if (plt_thrd_create(zwdpoll_tmr_chk_thrd, dpoll_ctx) < 0)
    {
        goto l_DEV_POLL_INIT_ERR5;
    }

    return ZW_ERR_NONE;

l_DEV_POLL_INIT_ERR5:
    plt_tmr_stop(&dpoll_ctx->net->plt_ctx, dpoll_ctx->tick_tmr_ctx);
l_DEV_POLL_INIT_ERR4:
    plt_sem_destroy(dpoll_ctx->tmr_sem);
l_DEV_POLL_INIT_ERR3:
    plt_mtx_destroy(dpoll_ctx->poll_mtx);
l_DEV_POLL_INIT_ERR2:
    util_msg_loop_shutdown(dpoll_ctx->evt_msg_loop_ctx, 0);
    util_msg_loop_exit(dpoll_ctx->evt_msg_loop_ctx);
l_DEV_POLL_INIT_ERR1:
    return ZW_ERR_NO_RES;

}


/**
zwdpoll_shutdown - Shutdown the polling facility
@param[in]	dpoll_ctx	    Polling context
@return
*/
void zwdpoll_shutdown(zwdpoll_ctx_t *dpoll_ctx)
{
    dpoll_ctx->run = 0;

    //Stop timer
    plt_tmr_stop(&dpoll_ctx->net->plt_ctx, dpoll_ctx->tick_tmr_ctx);

    //Stop message loop
    util_msg_loop_shutdown(dpoll_ctx->evt_msg_loop_ctx, 0);

    //Stop thread
    dpoll_ctx->tmr_chk_thrd_run = 0;
    plt_sem_post(dpoll_ctx->tmr_sem);

    while (dpoll_ctx->tmr_chk_thrd_sts)
    {
        plt_sleep(100);
    }

    plt_sleep(20);  //delay 20ms to give timer thread enough time to terminate and clean up

}


/**
zwdpoll_exit - Clean up
@param[in]	dpoll_ctx	    Polling context
@return
*/
void zwdpoll_exit(zwdpoll_ctx_t *dpoll_ctx)
{
    util_msg_loop_exit(dpoll_ctx->evt_msg_loop_ctx);

    //Flush poll queues
    util_list_flush(NULL, &dpoll_ctx->flirs_hd);
    util_list_flush(NULL, &dpoll_ctx->ao_hd);
    util_list_flush(NULL, &dpoll_ctx->priority_hd);

    plt_sem_destroy(dpoll_ctx->tmr_sem);
    plt_mtx_destroy(dpoll_ctx->poll_mtx);

}


/**
zwdpoll_add_all - Add all nodes except sleeping nodes for polling
@param[in]	dpoll_ctx   Polling context
@param[in]	noded	    Node
return
*/
void zwdpoll_add_all(zwdpoll_ctx_t *dpoll_ctx)
{
    zwnode_p        node;
    zwnet_p         nw = dpoll_ctx->net;
    dpoll_evt_msg_t msg;

    if (!dpoll_ctx->run)
    {
        return;
    }


    msg.type = DEV_POLL_EVT_ADD_NODE;

    plt_mtx_lck(nw->mtx);

    if ((node = (zwnode_p)nw->ctl.obj.next) != NULL)
    {
        while (node)
        {
            if (node->ep.intf)
            {   //Node has already populated with interfaces
                if (!node->sleep_cap)
                {   //Non-sleeping node
                    if (zwnode_get_desc(node, &msg.noded) == ZW_ERR_NONE)
                    {
                        util_msg_loop_send(dpoll_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(dpoll_evt_msg_t));
                    }
                }
            }
            node = (zwnode_p)node->obj.next;
        }
    }
    plt_mtx_ulck(nw->mtx);

}


/**
zwdpoll_add_node - Add node for polling
@param[in]	dpoll_ctx   Polling context
@param[in]	noded	    Node
return
*/
void zwdpoll_add_node(zwdpoll_ctx_t *dpoll_ctx, zwnoded_p noded)
{
    dpoll_evt_msg_t msg;

    msg.type = DEV_POLL_EVT_ADD_NODE;
    msg.noded = *noded;

    if (dpoll_ctx->run)
    {
        util_msg_loop_send(dpoll_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(dpoll_evt_msg_t));
    }
}


/**
zwdpoll_rm_node - Remove node from polling
@param[in]	dpoll_ctx   Polling context
@param[in]	noded	    Node
return
*/
void zwdpoll_rm_node(zwdpoll_ctx_t *dpoll_ctx, zwnoded_p noded)
{
    dpoll_evt_msg_t msg;

    msg.type = DEV_POLL_EVT_RM_NODE;
    msg.noded = *noded;

    if (dpoll_ctx->run)
    {
        util_msg_loop_send(dpoll_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(dpoll_evt_msg_t));
    }
}


/**
zwdpoll_rm_all - Remove all nodes from polling
@param[in]	dpoll_ctx   Polling context
return
*/
void zwdpoll_rm_all(zwdpoll_ctx_t *dpoll_ctx)
{
    dpoll_evt_msg_t msg;

    msg.type = DEV_POLL_EVT_RM_ALL;

    if (dpoll_ctx->run)
    {
        util_msg_loop_send(dpoll_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(dpoll_evt_msg_t));
    }
}


/**
zwdpoll_on_complt - Callback when node cache update operation has completed
@param[in]	dpoll_ctx   Polling context
@param[in]	noded	    Node
@return
*/
void zwdpoll_on_complt(zwdpoll_ctx_t *dpoll_ctx, zwnoded_p noded)
{
    dpoll_evt_msg_t msg;

    msg.type = DEV_POLL_EVT_OP_CMPLT;
    msg.noded = *noded;

    if (dpoll_ctx->run)
    {
        util_msg_loop_send(dpoll_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(dpoll_evt_msg_t));
    }
}


/**
@}
*/















