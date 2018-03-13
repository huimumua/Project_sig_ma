/**
@file   zip_ima.c - Z-wave installation and maintenance implementation.

@author David Chow

@version    1.0 14-9-17  Initial release

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
#include "../include/zwave/ZW_controller_api.h"

#define USE_ADDENDUM_A4     //Calculate LWR_RSSI based on Addendum to PSP-12298 Appendix A.4
#define SHOW_BK_RSSI        //Show background RSSI for each channel

// External function declarations
void    zwnet_notify(zwnet_p nw, uint8_t op, uint16_t sts, zwnet_sts_t *info);
int     zwnet_ctlr_send(zwnet_p nw, uint8_t *cmd, uint8_t len, tx_cmplt_cb_t cb, void *user);


// Internal function declarations
int     nhchk_sm(zwnet_p nw, zwnet_nh_chk_evt_t evt, uint8_t *data);


/**
@defgroup IMA Z-wave installation and maintenance APIs
Implementation of Z-wave installation and maintenance services such as network health check
@{
*/


/**
callback to report installation and maintenance statistics of a node
@param[in]	appl_ctx    context
@param[in]	stat        statistics array
@param[in]	stat_cnt	number of element in 'stat' array
@param[in]	node_id     node id
*/
static void nhchk_stat_cb(appl_layer_ctx_t *appl_ctx, zw_ima_stat_t *stat, uint8_t stat_cnt, uint8_t node_id)
{
    zwnet_p nw = (zwnet_p)appl_ctx->data;
    uint8_t data[16 + 16];

    data[0] = node_id;
    data[1] = stat_cnt;
    ALOGI("node stats check report:, type: %d, value:%d",stat->type, stat->val);

    memcpy(data + 4, &stat, sizeof(zw_ima_stat_t *));

    plt_mtx_lck(nw->mtx);

    //Call state-machine
    nhchk_sm(nw, EVT_NH_CHK_S_RPT, data);

    plt_mtx_ulck(nw->mtx);

}


/**
nhchk_probe_cb - send NOP callback function
@param[in]	appl_ctx    The application layer context
@param[in]	tx_sts		The transmit complete status
@param[in]	user_prm    The user specific parameter
@param[in]	nodeid      Node id
@param[in]	param       Additional parameters
@return
*/
static void  nhchk_probe_cb(appl_layer_ctx_t *appl_ctx, int8_t tx_sts, void *user_prm, uint8_t nodeid, zw_tx_cb_prm_t *param)
{
    zwnet_p nw = (zwnet_p)appl_ctx->data;
    uint8_t data[16 + 16];

    data[0] = nodeid;
    data[1] = (uint8_t)tx_sts;

    memcpy(data + 4, &param, sizeof(zw_tx_cb_prm_t *));


    if (tx_sts != TRANSMIT_COMPLETE_OK)
    {
        debug_zwapi_msg(appl_ctx->plt_ctx, "Send NOP with tx error:%d, node id:%u", tx_sts, nodeid);
    }

    plt_mtx_lck(nw->mtx);

    //Call state-machine
    nhchk_sm(nw, EVT_NH_CHK_TX_STS, data);

    plt_mtx_ulck(nw->mtx);
}


/**
nhchk_busy_cb - Network health check state-machine busy timer callback
@param[in] data     Pointer to network
@return
*/
static void nhchk_busy_cb(void *data)
{
    zwnet_p nw = (zwnet_p)data;

    //Stop timer
    plt_mtx_lck(nw->mtx);
    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
    nw->sm_tmr_ctx = NULL;

    //Call state-machine
    nhchk_sm(nw, EVT_NH_CHK_RETRY, NULL);
    plt_mtx_ulck(nw->mtx);

}


/**
nhchk_stat_tx_sts_cb - Transmit status callback function
@param[in]	appl_ctx    The application layer context
@param[in]	tx_sts		The transmit complete status
@param[in]	user_prm    The user specific parameter
@param[in]	node_id     Node id
@param[in]	param       Additional parameters
@return
*/
static void nhchk_stat_tx_sts_cb(appl_layer_ctx_t *appl_ctx, int8_t tx_sts, void *user_prm, uint8_t node_id, zw_tx_cb_prm_t *param)
{
    zwnet_p nw = (zwnet_p)appl_ctx->data;

    if (tx_sts != TRANSMIT_COMPLETE_OK)
    {
        debug_zwapi_ts_msg(appl_ctx->plt_ctx, "Send statistics clear with tx error:%d", tx_sts);
    }

    plt_mtx_lck(nw->mtx);

    //Call state-machine
    nhchk_sm(nw, EVT_NH_CHK_S_CLEAR, (uint8_t *)&tx_sts);

    plt_mtx_ulck(nw->mtx);
}


/**
nhchk_gw_rssi_cb - callback to report background RSSI from Z/IP gateway
@param[in]	appl_ctx    context
@param[in]	rssi        RSSI, valid values are -32 to -94 and RSSI_XXX; first value is for channel 0, second value is for channel 1, etc.
@param[in]	rssi_cnt	number of RSSI in the rssi array
*/
static void nhchk_gw_rssi_cb(appl_layer_ctx_t *appl_ctx, int8_t *rssi, uint8_t rssi_cnt)
{
    zwnet_p nw = (zwnet_p)appl_ctx->data;
    uint8_t data[8];
    uint8_t i;

    data[0] = rssi_cnt;

    for (i=0; i<rssi_cnt; i++)
    {
        //Substitute value for RSSI_BELOW_SENSITIVITY and RSSI_MAX_PWR_SATURATED
        if (rssi[i] == RSSI_MAX_PWR_SATURATED)
        {
            rssi[i] = 1;
        }
        else if (rssi[i] == RSSI_BELOW_SENSITIVITY)
        {
            rssi[i] = -100;
        }

        data[i + 1] = (uint8_t)rssi[i];
    }

    if (rssi_cnt == 2)
    {
        ALOGI("nhchk_gw_rssi_cb, GW background RSSI, ch 0:%d, ch1:%d", rssi[0], rssi[1]);
    }
    else if (rssi_cnt == 3)
    {
        ALOGI("nhchk_gw_rssi_cb, GW background RSSI, ch 0:%d, ch1:%d, ch2:%d", rssi[0], rssi[1], rssi[2]);
    }
    else if(rssi_cnt == 1)
    {
        ALOGI("nhchk_gw_rssi_cb, GW background RSSI, ch 0:%d", rssi[0]);
    }

    // callback to application
    zwcontrol_nwtwork_health_rssi_report(rssi, rssi_cnt);

    plt_mtx_lck(nw->mtx);

    //Call state-machine
    nhchk_sm(nw, EVT_NH_CHK_GW_RSSI, data);

    plt_mtx_ulck(nw->mtx);
}


/**
nhchk_tout_cb - Network health check state-machine timer callback
@param[in] data     Pointer to network
@return
*/
static void nhchk_tout_cb(void *data)
{
    zwnet_p   nw = (zwnet_p)data;

    //Stop timer
    plt_mtx_lck(nw->mtx);
    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
    nw->sm_tmr_ctx = NULL;

    //Call state-machine
    nhchk_sm(nw, EVT_NH_CHK_TMOUT, NULL);
    plt_mtx_ulck(nw->mtx);

}


/**
nhchk_pwrlvl_tout_cb - Network health check state-machine powerlevel test timer callback
@param[in] data     Pointer to network
@return
*/
static void nhchk_pwrlvl_tout_cb(void *data)
{
    zwnet_p   nw = (zwnet_p)data;

    //Stop timer
    plt_mtx_lck(nw->mtx);
    plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
    nw->sm_tmr_ctx = NULL;

    //Call state-machine
    nhchk_sm(nw, EVT_NH_CHK_PWRLVL_TMOUT, NULL);
    plt_mtx_ulck(nw->mtx);

}


/**
nhchk_failed - Network health check failure cleanup
@param[in]	nw      Network
@return
@pre    Caller must lock the nw->mtx before calling this function
*/
static void nhchk_failed(zwnet_p nw)
{
    nhchk_sm_ctx_t  *sm_ctx = &nw->nh_chk_sm_ctx;

    zwnet_notify(nw, nw->curr_op, OP_FAILED, NULL);

    //Reset operation to "no operation"
    nw->curr_op = ZWNET_OP_NONE;

    sm_ctx->state = NH_CHK_STA_IDLE;

}


/**
nhchk_val_ave_store - Calculate the average of network health value and store the result
@param[in]	nw      Network
@return
@pre    Caller must lock the nw->mtx before calling this function
*/
static void nhchk_val_ave_store(nhchk_sm_ctx_t *sm_ctx)
{
    uint8_t  ave;
    uint8_t  stat_cat;
    uint16_t remainder;

    ave = sm_ctx->nhv_sum / sm_ctx->chk_cnt;

    remainder = sm_ctx->nhv_sum % sm_ctx->chk_cnt;
    if (remainder > (sm_ctx->chk_cnt >> 1))
    {   //Have remainder that is greater than half of divisor, add 1 to the average
        ave++;
    }

    if (ave >= 8)
    {
        stat_cat = NW_HEALTH_GREEN;
    }
    else if (ave >= 4)
    {
        stat_cat = NW_HEALTH_YELLOW;
    }
    else if (ave >= 1)
    {
        stat_cat = NW_HEALTH_RED;
    }
    else
    {
        stat_cat = NW_HEALTH_CRITICAL;
    }

    //Store the result
    sm_ctx->health_rpt.sts[sm_ctx->node_idx].value = ave;
    sm_ctx->health_rpt.sts[sm_ctx->node_idx].sts_cat = stat_cat;

}


/**
nhchk_val_cal - Calculate network health value
@param[in]	nw              Network
@param[in]  stat            Network health check installation and maintenance statistics
@param[in]  lwr_pwrlvl_ok   Flag to indicate all the repeaters and destination node have 6 dB power reduction margin
@param[in]  lwr_rssi_ok     Flag to indicate the node has at least 17 dB LWR RSSI
@param[out] nhv             Calculated network health value
@param[out] sts_cat         Status category which is derived from network health value, see NW_HEALTH_XXX
@return
*/
static void nhchk_val_cal(zwnet_p nw, nhchk_ima_stat_t *stat, uint8_t lwr_pwrlvl_ok, uint8_t lwr_rssi_ok, uint8_t *nhv, uint8_t *sts_cat)
{
    debug_zwapi_msg(&nw->plt_ctx, "\nrc=%u, tc=%u, nb=%u, pec=%u, lwr_pwrlvl_ok=%u, lwr_rssi_ok=%u",
                    stat->rc, stat->tc, stat->nb, stat->per, lwr_pwrlvl_ok, lwr_rssi_ok);

    //Check if all transmissions were unsuccessful
    if (stat->tc == stat->per)
    {
        *nhv = 0;
        *sts_cat = NW_HEALTH_CRITICAL;
        return;
    }

    if ((stat->per == 0) && (stat->rc == 0) && (stat->nb > 2) && lwr_pwrlvl_ok && lwr_rssi_ok)
    {
        *nhv = 10;
        *sts_cat = NW_HEALTH_GREEN;
    }
    else if ((stat->per == 0) && (stat->rc == 1) && (stat->nb > 2) && lwr_pwrlvl_ok && lwr_rssi_ok)
    {
        *nhv = 9;
        *sts_cat = NW_HEALTH_GREEN;
    }
    else if ((stat->per == 0) && (stat->rc <= 1) && (stat->nb <= 2) && lwr_pwrlvl_ok && lwr_rssi_ok)
    {
        *nhv = 8;
        *sts_cat = NW_HEALTH_GREEN;
    }
    else if ((stat->per == 0) && (stat->rc <= 1) && (stat->nb > 2) && (!lwr_pwrlvl_ok || !lwr_rssi_ok))
    {
        *nhv = 7;
        *sts_cat = NW_HEALTH_YELLOW;
    }
    else if ((stat->per == 0) && (stat->rc <= 1) && (stat->nb <= 2) && (!lwr_pwrlvl_ok || !lwr_rssi_ok))
    {
        *nhv = 6;
        *sts_cat = NW_HEALTH_YELLOW;
    }
    else if ((stat->per == 0) && (stat->rc > 1) && (stat->rc <= 4))
    {
        *nhv = 5;
        *sts_cat = NW_HEALTH_YELLOW;
    }
    else if ((stat->per == 0) && (stat->rc > 4))
    {
        *nhv = 4;
        *sts_cat = NW_HEALTH_YELLOW;
    }
    else if (stat->per == 1)
    {
        *nhv = 3;
        *sts_cat = NW_HEALTH_RED;
    }
    else if (stat->per == 2)
    {
        *nhv = 2;
        *sts_cat = NW_HEALTH_RED;
    }
    else if ((stat->per > 2) && (stat->per < 10))
    {
        *nhv = 1;
        *sts_cat = NW_HEALTH_RED;
    }
    else
    {
        *nhv = 0;
        *sts_cat = NW_HEALTH_CRITICAL;
    }
}


/**
nhchk_ima_stat_parse - Parse installation and maintenance (IMA) statistics
@param[in]  stat        IMA statistics
@param[in]  stat_cnt    Number of entries in stat
@param[out] ima_stat    Output IMA statistics
@return zero on success; negative error code on failure
*/
static int nhchk_ima_stat_parse(zw_ima_stat_t *stat, uint8_t stat_cnt, nhchk_ima_stat_t *ima_stat)
{
    int     i;
    uint8_t msk = 0;

#define ZW_IMA_MASK_RC      0x01   ///< Route changes
#define ZW_IMA_MASK_TC      0x02   ///< Transmission count
#define ZW_IMA_MASK_NB      0x04   ///< Neighbors count
#define ZW_IMA_MASK_PEC     0x08   ///< Packet error count

#define ZW_IMA_MASK_ALL     0x0F   ///< All of the above bit-masks

    for (i=0; i<stat_cnt; i++)
    {
        switch (stat[i].type)
        {
            case ZW_IMA_STAT_RC:
                msk |= ZW_IMA_MASK_RC;
                ima_stat->rc = stat[i].val;
                break;

            case ZW_IMA_STAT_TC:
                msk |= ZW_IMA_MASK_TC;
                ima_stat->tc = stat[i].val;
                break;

            case ZW_IMA_STAT_NB:
                msk |= ZW_IMA_MASK_NB;
                ima_stat->nb = stat[i].val;
                break;

            case ZW_IMA_STAT_PEC:
                msk |= ZW_IMA_MASK_PEC;
                ima_stat->per = stat[i].val;
                break;
        }
    }

    if ((msk & ZW_IMA_MASK_ALL) != ZW_IMA_MASK_ALL)
    {   //Missing statistic entry
        ima_stat->tc = 0;
        return -1;
    }

    return 0;
}


/**
nhchk_int8_compare - Compare two int8_t records
@param[in]	a	  First integer record
@param[in]	b	  Second integer record
@return		Negative if the first argument is "less" than the second, zero if they are "equal",
            and positive if the first argument is “greater”.
 */
static int nhchk_int8_compare(const void *a, const void *b)
{
    const int8_t *rec_a = (const int8_t *)a;
    const int8_t *rec_b = (const int8_t *)b;

    return (*rec_a - *rec_b);
}


#ifdef SHOW_BK_RSSI
static void display_int(zwnet_p nw, int8_t *buf, const char *str)
{

    ALOGI("%s %d %d %d %d %d %d %d %d %d %d", str, buf[0],  buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9]);
}
#endif


/**
nhchk_rssi_cal - Calculate LWR RSSI
@param[in]	nw          Network
@param[in]  sm_ctx      State-machine context
@return
*/
static void nhchk_rssi_cal(zwnet_p nw, nhchk_sm_ctx_t *sm_ctx)
{
    uint8_t i;
    uint8_t j;
    uint8_t ch_cnt;
    int8_t  rssi_val;
    int8_t  rssi_min;
#ifndef USE_ADDENDUM_A4
    int8_t  rssi_bkgnd_min;
#endif
    int     rssi_sum;

    ch_cnt = (sm_ctx->bkgnd_rssi[2][0] == RSSI_NOT_AVAILABLE)? 2 : 3;

    for (i=0; i<ch_cnt; i++)
    {
#ifdef SHOW_BK_RSSI
        display_int(nw, &sm_ctx->bkgnd_rssi[i][0], "Background RSSI:");
#endif
        //Sort the RSSI readings in ascending order
        qsort (&sm_ctx->bkgnd_rssi[i][0], NHCHK_BKGND_RSSI_CNT, 1, nhchk_int8_compare);

        //Calculate the average of the 5 smallest RSSI value, then store the average back to the first item of the array
        for (j=0, rssi_sum=0; j<5; j++)
        {
            rssi_sum += sm_ctx->bkgnd_rssi[i][j];
        }

        sm_ctx->bkgnd_rssi[i][0] = (int8_t)(rssi_sum/5);

        ALOGI("Ave background RSSI for channel %u is %d", i, sm_ctx->bkgnd_rssi[i][0]);

    }

#ifdef USE_ADDENDUM_A4

    if (sm_ctx->tx_ch == NHCHK_UNKNOWN_TX_CH)
    {
        debug_zwapi_msg(&nw->plt_ctx, "No transmit channel!");
        return;
    }
    debug_zwapi_msg(&nw->plt_ctx, "Transmit channel: %u", sm_ctx->tx_ch);

    if (sm_ctx->tx_ch > 2)
    {
        debug_zwapi_msg(&nw->plt_ctx, "Invalid transmit channel!");
        return;
    }

    if (sm_ctx->rssi_cnt == 0)
    {
        debug_zwapi_msg(&nw->plt_ctx, "No entry in RSSI array!");
        return;
    }

    rssi_min = 127; //Start with max. value
    for (i=0; i<sm_ctx->rssi_cnt; i++)
    {
        rssi_val = sm_ctx->rssi[i] - sm_ctx->bkgnd_rssi[sm_ctx->tx_ch][0];

        if (rssi_val < rssi_min)
        {
            rssi_min = rssi_val;
        }
    }

#else
    if (ch_cnt == 2)
    {   //Calculation for 2-channel system
        if (sm_ctx->ack_ch == NHCHK_UNKNOWN_ACK_CH)
        {
            debug_zwapi_msg(&nw->plt_ctx, "No ACK channel!");
            return;
        }
        debug_zwapi_msg(&nw->plt_ctx, "ACK channel: %u", sm_ctx->ack_ch);

        if (sm_ctx->ack_ch > 1)
        {
            debug_zwapi_msg(&nw->plt_ctx, "Invalid ACK channel!");
            return;
        }

        if (sm_ctx->rssi_cnt == 0)
        {
            debug_zwapi_msg(&nw->plt_ctx, "No entry in RSSI array!");
            return;
        }

        rssi_min = 127; //Start with max. value
        for (i=0; i<sm_ctx->rssi_cnt; i++)
        {
            rssi_val = sm_ctx->rssi[i] - sm_ctx->bkgnd_rssi[sm_ctx->ack_ch][0];

            if (rssi_val < rssi_min)
            {
                rssi_min = rssi_val;
            }
        }
    }
    else
    {
        //Calculation for 3-channel system
        rssi_bkgnd_min = 127; //Start with max. value
        for (i=0; i<3; i++)
        {
            rssi_val = sm_ctx->bkgnd_rssi[i][0];

            if (rssi_val < rssi_bkgnd_min)
            {
                rssi_bkgnd_min = rssi_val;
            }
        }

        rssi_min = 127; //Start with max. value
        for (i=0; i<sm_ctx->rssi_cnt; i++)
        {
            rssi_val = sm_ctx->rssi[i] - rssi_bkgnd_min;

            if (rssi_val < rssi_min)
            {
                rssi_min = rssi_val;
            }
        }
    }
#endif

    //Determine whether the LWR RSSI meets the network health check threshold
    sm_ctx->lwr_rssi_ok = (rssi_min >= 17)? 1 : 0;

    debug_zwapi_msg(&nw->plt_ctx, "LWR RSSI: %d", rssi_min);

}


/**
nhchk_ima_rpt_parse - Parse installation and maintenance (IMA) report from Z/IP header extension
@param[in]	nw          Network
@param[in]  sm_ctx      State-machine context
@param[in]  ima_rpt     IMA report
@param[in]  ima_rpt_len IMA report length
@return
*/
static void nhchk_ima_rpt_parse(zwnet_p nw, nhchk_sm_ctx_t *sm_ctx, uint8_t *ima_rpt, uint8_t ima_rpt_len)
{
    int     i;
    uint8_t *lwr_val;
    uint8_t *rssi_val;
    uint8_t *ack_ch_val;
    uint8_t *tx_ch_val;
    uint8_t tx_ch_len;
    uint8_t ack_ch_len;
    uint8_t lwr_len;
    uint8_t rssi_len;
    int8_t  rssi_sign_val;


    //Parse for LWR
    lwr_val = util_tlv_get(ima_rpt, ima_rpt_len, ZIP_IME_LWR, &lwr_len);

    if (lwr_val)
    {
        if (lwr_len > NHCHK_MAX_REPEATER)
        {
            lwr_len = NHCHK_MAX_REPEATER;
        }

        sm_ctx->lwr_cnt = 0;

        for (i=0; i<lwr_len; i++)
        {
            if (lwr_val[i])
            {
                sm_ctx->lwr[sm_ctx->lwr_cnt++] = lwr_val[i];
            }
            else
            {
                break;
            }
        }
    }


    //Parse for RSSI
    rssi_val = util_tlv_get(ima_rpt, ima_rpt_len, ZIP_IME_RSSI, &rssi_len);

    if (rssi_val)
    {
        if (rssi_len > NHCHK_MAX_RSSI_HOP)
        {
            rssi_len = NHCHK_MAX_RSSI_HOP;
        }

        sm_ctx->rssi_cnt = 0;

        for (i=0; i<rssi_len; i++)
        {
            rssi_sign_val = (int8_t)rssi_val[i];

            if (rssi_sign_val == RSSI_NOT_AVAILABLE)
            {
                break;
            }

            //Substitute value for RSSI_BELOW_SENSITIVITY and RSSI_MAX_PWR_SATURATED
            if (rssi_sign_val == RSSI_MAX_PWR_SATURATED)
            {
                rssi_sign_val = 1;
            }
            else if (rssi_sign_val == RSSI_BELOW_SENSITIVITY)
            {
                rssi_sign_val = -100;
            }

            sm_ctx->rssi[sm_ctx->rssi_cnt++] = rssi_sign_val;
        }
    }

    //Parse for ACK channel
    ack_ch_val = util_tlv_get(ima_rpt, ima_rpt_len, ZIP_IME_ACK_CH, &ack_ch_len);

    if (ack_ch_val && (ack_ch_len == 1))
    {
        sm_ctx->ack_ch = *ack_ch_val;
    }
//  else    //Testing
//  {
//      sm_ctx->ack_ch = 1;//Testing
//  }

    //Parse for transmit channel
    tx_ch_val = util_tlv_get(ima_rpt, ima_rpt_len, ZIP_IME_TX_CH, &tx_ch_len);

    if (tx_ch_val && (tx_ch_len == 1))
    {
        sm_ctx->tx_ch = *tx_ch_val;
    }

    //Display parse result
    if (sm_ctx->lwr_cnt == NHCHK_UNKNOWN_LWR)
    {
        ALOGW("LWR not found in IMA report!");
    }
    else if (sm_ctx->lwr_cnt == 0)
    {
        ALOGI("Direct range to node id:%u", sm_ctx->health_rpt.sts[sm_ctx->node_idx].node_id);
    }
    else
    {
        for (i=0; i<sm_ctx->lwr_cnt; i++)
        {
            ALOGI("Repeater %d node id:%u", i+1, sm_ctx->lwr[i]);
        }
    }

    ALOGI("Number of RSSI hops:%u", sm_ctx->rssi_cnt);

    for (i=0; i<sm_ctx->rssi_cnt; i++)
    {
        ALOGI("RSSI hop %d value:%d", i+1, sm_ctx->rssi[i]);
    }

    if (sm_ctx->ack_ch == NHCHK_UNKNOWN_ACK_CH)
    {
        ALOGI("ACK channel is unknown");
    }
    else
    {
        debug_zwapi_msg(&nw->plt_ctx, "ACK channel is %u", sm_ctx->ack_ch);
    }

    if (sm_ctx->tx_ch == NHCHK_UNKNOWN_TX_CH)
    {
        ALOGI("Transmit channel is unknown");
    }
    else
    {
        ALOGI("Transmit channel is %u", sm_ctx->tx_ch);
    }

    // Report this info to application
    zwcontrol_network_ima_report(sm_ctx);

}


/**
nhchk_pwrlvl_sup_chk - Check if all repeaters to the node as well as the node itself support Powerlevel command class
@param[in]	nw          Network
@param[in]  lwr         Buffer that stores last working route node id
@param[in]  lwr_cnt     Number of entries in lwr or NHCHK_UNKNOWN_LWR
@param[in]  node_id     Node id of the destination node
@return zero on success; negative error code on failure
*/
static int nhchk_pwrlvl_sup_chk(zwnet_p nw, uint8_t *lwr, uint8_t lwr_cnt, uint8_t node_id)
{
    int         i;
    zwnode_p    node;
    uint8_t     node_lst[16];
    uint8_t     node_cnt;

    if (lwr_cnt == NHCHK_UNKNOWN_LWR)
    {
        return -1;
    }

    node_cnt = ((lwr_cnt + 1) <= 16)? (lwr_cnt + 1) : 16;

    //Copy repeaters
    for (i=0; i< (node_cnt - 1); i++)
    {
        node_lst[i] = lwr[i];
    }
    //Copy destination node
    node_lst[node_cnt - 1] = node_id;

    //Check
    for (i=0; i<node_cnt; i++)
    {
        node = zwnode_find(&nw->ctl, node_lst[i]);

        if (!node)
        {
            return -2;
        }

        if (!node->ep.intf)
        {
            return -3;
        }

        if (!zwif_find_cls(node->ep.intf, COMMAND_CLASS_POWERLEVEL))
        {
            return -4;
        }
    }

    return 0;
}


/**
nhchk_rssi_sup_chk - Check if all repeaters to the node as well as the node itself and controller support RSSI measurement (SDK >= 6.60)
@param[in]	nw          Network
@param[in]  lwr         Buffer that stores last working route node id
@param[in]  lwr_cnt     Number of entries in lwr or NHCHK_UNKNOWN_LWR
@param[in]  node_id     Node id of the destination node
@return zero on success; negative error code on failure
*/
static int nhchk_rssi_sup_chk(zwnet_p nw, uint8_t *lwr, uint8_t lwr_cnt, uint8_t node_id)
{
    static const uint8_t    sub_ver_lst[] = {1, 5, 24, 34, 38};  //protocol version = 4, subversion list that belongs to SDK < 6.60

    int         i;
    zwnode_p    node;
    uint8_t     node_lst[16];
    uint8_t     node_cnt;

    if (lwr_cnt == NHCHK_UNKNOWN_LWR)
    {
        return -1;
    }

    node_cnt = ((lwr_cnt + 2) <= 16)? (lwr_cnt + 2) : 16;

    //Copy repeaters
    for (i=0; i< (node_cnt - 2); i++)
    {
        node_lst[i] = lwr[i];
    }

    //Copy controller node
    node_lst[node_cnt - 1] = nw->ctl.nodeid;

    //Copy destination node
    node_lst[node_cnt - 2] = node_id;


    //Check
    for (i=0; i<node_cnt; i++)
    {
        node = zwnode_find(&nw->ctl, node_lst[i]);

        if (!node)
        {
            return -2;
        }

        if ((node->proto_ver >> 8) < 4)
        {
            return -3;
        }

        if (((node->proto_ver >> 8) == 4) && memchr(sub_ver_lst, node->proto_ver & 0x00FF, sizeof(sub_ver_lst)))
        {
            return -3;
        }
    }

    return 0;
}


/**
nhchk_pwrlvl_test - Start powerlevel node test with -6dBm power reduction
@param[in]	nw          Network
@param[in]  lwr         Buffer that stores last working route node id
@param[in]  lwr_cnt     Number of entries in lwr or NHCHK_UNKNOWN_LWR
@param[in]  node_id     Node id of the destination node
@param[in]  lwr_idx     Index to lwr array
@param[out]  src_nid    Source node id of the node that performs the powerlevel test
@return	ZW_ERR_XXX
*/
static int nhchk_pwrlvl_test(zwnet_p nw, uint8_t *lwr, uint8_t lwr_cnt, uint8_t node_id, uint8_t lwr_idx, uint8_t *src_nid)
{
    int         res;
    zwnode_p    test_node;
    zwnode_p    src_node;
    zwif_p      pwrlvl_if;
    uint8_t     test_node_id;
    uint8_t     src_node_id;
    zwnoded_t   test_node_desc;
    zwifd_t     pwr_lvl_ifd;

    //Determine test node id (NOP will be sent to this node)
    if (lwr_idx == 0)
    {
        test_node_id = nw->ctl.nodeid;
    }
    else
    {
        test_node_id = lwr[lwr_idx - 1];
    }

    //Determine source node id (NOP will be sent from this node, i.e. tester)
    if (lwr_idx == lwr_cnt)
    {
        src_node_id = node_id;
    }
    else
    {
        src_node_id = lwr[lwr_idx];
    }

    //Test node
    test_node = zwnode_find(&nw->ctl, test_node_id);
    if (!test_node)
    {
        return ZW_ERR_NODE_NOT_FOUND;
    }

    res = zwnode_get_desc(test_node, &test_node_desc);
    if (res != 0)
    {
        return res;
    }

    //Source node
    src_node = zwnode_find(&nw->ctl, src_node_id);
    if (!src_node)
    {
        return ZW_ERR_NODE_NOT_FOUND;
    }

    pwrlvl_if = zwif_find_cls(src_node->ep.intf, COMMAND_CLASS_POWERLEVEL);
    if (!pwrlvl_if)
    {
        return ZW_ERR_CLASS_NOT_FOUND;
    }

    zwif_get_desc(pwrlvl_if, &pwr_lvl_ifd);

    res = zwif_power_level_test_set(&pwr_lvl_ifd, &test_node_desc, 6, NHCHK_PWRLVL_FRM_CNT);

    *src_nid = src_node_id;

    return res;
}


/**
nhchk_pwrlvl_rpt_cb - report callback for power level test
@param[in]	ifd	        interface
@param[in]	node_id     Test node ID. If node id is 0, it means no test has been made and the rest of the
                        parameters should be ignored.
@param[in]	status      Status of the test operation. POWERLEVEL_TEST_XXX
@param[in]	frame_cnt   Number of frame count which has been acknowledged by the node as specified by node_id.
*/
void nhchk_pwrlvl_rpt_cb(zwifd_p ifd, uint8_t node_id, uint8_t status, uint16_t frame_cnt)
{
    zwnet_p nw = ifd->net;
    uint8_t data[16];

    data[0] = ifd->nodeid;//source node id that performed the test
    data[1] = status;
    data[2] = HI_OF_16BIT(frame_cnt);
    data[3] = LO_OF_16BIT(frame_cnt);

    plt_mtx_lck(nw->mtx);

    //Call state-machine
    nhchk_sm(nw, EVT_NH_CHK_PWRLVL_RPT, data);

    plt_mtx_ulck(nw->mtx);

}


/**
nhchk_pwrlvl_get - Get powerlevel node test report
@param[in]	nw          Network
@param[in]  src_nid     Source node id of the node that performed the powerlevel test
@param[in]  cb          Report callback function
@return	ZW_ERR_XXX
*/
static int nhchk_pwrlvl_get(zwnet_p nw, uint8_t src_nid, zwrep_power_level_test_fn cb)
{
    int         res;
    zwnode_p    src_node;
    zwif_p      pwrlvl_if;
    zwifd_t     pwr_lvl_ifd;

    //Source node
    src_node = zwnode_find(&nw->ctl, src_nid);
    if (!src_node)
    {
        return ZW_ERR_NODE_NOT_FOUND;
    }

    pwrlvl_if = zwif_find_cls(src_node->ep.intf, COMMAND_CLASS_POWERLEVEL);
    if (!pwrlvl_if)
    {
        return ZW_ERR_CLASS_NOT_FOUND;
    }

    zwif_get_desc(pwrlvl_if, &pwr_lvl_ifd);

    res = zwif_power_level_test_get(&pwr_lvl_ifd);

    return res;
}


/**
nhchk_sm - Network health check state-machine
@param[in] nw		Network
@param[in] evt      The event for the state-machine
@param[in] data     The data associated with the event
@return   Non-zero when the state-machine is started from idle state; otherwise return zero
@pre    Caller must lock the nw->mtx before calling this function
*/
int nhchk_sm(zwnet_p nw, zwnet_nh_chk_evt_t evt, uint8_t *data)
{
    static const uint8_t    stat_clr_cmd[] = {COMMAND_CLASS_NETWORK_MANAGEMENT_INSTALLATION_MAINTENANCE, STATISTICS_CLEAR};
    int                     res;
    nhchk_sm_ctx_t          *sm_ctx = &nw->nh_chk_sm_ctx;

    switch (sm_ctx->state)
    {
        //----------------------------------------------------------------
        case NH_CHK_STA_IDLE:
        //----------------------------------------------------------------
            if (evt == EVT_NH_CHK_START)
            {
                uint8_t i;
                uint8_t cnt;

                //Clear context
                memset(sm_ctx, 0, sizeof(nhchk_sm_ctx_t));
                sm_ctx->lwr_rssi_ok = 1;    //LWR RSSI measurement can be skipped for SDK version < 6.60
                sm_ctx->lwr_cnt = NHCHK_UNKNOWN_LWR;
                sm_ctx->ack_ch = NHCHK_UNKNOWN_ACK_CH;
                sm_ctx->tx_ch = NHCHK_UNKNOWN_TX_CH;

                //Send statistics clear command
                res = zwnet_ctlr_send(nw, (uint8_t *)stat_clr_cmd, 2, nhchk_stat_tx_sts_cb, sm_ctx);

                if (res == ZW_ERR_OP_FAILED)
                {
                    break;
                }

                //Save node list
                cnt = *data;

                for (i=0; i<cnt; i++)
                {
                    sm_ctx->health_rpt.sts[i].node_id = data[i + 1];
                }

                //Initialize state-machine
                sm_ctx->total_node = cnt;

                nw->sm_gen_retry = 0;

                //Change state
                sm_ctx->state = NH_CHK_STA_S_CLEAR;

                //Restart timer
                plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, nhchk_tout_cb, nw);

                return 1;//state-machine started successfully
            }
            break;

        //----------------------------------------------------------------
        case NH_CHK_STA_S_CLEAR:
        //----------------------------------------------------------------
            switch (evt)
            {
                case EVT_NH_CHK_RETRY:
                case EVT_NH_CHK_S_CLEAR:
                    {
                        int8_t *tx_sts = (int8_t *)data;

                        //Stop timer
                        plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                        nw->sm_tmr_ctx = NULL;

                        if ((evt == EVT_NH_CHK_RETRY) || (*tx_sts == TRANSMIT_COMPLETE_OK))
                        {   //Send the first NOP to node in the list

                            res = zwnode_probe_by_id(nw, sm_ctx->health_rpt.sts[sm_ctx->node_idx].node_id, nhchk_probe_cb, NULL, 0);
                            if (res < 0)
                            {
                                if ((res == ZW_ERR_BUSY) && (sm_ctx->busy_retry++ < NHCHK_MAX_BUSY_RETRY))
                                {   //Waiting to send failed, re-schedule the send NOP operation
                                    nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, NHCHK_BUSY_TIME, nhchk_busy_cb, nw);
                                    break;
                                }
                                //Failed
                                nhchk_failed(nw);
                                break;

                            }
                            //Send NOP o.k.
                            nw->sm_gen_retry = 0;
                            sm_ctx->busy_retry = 0;
                            sm_ctx->tx_cnt++;

                            //Change state
                            sm_ctx->state = NH_CHK_STA_SEND_NOP;

                            //Start timer
                            nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, nhchk_tout_cb, nw);

                            if ((sm_ctx->node_idx == 0) && !sm_ctx->notify_snd)
                            {
                                sm_ctx->notify_snd = 1;
                                zwnet_notify(nw, nw->curr_op, OP_HEALTH_CHK_STARTED, NULL);
                            }
                            break;
                        }
                    }

                    // Fall through

                case EVT_NH_CHK_TMOUT:
                    if (nw->sm_gen_retry++ < NHCHK_RETRY)
                    {   //Timeout, retry
                        //Send statistics clear command
                        res = zwnet_ctlr_send(nw, (uint8_t *)stat_clr_cmd, 2, nhchk_stat_tx_sts_cb, sm_ctx);

                        if (res == 0)
                        {
                            //Start timer
                            nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, nhchk_tout_cb, nw);
                            break;
                        }
                    }

                    nhchk_failed(nw);
                    break;

                default:
                    break;

            }
            break;

        //----------------------------------------------------------------
        case NH_CHK_STA_SEND_NOP:
        //----------------------------------------------------------------
            switch (evt)
            {
                case EVT_NH_CHK_RETRY:
                case EVT_NH_CHK_TX_STS:
                    {
                        uint8_t node_id;

                        node_id = sm_ctx->health_rpt.sts[sm_ctx->node_idx].node_id;

                        //Stop timer
                        plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                        nw->sm_tmr_ctx = NULL;

                        if (sm_ctx->tx_cnt >= NHCHK_NOP_PER_CHK)
                        {
                            int8_t          tx_sts;
                            zw_tx_cb_prm_t  *cb_param;

                            if (data)
                            {
                                tx_sts = (int8_t)data[1];

                                memcpy(&cb_param, data + 4, sizeof(zw_tx_cb_prm_t *));

                                //Store Last Working Route (LWR), RSSI array and ACK channel
                                if ((tx_sts == TRANSMIT_COMPLETE_OK) && (data[0] == node_id) && cb_param->ima_rpt_len)
                                {
                                    nhchk_ima_rpt_parse(nw, sm_ctx, cb_param->ima_rpt, cb_param->ima_rpt_len);
                                }
                            }

                            //Get statistics for network health check calculation
                            res = zw_ima_stat_get(&nw->appl_ctx, nhchk_stat_cb, node_id);
                            if (res < 0)
                            {
                                if (((res == SESSION_ERROR_PREVIOUS_COMMAND_UNCOMPLETED) || (res == APPL_ERROR_WAIT_CB))
                                    && (sm_ctx->busy_retry++ < NHCHK_MAX_BUSY_RETRY))
                                {   //Waiting to send failed, re-schedule the get statistics operation
                                    nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, NHCHK_BUSY_TIME, nhchk_busy_cb, nw);
                                    break;
                                }
                                //Failed
                                nhchk_failed(nw);

                                break;

                            }
                            //Send statistics get o.k.
                            nw->sm_gen_retry = 0;
                            sm_ctx->busy_retry = 0;

                            //Change state
                            sm_ctx->state = NH_CHK_STA_S_RPT;

                            //Start timer
                            nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, nhchk_tout_cb, nw);
                            break;
                        }

                        //Send the next NOP to node since NHCHK_NOP_PER_CHK of NOP must be sent in order to collect statistic
                        res = zwnode_probe_by_id(nw, node_id, nhchk_probe_cb, NULL,
                                                 (sm_ctx->tx_cnt == (NHCHK_NOP_PER_CHK - 1))? 1 : 0);

                        if (res < 0)
                        {
                            if ((res == ZW_ERR_BUSY) && (sm_ctx->busy_retry++ < NHCHK_MAX_BUSY_RETRY))
                            {   //Waiting to send failed, re-schedule the send NOP operation
                                nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, NHCHK_BUSY_TIME, nhchk_busy_cb, nw);
                                break;
                            }
                            //Failed
                            nhchk_failed(nw);

                            break;

                        }
                        //o.k.
                        nw->sm_gen_retry = 0;
                        sm_ctx->busy_retry = 0;
                        sm_ctx->tx_cnt++;

                        //Start timer
                        nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, nhchk_tout_cb, nw);

                    }
                    break;

                case EVT_NH_CHK_TMOUT:
                    if (nw->sm_gen_retry++ < NHCHK_RETRY)
                    {   //Timeout, retry
                        res = zwnode_probe_by_id(nw, sm_ctx->health_rpt.sts[sm_ctx->node_idx].node_id, nhchk_probe_cb, NULL,
                                                 (sm_ctx->tx_cnt >= NHCHK_NOP_PER_CHK)? 1 : 0);

                        if (res == 0)
                        {
                            //Start timer
                            nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, nhchk_tout_cb, nw);
                            break;
                        }
                    }

                    nhchk_failed(nw);
                    break;

                default:
                    break;

            }
            break;

        //----------------------------------------------------------------
        case NH_CHK_STA_S_RPT:
        //----------------------------------------------------------------
            switch (evt)
            {
                case EVT_NH_CHK_S_RPT:
                    {
                        uint8_t         node_id;
                        uint8_t         stat_cnt;
                        zw_ima_stat_t   *stat;

                        stat_cnt = data[1];

                        memcpy(&stat, data + 4, sizeof(zw_ima_stat_t *));

                        node_id = sm_ctx->health_rpt.sts[sm_ctx->node_idx].node_id;

                        if (node_id != data[0])
                        {
                            debug_zwapi_msg(&nw->plt_ctx, "stat report for node id:%u != expected node id:%u", data[0], node_id);
                            break;
                        }

                        //Stop timer
                        plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                        nw->sm_tmr_ctx = NULL;

                        res = nhchk_ima_stat_parse(stat, stat_cnt, &sm_ctx->ima_sta);

                        if (res < 0)
                        {
                            debug_zwapi_msg(&nw->plt_ctx, "nhchk_ima_stat_parse error:%d", res);
                            nhchk_failed(nw);
                            break;
                        }

                        //Check if all repeaters to the node as well as the node itself support Powerlevel command class
                        res = nhchk_pwrlvl_sup_chk(nw, sm_ctx->lwr, sm_ctx->lwr_cnt, node_id);

                        if (res == 0)
                        {   //Start powerlevel reduction test

                            sm_ctx->lwr_idx = 0;

                            res = nhchk_pwrlvl_test(nw, sm_ctx->lwr, sm_ctx->lwr_cnt, node_id, sm_ctx->lwr_idx, &sm_ctx->lwr_src_node);

                            if (res >= 0)
                            {
                                //Change state
                                sm_ctx->state = NH_CHK_STA_WAIT_PWRLVL_CMPT;

                                //Restart timer
                                plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                                nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, NHCHK_PWRLVL_WAIT_TIME, nhchk_pwrlvl_tout_cb, nw);

                                nw->sm_gen_retry = 0;
                                sm_ctx->busy_retry = 0;

                                break;
                            }
                            debug_zwapi_msg(&nw->plt_ctx, "nhchk_pwrlvl_test error:%d", res);
                        }
                        else
                        {
                            debug_zwapi_msg(&nw->plt_ctx, "nhchk_pwrlvl_sup_chk error:%d", res);
                        }

                        //Change state to calculate network health value
                        sm_ctx->state = NH_CHK_STA_WAIT_ALL_TEST;

                        nhchk_sm(nw, EVT_NH_CHK_START_CAL, NULL);
                        break;
                    }
                    break;

                case EVT_NH_CHK_TMOUT:
                    if (nw->sm_gen_retry++ < NHCHK_RETRY)
                    {   //Timeout, retry to get statistics for the node
                        res = zw_ima_stat_get(&nw->appl_ctx, nhchk_stat_cb, sm_ctx->health_rpt.sts[sm_ctx->node_idx].node_id);
                        if (res == 0)
                        {
                            //Start timer
                            nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, nhchk_tout_cb, nw);
                            break;
                        }
                    }

                    nhchk_failed(nw);
                    break;

                default:
                    break;

            }
            break;

        //----------------------------------------------------------------
        case NH_CHK_STA_WAIT_PWRLVL_CMPT:
        //----------------------------------------------------------------
            switch (evt)
            {
                case EVT_NH_CHK_PWRLVL_TMOUT:
                    {
                        //Get powerlevel test report
                        res = nhchk_pwrlvl_get(nw, sm_ctx->lwr_src_node, nhchk_pwrlvl_rpt_cb);

                        if (res >= 0)
                        {
                            //Change state
                            sm_ctx->state = NH_CHK_STA_WAIT_PWRLVL_RPT;

                            //Restart timer
                            plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                            nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, nhchk_tout_cb, nw);

                            break;
                        }
                        else
                        {
                            debug_zwapi_msg(&nw->plt_ctx, "nhchk_pwrlvl_get error:%d", res);
                        }

                        //Change state to calculate network health value
                        sm_ctx->state = NH_CHK_STA_WAIT_ALL_TEST;

                        nhchk_sm(nw, EVT_NH_CHK_START_CAL, NULL);
                    }
                    break;

                default:
                    break;

            }
            break;

        //----------------------------------------------------------------
        case NH_CHK_STA_WAIT_PWRLVL_RPT:
        //----------------------------------------------------------------
            switch (evt)
            {
                case EVT_NH_CHK_PWRLVL_RPT:
                    {
                        uint8_t     node_id;
                        uint8_t     src_node_id;//source node id that performed the test
                        uint8_t     status;
                        //uint16_t    frame_cnt;

                        src_node_id = data[0];
                        status = data[1];
                        //frame_cnt = U8_TO_16BIT(data + 2);

                        if (src_node_id != sm_ctx->lwr_src_node)
                        {
                            debug_zwapi_msg(&nw->plt_ctx, "Powertest result source node id:%u not equal to expected node id:%u", src_node_id, sm_ctx->lwr_src_node);
                            break;
                        }

                        node_id = sm_ctx->health_rpt.sts[sm_ctx->node_idx].node_id;

                        //Stop timer
                        plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                        nw->sm_tmr_ctx = NULL;

                        if (status == POWERLEVEL_TEST_SUCCES)
                        {
                            if (sm_ctx->lwr_idx >= sm_ctx->lwr_cnt)
                            {   //All the repeaters and destination node have at least 6 dB power reduction margin
                                sm_ctx->lwr_pwrlvl_ok = 1;

                                //Check if all the repeaters as well as destination node and controller node have firmware from SDK 6.60 or newer.
                                res = nhchk_rssi_sup_chk(nw, sm_ctx->lwr, sm_ctx->lwr_cnt, node_id);

                                if (res == 0)
                                {   //Get background RSSI value
                                    res = zw_gw_rssi_get(&nw->appl_ctx, nhchk_gw_rssi_cb);

                                    if (res == 0)
                                    {
                                        //Change state
                                        sm_ctx->state = NH_CHK_STA_BKGND_RSSI_RPT;

                                        //Restart timer
                                        plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                                        nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, NHCHK_RSSI_GET_TMOUT, nhchk_tout_cb, nw);

                                        nw->sm_gen_retry = 0;

                                        //Clear background RSSI array
                                        sm_ctx->bkgnd_rssi_idx = 0;
                                        memset(sm_ctx->bkgnd_rssi, RSSI_NOT_AVAILABLE, sizeof(sm_ctx->bkgnd_rssi));

                                        break;
                                    }
                                }
                                else
                                {
                                    ALOGW("nhchk_rssi_sup_chk failed, this node not support RSSI, nodeid: %d",node_id);
                                }

                                //Change state to calculate network health value
                                sm_ctx->state = NH_CHK_STA_WAIT_ALL_TEST;

                                nhchk_sm(nw, EVT_NH_CHK_START_CAL, NULL);
                                break;

                            }

                            //Start powerlevel reduction test for another repeater

                            sm_ctx->lwr_idx++;

                            res = nhchk_pwrlvl_test(nw, sm_ctx->lwr, sm_ctx->lwr_cnt, node_id, sm_ctx->lwr_idx, &sm_ctx->lwr_src_node);

                            if (res >= 0)
                            {
                                //Change state
                                sm_ctx->state = NH_CHK_STA_WAIT_PWRLVL_CMPT;

                                //Restart timer
                                plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                                nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, NHCHK_PWRLVL_WAIT_TIME, nhchk_pwrlvl_tout_cb, nw);

                                nw->sm_gen_retry = 0;
                                sm_ctx->busy_retry = 0;

                                break;
                            }
                            debug_zwapi_msg(&nw->plt_ctx, "nhchk_pwrlvl_test error:%d", res);
                        }
                        else if (status == POWERLEVEL_TEST_INPROGRESS)
                        {
                            if (sm_ctx->busy_retry++ < NHCHK_PWRLVL_WAIT_RETRY)
                            {
                                //Retry later

                                //Change state
                                sm_ctx->state = NH_CHK_STA_WAIT_PWRLVL_CMPT;

                                //Restart timer
                                plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                                nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, NHCHK_PWRLVL_WAIT_TIME, nhchk_pwrlvl_tout_cb, nw);
                                break;
                            }
                        }

                        //Change state to calculate network health value
                        sm_ctx->state = NH_CHK_STA_WAIT_ALL_TEST;

                        nhchk_sm(nw, EVT_NH_CHK_START_CAL, NULL);
                    }
                    break;

                case EVT_NH_CHK_TMOUT:
                    if (nw->sm_gen_retry++ < NHCHK_RETRY)
                    {   //Timeout, retry
                        //Get powerlevel test report
                        res = nhchk_pwrlvl_get(nw, sm_ctx->lwr_src_node, nhchk_pwrlvl_rpt_cb);

                        if (res >= 0)
                        {
                            //Restart timer
                            plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                            nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, nhchk_tout_cb, nw);

                            break;
                        }
                        else
                        {
                            debug_zwapi_msg(&nw->plt_ctx, "nhchk_pwrlvl_get error:%d", res);
                        }
                    }

                    //Change state to calculate network health value
                    sm_ctx->state = NH_CHK_STA_WAIT_ALL_TEST;

                    nhchk_sm(nw, EVT_NH_CHK_START_CAL, NULL);
                    break;

                default:
                    break;

            }
            break;

        //----------------------------------------------------------------
        case NH_CHK_STA_BKGND_RSSI_RPT:
        //----------------------------------------------------------------
            switch (evt)
            {
                case EVT_NH_CHK_GW_RSSI:
                    {
                        uint8_t rssi_cnt;
                        uint8_t i;

                        //Stop timer
                        plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                        nw->sm_tmr_ctx = NULL;

                        rssi_cnt = data[0];

                        //Store background RSSI of each channel
                        for (i=0; i<rssi_cnt; i++)
                        {
                            sm_ctx->bkgnd_rssi[i][sm_ctx->bkgnd_rssi_idx] = (int8_t)data[i+1];
                        }

                        if (sm_ctx->bkgnd_rssi_idx >= (NHCHK_BKGND_RSSI_CNT - 1))
                        {   //Have enough readings, calculate LWR RSSI
                            nhchk_rssi_cal(nw, sm_ctx);

                            //Change state to calculate network health value
                            sm_ctx->state = NH_CHK_STA_WAIT_ALL_TEST;

                            nhchk_sm(nw, EVT_NH_CHK_START_CAL, NULL);

                            break;

                        }

                        //Get another background RSSI reading
                        sm_ctx->bkgnd_rssi_idx++;
                        nw->sm_gen_retry = 0;

                        //At least 50 ms must pass between each measurement
                        plt_sleep(50);

                        zw_gw_rssi_get(&nw->appl_ctx, nhchk_gw_rssi_cb);

                        //Restart timer
                        plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                        nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, NHCHK_RSSI_GET_TMOUT, nhchk_tout_cb, nw);
                    }
                    break;

                case EVT_NH_CHK_TMOUT:
                    if (nw->sm_gen_retry++ < NHCHK_RETRY)
                    {   //Timeout, retry

                        zw_gw_rssi_get(&nw->appl_ctx, nhchk_gw_rssi_cb);

                        //Restart timer
                        plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                        nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, NHCHK_RSSI_GET_TMOUT, nhchk_tout_cb, nw);

                        break;
                    }

                    //Change state to calculate network health value
                    sm_ctx->state = NH_CHK_STA_WAIT_ALL_TEST;

                    nhchk_sm(nw, EVT_NH_CHK_START_CAL, NULL);
                    break;

                default:
                    break;

            }
            break;

        //----------------------------------------------------------------
        case NH_CHK_STA_WAIT_ALL_TEST:
        //----------------------------------------------------------------
            switch (evt)
            {
                case EVT_NH_CHK_START_CAL:
                    {
                        uint8_t node_id;
                        uint8_t nhv;
                        uint8_t sts_cat;

                        node_id = sm_ctx->health_rpt.sts[sm_ctx->node_idx].node_id;

                        nhchk_val_cal(nw, &sm_ctx->ima_sta, sm_ctx->lwr_pwrlvl_ok, sm_ctx->lwr_rssi_ok, &nhv, &sts_cat);

                        //Save the calculated network health value
                        ALOGI("node:%u, nhv=%u, sts_cat=%u", node_id, nhv, sts_cat);
                        sm_ctx->nhv_sum += nhv;
                        sm_ctx->chk_cnt++;

                        //Clear variables to prepare for next round of network health check calculation
                        nw->sm_gen_retry = 0;
                        sm_ctx->busy_retry = 0;
                        sm_ctx->tx_cnt = 0;
                        sm_ctx->lwr_pwrlvl_ok = 0;
                        sm_ctx->lwr_rssi_ok = 1;    //LWR RSSI measurement can be skipped for SDK version < 6.60
                        sm_ctx->ima_sta.tc = sm_ctx->ima_sta.per = 0;
                        sm_ctx->rssi_cnt = 0;
                        sm_ctx->lwr_cnt = NHCHK_UNKNOWN_LWR;
                        sm_ctx->ack_ch = NHCHK_UNKNOWN_ACK_CH;
                        sm_ctx->tx_ch = NHCHK_UNKNOWN_TX_CH;


                        if (sm_ctx->chk_cnt >= NHCHK_CHK_PER_NODE)
                        {   //Done for this node, calculate the average and store the result
                            zwnet_sts_t net_sts_prog = {0};

                            nhchk_val_ave_store(sm_ctx);
                            //Increment statistic count
                            sm_ctx->health_rpt.sts_cnt++;

                            //Point to next node
                            sm_ctx->node_idx++;

                            net_sts_prog.type = ZWNET_STS_INFO_HEALTH_CHK_PRG;
                            net_sts_prog.info.health_chk_progress.node_cnt = sm_ctx->node_idx;
                            net_sts_prog.info.health_chk_progress.total = sm_ctx->total_node;
                            zwnet_notify(nw, nw->curr_op, OP_HEALTH_CHK_PROGRESS, &net_sts_prog);

                            if (sm_ctx->node_idx >= sm_ctx->total_node)
                            {   //Done for all nodes
                                zwnet_sts_t net_sts = {0};

                                net_sts.type = ZWNET_STS_INFO_HEALTH_CHK_RPT;
                                memcpy(&net_sts.info.health_chk_rpt, &sm_ctx->health_rpt, sizeof(zw_health_rpt_t));

                                sm_ctx->health_rpt.sts_cnt = sm_ctx->total_node;

                                zwnet_notify(nw, nw->curr_op, OP_HEALTH_CHK_CMPLT, &net_sts);
                                zwnet_notify(nw, nw->curr_op, OP_DONE, NULL);

                                nw->curr_op = ZWNET_OP_NONE;

                                sm_ctx->state = NH_CHK_STA_IDLE;

                                break;
                            }

                            //Do the network health check for the next node
                            sm_ctx->chk_cnt = 0;
                            sm_ctx->nhv_sum = 0;
                        }
                        //Start another round of network health check as each node is required to go through NHCHK_CHK_PER_NODE rounds
                        //of network health check in order to calculate the average value.

                        //Send statistics clear command
                        res = zwnet_ctlr_send(nw, (uint8_t *)stat_clr_cmd, 2, nhchk_stat_tx_sts_cb, sm_ctx);

                        if (res == ZW_ERR_OP_FAILED)
                        {
                            nhchk_failed(nw);
                            break;
                        }
                        //Change state
                        sm_ctx->state = NH_CHK_STA_S_CLEAR;

                        //Restart timer
                        plt_tmr_stop(&nw->plt_ctx, nw->sm_tmr_ctx);
                        nw->sm_tmr_ctx = plt_tmr_start(&nw->plt_ctx, ZWNET_IP_NW_TMOUT, nhchk_tout_cb, nw);

                    }
                    break;

                default:
                    break;

            }
            break;

    }

    return 0;

}



/**
@}
*/






