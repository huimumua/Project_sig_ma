/**
@file   zip_intf_sm.c - Z/IP Interface state-machines implementation.

@author David Chow

@version    1.0 16-8-2017  Initial release

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
#ifdef TCP_PORTAL
#include <signal.h>
#endif
#include "../include/zip_api_pte.h"
#include "../include/zip_api_util.h"
#include "../include/zip_intf_sm.h"
//#include "../include/zwave/ZW_controller_api.h"

// External function declarations
void    zwnet_notify(zwnet_p nw, uint8_t op, uint16_t sts, zwnet_sts_t *info);
uint16_t *zwnet_cmd_cls_find(const uint16_t *cc_lst, uint16_t cc, unsigned cc_cnt);
void    zwnet_cmd_cls_show(zwnet_p nw, uint16_t *cmd_cls_buf, uint8_t cnt);
uint8_t zwnet_cmd_cls_dedup(uint16_t *cmd_cls_lst, uint8_t cmd_cnt);
void    zwnet_devdb_node_handler(zwnode_p node);
sup_cmd_cls_t *zwnet_sup_cmd_cls_find(sup_cmd_cls_t *cmd_cls_lst, uint16_t cmd_cls, uint8_t cmd_cls_cnt);
int     zwif_cfg_bulk_rpt_hdlr(zwifd_t *ifd, zwcfg_bulk_t *rpt, uint8_t rpt_to_follow, int rsp_bulk_get);


// Internal function declarations
int     ifsm_config_sm(zwnet_p nw, ifsm_config_evt_t evt, uint8_t *data);


/**
@defgroup Interface State-machine APIs
Implementation of interface state-machines
@{
*/


/**
ifsm_ifd_cmp - Compare two interfaces
@param[in] ifd1		Interface 1
@param[in] ifd2     Interface 2
@return   Non-zero is they are same; else return zero
*/
static int ifsm_ifd_cmp(zwifd_p ifd1, zwifd_p ifd2)
{
    return ((ifd1->nodeid == ifd2->nodeid)
            && (ifd1->epid == ifd2->epid)
            && (ifd1->cls == ifd2->cls))? 1 : 0;
}


/**
ifsm_config_cmplt - Configuration state-machine completion
@param[in] nw		Network
@param[in] cfg_ctx  configuration interface context
@param[in] intf     interface
@param[in] status   status: 0=ok; 1=ok and invoke callback; else negative value
@return
*/
static void ifsm_config_cmplt(zwnet_p nw, cfg_sm_ctx_t *cfg_ctx, zwif_p intf, int status)
{

    debug_zwapi_msg(&nw->plt_ctx, "bulk set completion status: %d", status);

    if ((status == 1) && intf && intf->tmp_data)
    {
        zwrep_cfg_bulk_fn   report_cb;
        int                 i;

        for (i=0; i<intf->rpt_num; i++)
        {
            if (intf->rpt[i].rpt_cmd == CONFIGURATION_BULK_REPORT_V2)
            {
                if_cfg_tmp_dat_t *cfg_dat = (if_cfg_tmp_dat_t *)intf->tmp_data;
                zwifd_t           ifd;

                report_cb = (zwrep_cfg_bulk_fn)intf->rpt[i].rpt_cb;
                //Callback to application
                if (report_cb)
                {
                    zwif_get_desc(intf, &ifd);
                    plt_mtx_ulck(nw->mtx);
                    report_cb(&ifd, &cfg_dat->bulk_rpt_hs);
                    plt_mtx_lck(nw->mtx);
                }
                break;
            }
        }
    }

    cfg_ctx->sta = CONFIG_STA_IDLE;
    cfg_ctx->ifd.nodeid = 0;

}


/**
ifsm_config_tmout_cb - State-machine timeout callback
@param[in] data     Pointer to network
@return
*/
static void ifsm_config_tmout_cb(void *data)
{
    zwnet_p         nw = (zwnet_p)data;
    cfg_sm_ctx_t    *cfg_ctx = &nw->ifsm_ctx->cfg_ctx;

    //Stop timer
    plt_mtx_lck(nw->mtx);
    plt_tmr_stop(&nw->plt_ctx, cfg_ctx->tmr_ctx);
    cfg_ctx->tmr_ctx = NULL;

    //Call state-machine
    ifsm_config_sm(nw, EVT_CONFIG_TMOUT, NULL);
    plt_mtx_ulck(nw->mtx);

}


/**
ifsm_config_tx_cb - send configuration parameter callback function
@param[in]	appl_ctx    The application layer context
@param[in]	tx_sts		The transmit complete status
@param[in]	user_prm    The user specific parameter
@param[in]	node_id     Node id
@param[in]	param       Additional parameters
@return
*/
static void ifsm_config_tx_cb(appl_layer_ctx_t  *appl_ctx, int8_t tx_sts, void *user_prm, uint8_t node_id, zw_tx_cb_prm_t *param)
{
    zwif_p    intf = (zwif_p)user_prm;
    zwnet_p   nw = (zwnet_p)appl_ctx->data;
    uint8_t   data[16 + 16];

    data[0] = (uint8_t)tx_sts;

    plt_mtx_lck(nw->mtx);

    memcpy(data + 4, &intf, sizeof(zwif_p));

    //Call state-machine
    ifsm_config_sm(nw, EVT_CONFIG_TX_STS, data);
    plt_mtx_ulck(nw->mtx);
}


/**
ifsm_config_bulk_cb - report callback for multiple configuration parameters
@param[in]	ifd	    interface
@param[in]	param   parameter value
*/
void ifsm_config_bulk_cb(zwifd_p ifd, zwcfg_bulk_p param)
{
    zwnet_p nw = ifd->net;
    uint8_t data[32];

    memcpy(data, &ifd, sizeof(zwifd_p));
    memcpy(data + sizeof(zwifd_p), &param, sizeof(zwcfg_bulk_p));

    plt_mtx_lck(nw->mtx);

    //Call state-machine
    ifsm_config_sm(nw, EVT_CONFIG_BULK_RPT, data);
    plt_mtx_ulck(nw->mtx);

}


/**
ifsm_config_bulk_cmd_gen - generate frame for CONFIGURATION_BULK_SET based on packet size limit
@param[in]	    cfg_bulk        bulk configuration parameter
@param[in]	    max_pkt_sz	    maximum packet size
@param[in,out]	remain_prm_cnt	input: remaining parameter count, output: adjusted remaining parameter count for next frame
@param[out]     cmd	            buffer to store the generated command
@param[out]     cmd_len	        number of bytes stored in "cmd" buffer
@return	ZW_ERR_XXX
*/
static int ifsm_config_bulk_cmd_gen(zwcfg_bulk_p cfg_bulk, uint16_t max_pkt_sz, uint8_t *remain_prm_cnt, uint8_t *cmd, uint8_t *cmd_len)
{
    uint16_t    i;
    uint16_t    j = 6;//Offset to first parameter value
    uint16_t    prm_cnt_to_send = *remain_prm_cnt;
    uint16_t    max_prm_cnt;
    uint16_t    prm_start;
    uint16_t    prm_offset;

    *cmd_len = 0;

    max_prm_cnt = (max_pkt_sz - 6)/cfg_bulk->size;

    if (prm_cnt_to_send > max_prm_cnt)
    {
        prm_cnt_to_send = max_prm_cnt;
    }

    prm_offset = cfg_bulk->param_cnt - (*remain_prm_cnt);
    prm_start = cfg_bulk->param_start + prm_offset;

    cmd[0] = COMMAND_CLASS_CONFIGURATION;
    cmd[1] = CONFIGURATION_BULK_SET_V2;
    cmd[2] = HI_OF_16BIT(prm_start);
    cmd[3] = LO_OF_16BIT(prm_start);
    cmd[4] = (uint8_t)prm_cnt_to_send;
    cmd[5] = cfg_bulk->size;
    if (cfg_bulk->handshake)
    {
        cmd[5] |= 0x40;
    }
    if (cfg_bulk->deflt_data)
    {   //Use default values
        cmd[5] |= 0x80;
        if (!cfg_bulk->handshake)
        {   //No handshake
            cmd[4] = cfg_bulk->param_cnt;
            *remain_prm_cnt = 0;
            *cmd_len = 6;
            return 0;
        }
    }
    else
    {
        //Copy non-default parameters
        for (i=0; i<prm_cnt_to_send; i++)
        {
            switch (cfg_bulk->size)
            {
                case 1:
                    cmd[j++] = cfg_bulk->data[prm_offset + i].u8_data;
                    break;

                case 2:
                    cmd[j++] = (cfg_bulk->data[prm_offset + i].u16_data >> 8) & 0xFF;
                    cmd[j++] = cfg_bulk->data[prm_offset + i].u16_data & 0xFF;
                    break;

                case 4:
                    cmd[j++] = (cfg_bulk->data[prm_offset + i].u32_data >> 24) & 0xFF;
                    cmd[j++] = (cfg_bulk->data[prm_offset + i].u32_data >> 16) & 0xFF;
                    cmd[j++] = (cfg_bulk->data[prm_offset + i].u32_data >> 8) & 0xFF;
                    cmd[j++] = cfg_bulk->data[prm_offset + i].u32_data & 0xFF;
                    break;

                default:
                    return ZW_ERR_VALUE;
            }
        }
    }

    //Output
    *remain_prm_cnt = (*remain_prm_cnt) - prm_cnt_to_send;
    *cmd_len = j;

    return 0;
}


/**
ifsm_config_bulk_prm_copy - copy bulk configuration parameter
@param[in]  src     source bulk configuration parameter
@param[out] dst     destination bulk configuration parameter
@return
*/
static void ifsm_config_bulk_prm_copy(zwcfg_bulk_p dst, zwcfg_bulk_p src)
{
    int i;

    dst->param_start = src->param_start;
    dst->param_cnt = src->param_cnt;
    dst->size = src->size;
    dst->handshake = src->handshake;
    dst->deflt_data = src->deflt_data;
    //Clear data
    memset(dst->data, 0, MAX_CFG_PARAM_CNT * 4);

    for (i=0; i<src->param_cnt; i++)
    {
        switch (src->size)
        {
            case 1:
                dst->data[i].u8_data = src->data[i].u8_data;
                break;

            case 2:
                dst->data[i].u16_data = src->data[i].u16_data;
                break;

            case 4:
                dst->data[i].u32_data = src->data[i].u32_data;
                break;

        }
    }
}


/**
ifsm_config_sm - Configuration state-machine
@param[in] nw		Network
@param[in] evt      The event for the state-machine
@param[in] data     The data associated with the event
@return   Non-zero when the state-machine is started from idle state; otherwise return zero
@pre        Caller must lock the nw->mtx before calling this function.
*/
int ifsm_config_sm(zwnet_p nw, ifsm_config_evt_t evt, uint8_t *data)
{
    int             res;
    zwifd_p         ifd;
    zwif_p          intf;
    cfg_sm_ctx_t    *cfg_ctx = &nw->ifsm_ctx->cfg_ctx;
    if_cfg_tmp_dat_t *cfg_dat;
    uint8_t         cmd[MAX_CFG_PARAM_CNT];
    uint8_t         cmd_len;

    if (evt == EVT_CONFIG_SHUTDOWN)
    {
        plt_tmr_stop(&nw->plt_ctx, cfg_ctx->tmr_ctx);
        cfg_ctx->tmr_ctx = NULL;
        cfg_ctx->sta = CONFIG_STA_SHUTDOWN;
        return 0;
    }

    switch (cfg_ctx->sta)
    {
        //----------------------------------------------------------------
        case CONFIG_STA_IDLE:
        //----------------------------------------------------------------
            if (evt == EVT_CONFIG_START)
            {
                uint16_t        max_pkt_sz;
                zwcfg_bulk_p    param;

                memcpy(&ifd, data, sizeof(zwifd_p));
                memcpy(&param, data + sizeof(zwifd_p), sizeof(zwcfg_bulk_p));

                //Save the interface
                cfg_ctx->ifd = *ifd;

                intf = zwif_get_if(ifd);
                if (!intf)
                {
                    return 0;
                }

                //Get maximum packet size for the interface
                res = zwif_max_pkt_sz(intf, &max_pkt_sz);
                if (res != 0)
                {
                    return 0;
                }

                //Allocate memory for the interface to store parameter information
                if (!intf->tmp_data)
                {
                    intf->tmp_data = calloc(1, sizeof(if_cfg_tmp_dat_t));
                }

                if (!intf->tmp_data)
                {
                    return 0;
                }

                cfg_dat = (if_cfg_tmp_dat_t *)intf->tmp_data;

                //Initialize the structure pointed by cfg_dat except cfg_dat->bulk_rpt which is used by zwif_config_bulk_get()
                //to consolidate all reports
                cfg_dat->max_pkt_sz = max_pkt_sz;
                cfg_dat->remain_prm_cnt = param->param_cnt;
                cfg_dat->prev_prm_start = (param->param_start == 0xFFFF)? 0xFFF0 : 0xFFFF;
                ifsm_config_bulk_prm_copy(&cfg_dat->bulk_param, param);

                memset(&cfg_dat->bulk_rpt_hs, 0, sizeof(zwcfg_bulk_t));
                cfg_dat->bulk_rpt_hs.param_start = param->param_start;
                cfg_dat->bulk_rpt_hs.param_cnt = param->param_cnt;

                //Send bulk set
                cfg_dat->prev_prm_cnt = cfg_dat->remain_prm_cnt;
                res = ifsm_config_bulk_cmd_gen(&cfg_dat->bulk_param, cfg_dat->max_pkt_sz, &cfg_dat->remain_prm_cnt, cmd, &cmd_len);
                if (res == 0)
                {
                    res = zwif_exec_ex(ifd, cmd, cmd_len, ifsm_config_tx_cb, intf, 0, NULL);
                    if (res >= 0)
                    {
                        //Restart timer
                        plt_tmr_stop(&nw->plt_ctx, cfg_ctx->tmr_ctx);
                        cfg_ctx->tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_TMOUT, ifsm_config_tmout_cb, nw);

                        //Change state
                        cfg_ctx->sta = (cfg_dat->bulk_param.handshake)? CONFIG_STA_WAIT_BULK_RPT : CONFIG_STA_WAIT_TX_STS;

                        cfg_ctx->retry_cnt = 0;

                        return 1;
                    }
                }

                //Error
//              free(intf->tmp_data);
//              intf->tmp_data = NULL;

            }
            break;

        //----------------------------------------------------------------
        case CONFIG_STA_WAIT_BULK_RPT:
        //----------------------------------------------------------------
            {
                if (evt == EVT_CONFIG_BULK_RPT)
                {
                    zwcfg_bulk_p    param;

                    memcpy(&ifd, data, sizeof(zwifd_p));
                    memcpy(&param, data + sizeof(zwifd_p), sizeof(zwcfg_bulk_p));

                    intf = zwif_get_if(ifd);

                    if (!intf || !intf->tmp_data)
                    {
                        break;
                    }

                    //Check whether the interface matches the request
                    if (!ifsm_ifd_cmp(&cfg_ctx->ifd, ifd))
                    {
                        break;
                    }

                    cfg_dat = (if_cfg_tmp_dat_t *)intf->tmp_data;

                    //Check for default data
                    if (cfg_dat->bulk_param.deflt_data && !param->deflt_data)
                    {
                        break;
                    }

                    //Check for repeated bulk report
                    if (cfg_dat->prev_prm_start == param->param_start)
                    {
                        break;
                    }

                    //stop timer
                    plt_tmr_stop(&nw->plt_ctx, cfg_ctx->tmr_ctx);
                    cfg_ctx->tmr_ctx = NULL;

                    //Save the report start parameter number
                    cfg_dat->prev_prm_start = param->param_start;

                    //Consolidate reports. NOTE: param will be changed after this.
                    zwif_cfg_bulk_rpt_hdlr(ifd, param, 0, 0);

                    //Check for any report to send
                    if (cfg_dat->remain_prm_cnt)
                    {
                        //Send bulk set
                        cfg_dat->prev_prm_cnt = cfg_dat->remain_prm_cnt;
                        res = ifsm_config_bulk_cmd_gen(&cfg_dat->bulk_param, cfg_dat->max_pkt_sz, &cfg_dat->remain_prm_cnt, cmd, &cmd_len);
                        if (res == 0)
                        {
                            res = zwif_exec_ex(ifd, cmd, cmd_len, ifsm_config_tx_cb, intf, 0, NULL);
                            if (res >= 0)
                            {
                                //Restart timer
                                plt_tmr_stop(&nw->plt_ctx, cfg_ctx->tmr_ctx);
                                cfg_ctx->tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_TMOUT, ifsm_config_tmout_cb, nw);

                                cfg_ctx->retry_cnt = 0;

                                break;
                            }
                        }
                        ifsm_config_cmplt(nw, cfg_ctx, intf, -1);
                    }
                    else
                    {
                        ifsm_config_cmplt(nw, cfg_ctx, intf, 1);
                    }

                }
                else if (evt == EVT_CONFIG_TMOUT)
                {
                    if (cfg_ctx->retry_cnt++ == 0)
                    {
                        ifd = &cfg_ctx->ifd;

                        intf = zwif_get_if(ifd);

                        if (!intf || !intf->tmp_data)
                        {
                            ifsm_config_cmplt(nw, cfg_ctx, intf, -1);
                            break;
                        }

                        cfg_dat = (if_cfg_tmp_dat_t *)intf->tmp_data;

                        //Resend bulk set
                        cfg_dat->remain_prm_cnt = cfg_dat->prev_prm_cnt;//restore previous remain param count
                        res = ifsm_config_bulk_cmd_gen(&cfg_dat->bulk_param, cfg_dat->max_pkt_sz, &cfg_dat->remain_prm_cnt, cmd, &cmd_len);
                        if (res == 0)
                        {
                            res = zwif_exec_ex(ifd, cmd, cmd_len, ifsm_config_tx_cb, intf, 0, NULL);
                            if (res >= 0)
                            {
                                //Restart timer
                                plt_tmr_stop(&nw->plt_ctx, cfg_ctx->tmr_ctx);
                                cfg_ctx->tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_TMOUT, ifsm_config_tmout_cb, nw);

                                break;
                            }
                        }
                    }

                    ifsm_config_cmplt(nw, cfg_ctx, NULL, -1);
                }
            }
            break;

        //----------------------------------------------------------------
        case CONFIG_STA_WAIT_TX_STS:
        //----------------------------------------------------------------
            {
                if (evt == EVT_CONFIG_TX_STS)
                {
                    zwifd_t cfg_ifd;
                    int8_t  tx_sts;

                    tx_sts = (int8_t)data[0];

                    memcpy(&intf, data + 4, sizeof(zwif_p));

                    zwif_get_desc(intf, &cfg_ifd);

                    if (!intf || !intf->tmp_data)
                    {
                        break;
                    }

                    //Check whether the interface matches the request
                    if (!ifsm_ifd_cmp(&cfg_ctx->ifd, &cfg_ifd))
                    {
                        break;
                    }

                    cfg_dat = (if_cfg_tmp_dat_t *)intf->tmp_data;

                    //stop timer
                    plt_tmr_stop(&nw->plt_ctx, cfg_ctx->tmr_ctx);
                    cfg_ctx->tmr_ctx = NULL;

                    if (tx_sts != TRANSMIT_COMPLETE_OK)
                    {
                        ifsm_config_cmplt(nw, cfg_ctx, intf, -1);
                        break;
                    }

                    //Check for any report to send
                    if (cfg_dat->remain_prm_cnt)
                    {
                        //Send bulk set
                        cfg_dat->prev_prm_cnt = cfg_dat->remain_prm_cnt;
                        res = ifsm_config_bulk_cmd_gen(&cfg_dat->bulk_param, cfg_dat->max_pkt_sz, &cfg_dat->remain_prm_cnt, cmd, &cmd_len);
                        if (res == 0)
                        {
                            res = zwif_exec_ex(&cfg_ifd, cmd, cmd_len, ifsm_config_tx_cb, intf, 0, NULL);
                            if (res >= 0)
                            {
                                //Restart timer
                                plt_tmr_stop(&nw->plt_ctx, cfg_ctx->tmr_ctx);
                                cfg_ctx->tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_TMOUT, ifsm_config_tmout_cb, nw);

                                cfg_ctx->retry_cnt = 0;

                                break;
                            }
                        }
                        ifsm_config_cmplt(nw, cfg_ctx, intf, -1);
                    }
                    else
                    {
                        ifsm_config_cmplt(nw, cfg_ctx, intf, 0);
                    }

                }
                else if (evt == EVT_CONFIG_TMOUT)
                {
                    ifsm_config_cmplt(nw, cfg_ctx, NULL, -1);
                }
            }
            break;

        //----------------------------------------------------------------
        case CONFIG_STA_SHUTDOWN:
        //----------------------------------------------------------------
            break;
    }

    return 0;
}


/**
ifsm_ctx_init - Initialize interface state-machines context
@param[in]	nw		    Network context
@return Interface state-machines context on success; else returns NULL
@post Caller must call ifsm_ctx_shutdown() followed by ifsm_ctx_exit() to shutdown and cleanp up if this call is successfully
*/
ifsm_ctx_t *ifsm_ctx_init(zwnet_p nw)
{
    ifsm_ctx_t *ctx;

    ctx = (ifsm_ctx_t *)calloc(1, sizeof(ifsm_ctx_t));

    if (!ctx)
    {
        return NULL;
    }

    ctx->nw = nw;

    return ctx;
}


/**
ifsm_ctx_shutdown - Shutdown interface state-machines context
@param[in]	ctx		    Interface state-machines context
@return
*/
void ifsm_ctx_shutdown(ifsm_ctx_t *ctx)
{
    zwnet_p nw;

    if (!ctx)
    {
        return;
    }

    nw = ctx->nw;

    plt_mtx_lck(nw->mtx);

    ifsm_config_sm(nw, EVT_CONFIG_SHUTDOWN, NULL);

    plt_mtx_ulck(nw->mtx);


}


/**
ifsm_ctx_exit - Clean up and free resources for interface state-machines context
@param[in]	ctx		    Interface state-machines context
@return
@post ctx must not be used again after this call
*/
void ifsm_ctx_exit(ifsm_ctx_t *ctx)
{
    if (!ctx)
    {
        return;
    }

    free(ctx);
}

/**
@}
*/






