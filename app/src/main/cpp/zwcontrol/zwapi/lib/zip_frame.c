/**
@file   zip_frame.c - Z/IP host controller interface frame layer implementation.

        To encapsulate Z-wave command into Z/IP packet command for sending.
        Also, decapsulate Z/IP packet command into Z-wave command and status .

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
#include "../include/zip_frame.h"
#include "../include/zwave/ZW_classcmd.h"


/**
@defgroup Frame Frame layer APIs
Assemble frame from the bytes received from transport layer
and handling of frame resend if required.
@{
*/


/**
frm_snd_tmout_cb - Send frame timeout callback
@param[in] data     Pointer to the frm_layer_ctx_t
@return
*/
static void frm_snd_tmout_cb(void *data)
{
    frm_layer_ctx_t     *frm_ctx = (frm_layer_ctx_t *)data;
    struct sockaddr     *sock_addr;
    int                 result;
    frm_snd_sts_t       status;
    uint8_t             seq_num;

    plt_mtx_lck(frm_ctx->wr_mtx);

    //Stop the send timer
    plt_tmr_stop(frm_ctx->plt_ctx, frm_ctx->snd_tmr_ctx);
    frm_ctx->snd_tmr_ctx = 0;

    if (!frm_ctx->run)
    {
        plt_mtx_ulck(frm_ctx->wr_mtx);
        return;
    }

    //Check number of resend has been exceeded
    if (++frm_ctx->resend_cnt > FRAME_MAX_RESEND)
    {
        status = FRAME_SEND_TIMEOUT;
        goto l_RPT_FRAME_ERROR;
    }

    //Check the buffer is valid
    if (!frm_ctx->resnd_frm_buf)
    {
        status = FRAME_SEND_TIMEOUT;
        goto l_RPT_FRAME_ERROR;
    }

    //Resend the ZIP packet
    if (frm_ctx->tpt_ctx.use_ipv4)
    {
        sock_addr = (struct sockaddr *)&frm_ctx->resnd_dst.ipv4;
    }
    else
    {
        sock_addr = (struct sockaddr *)&frm_ctx->resnd_dst.ipv6;
    }

    if (frm_ctx->frm_rcvd)
    {   //Z/IP gateway has already received the frame, don't need to resend
        result = ZWHCI_NO_ERROR;
    }
    else
    {
        result = tpt_snd_req_create(&frm_ctx->tpt_ctx, sock_addr, NULL,
                                    frm_ctx->resnd_frm_buf, frm_ctx->resnd_frm_len,
                                    (frm_ctx->resnd_frm_wkup_poll || frm_ctx->resnd_host_port)? TPT_SEND_FLG_USE_HOST_PORT :
                                    (frm_ctx->resnd_new_dtls)? TPT_SEND_FLG_NEW_DTLS_SESS : 0);
    }

    if (result == ZWHCI_NO_ERROR)
    {
        //Restart the send timer
        frm_ctx->snd_tmr_ctx = plt_tmr_start(frm_ctx->plt_ctx, frm_ctx->resnd_tmout,
                                             frm_snd_tmout_cb, frm_ctx);

        if (!frm_ctx->snd_tmr_ctx)
        {
            status = FRAME_SEND_SYSTEM;
            goto l_RPT_FRAME_ERROR;
        }
    }
    else
    {
        status = FRAME_SEND_SYSTEM;
        goto l_RPT_FRAME_ERROR;
    }
    plt_mtx_ulck(frm_ctx->wr_mtx);
    return;

l_RPT_FRAME_ERROR:
    //Stop resend
    if (frm_ctx->resnd_frm_buf)
    {
        seq_num = frm_ctx->resnd_frm_buf[4];
        free(frm_ctx->resnd_frm_buf);
        frm_ctx->resnd_frm_buf = NULL;
        frm_ctx->resend_cnt = 0;
    }
    else
    {
        seq_num = 0;
    }

    plt_mtx_ulck(frm_ctx->wr_mtx);

    //Call back session layer
    frm_ctx->snd_frm_sts_cb(frm_ctx, status, seq_num, NULL, 0, NULL);

}


/**
frm_send - Send a frame through transport layer
@param[in] frm_ctx      Context
@param[in] prm          Parameter
@return  0 on success, negative error number on failure
*/
int frm_send(frm_layer_ctx_t *frm_ctx, frm_snd_prm_t *prm)
{
    uint8_t     *zip_frm;
    uint16_t    frm_len;
    uint16_t    tpt_snd_flag = 0;
    int         result;

    if (!frm_ctx->run)
    {
        return ZWHCI_ERROR_SHUTDOWN;
    }

    plt_mtx_lck(frm_ctx->wr_mtx);

    //Check whether any write operation still pending
    if (frm_ctx->resnd_frm_buf
        && (prm->flag & ZIP_FLAG_ACK_REQ))
    {
        plt_mtx_ulck(frm_ctx->wr_mtx);
        return FRAME_ERROR_MULTIPLE_WRITE;
    }

    frm_len = ZIP_FRM_HDR_SIZE + ((prm->cmd_buf)? prm->cmd_len : 0)
              + ((prm->flag & ZIP_FLAG_ENCAP_INFO)? ZIP_FRM_ENCAP_FMT_SZ : 0)
              + ((prm->flag & ZIP_FLAG_IMA_GET)? ZIP_FRM_IMA_GET_SZ : 0);

    //Check for header extension
    if (prm->flag & (ZIP_FLAG_ENCAP_INFO | ZIP_FLAG_IMA_GET))
    {
        frm_len += ZIP_FRM_EXT_TOTAL_SZ;
    }

    zip_frm = (uint8_t *)malloc(frm_len);

    if (!zip_frm)
    {
        plt_mtx_ulck(frm_ctx->wr_mtx);
        return ZWHCI_ERROR_MEMORY;
    }

    //Fill in the frame
    zip_frm[0] = COMMAND_CLASS_ZIP;
    zip_frm[1] = COMMAND_ZIP_PACKET;
    zip_frm[2] = (prm->flag & ZIP_FLAG_ACK_REQ)? ZIP_FRM_ACK_REQ : 0;

    if (prm->flag & ZIP_FLAG_ACK_RESP)
    {
        zip_frm[2] |= ZIP_FRM_ACK_RESP;
    }
    else if (prm->flag & ZIP_FLAG_NACK_OPT_RESP)
    {
        zip_frm[2] |= (ZIP_FRM_NACK_RESP | ZIP_FRM_OP_ERR_RESP);
    }

    zip_frm[3] = (prm->flag & ZIP_FLAG_SECURE)? ZIP_FRM_SECURE : 0;
    if (prm->flag & ZIP_FLAG_MORE_INFO)
    {
        zip_frm[3] |= ZIP_FRM_MORE_INFO;
    }

    if (prm->cmd_buf && (prm->cmd_len > 0))
    {
        uint16_t content_offset = 0;
        //Check whether the content (prm->cmd_buf) is header extension or z-wave command
        if (prm->flag & ZIP_FLAG_HDR_EXT)
        {   //content is header extension
            zip_frm[3] |= ZIP_FRM_HDR_EXT;
        }
        else
        {   //content is z-wave command
            uint8_t *opt_ptr;

            opt_ptr = zip_frm + ZIP_FRM_HDR_SIZE + ZIP_FRM_EXT_TOTAL_SZ;

            zip_frm[3] |= ZIP_FRM_ZWAVE_CMD;

            //Add header extension for encapsulation info
            if (prm->flag & ZIP_FLAG_ENCAP_INFO)
            {
                content_offset += ZIP_FRM_ENCAP_FMT_SZ;

                *opt_ptr++ = ZIP_OPT_ENCAP_FMT | 0x80; //Option is critical
                *opt_ptr++ = 2; //Option content length
                *opt_ptr++ = (uint8_t)(prm->encap_fmt >> 8);
                *opt_ptr++ = (uint8_t)(prm->encap_fmt & 0x00FF);
            }

            //Add header extension to request IMA report
            if (prm->flag & ZIP_FLAG_IMA_GET)
            {
                content_offset += ZIP_FRM_IMA_GET_SZ;

                *opt_ptr++ = ZIP_OPT_IMA_GET; //Option is optional
                *opt_ptr++ = 0; //Option content length
            }

            //Have at least one header extension
            if (content_offset)
            {
                zip_frm[3] |= ZIP_FRM_HDR_EXT;
                content_offset += ZIP_FRM_EXT_TOTAL_SZ;
                zip_frm[ZIP_FRM_HDR_SIZE] = content_offset; //Header extension total size
            }
        }

        //Copy the content
        memcpy(zip_frm + ZIP_FRM_HDR_SIZE + content_offset, prm->cmd_buf, prm->cmd_len);
    }

    zip_frm[4] = prm->seq_num;
    zip_frm[5] = prm->ep_addr.src_ep;
    zip_frm[6] = (prm->ep_addr.dst_ep_type == 0)? prm->ep_addr.dst_ep : (prm->ep_addr.dst_ep | 0x80);

    if (prm->src_addr)
    {
        tpt_snd_flag |= TPT_SEND_FLG_USE_SRC_ADDR;
    }

    if (prm->wkup_poll || (prm->flag & ZIP_FLAG_HOST_PORT))
    {
        tpt_snd_flag |= TPT_SEND_FLG_USE_HOST_PORT;
    }

    result = tpt_snd_req_create(&frm_ctx->tpt_ctx, prm->dst, prm->src_addr, zip_frm, frm_len, tpt_snd_flag);

    if (result == 0)
    {
        frm_ctx->resnd_frm_wkup_poll = prm->wkup_poll;
        frm_ctx->frm_rcvd = 0;
        frm_ctx->resnd_new_dtls = (prm->flag & ZIP_FLAG_NO_NEW_DTLS)? 0 : 1;
        frm_ctx->resnd_host_port = (prm->flag & ZIP_FLAG_HOST_PORT)? 0 : 1;

        if (prm->flag & ZIP_FLAG_ACK_REQ)
        {   //Save a copy of the frame for resend if ACK is not received
            if (frm_ctx->resnd_frm_buf)
            {
                free(frm_ctx->resnd_frm_buf);
            }
            frm_ctx->resnd_frm_buf = zip_frm;
            frm_ctx->resnd_frm_len = frm_len;
            if (frm_ctx->tpt_ctx.use_ipv4)
            {
                frm_ctx->resnd_dst.ipv4 = *((struct sockaddr_in *)prm->dst);
            }
            else
            {
                frm_ctx->resnd_dst.ipv6 = *((struct sockaddr_in6 *)prm->dst);
            }
            frm_ctx->resend_cnt = 0;

            //Start send timer
            frm_ctx->snd_tmr_ctx = plt_tmr_start(frm_ctx->plt_ctx, frm_ctx->resnd_tmout, frm_snd_tmout_cb, frm_ctx);
        }
        else
        {   //Frame is not requesting ACK
            free(zip_frm);
        }

    }
    else
    {   //Failed to send to transport layer
        free(zip_frm);
    }
    plt_mtx_ulck(frm_ctx->wr_mtx);

    return result;
}


#ifdef ZIP_V2
/**
frm_keep_alive_resp_send - Send Z/IP keep alive response frame through transport layer
@param[in] frm_ctx      Context
@param[in] dst_addr     Destination address
@param[in] flag         Flag, see ZIP_FLAG_XXX
@return  0 on success, negative error number on failure
*/
static int frm_keep_alive_resp_send(frm_layer_ctx_t *frm_ctx, struct sockaddr *dst_addr, uint16_t flag)
{
    uint8_t     zip_frm[ZIP_FRM_KEEP_ALIVE_SZ];
    int         result;

    if (!frm_ctx->run)
    {
        return ZWHCI_ERROR_SHUTDOWN;
    }

    plt_mtx_lck(frm_ctx->wr_mtx);

    //Fill in the frame
    zip_frm[0] = COMMAND_CLASS_ZIP;
    zip_frm[1] = COMMAND_ZIP_KEEP_ALIVE;
    zip_frm[2] = 0x40;

    result = tpt_snd_req_create(&frm_ctx->tpt_ctx, dst_addr, NULL, zip_frm, ZIP_FRM_KEEP_ALIVE_SZ,
                                (flag & ZIP_FLAG_HOST_PORT)? TPT_SEND_FLG_USE_HOST_PORT : 0);

    plt_mtx_ulck(frm_ctx->wr_mtx);

    return result;
}
#endif


/**
frm_hdr_ext_get - Get frame header extension option
@param[in] buf      Buffer that stores the frame header extension
@param[in] dat_len  Number of bytes in buf
@param[in] opt      The option to get
@return             Pointer to the option type; NULL if the option is not found or error
*/
static uint8_t *frm_hdr_ext_get(uint8_t *buf, uint16_t data_len, uint8_t opt)
{
    uint8_t     *opt_ptr;
    uint16_t    hdr_ext_len;
    uint8_t     opt_len;

    if (data_len)
    {
        hdr_ext_len = buf[0];

        if ((hdr_ext_len <= data_len) && (hdr_ext_len >= 3))
        {
            hdr_ext_len--; //minus the first byte which is the "total header extension size" field
            opt_ptr = buf + 1;

            while (hdr_ext_len >= 2)
            {   //Check for option regardless of critical bit
                if ((*opt_ptr++ & ~0x80) == opt)
                {
                    //check the length field
                    if ((*opt_ptr + 2) <= hdr_ext_len)
                    {
                        return (opt_ptr - 1);
                    }
                    break;
                }

                opt_len = *opt_ptr;
                //Point to next option
                opt_ptr += (opt_len + 1);

                if ((opt_len + 2) < hdr_ext_len)
                {
                    hdr_ext_len -= (opt_len + 2);
                }
                else
                {
                    break;
                }
            }
        }
    }
    return NULL;
}


/**
frm_hdr_ext_chk - Check frame header extension for unrecognized critical option
@param[in] buf      Buffer that stores the frame header extension
@param[in] dat_len  Number of bytes in buf
@return             Pointer to the unrecognized option; NULL if o.k.
*/
static uint8_t *frm_hdr_ext_chk(uint8_t *buf, uint16_t data_len)
{
    uint8_t     *opt_ptr;
    uint16_t    hdr_ext_len;
    uint8_t     opt_len;

    if (data_len)
    {
        hdr_ext_len = buf[0];

        if ((hdr_ext_len <= data_len) && (hdr_ext_len >= 3))
        {
            hdr_ext_len--; //minus the first byte which is the total header extension length field
            opt_ptr = buf + 1;

            while (hdr_ext_len >= 2)
            {
                if (*opt_ptr++ & 0x80)
                {
                    //check the length field
                    if ((*opt_ptr + 2) <= hdr_ext_len)
                    {
                        uint8_t critical_opt;

                        critical_opt = *(opt_ptr - 1) & ~0x80;
                        //return error if option is unrecognized
                        if (critical_opt != ZIP_OPT_ENCAP_FMT)
                        {
                            return (opt_ptr - 1);
                        }
                    }
                    break;
                }

                opt_len = *opt_ptr;
                //Point to next option
                opt_ptr += (opt_len + 1);

                if ((opt_len + 2) < hdr_ext_len)
                {
                    hdr_ext_len -= (opt_len + 2);
                }
                else
                {
                    break;
                }
            }
        }
    }
    return NULL;
}


/**
frm_tpt_rd_cb - Frame received callback from transport layer
@param[in] tpt_ctx  Transport layer context
@param[in] buf      Buffer that stores the received bytes
@param[in] dat_len  Number of bytes received
@param[in] src_addr Z/IP packet source address
@param[in] dst_addr Optional destination address (as in the packet header) of the received packet
@return
*/
static void    frm_tpt_rd_cb(struct _tpt_layer_ctx *tpt_ctx, uint8_t *buf,
                             uint16_t data_len, struct sockaddr *src_addr, uint8_t *dst_addr)
{
    frm_layer_ctx_t     *frm_ctx = tpt_ctx->frm_layer_ctx;

    if (!frm_ctx->run)
    {
        return;
    }

    if (buf[0] == COMMAND_CLASS_ZIP)
    {
        uint16_t rcx_flag;

        //Check whether the packet is received from host listening socket
        if (tpt_ctx->use_ipv4)
        {
            struct sockaddr_in *ipv4_addr = (struct sockaddr_in *)src_addr;
            rcx_flag = (ipv4_addr->sin_port)? ZIP_FLAG_HOST_PORT : 0;
        }
        else
        {
            struct sockaddr_in6 *ipv6_addr = (struct sockaddr_in6 *)src_addr;
            rcx_flag = (ipv6_addr->sin6_port)? ZIP_FLAG_HOST_PORT : 0;
        }

        if ((data_len >= ZIP_FRM_HDR_SIZE) && (buf[1] == COMMAND_ZIP_PACKET))
        {   //ZIP packet

            int             send_ack;           //Flag to determine whether to send ACK as response
            int             cb_sts;             //Flag to determine whether to callback session layer to report status
            int             src_addr_matched;   //Flag to determine whether the source address matches previously sent packet
            frm_snd_sts_t   snd_sts;

            //Check for ACK Request
            if (buf[2] & ZIP_FRM_ACK_REQ)
            {
                send_ack = 1;
            }
            else    //Check for response
            {
                uint8_t ima_rpt_len = 0;
                uint8_t *ima_rpt = NULL;

                send_ack = 0;
                cb_sts = 1;     //default to invoking status callback
                src_addr_matched = 0;

                //
                //Get send frame status
                //

                snd_sts = FRAME_SEND_UNKNOWN;

                if (buf[2] & ZIP_FRM_ACK_RESP)
                {   //The previously sent Z/IP packet was received
                    snd_sts = FRAME_SEND_OK;
                }
                else if (buf[2] & ZIP_FRM_NACK_RESP)
                {   //NACK response
                    if (buf[2] & ZIP_FRM_WAIT_RESP)
                    {   //The message has not timed out yet.The destination host may have
                        //a long response time or the device is sleeping
                        snd_sts = FRAME_SEND_BUSY;
                    }
                    else
                    {   //Previously sent Z/IP packet didn't reach destination host
                        //because the host/device is poweroff or non-existence
                        snd_sts = FRAME_SEND_NACK;
                    }
                }

                if ((buf[3] & ZIP_FRM_HDR_EXT) && (data_len > ZIP_FRM_HDR_SIZE))
                {
                    uint8_t *ima_rpt_opt;

                    ima_rpt_opt = frm_hdr_ext_get(buf + ZIP_FRM_HDR_SIZE,
                                                  data_len - ZIP_FRM_HDR_SIZE, ZIP_OPT_IMA_RPT);
                    if (ima_rpt_opt)
                    {
                        ima_rpt_len = ima_rpt_opt[1];
                        ima_rpt = ima_rpt_opt + 2;
                    }
                }

                plt_mtx_lck(frm_ctx->wr_mtx);

                //Check for the source address (could be Z/IP gateway address) and sequence number to match the
                //previously sent packet
                if (tpt_ctx->use_ipv4)
                {
                    struct sockaddr_in  *sa4 = (struct sockaddr_in *)src_addr;

                    if ((frm_ctx->resnd_dst.ipv4.sin_addr.s_addr == sa4->sin_addr.s_addr)
                        || (memcmp(&frm_ctx->tpt_ctx.zip_router[12], &sa4->sin_addr.s_addr, 4) == 0))
                    {
                        src_addr_matched = 1;
                    }
                }
                else
                {
                    struct sockaddr_in6  *sa6 = (struct sockaddr_in6 *)src_addr;

                    if ((memcmp(frm_ctx->resnd_dst.ipv6.sin6_addr.s6_addr, sa6->sin6_addr.s6_addr, 16) == 0)
                        || (memcmp(frm_ctx->tpt_ctx.zip_router, sa6->sin6_addr.s6_addr, 16) == 0))
                    {
                        src_addr_matched = 1;
                    }
                }

                if (src_addr_matched)
                {
                    //Source address matched, check sequence number.
                    if (frm_ctx->resnd_frm_buf && (buf[4] == frm_ctx->resnd_frm_buf[4]))
                    {
                        //Sequence number matched
                        frm_ctx->frm_rcvd = 1;
                        if (snd_sts == FRAME_SEND_BUSY)
                        {   //Host busy

                            if (frm_ctx->resnd_frm_wkup_poll)
                            {   //Polling frame.  Don't wait for ACK as it will take a very long time for the node
                                //to wake up

                                //Free resend buffer
                                free(frm_ctx->resnd_frm_buf);
                                frm_ctx->resnd_frm_buf = NULL;
                                frm_ctx->resend_cnt = 0;

                                //Stop resend timer
                                plt_tmr_stop(frm_ctx->plt_ctx, frm_ctx->snd_tmr_ctx);
                                frm_ctx->snd_tmr_ctx = 0;

                            }
                            else
                            {
                                uint8_t *exp_delay_opt;
                                uint32_t exp_delay = 0;

                                //Restart resend timer with a "expected delay" timeout
                                plt_tmr_stop(frm_ctx->plt_ctx, frm_ctx->snd_tmr_ctx);

                                if ((buf[3] & ZIP_FRM_HDR_EXT) && (data_len > ZIP_FRM_HDR_SIZE))
                                {
                                    exp_delay_opt = frm_hdr_ext_get(buf + ZIP_FRM_HDR_SIZE,
                                                                    data_len - ZIP_FRM_HDR_SIZE, ZIP_OPT_EXP_DELAY);
                                    if (exp_delay_opt)
                                    {
                                        //Check expected delay length field
                                        if (exp_delay_opt[1] == 3)
                                        {
                                            exp_delay = ((uint32_t)exp_delay_opt[2])<<16
                                                        | ((uint32_t)exp_delay_opt[3])<<8 | ((uint32_t)exp_delay_opt[4]);

                                        }
                                    }
                                }

                                //Convert to miliseconds
                                exp_delay *= 1000;

                                //Wait only if the expected delay is within the frame layer timeout
                                if (exp_delay <= FRAME_SEND_TIMEOUT_MIN)
                                {
                                    exp_delay = FRAME_SEND_TIMEOUT_MIN;
                                    //Set expected delay timeout
                                    frm_ctx->snd_tmr_ctx = plt_tmr_start(frm_ctx->plt_ctx, exp_delay,
                                                                         frm_snd_tmout_cb, frm_ctx);
                                    //Don't invoke status callback
                                    cb_sts = 0;
                                }
                                else if (frm_ctx->resend_cnt < FRAME_MAX_RESEND)
                                {   //The expected delay is too long, wait once before giving up
                                    frm_ctx->resend_cnt++;

                                    exp_delay = FRAME_SEND_TIMEOUT_MIN;
                                    //Set expected delay timeout
                                    frm_ctx->snd_tmr_ctx = plt_tmr_start(frm_ctx->plt_ctx, exp_delay,
                                                                         frm_snd_tmout_cb, frm_ctx);
                                    //Don't invoke status callback
                                    cb_sts = 0;
                                }
                                else
                                {   //Don't wait
                                    //Free resend buffer
                                    free(frm_ctx->resnd_frm_buf);
                                    frm_ctx->resnd_frm_buf = NULL;
                                    frm_ctx->resend_cnt = 0;

                                    frm_ctx->snd_tmr_ctx = 0;
                                }
                            }
                        }
                        else if (snd_sts != FRAME_SEND_UNKNOWN)
                        {
                            //Free resend buffer
                            free(frm_ctx->resnd_frm_buf);
                            frm_ctx->resnd_frm_buf = NULL;
                            frm_ctx->resend_cnt = 0;

                            //Stop resend timer
                            plt_tmr_stop(frm_ctx->plt_ctx, frm_ctx->snd_tmr_ctx);
                            frm_ctx->snd_tmr_ctx = 0;

                        }
                    }
                }

                plt_mtx_ulck(frm_ctx->wr_mtx);

                if (cb_sts && (snd_sts != FRAME_SEND_UNKNOWN))
                {
                    frm_ctx->snd_frm_sts_cb(frm_ctx, snd_sts, buf[4], src_addr, ima_rpt_len, ima_rpt);
                }
            }

            //Check whether to send ACK response
            if (send_ack)
            {
                frm_snd_prm_t   snd_prm = {0};
                uint8_t         *critical_opt = NULL;

                snd_prm.cmd_buf = NULL;
                snd_prm.cmd_len = 0;

                //Check for unrecognized critical options
                if ((buf[3] & ZIP_FRM_HDR_EXT) && (data_len > ZIP_FRM_HDR_SIZE))
                {
                    uint8_t *opt_buf;
                    uint8_t opt_len;
                    critical_opt = frm_hdr_ext_chk(buf + ZIP_FRM_HDR_SIZE, data_len - ZIP_FRM_HDR_SIZE);

                    if (critical_opt)
                    {   //Unrecognized critical option
                        opt_len = critical_opt[1];
                        opt_buf = malloc(opt_len + 3);
                        if (opt_buf)
                        {
                            opt_buf[0] = opt_len + 3;   //total size of Z/IP header extension
                            opt_buf[1] = critical_opt[0];//option type
                            opt_buf[2] = opt_len;
                            if (opt_len)
                            {
                                memcpy(opt_buf + 3, critical_opt + 2, opt_len);
                            }
                            snd_prm.cmd_buf = opt_buf;
                            snd_prm.cmd_len = opt_len + 3;
                        }
                    }
                }

                snd_prm.dst = src_addr;
                snd_prm.src_addr = dst_addr;
                snd_prm.seq_num = buf[4];

                snd_prm.flag = (critical_opt)? ZIP_FLAG_NACK_OPT_RESP : ZIP_FLAG_ACK_RESP;
                snd_prm.flag |= rcx_flag;

                if (buf[3] & ZIP_FRM_SECURE)
                {
                    snd_prm.flag |= ZIP_FLAG_SECURE;
                }

                if (snd_prm.cmd_buf)
                {
                    snd_prm.flag |= ZIP_FLAG_HDR_EXT;
                }

                snd_prm.ep_addr.dst_ep_type = 0;
                snd_prm.ep_addr.dst_ep = buf[5];
                snd_prm.ep_addr.src_ep = buf[6];

#if defined(_WINDOWS) || defined(WIN32) || defined(OS_MAC_X)
                //Workaround for Windows Winsock and OSX sendto() return o.k. but doesn't send the packet
                plt_sleep(10);
#endif
                frm_send(frm_ctx, &snd_prm);

#if defined(_WINDOWS) || defined(WIN32) || defined(OS_MAC_X)
                //Workaround for Windows Winsock and OSX sendto() return o.k. but doesn't send the packet

                /*Winsock drops UDP packets if there's no ARP entry for the destination address
                (or the gateway for the destination).  Thus it's quite likely some of the first UDP packet
                gets dropped as at that time there's no ARP entry - and unlike most other operating systems,
                winsock only queues 1 packet while the the ARP request completes. The same behavior can be
                observed on Mac OS X
                */
                plt_sleep(10);
#endif

                if (snd_prm.cmd_buf)
                {
                    free(snd_prm.cmd_buf);
                }
            }

            //Check for Z-wave command
            if (buf[3] & ZIP_FRM_ZWAVE_CMD)
            {
                uint8_t  hdr_ext_len = 0;
                uint16_t flag;
                uint16_t encap_fmt = 0;

                flag = (buf[3] & ZIP_FRM_SECURE)? ZIP_FLAG_SECURE : 0;
                flag |= rcx_flag;

                if ((buf[3] & ZIP_FRM_HDR_EXT) && (data_len > ZIP_FRM_HDR_SIZE))
                {
                    uint8_t *encap_fmt_opt;
                    uint8_t *multicast_opt;

                    hdr_ext_len = buf[7];

                    //Check for Encapsulation Format Information Option
                    encap_fmt_opt = frm_hdr_ext_get(buf + ZIP_FRM_HDR_SIZE,
                                                    data_len - ZIP_FRM_HDR_SIZE, ZIP_OPT_ENCAP_FMT);
                    if (encap_fmt_opt)
                    {
                        //Check option length
                        if (encap_fmt_opt[1] == 2)
                        {
                            encap_fmt = ((uint16_t)encap_fmt_opt[2])<<8 | ((uint16_t)encap_fmt_opt[3]);
                            flag |= ZIP_FLAG_ENCAP_INFO;
                        }
                    }

                    //Check for Z-Wave Multicast Addressing Option
                    multicast_opt = frm_hdr_ext_get(buf + ZIP_FRM_HDR_SIZE,
                                                    data_len - ZIP_FRM_HDR_SIZE, ZIP_OPT_MULTICAST);
                    if (multicast_opt)
                    {
                        flag |= ZIP_FLAG_MULTICAST;
                    }
                }

                if (data_len > (ZIP_FRM_HDR_SIZE + hdr_ext_len))
                {
                    frm_ep_t    ep_addr;

                    ep_addr.src_ep = buf[5];
                    ep_addr.dst_ep_type = (buf[6] & 0x80)? 1 : 0;
                    ep_addr.dst_ep = buf[6] & 0x7F;

                    //Adjust the length to that of Z-wave command
                    data_len -= (ZIP_FRM_HDR_SIZE + hdr_ext_len);
                    //Pass the frame to the session layer
                    frm_ctx->rx_frm_cb(frm_ctx, buf + ZIP_FRM_HDR_SIZE + hdr_ext_len, data_len, src_addr, &ep_addr,
                                       flag, encap_fmt);
                }
            }
        }
#ifdef ZIP_V2
        else if ((data_len >= ZIP_FRM_KEEP_ALIVE_SZ) && (buf[1] == COMMAND_ZIP_KEEP_ALIVE)
                 && (buf[2] & 0x80))
        {   //ZIP keep alive request
    #if defined(_WINDOWS) || defined(WIN32) || defined(OS_MAC_X)
            //Workaround for Windows Winsock and OSX sendto() return o.k. but doesn't send the packet
            plt_sleep(10);
    #endif
            frm_keep_alive_resp_send(frm_ctx, src_addr, rcx_flag);
    #if defined(_WINDOWS) || defined(WIN32) || defined(OS_MAC_X)
            //Workaround for Windows Winsock and OSX sendto() return o.k. but doesn't send the packet
            plt_sleep(10);
    #endif
        }
#endif
    }
    else if (buf[0] == COMMAND_CLASS_ZIP_ND)
    {
        if ((data_len >= ZIP_FRM_ND_ADVT_SZ) && (buf[1] == ZIP_NODE_ADVERTISEMENT))
        {   //ZIP node / IPv6 advertisement
            frm_ep_t    ep_addr = {0};

            //Pass the frame to the session layer
            frm_ctx->rx_frm_cb(frm_ctx, buf, data_len, src_addr, &ep_addr, 0, 0);
        }
    }
}


/**
frm_node_id_get - Resolve IPv6 address into node id using ZIP-ND
@param[in] frm_ctx  Context
@param[in] zip_gw   Z/IP gateway address
@param[in] tgt_ip   Target IPv6 address of the Z-wave node
@param[in] retry    Flag to indicate if this call is second attempt to get node id
@return  0 on success, negative error number on failure
*/
int frm_node_id_get(frm_layer_ctx_t *frm_ctx, struct sockaddr *zip_gw, uint8_t *tgt_ip, int retry)
{
    uint8_t     zip_frm[ZIP_FRM_ND_SOLICIT_SZ];
    int         result;

    if (!frm_ctx->run)
    {
        return ZWHCI_ERROR_SHUTDOWN;
    }

    plt_mtx_lck(frm_ctx->wr_mtx);

    //Fill in the frame
    zip_frm[0] = COMMAND_CLASS_ZIP_ND;
    zip_frm[1] = ZIP_NODE_SOLICITATION;
    zip_frm[2] = 0;
    zip_frm[3] = 0;
    memcpy(zip_frm + 4, tgt_ip, IPV6_ADDR_LEN);

    result = tpt_snd_req_create(&frm_ctx->tpt_ctx, zip_gw, NULL, zip_frm, ZIP_FRM_ND_SOLICIT_SZ,
                                (retry)? TPT_SEND_FLG_NEW_DTLS_SESS : 0);

    plt_mtx_ulck(frm_ctx->wr_mtx);

    return result;
}


/**
frm_node_ipv6_get - Resolve a node id into IPv6 address using ZIP-ND
@param[in] frm_ctx  Context
@param[in] zip_gw   Z/IP gateway address
@param[in] node_id  Z-wave node id
@return  0 on success, negative error number on failure
*/
int frm_node_ipv6_get(frm_layer_ctx_t *frm_ctx, struct sockaddr *zip_gw, uint8_t node_id)
{
    uint8_t     zip_frm[ZIP_FRM_ND_INV_SOLICIT_SZ];
    int         result;

    if (!frm_ctx->run)
    {
        return ZWHCI_ERROR_SHUTDOWN;
    }

    if (node_id == 0)
    {
        return APPL_INVALID_NODE_ID;
    }

    plt_mtx_lck(frm_ctx->wr_mtx);

    //Fill in the frame
    zip_frm[0] = COMMAND_CLASS_ZIP_ND;
    zip_frm[1] = ZIP_INV_NODE_SOLICITATION;
    //zip_frm[2] = 0x04; //request site-local address
    zip_frm[2] = 0;
    zip_frm[3] = node_id;

    result = tpt_snd_req_create(&frm_ctx->tpt_ctx, zip_gw, NULL, zip_frm, ZIP_FRM_ND_INV_SOLICIT_SZ, 0);

    plt_mtx_ulck(frm_ctx->wr_mtx);

    return result;
}


/**
frm_gw_send - Send frame to Z/IP gateway without Z/IP encapsulation
@param[in] frm_ctx      Context
@param[in] cls          Command class
@param[in] cmd          Command
@param[in] data         Data
@param[in] data_len     Data length
@return  0 on success, negative error number on failure
*/
int frm_gw_send(frm_layer_ctx_t *frm_ctx, uint8_t cls, uint8_t cmd, uint8_t *data, uint8_t data_len)
{
    uint8_t             frm[252];
    int                 result;
    tpt_layer_ctx_t     *tpt_ctx = &frm_ctx->tpt_ctx;
    struct sockaddr_in6 gw_addr6;
    struct sockaddr_in  gw_addr4;
    struct sockaddr     *gw_addr;

    if (!frm_ctx->run)
    {
        return ZWHCI_ERROR_SHUTDOWN;
    }

    if (frm_ctx->tpt_ctx.use_ipv4)
    {
        //Convert destination node id to IPv4 address
        gw_addr4.sin_family = AF_INET;
        gw_addr4.sin_port = htons(ZWAVE_HOME_CTL_PORT);
        memcpy(&gw_addr4.sin_addr.s_addr, &tpt_ctx->zip_router[12], 4);
        gw_addr = (struct sockaddr *)&gw_addr4;
    }
    else
    {
        gw_addr6.sin6_family = AF_INET6;
        gw_addr6.sin6_port = htons(ZWAVE_HOME_CTL_PORT);
        gw_addr6.sin6_flowinfo = 0;
        gw_addr6.sin6_scope_id = 0;
        memcpy(gw_addr6.sin6_addr.s6_addr, tpt_ctx->zip_router, IPV6_ADDR_LEN);
        gw_addr = (struct sockaddr *)&gw_addr6;
    }

    plt_mtx_lck(frm_ctx->wr_mtx);

    //Fill in the frame
    frm[0] = cls;
    frm[1] = cmd;
    if (data)
    {
        if (data_len > 250)
        {   //Too large
            return ZWHCI_ERROR_TOO_LARGE;
        }
        memcpy(&frm[2], data, data_len);
    }

    result = tpt_snd_req_create(&frm_ctx->tpt_ctx, gw_addr, NULL, frm,
                                (data)? (2 + data_len) : 2, 0);

    plt_mtx_ulck(frm_ctx->wr_mtx);

    return result;
}


/**
frm_init - Init the frame layer.
Should be called once before calling the other frame layer functions
@param[in,out]	frm_ctx		Context
@return     Return zero indicates success, non-zero indicates failure.
@post       Caller should not modify the context after this call
*/
int frm_init(frm_layer_ctx_t   *frm_ctx)
{
    int ret_val;

    //Init transport layer
    frm_ctx->tpt_ctx.tpt_rd_cb = frm_tpt_rd_cb;
    frm_ctx->tpt_ctx.frm_layer_ctx = frm_ctx;
    frm_ctx->tpt_ctx.plt_ctx = frm_ctx->plt_ctx;

    ret_val = tpt_init(&frm_ctx->tpt_ctx);

    if (ret_val != 0)
        return ret_val;


    //Init frame layer
    frm_ctx->resnd_frm_buf = NULL;
    frm_ctx->resnd_tmout = FRAME_SEND_TIMEOUT_MIN;

    if (!plt_mtx_init(&frm_ctx->wr_mtx))
        goto l_FRAME_INIT_ERROR;

    frm_ctx->run = 1;

    return 0;


l_FRAME_INIT_ERROR:
    tpt_exit(&frm_ctx->tpt_ctx);

    return INIT_ERROR_FRAME;

}


/**
frm_exit - Clean up the frame layer
@param[in,out]	frm_ctx		Context
@return
*/
void frm_exit(frm_layer_ctx_t   *frm_ctx)
{
    frm_ctx->run = 0;

    tpt_exit(&frm_ctx->tpt_ctx);

    //Stop the send timer
    plt_mtx_lck(frm_ctx->wr_mtx);
    if (frm_ctx->snd_tmr_ctx)
    {
        plt_tmr_stop(frm_ctx->plt_ctx, frm_ctx->snd_tmr_ctx);
        frm_ctx->snd_tmr_ctx = 0;
    }

    //Free resend frame
    if (frm_ctx->resnd_frm_buf)
    {
        free(frm_ctx->resnd_frm_buf);
    }

    plt_mtx_ulck(frm_ctx->wr_mtx);

    plt_mtx_destroy(frm_ctx->wr_mtx);
}

/**
@}
*/


