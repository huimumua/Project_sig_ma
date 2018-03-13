/**
@file   zip_poll.c - Z-wave High Level API poll facility implementation.

@author David Chow

@version    1.0 26-3-13  Initial release

@copyright © 2014 SIGMA DESIGNS, INC. THIS IS AN UNPUBLISHED WORK PROTECTED BY SIGMA DESIGNS, INC.
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
#include "../include/zip_poll.h"


/**
@defgroup If_Poll Polling Interface APIs
Used to create and delete polling commands to a device
@{
*/

/**
zwpoll_mv_ent_endlist - Move entry to end of list
@param[in]	poll_ctx		Context
@param[in]	ent		        Poll entry
@return
@pre Caller must lock poll_mtx
*/
static void zwpoll_mv_ent_endlist(zwpoll_ctx_t *poll_ctx, poll_q_ent_t *ent)
{
    //There is only 1 entry in the list
    if (poll_ctx->poll_lst_hd == poll_ctx->poll_lst_tail)
    {
        return;
    }

    //The entry is already at the end of list
    if (poll_ctx->poll_lst_tail == ent)
    {
        return;
    }

    //The entry is the head of the list
    if (poll_ctx->poll_lst_hd == ent)
    {
        poll_ctx->poll_lst_hd = ent->next;
        poll_ctx->poll_lst_hd->prev = NULL;
    }
    else
    {
        ent->prev->next = ent->next;
        ent->next->prev = ent->prev;
    }

    //Adjust the former tail
    poll_ctx->poll_lst_tail->next = ent;

    ent->prev = poll_ctx->poll_lst_tail;
    ent->next = NULL;

    //Assign the entry as tail
    poll_ctx->poll_lst_tail = ent;

}


/**
zwpoll_add_ent_endlist - Add entry to end of list
@param[in]	poll_ctx		Context
@param[in]	ent		        New poll entry
@return     1 if the ent is the only entry in the list after adding; else return 0.
@pre Caller must lock poll_mtx
*/
static int zwpoll_add_ent_endlist(zwpoll_ctx_t *poll_ctx, poll_q_ent_t *ent)
{
    //The list is empty
    if (!poll_ctx->poll_lst_hd && !poll_ctx->poll_lst_tail)
    {
        poll_ctx->poll_lst_hd = poll_ctx->poll_lst_tail = ent;
        ent->next = ent->prev = NULL;
        return 1;
    }

    //Adjust the former tail
    poll_ctx->poll_lst_tail->next = ent;

    ent->prev = poll_ctx->poll_lst_tail;
    ent->next = NULL;

    //Assign the entry as tail
    poll_ctx->poll_lst_tail = ent;

    return 0;

}


/**
zwpoll_rm_ent - Remove entry from list
@param[in]	poll_ctx		Context
@param[in]	ent		        Poll entry
@return
@pre Caller must lock poll_mtx
@post ent should not be used as it is freed
*/
static void zwpoll_rm_ent(zwpoll_ctx_t *poll_ctx, poll_q_ent_t *ent)
{
    //There is only 1 entry in the list
    if (poll_ctx->poll_lst_hd == poll_ctx->poll_lst_tail)
    {
        poll_ctx->poll_lst_hd = poll_ctx->poll_lst_tail = NULL;
        free(ent);
        return;
    }

    //The entry is the head of the list
    if (poll_ctx->poll_lst_hd == ent)
    {
        poll_ctx->poll_lst_hd = ent->next;
        if (poll_ctx->poll_lst_hd)
        {
            poll_ctx->poll_lst_hd->prev = NULL;
        }
    }
    //The entry is the tail of the list
    else if (poll_ctx->poll_lst_tail == ent)
    {
        poll_ctx->poll_lst_tail = ent->prev;
        poll_ctx->poll_lst_tail->next = NULL;
    }
    else
    {
        ent->prev->next = ent->next;
        ent->next->prev = ent->prev;
    }

    free(ent);
}


/**
zwpoll_tx_sts_hdlr - handle transmit status
@param[in]	poll_ctx    Polling context
@param[in]	cur_tm      Current time
@param[in]	tx_sts		The transmit complete status
@return
*/
static void zwpoll_tx_sts_hdlr(zwpoll_ctx_t *poll_ctx, uint32_t cur_tm, int8_t tx_sts)
{
    plt_mtx_lck(poll_ctx->poll_mtx);

    if (tx_sts == TRANSMIT_COMPLETE_OK)
    {
        uint32_t    cmd_tm;

        //Calculate command time
        cmd_tm = util_time_diff32(cur_tm, poll_ctx->cur_start_tm);
        if (cmd_tm == 0)
        {
            cmd_tm++;
        }

        //Save the command time
        poll_ctx->cur_cmd_tm = cmd_tm;

        //Re-calculate next poll time
        poll_ctx->next_poll_tm = cur_tm + cmd_tm + MIN_POLL_TIME;

    }
    else
    {
        //Re-calculate next poll time
        poll_ctx->next_poll_tm = cur_tm + MIN_POLL_TIME;
    }

    plt_mtx_ulck(poll_ctx->poll_mtx);
}


/**
zwpoll_beam_tx_cb - send command to node that requires wake up beam (FLIRS) callback function
@param[in]	appl_ctx    The application layer context
@param[in]	tx_sts		The transmit complete status
@param[in]	user_prm    The user specific parameter
@param[in]	node_id     Node id
@param[in]	param       Additional parameters
@return
*/
void zwpoll_beam_tx_cb(appl_layer_ctx_t  *appl_ctx, int8_t tx_sts, void *user_prm, uint8_t node_id, zw_tx_cb_prm_t *param)
{
    zwnet_p         nw = (zwnet_p)appl_ctx->data;
    zwpoll_ctx_t    *poll_ctx = nw->poll_ctx;
    uint32_t        cur_tm;

    cur_tm = poll_ctx->tmr_tick;

    //Call back zwnode_wait_tx_cb
    zwnode_wait_tx_cb(appl_ctx, tx_sts, user_prm, node_id, param);

    zwpoll_tx_sts_hdlr(poll_ctx, cur_tm, tx_sts);

}


/**
zwpoll_tx_cb - send poll command callback function
@param[in]	appl_ctx    The application layer context
@param[in]	tx_sts		The transmit complete status
@param[in]	user_prm    The user specific parameter
@param[in]	node_id     Node id
@param[in]	param       Additional parameters
@return
*/
static void zwpoll_tx_cb(appl_layer_ctx_t  *appl_ctx, int8_t tx_sts, void *user_prm, uint8_t node_id, zw_tx_cb_prm_t *param)
{
    zwnet_p         nw = (zwnet_p)appl_ctx->data;
    zwpoll_ctx_t    *poll_ctx = nw->poll_ctx;
    uint32_t        cur_tm;

    cur_tm = poll_ctx->tmr_tick;

    zwpoll_tx_sts_hdlr(poll_ctx, cur_tm, tx_sts);

}


/**
zwpoll_on_wkup - Callback when a sleeping node sends wake up notification and there are commands in the queue
@param[in]	node	Node
@return
*/
void zwpoll_on_wkup(zwnode_p node)
{
    zwnet_p         nw = node->net;
    poll_evt_msg_t  msg;

    msg.type = POLL_EVT_TYPE_WAKEUP;
    msg.nodeid = node->nodeid;

    util_msg_loop_send(nw->poll_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(poll_evt_msg_t));

}


/**
zwpoll_wkup_chk - Check entries that are waiting for wakeup notification from the node
@param[in]	node_id	Node ID of the sleeping node
@return
*/
static void zwpoll_wkup_chk(zwpoll_ctx_t *poll_ctx, uint8_t node_id)
{
    poll_q_ent_t    *ent;
    poll_q_ent_t    *prev_ent;
    uint32_t        curr_time;

    plt_mtx_lck(poll_ctx->poll_mtx);

    if (poll_ctx->poll_lst_hd == NULL)
    {   //Poll queue is empty
        plt_mtx_ulck(poll_ctx->poll_mtx);
        return;
    }

    curr_time = poll_ctx->tmr_tick;
    ent = poll_ctx->poll_lst_hd;

    while (ent)
    {
        if (ent->active_sts == POLL_STS_ACTIVE)
        {
            if (ent->node_id == node_id)
            {
                if (ent->wr_cmd_q)
                {   //This entry had written a command to the command queue
                    //before this wakeup notification arrived
                    ent->wr_cmd_q = 0;
                    ent->expired = 0;

                    if (ent->poll_cnt == 0)
                    {   //Repetitive polling
                        ent->next_poll_tm = ent->interval + curr_time;
                    }
                    else
                    {   //Non-repetitive polling
                        uint16_t    intv_cnt;
                        uint16_t    last_tot_poll_cnt;  //For checking overflow case

                        intv_cnt = util_time_diff32(curr_time, ent->next_poll_tm)/ent->interval + 1;
                        last_tot_poll_cnt = ent->tot_poll_cnt;
                        ent->tot_poll_cnt += intv_cnt;
                        ent->next_poll_tm = ent->next_poll_tm + (intv_cnt * ent->interval);

                        //Check whether this is the last poll
                        if ((ent->tot_poll_cnt > ent->poll_cnt) || (ent->tot_poll_cnt < last_tot_poll_cnt))
                        {
                            ent->active_sts = POLL_STS_INACTIVE_WAIT_RPT;//not active, waiting for report
                            ent->rpt_tout = POLL_RPT_TIMEOUT + curr_time;
                        }
                    }
                    //Save previous entry since the current entry will be moved to the end of list
                    prev_ent = ent->prev;

                    //Move entry to end of list
                    zwpoll_mv_ent_endlist(poll_ctx, ent);

                    ent = prev_ent;
                }
            }
        }

        //Next
        if (!ent)
        {
            ent = poll_ctx->poll_lst_hd;
        }
        else
        {
            ent = ent->next;
        }
    }
    plt_mtx_ulck(poll_ctx->poll_mtx);

}


/**
zwpoll_same_node_poll - Poll the same node for expired entries
@param[in]	poll_ctx	Context
@return
@pre    Caller must lock the poll_mtx
*/
static void zwpoll_same_node_poll(zwpoll_ctx_t *poll_ctx)
{
    poll_q_ent_t    *ent;
    uint32_t        curr_time;

    if (poll_ctx->poll_lst_hd == NULL)
    {   //Poll queue is empty
        return;
    }

    ent = poll_ctx->poll_lst_hd;

    while (ent)
    {
        if (ent->active_sts == POLL_STS_ACTIVE)
        {
            if (ent->node_id == poll_ctx->cur_node_id)
            {
                if (ent->expired && !ent->sleeping && (ent->poll_seq_num != poll_ctx->poll_seq_num))
                {
                    zwif_exec_ex(&ent->ifd, ent->dat_buf, ent->dat_len,
                                 zwpoll_tx_cb, NULL, ZWIF_OPT_POLL, NULL);

                    //Update poll sequence number to prevent it is polled again whtihin the same node polling cycle
                    ent->poll_seq_num = poll_ctx->poll_seq_num;
                    ent->wait_rpt = 1;

                    curr_time = poll_ctx->tmr_tick;

                    //Save the following for calculation of command time
                    poll_ctx->cur_start_tm = curr_time;
                    poll_ctx->cur_cmd_tm = 0;
                    poll_ctx->cur_cmd_cls = ent->cmd_cls;
                    poll_ctx->cur_rpt = ent->rpt;

                    //Update poll context next polling time
                    poll_ctx->next_poll_tm = curr_time + MIN_POLL_TIME;

                    if (ent->poll_cnt == 0)
                    {   //Repetitive polling
                        ent->next_poll_tm = ent->interval + curr_time;
                    }
                    else
                    {   //Non-repetitive polling
                        uint16_t    intv_cnt;
                        uint16_t    last_tot_poll_cnt;  //For checking overflow case

                        intv_cnt = util_time_diff32(curr_time, ent->next_poll_tm)/ent->interval + 1;
                        last_tot_poll_cnt = ent->tot_poll_cnt;
                        ent->tot_poll_cnt += intv_cnt;
                        ent->next_poll_tm = ent->next_poll_tm + (intv_cnt * ent->interval);

                        //Check whether this is the last poll
                        if ((ent->tot_poll_cnt > ent->poll_cnt) || (ent->tot_poll_cnt < last_tot_poll_cnt))
                        {
                            //debug_zwapi_ts_msg(&poll_ctx->net->plt_ctx, "Expiring entry:%u", ent->handle);
                            ent->active_sts = POLL_STS_INACTIVE_WAIT_RPT;//not active, waiting for report
                            ent->rpt_tout = POLL_RPT_TIMEOUT + curr_time;
                        }
                    }

                    ent->expired = 0;

                    //Move entry to end of list
                    zwpoll_mv_ent_endlist(poll_ctx, ent);

                    return;
                }
            }
        }

        //Next
        ent = ent->next;
    }

    //No polling expired in the same node, next poll will be for the other node
    poll_ctx->poll_seq_num += 2; //Make sure it is always an odd number

}


/**
zwpoll_on_rpt_rcv - Callback when a report is received
@param[in]	intf	Interface
@param[in]	cmd_buf	The report command received
@param[in]	cmd_len	The length of cmd_buf
@return
*/
void zwpoll_on_rpt_rcv(zwif_p intf, uint8_t *cmd_buf, uint16_t cmd_len)
{
    zwnode_p        node = intf->ep->node;
    zwnet_p         nw = node->net;
    zwpoll_ctx_t    *poll_ctx = nw->poll_ctx;
    poll_evt_msg_t  msg;

    //Get command class and the report command
    if (!zwif_cls_cmd_get(cmd_buf, cmd_len, &msg.cls, &msg.rpt))
    {
        return;
    }

    msg.type = POLL_EVT_TYPE_REPORT;
    msg.nodeid = node->nodeid;

    util_msg_loop_send(poll_ctx->evt_msg_loop_ctx, (void *)&msg, sizeof(poll_evt_msg_t));
}


/**
zwpoll_rpt_rcv_chk - Callback when a report is received
@param[in]	poll_ctx	Poll context
@param[in]	msg	        Message
@return
*/
static void zwpoll_rpt_rcv_chk(zwpoll_ctx_t *poll_ctx, poll_evt_msg_t *msg)
{
    poll_q_ent_t    *ent;

    plt_mtx_lck(poll_ctx->poll_mtx);

    if (poll_ctx->poll_lst_hd == NULL)
    {   //Poll queue is empty
        plt_mtx_ulck(poll_ctx->poll_mtx);
        return;
    }

    if ((poll_ctx->cur_node_id == msg->nodeid) && (poll_ctx->cur_cmd_cls == msg->cls)
        && (poll_ctx->cur_rpt == msg->rpt))
    {   //Found matching node and report of the latest report_get polling command

        //Re-calculate next poll time
        poll_ctx->next_poll_tm = poll_ctx->tmr_tick + poll_ctx->cur_cmd_tm + MIN_POLL_TIME;

        //Clear the command class and report
        poll_ctx->cur_cmd_cls = poll_ctx->cur_rpt = 0;
    }

    //Start from end of list
    ent = poll_ctx->poll_lst_tail;

    while (ent)
    {
        if (ent->node_id == msg->nodeid)
        {
            if (ent->wait_rpt)
            {
                if ((ent->cmd_cls == msg->cls) && (ent->rpt == msg->rpt))
                {   //The polling report has arrived
                    ent->wait_rpt = 0;

                    if (ent->active_sts == POLL_STS_INACTIVE_WAIT_RPT)
                    {   //Callback to report poll completion
                        if (ent->cmplt_cb)
                        {
                            poll_cb_msg_t cb_msg;

                            cb_msg.cmplt_cb = ent->cmplt_cb;
                            cb_msg.handle = ent->handle;
                            cb_msg.usr_param = ent->usr_param;
                            cb_msg.usr_token = ent->usr_token;

                            util_msg_loop_send(poll_ctx->cb_msg_loop_ctx, (void *)&cb_msg, sizeof(poll_cb_msg_t));
                        }

                        //Remove the entry
                        zwpoll_rm_ent(poll_ctx, ent);
                    }
                    //Poll for other expired entries in the same node
                    zwpoll_same_node_poll(poll_ctx);
                    break;
                }
            }
        }

        //Next
        ent = ent->prev;
    }
    plt_mtx_ulck(poll_ctx->poll_mtx);
}


/**
zwpoll_chk_all - Check all entries and mark expiry flag if poll timer has expired
@param[in]	poll_ctx		Context
@return
@pre Caller must lock poll_mtx
*/
static void zwpoll_chk_all(zwpoll_ctx_t *poll_ctx)
{
    poll_q_ent_t *ent;
    poll_q_ent_t *prev_ent;      //Pointer to previous list entry

    if (!poll_ctx->poll_lst_hd)
    {
        return;
    }

    ent = poll_ctx->poll_lst_hd;

    while (ent)
    {
        if (!ent->expired)
        {
            if (ent->active_sts == POLL_STS_ACTIVE)
            {
                //Check whether the next poll time has expired
                if (util_tmr_exp_chk32(poll_ctx->tmr_tick, ent->next_poll_tm))
                {   //Expired, mark it
                    ent->expired = 1;
                }
            }
            else    //active_sts == POLL_STS_INACTIVE_WAIT_RPT
            {
                //Check whether the report timeout occured
                if (util_tmr_exp_chk32(poll_ctx->tmr_tick, ent->rpt_tout))
                {   //Callback and remove the entry

                    if (ent->cmplt_cb)
                    {
                        poll_cb_msg_t cb_msg;

                        cb_msg.cmplt_cb = ent->cmplt_cb;
                        cb_msg.handle = ent->handle;
                        cb_msg.usr_param = ent->usr_param;
                        cb_msg.usr_token = ent->usr_token;

                        util_msg_loop_send(poll_ctx->cb_msg_loop_ctx, (void *)&cb_msg, sizeof(poll_cb_msg_t));
                    }

                    //Save previous entry since the current entry will be removed
                    prev_ent = ent->prev;
                    //Remove the entry
                    zwpoll_rm_ent(poll_ctx, ent);
                    ent = prev_ent;

                }
            }
        }

        //Next
        if (!ent)
        {
            ent = poll_ctx->poll_lst_hd;
        }
        else
        {
            ent = ent->next;
        }
    }
}


/**
zwpoll_chk_sleeping_node - Check sleeping node entries
@param[in]	poll_ctx		Context
@return
@pre Caller must lock poll_mtx
*/
static void zwpoll_chk_sleeping_node(zwpoll_ctx_t *poll_ctx)
{
    poll_q_ent_t *ent;
    poll_q_ent_t *prev_ent;      //Pointer to previous list entry

    if (!poll_ctx->poll_lst_hd)
    {
        return;
    }

    ent = poll_ctx->poll_lst_hd;

    while (ent)
    {
        if (ent->sleeping && ent->poll_cnt)
        {
            //Check whether the report timeout occured
            if (util_tmr_exp_chk32(poll_ctx->tmr_tick, ent->slp_node_tout))
            {   //Callback and remove the entry

                if (ent->cmplt_cb)
                {
                    poll_cb_msg_t cb_msg;

                    cb_msg.cmplt_cb = ent->cmplt_cb;
                    cb_msg.handle = ent->handle;
                    cb_msg.usr_param = ent->usr_param;
                    cb_msg.usr_token = ent->usr_token;

                    util_msg_loop_send(poll_ctx->cb_msg_loop_ctx, (void *)&cb_msg, sizeof(poll_cb_msg_t));
                }

                //Save previous entry since the current entry will be moved to the end of list
                prev_ent = ent->prev;
                //Remove the entry
                zwpoll_rm_ent(poll_ctx, ent);
                ent = prev_ent;

            }
        }

        //Next
        if (!ent)
        {
            ent = poll_ctx->poll_lst_hd;
        }
        else
        {
            ent = ent->next;
        }
    }
}


/**
zwpoll_handle_expiry - Handle poll timer expiry of a non-sleeping node
@param[in]	poll_ctx		Context
@param[in]	ent		        Poll entry that has expired
@return
@pre Caller must lock poll_mtx
*/
static void zwpoll_handle_expiry(zwpoll_ctx_t *poll_ctx, poll_q_ent_t *ent)
{
    uint32_t    curr_time;

    poll_ctx->poll_seq_num += 2; //Make sure it is always an odd number
    poll_ctx->cur_node_id = ent->node_id;

    zwif_exec_ex(&ent->ifd, ent->dat_buf, ent->dat_len,
                 zwpoll_tx_cb, NULL, ZWIF_OPT_POLL, NULL);

    curr_time = poll_ctx->tmr_tick;

    //Save the following for calculation of command time
    poll_ctx->cur_start_tm = curr_time;
    poll_ctx->cur_cmd_tm = 0;
    poll_ctx->cur_cmd_cls = ent->cmd_cls;
    poll_ctx->cur_rpt = ent->rpt;

    //Update poll sequence number to prevent it is polled again whtihin the same node polling cycle
    ent->poll_seq_num = poll_ctx->poll_seq_num;
    ent->wait_rpt = 1;

    //Update poll context next polling time
    poll_ctx->next_poll_tm = curr_time + MIN_POLL_TIME;

    if (ent->poll_cnt == 0)
    {   //Repetitive polling
        ent->next_poll_tm = ent->interval + curr_time;
    }
    else
    {   //Non-repetitive polling
        uint16_t    intv_cnt;
        uint16_t    last_tot_poll_cnt;  //For checking overflow case

        intv_cnt = util_time_diff32(curr_time, ent->next_poll_tm)/ent->interval + 1;
        last_tot_poll_cnt = ent->tot_poll_cnt;
        ent->tot_poll_cnt += intv_cnt;
        ent->next_poll_tm = ent->next_poll_tm + (intv_cnt * ent->interval);

        //Check whether this is the last poll
        if ((ent->tot_poll_cnt > ent->poll_cnt) || (ent->tot_poll_cnt < last_tot_poll_cnt))
        {
            //debug_zwapi_ts_msg(&poll_ctx->net->plt_ctx, "zwpoll_handle_expiry: Expiring entry:%u", ent->handle);
            ent->active_sts = POLL_STS_INACTIVE_WAIT_RPT;//not active, waiting for report
            ent->rpt_tout = POLL_RPT_TIMEOUT + curr_time;
        }
    }

    ent->expired = 0;

    //Move entry to end of list
    zwpoll_mv_ent_endlist(poll_ctx, ent);
}


/**
zwpoll_handle_expiry_sleeping_node - Handle poll timer expiry of a sleeping node
@param[in]	poll_ctx		Context
@param[in]	ent		        Poll entry that has expired
@return
@pre Caller must lock poll_mtx
*/
static void zwpoll_handle_expiry_sleeping_node(zwpoll_ctx_t *poll_ctx, poll_q_ent_t *ent)
{
    //Send to the command queue. No callback required.
    zwif_exec_ex(&ent->ifd, ent->dat_buf, ent->dat_len,
                 NULL, NULL, ZWIF_OPT_POLL, NULL);

    ent->wait_rpt = 1;

    //Move entry to end of list
    zwpoll_mv_ent_endlist(poll_ctx, ent);
}


/**
zwpoll_tmr_chk_thrd - thread to process timer tick event
@param[in]	data		Context
@return
*/
static void zwpoll_tmr_chk_thrd(void *data)
{
    zwpoll_ctx_t    *poll_ctx = (zwpoll_ctx_t *)data;
    poll_q_ent_t    *prev_ent;      //Pointer to previous list entry
    poll_q_ent_t    *ent;
    poll_q_ent_t    *saved_tail;    //Saved poll list tail
    int             last_poll_ent;
    uint32_t        next_exp_chk_tm;
    uint32_t        prev_poll_tm;

    poll_ctx->tmr_chk_thrd_sts = 1;

    next_exp_chk_tm = poll_ctx->tmr_tick + CHECK_EXPIRY_INTERVAL;

    while (1)
    {
        //Wait for timer tick event
        plt_sem_wait(poll_ctx->tmr_sem);

        //Check whether to exit the thread
        if (poll_ctx->tmr_chk_thrd_run == 0)
        {
            poll_ctx->tmr_chk_thrd_sts = 0;
            return;
        }

        plt_mtx_lck(poll_ctx->poll_mtx);

        //Check whether it's time to check entries for expiry
        if (util_tmr_exp_chk32(poll_ctx->tmr_tick, next_exp_chk_tm))
        {
            zwpoll_chk_all(poll_ctx);
            zwpoll_chk_sleeping_node(poll_ctx);
            //Recalculate next checking time
            next_exp_chk_tm = poll_ctx->tmr_tick + CHECK_EXPIRY_INTERVAL;
        }

        //Check whether the next poll time has expired
        if (util_tmr_exp_chk32(poll_ctx->tmr_tick, poll_ctx->next_poll_tm) == 0)
        {   //Not expire yet, continue to wait
            plt_mtx_ulck(poll_ctx->poll_mtx);
            continue;
        }

        if (poll_ctx->poll_lst_hd == NULL)
        {   //Poll queue is empty
            poll_ctx->next_poll_tm = poll_ctx->tmr_tick + MIN_POLL_TIME;

            plt_mtx_ulck(poll_ctx->poll_mtx);
            continue;
        }

        ent = poll_ctx->poll_lst_hd;
        saved_tail = poll_ctx->poll_lst_tail;
        last_poll_ent = 0;

        //Save the poll time
        prev_poll_tm = poll_ctx->next_poll_tm;

        while (ent)
        {
            if (ent == saved_tail)
            {   //This is the last entry to check for polling
                last_poll_ent = 1;
            }

            if (ent->active_sts == POLL_STS_ACTIVE)
            {
                if (ent->expired)
                {
                    if (ent->sleeping)
                    {
                        if (!ent->wr_cmd_q)
                        {
                            ent->wr_cmd_q = 1;

                            //Save previous entry since the current entry will be moved to the end of list
                            prev_ent = ent->prev;
                            zwpoll_handle_expiry_sleeping_node(poll_ctx, ent);
                            ent = prev_ent;
                        }
                    }
                    else
                    {
                        zwpoll_handle_expiry(poll_ctx, ent);
                        break;
                    }
                }
            }

            if (last_poll_ent)
            {
                break;
            }

            //Next
            if (!ent)
            {
                ent = poll_ctx->poll_lst_hd;
            }
            else
            {
                ent = ent->next;
            }
        }

        //Check whether poll time has been refreshed
        if (prev_poll_tm == poll_ctx->next_poll_tm)
        {   //Recalculate poll time using shorter time since the minimum poll time has been met.
            poll_ctx->next_poll_tm = poll_ctx->tmr_tick + CHECK_EXPIRY_INTERVAL;
        }

        plt_mtx_ulck(poll_ctx->poll_mtx);
    }
}


/**
zwpoll_tmr_tick_cb - Timer tick timeout callback
@param[in] data     Pointer to poll context
@return
*/
static void    zwpoll_tmr_tick_cb(void *data)
{
    zwpoll_ctx_t   *poll_ctx = (zwpoll_ctx_t *)data;

    //Increment timer tick
    poll_ctx->tmr_tick++;

    //Send timer tick event
    if (poll_ctx->run)
    {
        plt_sem_post(poll_ctx->tmr_sem);
    }
}


/**
zwpoll_rm_all - remove all polling requests
@param[in]	poll_ctx    Context
@return
*/
static void zwpoll_rm_all(zwpoll_ctx_t *poll_ctx)
{
    poll_q_ent_t *ent;
    poll_q_ent_t *rm_ent;      //Entry to be removed

    plt_mtx_lck(poll_ctx->poll_mtx);
    if (!poll_ctx->poll_lst_hd)
    {
        plt_mtx_ulck(poll_ctx->poll_mtx);
        return;
    }

    ent = poll_ctx->poll_lst_hd;

    while (ent)
    {
        rm_ent = ent;
        ent = ent->next;
        free(rm_ent);
    }

    poll_ctx->poll_lst_hd = poll_ctx->poll_lst_tail = NULL;

    plt_mtx_ulck(poll_ctx->poll_mtx);

}


/**
zwpoll_rm - remove a polling request
@param[in]	net	        network
@param[in]	handle	    handle of the polling request to remove
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/
int zwpoll_rm(zwnet_p net, uint16_t handle)
{
    zwpoll_ctx_t *poll_ctx = net->poll_ctx;
    poll_q_ent_t *ent;

    if (!net->poll_enable)
    {
        return ZW_ERR_UNSUPPORTED;
    }

    plt_mtx_lck(poll_ctx->poll_mtx);
    if (!poll_ctx->poll_lst_hd)
    {
        plt_mtx_ulck(poll_ctx->poll_mtx);
        return ZW_ERR_FAILED;
    }

    ent = poll_ctx->poll_lst_hd;

    while (ent)
    {
        if (ent->handle == handle)
        {
            zwpoll_rm_ent(poll_ctx, ent);
            plt_mtx_ulck(poll_ctx->poll_mtx);
            return ZW_ERR_NONE;
        }

        ent = ent->next;
    }

    plt_mtx_ulck(poll_ctx->poll_mtx);
    return ZW_ERR_FAILED;
}


/**
zwpoll_rm_mul - remove multiple polling requests
@param[in]	net	        network
@param[in]	usr_token	usr_token of the polling requests to remove
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/
int zwpoll_rm_mul(zwnet_p net, uint32_t usr_token)
{
    zwpoll_ctx_t *poll_ctx = net->poll_ctx;
    poll_q_ent_t *ent;
    poll_q_ent_t *rm_ent;      //Entry to be removed

    if (!net->poll_enable)
    {
        return ZW_ERR_UNSUPPORTED;
    }

    plt_mtx_lck(poll_ctx->poll_mtx);
    if (!poll_ctx->poll_lst_hd)
    {
        plt_mtx_ulck(poll_ctx->poll_mtx);
        return ZW_ERR_FAILED;
    }

    ent = poll_ctx->poll_lst_hd;

    while (ent)
    {
        rm_ent = ent;
        ent = ent->next;

        if (rm_ent->usr_token == usr_token)
        {
            zwpoll_rm_ent(poll_ctx, rm_ent);
        }
    }

    plt_mtx_ulck(poll_ctx->poll_mtx);
    return ZW_ERR_NONE;
}


/**
zwpoll_node_rm - remove polling requests that belong to the specified node
@param[in]	net	        network
@param[in]	node_id	    node id
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/
int zwpoll_node_rm(zwnet_p net, uint8_t node_id)
{
    zwpoll_ctx_t *poll_ctx = net->poll_ctx;
    poll_q_ent_t *ent;
    poll_q_ent_t *rm_ent;      //Entry to be removed

    if (!net->poll_enable)
    {
        return ZW_ERR_UNSUPPORTED;
    }

    plt_mtx_lck(poll_ctx->poll_mtx);
    if (!poll_ctx->poll_lst_hd)
    {
        plt_mtx_ulck(poll_ctx->poll_mtx);
        return ZW_ERR_FAILED;
    }

    ent = poll_ctx->poll_lst_hd;

    while (ent)
    {
        rm_ent = ent;
        ent = ent->next;

        if (rm_ent->node_id == node_id)
        {
            zwpoll_rm_ent(poll_ctx, rm_ent);
        }
    }

    plt_mtx_ulck(poll_ctx->poll_mtx);
    return ZW_ERR_NONE;
}


/**
zwpoll_add - add a polling request into polling queue
@param[in]	net	        network
@param[in]	poll_ent	polling request entry (allocated by caller). All unused fields must be zeroed.
@param[out]	handle	    Polling request handle
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
@post       If return error, caller must free poll_ent
*/
int zwpoll_add(zwnet_p net, poll_q_ent_t *poll_ent, uint16_t *handle)
{
    zwpoll_ctx_t *poll_ctx = net->poll_ctx;
    int          result;
    int          first_ent;


    result = zwnode_get_rpt(poll_ent->dat_buf, poll_ent->dat_len, &poll_ent->cmd_cls, &poll_ent->rpt);

    if (!result)
    {
        return ZW_ERR_RPT_NOT_FOUND;
    }

    if (poll_ent->poll_cnt > 0)
    {   //Non-repetitive poll
        uint32_t    poll_intv;  //poll interval in seconds
        poll_ent->tot_poll_cnt = 1;//include the one which will be executed shortly after this

        //Adjust poll count for interval of less than 4 seconds to take into consideration of SSL delay
        //and coarse timer resolution
        poll_intv = poll_ent->interval/POLL_TICK_PER_SEC;
        if (poll_intv < 4)
        {
            switch (poll_intv)
            {
                case 1:
                    poll_ent->poll_cnt += 3;
                    break;

                case 2:
                    poll_ent->poll_cnt += 2;
                    break;

                case 3:
                    poll_ent->poll_cnt++;
                    break;
            }
        }
    }

    poll_ent->active_sts = (poll_ent->poll_cnt == 1)? POLL_STS_INACTIVE_WAIT_RPT : POLL_STS_ACTIVE;

    plt_mtx_lck(poll_ctx->poll_mtx);

    *handle = poll_ent->handle = ++poll_ctx->handle_gen;//TODO: make sure it's unique

    first_ent = zwpoll_add_ent_endlist(poll_ctx, poll_ent);

    //Execute the get_report command. No callback required as this is not considered a poll.
    zwif_exec_ex(&poll_ent->ifd, poll_ent->dat_buf, poll_ent->dat_len,
                 NULL, NULL, ZWIF_OPT_POLL, NULL);

    //Important: start timer only after the sending of first command as it may take some time
    //           to decrypt SSL Z/IP command by ZIPR
    poll_ent->next_poll_tm = poll_ent->interval + poll_ctx->tmr_tick;

    //debug_zwapi_ts_msg(&net->plt_ctx, "Poll entry added");

    if (first_ent)
    {   //This is the only entry in the poll list
        poll_ctx->next_poll_tm = poll_ctx->tmr_tick + MIN_POLL_TIME;

        //Save the following for calculation of command time
        poll_ctx->cur_start_tm = poll_ctx->tmr_tick;
        poll_ctx->cur_cmd_tm = 0;
        poll_ctx->cur_cmd_cls = poll_ent->cmd_cls;
        poll_ctx->cur_rpt = poll_ent->rpt;
    }

    if (poll_ent->sleeping)
    {
        poll_ent->wr_cmd_q = 1;

        if (poll_ent->poll_cnt == 1)
        {
            poll_ent->slp_node_tout = poll_ctx->tmr_tick + poll_ent->interval;
        }
        else if (poll_ent->poll_cnt > 1)
        {
            poll_ent->slp_node_tout = poll_ctx->tmr_tick + (poll_ent->interval * (poll_ent->poll_cnt - 1));
        }
    }

    poll_ent->rpt_tout = POLL_RPT_TIMEOUT + poll_ctx->tmr_tick;
    poll_ent->wait_rpt = 0;

    plt_mtx_ulck(poll_ctx->poll_mtx);

    return ZW_ERR_NONE;
}


/**
zwpoll_evt_msg_hdlr - Process message loop on message arrival
@param[in]	usr_prm	    user parameter supplied during initialization
@param[in]	msg	        message
return      1 to exit message loop; else return 0 to continue the message loop
*/
static int zwpoll_evt_msg_hdlr(void *usr_prm, void *msg)
{
    zwpoll_ctx_t    *poll_ctx = (zwpoll_ctx_t *)usr_prm;
    poll_evt_msg_t  *evt_msg = (poll_evt_msg_t *)msg;

    if (poll_ctx->run)
    {
        if (evt_msg->type == POLL_EVT_TYPE_WAKEUP)
        {
            zwpoll_wkup_chk(poll_ctx, evt_msg->nodeid);
        }
        else if (evt_msg->type == POLL_EVT_TYPE_REPORT)
        {
            zwpoll_rpt_rcv_chk(poll_ctx, evt_msg);
        }
    }

    return 0;
}


/**
zwpoll_cb_msg_hdlr - Process message loop on message arrives
@param[in]	usr_prm	    user parameter supplied during initialization
@param[in]	msg	        message
return      1 to exit message loop; else return 0 to continue the message loop
*/
static int zwpoll_cb_msg_hdlr(void *usr_prm, void *msg)
{
    zwpoll_ctx_t    *poll_ctx = (zwpoll_ctx_t *)usr_prm;
    poll_cb_msg_t   *cb_msg = (poll_cb_msg_t *)msg;

    if (poll_ctx->run)
    {
        cb_msg->cmplt_cb(poll_ctx->net, cb_msg->handle, cb_msg->usr_token, cb_msg->usr_param);
    }

    return 0;

}


/**
zwpoll_init - Initialize the polling facility
@param[in]	poll_ctx	    Polling context
@return  0 on success; negative error number on failure
@note  Must call zwpoll_shutdown followed by zwpoll_exit to shutdown and clean up the polling facility
*/
int zwpoll_init(zwpoll_ctx_t *poll_ctx)
{
    int     res;

    poll_ctx->run = 1;
    poll_ctx->tmr_tick = 0;
    poll_ctx->next_poll_tm = MIN_POLL_TIME;
    poll_ctx->poll_lst_hd = NULL;
    poll_ctx->poll_lst_tail = NULL;
    poll_ctx->handle_gen = 0;
    poll_ctx->poll_seq_num = 1; //Make sure it is always an odd number
    poll_ctx->cur_node_id = 0;

    res = util_msg_loop_init(zwpoll_cb_msg_hdlr, poll_ctx, NULL, 3, &poll_ctx->cb_msg_loop_ctx);

    if (res != 0)
    {
        return res;
    }

    res = util_msg_loop_init(zwpoll_evt_msg_hdlr, poll_ctx, NULL, 4, &poll_ctx->evt_msg_loop_ctx);

    if (res != 0)
    {
        goto l_POLL_INIT_ERROR1;
    }

    if (!plt_mtx_init(&poll_ctx->poll_mtx))
        goto l_POLL_INIT_ERROR2;

    if (!plt_sem_init(&poll_ctx->tmr_sem))
    {
        goto l_POLL_INIT_ERROR3;
    }
    poll_ctx->tick_tmr_ctx = plt_periodic_start(&poll_ctx->net->plt_ctx, POLL_TIMER_TICK, zwpoll_tmr_tick_cb, poll_ctx);
    if (!poll_ctx->tick_tmr_ctx)
        goto l_POLL_INIT_ERROR4;

    //Start timer check thread
    poll_ctx->tmr_chk_thrd_run = 1;
    if (plt_thrd_create(zwpoll_tmr_chk_thrd, poll_ctx) < 0)
    {
        goto l_POLL_INIT_ERROR5;
    }

    return ZW_ERR_NONE;

l_POLL_INIT_ERROR5:
    plt_tmr_stop(&poll_ctx->net->plt_ctx, poll_ctx->tick_tmr_ctx);
l_POLL_INIT_ERROR4:
    plt_sem_destroy(poll_ctx->tmr_sem);
l_POLL_INIT_ERROR3:
    plt_mtx_destroy(poll_ctx->poll_mtx);
l_POLL_INIT_ERROR2:
    util_msg_loop_shutdown(poll_ctx->evt_msg_loop_ctx, 0);
    util_msg_loop_exit(poll_ctx->evt_msg_loop_ctx);
l_POLL_INIT_ERROR1:
    util_msg_loop_shutdown(poll_ctx->cb_msg_loop_ctx, 0);
    util_msg_loop_exit(poll_ctx->cb_msg_loop_ctx);
    return ZW_ERR_NO_RES;

}


/**
zwpoll_shutdown - Shutdown the polling facility
@param[in]	poll_ctx	    Polling context
@return
*/
void zwpoll_shutdown(zwpoll_ctx_t *poll_ctx)
{
    poll_ctx->run = 0;
    util_msg_loop_shutdown(poll_ctx->cb_msg_loop_ctx, 0);
    util_msg_loop_shutdown(poll_ctx->evt_msg_loop_ctx, 0);

    //Stop timer
    plt_tmr_stop(&poll_ctx->net->plt_ctx, poll_ctx->tick_tmr_ctx);

    //Stop thread
    poll_ctx->tmr_chk_thrd_run = 0;
    plt_sem_post(poll_ctx->tmr_sem);

    while (poll_ctx->tmr_chk_thrd_sts)
    {
        plt_sleep(100);
    }

    plt_sleep(20);  //delay 20ms to give timer thread enough time to terminate and clean up

}


/**
zwpoll_exit - Clean up
@param[in]	poll_ctx	    Polling context
@return
*/
void zwpoll_exit(zwpoll_ctx_t *poll_ctx)
{
    util_msg_loop_exit(poll_ctx->cb_msg_loop_ctx);
    util_msg_loop_exit(poll_ctx->evt_msg_loop_ctx);

    //Flush the poll queue
    zwpoll_rm_all(poll_ctx);

    plt_sem_destroy(poll_ctx->tmr_sem);
    plt_mtx_destroy(poll_ctx->poll_mtx);
}


/**
@}
*/















