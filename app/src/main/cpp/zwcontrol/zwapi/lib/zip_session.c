/**
@file   zip_session.c - Z/IP host controller interface session layer implementation.

        To handle commands sending and the corresponding responses and/or callback.

@author David Chow

@version    1.0 7-6-11  Initial release

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
#include <sys/types.h>
#include "../include/zip_session.h"

/**
@defgroup Session Session layer APIs
To handle commands sending and the corresponding responses and/or callback.
@{
*/


/**
ssn_cmd_snd - Send a command
@param[in,out] ssn_ctx      Context.
@param[in] param            The parameters related to the command.
@return  0 on success, negative error number on failure
*/
int ssn_cmd_snd(ssn_layer_ctx_t *ssn_ctx, ssn_cmd_snd_param_t *param)
{
    int                 ret_val;
    frm_snd_prm_t       frm_param = {0};

    if (!ssn_ctx->cb_thrd_run)
    {
        return ZWHCI_ERROR_SHUTDOWN;
    }

    plt_mtx_lck(ssn_ctx->wr_mtx);

    //Check whether the previous command has completed
    if (param->sts_cb)
    {   //Current command requires send status callback
        if (ssn_ctx->sts_cb)
        {
            plt_mtx_ulck(ssn_ctx->wr_mtx);
            return SESSION_ERROR_PREVIOUS_COMMAND_UNCOMPLETED;
        }
    }

    //Send to frame layer
    frm_param.dst = param->dst;
    frm_param.seq_num = param->seq_num;
    frm_param.cmd_buf = param->cmd_buf;
    frm_param.cmd_len = param->cmd_len;
    frm_param.ep_addr = param->ep_addr;
    frm_param.flag = param->flag;
    frm_param.encap_fmt = param->encap_fmt;
    frm_param.wkup_poll = param->wkup_poll;

    //Check whether ACK request is required
    if (param->sts_cb)
    {
        frm_param.flag |= ZIP_FLAG_ACK_REQ;
    }

    ret_val = frm_send(&ssn_ctx->frm_ctx, &frm_param);

    //Save the send status callback function and other information
    if ((ret_val == 0) && param->sts_cb)
    {
        ssn_ctx->sts_cb = param->sts_cb;
        ssn_ctx->cb_prm = param->cb_prm;
        ssn_ctx->node_id = param->node_id;
        ssn_ctx->seq_num = param->seq_num;
    }

    plt_mtx_ulck(ssn_ctx->wr_mtx);
    return ret_val;
}


/**
ssn_rx_frm_cb - Callback from frame layer whenever a complete frame is received
@param[in] frm_ctx      Frame layer context.
@param[in] buf          Buffer that stores the received frame
@param[in] dat_len      Number of bytes received
@param[in] src_addr     Source address of sender
@param[in] ep_addr      Source and destination endpoint addresses
@param[in] flag         Flag, see ZIP_FLAG_XXX
@param[in] encap_fmt    Z-wave encapsulation format (valid if flag ZIP_FLAG_ENCAP_INFO is set).
                        This format should be passed back when responding
                        to unsolicited command with the flag ZIP_FLAG_ENCAP_INFO is set.
@return
*/
static void    ssn_rx_frm_cb(struct _frm_layer_ctx *frm_ctx,
                             uint8_t *buf, uint16_t dat_len,
                             struct sockaddr *src_addr, frm_ep_t *ep_addr,
                             uint16_t flag, uint16_t encap_fmt)
{
    ssn_layer_ctx_t     *ssn_ctx = frm_ctx->ssn_layer_ctx;
    int32_t             ret_val;  //Return value
    ssn_cb_req_t        *cb_req;  //callback request


    //Build a callback request for the callback thread to execute
    cb_req = (ssn_cb_req_t *)calloc(1, sizeof(ssn_cb_req_t) + dat_len);

    if (!cb_req)
    {
        return;
    }

    cb_req->len = dat_len;
    cb_req->flag = flag;
    cb_req->encap_fmt = encap_fmt;
    if (frm_ctx->tpt_ctx.use_ipv4)
    {
        cb_req->src_addr.ipv4 = *((struct sockaddr_in *)src_addr);
    }
    else
    {
        cb_req->src_addr.ipv6 = *((struct sockaddr_in6 *)src_addr);
    }
    cb_req->ep_addr = *ep_addr;
    memcpy(cb_req->dat_buf, buf, dat_len);

    //Submit the callback request
    ret_val = util_list_add(ssn_ctx->cb_thrd_mtx, &ssn_ctx->cb_req_hd,
                             (uint8_t *)cb_req, sizeof(ssn_cb_req_t) + dat_len);

    free(cb_req);

    if (ret_val < 0)
    {
        return;
    }
    plt_sem_post(ssn_ctx->cb_thrd_sem);

}


/**
ssn_snd_frm_sts_cb - Send frame status callback from frame layer
@param[in]  frm_ctx  Frame layer context.
@param[in]  status   The status of sending a frame.
@param[in]	seq_num  Z/IP frame sequence number
@param[in]  src_addr Z/IP packet source address
@param[in]	ima_rpt_len	Z/IP header extension IMA report length
@param[in]	ima_rpt	    Buffer to store the Z/IP header extension IMA report; this field is valid only if ima_rpt_len > 0
@return
*/
static void    ssn_snd_frm_sts_cb(struct _frm_layer_ctx *frm_ctx, frm_snd_sts_t status, uint8_t seq_num,
                                  struct sockaddr *src_addr, uint8_t ima_rpt_len, uint8_t *ima_rpt)
{
    ssn_layer_ctx_t     *ssn_ctx = frm_ctx->ssn_layer_ctx;
    snd_sts_cb_t        sts_cb;
    void                *cb_prm;
    int8_t              send_frm_sts;
    uint8_t             node_id;

    plt_mtx_lck(ssn_ctx->wr_mtx);
    if ((ssn_ctx->seq_num == seq_num) && ssn_ctx->sts_cb)
    {   //current frame status
        sts_cb = ssn_ctx->sts_cb;
        cb_prm = ssn_ctx->cb_prm;
        node_id = ssn_ctx->node_id;

        ssn_ctx->sts_cb = NULL;
    }
    else    //old frame status
    {
        sts_cb = ssn_ctx->old_sts_cb;
        cb_prm = ssn_ctx->old_cb_prm;
        node_id = 0xFF;

    }
    plt_mtx_ulck(ssn_ctx->wr_mtx);

    send_frm_sts = ZWHCI_NO_ERROR;

    switch (status)
    {
        case FRAME_SEND_OK:
            //send_frm_sts = ZWHCI_NO_ERROR;
            break;

        case FRAME_SEND_TIMEOUT://Send frame timeout due to no ACK received
            send_frm_sts = SESSION_ERROR_SND_FRM_TMOUT;
            break;

        case FRAME_SEND_NACK:
            send_frm_sts = SESSION_ERROR_UNREACHABLE;
            break;

        case FRAME_SEND_BUSY:
            send_frm_sts = SESSION_ERROR_DEST_BUSY;
            break;

        case FRAME_SEND_SYSTEM:
        case FRAME_SEND_UNKNOWN:    //Should not occur
            send_frm_sts = SESSION_ERROR_SYSTEM;
            break;
    }

    if (sts_cb)
    {
        sts_cb(ssn_ctx, send_frm_sts, cb_prm, node_id, seq_num, src_addr, ima_rpt_len, ima_rpt);
    }
}


/**
ssn_cb_thrd - Thread to serve the callback requests
@param[in]	data		The session context
@return
*/
static void ssn_cb_thrd(void   *data)
{
    ssn_layer_ctx_t *ssn_ctx = (ssn_layer_ctx_t *)data;
    util_lst_t      *cb_req_lst;
    ssn_cb_req_t    *cb_req;
    struct sockaddr *sock_addr;
    int             use_ipv4;

    ssn_ctx->cb_thrd_sts = 1;

    use_ipv4 = ssn_ctx->frm_ctx.tpt_ctx.use_ipv4;

    while (1)
    {
        //Wait for callback request
        plt_sem_wait(ssn_ctx->cb_thrd_sem);

        //Check whether to exit the thread
        if (ssn_ctx->cb_thrd_run == 0)
        {
            ssn_ctx->cb_thrd_sts = 0;
            return;
        }

        cb_req_lst = util_list_get(ssn_ctx->cb_thrd_mtx, &ssn_ctx->cb_req_hd);

        //Callback
        if (cb_req_lst)
        {
            cb_req = (ssn_cb_req_t *)cb_req_lst->wr_buf;

            if (use_ipv4)
            {
                sock_addr = (struct sockaddr *)&cb_req->src_addr.ipv4;
            }
            else
            {
                sock_addr = (struct sockaddr *)&cb_req->src_addr.ipv6;
            }

            if (ssn_ctx->cb_thrd_run)
            {
                ssn_ctx->unsolicited_cmd_cb(ssn_ctx, sock_addr, &cb_req->ep_addr, cb_req->dat_buf, cb_req->len,
                                            cb_req->flag, cb_req->encap_fmt);
            }

            free(cb_req_lst);
        }
    }
}


/**
ssn_init - Init the session layer.
Should be called once before calling the other session layer functions
@param[in,out]	ssn_ctx		Context
@return     Return zero indicates success, non-zero indicates failure.
@post       Caller should not modify the context after this call
*/
int ssn_init(ssn_layer_ctx_t   *ssn_ctx)
{
    int     ret_val;

    //Init frame layer
    ssn_ctx->frm_ctx.snd_frm_sts_cb = ssn_snd_frm_sts_cb;
    ssn_ctx->frm_ctx.rx_frm_cb = ssn_rx_frm_cb;
    ssn_ctx->frm_ctx.ssn_layer_ctx = ssn_ctx;
    ssn_ctx->frm_ctx.plt_ctx = ssn_ctx->plt_ctx;
    ret_val = frm_init(&ssn_ctx->frm_ctx);

    if (ret_val != 0)
        return ret_val;

    //Init session layer
    ssn_ctx->sts_cb = NULL;

    if (!plt_mtx_init(&ssn_ctx->wr_mtx))
        goto l_SESSION_INIT_ERROR;

    if (!plt_mtx_init(&ssn_ctx->cb_thrd_mtx))
        goto l_SESSION_INIT_ERROR1;

    if (!plt_sem_init(&ssn_ctx->cb_thrd_sem))
        goto l_SESSION_INIT_ERROR2;


    ssn_ctx->cb_thrd_run = 1;
    if (plt_thrd_create(ssn_cb_thrd, ssn_ctx) < 0)
        goto l_SESSION_INIT_ERROR3;


    return 0;


l_SESSION_INIT_ERROR3:
    plt_sem_destroy(ssn_ctx->cb_thrd_sem);

l_SESSION_INIT_ERROR2:
    plt_mtx_destroy(ssn_ctx->cb_thrd_mtx);

l_SESSION_INIT_ERROR1:
    plt_mtx_destroy(ssn_ctx->wr_mtx);

l_SESSION_INIT_ERROR:
    frm_exit(&ssn_ctx->frm_ctx);

    return INIT_ERROR_SESSION;
}


/**
ssn_exit - Clean up the session layer
@param[in,out]	ssn_ctx		Context
@return
*/
void ssn_exit(ssn_layer_ctx_t   *ssn_ctx)
{

    frm_exit(&ssn_ctx->frm_ctx);

    //Stop the thread
    ssn_ctx->cb_thrd_run = 0;
    plt_sem_post(ssn_ctx->cb_thrd_sem);

    //Wait until the received packet is processed
    while (ssn_ctx->cb_thrd_sts)
    {
        plt_sleep(100);
    }
    //Extra time for the thread to fully exit
    //Needed to avoid program crash if calling init and exit in a very short interval
    plt_sleep(100);


    //Flush lists
    util_list_flush(ssn_ctx->cb_thrd_mtx, &ssn_ctx->cb_req_hd);

    plt_sem_destroy(ssn_ctx->cb_thrd_sem);
    plt_mtx_destroy(ssn_ctx->cb_thrd_mtx);
    plt_mtx_destroy(ssn_ctx->wr_mtx);

}


/**
ssn_old_snd_status_cb_set - Set callback function to receive old (non current) send status
@param[in]	ssn_ctx     Context
@param[in]	sts_cb      Status callback function
@param[in]	usr_prm     Use-defined parameter when callback
@return
*/
void ssn_old_snd_status_cb_set(ssn_layer_ctx_t *ssn_ctx, snd_sts_cb_t sts_cb, void *usr_prm)
{
    plt_mtx_lck(ssn_ctx->wr_mtx);
    ssn_ctx->old_sts_cb = sts_cb;
    ssn_ctx->old_cb_prm = usr_prm;
    plt_mtx_ulck(ssn_ctx->wr_mtx);
}


/**
@}
*/


