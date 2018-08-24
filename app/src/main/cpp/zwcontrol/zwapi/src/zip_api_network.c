/**
@file   zip_api_network.c - Z/IP High Level API network implementation.

@author David Chow

@version    1.0 7-6-11  Initial release

@copyright � 2014 SIGMA DESIGNS, INC. THIS IS AN UNPUBLISHED WORK PROTECTED BY SIGMA DESIGNS, INC.
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
#if !defined(_WINDOWS) && !defined(WIN32)
#include <unistd.h>
#endif
#ifdef TCP_PORTAL
#include <signal.h>
#endif
#include "../include/zip_api_pte.h"
#include "../include/zip_api_util.h"
#include "../include/zip_poll.h"
#include "../include/zip_set_poll.h"
#include "../include/zip_dev_poll.h"
#include "../include/zip_supervision.h"
#include "../include/zip_wkup_poll.h"
#include "../include/zip_node_sts.h"
#include "../include/zip_network_save.h"
#include "../include/zip_event_gen.h"
#include "../include/zip_intf_sm.h"
#ifdef ZIP_V2
#include "../include/zip_mdns.h"
#endif
#include "../include/zwave/ZW_controller_api.h"

//#define TEST_NET_OP     ///< Testing network operation (for debugging purposes)
//#define HARDCODE_GW_VER 0x0250      // Hardcode gateway version to get around with version 2.20 backward compatibility issue
//#define ADD_BIN_SWITCH_INTF //For testing

/** Incoming message type bitmask */
#define ZWNET_MSG_TYPE_SECURE       0x01    ///< Secure message
#define ZWNET_MSG_TYPE_CRC16        0x02    ///< CRC-16 encapsulated message
#define ZWNET_MSG_TYPE_MULTICAST    0x04    ///< Z-wave multicast message

/** Application command handler parameters */
typedef struct
{
    appl_layer_ctx_t    *appl_ctx;      ///< Application layer context
    frm_ep_t            *ep_addr;       ///< Endpoint address
    zwnet_p             nw;             ///< Network
    uint8_t             src_node;       ///< Source node id
    uint8_t             msg_type;       ///< Incoming message type, see ZWNET_MSG_TYPE_XXX
    uint8_t             tot_mul_cmd;    ///< Total number of commands encapsulated in a multi-command encapsulated packet
    uint8_t             mul_cmd_num;    ///< The N th (start from 1)command of the multi-command encapsulated packet
    uint16_t            encap_fmt_valid;///< Flag to indicate whether the field encap_fmt is valid
    uint16_t            encap_fmt;      ///< Incoming message encapsulation format
    uint8_t             spvsn_encap;    ///< Flag to indicate whether the command is encap in supervision get command
    uint8_t             spvsn_sess_id;  ///< supervision get command session id
}
appl_cmd_prm_t;

/** Saved application command */
typedef struct  _appl_cmd_saved
{
    uint16_t        cmd_len;                    /**< Z-wave command length*/
    uint8_t         cmd_buf[MAX_ZWAVE_PKT_SIZE];/**< Z-wave command buffer*/
    frm_ep_t        ep_address;                 /**< Endpoint address*/
    appl_cmd_prm_t  cmd_param;                  /**< Command parameters to appl_cmd_hdlr*/
}
appl_cmd_saved_t;


uint8_t       zwif_tx_sts_get(int8_t tx_sts);
int           zwnet_node_info_sm(zwnet_p   nw, zwnet_ni_evt_t  evt, uint8_t *data);
zwnet_mul_cmd_state_t zwnet_mul_cmd_sm(zwnet_p nw, zwnet_mul_cmd_evt_t evt, uint8_t *data, uint8_t len);
zwthrmo_op_sta_t      *zwif_thrmo_op_sta_cache_find(zwthrmo_op_sta_t *op_sta_cch, uint8_t cnt, uint8_t log_type);
void          zwnet_sm_node_propty_updt(zwnode_p node);
int           zwnet_sm_ver_rpt_hdlr(zwnet_p nw, zwif_p *ver_intf, void *ver_rpt_cb, uint16_t cmd_cls, uint8_t cmd_cls_ver,
                                    int tx_opt, int skip_xtra_cc_basic);
int           zwnet_ni_sec_updt(zwep_p ep, uint16_t *sec_cls, uint8_t len);
int           zwnet_1_ep_info_sm(zwnet_p nw, zwnet_1_ep_evt_t evt, uint8_t *data);
int           nhchk_sm(zwnet_p nw, zwnet_nh_chk_evt_t evt, uint8_t *data);
int           zwnet_plst_sm(zwnet_p nw, zwnet_plst_evt_t evt, uint8_t *data);
void          zwnet_plst_shutdown(zwnet_p nw);

static int    zwnet_updt_sm(zwnet_p nw, zwnet_nu_evt_t  evt, uint8_t *data);
static int    zwnet_init_sm(zwnet_p nw, zwnet_init_evt_t  evt, uint8_t *data);
static int    zwnet_rst_sm(zwnet_p nw, zwnet_rst_evt_t  evt, uint8_t *data);
static int    zwnet_add_rplc_node_sm(zwnet_p nw, zwnet_add_evt_t evt, uint8_t *data);
static int    zwnet_join_other_nw_sm(zwnet_p nw, zwnet_join_nw_evt_t evt, uint8_t *data);
static void   zwnet_sm_nu_stop(zwnet_p nw);
static void   zwnet_ctlr_id_addr_cb(appl_layer_ctx_t *appl_ctx, nd_advt_rpt_t *rpt);
static int    zwnet_n2ip_sm(zwnet_p nw, n2ip_evt_t evt, uint8_t *data);
static void   zwnet_n2ip_lst_cb(zwnet_p nw, int status, void **prm);
static int    zwnet_ctlr_info_sm(zwnet_p nw, zwnet_ctlr_evt_t evt, uint8_t *data);
static void   zwnet_failed_id_rm_cb(appl_layer_ctx_t *appl_ctx, uint8_t sts, uint8_t node_id);
static int    zwnet_nw_change_sm(zwnet_p nw, zwnet_nc_evt_t evt, uint8_t *data);
static void   zwnet_unsolic_node_list_cb(appl_layer_ctx_t *appl_ctx, uint8_t sts,
                                         uint8_t node_list_ctlr_id, uint8_t *node_list, uint8_t nl_len);
static void   zwnet_unsolic_node_add_cb(appl_layer_ctx_t *appl_ctx, appl_node_add_cb_prm_t *cb_prm);
static void   zwnet_unsolic_ss_started_cb(appl_layer_ctx_t *appl_ctx, zw_dsk_t *dsk);
static void   zwnet_unsolic_inif_cb(appl_layer_ctx_t *appl_ctx, zw_dsk_t *dsk);
static void   zwnet_unsolic_pl_cb(appl_layer_ctx_t *appl_ctx, zw_dsk_t *dsk, pl_info_t *info, uint8_t info_cnt);
static void   zwnet_node_rm_all(zwnet_p nw);
static int    handle_association(uint16_t cmd_len, uint8_t *cmd_buf, appl_cmd_prm_t *prm);
static void   zwnet_node_add_rplc_hdlr(appl_layer_ctx_t *appl_ctx, appl_node_add_cb_prm_t *cb_prm);

/**
@defgroup Net Network APIs
A Z-Wave Network is seen through a local controller node which can manage
control and data in the mesh network.
@{
*/

static const sup_cmd_cls_t    local_sup_cmd_class[] =
{
    {COMMAND_CLASS_MULTI_CMD, 1, BITMASK_CMD_CLS_INSECURE}
    ,{COMMAND_CLASS_ASSOCIATION, 2, BITMASK_CMD_CLS_INSECURE}
    ,{COMMAND_CLASS_ASSOCIATION_GRP_INFO, 3, BITMASK_CMD_CLS_INSECURE}
    ,{COMMAND_CLASS_DEVICE_RESET_LOCALLY, 1, BITMASK_CMD_CLS_INSECURE}
//  ,{COMMAND_CLASS_CONFIGURATION, 2, BITMASK_CMD_CLS_SECURE}//Testing
//#ifdef CRC16_ENCAP
//    ,{COMMAND_CLASS_CRC_16_ENCAP, 1, BITMASK_CMD_CLS_INSECURE}
//#endif
};

static const cmd_get_resp_t gw_cmd_get_resp_tbl[] =
{   //Format: command class, get command, report command
    {COMMAND_CLASS_BASIC, BASIC_GET, BASIC_REPORT},
    {COMMAND_CLASS_ZWAVEPLUS_INFO_V2, ZWAVEPLUS_INFO_GET, ZWAVEPLUS_INFO_REPORT},
	{COMMAND_CLASS_POWERLEVEL, POWERLEVEL_GET, POWERLEVEL_REPORT},
    {COMMAND_CLASS_POWERLEVEL, POWERLEVEL_TEST_NODE_GET, POWERLEVEL_TEST_NODE_REPORT},
    {COMMAND_CLASS_MANUFACTURER_SPECIFIC, MANUFACTURER_SPECIFIC_GET, MANUFACTURER_SPECIFIC_REPORT},
    {COMMAND_CLASS_MANUFACTURER_SPECIFIC, DEVICE_SPECIFIC_GET_V2, DEVICE_SPECIFIC_REPORT_V2},
    {COMMAND_CLASS_FIRMWARE_UPDATE_MD, FIRMWARE_MD_GET, FIRMWARE_MD_REPORT},
    {COMMAND_CLASS_SUPERVISION, SUPERVISION_GET, SUPERVISION_REPORT}
    //{COMMAND_CLASS_FIRMWARE_UPDATE_MD, FIRMWARE_UPDATE_MD_REQUEST_GET, FIRMWARE_UPDATE_MD_REQUEST_REPORT},
    //{COMMAND_CLASS_FIRMWARE_UPDATE_MD, FIRMWARE_UPDATE_MD_GET, FIRMWARE_UPDATE_MD_REPORT}//For controller, it won't send FIRMWARE_UPDATE_MD_GET

};

/**
zwnet_get_rpt - Check the input command to find the corresponding REPORT
@param[in]	cmd		    Command
@param[in]  len	        The length of command in bytes
@param[out] cmd_cls	    The command class of the report
@param[out]	rpt	        The report
@return     1 if the corresponding report is found; otherwise return 0
*/
static int zwnet_get_rpt(uint8_t* cmd, uint8_t len, uint16_t *cmd_cls, uint8_t *rpt)
{
    int         i;
    int         total_ent;  //total entries in the table
    uint16_t    cls;
    uint8_t     get_cmd;


    if (!zwif_cls_cmd_get(cmd, len, &cls, &get_cmd))
    {
        return 0;
    }

    total_ent = sizeof(gw_cmd_get_resp_tbl)/ sizeof(cmd_get_resp_t);

    for (i=0; i<total_ent; i++)
    {
        if ((gw_cmd_get_resp_tbl[i].cls == cls) && (gw_cmd_get_resp_tbl[i].get_cmd == get_cmd))
        {   //Found
            *cmd_cls = cls;
            *rpt = gw_cmd_get_resp_tbl[i].rpt;
            return 1;
        }
    }
    return 0;
}


static const cmd_set_ent_t gw_set_cmd_tbl[] =
{   //Format: command class, set command
    {COMMAND_CLASS_BASIC, BASIC_SET},
	{COMMAND_CLASS_POWERLEVEL, POWERLEVEL_SET},
    {COMMAND_CLASS_POWERLEVEL, POWERLEVEL_TEST_NODE_SET}
};

/**
zwnet_is_set_cmd - Check the input command to check whether it is a set command
@param[in]	cmd		    Command
@param[in]  len	        The length of command in bytes
@param[out] cmd_cls	    The command class of the set command
@return     1 if the buffer contains set command; otherwise return 0
*/
static int zwnet_is_set_cmd(uint8_t* cmd, uint8_t len, uint16_t *cmd_cls)
{
    int         i;
    int         total_ent;  //total entries in the table
    uint16_t    cls;
    uint8_t     set_cmd;


    if (!zwif_cls_cmd_get(cmd, len, &cls, &set_cmd))
    {
        return 0;
    }

    total_ent = sizeof(gw_set_cmd_tbl)/ sizeof(cmd_set_ent_t);

    for (i=0; i<total_ent; i++)
    {
        if ((gw_set_cmd_tbl[i].cls == cls) && (gw_set_cmd_tbl[i].set_cmd == set_cmd))
        {   //Found
            *cmd_cls = cls;
            return 1;
        }
    }
    return 0;
}


static const cmd_set_ent_t lc_set_cmd_tbl[] =
{   //Format: command class, set command
	{COMMAND_CLASS_ASSOCIATION, ASSOCIATION_SET},
    {COMMAND_CLASS_ASSOCIATION, ASSOCIATION_REMOVE}
};

/**
zwnet_is_local_set_cmd - Check the input command to check whether it is a local set command
@param[in]	cmd		    Command
@param[in]  len	        The length of command in bytes
@param[out] cmd_cls	    The command class of the set command
@return     1 if the buffer contains set command; otherwise return 0
*/
static int zwnet_is_local_set_cmd(uint8_t* cmd, uint8_t len, uint16_t *cmd_cls)
{
    int         i;
    int         total_ent;  //total entries in the table
    uint16_t    cls;
    uint8_t     set_cmd;


    if (!zwif_cls_cmd_get(cmd, len, &cls, &set_cmd))
    {
        return 0;
    }

    total_ent = sizeof(lc_set_cmd_tbl)/ sizeof(cmd_set_ent_t);

    for (i=0; i<total_ent; i++)
    {
        if ((lc_set_cmd_tbl[i].cls == cls) && (lc_set_cmd_tbl[i].set_cmd == set_cmd))
        {   //Found
            *cmd_cls = cls;
            return 1;
        }
    }
    return 0;
}


/**
zwnet_exec_req_free - free exec request user data
@param[in]	exec_req exec request
@return
@post  Caller MUST exit thread immediately after calling this function.
*/
static void zwnet_exec_req_free(zwnet_exec_req_t *exec_req)
{
    if (exec_req->action == EXEC_ACT_ADD_NODE_STS_CB)
    {
        zwnet_exec_req_addS2_t  *add_req;
        appl_node_info_t        *node_info;

        add_req = (zwnet_exec_req_addS2_t *)exec_req->user_dat;

        if (add_req)
        {
            //Free allocated memory
            node_info = &add_req->add_dat.node_info;

            if (node_info->cmd_cnt)
            {
                free(node_info->cmd_cls);
            }

            if (node_info->cmd_cnt_sec)
            {
                free(node_info->cmd_cls_sec);
            }

            free(add_req);
        }
    }
    else if (exec_req->action == EXEC_ACT_WKUP_NO_MORE_INFO)
    {
        zwnet_exec_wunmi_t  *wunmi_ctx = (zwnet_exec_wunmi_t *)exec_req->user_dat;
        int                 wait_tmr_thd = 0;

        if (wunmi_ctx)
        {
            plt_mtx_lck(wunmi_ctx->nw->mtx);
            if (wunmi_ctx->tmr_ctx)
            {
                if(!plt_tmr_stop(&wunmi_ctx->nw->plt_ctx, wunmi_ctx->tmr_ctx))
                {   //Timer thread created on timeout. Wait for the timer thread to exit
                    wait_tmr_thd = 1;
                }
            }
            plt_mtx_ulck(wunmi_ctx->nw->mtx);

            if (wait_tmr_thd)
            {
                plt_sleep(1000);

                plt_mtx_lck(wunmi_ctx->nw->mtx);
                if (wunmi_ctx->tmr_ctx)
                {
                    //Some unknown error. Don't free
                    plt_mtx_ulck(wunmi_ctx->nw->mtx);
                    return;
                }
                plt_mtx_ulck(wunmi_ctx->nw->mtx);
            }

            free(wunmi_ctx);
        }
    }
    else if (exec_req->action == EXEC_ACT_RPLC_NODE_STS_CB)
    {
        if (exec_req->user_dat)
        {
            free(exec_req->user_dat);
        }
    }

}


/**
zwnet_clear_all - clear all network entries and sub-system states
@param[in]	nw network
@return
*/
static void zwnet_clear_all(zwnet_p nw)
{
    //Remove all the old nodes
    zwnet_node_rm_all(nw);

    //Remove all the old pollings
    zwdpoll_rm_all(nw->dpoll_ctx);
    zwspoll_rm_all(nw->spoll_ctx);
    zwspv_rm_all(nw->spv_ctx);
    if (nw->multi_client)
    {
        wkuppoll_rm_all(nw->wupoll_ctx);
    }
    zwevtg_rm_all(nw->evtg_ctx);
    nodests_rm_all(nw->nsts_ctx);

    //Remove all Z/IP frame sequence numbers
    appl_seq_num_rm_by_nodeid(&nw->appl_ctx, 0xFF);

    //Initialize controller S2 keys to all keys
    nw->ctl.s2_keys_valid = 1;
    nw->ctl.s2_grnt_keys = SEC_KEY_ALL_S2_SO;

    //Clear association group member
    nw->stAGIData.pAGIGroupList->stNodeEpInfo.byNodeID = 0;

    //Remove network info persistent storage
    if (nw->node_info_file)
    {
#if defined(_WINDOWS) || defined(WIN32)
        _unlink(nw->node_info_file);
#else
        unlink(nw->node_info_file);
#endif
        free(nw->node_info_file);
        nw->node_info_file = NULL;
    }
}


/**
zwnet_free - free network context if it is the last one to use it
@param[in]	net network
@return
@post  Caller MUST exit thread immediately after calling this function.
*/
static void zwnet_free(zwnet_p net)
{
    int do_cleanup = 0;

    plt_mtx_lck(net->mtx);
    if (net->ctx_ref_cnt > 1)
    {   //Others still using it
        net->ctx_ref_cnt--;
    }
    else
    {
        net->ctx_ref_cnt = 0;
        do_cleanup = 1;
    }
    plt_mtx_ulck(net->mtx);

    //Free resources
    if (do_cleanup)
    {
        zwnet_mcmd_sm_req_t     *mul_cmd_sm_req;
        zwnet_exec_req_t        *exec_req;
        util_lst_t              *req_lst;

        while ((req_lst = util_list_get(net->nw_exec_mtx, &net->nw_exec_req_hd)) != NULL)
        {
            exec_req = (zwnet_exec_req_t *)req_lst->wr_buf;

            zwnet_exec_req_free(exec_req);

            free(req_lst);
        }

        while ((req_lst = util_list_get(net->mul_cmd_sm_mtx, &net->mul_cmd_sm_hd)) != NULL)
        {
            mul_cmd_sm_req = (zwnet_mcmd_sm_req_t *)req_lst->wr_buf;

            util_list_flush(NULL, &mul_cmd_sm_req->mul_cmd_hd);

            free(req_lst);
        }

        zwnet_node_rm_all(net);

        if (net->poll_enable)
        {
            zwpoll_exit(net->poll_ctx);
        }

        if (net->spoll_ctx)
        {
            zwspoll_exit(net->spoll_ctx);
        }

        if (net->spv_ctx)
        {
            zwspv_exit(net->spv_ctx);
        }

        if (net->dpoll_ctx)
        {
            zwdpoll_exit(net->dpoll_ctx);
        }

        if (net->wupoll_ctx)
        {
            wkuppoll_exit(net->wupoll_ctx);
        }

        if (net->evtg_ctx)
        {
            zwevtg_exit(net->evtg_ctx);
        }

        if (net->nsts_ctx)
        {
            nodests_exit(net->nsts_ctx);
        }

        if (net->zwsave_ctx)
        {
            zwsave_exit(net->zwsave_ctx);
        }

        if (net->ifsm_ctx)
        {
            ifsm_ctx_exit(net->ifsm_ctx);
        }

        while (net->stAGIData.pAGIGroupList != NULL)
        {
            AGI_Group *pGroup = net->stAGIData.pAGIGroupList;
            net->stAGIData.pAGIGroupList = net->stAGIData.pAGIGroupList->pNext;
            free(pGroup);
            pGroup = NULL;
        }

        plt_sem_destroy(net->wait_q_sem);
        plt_sem_destroy(net->cmd_q_sem);
        plt_sem_destroy(net->nw_exec_sem);
        plt_sem_destroy(net->cb_sem);
        plt_mtx_destroy(net->cmd_q_mtx);
        plt_mtx_destroy(net->nw_exec_mtx);
        plt_mtx_destroy(net->cch_set_ctx.mtx);
        plt_mtx_destroy(net->cb_mtx);
        plt_mtx_destroy(net->mul_cmd_sm_mtx);
        plt_mtx_destroy(net->mtx);

        plt_exit(&net->plt_ctx);

    #ifdef SUPPORT_SIMPLE_AV_CONTROL
        free(net->av_sup_bitmask);
    #endif
        zwdev_cfg_free(net->dev_cfg_rec_buf.dev_cfg_records, net->dev_cfg_rec_buf.dev_cfg_cnt);
        free(net->sup_cmd_cls);
        free(net->poll_ctx);
        free(net->spoll_ctx);
        free(net->spv_ctx);
        free(net->dpoll_ctx);
        free(net->wupoll_ctx);
        free(net->gw_sup_cmd_cls);
        free(net->pref_dir);
        free(net->net_info_dir);
        free(net->node_info_file);
        free(net->zwsave_ctx);
        free(net->sec2_add_prm.dsk);
        free(net->evtg_ctx);
        free(net->nsts_ctx);
        free(net->inif_dsk);
        free(net->cfg_name);
        free(net->cfg_info);
        if (net->pl_ctx.pl_list_cnt)
        {
            free(net->pl_ctx.pl_list);
        }
        if (net->appl_cmd_cnt)
        {
            free(net->appl_cmd_saved);
        }
        free(net);
    }
}


/**
zwnet_notify - Create a network operation callback request to the callback thread
@param[in]	nw      Network
@param[in]	op		Network operation ZWNET_OP_XXX
@param[in]	sts		Status of current operation
@param[in]	info	Additional information for the specified op and sts; NULL if there is no additional info
@return
*/
void zwnet_notify(zwnet_p nw, uint8_t op, uint16_t sts, zwnet_sts_t *info)
{
    zwnet_cb_req_t  cb_req = {0};

    if (nw->init.notify)
    {
        cb_req.type = CB_TYPE_NET_OP;
        cb_req.cb = nw->init.notify;
        cb_req.param.net_op.user = nw->init.user;
        cb_req.param.net_op.op = op;
        cb_req.param.net_op.sts = sts;
        if (info)
        {
            cb_req.param.net_op.sts_info_valid = 1;
            cb_req.param.net_op.sts_info = *info;
        }

        util_list_add(nw->cb_mtx, &nw->cb_req_hd,
                      (uint8_t *)&cb_req, sizeof(zwnet_cb_req_t));
        plt_sem_post(nw->cb_sem);
    }
}


/**
zwnet_node_cb - Create a node status callback request to the callback thread
@param[in]	nw      Network
@param[in]	node_id	Newly added, removed or updated node id
@param[in]	mode	ZWNET_NODE_XXX
@return
*/
void zwnet_node_cb(zwnet_p nw, uint8_t node_id, int mode)
{
    zwnet_cb_req_t  cb_req;

    if (nw->init.node && nw->init_done)
    {
        cb_req.type = CB_TYPE_NODE;
        cb_req.cb = nw->init.node;
        cb_req.param.node.user = nw->init.user;
        cb_req.param.node.node_id = node_id;
        cb_req.param.node.mode = mode;
        ALOGI("zwnet_node_cb, node_id:%d, mode:%d",node_id, mode);

        util_list_add(nw->cb_mtx, &nw->cb_req_hd,
                      (uint8_t *)&cb_req, sizeof(zwnet_cb_req_t));
        plt_sem_post(nw->cb_sem);
    }
}


/**
zwnet_node_sts_cb - Create a node alive status callback request to the callback thread
@param[in]	appl_ctx    The application layer context
@param[in]	node_id	    Node
@param[in]	sts		    Status (APPL_NODE_STS_XXX)
@return
*/
static void zwnet_node_sts_cb(appl_layer_ctx_t *appl_ctx, uint8_t node_id, uint8_t sts)
{
    zwnet_p nw = (zwnet_p)appl_ctx->data;

    if (nw && nw->init.node)
    {
        ALOGI("call zwnet_node_cb, node_id:%d, sts:%d",node_id, sts);
        zwnet_node_cb(nw, node_id, (sts + ZWNET_NODE_STATUS_ALIVE));
    }
}


/**
zwnet_node_list_update - Update the network structure.
@param[in]	nw		    Network
@param[in]	node_list   Bitmask of nodes currently stored in the EEPROM
@param[in]	nl_len      Length of node_list in bytes
@param[in]	notify_user Flag to determine whether to notify user application on add/delete node
@param[out]	new_node_list   Buffer to store the newly added node ids (optional)
@param[out]	new_nl_len      Number of new node ids in new_node_list (optional)
@return		ZW_ERR_xxx.
*/
static int zwnet_node_list_update(zwnet_p nw, uint8_t *node_list, uint8_t nl_len, uint8_t notify_user,
                                  uint8_t *new_node_list, uint8_t *new_nl_len)
{
    unsigned    i;
    unsigned    total_nodes;
    uint8_t     num_new_nodes = 0;
    zwnode_p    node;
    zwnode_p    nxt_node;

    //Init output
    if (new_nl_len)
    {
        *new_nl_len = 0;
    }

    if (!node_list)
    {
        return ZW_ERR_VALUE;
    }

    plt_mtx_lck(nw->mtx);

    total_nodes = nl_len * 8;

    for (i = 0; i < total_nodes; i++)
    {
        if (((node_list[i>>3]) >> (i & 7)) & 0x01)
        {
            node = zwnode_find(&nw->ctl, i+1);

            //Check whether the node has already existed in the network structure
            if (!node)
            {   //Not found. Create a new node
                node = (zwnode_p)calloc(1, sizeof(zwnode_t));
                if (!node)
                {
                    plt_mtx_ulck(nw->mtx);
                    return ZW_ERR_MEMORY;
                }

                //Init non-zero value
                node->wkup_intv = -1;

                //Init all the back links
                node->net = nw;
                node->ep.node = node;

                node->nodeid = (i+1);

                if (new_node_list)
                {
                    new_node_list[num_new_nodes++] = i + 1;
                }

                //Init end point
                node->ep.epid = VIRTUAL_EP_ID;
                node->ep.intf = NULL;

                zwobj_add(&nw->ctl.obj.next, &node->obj);

                //Notify user application on the added node
                if (notify_user)
                {
                    zwnet_node_cb(nw, node->nodeid, ZWNET_NODE_ADDED);
                }
            }
        }
    }

    //Remove nodes that are not in the list
    if ((node = (zwnode_p)nw->ctl.obj.next) != NULL)
    {
        while (node)
        {
            //Save the next node
            nxt_node = (zwnode_p)node->obj.next;

            if ((node->nodeid > 0) && (node->nodeid <= total_nodes))
            {
                i = node->nodeid - 1;
                if ((((node_list[i>>3]) >> (i & 7)) & 0x01) == 0)
                {   //Not in the list

                    //Notify user application on the removed node
                    if (notify_user)
                    {
                        zwnet_node_cb(nw, node->nodeid, ZWNET_NODE_REMOVED);
                    }
                    //Remove the node
                    zip_node_ipaddr_rm(&nw->appl_ctx, node->nodeid);
                    zwnode_rm(nw, node->nodeid);
                }
            }
            node = nxt_node;
        }
    }

    plt_mtx_ulck(nw->mtx);

    if (new_nl_len)
    {
        *new_nl_len = num_new_nodes;
    }

    return ZW_ERR_NONE;

}


/**
zwnet_zw_role_get - Get the controller Z-wave role
@param[in]	ctl_role        Controller role, ZWNET_CTLR_ROLE_XXX
@param[in]	nlist_ctl_id    Node list controller id
@param[in]	ctl_id          Controller id
@return     controller Z-wave role (ZW_ROLE_XXX)
*/
static uint8_t zwnet_zw_role_get(uint8_t ctl_role, uint8_t nlist_ctl_id, uint8_t ctl_id)
{
    /**********************************************************************************
    ------------------------------------------------------------------
    | Role      | Command Classes    | Nodelist Report               |
    ------------------------------------------------------------------
    | SIS       | NM inclusion       | NODE_LIST_REPORT_LATEST_LIST  |
    |           |                    | NlistCtlrId == GwNodeID       |
    ------------------------------------------------------------------
    | Inclusion | NM inclusion       | NODE_LIST_REPORT_LATEST_LIST* |
    |           |                    | NlistCtlrId == SUCNodeID      |
    ------------------------------------------------------------------
    | Primary   | NM inclusion       | NODE_LIST_REPORT_LATEST_LIST  |
    |           | NM Primary         | NlistCtlrId == GwNodeID       |
    ------------------------------------------------------------------
    | Secondary |                    | NODE_LIST_REPORT_NO_GUARANTEE |
    |           |                    | NlistCtlrId == 0              |
    ------------------------------------------------------------------

    * If the is no connection to the SUC the Node List report will show
      NODE_LIST_REPORT_NO_GUARANTEE in this case.

    How to end up in each role:

    SIS:
        - GW set_default
        - GW is excluded from network
        - GW was included Into a non sis network and got the SIS role from assigned after inclusion.
    Inclusion:
        - GW is included into a SIS network.
    Primary:
        - GW is included into a non-SIS network, as secondary, and some does a controller change and make the GW primary.
        - GW uses a non-SIS capable SerialAPI, this applies for 3xx series chips, which we don't support.
    Secondary:
        - GW is included into a non-SIS network
        - GW was primary but have performed a controller change to another controller.
    ***********************************************************************************/

    //Check for secondary controller
    if ((ctl_role & (ZWNET_CTLR_ROLE_INCL | ZWNET_CTLR_ROLE_PRI)) == 0)
    {
        return ZW_ROLE_SECONDARY;
    }

    if (ctl_role & ZWNET_CTLR_ROLE_INCL)
    {   //Check for primary controller
        if (ctl_role & ZWNET_CTLR_ROLE_PRI)
        {
            if (nlist_ctl_id == ctl_id)
            {
                return ZW_ROLE_PRIMARY;
            }
            else
            {
                return ZW_ROLE_UNKNOWN;
            }
        }

        //Check for SIS and Inclusion controller
        if (nlist_ctl_id == ctl_id)
        {
            return ZW_ROLE_SIS;
        }
        else
        {
            return ZW_ROLE_INCLUSION;
        }
    }

    return ZW_ROLE_UNKNOWN;
}


/**
zwnet_node_list_get_cb - Get node list callback
@param[in]	appl_ctx            The application layer context
@param[in]	sts                 Status of previous call to "get node list"
@param[in]	node_list_ctlr_id   The node id of the controller which keeps the latest updated node list
@param[in]	node_list           Bitmask of nodes. The first bit in the bitmap represents node ID 1;
                                the last bit represents node ID 232
@param[in]	nl_len              Length of node_list in bytes
@return		ZW_ERR_xxx.
*/
static void zwnet_node_list_get_cb(appl_layer_ctx_t *appl_ctx, uint8_t sts,
                                   uint8_t node_list_ctlr_id, uint8_t *node_list, uint8_t nl_len)
{
    const char *status_msg[] = { "OK", "No guarantee that returned node list is latest update",
                                 "unknown"
                               };
    zwnet_t *nw = (zwnet_t *)appl_ctx->data;
    int     result;
    int     status;
    int     tot_nodes = 0;  //total number of nodes in the network (including Z/IP gateway attached controller)

    if (nl_len > 29)
    {   //Maximum node list length is 29 bytes
        nl_len = 29;
    }

    status = (sts > 1)? 2 : sts;


    nw->nlist_ctl_id = node_list_ctlr_id;

    //Get and save controller z-wave role
    nw->zwave_role = zwnet_zw_role_get(nw->ctl_role, nw->nlist_ctl_id, nw->ctl.nodeid);

    debug_zwapi_msg(&nw->plt_ctx, "Get node list status:%u(%s), suc:%u, zw_role:%u",
                    sts, status_msg[status], node_list_ctlr_id, nw->zwave_role);

    //Check for SIS role
    if (nw->zwave_role == ZW_ROLE_SIS)
    {
        if (nw->curr_op == ZWNET_OP_INITIALIZE)
        {   //Set sis status to handled
            nw->nc_sm_ctx.sis_status = 2;
        }
    }
    else
    {   //Not SIS
        if (nw->curr_op == ZWNET_OP_RESET || nw->curr_op == ZWNET_OP_INITIATE)
        {   //Set sis status to unhandled
            nw->nc_sm_ctx.sis_status = 0;
        }
    }


    if (node_list && (nl_len >= 29))
    {
        char        num_str[256];
        unsigned    i, j;

        debug_zwapi_msg(&nw->plt_ctx, "node id:");

        num_str[0] = '\0';
        for (i = j = 0; i < 232; i++)
        {
            if (((node_list[i>>3]) >> (i & 7)) & 0x01)
            {
                tot_nodes++;
                util_num_string_add(num_str, 256, (unsigned)(i+1));
                if (j++ > 30)
                {
                    debug_zwapi_msg(&nw->plt_ctx, "%s", num_str);
                    j = 0;
                    num_str[0] = '\0';
                }
            }
        }
        debug_zwapi_msg(&nw->plt_ctx, "%s", num_str);
    }

    if (tot_nodes == 1)
    {   //New network
        if (nw->ctl_cap & ZWNET_CTLR_CAP_S2)
        {
            //Initialize controller S2 keys to all keys
            nw->ctl.s2_keys_valid = 1;
            nw->ctl.s2_grnt_keys = SEC_KEY_ALL_S2_SO;

            //Clear and set controller security property
            nw->ctl.propty &= ~(NODE_PROPTY_SECURE_CAP_S0 | NODE_PROPTY_SECURE_CAP_S2 | NODE_PROPTY_ADD_SECURE | NODE_PROPTY_ADD_INSECURE);
            zwnode_secure_propty_set(&nw->ctl);
        }
    }

    result = zwnet_node_list_update(nw, node_list, nl_len,
                                    (nw->curr_op == ZWNET_OP_INITIALIZE || nw->curr_op == ZWNET_OP_INITIATE || nw->curr_op == ZWNET_OP_RESET)? 0 : 1,
                                    NULL, NULL);
    if (result != 0)
    {
        debug_zwapi_msg(&nw->plt_ctx, "zwnet_node_list_update with error:%d", result);
    }

    //Call state-machines
    plt_mtx_lck(nw->mtx);
    zwnet_init_sm(nw, EVT_INI_NODE_LIST, NULL);
    zwnet_updt_sm(nw, EVT_NU_NODE_LIST, NULL);
    zwnet_rst_sm(nw, EVT_RST_NODE_LIST, NULL);
    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_cmd_cls_find - Find a command class in the given command class list.
@param[in]	cc_lst	Command class list
@param[in]	cc      Command class to be searched
@param[in]	cc_cnt	Number of command classes in cc_list
@return		Pointer to the found command class; else NULL.
*/
uint16_t *zwnet_cmd_cls_find(const uint16_t *cc_lst, uint16_t cc, unsigned cc_cnt)
{
    while (cc_cnt-- > 0)
    {
        if (*cc_lst == cc)
        {
            return (uint16_t *)cc_lst;
        }
        cc_lst++;
    }

    return NULL;
}


/**
zwnet_cmd_cls_dedup - Remove duplicate command class in the command class list.
@param[in,out]	cmd_cls_lst	    Command class list
@param[in]	    cmd_cnt	        Number of command classes in command class list
@return		New number of command classes in the command class list
*/
uint8_t zwnet_cmd_cls_dedup(uint16_t *cmd_cls_lst, uint8_t cmd_cnt)
{
    int         i;
    uint16_t *  new_cmd_cls_lst;
    uint8_t     new_cmd_cnt;

    if (cmd_cnt == 0)
    {
        return 0;
    }

    new_cmd_cls_lst = (uint16_t *)malloc(cmd_cnt * sizeof(uint16_t));

    if (!new_cmd_cls_lst)
    {
        return cmd_cnt;
    }

    new_cmd_cnt = 0;
    for (i=0; i<cmd_cnt; i++)
    {
        if (zwnet_cmd_cls_find(new_cmd_cls_lst, cmd_cls_lst[i], new_cmd_cnt) == NULL)
        {   //Add to the new list if the command class is not found in it
            new_cmd_cls_lst[new_cmd_cnt++] = cmd_cls_lst[i];
        }
    }

    //Copy back to the original buffer of the deduplicated command class list
    memcpy(cmd_cls_lst, new_cmd_cls_lst, new_cmd_cnt * sizeof(uint16_t));

    free(new_cmd_cls_lst);

    return new_cmd_cnt;

}


/**
zwnet_cmd_cls_add - Add a command class to the command class list.
@param[in]	new_cc      New command class to be added to the command class list
@param[in]	src_cc_lst	Source command class list
@param[in]	src_cc_cnt	Number of command classes in source command class list
@param[out]	dst_cc_lst	Destination command class list
@param[out]	dst_cc_cnt	Number of command classes in destination command class list
@return		ZW_ERR_xxx.
@post       The caller must free the allocated memory returned by dst_cc_lst if the new_cc is added. This can be
            checked by comparing dst_cc_lst and src_cc_lst.
*/
int zwnet_cmd_cls_add(uint16_t new_cc, const uint16_t *src_cc_lst, uint8_t src_cc_cnt,
                      uint16_t **dst_cc_lst, uint8_t *dst_cc_cnt)
{
    //Check whether the command class list contains new_cc
    if ((src_cc_cnt > 0)
        && (zwnet_cmd_cls_find(src_cc_lst, new_cc, src_cc_cnt) == NULL))
    {
        //Add new_cc into the command class list
        *dst_cc_lst = (uint16_t *)malloc((src_cc_cnt + 1) * sizeof(uint16_t));

        if (!*dst_cc_lst)
        {
            return ZW_ERR_MEMORY;
        }

        //Copy old command class list to new command class list with added new_cc
        *dst_cc_lst[0] = new_cc;
        memcpy(*dst_cc_lst + 1, src_cc_lst, src_cc_cnt * sizeof(uint16_t));

        //Adjust the new command class length
        *dst_cc_cnt = src_cc_cnt + 1;
    }
    else
    {   //Found new_cc or no command classes info.
        //Return the original command class list
        *dst_cc_lst = (uint16_t *)src_cc_lst;
        *dst_cc_cnt = src_cc_cnt;
    }
    return ZW_ERR_NONE;

}


/**
zwnet_node_secure_propty_set - Set node security properties
@param[in]	node		    Node
@param[in]	cmd_cls         Command classes
@param[in]	cmd_cls_cnt     Total number of command classes
@param[in]	cmd_cnt_sec     Total number of security supported command classes
@return		Non-zero on set successfully; else return zero
*/
int zwnet_node_secure_propty_set(zwnode_p node, uint16_t *cmd_cls, uint8_t cmd_cls_cnt, uint8_t cmd_cnt_sec)
{
    uint8_t secure_cap = 0;

    if (cmd_cls_cnt == 0)
    {   //Workaround for Z/IP gateway version 2.25 where node add status contains no command class.
        //Do not set if there is no unsecure command class.
        return 0;
    }

    if (zwnet_cmd_cls_find(cmd_cls, COMMAND_CLASS_SECURITY, cmd_cls_cnt))
    {
        secure_cap |= NODE_PROPTY_SECURE_CAP_S0;
    }
    if (zwnet_cmd_cls_find(cmd_cls, COMMAND_CLASS_SECURITY_2, cmd_cls_cnt))
    {
        secure_cap |= NODE_PROPTY_SECURE_CAP_S2;
    }

    //Check whether node security property has been set
    if ((node->propty & (NODE_PROPTY_ADD_SECURE | NODE_PROPTY_ADD_INSECURE)) == 0)
    {   //Not set yet

        //Grant keys have higher priority
        if (node->s2_keys_valid)
        {
            node->propty |= (node->s2_grnt_keys & SEC_KEY_ALL_S2_SO)? NODE_PROPTY_ADD_SECURE : NODE_PROPTY_ADD_INSECURE;
        }
        else
        {
            node->propty |= (cmd_cnt_sec > 0)? NODE_PROPTY_ADD_SECURE : NODE_PROPTY_ADD_INSECURE;
        }
        node->propty |= secure_cap;
        return 1;
    }
    else if (node->propty & NODE_PROPTY_ADD_INSECURE)
    {   //Insecure node. Try to find any indication of security included

        //Grant keys have higher priority
        if (node->s2_keys_valid)
        {
            if (node->s2_grnt_keys & SEC_KEY_ALL_S2_SO)
            {
                node->propty &= ~NODE_PROPTY_ADD_INSECURE;
                node->propty |= NODE_PROPTY_ADD_SECURE;
            }
        }
        else
        {
            if (cmd_cnt_sec > 0)
            {   //Found a secure interface, set it to secure
                node->propty &= ~NODE_PROPTY_ADD_INSECURE;
                node->propty |= NODE_PROPTY_ADD_SECURE;
            }
        }

        node->propty |= secure_cap;
        return 1;
    }
    return 0;
}


/**
zwnet_node_info_update - Update or create a new node in the network structure
@param[in]	nw		        Network
@param[in]	node_info       Node information
@param[in]	s2_node_info    Optional node information specific to security 2
@return		ZW_ERR_xxx.
*/
int zwnet_node_info_update(zwnet_p nw, appl_node_info_t *node_info, sec2_node_info_t *s2_node_info)
{
    unsigned    i;
    int         node_has_changed;
    int         result;
    int         node_removed = 0;   //Flag to indicate whether an old node has been removed
    zwnode_p    node;
    zwif_p      intf;
    uint16_t    *new_cmd_cls;
    uint16_t    tmout;
    uint8_t     propty;
    uint8_t     new_cmd_cnt;
    uint8_t		s2_grnt_keys;
    uint8_t		s2_keys_valid;
    uint8_t     old_node_propty = 0;

#ifdef ADD_BIN_SWITCH_INTF
    uint16_t    *bin_cmd_cls;
    uint8_t     bin_cmd_cnt;

    result = zwnet_cmd_cls_add(COMMAND_CLASS_SWITCH_BINARY, node_info->cmd_cls, node_info->cmd_cnt,
                               &bin_cmd_cls, &bin_cmd_cnt);
    if (result < 0)
    {
        return result;
    }

    result = zwnet_cmd_cls_add(COMMAND_CLASS_BASIC, bin_cmd_cls, bin_cmd_cnt,
                               &new_cmd_cls, &new_cmd_cnt);
    free(bin_cmd_cls);
    if (result < 0)
    {
        return result;
    }
#else
    result = zwnet_cmd_cls_add(COMMAND_CLASS_BASIC, node_info->cmd_cls, node_info->cmd_cnt,
                               &new_cmd_cls, &new_cmd_cnt);
    if (result < 0)
    {
        return result;
    }
#endif

    //Remove any duplicate command classes
    new_cmd_cnt = zwnet_cmd_cls_dedup(new_cmd_cls, new_cmd_cnt);

    node_has_changed = 0;
    plt_mtx_lck(nw->mtx);

    //Update node alive status
    //appl_updt_node_sts(&nw->appl_ctx, node_info->node_id, APPL_NODE_STS_UP, 1);//Node info cached report is unreliable to conclude the node is UP.

    node = zwnode_find(&nw->ctl, node_info->node_id);
    if (node)
    {
        ALOGI("This node has already added into network, now update node info.");
        node->isNew = 0;
        node->listen = node_info->listen;//update listening flag as old node information in persistent storage doesn't store this variable

        //Existing node found, check whether there is any changes
        if (new_cmd_cnt == 0)
        {   //No device & command classes info to compare,
            //assume no changes

            //Since Z/IP gateway couldn't get the node info from the
            //device, the node should be down/sleeping.
            appl_updt_node_sts(&nw->appl_ctx, node_info->node_id, APPL_NODE_STS_DOWN, 1);

            plt_mtx_ulck(nw->mtx);
            return ZW_ERR_NONE;

        }

        //Check basic, generic, specific device classes
        if (node->basic != node_info->basic)
        {
            node_has_changed = 1;
        }

        if (node->ep.generic != node_info->gen)
        {
            node_has_changed = 1;
        }

        if (node->ep.specific != node_info->spec)
        {
            node_has_changed = 1;
        }

        if (!node_has_changed)
        {   //Check command classes
            for (i = 0; i < new_cmd_cnt; i++)
            {
                if (zwif_find_cls(node->ep.intf, new_cmd_cls[i]) == NULL)
                {
                    node_has_changed = 1;
                    break;
                }
            }
        }

        if (!node_has_changed)
        {
            if (i != zwif_count(node->ep.intf))
            {
                node_has_changed = 1;
            }
        }

        if (node_has_changed)
        {
            node_removed = 1;

            //Saving s2 info to ensure the replacing node also have the same s2 info
            s2_grnt_keys = node->s2_grnt_keys;
            s2_keys_valid = node->s2_keys_valid;

            //Saving node properties
            old_node_propty = node->propty;

            //Remove the node
            zwnode_rm(nw, node_info->node_id);
        }
        else
        {   //Nothing has changed

            //Check whether node security property has been set
            zwnet_node_secure_propty_set(node, new_cmd_cls, new_cmd_cnt, node_info->cmd_cnt_sec);

            if (s2_node_info && s2_node_info->dsk)
            {
                strcpy(node->s2_dsk, s2_node_info->dsk);
            }

            //Update security commands
            if (node->propty & NODE_PROPTY_ADD_SECURE)
            {
                //Save granted keys
                if (s2_node_info && s2_node_info->grnt_keys_valid)
                {
                    node->s2_grnt_keys = s2_node_info->grnt_keys;
                    node->s2_keys_valid = 1;
                }
                result = zwnet_ni_sec_updt(&node->ep, node_info->cmd_cls_sec, node_info->cmd_cnt_sec);
            }
            else
            {
                result = ZW_ERR_NONE;
            }

            plt_mtx_ulck(nw->mtx);
            if (new_cmd_cls != node_info->cmd_cls)
            {
                free(new_cmd_cls);
            }

            return result;
        }
    }

    //Add a new node
    node = (zwnode_p)calloc(1, sizeof(zwnode_t));
    if (!node)
    {
        plt_mtx_ulck(nw->mtx);
        if (new_cmd_cls != node_info->cmd_cls)
        {
            free(new_cmd_cls);
        }
        return ZW_ERR_MEMORY;
    }

    node->isNew = 1;

    //Init non-zero value
    node->wkup_intv = -1;

    //Init all the back links
    node->net = nw;
    node->ep.node = node;

    //Save node id
    node->nodeid = node_info->node_id;

    zwobj_add(&nw->ctl.obj.next, &node->obj);

    //Assign default end point
    node->ep.epid = VIRTUAL_EP_ID;

    //Update basic, generic, specific device classes
    node->basic = node_info->basic;
    node->ep.generic = node_info->gen;
    node->ep.specific = node_info->spec;
    node->ep.intf = NULL;

    //Update node properties
    node->sensor = node_info->sensor;
    node->listen = node_info->listen;

    //node->sensor = 1;  // Testing

    //Create unsecure interfaces based on command classes
    for (i = 0; i < new_cmd_cnt; i++)
    {
        //Get default interface property
        zwnet_dev_if_propty_tmout_get(&node->ep, new_cmd_cls[i], 1, IF_PROPTY_UNSECURE, &propty, &tmout);

        intf = zwif_create(new_cmd_cls[i], 1, propty /*IF_PROPTY_UNSECURE*/);

        if (!intf)
        {
            plt_mtx_ulck(nw->mtx);
            if (new_cmd_cls != node_info->cmd_cls)
            {
                free(new_cmd_cls);
            }
            return ZW_ERR_MEMORY;
        }

//      if (tmout != intf->tmout)
//      {   //Device database has modified time out value
//          intf->tmout = tmout;
//      }

        //Save back link to end point
        intf->ep = &node->ep;

        //Add interface to the end point
        if (node->ep.intf)
        {   //Second interface onwards
            zwobj_add(&node->ep.intf->obj.next, &intf->obj);
        }
        else
        {   //First interface
            node->ep.intf = intf;
        }
    }

    //Save node DSK
    if (s2_node_info && s2_node_info->dsk)
    {
        node->security_incl_status = 2;
        strcpy(node->s2_dsk, s2_node_info->dsk);
    }


    //Save granted keys
    if (s2_node_info && s2_node_info->grnt_keys_valid)
    {
        node->s2_grnt_keys = s2_node_info->grnt_keys;
        node->s2_keys_valid = 1;
    }
    else if (nw->ctl_propty & CTLR_PROPTY_NIF_GRANT_KEYS)
    {   //Node info contains grant keys
        node->s2_grnt_keys = node_info->resrvd;
        node->s2_keys_valid = 1;
    }
    else if (node_removed)
    {   //Restore the s2 info
        node->s2_grnt_keys = s2_grnt_keys;
        node->s2_keys_valid = s2_keys_valid;
    }

    //
    //Update security commands
    //
    if ((old_node_propty & (NODE_PROPTY_ADD_SECURE | NODE_PROPTY_ADD_INSECURE)) == 0)
    {    //Security property has not been set yet
        zwnet_node_secure_propty_set(node, new_cmd_cls, new_cmd_cnt, node_info->cmd_cnt_sec);
    }
    else
    {
        //Restore back node security property
        node->propty &= ~(NODE_PROPTY_SECURE_CAP_S0 | NODE_PROPTY_SECURE_CAP_S2 | NODE_PROPTY_ADD_SECURE | NODE_PROPTY_ADD_INSECURE);
        node->propty |= old_node_propty;
    }

    if (node->propty & NODE_PROPTY_ADD_SECURE)
    {
        result = zwnet_ni_sec_updt(&node->ep, node_info->cmd_cls_sec, node_info->cmd_cnt_sec);
    }
    else
    {
        result = ZW_ERR_NONE;
    }


    //Update node properties based on interfaces at the root level
    zwnet_sm_node_propty_updt(node);

    plt_mtx_ulck(nw->mtx);
    if (new_cmd_cls != node_info->cmd_cls)
    {
        free(new_cmd_cls);
    }

    return result;

}


/**
zwnet_ni_sec_updt - Update secure command classes to interfaces.
@param[in]	ep		    Endpoint
@param[in]	sec_cls     Buffer that stores the security command classes
@param[in]  cnt         Length of buffer
@return		ZW_ERR_xxx.
@pre        Caller must lock the nw->mtx before calling this function.
*/
int zwnet_ni_sec_updt(zwep_p ep, uint16_t *sec_cls, uint8_t cnt)
{
    unsigned    i;
    int         skip_sec_rm = 0; //Flag to indicate whether to skip remove secure property from interfaces
    zwif_p      intf;
    uint16_t    *new_sec_cls = NULL;
    uint8_t     cls_ver;    //version of the command class

    if (!sec_cls || !cnt)
    {
        return ZW_ERR_NONE;
    }

    //Check whether the secure list contains basic command class
    if (zwnet_cmd_cls_find(sec_cls, COMMAND_CLASS_BASIC, cnt) == NULL)
    {
        //Add basic command class into the secure list
        new_sec_cls = (uint16_t *)malloc((cnt + 1) * sizeof(uint16_t));

        if (!new_sec_cls)
        {
            return ZW_ERR_MEMORY;
        }

        //Copy old secure list to new secure list with added basic command class
        new_sec_cls[0] = COMMAND_CLASS_BASIC;
        memcpy(new_sec_cls + 1, sec_cls, cnt * sizeof(uint16_t));

        //Adjust the input parameters
        sec_cls = new_sec_cls;
        cnt++;

    }

    //Check the existence of the interface which corresponds
    //to the command class in the secure list
    for (i = 0; i < cnt; i++)
    {
        intf = zwif_find_cls(ep->intf, sec_cls[i]);

        if (intf)
        {   //Interface has already existed, update its secure property
            intf->propty |= IF_PROPTY_SECURE;
        }
        else
        {   //Create interface for the secure command class

            cls_ver = 1;

            if (ep->epid)
            {
                //The version number of the command class should be
                //inherited from the virtual end point (root device)
                intf = zwif_find_cls(ep->node->ep.intf, sec_cls[i]);
                if (intf)
                {
                    cls_ver = intf->real_ver;
                }
            }

            intf = zwif_create(sec_cls[i], cls_ver, IF_PROPTY_SECURE);

            if (!intf)
            {
                if (new_sec_cls)
                {
                    free(new_sec_cls);
                }
                return ZW_ERR_MEMORY;
            }

            //Save back link to end point
            intf->ep = ep;

            //Add interface to the end point
            if (ep->intf)
            {   //Second interface onwards
                zwobj_add(&ep->intf->obj.next, &intf->obj);
            }
            else
            {   //First interface
                ep->intf = intf;
            }
        }
    }

    //For real endpoint, the interface property depends on node level COMMAND_CLASS_MULTI_CHANNEL_V2 interface property
    if (ep->epid > 0)
    {
        intf = zwif_find_cls(ep->node->ep.intf, COMMAND_CLASS_MULTI_CHANNEL_V2);
        if (intf)
        {
            if ((intf->propty & (IF_PROPTY_SECURE | IF_PROPTY_UNSECURE)) == IF_PROPTY_SECURE)
            {
                //The COMMAND_CLASS_MULTI_CHANNEL_V2 interface property is secure only,
                //all the endpoint interfaces must be accessed through security encapsulation
                skip_sec_rm = 1;
            }
        }
    }


    //Make sure each existing secure interface to match the new secure command class
    if (!skip_sec_rm)
    {
        intf = ep->intf;
        while (intf)
        {
            if (intf->propty & IF_PROPTY_SECURE)
            {   //Secure interface, must find a match
                if (zwnet_cmd_cls_find(sec_cls, intf->cls, i) == NULL)
                {
                    //No match, change the interface to not supporting secure mode
                    //with the exception of basic command class.
                    if (intf->cls != COMMAND_CLASS_BASIC)
                    {
                        intf->propty &= ~IF_PROPTY_SECURE;
                    }
                }
            }
            intf = (zwif_p)intf->obj.next;
        }
    }

    if (new_sec_cls)
    {
        free(new_sec_cls);
    }
    return ZW_ERR_NONE;
}


/**
zwnet_zwver_rpt_cb - Z-wave library version report callback
@param[in]	intf	    The interface that received the report
@param[in]	proto_ver	Z-Wave Protocol Version
@param[in]	app_ver	    Application Version
@param[in]	lib_type	Z-Wave Library Type
@param[in]	ext_ver	    Extended version information if this parameter is not NULL
@return
*/
void zwnet_zwver_rpt_cb(zwif_p intf, uint16_t proto_ver, uint16_t app_ver, uint8_t lib_type, ext_ver_t *ext_ver)
{
    zwnet_p     nw;
    uint8_t     data[32];

    data[0] = proto_ver >> 8;
    data[1] = proto_ver & 0xFF;
    data[2] = app_ver >> 8;
    data[3] = app_ver & 0xFF;
    data[4] = lib_type;

    memcpy(data + 6, &intf, sizeof(zwif_p));
    memcpy(data + 6 + sizeof(zwif_p), &ext_ver, sizeof(ext_ver_t *));
    nw = intf->ep->node->net;

    plt_mtx_lck(nw->mtx);
    zwnet_node_info_sm(nw, EVT_ZWVER_REPORT, data);

    //IMPORTANT NOTE:  The intf may point to invalid (freed)
    //                 memory after this call. Place the
    //                 zwnet_ctlr_info_sm() at the last line
    if (intf->ep->node == &nw->ctl)
    {
        zwnet_ctlr_info_sm(nw, EVT_CTLR_VER_REPORT, data);
    }
    plt_mtx_ulck(nw->mtx);

}


#ifndef TCP_PORTAL
/**
zwnet_unsolicit_rpt_cb - Z/IP unsolicited destination report callback
@param[in]	ifd	            The interface that received the report
@param[in]	dst_ip	        unsolicited destination IPv6 address
@param[in]	dst_port	    unsolicited destination port
@return
*/
static void zwnet_unsolicit_rpt_cb(zwifd_p ifd, uint8_t *dst_ip, uint16_t dst_port)
{
    zwnet_p     nw;
    uint8_t     data[32];

    data[0] = dst_port >> 8;
    data[1] = dst_port & 0xFF;

    memcpy(data + 2, &dst_ip, sizeof(uint8_t *));
    memcpy(data + 2 + sizeof(uint8_t *), &ifd, sizeof(zwifd_p));

    nw = ifd->net;

    plt_mtx_lck(nw->mtx);
    zwnet_init_sm(nw, EVT_INI_CTLR_UNSOLICIT, data);
    plt_mtx_ulck(nw->mtx);

}
#endif


/**
zwnet_cmd_cls_show - show binary string of command classes
@param[in] nw		    Network
@param[in] cmd_cls_buf  Buffer that stores the command classes
@param[in] cnt          The number of command classes in cmd_cls
@return
*/
int has_s2_cls = 0;
void zwnet_cmd_cls_show(zwnet_p nw, uint16_t *cmd_cls_buf, uint8_t cnt)
{
    uint8_t     tmp_cnt;
    char        tmp[8];
    char        hex_str[MAX_BIN_LINE_LEN * 8];

    while (cnt > 0)
    {
        //Split the binary string into multiple of MAX_BIN_LINE_LEN
        if (cnt >= MAX_BIN_LINE_LEN)
        {
            cnt -= MAX_BIN_LINE_LEN;
            tmp_cnt = MAX_BIN_LINE_LEN;
        }
        else
        {
            tmp_cnt = cnt;
            cnt = 0;
        }

        hex_str[0] = '\0';

        //Convert into hex string
        while (tmp_cnt-- > 0)
        {
#ifdef USE_SAFE_VERSION
            sprintf_s(tmp, 8, "%02X, ",(unsigned) *cmd_cls_buf++);
            strcat_s(hex_str, MAX_BIN_LINE_LEN * 8, tmp);
#else
            if(*cmd_cls_buf == 0x9F)
            {
                ALOGI("node supported S2 CC");
                has_s2_cls = 1;
            }
            sprintf(tmp,"%02X, ",(unsigned) *cmd_cls_buf++);
            strcat(hex_str, tmp);
#endif
        }

        debug_zwapi_msg(&nw->plt_ctx, "%s", hex_str);
    }
}

#ifdef MAP_IP_ASSOC_TO_ASSOC
static void zwnet_ip_assoc_map(appl_node_info_t *cached_ni)
{
    int     i;
    int     multi_ch_present = 0;

    for (i=0; i<cached_ni->cmd_cnt; i++)
    {
        if (cached_ni->cmd_cls[i] == COMMAND_CLASS_MULTI_CHANNEL_V2)
        {
            multi_ch_present = 1;
            break;
        }
    }

    if (!multi_ch_present)
    {
        for (i=0; i<cached_ni->cmd_cnt_sec; i++)
        {
            if (cached_ni->cmd_cls_sec[i] == COMMAND_CLASS_MULTI_CHANNEL_V2)
            {
                multi_ch_present = 1;
                break;
            }
        }
    }

    //Search for IP Assoc CC
    for (i=0; i<cached_ni->cmd_cnt; i++)
    {
        if (cached_ni->cmd_cls[i] == COMMAND_CLASS_IP_ASSOCIATION)
        {
            cached_ni->cmd_cls[i] = (multi_ch_present)? COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V2 : COMMAND_CLASS_ASSOCIATION;
            break;
        }
    }

    for (i=0; i<cached_ni->cmd_cnt_sec; i++)
    {
        if (cached_ni->cmd_cls_sec[i] == COMMAND_CLASS_IP_ASSOCIATION)
        {
            cached_ni->cmd_cls_sec[i] = (multi_ch_present)? COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V2 : COMMAND_CLASS_ASSOCIATION;
            break;
        }
    }
}
#endif


/**
zwnet_node_updt_cb - Update node neighbors callback function
@param[in]	appl_ctx    The application layer context
@param[in]	sts		    The transmit complete status
@param[in]	node_id     The node that performs node neighbors discovery
@return
*/
static void zwnet_node_updt_cb(appl_layer_ctx_t    *appl_ctx, uint8_t sts, uint8_t node_id)
{
    /*
    #define REQUEST_NEIGHBOR_UPDATE_STARTED     0x21
    #define REQUEST_NEIGHBOR_UPDATE_DONE        0x22
    #define REQUEST_NEIGHBOR_UPDATE_FAILED      0x23
    */
    const char *status_msg[] = { "New neighbor list received", "Failed",
        "unknown"
    };
    zwnet_t *nw = (zwnet_t *)appl_ctx->data;
    int status;

    status = ((sts >= REQUEST_NEIGHBOR_UPDATE_DONE) && (sts <= REQUEST_NEIGHBOR_UPDATE_FAILED))?
             (sts - REQUEST_NEIGHBOR_UPDATE_DONE) : 2;
    debug_zwapi_msg(&nw->plt_ctx, "req_node_nbr_updt_cb: status:%u(%s), node id:%u", sts, status_msg[status], node_id);

    //Call state-machine
    plt_mtx_lck(nw->mtx);
    if (sts == REQUEST_NEIGHBOR_UPDATE_DONE)
    {
        zwnet_updt_sm(nw, EVT_NB_UPDT_DONE, NULL);
    }
    else
    {
        zwnet_updt_sm(nw, EVT_NB_UPDT_FAILED, NULL);
    }
    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_node_info_cmplt_cb - Get node info complete callback function
@param[in]	nw              The network
@param[in]	first_node_id   The first node id
@return
*/
static void zwnet_node_info_cmplt_cb(zwnet_p nw, uint8_t first_node_id)
{
    plt_mtx_lck(nw->mtx);
    zwnet_init_sm(nw, EVT_INI_NI_UPDT_DONE, NULL);
    zwnet_updt_sm(nw, EVT_NI_UPDT_DONE, NULL);
    zwnet_nw_change_sm(nw, EVT_NC_NODE_INFO_DONE, NULL);
    plt_mtx_ulck(nw->mtx);
}


/**
zwnet_sm_nu_tout_cb - Network update state machine timer callback
@param[in] data     Pointer to network
@return
*/
static void    zwnet_sm_nu_tout_cb(void *data)
{
    zwnet_p   nw = (zwnet_p)data;

    //Stop timer
    plt_mtx_lck(nw->mtx);
    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
    nw->sm_tmr_ctx = NULL;//make sure timer context is null, else restart timer will crash
    zwnet_updt_sm(nw, EVT_NU_TMOUT, NULL);
    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_sm_neighbor_updt - Get the node neighbor update and update the state-machine's state
@param[in]	nw		    Network
@return  0 on success, negative error number on failure
*/
static int zwnet_sm_neighbor_updt(zwnet_p nw)
{
    int      result;
    zwnode_p node;

    //Get the node id list
    if ((node = (zwnode_p)nw->ctl.obj.next) != NULL)
    {
        nw->nu_sm_ctx.num_node = 0;
        nw->nu_sm_ctx.rd_idx = 0;

        while (node)
        {
            nw->nu_sm_ctx.node_id[nw->nu_sm_ctx.num_node++] = node->nodeid;
            node = (zwnode_p)node->obj.next;
        }

        result = zw_request_node_neighbor_update(&nw->appl_ctx,
                                                 zwnet_node_updt_cb, nw->nu_sm_ctx.node_id[0]);
        if (result == 0)
        {
            //Change state
            nw->nu_sm_ctx.sta = NU_STA_NEIGHBOR_UPDT;

            //Update progress status
            nw->nu_progress_sts = OP_NU_NEIGHBOR;

            //Stop timer
            plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);

            //Start timer
            nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_NODE_NBR_UPDT_TMOUT, zwnet_sm_nu_tout_cb, nw);

            //Callback to report progress of the network update operation
            zwnet_notify(nw, ZWNET_OP_UPDATE, nw->nu_progress_sts, NULL);
        }

        return result;
    }

    return ZW_ERR_NODE_NOT_FOUND;
}


/**
zwnet_sm_node_info_updt - Get the node info and update the state-machine's state
@param[in]	nw		    Network
@return  0 on success, negative error number on failure
*/
static int zwnet_sm_node_info_updt(zwnet_p nw)
{
    zwnode_p    node;
    sm_job_t    sm_job = {0};
    uint8_t     node_id[256];

    sm_job.op = ZWNET_OP_UPDATE;
    sm_job.num_node = 0;
    sm_job.node_id = node_id;
    sm_job.cb = zwnet_node_info_cmplt_cb;
    sm_job.auto_cfg = 0;
    sm_job.assign_rr = 1;
    sm_job.delay_slp_node = 1;

    //Get the node id list
    if ((node = (zwnode_p)nw->ctl.obj.next) != NULL)
    {
        while (node)
        {
            node_id[sm_job.num_node++] = node->nodeid;
            node = (zwnode_p)node->obj.next;
        }

        if (zwnet_node_info_sm(nw, EVT_GET_NODE_INFO, (uint8_t *)&sm_job))
        {
            //Change state
            nw->nu_sm_ctx.sta = NU_STA_NODE_INFO_UPDT;

            //Update progress status
            nw->nu_progress_sts = OP_NU_GET_NODE_INFO;

            //Callback to report progress of the network update operation
            zwnet_notify(nw, ZWNET_OP_UPDATE, nw->nu_progress_sts, NULL);
            return ZW_ERR_NONE;

        }
        return ZW_ERR_OP_FAILED;
    }
    return ZW_ERR_NODE_NOT_FOUND;
}


/**
zwnet_sm_nu_failed - Stop the network update state-machine
                     due to failure to start node info update
@param[in]	nw		Network
@return
*/
static void    zwnet_sm_nu_failed(zwnet_p nw)
{
    //Reset to idle state
    nw->nu_sm_ctx.sta = NU_STA_IDLE;
    nw->nu_sm_ctx.num_node = 0;

    //Reset operation to "no operation"
    nw->curr_op = ZWNET_OP_NONE;

    //Invoke notify callback
    zwnet_notify(nw, ZWNET_OP_UPDATE, OP_FAILED, NULL);
}


/**
zwnet_update_cb - Request network topology updates from the SUC/SIS node callback function
@param[in]	appl_ctx    The application layer context
@param[in]	sts		    Status of update
@return
*/
static void req_nw_updt_cb(appl_layer_ctx_t *appl_ctx, uint8_t sts)
{
    /*
    #define ZW_SUC_UPDATE_DONE      0x00
    #define ZW_SUC_UPDATE_ABORT     0x01
    #define ZW_SUC_UPDATE_WAIT      0x02
    #define ZW_SUC_UPDATE_DISABLED  0x03
    #define ZW_SUC_UPDATE_OVERFLOW  0x04
    */
    const char *status_msg[] = { "OK", "Aborted", "SUC node is busy", "The SUC functionality is disabled",
        "The controller has to make a replication",
        "unknown"
    };
    zwnet_p     nw = (zwnet_p)appl_ctx->data;
    int status;

    status = (/*(sts >= 0) &&*/ (sts <= 4))? sts : 5;
    debug_zwapi_msg(&nw->plt_ctx, "req_nw_updt_cb: status:%u(%s)", sts, status_msg[status]);

    //Call state-machine
    plt_mtx_lck(nw->mtx);
    zwnet_updt_sm(nw, (sts == ZW_SUC_UPDATE_DONE)?
                  EVT_NU_TOPOLOGY_DONE : EVT_NU_TOPOLOGY_FAILED, NULL);
    plt_mtx_ulck(nw->mtx);
}


/**
zwnet_ctlr_node_info_cb - Request controller node info callback function
@param[in]	appl_ctx    The application layer context
@param[in]	cached_ni   Cached node information
@return
*/
static void zwnet_ctlr_node_info_cb(appl_layer_ctx_t *appl_ctx, appl_node_info_t *cached_ni)
{
    zwnet_t *nw = (zwnet_t *)appl_ctx->data;
    int cmd_cls_exist = 0;

    ALOGI("zwnet_ctlr_node_info_cb for node id:%u, granted_keys:%x", cached_ni->node_id, cached_ni->resrvd);
    debug_zwapi_msg(&nw->plt_ctx, "listening=%u, optional functions=%u", cached_ni->listen, cached_ni->optional);
    debug_zwapi_msg(&nw->plt_ctx, "status=%u, age=%u minutes old", cached_ni->status, (unsigned)(1 << cached_ni->age));
    debug_zwapi_msg(&nw->plt_ctx, "Device type: basic=%02Xh, generic=%02Xh, specific=%02X", cached_ni->basic, cached_ni->gen, cached_ni->spec);

    if (cached_ni->cmd_cnt > 0)
    {
        debug_zwapi_msg(&nw->plt_ctx, "unsecure command classes:");
        zwnet_cmd_cls_show(nw, cached_ni->cmd_cls, cached_ni->cmd_cnt);
        cmd_cls_exist = 1;

    }

    if (cached_ni->cmd_cnt_sec > 0)
    {
        debug_zwapi_msg(&nw->plt_ctx, "secure command classes:");
        zwnet_cmd_cls_show(nw, cached_ni->cmd_cls_sec, cached_ni->cmd_cnt_sec);
        cmd_cls_exist = 1;

    }

    if (cmd_cls_exist)
    {
#ifdef MAP_IP_ASSOC_TO_ASSOC
        zwnet_ip_assoc_map(cached_ni);
#endif
        //Update the network data structure
        plt_mtx_lck(nw->mtx);
        zwnet_ctlr_info_sm(nw, EVT_CTLR_NODE_INFO, (uint8_t *)cached_ni);
        plt_mtx_ulck(nw->mtx);

    }
}


/**
zwnet_ctlr_role_get - Get the controller role based on node information
@param[in]	cmd_cls         Command classes
@param[in]	cmd_cls_sec     Security supported command classes
@param[in]	cmd_cnt         The number of command classes in cmd_cls
@param[in]	cmd_cnt_sec     The number of command classes in cmd_cls_sec
@return     controller role
*/
static uint8_t zwnet_ctlr_role_get(uint16_t *cmd_cls, uint16_t *cmd_cls_sec, uint8_t cmd_cnt, uint8_t cmd_cnt_sec)
{
    int         i;
    uint16_t    nw_mgnt_cmd_cls[] = {
                                        COMMAND_CLASS_NETWORK_MANAGEMENT_INCLUSION,
                                        COMMAND_CLASS_NETWORK_MANAGEMENT_PRIMARY,
                                        COMMAND_CLASS_NETWORK_MANAGEMENT_PROXY
                                    };
    uint8_t     ctlr_roles[] =  {
                                    ZWNET_CTLR_ROLE_INCL,
                                    ZWNET_CTLR_ROLE_PRI,
                                    ZWNET_CTLR_ROLE_PROXY
                                };
    uint8_t     ctl_role;


    ctl_role = 0;

    for (i=0; i<3; i++)
    {
        if (zwnet_cmd_cls_find(cmd_cls, nw_mgnt_cmd_cls[i], cmd_cnt)
            || zwnet_cmd_cls_find(cmd_cls_sec, nw_mgnt_cmd_cls[i], cmd_cnt_sec))
        {
            ctl_role |= ctlr_roles[i];
        }
    }

    return ctl_role;
}


/**
zwnet_ini_ctlr_info_cmplt_cb - Get controller info complete callback function
@param[in]	nw    The network
@return
*/
static void zwnet_ini_ctlr_info_cmplt_cb(zwnet_p nw, int status)
{
    zwnet_init_sm(nw, EVT_INI_CTLR_INFO_DONE, (uint8_t *)&status);
}


/**
zwnet_rst_ctlr_info_cmplt_cb - Get controller info complete callback function
@param[in]	nw    The network
@return
*/
static void zwnet_rst_ctlr_info_cmplt_cb(zwnet_p nw, int status)
{
    zwnet_rst_sm(nw, EVT_RST_CTLR_INFO_DONE, (uint8_t *)&status);
}


/**
zwnet_updt_ctlr_info_cmplt_cb - Get controller info complete callback function
@param[in]	nw    The network
@return
*/
static void zwnet_updt_ctlr_info_cmplt_cb(zwnet_p nw, int status)
{
    zwnet_updt_sm(nw, EVT_NU_CTLR_INFO_DONE, (uint8_t *)&status);
}


/**
zwnet_updt_sm - Network update state-machine
@param[in]	nw		Network
@param[in] evt      The event for the state-machine
@param[in] data     The data associated with the event
@return             0 if the state-machine is started from idle state; else return non-zero
@pre    Caller must lock the nw->mtx before calling this function
*/
static int    zwnet_updt_sm(zwnet_p  nw, zwnet_nu_evt_t  evt, uint8_t *data)
{
    int32_t result;

    switch (nw->nu_sm_ctx.sta)
    {
        //----------------------------------------------------------------
        case NU_STA_IDLE:
        //----------------------------------------------------------------
            if (evt == EVT_NU_START)
            {
                ctlr_sm_ctx_t   ctlr_sm_ctx = {0};

                //Reset the state machine context
                memset(&nw->nu_sm_ctx, 0, sizeof(nu_sm_ctx_t));
                ctlr_sm_ctx.ctlr_sm_cb = zwnet_updt_ctlr_info_cmplt_cb;

                if (zwnet_ctlr_info_sm(nw, EVT_CTLR_START, (uint8_t *)&ctlr_sm_ctx))
                {
                    //Change state
                    nw->nu_sm_ctx.sta = NU_STA_CTLR_INFO;
                    return 0;//state-machine started successfully

                }

                debug_zwapi_msg(&nw->plt_ctx, "zwnet_updt_sm: failed to get Z/IP gateway controller info");
                break;

            }
            break;

        //----------------------------------------------------------------
        case NU_STA_CTLR_INFO:
        //----------------------------------------------------------------
            if (evt == EVT_NU_CTLR_INFO_DONE)
            {
                int *status = (int *)data;

                if (*status == 0)
                {   //o.k.
                    //Start network topology update
                    result = zw_request_network_update(&nw->appl_ctx, req_nw_updt_cb);

                    if (result == 0)
                    {
                        //Change state
                        nw->nu_sm_ctx.sta = NU_STA_TOPOLOGY_UPDT;

                        //Start timer
                        nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, 10000, zwnet_sm_nu_tout_cb, nw);

                        //Update progress status
                        nw->nu_progress_sts = OP_NU_TOPOLOGY;

                        //Callback to report progress of the network update operation
                        zwnet_notify(nw, ZWNET_OP_UPDATE, nw->nu_progress_sts, NULL);

                        break;
                    }

                    debug_zwapi_msg(&nw->plt_ctx, "zw_request_network_update with error:%d", result);
                }

                zwnet_sm_nu_failed(nw);

            }
            break;

        //----------------------------------------------------------------
        case NU_STA_TOPOLOGY_UPDT:
        //----------------------------------------------------------------
            switch (evt)
            {
                case EVT_NU_TMOUT:
                case EVT_NU_TOPOLOGY_FAILED:
                    if (evt == EVT_NU_TMOUT)
                    {
                        debug_zwapi_msg(&nw->plt_ctx, "zwnet_updt_sm: network topology update timeout");
                    }
                    else
                    {
                        debug_zwapi_msg(&nw->plt_ctx, "zwnet_updt_sm: network topology update failed");
                    }
                    //fall through

                case EVT_NU_TOPOLOGY_DONE:
                    //Stop timer
                    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                    nw->sm_tmr_ctx = NULL;

                    //Get node list
                    result = zip_node_list_get(&nw->appl_ctx, zwnet_node_list_get_cb);

                    if (result == 0)
                    {
                        //Change state
                        nw->nu_sm_ctx.sta = NU_STA_NODE_LIST;

                        //Start timer
                        nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_nu_tout_cb, nw);

                        break;
                    }

                    debug_zwapi_msg(&nw->plt_ctx, "zip_node_list_get with error:%d", result);
                    zwnet_sm_nu_failed(nw);
                    break;

                default:
                    break;

            }
            break;

        //----------------------------------------------------------------
        case NU_STA_NODE_LIST:
        //----------------------------------------------------------------
            switch (evt)
            {
                zwnode_p    node;

                case EVT_NU_TMOUT:
                    debug_zwapi_msg(&nw->plt_ctx, "zwnet_updt_sm: get node list timeout");
                    zwnet_sm_nu_failed(nw);
                    break;

                case EVT_NU_NODE_LIST:
                    //Stop timer
                    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                    nw->sm_tmr_ctx = NULL;

                    //Discover the node-ip address information
                    if ((node = (zwnode_p)nw->ctl.obj.next) != NULL)
                    {
                        n2ip_sm_ctx_t   n2ip_sm_ctx = {0};

                        //Create the node id list
                        n2ip_sm_ctx.num_node = 0;
                        while (node && (n2ip_sm_ctx.num_node < MAX_ZWAVE_NODE))
                        {
                            n2ip_sm_ctx.node_id[n2ip_sm_ctx.num_node++] = node->nodeid;
                            node = (zwnode_p)node->obj.next;
                        }

                        //Call the resolve node to ip address state-machine
                        n2ip_sm_ctx.cb = zwnet_n2ip_lst_cb;
                        n2ip_sm_ctx.params[0] = (void *)(unsigned)ZWNET_OP_UPDATE;

                        result = zwnet_n2ip_sm(nw, EVT_N2IP_START, (uint8_t *)&n2ip_sm_ctx);

                        if (result == 0)
                        {
                            //Change state
                            nw->nu_sm_ctx.sta = NU_STA_NODE_IP_DSCVRY;
                            break;
                        }
                        else
                        {
                            debug_zwapi_msg(&nw->plt_ctx, "zwnet_updt_sm resolve node id with error:%d", result);
                        }
                    }

                    //The network has no other node
                    zwnet_sm_nu_stop(nw);

                    //Callback to report completion of the network update operation
                    zwnet_notify(nw, ZWNET_OP_UPDATE, OP_DONE, NULL);

                    break;

                default:
                    break;

            }
            break;

        //----------------------------------------------------------------
        case NU_STA_NODE_IP_DSCVRY:
        //----------------------------------------------------------------
            if (evt == EVT_NU_NODE_ID_ADDR)
            {
                if (*data == 0)
                {
                    //Check whether the controller supports Network Management Inclusion Command Class
                    if (nw->ctl_role & ZWNET_CTLR_ROLE_INCL)
                    {   //Start node neighbor update
                        result = zwnet_sm_neighbor_updt(nw);
                        if (result == 0)
                        {
                            break;
                        }
                        else
                        {
                            debug_zwapi_msg(&nw->plt_ctx, "request_node_neighbor_update with error:%d", result);
                        }
                    }

                    //Node neighbor update failed or controller is secondary controller, proceed to node info update
                    result = zwnet_sm_node_info_updt(nw);
                    if (result < 0)
                    {
                        debug_zwapi_msg(&nw->plt_ctx, "zwnet_sm_node_info_updt with error:%d", result);
                        zwnet_sm_nu_stop(nw);
                        //Callback to report completion of the network update operation
                        zwnet_notify(nw, ZWNET_OP_UPDATE, OP_DONE, NULL);
                    }

                    break;
                }
                //Resolve node to ip address failed
                debug_zwapi_msg(&nw->plt_ctx, "Failed to get node HAN address");
                zwnet_sm_nu_failed(nw);
                break;
            }
            break;

        //----------------------------------------------------------------
        case NU_STA_NEIGHBOR_UPDT:
        //----------------------------------------------------------------
            switch (evt)
            {
                case EVT_NU_TMOUT:
                case EVT_NB_UPDT_FAILED:
                    if (evt == EVT_NU_TMOUT)
                    {
                        debug_zwapi_msg(&nw->plt_ctx, "zwnet_updt_sm: neighbor update timeout");
                    }
                    else
                    {
                        debug_zwapi_msg(&nw->plt_ctx, "zwnet_updt_sm: neighbor update failed on node:%u",
                                        nw->nu_sm_ctx.node_id[nw->nu_sm_ctx.rd_idx]);
                    }

                    //Add to the failed list if it is not in the list
                    if (memchr(nw->nu_sm_ctx.failed_id, nw->nu_sm_ctx.node_id[nw->nu_sm_ctx.rd_idx],
                               nw->nu_sm_ctx.num_failed_node) == NULL)
                    {
                        nw->nu_sm_ctx.failed_id[nw->nu_sm_ctx.num_failed_node++] = nw->nu_sm_ctx.node_id[nw->nu_sm_ctx.rd_idx];
                    }
                    //fall through

                case EVT_NB_UPDT_DONE:
                    //Stop timer
                    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                    nw->sm_tmr_ctx = NULL;

                    if (++nw->nu_sm_ctx.rd_idx < nw->nu_sm_ctx.num_node)
                    {
                        //Process the next node
                        result = zw_request_node_neighbor_update(&nw->appl_ctx,
                                                                 zwnet_node_updt_cb,
                                                                 nw->nu_sm_ctx.node_id[nw->nu_sm_ctx.rd_idx]);

                        if (result == 0)
                        {
                            //Start timer
                            nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_NODE_NBR_UPDT_TMOUT, zwnet_sm_nu_tout_cb, nw);
                            break;
                        }
                    }
                    else if ((nw->nu_sm_ctx.num_failed_node > 0)
                             && (nw->nu_sm_ctx.num_retry < MAX_NEIGHBOR_UPDT_RETRIES))
                    {
                        nw->nu_sm_ctx.num_retry++;
                        debug_zwapi_msg(&nw->plt_ctx, "zwnet_updt_sm: neighbor update retry: %u", nw->nu_sm_ctx.num_retry);
                        memcpy(nw->nu_sm_ctx.node_id, nw->nu_sm_ctx.failed_id, nw->nu_sm_ctx.num_failed_node);
                        nw->nu_sm_ctx.num_node = nw->nu_sm_ctx.num_failed_node;
                        nw->nu_sm_ctx.num_failed_node = 0;
                        nw->nu_sm_ctx.rd_idx = 0;

                        result = zw_request_node_neighbor_update(&nw->appl_ctx,
                                                                 zwnet_node_updt_cb, nw->nu_sm_ctx.node_id[0]);

                        if (result == 0)
                        {
                            //Start timer
                            nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_NODE_NBR_UPDT_TMOUT, zwnet_sm_nu_tout_cb, nw);
                            break;
                        }
                    }

                    //Proceed to node info update
                    result = zwnet_sm_node_info_updt(nw);
                    if (result < 0)
                    {
                        debug_zwapi_msg(&nw->plt_ctx, "zwnet_sm_node_info_updt with error:%d", result);
                        zwnet_sm_nu_stop(nw);
                        //Callback to report completion of the network update operation
                        zwnet_notify(nw, ZWNET_OP_UPDATE, OP_DONE, NULL);
                    }

                    break;

                default:
                    break;
            }
            break;

        //----------------------------------------------------------------
        case NU_STA_NODE_INFO_UPDT:
        //----------------------------------------------------------------
            if (evt == EVT_NI_UPDT_DONE)
            {
                //Reset to idle state
                nw->nu_sm_ctx.sta = NU_STA_IDLE;
                nw->nu_sm_ctx.num_node = 0;
            }
            break;


    }
    return -1;
}


/**
zwnode_msg_s2_scheme_chk - Check if the incoming message uses the node highest s2 scheme key for encryption
@param[in]  node		    Node
@param[in]  encap_fmt       Incoming message encapsulation format
@param[in]  encap_fmt_valid Flag to indicate whether the field encap_fmt is valid
@return Non-zero if checking is okay; else zero.
*/
static int zwnode_msg_s2_scheme_chk(zwnode_p node, uint16_t encap_fmt, uint16_t encap_fmt_valid)
{
    uint16_t highest_key;

    //Check if node supports S2
    if (!(node->propty & NODE_PROPTY_SECURE_CAP_S2))
    {   //It does not support S2, check pass
        return 1;
    }

    //Get node highest granted key
    if (!node->s2_keys_valid)
    {   //No granted key info
        return 0;
    }

    if (node->s2_grnt_keys & SEC_KEY_BITMSK_S2_K2)
    {
        highest_key = SEC_KEY_BITMSK_S2_K2;
    }
    else if (node->s2_grnt_keys & SEC_KEY_BITMSK_S2_K1)
    {
        highest_key = SEC_KEY_BITMSK_S2_K1;
    }
    else if (node->s2_grnt_keys & SEC_KEY_BITMSK_S2_K0)
    {
        highest_key = SEC_KEY_BITMSK_S2_K0;
    }
    else if (node->s2_grnt_keys & SEC_KEY_BITMSK_S0)
    {   //In legacy S0 network, check pass
        return 1;
    }
    else
    {   //No security key granted
        return 0;
    }

    //Check against message encapsulation format
    if (!encap_fmt_valid)
    {
        return 0;
    }

    if ((encap_fmt >> 8) & highest_key)
    {   //Matched
        return 1;
    }
    return 0;
}


/**
zwnet_rpt_send - send report
@param[in]  nw          network
@param[in]  cmd_buf     command buffer
@param[in]  len         length of cmd_buf
@param[in]  dst_node    destination node id of the transmission
@param[in]  ep_addr     endpoints of the transmission
@param[in]  msg_type    message encapsulation type: ZWNET_MSG_TYPE_XXX
@param[in]	encap_fmt   optional encapsulation format info. if the incoming command has this info,
                        it must be passed back when responding to this command.

@return     ZW_ERR_xxx
*/
static int zwnet_rpt_send(zwnet_p nw, uint8_t *cmd_buf, uint8_t len, uint8_t dst_node, frm_ep_t *ep_addr,
                          uint8_t msg_type, uint16_t *encap_fmt)
{
    int             result;
    appl_snd_data_t *prm;

    //Allocate buffer that is large enough for CRC-16 encapsulation
    prm = (appl_snd_data_t *)calloc(1, sizeof(appl_snd_data_t) + len + CRC16_OVERHEAD);
    if (!prm)
        return ZW_ERR_MEMORY;

    //Prepare the report
    prm->dat_buf = (uint8_t *)&prm[1];
    memcpy(prm->dat_buf, cmd_buf, len);
    prm->node_id = dst_node;
    prm->ep_addr = *ep_addr;
    prm->dat_len = len;
    //prm->flag = ZIP_FLAG_HOST_PORT; //djnakata

    if (encap_fmt)
    {
        prm->flag |= ZIP_FLAG_ENCAP_INFO;
        prm->encap_fmt = *encap_fmt;
    }

    //Send the report
    if ((msg_type & ZWNET_MSG_TYPE_SECURE)
        || nw->use_encap_hdr //Deprecate "secure origin" bit in Z/IP header. Alwasy set "secure origin" bit to 1
        )
    {
        prm->flag |= ZIP_FLAG_SECURE;
    }
    //Send the command without queuing
    result = zw_send_data(&nw->appl_ctx, prm, NULL, NULL);

    free(prm);
    return result;
}


#ifdef SUPPORT_SIMPLE_AV_CONTROL
    #define SIMPLE_AV_CONTROL_SUPPORTED_BITMASK_LEN 32
/**
handle_simple_av - Handle simple AV control
@param[in]  nw          Network
@param[in]  ep          Endpoint
@param[in]  ep_addr     Endpoint address
@param[in]  cmd_len     Command buffer length
@param[in]  cmd_buf     Command buffer
@param[in]  src_node    Source node of command (message)
@param[in]  msg_type    Message type
@param[in]  prm         Param to appl_cmd_hdlr
@return Non-zero if command handled; else zero
*/
static int handle_simple_av(zwnet_p nw, zwep_p ep, frm_ep_t *ep_addr, uint16_t cmd_len, uint8_t *cmd_buf,
                            uint8_t src_node, uint8_t msg_type, appl_cmd_prm_t *prm)
{
    int cmd_handled = 0;

    //Respond only for local endpoint 0
    if ((ep_addr->dst_ep_type == 0) && (ep_addr->dst_ep == 0) &&
        (cmd_len >= 2) &&
        (cmd_buf[0] == COMMAND_CLASS_SIMPLE_AV_CONTROL))
    {
        static uint8_t  av_set_seq_num = 0xFF;
        int             result;
        zwepd_t         epd;

        cmd_handled = 1;

        if ((cmd_buf[1] == SIMPLE_AV_CONTROL_SET) &&
            (cmd_len >= 8) &&
            (cmd_buf[2] != av_set_seq_num)) // Discard duplicates
        {
            av_set_seq_num = cmd_buf[2];

            if (nw->init.av_set)
            {
                int i;
                uint16_t cmds[10];
                uint8_t num;

                num = (cmd_len - 6) / 2;
                if (num > sizeof(cmds))
                {
                    num = sizeof(cmds);
                }

                for (i = 0; i < num; i++)
                {
                    cmds[i] = cmd_buf[6 + (2 * i)] << 8 | cmd_buf[6 + (2 * i) + 1];
                }

                zwep_get_desc(ep, &epd);
                nw->init.av_set(&epd, cmds, num, cmd_buf[3] & 0x7);
            }
        }
        else if ((cmd_buf[1] == SIMPLE_AV_CONTROL_GET) &&
                 (nw->av_sup_bitmask_len > 0))
        {
            uint8_t c_buf[3];
            frm_ep_t rpt_ep;

            //Prepare the report
            c_buf[0] = COMMAND_CLASS_SIMPLE_AV_CONTROL;
            c_buf[1] = SIMPLE_AV_CONTROL_REPORT;
            // Maximum of SIMPLE_AV_CONTROL_SUPPORTED_BITMASK_LEN bytes are allowed in one report
            c_buf[2] = ((nw->av_sup_bitmask_len - 1) / SIMPLE_AV_CONTROL_SUPPORTED_BITMASK_LEN) + 1;

            //Send the report
            rpt_ep.src_ep = 0;
            rpt_ep.dst_ep_type = 0;
            rpt_ep.dst_ep = ep_addr->src_ep;
            result = zwnet_rpt_send(nw, c_buf, 3, src_node, &rpt_ep, msg_type,
                                    (prm->encap_fmt_valid)? &prm->encap_fmt : NULL);
            if (result < 0)
            {
                debug_zwapi_msg(&nw->plt_ctx, "Send  AV control rpt with error:%d", result);
            }
        }
        else if ((cmd_buf[1] == SIMPLE_AV_CONTROL_SUPPORTED_GET) &&
                 (cmd_len >= 3))
        {
            uint8_t c_buf[SIMPLE_AV_CONTROL_SUPPORTED_BITMASK_LEN + 3];
            uint16_t start_idx, bitmask_len;
            frm_ep_t rpt_ep;

            //Prepare the report
            c_buf[0] = COMMAND_CLASS_SIMPLE_AV_CONTROL;
            c_buf[1] = SIMPLE_AV_CONTROL_SUPPORTED_REPORT;

            if (cmd_buf[2] >= 1)
            {
                start_idx = (cmd_buf[2] - 1) * SIMPLE_AV_CONTROL_SUPPORTED_BITMASK_LEN;

                if (start_idx < nw->av_sup_bitmask_len)
                {
                    bitmask_len = ((start_idx + SIMPLE_AV_CONTROL_SUPPORTED_BITMASK_LEN) <= nw->av_sup_bitmask_len)
                                  ? SIMPLE_AV_CONTROL_SUPPORTED_BITMASK_LEN : (nw->av_sup_bitmask_len - start_idx);

                    c_buf[2] = cmd_buf[2];
                    memcpy(&c_buf[3], &nw->av_sup_bitmask[start_idx], bitmask_len);

                    //Send the report
                    rpt_ep.src_ep = 0;
                    rpt_ep.dst_ep_type = 0;
                    rpt_ep.dst_ep = ep_addr->src_ep;
                    result = zwnet_rpt_send(nw, c_buf, bitmask_len + 3, src_node, &rpt_ep, msg_type,
                                            (prm->encap_fmt_valid)? &prm->encap_fmt : NULL);
                    if (result < 0)
                    {
                        debug_zwapi_msg(&nw->plt_ctx, "Send  AV control supported rpt with error:%d", result);
                    }
                }
            }
        }
    }

    return cmd_handled;
}
#endif


/**
zwnet_mul_cmd_parse - Parse multi-command packet
@param[in] cmd_buf         Multi-command buffer
@param[in] cmd_cnt         Number of commands
@param[in] cmd_len         Buffer length
@return       Non-zero on success; else return zero
*/
static int zwnet_mul_cmd_parse(uint8_t cmd_cnt, uint8_t *cmd_buf, uint8_t cmd_len)
{
    uint8_t i;
    uint8_t enc_cmd_len;
    uint8_t valid_cmd_cnt = 0;
    uint8_t bytes_to_process = cmd_len;

    for (i=0; i < cmd_cnt; i++)
    {
        if (bytes_to_process > 0)
        {
            enc_cmd_len = *cmd_buf++;
            bytes_to_process--;

            //Check whether there are enough bytes for the encapsulated command
            if (bytes_to_process >= enc_cmd_len)
            {
                valid_cmd_cnt++;

                //Adjustment
                cmd_buf += enc_cmd_len;
                bytes_to_process -= enc_cmd_len;

            }
            else
            {   //Error
                return 0;
            }
        }
    }

    return (valid_cmd_cnt == cmd_cnt)? 1 : 0;
}


/**
zwnet_sup_cmd_cls_ver_get - Get the command class version from  a command class list
@param[in] cmd_cls_buf     The buffer to store the command classes
@param[in] cmd_cls         The command class
@param[in] cmd_cls_cnt     The number of command classes stored in the cmd_cls_buf
@return       The version of the command class if found; else return zero
*/
static uint8_t zwnet_sup_cmd_cls_ver_get(sup_cmd_cls_t *cmd_cls_buf, uint16_t cmd_cls, uint8_t cmd_cls_cnt)
{
    uint8_t i;

    for (i=0; i<cmd_cls_cnt; i++)
    {
        if (cmd_cls_buf[i].cls == cmd_cls)
        {
            return cmd_cls_buf[i].ver;
        }
    }

    return 0;
}


/**
zwnet_sup_cmd_cls_find - Find command class from a command class list
@param[in]  cmd_cls_lst		Command class list
@param[in]  cmd_cls         Command class
@param[in]  cmd_cls_cnt     Number of entries in the command class list
@return       Pointer to the command class if found; else return NULL
*/
sup_cmd_cls_t *zwnet_sup_cmd_cls_find(sup_cmd_cls_t *cmd_cls_lst, uint16_t cmd_cls, uint8_t cmd_cls_cnt)
{
    uint8_t i;

    if (!cmd_cls_lst)
    {
        return NULL;
    }

    for (i=0; i<cmd_cls_cnt; i++)
    {
        if (cmd_cls_lst[i].cls == cmd_cls)
        {
            return &cmd_cls_lst[i];
        }
    }

    return NULL;
}


#ifdef  DYNAMIC_APP_NIF_SET
/**
zwnet_sup_cmd_cls_ppty_set - Set command class property of a command class list
@param[in]  cmd_cls_lst		Command class list
@param[in]  cmd_cls_cnt     Number of entries in the command class list
@param[in]  ppty		    Command class property, BITMASK_CMD_CLS_XXX
@return
*/
static void zwnet_sup_cmd_cls_ppty_set(sup_cmd_cls_t *cmd_cls_lst, uint8_t cmd_cls_cnt, uint8_t ppty)
{
    uint8_t i;

    for (i=0; i<cmd_cls_cnt; i++)
    {
        cmd_cls_lst[i].propty = ppty;
    }

}


/**
zwnet_ctlr_sec_ppty_set - Set controller interfaces security property
@param[in]  ctlr_intf		First interface of controller
@param[in]  cmd_cls_lst		Command class list
@param[in]  cmd_cls_cnt     Number of entries in the command class list
@param[in]  ppty		    Security property, IF_PROPTY_SECURE or IF_PROPTY_UNSECURE
@return
*/
static void zwnet_ctlr_sec_ppty_set(zwif_p ctlr_intf, sup_cmd_cls_t *cmd_cls_lst, uint8_t cmd_cls_cnt, uint8_t ppty)
{
    zwif_p  intf;
    uint8_t i;

    for (i=0; i<cmd_cls_cnt; i++)
    {
        intf = zwif_find_cls(ctlr_intf, cmd_cls_lst[i].cls);

        if (intf)
        {
            intf->propty &= ~(IF_PROPTY_SECURE | IF_PROPTY_UNSECURE);
            intf->propty |= ppty;

        }
    }
}
#endif


/**
zwnet_mul_cmd_resp_add - Add an entry for multi-command encapsulated query
@param[in]  nw		    network
@param[in]  cmd_buf     command buffer
@param[in]  len         length of cmd_buf
@param[in]  rpt_cls     expected report if type is MUL_CMD_TYPE_RPT_GET
@param[in]  rpt         expected command class of report if type is MUL_CMD_TYPE_RPT_GET
@param[in]  type        command type, see MUL_CMD_TYPE_XXX
@return
*/
static void zwnet_mul_cmd_resp_add(zwnet_p nw, uint8_t *cmd_buf, uint8_t len, uint16_t rpt_cls, uint8_t rpt, uint8_t type)
{
    mul_cmd_ent_t   *mul_cmd_ent;

    mul_cmd_ent = (mul_cmd_ent_t *)calloc(1, sizeof(mul_cmd_ent_t) + len);

    if (mul_cmd_ent)
    {
        mul_cmd_ent->type = type;
        mul_cmd_ent->rpt = rpt;
        mul_cmd_ent->rpt_cls = rpt_cls;
        mul_cmd_ent->len = len;
        memcpy(mul_cmd_ent->cmd, cmd_buf, len);
        util_list_add(NULL, &nw->mul_cmd_hd,
                      (uint8_t *)mul_cmd_ent, sizeof(mul_cmd_ent_t) + len);
        free(mul_cmd_ent);
    }
}


/**
zwnet_spvsn_rpt_send - Handle single command
@param[in]  prm         Parameters of received command
@param[in]  status      Status, SPV_STATUS_XXX
@return ZW_ERR_XXX
*/
static int zwnet_spvsn_rpt_send(appl_cmd_prm_t *prm, uint8_t status)
{
    uint8_t     send_buf[8];
    frm_ep_t    rpt_ep;
    int         result;

    rpt_ep.src_ep = 0;
    rpt_ep.dst_ep_type = 0;
    rpt_ep.dst_ep = prm->ep_addr->src_ep;

    send_buf[0] = COMMAND_CLASS_SUPERVISION;
    send_buf[1] = SUPERVISION_REPORT;
    send_buf[2] = prm->spvsn_sess_id;
    send_buf[3] = status;
    send_buf[4] = 0;//duration

    result = zwnet_rpt_send(prm->nw, send_buf, 5, prm->src_node, &rpt_ep, prm->msg_type, (prm->encap_fmt_valid)? &prm->encap_fmt : NULL);

    if (result < 0)
    {
        debug_zwapi_msg(&prm->nw->plt_ctx, "Send supervision rpt with error:%d", result);
    }

    return result;
}


/**
zwnet_unhandle_cmd_save - Save unhandled command
@param[in]  cmd_len     Command length
@param[in]  cmd_buf     Command buffer
@param[in]  prm         Parameters of command
@return
*/
static void zwnet_unhandle_cmd_save(uint16_t cmd_len, uint8_t *cmd_buf, appl_cmd_prm_t *prm)
{
    zwnet_p             nw = prm->nw;
    appl_cmd_saved_t    *appl_cmd;

    plt_mtx_lck(nw->mtx);
    if (!nw->appl_cmd_cnt)
    {   //Allocate memory for the first command
        nw->appl_cmd_saved = (appl_cmd_saved_t *)calloc(1, sizeof(appl_cmd_saved_t));
        if (!nw->appl_cmd_saved)
        {
            plt_mtx_ulck(nw->mtx);
            return;
        }

        appl_cmd = nw->appl_cmd_saved;

        nw->appl_cmd_cnt++;
    }
    else
    {   //Extend the array
        appl_cmd_saved_t    *tmp_cmd;
        unsigned            i;

        tmp_cmd = (appl_cmd_saved_t *)realloc(nw->appl_cmd_saved, (nw->appl_cmd_cnt + 1) * sizeof(appl_cmd_saved_t));

        if (!tmp_cmd)
        {
            plt_mtx_ulck(nw->mtx);
            return;
        }

        //Reassign cmd_param.ep_addr as the address may have changed after realloc()
        for (i=0; i<nw->appl_cmd_cnt; i++)
        {
            tmp_cmd[i].cmd_param.ep_addr = &tmp_cmd[i].ep_address;
        }

        nw->appl_cmd_saved = tmp_cmd;

        appl_cmd = &nw->appl_cmd_saved[nw->appl_cmd_cnt];

        memset(appl_cmd, 0, sizeof(appl_cmd_saved_t));

        nw->appl_cmd_cnt++;
    }

    appl_cmd->cmd_len = (cmd_len < MAX_ZWAVE_PKT_SIZE)? cmd_len : MAX_ZWAVE_PKT_SIZE;

    memcpy(appl_cmd->cmd_buf, cmd_buf, appl_cmd->cmd_len);

    appl_cmd->ep_address = *prm->ep_addr;

    appl_cmd->cmd_param = *prm;

    appl_cmd->cmd_param.ep_addr = &appl_cmd->ep_address;

    plt_mtx_ulck(nw->mtx);

}


/**
appl_cmd_hdlr - Handle single command
@param[in]  cmd_len     Command length
@param[in]  cmd_buf     Command buffer
@param[in]  prm         Parameters of command
@return     Zero on success; -1 if supervision_get command failed; -2 if supervision_get command security scheme failed
*/
static int appl_cmd_hdlr(uint16_t cmd_len, uint8_t *cmd_buf, appl_cmd_prm_t *prm)
{
    int         cmd_handled = 0;
    int         ret_val = 0;
    zwnode_p    node;
    zwif_p      intf;
    zwnet_p     nw = prm->nw;
    uint16_t    *encap_format;
    frm_ep_t    *ep_addr = prm->ep_addr;
    uint8_t     src_node = prm->src_node;
    uint8_t     msg_type = prm->msg_type;
    uint8_t     epid = 0;   //Default to virtual end point

    //Display command received
//#ifdef SHOW_PACKET_INFO
    ALOGI("Receive frame from node:%u, cmd:", (unsigned)src_node);
    debug_zwapi_bin_msg(&nw->plt_ctx, cmd_buf, cmd_len);
//#endif
    //Save unhandled learn mode command
    if ((nw->curr_op == ZWNET_OP_INITIATE) && (src_node == UNSOLICITED_NODE_ID)
        && (nw->init_sm_sta < INI_STA_EXE_UNHANDLE_CMD))
    {
        zwnet_unhandle_cmd_save(cmd_len, cmd_buf, prm);
        return 0;
    }

    //Call multi-cmd handler report handle
    if (!prm->tot_mul_cmd && (src_node == nw->ctl.nodeid))
    {
        plt_mtx_lck(nw->mtx);
        zwnet_mul_cmd_sm(nw, EVT_MUL_CMD_REPORT, cmd_buf, cmd_len);
        plt_mtx_ulck(nw->mtx);

    }

    encap_format = (prm->encap_fmt_valid)? &prm->encap_fmt : NULL;

    //Check for Supervision get command
    if (prm->spvsn_encap)
    {   //The encapsulated command must either destined for the Z/IP gateway or local
        goto l_CHECK_UNHANDLE_CMD;
    }

    //
    //Find the interface to handle the report
    //
    node = zwnode_find(&nw->ctl, src_node);
    if (node && (cmd_len >= 2))
    {
        zwep_p  ep;
        //Determine if this is a multi-channel encapsulation command
        if (ep_addr->src_ep)
        {
            epid = ep_addr->src_ep;
        }

        //Change endpoint id based on device database entry
        if (node->dev_cfg_valid)
        {
            uint8_t     target_ep;
            uint16_t    cls;
            uint8_t     rpt_cmd;

            if (zwif_cls_cmd_get(cmd_buf, cmd_len, &cls, &rpt_cmd))
            {
                //Find if this command need to be redirected to another endpoint
                if (zwdev_redir_ep_get(node->dev_cfg_rec.ep_rec, epid, cls, rpt_cmd, &target_ep))
                {
                    epid = target_ep;
                }
            }
        }

        //Find end point
        ep = zwep_find(&node->ep, epid);
        if (ep)
        {
            uint16_t    cls;

            cls = cmd_buf[0];

            if (cmd_buf[0] >= 0xF1)
            {   //Extended command class
                cls = (cls << 8) | cmd_buf[1];
            }

            //Find the interface that corresponds with the command class
            intf = zwif_find_cls(ep->intf, cls);

            if (intf)
            {
                //Check if the incoming message uses the node highest s2 scheme key for encryption.
                if ((msg_type & ZWNET_MSG_TYPE_SECURE) && !(intf->propty & IF_PROPTY_UNSECURE))
                {
                    if ((src_node != nw->ctl.nodeid) && !zwnode_msg_s2_scheme_chk(node, prm->encap_fmt, prm->encap_fmt_valid))
                    {
                        goto l_CHECK_UNHANDLE_CMD;
                    }
                }

                //Check message type against interface security property
                if ((intf->propty & IF_PROPTY_SECURE) && !(intf->propty & IF_PROPTY_UNSECURE))
                {   //Only accept secure message

                    if ((msg_type & ZWNET_MSG_TYPE_SECURE) == 0)
                    {
                        if (src_node != nw->ctl.nodeid)
                        {   //Exception for Z/IP gateway controller since the communication channel between Z/IP gateway
                            //and Z/IP client is always deemed to be secure.
                            goto l_CHECK_UNHANDLE_CMD;
                        }
                    }
                }

                //Block "wake up notification" in multi-client mode
                if (nw->multi_client)
                {
                    if ((cls == COMMAND_CLASS_WAKE_UP) && (cmd_buf[1] == WAKE_UP_NOTIFICATION))
                    {
                        // Anyway, we should report wake up notification here.
                        //goto l_CHECK_MULTI_CMD;
                    }
                }

                //Check whether the report is the one which the node is waiting for
                zwnode_wait_rpt_chk(intf, cmd_buf, cmd_len);

                //Check whether the report is the polling report
                zwpoll_on_rpt_rcv(intf, cmd_buf, cmd_len);

                //Inform post-set polling system
                zwspoll_on_rpt_rcv(intf, cmd_buf, cmd_len);

                //Inform supervision system
                if(zwspv_on_rpt_rcv(intf, cmd_buf, cmd_len))
                {   //Report is processed
                    goto l_CHECK_MULTI_CMD;
                }

                // Central scene notification reported at here
                if(cmd_buf[0] == COMMAND_CLASS_CENTRAL_SCENE && cmd_buf[1] == CENTRAL_SCENE_NOTIFICATION)
                {
                    if (cmd_len >= 5)
                    {
                        zwcentral_scene_notify_t notify_info;
                        notify_info.seq_num = cmd_buf[2];
                        notify_info.key_attr = cmd_buf[3] & 0x07;
                        notify_info.scene_num = cmd_buf[4];
                        zwifd_t ifd;
                        zwif_get_desc(intf, &ifd);
                        //Callback the registered function
                        zwcontrol_central_scene_notification_report_cb(&ifd, &notify_info);
                    }
                }
                else {
                    //Invoke the report handler
                    cmd_handled = zwif_rep_hdlr(intf, cmd_buf, cmd_len, 0);
                }
            }

#ifdef SUPPORT_SIMPLE_AV_CONTROL
            if (!cmd_handled)
            {
                cmd_handled = handle_simple_av(nw, ep, ep_addr, cmd_len, cmd_buf, src_node, msg_type, prm);
            }
#endif
        }
    }

l_CHECK_UNHANDLE_CMD:

    if (!cmd_handled && (cmd_len >= 2))
    {
        uint16_t    cls;

        //
        //Unhandled command.  Check the message type (secure/insecure) against the controller interface security property before handle it.
        //
        cls = cmd_buf[0];

        if (cmd_buf[0] >= 0xF1)
        {   //Extended command class
            cls = (cls << 8) | cmd_buf[1];
        }

        intf = zwif_find_cls(nw->ctl.ep.intf, cls);

        if (intf)
        {   //Interface of Z/IP gateway

            if ((msg_type & ZWNET_MSG_TYPE_SECURE) && !(intf->propty & IF_PROPTY_UNSECURE))
            {
                //Check if the incoming message uses the Z/IP gateway highest s2 scheme key for encryption
                if (!zwnode_msg_s2_scheme_chk(&nw->ctl, prm->encap_fmt, prm->encap_fmt_valid))
                {
                    if (prm->spvsn_encap)
                    {
                        ret_val = -2;
                    }
                    goto l_CHECK_MULTI_CMD;
                }
            }

            if ((intf->propty & IF_PROPTY_SECURE) && !(intf->propty & IF_PROPTY_UNSECURE))
            {    //Secure only interface
                if ((msg_type & ZWNET_MSG_TYPE_SECURE) == 0)
                {   //Incoming message is unsecure
                    if (src_node != nw->ctl.nodeid)
                    {   //Exception for Z/IP gateway controller since the communication channel between Z/IP gateway
                        //and Z/IP client is always deemed to be secure.
                        if (prm->spvsn_encap)
                        {
                            ret_val = -2;
                        }
                        goto l_CHECK_MULTI_CMD;
                    }
                }
            }
        }
        else
        {   //Z/IP gateway does not support the command class
            if (prm->spvsn_encap)
            {
                ret_val = -1;
                goto l_CHECK_MULTI_CMD;
            }
            goto l_CALLBACK_UNHANDLE_CMD;
        }

        //Assume command will be handled
        cmd_handled = 1;

        switch(cmd_buf[0])
        {
            //------------------------------------------------------------------------------------
            case COMMAND_CLASS_VERSION:
            //------------------------------------------------------------------------------------

                //A receiving node MUST NOT return a response to a GET command if it is received via multicast addressing
                if (msg_type & ZWNET_MSG_TYPE_MULTICAST)
                {
                    break;
                }

                if(cmd_buf[1] == VERSION_COMMAND_CLASS_GET)
                {
                    uint8_t     send_buf[8];
                    uint8_t     i = 0;
                    uint8_t     cmd_cls_ver;
                    frm_ep_t    rpt_ep;
                    int         result;

                    if (cmd_len < 3)
                    {
                        goto l_CHECK_MULTI_CMD;
                    }

                    rpt_ep.src_ep = 0;
                    rpt_ep.dst_ep_type = 0;
                    rpt_ep.dst_ep = ep_addr->src_ep;

                    send_buf[i++] = cmd_buf[0];
                    send_buf[i++] = VERSION_COMMAND_CLASS_REPORT;
                    send_buf[i++] = cmd_buf[2];

                    cmd_cls_ver = zwnet_sup_cmd_cls_ver_get(nw->sup_cmd_cls, cmd_buf[2], nw->sup_cmd_cls_cnt);

                    if (cmd_cls_ver > 0)
                    {
                        //Check the command class is not listed as Z/IP gateway supported command class
                        //if (!zwnet_sup_cmd_cls_find(nw->gw_sup_cmd_cls, cmd_buf[2], nw->gw_sup_cmd_cls_cnt))
                        //{
                            send_buf[i++] = cmd_cls_ver;

                            if (prm->tot_mul_cmd == 0)
                            {
                                result = zwnet_rpt_send(nw, send_buf, i, src_node, &rpt_ep, msg_type, encap_format);
                                if (result < 0)
                                    debug_zwapi_msg(&nw->plt_ctx, "VERSION_COMMAND_CLASS_REPORT: Send rpt with error:%d", result);
                            }
                            else
                            {
                                zwnet_mul_cmd_resp_add(nw, send_buf, i, 0, 0, MUL_CMD_TYPE_RPT);
                            }
                            break;
                        //}
                    }

                    if (prm->tot_mul_cmd)
                    {   //Multi-command encapsulated query, help Z/IP gateway to respond
                        zwif_p intf;

                        intf = zwif_find_cls(nw->ctl.ep.intf, cmd_buf[2]);
                        send_buf[i++] = (intf)? intf->ver : 0;
                        zwnet_mul_cmd_resp_add(nw, send_buf, i, 0, 0, MUL_CMD_TYPE_RPT);
                    }
                }
                else if (cmd_buf[1] == VERSION_GET)
                {
                    if (prm->tot_mul_cmd)
                    {
                        zwnet_mul_cmd_resp_add(nw, cmd_buf, cmd_len, COMMAND_CLASS_VERSION, VERSION_REPORT, MUL_CMD_TYPE_RPT_GET);
                    }
                }
                break;

            //------------------------------------------------------------------------------------
            case COMMAND_CLASS_ASSOCIATION_GRP_INFO:
            case COMMAND_CLASS_ASSOCIATION:
            //------------------------------------------------------------------------------------
                {
                    int assc_ret;

                    assc_ret = handle_association(cmd_len, cmd_buf, prm);

                    if (prm->spvsn_encap && (assc_ret < 0))
                    {
                        ret_val = -1;
                    }
                }
                break;

#ifdef  TEST_HANDLE_BASIC_COMMAND_CLASS
            //------------------------------------------------------------------------------------
            case COMMAND_CLASS_BASIC:
            //------------------------------------------------------------------------------------
                {
                    frm_ep_t    rpt_ep;
                    int         result;

                    if (cmd_buf[1] == BASIC_GET)
                    {
                        uint8_t c_buf[3];

                        //Prepare the report
                        c_buf[0] = COMMAND_CLASS_BASIC;
                        c_buf[1] = BASIC_REPORT;
                        c_buf[2] = nw->basic_val;

                        rpt_ep.src_ep = 0;
                        rpt_ep.dst_ep_type = 0;
                        rpt_ep.dst_ep = ep_addr->src_ep;

                        //Send the report
                        result = zwnet_rpt_send(nw, c_buf, 3, src_node, &rpt_ep, msg_type, encap_format);
                        if (result < 0)
                        {
                            debug_zwapi_msg(&nw->plt_ctx, "Send basic rpt with error:%d", result);
                        }
                    }
                    else if (cmd_buf[1] == BASIC_SET)
                    {   //Store the value
                        nw->basic_val = cmd_buf[2];
                    }
                }
                break;
#endif
            //------------------------------------------------------------------------------------
            default:
            //------------------------------------------------------------------------------------
                //Command not handled

                if (prm->tot_mul_cmd)
                {   //Multi-command encapsulated query, help Z/IP gateway to respond
                    uint16_t    cmd_cls;
                    uint8_t     rpt;

                    if (zwnet_get_rpt(cmd_buf, cmd_len, &cmd_cls, &rpt))
                    {   //Get report command
                        if (zwif_find_cls(nw->ctl.ep.intf, cmd_cls))
                        {
                            //A receiving node MUST NOT return a response to a GET command if it is received via multicast addressing
                            if (msg_type & ZWNET_MSG_TYPE_MULTICAST)
                            {
                                break;
                            }

                            zwnet_mul_cmd_resp_add(nw, cmd_buf, cmd_len, cmd_cls, rpt, MUL_CMD_TYPE_RPT_GET);
                            break;
                        }
                    }
                    else if (zwnet_is_set_cmd(cmd_buf, cmd_len, &cmd_cls))
                    {   //Set command
                        if (zwif_find_cls(nw->ctl.ep.intf, cmd_cls))
                        {
                            zwnet_mul_cmd_resp_add(nw, cmd_buf, cmd_len, 0, 0, MUL_CMD_TYPE_SET);
                            break;
                        }
                    }
                }

                cmd_handled = 0;
                break;
        }
    }

l_CALLBACK_UNHANDLE_CMD:
    //Callback user application if unhandled
    if (!cmd_handled)
    {
        if (nw->init.unhandled_cmd)
        {
            nw->init.unhandled_cmd(nw->init.user, prm->src_node, epid, cmd_buf, cmd_len);
        }
    }

l_CHECK_MULTI_CMD:

    if (prm->tot_mul_cmd && nw->mul_cmd_hd && (prm->mul_cmd_num == prm->tot_mul_cmd))
    {   //Finished processing all commands in a multi-command encapsulated packet
        zwnet_mcmd_sm_req_t     mul_cmd_req = {0};

        mul_cmd_req.snd_prm.node_id = src_node;
        mul_cmd_req.snd_prm.ep_addr.dst_ep = ep_addr->src_ep;
        mul_cmd_req.snd_prm.flag = ZIP_FLAG_HOST_PORT;

        if (encap_format)
        {
            mul_cmd_req.snd_prm.flag |= ZIP_FLAG_ENCAP_INFO;
            mul_cmd_req.snd_prm.encap_fmt = *encap_format;
        }

        if ((msg_type & ZWNET_MSG_TYPE_SECURE)
            || nw->use_encap_hdr //Deprecate "secure origin" bit in Z/IP header. Alwasy set "secure origin" bit to 1
            )
        {
            mul_cmd_req.snd_prm.flag |= ZIP_FLAG_SECURE;
        }

        node = zwnode_find(&nw->ctl, src_node);
        if (node && zwif_find_cls(node->ep.intf, COMMAND_CLASS_MULTI_CMD))
        {   //Destination node supports multi-command command class
            //debug_zwapi_ts_msg(&nw->plt_ctx, "node:%u supports multi-command command class", (unsigned)src_node);

            mul_cmd_req.mcmd_resp = 1;
        }

        //Uncomment the following to always use multi-command encapsulation to send response
        //mul_cmd_req.mcmd_resp = 1;

        mul_cmd_req.mul_cmd_hd = nw->mul_cmd_hd;
        nw->mul_cmd_hd = NULL;

        util_list_add(nw->mul_cmd_sm_mtx, &nw->mul_cmd_sm_hd,
                      (uint8_t *)&mul_cmd_req, sizeof(zwnet_mcmd_sm_req_t));

        plt_mtx_lck(nw->mtx);
        //Call state-machine
        zwnet_mul_cmd_sm(nw, EVT_MUL_CMD_START, NULL, 0);
        plt_mtx_ulck(nw->mtx);

    }

    return ret_val;
}


/**
application_command_handler_cb - Application command handler callback function
@param[in]	appl_ctx    The application layer context
@param[in]	src_addr    Source address of the command
@param[in]	ep_addr     Source and destination endpoint
@param[in]	rx_len      The length of the received command
@param[in]	rx_buf      Receive buffer
@param[in]  flag        Flag, see ZIP_FLAG_XXX
@param[in]	encap_fmt   Z-wave encapsulation format (valid if flag ZIP_FLAG_ENCAP_INFO is set).
                        This format should be passed back when responding
                        to unsolicited command with the flag ZIP_FLAG_ENCAP_INFO is set.
@return
*/
static void    application_command_handler_cb(appl_layer_ctx_t *appl_ctx, struct sockaddr *src_addr,
                                              frm_ep_t *ep_addr, uint8_t *rx_buf, uint16_t rx_len,
                                              uint16_t flag, uint16_t encap_fmt)
{
    int             res;
    zwnet_p         nw;
    uint8_t         msg_type; //incoming message type
    uint8_t         src_node;
    appl_cmd_prm_t  prm = {0};

    nw = (zwnet_p)appl_ctx->data;

    if (!nw->cb_thrd_run)
    {
        return;
    }

    msg_type = 0;

    if (appl_ctx->use_ipv4)
    {
        struct sockaddr_in  *sa4 = (struct sockaddr_in *)src_addr;

        if (memcmp(&nw->zip_router_addr[12], &sa4->sin_addr.s_addr, 4) == 0)
        {
            src_node = nw->ctl.nodeid;
        }
        else
        {   //Resolve the source IP address to the node id
            if (!zip_translate_ipaddr(appl_ctx, (uint8_t *)&sa4->sin_addr.s_addr, 4, &src_node))
            {
                //debug_zwapi_msg(&nw->plt_ctx, "Failed to resolve source IP address to the node id");
                //Assign temporary node id for sending report purposes
                src_node = UNSOLICITED_NODE_ID;
                zip_node_ipaddr_set(appl_ctx, (uint8_t *)&sa4->sin_addr.s_addr, 4, src_node);
            }
        }
    }
    else
    {
        struct sockaddr_in6  *sa6 = (struct sockaddr_in6 *)src_addr;

        if (memcmp(sa6->sin6_addr.s6_addr, nw->zip_router_addr, IPV6_ADDR_LEN) == 0)
        {
            src_node = nw->ctl.nodeid;
        }
        else
        {   //Resolve the source IP address to the node id
            if (!zip_translate_ipaddr(appl_ctx, sa6->sin6_addr.s6_addr, IPV6_ADDR_LEN, &src_node))
            {
                //debug_zwapi_msg(&nw->plt_ctx, "Failed to resolve source IP address to the node id");
                //Assign temporary node id for sending report purposes
                src_node = UNSOLICITED_NODE_ID;
                zip_node_ipaddr_set(appl_ctx, sa6->sin6_addr.s6_addr, IPV6_ADDR_LEN, src_node);
            }
        }
    }

    //Update node alive status
    appl_updt_node_sts(appl_ctx, src_node, APPL_NODE_STS_UP, 1);


#ifdef CRC16_ENCAP
    //Check for CRC-16 encapsulation
//  if ((rx_len > CRC16_OVERHEAD) &&
//      (rx_buf[0] == COMMAND_CLASS_CRC_16_ENCAP) &&
//      (rx_buf[1] == CRC_16_ENCAP))
//  {
//      //Check for data integrity
//      if (zwutl_crc16_chk(CRC_INIT, rx_buf, rx_len) == 0)
//      {
//          rx_len -= CRC16_OVERHEAD;
//          memmove(rx_buf, rx_buf + 2, rx_len);
//          msg_type = ZWNET_MSG_TYPE_CRC16;
//      }
//      else
//      {
//          debug_zwapi_ts_msg(&nw->plt_ctx, "CRC-16 checksum failed");
//          return;
//      }
//
//  }
#endif

    //
    //Check for security encapsulated command
    //

    if (nw->use_encap_hdr)
    {   //"secure origin" bit in Z/IP header is deprecated. Use Encapsulation Format Info to determine
        //whether message is secure.
        if (flag & ZIP_FLAG_ENCAP_INFO)
        {
            if ((encap_fmt >> 8) & SEC_KEY_ALL_S2_SO)
            {
                msg_type = ZWNET_MSG_TYPE_SECURE;
            }
        }
    }
    else
    {   //Old Z/IP gateway using secure flag to indicate secure packet from Z-wave legacy S0 encapsulation
        if (flag & ZIP_FLAG_SECURE)
        {
            if (msg_type == 0)
            {
                msg_type = ZWNET_MSG_TYPE_SECURE;
            }
        }
    }

    if (flag & ZIP_FLAG_ENCAP_INFO)
    {
        prm.encap_fmt_valid = 1;
        prm.encap_fmt = encap_fmt;
    }

    if (flag & ZIP_FLAG_MULTICAST)
    {
        msg_type |= ZWNET_MSG_TYPE_MULTICAST;
    }

    prm.msg_type = msg_type;
    prm.nw = nw;
    prm.ep_addr = ep_addr;
    prm.src_node = src_node;
    prm.appl_ctx = appl_ctx;

    //
    //Check for Supervision Get Encapsulated Command
    //
    if ((rx_buf[0] == COMMAND_CLASS_SUPERVISION) && (rx_buf[1] == SUPERVISION_GET))
    {
        uint8_t     encap_len;
        uint16_t    set_cmd_cls;
        zwif_p      spvsn_if;

        //A receiving node MUST NOT return a response to a GET command if it is received via multicast addressing
        if (msg_type & ZWNET_MSG_TYPE_MULTICAST)
        {
            return;
        }

        spvsn_if = zwif_find_cls(nw->ctl.ep.intf, COMMAND_CLASS_SUPERVISION);
        if (!spvsn_if)
        {   //Z/IP gateway does not support Supervision command class
            return;
        }

        if ((msg_type & ZWNET_MSG_TYPE_SECURE)
            && ((spvsn_if->propty & IF_PROPTY_SECURE) && !(spvsn_if->propty & IF_PROPTY_UNSECURE)))
        {   //Secure incoming message and secure only interface, check if the S2 scheme use is the highest granted to Z/IP gateway
            if (!zwnode_msg_s2_scheme_chk(&nw->ctl, prm.encap_fmt, prm.encap_fmt_valid))
                return;
        }

        if (rx_len < 4)
        {
            return;
        }

        //Mark the command as supervision encapsulated
        prm.spvsn_sess_id = rx_buf[2] & 0x3F;
        prm.spvsn_encap = 1;

        encap_len = rx_buf[3];

        if (rx_len < (4 + encap_len))
        {
            return;
        }

        //Check for multi command encapsulation in supervision_get
        if ((rx_buf[4] == COMMAND_CLASS_MULTI_CMD) && (rx_buf[5] == MULTI_CMD_ENCAP))
        {   //Multi command encapsulated
            uint8_t     *cmd_ptr;
            uint8_t     i;
            uint8_t     cmd_cnt = rx_buf[6];
            uint8_t     bytes_to_process;
            uint8_t     enc_cmd_len;
            uint8_t     status = SPV_STATUS_SUCCESS;

            if (!((cmd_cnt > 0) && (rx_len > 7)))
            {
                zwnet_spvsn_rpt_send(&prm, SPV_STATUS_FAIL);
                return;
            }

            bytes_to_process = rx_len - 7;
            cmd_ptr = rx_buf + 7;

            if (!zwnet_mul_cmd_parse(cmd_cnt, cmd_ptr, bytes_to_process))
            {   //Parse failed
                zwnet_spvsn_rpt_send(&prm, SPV_STATUS_FAIL);
                return;
            }

            //Parse individual Z-wave command
            for (i=0; i < cmd_cnt; i++)
            {
                if (bytes_to_process == 0)
                {
                    status = SPV_STATUS_FAIL;
                    break;
                }

                enc_cmd_len = *cmd_ptr++;
                bytes_to_process--;

                //Check whether there are enough bytes for the encapsulated command
                if (bytes_to_process >= enc_cmd_len)
                {   //Supervision get should be use for set Z-wave command, not for report_get command
                    if (!zwnet_is_set_cmd(cmd_ptr, enc_cmd_len, &set_cmd_cls)
                        && !zwnet_is_local_set_cmd(cmd_ptr, enc_cmd_len, &set_cmd_cls))
                    {
                        status = SPV_STATUS_NO_SUPPORT;
                        break;
                    }
                    //Adjustment
                    cmd_ptr += enc_cmd_len;
                    bytes_to_process -= enc_cmd_len;
                }
                else
                {   //Error
                    status = SPV_STATUS_FAIL;
                    break;
                }
            }

            if (status != SPV_STATUS_SUCCESS)
            {
                zwnet_spvsn_rpt_send(&prm, status);
                return;
            }

        }
        else if (zwnet_is_local_set_cmd(&rx_buf[4], encap_len, &set_cmd_cls))
        {   //Local supported set command

        }
        else
        {
            zwnet_spvsn_rpt_send(&prm, SPV_STATUS_NO_SUPPORT);
            return;
        }

        //Move the receive buffer from the offset of supervision get command
        rx_len -= 4;
        memmove(rx_buf, rx_buf + 4, rx_len);
    }

    //Check for Multi Command Encapsulated Command
    if ((rx_buf[0] == COMMAND_CLASS_MULTI_CMD) && (rx_buf[1] == MULTI_CMD_ENCAP))
    {
        int         err_cnt = 0;
        uint8_t     *cmd_ptr;
        uint8_t     cmd_cnt;
        uint8_t     bytes_to_process;
        uint8_t     enc_cmd_len;
        uint8_t     i;

        if (rx_len >= 3)
        {
            cmd_cnt = rx_buf[2];
            if (cmd_cnt > 0)
            {
                bytes_to_process = rx_len - 3;
                cmd_ptr = rx_buf + 3;

                if (!zwnet_mul_cmd_parse(cmd_cnt, cmd_ptr, bytes_to_process))
                {
                    return;
                }

                res = 0;

                //Process each command in the multi-cmd encapsulation
                for (i=0; i < cmd_cnt; i++)
                {
                    if (bytes_to_process > 0)
                    {
                        enc_cmd_len = *cmd_ptr++;
                        bytes_to_process--;

                        //Check whether there are enough bytes for the encapsulated command
                        if (bytes_to_process >= enc_cmd_len)
                        {
                            prm.mul_cmd_num = i+1;
                            prm.tot_mul_cmd = cmd_cnt;
                            res = appl_cmd_hdlr(enc_cmd_len, cmd_ptr, &prm);

                            if (prm.spvsn_encap)
                            {
                                if (res < 0)
                                {
                                    err_cnt++;
                                }
                            }
                            //Adjustment
                            cmd_ptr += enc_cmd_len;
                            bytes_to_process -= enc_cmd_len;

                        }
                        else
                        {   //Error
                            break;
                        }
                    }
                }

                //Send supervision report for overall result of all processed commands
                if (prm.spvsn_encap)
                {
                    zwnet_spvsn_rpt_send(&prm, (err_cnt == 0)? SPV_STATUS_SUCCESS : (res == -2)? SPV_STATUS_NO_SUPPORT : SPV_STATUS_FAIL);
                }
            }
        }
    }
    else
    {   //Single command
        res = appl_cmd_hdlr(rx_len, rx_buf, &prm);

        //Send supervision report for result of processed command
        if (prm.spvsn_encap)
        {
            zwnet_spvsn_rpt_send(&prm, (res == 0)? SPV_STATUS_SUCCESS : (res == -2)? SPV_STATUS_NO_SUPPORT : SPV_STATUS_FAIL);
        }
    }
}


/**
zwnet_exec_unhandle_cmd - Execute unhandled commands
@param[in]  nw      Network
@return
*/
static void zwnet_exec_unhandle_cmd(zwnet_p nw)
{
    appl_cmd_saved_t    *appl_cmd;
    uint16_t            i;

    plt_mtx_lck(nw->mtx);

    for (i=0; i<nw->appl_cmd_cnt; i++)
    {
        appl_cmd = &nw->appl_cmd_saved[i];

        appl_cmd_hdlr(appl_cmd->cmd_len, appl_cmd->cmd_buf, &appl_cmd->cmd_param);

    }

    if (nw->appl_cmd_cnt)
    {
        nw->appl_cmd_cnt = 0;
        free(nw->appl_cmd_saved);
        nw->appl_cmd_saved = NULL;
    }

    plt_mtx_ulck(nw->mtx);
}


/**
zwnet_node_probe_cb - send NOP callback function
@param[in]	appl_ctx    The application layer context
@param[in]	tx_sts		The transmit complete status
@param[in]	user_prm    The user specific parameter
@param[in]	nodeid      Node id
@param[in]	param       Additional parameters
@return
*/
static void  zwnet_node_probe_cb(appl_layer_ctx_t *appl_ctx, int8_t tx_sts, void *user_prm, uint8_t nodeid, zw_tx_cb_prm_t *param)
{
	unsigned  node_id = (uintptr_t)user_prm;
    zwnet_p   nw = (zwnet_p)appl_ctx->data;

    if (tx_sts == TRANSMIT_COMPLETE_OK)
    {
        debug_zwapi_msg(appl_ctx->plt_ctx, "Node:%u is alive", node_id);
    }
    else
    {
        //Send request for failed node removal
        zwnet_exec_req_t     rm_failed_req = {0};

        rm_failed_req.node_id = node_id;
        rm_failed_req.action = EXEC_ACT_RMV_FAILED;
        debug_zwapi_msg(appl_ctx->plt_ctx, "Send NOP with tx status:%d", tx_sts);

        util_list_add(nw->nw_exec_mtx, &nw->nw_exec_req_hd,
                      (uint8_t *)&rm_failed_req, sizeof(zwnet_exec_req_t));
        plt_sem_post(nw->nw_exec_sem);
    }
}


/**
zwnet_restart_node_probe_cb - send NOP callback function
@param[in]	appl_ctx    The application layer context
@param[in]	tx_sts		The transmit complete status
@param[in]	user_prm    The user specific parameter
@param[in]	node_id     Node id
@param[in]	param       Additional parameters
@return
*/
static void  zwnet_restart_node_probe_cb(appl_layer_ctx_t *appl_ctx, int8_t tx_sts, void *user_prm, uint8_t node_id, zw_tx_cb_prm_t *param)
{
	unsigned    nodeid = (uintptr_t)user_prm;
    zwnet_p     nw = (zwnet_p)appl_ctx->data;
    zwnode_p    node;
    zwnoded_t   noded;

    if (nodeid == 0)
    {
        return;
    }

    plt_mtx_lck(nw->mtx);
    node = zwnode_find(&nw->ctl, nodeid);

    if (!node)
    {
        plt_mtx_ulck(nw->mtx);
        return;
    }

    zwnode_get_desc(node, &noded);

    plt_mtx_ulck(nw->mtx);

    if (tx_sts == TRANSMIT_COMPLETE_OK)
    {
        debug_zwapi_msg(appl_ctx->plt_ctx, "Node:%u is alive", nodeid);
        node->restart_cb(&noded, ZW_FW_UPDT_RESTART_OK);
    }
    else if (--node->poll_tgt_cnt > 0)
    {
        //Send request for another probe
        zwnet_exec_req_t     req = {0};

        req.node_id = nodeid;
        req.action = EXEC_ACT_PROBE_RESTART_NODE;

        debug_zwapi_msg(appl_ctx->plt_ctx, "zwnet_restart_node_probe_cb: Send NOP with tx status:%d", tx_sts);

        util_list_add(nw->nw_exec_mtx, &nw->nw_exec_req_hd,
                      (uint8_t *)&req, sizeof(zwnet_exec_req_t));
        plt_sem_post(nw->nw_exec_sem);
    }
    else
    {   //Report failure
        debug_zwapi_msg(appl_ctx->plt_ctx, "zwnet_restart_node_probe_cb: Send NOP with tx status:%d", tx_sts);
        node->restart_cb(&noded, ZW_FW_UPDT_RESTART_FAILED);
    }
}


static const char *rm_failed_status_msg[] = { "OK",
                                              "Node id not in failing list",
                                              "Failed",
                                              "The node is working properly",
                                              "unknown"

                                            };
/**
zwnet_rst_node_rm_cb - Remove self-reset node callback function
@param[in]	appl_ctx    The application layer context
@param[in]	sts		    Status
@param[in]	node_id     The node id of the removed node
@return
*/
static void zwnet_rst_node_rm_cb(appl_layer_ctx_t *appl_ctx, uint8_t sts, uint8_t node_id)
{
    int                 status;
    zwnet_p             nw = (zwnet_p)appl_ctx->data;
    zwnet_exec_req_t    rm_failed_req = {0};

    if (sts == ZW_FAILED_NODE_REMOVED)
    {
        status = 0;
    }
    else if (sts == ZW_FAILED_NODE_NOT_FOUND)
    {
        status = 1;
    }
    else if (sts == ZW_FAILED_NODE_REMOVE_FAIL || sts == ZW_FAILED_NODE_NOT_REMOVED)
    {
        status = 2;
    }
    else if (sts == ZW_NODE_OK)
    {
        status = 3;
    }
    else
    {
        status = 4;
    }

    debug_zwapi_msg(&nw->plt_ctx, "zwnet_rst_node_rm_cb: status:%u(%s), node id:%u", sts,
                    rm_failed_status_msg[status], node_id);

    if (sts == ZW_FAILED_NODE_REMOVED)
    {
        //Node was removed, call back the original routine
        zwnet_failed_id_rm_cb(appl_ctx, sts, node_id);
        return;
    }

    zwnet_abort(nw);

    //Retry to remove the node
    debug_zwapi_msg(&nw->plt_ctx, "Retrying ...");
    rm_failed_req.node_id = node_id;
    rm_failed_req.action = EXEC_ACT_RMV_FAILED_RETRY;
    util_list_add(nw->nw_exec_mtx, &nw->nw_exec_req_hd,
                  (uint8_t *)&rm_failed_req, sizeof(zwnet_exec_req_t));
    plt_sem_post(nw->nw_exec_sem);

}


/**
zwnet_wunmi_tmout_cb - Wakeup no more info timeout callback
@param[in] data     Pointer to context
@return
*/
static void zwnet_wunmi_tmout_cb(void *data)
{
    zwnet_exec_wunmi_t  *wunmi_ctx = (zwnet_exec_wunmi_t *)data;

    //Stop send timer
    plt_mtx_lck(wunmi_ctx->nw->mtx);
    plt_tmr_stop(&wunmi_ctx->nw->plt_ctx, wunmi_ctx->tmr_ctx);
    wunmi_ctx->timeout = 1;
    wunmi_ctx->tmr_ctx = NULL;
    plt_mtx_ulck(wunmi_ctx->nw->mtx);

}


/**
zwnet_exec_thrd - Thread to execute certain operations
@param[in]	data		Network
@return
@note Operation submitted to this thread MUST NOT be time-sensitive because the thread may be sleeping at the time
      the operation is submitted.  Also, the operation is executed based on first-in-first-out basis.
*/
static void zwnet_exec_thrd(void *data)
{
    int                 result;
    zwnet_p             nw = (zwnet_p)data;
    zwnode_p            node;
    util_lst_t          *req_lst;
    zwnet_exec_req_t    *exec_req;
    uint8_t             node_id;

    nw->nw_exec_thrd_sts = 1;
    while (1)
    {
        //Wait for a request
        plt_sem_wait(nw->nw_exec_sem);

        //Check whether to exit the thread
        if (nw->nw_exec_thrd_run == 0)
        {
            nw->nw_exec_thrd_sts = 0;
            return;
        }

        while ((req_lst = util_list_get(nw->nw_exec_mtx, &nw->nw_exec_req_hd)) != NULL)
        {
            exec_req = (zwnet_exec_req_t *)req_lst->wr_buf;

            //Check whether to exit the thread
            if (nw->nw_exec_thrd_run == 0)
            {
                zwnet_exec_req_free(exec_req);
                free(req_lst);
                plt_sleep(2);//Act as memory barrier
                nw->nw_exec_thrd_sts = 0;
                return;
            }

            //Get the node id of the request
            node_id = exec_req->node_id;

            switch (exec_req->action)
            {
                //-------------------------------------------------
                case EXEC_ACT_PROBE_FAILED_NODE:
                //-------------------------------------------------
                    //Sleep to allow the sender of DEVICE_RESET_LOCALLY_NOTIFICATION
                    //to reset the device
                    plt_sleep(3000);

                    plt_mtx_lck(nw->mtx);

                    node = zwnode_find(&nw->ctl, node_id);

                    if (node)
                    {
                        //Check for any network operation going on
                        if (nw->curr_op != ZWNET_OP_NONE)
                        {   //Busy, re-schedule the send NOP operation
                            plt_mtx_ulck(nw->mtx);
                            util_list_add(nw->nw_exec_mtx, &nw->nw_exec_req_hd,
                                          (uint8_t *)exec_req, sizeof(zwnet_exec_req_t));
                            break;
                        }

						result = zwnode_probe(node, zwnet_node_probe_cb, (void *)((uintptr_t)node->nodeid));
                        if (result < 0)
                        {
                            if (result == ZW_ERR_BUSY)
                            {   //Send wait failed, re-schedule the send NOP operation
                                util_list_add(nw->nw_exec_mtx, &nw->nw_exec_req_hd,
                                              (uint8_t *)exec_req, sizeof(zwnet_exec_req_t));
                            }
                        }
                    }

                    plt_mtx_ulck(nw->mtx);
                    break;

                //-------------------------------------------------
                case EXEC_ACT_RMV_FAILED:
                case EXEC_ACT_RMV_FAILED_RETRY:
                //-------------------------------------------------
                    if (exec_req->action == EXEC_ACT_RMV_FAILED_RETRY)
                    {
                        //Wait for 8 seconds
                        plt_sleep(8000);
                        //Check whether to exit the thread
                        if (nw->nw_exec_thrd_run == 0)
                        {
                            break;
                        }
                    }

                    plt_mtx_lck(nw->mtx);

                    node = zwnode_find(&nw->ctl, node_id);

                    if (node)
                    {
                        //Check for any network operation going on
                        if (nw->curr_op != ZWNET_OP_NONE)
                        {   //Busy, re-schedule the removed failed node operation
                            plt_mtx_ulck(nw->mtx);
                            util_list_add(nw->nw_exec_mtx, &nw->nw_exec_req_hd,
                                          (uint8_t *)exec_req, sizeof(zwnet_exec_req_t));
                            plt_sleep(3000);
                            break;
                        }

                        if (!(nw->ctl_role & ZWNET_CTLR_ROLE_INCL))
                        {   //Non-inclusion controller can't remove node
                            plt_mtx_ulck(nw->mtx);
                            break;
                        }

                        //Remove failed node from the protocol layer failed node ID list
                        if (zw_remove_failed_node_id(&nw->appl_ctx, zwnet_rst_node_rm_cb, node->nodeid) == 0)
                        {
                            nw->curr_op = ZWNET_OP_RM_FAILED_ID;
                            nw->failed_id = node->nodeid;
                        }
                        else
                        {   //Can't send command to Z/IP gateway
                            //re-schedule the removed failed node operation
                            plt_mtx_ulck(nw->mtx);
                            util_list_add(nw->nw_exec_mtx, &nw->nw_exec_req_hd,
                                          (uint8_t *)exec_req, sizeof(zwnet_exec_req_t));
                            plt_sleep(3000);
                            break;
                        }

                    }

                    plt_mtx_ulck(nw->mtx);
                    break;

                //-------------------------------------------------
                case EXEC_ACT_PROBE_RESTART_NODE:
                //-------------------------------------------------
                {
                    zwnoded_t noded;

                    plt_sleep(FW_UPDT_RESTART_POLL_INTERVAL);

                    plt_mtx_lck(nw->mtx);

                    node = zwnode_find(&nw->ctl, node_id);

                    if (node)
                    {
                        //Check for any network operation going on
                        if (nw->curr_op != ZWNET_OP_NONE)
                        {   //Busy, re-schedule the send NOP operation
                            plt_mtx_ulck(nw->mtx);
                            util_list_add(nw->nw_exec_mtx, &nw->nw_exec_req_hd,
                                          (uint8_t *)exec_req, sizeof(zwnet_exec_req_t));
                            break;
                        }

						result = zwnode_probe(node, zwnet_restart_node_probe_cb, (void *)((uintptr_t)node_id));
                        if (result == 0)
                        {   //o.k.
                            plt_mtx_ulck(nw->mtx);
                            break;
                        }
                        else if (result == ZW_ERR_BUSY)
                        {   //Re-schedule the send NOP operation
                            plt_mtx_ulck(nw->mtx);
                            util_list_add(nw->nw_exec_mtx, &nw->nw_exec_req_hd,
                                          (uint8_t *)exec_req, sizeof(zwnet_exec_req_t));
                            break;
                        }
                    }
                    else
                    {
                        plt_mtx_ulck(nw->mtx);
                        break;
                    }

                    result = zwnode_get_desc(node, &noded);
                    plt_mtx_ulck(nw->mtx);
                    if ((result == 0) && node->restart_cb)
                    {
                        //Report failure
                        node->restart_cb(&noded, ZW_FW_UPDT_RESTART_FAILED);
                    }

                }
                break;

                //-------------------------------------------------
                case EXEC_ACT_ADD_NODE_STS_CB:
                //-------------------------------------------------
                    if (exec_req->retry_cnt++ < 20)
                    {
                        //Wait for 0.5 second
                        plt_sleep(500);
                    }
                    else
                    {
                        //Wait for 1 second
                        plt_sleep(1000);
                    }

                    //Check whether to exit the thread
                    if (nw->nw_exec_thrd_run == 0)
                    {
                        break;
                    }

                    plt_mtx_lck(nw->mtx);
                    if (!((nw->curr_op == ZWNET_OP_ADD_NODE || nw->curr_op == ZWNET_OP_RP_NODE) && nw->incl_on_behalf))
                    {
                        //Cancel low priority operation
                        if (nw->curr_op == ZWNET_OP_NODE_CACHE_UPT)
                        {
                            zwnet_abort(nw);
                            //debug_zwapi_msg(&nw->plt_ctx, "Canceled low priority operation");
                        }

                        //No other operation, enter into inclusion on-behalf mode
                        if (nw->curr_op == ZWNET_OP_NONE)
                        {
                            appl_node_add_cb_prm_t  cb_prm;
                            zwnet_exec_req_addS2_t  *add_req = (zwnet_exec_req_addS2_t *)exec_req->user_dat;

                            nw->curr_op = ZWNET_OP_ADD_NODE;
                            nw->incl_on_behalf = IOB_UNSOLICITED;
                            plt_mtx_ulck(nw->mtx);

                            debug_zwapi_msg(&nw->plt_ctx, "Enter into unsolicited inclusion on-behalf mode");
                            cb_prm.rpt_type = NODE_ADD_STATUS;
                            cb_prm.rpt_prm.node_info = add_req->add_dat;
                            cb_prm.flag = add_req->rcx_flag;

                            zwnet_node_add_rplc_hdlr(&nw->appl_ctx, &cb_prm);

                            break;
                        }
                        else
                        {   //Busy, re-schedule the operation
                            plt_mtx_ulck(nw->mtx);
                            debug_zwapi_msg(&nw->plt_ctx, "Busy, delay zwnet_node_add_rplc_hdlr");
                            util_list_add(nw->nw_exec_mtx, &nw->nw_exec_req_hd,
                                          (uint8_t *)exec_req, sizeof(zwnet_exec_req_t));
                            exec_req->user_dat = NULL;  //prevent user data being freed since it is re-used in re-schedule operation
                            break;
                        }
                    }
                    plt_mtx_ulck(nw->mtx);
                    break;

                //-------------------------------------------------
                case EXEC_ACT_WKUP_NO_MORE_INFO:
                //-------------------------------------------------
                {
                    if (nw->dflt_wunmi_dly == 0)
                    {
                        break;
                    }

                    if (exec_req->user_dat)
                    {   //Existing entry
                        zwnet_exec_wunmi_t  *wunmi_ctx = (zwnet_exec_wunmi_t *)exec_req->user_dat;
                        int                 time_out = 0;

                        plt_mtx_lck(nw->mtx);
                        if (wunmi_ctx->timeout)
                        {
                            time_out = 1;

                            node = zwnode_find(&nw->ctl, node_id);

                            if (node)
                            {
                                zwifd_t     ifd;
                                zwif_p      intf;
                                uint8_t     resp[] = { COMMAND_CLASS_WAKE_UP, WAKE_UP_NO_MORE_INFORMATION };

                                intf = zwif_find_cls(node->ep.intf, COMMAND_CLASS_WAKE_UP);

                                if (intf)
                                {
                                    //Send the response
                                    zwif_get_desc(intf, &ifd);
                                    zwif_exec_ex(&ifd, resp, 2, zwif_tx_sts_cb, NULL, ZWIF_OPT_SKIP_MUL_Q, NULL);

                                }
                            }
                        }
                        plt_mtx_ulck(nw->mtx);

                        if (!time_out)
                        {   //Reschedule poll for timeout
                            util_list_add(nw->nw_exec_mtx, &nw->nw_exec_req_hd,
                                          (uint8_t *)exec_req, sizeof(zwnet_exec_req_t));
                            exec_req->user_dat = NULL;  //prevent user data being freed since it is re-used in reschedule poll
                            plt_sleep(970);
                        }
                    }
                    else
                    {   //New entry
                        zwnet_exec_wunmi_t  *wunmi_ctx;

                        wunmi_ctx = (zwnet_exec_wunmi_t *)malloc(sizeof(zwnet_exec_wunmi_t));

                        if (!wunmi_ctx)
                        {
                            break;
                        }

                        exec_req->user_dat = wunmi_ctx;

                        wunmi_ctx->timeout = 0;
                        wunmi_ctx->nw = nw;
                        wunmi_ctx->tmr_ctx = plt_tmr_start(&nw->plt_ctx, nw->dflt_wunmi_dly * 1000, zwnet_wunmi_tmout_cb, wunmi_ctx);

                        if (wunmi_ctx->tmr_ctx)
                        {   //Reschedule poll for timeout
                            util_list_add(nw->nw_exec_mtx, &nw->nw_exec_req_hd,
                                          (uint8_t *)exec_req, sizeof(zwnet_exec_req_t));
                            exec_req->user_dat = NULL;  //prevent user data being freed since it is re-used in reschedule poll
                            plt_sleep(970);
                        }
                    }
                }
                break;

                //-------------------------------------------------
                case EXEC_ACT_RPLC_NODE_STS_CB:
                //-------------------------------------------------
                    if (exec_req->retry_cnt++ < 20)
                    {
                        //Wait for 0.5 second
                        plt_sleep(500);
                    }
                    else
                    {
                        //Wait for 1 second
                        plt_sleep(1000);
                    }

                    //Check whether to exit the thread
                    if (nw->nw_exec_thrd_run == 0)
                    {
                        break;
                    }

                    plt_mtx_lck(nw->mtx);
                    if (!((nw->curr_op == ZWNET_OP_ADD_NODE || nw->curr_op == ZWNET_OP_RP_NODE) && nw->incl_on_behalf))
                    {
                        //Cancel low priority operation
                        if (nw->curr_op == ZWNET_OP_NODE_CACHE_UPT)
                        {
                            zwnet_abort(nw);
                            //debug_zwapi_msg(&nw->plt_ctx, "Canceled low priority operation");
                        }

                        //No other operation, enter into inclusion on-behalf mode
                        if (nw->curr_op == ZWNET_OP_NONE)
                        {
                            appl_node_add_cb_prm_t  cb_prm;
                            zwnet_exec_req_rpS2_t   *rplc_req = (zwnet_exec_req_rpS2_t *)exec_req->user_dat;

                            nw->curr_op = ZWNET_OP_RP_NODE;
                            nw->incl_on_behalf = IOB_UNSOLICITED;
                            plt_mtx_ulck(nw->mtx);

                            debug_zwapi_msg(&nw->plt_ctx, "Enter into unsolicited replace on-behalf mode");
                            cb_prm.rpt_type = FAILED_NODE_REPLACE_STATUS;
                            cb_prm.rpt_prm.node_rp_info = rplc_req->rp_dat;
                            cb_prm.flag = rplc_req->rcx_flag;

                            zwnet_node_add_rplc_hdlr(&nw->appl_ctx, &cb_prm);

                            break;
                        }
                        else
                        {   //Busy, re-schedule the operation
                            plt_mtx_ulck(nw->mtx);
                            debug_zwapi_msg(&nw->plt_ctx, "Busy, delay zwnet_node_add_rplc_hdlr");
                            util_list_add(nw->nw_exec_mtx, &nw->nw_exec_req_hd,
                                          (uint8_t *)exec_req, sizeof(zwnet_exec_req_t));
                            exec_req->user_dat = NULL;  //prevent user data being freed since it is re-used in re-schedule operation
                            break;
                        }
                    }
                    plt_mtx_ulck(nw->mtx);
                    break;

            }

            zwnet_exec_req_free(exec_req);
            free(req_lst);
            //Check whether to exit the thread
            if (nw->nw_exec_thrd_run == 0)
            {
                plt_sleep(2);//Act as memory barrier
                nw->nw_exec_thrd_sts = 0;
                return;
            }
        }
    }
}


/**
zwnet_sup_cached_cb_rpt - Callback user application using cached interface data
@param[in]	nw		Network
@param[in]	cb      Callback function
@param[in]	cb_req  Request of the supported report callback
@return
*/
static void zwnet_sup_cached_cb_rpt(zwnet_p nw, void *cb, zwnet_sup_rpt_cb_req_t *cb_req)
{
    zwif_p  intf;

    if (!cb)
        return;

    //Check for cached data
    plt_mtx_lck(nw->mtx);
    intf = zwif_get_if(&cb_req->ifd);
    switch (cb_req->rpt_type)
    {
            //-------------------------------------------------
        case CB_RPT_TYP_THRMO_FAN_MD:
            //-------------------------------------------------
            {
                if_thrmo_fan_mode_data_t    *thrmo_fan_mode_dat;
                zwrep_thrmo_fan_md_sup_fn   rpt_cb = (zwrep_thrmo_fan_md_sup_fn)cb;

                if (intf && intf->data_cnt)
                {   //Have cache
                    thrmo_fan_mode_dat = (if_thrmo_fan_mode_data_t *)malloc(intf->data_item_sz);

                    if (thrmo_fan_mode_dat)
                    {
                        memcpy(thrmo_fan_mode_dat, intf->data, intf->data_item_sz);

                        plt_mtx_ulck(nw->mtx);
                        rpt_cb(&cb_req->ifd, thrmo_fan_mode_dat->off, thrmo_fan_mode_dat->mode_len, thrmo_fan_mode_dat->mode, 1);
                        free(thrmo_fan_mode_dat);
                    }
                    else
                    {
                        plt_mtx_ulck(nw->mtx);
                    }
                }
                else
                {   //No cache
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 0, 0, NULL, 0);
                }
            }
            break;

            //-------------------------------------------------
        case CB_RPT_TYP_THRMO_MD:
            //-------------------------------------------------
            {
                if_thrmo_mode_data_t    *thrmo_mode_dat;
                zwrep_thrmo_md_sup_fn   rpt_cb = (zwrep_thrmo_md_sup_fn)cb;

                if (intf && intf->data_cnt)
                {   //Have cache
                    thrmo_mode_dat = (if_thrmo_mode_data_t *)malloc(intf->data_item_sz);

                    if (thrmo_mode_dat)
                    {
                        memcpy(thrmo_mode_dat, intf->data, intf->data_item_sz);

                        plt_mtx_ulck(nw->mtx);
                        rpt_cb(&cb_req->ifd, thrmo_mode_dat->mode_len, thrmo_mode_dat->mode, 1);
                        free(thrmo_mode_dat);
                    }
                    else
                    {
                        plt_mtx_ulck(nw->mtx);
                    }
                }
                else
                {   //No cache
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 0, NULL, 0);
                }
            }
            break;

            //-------------------------------------------------
        case CB_RPT_TYP_THRMO_SETPOINT:
            //-------------------------------------------------
            {
                if_thrmo_setp_data_t    *thrmo_setp_dat;
                zwrep_thrmo_setp_sup_fn rpt_cb = (zwrep_thrmo_setp_sup_fn)cb;

                if (intf && intf->data_cnt)
                {   //Have cache
                    if_thrmo_setp_cap_t *thrmo_setp_cap;
                    int                 i;

                    thrmo_setp_cap = (if_thrmo_setp_cap_t *)intf->data;

                    thrmo_setp_dat = (if_thrmo_setp_data_t *)malloc(sizeof(if_thrmo_setp_data_t) + thrmo_setp_cap->setp_cnt);

                    if (thrmo_setp_dat)
                    {
                        thrmo_setp_dat->setp_len = thrmo_setp_cap->setp_cnt;

                        for (i=0; i<thrmo_setp_cap->setp_cnt; i++)
                        {
                            thrmo_setp_dat->setp[i] = thrmo_setp_cap->setp_cap[i].min.type;
                        }

                        plt_mtx_ulck(nw->mtx);
                        rpt_cb(&cb_req->ifd, thrmo_setp_dat->setp_len, thrmo_setp_dat->setp, 1);
                        free(thrmo_setp_dat);
                        return;
                    }
                }
                //No cache or no memory
                plt_mtx_ulck(nw->mtx);
                rpt_cb(&cb_req->ifd, 0, NULL, 0);
            }
            break;

            //-------------------------------------------------
        case CB_RPT_TYP_MAX_GROUP:
            //-------------------------------------------------
            {
                uint8_t             max_grp;
                zwrep_group_sup_fn  rpt_cb = (zwrep_group_sup_fn)cb;

                if (intf && intf->data_cnt)
                {   //Have cache
                    max_grp = *((uint8_t *)intf->data);

                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, max_grp, 1);
                }
                else
                {   //No cache
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 0, 0);
                }

            }
            break;

            //-------------------------------------------------
        case CB_RPT_TYP_MUL_SWITCH:
            //-------------------------------------------------
            {
                if_mul_switch_data_t    mul_switch_data;
                zwrep_lvl_sup_fn        rpt_cb = (zwrep_lvl_sup_fn)cb;

                if (intf && intf->data_cnt)
                {   //Have cache
                    mul_switch_data = *((if_mul_switch_data_t *)intf->data);

                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, mul_switch_data.pri_type, mul_switch_data.sec_type, 1);
                }
                else
                {   //No cache
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 0, 0, 0);
                }

            }
            break;

            //-------------------------------------------------
        case CB_RPT_TYP_AV:
            //-------------------------------------------------
            {
                uint16_t    length;
                uint8_t     *mask;
                zwrep_av_fn rpt_cb = (zwrep_av_fn)cb;

                if (intf && intf->data_cnt)
                {   //Have cache
                    mask = (uint8_t *)malloc(intf->data_item_sz);

                    if (mask)
                    {
                        memcpy(mask, intf->data, intf->data_item_sz);
                        length = intf->data_item_sz;

                        plt_mtx_ulck(nw->mtx);
                        rpt_cb(&cb_req->ifd, length, mask, 1);
                        free(mask);
                    }
                    else
                    {
                        plt_mtx_ulck(nw->mtx);
                    }
                }
                else
                {   //No cache
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 0, NULL, 0);
                }
            }
            break;

            //-------------------------------------------------
        case CB_RPT_TYP_ALARM_TYPE:
            //-------------------------------------------------
            {
                int                 i;
                uint8_t             ztype[248];
                if_alarm_data_t     *alarm_dat;
                zwrep_alrm_sup_fn   rpt_cb = (zwrep_alrm_sup_fn)cb;

                if (intf && intf->data_cnt)
                {   //Have cache
                    alarm_dat = (if_alarm_data_t *)malloc(intf->data_item_sz);

                    if (alarm_dat)
                    {
                        memcpy(alarm_dat, intf->data, intf->data_item_sz);

                        for (i=0; i<alarm_dat->type_evt_cnt; i++)
                        {
                            ztype[i] = alarm_dat->type_evt[i].ztype;
                        }

                        plt_mtx_ulck(nw->mtx);
                        rpt_cb(&cb_req->ifd, alarm_dat->have_vtype, i, ztype, 1);
                        free(alarm_dat);
                    }
                    else
                    {
                        plt_mtx_ulck(nw->mtx);
                    }
                }
                else
                {   //No cache
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 0, 0, NULL, 0);
                }
            }
            break;

            //-------------------------------------------------
        case CB_RPT_TYP_ALARM_EVENT:
            //-------------------------------------------------
            {
                int                 i;
                int                 j;
                int                 found = 0;
                int                 max_evt;
                if_alarm_data_t     *alarm_dat;
                zwrep_alrm_evt_fn   rpt_cb = (zwrep_alrm_evt_fn)cb;
                uint8_t             *evt_bitmask;
                uint8_t             sup_evt[248];
                uint8_t             evt_len;
                uint8_t             ztype;

                if (intf && intf->data_cnt)
                {   //Have cache
                    alarm_dat = (if_alarm_data_t *)intf->data;

                    ztype = (uint8_t)cb_req->extra;

                    //Find matching alarm type in cache
                    for (i=0; i<alarm_dat->type_evt_cnt; i++)
                    {
                        if (alarm_dat->type_evt[i].ztype == ztype)
                        {   //Found alarm type
                            //Check whether there is an event cache
                            evt_len = alarm_dat->type_evt[i].evt_len;
                            if (evt_len)
                            {
                                found = 1;
                                if (evt_len > MAX_EVT_BITMASK_LEN)
                                {
                                    evt_len = MAX_EVT_BITMASK_LEN;
                                }
                                max_evt = evt_len * 8;
                                evt_len = 0;
                                evt_bitmask = alarm_dat->type_evt[i].evt;

                                for (j=0; j < max_evt; j++)
                                {
                                    if ((evt_bitmask[(j>>3)] >> (j & 0x07)) & 0x01)
                                    {
                                        sup_evt[evt_len++] = j;
                                    }
                                }
                                break;
                            }
                        }
                    }

                    plt_mtx_ulck(nw->mtx);

                    if (found)
                    {
                        rpt_cb(&cb_req->ifd, ztype, evt_len, sup_evt, 1);
                    }
                    else
                    {
                        rpt_cb(&cb_req->ifd, 0, 0, NULL, 0);
                    }
                }
                else
                {   //No cache
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 0, 0, NULL, 0);
                }
            }
            break;

            //-------------------------------------------------
        case CB_RPT_TYP_PROT:
            //-------------------------------------------------
            {
                zwprot_sup_t        if_prot_data;
                zwrep_prot_sup_fn   rpt_cb = (zwrep_prot_sup_fn)cb;

                if (intf && intf->data_cnt)
                {   //Have cache
                    if_prot_data = *((zwprot_sup_t *)intf->data);

                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, &if_prot_data, 1);
                }
                else
                {   //No cache
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, NULL, 0);
                }

            }
            break;

            //-------------------------------------------------
        case CB_RPT_TYP_MAX_USR_CODES:
            //-------------------------------------------------
            {
                uint8_t             max_usr_codes;
                zwrep_usr_sup_fn    rpt_cb = (zwrep_usr_sup_fn)cb;

                if (intf && intf->data_cnt)
                {   //Have cache
                    max_usr_codes = *((uint8_t *)intf->data);

                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, max_usr_codes, 1);
                }
                else
                {   //No cache
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 0, 0);
                }

            }
            break;

            //-------------------------------------------------
        case CB_RPT_TYP_METER:
            //-------------------------------------------------
            {
                zwmeter_cap_t       if_meter_data;
                zwrep_meter_sup_fn  rpt_cb = (zwrep_meter_sup_fn)cb;

                if (intf && intf->data_cnt)
                {   //Have cache
                    if_meter_data = *((zwmeter_cap_t *)intf->data);

                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, &if_meter_data, 1);
                }
                else
                {   //No cache
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, NULL, 0);
                }

            }
            break;

            //-------------------------------------------------
        case CB_RPT_TYP_METER_DESC:
            //-------------------------------------------------
            {
                zwmeter_t        if_meter_data;
                zwrep_meterd_fn  rpt_cb = (zwrep_meterd_fn)cb;

                if (intf && intf->data_cnt)
                {   //Have cache
                    if_meter_data = *((zwmeter_t *)intf->data);

                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, &if_meter_data, 1);
                }
                else
                {   //No cache
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, NULL, 0);
                }

            }
            break;

            //-------------------------------------------------
        case CB_RPT_TYP_BIN_SENSOR:
            //-------------------------------------------------
            {
                uint8_t               snsr_type[248];
                uint8_t               snsr_cnt;
                zwrep_bsensor_sup_fn  rpt_cb = (zwrep_bsensor_sup_fn)cb;

                if (intf && intf->data_cnt)
                {   //Have cache
                    snsr_cnt = intf->data_cnt;
                    memcpy(snsr_type, intf->data, snsr_cnt);
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, snsr_cnt, snsr_type, 1);
                }
                else
                {   //No cache
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 0, NULL, 0);
                }

            }
            break;

            //-------------------------------------------------
        case CB_RPT_TYP_SENSOR:
            //-------------------------------------------------
            {
                uint8_t               snsr_type[248];
                uint8_t               snsr_cnt;
                uint8_t               i;
                if_sensor_data_t      *sup_snsr;
                zwrep_sensor_sup_fn   rpt_cb = (zwrep_sensor_sup_fn)cb;

                if (intf && intf->data_cnt)
                {   //Have cache
                    sup_snsr = (if_sensor_data_t *)intf->data;
                    snsr_cnt = intf->data_cnt;

                    for (i=0; i<snsr_cnt; i++)
                    {
                        snsr_type[i] = sup_snsr[i].sensor_type;
                    }
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, snsr_cnt, snsr_type, 1);
                }
                else
                {   //No cache
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 0, NULL, 0);
                }

            }
            break;

            //-------------------------------------------------
        case CB_RPT_TYP_SENSOR_UNIT:
            //-------------------------------------------------
            {
                uint8_t               snsr_type;
                uint8_t               snsr_cnt;
                uint8_t               sensor_unit_msk = 0;
                uint8_t               i;
                if_sensor_data_t      *sup_snsr;
                zwrep_sensor_unit_fn  rpt_cb = (zwrep_sensor_unit_fn)cb;

                if (intf && intf->data_cnt)
                {   //Have cache
                    sup_snsr = (if_sensor_data_t *)intf->data;
                    snsr_cnt = intf->data_cnt;
                    snsr_type = (uint8_t)cb_req->extra;

                    //Search for the sensor type
                    for (i=0; i<snsr_cnt; i++)
                    {
                        if (sup_snsr[i].sensor_type == snsr_type)
                        {
                            sensor_unit_msk = sup_snsr[i].sensor_unit;
                            break;
                        }
                    }
                    plt_mtx_ulck(nw->mtx);
                    if (sensor_unit_msk)
                    {
                        rpt_cb(&cb_req->ifd, snsr_type, sensor_unit_msk, 1);
                    }
                    else
                    {
                        rpt_cb(&cb_req->ifd, 0, 0, 0);
                    }
                }
                else
                {   //No cache
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 0, 0, 0);
                }

            }
            break;

            //-------------------------------------------------
        case CB_RPT_TYP_CENTRAL_SCENE:
            //-------------------------------------------------
            {
                if_csc_data_t         *csc_sup_dat;
                zwrep_csc_sup_fn      rpt_cb = (zwrep_csc_sup_fn)cb;

                if (intf && intf->data_cnt)
                {   //Have cache
                    csc_sup_dat = (if_csc_data_t *)intf->data;

                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd,
                           csc_sup_dat->scene_cnt,
                           csc_sup_dat->sameKA,
                           csc_sup_dat->KA_array_len,
                           csc_sup_dat->KA_array,
                           csc_sup_dat->slow_rfsh,
                           1);
                }
                else
                {   //No cache
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 0, 0, 0, NULL, 0, 0);
                }

            }
            break;

            //-------------------------------------------------
        case CB_RPT_TYP_ALARM_SENSOR:
            //-------------------------------------------------
            {
                uint8_t                 snsr_type[248];
                uint8_t                 snsr_cnt;
                zwrep_alrm_snsr_sup_fn  rpt_cb = (zwrep_alrm_snsr_sup_fn)cb;

                if (intf && intf->data_cnt)
                {   //Have cache
                    snsr_cnt = intf->data_cnt;
                    memcpy(snsr_type, intf->data, snsr_cnt);
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, snsr_cnt, snsr_type, 1);
                }
                else
                {   //No cache
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 0, NULL, 0);
                }

            }
            break;

            //-------------------------------------------------
        case CB_RPT_TYP_BAR_NOTIF_TYPE:
            //-------------------------------------------------
            {
                uint8_t                     bar_notif_type[256];
                uint8_t                     bar_notif_cnt;
                zwrep_barrier_notif_sup_fn  rpt_cb = (zwrep_barrier_notif_sup_fn)cb;

                if (intf && intf->data_cnt)
                {   //Have cache
                    bar_notif_cnt = intf->data_cnt;
                    memcpy(bar_notif_type, intf->data, bar_notif_cnt);
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, bar_notif_cnt, bar_notif_type, 1);
                }
                else
                {   //No cache
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 0, NULL, 0);
                }
            }
            break;

            //-------------------------------------------------
        case CB_RPT_TYP_COL_SWITCH:
            //-------------------------------------------------
            {
                uint8_t                color_sw_id[256];
                uint8_t                color_sw_cnt;
                zwrep_color_sw_sup_fn  rpt_cb = (zwrep_color_sw_sup_fn)cb;

                if (intf && intf->data_cnt)
                {   //Have cache
                    color_sw_cnt = intf->data_cnt;
                    memcpy(color_sw_id, intf->data, color_sw_cnt);
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, color_sw_cnt, color_sw_id, 1);
                }
                else
                {   //No cache
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 0, NULL, 0);
                }
            }
            break;

            //-------------------------------------------------
        case CB_RPT_TYP_THRMO_SETP_RANGE:
            //-------------------------------------------------
            {
                uint8_t                     setp_type;
                uint8_t                     i;
                if_thrmo_setp_cap_t         *thrmo_setp_cap;
                zwrep_thrmo_setp_range_fn   rpt_cb = (zwrep_thrmo_setp_range_fn)cb;
                zwsetp_range_t              setp_range;

                if (intf && intf->data_cnt)
                {   //Have cache
                    setp_type = (uint8_t)cb_req->extra;
                    thrmo_setp_cap = (if_thrmo_setp_cap_t *)intf->data;

                    //Search for the setpoint type
                    for (i=0; i<thrmo_setp_cap->setp_cnt; i++)
                    {
                        if (thrmo_setp_cap->setp_cap[i].min.type == setp_type)
                        {
                            if(thrmo_setp_cap->setp_cap[i].valid)
                            {
                                setp_range = thrmo_setp_cap->setp_cap[i];
                                plt_mtx_ulck(nw->mtx);
                                rpt_cb(&cb_req->ifd, setp_type, &setp_range.min, &setp_range.max, 1);
                                return;
                            }
                        }
                    }
                }
                //No cache
                plt_mtx_ulck(nw->mtx);
                rpt_cb(&cb_req->ifd, 0, NULL, NULL, 0);

            }
            break;

            //-------------------------------------------------
        case CB_RPT_TYP_THRMO_OP_STA_LOG:
            //-------------------------------------------------
            {
                uint8_t                         thrm_sta_log[256];
                uint8_t                         thrm_sta_cnt;
                zwrep_thrmo_op_sta_log_sup_fn   rpt_cb = (zwrep_thrmo_op_sta_log_sup_fn)cb;

                if (intf && intf->data_cnt)
                {   //Have cache
                    thrm_sta_cnt = intf->data_cnt;
                    memcpy(thrm_sta_log, intf->data, thrm_sta_cnt);
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, thrm_sta_cnt, thrm_sta_log, 1);
                }
                else
                {   //No cache
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 0, NULL, 0);
                }
            }
            break;

        default:
            plt_mtx_ulck(nw->mtx);

    }
}


/**
zwnet_snsr_cache_get - Get cached sensor value
@param[in]  intf        Interface
@param[in]	extra_cnt 	Extra parameter count
@param[in]	extra 	    Extra parameters. First parameter is type, second parameter is unit.
@param[out] snsr_value  Sensor value
@param[out] time_stamp  Time stamp
@return non-zero if cached data found; else return zero
*/
static int zwnet_snsr_cache_get(zwif_p intf, uint8_t extra_cnt, uint16_t *extra, zwsensor_t *snsr_value, time_t *time_stamp)
{
    int     i;
    int     ent_sz;     //entry size
    time_t  *tmstamp;
    uint8_t *cch_data;
    zwsensor_t *cch_snsr_value;

    ent_sz = sizeof(time_t) + sizeof(zwsensor_t);

    if (!intf->cch_data || (extra_cnt == 0))
    {
        return 0;
    }

    cch_data = intf->cch_data;

    //Find matching type and unit
    for (i=0; i<intf->cch_dat_cnt; i++)
    {
        tmstamp = (time_t *)(cch_data + (i * ent_sz));
        cch_snsr_value = (zwsensor_t *)(tmstamp + 1);
        //Check for type
        if (cch_snsr_value->type == extra[0])
        {   //If extra_cnt == 1 don't check unit; else check unit for matching
            if ((extra_cnt == 1) || (cch_snsr_value->unit == extra[1]))
            {
                //Found, retrieve cache report
                *time_stamp = *tmstamp;
                *snsr_value = *cch_snsr_value;
                return 1;
            }
        }
    }
    return 0;
}


/**
zwnet_thrmo_setp_cache_get - Get cached thermostat setpoint value
@param[in]  intf        Interface
@param[in]	extra_cnt 	Extra parameter count
@param[in]	extra 	    Extra parameters. First parameter is type, second parameter is unit.
@param[out] setp_val    Thermostat setpoint value
@param[out] time_stamp  Time stamp
@return non-zero if cached data found; else return zero
*/
static int zwnet_thrmo_setp_cache_get(zwif_p intf, uint8_t extra_cnt, uint16_t *extra, zwsetp_t *setp_val, time_t *time_stamp)
{
    int     i;
    int     ent_sz;     //entry size
    time_t  *tmstamp;
    uint8_t *cch_data;
    zwsetp_t *cch_setp_val;

    ent_sz = sizeof(time_t) + sizeof(zwsetp_t);

    if (!intf->cch_data || (extra_cnt != 1))
    {
        return 0;
    }

    cch_data = intf->cch_data;

    //Find matching type
    for (i=0; i<intf->cch_dat_cnt; i++)
    {
        tmstamp = (time_t *)(cch_data + (i * ent_sz));
        cch_setp_val = (zwsetp_t *)(tmstamp + 1);
        if (cch_setp_val->type == extra[0])
        {
            //Found, retrieve cache report
            *time_stamp = *tmstamp;
            *setp_val = *cch_setp_val;
            return 1;
        }
    }
    return 0;
}


/**
zwnet_bsnsr_cache_get - Get cached binary sensor value
@param[in]  intf        Interface
@param[in]	extra_cnt 	Extra parameter count
@param[in]	extra 	    Extra parameters. First parameter is type.
@param[out] snsr_value  Sensor value
@param[out] time_stamp  Time stamp
@return non-zero if cached data found; else return zero
*/
int zwnet_bsnsr_cache_get(zwif_p intf, uint8_t extra_cnt, uint16_t *extra, zwbsnsr_t *snsr_value, time_t *time_stamp)
{
    int         i;
    int         ent_sz;     //entry size
    time_t      *tmstamp;
    uint8_t     *cch_data;
    zwbsnsr_t   *cch_snsr_value;

    ent_sz = sizeof(time_t) + sizeof(zwbsnsr_t);

    if (!intf->cch_data || (extra_cnt != 1))
    {
        return 0;
    }

    cch_data = intf->cch_data;

    //Find matching type
    for (i=0; i<intf->cch_dat_cnt; i++)
    {
        tmstamp = (time_t *)(cch_data + (i * ent_sz));
        cch_snsr_value = (zwbsnsr_t *)(tmstamp + 1);
        if (cch_snsr_value->type == *extra)
        {
            //Found, retrieve cache report
            *time_stamp = *tmstamp;
            *snsr_value = *cch_snsr_value;
            return 1;
        }
    }

    return 0;
}


/**
zwnet_color_cache_get - Get cached color component value
@param[in]  intf        	Interface
@param[in]	extra_cnt 		Extra parameter count
@param[in]	extra 	    	Extra parameters. First parameter is id.
@param[out] color_sw_data  	Color switch value
@param[out] time_stamp  	Time stamp
@return non-zero if cached data found; else return zero
*/
int zwnet_color_cache_get(zwif_p intf, uint8_t extra_cnt, uint16_t *extra, zwcolor_t *color_sw_data, time_t *time_stamp)
{
    int         i;
    int         ent_sz;     //entry size
    time_t      *tmstamp;
    uint8_t     *cch_data;
    zwcolor_t   *cch_color_value;

    ent_sz = sizeof(time_t) + sizeof(zwcolor_t);

    if (!intf->cch_data || (extra_cnt != 1))
    {
        return 0;
    }

    cch_data = intf->cch_data;

    //Find matching id
    for (i=0; i<intf->cch_dat_cnt; i++)
    {
        tmstamp = (time_t *)(cch_data + (i * ent_sz));
        cch_color_value = (zwcolor_t *)(tmstamp + 1);
        if (cch_color_value->id == *extra)
        {
            //Found, retrieve cache report
            *time_stamp = *tmstamp;
            *color_sw_data = *cch_color_value;
            return 1;
        }
    }

    return 0;
}


/**
zwnet_meter_cache_get - Get cached meter value
@param[in]  intf        Interface
@param[in]	extra_cnt 	Extra parameter count
@param[in]	extra 	    Extra parameters
@param[out] meter_value Meter value
@param[out] time_stamp  Time stamp
@return non-zero if cached data found; else return zero
*/
static int zwnet_meter_cache_get(zwif_p intf, uint8_t extra_cnt, uint16_t *extra, zwmeter_dat_t *meter_value, time_t *time_stamp)
{
    int         i;
    int         ent_sz;     //entry size
    time_t      *tmstamp;
    uint8_t     *cch_data;
    zwmeter_dat_t   *cch_meter_value;

    ent_sz = sizeof(time_t) + sizeof(zwmeter_dat_t);

    if (!intf->cch_data || (extra_cnt != 1))
    {
        return 0;
    }

    cch_data = intf->cch_data;

    //Find matching unit
    for (i=0; i<intf->cch_dat_cnt; i++)
    {
        tmstamp = (time_t *)(cch_data + (i * ent_sz));
        cch_meter_value = (zwmeter_dat_t *)(tmstamp + 1);
        if (cch_meter_value->unit == *extra)
        {
            //Found, retrieve cache report
            *time_stamp = *tmstamp;
            *meter_value = *cch_meter_value;
            return 1;
        }
    }
    return 0;
}


/**
zwnet_alrm_cache_get - Get cached alarm report value
@param[in]  intf        Interface
@param[in]	extra_cnt 	Extra parameter count
@param[in]	extra 	    Extra parameters. First parameter is type, second parameter is unit.
@param[out] ppalrm_info  Double pointer of the alarm report. Create in this function and to be freed by the caller function
@param[out] time_stamp  Time stamp
@return non-zero if cached data found; else return zero
*/
static int zwnet_alrm_cache_get(zwif_p intf, uint8_t extra_cnt, uint16_t *extra, zwalrm_p *ppalrm_info, time_t *time_stamp)
{
    int         i;
    time_t      *tmstamp;
	cch_1lvl_t  *p1lvl = (cch_1lvl_t *)intf->cch_data;
    zwalrm_p	cch_alrm_value;
	uint8_t		bFound = 0;


	//extra_cnt should be 1 or 2 or 3
    if (!intf->cch_data || (extra_cnt < 1) || (extra_cnt > 3) || (intf->b2lvlcch != 1))
    {
        return 0;
    }

    for (i = 0; i < intf->cch_dat_cnt; i++, p1lvl++)
	{
		if(p1lvl && p1lvl->pcch_ele)
		{
			tmstamp = (time_t *)p1lvl->pcch_ele;
			cch_alrm_value = (zwalrm_p)(tmstamp + 1);

			if(extra_cnt == 1)
			{
				if((extra[0] == 0xFFFF) || (cch_alrm_value->type == extra[0]))
				{
					bFound = 1;
				}
			}
			else if(extra_cnt == 2)
			{
				if(((extra[0] == 0xFFFF) || (cch_alrm_value->type == extra[0]))
				   && (cch_alrm_value->ex_type == extra[1]))
				{
					bFound = 1;
				}
			}
			else if(extra_cnt == 3)
			{
				if(((extra[0] == 0xFFFF) || (cch_alrm_value->type == extra[0]))
			       && (cch_alrm_value->ex_type == extra[1])
			       && (cch_alrm_value->ex_event == extra[2]))
				{
					bFound = 1;
				}
			}

			if(bFound == 1)
			{
				//Found, retrieve cache report
				*ppalrm_info = calloc(1, p1lvl->cch_ele_sz - sizeof(time_t));
				if(*ppalrm_info)
				{
					*time_stamp = *tmstamp;
					memcpy(*ppalrm_info, cch_alrm_value, p1lvl->cch_ele_sz - sizeof(time_t));
                    return 1;
				}
                return 0; //No more memory
			}
		}
	}
    return 0;
}


/**
zwnet_alrm_snsr_cache_get - Get cached alarm sensor value
@param[in]  intf        Interface
@param[in]	extra_cnt 	Extra parameter count
@param[in]	extra 	    Extra parameters. First parameter is type.
@param[out] snsr_value  Sensor value
@param[out] time_stamp  Time stamp
@return non-zero if cached data found; else return zero
*/
static int zwnet_alrm_snsr_cache_get(zwif_p intf, uint8_t extra_cnt, uint16_t *extra, zw_alrm_snsr_t *snsr_value, time_t *time_stamp)
{
	int         i;
	int         ent_sz;     //entry size
	time_t      *tmstamp;
	uint8_t     *cch_data;
	zw_alrm_snsr_t   *cch_snsr_value;

	ent_sz = sizeof(time_t) + sizeof(zwbsnsr_t);

	if (!intf->cch_data || (extra_cnt != 1))
	{
		return 0;
	}

	cch_data = intf->cch_data;

	//Find matching type
	for (i = 0; i<intf->cch_dat_cnt; i++)
	{
		tmstamp = (time_t *)(cch_data + (i * ent_sz));
		cch_snsr_value = (zw_alrm_snsr_t *)(tmstamp + 1);
		if (cch_snsr_value->type == *extra)
		{
			//Found, retrieve cache report
			*time_stamp = *tmstamp;
			*snsr_value = *cch_snsr_value;
			return 1;
		}
	}

	return 0;
}


/**
zwnet_barrier_cache_get - Get cached barrier operator state/subsystem state
@param[in]  intf        Interface
@param[in]	extra_cnt 	Extra parameter count
@param[in]	extra 	    Extra parameters. First parameter is subsystem type.
@param[out] barrier_dat Barrier operator state/subsystem state data
@param[out] time_stamp  Time stamp
@return non-zero if cached data found; else return zero
*/
static int zwnet_barrier_cache_get(zwif_p intf, uint8_t extra_cnt, uint16_t *extra, zwbarrier_t *barrier_dat, time_t *time_stamp)
{
    int         i;
    int         ent_sz;     //entry size
    time_t      *tmstamp;
    uint8_t     *cch_data;
    zwbarrier_t *cch_barrier_dat;

    ent_sz = sizeof(time_t) + sizeof(zwbarrier_t);

    if (!intf->cch_data || (extra_cnt != 1))
    {
        return 0;
    }

    cch_data = intf->cch_data;

    //Find matching type
    for (i=0; i<intf->cch_dat_cnt; i++)
    {
        tmstamp = (time_t *)(cch_data + (i * ent_sz));
        cch_barrier_dat = (zwbarrier_t *)(tmstamp + 1);
        if (cch_barrier_dat->subsys_type == extra[0])
        {
            //Found, retrieve cache report
            *time_stamp = *tmstamp;
            *barrier_dat = *cch_barrier_dat;
            return 1;
        }
    }
    return 0;
}


/**
zwnet_thrmo_op_sta_log_cache_get - Get cached thermostat operating state logging
@param[in]  intf        Interface
@param[in]	extra_cnt 	Extra parameter count
@param[in]	extra 	    Extra parameters
@param[out] sta_log     Thermostat operating state log
@param[out] sta_log_cnt Thermostat operating state log count
@return non-zero if cached data found; else return zero
*/
static int zwnet_thrmo_op_sta_log_cache_get(zwif_p intf, uint8_t extra_cnt, uint16_t *extra, zwthrmo_op_sta_log_t *sta_log, uint8_t *sta_log_cnt)
{
    int                 i;
    zwthrmo_op_sta_t    *cch_log;
    zwthrmo_op_sta_t    *found_cch_log;
    uint8_t             cch_log_cnt;
    uint8_t             tmp_sta_log_cnt;

    *sta_log_cnt = 0;

    if (!intf->cch_data || (extra_cnt == 0))
    {
        return 0;
    }

    //The first item is reserved for operating state
    cch_log = (zwthrmo_op_sta_t *)intf->cch_data;
    cch_log++;

    //Find the state log and compare with the live state log
    cch_log_cnt = (intf->cch_dat_cnt > 1)? (intf->cch_dat_cnt - 1) : 0;

    if (cch_log_cnt == 0)
    {
        return 0;
    }

    tmp_sta_log_cnt = 0;

    //Find matching state log type
    for (i=0; i<extra_cnt; i++)
    {
        found_cch_log = zwif_thrmo_op_sta_cache_find(cch_log, cch_log_cnt, extra[i]);

        if (found_cch_log)
        {
            sta_log[tmp_sta_log_cnt].log_type = found_cch_log->log_type;
            sta_log[tmp_sta_log_cnt].today_hr = found_cch_log->today_hr;
            sta_log[tmp_sta_log_cnt].today_mn = found_cch_log->today_mn;
            sta_log[tmp_sta_log_cnt].prev_hr = found_cch_log->prev_hr;
            sta_log[tmp_sta_log_cnt].prev_mn = found_cch_log->prev_mn;
            sta_log[tmp_sta_log_cnt].tmstamp = found_cch_log->tmstamp;

            tmp_sta_log_cnt++;
        }
    }

    if (tmp_sta_log_cnt)
    {   //Cache is different from live report
        *sta_log_cnt = tmp_sta_log_cnt;
        return 1;
    }
    return 0;
}


/**
zwnet_dat_cached_cb_rpt - Callback user application using cached interface data
@param[in]	nw		Network
@param[in]	cb      Callback function
@param[in]	cb_req  Request of the report callback
@return
*/
static void zwnet_dat_cached_cb_rpt(zwnet_p nw, void *cb, zwnet_dat_rpt_cb_req_t *cb_req)
{
    zwif_p  intf;
    time_t  *tm_stamp_p;
    time_t  time_stamp = 0;

    //Check for cached data
    plt_mtx_lck(nw->mtx);
    intf = zwif_get_if(&cb_req->ifd);

    switch(cb_req->ifd.cls)
    {
        //-------------------------------------------------
        case COMMAND_CLASS_DOOR_LOCK:
        //-------------------------------------------------
            if (cb_req->zw_rpt == DOOR_LOCK_OPERATION_REPORT)
            {
                zwdlck_op_t         dlck_op = {0};
                zwrep_dlck_op_fn    rpt_cb = (zwrep_dlck_op_fn)cb;

                if (intf && intf->cch_dat_cnt)
                {
                    tm_stamp_p = (time_t *)intf->cch_data;
                    time_stamp = *tm_stamp_p;
                    memcpy(&dlck_op, tm_stamp_p + 1, sizeof(zwdlck_op_t));
                }
                plt_mtx_ulck(nw->mtx);
                rpt_cb(&cb_req->ifd, &dlck_op, time_stamp);
            }
            else if (cb_req->zw_rpt == DOOR_LOCK_CONFIGURATION_REPORT)
            {
                zwdlck_cfg_t         dlck_cfg = {0};
                zwrep_dlck_cfg_fn    rpt_cb = (zwrep_dlck_cfg_fn)cb;
                if_dlck_cch_data_t   *dlck_cch;

                if (intf && intf->cch_dat_cnt)
                {
                    dlck_cch = (if_dlck_cch_data_t *)(((time_t *)intf->cch_data) + 1);
                    time_stamp = dlck_cch->cfg_ts;
                    dlck_cfg = dlck_cch->cfg;
                }
                plt_mtx_ulck(nw->mtx);
                rpt_cb(&cb_req->ifd, &dlck_cfg, time_stamp);
            }
            break;

        //-------------------------------------------------
        case COMMAND_CLASS_SWITCH_BINARY:
        //-------------------------------------------------
            if (cb_req->zw_rpt == SWITCH_BINARY_REPORT)
            {
                zwswitch_t          sw_dat = {0};
                zwrep_switch_fn     rpt_cb = (zwrep_switch_fn)cb;

                if (intf && intf->cch_dat_cnt)
                {
                    tm_stamp_p = (time_t *)intf->cch_data;
                    time_stamp = *tm_stamp_p;
                    memcpy(&sw_dat, tm_stamp_p + 1, sizeof(zwswitch_t));
                }
                plt_mtx_ulck(nw->mtx);
                rpt_cb(&cb_req->ifd, &sw_dat, time_stamp);
            }
            break;

        //-------------------------------------------------
        case COMMAND_CLASS_SWITCH_MULTILEVEL:
        //-------------------------------------------------
            if (cb_req->zw_rpt == SWITCH_MULTILEVEL_REPORT)
            {
                zwlevel_dat_t   lvl_dat = {0};
                zwrep_level_fn  rpt_cb = (zwrep_level_fn)cb;

                if (intf && intf->cch_dat_cnt)
                {
                    tm_stamp_p = (time_t *)intf->cch_data;
                    time_stamp = *tm_stamp_p;
                    memcpy(&lvl_dat, tm_stamp_p + 1, sizeof(zwlevel_dat_t));
                }
                plt_mtx_ulck(nw->mtx);
                rpt_cb(&cb_req->ifd, &lvl_dat, time_stamp);
            }
            break;

        //-------------------------------------------------
        case COMMAND_CLASS_SWITCH_COLOR:
        //-------------------------------------------------
            if (cb_req->zw_rpt == SWITCH_COLOR_REPORT)
            {
				int                     cache_found = 0;
				zwcolor_t               color_sw_data = {0};
                zwrep_color_sw_get_fn   rpt_cb = (zwrep_color_sw_get_fn)cb;

                if (intf && intf->cch_dat_cnt)
                {
					if (cb_req->extra_cnt == 1)
					{
						cache_found = zwnet_color_cache_get(intf, 1, cb_req->extra, &color_sw_data, &time_stamp);
					}
                }
                plt_mtx_ulck(nw->mtx);
                if (!cache_found)
                {   //Return caller's parameter
                    if (cb_req->extra_cnt > 0)
                    {
						color_sw_data.id = (uint8_t)cb_req->extra[0];
                    }
                }
                rpt_cb(&cb_req->ifd, &color_sw_data, time_stamp);
            }
            break;

        //-------------------------------------------------
        case COMMAND_CLASS_SENSOR_MULTILEVEL:
        //-------------------------------------------------
            if (cb_req->zw_rpt == SENSOR_MULTILEVEL_REPORT)
            {
                int             cache_found = 0;
                zwsensor_t      snsr_value = {0};
                zwrep_sensor_fn rpt_cb = (zwrep_sensor_fn)cb;

                if (intf && intf->cch_dat_cnt)
                {
                    if (cb_req->extra_cnt == 2)
                    {
                        if (cb_req->extra[0] == 0)
                        {   //No type is specified, return the factory default sensor type
                            tm_stamp_p = (time_t *)intf->cch_data;
                            time_stamp = *tm_stamp_p;
                            memcpy(&snsr_value, tm_stamp_p + 1, sizeof(zwsensor_t));
                            cache_found = 1;
                        }
                        else
                        {
                            cache_found = zwnet_snsr_cache_get(intf, 1, cb_req->extra, &snsr_value, &time_stamp);
                        }
                    }
                    else if (cb_req->extra_cnt > 2)
                    {
                        cache_found = zwnet_snsr_cache_get(intf, cb_req->extra_cnt - 2, cb_req->extra, &snsr_value, &time_stamp);
                    }
                }
                plt_mtx_ulck(nw->mtx);

                if (!cache_found)
                {   //Return caller's parameters
                    if (cb_req->extra_cnt >= 2)
                    {
                        snsr_value.type = (uint8_t)cb_req->extra[cb_req->extra_cnt - 2];
						snsr_value.unit = (uint8_t)cb_req->extra[cb_req->extra_cnt - 1];
                    }
                }

                rpt_cb(&cb_req->ifd, &snsr_value, time_stamp);
            }
            break;

        //-------------------------------------------------
        case COMMAND_CLASS_THERMOSTAT_MODE:
        //-------------------------------------------------
            if (cb_req->zw_rpt == THERMOSTAT_MODE_REPORT)
            {
                zwthrmo_mode_t      thrmo_md;
                zwrep_thrmo_md_fn   rpt_cb = (zwrep_thrmo_md_fn)cb;

                if (intf && intf->cch_dat_cnt)
                {
                    tm_stamp_p = (time_t *)intf->cch_data;
                    time_stamp = *tm_stamp_p;
                    memcpy(&thrmo_md, tm_stamp_p + 1, sizeof(zwthrmo_mode_t));
                }
                plt_mtx_ulck(nw->mtx);
                rpt_cb(&cb_req->ifd, thrmo_md.mode, thrmo_md.manf_dat, thrmo_md.manf_dat_cnt, time_stamp);
            }
            break;

        //-------------------------------------------------
        case COMMAND_CLASS_THERMOSTAT_OPERATING_STATE:
        //-------------------------------------------------
            if (cb_req->zw_rpt == THERMOSTAT_OPERATING_STATE_REPORT)
            {
                uint8_t                 op_state = 0;
                zwthrmo_op_sta_t        *op_sta_cch;
                zwrep_thrmo_op_sta_fn   rpt_cb = (zwrep_thrmo_op_sta_fn)cb;

                if (intf && intf->cch_dat_cnt)
                {
                    op_sta_cch = (zwthrmo_op_sta_t *)intf->cch_data;
                    if (op_sta_cch->op_sta != 0xFF)
                    {
                        time_stamp = op_sta_cch->tmstamp;
                        op_state = op_sta_cch->op_sta;
                    }
                }
                plt_mtx_ulck(nw->mtx);
                rpt_cb(&cb_req->ifd, op_state, time_stamp);
            }
            else if (cb_req->zw_rpt == THERMOSTAT_OPERATING_STATE_LOGGING_REPORT_V2)
            {
                int                     cache_found = 0;
                zwthrmo_op_sta_log_t    sta_log[MAX_XTR_PRM_CNT];
                uint8_t                 sta_log_cnt = 0;
                zwrep_thrmo_op_sta_log_fn rpt_cb = (zwrep_thrmo_op_sta_log_fn)cb;

                if (intf && (intf->cch_dat_cnt > 1))
                {
                    cache_found = zwnet_thrmo_op_sta_log_cache_get(intf, cb_req->extra_cnt, cb_req->extra, sta_log, &sta_log_cnt);
                }
                plt_mtx_ulck(nw->mtx);
                if (!cache_found)
                {   //Return caller's parameters
                    if (cb_req->extra_cnt)
                    {   int i;
                        sta_log_cnt = cb_req->extra_cnt;
                        memset(sta_log, 0, sizeof(zwthrmo_op_sta_log_t) * sta_log_cnt);
                        for (i=0; i<sta_log_cnt; i++)
                        {
                            sta_log[i].log_type = (uint8_t)cb_req->extra[i];
                        }
                    }
                }

                rpt_cb(&cb_req->ifd, sta_log, sta_log_cnt, cache_found);
            }
            break;

        //-------------------------------------------------
        case COMMAND_CLASS_THERMOSTAT_SETPOINT:
        //-------------------------------------------------
            if (cb_req->zw_rpt == THERMOSTAT_SETPOINT_REPORT)
            {
                int                 cache_found = 0;
                zwsetp_t            setp_val = {0};
                zwrep_thrmo_setp_fn rpt_cb = (zwrep_thrmo_setp_fn)cb;

                if (intf && intf->cch_dat_cnt)
                {
                    cache_found = zwnet_thrmo_setp_cache_get(intf, cb_req->extra_cnt, cb_req->extra, &setp_val, &time_stamp);
                }
                plt_mtx_ulck(nw->mtx);
                if (!cache_found)
                {   //Return caller's parameter
                    if (cb_req->extra_cnt == 1)
                    {
						setp_val.type = (uint8_t)cb_req->extra[0];
                    }
                }

                rpt_cb(&cb_req->ifd, &setp_val, time_stamp);
            }
            break;

        //-------------------------------------------------
        case COMMAND_CLASS_SENSOR_BINARY:
        //-------------------------------------------------
            if (cb_req->zw_rpt == SENSOR_BINARY_REPORT)
            {
                int                 cache_found = 0;
                zwbsnsr_t           snsr_value = {0};
                zwrep_bsensor_fn    rpt_cb = (zwrep_bsensor_fn)cb;

                if (intf && intf->cch_dat_cnt)
                {
                    if (cb_req->extra_cnt == 1)
                    {   //No parameter specified, return default report
                        tm_stamp_p = (time_t *)intf->cch_data;
                        time_stamp = *tm_stamp_p;
                        memcpy(&snsr_value, tm_stamp_p + 1, sizeof(zwbsnsr_t));
                        cache_found = 1;
                    }
                    else if (cb_req->extra_cnt > 1)
                    {
                        cache_found = zwnet_bsnsr_cache_get(intf, cb_req->extra_cnt - 1, cb_req->extra, &snsr_value, &time_stamp);
                    }
                }
                plt_mtx_ulck(nw->mtx);
                if (!cache_found)
                {   //Return caller's parameter
                    if (cb_req->extra_cnt > 0)
                    {
						snsr_value.type = (uint8_t)cb_req->extra[cb_req->extra_cnt - 1];
                    }
                }
                rpt_cb(&cb_req->ifd, snsr_value.state, snsr_value.type, time_stamp);
            }
            break;

        //-------------------------------------------------
        case COMMAND_CLASS_METER:
        //-------------------------------------------------
            if (cb_req->zw_rpt == METER_REPORT)
            {
                int             cache_found = 0;
                zwmeter_dat_t   meter_value = {0};
                zwrep_meter_fn  rpt_cb = (zwrep_meter_fn)cb;

                if (intf && intf->cch_dat_cnt)
                {
                    if (cb_req->extra_cnt == 1)
                    {   //No parameter specified, return default report
                        tm_stamp_p = (time_t *)intf->cch_data;
                        time_stamp = *tm_stamp_p;
                        memcpy(&meter_value, tm_stamp_p + 1, sizeof(zwmeter_dat_t));
                        cache_found = 1;
                    }
                    else if (cb_req->extra_cnt > 1)
                    {
                        cache_found = zwnet_meter_cache_get(intf, cb_req->extra_cnt - 1, cb_req->extra, &meter_value, &time_stamp);
                    }
                }
                plt_mtx_ulck(nw->mtx);
                if (!cache_found)
                {   //Return caller's parameter
                    if (cb_req->extra_cnt > 0)
                    {
						meter_value.unit = (uint8_t)cb_req->extra[cb_req->extra_cnt - 1];
                    }
                }

                rpt_cb(&cb_req->ifd, &meter_value, time_stamp);
            }
            break;

        //-------------------------------------------------
        case COMMAND_CLASS_BATTERY:
        //-------------------------------------------------
            if (cb_req->zw_rpt == BATTERY_REPORT)
            {
                uint8_t     battery_lvl = 0;
                zwrep_ts_fn rpt_cb = (zwrep_ts_fn)cb;

                if (intf && intf->cch_dat_cnt)
                {
                    tm_stamp_p = (time_t *)intf->cch_data;
                    time_stamp = *tm_stamp_p;
                    memcpy(&battery_lvl, tm_stamp_p + 1, sizeof(uint8_t));
                }
                plt_mtx_ulck(nw->mtx);
                rpt_cb(&cb_req->ifd, battery_lvl, time_stamp);
            }
            break;

        //-------------------------------------------------
        case COMMAND_CLASS_BASIC:
        //-------------------------------------------------
            if (cb_req->zw_rpt == BASIC_REPORT)
            {
                zwbasic_t       basic_val = {0};
                zwrep_basic_fn  rpt_cb = (zwrep_basic_fn)cb;

                if (intf && intf->cch_dat_cnt)
                {
                    tm_stamp_p = (time_t *)intf->cch_data;
                    time_stamp = *tm_stamp_p;
                    memcpy(&basic_val, tm_stamp_p + 1, sizeof(zwbasic_t));
                }
                plt_mtx_ulck(nw->mtx);
                rpt_cb(&cb_req->ifd, &basic_val, time_stamp);
            }
            break;

        //-------------------------------------------------
        case COMMAND_CLASS_THERMOSTAT_FAN_MODE:
        //-------------------------------------------------
            if (cb_req->zw_rpt == THERMOSTAT_FAN_MODE_REPORT)
            {
                uint16_t                fan_md_cch = 0;
                zwrep_thrmo_fan_md_fn   rpt_cb = (zwrep_thrmo_fan_md_fn)cb;

                if (intf && intf->cch_dat_cnt)
                {
                    tm_stamp_p = (time_t *)intf->cch_data;
                    time_stamp = *tm_stamp_p;
                    memcpy(&fan_md_cch, tm_stamp_p + 1, sizeof(uint16_t));
                }
                plt_mtx_ulck(nw->mtx);
                rpt_cb(&cb_req->ifd, (fan_md_cch >> 8), (fan_md_cch & 0x00FF), time_stamp);
            }
            break;

        //-------------------------------------------------
        case COMMAND_CLASS_THERMOSTAT_FAN_STATE:
        //-------------------------------------------------
            if (cb_req->zw_rpt == THERMOSTAT_FAN_STATE_REPORT)
            {
                uint8_t                 fan_state = 0;
                zwrep_thrmo_fan_sta_fn  rpt_cb = (zwrep_thrmo_fan_sta_fn)cb;

                if (intf && intf->cch_dat_cnt)
                {
                    tm_stamp_p = (time_t *)intf->cch_data;
                    time_stamp = *tm_stamp_p;
                    memcpy(&fan_state, tm_stamp_p + 1, sizeof(uint8_t));
                }
                plt_mtx_ulck(nw->mtx);
                rpt_cb(&cb_req->ifd, fan_state, time_stamp);
            }
            break;

		 //-------------------------------------------------
        case COMMAND_CLASS_ALARM:
        //-------------------------------------------------
            if (cb_req->zw_rpt == ALARM_REPORT)
            {
                int             cache_found = 0;
				zwalrm_p		alrm_info = NULL;
                zwalrm_t        alrm_info_struct = {0};
                zwrep_alrm_fn	rpt_cb = (zwrep_alrm_fn)cb;

				if (intf && intf->cch_dat_cnt)
                {
                    if (cb_req->extra_cnt > 3)
                    {
                        cache_found = zwnet_alrm_cache_get(intf, cb_req->extra_cnt - 3, cb_req->extra, &alrm_info, &time_stamp);
                    }
                }

                plt_mtx_ulck(nw->mtx);

                if (!cache_found)
                {   //Return caller's parameter
                    if (cb_req->extra_cnt >= 3)
                    {
                        alrm_info = &alrm_info_struct;
                        alrm_info->ex_info = 1;
						alrm_info->type = (uint8_t)(cb_req->extra[cb_req->extra_cnt - 3] & 0xFF);
						alrm_info->ex_type = (uint8_t)cb_req->extra[cb_req->extra_cnt - 2];
						alrm_info->ex_event = (uint8_t)cb_req->extra[cb_req->extra_cnt - 1];
                    }
                }

                rpt_cb(&cb_req->ifd, alrm_info, time_stamp);

				if(alrm_info && (alrm_info != &alrm_info_struct))
					free(alrm_info);
            }
            break;

		//-------------------------------------------------
		case COMMAND_CLASS_CENTRAL_SCENE:
		//-------------------------------------------------
			//Actually this might never be used, after all, there is no Get command
			//in this CC. So actually no one can ask for the cached report.
			if (cb_req->zw_rpt == CENTRAL_SCENE_NOTIFICATION)
			{
				zwcsc_notif_t   csc_dat = { 0 };
				zwrep_csc_fn	rpt_cb = (zwrep_csc_fn)cb;

				if (intf && intf->cch_dat_cnt)
				{
					tm_stamp_p = (time_t *)intf->cch_data;
					time_stamp = *tm_stamp_p;
					memcpy(&csc_dat, tm_stamp_p + 1, sizeof(zwcsc_notif_t));
				}
				plt_mtx_ulck(nw->mtx);
				rpt_cb(&cb_req->ifd, &csc_dat, time_stamp);
			}
			break;

		//-------------------------------------------------
		case COMMAND_CLASS_SENSOR_ALARM:
		//-------------------------------------------------
			if (cb_req->zw_rpt == SENSOR_ALARM_REPORT)
			{
				int                 cache_found = 0;
				zw_alrm_snsr_t		snsr_value = { 0 };
				zwrep_alrm_snsr_fn    rpt_cb = (zwrep_alrm_snsr_fn)cb;

				if (intf && intf->cch_dat_cnt)
				{
					if (cb_req->extra_cnt == 1)
					{   //No parameter specified, return default report
						tm_stamp_p = (time_t *)intf->cch_data;
						time_stamp = *tm_stamp_p;
						memcpy(&snsr_value, tm_stamp_p + 1, sizeof(zw_alrm_snsr_t));
						cache_found = 1;
					}
					else if (cb_req->extra_cnt > 1)
					{
						cache_found = zwnet_alrm_snsr_cache_get(intf, cb_req->extra_cnt - 1, cb_req->extra, &snsr_value, &time_stamp);
					}
				}
				plt_mtx_ulck(nw->mtx);
				if (!cache_found)
				{   //Return caller's parameter
					if (cb_req->extra_cnt > 0)
					{
						snsr_value.type = (uint8_t)cb_req->extra[cb_req->extra_cnt - 1];
					}
				}
				rpt_cb(&cb_req->ifd, &snsr_value, time_stamp);
			}
			break;

        //-------------------------------------------------
        case COMMAND_CLASS_BARRIER_OPERATOR:
        //-------------------------------------------------
            if (cb_req->zw_rpt == BARRIER_OPERATOR_REPORT)
            {
                zwbarrier_t         barrier_dat = {0};
                uint16_t            extra = 0;
                zwrep_barrier_fn    rpt_cb = (zwrep_barrier_fn)cb;

                if (intf && intf->cch_dat_cnt)
                {
                    zwnet_barrier_cache_get(intf, 1, &extra, &barrier_dat, &time_stamp);
                }
                plt_mtx_ulck(nw->mtx);

                rpt_cb(&cb_req->ifd, barrier_dat.subsys_sta, time_stamp);
            }
            else if (cb_req->zw_rpt == BARRIER_OPERATOR_SIGNAL_REPORT)
            {
                int                     cache_found = 0;
                zwbarrier_t             barrier_dat = {0};
                zwrep_barrier_subsys_fn rpt_cb = (zwrep_barrier_subsys_fn)cb;

                if (intf && intf->cch_dat_cnt)
                {
                    cache_found = zwnet_barrier_cache_get(intf, cb_req->extra_cnt, cb_req->extra, &barrier_dat, &time_stamp);
                }
                plt_mtx_ulck(nw->mtx);

                if (!cache_found)
                {   //Return caller's parameter
                    if (cb_req->extra_cnt == 1)
                    {
                        barrier_dat.subsys_type = (uint8_t)cb_req->extra[0];
                    }
                }

                rpt_cb(&cb_req->ifd, barrier_dat.subsys_type, barrier_dat.subsys_sta, time_stamp);
            }
            break;

        //-------------------------------------------------
        case COMMAND_CLASS_PROTECTION:
        //-------------------------------------------------
            if (cb_req->zw_rpt == PROTECTION_REPORT)
            {
                zwprot_cch_t    *cch_prot;
                zwrep_prot_fn   rpt_cb = (zwrep_prot_fn)cb;
                uint8_t         local_prot = 0;
                uint8_t         rf_prot = 0;

                if (intf && intf->cch_dat_cnt)
                {
                    cch_prot = (zwprot_cch_t *)intf->cch_data;
                    time_stamp = cch_prot->tmstp_prot;
                    local_prot = cch_prot->local_prot;
                    rf_prot = cch_prot->rf_prot;
                }
                plt_mtx_ulck(nw->mtx);
                rpt_cb(&cb_req->ifd, local_prot, rf_prot, time_stamp);
            }
            else if (cb_req->zw_rpt == PROTECTION_EC_REPORT_V2)
            {
                zwprot_cch_t        *cch_prot;
                zwrep_prot_ec_fn    rpt_cb = (zwrep_prot_ec_fn)cb;
                uint8_t             ec_nodeid = 0;

                if (intf && intf->cch_dat_cnt)
                {
                    cch_prot = (zwprot_cch_t *)intf->cch_data;
                    time_stamp = cch_prot->tmstp_ec;
                    ec_nodeid = cch_prot->ec_nodeid;
                }
                plt_mtx_ulck(nw->mtx);
                rpt_cb(&cb_req->ifd, ec_nodeid, time_stamp);
            }
            else if (cb_req->zw_rpt == PROTECTION_TIMEOUT_REPORT_V2)
            {
                zwprot_cch_t        *cch_prot;
                zwrep_prot_ec_fn    rpt_cb = (zwrep_prot_ec_fn)cb;
                uint8_t             remain_tm = 0;

                if (intf && intf->cch_dat_cnt)
                {
                    cch_prot = (zwprot_cch_t *)intf->cch_data;
                    time_stamp = cch_prot->tmstp_remain;
                    remain_tm = cch_prot->remain_tm;
                }
                plt_mtx_ulck(nw->mtx);
                rpt_cb(&cb_req->ifd, remain_tm, time_stamp);
            }
            break;


        default:
            plt_mtx_ulck(nw->mtx);
    }
}


/**
zwnet_db_cb_rpt - Callback user application using device specific configuration database
@param[in]	nw		Network
@param[in]	cb_req  Callback function
@param[in]	req		Request of the callback
@return
*/
static void zwnet_db_cb_rpt(zwnet_p nw, void *cb, zwnet_db_rpt_cb_req_t *cb_req)
{
    zwnode_p    node;
    zwif_p      intf;
    void        *intf_db_data = NULL;


    plt_mtx_lck(nw->mtx);
    intf = zwif_get_if(&cb_req->ifd);
    if (intf)
    {
        node = intf->ep->node;

        if (node->dev_cfg_valid)
        {
            intf_db_data = zwdev_if_get(node->dev_cfg_rec.ep_rec, intf->ep->epid, (uint8_t)cb_req->if_type);
        }
    }

    switch (cb_req->rpt_type)
    {
        //-------------------------------------------------
        case CB_RPT_TYP_ALARM_TYPE:
        //-------------------------------------------------
            {
                zwrep_alrm_sup_fn   rpt_cb = (zwrep_alrm_sup_fn)cb;

                if (intf_db_data)
                {
                    int                     len = 0;
                    uint8_t                 ztype[248];
                    if_rec_alarm_match_t    *alarm_rec = (if_rec_alarm_match_t *)intf_db_data;

                    while (alarm_rec && (len < 248))
                    {
                        //go through both match block and result block to return all the types
                        if (alarm_rec->ex_type != -1)
                        {
                            UNIQUELY_ADD_TO_ARRAY(ztype, alarm_rec->ex_type, len)
                        }
                        if (alarm_rec->pResult->ex_type != -1)
                        {
                            UNIQUELY_ADD_TO_ARRAY(ztype, alarm_rec->pResult->ex_type, len)
                        }
                        alarm_rec = alarm_rec->next;
                    }

                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 1, len, ztype, 1);
                }
                else
                {
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 0, 0, NULL, 0);
                }
            }
            break;

        //-------------------------------------------------
        case CB_RPT_TYP_ALARM_EVENT:
        //-------------------------------------------------
            {
                zwrep_alrm_evt_fn       rpt_cb = (zwrep_alrm_evt_fn)cb;

                if (intf_db_data)
                {
                    int                     len = 0;
                    uint8_t                 ztype, zevt[248];
                    if_rec_alarm_match_t    *alarm_rec = (if_rec_alarm_match_t *)intf_db_data;

                    ztype = (uint8_t)cb_req->extra;
                    memset(zevt, 0, 248);

                    while (alarm_rec && (len < 248))
                    {
                        if (alarm_rec->ex_type == ztype)
                        {
                            if (alarm_rec->ex_event != -1 && alarm_rec->ex_event != 0xFE)
                                UNIQUELY_ADD_TO_ARRAY(zevt, alarm_rec->ex_event, len)
                        }

                        if (alarm_rec->pResult->ex_type == ztype)
                        {
                            if (alarm_rec->pResult->ex_event != -1 && alarm_rec->pResult->ex_event != 0xFE)
                                UNIQUELY_ADD_TO_ARRAY(zevt, alarm_rec->pResult->ex_event, len)
                        }
                        alarm_rec = alarm_rec->next;
                    }

                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, ztype, len, zevt, 1);
                }
                else
                {
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 0, 0, NULL, 0);
                }

            }
            break;

        //-------------------------------------------------
        case CB_RPT_TYP_METER:
        //-------------------------------------------------
            {
                zwrep_meter_sup_fn  rpt_cb = (zwrep_meter_sup_fn)cb;

                if (intf_db_data)
                {
                    zwmeter_cap_t   if_meter_data;
                    if_rec_meter_t  *meter_rec = (if_rec_meter_t *)intf_db_data;

                    if_meter_data.reset_cap = 0;
                    if_meter_data.type = meter_rec->type;
                    if_meter_data.unit_sup = meter_rec->unit_supp;

                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, &if_meter_data, 1);
                }
                else
                {
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, NULL, 0);
                }
            }
            break;

        //-------------------------------------------------
        case CB_RPT_TYP_SENSOR:
        //-------------------------------------------------
            {
                zwrep_sensor_sup_fn   rpt_cb = (zwrep_sensor_sup_fn)cb;

                if (intf_db_data)
                {
                    uint8_t         snsr_type;
                    if_rec_snsr_t   *snsr_rec = (if_rec_snsr_t *)intf_db_data;

                    snsr_type = snsr_rec->type;
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 1, &snsr_type, 1);
                }
                else
                {
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 0, NULL, 0);
                }

            }
            break;

        //-------------------------------------------------
        case CB_RPT_TYP_SENSOR_UNIT:
        //-------------------------------------------------
            {
                zwrep_sensor_unit_fn  rpt_cb = (zwrep_sensor_unit_fn)cb;

                if (intf_db_data)
                {
                    uint8_t         snsr_type;
                    uint8_t         sensor_unit_msk;
                    if_rec_snsr_t   *snsr_rec = (if_rec_snsr_t *)intf_db_data;

                    snsr_type = (uint8_t)cb_req->extra;

                    sensor_unit_msk = (snsr_rec->type == snsr_type)? (0x01 << snsr_rec->unit) : 0;

                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, snsr_type, sensor_unit_msk, 1);
                }
                else
                {
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 0, 0, 0);
                }
            }
            break;

        //-------------------------------------------------
        case CB_RPT_TYP_BIN_SENSOR:
        //-------------------------------------------------
            {
                zwrep_bsensor_sup_fn  rpt_cb = (zwrep_bsensor_sup_fn)cb;

                if (intf_db_data)
                {
                    uint8_t         snsr_type;
                    if_rec_bsnsr_t  *snsr_rec = (if_rec_bsnsr_t *)intf_db_data;

                    snsr_type = snsr_rec->type;
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 1, &snsr_type, 1);
                }
                else
                {
                    plt_mtx_ulck(nw->mtx);
                    rpt_cb(&cb_req->ifd, 0, NULL, 0);
                }
            }
            break;

        //-------------------------------------------------
        case CB_RPT_TYP_THRMO_SETP_RANGE:
        //-------------------------------------------------
            {
                zwrep_thrmo_setp_range_fn  rpt_cb = (zwrep_thrmo_setp_range_fn)cb;

                if (intf_db_data)
                {
                    uint8_t                 setp_type;
                    unsigned                i;
                    if_rec_thrmo_setp_t     *setp_rec = (if_rec_thrmo_setp_t *)intf_db_data;
                    zwsetp_temp_range_t     setp_range;

                    setp_type = (uint8_t)cb_req->extra;

                    for (i=0; i<setp_rec->type_cnt; i++)
                    {
                        if (setp_rec->temp_range[i].min.type == setp_type)
                        {
                            setp_range = setp_rec->temp_range[i];
                            plt_mtx_ulck(nw->mtx);
                            rpt_cb(&cb_req->ifd, setp_type, &setp_range.min, &setp_range.max, 1);
                            return;
                        }
                    }
                }
                plt_mtx_ulck(nw->mtx);
                rpt_cb(&cb_req->ifd, 0, NULL, NULL, 0);
            }
            break;

        default:
            plt_mtx_ulck(nw->mtx);

    }
}


/**
zwnet_cb_thrd - Thread to execute certain operations
@param[in]	data		Network
@return
*/
static void zwnet_cb_thrd(void *data)
{
    zwnet_p         nw = (zwnet_p)data;
    util_lst_t      *req_lst;
    zwnet_cb_req_t  *cb_req;

    nw->cb_thrd_sts = 1;

    //Increment network context reference count
    plt_mtx_lck(nw->mtx);
    nw->ctx_ref_cnt++;
    plt_mtx_ulck(nw->mtx);

    while (1)
    {
        //Wait for a request
        plt_sem_wait(nw->cb_sem);

        //Check whether to exit the thread
        if (nw->cb_thrd_run == 0)
        {
            nw->cb_thrd_sts = 0;
            zwnet_free(nw);
            return;
        }

        while ((req_lst = util_list_get(nw->cb_mtx, &nw->cb_req_hd)) != NULL)
        {
            //Check whether to exit the thread
            if (nw->cb_thrd_run == 0)
            {
                free(req_lst);
                nw->cb_thrd_sts = 0;
                zwnet_free(nw);
                return;
            }

            cb_req = (zwnet_cb_req_t *)req_lst->wr_buf;

            switch (cb_req->type)
            {
                case CB_TYPE_DAT_CACHED_REPORT:
                    zwnet_dat_cached_cb_rpt(nw, cb_req->cb, &cb_req->param.dat_rpt);
                    break;

                case CB_TYPE_SUP_CACHED_REPORT:
                    zwnet_sup_cached_cb_rpt(nw, cb_req->cb, &cb_req->param.sup_rpt);
                    break;

                case CB_TYPE_DB_REPORT:
                    zwnet_db_cb_rpt(nw, cb_req->cb, &cb_req->param.db_rpt);
                    break;

                case CB_TYPE_NET_OP:
                    {
                        zwnet_notify_fn cb = (zwnet_notify_fn)cb_req->cb;

                        if ((cb_req->param.net_op.op == ZWNET_OP_INITIALIZE)
                            && (cb_req->param.net_op.sts == OP_DONE))
                        {   //Sleep 10ms to prevent race condition
                            plt_sleep(10);
                        }

                        if ((cb_req->param.net_op.op == ZWNET_OP_SLEEP_NODE_UPT)
                            && (cb_req->param.net_op.sts == OP_DONE))
                        {   //For sleeping node update operation, just save the detailed node info without callback
                            //to user application
                            zwsave_save(nw->zwsave_ctx);
                            break;
                        }

                        cb(cb_req->param.net_op.user, cb_req->param.net_op.op, cb_req->param.net_op.sts,
                           (cb_req->param.net_op.sts_info_valid)? &cb_req->param.net_op.sts_info : NULL);

                        // Save network changes to persistent storage
                        if (cb_req->param.net_op.sts == OP_DONE)
                        {
                            switch(cb_req->param.net_op.op)
                            {
                                case ZWNET_OP_ADD_NODE:
                                case ZWNET_OP_RM_NODE:
                                case ZWNET_OP_RP_NODE:
                                case ZWNET_OP_RM_FAILED_ID:
                                case ZWNET_OP_INITIATE:
                                case ZWNET_OP_UPDATE:
                                case ZWNET_OP_RESET:
                                case ZWNET_OP_MIGRATE:
                                case ZWNET_OP_NODE_UPDATE:
                                case ZWNET_OP_NW_CHANGED:
                                    zwsave_save(nw->zwsave_ctx);

                                    if ((cb_req->param.net_op.op == ZWNET_OP_NW_CHANGED)
                                        || (cb_req->param.net_op.op == ZWNET_OP_UPDATE)
                                        || (cb_req->param.net_op.op == ZWNET_OP_INITIATE))
                                    {
                                        //Add all nodes for device polling
                                        zwdpoll_add_all(nw->dpoll_ctx);

                                        if (nw->multi_client)
                                        {
                                            //Add all nodes for wakeup notification polling
                                            wkuppoll_add_all(nw->wupoll_ctx);
                                        }

                                        //Add all nodes for node status polling
                                        nodests_add_all(nw->nsts_ctx);
                                    }
                                    break;

                                case ZWNET_OP_INITIALIZE:
                                    //Add all nodes for device polling
                                    zwdpoll_add_all(nw->dpoll_ctx);
                                    if (nw->multi_client)
                                    {
                                        //Add all nodes for wakeup notification polling
                                        wkuppoll_add_all(nw->wupoll_ctx);
                                    }
                                    //Add all nodes for node status polling
                                    nodests_add_all(nw->nsts_ctx);
                                    break;

                            }
                        }
                        else if (cb_req->param.net_op.op == ZWNET_OP_INITIALIZE)
                        {   //For loading network from persistent storage or through Z-wave during initialization,
                            //save only if there is new acquired node information from Z-wave.
                            if (cb_req->param.net_op.sts & OP_GET_NI_TOTAL_NODE_MASK)
                            {
                                uint16_t    total_nodes;
                                uint16_t    cmplt_nodes;

                                total_nodes = (cb_req->param.net_op.sts & OP_GET_NI_TOTAL_NODE_MASK) >> 8;
                                cmplt_nodes = cb_req->param.net_op.sts & OP_GET_NI_NODE_CMPLT_MASK;

                                if (total_nodes == cmplt_nodes)
                                {
                                    zwsave_save(nw->zwsave_ctx);
                                }
                            }
                        }
                    }
                    break;

                case CB_TYPE_NODE:
                    {
                        zwnode_p      node;
                        zwnoded_t     node_desc = {0};
                        zwnet_node_fn cb = (zwnet_node_fn)cb_req->cb;

                        if (cb_req->param.node.mode == ZWNET_NODE_REMOVED)
                        {   //The node has already been removed, create a simple one.
                            node_desc.nodeid = cb_req->param.node.node_id;
                            node_desc.net = nw;
                            cb(cb_req->param.node.user, &node_desc, cb_req->param.node.mode);
                            //Remove it from pollings
                            zwdpoll_rm_node(nw->dpoll_ctx, &node_desc);
                            zwspoll_rm(nw->spoll_ctx, node_desc.nodeid);
                            zwspv_rm(nw->spv_ctx, node_desc.nodeid);
                            if (nw->multi_client)
                            {
                                //Remove it from wakeup notification polling
                                wkuppoll_rm(nw->wupoll_ctx, node_desc.nodeid);
                            }
                            zwevtg_rm(nw->evtg_ctx, node_desc.nodeid);
                            nodests_rm(nw->nsts_ctx, node_desc.nodeid);

                            //Remove all Z/IP frame sequence numbers used by the node
                            appl_seq_num_rm_by_nodeid(&nw->appl_ctx, node_desc.nodeid);
                            break;
                        }

                        plt_mtx_lck(nw->mtx);
                        node = zwnode_find(&nw->ctl, cb_req->param.node.node_id);
                        if (node)
                        {
                            if (zwnode_get_desc(node, &node_desc) == ZW_ERR_NONE)
                            {
                                plt_mtx_ulck(nw->mtx);
                                cb(cb_req->param.node.user, &node_desc, cb_req->param.node.mode);
                                break;
                            }
                        }
                        plt_mtx_ulck(nw->mtx);
                    }
                    break;

                case CB_TYPE_1_EP_SM:
                    {
                        zwif_p      intf;

                        plt_mtx_lck(nw->mtx);
                        intf = zwif_get_if(&cb_req->param.ep_cb.ifd);
                        if (intf)
                        {
                            zwnet_1_ep_info_sm(nw, cb_req->param.ep_cb.evt, (uint8_t *)intf);
                        }
                        plt_mtx_ulck(nw->mtx);

                    }
                    break;

                case CB_TYPE_S2_DSK:
                    {
                        add_node_sec2_fn    cb = (add_node_sec2_fn)cb_req->cb;
                        sec2_add_cb_prm_t   cb_param;

                        cb_param.cb_type = S2_CB_TYPE_DSK;
                        cb_param.cb_prm.dsk.pin_required = cb_req->param.dsk_cb.pin_required;
                        cb_param.cb_prm.dsk.dsk = cb_req->param.dsk_cb.dsk;

                        cb(cb_req->param.dsk_cb.user, &cb_param);
                    }
                    break;

                case CB_TYPE_S2_REQ_KEY:
                    {
                        add_node_sec2_fn    cb = (add_node_sec2_fn)cb_req->cb;
                        sec2_add_cb_prm_t   cb_param;

                        cb_param.cb_type = S2_CB_TYPE_REQ_KEY;
                        cb_param.cb_prm.req_key.req_csa = cb_req->param.req_key.req_key.req_csa;
                        cb_param.cb_prm.req_key.req_keys = cb_req->param.req_key.req_key.req_keys;
                        if (cb_param.cb_prm.req_key.req_csa)
                        {
                            plt_mtx_lck(nw->mtx);
                            memcpy(cb_param.cb_prm.req_key.csa_pin, nw->gw_dsk, 5);
			    memcpy(cb_param.cb_prm.req_key.csa_pin+5, nw->gw_dsk+6, 5);
                            cb_param.cb_prm.req_key.csa_pin[10] = '\0';
                            plt_mtx_ulck(nw->mtx);
                        }
                        else
                        {
                            cb_param.cb_prm.req_key.csa_pin[0] = '\0';
                        }

                        cb(cb_req->param.req_key.user, &cb_param);
                    }
                    break;

                case CB_TYPE_GW_DSK:
                    {
                        get_dsk_fn    cb = (get_dsk_fn)cb_req->cb;

                        if (cb)
                        {
                            cb(cb_req->param.gw_dsk.user, cb_req->param.gw_dsk.dsk);
                        }
                    }
                    break;

                case CB_TYPE_POST_SET:
                    {
                        zw_postset_fn   cb = (zw_postset_fn)cb_req->cb;
                        zwifd_p         ifd = &cb_req->param.post_set.ifd;

                        cb(nw, ifd->nodeid, ifd->epid, ifd->cls, cb_req->param.post_set.user, cb_req->param.post_set.reason);
                    }
                    break;

                case CB_TYPE_SIMULATED_REPORT:
                    {
                        zwif_p  intf;

                        plt_mtx_lck(nw->mtx);
                        //Insert a report to simulate a live report for the interface
                        intf = zwif_get_if(&cb_req->param.sim_rpt.ifd);
                        if (intf)
                        {
                            zwif_rep_hdlr(intf, cb_req->param.sim_rpt.rpt, cb_req->param.sim_rpt.rpt_len, 1);
                        }
                        plt_mtx_ulck(nw->mtx);
                    }
                    break;

                case CB_TYPE_INIF:
                    {
                        pl_info_fn    cb = (pl_info_fn)cb_req->cb;

                        cb(cb_req->param.inif.user, &cb_req->param.inif.lst_ent);
                    }
                    break;

#ifdef TCP_PORTAL
                case CB_TYPE_NET_ERR:
                    {
                         net_err_cb_t cb = (net_err_cb_t)cb_req->cb;
                         cb(cb_req->param.net_err.user);
                    }
                    break;
#endif
            }

            free(req_lst);

            //Check whether to exit the thread
            if (nw->cb_thrd_run == 0)
            {
                nw->cb_thrd_sts = 0;
                zwnet_free(nw);
                return;
            }
        }
    }
}


/**
zwnet_sup_cmd_cls_create - Create a command class list from two command class lists with deduplicate of repeated command class
@param[in]  cmd_cls1		First command class list (higher priority)
@param[in]  cmd_cls_cnt1    Number of entries in the first command class list
@param[in]  cmd_cls2		Second command class list
@param[in]  cmd_cls_cnt2    Number of entries in the second command class list
@param[out] cmd_cls_buf     The buffer to store the generated command classes
@param[out] cmd_cls_cnt     The number of command classes stored in the cmd_cls_buf
@return       0 if o.k.; else return non-zero
@post         If return is 0, the caller must free the output buffer cmd_cls_buf
*/
static int zwnet_sup_cmd_cls_create(const sup_cmd_cls_t *cmd_cls1, uint8_t cmd_cls_cnt1,
                              const sup_cmd_cls_t *cmd_cls2, uint8_t cmd_cls_cnt2,
                              sup_cmd_cls_t **cmd_cls_buf, uint8_t *cmd_cls_cnt)
{
    int             i;
    uint8_t         buf_index;
    uint8_t         tot_cmd_cls;
    sup_cmd_cls_t   *tmp_cmd_buf;

    //Calculate total number of command classes
    tot_cmd_cls = cmd_cls_cnt1 + cmd_cls_cnt2;

    //Copy and deduplicate command classes
    tmp_cmd_buf = (sup_cmd_cls_t *)malloc(tot_cmd_cls * sizeof(sup_cmd_cls_t));
    if (!tmp_cmd_buf)
    {
        return ZW_ERR_MEMORY;
    }

    //Copy the first command class list
    memcpy(tmp_cmd_buf, cmd_cls1, cmd_cls_cnt1 * sizeof(sup_cmd_cls_t));

    //Copy and deduplicate the second command class list
    buf_index = cmd_cls_cnt1;
    for (i=0; i<cmd_cls_cnt2; i++)
    {
        if(!zwnet_sup_cmd_cls_find(tmp_cmd_buf, cmd_cls2[i].cls, buf_index))
        {   //Not found
            tmp_cmd_buf[buf_index++] = cmd_cls2[i];
        }
    }

    //Assign return values
    *cmd_cls_buf = tmp_cmd_buf;
    *cmd_cls_cnt = buf_index;
    return 0;

}


#ifdef CONFIG_DEBUG
/**
zwnet_4bytes_align_chk - Check for 4-byte aligned memory access
@return       non-zero if success; else return zero
*/
static int zwnet_4bytes_align_chk(void)
{
    int                 i;
    ext_ver_t           *ext_ver;
    if_alarm_data_t     *alrm_data;
    zw_grp_info_t       *grp_info;
    if_grp_info_dat_t   *grp_info_dat;
    zwif_t              *intf;
    if_thrmo_setp_cap_t *setp_cap;
    util_lst_t          *util_lst;

    for (i=0; i<2; i++)
    {
        ext_ver = malloc(sizeof(ext_ver_t));
        if (ext_ver)
        {
            if (((uintptr_t)(ext_ver->fw_ver)) & 0x03)
            {
                free(ext_ver);
                return 0;
            }
            free(ext_ver);
        }

        if (sizeof(zwifd_t)  & 0x03)
        {
            return 0;
        }

        alrm_data = malloc(sizeof(if_alarm_data_t));
        if (alrm_data)
        {
            if (((uintptr_t)(alrm_data->type_evt)) & 0x03)
            {
                free(alrm_data);
                return 0;
            }
            free(alrm_data);
        }

        grp_info = malloc(sizeof(zw_grp_info_t));
        if (grp_info)
        {
            if (((uintptr_t)(grp_info->cmd_lst)) & 0x03)
            {
                free(grp_info);
                return 0;
            }
            free(grp_info);
        }

        grp_info_dat = malloc(sizeof(if_grp_info_dat_t));
        if (grp_info_dat)
        {
            if (((uintptr_t)(grp_info_dat->grp_info)) & 0x03)
            {
                free(grp_info_dat);
                return 0;
            }
            free(grp_info_dat);
        }

        intf = malloc(sizeof(zwif_t));
        if (intf)
        {
            if (((uintptr_t)(intf->rpt)) & 0x03)
            {
                free(intf);
                return 0;
            }
            free(intf);
        }

        setp_cap = malloc(sizeof(if_thrmo_setp_cap_t));
        if (setp_cap)
        {
            if (((uintptr_t)(setp_cap->setp_cap)) & 0x03)
            {
                free(setp_cap);
                return 0;
            }
            free(setp_cap);
        }

        util_lst = malloc(sizeof(util_lst_t));
        if (util_lst)
        {
            if (((uintptr_t)(util_lst->wr_buf)) & 0x03)
            {
                free(util_lst);
                return 0;
            }
            free(util_lst);
        }
    }

    return 1;
}
#endif


#ifdef TCP_PORTAL
/**
zwnet_net_err_cb - Network error callback in portal mode
@return
*/
static void zwnet_net_err_cb(void *user)
{
    zwnet_p         nw = (zwnet_p)user;
    zwnet_cb_req_t  cb_req;

    if (nw->init.net_err)
    {
        cb_req.type = CB_TYPE_NET_ERR;
        cb_req.cb = nw->init.net_err;
        cb_req.param.net_err.user = nw->init.user;

        util_list_add(nw->cb_mtx, &nw->cb_req_hd,
                      (uint8_t *)&cb_req, sizeof(zwnet_cb_req_t));
        plt_sem_post(nw->cb_sem);
    }
}
#endif


/**
zwnet_appl_status_cb - Application status callback
@param[in]	appl_ctx    The application layer context
@param[in]	cmd         APPLICATION_BUSY or APPLICATION_REJECTED_REQUEST
@param[in]	status      Status
@return
*/
static void zwnet_appl_status_cb(appl_layer_ctx_t *appl_ctx, uint8_t cmd, uint8_t status)
{
    zwnet_p nw = (zwnet_p)appl_ctx->data;
    uint8_t data[2];

    data[0] = cmd;
    data[1] = status;

    if (cmd == APPLICATION_BUSY)
    {
        debug_zwapi_msg(&nw->plt_ctx, "Received APPLICATION_BUSY");
    }
    else
    {
        debug_zwapi_msg(&nw->plt_ctx, "Received APPLICATION_REJECTED_REQUEST");
    }

    plt_mtx_lck(nw->mtx);

    zwnet_node_info_sm(nw, EVT_APPL_STATUS, data);
    zwnet_init_sm(nw, EVT_INI_APPL_STATUS, data);
    zwnet_ctlr_info_sm(nw, EVT_CTLR_APPL_STATUS, data);

    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_dev_global_set - Set device global settings to network structure
@param[in]	nw              Network
@param[in]	global_sett     Device global settings
@param[in]	global_sett_cnt Device global settings count
@return ZW_ERR_XXX
*/
static int zwnet_dev_global_set(zwnet_p nw, dev_global_sett_t *global_sett, uint16_t global_sett_cnt)
{
    uint16_t i;

    for (i=0; i<global_sett_cnt; i++)
    {
        switch (global_sett[i].type)
        {
            case GLOB_SET_TYPE_WKUP_INTV:
                nw->dflt_wkup_intv = global_sett[i].sett.wkup_intv;
                break;

            case GLOB_SET_TYPE_WUNMI_DELAY:
                nw->dflt_wunmi_dly = global_sett[i].sett.wunmi_delay;
                break;
        }
    }

    return 0;
}


/**
zwnet_cmd_cls_cfg_load - Load command class configuration file
@param[in]      cfg_file	Command classes configuration file
@param[in,out]  cmd_cls_cnt Input: the maximum entries the cmd_cls_buf can store. Output: number of entries stored
@param[out]     cmd_cls_buf The buffer to store the command classes
@return       ZW_ERR_XXX
*/
static int zwnet_cmd_cls_cfg_load(const char *cfg_file, uint16_t *cmd_cls_cnt, uint16_t *cmd_cls_buf)
{
    FILE        *file_p;
    int         res;
    char        line[384];
    char        cmd_cls_name[386];
    uint16_t    max_cnt = *cmd_cls_cnt;
    uint16_t    cc_cnt = 0;

    //Init
    *cmd_cls_cnt = 0;

    if (!cfg_file || !(*cfg_file))
    {   //Accept NULL file name or "" file name
        return 0;
    }

#ifdef USE_SAFE_VERSION
    if (fopen_s(&file_p, cfg_file, "rt") != 0)
    {
        return ZW_ERR_FILE_OPEN;
    }
#else
    file_p = fopen(cfg_file, "rt");
    if (!file_p)
    {
        return ZW_ERR_FILE_OPEN;
    }
#endif

    //Always enable the following command classes
    cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_ASSOCIATION;
    cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_ZWAVEPLUS_INFO;
    cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_ASSOCIATION_GRP_INFO;

    while (fgets(line, 384, file_p) && (cc_cnt < max_cnt))
    {
        if (*line == '#')
        {   //Skip comment line
            continue;
        }

#ifdef USE_SAFE_VERSION
        res = sscanf_s(line, "%s", cmd_cls_name, 386);
#else
        res = sscanf(line, "%s", cmd_cls_name);
#endif
        if (res == 1)
        {
            if (strcmp(cmd_cls_name, "COMMAND_CLASS_CONFIGURATION") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_CONFIGURATION;
            }
            else if (strcmp(cmd_cls_name, "COMMAND_CLASS_CENTRAL_SCENE") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_CENTRAL_SCENE;
            }
            else if (strcmp(cmd_cls_name, "COMMAND_CLASS_SENSOR_MULTILEVEL") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_SENSOR_MULTILEVEL;
            }
            else if (strcmp(cmd_cls_name, "COMMAND_CLASS_THERMOSTAT_FAN_MODE") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_THERMOSTAT_FAN_MODE;
            }
            else if (strcmp(cmd_cls_name, "COMMAND_CLASS_THERMOSTAT_MODE") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_THERMOSTAT_MODE;
            }
            else if (strcmp(cmd_cls_name, "COMMAND_CLASS_THERMOSTAT_SETPOINT") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_THERMOSTAT_SETPOINT;
            }
            else if (strcmp(cmd_cls_name, "COMMAND_CLASS_SWITCH_MULTILEVEL") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_SWITCH_MULTILEVEL;
            }
            else if (strcmp(cmd_cls_name, "COMMAND_CLASS_SIMPLE_AV_CONTROL") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_SIMPLE_AV_CONTROL;
            }
            else if (strcmp(cmd_cls_name, "COMMAND_CLASS_ALARM") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_ALARM;
            }
            else if (strcmp(cmd_cls_name, "COMMAND_CLASS_PROTECTION") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_PROTECTION;
            }
            else if (strcmp(cmd_cls_name, "COMMAND_CLASS_USER_CODE") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_USER_CODE;
            }
            else if (strcmp(cmd_cls_name, "COMMAND_CLASS_METER") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_METER;
            }
            else if (strcmp(cmd_cls_name, "COMMAND_CLASS_METER_TBL_MONITOR") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_METER_TBL_MONITOR;
            }
            else if (strcmp(cmd_cls_name, "COMMAND_CLASS_SENSOR_BINARY") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_SENSOR_BINARY;
            }
            else if (strcmp(cmd_cls_name, "COMMAND_CLASS_DOOR_LOCK") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_DOOR_LOCK;
            }
            else if (strcmp(cmd_cls_name, "COMMAND_CLASS_SENSOR_ALARM") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_SENSOR_ALARM;
            }
            else if (strcmp(cmd_cls_name, "COMMAND_CLASS_BARRIER_OPERATOR") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_BARRIER_OPERATOR;
            }
            else if (strcmp(cmd_cls_name, "COMMAND_CLASS_SWITCH_COLOR") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_SWITCH_COLOR;
            }
            else if (strcmp(cmd_cls_name, "COMMAND_CLASS_THERMOSTAT_OPERATING_STATE") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_THERMOSTAT_OPERATING_STATE;
            }
            else if (strcmp(cmd_cls_name, "COMMAND_CLASS_SWITCH_BINARY") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_SWITCH_BINARY;
            }
            else if (strcmp(cmd_cls_name, "COMMAND_CLASS_BATTERY") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_BATTERY;
            }
            else if (strcmp(cmd_cls_name, "COMMAND_CLASS_THERMOSTAT_FAN_STATE") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_THERMOSTAT_FAN_STATE;
            }
            else if (strcmp(cmd_cls_name, "COMMAND_CLASS_NODE_NAMING") == 0)
            {
                cmd_cls_buf[cc_cnt++] = COMMAND_CLASS_NODE_NAMING;
            }
        }
    }

    //Output
    *cmd_cls_cnt = cc_cnt;

    fclose(file_p);

    return 0;
}


/**
zwnet_init - Initialize network and return this network
@param[in]	init	        client initialization parameters
@param[out]	net	            network handle
@return		ZW_ERR_XXX
*/
int zwnet_init(const zwnet_init_p init, zwnet_p *net)
{

    static const char   *init_layer[] = {   "ok",
                                            "transport",
                                            "frame",
                                            "session",
                                            "application"
                                        };

    int                 result;
    zwnet_p             nw;
    zwnet_init_sm_prm_t init_sm_prm = {0};
    dev_global_sett_t   *global_sett = NULL;    // Device global settings
    uint16_t            global_sett_cnt = 0;    // Device global settings count
#ifdef TCP_PORTAL
    sigset_t            sig_set;
    appl_portal_prm_t   init_portal_prm;

    //Block SIGPIPE signal
    sigemptyset(&sig_set);
    sigaddset(&sig_set, SIGPIPE);
    result = pthread_sigmask(SIG_BLOCK, &sig_set, NULL);
    if (result != 0)
    {
        return ZW_ERR_FAILED;
    }
#else
    appl_ce_prm_t       init_ce_prm;

    if (init->dtls_psk_len > 0)
    {
        //Check DTLS pre-shared key length
        if (init->dtls_psk_len < 16)
        {
            return ZW_ERR_PSK_TOO_SHORT;
        }

        //Check host listening port
        if (init->host_port == ZWAVE_HOME_CTL_PORT)
        {
            return ZW_ERR_DISALLOWED;
        }
    }
#endif

#ifdef CONFIG_DEBUG
    //Check for 4-byte aligned memory access
    if (!zwnet_4bytes_align_chk())
    {
        return ZW_ERR_UNEXPECTED;
    }
#endif

    //Allocate memory for network internal structure
    nw = (zwnet_p) calloc(1, sizeof(zwnet_t));

    if (!nw)
        return ZW_ERR_MEMORY;

    //Set default values
    zwnode_ctl_clr(nw);
    nw->dflt_wunmi_dly = 60;

    //Load command class configuration file
    nw->cmd_cls_cfg_cnt = MAX_CC_EN;
    if ((result = zwnet_cmd_cls_cfg_load(init->cmd_cls_cfg_file, &nw->cmd_cls_cfg_cnt, nw->cmd_cls_cfg)) != 0)
    {
        goto l_ZWNET_INI_ERR2;
    }

    //Create supported command class list based on local and user application list
    if ((result = zwnet_sup_cmd_cls_create(local_sup_cmd_class, sizeof(local_sup_cmd_class)/sizeof(sup_cmd_cls_t),
                                           init->sup_cmd_cls, init->sup_cmd_cls_cnt,
                                           &nw->sup_cmd_cls, &nw->sup_cmd_cls_cnt)) != 0)
    {
        goto l_ZWNET_INI_ERR2;
    }

    //Load device specific configurations
    //Check which mode of device specific configurations is selected by the user application
    result = ZW_ERR_VALUE;

    //Clear error location
    memset(&init->err_loc, 0, sizeof(dev_cfg_error_t));

    if (init->dev_cfg_file)
    {   //HCAPI internal managed
        if (*init->dev_cfg_file)
        {
            result = zwdev_cfg_load(init->dev_cfg_file, &nw->dev_cfg_rec_buf.dev_cfg_records, &nw->dev_cfg_rec_buf.dev_cfg_cnt,
                                    &global_sett, &global_sett_cnt, &init->err_loc);
        }
        else
        {   //Accept empty device specific configuration record
            result = ZW_ERR_NONE;
        }
    }
    else if (init->dev_cfg_usr)
    {   //User application managed
        nw->dev_cfg_ctx = init->dev_cfg_usr->dev_cfg_ctx;
        nw->dev_rec_find_fn = init->dev_cfg_usr->dev_rec_find_fn;

        if (init->dev_cfg_usr->dev_glob_sett && init->dev_cfg_usr->global_sett_cnt)
        {
            global_sett = init->dev_cfg_usr->dev_glob_sett;
            global_sett_cnt = init->dev_cfg_usr->global_sett_cnt;
        }

        if (nw->dev_cfg_ctx && nw->dev_rec_find_fn)
        {
            result = ZW_ERR_NONE;
        }
    }

    if (result < 0)
    {
        goto l_ZWNET_INI_ERR3;
    }

    //No error. Clear error location
    memset(&init->err_loc, 0, sizeof(dev_cfg_error_t));

    //Set device global settigs
    if (global_sett && global_sett_cnt)
    {
        result = zwnet_dev_global_set(nw, global_sett, global_sett_cnt);
        if (init->dev_cfg_file)
        {
            zwdev_global_sett_free(global_sett, global_sett_cnt);
        }

        if (result < 0)
        {
            goto l_ZWNET_INI_ERR3;
        }
    }

    //Save directory for storing network/user preference files
    nw->pref_dir = util_path_dup(init->pref_dir);

    //Save directory for storing network and node information file
    nw->net_info_dir = util_path_dup(init->net_info_dir);

#ifdef SUPPORT_SIMPLE_AV_CONTROL
    //Allocate memory for bitmask of supported AV commands
    nw->av_sup_bitmask = (uint8_t *) calloc(1, init->av_sup_bitmask_len);

    if (!nw->av_sup_bitmask)
    {
        result = ZW_ERR_MEMORY;
        goto l_ZWNET_INI_ERR3;
    }
    memcpy(nw->av_sup_bitmask, init->av_sup_bitmask, init->av_sup_bitmask_len);
    nw->av_sup_bitmask_len = init->av_sup_bitmask_len;
#endif

	//Initialize Association CC and AGI CC data
	nw->stAGIData.pAGIGroupList = (AGI_Group *)calloc(sizeof(AGI_Group), 1);

	if(nw->stAGIData.pAGIGroupList == NULL)
	{
		result = ZW_ERR_MEMORY;
		goto l_ZWNET_INI_ERR3;
	}

	//Controller only has 1 group, i.e. the lifeline group. And the group only sends Device reset locally command
	nw->stAGIData.pAGIGroupList->byGroupID = 1;
	nw->stAGIData.pAGIGroupList->wProfile = (ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL << 8) | ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL_LIFELINE;
	nw->stAGIData.pAGIGroupList->wEventCode = 0;

#ifdef USE_SAFE_VERSION
    strcpy_s(nw->stAGIData.pAGIGroupList->cchGroupName, ZW_AGI_GROUPNAME_STR_MAX, "LifeLine\0");
#else
    strcpy(nw->stAGIData.pAGIGroupList->cchGroupName, "LifeLine\0");
#endif

	nw->stAGIData.pAGIGroupList->byMaxNodes = 1;
	nw->stAGIData.pAGIGroupList->stCCList.CCMSB = COMMAND_CLASS_DEVICE_RESET_LOCALLY;
	nw->stAGIData.pAGIGroupList->stCCList.CCID = DEVICE_RESET_LOCALLY_NOTIFICATION;

	nw->stAGIData.byNofGroups++;		//Supports only 1 group

    //Allocate memory for polling facility
    nw->poll_ctx = (zwpoll_ctx_t *)calloc(1, sizeof(zwpoll_ctx_t));

    if (!nw->poll_ctx)
    {
        result = ZW_ERR_MEMORY;
        goto l_ZWNET_INI_ERR4;
    }

    //Initialize platform
#ifdef TCP_PORTAL
    if (plt_init(&nw->plt_ctx, init->print_txt_fn, init->display_ctx) != 0)
#else
    if (plt_init(&nw->plt_ctx, init->print_txt_fn) != 0)
#endif
    {
        result = ZW_ERR_NO_RES;
        goto l_ZWNET_INI_ERR5;
    }

    if (!plt_mtx_init(&nw->mtx))
    {
        result = ZW_ERR_NO_RES;
        goto l_ZWNET_INI_ERR6;
    }

    //Increment network context reference count
    nw->ctx_ref_cnt++;

    if (!plt_sem_init(&nw->cmd_q_sem))
    {
        result = ZW_ERR_NO_RES;
        goto l_ZWNET_INI_ERR7;
    }

    if (!plt_mtx_init(&nw->cmd_q_mtx))
    {
        result = ZW_ERR_NO_RES;
        goto l_ZWNET_INI_ERR8;
    }

    if (!plt_sem_init(&nw->nw_exec_sem))
    {
        result = ZW_ERR_NO_RES;
        goto l_ZWNET_INI_ERR9;
    }

    if (!plt_mtx_init(&nw->nw_exec_mtx))
    {
        result = ZW_ERR_NO_RES;
        goto l_ZWNET_INI_ERR10;
    }

    if (!plt_mtx_init(&nw->cch_set_ctx.mtx))
    {
        result = ZW_ERR_NO_RES;
        goto l_ZWNET_INI_ERR10_B;
    }

    if (!plt_mtx_init(&nw->mul_cmd_sm_mtx))
    {
        result = ZW_ERR_NO_RES;
        goto l_ZWNET_INI_ERR10_C;
    }

    if (!plt_sem_init(&nw->cb_sem))
    {
        result = ZW_ERR_NO_RES;
        goto l_ZWNET_INI_ERR11;
    }

    if (!plt_mtx_init(&nw->cb_mtx))
    {
        result = ZW_ERR_NO_RES;
        goto l_ZWNET_INI_ERR12;
    }

    if (!plt_sem_init(&nw->wait_q_sem))
    {
        result = ZW_ERR_NO_RES;
        goto l_ZWNET_INI_ERR13;
    }

    //Init state-machine
    nw->init_sm_sta = INI_STA_IDLE;
    nw->ni_sm_sta = ZWNET_STA_IDLE;
    nw->nu_sm_ctx.sta = NU_STA_IDLE;

    //Reset operation to "no operation"
    nw->curr_op = ZWNET_OP_NONE;

    //Save a copy of network initialization data
    nw->init = *init;

    //Init all the back links
    nw->ctl.net = nw;
    nw->ctl.ep.node = &nw->ctl;

    //
    //Init lower layers
    //
    nw->appl_ctx.application_command_handler_cb = application_command_handler_cb;
    nw->appl_ctx.node_sts_cb = zwnet_node_sts_cb;
    nw->appl_ctx.nodelst_cb = zwnet_unsolic_node_list_cb;
    nw->appl_ctx.add_node_sts_cb = zwnet_unsolic_node_add_cb;
    nw->appl_ctx.ss_started_cb = zwnet_unsolic_ss_started_cb;
    nw->appl_ctx.inif_cb = zwnet_unsolic_inif_cb;
    nw->appl_ctx.pl_entry_cb = zwnet_unsolic_pl_cb;
    nw->appl_ctx.appl_sts_cb = zwnet_appl_status_cb;
    nw->appl_ctx.cb_tmout_ms = APPL_CB_TMOUT_MIN;
    nw->appl_ctx.data = nw;
    nw->appl_ctx.plt_ctx = &nw->plt_ctx;

#ifdef TCP_PORTAL
    init_portal_prm.multi_client = nw->multi_client = 0;   //Disable multiple Z/IP clients support
    init_portal_prm.portal_fd = nw->init.portal_fd;
    init_portal_prm.portal_ssl = nw->init.portal_ssl;
    init_portal_prm.portal_prof = (tls_clnt_prof_t *)&nw->init.portal_prof;
    init_portal_prm.zip_router = nw->init.zip_router;
    init_portal_prm.err_cb = zwnet_net_err_cb;
    init_portal_prm.user = nw;

    result = zwhci_init(&nw->appl_ctx, &init_portal_prm);

#else
    init_ce_prm.multi_client = nw->multi_client = 0;    //Disable multiple Z/IP clients support initially
    init_ce_prm.zip_router = nw->init.zip_router;
    init_ce_prm.host_port = nw->init.host_port;
    init_ce_prm.use_ipv4 = nw->init.use_ipv4;
    init_ce_prm.dtls_psk = nw->init.dtls_psk;
    init_ce_prm.dtls_psk_len = nw->init.dtls_psk_len;

    result = zwhci_init(&nw->appl_ctx, &init_ce_prm);

#endif
    if (result != 0)
    {
        debug_zwapi_msg(&nw->plt_ctx, "Init %s layer failed", init_layer[result]);
        result = ZW_ERR_TRANSPORT_INI - (result - 1);
        goto l_ZWNET_INI_ERR14;
    }

    //Save a copy of Z/IP gateway address which could be either IPv6 address or IPv4-mapped IPv6 address
    memcpy(nw->zip_router_addr,
           nw->appl_ctx.ssn_ctx.frm_ctx.tpt_ctx.zip_router, IPV6_ADDR_LEN);

#ifdef TCP_PORTAL
    //Save PAN prefix
    memcpy(nw->zip_pan, nw->init.portal_prof.clnt_pan_prefix, IPV6_ADDR_LEN);
#endif

    nw->cmd_q_thrd_run = 1;
    if (plt_thrd_create(zwnode_cmd_q_thrd, nw) < 0)
    {
        result = ZW_ERR_NO_RES;
        goto l_ZWNET_INI_ERR15;
    }

    nw->wait_q_thrd_run = 1;
    if (plt_thrd_create(zwnode_wait_q_thrd, nw) < 0)
    {
        result = ZW_ERR_NO_RES;
        goto l_ZWNET_INI_ERR15;
    }

    nw->nw_exec_thrd_run = 1;
    if (plt_thrd_create(zwnet_exec_thrd, nw) < 0)
    {
        result = ZW_ERR_NO_RES;
        goto l_ZWNET_INI_ERR15;
    }

    nw->cb_thrd_run = 1;
    if (plt_thrd_create(zwnet_cb_thrd, nw) < 0)
    {
        result = ZW_ERR_NO_RES;
        goto l_ZWNET_INI_ERR15;
    }

    //Initialize poll facility
    nw->poll_ctx->net = nw;
    result = zwpoll_init(nw->poll_ctx);
    if (result != 0)
    {
        debug_zwapi_msg(&nw->plt_ctx, "Init poll facility failed:%d", result);
        goto l_ZWNET_INI_ERR15;
    }
    nw->poll_enable = 1;

    //Initialize post-set poll facility
    nw->spoll_ctx = (zwspoll_ctx_t *)calloc(1, sizeof(zwspoll_ctx_t));

    if (!nw->spoll_ctx)
    {
        debug_zwapi_msg(&nw->plt_ctx, "Failed to calloc for post-set poll");
        goto l_ZWNET_INI_ERR15;
    }

    nw->spoll_ctx->net = nw;
    result = zwspoll_init(nw->spoll_ctx);
    if (result != 0)
    {
        free(nw->spoll_ctx);
        nw->spoll_ctx = NULL;
        debug_zwapi_msg(&nw->plt_ctx, "Init post-set poll facility failed:%d", result);
        goto l_ZWNET_INI_ERR15;
    }

    //Initialize supervision facility
    nw->spv_ctx = (zwspv_ctx_t *)calloc(1, sizeof(zwspv_ctx_t));

    if (!nw->spv_ctx)
    {
        debug_zwapi_msg(&nw->plt_ctx, "Failed to calloc for supervision");
        goto l_ZWNET_INI_ERR15;
    }

    nw->spv_ctx->net = nw;
    result = zwspv_init(nw->spv_ctx);
    if (result != 0)
    {
        free(nw->spv_ctx);
        nw->spv_ctx = NULL;
        debug_zwapi_msg(&nw->plt_ctx, "Init supervision facility failed:%d", result);
        goto l_ZWNET_INI_ERR15;
    }

    //Initialize device polling facility
    nw->dpoll_ctx = (zwdpoll_ctx_t *)calloc(1, sizeof(zwdpoll_ctx_t));

    if (!nw->dpoll_ctx)
    {
        debug_zwapi_msg(&nw->plt_ctx, "Failed to calloc for device poll");
        goto l_ZWNET_INI_ERR15;
    }

    nw->dpoll_ctx->net = nw;
    result = zwdpoll_init(nw->dpoll_ctx);
    if (result != 0)
    {
        free(nw->dpoll_ctx);
        nw->dpoll_ctx = NULL;
        debug_zwapi_msg(&nw->plt_ctx, "Init device poll facility failed:%d", result);
        goto l_ZWNET_INI_ERR15;
    }

    //Initialize event generation facility
    nw->evtg_ctx = (zwevtg_ctx_t *)calloc(1, sizeof(zwevtg_ctx_t));

    if (!nw->evtg_ctx)
    {
        debug_zwapi_msg(&nw->plt_ctx, "Failed to calloc for event generation");
        goto l_ZWNET_INI_ERR15;
    }

    nw->evtg_ctx->net = nw;
    result = zwevtg_init(nw->evtg_ctx);
    if (result != 0)
    {
        free(nw->evtg_ctx);
        nw->evtg_ctx = NULL;
        debug_zwapi_msg(&nw->plt_ctx, "Init event generation facility failed:%d", result);
        goto l_ZWNET_INI_ERR15;
    }

    //Initialize node status polling facility
    nw->nsts_ctx = (nodests_ctx_t *)calloc(1, sizeof(nodests_ctx_t));

    if (!nw->nsts_ctx)
    {
        debug_zwapi_msg(&nw->plt_ctx, "Failed to calloc for node status polling");
        goto l_ZWNET_INI_ERR15;
    }

    nw->nsts_ctx->net = nw;
    result = nodests_init(nw->nsts_ctx);
    if (result != 0)
    {
        free(nw->nsts_ctx);
        nw->nsts_ctx = NULL;
        debug_zwapi_msg(&nw->plt_ctx, "Init node status polling facility failed:%d", result);
        goto l_ZWNET_INI_ERR15;
    }

    //Initialize the saving network information to persistent storage facility
    nw->zwsave_ctx = (zwsave_ctx_t *)calloc(1, sizeof(zwsave_ctx_t));

    if (!nw->zwsave_ctx)
    {
        debug_zwapi_msg(&nw->plt_ctx, "Failed to calloc for network save facility");
        goto l_ZWNET_INI_ERR15;
    }

    nw->zwsave_ctx->net = nw;
    result = zwsave_init(nw->zwsave_ctx);
    if (result != 0)
    {
        free(nw->zwsave_ctx);
        nw->zwsave_ctx = NULL;
        debug_zwapi_msg(&nw->plt_ctx, "Init network save facility failed:%d", result);
        goto l_ZWNET_INI_ERR15;
    }

    if ((nw->ifsm_ctx = ifsm_ctx_init(nw)) == NULL)
    {
        debug_zwapi_msg(&nw->plt_ctx, "Failed to initialize interface state-machines context");
        goto l_ZWNET_INI_ERR15;
    }

    //Start network initialization state-machine
    nw->curr_op = ZWNET_OP_INITIALIZE;

    init_sm_prm.nm_op = ZWNET_OP_INITIALIZE;
    init_sm_prm.enable_file_load = 1;

    if (zwnet_init_sm(nw, EVT_INI_START, (uint8_t *)&init_sm_prm) != 0)
    {
        nw->curr_op = ZWNET_OP_NONE;
        debug_zwapi_msg(&nw->plt_ctx, "Fail to start zwnet_init_sm");
        result = ZW_ERR_FAILED;
        goto l_ZWNET_INI_ERR15;
    }

    debug_zwapi_msg(&nw->plt_ctx, "zwnet_init done!");

    //No error
    nw->init_done = 1;
    *net = nw;
    return ZW_ERR_NONE;

l_ZWNET_INI_ERR15:
    zwnet_exit(nw);
    return result;

l_ZWNET_INI_ERR14:
    plt_sem_destroy(nw->wait_q_sem);
l_ZWNET_INI_ERR13:
    plt_mtx_destroy(nw->cb_mtx);
l_ZWNET_INI_ERR12:
    plt_sem_destroy(nw->cb_sem);
l_ZWNET_INI_ERR11:
    plt_sem_destroy(nw->mul_cmd_sm_mtx);
l_ZWNET_INI_ERR10_C:
    plt_mtx_destroy(nw->cch_set_ctx.mtx);
l_ZWNET_INI_ERR10_B:
    plt_mtx_destroy(nw->nw_exec_mtx);
l_ZWNET_INI_ERR10:
    plt_sem_destroy(nw->nw_exec_sem);
l_ZWNET_INI_ERR9:
    plt_mtx_destroy(nw->cmd_q_mtx);
l_ZWNET_INI_ERR8:
    plt_sem_destroy(nw->cmd_q_sem);
l_ZWNET_INI_ERR7:
    plt_mtx_destroy(nw->mtx);
l_ZWNET_INI_ERR6:
    plt_exit(&nw->plt_ctx);
l_ZWNET_INI_ERR5:
    free(nw->poll_ctx);
l_ZWNET_INI_ERR4:
	while (nw->stAGIData.pAGIGroupList != NULL)
	{
		AGI_Group *pGroup = nw->stAGIData.pAGIGroupList;
		nw->stAGIData.pAGIGroupList = nw->stAGIData.pAGIGroupList->pNext;
		free(pGroup);
		pGroup = NULL;
	}

l_ZWNET_INI_ERR3:
    free(nw->pref_dir);
    free(nw->net_info_dir);
#ifdef SUPPORT_SIMPLE_AV_CONTROL
    free(nw->av_sup_bitmask);
#endif
    zwdev_cfg_free(nw->dev_cfg_rec_buf.dev_cfg_records, nw->dev_cfg_rec_buf.dev_cfg_cnt);
    free(nw->sup_cmd_cls);
l_ZWNET_INI_ERR2:
//    free(nw->node_info_file);
//l_ZWNET_INI_ERR1:
    free(nw);
    return result;

}


/**
zwnet_exit - clean up network
@param[in]	net network
@return
*/
void zwnet_exit(zwnet_p net)
{
    int             wait_count;
    int             result;
    util_lst_t      *req_lst;
    cmd_q_req_t     *cmd_q_req;
    mcmd_sm_ctx_t   *sm_ctx = &net->mul_cmd_sm_ctx;

    //Cancel any network management operation
    zwnet_abort(net);

    //Set flags ASAP to stop everything
    net->cb_thrd_run = 0;
    //net->appl_ctx.ssn_ctx.frm_ctx.tpt_ctx.wr_thrd_run = 0;
    //net->appl_ctx.ssn_ctx.frm_ctx.tpt_ctx.rd_thrd_run = 0;
    net->appl_ctx.ssn_ctx.frm_ctx.run = 0;
    net->appl_ctx.ssn_ctx.cb_thrd_run = 0;
    net->appl_ctx.cb_thrd_run = 0;
    net->cmd_q_thrd_run = net->wait_q_thrd_run = net->nw_exec_thrd_run = 0;

    if (net->poll_enable)
    {
        net->poll_ctx->run = 0;
    }

    if (net->spoll_ctx)
    {
        net->spoll_ctx->run = 0;
    }

    if (net->spv_ctx)
    {
        net->spv_ctx->run = 0;
    }

    if (net->dpoll_ctx)
    {
        net->dpoll_ctx->run = 0;
    }

    if (net->wupoll_ctx)
    {
        net->wupoll_ctx->run = 0;
    }

    if (net->evtg_ctx)
    {
        net->evtg_ctx->run = 0;
    }

    //Stop all threads
    plt_sem_post(net->cmd_q_sem);
    plt_sem_post(net->wait_q_sem);
    plt_sem_post(net->nw_exec_sem);
    plt_sem_post(net->cb_sem);

    //Unblock all "wait for send" threads
    zwhci_shutdown(&net->appl_ctx);

    //Wait all threads to exit
    while (net->cmd_q_thrd_sts)
    {
        plt_sleep(100);
    }

    while (net->wait_q_thrd_sts)
    {
        plt_sleep(100);
    }

    while (net->nw_exec_thrd_sts)
    {
        plt_sleep(100);
    }

    wait_count = 50;    //Wait for 5 seconds
    while (wait_count-- > 0)
    {
        if (net->cb_thrd_sts == 0)
            break;
        plt_sleep(100);
    }

    //Give user application a chance to complete any pending callback from HCAPI
    plt_sleep(100);

    //Flush lists
    while ((req_lst = util_list_get(net->cmd_q_mtx, &net->cmd_q_req_hd)) != NULL)
    {
        cmd_q_req = (cmd_q_req_t *)req_lst->wr_buf;
        zwnode_mul_cmd_rm_q(net, &cmd_q_req->req_hd);
        free(req_lst);
    }

    util_list_flush(net->mtx, &net->wait_q_req_hd);
    util_list_flush(net->mtx, &net->cb_req_hd);

    //Shutdown poll facility
    if (net->poll_enable)
    {
        zwpoll_shutdown(net->poll_ctx);
    }

    //Shutdown post-set poll facility
    if (net->spoll_ctx)
    {
        zwspoll_shutdown(net->spoll_ctx);
    }

    //Shutdown supervision facility
    if (net->spv_ctx)
    {
        zwspv_shutdown(net->spv_ctx);
    }

    //Shutdown device poll facility
    if (net->dpoll_ctx)
    {
        zwdpoll_shutdown(net->dpoll_ctx);
    }

    //Shutdown wakeup notification poll facility
    if (net->wupoll_ctx)
    {
        wkuppoll_shutdown(net->wupoll_ctx);
    }

    //Shutdown event generation facility
    if (net->evtg_ctx)
    {
        zwevtg_shutdown(net->evtg_ctx);
    }

    //Shutdown node status polling facility
    if (net->nsts_ctx)
    {
        nodests_shutdown(net->nsts_ctx);
    }

    //Shutdown network save facility
    if (net->zwsave_ctx)
    {
        zwsave_shutdown(net->zwsave_ctx);
    }

    //Shutdown provisioning list state-machine
    zwnet_plst_shutdown(net);

    //Shutdown interface state-machines context
    if (net->ifsm_ctx)
    {
        ifsm_ctx_shutdown(net->ifsm_ctx);
    }

    //Shutdown multi-command handler state-machine
    plt_mtx_lck(net->mtx);
    plt_tmr_stop(&net->plt_ctx, sm_ctx->tmr_ctx);
    sm_ctx->tmr_ctx = NULL;
    zwnet_mul_cmd_sm(net, EVT_MUL_CMD_SHUTDOWN, NULL, 0);
    plt_mtx_ulck(net->mtx);


    //Check whether to save node info.  Don't save into file if home id = 0 or
    //network initialization failed;
    //as this indicates the internal network structure was not initialized.
    if (net->node_info_file && (net->homeid != 0) && (!net->nw_init_failed))
    {
        result = zwutl_ni_save(net, net->node_info_file);
        if (result < 0)
        {
            debug_zwapi_msg(&net->plt_ctx, "Save node information file '%s' failed: %d", net->node_info_file, result);
        }
    }

    //Shutdown all lower layers
    zwhci_exit(&net->appl_ctx);

#ifdef TCP_PORTAL
    //Close TLS connection
    tls_conn_close(net->init.portal_fd, net->init.portal_ssl);
#endif

    //Free resources
    zwnet_free(net);

}


/**
zwnet_sm_ini_failed - Stop the network initialization state-machine due to failure
@param[in]	nw		Network
@return
*/
static void    zwnet_sm_ini_failed(zwnet_p nw)
{
    //Reset to idle state
    nw->init_sm_sta = INI_STA_IDLE;

    //Reset operation to "no operation"
    nw->curr_op = ZWNET_OP_NONE;

    //Set flag to indicate network initialization failed
    if (nw->init_sm_prm.nm_op == ZWNET_OP_INITIALIZE)
    {
        nw->nw_init_failed = 1;
    }

    //Invoke notify callback
#ifdef IGNORE_INIT_ERROR
    zwnet_notify(nw, nw->init_sm_prm.nm_op, OP_DONE, NULL);
#else
    zwnet_notify(nw, nw->init_sm_prm.nm_op, OP_FAILED, NULL);
#endif

}


/**
zwnet_sm_ini_tout_cb - Network initialization state machine timer callback
@param[in] data     Pointer to network
@return
*/
static void    zwnet_sm_ini_tout_cb(void *data)
{
    zwnet_p   nw = (zwnet_p)data;

    //Stop timer
    plt_mtx_lck(nw->mtx);
    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
    nw->sm_tmr_ctx = NULL;//make sure timer context is null, else restart timer will crash

    //Call state-machine
    zwnet_init_sm(nw, EVT_INI_TMOUT, NULL);
    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_ctlr_id_addr_cb - Request controller node id/PAN address callback function
@param[in]	appl_ctx    The application layer context
@param[in]	rpt		    Status report
@return
*/
static void zwnet_ctlr_id_addr_cb(appl_layer_ctx_t *appl_ctx, nd_advt_rpt_t *rpt)
{
    zwnet_p             nw = (zwnet_p)appl_ctx->data;
    zwnet_ctlr_evt_t    evt = EVT_CTLR_ID_ADDR_FAILED;

    ALOGI("zwnet_ctlr_id_addr_cb:");
    switch (rpt->status)
    {
        case ZIP_ND_INFORMATION_OK:
            ALOGI("Home id:%08X,  ctlr node id:%u, local=%u, ip:", rpt->homeid, rpt->node_id, rpt->addr_local);
            debug_zwapi_bin_msg(&nw->plt_ctx, rpt->ipv6_addr, IPV6_ADDR_LEN);
            if (rpt->addr_local == 0)
            {   //Global address
                evt = EVT_CTLR_NODE_ID;
            }
          else
          {   //Site-local (PAN) address
              evt = EVT_CTLR_PAN_ADDR;
          }
            break;

        case ZIP_ND_INFORMATION_OBSOLETE:
            debug_zwapi_msg(&nw->plt_ctx, "Obsolete node id:%u, local=%u, ip:", rpt->node_id, rpt->addr_local);
            debug_zwapi_bin_msg(&nw->plt_ctx, rpt->ipv6_addr, IPV6_ADDR_LEN);
            break;

        case ZIP_ND_INFORMATION_NOT_FOUND:
            debug_zwapi_msg(&nw->plt_ctx, "Node not found");
            break;

    }
    //Call state-machine
    plt_mtx_lck(nw->mtx);
    zwnet_ctlr_info_sm(nw, evt, (uint8_t *)rpt);
    plt_mtx_ulck(nw->mtx);
}


/**
zwnet_n2ip_lst_cb - Resolve node to ip address state-machine completion callback
@param[in]	nw          network
@param[in]  status	    0=ok; non-zero=failed
@param[in]	prm	        user parameters
@return
@pre        Caller must lock the nw->mtx before calling this function.
*/
static void zwnet_n2ip_lst_cb(zwnet_p nw, int status, void **prm)
{
    uint8_t sts;

    sts = status & 0xFF;

	if (((uintptr_t)prm[0]) == ZWNET_OP_INITIALIZE)
    {
        zwnet_init_sm(nw, EVT_INI_NODE_ID_ADDR, &sts);
    }
	else if (((uintptr_t)prm[0]) == ZWNET_OP_UPDATE)
    {
        zwnet_updt_sm(nw, EVT_NU_NODE_ID_ADDR, &sts);
    }
	else if (((uintptr_t)prm[0]) == ZWNET_OP_NW_CHANGED)
    {
        zwnet_nw_change_sm(nw, EVT_NC_NODE_ID_ADDR, &sts);
    }

}


/**
zwnet_ctlr_intf_create - Create Z/IP gateway interfaces and hide them, except a few interfaces
@param[in]	node		    Node to which the newly created interface will be attached
@param[in]	node_info       Node information
@return		ZW_ERR_xxx.
*/
static int zwnet_ctlr_intf_create(zwnode_p node, appl_node_info_t *node_info)
{
    unsigned    i;
    unsigned    tot_intf;
    int         result;
    int         found;
    zwif_p      intf;
    uint16_t    *new_cmd_cls;
    uint8_t     new_cmd_cnt;
    uint16_t    ctlr_cmd_cls[] = {  //Non-hidden command classes
                                   COMMAND_CLASS_FIRMWARE_UPDATE_MD,
                                   COMMAND_CLASS_ZIP_GATEWAY,
                                   COMMAND_CLASS_ZIP_PORTAL
                                 };


    result = zwnet_cmd_cls_add(COMMAND_CLASS_BASIC, node_info->cmd_cls, node_info->cmd_cnt,
                               &new_cmd_cls, &new_cmd_cnt);

    if (result < 0)
    {
        return result;
    }

    //Remove any duplicate command classes
    new_cmd_cnt = zwnet_cmd_cls_dedup(new_cmd_cls, new_cmd_cnt);

    //Create unsecure interfaces based on command classes
    for (i=0; i < new_cmd_cnt; i++)
    {
        intf = zwif_create(new_cmd_cls[i], 1, IF_PROPTY_UNSECURE);

        if (!intf)
        {
            if (new_cmd_cls != node_info->cmd_cls)
            {
                free(new_cmd_cls);
            }
            return ZW_ERR_MEMORY;
        }

        //Save back link to end point
        intf->ep = &node->ep;

        //Add interface to the end point
        if (node->ep.intf)
        {   //Second interface onwards
            zwobj_add(&node->ep.intf->obj.next, &intf->obj);
        }
        else
        {   //First interface
            node->ep.intf = intf;
        }
    }

    //Clear and set node security property
    node->propty &= ~(NODE_PROPTY_SECURE_CAP_S0 | NODE_PROPTY_SECURE_CAP_S2 | NODE_PROPTY_ADD_SECURE | NODE_PROPTY_ADD_INSECURE);
    zwnet_node_secure_propty_set(node, new_cmd_cls, new_cmd_cnt, node_info->cmd_cnt_sec);

    //Update security commands
    //Note: It is possible for Z/IP gateway being included unsecurely but has secure interfaces at the LAN side
    result = zwnet_ni_sec_updt(&node->ep, node_info->cmd_cls_sec, node_info->cmd_cnt_sec);

    //Set all interfaces as hidden except those listed in ctlr_cmd_cls[]
    tot_intf = sizeof(ctlr_cmd_cls)/sizeof(uint16_t);
    intf = node->ep.intf;
    while (intf)
    {
        for (i=found=0; i < tot_intf; i++)
        {
            if (ctlr_cmd_cls[i] == intf->cls)
            {
                found=1;
                break;
            }
        }

        if (!found)
        {   //set to hidden
            //tiny
            //intf->propty |= IF_PROPTY_HIDDEN;
        }

        intf = (zwif_p)intf->obj.next;
    }

    if (new_cmd_cls != node_info->cmd_cls)
    {
        free(new_cmd_cls);
    }

    return result;
}


/**
zwnet_nif_set - Create a command class list from local supported cc list, both secure and insecure suitable to add onto Z/IP gateway NIF
@param[in]      nw		        Network
@param[out]     cmd_cls_buf     The buffer to store the generated command classes
@param[in,out]  buf_len         Input: specify the cmd_cls_buf length in bytes. Output: the length of output
@return       0 if o.k.; else return non-zero
*/
static int zwnet_nif_set(zwnet_p  nw, uint8_t *cmd_cls_buf, uint8_t *buf_len)
{
    int         i;
    int         j;
    uint8_t     buf_len_avail = *buf_len;
    uint8_t     buf_index;
    uint8_t     buf_idx_secure = 0; //Buffer index for start of secure command class
    uint8_t     cmd_cls_ppty_msk;
    uint16_t    cmd_cls;

    buf_index = 0;
    for (j=0; j<2; j++)
    {
        if (j==0)
        {   //Process insecure command class
            cmd_cls_ppty_msk = BITMASK_CMD_CLS_INSECURE;
        }
        else
        {   //Process secure command class
            if (buf_len_avail < 2)
            {
                return ZW_ERR_MEMORY;
            }

            //Add security scheme 0 mark
            cmd_cls_buf[buf_index++] = 0xF1;
            cmd_cls_buf[buf_index++] = 0;
            buf_len_avail -= 2;
            buf_idx_secure = buf_index;

            cmd_cls_ppty_msk = BITMASK_CMD_CLS_SECURE;
        }

        for (i=0; i<nw->sup_cmd_cls_cnt; i++)
        {
            if (nw->sup_cmd_cls[i].propty & cmd_cls_ppty_msk)
            {
                cmd_cls = nw->sup_cmd_cls[i].cls;

                //Deduplicate against Z/IP gateway command class
                //if (!zwnet_sup_cmd_cls_find(nw->gw_sup_cmd_cls, cmd_cls, nw->gw_sup_cmd_cls_cnt))
                {   //Not found in existing Z/IP gateway NIF.  Add the command class to output list
                    if (cmd_cls > 0xF100)
                    {   //Extended command class
                        if (buf_len_avail > 1)
                        {
                            cmd_cls_buf[buf_index++] = (uint8_t)(cmd_cls >> 8);
                            cmd_cls_buf[buf_index++] = (uint8_t)(cmd_cls & 0x00FF);
                            buf_len_avail -= 2;
                        }
                    }
                    else
                    {
                        if (buf_len_avail > 0)
                        {
                            cmd_cls_buf[buf_index++] = (uint8_t)(cmd_cls & 0x00FF);
                            buf_len_avail--;
                        }
                    }
                }
            }
        }
    }

    *buf_len = (buf_index > buf_idx_secure)? buf_index : (buf_index - 2);
    return 0;

}


/**
zwnet_dev_rec_find - Find device specific configuration record
@param[in]	nw		    Network
@param[in]	vid		    Vendor ID
@param[in]	ptype		Product Type ID
@param[in]	pid		    Product ID
@param[out]	dev_rec     Device record
@return  Non-zero on found; zero on not found
*/
int zwnet_dev_rec_find(zwnet_p nw, uint16_t vid, uint16_t ptype, uint16_t pid, dev_rec_t *dev_rec)
{
    dev_rec_srch_key_t  srch_key = {vid, ptype, pid};

    //Check whether use user supplied search function
    if (nw->dev_rec_find_fn)
    {
        return nw->dev_rec_find_fn(nw->dev_cfg_ctx, &srch_key, dev_rec);
    }
    else
    {
        return zwdev_cfg_find(&srch_key, nw->dev_cfg_rec_buf.dev_cfg_records, nw->dev_cfg_rec_buf.dev_cfg_cnt, dev_rec);
    }
}


/**
zwnet_dev_if_propty_tmout_get - Get interface property based on interface version and from device database (higher priority)
@param[in]	ep	        Endpoint
@param[in]	cmd_cls	    Command class
@param[in]	real_ver	Real command class version as reported by the device
@param[in]	propty	    Original interface property
@param[out]	new_propty	New interface property
@param[out]	tmout	    Interface timeout value
@return
*/
void zwnet_dev_if_propty_tmout_get(zwep_p ep, uint16_t cmd_cls, uint8_t real_ver,
                                   uint8_t propty, uint8_t *new_propty, uint16_t *tmout)
{
    if_rec_t    *if_rec;
    uint16_t    time_out = 0;

    //Set property based on real version
    switch(cmd_cls)
    {
        case COMMAND_CLASS_ALARM:
            if (real_ver < 4)
            {   //Not capable to sent event clear notification
                propty &= ~IF_PROPTY_ALRM_EVT_CLR;
                time_out = DFLT_EVT_CLR_TIMEOUT;
            }
            else
            {   //Capable to sent event clear notification
                propty |= IF_PROPTY_ALRM_EVT_CLR;
            }
            break;

        case COMMAND_CLASS_SENSOR_BINARY:
        case COMMAND_CLASS_SENSOR_ALARM:
            //Default to capable of sending event clear
            propty |= IF_PROPTY_BSNSR_EVT_CLR;
            break;

    }

    //Clear flag that was set by device database previously and was saved in persistent storage
    propty &= ~IF_PROPTY_HIDDEN_POLL;

    //Check for node database entry validity
    if (!ep->node->dev_cfg_valid)
    {   //No entry found for this node
        goto l_DEV_PROPTY_IF_DONE;
    }

    if_rec = zwdev_if_get_by_cmd_cls(ep->node->dev_cfg_rec.ep_rec, ep->epid, cmd_cls);
    if (!if_rec)
    {
        goto l_DEV_PROPTY_IF_DONE;
    }

    //Valid node database entry
    switch(cmd_cls)
    {
        case COMMAND_CLASS_ALARM:
            if (!(if_rec->propty & IF_PROPTY_ALRM_EVT_CLR))
            {   //Not capable to sent event clear notification
                propty &= ~IF_PROPTY_ALRM_EVT_CLR;
                time_out = if_rec->tmout;
            }
            break;

        case COMMAND_CLASS_SENSOR_BINARY:
        case COMMAND_CLASS_SENSOR_ALARM:
            if (!(if_rec->propty & IF_PROPTY_BSNSR_EVT_CLR))
            {   //Not capable to sent event clear notification
                propty &= ~IF_PROPTY_BSNSR_EVT_CLR;
                time_out = if_rec->tmout;
            }
            break;

    }

    //For all interfaces
    if ((if_rec->propty & IF_PROPTY_HIDDEN_POLL) != 0)
    {
        //tiny
        //propty  |= IF_PROPTY_HIDDEN_POLL;
    }

l_DEV_PROPTY_IF_DONE:
    *new_propty = propty;
    *tmout = time_out;
}


/**
zwnet_dev_usr_def_ver_updt - Update user-defined interface version in the node
@param[in]	node		Node
@return
*/
void zwnet_dev_usr_def_ver_updt(zwnode_p  node)
{
    zwep_p      ep;
    zwif_p      intf;
    zwif_p      new_intf;

    uint8_t     usr_def_ver;
    uint8_t     real_ver;
    uint8_t     propty;
    uint16_t    tmout;

    if (!node->dev_cfg_valid)
    {
        return;
    }

    ep = &node->ep;

    while (ep)
    {
        intf = ep->intf;
        while (intf)
        {
            //Get user-defined version
            usr_def_ver = zwdev_if_ver_get(node->dev_cfg_rec.ep_rec, ep->epid, intf->cls);

            //Get interface property according to device database
            zwnet_dev_if_propty_tmout_get(ep, intf->cls, intf->real_ver, intf->propty, &propty, &tmout);

            if (propty != intf->propty)
            {   //Device database has modified interface property
                intf->propty = propty;
            }

            if (tmout != intf->tmout)
            {   //Device database has modified time out value
                intf->tmout = tmout;
            }

            //Update user-defined version
            if (usr_def_ver > intf->ver)
            {
                real_ver = intf->ver;

                //Replace the old interface with a new interface
                new_intf = zwif_create(intf->cls, usr_def_ver, propty);

                if (new_intf)
                {
                    //Update real version
                    new_intf->real_ver = real_ver;

                    //Update timeout value
                    new_intf->tmout = tmout;

                    //Save back link to end point
                    new_intf->ep = ep;

                    //Move interface specific data to the newly created interface
                    zwif_dat_mv(intf, new_intf);

                    //Replace the old interface in the end point
                    zwobj_rplc((zwobj_p *)(&ep->intf), &intf->obj, &new_intf->obj);

                    intf = new_intf;

                }
            }

            intf = (zwif_p)intf->obj.next;
        }
        ep = (zwep_p)ep->obj.next;
    }
}

/**
zwnet_add_user_def_intf - Add user defined interface into the endpoints of the node
@param[in]	node  Node
@return
@Note Real version for the interface will be 0. If such interface has already existed in the endpoint, it won't
be created.
*/
void zwnet_add_user_def_intf(zwnode_p node)
{
	zwep_p  ep;
	zwif_p  new_intf;
	zwif_p  intf;
	ep_rec_t  *ep_rec_list;

	if (node->dev_cfg_valid)
	{
		for (ep_rec_list = node->dev_cfg_rec.ep_rec; ep_rec_list != NULL; ep_rec_list = ep_rec_list->next)
		{
			if (ep_rec_list->new_if != 0)
			{
				//Need to create a new/fake interface for this endpoint
				//For now only create unsecure interface

                ep = zwep_find(&node->ep, ep_rec_list->id);

                if (!ep)
                {
                    continue;
                }

                ep_rec_list->new_if_propty |= IF_PROPTY_UNSECURE;

				new_intf = zwif_create(ep_rec_list->new_if, ep_rec_list->new_if_ver, ep_rec_list->new_if_propty /*IF_PROPTY_UNSECURE*/);

				if (new_intf)
				{
					//Real version is 0 for new/fake interface
					new_intf->real_ver = 0;

                    intf = zwif_find_cls(ep->intf, new_intf->cls);
                    if (intf && (intf->real_ver != 0))
                    {   //A real interface has already existed, don't modify the real interface.
                        zwif_dat_rm(new_intf);
                       free(new_intf);
                       continue;
                    }

					//Save back link to end point
					new_intf->ep = ep;

					if (intf && intf->real_ver == 0)
					{
						//Move interface specific data to the newly created interface
						zwif_dat_mv(intf, new_intf);

						//Replace the old interface in the end point
						zwobj_rplc((zwobj_p *)(&ep->intf), &intf->obj, &new_intf->obj);
					}
					else
					{
						//Add interface to the end point
						if (ep->intf)
						{   //Second interface onwards
							zwobj_add(&ep->intf->obj.next, &new_intf->obj);
						}
						else
						{   //First interface
							ep->intf = new_intf;
						}
					}
				}
			}
		}
	}
}


/**
zwnet_devdb_node_handler - Modify node based on device database
@param[in]	node  Node
@return
*/
void zwnet_devdb_node_handler(zwnode_p node)
{
    if (zwnet_dev_rec_find(node->net, node->vid, node->vtype, node->pid, &node->dev_cfg_rec))
    {
        node->dev_cfg_valid = 1;
        node->category = node->dev_cfg_rec.category;
        //Update user-defined interface versions in the node
        zwnet_dev_usr_def_ver_updt(node);
        //Check for any interfaces to "create"
        zwnet_add_user_def_intf(node);
    }
    else
    {
        node->dev_cfg_valid = 0;
    }
}


/**
zwnet_load - load network node information and settings
@param[in]	nw		        Network
@param[in]	node_info_file	Node information file name.
@return		ZW_ERR_XXX
*/
int zwnet_load(zwnet_p nw, const char *node_info_file)
{
    int         result;
    zwnode_t    save_ctlr;

    //Save a copy of controller node
    save_ctlr = nw->ctl;

    //Check whether to load node information file
    if (node_info_file)
    {
        result = zwutl_ni_load(nw, node_info_file);
        if (result < 0)
        {
            debug_zwapi_msg(&nw->plt_ctx, "Load node info file '%s' failed: %d", node_info_file, result);
            return result;
        }
        debug_zwapi_msg(&nw->plt_ctx, "Network info loaded ...");
    }

    //Restore controller node info
    nw->ctl.app_ver = save_ctlr.app_ver;
    nw->ctl.proto_ver = save_ctlr.proto_ver;
    nw->ctl.lib_type = save_ctlr.lib_type;
    nw->ctl.basic = save_ctlr.basic;
    nw->ctl.ep.specific = save_ctlr.ep.specific;
    nw->ctl.ep.generic = save_ctlr.ep.generic;
    nw->ctl.propty = save_ctlr.propty;
    nw->ctl.listen = 1; //Always listening


    if (save_ctlr.s2_keys_valid)
    {   //Restore valid s2 keys assigned to controller before node info file was loaded
        nw->ctl.s2_keys_valid = 1;
        nw->ctl.s2_grnt_keys = save_ctlr.s2_grnt_keys;
    }
    else if (nw->ctl.s2_keys_valid)
    {   //Node information file contains valid S2 granted keys, re-assess the controller node security property

        //Clear and set node security property
        nw->ctl.propty &= ~(NODE_PROPTY_SECURE_CAP_S0 | NODE_PROPTY_SECURE_CAP_S2 | NODE_PROPTY_ADD_SECURE | NODE_PROPTY_ADD_INSECURE);
        zwnode_secure_propty_set(&nw->ctl);

    }

    return ZW_ERR_NONE;
}


/**
zwnet_save - Save network node information and security layer key and setting into a file
@param[in]  net             Network
@param[in]  node_info_file  Node information file name
@return         ZW_ERR_XXX.
*/
int zwnet_save(zwnet_p net, const char *node_info_file)
{
    if (node_info_file)
    {
        return zwutl_ni_save(net, node_info_file);
    }

    return ZW_ERR_FILE_OPEN;
}


/**
zwnet_app_nif_set - Push application and local supported command classes to Z/IP gateway
@param[in]	nw		    Network
@param[in]	zipgw_ifd	Z/IP gateway command class interface descriptor
@return		ZW_ERR_XXX
*/
static void zwnet_app_nif_set(zwnet_p nw, zwifd_p zipgw_ifd)
{
    int     cmd_cnt = 0;
    uint8_t cmd[200 + 2];
    uint8_t cmd_buf_len = 200;
#ifdef  DYNAMIC_APP_NIF_SET
    uint8_t cmd_cls_ppty = BITMASK_CMD_CLS_INSECURE;
    uint8_t intf_ppty = IF_PROPTY_UNSECURE;
#endif

    //
    // Set supported command class in node information frame
    //
    cmd[cmd_cnt++] = COMMAND_CLASS_ZIP_GATEWAY;
    cmd[cmd_cnt++] = COMMAND_APPLICATION_NODE_INFO_SET;
#ifdef  DYNAMIC_APP_NIF_SET
    //-----------------------------------------------------------------------------------------
    //Determine if we are �alone� in the network, if so set Application NIF to Non-Secure NIF
    //If other nodes in the network and we are still secure, set Application NIF to Secure NIF
    //If other nodes in the network and we are non-secure, set Application NIF to Non-Secure NIF
    //-----------------------------------------------------------------------------------------
    if (nw->ctl.obj.next && (nw->ctl.propty & NODE_PROPTY_ADD_SECURE))
    {   //Other nodes in the network and Z/IP gateway is secure
        cmd_cls_ppty = BITMASK_CMD_CLS_SECURE;
        intf_ppty = IF_PROPTY_SECURE;
        debug_zwapi_msg(&nw->plt_ctx, "APPL NIF is set to secure");
    }
    else
    {
        debug_zwapi_msg(&nw->plt_ctx, "APPL NIF is set to unsecure");
    }

    zwnet_sup_cmd_cls_ppty_set(nw->sup_cmd_cls, nw->sup_cmd_cls_cnt, cmd_cls_ppty);
    zwnet_ctlr_sec_ppty_set(nw->ctl.ep.intf, nw->sup_cmd_cls, nw->sup_cmd_cls_cnt, intf_ppty);
#endif
    if ((zwnet_nif_set(nw, cmd + cmd_cnt, &cmd_buf_len) == 0)
        && cmd_buf_len) //Make sure there is at least a command class to set
    {
        cmd_cnt += cmd_buf_len;

        //Send the command
        zwif_exec_ex(zipgw_ifd, cmd, cmd_cnt, zwif_tx_sts_cb, NULL, ZWIF_OPT_GW_PORTAL, NULL);
    }
}


/**
zwnet_sm_ini_node_lst_retry_cb - Get node list retry timer callback
@param[in] data     Pointer to network
@return
*/
static void zwnet_sm_ini_node_lst_retry_cb(void *data)
{
    zwnet_p   nw = (zwnet_p)data;

    //Stop timer
    plt_mtx_lck(nw->mtx);
    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
    nw->sm_tmr_ctx = NULL;//make sure timer context is null, else restart timer will crash

    //Call state-machine
    zwnet_init_sm(nw, EVT_INI_NODE_LIST_RETRY, NULL);
    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_init_sm - Network initialization state-machine
@param[in] nw		Network
@param[in] evt      The event for the state-machine
@param[in] data     The data associated with the event
@return             0 if the state-machine is started from idle state; else return non-zero
@pre    Caller must lock the nw->mtx before calling this function
*/
static int    zwnet_init_sm(zwnet_p  nw, zwnet_init_evt_t  evt, uint8_t *data)
{
    int     result;

    switch (nw->init_sm_sta)
    {
        //----------------------------------------------------------------
        case INI_STA_IDLE:
        //----------------------------------------------------------------
            if (evt == EVT_INI_START)
            {
                ctlr_sm_ctx_t   ctlr_sm_ctx = {0};

                //Save the parameters
                nw->init_sm_prm = *((zwnet_init_sm_prm_t *)data);

                ctlr_sm_ctx.ctlr_sm_cb = zwnet_ini_ctlr_info_cmplt_cb;
                ctlr_sm_ctx.initiate_ctx = nw->init_sm_prm.initiate_ctx;

                if (zwnet_ctlr_info_sm(nw, EVT_CTLR_START, (uint8_t *)&ctlr_sm_ctx))
                {
                    //Change state
                    nw->init_sm_sta = INI_STA_CTLR_INFO;

                    return 0;//state-machine started successfully
                }
                debug_zwapi_msg(&nw->plt_ctx, "zwnet_init_sm: failed to get Z/IP gateway controller info");
            }
            break;

        //----------------------------------------------------------------
        case INI_STA_CTLR_INFO:
        //----------------------------------------------------------------
            if (evt == EVT_INI_CTLR_INFO_DONE)
            {
                int *status = (int *)data;

                if (*status == 0)
                {   //o.k.

                    //Execute unhandled commands from the start of learn mode till now
                    nw->init_sm_sta = INI_STA_EXE_UNHANDLE_CMD;
                    zwnet_exec_unhandle_cmd(nw);

                    //Get node list
                    result = zip_node_list_get(&nw->appl_ctx, zwnet_node_list_get_cb);
                    nw->sm_busy_retry = nw->sm_gen_retry = 0;   //reset retry counts

                    if (result == 0)
                    {
                        //Change state
                        nw->init_sm_sta = INI_STA_NODE_LIST;

                        //Start timer
                        nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_ini_tout_cb, nw);

                        break;
                    }

                    debug_zwapi_msg(&nw->plt_ctx, "zip_node_list_get with error:%d", result);
                }

                zwnet_sm_ini_failed(nw);

            }
            break;

        //----------------------------------------------------------------
        case INI_STA_NODE_LIST:
        //----------------------------------------------------------------
            switch (evt)
            {
                case EVT_INI_TMOUT:
                    debug_zwapi_msg(&nw->plt_ctx, "zwnet_init_sm: get node list timeout");

                    if (nw->sm_gen_retry++ < ZWNET_CTLR_SM_RETRY)
                    {
                        //Start the delay timer
                        nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, 100, zwnet_sm_ini_node_lst_retry_cb, nw);
                        break;
                    }

                    if (nw->init_sm_prm.nm_op == ZWNET_OP_INITIATE)
                    {
                        //Change to idle state
                        nw->init_sm_sta = INI_STA_IDLE;

                        //Reset operation to "no operation"
                        nw->curr_op = ZWNET_OP_NONE;

                        //Invoke notify callback
                        zwnet_notify(nw, nw->init_sm_prm.nm_op, OP_DONE, NULL);
                    }
                    else
                    {
                        zwnet_sm_ini_failed(nw);
                    }
                    break;

                case EVT_INI_APPL_STATUS:
                    if (data[0] == APPLICATION_BUSY)
                    {
                        if (nw->sm_busy_retry++ < ZWNET_CTLR_BUSY_RETRY)
                        {
                            plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                            //Start the delay timer
                            nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_APPL_BUSY_RETRY_INTV, zwnet_sm_ini_node_lst_retry_cb, nw);
                        }
                    }
                    break;

                case EVT_INI_NODE_LIST_RETRY:
                    {
                        //Get node list
                        result = zip_node_list_get(&nw->appl_ctx, zwnet_node_list_get_cb);

                        if (result == 0)
                        {
                            debug_zwapi_msg(&nw->plt_ctx, "zwnet_ini_sm: Retrying get node list ...");

                            //Start timer
                            nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_ini_tout_cb, nw);

                            break;
                        }
                        zwnet_sm_ini_failed(nw);
                    }
                    break;

                case EVT_INI_NODE_LIST:
                    {
                        zwnode_p    node;
#ifdef TCP_PORTAL
                        uint8_t     cmd[48];
#endif
                        zwifd_t     zipgw_ifd;
                        zwif_p      zipgw_intf;

                        //Stop timer
                        plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                        nw->sm_tmr_ctx = NULL;

                        //Load node information from file
                        if (nw->init_sm_prm.enable_file_load)
                        {
                            zwnet_load(nw, nw->node_info_file);
                        }

                        //Get Z/IP gateway interface
                        zipgw_intf = zwif_find_cls(nw->ctl.ep.intf, COMMAND_CLASS_ZIP_GATEWAY);

                        if (zipgw_intf)
                        {
                            zwif_get_desc(zipgw_intf, &zipgw_ifd);

                            zwnet_app_nif_set(nw, &zipgw_ifd);

#ifdef TCP_PORTAL
                            //
                            //Configure unsolicited destination address
                            //
                            //Prepare the set command
                            cmd[0] = COMMAND_CLASS_ZIP_GATEWAY;
                            cmd[1] = UNSOLICITED_DESTINATION_SET;
                            memcpy(cmd + 2, nw->init.portal_prof.clnt_unsolicited_dst, IPV6_ADDR_LEN);
                            cmd[18] = (nw->init.portal_prof.clnt_unsolicited_port >> 8); //MSB
                            cmd[19] = (nw->init.portal_prof.clnt_unsolicited_port & 0xFF); //LSB

                            //Send the command
                            zwif_exec_ex(&zipgw_ifd, cmd, 20, zwif_tx_sts_cb, NULL, ZWIF_OPT_GW_PORTAL, NULL);
#else
                            //
                            //Get existing unsolicited destination address
                            //
                            //Setup report callback
                            result = zwif_set_report(&zipgw_ifd, zwnet_unsolicit_rpt_cb, UNSOLICITED_DESTINATION_REPORT);

                            if (result == 0)
                            {
                                //Request for report
                                result = zwif_get_report_ex(&zipgw_ifd, NULL, 0, UNSOLICITED_DESTINATION_GET,
                                                            zwif_tx_sts_cb, ZWIF_RPT_GET_ZIP);

                                if (result == 0)
                                {
                                    //Change state
                                    nw->init_sm_sta = INI_STA_CTLR_UNSOLICIT;

                                    //Start timer
                                    nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_ini_tout_cb, nw);
                                    break;
                                }
                                else
                                {
                                    debug_zwapi_msg(&nw->plt_ctx, "zwnet_init_sm: get unsolicited dst with error:%d", result);
                                }
                            }
                            else
                            {
                                debug_zwapi_msg(&nw->plt_ctx, "zwnet_init_sm: zwif_set_report with error:%d", result);
                            }
#endif

                        }
                        else
                        {
                            debug_zwapi_msg(&nw->plt_ctx, "zwnet_init_sm: zip gateway interface not found");
                        }

                        //Discover the node-ip address information
                        if (((node = (zwnode_p)nw->ctl.obj.next) != NULL))
                        {
                            n2ip_sm_ctx_t   n2ip_sm_ctx = {0};

                            //Create the node id list
                            n2ip_sm_ctx.num_node = 0;
                            while (node && (n2ip_sm_ctx.num_node < MAX_ZWAVE_NODE))
                            {
                                n2ip_sm_ctx.node_id[n2ip_sm_ctx.num_node++] = node->nodeid;
                                node = (zwnode_p)node->obj.next;
                            }

                            //Call the resolve node to ip address state-machine
                            n2ip_sm_ctx.cb = zwnet_n2ip_lst_cb;
                            n2ip_sm_ctx.params[0] = (void *)(unsigned)ZWNET_OP_INITIALIZE;

                            result = zwnet_n2ip_sm(nw, EVT_N2IP_START, (uint8_t *)&n2ip_sm_ctx);

                            if (result == 0)
                            {
                                //Change state
                                nw->init_sm_sta = INI_STA_NODE_IP_DSCVRY;

                            }
                            else
                            {
                                debug_zwapi_msg(&nw->plt_ctx, "Failed to get node HAN address");
                                zwnet_sm_ini_failed(nw);
                            }
                            break;
                        }

                        //There are no other nodes in the network
//                      //Load node information from file
//                      if (nw->init_sm_prm.enable_file_load)
//                      {
//                          zwnet_load(nw, nw->node_info_file);
//                      }

                        //Change to idle state
                        nw->init_sm_sta = INI_STA_IDLE;

                        //Reset operation to "no operation"
                        nw->curr_op = ZWNET_OP_NONE;

                        //Invoke notify callback
                        zwnet_notify(nw, nw->init_sm_prm.nm_op, OP_DONE, NULL);

                    }
                    break;

                default:
                    break;

            }
            break;

#ifndef TCP_PORTAL
        //----------------------------------------------------------------
        case INI_STA_CTLR_UNSOLICIT:
        //----------------------------------------------------------------
            switch (evt)
            {
                zwnode_p    node;

                case EVT_INI_TMOUT:
                    {
                        debug_zwapi_msg(&nw->plt_ctx, "zwnet_init_sm: get unsolicited destination address timeout");
                    }
                    //fall through

                case EVT_INI_CTLR_UNSOLICIT:
                    {

                        if (evt == EVT_INI_CTLR_UNSOLICIT)
                        {
                            uint8_t     *dst_ip;
                            zwifd_p     ifd;

                            //Stop timer
                            plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                            nw->sm_tmr_ctx = NULL;

                            memcpy(&dst_ip, data + 2, sizeof(uint8_t *));
                            memcpy(&ifd, data + 2 + sizeof(uint8_t *), sizeof(zwifd_p));

                            if (util_ipv6_zero_cmp(dst_ip) == 0)
                            {   //Unsolicited destination address is unconfigured
                                uint8_t     local_ipv4[IPV4_ADDR_LEN];
                                uint8_t     cmd[20];
                                uint8_t     local_ip[IPV6_ADDR_LEN];

                                //Configure unsolicited destination address
                                if (nw->init.use_ipv4)
                                {
                                    result = util_local_addr_get(&nw->zip_router_addr[12], local_ip, 1);

                                    //Save the local IPv4 address
                                    memcpy(local_ipv4, local_ip, IPV4_ADDR_LEN);
                                    //Convert the IPv4 address to IPv4-mapped IPv6 address
                                    memset(local_ip, 0, IPV6_ADDR_LEN);
                                    local_ip[10] = 0xFF;
                                    local_ip[11] = 0xFF;
                                    memcpy(&local_ip[12], local_ipv4, IPV4_ADDR_LEN);
                                }
                                else
                                {   //IPv6
                                    //result = util_local_addr_get(nw->zip_router_addr, local_ip, 0);
                                    memcpy(local_ip, nw->appl_ctx.ssn_ctx.frm_ctx.tpt_ctx.listen_addr, IPV6_ADDR_LEN);
                                    result = 0;

                                }

                                if (result == 0)
                                {
                                    //Prepare the set command
                                    cmd[0] = COMMAND_CLASS_ZIP_GATEWAY;
                                    cmd[1] = UNSOLICITED_DESTINATION_SET;
                                    memcpy(cmd + 2, local_ip, IPV6_ADDR_LEN);
                                    cmd[18] = (nw->appl_ctx.ssn_ctx.frm_ctx.tpt_ctx.host_port >> 8); //MSB
                                    cmd[19] = (nw->appl_ctx.ssn_ctx.frm_ctx.tpt_ctx.host_port & 0xFF); //LSB

                                    //Send the command
                                    zwif_exec_ex(ifd, cmd, 20, zwif_tx_sts_cb, NULL, ZWIF_OPT_GW_PORTAL, NULL);
                                }
                                else
                                {
                                    debug_zwapi_msg(&nw->plt_ctx, "zwnet_init_sm: get local addr failed:%d", result);
                                }
                            }
                        }

                        //Discover the node-ip address information
                        if (((node = (zwnode_p)nw->ctl.obj.next) != NULL))
                        {
                            n2ip_sm_ctx_t   n2ip_sm_ctx = {0};

                            //Create the node id list
                            n2ip_sm_ctx.num_node = 0;
                            while (node && (n2ip_sm_ctx.num_node < MAX_ZWAVE_NODE))
                            {
                                n2ip_sm_ctx.node_id[n2ip_sm_ctx.num_node++] = node->nodeid;
                                node = (zwnode_p)node->obj.next;
                            }

                            //Call the resolve node to ip address state-machine
                            n2ip_sm_ctx.cb = zwnet_n2ip_lst_cb;
                            n2ip_sm_ctx.params[0] = (void *)(unsigned)ZWNET_OP_INITIALIZE;

                            result = zwnet_n2ip_sm(nw, EVT_N2IP_START, (uint8_t *)&n2ip_sm_ctx);

                            if (result == 0)
                            {
                                //Change state
                                nw->init_sm_sta = INI_STA_NODE_IP_DSCVRY;

                            }
                            else
                            {
                                debug_zwapi_msg(&nw->plt_ctx, "Failed to get node HAN address");
                                zwnet_sm_ini_failed(nw);
                            }
                            break;
                        }

                        //There are no other nodes in the network
//                      //Load node information from file
//                      if (nw->init_sm_prm.enable_file_load)
//                      {
//                          zwnet_load(nw, nw->node_info_file);
//                      }

                        //Change to idle state
                        nw->init_sm_sta = INI_STA_IDLE;

                        //Reset operation to "no operation"
                        nw->curr_op = ZWNET_OP_NONE;

                        //Invoke notify callback
                        zwnet_notify(nw, nw->init_sm_prm.nm_op, OP_DONE, NULL);
                    }
                    break;

                default:
                    break;

            }
            break;
#endif

        //----------------------------------------------------------------
        case INI_STA_NODE_IP_DSCVRY:
        //----------------------------------------------------------------
            if (evt == EVT_INI_NODE_ID_ADDR)
            {
                if (*data == 0)
                {
                    zwnode_p  node;
                    sm_job_t  sm_job = {0};
                    uint8_t   node_id[MAX_ZWAVE_NODE];


//                  //Load node information from file
//                  if (nw->init_sm_prm.enable_file_load)
//                  {
//                      zwnet_load(nw, nw->node_info_file);
//                  }

                    //Load node information through network for nodes that are not loaded
                    //with detailed information
                    sm_job.op = nw->init_sm_prm.nm_op;
                    sm_job.num_node = 0;
                    sm_job.node_id = node_id;
                    sm_job.cb = zwnet_node_info_cmplt_cb;
                    sm_job.auto_cfg = 0;
                    sm_job.delay_slp_node = 1;

                    //Create the node id list
                    node = (zwnode_p)nw->ctl.obj.next;
                    while (node)
                    {
                        if (node->ep.intf)
                        {
                            //Modify node based on device database
                            zwnet_devdb_node_handler(node);

                            //Set node security property if it has not been set yet
                            zwnode_secure_propty_set(node);
                        }
                        else
                        {   //Add node without interface to the query list
                            node_id[sm_job.num_node++] = node->nodeid;
                        }

                        //Mark node which couldn't resolve its id to IP address as sleeping
                        if (nw->init_sm_prm.enable_file_load
                            && (!nw->appl_ctx.node_ip_tbl[node->nodeid-1].valid))
                        {
                            appl_updt_node_sts(&nw->appl_ctx, node->nodeid, APPL_NODE_STS_DOWN, 1);
                        }
                        node = (zwnode_p)node->obj.next;
                    }

                    //Submit job to the state-machine
                    if (sm_job.num_node > 0)
                    {
                        if (zwnet_node_info_sm(nw, EVT_GET_NODE_INFO, (uint8_t *)&sm_job))
                        {
                            //Change state
                            nw->init_sm_sta = INI_STA_NODE_INFO;
                        }
                        else
                        {
                            zwnet_sm_ini_failed(nw);
                        }
                        break;
                    }

                    //
                    //  All nodes loaded with detailed information
                    //

                    //Change to idle state
                    nw->init_sm_sta = INI_STA_IDLE;

                    //Reset operation to "no operation"
                    nw->curr_op = ZWNET_OP_NONE;

                    //Invoke notify callback
                    zwnet_notify(nw, nw->init_sm_prm.nm_op, OP_DONE, NULL);
                    break;
                }
                //Resolve node to ip address failed
                debug_zwapi_msg(&nw->plt_ctx, "Failed to get node HAN address");
                zwnet_sm_ini_failed(nw);
            }
            break;

        //----------------------------------------------------------------
        case INI_STA_NODE_INFO:
        //----------------------------------------------------------------
            if (evt == EVT_INI_NI_UPDT_DONE)
            {
                //Reset to idle state
                nw->init_sm_sta = INI_STA_IDLE;
            }
            break;

        default:
            break;
    }

    return -1;
}


/**
zwnet_sm_rst_failed - Stop the controller reset state-machine due to failure
@param[in]	nw		Network
@return
*/
static void    zwnet_sm_rst_failed(zwnet_p nw)
{
    //Reset to idle state
    nw->rst_sm_sta = RST_STA_IDLE;

    //Reset operation to "no operation"
    nw->curr_op = ZWNET_OP_NONE;

    //Invoke notify callback
    zwnet_notify(nw, ZWNET_OP_RESET, OP_FAILED, NULL);

}


/**
zwnet_sm_rst_tout_cb - Controller reset state machine timer callback
@param[in] data     Pointer to network
@return
*/
static void    zwnet_sm_rst_tout_cb(void *data)
{
    zwnet_p   nw = (zwnet_p)data;

    //Stop timer
    plt_mtx_lck(nw->mtx);
    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
    nw->sm_tmr_ctx = NULL;

    //Call state-machine
    zwnet_rst_sm(nw, EVT_RST_TMOUT, NULL);
    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_rst_sm_resend_DRL_cb - Timer callback to ask the state-machine to resend "device reset locally"
@param[in] data     Pointer to network
@return
*/
static void    zwnet_rst_sm_resend_DRL_cb(void *data)
{
    zwnet_p   nw = (zwnet_p)data;
    uint8_t   snd_drl = 2;  //Don't reset retry counter

    //Stop timer
    plt_mtx_lck(nw->mtx);
    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
    nw->sm_tmr_ctx = NULL;

    //Call state-machine
    zwnet_rst_sm(nw, EVT_RST_START, &snd_drl);
    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_rst_sm_send_dflt_set_cb - Timer callback to ask the state-machine to send "default set"
@param[in] data     Pointer to network
@return
*/
static void    zwnet_rst_sm_send_dflt_set_cb(void *data)
{
    zwnet_p   nw = (zwnet_p)data;

    //Stop timer
    plt_mtx_lck(nw->mtx);
    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
    nw->sm_tmr_ctx = NULL;

    //Call state-machine
    zwnet_rst_sm(nw, EVT_RST_DRL_TX_STS, NULL);
    plt_mtx_ulck(nw->mtx);

}


/**
zip_send_DRL_cb - callback for sending "device reset locally"
@param[in]	appl_ctx	Context
@param[in]	tx_sts      Transmit completion status
@param[in]	user_prm    User parameter
@param[in]	node_id     Node id
@param[in]	param       Additional parameters
@return
@remark Not declared as static and put the declaration in header file for application layer callback.
		Purpose of this function is to make the ZIP_FLAG_ACK_REQ flag set when the data is sent.
*/
void zip_send_DRL_cb(appl_layer_ctx_t *appl_ctx, int8_t tx_sts, void *user_prm, uint8_t node_id, zw_tx_cb_prm_t *param)
{
    zwnet_p   nw = (zwnet_p)user_prm;

    if (tx_sts != 0)
    {
        debug_msg_show(appl_ctx->plt_ctx, "zip_send_DRL_cb error:%d", tx_sts);
    }

    plt_mtx_lck(nw->mtx);

    //Call state-machine
    zwnet_rst_sm(nw, EVT_RST_DRL_TX_STS, NULL);
    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_reset_cb - Restore factory default callback function
@param[in]	appl_ctx    The application layer context
@param[in]	sts         Status of the restore factory default operation
@param[in]	nodeid      Node id
@return
*/
static void zwnet_reset_cb(appl_layer_ctx_t *appl_ctx, uint8_t sts, uint8_t nodeid)
{
    //DEFAULT_SET_DONE = 6
    //DEFAULT_SET_BUSY = 7
    zwnet_p     nw = (zwnet_p)appl_ctx->data;
    const char *status_msg[] = { "OK", "Failed",  "unknown"};
    int status;

    status = ((sts >= 6) && (sts <= 7))? (sts - 6) : 2;
    debug_zwapi_msg(&nw->plt_ctx, "Restored factory default: node id:%u, status:%u(%s)", nodeid, sts, status_msg[status]);

    //Call state-machine
    plt_mtx_lck(nw->mtx);
    zwnet_rst_sm(nw, (sts == 6)? EVT_RST_DONE : EVT_RST_FAILED, NULL);
    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_rst_sm - Controller reset state-machine
@param[in]	nw		Network
@param[in] evt      The event for the state-machine
@param[in] data     The data associated with the event
@return             0 if the state-machine is started from idle state; else return non-zero
@pre    Caller must lock the nw->mtx before calling this function
*/
static int    zwnet_rst_sm(zwnet_p  nw, zwnet_rst_evt_t  evt, uint8_t *data)
{
    int32_t result;

    switch (nw->rst_sm_sta)
    {
        //----------------------------------------------------------------
        case RST_STA_IDLE:
        //----------------------------------------------------------------
            if (evt == EVT_RST_START)
            {
                if (*data)
                {   //Send "device reset locally"
                    zwif_p          intf;
                    appl_snd_data_t *prm;
                    uint8_t         cmd_buf[4];

                    if (*data == 1)
                    {
                        nw->sm_gen_retry = 0;
                    }

                    prm = (appl_snd_data_t *)calloc(1, sizeof(appl_snd_data_t));
                    if (!prm)
                    {
                        break;
                    }

                    cmd_buf[0] = COMMAND_CLASS_DEVICE_RESET_LOCALLY;
                    cmd_buf[1] = DEVICE_RESET_LOCALLY_NOTIFICATION;
                    prm->dat_buf = cmd_buf;
                    prm->dat_len = 2;
                    prm->node_id = nw->stAGIData.pAGIGroupList->stNodeEpInfo.byNodeID;

                    //Get controller device reset locally interface
                    intf = zwif_find_cls(nw->ctl.ep.intf, COMMAND_CLASS_DEVICE_RESET_LOCALLY);

                    if (intf)
                    {
                        if (nw->use_encap_hdr)
                        {   //Deprecate "secure origin" bit in Z/IP header. Alwasy set "secure origin" bit to 1
                            prm->flag = ZIP_FLAG_SECURE;

                            prm->flag |= ZIP_FLAG_ENCAP_INFO;

                            if (intf->propty & IF_PROPTY_SECURE)
                            {   //Secure message
                                //Use the Z/IP gateway controller highest
                                //granted key if available. If not, default to S0 key.
                                prm->encap_fmt = (nw->ctl.s2_keys_valid)? zwutil_sec_encap_fmt_get(nw->ctl.s2_grnt_keys) :
                                                                          (((uint16_t)SEC_KEY_BITMSK_S0)<< 8);
                            }
                        }
                        else
                        {
                            if (intf->propty & IF_PROPTY_SECURE)
                            {
                                prm->flag = ZIP_FLAG_SECURE;
                            }
                        }
                    }

                    result = zw_send_data(&nw->appl_ctx, prm, zip_send_DRL_cb, nw);
                    free(prm);

                    if (result < 0)
                    {
                        debug_zwapi_msg(&nw->plt_ctx, "send device reset locally with error:%d", result);

                        if ((result == SESSION_ERROR_PREVIOUS_COMMAND_UNCOMPLETED) || (result == APPL_ERROR_WAIT_CB))
                        {
                            if (nw->sm_gen_retry++ < ZWNET_CTLR_SM_RETRY)
                            {   //Timeout, retry later
                                plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);

                                //Start timer to retry later
                                nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_TMOUT, zwnet_rst_sm_resend_DRL_cb, nw);

                                return 0;//state-machine started successfully
                            }
                        }
                    }
                    else
                    {   //Send O.K.

                        //Change state
                        nw->rst_sm_sta = RST_STA_DEV_RST_LOCAL;

                        nw->sm_gen_retry = 0;

                        //Stop timer
                        plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);

                        nw->sm_tmr_ctx = NULL;

                        return 0;//state-machine started successfully

                    }
                }

                //Skip sending "device reset locally" or failed to send it, send "default set" to Z/IP gateway instead.
                nw->rst_sm_sta = RST_STA_DEV_RST_LOCAL;

                nw->sm_gen_retry = 0;

                plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);

                //Start timer
                nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, 100, zwnet_rst_sm_send_dflt_set_cb, nw);

                return 0;//state-machine started successfully

            }
            break;

        //----------------------------------------------------------------
        case RST_STA_DEV_RST_LOCAL:
        //----------------------------------------------------------------
            switch (evt)
            {
                case EVT_RST_DRL_TX_STS:
                    //Stop timer
                    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                    nw->sm_tmr_ctx = NULL;

                    //Reset Z/IP gateway controller
                    result = zw_set_default(&nw->appl_ctx, zwnet_reset_cb, 0);

                    if (result < 0)
                    {
                        debug_zwapi_msg(&nw->plt_ctx, "send default set with error:%d", result);

                        if ((result == SESSION_ERROR_PREVIOUS_COMMAND_UNCOMPLETED) || (result == APPL_ERROR_WAIT_CB))
                        {
                            if (nw->sm_gen_retry++ < 2)
                            {   //Timeout, retry later
                                plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);

                                //Start timer to retry
                                nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_TMOUT, zwnet_rst_sm_send_dflt_set_cb, nw);

                                break;
                            }
                        }
                        zwnet_sm_rst_failed(nw);
                    }
                    else
                    {   //Send O.K.
                        //Change state
                        nw->rst_sm_sta = RST_STA_RESET;

                        //Stop timer
                        plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);

                        //Start timer
                        nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_RST_IP_NW_TMOUT, zwnet_sm_rst_tout_cb, nw);
                    }
                    break;

                default:
                    break;

            }
            break;

        //----------------------------------------------------------------
        case RST_STA_RESET:
        //----------------------------------------------------------------
            switch (evt)
            {
                case EVT_RST_TMOUT:
                    debug_zwapi_msg(&nw->plt_ctx, "zwnet_rst_sm: reset Z/IP gateway controller timeout");
                    zwnet_sm_rst_failed(nw);
                    break;

                case EVT_RST_FAILED:
                    //Stop timer
                    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                    nw->sm_tmr_ctx = NULL;

                    debug_zwapi_msg(&nw->plt_ctx, "zwnet_rst_sm: reset Z/IP gateway controller failed");
                    zwnet_sm_rst_failed(nw);
                    break;

                case EVT_RST_DONE:
                    {
                        ctlr_sm_ctx_t   ctlr_sm_ctx = {0};

                        //Stop timer
                        plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                        nw->sm_tmr_ctx = NULL;

                        zwnet_clear_all(nw);

                        ctlr_sm_ctx.ctlr_sm_cb = zwnet_rst_ctlr_info_cmplt_cb;

                        if (zwnet_ctlr_info_sm(nw, EVT_CTLR_START, (uint8_t *)&ctlr_sm_ctx))
                        {
                            //Change state
                            nw->rst_sm_sta = RST_STA_CTLR_INFO;

                            break;
                        }

                        debug_zwapi_msg(&nw->plt_ctx, "zwnet_rst_sm: failed to get Z/IP gateway controller info");
                        zwnet_sm_rst_failed(nw);
                    }
                    break;

                default:
                    break;

            }
            break;

        //----------------------------------------------------------------
        case RST_STA_CTLR_INFO:
        //----------------------------------------------------------------
            if (evt == EVT_RST_CTLR_INFO_DONE)
            {
                int *status = (int *)data;

                if (*status == 0)
                {   //o.k.
                    //Get node list
                    result = zip_node_list_get(&nw->appl_ctx, zwnet_node_list_get_cb);

                    if (result == 0)
                    {
                        //Change state
                        nw->rst_sm_sta = RST_STA_NODE_LIST;

                        //Start timer
                        nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_rst_tout_cb, nw);

                        break;
                    }

                    debug_zwapi_msg(&nw->plt_ctx, "zip_node_list_get with error:%d", result);
                }

                //Reset node list controller id to controller
                nw->nlist_ctl_id = nw->ctl.nodeid;

                zwnet_sm_rst_failed(nw);

            }
            break;

        //----------------------------------------------------------------
        case RST_STA_NODE_LIST:
        //----------------------------------------------------------------
            switch (evt)
            {
                case EVT_RST_TMOUT:
                    debug_zwapi_msg(&nw->plt_ctx, "zwnet_rst_sm: zip_node_list_get timeout");
                    nw->nlist_ctl_id = nw->ctl.nodeid;
                    zwnet_sm_rst_failed(nw);
                    break;

                case EVT_RST_NODE_LIST:
                    {
                        zwifd_t zipgw_ifd;
                        zwif_p  zipgw_intf;

                        //Stop timer
                        plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                        nw->sm_tmr_ctx = NULL;

                        //Get Z/IP gateway interface
                        zipgw_intf = zwif_find_cls(nw->ctl.ep.intf, COMMAND_CLASS_ZIP_GATEWAY);

                        if (zipgw_intf)
                        {
                            zwif_get_desc(zipgw_intf, &zipgw_ifd);

                            zwnet_app_nif_set(nw, &zipgw_ifd);
                        }

                        //Reset to idle state
                        nw->rst_sm_sta = RST_STA_IDLE;

                        //Reset operation to "no operation"
                        nw->curr_op = ZWNET_OP_NONE;

                        //Invoke notify callback
                        zwnet_notify(nw, ZWNET_OP_RESET, OP_DONE, NULL);
                    }
                    break;

                default:
                    break;
            }
            break;
    }

    return -1;
}


/**
zwnet_get_desc - get read-only network descriptor
@param[in]	net		network
@return		network descriptor
*/
zwnetd_p zwnet_get_desc(zwnet_p net)
{
    plt_mtx_lck(net->mtx);
    net->net_desc.id = net->homeid;
    net->net_desc.ctl_id = net->ctl.nodeid;
    net->net_desc.ctl_role = net->ctl_role;
    net->net_desc.ctl_cap = net->ctl_cap;
    net->net_desc.ctl_zw_role = net->zwave_role;
    net->net_desc.user = net->init.user;
    net->net_desc.plt_ctx = &net->plt_ctx;
    plt_mtx_ulck(net->mtx);

    return &net->net_desc;
}


/**
zwnet_sm_ni_stop - Stop the node info state-machine
@param[in]	nw		Network
@return
@pre        Caller must lock the nw->mtx before calling this function.
*/
static void    zwnet_sm_ni_stop(zwnet_p nw)
{
    //Stop timer
    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
    nw->sm_tmr_ctx = NULL;    //make sure timer context is null, else restart timer will crash

    //Reset end point information state-machine to idle state
    nw->ep_sm_sta = EP_STA_IDLE;

    //Reset single end point information state-machine to idle state
    nw->single_ep_sta = S_EP_STA_IDLE;
    memset(&nw->single_ep_job, 0, sizeof(ep_sm_job_t));

    //Reset node information state-machine to idle state
    nw->ni_sm_sta = ZWNET_STA_IDLE;
    if (nw->sm_job.node_id)
    {
        free(nw->sm_job.node_id);
        nw->sm_job.node_id = NULL;
    }
    nw->sm_job.num_node = 0;

}


/**
zwnet_sm_nu_stop - Stop the network update state-machine
@param[in]	nw		Network
@return
*/
static void    zwnet_sm_nu_stop(zwnet_p nw)
{
    plt_mtx_lck(nw->mtx);

    //Stop node info update
    zwnet_sm_ni_stop(nw);

    //Reset to idle state
    nw->nu_sm_ctx.sta = NU_STA_IDLE;
    nw->nu_sm_ctx.num_node = 0;
    nw->ctlr_sm_sta = CTLR_STA_IDLE;
    nw->n2ip_sm_ctx.state = N2IP_STA_IDLE;

    //Reset operation to "no operation"
    nw->curr_op = ZWNET_OP_NONE;

    plt_mtx_ulck(nw->mtx);
}


/**
zwnet_abort - abort current action/transmission
@param[in]	net		network
@return		ZW_ERR_XXX
*/
int zwnet_abort(zwnet_p net)
{
    int32_t result = 0;
    uint8_t cancel_op;

    plt_mtx_lck(net->mtx);
    cancel_op = net->curr_op;
    plt_mtx_ulck(net->mtx);

    switch (cancel_op)
    {
        case ZWNET_OP_INITIALIZE:
            plt_mtx_lck(net->mtx);
            //Stop state-machines
            zwnet_sm_ni_stop(net);
            net->init_sm_sta = INI_STA_IDLE;
            net->n2ip_sm_ctx.state = N2IP_STA_IDLE;
            net->ctlr_sm_sta = CTLR_STA_IDLE;
            plt_mtx_ulck(net->mtx);
            break;

        case ZWNET_OP_RESET:
            plt_mtx_lck(net->mtx);
            //Stop state-machine
            net->rst_sm_sta = RST_STA_IDLE;
            net->ctlr_sm_sta = CTLR_STA_IDLE;
            //Stop timer
            plt_tmr_stop(&net->plt_ctx, net->sm_tmr_ctx);
            net->sm_tmr_ctx = NULL;
            plt_mtx_ulck(net->mtx);
            break;

        case ZWNET_OP_ADD_NODE:
            {
                if (!net->incl_on_behalf)
                {
                    result = zw_add_node_to_network(&net->appl_ctx, NULL, ADD_NODE_STOP, TRANSMIT_OPTION_EXPLORE);
                    if (result != 0)
                    {
                        ALOGE("zw_add_node_stop with error:%d", result);
                    }
                }
                plt_mtx_lck(net->mtx);
                //Stop state-machines
                zwnet_sm_ni_stop(net);
                net->n2ip_sm_ctx.state = N2IP_STA_IDLE;
                net->add_sm_sta = ADD_STA_IDLE;
                plt_mtx_ulck(net->mtx);
            }
            break;

        case ZWNET_OP_RM_NODE:
            {
                result = zw_remove_node_from_network(&net->appl_ctx, NULL, REMOVE_NODE_STOP);
                if (result != 0)
                {
                    ALOGE("zw_remove_node_from_network with error:%d", result);
                }
                zwnet_notify(net, ZWNET_OP_RM_NODE, OP_FAILED, NULL);
            }
            break;

        case ZWNET_OP_INITIATE:
            {
                result = zw_set_learn_mode(&net->appl_ctx, NULL, 0, ZW_SET_LEARN_MODE_DISABLE);
                if (result != 0)
                {
                    debug_zwapi_msg(&net->plt_ctx, "zw_set_learn_mode 'stop' with error:%d", result);
                }
                plt_mtx_lck(net->mtx);
                //Stop state-machines
                zwnet_sm_ni_stop(net);
                net->init_sm_sta = INI_STA_IDLE;
                net->n2ip_sm_ctx.state = N2IP_STA_IDLE;
                net->ctlr_sm_sta = CTLR_STA_IDLE;
                net->join_sm_sta = JOIN_STA_IDLE;
                plt_mtx_ulck(net->mtx);

            }
            break;

        case ZWNET_OP_MIGRATE:
            {
                result = zw_controller_change(&net->appl_ctx, NULL, CONTROLLER_CHANGE_STOP, TRANSMIT_OPTION_EXPLORE);
                if (result != 0)
                {
                    ALOGI("zw_controller_change with error:%d", result);
                }
                plt_mtx_lck(net->mtx);
                //Stop state-machines
                zwnet_sm_ni_stop(net);
                net->n2ip_sm_ctx.state = N2IP_STA_IDLE;
                plt_mtx_ulck(net->mtx);
            }
            break;

        case ZWNET_OP_UPDATE:
            {
                zwnet_sm_nu_stop(net);
            }
            break;

        case ZWNET_OP_NODE_UPDATE:
        case ZWNET_OP_SLEEP_NODE_UPT:
            {
                plt_mtx_lck(net->mtx);
                zwnet_sm_ni_stop(net);
                plt_mtx_ulck(net->mtx);
            }
            break;

        case ZWNET_OP_RP_NODE:
            {
                if (!net->incl_on_behalf)
                {
                    result = zw_replace_failed_node(&net->appl_ctx, NULL, net->failed_id, 0, STOP_FAILED_NODE_REPLACE);
                    if (result != 0)
                    {
                        ALOGI("zw_replace_failed_node stop with error:%d", result);
                    }
                }
                plt_mtx_lck(net->mtx);
                //Stop state-machines
                zwnet_sm_ni_stop(net);
                net->n2ip_sm_ctx.state = N2IP_STA_IDLE;
                net->add_sm_sta = ADD_STA_IDLE;
                plt_mtx_ulck(net->mtx);
            }
            break;

        case ZWNET_OP_NW_CHANGED:
            {
                plt_mtx_lck(net->mtx);
                //Stop timer
                plt_tmr_stop(&net->plt_ctx, net->nc_sm_ctx.tmr_ctx);
                net->nc_sm_ctx.tmr_ctx = NULL;
                //Stop state-machines
                zwnet_sm_ni_stop(net);
                net->nc_sm_ctx.sta = NC_STA_IDLE;
                net->n2ip_sm_ctx.state = N2IP_STA_IDLE;
                plt_mtx_ulck(net->mtx);

            }
            break;

        case ZWNET_OP_NODE_CACHE_UPT:
            {
                plt_mtx_lck(net->mtx);
                //Stop timer
                plt_tmr_stop(&net->plt_ctx, net->sm_tmr_ctx);
                net->sm_tmr_ctx = NULL;

                //Reset end point information state-machine to idle state
                net->ep_sm_sta = EP_STA_IDLE;

                //Reset single end point information state-machine to idle state
                net->single_ep_sta = S_EP_STA_IDLE;
                memset(&net->single_ep_job, 0, sizeof(ep_sm_job_t));

                //Reset operation to "no operation"
                net->curr_op = ZWNET_OP_NONE;

                plt_mtx_ulck(net->mtx);

            }
            break;

        case ZWNET_OP_SAVE_NW:
            {
                //This operation can't be stopped.  Wait until it has completed.
                int  wait_count = 200;  //Wait for max. of 20 seconds
                while (wait_count-- > 0)
                {
                    if (net->curr_op == ZWNET_OP_NONE)
                        break;
                    plt_sleep(100);
                }
            }
            break;

        case ZWNET_OP_FW_UPDT:
            plt_mtx_lck(net->mtx);
            //Stop state-machine
            net->fw_updt_sta = FW_UPDT_STA_IDLE;
            zwnet_fw_updt_free(net);
            memset(&net->fw_updt_ifd, 0, sizeof(zwifd_t));
            //Stop timer
            plt_tmr_stop(&net->plt_ctx, net->sm_tmr_ctx);
            net->sm_tmr_ctx = NULL;
            plt_mtx_ulck(net->mtx);
            break;

        case ZWNET_OP_HEALTH_CHK:
            {
                plt_mtx_lck(net->mtx);
                //Stop timer
                plt_tmr_stop(&net->plt_ctx, net->sm_tmr_ctx);
                net->sm_tmr_ctx = NULL;

                //Reset state-machine to idle state
                net->nh_chk_sm_ctx.state = NH_CHK_STA_IDLE;

                //Reset operation to "no operation"
                net->curr_op = ZWNET_OP_NONE;

                plt_mtx_ulck(net->mtx);

            }
            break;

    }

    //Reset operation to "no operation"
    plt_mtx_lck(net->mtx);
    if (cancel_op == net->curr_op)
    {
        net->curr_op = ZWNET_OP_NONE;
    }
    plt_mtx_ulck(net->mtx);

    if(result == 0)
    {
        ALOGI("Abort op done, current op is: %d",net->curr_op);
    }

    //Return result
    return(result == 0)? ZW_ERR_NONE : ZW_ERR_OP_FAILED;

}


/**
zwnet_node_rm_cb - Remove node from network callback function
@param[in]	appl_ctx        The application layer context
@param[in]	sts		        Remove node status
@param[in]	rm_node_id     Original node id of the removed node
@return
*/
static void zwnet_node_rm_cb(appl_layer_ctx_t *appl_ctx, uint8_t sts, uint8_t rm_node_id)
{
    const char *status_msg[] = { "OK", "Learn Ready", "Failed", "unknown"};

    int         status;
    zwnet_p     nw = (zwnet_p)appl_ctx->data;

    switch (sts)
    {
        case REMOVE_NODE_STATUS_DONE:
            status = 0;
            break;
        case REMOVE_NODE_STATUS_LEARN_READY:
            status = 1;
            break;
        case REMOVE_NODE_STATUS_FAILED:
            status = 2;
            break;

        default:
            status = 3;
    }

    ALOGI("zwnet_node_rm_cb: status:%u (%s)", sts, status_msg[status]);

    if (sts == REMOVE_NODE_STATUS_DONE)
    {
        zwnode_p    node;

        debug_zwapi_msg(&nw->plt_ctx, "removed node id:%u", rm_node_id);

        if (rm_node_id > 0)
        {   //Valid node id
            plt_mtx_lck(nw->mtx);
            node = zwnode_find(&nw->ctl, rm_node_id);
            if (node)
            {
                zwnet_node_cb(nw, node->nodeid, ZWNET_NODE_REMOVED);
                //Remove the node
                zip_close_dtls_conn(&nw->appl_ctx, rm_node_id);
                zip_node_ipaddr_rm(&nw->appl_ctx, rm_node_id);
                zwnode_rm(nw, rm_node_id);

            }
            plt_mtx_ulck(nw->mtx);
        }
    }

    //Cancel operation without sending REMOVE_NODE_STOP

    //Reset operation to "no operation"
    //nw->curr_op = ZWNET_OP_NONE;

    //Callback to notify status
    if (sts == REMOVE_NODE_STATUS_DONE)
    {
        //Reset operation to "no operation"
        nw->curr_op = ZWNET_OP_NONE;

        zwnet_sts_t info = {0};

        info.type = ZWNET_STS_INFO_NODE_ID;
        info.info.node_id = rm_node_id;

        zwnet_notify(nw, ZWNET_OP_RM_NODE, OP_DONE, &info);
    }
    else if(sts == REMOVE_NODE_STATUS_LEARN_READY)
    {
        zwnet_notify(nw, ZWNET_OP_RM_NODE, REMOVE_NODE_STATUS_LEARN_READY, NULL);
    }
    else
    {
        nw->curr_op = ZWNET_OP_NONE;
        zwnet_notify(nw, ZWNET_OP_RM_NODE, OP_FAILED, NULL);
    }

}


/**
zwnet_add_ni_complt_cb - Get detailed node info completion callback
@param[in]	nw        network
@param[in]	node_id   node id
@return
*/
static void zwnet_add_ni_complt_cb(zwnet_p nw, uint8_t node_id)
{
    zwnode_p      node;
    zwnoded_t     node_desc = {0};

    plt_mtx_lck(nw->mtx);
    node = zwnode_find(&nw->ctl, node_id);

    if (!node)
    {
        plt_mtx_ulck(nw->mtx);
        return;
    }

    if (zwnode_get_desc(node, &node_desc) == ZW_ERR_NONE)
    {
        int add_dev_poll = 0;
        int add_wkup_poll = 0;
        int32_t wkup_intv = 0;

        if (node->ep.intf)
        {   //Node has already populated with interfaces

            //Find wake up interface
            if (zwif_find_cls(node->ep.intf, COMMAND_CLASS_WAKE_UP) && (!node->listen))
            {   //sleeping node and support wakeup command class
                add_wkup_poll = 1;
                wkup_intv = node->wkup_intv;
            }
            else
            {   //Non-sleeping node
                add_dev_poll = 1;
            }
        }

        plt_mtx_ulck(nw->mtx);

        //Add it to device polling
        if (add_dev_poll)
        {   //Non-sleeping node
            zwdpoll_add_node(nw->dpoll_ctx, &node_desc);
        }

        //Add it to wakeup polling
        if (add_wkup_poll)
        {   //Sleeping node
            if (nw->multi_client)
            {
                wkuppoll_add(nw->wupoll_ctx, node_id, wkup_intv);
            }

            nodests_add(nw->nsts_ctx, node_id, NODSTS_STA_UP, wkup_intv);
        }
    }
    else
    {
        plt_mtx_ulck(nw->mtx);
    }
}


/**
zwnet_add_ni_get - Get detailed node info to populate command classes and interfaces
@param[in]	nw              network
@param[in]  node_id	        the newly added node id
@param[in]	op	            current add node operation
@param[in]	sts_get_ni	    status used to notify upper layer about get detailed node info
@param[in]	skip_nif	    flag to determine whether to skip node info cached get
@return     ZW_ERR_XXX
@pre        Caller must lock the nw->mtx before calling this function.
*/
static void zwnet_add_ni_get(zwnet_p nw, uint8_t node_id, uint8_t op, int sts_get_ni,
                             uint8_t skip_nif)
{
    sm_job_t  sm_job = {0};

    sm_job.op = op;
    sm_job.num_node = 1;
    sm_job.node_id = &node_id;
    sm_job.delay = ZWNET_NODE_INFO_GET_DELAY;
    sm_job.cb = zwnet_add_ni_complt_cb;
    sm_job.auto_cfg = 1;
    sm_job.skip_nif = skip_nif;
    sm_job.assign_rr = 1;

    if (nw->ctl_cap & ZWNET_CTLR_CAP_S2)
    {
        sm_job.get_gw_dsk = 1;
    }

    //Get the detailed node information
    if (!zwnet_node_info_sm(nw, EVT_DELAYED_GET_NODE_INFO, (uint8_t *)&sm_job))
    {
        debug_zwapi_msg(&nw->plt_ctx, "zwnet_add_ni_get failed");
        zwnet_abort(nw);
        zwnet_notify(nw, op, OP_FAILED, NULL);
        return;
    }

    //Notify the progress of the operation
    zwnet_notify(nw, op, sts_get_ni, NULL);
}


/**
zwnet_n2ip_cb - Resolve node to ip address state-machine completion callback
@param[in]	nw              network
@param[in]  status	        0=ok; non-zero=failed
@param[in]	prm	            user parameters
@return
@pre        Caller must lock the nw->mtx before calling this function.
*/
static void zwnet_n2ip_cb(zwnet_p nw, int status, void **prm)
{
    if (status == 0)
    {
        //debug_zwapi_msg(&nw->plt_ctx, "zwnet_n2ip_cb ok");
		zwnet_add_ni_get(nw, (uintptr_t)prm[0], (uintptr_t)prm[1], (uintptr_t)prm[2], 1);
    }
    else
    {
        debug_zwapi_msg(&nw->plt_ctx, "zwnet_n2ip_cb failed");
        zwnet_abort(nw);
		zwnet_notify(nw, (uintptr_t)prm[1], OP_FAILED, NULL);
    }
}


/**
zwnet_failed_id_rp_cb - Replace failed node id callback function
@param[in]	appl_ctx    The application layer context
@param[in]	node_rp_s2  Replace node callback info with security 2 support
@return
*/
static void zwnet_failed_id_rp_cb(appl_layer_ctx_t *appl_ctx, appl_node_rp_sec2_t *node_rp_s2)
{
    const char *status_msg[] = { "The failed node has been replaced", "Node id not in failing list", "Failed",
        "The failed node is ready to be replaced", "The node is working properly", "Security failed", "unknown"

    };
    uint8_t     sts;
    uint8_t     node_id;
    int         status;
    zwnet_p     nw = (zwnet_p)appl_ctx->data;

    sts = node_rp_s2->sts;

    node_id = node_rp_s2->node_id;

    if (sts == ZW_FAILED_NODE_REPLACE_DONE)
    {
        status = 0;
    }
    else if (sts == ZW_FAILED_NODE_REPLACE_FAILED)
    {
        status = 1;
    }
    else if (sts == ZW_FAILED_NODE_REMOVE_FAIL)
    {
        status = 2;
    }
    else if (sts == ZW_FAILED_NODE_REPLACE)
    {
        status = 3;
    }
    else if (sts == ZW_NODE_OK)
    {
        status = 4;
    }
    else if (sts == 9 /*security failed*/)
    {
        status = 5;
    }
    else
    {
        status = 6;
    }

    debug_zwapi_msg(&nw->plt_ctx, "rplc_failed_node_cb: status:%u(%s), node id:%u", sts, status_msg[status], node_id);

    if (node_rp_s2->sec2_valid)
    {
        debug_zwapi_msg(&nw->plt_ctx, "security 2: granted key mask:%02Xh, key exchange failed type:%u",
                                        (unsigned)node_rp_s2->grnt_keys, (unsigned)node_rp_s2->key_xchg_fail_type);
    }

    if ((sts == ZW_FAILED_NODE_REPLACE_DONE) || (sts == 9 /*security failed*/))
    {
        zwnode_p    node;

        //Notify the progress of the operation
        zwnet_notify(nw, ZWNET_OP_RP_NODE, OP_RP_NODE_PROTOCOL_DONE, NULL);

        //callback upper layer
        plt_mtx_lck(nw->mtx);
        node = zwnode_find(&nw->ctl, node_id);

        if (node)
        {
            //Clear node security property
            node->propty &= ~(NODE_PROPTY_SECURE_CAP_S0 | NODE_PROPTY_SECURE_CAP_S2 | NODE_PROPTY_ADD_SECURE | NODE_PROPTY_ADD_INSECURE);

            //Save granted keys
            if (node_rp_s2->sec2_valid)
            {
                node->s2_grnt_keys = node_rp_s2->grnt_keys;
                node->s2_keys_valid = 1;
            }
            else
            {
                node->s2_keys_valid = 0;
            }

            zwnet_node_cb(nw, node->nodeid, ZWNET_NODE_UPDATED);
        }

        //Get the detailed node information
        zwnet_add_ni_get(nw, node_id, ZWNET_OP_RP_NODE, OP_RP_NODE_GET_NODE_INFO, 0);
        plt_mtx_ulck(nw->mtx);

    }
    else if (sts == ZW_FAILED_NODE_REPLACE)
    {
    }
    else
    {
        zwnet_abort(nw);
        //Callback to notify status
        zwnet_notify(nw, ZWNET_OP_RP_NODE, OP_FAILED, NULL);
    }
}


/**
zwnet_add_node_cb - Add new node callback function
@param[in]	appl_ctx    The application layer context
@param[in]	add_ni_s2   Add node callback info with security 2 support
@return
*/
static void zwnet_add_node_cb(appl_layer_ctx_t *appl_ctx, appl_node_info_sec2_t *add_ni_s2)
{
/*
    #define ADD_NODE_STATUS_DONE                 6
    #define ADD_NODE_STATUS_FAILED               7
    #define NODE_ADD_STATUS_SECURITY_FAILED      9
*/
    const char *status_msg[] = { "OK", "Failed", "Added but insecure",
                                 "Learn Ready", "unknown"};

    zwnet_p             nw = (zwnet_p)appl_ctx->data;
    zwnode_p            node;
    appl_node_info_t    *add_ni = &add_ni_s2->node_info;
    int                 status;
    char                *device_dsk = NULL;
    sec2_node_info_t    *s2_nif_p = NULL;
    sec2_node_info_t    s2_nif = {0};

    switch (add_ni->status)
    {
        case ADD_NODE_STATUS_DONE:
            status = 0;
            break;

        case ADD_NODE_STATUS_FAILED:
            status = 1;
            break;

        case NODE_ADD_STATUS_SECURITY_FAILED:
            status = 2;
            break;

        case ADD_NODE_STATUS_LEARN_READY:
            status = 3;
            break;

        default:
            status = 4;
    }

    int security_incl_status = 0;
    int has_s0_cls = 0;
    has_s2_cls = 0;

    ALOGI("add_node_nw_cb: status:%u (%s)", add_ni->status, status_msg[status]);

    if (add_ni_s2->sec2_valid)
    {
        debug_zwapi_msg(&nw->plt_ctx, "security 2: granted key mask:%02Xh, key exchange failed type:%u",
                                        (unsigned)add_ni_s2->grnt_keys, (unsigned)add_ni_s2->key_xchg_fail_type);
    }

    if (add_ni_s2->dsk_len)
    {
        device_dsk = zwutil_dsk_to_str(add_ni_s2->dsk, add_ni_s2->dsk_len);

        if (device_dsk)
        {
            debug_zwapi_msg(&nw->plt_ctx, "DSK:%s", device_dsk);
        }
        else
        {
            debug_zwapi_msg(&nw->plt_ctx, "Invalid DSK:");
            debug_zwapi_bin_msg(&nw->plt_ctx, add_ni_s2->dsk, add_ni_s2->dsk_len);
        }
    }

    if ((add_ni->status == ADD_NODE_STATUS_DONE)
        || (add_ni->status == NODE_ADD_STATUS_SECURITY_FAILED))
    {
        n2ip_sm_ctx_t   n2ip_sm_ctx = {0};

        debug_zwapi_msg(&nw->plt_ctx, "new node id:%u", add_ni->node_id);
        debug_zwapi_msg(&nw->plt_ctx, "listening=%u, optional functions=%u, sensor=%u", add_ni->listen, add_ni->optional, add_ni->sensor);
        debug_zwapi_msg(&nw->plt_ctx, "Device type: basic=%02Xh, generic=%02Xh, specific=%02X", add_ni->basic, add_ni->gen, add_ni->spec);
        if (add_ni->cmd_cnt > 0)
        {
            debug_zwapi_msg(&nw->plt_ctx, "unsecure command classes:");
            zwnet_cmd_cls_show(nw, add_ni->cmd_cls, add_ni->cmd_cnt);
        }
        if (add_ni->cmd_cnt_sec > 0)
        {
            debug_zwapi_msg(&nw->plt_ctx, "secure command classes:");
            has_s0_cls = 1;
            zwnet_cmd_cls_show(nw, add_ni->cmd_cls_sec, add_ni->cmd_cnt_sec);
        }

        //Create/Update node info
#ifdef MAP_IP_ASSOC_TO_ASSOC
        zwnet_ip_assoc_map(add_ni);
#endif

        if (add_ni_s2->sec2_valid)
        {
            s2_nif.grnt_keys_valid = 1;
            s2_nif.grnt_keys = add_ni_s2->grnt_keys;
            ALOGI("granted key callback, key:%x",add_ni_s2->grnt_keys);

            s2_nif_p = &s2_nif;
        }

        if (device_dsk)
        {
            s2_nif.dsk = device_dsk;

            s2_nif_p = &s2_nif;
        }

        if(has_s2_cls && has_s0_cls && (add_ni_s2->grnt_keys != 0x80 && add_ni_s2->grnt_keys != 0))
        {
            security_incl_status = 2; //s2 inclusion
        }
        else if((!has_s2_cls && has_s0_cls) || (add_ni_s2->grnt_keys == 0x80))
        {
            security_incl_status = 1; // s0 inclusion
        }

        if (add_ni->status == NODE_ADD_STATUS_SECURITY_FAILED)
        {
            ALOGI("Node added but insecure!");
            security_incl_status = 0;
        }

        zwnet_node_info_update(nw, add_ni, s2_nif_p);

        plt_mtx_lck(nw->mtx);

        node = zwnode_find(&nw->ctl, add_ni->node_id);

        if (node)
        {
            node->security_incl_status = security_incl_status;
            zwnet_node_cb(nw, node->nodeid, ZWNET_NODE_ADDED);
        }

        //Notify the progress of the operation
        zwnet_notify(nw, nw->curr_op /*ZWNET_OP_ADD_NODE*/, OP_ADD_NODE_PROTOCOL_DONE, NULL);

        //Call the resolve node to ip address state-machine and assign the callback function
        //to get detailed node information
        n2ip_sm_ctx.cb = zwnet_n2ip_cb;
        n2ip_sm_ctx.node_id[0] = add_ni->node_id;
        n2ip_sm_ctx.num_node = 1;
		n2ip_sm_ctx.params[0] = (void *)(uintptr_t)add_ni->node_id;
        n2ip_sm_ctx.params[1] = (void *)(uintptr_t)nw->curr_op /*ZWNET_OP_ADD_NODE*/;
        n2ip_sm_ctx.params[2] = (void *)(unsigned)OP_ADD_NODE_GET_NODE_INFO;

        if (zwnet_n2ip_sm(nw, EVT_N2IP_START, (uint8_t *)&n2ip_sm_ctx) != 0)
        {
            zwnet_abort(nw);
            zwnet_notify(nw, (unsigned)nw->curr_op /*ZWNET_OP_ADD_NODE*/, OP_FAILED, NULL);
        }
        plt_mtx_ulck(nw->mtx);
    }
    else if(add_ni->status == ADD_NODE_STATUS_LEARN_READY)
    {
        zwnet_notify(nw, (unsigned)nw->curr_op /*ZWNET_OP_ADD_NODE*/, OP_ADD_NODE_LEARN_READY, NULL);
    }
    else
    {
        uint8_t prev_op;
        //Add node failed or unknown error.  Cancel operation without sending ADD_NODE_STOP
        plt_mtx_lck(nw->mtx);
        zwnet_sm_ni_stop(nw);

        //Reset operation to "no operation"
        prev_op = nw->curr_op;
        nw->curr_op = ZWNET_OP_NONE;
        plt_mtx_ulck(nw->mtx);

        zwnet_notify(nw, prev_op /*ZWNET_OP_ADD_NODE*/, OP_FAILED, NULL);
    }

    free(device_dsk);
}


/**
zwnet_node_add_key_req_cb - Add node to network security 2 key request callback function
@param[in]	appl_ctx        Application layer context
@param[in]	req_keys_rpt    Request keys / CSA report
@param[in]  flag            Received packet flag, see ZIP_FLAG_XXX
@return
*/
static void zwnet_node_add_key_req_cb(appl_layer_ctx_t *appl_ctx, appl_node_keys_rpt_t req_keys_rpt, uint16_t flag)
{
    zwnet_p             nw = (zwnet_p)appl_ctx->data;
    void                *usr_param;
    add_node_sec2_fn    cb;
    zwnet_cb_req_t      cb_req;

    plt_mtx_lck(nw->mtx);
    //Save CSA request status
    nw->csa_requested = req_keys_rpt.req_csa;

    //Save received packet flag
    nw->rcx_flag = flag;

    //Callback user to enter pin or to verify
    if (nw->incl_on_behalf == IOB_UNSOLICITED)
    {
        cb = nw->init.s2_unsolicited_cb;
        usr_param = nw->init.user;
    }
    else
    {
        cb = nw->sec2_add_prm.cb;
        usr_param = nw->sec2_add_prm.usr_param;
    }
    plt_mtx_ulck(nw->mtx);


    if (cb)
    {
        cb_req.type = CB_TYPE_S2_REQ_KEY;
        cb_req.cb = cb;
        cb_req.param.req_key.user = usr_param;
        cb_req.param.req_key.req_key = req_keys_rpt;

        util_list_add(nw->cb_mtx, &nw->cb_req_hd,
                      (uint8_t *)&cb_req, sizeof(zwnet_cb_req_t));
        plt_sem_post(nw->cb_sem);
    }
}


/**
zwnet_node_dsk_cb - Add node to network security 2 DSK report callback function
@param[in]	appl_ctx    Application layer context
@param[in]	dsk_rpt     DSK report
@param[in]  flag        Received packet flag, see ZIP_FLAG_XXX
@return
*/
static void zwnet_node_dsk_cb(appl_layer_ctx_t *appl_ctx, appl_dsk_rpt_t *dsk_rpt, uint16_t flag)
{
    zwnet_p     nw = (zwnet_p)appl_ctx->data;
    char        *rx_dsk;
    char        zero_str[] = "00000";
    int         res;
    int         accept = 0;
    void        *usr_param;
    add_node_sec2_fn    cb;
    zwnet_cb_req_t  cb_req;

    rx_dsk = zwutil_dsk_to_str(dsk_rpt->dsk_keys, MAX_DSK_KEY_LEN);

    if (!rx_dsk)
    {
        return;
    }

    debug_zwapi_msg(&nw->plt_ctx, "Joining device DSK: %s", rx_dsk);

    plt_mtx_lck(nw->mtx);

    //Save received packet flag
    nw->rcx_flag = flag;

    if ((dsk_rpt->obscure_key_len == 0)
        && (dsk_rpt->dsk_keys[0] != 0) && (dsk_rpt->dsk_keys[1] != 0))
    {   //No user input is necessary, auto-accept the DSK

        res = zwnet_add_sec2_accept(nw, 1, rx_dsk);

        plt_mtx_ulck(nw->mtx);

        if (res < 0)
        {
            debug_zwapi_msg(&nw->plt_ctx, "zwnet_node_dsk_cb: zwnet_add_sec2_accept auto-accept with error:%d", res);
        }

        free(rx_dsk);
        return;

    }

    if (nw->incl_on_behalf == IOB_UNSOLICITED)
    {
        //Callback user to enter pin or to verify
        cb = nw->init.s2_unsolicited_cb;
        usr_param = nw->init.user;
    }
    else
    {
        if (nw->sec2_add_prm.dsk)
        {   //User has already provided DSK, check for matches
            if (strcmp(rx_dsk, nw->sec2_add_prm.dsk) == 0)
            {   //Matched
                accept = 1;
            }
            else if ((dsk_rpt->dsk_keys[0] == 0) && (dsk_rpt->dsk_keys[1] == 0))
            {   //Some part (first 5 digits) of the dsk is obscured
                if (strcmp(rx_dsk + 5, nw->sec2_add_prm.dsk + 5) == 0)
                {
                    accept = 1;
                }
            }

            res = zwnet_add_sec2_accept(nw, accept, nw->sec2_add_prm.dsk);

            plt_mtx_ulck(nw->mtx);

            if (res < 0)
            {
                debug_zwapi_msg(&nw->plt_ctx, "zwnet_node_dsk_cb: zwnet_add_sec2_accept with error:%d", res);
            }

            free(rx_dsk);
            return;
        }

        //Callback user to enter pin or to verify
        cb = nw->sec2_add_prm.cb;
        usr_param = nw->sec2_add_prm.usr_param;
    }

    plt_mtx_ulck(nw->mtx);


    if (cb)
    {
        if (strncmp(zero_str, rx_dsk, 5) == 0)
        {
            cb_req.param.dsk_cb.pin_required = 1;
#ifdef USE_SAFE_VERSION
            strncpy_s(cb_req.param.dsk_cb.dsk, 48, rx_dsk + 5, 47);
#else
            strncpy(cb_req.param.dsk_cb.dsk, rx_dsk + 5, 47);
#endif
            cb_req.param.dsk_cb.dsk[47] = '\0';
        }
        else
        {
            cb_req.param.dsk_cb.pin_required = 0;
#ifdef USE_SAFE_VERSION
            strncpy_s(cb_req.param.dsk_cb.dsk, 48, rx_dsk, 47);
#else
            strncpy(cb_req.param.dsk_cb.dsk, rx_dsk, 47);
#endif
            cb_req.param.dsk_cb.dsk[47] = '\0';
        }

        cb_req.type = CB_TYPE_S2_DSK;
        cb_req.cb = cb;
        cb_req.param.dsk_cb.user = usr_param;

        util_list_add(nw->cb_mtx, &nw->cb_req_hd,
                      (uint8_t *)&cb_req, sizeof(zwnet_cb_req_t));
        plt_sem_post(nw->cb_sem);
    }

    free(rx_dsk);
}


/**
zwnet_node_add_rplc_hdlr - Add/replace node callback function with support of security 2
@param[in]	appl_ctx    The application layer context
@param[in]	cb_prm		Add node callback parameters
@return
*/
static void zwnet_node_add_rplc_hdlr(appl_layer_ctx_t *appl_ctx, appl_node_add_cb_prm_t *cb_prm)
{
    const uint8_t add_node_status_lst[] = {ADD_NODE_STATUS_DONE, ADD_NODE_STATUS_FAILED, NODE_ADD_STATUS_SECURITY_FAILED};
    const uint8_t node_rp_status_lst[] = {ZW_FAILED_NODE_REPLACE_DONE, ZW_FAILED_NODE_REPLACE_FAILED, NODE_ADD_STATUS_SECURITY_FAILED};
    zwnet_t *nw = (zwnet_t *)appl_ctx->data;

    plt_mtx_lck(nw->mtx);
    if ((nw->curr_op != ZWNET_OP_ADD_NODE) && (nw->curr_op != ZWNET_OP_RP_NODE))
    {
        plt_mtx_ulck(nw->mtx);
        return;
    }

    //Convert add node status to replace failed node status
    //This is the workaround for older Z/IP gateway that sends "add node status" instead of "replace failed node status" for replace-on-behalf operation.
    if ((nw->ctl_cap & ZWNET_CTLR_CAP_INC_ON_BEHALF) && (cb_prm->rpt_type == NODE_ADD_STATUS)
        && nw->incl_on_behalf)
    {
        if ((nw->curr_op == ZWNET_OP_RP_NODE) || zwnode_find(&nw->ctl, cb_prm->rpt_prm.node_info.node_info.node_id))
        {
            //Existing operation is replace node or node already exists
            int                   i;
            appl_node_info_sec2_t *node_info_s2 = &cb_prm->rpt_prm.node_info;
            appl_node_info_t      *node_info = &node_info_s2->node_info;;
            appl_node_rp_sec2_t   node_rp_s2 = {0};

            if (nw->incl_on_behalf == IOB_UNSOLICITED)
            {   //Change the current operation to replace failed node as the default is set to add node
                nw->curr_op = ZWNET_OP_RP_NODE;
                //nw->failed_id = node_info->node_id;
            }

            plt_mtx_ulck(nw->mtx);

            node_rp_s2.sec2_valid = node_info_s2->sec2_valid;
            if (node_rp_s2.sec2_valid)
            {
                node_rp_s2.grnt_keys = node_info_s2->grnt_keys;
                node_rp_s2.key_xchg_fail_type = node_info_s2->key_xchg_fail_type;
            }

            //Translate status
            node_rp_s2.sts = 0xFF;
            for (i=0; i<sizeof(add_node_status_lst); i++)
            {
                if (node_info->status == add_node_status_lst[i])
                {
                    node_rp_s2.sts = node_rp_status_lst[i];
                    break;
                }
            }
            node_rp_s2.node_id = node_info->node_id;

            //Call replace failed node status callback function
            zwnet_failed_id_rp_cb(appl_ctx, &node_rp_s2);
            return;

        }
    }

    plt_mtx_ulck(nw->mtx);

    switch (cb_prm->rpt_type)
    {
        case NODE_ADD_STATUS:
            zwnet_add_node_cb(appl_ctx, &cb_prm->rpt_prm.node_info);
            break;

        case FAILED_NODE_REPLACE_STATUS:
            zwnet_failed_id_rp_cb(appl_ctx, &cb_prm->rpt_prm.node_rp_info);
            break;

        case NODE_ADD_KEYS_REPORT_V2:
            zwnet_node_add_key_req_cb(appl_ctx, cb_prm->rpt_prm.req_keys_rpt, cb_prm->flag);
            break;

        case NODE_ADD_DSK_REPORT_V2:
            zwnet_node_dsk_cb(appl_ctx, &cb_prm->rpt_prm.dsk_rpt, cb_prm->flag);
            break;
    }
}


/**
zwnet_node_add_sec2_cb - Add node to network callback function with support of security 2
@param[in]	appl_ctx    The application layer context
@param[in]	cb_prm      Add node callback parameters
@return
*/
static void zwnet_node_add_sec2_cb(appl_layer_ctx_t *appl_ctx, appl_node_add_cb_prm_t *cb_prm)
{
    zwnet_t *nw = (zwnet_t *)appl_ctx->data;

    if (nw->ctl_cap & ZWNET_CTLR_CAP_INC_ON_BEHALF)
    {   //Only controller with inclusion on-behalf capability will receive unsolicited add node status
        int unsolicited_status = 0;

        plt_mtx_lck(nw->mtx);
        if ((nw->curr_op != ZWNET_OP_ADD_NODE) && (nw->curr_op != ZWNET_OP_RP_NODE))
        {
            unsolicited_status = 1;
        }
        plt_mtx_ulck(nw->mtx);

        if (unsolicited_status)
        {
            zwnet_unsolic_node_add_cb(appl_ctx, cb_prm);
            return;
        }
    }

    zwnet_node_add_rplc_hdlr(appl_ctx, cb_prm);

}


/**
zwnet_add_sec2_accept - accept/reject newly added node into security 2 mode
@param[in]	net		    network
@param[in]	accept		1=accept; 0=reject
@param[in]	dsk	        Complete Device Specific Key (DSK) of the added node if accept=1; else this can be NULL.
@return		ZW_ERR_XXX
@pre        The function should be called only after receiving security 2 DSK callback.
@see		zwnet_add
*/
int zwnet_add_sec2_accept(zwnet_p net, int accept, char *dsk)
{
    uint8_t     dsk_bin[MAX_DSK_KEY_LEN];
    uint8_t     dsk_len;
    int         res;

    if (accept)
    {
        if (!dsk || !zwutil_str_to_dsk(dsk, dsk_bin, &dsk_len))
        {
            return ZW_ERR_VALUE;
        }
    }

    res = zw_sec2_dsk_set(&net->appl_ctx, accept, 2, dsk_bin, net->rcx_flag & ZIP_FLAG_HOST_PORT);

    if (res < 0)
    {
        debug_zwapi_msg(&net->plt_ctx, "zw_sec2_dsk_set error:%d", res);
    }

    return res;
}


/**
zwnet_add_sec2_grant_key - grant keys and CSA to the newly added node in security 2 mode
@param[in]	net		        network
@param[in]	granted_keys    security 2 granted keys (bit-mask), see SEC_KEY_BITMSK_XXX
@param[in]	grant_csa       grant client-side authentication (CSA); 1=grant, 0=reject. If joining node
                            didn't request CSA, it is ignored.
@return		ZW_ERR_XXX
@pre        The function should be called only after receiving security 2 requested keys callback.
@see		zwnet_add
*/
int zwnet_add_sec2_grant_key(zwnet_p net, uint8_t granted_keys, uint8_t grant_csa)
{
    int res;

    res = zw_sec2_key_set(&net->appl_ctx, granted_keys, (net->csa_requested)? grant_csa : 0, net->rcx_flag & ZIP_FLAG_HOST_PORT);

    if (res < 0)
    {
        debug_zwapi_msg(&net->plt_ctx, "zw_sec2_key_set error:%d", res);
    }

    return res;
}


/**
zwnet_sm_gw_dsk_cb - Z/IP gateway DSK report callback
@param[in]	appl_ctx    The application layer context
@param[in]	dsk         Z/IP gateway DSK
@param[in]	dsk_len     Z/IP gateway DSK length
@param[in]	type            Z/IP gateway DSK type, see GW_DSK_TYPE_XXX
@return		ZW_ERR_XXX
*/
void zwnet_sm_gw_dsk_cb(appl_layer_ctx_t *appl_ctx, uint8_t *dsk, uint8_t dsk_len, int type)
{
    zwnet_p nw = (zwnet_p)appl_ctx->data;
    char    *rx_dsk;

    if (dsk_len != MAX_DSK_KEY_LEN)
    {
        debug_zwapi_msg(&nw->plt_ctx, "zwnet_sm_gw_dsk_cb: invalid dsk key length of %u", dsk_len);
        return;
    }

    rx_dsk = zwutil_dsk_to_str(dsk, dsk_len);

    if (!rx_dsk)
    {
        return;
    }

    //Save a copy of gateway dsk
    plt_mtx_lck(nw->mtx);
#ifdef USE_SAFE_VERSION
	strncpy_s(nw->gw_dsk, DSK_STR_LEN, rx_dsk, DSK_STR_LEN - 1);
#else
	strncpy(nw->gw_dsk, rx_dsk, DSK_STR_LEN - 1);
#endif
    nw->gw_dsk[DSK_STR_LEN - 1] = '\0';
    free(rx_dsk);

    //Call state-machines
    zwnet_node_info_sm(nw, EVT_GW_DSK, NULL);

    zwnet_ctlr_info_sm(nw, EVT_CTLR_DSK_REPORT, NULL);

    zwnet_add_rplc_node_sm(nw, EVT_ADD_DSK_RPT, NULL);

    if (type != GW_DSK_TYPE_ADD)
    {
        zwnet_join_other_nw_sm(nw, EVT_JOIN_DSK_RPT, (uint8_t *)nw->gw_dsk);
    }
    plt_mtx_ulck(nw->mtx);

    debug_zwapi_msg(&nw->plt_ctx, "Z/IP gateway dsk: %s, type: %d", nw->gw_dsk, type);

}


/**
zwnet_sm_add_tout_cb - Add node state-machine timer callback
@param[in] data     Pointer to network
@return
*/
static void zwnet_sm_add_tout_cb(void *data)
{
    zwnet_p   nw = (zwnet_p)data;

    //Stop timer
    plt_mtx_lck(nw->mtx);
    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
    nw->sm_tmr_ctx = NULL;

    //Call state-machine
    zwnet_add_rplc_node_sm(nw, EVT_ADD_TMOUT, NULL);
    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_add_rplc_node_sm - Add/replace node state-machine
@param[in] nw		Network
@param[in] evt      The event for the state-machine
@param[in] data     The data associated with the event
@return   Non-zero when the state-machine is started from idle state; otherwise return zero
@pre    Caller must lock the nw->mtx before calling this function
*/
static int zwnet_add_rplc_node_sm(zwnet_p nw, zwnet_add_evt_t evt, uint8_t *data)
{
    int res;

    switch (nw->add_sm_sta)
    {
        //----------------------------------------------------------------
        case ADD_STA_IDLE:
        //----------------------------------------------------------------
            if (evt == EVT_ADD_START)
            {
                memcpy(&nw->add_sm_ctx, data, sizeof(add_sm_ctx_t));

                //Check for support of S2
                if (nw->add_sm_ctx.s2_security)
                {
                    res = zw_sec2_dsk_get(&nw->appl_ctx, zwnet_sm_gw_dsk_cb, GW_DSK_TYPE_ADD);

                    if (res < 0)
                    {
                        debug_zwapi_msg(&nw->plt_ctx, "zw_sec2_dsk_get error:%d", res);
                        return 0;
                    }

                    //Change state
                    nw->add_sm_sta = ADD_STA_CTLR_DSK;

                    //Restart timer
                    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                    nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_add_tout_cb, nw);

                    return 1;//state-machine started successfully
                }
                else if (!nw->add_sm_ctx.incl_on_behalf)
                {   //Non S2
                    if (nw->add_sm_ctx.mode == SM_MODE_ADD)
                    {
                        res = zw_add_node_to_network(&nw->appl_ctx, zwnet_node_add_sec2_cb, ADD_NODE_ANY, TRANSMIT_OPTION_EXPLORE);
                    }
                    else
                    {
                        res = zw_replace_failed_node(&nw->appl_ctx, zwnet_node_add_sec2_cb, nw->add_sm_ctx.node_id, 0, START_FAILED_NODE_REPLACE);
                    }

                    if (res == 0)
                    {
                        return 1;//state-machine started successfully
                    }
                }
            }
            break;

        //----------------------------------------------------------------
        case ADD_STA_CTLR_DSK:
        //----------------------------------------------------------------
            switch (evt)
            {
                case EVT_ADD_DSK_RPT:
                    {
                        //Stop timer
                        plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                        nw->sm_tmr_ctx = NULL;

                        if (!nw->add_sm_ctx.incl_on_behalf)
                        {
                            if (nw->add_sm_ctx.mode == SM_MODE_ADD)
                            {
                                res = zw_add_node_to_network(&nw->appl_ctx, zwnet_node_add_sec2_cb, ADD_NODE_ANY_S2, TRANSMIT_OPTION_EXPLORE);
                            }
                            else
                            {
                                res = zw_replace_failed_node(&nw->appl_ctx, zwnet_node_add_sec2_cb, nw->add_sm_ctx.node_id, 0, START_FAILED_NODE_REPLACE_S2);
                            }

                            if (res < 0)
                            {
                                //Add node failed

                                zwnet_notify(nw, nw->curr_op, OP_FAILED, NULL);

                                //Reset operation to "no operation"
                                nw->curr_op = ZWNET_OP_NONE;
                            }
                        }

                        //Job done
                        nw->add_sm_sta = ADD_STA_IDLE;

                        break;
                    }

                case EVT_ADD_TMOUT:
                    zwnet_notify(nw, nw->curr_op, OP_FAILED, NULL);

                    //Reset operation to "no operation"
                    nw->curr_op = ZWNET_OP_NONE;

                    //Job done
                    nw->add_sm_sta = ADD_STA_IDLE;
                    break;

                default:
                    break;

            }
    }

    return 0;

}


/**
zwnet_add - called by inclusion controller to add/remove initiating node to network
@param[in] net              Network
@param[in] add              0=remove; else add node
@param[in] sec2_param       Parameters for adding node with security 2 protocol.
                            This parameter is ignored when non-security 2 Z/IP gateway is detected or when removing node.
@param[in] incl_on_behalf   Flag to indicate enter into inclusion on-behalf (iob) mode; 1=enter into iob mode, 0=normal add node.
                            This parameter is ignored when parameter add = 0
@return		ZW_ERR_XXX, otherwise node callback will be called.
@see		zwnet_initiate
*/
int zwnet_add(zwnet_p net, uint8_t add, sec2_add_prm_t *sec2_param, int incl_on_behalf)
{
    int32_t result;

    //Check if inclusion on-behalf is supported
    if (add && incl_on_behalf)
    {
        if (!(net->ctl_cap & ZWNET_CTLR_CAP_INC_ON_BEHALF) || (net->zwave_role != ZW_ROLE_SIS))
        {
            return ZW_ERR_UNSUPPORTED;
        }
    }

    //Check if controller is capable to add / remove node
    if (!(net->ctl_role & ZWNET_CTLR_ROLE_INCL))
    {
        return ZW_ERR_UNSUPPORTED;
    }

    plt_mtx_lck(net->mtx);
    if (net->curr_op != ZWNET_OP_NONE)
    {
        //Cancel low priority operation
        if (net->curr_op == ZWNET_OP_NODE_CACHE_UPT)
        {
            zwnet_abort(net);
            debug_zwapi_msg(&net->plt_ctx, "Canceled low priority operation");
        }
        else
        {
            ALOGI("Current operation not completed yet, try again later");
            plt_mtx_ulck(net->mtx);
            return ZW_ERR_LAST_OP_NOT_DONE;
        }
    }


    if (add == 0)
    {   //Remove node

        result = zw_remove_node_from_network(&net->appl_ctx, zwnet_node_rm_cb, REMOVE_NODE_ANY);

        if (result == 0)
        {
            net->curr_op = ZWNET_OP_RM_NODE;
            plt_mtx_ulck(net->mtx);
            return ZW_ERR_NONE;
        }
        ALOGI("zwnet_add: remove node with error:%d", result);
        plt_mtx_ulck(net->mtx);
        return ZW_ERR_OP_FAILED;
    }
    else
    {   //Add node
        add_sm_ctx_t    sm_ctx = {0};

        sm_ctx.mode = SM_MODE_ADD;

        if (sec2_param && (net->ctl_cap & ZWNET_CTLR_CAP_S2))
        {   //Support Security 2

            //Check input parameters
            if ((!sec2_param->cb)
                || (sec2_param->dsk && !zwutil_dsk_chk(sec2_param->dsk)))
            {
                plt_mtx_ulck(net->mtx);
                return ZW_ERR_VALUE;
            }

            //Save input parameters
            if (net->sec2_add_prm.dsk)
            {
                free(net->sec2_add_prm.dsk);
            }
            net->sec2_add_prm = *sec2_param;

            if (sec2_param->dsk)
            {
#ifdef USE_SAFE_VERSION
		net->sec2_add_prm.dsk = _strdup(sec2_param->dsk);
#else
		net->sec2_add_prm.dsk = strdup(sec2_param->dsk);
#endif
            }
            net->csa_requested = 0;
            sm_ctx.s2_security = 1;

        }

        net->incl_on_behalf = sm_ctx.incl_on_behalf = (incl_on_behalf)? IOB_USER_INITIATE : 0;

        if (zwnet_add_rplc_node_sm(net, EVT_ADD_START, (uint8_t *)&sm_ctx))
        {
            net->curr_op = ZWNET_OP_ADD_NODE;
            plt_mtx_ulck(net->mtx);
            return ZW_ERR_NONE;
        }

        ALOGI("zwnet_add failed");
        plt_mtx_ulck(net->mtx);
        return ZW_ERR_OP_FAILED;
    }
}


/**
zwnet_node_rm_all - Remove all nodes from the network.
@param[in]	nw	    The network
*/
static void zwnet_node_rm_all(zwnet_p nw)
{
    zwnode_p    node;

    plt_mtx_lck(nw->mtx);
    //
    //Remove the second node onwards
    //
    while ((node = (zwnode_p)nw->ctl.obj.next) != NULL)
    {
        zwnode_ep_rm_all(node);

        //Remove command queues
        zwnode_cmd_q_rm(node);
        zwnode_wait_cmd_q_rm(node);

        //Remove polling requests
        zwpoll_node_rm(nw, node->nodeid);

        //Stop timers
        plt_tmr_stop(&nw->plt_ctx, node->mul_cmd_tmr_ctx);
        node->mul_cmd_tmr_ctx = NULL;
        plt_tmr_stop(&nw->plt_ctx, node->wait_tmr_ctx);
        node->wait_tmr_ctx = NULL;

        //Remove entry in the node-ip address table
        zip_node_ipaddr_rm(&nw->appl_ctx, node->nodeid);

        //Free extended version information
        if (node->ext_ver)
        {
            free(node->ext_ver);
        }

        //Remove node
        zwobj_del(&nw->ctl.obj.next, &node->obj);
    }
    //
    //Remove the first node (controller node)
    //

    //Remove the first and only one ep
    zwnode_ep_rm_all(&nw->ctl);

    zwnode_ctl_clr(nw);

    //Free extended version information
    if (nw->ctl.ext_ver)
    {
        free(nw->ctl.ext_ver);
        nw->ctl.ext_ver = NULL;
    }

    plt_mtx_ulck(nw->mtx);
}


/**
zwnet_failed_id_rm_cb - Remove failed node id callback function
@param[in]	appl_ctx    The application layer context
@param[in]	sts		    Status
@param[in]	node_id     The node id of the removed node
@return
*/
static void zwnet_failed_id_rm_cb(appl_layer_ctx_t *appl_ctx, uint8_t sts, uint8_t node_id)
{
    int         status;
    zwnet_p     nw = (zwnet_p)appl_ctx->data;

    if (sts == ZW_FAILED_NODE_REMOVED)
    {
        status = 0;
    }
    else if (sts == ZW_FAILED_NODE_NOT_FOUND)
    {
        status = 1;
    }
    else if (sts == ZW_FAILED_NODE_REMOVE_FAIL || sts == ZW_FAILED_NODE_NOT_REMOVED)
    {
        status = 2;
    }
    else if (sts == ZW_NODE_OK)
    {
        status = 3;
    }
    else
    {
        status = 4;
    }

    ALOGI("rm_failed_node_cb: status:%u(%s), node id:%u", sts,
                    rm_failed_status_msg[status], node_id);

    if (sts == ZW_FAILED_NODE_REMOVED)
    {
        zwnode_p    node;

        if (nw->failed_id > 0)
        {   //Valid node id
            plt_mtx_lck(nw->mtx);
            node = zwnode_find(&nw->ctl, nw->failed_id);
            if (node)
            {
                zwnet_node_cb(nw, node->nodeid, ZWNET_NODE_REMOVED);
            }
            //Remove the failed node
            zip_node_ipaddr_rm(&nw->appl_ctx, nw->failed_id);
            zwnode_rm(nw, nw->failed_id);
            plt_mtx_ulck(nw->mtx);
        }
    }

    zwnet_abort(nw);
    //Callback to notify status
    if (sts == ZW_FAILED_NODE_REMOVED)
    {
        zwnet_sts_t info = {0};

        info.type = ZWNET_STS_INFO_NODE_ID;
        info.info.node_id = node_id;

        zwnet_notify(nw, ZWNET_OP_RM_FAILED_ID, OP_DONE, &info);

    }
    else
    {
        zwnet_notify(nw, ZWNET_OP_RM_FAILED_ID, OP_FAILED, NULL);
    }
}


/**
zwnet_fail - called by inclusion controller to remove failed node from
             the routing table or replace failed node with initiating node
@param[in]	net		network
@param[in]	nodeid	        failed node id. This parameter is ignored when rplc_on_behalf = 1.
@param[in]	replace	        replace failed node if non-zero; else remove failed node if zero
@param[in]	sec2_param	Optional parameters for replacing node with security 2 protocol.  This parameter is ignored
                                when removing failed node (i.e. replace = 0).
@param[in]	rplc_on_behalf	flag to indicate enter into "replace failed node" on-behalf (rob) mode; 1=enter into rob mode,
                                0=normal replace failed node. This parameter is ignored when parameter replace = 0.
@return		ZW_ERR_XXX, otherwise node callback will be called.
@see		zwnet_initiate
*/
int zwnet_fail(zwnet_p net, uint8_t nodeid, uint8_t replace, sec2_add_prm_t *sec2_param, int rplc_on_behalf)
{
    int32_t             res;
    appl_layer_ctx_t    *appl_ctx = (appl_layer_ctx_t   *)(&net->appl_ctx);

    //Check if inclusion on-behalf is supported
    if (replace && rplc_on_behalf)
    {
        if (!(net->ctl_cap & ZWNET_CTLR_CAP_INC_ON_BEHALF) || (net->zwave_role != ZW_ROLE_SIS))
        {
            return ZW_ERR_UNSUPPORTED;
        }
    }

    if (!(net->ctl_role & ZWNET_CTLR_ROLE_INCL))
    {
        return ZW_ERR_UNSUPPORTED;
    }

    plt_mtx_lck(net->mtx);
    if (net->curr_op != ZWNET_OP_NONE)
    {
        //Cancel low priority operation
        if (net->curr_op == ZWNET_OP_NODE_CACHE_UPT)
        {
            zwnet_abort(net);
            ALOGI("Canceled low priority operation");
        }
        else
        {
            ALOGI("Current operation not completed yet, try again later");
            plt_mtx_ulck(net->mtx);
            return ZW_ERR_LAST_OP_NOT_DONE;
        }
    }

    if (replace == 0)
    {   //Remove failed node from the protocol layer failed node ID list
        res = zw_remove_failed_node_id(appl_ctx, zwnet_failed_id_rm_cb, nodeid);
        if (res == 0)
        {
            net->curr_op = ZWNET_OP_RM_FAILED_ID;
            net->failed_id = nodeid;
            plt_mtx_ulck(net->mtx);
            return ZW_ERR_NONE;
        }
    }
    else
    {   //Replace failed node
        add_sm_ctx_t    sm_ctx = {0};

        sm_ctx.mode = SM_MODE_REPLACE;
        sm_ctx.node_id = nodeid;

        if (sec2_param && (net->ctl_cap & ZWNET_CTLR_CAP_S2))
        {   //Support Security 2

            //Check input parameters
            if ((!sec2_param->cb)
                || (sec2_param->dsk && !zwutil_dsk_chk(sec2_param->dsk)))
            {
                plt_mtx_ulck(net->mtx);
                return ZW_ERR_VALUE;
            }

            //Save input parameters
            if (net->sec2_add_prm.dsk)
            {
                free(net->sec2_add_prm.dsk);
            }

            net->sec2_add_prm = *sec2_param;
            if (sec2_param->dsk)
            {
#ifdef USE_SAFE_VERSION
                net->sec2_add_prm.dsk = _strdup(sec2_param->dsk);
#else
		net->sec2_add_prm.dsk = strdup(sec2_param->dsk);
#endif
            }

            sm_ctx.s2_security = 1;
        }

        net->incl_on_behalf = sm_ctx.incl_on_behalf = (rplc_on_behalf)? IOB_USER_INITIATE : 0;

        if (zwnet_add_rplc_node_sm(net, EVT_ADD_START, (uint8_t *)&sm_ctx))
        {
            net->curr_op = ZWNET_OP_RP_NODE;
            net->failed_id = (rplc_on_behalf)? 0 : nodeid;
            plt_mtx_ulck(net->mtx);
            return ZW_ERR_NONE;
        }
    }
    plt_mtx_ulck(net->mtx);
    return ZW_ERR_OP_FAILED;

}


/**
zwnet_sec2_get_dsk_cb - Z/IP gateway DSK report callback
@param[in]	appl_ctx    The application layer context
@param[in]	dsk         Z/IP gateway DSK
@param[in]	dsk_len     Z/IP gateway DSK length
@param[in]	type        Z/IP gateway DSK type, see GW_DSK_TYPE_XXX
@return		ZW_ERR_XXX
*/
static void zwnet_sec2_get_dsk_cb(appl_layer_ctx_t *appl_ctx, uint8_t *dsk, uint8_t dsk_len, int type)
{
    zwnet_p         nw = (zwnet_p)appl_ctx->data;
    char            *rx_dsk;
    zwnet_cb_req_t  cb_req;

    if (dsk_len != MAX_DSK_KEY_LEN)
    {
        debug_zwapi_msg(&nw->plt_ctx, "zwnet_sec2_get_dsk_cb: invalid dsk key length of %u", dsk_len);
        return;
    }

    if (!nw->dsk_cb)
    {
        return;
    }

    rx_dsk = zwutil_dsk_to_str(dsk, dsk_len);

    if (!rx_dsk)
    {
        return;
    }

    cb_req.type = CB_TYPE_GW_DSK;
    cb_req.cb = nw->dsk_cb;
    cb_req.param.gw_dsk.user = nw->dsk_cb_prm;
#ifdef USE_SAFE_VERSION
	strncpy_s(cb_req.param.gw_dsk.dsk, 48, rx_dsk, 47);
#else
	strncpy(cb_req.param.gw_dsk.dsk, rx_dsk, 47);
#endif
    cb_req.param.gw_dsk.dsk[47] = '\0';

    util_list_add(nw->cb_mtx, &nw->cb_req_hd,
                  (uint8_t *)&cb_req, sizeof(zwnet_cb_req_t));
    plt_sem_post(nw->cb_sem);

    free(rx_dsk);
}


/**
zwnet_sec2_get_dsk - Get Z/IP gateway security 2 DSK
@param[in]	net	    network
@param[in]	cb      callback to report Z/IP gateway DSK
@param[in]	usr_ctx user context used in callback
@return		ZW_ERR_XXX
*/
int zwnet_sec2_get_dsk(zwnet_p net, get_dsk_fn cb, void *usr_ctx)
{
    int res = ZW_ERR_UNSUPPORTED;

    plt_mtx_lck(net->mtx);

    if ((net->curr_op == ZWNET_OP_ADD_NODE)
        || (net->curr_op == ZWNET_OP_RP_NODE)
        || (net->curr_op == ZWNET_OP_INITIATE))
    {   //Don't allow dsk get operation as it would interfere add, replace and joining operations
        plt_mtx_ulck(net->mtx);
        return ZW_ERR_LAST_OP_NOT_DONE;
    }

    if (net->ctl_cap & ZWNET_CTLR_CAP_S2)
    {
        net->dsk_cb = cb;
        net->dsk_cb_prm = usr_ctx;

        res = zw_sec2_dsk_get(&net->appl_ctx, zwnet_sec2_get_dsk_cb, GW_DSK_TYPE_JOIN);

        if (res < 0)
        {
            debug_zwapi_msg(&net->plt_ctx, "zwnet_sec2_get_dsk error:%d", res);
        }
    }

    plt_mtx_ulck(net->mtx);

    return res;
}


/**
zwnet_initiate_cb - Set learn mode callback function
@param[in]	appl_ctx    The application layer context
@param[in]	cb_prm		Callback parameters
@return
*/
static void zwnet_initiate_cb(appl_layer_ctx_t *appl_ctx, appl_lrn_mod_sec2_t *cb_prm)
{
    //LEARN_MODE_DONE = 6
    //LEARN_MODE_FAILED = 7
    //LEARN_MODE_DONE_SECURE = 8
    //LEARN_MODE_SECURITY_FAILED = 9
    //LEARN_MODE_INTERVIEW_COMPLETED = 10

    const char *status_msg[] = { "OK", "Failed", "Included securely",
                                 "Included but unsecure", "post-inclusion interview completed", "unknown" };

    uint8_t     sts;
    uint8_t     node_id;
    int         status;
    int         run_init_sm = 0;
    zwnet_p     nw = (zwnet_p)appl_ctx->data;

    sts = cb_prm->sts;
    node_id = cb_prm->node_id;

    status = ((sts >= 6) && (sts <= 10))? (sts - 6) : 5;
    debug_zwapi_msg(appl_ctx->plt_ctx, "set_learn_mode_cb: status:%u(%s), new node id:%u", sts, status_msg[status], node_id);

    if (cb_prm->sec2_valid)
    {
        char    *dsk;

        debug_zwapi_msg(&nw->plt_ctx, "security 2: granted key mask:%02Xh", (unsigned)cb_prm->grnt_keys);
        dsk = zwutil_dsk_to_str(cb_prm->dsk, MAX_DSK_KEY_LEN);
        if (dsk)
        {
            debug_zwapi_msg(&nw->plt_ctx, "security 2: DSK of including controller:%s", dsk);
            free(dsk);
        }
    }

    if ((sts == LEARN_MODE_DONE)
        || (sts == LEARN_MODE_DONE_SECURE)
        || (sts == LEARN_MODE_SECURITY_FAILED))
    {
        //
        //Callback upper layer and update the network structure
        //

        //Stop initiate
        //zw_set_learn_mode(&nw->appl_ctx, NULL, 0, ZW_SET_LEARN_MODE_DISABLE);

        //Notify the progress of the operation
        zwnet_notify(nw, ZWNET_OP_INITIATE, OP_INI_PROTOCOL_DONE, NULL);

        if (nw->initiate_ctx.join_new_nw)
        {
            zwnet_clear_all(nw);

            //Save S2 granted key
            if (cb_prm->sec2_valid)
            {
                nw->ctl.s2_keys_valid = 1;
                nw->ctl.s2_grnt_keys = cb_prm->grnt_keys;
            }
            else
            {
                nw->ctl.s2_keys_valid = 0;
            }
        }

        if ((nw->ctl_propty & CTLR_PROPTY_INTVW_CMPLT_STS) != 0)
        {
            if (node_id == 1)
            {   //Leaving network, get new network info
                run_init_sm = 1;
            }
            else
            {   //Wait for second status (which is LEARN_MODE_INTERVIEW_COMPLETED) from Z/IP gateway
                return;
            }
        }
        else
        {   //There will not be second status (which is LEARN_MODE_INTERVIEW_COMPLETED) from Z/IP gateway
            run_init_sm = 1;
        }

    }

    if (run_init_sm || (sts == LEARN_MODE_INTERVIEW_COMPLETED))
    {   //Run initialization state-machine
        zwnet_init_sm_prm_t     init_sm_prm = {0};

        if (nw->curr_op != ZWNET_OP_INITIATE)
        {
            debug_zwapi_msg(&nw->plt_ctx, "zwnet_initiate_cb: incorrect network operation:%u", nw->curr_op);
            return;
        }

        init_sm_prm.nm_op = ZWNET_OP_INITIATE;
        init_sm_prm.enable_file_load = 0;
        if (!nw->initiate_ctx.join_new_nw)
        {   //Not joining a new network
            init_sm_prm.initiate_ctx = &nw->initiate_ctx;
        }

        if (zwnet_init_sm(nw, EVT_INI_START, (uint8_t *)&init_sm_prm) == 0)
        {
            return;
        }
    }

    //Failed, reset operation to "no operation"
    nw->curr_op = ZWNET_OP_NONE;

    zwnet_notify(nw, ZWNET_OP_INITIATE, OP_FAILED, NULL);
}


/**
zwnet_sm_join_tout_cb - Add node state-machine timer callback
@param[in] data     Pointer to network
@return
*/
static void zwnet_sm_join_tout_cb(void *data)
{
    zwnet_p   nw = (zwnet_p)data;

    //Stop timer
    plt_mtx_lck(nw->mtx);
    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
    nw->sm_tmr_ctx = NULL;

    //Call state-machine
    zwnet_join_other_nw_sm(nw, EVT_JOIN_TMOUT, NULL);
    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_join_other_nw_sm - Join other network state-machine
@param[in] nw		Network
@param[in] evt      The event for the state-machine
@param[in] data     The data associated with the event
@return   Non-zero when the state-machine is started from idle state; otherwise return zero
@pre    Caller must lock the nw->mtx before calling this function
*/
static int zwnet_join_other_nw_sm(zwnet_p nw, zwnet_join_nw_evt_t evt, uint8_t *data)
{
    int res;

    switch (nw->join_sm_sta)
    {
        //----------------------------------------------------------------
        case JOIN_STA_IDLE:
        //----------------------------------------------------------------
            if (evt == EVT_JOIN_START)
            {
                memcpy(&nw->join_sm_ctx, data, sizeof(join_sm_ctx_t));

                res = zw_sec2_dsk_get(&nw->appl_ctx, zwnet_sm_gw_dsk_cb, GW_DSK_TYPE_JOIN);

                if (res < 0)
                {
                    debug_zwapi_msg(&nw->plt_ctx, "zw_sec2_dsk_get error:%d", res);
                    return 0;
                }

                //Change state
                nw->join_sm_sta = JOIN_STA_CTLR_DSK;

                //Restart timer
                plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_join_tout_cb, nw);

                return 1;//state-machine started successfully
            }
            break;

        //----------------------------------------------------------------
        case JOIN_STA_CTLR_DSK:
        //----------------------------------------------------------------
            switch (evt)
            {
                case EVT_JOIN_DSK_RPT:
                    {
                        zwnet_cb_req_t  cb_req;
                        char            *gw_dsk = (char *)data;

                        //Stop timer
                        plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                        nw->sm_tmr_ctx = NULL;

                        //Callback user to display DSK
                        cb_req.type = CB_TYPE_GW_DSK;
                        cb_req.cb = nw->join_sm_ctx.cb;
                        cb_req.param.gw_dsk.user = nw->join_sm_ctx.user;
                    #ifdef USE_SAFE_VERSION
                        strncpy_s(cb_req.param.gw_dsk.dsk, 48, gw_dsk, 47);
                    #else
                        strncpy(cb_req.param.gw_dsk.dsk, gw_dsk, 47);
                    #endif
                        cb_req.param.gw_dsk.dsk[47] = '\0';

                        util_list_add(nw->cb_mtx, &nw->cb_req_hd,
                                      (uint8_t *)&cb_req, sizeof(zwnet_cb_req_t));
                        plt_sem_post(nw->cb_sem);

                        res = zw_set_learn_mode(&nw->appl_ctx, zwnet_initiate_cb, 0, ZW_SET_LEARN_MODE_NWI);

                        if (res != 0)
                        {
                            //Set learn mode failed
                            zwnet_notify(nw, nw->curr_op, OP_FAILED, NULL);

                            //Reset operation to "no operation"
                            nw->curr_op = ZWNET_OP_NONE;
                        }

                        //Job done
                        nw->join_sm_sta = JOIN_STA_IDLE;

                        break;
                    }

                case EVT_JOIN_TMOUT:
                    zwnet_notify(nw, nw->curr_op, OP_FAILED, NULL);

                    //Reset operation to "no operation"
                    nw->curr_op = ZWNET_OP_NONE;

                    //Job done
                    nw->join_sm_sta = JOIN_STA_IDLE;
                    break;

                default:
                    break;

            }
    }

    return 0;

}


/**
zwnet_initiate_ex - called by a controller to join/leave a Z-wave network
@param[in]	net             network
@param[in]	cb              callback to report Z/IP gateway DSK when joining another Z-wave network.
                            Note that the callback will not be invoked if the Z/IP gateway is not S2 capable
                            or this API is invoked to leave a Z-wave network.
@param[in]	usr_ctx         user context used in callback
@param[in]	joining_mode    mode when joining a new network
@return		ZW_ERR_XXX, otherwise node callback will be called
*/
static int zwnet_initiate_ex(zwnet_p net, get_dsk_fn cb, void *usr_ctx, uint8_t joining_mode)
{
    int32_t result;
    uint8_t mode;

    plt_mtx_lck(net->mtx);
    if (net->curr_op != ZWNET_OP_NONE)
    {
        //Cancel low priority operation
        if (net->curr_op == ZWNET_OP_NODE_CACHE_UPT)
        {
            zwnet_abort(net);
            debug_zwapi_msg(&net->plt_ctx, "Canceled low priority operation");
        }
        else
        {
            debug_zwapi_msg(&net->plt_ctx, "Current operation not completed yet, try again later");
            plt_mtx_ulck(net->mtx);
            return ZW_ERR_LAST_OP_NOT_DONE;
        }
    }

    memset(&net->initiate_ctx, 0, sizeof(initiate_ctx_t));

    //Remove previously saved unhandled commands
    if (net->appl_cmd_cnt)
    {
        net->appl_cmd_cnt = 0;
        free(net->appl_cmd_saved);
        net->appl_cmd_saved = NULL;
    }

    if (net->ctl.obj.next)
    {   //There are other nodes in the network

//      debug_zwapi_msg(&net->plt_ctx, "Z-wave role:%d", net->zwave_role);

        if ((net->zwave_role == ZW_ROLE_SIS) || (net->zwave_role == ZW_ROLE_PRIMARY))
        {
            debug_zwapi_msg(&net->plt_ctx, "Initiate disallowed for SIS or primary controller network containing other nodes");
            plt_mtx_ulck(net->mtx);
            return ZW_ERR_DISALLOWED;
        }

        //Save network information before starting the learn mode
        net->initiate_ctx.homeid = net->homeid;
        net->initiate_ctx.ctlr_node = net->ctl.nodeid;

        mode = ZW_SET_LEARN_MODE_CLASSIC;
        debug_zwapi_msg(&net->plt_ctx, "Set learn mode for exclusion");
        result = zw_set_learn_mode(&net->appl_ctx, zwnet_initiate_cb, 0, mode);
    }
    else
    {
        mode = joining_mode;

        net->initiate_ctx.join_new_nw = 1;

        debug_zwapi_msg(&net->plt_ctx, "Set learn mode for joining other network");
        if (net->ctl_cap & ZWNET_CTLR_CAP_S2)
        {
            join_sm_ctx_t   sm_ctx = {0};

            if (cb)
            {
                sm_ctx.cb = cb;
                sm_ctx.user = usr_ctx;

                result = (zwnet_join_other_nw_sm(net, EVT_JOIN_START, (uint8_t *)&sm_ctx))? 0 : ZW_ERR_FAILED;
            }
            else
            {
                result = ZW_ERR_VALUE;
            }
        }
        else
        {
            result = zw_set_learn_mode(&net->appl_ctx, zwnet_initiate_cb, 0, mode);
        }
    }

    if (result != 0)
    {
        debug_zwapi_msg(&net->plt_ctx, "zw_set_learn_mode with error:%d", result);
        plt_mtx_ulck(net->mtx);
        return ZW_ERR_OP_FAILED;
    }

    net->curr_op = ZWNET_OP_INITIATE;
    plt_mtx_ulck(net->mtx);
    return ZW_ERR_NONE;

}


/**
zwnet_initiate - called by a controller to join/leave a Z-wave network
@param[in]	net		network
@param[in]	cb      callback to report Z/IP gateway DSK when joining another Z-wave network.
                    Note that the callback will not be invoked if the Z/IP gateway is not S2 capable
                    or this API is invoked to leave a Z-wave network.
@param[in]	usr_ctx user context used in callback
@return		ZW_ERR_XXX, otherwise node callback will be called
@see		zwnet_add, zwnet_fail, zwnet_migrate
*/
int zwnet_initiate(zwnet_p net, get_dsk_fn cb, void *usr_ctx)
{
    return zwnet_initiate_ex(net, cb, usr_ctx, ZW_SET_LEARN_MODE_NWI);
}


/**
zwnet_initiate_classic - called by a controller to join/leave a Z-wave network in classic mode
@param[in]	net		network
@param[in]	cb      callback to report Z/IP gateway DSK when joining another Z-wave network.
                    Note that the callback will not be invoked if the Z/IP gateway is not S2 capable
                    or this API is invoked to leave a Z-wave network.
@param[in]	usr_ctx user context used in callback
@return		ZW_ERR_XXX, otherwise node callback will be called
@note   This API is provided mainly for backward compatibility with old Z-wave controller which only supports classic mode
*/
int zwnet_initiate_classic(zwnet_p net, get_dsk_fn cb, void *usr_ctx)
{
    return zwnet_initiate_ex(net, cb, usr_ctx, ZW_SET_LEARN_MODE_CLASSIC);
}


/**
zwnode_update - Update node status and information
@param[in]	noded	Node
@return		ZW_ERR_xxx
*/
int zwnode_update(zwnoded_p noded)
{
    sm_job_t  sm_job = {0};
    zwnet_p   net;

    net = noded->net;

    plt_mtx_lck(net->mtx);
    if (net->curr_op != ZWNET_OP_NONE)
    {
        //Cancel low priority operation
        if (net->curr_op == ZWNET_OP_NODE_CACHE_UPT)
        {
            zwnet_abort(net);
            debug_zwapi_msg(&net->plt_ctx, "Canceled low priority operation");
        }
        else
        {
            debug_zwapi_msg(&net->plt_ctx, "Current operation not completed yet, try again later");
            plt_mtx_ulck(net->mtx);
            return ZW_ERR_LAST_OP_NOT_DONE;
        }
    }


    //Block node update of controller itself
    if (net->ctl.nodeid == noded->nodeid)
    {
        plt_mtx_ulck(net->mtx);
        return  ZW_ERR_UNSUPPORTED;
    }

    net->curr_op = ZWNET_OP_NODE_UPDATE;
    sm_job.op = ZWNET_OP_NODE_UPDATE;
    sm_job.num_node = 1;
    sm_job.node_id = &noded->nodeid;
    sm_job.cb = NULL;
    sm_job.auto_cfg = 0;

    if (!zwnet_node_info_sm(net, EVT_GET_NODE_INFO, (uint8_t *)&sm_job))
    {
        debug_zwapi_msg(&net->plt_ctx, "zwnode_update failed");
        net->curr_op = ZWNET_OP_NONE;
        plt_mtx_ulck(net->mtx);
        return ZW_ERR_OP_FAILED;
    }
    plt_mtx_ulck(net->mtx);

    return ZW_ERR_NONE;
}


/**
zwnet_update - Update network status and information
@param[in]	net		network
@return		ZW_ERR_XXX
*/
int zwnet_update(zwnet_p net)
{
    plt_mtx_lck(net->mtx);
    if (net->curr_op != ZWNET_OP_NONE)
    {
        //Cancel low priority operation
        if (net->curr_op == ZWNET_OP_NODE_CACHE_UPT)
        {
            zwnet_abort(net);
            debug_zwapi_msg(&net->plt_ctx, "Canceled low priority operation");
        }
        else
        {
            debug_zwapi_msg(&net->plt_ctx, "Current operation not completed yet, try again later");
            plt_mtx_ulck(net->mtx);
            return ZW_ERR_LAST_OP_NOT_DONE;
        }
    }

    //Call state-machine
    if (zwnet_updt_sm(net, EVT_NU_START, NULL) != 0)
    {
        plt_mtx_ulck(net->mtx);
        debug_zwapi_msg(&net->plt_ctx, "zwnet_update failed");
        return ZW_ERR_OP_FAILED;
    }
    net->curr_op = ZWNET_OP_UPDATE;
    plt_mtx_ulck(net->mtx);
    return ZW_ERR_NONE;

}


/**
zwnet_reset - Detach self from network ie. forget all nodes.
              A power-cycle is required after the reset.
@param[in]	net		network
@return		ZW_ERR_XXX
*/
int zwnet_reset(zwnet_p net)
{
    uint8_t     send_DRL = 0;   //Flag to indicate whether to send "device reset locally"
	AGI_Group   *pMatchAGIGroup = net->stAGIData.pAGIGroupList;

    plt_mtx_lck(net->mtx);
    if (net->curr_op != ZWNET_OP_NONE)
    {
        //Cancel low priority operation
        if (net->curr_op == ZWNET_OP_NODE_CACHE_UPT)
        {
            zwnet_abort(net);
            debug_zwapi_msg(&net->plt_ctx, "Canceled low priority operation");
        }
        else
        {
            debug_zwapi_msg(&net->plt_ctx, "Current operation not completed yet, try again later");
            plt_mtx_ulck(net->mtx);
            return ZW_ERR_LAST_OP_NOT_DONE;
        }
    }

	//As long as the lifeline AGI group has been assigned with some Node ID, send the device notification command,
    //except that the controller role is a secondary controller since it can't send any command to other node.
	if (pMatchAGIGroup && pMatchAGIGroup->stNodeEpInfo.byNodeID
        && (net->zwave_role != ZW_ROLE_SECONDARY))
	{
        send_DRL = 1;
	}

    net->curr_op = ZWNET_OP_RESET;

    if (zwnet_rst_sm(net, EVT_RST_START, &send_DRL) != 0)
    {
        net->curr_op = ZWNET_OP_NONE;
        debug_zwapi_msg(&net->plt_ctx, "Fail to reset controller");
        plt_mtx_ulck(net->mtx);
        return ZW_ERR_OP_FAILED;
    }

    plt_mtx_ulck(net->mtx);
    return ZW_ERR_NONE;
}


/**
zwnet_migrate_cb - Primary controller migration callback function
@param[in]	appl_ctx    The application layer context
@param[in]	add_ni		Add node callback info
@return
*/
static void zwnet_migrate_cb(appl_layer_ctx_t *appl_ctx, appl_node_info_t *add_ni)
{
/*
    #define ADD_NODE_STATUS_DONE                 6
    #define ADD_NODE_STATUS_FAILED               7
    #define NODE_ADD_STATUS_SECURITY_FAILED      9
*/
    const char *status_msg[] = { "OK", "Failed", "Migrated but insecure",
        "unknown"
    };
    zwnet_p             nw = (zwnet_p)appl_ctx->data;
    zwnode_p            node;
    int                 status;

    switch (add_ni->status)
    {
        case ADD_NODE_STATUS_DONE:
            status = 0;
            break;

        case ADD_NODE_STATUS_FAILED:
            status = 1;
            break;

        case NODE_ADD_STATUS_SECURITY_FAILED:
            status = 2;
            break;

        default:
            status = 3;
    }

    debug_zwapi_msg(&nw->plt_ctx, "zwnet_migrate_cb: status:%u (%s)", add_ni->status, status_msg[status]);

    if ((add_ni->status == ADD_NODE_STATUS_DONE)
        || (add_ni->status == NODE_ADD_STATUS_SECURITY_FAILED))
    {
        n2ip_sm_ctx_t   n2ip_sm_ctx = {0};

        debug_zwapi_msg(&nw->plt_ctx, "new node id:%u", add_ni->node_id);
        debug_zwapi_msg(&nw->plt_ctx, "listening=%u, optional functions=%u, sensor=%u", add_ni->listen, add_ni->optional, add_ni->sensor);
        debug_zwapi_msg(&nw->plt_ctx, "Device type: basic=%02Xh, generic=%02Xh, specific=%02X", add_ni->basic, add_ni->gen, add_ni->spec);
        if (add_ni->cmd_cnt > 0)
        {
            debug_zwapi_msg(&nw->plt_ctx, "unsecure command classes:");
            zwnet_cmd_cls_show(nw, add_ni->cmd_cls, add_ni->cmd_cnt);

        }
        if (add_ni->cmd_cnt_sec > 0)
        {
            debug_zwapi_msg(&nw->plt_ctx, "secure command classes:");
            zwnet_cmd_cls_show(nw, add_ni->cmd_cls_sec, add_ni->cmd_cnt_sec);

        }
        //Stop the add node operation
        //zw_controller_change(appl_ctx, NULL, CONTROLLER_CHANGE_STOP, TRANSMIT_OPTION_EXPLORE);
#ifdef MAP_IP_ASSOC_TO_ASSOC
        zwnet_ip_assoc_map(add_ni);
#endif

        //Create/Update node info
        zwnet_node_info_update(nw, add_ni, NULL);

        plt_mtx_lck(nw->mtx);
        node = zwnode_find(&nw->ctl, add_ni->node_id);

        if (node)
        {
            zwnet_node_cb(nw, node->nodeid, ZWNET_NODE_ADDED);
        }

        //Notify the progress of the operation
        zwnet_notify(nw, nw->curr_op, OP_ADD_NODE_PROTOCOL_DONE, NULL);

        //Call the resolve node to ip address state-machine and assign the callback function
        //to get detailed node information
        n2ip_sm_ctx.cb = zwnet_n2ip_cb;
        n2ip_sm_ctx.node_id[0] = add_ni->node_id;
        n2ip_sm_ctx.num_node = 1;
		n2ip_sm_ctx.params[0] = (void *)(uintptr_t)add_ni->node_id;
		n2ip_sm_ctx.params[1] = (void *)(uintptr_t)nw->curr_op;
        n2ip_sm_ctx.params[2] = (void *)(unsigned)OP_ADD_NODE_GET_NODE_INFO;

        if (zwnet_n2ip_sm(nw, EVT_N2IP_START, (uint8_t *)&n2ip_sm_ctx) != 0)
        {
            zwnet_abort(nw);
            zwnet_notify(nw, nw->curr_op, OP_FAILED, NULL);
        }
        plt_mtx_ulck(nw->mtx);

    }
    else
    {
        //Cancel operation without sending CONTROLLER_CHANGE_STOP
        plt_mtx_lck(nw->mtx);
        zwnet_sm_ni_stop(nw);
        plt_mtx_ulck(nw->mtx);

        zwnet_notify(nw, nw->curr_op, OP_FAILED, NULL);

        //Reset operation to "no operation"
        nw->curr_op = ZWNET_OP_NONE;

    }
}


/**
zwnet_migrate - Called by primary controller or SUC to make initiating controller primary.
@param[in]	net		network
@return		ZW_ERR_XXX, otherwise node callback will be called
@note       The controller invoking this function will become secondary.
*/
int zwnet_migrate(zwnet_p net)
{
    int32_t result;

    plt_mtx_lck(net->mtx);
    if (net->curr_op != ZWNET_OP_NONE)
    {
        //Cancel low priority operation
        if (net->curr_op == ZWNET_OP_NODE_CACHE_UPT)
        {
            zwnet_abort(net);
            debug_zwapi_msg(&net->plt_ctx, "Canceled low priority operation");
        }
        else
        {
            debug_zwapi_msg(&net->plt_ctx, "Current operation not completed yet, try again later");
            plt_mtx_ulck(net->mtx);
            return ZW_ERR_LAST_OP_NOT_DONE;
        }
    }

    if (!(net->ctl_role & ZWNET_CTLR_ROLE_PRI))
    {
        plt_mtx_ulck(net->mtx);
        return ZW_ERR_UNSUPPORTED;
    }

    result = zw_controller_change(&net->appl_ctx, zwnet_migrate_cb, CONTROLLER_CHANGE_START, TRANSMIT_OPTION_EXPLORE);

    if (result != 0)
    {
        debug_zwapi_msg(&net->plt_ctx, "zwnet_migrate with error:%d", result);
        plt_mtx_ulck(net->mtx);
        return ZW_ERR_OP_FAILED;
    }
    net->curr_op = ZWNET_OP_MIGRATE;
    plt_mtx_ulck(net->mtx);
    return ZW_ERR_NONE;
}


/**
zwnet_get_node - get first node (local controller) in the network
@param[in]	net		network
@param[out]	noded	node
@return		ZW_ERR_NONE if successful
*/
int zwnet_get_node(zwnet_p net, zwnoded_p noded)
{
    int result;
    plt_mtx_lck(net->mtx);
    result = zwnode_get_desc(&net->ctl, noded);
    plt_mtx_ulck(net->mtx);
    return  result;
}


/**
zwnet_get_node_by_id - get node with specified node id in the network
@param[in]	net		    Network
@param[in]	nodeid		Node id
@param[out]	noded	    Node descriptor (Can be NULL, if the purpose is to verify the existence of a node)
@return		ZW_ERR_NONE if successful
*/
int zwnet_get_node_by_id(zwnet_p net, uint8_t nodeid, zwnoded_p noded)
{
    int         result;
    zwnode_p    node;

    plt_mtx_lck(net->mtx);

    node = zwnode_find(&net->ctl, nodeid);

    if (node)
    {
        if (noded)
        {   //Create output
            result = zwnode_get_desc(node, noded);
        }
        else
        {   //Skip output
            result = ZW_ERR_NONE;
        }
        plt_mtx_ulck(net->mtx);
        return result;
    }

    plt_mtx_ulck(net->mtx);
    return  ZW_ERR_NODE_NOT_FOUND;
}


/**
zwnet_get_ep_by_id - get endpoint with specified node and endpoint id in the network
@param[in]	net		    Network
@param[in]	nodeid		Node id
@param[in]	epid		Endpoint id
@param[out]	epd	        Endpoint descriptor (Can be NULL, if the purpose is to verify the existence of an endpoint)
@return		ZW_ERR_NONE if successful
*/
int zwnet_get_ep_by_id(zwnet_p net, uint8_t nodeid, uint8_t epid, zwepd_p epd)
{
    zwnode_p    node;
    zwep_p      ep;

    plt_mtx_lck(net->mtx);

    node = zwnode_find(&net->ctl, nodeid);

    if (node)
    {   //Found node
        ep = zwep_find(&node->ep, epid);
        if (ep)
        {   //Found endpoint
            if (epd)
                zwep_get_desc(ep, epd);

            plt_mtx_ulck(net->mtx);
            return ZW_ERR_NONE;
        }
    }

    plt_mtx_ulck(net->mtx);
    return  ZW_ERR_EP_NOT_FOUND;
}


/**
zwnet_get_if_by_id - get interface with specified node, endpoint and interface id in the network
@param[in]	net		    Network
@param[in]	nodeid		Node id
@param[in]	epid		Endpoint id
@param[in]	cls		    Interface id (command class)
@param[out]	ifd	        Interface descriptor (Can be NULL, if the purpose is to verify the existence of an interface)
@return		ZW_ERR_NONE if successful
*/
int zwnet_get_if_by_id(zwnet_p net, uint8_t nodeid, uint8_t epid, uint16_t cls, zwifd_p ifd)
{
    zwnode_p    node;
    zwep_p      ep;
    zwif_p      intf;

    plt_mtx_lck(net->mtx);

    node = zwnode_find(&net->ctl, nodeid);

    if (node)
    {   //Found node
        ep = zwep_find(&node->ep, epid);
        if (ep)
        {   //Found endpoint
            intf = zwif_find_cls(ep->intf, cls);

            if (intf)
            {   //Found interface
                if (ifd)
                    zwif_get_desc(intf, ifd);

                plt_mtx_ulck(net->mtx);
                return ZW_ERR_NONE;
            }
        }
    }

    plt_mtx_ulck(net->mtx);
    return  ZW_ERR_INTF_NOT_FOUND;
}


/**
zwnet_version - Get the home controller API version and subversion
@param[out]	ver		Version
@param[out]	subver	Sub version
@return
*/
void zwnet_version(uint8_t *ver, uint8_t *subver)
{
    *ver = APPL_VERSION;
    *subver = APPL_SUBVERSION;
}


/**
zwnet_node_info_dump - dump the nodes information
@param[in]	net		The network
@return
*/
void    zwnet_node_info_dump(zwnet_p net)
{
    zwnode_p    zw_node;
    zwif_p      intf;
    zwep_p      ep;

    plt_mtx_lck(net->mtx);

    zw_node = &net->ctl;
    while (zw_node)
    {
        //Show node info
        debug_zwapi_msg(&net->plt_ctx, "__________________________________________________________________________");
        debug_zwapi_msg(&net->plt_ctx, "Node id:%u, Home id:%08X", (unsigned)zw_node->nodeid, (unsigned)net->homeid);
        debug_zwapi_msg(&net->plt_ctx, "Node security status:%s", (zw_node->propty & NODE_PROPTY_ADD_SECURE)? "secured" : "unsecured");
        debug_zwapi_msg(&net->plt_ctx, "Device class: basic:%02Xh, generic:%02Xh, specific:%02Xh",
                     (unsigned)zw_node->basic, (unsigned)zw_node->ep.generic,
                     (unsigned)zw_node->ep.specific);
        debug_zwapi_msg(&net->plt_ctx, "Z-wave library type:%u", zw_node->lib_type);
        debug_zwapi_msg(&net->plt_ctx, "Z-wave protocol version:%u.%02u", (unsigned)(zw_node->proto_ver >> 8),
                     (unsigned)(zw_node->proto_ver & 0xFF));
        debug_zwapi_msg(&net->plt_ctx, "Application version:%u.%02u", (unsigned)(zw_node->app_ver >> 8),
                     (unsigned)(zw_node->app_ver & 0xFF));

        ep = &zw_node->ep;
        while (ep)
        {
            debug_zwapi_msg(&net->plt_ctx, "end point id:%u", ep->epid);
            debug_zwapi_msg(&net->plt_ctx, "end point device class: generic:%02Xh, specific:%02Xh",
                            (unsigned)ep->generic, (unsigned)ep->specific);
            debug_zwapi_msg(&net->plt_ctx, "end point name:%s", ep->name);
            debug_zwapi_msg(&net->plt_ctx, "end point location:%s", ep->loc);

            intf = ep->intf;
            //Show command class info
            while (intf)
            {
                debug_zwapi_msg(&net->plt_ctx, "  Command class: %02X, ver:%u, real ver:%u", (unsigned)intf->cls,
                                (unsigned)intf->ver, (unsigned)intf->real_ver);
                //Show report commands
                if (intf->rpt_num > 0)
                {
                    int i;
                    for (i=0; i<intf->rpt_num; i++)
                    {
                        debug_zwapi_msg(&net->plt_ctx, "              Report command: %02X", (unsigned)intf->rpt[i].rpt_cmd);
                    }
                }
                //Get the next interface
                intf = (zwif_p)intf->obj.next;
            }
            //Get the next end point
            ep = (zwep_p)ep->obj.next;
        }
        //Get the next node
        zw_node = (zwnode_p)zw_node->obj.next;
    }

    plt_mtx_ulck(net->mtx);
}

/**
zwnet_get_user - get user context
@param[in]	net	        Network
@return	user context passed during initialization
*/
void *zwnet_get_user(zwnet_p net)
{
    return net->init.user;
}


/**
zwnet_send_nif_cb - send node information frame callback
@param[in]	appl_ctx	Application layer context
@param[in]	tx_sts      Transmit status
@param[in]	user_prm    The user specific parameter
@param[in]	node_id     Node id
@param[in]	param       Additional parameters
@return		ZW_ERR_xxx
*/
static void zwnet_send_nif_cb(appl_layer_ctx_t *appl_ctx, int8_t tx_sts, void *user_prm, uint8_t node_id, zw_tx_cb_prm_t *param)
{
    zwnet_p nw;

    nw = (zwnet_p)appl_ctx->data;

    if (tx_sts != ZWHCI_NO_ERROR)
    {
        //Display error ZWNET_TX_XXX
        debug_zwapi_msg(&nw->plt_ctx, "zwnet_send_nif_cb with error:%u", zwif_tx_sts_get(tx_sts));
    }

    zwnet_abort(nw);
    //Callback
    zwnet_notify(nw, ZWNET_OP_SEND_NIF,
                    (tx_sts == ZWHCI_NO_ERROR)? OP_DONE : OP_FAILED, NULL);
}


/**
zwnet_send_nif - send node information frame to a node or broadcast it
@param[in]	net		    Network
@param[in]	noded	    Destination node to receive the node information frame
@param[in]	broadcast	Broadcast flag. 1= broadcast; 0= single cast
@return		ZW_ERR_XXX.
*/
int zwnet_send_nif(zwnet_p net, int noded, uint8_t broadcast)
{
    int             result;
    appl_snd_nif_t  prm;

    plt_mtx_lck(net->mtx);
    if (net->curr_op != ZWNET_OP_NONE)
    {
        //Cancel low priority operation
        if (net->curr_op == ZWNET_OP_NODE_CACHE_UPT)
        {
            zwnet_abort(net);
            ALOGI("Canceled low priority operation");
        }
        else
        {
            ALOGI("Current operation not completed yet, try again later, current op:%d",net->curr_op);
            plt_mtx_ulck(net->mtx);
            return ZW_ERR_LAST_OP_NOT_DONE;
        }
    }

    prm.tx_opt = TRANSMIT_OPTION_EXPLORE;
    if (!broadcast)
    {
        prm.tx_opt |= TRANSMIT_OPTION_ACK;
    }

    prm.cb = zwnet_send_nif_cb;
    prm.cb_prm = NULL;
    prm.fr_node_id = 0;
    prm.to_node_id = (broadcast)? 0xFF : noded;

    result = zip_node_info_send(&net->appl_ctx, &prm);

    if (result == 0)
    {
        net->curr_op = ZWNET_OP_SEND_NIF;
    }

    plt_mtx_ulck(net->mtx);
    return result;

}


/**
zwnet_sm_n2ip_tout_cb - Resolve node to ip address state machine timer callback
@param[in] data     Pointer to network
@return
*/
static void    zwnet_sm_n2ip_tout_cb(void *data)
{
    zwnet_p   nw = (zwnet_p)data;

    //Stop timer
    plt_mtx_lck(nw->mtx);
    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
    nw->sm_tmr_ctx = NULL;//make sure timer context is null, else restart timer will crash

    //Call state-machine
    zwnet_n2ip_sm(nw, EVT_N2IP_TMOUT, NULL);
    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_get_ip_addr_cb - Resolve a node id into IPv6 address callback function
@param[in]	appl_ctx    The application layer context
@param[in]	rpt		    Status report
@return
*/
static void zwnet_get_ip_addr_cb(appl_layer_ctx_t *appl_ctx, nd_advt_rpt_t *rpt)
{
    zwnet_p nw = (zwnet_p)appl_ctx->data;

    debug_zwapi_msg(&nw->plt_ctx, "zwnet_get_ip_addr_cb:");

    switch (rpt->status)
    {
        case ZIP_ND_INFORMATION_OK:
            debug_zwapi_msg(&nw->plt_ctx, "Home id:%08X,  node id:%u, local=%u, ip:", rpt->homeid, rpt->node_id, rpt->addr_local);
            debug_zwapi_bin_msg(&nw->plt_ctx, rpt->ipv6_addr, IPV6_ADDR_LEN);
            break;

        case ZIP_ND_INFORMATION_OBSOLETE:
            debug_zwapi_msg(&nw->plt_ctx, "Obsolete node id:%u, local=%u, ip:", rpt->node_id, rpt->addr_local);
            debug_zwapi_bin_msg(&nw->plt_ctx, rpt->ipv6_addr, IPV6_ADDR_LEN);
            break;

        case ZIP_ND_INFORMATION_NOT_FOUND:
            debug_zwapi_msg(&nw->plt_ctx, "Node not found");
            break;

    }
    //Call state-machine
    plt_mtx_lck(nw->mtx);
    zwnet_n2ip_sm(nw, EVT_N2IP_NODE_ID_ADDR, (uint8_t *)rpt);
    plt_mtx_ulck(nw->mtx);
}


/**
zwnet_sm_ip_retry - Retry to resolve node to ip address and update the state-machine's state
@param[in]	nw		    Network
@return  0 on success, negative error number on failure
*/
static int zwnet_sm_ip_retry(zwnet_p nw)
{
    if (nw->n2ip_sm_ctx.retry_cnt < ZWNET_N2IP_RETRY_MAX)
    {
        //Start timer
        nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_N2IP_RETRY_TMOUT, zwnet_sm_n2ip_tout_cb, nw);

        //Change state
        nw->n2ip_sm_ctx.state = N2IP_STA_WAIT_VALID_IP;

        return 0;
    }

    return -1;
}


/**
zwnet_n2ip_sm - Resolve node to ip address state-machine
@param[in]	nw		Network
@param[in] evt      The event for the state-machine
@param[in] data     The data associated with the event
@return             0 if the state-machine was started successfully; otherwise return non-zero
@pre    Caller must lock the nw->mtx before calling this function
*/
static int zwnet_n2ip_sm(zwnet_p nw, n2ip_evt_t evt, uint8_t *data)
{
    int result;

    switch (nw->n2ip_sm_ctx.state)
    {
        //----------------------------------------------------------------
        case N2IP_STA_IDLE:
        //----------------------------------------------------------------
            if (evt == EVT_N2IP_START)
            {
                n2ip_sm_ctx_t   *n2ip_sm_ctx = (n2ip_sm_ctx_t *)data;

                //Save the parameters
                nw->n2ip_sm_ctx = *n2ip_sm_ctx;

                //Get the first node IP address
                result = zip_node_ipv6_get(&nw->appl_ctx, zwnet_get_ip_addr_cb, nw->n2ip_sm_ctx.node_id[0]);

                if (result == 0)
                {
                    //Change state
                    nw->n2ip_sm_ctx.state = N2IP_STA_NODE_IP_DSCVRY;

                    //Restart timer
                    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                    nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_N2IP_TMOUT, zwnet_sm_n2ip_tout_cb, nw);

                    nw->n2ip_sm_ctx.rd_idx = 0;
                    nw->n2ip_sm_ctx.retry_cnt = 0;
                    nw->n2ip_sm_ctx.retx_cnt = 0;

                    return 0;
                }

                debug_zwapi_msg(&nw->plt_ctx, "zwnet_n2ip_sm: failed to resolve node to ip from Z/IP gateway controller");
                break;

            }
            break;

        //----------------------------------------------------------------
        case N2IP_STA_NODE_IP_DSCVRY:
        //----------------------------------------------------------------
            switch (evt)
            {
                case EVT_N2IP_TMOUT:
                    debug_zwapi_msg(&nw->plt_ctx, "zwnet_n2ip_sm: resolve node to ip timeout, retransmit count:%u", nw->n2ip_sm_ctx.retx_cnt);
                    if (nw->n2ip_sm_ctx.retx_cnt++ < ZWNET_N2IP_RETRANSMIT_MAX)
                    {
                        //Get the node IP address
                        result = zip_node_ipv6_get(&nw->appl_ctx, zwnet_get_ip_addr_cb,
                                                   nw->n2ip_sm_ctx.node_id[nw->n2ip_sm_ctx.rd_idx]);

                        if (result == 0)
                        {
                            //Start timer
                            nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_N2IP_TMOUT, zwnet_sm_n2ip_tout_cb, nw);

                            break;
                        }
                        debug_zwapi_msg(&nw->plt_ctx, "zwnet_n2ip_sm: failed to resolve node to ip from Z/IP gateway controller");
                    }
                    //Change to idle state
                    nw->n2ip_sm_ctx.state = N2IP_STA_IDLE;

                    //Callback to report failure
                    if (nw->n2ip_sm_ctx.cb)
                    {
                        nw->n2ip_sm_ctx.cb(nw, 0xFF, nw->n2ip_sm_ctx.params);
                    }

                    break;

                case EVT_N2IP_NODE_ID_ADDR:
                    {
                        nd_advt_rpt_t *rpt = (nd_advt_rpt_t *)data;
                        int           send_cmd_ok = 1;  //Flag to indicate whether sending resolve node to
                                                        //ip address command is successful.

                        //Check whether the report is for the expected node
                        if (rpt->node_id != nw->n2ip_sm_ctx.node_id[nw->n2ip_sm_ctx.rd_idx])
                        {   //The node id is different
                            break;
                        }

                        //Stop timer
                        plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                        nw->sm_tmr_ctx = NULL;

                        if (rpt->status == ZIP_ND_INFORMATION_OK)
                        {
                            appl_layer_ctx_t    *appl_ctx = &nw->appl_ctx;
                            uint8_t             node_id;

                            //Save the node-ip address into the translation table
                            if (appl_ctx->use_ipv4)
                            {   //IPv4
                                if (util_ipv4_zero_cmp(&rpt->ipv6_addr[12]) != 0)
                                {
                                    //Remove the existing IP assigned to any obsolete node ID
                                    if (zip_translate_ipaddr(appl_ctx, &rpt->ipv6_addr[12], 4, &node_id))
                                    {
                                        zip_close_dtls_conn(appl_ctx, node_id);
                                        zip_node_ipaddr_rm(appl_ctx, node_id);
                                    }

                                    zip_node_ipaddr_set(appl_ctx, &rpt->ipv6_addr[12], 4, rpt->node_id);
                                }
                                else
                                {
                                    if(zwnet_sm_ip_retry(nw) == 0)
                                    {
                                        break;
                                    }
                                }
                            }
                            else    //IPv6
                            {
                                if (util_ipv6_zero_cmp(rpt->ipv6_addr) != 0)
                                {
                                    //Remove the existing IP assigned to any obsolete node ID
                                    if (zip_translate_ipaddr(appl_ctx, rpt->ipv6_addr, IPV6_ADDR_LEN, &node_id))
                                    {
                                        zip_close_dtls_conn(appl_ctx, node_id);
                                        zip_node_ipaddr_rm(appl_ctx, node_id);
                                    }
                                    zip_node_ipaddr_set(appl_ctx, rpt->ipv6_addr, IPV6_ADDR_LEN, rpt->node_id);
                                }
                                else
                                {
                                    if(zwnet_sm_ip_retry(nw) == 0)
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                        else if ((rpt->status == ZIP_ND_INFORMATION_OBSOLETE) && nw->n2ip_sm_ctx.retry_on_obsolete)
                        {
                            if(zwnet_sm_ip_retry(nw) == 0)
                            {
                                break;
                            }
                        }

                        //Check whether there is any node unprocessed
                        if (++nw->n2ip_sm_ctx.rd_idx < nw->n2ip_sm_ctx.num_node)
                        {
                            //Get the node IP address
                            result = zip_node_ipv6_get(&nw->appl_ctx, zwnet_get_ip_addr_cb,
                                                       nw->n2ip_sm_ctx.node_id[nw->n2ip_sm_ctx.rd_idx]);

                            if (result == 0)
                            {
                                //Start timer
                                nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_N2IP_TMOUT, zwnet_sm_n2ip_tout_cb, nw);

                                nw->n2ip_sm_ctx.retx_cnt = 0;
                                nw->n2ip_sm_ctx.retry_cnt = 0;

                                break;
                            }
                            send_cmd_ok = 0;
                            debug_zwapi_msg(&nw->plt_ctx, "zwnet_n2ip_sm: failed to resolve node to ip from Z/IP gateway controller");
                        }

                        //Change to idle state
                        nw->n2ip_sm_ctx.state = N2IP_STA_IDLE;

                        //Callback to report status
                        if (nw->n2ip_sm_ctx.cb)
                        {
                            nw->n2ip_sm_ctx.cb(nw, (send_cmd_ok)? 0 : 0xFF, nw->n2ip_sm_ctx.params);
                        }
                    }
                    break;

                default:
                    break;

            }
            break;

        //----------------------------------------------------------------
        case N2IP_STA_WAIT_VALID_IP:
        //----------------------------------------------------------------
            if (evt == EVT_N2IP_TMOUT)
            {
                //Increment retry count
                nw->n2ip_sm_ctx.retry_cnt++;

                //Get the node IP address
                result = zip_node_ipv6_get(&nw->appl_ctx, zwnet_get_ip_addr_cb,
                                           nw->n2ip_sm_ctx.node_id[nw->n2ip_sm_ctx.rd_idx]);

                if (result == 0)
                {
                    //Start timer
                    nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_N2IP_TMOUT, zwnet_sm_n2ip_tout_cb, nw);

                    //Change state
                    nw->n2ip_sm_ctx.state = N2IP_STA_NODE_IP_DSCVRY;

                    //Reset re-transmission count
                    nw->n2ip_sm_ctx.retx_cnt = 0;

                    break;
                }

                debug_zwapi_msg(&nw->plt_ctx, "zwnet_n2ip_sm: failed to resolve node to ip from Z/IP gateway controller");

                //Change to idle state
                nw->n2ip_sm_ctx.state = N2IP_STA_IDLE;

                //Callback to report failure
                if (nw->n2ip_sm_ctx.cb)
                {
                    nw->n2ip_sm_ctx.cb(nw, 0xFF, nw->n2ip_sm_ctx.params);
                }
            }
            break;
    }

    return -1;
}


/**
zwnet_sm_ctlr_cmplt - Stop the controller info state-machine and callback with status
@param[in]	nw		Network
@param[in]	status	Completion status
@return
*/
static void zwnet_sm_ctlr_cmplt(zwnet_p nw, int status)
{
    //Reset to idle state
    nw->ctlr_sm_sta = CTLR_STA_IDLE;

    //Callback
#ifdef IGNORE_INIT_ERROR
    nw->ctlr_sm_ctx.ctlr_sm_cb(nw, 0);
#else
    nw->ctlr_sm_ctx.ctlr_sm_cb(nw, status);
#endif

}


/**
zwnet_sm_ctlr_tout_cb - Controller info state-machine timer callback
@param[in] data     Pointer to network
@return
*/
static void zwnet_sm_ctlr_tout_cb(void *data)
{
    zwnet_p   nw = (zwnet_p)data;

    //Stop timer
    plt_mtx_lck(nw->mtx);
    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
    nw->sm_tmr_ctx = NULL;

    //Call state-machine
    zwnet_ctlr_info_sm(nw, EVT_CTLR_TMOUT, NULL);
    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_ctlr_ver_rpt_cb - Version report callback
@param[in]	intf	The interface that received the report
@param[in]	cls	    The command class the version is referred to
@param[in]	ver	    The version of cls
@return
*/
static void zwnet_ctlr_ver_rpt_cb(zwif_p intf, uint16_t cls, uint8_t ver)
{
    zwnet_p     nw;
    uint8_t     data[16];

    data[0] = cls >> 8;
    data[1] = cls & 0x00FF;
    data[2] = ver;
    memcpy(data + 4, &intf, sizeof(zwif_p));
    nw = intf->ep->node->net;

    plt_mtx_lck(nw->mtx);
    zwnet_ctlr_info_sm(nw, EVT_CTLR_CMD_VER_REPORT, data);
    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_sm_ctlr_cc_ver_get - Get version of each command class
@param[in]	nw		    Network
@param[in]	intf		Version interface
@param[in]	cmd_cls     Command class to query
@return  0 on success, negative error number on failure
*/
static int zwnet_sm_ctlr_cc_ver_get(zwnet_p nw, zwif_p intf, uint16_t cmd_cls)
{
    int     result;
    zwifd_t ifd;
    uint8_t param;

    zwif_get_desc(intf, &ifd);

    //Setup report callback
    result = zwif_set_report(&ifd, zwnet_ctlr_ver_rpt_cb, VERSION_COMMAND_CLASS_REPORT);
    if (result != 0)
    {
        return result;
    }

    //Get the version of command class
    param = (uint8_t)cmd_cls;
    result = zwif_get_report_ex(&ifd, &param, 1, VERSION_COMMAND_CLASS_GET, zwif_tx_sts_cb, ZWIF_RPT_GET_ZIP);

    if (result < 0)
    {
        debug_zwapi_msg(&nw->plt_ctx, "zwnet_sm_ctlr_cc_ver_get: zwif_get_report with error:%d", result);
        return result;
    }
    //Save the command class in version get
    nw->ni_sm_cls = cmd_cls;

    //Start timer
    nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_ctlr_tout_cb, nw);

    return ZW_ERR_NONE;
}


/**
zwnet_gw_sup_cmd_cls_create - Create a Z/IP gateway supported command class list
@param[in]  ni		        Node info
@param[out] cmd_cls_buf     The buffer to store the generated command classes
@param[out] cmd_cls_cnt     The number of command classes stored in the cmd_cls_buf
@return       0 if o.k.; else return non-zero
@post         If return is 0, the caller must free the output buffer cmd_cls_buf
*/
static int zwnet_gw_sup_cmd_cls_create(appl_node_info_t *ni, sup_cmd_cls_t **cmd_cls_buf, uint8_t *cmd_cls_cnt)
{
    int             i;
    uint8_t         buf_index;
    uint8_t         tot_cmd_cls;
    sup_cmd_cls_t   *tmp_cmd_buf;

    //Calculate total number of command classes
    tot_cmd_cls = ni->cmd_cnt + ni->cmd_cnt_sec;

    if (tot_cmd_cls == 0)
    {
        return ZW_ERR_VALUE;
    }

    //Copy and deduplicate command classes
    tmp_cmd_buf = (sup_cmd_cls_t *)calloc(tot_cmd_cls, sizeof(sup_cmd_cls_t));
    if (!tmp_cmd_buf)
    {
        return ZW_ERR_MEMORY;
    }

    //Copy the insecure command class list
    buf_index = 0;
    for (i=0; i<ni->cmd_cnt; i++)
    {
        tmp_cmd_buf[buf_index].cls = ni->cmd_cls[i];
        tmp_cmd_buf[buf_index++].propty = BITMASK_CMD_CLS_INSECURE;
    }

    //Copy and deduplicate the secure command class list
    for (i=0; i<ni->cmd_cnt_sec; i++)
    {
        if(!zwnet_sup_cmd_cls_find(tmp_cmd_buf, ni->cmd_cls_sec[i], buf_index))
        {   //Not found
            tmp_cmd_buf[buf_index].cls = ni->cmd_cls_sec[i];
            tmp_cmd_buf[buf_index++].propty = BITMASK_CMD_CLS_SECURE;
        }
        //TODO: mark the existing class property with secure attribute if found.
    }

    //Assign return values
    *cmd_cls_buf = tmp_cmd_buf;
    *cmd_cls_cnt = buf_index;
    return 0;

}


/**
zwnet_ctlr_manf_rpt_cb - manufacturer and product id report callback
@param[in]	intf	        The interface that received the report
@param[in]	manf_pdt_id	    Array that stores manufacturer, product type and product id
@return
*/
static void zwnet_ctlr_manf_rpt_cb(zwif_p intf, uint16_t *manf_pdt_id)
{
    zwnet_p     nw = intf->ep->node->net;
    uint8_t     data[16 + 6];

    memcpy(data, manf_pdt_id, 6);
    memcpy(data + 6, &intf, sizeof(zwif_p));
    plt_mtx_lck(nw->mtx);
    zwnet_ctlr_info_sm(nw, EVT_CTLR_MANF_SPEC_REPORT, data);
    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_ctlr_sm_manf_get - Get the manufacturer and product id of the node and update the state-machine's state
@param[in]	nw		    Network
@param[in]	intf		interface
@return  0 on success, negative error number on failure
*/
static int zwnet_ctlr_sm_manf_get(zwnet_p nw, zwif_p intf)
{
    int     result;
    zwifd_t ifd;

    zwif_get_desc(intf, &ifd);

    //Setup report callback
    result = zwif_set_report(&ifd, zwnet_ctlr_manf_rpt_cb, MANUFACTURER_SPECIFIC_REPORT);
    if (result != 0)
    {
        return result;
    }

    //Get the manufacturer id, product type id and product id
    result = zwif_get_report_ex(&ifd, NULL, 0, MANUFACTURER_SPECIFIC_GET, zwif_tx_sts_cb, ZWIF_RPT_GET_ZIP);
    if (result < 0)
    {
        debug_zwapi_msg(&nw->plt_ctx, "zwnet_ctlr_sm_manf_get: zwif_get_report with error:%d", result);
        return result;
    }

    //Update state-machine's state
    nw->ctlr_sm_sta = CTLR_STA_CTLR_MANF_SPEC;

    //Change sub-state
    nw->ctlr_sm_sub_sta = MS_SUBSTA_PDT_ID;

    //Restart timer
    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
    nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_ctlr_tout_cb, nw);
    return ZW_ERR_NONE;

}


/**
zwnet_ctlr_mbx_cfg_rpt_cb - mailbox configuration report callback
@param[in]	    intf        interface
@param[in]	    mbx_cfg     mailbox configuration
@return
*/
static void zwnet_ctlr_mbx_cfg_rpt_cb(zwif_p intf, mailbox_cfg_t *mbx_cfg)
{
    zwnet_p     nw = intf->ep->node->net;
    uint8_t     data[20];

    memcpy(data, &mbx_cfg, sizeof(mailbox_cfg_t *));
    memcpy(data + sizeof(mailbox_cfg_t *), &intf, sizeof(zwif_p));
    plt_mtx_lck(nw->mtx);
    zwnet_ctlr_info_sm(nw, EVT_CTLR_MAILBOX_CFG_REPORT, data);
    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_ctlr_sm_mbx_get - Get mailbox configuration and update the state-machine's state
@param[in]	nw		    Network
@param[in]	intf		interface
@return  0 on success, negative error number on failure
*/
static int zwnet_ctlr_sm_mbx_get(zwnet_p nw, zwif_p intf)
{
    int     result;
    zwifd_t ifd;

    zwif_get_desc(intf, &ifd);

    //Setup report callback
    result = zwif_set_report(&ifd, zwnet_ctlr_mbx_cfg_rpt_cb, MAILBOX_CONFIGURATION_REPORT);
    if (result != 0)
    {
        return result;
    }

    //Get report
    result = zwif_get_report_ex(&ifd, NULL, 0, MAILBOX_CONFIGURATION_GET, zwif_tx_sts_cb, ZWIF_RPT_GET_ZIP);
    if (result < 0)
    {
        debug_zwapi_msg(&nw->plt_ctx, "zwnet_ctlr_sm_mbx_get: zwif_get_report with error:%d", result);
        return result;
    }

    //Update state-machine's state
    nw->ctlr_sm_sta = CTLR_STA_MAILBOX;

    //Restart timer
    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
    nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_ctlr_tout_cb, nw);
    return ZW_ERR_NONE;

}


/**
zwnet_ctlr_dev_id_rpt_cb - device id report callback
@param[in]	intf	    The interface that received the report
@param[in]	dev_id	    Device ID
@return
*/
static void zwnet_ctlr_dev_id_rpt_cb(zwif_p intf, dev_id_t *dev_id)
{
    zwnet_p     nw = intf->ep->node->net;
    uint8_t     data[32];

    memcpy(data, &dev_id, sizeof(dev_id_t *));
    memcpy(data + sizeof(dev_id_t *), &intf, sizeof(zwif_p));
    plt_mtx_lck(nw->mtx);
    zwnet_ctlr_info_sm(nw, EVT_CTLR_DEVICE_ID_REPORT, data);
    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_ctlr_sm_dev_id_get - Get device id
@param[in]	nw		    Network
@param[in]	intf		interface
@param[in]	dev_id_type Device id type: DEV_ID_TYPE_XXX
@return  0 on success, negative error number on failure
*/
static int zwnet_ctlr_sm_dev_id_get(zwnet_p nw, zwif_p intf, uint8_t dev_id_type)
{
    int     result;
    zwifd_t ifd;

    zwif_get_desc(intf, &ifd);

    //Setup report callback
    result = zwif_set_report(&ifd, zwnet_ctlr_dev_id_rpt_cb, DEVICE_SPECIFIC_REPORT_V2);
    if (result != 0)
    {
        return result;
    }

    //Get the device id
    result = zwif_get_report_ex(&ifd, &dev_id_type, 1, DEVICE_SPECIFIC_GET_V2, zwif_tx_sts_cb, ZWIF_RPT_GET_ZIP);
    if (result < 0)
    {
        debug_zwapi_msg(&nw->plt_ctx, "zwnet_ctlr_sm_dev_id_get: zwif_get_report with error:%d", result);
        return result;
    }

    //Restart timer
    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
    nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_ctlr_tout_cb, nw);

    //Change sub-state
    nw->ctlr_sm_sub_sta = MS_SUBSTA_DEV_ID_OEM;

    return ZW_ERR_NONE;

}


/**
zwnet_sm_ctlr_ni_retry_cb - Retry to get controller node info timer callback
@param[in] data     Pointer to network
@return
*/
static void zwnet_sm_ctlr_ni_retry_cb(void *data)
{
    zwnet_p   nw = (zwnet_p)data;

    //Stop timer
    plt_mtx_lck(nw->mtx);
    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
    nw->sm_tmr_ctx = NULL;

    //Call state-machine
    zwnet_ctlr_info_sm(nw, EVT_CTLR_NODE_INFO_RETRY, NULL);
    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_ctlr_info_sm - Controller info state-machine
@param[in] nw		Network
@param[in] evt      The event for the state-machine
@param[in] data     The data associated with the event
@return   Non-zero when the state-machine is started from idle state; otherwise return zero
@pre    Caller must lock the nw->mtx before calling this function
*/
static int zwnet_ctlr_info_sm(zwnet_p nw, zwnet_ctlr_evt_t evt, uint8_t *data)
{
    int result;

    switch (nw->ctlr_sm_sta)
    {
        //----------------------------------------------------------------
        case CTLR_STA_IDLE:
        //----------------------------------------------------------------
            if (evt == EVT_CTLR_START)
            {
                //Save the callback function
                nw->ctlr_sm_ctx = *((ctlr_sm_ctx_t *)data);

                //Get Z/IP gateway controller node id
                result = zip_node_id_get(&nw->appl_ctx, zwnet_ctlr_id_addr_cb,
                                         nw->appl_ctx.ssn_ctx.frm_ctx.tpt_ctx.zip_router, 0);
                nw->sm_gen_retry = 0;

                if (result == 0)
                {
                    //Change state
                    nw->ctlr_sm_sta = CTLR_STA_CTLR_NODE_ID;

                    //Stop timer
                    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);

                    //Start timer
                    nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_ctlr_tout_cb, nw);

                    return 1;//state-machine started successfully
                }

                debug_zwapi_msg(&nw->plt_ctx, "zwnet_ctlr_info_sm: failed to get Z/IP gateway controller node id");
            }
            break;

        //----------------------------------------------------------------
        case CTLR_STA_CTLR_NODE_ID:
        //----------------------------------------------------------------
            switch (evt)
            {
                nd_advt_rpt_t *rpt;

                case EVT_CTLR_NODE_ID:
                    //Stop timer
                    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                    nw->sm_tmr_ctx = NULL;

                    rpt = (nd_advt_rpt_t *)data;

                    //Check whether learn mode resulted in leaving a network or replication/controller shift
                    if (nw->ctlr_sm_ctx.initiate_ctx)
                    {
                        if ((nw->ctlr_sm_ctx.initiate_ctx->homeid == rpt->homeid)
                            && (nw->ctlr_sm_ctx.initiate_ctx->ctlr_node == rpt->node_id))
                        {   //replication or controller shift
                            ALOGI("Learn mode resulted in replication or controller shift");
                        }
                        else
                        {
                            ALOGI("Learn mode resulted in leaving network");

                            zwnet_clear_all(nw);
                        }
                    }

                    //Save the controller node id
                    nw->appl_ctx.ctl_node_id = nw->ctl.nodeid = rpt->node_id;

                    //Update alive status
                    appl_updt_node_sts(&nw->appl_ctx, rpt->node_id, APPL_NODE_STS_UP, 1);

                    //Save the home id
                    nw->homeid = rpt->homeid;

                    //Update network and node info file name
                    if (nw->homeid)
                    {
                        char *file_name;

                        result = zwutl_file_name_get("nif", nw->net_info_dir, nw->homeid, 0, &file_name, 0);
                        if (result == 0)
                        {
                            if (nw->node_info_file)
                            {
                                if (strcmp(nw->node_info_file, file_name) == 0)
                                {   //No  change
                                    free(file_name);
                                }
                                else
                                {   //Update and remove the old file
#if defined(_WINDOWS) || defined(WIN32)
									_unlink(nw->node_info_file);
#else
                                	unlink(nw->node_info_file);
#endif
                                    free(nw->node_info_file);
                                    nw->node_info_file = file_name;
                                }
                            }
                            else
                            {
                                nw->node_info_file = file_name;
                            }
                        }
                    }

                    //Save the node-ip address into the translation table
                    if (nw->appl_ctx.use_ipv4)
                    {
                        zip_node_ipaddr_set(&nw->appl_ctx, &rpt->ipv6_addr[12], 4, rpt->node_id);
                    }
                    else
                    {
                        //Save the controller PAN address (/64 prefix) as PAN network address
                        memcpy(nw->zip_pan, rpt->ipv6_addr, 8);

                        zip_node_ipaddr_set(&nw->appl_ctx, rpt->ipv6_addr, IPV6_ADDR_LEN, rpt->node_id);
                    }

                    //Get Z/IP gateway controller node info
                    result = zip_node_info_chd_get(&nw->appl_ctx, zwnet_ctlr_node_info_cb, nw->ctl.nodeid, ZWNET_NI_CACHE_GET_REFRESH);
                    nw->sm_busy_retry = nw->sm_gen_retry = 0;   //reset retry counts

                    if (result == 0)
                    {
                        //Change state
                        nw->ctlr_sm_sta = CTLR_STA_CTLR_NODE_INFO;

                        //Start timer
                        nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_ctlr_tout_cb, nw);

                        break;
                    }
                    debug_zwapi_msg(&nw->plt_ctx, "Failed to get Z/IP gateway controller node info");

                    //fall through

                case EVT_CTLR_ID_ADDR_FAILED:
                    //Stop timer
                    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                    nw->sm_tmr_ctx = NULL;
                    zwnet_sm_ctlr_cmplt(nw, -1);
                    break;

                case EVT_CTLR_TMOUT:
                    if (nw->sm_gen_retry++ < ZWNET_CTLR_SM_RETRY)
                    {   //Timeout, retry to Get Z/IP gateway controller node id
                        result = zip_node_id_get(&nw->appl_ctx, zwnet_ctlr_id_addr_cb,
                                                 nw->appl_ctx.ssn_ctx.frm_ctx.tpt_ctx.zip_router, 1);

                        if (result == 0)
                        {
                            //Start timer
                            nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_ctlr_tout_cb, nw);

                            break;
                        }
                    }
                    appl_updt_node_sts(&nw->appl_ctx, 0, APPL_NODE_STS_DOWN, 1);
                    zwnet_sm_ctlr_cmplt(nw, -1);
                    break;

                default:
                    break;

            }
            break;

        //----------------------------------------------------------------
        case CTLR_STA_CTLR_NODE_INFO:
        //----------------------------------------------------------------
            switch (evt)
            {
                appl_node_info_t    *ni;

                case EVT_CTLR_NODE_INFO:
                    {
                        zwif_p  ver_intf;

                        //Stop timer
                        plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                        nw->sm_tmr_ctx = NULL;

                        ni = (appl_node_info_t *)data;

                        //Set controller's capabilities
                        if (zwnet_cmd_cls_find(ni->cmd_cls, COMMAND_CLASS_SECURITY_2, ni->cmd_cnt))
                        {
                            nw->ctl_cap |= ZWNET_CTLR_CAP_S2;
                        }

                        if (zwnet_cmd_cls_find(ni->cmd_cls, COMMAND_CLASS_INCLUSION_CONTROLLER, ni->cmd_cnt))
                        {
                            nw->ctl_cap |= ZWNET_CTLR_CAP_INC_ON_BEHALF;
                        }

                        if (zwnet_cmd_cls_find(ni->cmd_cls_sec, COMMAND_CLASS_NODE_PROVISIONING, ni->cmd_cnt_sec)
                            || zwnet_cmd_cls_find(ni->cmd_cls, COMMAND_CLASS_NODE_PROVISIONING, ni->cmd_cnt))
                        {
                            nw->ctl_cap |= ZWNET_CTLR_CAP_SMART_START;
                        }

                        //Save the Z/IP gateway supported command classes
                        free(nw->gw_sup_cmd_cls);
                        nw->gw_sup_cmd_cls = NULL;

                        result = zwnet_gw_sup_cmd_cls_create(ni, &nw->gw_sup_cmd_cls, &nw->gw_sup_cmd_cls_cnt);

                        if (result != 0)
                        {
                            debug_zwapi_msg(&nw->plt_ctx, "zwnet_ctlr_info_sm: Save Z/IP gw supported command class error:%d", result);
                            zwnet_sm_ctlr_cmplt(nw, -1);
                            break;
                        }

                        //Save the controller role
                        nw->ctl_role = zwnet_ctlr_role_get(ni->cmd_cls, ni->cmd_cls_sec, ni->cmd_cnt, ni->cmd_cnt_sec);

                        //Save the device class info
                        nw->ctl.basic = ni->basic;
                        nw->ctl.ep.specific = ni->spec;
                        nw->ctl.ep.generic = ni->gen;
                        if(ni->resrvd == 0x80)
                        {
                           nw->ctl.security_incl_status = 1; 
                        }
                        else if((ni->resrvd != 0x80) && (ni->resrvd != 0))
                        {
                            nw->ctl.security_incl_status = 2;
                        }
                        else if(ni->resrvd == 0)
                        {
                            nw->ctl.security_incl_status = 0;
                        }
                        ALOGI("Ctrl security grant keys %x, security status: %d",ni->resrvd, nw->ctl.security_incl_status);

                        //Rebuild all interfaces
                        zwep_intf_rm_all(&nw->ctl.ep);

                        result = zwnet_ctlr_intf_create(&nw->ctl, ni);

                        if (result != 0)
                        {
                            debug_zwapi_msg(&nw->plt_ctx, "zwnet_ctlr_info_sm: Create Z/IP gw interfaces error:%d", result);
                            zwnet_sm_ctlr_cmplt(nw, -1);
                            break;
                        }

                        //Find version interface
                        ver_intf = zwif_find_cls(nw->ctl.ep.intf, COMMAND_CLASS_VERSION);

                        if (ver_intf)
                        {
                            zwif_p      nxt_intf;

                            nxt_intf = (zwif_p)ver_intf->ep->intf->obj.next;

                            if (nxt_intf)
                            {
                                //Get second command class version, the first one is always basic command class
                                result = zwnet_sm_ctlr_cc_ver_get(nw, ver_intf, nxt_intf->cls);
                                nw->sm_gen_retry = 0;

                                if (result == 0)
                                {
                                    //Change state
                                    nw->ctlr_sm_sta = CTLR_STA_CTLR_VERSION;

                                    //Change sub-state
                                    nw->ctlr_sm_sub_sta = VER_SUBSTA_CC;

                                    break;
                                }
                            }
                        }
                        else
                        {
                            debug_zwapi_msg(&nw->plt_ctx, "zwnet_ctlr_info_sm: Z/IP controller doesn't support version command class");

                            //Job done, callback
                            zwnet_sm_ctlr_cmplt(nw, 0);

                            break;
                        }

                        zwnet_sm_ctlr_cmplt(nw, -1);

                        break;
                    }

                case EVT_CTLR_APPL_STATUS:
                    if (data[0] == APPLICATION_BUSY)
                    {
                        //Stop timer
                        plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                        nw->sm_tmr_ctx = NULL;

                        if (nw->sm_busy_retry++ < ZWNET_CTLR_BUSY_RETRY)
                        {
                            //Start the delay timer
                            nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_APPL_BUSY_RETRY_INTV, zwnet_sm_ctlr_ni_retry_cb, nw);
                        }
                        else
                        {   //Reached maximum retry
                            appl_updt_node_sts(&nw->appl_ctx, 0, APPL_NODE_STS_DOWN, 1);
                            zwnet_sm_ctlr_cmplt(nw, -1);
                        }
                    }
                    break;

                case EVT_CTLR_NODE_INFO_RETRY:
                    {
                        //Get Z/IP gateway controller node info
                        result = zip_node_info_chd_get(&nw->appl_ctx, zwnet_ctlr_node_info_cb, nw->ctl.nodeid, ZWNET_NI_CACHE_GET_REFRESH);

                        if (result == 0)
                        {
                            debug_zwapi_msg(&nw->plt_ctx, "Retrying to get Z/IP gateway controller node info ...");

                            //Start timer
                            nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_ctlr_tout_cb, nw);

                            break;
                        }

                        appl_updt_node_sts(&nw->appl_ctx, 0, APPL_NODE_STS_DOWN, 1);
                        zwnet_sm_ctlr_cmplt(nw, -1);
                    }
                    break;

                case EVT_CTLR_TMOUT:
                    if (nw->sm_gen_retry++ < ZWNET_CTLR_SM_RETRY)
                    {
                        //Start the delay timer
                        nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, 100, zwnet_sm_ctlr_ni_retry_cb, nw);
                        break;
                    }
                    appl_updt_node_sts(&nw->appl_ctx, 0, APPL_NODE_STS_DOWN, 1);
                    zwnet_sm_ctlr_cmplt(nw, -1);
                    break;

                default:
                    break;

            }
            break;

        //----------------------------------------------------------------
        case CTLR_STA_CTLR_VERSION:
        //----------------------------------------------------------------
            switch (evt)
            {
                zwep_p      ep;
                zwif_p      ver_intf;
                zwnode_p    node;

                case EVT_CTLR_CMD_VER_REPORT:
                    {
                        uint16_t    cmd_cls;
                        uint8_t     cmd_cls_ver;

                        //Check sub-state
                        if (nw->ctlr_sm_sub_sta != VER_SUBSTA_CC)
                        {
                            break;
                        }

                        cmd_cls = U8_TO_16BIT(data);
                        cmd_cls_ver = data[2];

                        //Check whether the reported command class matches the request
                        if (cmd_cls != nw->ni_sm_cls)
                        {
                            break;
                        }

                        //Stop timer
                        plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                        nw->sm_tmr_ctx = NULL;

                        memcpy(&ver_intf, data + 4, sizeof(zwif_p));

                        //Check for support of security 2
                        //NOTE: Do not rely on the existance of COMMAND_CLASS_SECURITY_2 as the Z/IP gateway could hide
                        //      it if the controller fails to join another S2 network.
                        if ((cmd_cls == COMMAND_CLASS_NETWORK_MANAGEMENT_INCLUSION) && (cmd_cls_ver > 1))
                        {
                            debug_zwapi_msg(&nw->plt_ctx, "Network Mgmt Inclusion CC version:%u", cmd_cls_ver);

                            nw->ctl_cap |= ZWNET_CTLR_CAP_S2;
                        }
                        //Check if node info cached report contains grant keys
                        else if ((cmd_cls == COMMAND_CLASS_NETWORK_MANAGEMENT_PROXY) && (cmd_cls_ver > 1))
                        {
                            debug_zwapi_msg(&nw->plt_ctx, "Network Mgmt Proxy CC version:%u", cmd_cls_ver);

                            nw->ctl_propty |= CTLR_PROPTY_NIF_GRANT_KEYS;
                        }

                        result = zwnet_sm_ver_rpt_hdlr(nw, &ver_intf, zwnet_ctlr_ver_rpt_cb, cmd_cls, cmd_cls_ver, ZWIF_RPT_GET_ZIP, 1);
                        nw->sm_gen_retry = 0;

                        if (result == 0)
                        {
                            //Start timer
                            nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, (nw->ni_sm_cls == COMMAND_CLASS_BASIC)? ZWNET_SHORT_TMOUT : ZWNET_IP_NW_TMOUT,
                                                           zwnet_sm_ctlr_tout_cb, nw);
                            break;
                        }
                        else if (result == 1)
                        {   //No more command class version to query
                            zwifd_t ver_ifd;

                            if ((nw->ctl_cap & ZWNET_CTLR_CAP_S2) == 0)
                            {   //Invalidate S2 keys if controller doesn't support S2
                                nw->ctl.s2_keys_valid = 0;
                            }

                            //Setup version report callback
                            zwif_get_desc(ver_intf, &ver_ifd);

                            result = zwif_set_report(&ver_ifd, zwnet_zwver_rpt_cb, VERSION_REPORT);
                            if (result == 0)
                            {
                                //Get library, protocol and application versions
                                result = zwif_get_report_ex(&ver_ifd, NULL, 0,
                                                         VERSION_GET, zwif_tx_sts_cb, ZWIF_RPT_GET_ZIP);

                                if (result == 0)
                                {
                                    //Change sub-state
                                    nw->ctlr_sm_sub_sta = VER_SUBSTA_DEVICE;

                                    //Start timer
                                    nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_ctlr_tout_cb, nw);
                                    break;
                                }
                                else
                                {
                                    debug_zwapi_msg(&nw->plt_ctx, "zwnet_ctlr_info_sm: zwif_get_report detailed version with error:%d", result);
                                }
                            }
                        }
                        zwnet_sm_ctlr_cmplt(nw, -1);
                    }
                    break;

                case EVT_CTLR_VER_REPORT:
                    {
                        ext_ver_t   *ext_ver;
                        zwif_p      manf_intf;
                        zwif_p      mailbox_intf;
                        uint16_t    gw_ver;

                        //Check sub-state
                        if (nw->ctlr_sm_sub_sta != VER_SUBSTA_DEVICE)
                        {
                            break;
                        }

                        //Stop timer
                        plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                        nw->sm_tmr_ctx = NULL;

                        memcpy(&ver_intf, data + 6, sizeof(zwif_p));
                        ep = ver_intf->ep;
                        node = ep->node;

                        memcpy(&ext_ver, data + 6 + sizeof(zwif_p), sizeof(ext_ver_t *));

                        node->proto_ver = ((uint16_t)data[0]) << 8 | data[1];
                        node->app_ver = ((uint16_t)data[2]) << 8 | data[3];
                        node->lib_type = data[4];

                        if (ext_ver)
                        {
                            if (node->ext_ver)
                            {
                                free(node->ext_ver);
                            }

                            node->ext_ver = (ext_ver_t *)malloc(sizeof(ext_ver_t) + (ext_ver->fw_cnt*2));
                            if (node->ext_ver)
                            {
                                node->ext_ver->hw_ver = ext_ver->hw_ver;
                                node->ext_ver->fw_cnt = ext_ver->fw_cnt;
                                memcpy(node->ext_ver->fw_ver, ext_ver->fw_ver, (ext_ver->fw_cnt*2));
                            }
                        }

                        //Auto select whether to use multi-client based on Z/IP gateway version
                        if (node->ext_ver && node->ext_ver->fw_cnt)
                        {
#ifdef HARDCODE_GW_VER
                            node->ext_ver->fw_ver[0] = HARDCODE_GW_VER;
#endif
                            if (node->ext_ver->fw_ver[0] >= ZWNET_ZIPGW_VER_MULTI_CLIENT)
                            {   //Turn on multi-client support
                                nw->multi_client = nw->appl_ctx.multi_client = 1;

                                if (!nw->wupoll_ctx)
                                {
                                    //Initialize wakeup notification polling facility
                                    nw->wupoll_ctx = (wupoll_ctx_t *)calloc(1, sizeof(wupoll_ctx_t));

                                    if (!nw->wupoll_ctx)
                                    {
                                        debug_zwapi_msg(&nw->plt_ctx, "Failed to calloc for wakeup notification poll");
                                        zwnet_sm_ctlr_cmplt(nw, -1);
                                        break;
                                    }

                                    nw->wupoll_ctx->net = nw;
                                    result = wkuppoll_init(nw->wupoll_ctx);
                                    if (result != 0)
                                    {
                                        free(nw->wupoll_ctx);
                                        nw->wupoll_ctx = NULL;
                                        debug_zwapi_msg(&nw->plt_ctx, "Init wakeup notification poll facility failed:%d", result);
                                        zwnet_sm_ctlr_cmplt(nw, -1);
                                        break;
                                    }
                                }
                            }

                            gw_ver = node->ext_ver->fw_ver[0];
                            if (gw_ver >= ZWNET_ZIPGW_VER_SEC_HDR_EXT
                                || ((gw_ver >= ZWNET_ZIPGW_VER_HDR_EXT_LO) && (gw_ver <= ZWNET_ZIPGW_VER_HDR_EXT_HI)))
                            {
                                nw->use_encap_hdr = 1;
                            }

                            if (gw_ver >= ZWNET_ZIPGW_VER_EP_NIF)
                            {
                                nw->ctl_propty |= (CTLR_PROPTY_EP_NIF | CTLR_PROPTY_INTVW_CMPLT_STS);
                            }

                            if (gw_ver >= ZWNET_ZIPGW_VER_IMA)
                            {
                                if (zwif_find_cls(nw->ctl.ep.intf, COMMAND_CLASS_NETWORK_MANAGEMENT_INSTALLATION_MAINTENANCE))
                                {
                                    debug_zwapi_msg(&nw->plt_ctx, "Support IMA");

                                    nw->ctl_cap |= ZWNET_CTLR_CAP_IMA;
                                }
                            }

                        }
                        debug_zwapi_msg(&nw->plt_ctx, "Multi-client support : %c",
                                        (nw->multi_client)? 'Y' : 'N');

                        if (!nw->multi_client)
                        {   //Single client mode

                            nw->dflt_wunmi_dly = 2; //Set default wakeup no more info delay

                            //Disable mailbox

                            //Check for mailbox interface
                            mailbox_intf = zwif_find_cls(ep->intf, COMMAND_CLASS_MAILBOX);
                            if (mailbox_intf)
                            {
                                uint8_t     cmd[4];
                                zwifd_t     ifd;

                                cmd[0] = COMMAND_CLASS_MAILBOX;
                                cmd[1] = MAILBOX_CONFIGURATION_SET;
                                cmd[2] = 0;//Disable mailbox

                                //Send the command
                                zwif_get_desc(mailbox_intf, &ifd);
                                zwif_exec(&ifd, cmd, 3, zwif_exec_cb);
                            }
                        }
                        //Check for support of S2 and get Z/IP gateway DSK
                        if (nw->ctl_cap & ZWNET_CTLR_CAP_S2)
                        {
                            if (zw_sec2_dsk_get(&nw->appl_ctx, zwnet_sm_gw_dsk_cb, GW_DSK_TYPE_ADD) == 0)
                            {
                                //Update state-machine's state
                                nw->ctlr_sm_sta = CTLR_STA_CTLR_DSK;

                                //Restart timer
                                plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                                nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_ctlr_tout_cb, nw);

                                break;
                            }
                        }
                        if (!nw->multi_client)
                        {
                            //Check for manufacturer specific interface
                            manf_intf = zwif_find_cls(ep->intf, COMMAND_CLASS_MANUFACTURER_SPECIFIC);
                            if (manf_intf)
                            {
                                if (zwnet_ctlr_sm_manf_get(nw, manf_intf) == 0)
                                {
                                    break;
                                }
                            }

                            //Job done, callback
                            zwnet_sm_ctlr_cmplt(nw, 0);

                            break;
                        }
                        else
                        {
                            //Check for mailbox interface
                            mailbox_intf = zwif_find_cls(ep->intf, COMMAND_CLASS_MAILBOX);
                            if (mailbox_intf)
                            {
                                if (zwnet_ctlr_sm_mbx_get(nw, mailbox_intf) == 0)
                                {
                                    break;
                                }
                            }
                        }
                    }
                    zwnet_sm_ctlr_cmplt(nw, -1);
                    break;

                case EVT_CTLR_TMOUT:
                    if (nw->sm_gen_retry++ < ZWNET_CTLR_SM_RETRY)
                    {
                        zwif_p  ver_intf;
                        zwifd_t ver_ifd;

                        //Find version interface
                        ver_intf = zwif_find_cls(nw->ctl.ep.intf, COMMAND_CLASS_VERSION);

                        if (ver_intf)
                        {
                            if (nw->ctlr_sm_sub_sta == VER_SUBSTA_CC)
                            {   //Get command class version
                                result = zwnet_sm_ctlr_cc_ver_get(nw, ver_intf, nw->ni_sm_cls);
                                if (result == 0)
                                {
                                    break;
                                }
                            }
                            else
                            {
                                zwif_get_desc(ver_intf, &ver_ifd);
                                //Get library, protocol and application versions
                                result = zwif_get_report_ex(&ver_ifd, NULL, 0,
                                                         VERSION_GET, zwif_tx_sts_cb, ZWIF_RPT_GET_ZIP);

                                if (result == 0)
                                {
                                    //Start timer
                                    nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_ctlr_tout_cb, nw);
                                    break;
                                }
                            }
                        }
                    }
                    zwnet_sm_ctlr_cmplt(nw, -1);
                    break;

                default:
                    break;

            }
            break;


        //----------------------------------------------------------------
        case CTLR_STA_CTLR_DSK:
        //----------------------------------------------------------------
            if (!nw->multi_client)
            {   //Non multi-client support
                switch (evt)
                {

                    case EVT_CTLR_DSK_REPORT:
                        {
                            zwif_p      manf_intf;

                            //Stop timer
                            plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                            nw->sm_tmr_ctx = NULL;

                            //Check for manufacturer specific interface
                            manf_intf = zwif_find_cls(nw->ctl.ep.intf, COMMAND_CLASS_MANUFACTURER_SPECIFIC);
                            if (manf_intf)
                            {
                                if (zwnet_ctlr_sm_manf_get(nw, manf_intf) == 0)
                                {
                                    break;
                                }
                            }

                            //Job done, callback
                            zwnet_sm_ctlr_cmplt(nw, 0);

                            break;
                        }

                    case EVT_CTLR_TMOUT:
                        zwnet_sm_ctlr_cmplt(nw, -1);
                        break;

                    default:
                        break;

                }
                break;
            }
            else
            {   //Multi-client support
                switch (evt)
                {

                    case EVT_CTLR_DSK_REPORT:
                        {
                            zwif_p      mailbox_intf;

                            //Stop timer
                            plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                            nw->sm_tmr_ctx = NULL;

                            //Check for mailbox interface
                            mailbox_intf = zwif_find_cls(nw->ctl.ep.intf, COMMAND_CLASS_MAILBOX);
                            if (mailbox_intf)
                            {
                                if (zwnet_ctlr_sm_mbx_get(nw, mailbox_intf) == 0)
                                {
                                    break;
                                }
                            }
                        }

                        //fall through

                    case EVT_CTLR_TMOUT:
                        zwnet_sm_ctlr_cmplt(nw, -1);
                        break;

                    default:
                        break;

                }
                break;
            }

        //----------------------------------------------------------------
        case CTLR_STA_MAILBOX:
        //----------------------------------------------------------------
            switch (evt)
            {
                zwep_p      ep;
                zwif_p      mbx_intf;

                case EVT_CTLR_MAILBOX_CFG_REPORT:
                    {
                        mailbox_cfg_t   *mbx_cfg;
                        zwif_p          manf_intf;

                        //Stop timer
                        plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                        nw->sm_tmr_ctx = NULL;

                        memcpy(&mbx_cfg, data, sizeof(mailbox_cfg_t *));
                        memcpy(&mbx_intf, data + sizeof(mailbox_cfg_t *), sizeof(zwif_p));
                        ep = mbx_intf->ep;

                        debug_zwapi_msg(&nw->plt_ctx, "Mailbox: supported mode=%u, current mode=%u, capacity=%u",
                                        mbx_cfg->supp_mode, mbx_cfg->mode, mbx_cfg->frame_cap);

                        //Check to ensure mailbox service is enabled
                        if (mbx_cfg->mode != MBX_MODE_SERVICE)
                        {
                            if (mbx_cfg->supp_mode & MBX_SUPP_BMSK_SERVICE)
                            {   //Enable mailbox service

                                uint8_t     cmd[20];
                                zwifd_t     ifd;

                                cmd[0] = COMMAND_CLASS_MAILBOX;
                                cmd[1] = MAILBOX_CONFIGURATION_SET;
                                cmd[2] = MBX_MODE_SERVICE;
                                memset(cmd + 3, 0, IPV6_ADDR_LEN + 1);

                                //Send the command
                                zwif_get_desc(mbx_intf, &ifd);
                                zwif_exec_ex(&ifd, cmd, 20, zwif_exec_cb, NULL, ZWIF_OPT_SECURE, NULL);
                            }
                            else
                            {
                                zwnet_sm_ctlr_cmplt(nw, -1);
                                break;
                            }
                        }

                        //Check for manufacturer specific interface
                        manf_intf = zwif_find_cls(ep->intf, COMMAND_CLASS_MANUFACTURER_SPECIFIC);
                        if (manf_intf)
                        {
                            if (zwnet_ctlr_sm_manf_get(nw, manf_intf) == 0)
                            {
                                break;
                            }
                        }

                        //Job done, callback
                        zwnet_sm_ctlr_cmplt(nw, 0);

                        break;
                    }

                case EVT_CTLR_TMOUT:
                    zwnet_sm_ctlr_cmplt(nw, -1);
                    break;

                default:
                    break;

            }
            break;
        //----------------------------------------------------------------
        case CTLR_STA_CTLR_MANF_SPEC:
        //----------------------------------------------------------------
            {
                zwep_p  ep;

                ep = NULL;

                if (evt == EVT_CTLR_MANF_SPEC_REPORT)
                {
                    zwif_p      intf;
                    zwnode_p    node;
                    uint16_t    *manf_pdt_id;

                    memcpy(&intf, data + 6, sizeof(zwif_p));
                    ep = intf->ep;
                    node = ep->node;

                    //Check sub-state
                    if (nw->ctlr_sm_sub_sta != MS_SUBSTA_PDT_ID)
                    {
                        break;
                    }

                    //Stop timer
                    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                    nw->sm_tmr_ctx = NULL;

                    //Copy manufacturer id, product type id and product id to the node
                    manf_pdt_id = (uint16_t *)data;
                    node->vid = manf_pdt_id[0];
                    node->vtype = manf_pdt_id[1];
                    node->pid = manf_pdt_id[2];

                    //Check if version 2 is supported
                    if (intf->ver >= 2)
                    {
                        //Get device serial number
                        if (zwnet_ctlr_sm_dev_id_get(nw, intf, DEV_ID_TYPE_OEM) == 0)
                            break;
                    }

                    //Job done, callback
                    zwnet_sm_ctlr_cmplt(nw, 0);
                }
                else if (evt == EVT_CTLR_DEVICE_ID_REPORT)
                {
                    zwif_p      intf;
                    zwnode_p    node;
                    dev_id_t    *dev_id;

                    memcpy(&dev_id, data, sizeof(dev_id_t *));
                    memcpy(&intf, data + sizeof(dev_id_t *), sizeof(zwif_p));
                    ep = intf->ep;
                    node = ep->node;

                    //Check sub-state
                    if (nw->ctlr_sm_sub_sta != MS_SUBSTA_DEV_ID_OEM)
                    {
                        break;
                    }

                    //Stop timer
                    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                    nw->sm_tmr_ctx = NULL;

                    if (dev_id->type != DEV_ID_TYPE_OEM)
                    {
                        //Save the device id
                        node->dev_id = *dev_id;
                    }

                    //Job done, callback
                    zwnet_sm_ctlr_cmplt(nw, 0);
                }
                else if (evt == EVT_CTLR_TMOUT)
                {
                    zwnet_sm_ctlr_cmplt(nw, -1);
                }
            }
            break;
    }

    return 0;
}


/**
zwnet_ip_aton - Convert IPv4 / IPv6 address string to numeric equivalent
@param[in]  addr_str     Null terminated IPv4 / IPv6 address string
@param[out] addr_buf     Buffer that should be at least 16-byte long to store the result
@param[out] ipv4         Flag to indicate the converted numeric IP address is IPv4 or IPv6. 1=IPv4; 0=IPv6.
@return     Zero if successful; otherwise non-zero
*/
int zwnet_ip_aton(char *addr_str, uint8_t *addr_buf, int *ipv4)
{
    return util_ip_aton(addr_str, addr_buf, ipv4);
}


/**
zwnet_ip_ntoa - Convert IPv4 / IPv6 address in numerical form to string equivalent
@param[in]  addr         IPv4 / IPv6 address in numerical form
@param[out] addr_str     Buffer to store the converted address string
@param[in]  addr_str_len The size of the addr_str in bytes
@param[in]  ipv4         Flag to indicate the addr parameter is IPv4 or IPv6. 1=IPv4; 0=IPv6.
@return     Zero if successful; otherwise non-zero
*/
int zwnet_ip_ntoa(uint8_t *addr, char *addr_str, int addr_str_len, int ipv4)
{
    return util_ip_ntoa(addr, addr_str, addr_str_len, ipv4);
}


/**
zwnet_local_addr_get - Get local address that is reachable by destination host
@param[in]	net	        network
@param[in]  dest_ip     Destination host address either IPv4 or IPv6 according to use_ipv4 flag
@param[in]  use_ipv4    Flag to indicate IP address type. 1= IPv4; 0= IPv6
@param[out] local_ip    Buffer of 16-byte to store the local address (either IPv4 or IPv6 according to use_ipv4 flag)
@return     Zero if successful; otherwise non-zero
*/
int zwnet_local_addr_get(zwnet_p net, uint8_t *dest_ip, uint8_t *local_ip, int use_ipv4)
{
#ifdef TCP_PORTAL
    if (use_ipv4)
    {   //Portal doesn't support IPv4 address
        return -1;
    }
    memcpy(local_ip, net->init.portal_prof.svr_ipv6_addr, IPV6_ADDR_LEN);
    return 0;
#else
    //IPv6, use the listening address
    if (!use_ipv4
        && (memcmp(net->zip_router_addr, dest_ip, IPV6_ADDR_LEN) == 0))
    {
        memcpy(local_ip, net->appl_ctx.ssn_ctx.frm_ctx.tpt_ctx.listen_addr, IPV6_ADDR_LEN);
        return 0;
    }

    return util_local_addr_get(dest_ip, local_ip, use_ipv4);
#endif
}


/**
zwnet_listen_port_get - Get local Z/IP listening port number
@param[in]	net	        network
@return     The Z/IP client listening port number
@pre  Must call zwnet_init() before calling this function
*/
uint16_t zwnet_listen_port_get(zwnet_p net)
{
    return net->appl_ctx.ssn_ctx.frm_ctx.tpt_ctx.host_port;
}


/**
zwnet_gw_discvr_start - Start Z/IP gateway discovery
@param[in]	    cb		    Callback function when the gateway discovery has completed
@param[in]	    usr_param	User defined parameter used in callback function
@param[in]	    ipv4	    Flag to indicate whether to use IPv4 as transport IP protocol. 1= use IPv4; 0= use IPv6
@param[in]	    use_mdns	Flag to indicate whether to use MDNS for gateway discovery.  Note: MDNS gateway discovery
                            is only supported in Z/IP gateway version 2
@return Context on success, null on failure.
@post   Caller is required to call zwnet_gw_discvr_stop() with the returned context if it is not null.
*/
void *zwnet_gw_discvr_start(zwnet_gw_discvr_cb_t cb, void *usr_param, int ipv4, int use_mdns)
{
#ifdef ZIP_V2
    if (use_mdns)
    {
        return mdns_gw_discvr_start((mdns_gw_discvr_cb_t)cb, usr_param, ipv4);
    }
    else
#endif
    return util_gw_discvr_start((util_gw_discvr_cb_t)cb, usr_param, ipv4);
}


/**
zwnet_gw_discvr_stop - Stop Z/IP gateway discovery
@param[in]	    ctx		    The context returned from the call to zwnet_gw_discvr_start()
@return Zero on success, non-zero on failure.
@post After the call, the ctx is invalid and should not be used
*/
int zwnet_gw_discvr_stop(void *ctx)
{
#ifdef ZIP_V2
    int ret;

    //Try MDNS stop first
    ret = mdns_gw_discvr_stop(ctx);

    return (ret == -2)? util_gw_discvr_stop(ctx) : ret;

#else
    return util_gw_discvr_stop(ctx);
#endif
}


/**
handle_association - Handler function for Association CC and AGI CC
@param[in]	cmd_len	    Length of the incoming command buffer
@param[in]	cmd_buf		Incoming command buffer
@param[in]	prm			Application command param context
@return     Zero on success; -1 if ASSOCIATION_SET failed; -2 if ASSOCIATION_REMOVE failed.
*/
static int handle_association(uint16_t cmd_len, uint8_t *cmd_buf, appl_cmd_prm_t *prm)
{
    zwnet_p     nw  = prm->nw;
    uint8_t     src_node = prm->src_node;
    uint8_t     msg_type = prm->msg_type;
    uint16_t    *encap_format = (prm->encap_fmt_valid)? &prm->encap_fmt : NULL;
    frm_ep_t    *ep_addr = prm->ep_addr;
    uint8_t     send_buf[46] = {0};
    uint8_t     i = 0;
    int         result = 0;
    frm_ep_t    rpt_ep;

    //Prepare the endpoint address
    rpt_ep.src_ep = 0;
    rpt_ep.dst_ep_type = 0;
    rpt_ep.dst_ep = ep_addr->src_ep;

    switch (cmd_buf[0])
    {
        case COMMAND_CLASS_ASSOCIATION:
            {
                //A receiving node MUST NOT return a response to a GET command if it is received via multicast addressing
                if (msg_type & ZWNET_MSG_TYPE_MULTICAST)
                {
                    if ((cmd_buf[1] == ASSOCIATION_GET)
                        || (cmd_buf[1] == ASSOCIATION_SPECIFIC_GROUP_GET_V2)
                        || (cmd_buf[1] == ASSOCIATION_GROUPINGS_GET))
                    {
                        return 0;
                    }
                }

                switch (cmd_buf[1])
                {
                    case ASSOCIATION_SET:
                        {
                            if (cmd_len >= 4)
                            {
                                //Since only support 1 node, just fill in the one node. If there are more nodes, ignore.
                                AGI_Group *pMatchAGIGroup = nw->stAGIData.pAGIGroupList;

                                //Ignore the incoming command if it is not for Group ID 1.
                                if (cmd_buf[2] != nw->stAGIData.pAGIGroupList[0].byGroupID)
                                {
                                    debug_zwapi_msg(&nw->plt_ctx, "Group ID %d not supported", cmd_buf[2]);
                                    return -1;
                                }

                                if (pMatchAGIGroup->stNodeEpInfo.byNodeID == 0)
                                {   //Empty, assign new node id
                                    pMatchAGIGroup->stNodeEpInfo.byNodeID = cmd_buf[3];
                                    pMatchAGIGroup->stNodeEpInfo.byEpID = 0;
                                }
                                else if (pMatchAGIGroup->stNodeEpInfo.byNodeID != cmd_buf[3])
                                {
                                    return -1;
                                }
                            }
                            else
                            {
                                return -1;
                            }
                        }
                        break;

                    case ASSOCIATION_GET:
                        {
                            if (cmd_len >= 3)
                            {
                                AGI_Group *pMatchAGIGroup = nw->stAGIData.pAGIGroupList;

                                //If an "Association Get" is received, a report MUST be returned.
                                //A node that receives an unsupported Grouping Identifier SHOULD return information relating to GroupID 1.

                                send_buf[i++] = cmd_buf[0];
                                send_buf[i++] = ASSOCIATION_REPORT;
                                send_buf[i++] = pMatchAGIGroup->byGroupID;
                                send_buf[i++] = pMatchAGIGroup->byMaxNodes;
                                send_buf[i++] = 0; //Report to follow

                                if (pMatchAGIGroup->stNodeEpInfo.byNodeID != 0)
                                    send_buf[i++] = pMatchAGIGroup->stNodeEpInfo.byNodeID;

                                if (prm->tot_mul_cmd == 0)
                                {
                                    result = zwnet_rpt_send(nw, send_buf, i, src_node, &rpt_ep, msg_type, encap_format);
                                    if (result < 0)
                                        debug_zwapi_msg(&nw->plt_ctx, "Association ASSOCIATION_REPORT: Send rpt with error:%d", result);
                                }
                                else
                                {
                                    zwnet_mul_cmd_resp_add(nw, send_buf, i, 0, 0, MUL_CMD_TYPE_RPT);
                                }
                            }
                        }
                        break;

                    case ASSOCIATION_REMOVE:
                        {
                            if (cmd_len >= 3)
                            {
                                //Group ID can be 0 in this command
                                AGI_Group *pMatchAGIGroup = nw->stAGIData.pAGIGroupList;

                                //If no node list is supplied or the node ID is matched, remove the node ID.
                                if (cmd_len == 3)
                                {
                                    //group ID >= 1 && Number of node ID's in list 0
                                    //group ID = 0 && Number of node ID's in list 0
                                    if (cmd_buf[2] == pMatchAGIGroup->byGroupID || cmd_buf[2] == 0)
                                        pMatchAGIGroup->stNodeEpInfo.byNodeID = 0;  //Remove the node
                                    else
                                    {
                                        debug_zwapi_msg(&nw->plt_ctx, "Group ID %d not supported", cmd_buf[2]);
                                        return -2;
                                    }
                                }
                                else if (cmd_len >= 4)
                                {
                                    //group ID = 0 && Number of node ID's in list match record
                                    //group ID match record && Number of node ID's in list match record
                                    if (cmd_buf[2] == pMatchAGIGroup->byGroupID || cmd_buf[2] == 0)
                                    {
                                        if (cmd_buf[3] == pMatchAGIGroup->stNodeEpInfo.byNodeID)
                                            pMatchAGIGroup->stNodeEpInfo.byNodeID = 0;  //Remove the node
                                        else
                                        {
                                            debug_zwapi_msg(&nw->plt_ctx, "Node ID not listed:%d", cmd_buf[3]);
                                            return -2;
                                        }
                                    }
                                    else
                                    {
                                        debug_zwapi_msg(&nw->plt_ctx, "Group ID not supported:%d", cmd_buf[2]);
                                        return -2;
                                    }
                                }
                            }
                            else
                            {
                                return -2;
                            }
                        }
                        break;

                    case ASSOCIATION_GROUPINGS_GET:
                        {
                            send_buf[i++] = cmd_buf[0];
                            send_buf[i++] = ASSOCIATION_GROUPINGS_REPORT;
                            send_buf[i++] = nw->stAGIData.byNofGroups;

                            if (prm->tot_mul_cmd == 0)
                            {
                                result = zwnet_rpt_send(nw, send_buf, i, src_node, &rpt_ep, msg_type, encap_format);
                                if (result < 0)
                                    debug_zwapi_msg(&nw->plt_ctx, "Association ASSOCIATION_GROUPINGS_REPORT: Send rpt with error:%d", result);
                            }
                            else
                            {
                                zwnet_mul_cmd_resp_add(nw, send_buf, i, 0, 0, MUL_CMD_TYPE_RPT);
                            }

                        }
                        break;

                    case ASSOCIATION_SPECIFIC_GROUP_GET_V2:
                        {
                            AGI_Group *pMatchAGIGroup = nw->stAGIData.pAGIGroupList;
                            send_buf[i++] = cmd_buf[0];
                            send_buf[i++] = ASSOCIATION_SPECIFIC_GROUP_REPORT_V2;
                            send_buf[i++] = pMatchAGIGroup->byGroupID;

                            if (prm->tot_mul_cmd == 0)
                            {
                                result = zwnet_rpt_send(nw, send_buf, i, src_node, &rpt_ep, msg_type, encap_format);
                                if (result < 0)
                                    debug_zwapi_msg(&nw->plt_ctx, "Association ASSOCIATION_SPECIFIC_GROUP_REPORT: Send rpt with error:%d", result);
                            }
                            else
                            {
                                zwnet_mul_cmd_resp_add(nw, send_buf, i, 0, 0, MUL_CMD_TYPE_RPT);
                            }

                        }
                        break;
                }
            }
            break;

        case COMMAND_CLASS_ASSOCIATION_GRP_INFO:
            {
                //A receiving node MUST NOT return a response to a GET command if it is received via multicast addressing
                if (msg_type & ZWNET_MSG_TYPE_MULTICAST)
                {
                    return 0;
                }

                switch (cmd_buf[1])
                {
                    case ASSOCIATION_GROUP_NAME_GET:
                        {
                            if (cmd_len >= 3)
                            {
                                size_t  name_len;

                                //If an "Association Group Name Get" is received, a report MUST be returned.
                                //A node that receives an unsupported Grouping Identifier SHOULD return information relating to GroupID 1.

                                send_buf[i++] = cmd_buf[0];
                                send_buf[i++] = ASSOCIATION_GROUP_NAME_REPORT;
                                send_buf[i++] = nw->stAGIData.pAGIGroupList[0].byGroupID;
                                name_len = strlen(nw->stAGIData.pAGIGroupList[0].cchGroupName);
                                send_buf[i++] = name_len;
                                memcpy(send_buf + i, nw->stAGIData.pAGIGroupList[0].cchGroupName, name_len);

                                i += name_len;

                                if (prm->tot_mul_cmd == 0)
                                {
                                    result = zwnet_rpt_send(nw, send_buf, i, src_node, &rpt_ep, msg_type, encap_format);
                                    if (result < 0)
                                        debug_zwapi_msg(&nw->plt_ctx, "AGI ASSOCIATION_GROUP_NAME_REPORT: Send rpt with error:%d", result);
                                }
                                else
                                {
                                    zwnet_mul_cmd_resp_add(nw, send_buf, i, 0, 0, MUL_CMD_TYPE_RPT);
                                }
                            }
                        }
                        break;

                    case ASSOCIATION_GROUP_INFO_GET:
                        {
                            if (cmd_len >= 4)
                            {
                                uint8_t   byListMode = cmd_buf[2] & ASSOCIATION_GROUP_INFO_GET_PROPERTIES1_LIST_MODE_BIT_MASK;
                                AGI_Group *pMatchAGIGroup = nw->stAGIData.pAGIGroupList;

                                //If List Mode is set to 1, a receiving node MUST ignore the Grouping Identifier field and return the
                                //properties of all its supported associations groups.
                                //If List Mode is set to 0, a receiving node MUST advertise the properties of the association group
                                //identified by the Grouping Identifier.

                                //If an "Association Group Info Get" is received, a report MUST be returned.
                                //A node that receives an unsupported Grouping Identifier SHOULD return information relating to GroupID 1.

                                send_buf[i++] = cmd_buf[0];
                                send_buf[i++] = ASSOCIATION_GROUP_INFO_REPORT;
                                //on return report: Listmode = byListMode (shift bit 6 to bit 7), dynamic info = 0, group count = 1
                                send_buf[i++] = (byListMode << 1) | nw->stAGIData.byNofGroups;

                                send_buf[i++] = pMatchAGIGroup->byGroupID;

                                send_buf[i++] = 0; //Mode = 0
                                send_buf[i++] = (uint8_t)((pMatchAGIGroup->wProfile & 0xFF00) >> 8);//MSB of Profile
                                send_buf[i++] = (uint8_t)(pMatchAGIGroup->wProfile & 0xFF);//LSB of Profile
                                send_buf[i++] = 0; //Reserved
                                send_buf[i++] = (uint8_t)((pMatchAGIGroup->wEventCode & 0xFF00) >> 8);
                                send_buf[i++] = (uint8_t)(pMatchAGIGroup->wEventCode & 0xFF);

                                if (prm->tot_mul_cmd == 0)
                                {
                                    result = zwnet_rpt_send(nw, send_buf, i, src_node, &rpt_ep, msg_type, encap_format);
                                    if (result < 0)
                                        debug_zwapi_msg(&nw->plt_ctx, "AGI ASSOCIATION_GROUP_INFO_REPORT: Send rpt with error:%d", result);
                                }
                                else
                                {
                                    zwnet_mul_cmd_resp_add(nw, send_buf, i, 0, 0, MUL_CMD_TYPE_RPT);
                                }
                            }
                        }
                        break;

                    case ASSOCIATION_GROUP_COMMAND_LIST_GET:
                        {
                            if (cmd_len >= 4)
                            {
                                uint8_t j = 0;
                                AGI_Group *pMatchAGIGroup = nw->stAGIData.pAGIGroupList;

                                //If an "Association Group Command List Get" is received, a report MUST be returned.
                                //A node that receives an unsupported Grouping Identifier SHOULD return information relating to GroupID 1.

                                send_buf[i++] = cmd_buf[0];
                                send_buf[i++] = ASSOCIATION_GROUP_COMMAND_LIST_REPORT;
                                send_buf[i++] = pMatchAGIGroup->byGroupID;
                                j = i++;
                                send_buf[i++] = pMatchAGIGroup->stCCList.CCMSB;

                                if (pMatchAGIGroup->stCCList.CCLSB != 0)
                                    send_buf[i++] = pMatchAGIGroup->stCCList.CCLSB;

                                send_buf[i++] = pMatchAGIGroup->stCCList.CCID;

                                send_buf[j] = i - j - 1;

                                if (prm->tot_mul_cmd == 0)
                                {
                                    result = zwnet_rpt_send(nw, send_buf, i, src_node, &rpt_ep, msg_type, encap_format);
                                    if (result < 0)
                                        debug_zwapi_msg(&nw->plt_ctx, "AGI ASSOCIATION_GROUP_COMMAND_LIST_REPORT: Send rpt with error:%d", result);
                                }
                                else
                                {
                                    zwnet_mul_cmd_resp_add(nw, send_buf, i, 0, 0, MUL_CMD_TYPE_RPT);
                                }
                            }
                        }
                        break;
                }
            }
            break;
    }
    return 0;
}


/**
zwnet_pan_prefix_get - Get PAN prefix
@param[in]	net	        Network
@param[out] pan_prefix  Buffer to store the PAN prefix, it must be at least 16 bytes in size
@param[out] prefix_len  PAN prefix length
@return     ZW_ERR_XXX
*/
int zwnet_pan_prefix_get(zwnet_p net, uint8_t *pan_prefix, uint8_t *prefix_len)
{
    if (util_ipv6_zero_cmp(net->zip_pan) == 0)
    {
        return ZW_ERR_UNSUPPORTED;
    }

    memcpy(pan_prefix, net->zip_pan, IPV6_ADDR_LEN);
    *prefix_len = 64;

    return 0;
}


/**
zwnet_node_sts_get - Get node status
@param[in]	net		        Network
@param[in]	node_id		    Node id (ranging from 1 to 232)
@return node status (ZWNET_NODE_STS_XXX)
*/
uint8_t zwnet_node_sts_get(zwnet_p net, uint8_t node_id)
{
    return appl_get_node_sts(&net->appl_ctx, node_id);

}


/**
zwnet_all_node_sts_get - Get all node status
@param[in]	net		        Network
@param[out]	node_sts_buf	Buffer (min. size of 233 bytes)to store all the node status. Individual node status
                            (ZWNET_NODE_STS_XXX) can be access using the node id as index to the buffer.
@return
*/
void zwnet_all_node_sts_get(zwnet_p net, uint8_t *node_sts_buf)
{
    appl_get_all_node_sts(&net->appl_ctx, node_sts_buf);
}


/**
zwnet_sm_nc_tout_cb - Network change state machine timer callback
@param[in] data     Pointer to network
@return
*/
static void zwnet_sm_nc_tout_cb(void *data)
{
    zwnet_p nw = (zwnet_p)data;

    //Stop timer
    plt_mtx_lck(nw->mtx);
    plt_tmr_stop(&nw->plt_ctx, nw->nc_sm_ctx.tmr_ctx);
    nw->nc_sm_ctx.tmr_ctx = NULL;//make sure timer context is null, else restart timer will crash
    zwnet_nw_change_sm(nw, EVT_NC_TMOUT, NULL);
    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_sm_nc_retry_cb - Network change state machine retry callback
@param[in] data     Pointer to network
@return
*/
static void zwnet_sm_nc_retry_cb(void *data)
{
    zwnet_p nw = (zwnet_p)data;

    //Stop timer
    plt_mtx_lck(nw->mtx);
    plt_tmr_stop(&nw->plt_ctx, nw->nc_sm_ctx.tmr_ctx);
    nw->nc_sm_ctx.tmr_ctx = NULL;//make sure timer context is null, else restart timer will crash
    zwnet_nw_change_sm(nw, EVT_NC_START_RETRY, NULL);
    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_nc_node_info_cb - Request controller node info callback function
@param[in]	appl_ctx    The application layer context
@param[in]	cached_ni   Cached node information
@return
*/
static void zwnet_nc_node_info_cb(appl_layer_ctx_t *appl_ctx, appl_node_info_t *cached_ni)
{
    zwnet_t *nw = (zwnet_t *)appl_ctx->data;
    int cmd_cls_exist = 0;

    if (cached_ni->cmd_cnt > 0)
    {
        debug_zwapi_msg(&nw->plt_ctx, "unsecure command classes:");
        zwnet_cmd_cls_show(nw, cached_ni->cmd_cls, cached_ni->cmd_cnt);
        cmd_cls_exist = 1;

    }

    if (cached_ni->cmd_cnt_sec > 0)
    {
        debug_zwapi_msg(&nw->plt_ctx, "secure command classes:");
        zwnet_cmd_cls_show(nw, cached_ni->cmd_cls_sec, cached_ni->cmd_cnt_sec);
        cmd_cls_exist = 1;

    }

    if (cmd_cls_exist)
    {
#ifdef MAP_IP_ASSOC_TO_ASSOC
        zwnet_ip_assoc_map(cached_ni);
#endif
        //Update the network data structure
        plt_mtx_lck(nw->mtx);
        zwnet_nw_change_sm(nw, EVT_NC_CTLR_INFO, (uint8_t *)cached_ni);
        plt_mtx_ulck(nw->mtx);

    }
}


/**
zwnet_unsolic_node_list_cb - Unsolicited node list callback
@param[in]	appl_ctx            The application layer context
@param[in]	sts                 Status of previous call to "get node list"
@param[in]	node_list_ctlr_id   The node id of the controller which keeps the latest updated node list
@param[in]	node_list           Bitmask of nodes. The first bit in the bitmap represents node ID 1;
                                the last bit represents node ID 232
@param[in]	nl_len              Length of node_list in bytes
@return
*/
static void zwnet_unsolic_node_list_cb(appl_layer_ctx_t *appl_ctx, uint8_t sts,
                                   uint8_t node_list_ctlr_id, uint8_t *node_list, uint8_t nl_len)
{
    const char *status_msg[] = { "OK", "No guarantee that returned node list is latest update",
                                 "unknown"
                               };
    zwnet_t *nw = (zwnet_t *)appl_ctx->data;
    int     status;
    uint8_t new_node_list[240]; //First byte is the number of newly added node, the rest are node ids.

    if (nl_len > 29)
    {   //Maximum node list length is 29 bytes
        nl_len = 29;
    }

    new_node_list[0] = nl_len;
    memcpy(new_node_list + 1, node_list, nl_len);


    status = (sts > 1)? 2 : sts;

    debug_zwapi_ts_msg(&nw->plt_ctx, "Unsolicited node list status:%u(%s), suc:%u", sts, status_msg[status], node_list_ctlr_id);

    //Save SUC/SIS id
    nw->nlist_ctl_id = node_list_ctlr_id;

    if (nl_len >= 29)
    {
        char        num_str[256];
        unsigned    i, j;

        debug_zwapi_msg(&nw->plt_ctx, "node id:");

        num_str[0] = '\0';
        for (i = j = 0; i < 232; i++)
        {
            if (((node_list[i>>3]) >> (i & 7)) & 0x01)
            {
                util_num_string_add(num_str, 256, (unsigned)(i+1));
                if (j++ > 30)
                {
                    debug_zwapi_msg(&nw->plt_ctx, "%s", num_str);
                    j = 0;
                    num_str[0] = '\0';
                }
            }
        }
        debug_zwapi_msg(&nw->plt_ctx, "%s", num_str);
    }

    //Call state-machine
    plt_mtx_lck(nw->mtx);
    zwnet_nw_change_sm(nw, EVT_NC_START, new_node_list);
    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_nw_change_sm - Network change state-machine
@param[in]	nw		Network
@param[in] evt      The event for the state-machine
@param[in] data     The data associated with the event
@return             0 if the state-machine is started from idle state; else return non-zero
@pre    Caller must lock the nw->mtx before calling this function
*/
static void zwnet_nw_change_failed(zwnet_p nw)
{
    nc_sm_ctx_t *ctx = &nw->nc_sm_ctx;

    //At least, update the node list before failing the state-machine
    zwnet_node_list_update(nw, ctx->new_node_list, ctx->new_node_num, 1, NULL, NULL);
    appl_updt_node_sts(&nw->appl_ctx, 0, APPL_NODE_STS_DOWN, 1);
    ctx->sta = NC_STA_IDLE;
    nw->curr_op = ZWNET_OP_NONE;
    //Callback to report completion of the network update operation
    zwnet_notify(nw, ZWNET_OP_NW_CHANGED, OP_FAILED, NULL);

}


/**
zwnet_nw_change_sm - Network change state-machine
@param[in]	nw		Network
@param[in] evt      The event for the state-machine
@param[in] data     The data associated with the event
@return             0 if the state-machine is started from idle state; else return non-zero
@pre    Caller must lock the nw->mtx before calling this function
*/
static int    zwnet_nw_change_sm(zwnet_p nw, zwnet_nc_evt_t evt, uint8_t *data)
{
    int             result;
    nc_sm_ctx_t     *ctx = &nw->nc_sm_ctx;

    switch (ctx->sta)
    {
        //----------------------------------------------------------------
        case NC_STA_IDLE:
        //----------------------------------------------------------------
            if (evt == EVT_NC_START)
            {   //Initialize
                plt_tmr_stop(&nw->plt_ctx, ctx->tmr_ctx);
                ctx->tmr_ctx = NULL;
                ctx->retry_cnt = 2;

                if (data)
                {   //Save new node list
                    ctx->new_node_num = (data[0] <= 232)? data[0] : 232;
                    memcpy(ctx->new_node_list, data + 1, ctx->new_node_num);
                }

                if (nw->curr_op != ZWNET_OP_NONE)
                {
                    //Cancel low priority operation
                    if (nw->curr_op == ZWNET_OP_NODE_CACHE_UPT)
                    {
                        zwnet_abort(nw);
                        debug_zwapi_msg(&nw->plt_ctx, "Canceled low priority operation");
                    }
                    else
                    {
                        debug_zwapi_msg(&nw->plt_ctx, "Current operation not completed yet, retrying ...");
                        ctx->tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_NW_CHANGE_RETRY_INTV, zwnet_sm_nc_retry_cb, nw);
                        //Change state
                        ctx->sta = NC_STA_RETRY;
                        return 0;//state-machine started successfully
                    }
                }

                //Get Z/IP gateway controller node info
                result = zip_node_info_chd_get(&nw->appl_ctx, zwnet_nc_node_info_cb, nw->ctl.nodeid, ZWNET_NI_CACHE_GET_REFRESH);

                if (result == 0)
                {
                    //Set network operation
                    nw->curr_op = ZWNET_OP_NW_CHANGED;

                    //Change state
                    ctx->sta = NC_STA_CTLR_INFO;

                    //Start timer
                    ctx->tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_nc_tout_cb, nw);

                    return 0;//state-machine started successfully
                }
                else if ((result == SESSION_ERROR_PREVIOUS_COMMAND_UNCOMPLETED) || (result == APPL_ERROR_WAIT_CB))
                {   //Handle corner case : device poll for node X is on-going and at the same time node X is removed
                    debug_zwapi_msg(&nw->plt_ctx, "Send Z/IP info cache get err, retrying ...");
                    ctx->tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_NW_CHANGE_RETRY_INTV, zwnet_sm_nc_retry_cb, nw);
                    //Change state
                    ctx->sta = NC_STA_RETRY;
                    return 0;//state-machine started successfully
                }

                debug_zwapi_msg(&nw->plt_ctx, "zwnet_nw_change_sm: failed to get Z/IP gateway controller info, err=%d", result);
            }
            break;

        //----------------------------------------------------------------
        case NC_STA_RETRY:  //NOTE: Network operation is still not in ZWNET_OP_NW_CHANGED
        //----------------------------------------------------------------
            if (evt == EVT_NC_START_RETRY)
            {
                if (nw->curr_op != ZWNET_OP_NONE)
                {
                    //Cancel low priority operation
                    if (nw->curr_op == ZWNET_OP_NODE_CACHE_UPT)
                    {
                        zwnet_abort(nw);
                        debug_zwapi_msg(&nw->plt_ctx, "Canceled low priority operation");
                    }
                    else if (nw->curr_op == ZWNET_OP_NW_CHANGED)
                    {
                        //Retry due to failure in NC_STA_NODE_LIST
                    }
                    else
                    {
                        debug_zwapi_msg(&nw->plt_ctx, "Current operation not completed yet, retrying ...");
                        ctx->tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_NW_CHANGE_RETRY_INTV, zwnet_sm_nc_retry_cb, nw);
                        break;
                    }
                }

                //Get a fresh copy of nodelist since it might be obsolete
                result = zip_node_list_get(&nw->appl_ctx, zwnet_unsolic_node_list_cb);

                if (result == 0)
                {
                    //Set network operation
                    nw->curr_op = ZWNET_OP_NW_CHANGED;

                    //Change state
                    ctx->sta = NC_STA_NODE_LIST;

                    //Start timer
                    ctx->tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_nc_tout_cb, nw);

                    break;
                }
                else if ((result == SESSION_ERROR_PREVIOUS_COMMAND_UNCOMPLETED) || (result == APPL_ERROR_WAIT_CB))
                {
                    if (ctx->retry_cnt > 0)
                    {
                        debug_zwapi_msg(&nw->plt_ctx, "Get fresh node list err, retrying ...");
                        ctx->retry_cnt--;
                        ctx->tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_NW_CHANGE_RETRY_INTV, zwnet_sm_nc_retry_cb, nw);
                        break;
                    }
                }

                debug_zwapi_msg(&nw->plt_ctx, "zwnet_nw_change_sm: failed to get node list, err=%d", result);
                zwnet_nw_change_failed(nw);

            }
            break;

        //----------------------------------------------------------------
        case NC_STA_NODE_LIST:
        //----------------------------------------------------------------
            if (evt == EVT_NC_START)
            {   //Initialize
                plt_tmr_stop(&nw->plt_ctx, ctx->tmr_ctx);
                ctx->tmr_ctx = NULL;

                if (data)
                {   //Save new node list
                    ctx->new_node_num = (data[0] <= 232)? data[0] : 232;
                    memcpy(ctx->new_node_list, data + 1, ctx->new_node_num);
                }

                //Get Z/IP gateway controller node info
                result = zip_node_info_chd_get(&nw->appl_ctx, zwnet_nc_node_info_cb, nw->ctl.nodeid, ZWNET_NI_CACHE_GET_REFRESH);

                if (result == 0)
                {
                    //Change state
                    ctx->sta = NC_STA_CTLR_INFO;

                    //Start timer
                    ctx->tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_nc_tout_cb, nw);

                    break;
                }
                else if ((result == SESSION_ERROR_PREVIOUS_COMMAND_UNCOMPLETED) || (result == APPL_ERROR_WAIT_CB))
                {   //Handle corner case : device poll for node X is on-going and at the same time node X is removed
                    if (ctx->retry_cnt > 0)
                    {
                        debug_zwapi_msg(&nw->plt_ctx, "Send Z/IP info cache get err, retrying ...");
                        ctx->retry_cnt--;
                        ctx->sta = NC_STA_RETRY;
                        ctx->tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_NW_CHANGE_RETRY_INTV, zwnet_sm_nc_retry_cb, nw);
                        break;
                    }
                }

                debug_zwapi_msg(&nw->plt_ctx, "zwnet_nw_change_sm: failed to get Z/IP gateway controller info, err=%d", result);
                zwnet_nw_change_failed(nw);

            }
            else if (evt == EVT_NC_TMOUT)
            {
                if (ctx->retry_cnt > 0)
                {
                    debug_zwapi_msg(&nw->plt_ctx, "Get fresh node list timeout retry count = %u", ctx->retry_cnt);
                    ctx->retry_cnt--;

                    //Get fresh node list
                    result = zip_node_list_get(&nw->appl_ctx, zwnet_unsolic_node_list_cb);

                    if (result == 0)
                    {
                        //Start timer
                        ctx->tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_nc_tout_cb, nw);
                        break;
                    }
                    else if ((result == SESSION_ERROR_PREVIOUS_COMMAND_UNCOMPLETED) || (result == APPL_ERROR_WAIT_CB))
                    {
                        debug_zwapi_msg(&nw->plt_ctx, "Get fresh node list err, retrying ...");
                        ctx->sta = NC_STA_RETRY;
                        ctx->tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_NW_CHANGE_RETRY_INTV, zwnet_sm_nc_retry_cb, nw);
                        break;
                    }
                }
                zwnet_nw_change_failed(nw);
            }
            break;

        //----------------------------------------------------------------
        case NC_STA_CTLR_INFO:
        //----------------------------------------------------------------
            switch (evt)
            {
                case EVT_NC_CTLR_INFO:
                    {
                        appl_node_info_t    *ni;
                        uint8_t tmp_node_list[240];
                        uint8_t tmp_node_num;


                        //Stop timer
                        plt_tmr_stop(&nw->plt_ctx, ctx->tmr_ctx);
                        ctx->tmr_ctx = NULL;

                        ni = (appl_node_info_t *)data;

                        //Save the Z/IP gateway supported command classes
                        free(nw->gw_sup_cmd_cls);
                        nw->gw_sup_cmd_cls = NULL;

                        result = zwnet_gw_sup_cmd_cls_create(ni, &nw->gw_sup_cmd_cls, &nw->gw_sup_cmd_cls_cnt);

                        if (result != 0)
                        {
                            debug_zwapi_msg(&nw->plt_ctx, "zwnet_nw_change_sm: Save Z/IP gw supported command class error:%d", result);
                        }

                        //Update controller role
                        nw->ctl_role = zwnet_ctlr_role_get(ni->cmd_cls, ni->cmd_cls_sec, ni->cmd_cnt, ni->cmd_cnt_sec);

                        //Update controller z-wave role
                        nw->zwave_role = zwnet_zw_role_get(nw->ctl_role, nw->nlist_ctl_id, nw->ctl.nodeid);

                        if ((nw->zwave_role == ZW_ROLE_SIS) && (ctx->sis_status == 0))
                        {
                            ctx->sis_status = 1;
                        }

                        result = zwnet_node_list_update(nw, ctx->new_node_list, ctx->new_node_num,
                                                        1, tmp_node_list, &tmp_node_num);

                        if (result != 0)
                        {
                            debug_zwapi_msg(&nw->plt_ctx, "zwnet_node_list_update with error:%d", result);
                        }

                        ctx->new_node_num = tmp_node_num;
                        memcpy(ctx->new_node_list, tmp_node_list, tmp_node_num);

                        //Discover the node-ip address information of newly added nodes
                        if (ctx->new_node_num)
                        {
                            n2ip_sm_ctx_t   n2ip_sm_ctx = {0};

                            //Retry on INFORMATION_OBSOLETE because of Z/IP gateway timing issue
                            n2ip_sm_ctx.retry_on_obsolete = 1;

                            //Create the node id list
                            n2ip_sm_ctx.num_node = ctx->new_node_num;
                            memcpy(n2ip_sm_ctx.node_id, ctx->new_node_list, n2ip_sm_ctx.num_node);

                            //Call the resolve node to ip address state-machine
                            n2ip_sm_ctx.cb = zwnet_n2ip_lst_cb;
                            n2ip_sm_ctx.params[0] = (void *)(unsigned)ZWNET_OP_NW_CHANGED;

                            result = zwnet_n2ip_sm(nw, EVT_N2IP_START, (uint8_t *)&n2ip_sm_ctx);

                            if (result == 0)
                            {
                                //Change state
                                ctx->sta = NC_STA_NODE_IP_DSCVRY;
                                break;
                            }
                            else
                            {
                                debug_zwapi_msg(&nw->plt_ctx, "zwnet_nw_change_sm resolve node id with error:%d", result);
                            }

                        }
                        else if (ctx->sis_status == 1)
                        {   //Controller has just been assigned SIS role
                            uint8_t dat = 0;
                            ctx->sta = NC_STA_NODE_IP_DSCVRY;
                            zwnet_nw_change_sm(nw, EVT_NC_NODE_ID_ADDR, &dat);
                            break;

                        }
                        //Done
                        ctx->sta = NC_STA_IDLE;
                        nw->curr_op = ZWNET_OP_NONE;
                        //Callback to report completion of the network update operation
                        zwnet_notify(nw, ZWNET_OP_NW_CHANGED, OP_DONE, NULL);

                    }
                    break;

                case EVT_NC_TMOUT:
                    if (ctx->retry_cnt > 0)
                    {
                        debug_zwapi_msg(&nw->plt_ctx, "Send Z/IP info cache get retry count = %u", ctx->retry_cnt);
                        ctx->retry_cnt--;

                        //Get Z/IP gateway controller node info
                        result = zip_node_info_chd_get(&nw->appl_ctx, zwnet_nc_node_info_cb, nw->ctl.nodeid, ZWNET_NI_CACHE_GET_REFRESH);

                        if (result == 0)
                        {
                            //Start timer
                            ctx->tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_nc_tout_cb, nw);
                            break;
                        }
                        else if ((result == SESSION_ERROR_PREVIOUS_COMMAND_UNCOMPLETED) || (result == APPL_ERROR_WAIT_CB))
                        {   //Handle corner case : device poll for node X is on-going and at the same time node X is removed
                            debug_zwapi_msg(&nw->plt_ctx, "Send Z/IP info cache get err, retrying ...");
                            ctx->tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, zwnet_sm_nc_tout_cb, nw);
                            ctx->retry_cnt++;   //Don't count this as retry
                            break;
                        }
                    }
                    zwnet_nw_change_failed(nw);
                    break;

                default:
                    break;

            }
            break;

        //----------------------------------------------------------------
        case NC_STA_NODE_IP_DSCVRY:
        //----------------------------------------------------------------
            if (evt == EVT_NC_NODE_ID_ADDR)
            {
                if (*data == 0)
                {
                    zwnode_p  node;
                    sm_job_t  sm_job = {0};
                    uint8_t   node_id[256];

                    //Node info update
                    sm_job.op = ZWNET_OP_NW_CHANGED;
                    sm_job.num_node = 0;
                    sm_job.node_id = node_id;
                    sm_job.cb = zwnet_node_info_cmplt_cb;

                    if (ctx->sis_status == 1)
                    {
                        //Controller has just been assigned SIS role, get node info and assign life-line
                        ctx->sis_status = 2;
                        sm_job.auto_cfg = 1;

                        node = (zwnode_p)nw->ctl.obj.next;
                        while (node)
                        {
                            //Add node without interface or "always on" node to the query list
                            if (!node->ep.intf || !node->sleep_cap)
                            {
                                node_id[sm_job.num_node++] = node->nodeid;
                            }
                            node = (zwnode_p)node->obj.next;
                        }
                    }
                    else
                    {   //Check whether a node has been added
                        if (ctx->new_node_num)
                        {
                            sm_job.num_node = ctx->new_node_num;
                            sm_job.node_id = ctx->new_node_list;
                        }
                    }

                    if (sm_job.num_node)
                    {
                        if (zwnet_node_info_sm(nw, EVT_GET_NODE_INFO, (uint8_t *)&sm_job))
                        {
                            //Change state
                            ctx->sta = NC_STA_NODE_INFO_UPDT;

                            break;
                        }
                    }
                }
                else
                {
                    //Resolve node to ip address failed
                    debug_zwapi_msg(&nw->plt_ctx, "Failed to get node PAN address");
                }
                //Done
                ctx->sta = NC_STA_IDLE;
                nw->curr_op = ZWNET_OP_NONE;
                //Callback to report completion of the network update operation
                zwnet_notify(nw, ZWNET_OP_NW_CHANGED, OP_DONE, NULL);

            }
            break;

        //----------------------------------------------------------------
        case NC_STA_NODE_INFO_UPDT:
        //----------------------------------------------------------------
            if (evt == EVT_NC_NODE_INFO_DONE)
            {
                //Reset to idle state
                ctx->sta = NC_STA_IDLE;
                nw->curr_op = ZWNET_OP_NONE;
            }
            break;
    }
    return -1;
}

/**
@}
*/

#ifdef TCP_PORTAL
/**
@defgroup TLSutil TLS Network Utility APIs
Utilities to communicate securely using TLS
@ingroup zwarecapi
@{
*/

/**
zwportal_init - Start portal to listen for secure TLS connection
@param[in]	    init_prm	Portal initialization parameters
@return Context on success, null on failure.
@post   Caller is required to call zwportal_shutdown() and zwportal_exit() with the returned context if it is not null.
*/
void *zwportal_init(zwportal_init_t *init_prm)
{
    //Sanity check for consistency
    if (sizeof(clnt_prof_t) != sizeof(tls_clnt_prof_t))
    {
        return NULL;
    }

    return tls_svr_start((tls_clnt_prof_t *)init_prm->clnt_prof, init_prm->prof_cnt,
                         (tls_svr_cb_t)init_prm->cb, init_prm->usr_param,
                         init_prm->ca_file, init_prm->ssl_file, init_prm->pvt_key_file,
                         init_prm->svr_port);
}


/**
zwportal_shutdown - Shutdown portal listening socket
@param[in]	    ctx		    The context returned from the call to zwportal_init()
@return Zero on success, non-zero on failure.
@post   Caller is required to call zwportal_exit() to free up resources.
*/
int zwportal_shutdown(void *ctx)
{
    return tls_svr_shutdown(ctx);
}


/**
zwportal_exit - Stop portal and free up resources
@param[in]	    ctx		    The context returned from the call to zwportal_init()
@return Zero on success, non-zero on failure.
@post After the call, the ctx is invalid and should not be used
*/
int zwportal_exit(void *ctx)
{
    return tls_svr_stop(ctx);
}


/**
zwportal_clnt_conn_close - Close client's socket fd and SSL
@param[in]	    sfd	 Client socket fd
@param[in]	    ssl	 Client SSL object pointer
@return
*/
void zwportal_clnt_conn_close(int sfd, void *ssl)
{
    tls_conn_close(sfd, ssl);
}


/**
zwportal_clnt_add - Add a client profile to the portal internal list, overwriting old entry with the same gateway id.
@param[in]	    ctx		    The context returned from the call to zwportal_init()
@param[in]	    clnt_prof	Client profile
@return                     Return non-zero on success, zero on failure.
*/
int zwportal_clnt_add(void *ctx, clnt_prof_t *clnt_prof)
{
    return tls_clnt_add(ctx, (tls_clnt_prof_t *)clnt_prof);
}


/**
zwportal_clnt_rm - Remove a client profile from the portal internal list
@param[in]	    ctx		    The context returned from the call to zwportal_init()
@param[in]	    gw_id	    Gateway id
@return                     Return non-zero on success, zero on failure.
*/
int zwportal_clnt_rm(void *ctx, uint8_t *gw_id)
{
    return tls_clnt_rm(ctx, gw_id);
}


/**
zwportal_clnt_find - Find a client profile in the portal internal list based on gateway id.
@param[in]	    ctx		    The context returned from the call to zwportal_init()
@param[in,out]	clnt_prof	Client profile buffer with the gateway id set as input key to be searched.
                            On success, this buffer will be returned with found entry.
@return                     Return non-zero on success, zero on failure.
*/
int zwportal_clnt_find(void *ctx, clnt_prof_t *clnt_prof)
{
    return tls_clnt_find(ctx, (tls_clnt_prof_t *)clnt_prof);
}


/**
zwportal_clnt_list_free - Free client profiles list
@param[in]	    lst_hd		List head of the client profiles list
@return
*/
void zwportal_clnt_list_free(clnt_prof_lst *lst_hd)
{
    tls_clnt_list_free((tls_clnt_prof_lst *)lst_hd);
}


/**
zwportal_clnt_list_get - Get all the client profiles in the portal internal list
@param[in]	    ctx		    The context returned from the call to zwportal_init()
@param[out]	    lst_hd		List head of the client profiles list
@return                     Number of client profiles in the list; negative value on failure.
@post Caller must call zwportal_clnt_list_free() to free the linked-list if return value is greater than zero.
*/
int zwportal_clnt_list_get(void *ctx, clnt_prof_lst **lst_hd)
{
    return tls_clnt_list_get(ctx, (tls_clnt_prof_lst **)lst_hd);
}

/**
@}
*/
#endif
/**
@defgroup If_Poll Polling Interface APIs
Used to create and delete polling commands to a device
@ingroup zwarecapi
@{
*/


/**
zwnet_poll_rm - remove a polling request
@param[in]	net	        network
@param[in]	handle	    handle of the polling request to remove
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/
int zwnet_poll_rm(zwnet_p net, uint16_t handle)
{
    return zwpoll_rm(net, handle);
}


/**
zwnet_poll_rm_mul - remove multiple polling requests
@param[in]	net	        network
@param[in]	usr_token	usr_token of the polling requests to remove
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/
int zwnet_poll_rm_mul(zwnet_p net, uint32_t usr_token)
{
    return zwpoll_rm_mul(net, usr_token);
}

/**
@}
@defgroup NW_Pref_storage Preference storage APIs
Used to store and retrieve preferences
@{
*/


/**
zwnet_pref_set - Store network preference into persistent storage
@param[in]	net		    Network
@param[in]	buf		    Buffer that contains the network preference
@param[in]	buf_size	Buffer size in bytes
@return		ZW_ERR_XXX
@see		zwnet_pref_get
*/
int zwnet_pref_set(zwnet_p net, void *buf, uint16_t buf_size)
{
    return zwutl_pref_set(net, buf, buf_size, 0, 0);
}


/**
zwnet_pref_get - Retrieve network preference from persistent storage
@param[in]	net		    Network
@param[out]	buf		    Return buffer that contains the network preference
@param[out]	buf_size	Buffer size in bytes
@return		ZW_ERR_XXX
@post       Caller MUST free the return buffer "buf" if the call is successful.
@see		zwnet_pref_set
*/
int zwnet_pref_get(zwnet_p net, void **buf, uint16_t *buf_size)
{
    return zwutl_pref_get(net, 0, buf, buf_size, 0);
}


/**
zwnet_client_pref_set - Store client preference into persistent storage
@param[in]	net		    Network
@param[in]	client_id	User-defined client id to identify the preference
@param[in]	buf		    Buffer that contains the client preference
@param[in]	buf_size	Buffer size in bytes
@return		ZW_ERR_XXX
@see		zwnet_client_pref_get
*/
int zwnet_client_pref_set(zwnet_p net, uint32_t client_id, void *buf, uint16_t buf_size)
{
    return zwutl_pref_set(net, buf, buf_size, client_id, 1);
}


/**
zwnet_client_pref_get - Retrieve client preference from persistent storage
@param[in]	net		    Network
@param[in]	client_id	User-defined client id to identify the preference
@param[out]	buf		    Return buffer that contains the client preference
@param[out]	buf_size	Buffer size in bytes
@return		ZW_ERR_XXX
@post       Caller MUST free the return buffer "buf" if the call is successful.
@see		zwnet_client_pref_set
*/
int zwnet_client_pref_get(zwnet_p net, uint32_t client_id, void **buf, uint16_t *buf_size)
{
    return zwutl_pref_get(net, client_id, buf, buf_size, 1);
}


/**
zwnet_unsolic_node_add_cb - Unsolicited add node status callback function
@param[in]	appl_ctx    The application layer context
@param[in]	cb_prm		Add node callback parameters
@return
*/
static void zwnet_unsolic_node_add_cb(appl_layer_ctx_t *appl_ctx, appl_node_add_cb_prm_t *cb_prm)
{
    zwnet_t *nw = (zwnet_t *)appl_ctx->data;
    uint8_t rpt_type;

    //NOTE: This function may be called when current operation is neither add node nor replace node. See zwnet_node_add_sec2_cb()

    if (!(nw->ctl_cap & ZWNET_CTLR_CAP_INC_ON_BEHALF))
    {   //Only controller with inclusion on-behalf capability will receive unsolicited add node status
        return;
    }

    rpt_type = cb_prm->rpt_type;

    if ((rpt_type == NODE_ADD_STATUS) || (rpt_type == FAILED_NODE_REPLACE_STATUS)
        || (rpt_type == NODE_ADD_DSK_REPORT_V2) || (rpt_type == NODE_ADD_KEYS_REPORT_V2))
    {
        plt_mtx_lck(nw->mtx);
        if (!((nw->curr_op == ZWNET_OP_ADD_NODE || nw->curr_op == ZWNET_OP_RP_NODE) && nw->incl_on_behalf))
        {
            //Cancel low priority operation
            if (nw->curr_op == ZWNET_OP_NODE_CACHE_UPT)
            {
                zwnet_abort(nw);
                debug_zwapi_msg(&nw->plt_ctx, "Canceled low priority operation");
            }

            //No other operation, enter into inclusion on-behalf mode
            if (nw->curr_op == ZWNET_OP_NONE)
            {
                nw->curr_op = (rpt_type == FAILED_NODE_REPLACE_STATUS)? ZWNET_OP_RP_NODE : ZWNET_OP_ADD_NODE;
                nw->incl_on_behalf = IOB_UNSOLICITED;
                debug_zwapi_msg(&nw->plt_ctx, "Enter into inclusion on-behalf or Smart Start mode ...");
            }
            else    //Other operation in progress, delay processing ...
            {
                if (rpt_type == NODE_ADD_STATUS)
                {
                    zwnet_exec_req_t        add_node_sts_req = {0};
                    zwnet_exec_req_addS2_t  *add_req;
                    uint16_t                *tmp_cls;
                    appl_node_info_t        *node_info;

                    debug_zwapi_msg(&nw->plt_ctx, "Current operation not completed yet, delay processing of inclusion on-behalf");
                    plt_mtx_ulck(nw->mtx);

                    add_req = (zwnet_exec_req_addS2_t *)malloc(sizeof(zwnet_exec_req_addS2_t));
                    if (!add_req)
                    {
                        return;
                    }

                    add_req->rcx_flag = cb_prm->flag;
                    add_req->add_dat = cb_prm->rpt_prm.node_info;

                    node_info = &add_req->add_dat.node_info;

                    if (node_info->cmd_cnt)
                    {
                        tmp_cls = node_info->cmd_cls;
                        node_info->cmd_cls = (uint16_t *)malloc(node_info->cmd_cnt * sizeof(uint16_t));
                        if (node_info->cmd_cls)
                        {
                            memcpy(node_info->cmd_cls, tmp_cls, node_info->cmd_cnt * sizeof(uint16_t));
                        }
                        else
                        {   //Failed memory allocation
                            node_info->cmd_cnt = 0;
                        }
                    }

                    if (node_info->cmd_cnt_sec)
                    {
                        tmp_cls = node_info->cmd_cls_sec;
                        node_info->cmd_cls_sec = (uint16_t *)malloc(node_info->cmd_cnt_sec * sizeof(uint16_t));
                        if (node_info->cmd_cls_sec)
                        {
                            memcpy(node_info->cmd_cls_sec, tmp_cls, node_info->cmd_cnt_sec * sizeof(uint16_t));
                        }
                        else
                        {   //Failed memory allocation
                            node_info->cmd_cnt_sec = 0;
                        }
                    }

                    add_node_sts_req.node_id = node_info->node_id;
                    add_node_sts_req.action = EXEC_ACT_ADD_NODE_STS_CB;
                    add_node_sts_req.user_dat = add_req;

                    util_list_add(nw->nw_exec_mtx, &nw->nw_exec_req_hd,
                                  (uint8_t *)&add_node_sts_req, sizeof(zwnet_exec_req_t));
                    plt_sem_post(nw->nw_exec_sem);

                    return;
                }

                if (rpt_type == FAILED_NODE_REPLACE_STATUS)
                {
                    zwnet_exec_req_t        rplc_node_sts_req = {0};
                    zwnet_exec_req_rpS2_t   *rplc_req;

                    debug_zwapi_msg(&nw->plt_ctx, "Current operation not completed yet, delay processing of replace on-behalf");
                    plt_mtx_ulck(nw->mtx);

                    rplc_req = (zwnet_exec_req_rpS2_t *)malloc(sizeof(zwnet_exec_req_rpS2_t));
                    if (!rplc_req)
                    {
                        return;
                    }

                    rplc_req->rcx_flag = cb_prm->flag;
                    rplc_req->rp_dat = cb_prm->rpt_prm.node_rp_info;

                    rplc_node_sts_req.node_id = rplc_req->rp_dat.node_id;
                    rplc_node_sts_req.action = EXEC_ACT_RPLC_NODE_STS_CB;
                    rplc_node_sts_req.user_dat = rplc_req;

                    util_list_add(nw->nw_exec_mtx, &nw->nw_exec_req_hd,
                                  (uint8_t *)&rplc_node_sts_req, sizeof(zwnet_exec_req_t));
                    plt_sem_post(nw->nw_exec_sem);

                    return;
                }
            }
        }
        plt_mtx_ulck(nw->mtx);
    }

    zwnet_node_add_rplc_hdlr(appl_ctx, cb_prm);

}


/**
zwnet_unsolic_ss_started_cb - Unsolicited Smart Start join started callback function
@param[in]	appl_ctx    The application layer context
@param[in]	dsk         DSK
@return
*/
static void zwnet_unsolic_ss_started_cb(appl_layer_ctx_t *appl_ctx, zw_dsk_t *dsk)
{
    zwnet_t     *nw = (zwnet_t *)appl_ctx->data;
    char        *dsk_str;
    zwnet_sts_t op_sts = {0};

    dsk_str = zwutil_dsk_to_str(dsk->dsk, dsk->dsk_len);

    if (!dsk_str)
    {
        debug_zwapi_msg(&nw->plt_ctx, "zwnet_unsolic_ss_started_cb: invalid dsk key");
        return;
    }

    debug_zwapi_msg(&nw->plt_ctx, "zwnet_unsolic_ss_started_cb: dsk key:%s", dsk_str);

    op_sts.type = ZWNET_STS_INFO_SS_START;
    strncpy(op_sts.info.s2_dsk, dsk_str, MAX_DSK_STR_LEN + 1);
    zwnet_notify(nw, ZWNET_OP_ADD_NODE, OP_ADD_NODE_PROTOCOL_START, &op_sts);

    free(dsk_str);

}


/**
zwnet_unsolic_inif_cb - Unsolicited included node information frame (INIF) callback function
@param[in]	appl_ctx    The application layer context
@param[in]	dsk         DSK
@return
*/
static void zwnet_unsolic_inif_cb(appl_layer_ctx_t *appl_ctx, zw_dsk_t *dsk)
{
    zwnet_p         nw = (zwnet_p)appl_ctx->data;
    char            *dsk_str;

    dsk_str = zwutil_dsk_to_str(dsk->dsk, dsk->dsk_len);

    if (!dsk_str)
    {
        debug_zwapi_msg(&nw->plt_ctx, "zwnet_unsolic_inif_cb: invalid dsk key");
        return;
    }

    debug_zwapi_msg(&nw->plt_ctx, "zwnet_unsolic_inif_cb: dsk key:%s", dsk_str);

    plt_mtx_lck(nw->mtx);
    if (nw->inif_dsk)
    {
        free(nw->inif_dsk);
    }

    //Save the DSK
    nw->inif_dsk = dsk_str;
    plt_mtx_ulck(nw->mtx);

}


/**
zwnet_unsolic_pl_cb - Unsolicited provisioning list entry information callback
@param[in]	appl_ctx    The application layer context
@param[in]	dsk         DSK
@param[in]	info		Buffer to store additional information of the device (optional).
@param[in]	info_cnt    Number of additional information stored in "info".
@return
*/
static void zwnet_unsolic_pl_cb(appl_layer_ctx_t *appl_ctx, zw_dsk_t *dsk, pl_info_t *info, uint8_t info_cnt)
{
    zwnet_p             nw = (zwnet_p)appl_ctx->data;
    char                *dsk_str;
    pl_info_fn          cb;
    zwnet_cb_req_t      cb_req = {0};
    zwnet_inif_cb_req_t *inif = &cb_req.param.inif;

    cb = nw->init.inif_cb;

    if (!cb || (dsk->dsk_len != MAX_DSK_KEY_LEN))
    {
        return;
    }

    dsk_str = zwutil_dsk_to_str(dsk->dsk, dsk->dsk_len);

    if (!dsk_str)
    {
        debug_zwapi_msg(&nw->plt_ctx, "zwnet_unsolic_pl_cb: invalid dsk key");
        return;
    }

    debug_zwapi_msg(&nw->plt_ctx, "zwnet_unsolic_pl_cb: dsk key:%s", dsk_str);

    plt_mtx_lck(nw->mtx);

    if (!nw->inif_dsk)
    {
        plt_mtx_ulck(nw->mtx);
        debug_zwapi_msg(&nw->plt_ctx, "No previous INIF found, ignore it");
        free(dsk_str);
        return;
    }


    if (strcmp(nw->inif_dsk, dsk_str) != 0)
    {
        plt_mtx_ulck(nw->mtx);
        debug_zwapi_msg(&nw->plt_ctx, "Previous INIF dsk '%s' is different", nw->inif_dsk);
        free(dsk_str);
        return;
    }

    strncpy(inif->lst_ent.dsk, dsk_str, MAX_DSK_STR_LEN);
    inif->lst_ent.dsk[MAX_DSK_STR_LEN] = '\0';
    free(dsk_str);
    free(nw->inif_dsk);
    nw->inif_dsk = NULL;

    plt_mtx_ulck(nw->mtx);


    inif->lst_ent.info_cnt = info_cnt;

    if (info_cnt)
    {
        int i;

        if (info_cnt > PL_MAX_INFO_TYPE)
        {
            inif->lst_ent.info_cnt = info_cnt = PL_MAX_INFO_TYPE;
        }

        for (i=0; i<info_cnt; i++)
        {
            inif->lst_ent.info[i] = info[i];
        }
    }

    cb_req.type = CB_TYPE_INIF;
    cb_req.cb = cb;
    inif->user = nw->init.user;

    util_list_add(nw->cb_mtx, &nw->cb_req_hd,
                  (uint8_t *)&cb_req, sizeof(zwnet_cb_req_t));
    plt_sem_post(nw->cb_sem);

}


/**
zwnet_pl_add - Add a provisioning list entry
@param[in]	net		    Network
@param[in]	dsk		    Device Specific Key (DSK). The format of the DSK must be 8 groups of 5 digits separated by '-' as shown in the example:
                        34028-23669-20938-46346-33746-07431-56821-14553
@param[in]	info		Buffer to store additional information of the device (optional). Info type PL_INFO_TYPE_NW_STS must not be
                        used in this function.
@param[in]	info_cnt    Number of additional information stored in "info".
@return		ZW_ERR_XXX
*/
int zwnet_pl_add(zwnet_p net, char *dsk, pl_info_t *info, uint8_t info_cnt)
{
    uint8_t     dsk_bin[MAX_DSK_KEY_LEN];
    uint8_t     dsk_len;
    int         res;

    if (!(net->ctl_cap & ZWNET_CTLR_CAP_SMART_START))
    {
        return ZW_ERR_UNSUPPORTED;
    }

    if (!dsk || !zwutil_str_to_dsk(dsk, dsk_bin, &dsk_len))
    {
        return ZW_ERR_VALUE;
    }

    res = zw_pl_set(&net->appl_ctx, dsk_bin, dsk_len, info, info_cnt);

    if (res < 0)
    {
        debug_zwapi_msg(&net->plt_ctx, "zwnet_pl_add error:%d", res);

        res = ZW_ERR_FAILED;
    }

    return res;
}


/**
zwnet_pl_get_cb - Provisioning list entry information report callback
@param[in]	appl_ctx    The application layer context
@param[in]	dsk         DSK
@param[in]	info		Buffer to store additional information of the device (optional).
@param[in]	info_cnt    Number of additional information stored in "info".
@return
*/
static void zwnet_pl_get_cb(appl_layer_ctx_t *appl_ctx, zw_dsk_t *dsk, pl_info_t *info, uint8_t info_cnt)
{
    zwnet_p         nw = (zwnet_p)appl_ctx->data;
    char            *dsk_str;
    pl_info_fn      cb;
    void            *usr_ctx;
    pl_lst_ent_t    pl_ent;
    //zwnet_cb_req_t  cb_req;

    plt_mtx_lck(nw->mtx);

    cb = nw->pl_ctx.info_cb;
    usr_ctx = nw->pl_ctx.info_usr_ctx;

    plt_mtx_ulck(nw->mtx);

    if (!cb || (dsk->dsk_len != MAX_DSK_KEY_LEN))
    {
        return;
    }

    dsk_str = zwutil_dsk_to_str(dsk->dsk, dsk->dsk_len);

    if (!dsk_str)
    {
        debug_zwapi_msg(&nw->plt_ctx, "zwnet_pl_get_cb: invalid dsk key");
        return;
    }

    strncpy(pl_ent.dsk, dsk_str, MAX_DSK_STR_LEN);
    pl_ent.dsk[MAX_DSK_STR_LEN] = '\0';
    free(dsk_str);

    pl_ent.info_cnt = info_cnt;
    if (info_cnt)
    {
        int i;

        if (info_cnt > PL_MAX_INFO_TYPE)
        {
            pl_ent.info_cnt = info_cnt = PL_MAX_INFO_TYPE;
        }

        for (i=0; i<info_cnt; i++)
        {
            pl_ent.info[i] = info[i];
        }

    }

    cb(usr_ctx, &pl_ent);

//    cb_req.type = CB_TYPE_GW_DSK;
//    cb_req.cb = nw->dsk_cb;
//    cb_req.param.gw_dsk.user = nw->dsk_cb_prm;
//#ifdef USE_SAFE_VERSION
//    strncpy_s(cb_req.param.gw_dsk.dsk, 48, dsk_str, 47);
//#else
//    strncpy(cb_req.param.gw_dsk.dsk, dsk_str, 47);
//#endif
//    cb_req.param.gw_dsk.dsk[47] = '\0';
//
//    util_list_add(nw->cb_mtx, &nw->cb_req_hd,
//                  (uint8_t *)&cb_req, sizeof(zwnet_cb_req_t));
//    plt_sem_post(nw->cb_sem);
//
//    free(dsk_str);
}


/**
zwnet_pl_get - Get a provisioning list entry information through callback
@param[in]	net		    Network
@param[in]	dsk		    Device Specific Key (DSK). The format of the DSK must be 8 groups of 5 digits separated by '-' as shown in the example:
                        34028-23669-20938-46346-33746-07431-56821-14553
@param[in]	cb          Callback to report provisioning list entry information
@param[in]	usr_ctx     User context used in callback
@return		ZW_ERR_XXX
*/
int zwnet_pl_get(zwnet_p net, char *dsk, pl_info_fn cb, void *usr_ctx)
{
    uint8_t     dsk_bin[MAX_DSK_KEY_LEN];
    uint8_t     dsk_len;
    int         res;

    if (!(net->ctl_cap & ZWNET_CTLR_CAP_SMART_START))
    {
        return ZW_ERR_UNSUPPORTED;
    }

    if (!dsk || !zwutil_str_to_dsk(dsk, dsk_bin, &dsk_len))
    {
        return ZW_ERR_VALUE;
    }

    plt_mtx_lck(net->mtx);

    net->pl_ctx.info_cb = cb;
    net->pl_ctx.info_usr_ctx = usr_ctx;

    res = zw_pl_get(&net->appl_ctx, dsk_bin, dsk_len, zwnet_pl_get_cb);

    if (res < 0)
    {
        debug_zwapi_msg(&net->plt_ctx, "zw_pl_get error:%d", res);

        res = ZW_ERR_FAILED;
    }

    plt_mtx_ulck(net->mtx);

    return res;
}


/**
zwnet_pl_del - Delete a provisioning list entry
@param[in]	net		    Network
@param[in]	dsk		    Device Specific Key (DSK). The format of the DSK must be 8 groups of 5 digits separated by '-' as shown in the example:
                        34028-23669-20938-46346-33746-07431-56821-14553
@return		ZW_ERR_XXX
*/
int zwnet_pl_del(zwnet_p net, char *dsk)
{
    uint8_t     dsk_bin[MAX_DSK_KEY_LEN];
    uint8_t     dsk_len;
    int         res;

    if (!(net->ctl_cap & ZWNET_CTLR_CAP_SMART_START))
    {
        return ZW_ERR_UNSUPPORTED;
    }

    if (!dsk || !zwutil_str_to_dsk(dsk, dsk_bin, &dsk_len))
    {
        return ZW_ERR_VALUE;
    }

    res = zw_pl_del(&net->appl_ctx, dsk_bin, dsk_len);

    if (res < 0)
    {
        debug_zwapi_msg(&net->plt_ctx, "zwnet_pl_del error:%d", res);

        res = ZW_ERR_FAILED;
    }

    return res;
}


/**
zwnet_pl_list_get - Get all provisioning list entries through callback
@param[in]	net		    Network
@param[in]	cb          Callback to report provisioning list entries
@param[in]	usr_ctx     User context used in callback
@return		ZW_ERR_XXX
*/
int zwnet_pl_list_get(zwnet_p net, pl_list_fn cb, void *usr_ctx)
{
    int     res;
    uint8_t data[32];

    if (!(net->ctl_cap & ZWNET_CTLR_CAP_SMART_START))
    {
        return ZW_ERR_UNSUPPORTED;
    }

    if (!cb)
    {
        return ZW_ERR_VALUE;
    }

    memcpy(data, &cb, sizeof(pl_list_fn));
    memcpy(data + sizeof(pl_list_fn), &usr_ctx, sizeof(void *));

    plt_mtx_lck(net->mtx);

    //Call state-machine
    res = zwnet_plst_sm(net, EVT_PLIST_START, data);

    plt_mtx_ulck(net->mtx);

    if (res)
    {   //OK
        res = ZW_ERR_NONE;
    }
    else
    {   //Another provisioning list get is in progress
        res = ZW_ERR_LAST_OP_NOT_DONE;
    }

    return res;
}


/**
zwnet_pl_list_del - Delete all provisioning list entries
@param[in]	net		network
@return		ZW_ERR_XXX
*/
int zwnet_pl_list_del(zwnet_p net)
{
    int  res;

    if (!(net->ctl_cap & ZWNET_CTLR_CAP_SMART_START))
    {
        return ZW_ERR_UNSUPPORTED;
    }

    res = zw_pl_del(&net->appl_ctx, NULL, 0);

    if (res < 0)
    {
        debug_zwapi_msg(&net->plt_ctx, "zwnet_pl_list_del error:%d", res);

        res = ZW_ERR_FAILED;
    }

    return res;
}

#ifdef PL_S2_GRANT_MODE_API
/**
zwnet_pl_s2_grnt_mode_set - Set whether to ask for confirmation before granting S2 keys to non-Smart Start node in provisioning list
@param[in]	net		    Network
@param[in]	mode		1 = ask for confirmation; 0 = do not ask for confirmation
@return		ZW_ERR_XXX
@note       The setting only applies to non-Smart Start node present in the provisioning list
*/
int zwnet_pl_s2_grnt_mode_set(zwnet_p net, uint8_t mode)
{
    int res;

    if (!(net->ctl_cap & ZWNET_CTLR_CAP_SMART_START))
    {
        return ZW_ERR_UNSUPPORTED;
    }

    if (mode > 1)
    {
        return ZW_ERR_VALUE;
    }

    res = zw_pl_s2_grnt_mode_set(&net->appl_ctx, mode);

    if (res < 0)
    {
        debug_zwapi_msg(&net->plt_ctx, "zwnet_pl_s2_grnt_mode_set error:%d", res);

        res = ZW_ERR_FAILED;
    }

    return res;
}


/**
zwnet_pl_s2_grnt_mode_get_cb - S2 keys grant mode setting report callback
@param[in]	appl_ctx    The application layer context
@param[in]	mode		1 = ask for confirmation; 0 = do not ask for confirmation
@return
*/
static void zwnet_pl_s2_grnt_mode_get_cb(appl_layer_ctx_t *appl_ctx, uint8_t mode)
{
    zwnet_p             nw = (zwnet_p)appl_ctx->data;
    pl_s2_grnt_mode_fn  cb;
    void                *usr_ctx;

    plt_mtx_lck(nw->mtx);

    cb = nw->pl_ctx.grnt_mode_cb;
    usr_ctx = nw->pl_ctx.grnt_mode_usr_ctx;

    plt_mtx_ulck(nw->mtx);

    if (cb)
    {
        cb(usr_ctx, mode);
    }
}


/**
zwnet_pl_s2_grnt_mode_get - Get S2 keys grant mode setting through callback
@param[in]	net		    Network
@param[in]	cb          Callback to report S2 keys grant mode setting
@param[in]	usr_ctx     User context used in callback
@return		ZW_ERR_XXX
@note       The setting only applies to non-Smart Start node present in the provisioning list
*/
int zwnet_pl_s2_grnt_mode_get(zwnet_p net, pl_s2_grnt_mode_fn cb, void *usr_ctx)
{
    int res;

    if (!(net->ctl_cap & ZWNET_CTLR_CAP_SMART_START))
    {
        return ZW_ERR_UNSUPPORTED;
    }

    plt_mtx_lck(net->mtx);

    net->pl_ctx.grnt_mode_usr_ctx = usr_ctx;
    net->pl_ctx.grnt_mode_cb = cb;

    res = zw_pl_s2_grnt_mode_get(&net->appl_ctx, zwnet_pl_s2_grnt_mode_get_cb);

    if (res < 0)
    {
        debug_zwapi_msg(&net->plt_ctx, "zwnet_pl_s2_grnt_mode_get error:%d", res);

        res = ZW_ERR_FAILED;
    }

    plt_mtx_ulck(net->mtx);

    return res;
}
#endif


/**
zwnet_health_chk - Start health check on all but sleeping nodes
@param[in]	net		        network
@return		ZW_ERR_XXX. If ZW_ERR_NONE is returned network notify callback will be called.
*/
int zwnet_health_chk(zwnet_p net)
{
    zwnode_p    node;
    int         i;
    uint8_t     node_list[MAX_NODE_STS + 2];    //First byte is the number of nodes

    if (!(net->ctl_cap & ZWNET_CTLR_CAP_IMA))
    {
        return ZW_ERR_UNSUPPORTED;
    }

    //Check if controller is secondary controller
    if (net->zwave_role == ZW_ROLE_SECONDARY)
    {
        return ZW_ERR_UNSUPPORTED;
    }

    plt_mtx_lck(net->mtx);

    if (net->curr_op != ZWNET_OP_NONE)
    {
        //Cancel low priority operation
        if (net->curr_op == ZWNET_OP_NODE_CACHE_UPT)
        {
            zwnet_abort(net);
            debug_zwapi_msg(&net->plt_ctx, "Canceled low priority operation");
        }
        else
        {
            debug_zwapi_msg(&net->plt_ctx, "Current operation not completed yet, try again later");
            plt_mtx_ulck(net->mtx);
            return ZW_ERR_LAST_OP_NOT_DONE;
        }
    }

    i = 0;

    if ((node = (zwnode_p)net->ctl.obj.next) != NULL)
    {
        while (node && (i<MAX_NODE_STS))
        {
            if (!node->sleep_cap)
            {   //Non-sleeping node
                i++;
                node_list[i] = node->nodeid;
            }
            node = (zwnode_p)node->obj.next;
        }
        node_list[0] = i;
    }

    if (i == 0)
    {   //No non-sleeping node in the network
        plt_mtx_ulck(net->mtx);
        return ZW_ERR_NODE_NOT_FOUND;
    }

    //Call network health check state-machine
    if (nhchk_sm(net, EVT_NH_CHK_START, node_list))
    {
        net->curr_op = ZWNET_OP_HEALTH_CHK;
        plt_mtx_ulck(net->mtx);
        return ZW_ERR_NONE;
    }

    debug_zwapi_msg(&net->plt_ctx, "zwnet_health_chk failed");
    plt_mtx_ulck(net->mtx);
    return ZW_ERR_OP_FAILED;
}



#ifdef TEST_NET_OP
/**
zwnet_op_get - Get network current operation
@param[in]	net		    Network
@return		Network current operation
*/
uint8_t zwnet_op_get(zwnet_p net)
{
    return net->curr_op;
}

/**
zwnet_op_set - Set network operation
@param[in]	net		    Network
@param[in]	op		    operation
@return
*/
void zwnet_op_set(zwnet_p net, uint8_t op)
{
    plt_mtx_lck(net->mtx);
    net->curr_op = op;
    plt_mtx_ulck(net->mtx);
}
#endif



/**
@}
*/


