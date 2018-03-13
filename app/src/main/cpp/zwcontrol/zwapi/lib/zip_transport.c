/**
@file   zip_transport.c - Z/IP host controller interface transport layer implementation.

        Platform specific implementation of ipV6 UDP function.

@author David Chow

@version    1.0 7-5-10  Initial release

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

#if defined(_WINDOWS) || defined(WIN32)
    #include "../include/zw_plt_windows.h"  //use in Windows platform
    #include <process.h>                    //for _beginthread, _endthread
#elif defined(OS_LINUX)
#define _GNU_SOURCE         //For IPv6
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include "zip_api.h"
#endif
#include "../include/zip_transport.h"

#ifdef ZIP_V2
#include "../include/zip_dtls.h"
#endif

#ifdef  DEBUG_ZIPAPI
extern void ip_addr_show(plt_ctx_t *plt_ctx, struct sockaddr *sock_addr, socklen_t sock_len);
#endif
#ifndef TCP_PORTAL
static int tpt_setup(tpt_layer_ctx_t *tpt_ctx);
#endif

//#define TEST_RESEND_ADD_NODE

/**
@defgroup Transport Transport layer APIs
Platform specific implementation of ipV6 UDP function.
@{
*/


/**
tpt_snd_req_create - create and queue a send request to the write thread.
@param[in,out]	tpt_ctx		Context
@param[in]      dst         Destination address
@param[in]      src_addr    Optional source address to use when sending the packet; valid if TPT_SEND_FLG_USE_SRC_ADDR is set
@param[in]      buf         Buffer that store the data to be sent
@param[in]      dat_sz      Size of data to be sent
@param[in]      flag        Additional options, see TPT_SEND_FLG_XXX
@return                     Return 0 on success, negative error number on failure.
*/
int tpt_snd_req_create(tpt_layer_ctx_t *tpt_ctx, struct sockaddr *dst, uint8_t *src_addr, uint8_t *buf, uint16_t dat_sz, uint16_t flag)
{
    int32_t     ret_val;    //Return value
    send_pkt_t  *send_pkt;
    uint8_t nodeid = 0;

    send_pkt = (send_pkt_t *)calloc(1, sizeof(send_pkt_t) + dat_sz);

    if (!send_pkt)
    {
        return ZWHCI_ERROR_MEMORY;
    }

    if (tpt_ctx->use_ipv4)
    {
        //djnakata
        struct sockaddr_in *destaddr = (struct sockaddr_in*)dst;

        if(strcmp(inet_ntoa(destaddr->sin_addr), "127.0.0.1") == 0 ||
           strcmp(inet_ntoa(destaddr->sin_addr), "0.0.0.3") == 0)
        {
            nodeid = 1;
        }
        else
        {
            char ipstr[50] = {0};
            strcpy(ipstr, inet_ntoa(destaddr->sin_addr));

            nodeid = (uint8_t)atoi(ipstr+6);
        }

        struct sockaddr_in sendaddr;
        sendaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        send_pkt->dst.ipv4 = sendaddr;
        //send_pkt->dst.ipv4 = *((struct sockaddr_in *)dst);  //djnakata
    }
    else
    {
        send_pkt->dst.ipv6 = *((struct sockaddr_in6 *)dst);
    }

    if (flag & TPT_SEND_FLG_USE_SRC_ADDR)
    {
        memcpy(send_pkt->src_addr, src_addr, (tpt_ctx->use_ipv4)? IPV4_ADDR_LEN : IPV6_ADDR_LEN);
    }
    send_pkt->flag = flag;
    send_pkt->len = dat_sz;

    if(tpt_ctx->use_ipv4)
    {
        //djnakata
        dat_sz = dat_sz + 1;
        send_pkt->len = dat_sz;

        uint8_t *buf1 = NULL;
        buf1 = (uint8_t*)malloc(dat_sz);
        buf1[0] = nodeid;

        memcpy(buf1+1, buf, dat_sz-1);
        memcpy(send_pkt->buf, buf1, dat_sz);
        free(buf1);
    }
    else
    {
        memcpy(send_pkt->buf, buf, dat_sz);
    }

    ret_val = util_list_add(tpt_ctx->wr_req_mtx, &tpt_ctx->wr_req_hd, (uint8_t *)send_pkt, sizeof(send_pkt_t) + dat_sz);

    free(send_pkt);

    if (ret_val)
    {
        return ret_val;
    }

    // Update write started flag
    tpt_ctx->wr_started = 1;

    // Notify writer thread
    plt_sem_post(tpt_ctx->wr_q_sem);

    return ZWHCI_NO_ERROR;
}

#ifdef TCP_PORTAL

/**
tpt_portal_wr_thrd - thread for sending TLS packet
@param[in]	data		Context
@return
*/
static void tpt_portal_wr_thrd(void   *data)
{
    tpt_layer_ctx_t     *tpt_ctx = (tpt_layer_ctx_t *)data;
    util_lst_t          *wr_req;
    send_pkt_t          *send_pkt;
    uint8_t             data_buf[MAX_RCX_SIZE];
    uint16_t            data_len;
    ssize_t             bytes_sent;
    struct sockaddr_in6 src_addr6;
    struct sockaddr_in6 dst_addr6;
    int                 conn_lost = 0;  //Flag to indicate TLS connection to Z/IP gateway has lost

    src_addr6.sin6_family = AF_INET6;
    src_addr6.sin6_port = htons(ZWAVE_HOME_CTL_PORT);
    src_addr6.sin6_flowinfo = 0;
    src_addr6.sin6_scope_id = 0;

    tpt_ctx->wr_thrd_sts = 1;

    while (1)
    {
        //Wait for a request
        plt_sem_wait(tpt_ctx->wr_q_sem);

        //Check whether to exit the thread
        if (tpt_ctx->wr_thrd_run == 0)
        {
            tpt_ctx->wr_thrd_sts = 0;
            return;
        }

        wr_req = util_list_get(tpt_ctx->wr_req_mtx, &tpt_ctx->wr_req_hd);

        if (wr_req)
        {
            send_pkt = (send_pkt_t *)wr_req->wr_buf;
            data_len = (send_pkt->len < MAX_RCX_SIZE)? send_pkt->len : MAX_RCX_SIZE;

            if (!conn_lost)
            {
                memcpy(src_addr6.sin6_addr.s6_addr, tpt_ctx->zipr_prof.svr_ipv6_addr, IPV6_ADDR_LEN);

                //Copy data to local buffer to prevent memory leak
                memcpy(data_buf, send_pkt->buf, data_len);
                dst_addr6 = send_pkt->dst.ipv6;
                free(wr_req);

                bytes_sent = tls_tx(tpt_ctx->ssl, data_buf, data_len,
                                    &src_addr6, &dst_addr6);
    #ifdef SHOW_PACKAT_INFO
                debug_msg_show(tpt_ctx->plt_ctx, "Send udp len: %d", bytes_sent);
                debug_bin_show(tpt_ctx->plt_ctx, data_buf, data_len);
                debug_msg_show(tpt_ctx->plt_ctx, "To:");
                ip_addr_show(tpt_ctx->plt_ctx, (struct sockaddr *)&dst_addr6, sizeof(struct sockaddr_in6));
                debug_msg_show(tpt_ctx->plt_ctx, "From:");
                ip_addr_show(tpt_ctx->plt_ctx, (struct sockaddr *)&src_addr6, sizeof(struct sockaddr_in6));
    #endif
            }
            else
            {   //Connection loss
                bytes_sent = SSL_ERROR_CONN_LOST;
                free(wr_req);
            }

            if (bytes_sent != data_len)
            {
                //perror("Send udp failed");
                debug_msg_show(tpt_ctx->plt_ctx, "Send udp failed. Err: %d", bytes_sent);
                if (bytes_sent == SSL_ERROR_CONN_LOST)
                {
                    conn_lost = 1;
                }
            }
        }
    }
}

#else

/**
tpt_send_with_src_addr - create and queue a send request to the write thread.
@param[in]	    sock_fd		Socket file descriptor
@param[in]      buf         Buffer that store the data to be sent
@param[in]      len         Size of data to be sent
@param[in]      src_addr    Source address to use when sending the packet
@param[in]      dst_addr    Destination address
@param[in]      dst_len     Length of dst_addr
@return                     Return the number of characters sent on success; negative value on failure.
*/
static ssize_t tpt_send_with_src_addr(int sock_fd, uint8_t *buf, uint16_t len, uint8_t *src_addr, sockaddr_stor_t *dst_addr, socklen_t dst_len)
{
    int             cmsg_space;
    ssize_t         ret;
    char            msg_control[256];
    struct msghdr   msg;
    struct cmsghdr  *cmsg;
    struct iovec    iov[1];

    iov[0].iov_base = buf;
    iov[0].iov_len = len;

    //Send packet using the supplied source address
    msg.msg_name = dst_addr;
    msg.msg_namelen = dst_len;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    msg.msg_control = msg_control;
    msg.msg_controllen = sizeof(msg_control);
    msg.msg_flags = 0;

    cmsg_space = 0;
    cmsg = CMSG_FIRSTHDR(&msg);

    if (dst_addr->ss.ss_family == AF_INET)
    {
        struct in_pktinfo *pktinfo4;

        cmsg->cmsg_level = IPPROTO_IP;
        cmsg->cmsg_type = IP_PKTINFO;
        cmsg->cmsg_len = CMSG_LEN(sizeof(struct in_pktinfo));
        pktinfo4 = (struct in_pktinfo *) CMSG_DATA(cmsg);

        //src_interface_index 0 allows choosing interface of the source ip specified
        memset(pktinfo4, 0, sizeof(struct in_pktinfo));
        //pktinfo4->ipi_ifindex = 0;
        memcpy(&pktinfo4->ipi_spec_dst.s_addr, src_addr, IPV4_ADDR_LEN);
        cmsg_space += CMSG_SPACE(sizeof(struct in_pktinfo));
    }
    else
    {
        struct in6_pktinfo *pktinfo6;

        cmsg->cmsg_level = IPPROTO_IPV6;
        cmsg->cmsg_type = IPV6_PKTINFO;
        cmsg->cmsg_len = CMSG_LEN(sizeof(struct in6_pktinfo));
        pktinfo6 = (struct in6_pktinfo *) CMSG_DATA(cmsg);

        memset(pktinfo6, 0, sizeof(struct in6_pktinfo));
        //pktinfo6->ipi6_addr = src_addr->s6.sin6_addr;
        memcpy(pktinfo6->ipi6_addr.s6_addr, src_addr, IPV6_ADDR_LEN);
        cmsg_space += CMSG_SPACE(sizeof(struct in6_pktinfo));
    }

    msg.msg_controllen = cmsg_space;

    if ((ret = sendmsg(sock_fd, &msg, 0)) < 0)
    {
#ifdef OS_LINUX
        perror("sendmsg failed");
#endif
    }

    return ret;

}

/**
tpt_wr_thrd - thread for sending IPv4/IPv6 UDP packet
@param[in]	data		Context
@return
*/
static void tpt_wr_thrd(void *data)
{
    tpt_layer_ctx_t     *tpt_ctx = (tpt_layer_ctx_t *)data;
    util_lst_t          *wr_req;
    send_pkt_t          *send_pkt;
#if defined(_WINDOWS) || defined(WIN32) || defined(TCP_PORTAL)
    int                 bytes_sent;
#else
    ssize_t             bytes_sent;
#endif
    struct sockaddr     *sock_addr;
    socklen_t           sock_len;

    tpt_ctx->wr_thrd_sts = 1;

    while (1)
    {
        //Wait for a request
        plt_sem_wait(tpt_ctx->wr_q_sem);

        //Check whether to exit the thread
        if (tpt_ctx->wr_thrd_run == 0)
        {
            tpt_ctx->wr_thrd_sts = 0;
            return;
        }

        wr_req = util_list_get(tpt_ctx->wr_req_mtx, &tpt_ctx->wr_req_hd);

        if (wr_req)
        {
            send_pkt = (send_pkt_t *)wr_req->wr_buf;

            if (tpt_ctx->use_ipv4)
            {   //IPv4
                sock_len = sizeof(struct sockaddr_in);
                sock_addr = (struct sockaddr *)&send_pkt->dst.ipv4;

                if (send_pkt->flag & TPT_SEND_FLG_USE_SRC_ADDR)
                {
                    bytes_sent = tpt_send_with_src_addr(tpt_ctx->sock_fd, send_pkt->buf, send_pkt->len,
                                                        send_pkt->src_addr, (sockaddr_stor_t *)sock_addr, sock_len);
                }
                else
                {   //Let system to choose the source IP
                    bytes_sent = sendto(tpt_ctx->sock_fd, send_pkt->buf, send_pkt->len,
                                        0, (const struct sockaddr *)sock_addr, sock_len);
                }
            }
            else
            {   // IPv6
                sock_len = sizeof(struct sockaddr_in6);
                sock_addr = (struct sockaddr *)&send_pkt->dst.ipv6;
                //Must use a local IP that can reach the Z/IP gateway since the system can't choose a correct
                //source IP if the destination is a PAN address.
                bytes_sent = tpt_send_with_src_addr(tpt_ctx->sock_fd, send_pkt->buf, send_pkt->len,
                                                    (send_pkt->flag & TPT_SEND_FLG_USE_SRC_ADDR)? send_pkt->src_addr : tpt_ctx->listen_addr,
                                                    (sockaddr_stor_t *)sock_addr, sock_len);
            }


    #ifdef SHOW_PACKAT_INFO
            debug_msg_show(tpt_ctx->plt_ctx, "Send udp len: %d", bytes_sent);
    #endif

    #ifdef TEST_RESEND_ADD_NODE
            if (send_pkt->len == 13)
            {
                if (send_pkt->buf[7] == 0x34
                    && send_pkt->buf[8] == 0x01
                    && send_pkt->buf[11] == 0x01)
                {
                    bytes_sent = sendto(tpt_ctx->sock_fd, send_pkt->buf, send_pkt->len,
                                        0, (const struct sockaddr *)sock_addr, sock_len);
                    debug_msg_show(tpt_ctx->plt_ctx, "Send udp len: %d", bytes_sent);
                }
            }
    #endif

            if (bytes_sent != send_pkt->len)
            {
                //perror("Send udp failed");
                debug_msg_show(tpt_ctx->plt_ctx, "Send udp failed. Err: %d", bytes_sent);
            }

            free(wr_req);
        }
    }
}

#endif

#ifdef TCP_PORTAL
/**
tpt_portal_rd_thrd - thread to receive TLS packets
@param[in]	data		Context
@return
*/
static void tpt_portal_rd_thrd(void   *data)
{
    tpt_layer_ctx_t     *tpt_ctx = (tpt_layer_ctx_t *)data;
    int                 loop_cnt;
    int                 rcx_size;
    int                 max_fd;
    int                 rv;
    int                 inactivity_cnt;     //SSL inactivity count
    fd_set              readfds;
    struct timeval      tv;
    struct sockaddr_in6 sa6;
    uint8_t             buf[MAX_TLS_TX_SIZE] = {0};

    tpt_ctx->rd_thrd_sts = 1;

    inactivity_cnt = 0;

    //Delay 1 second or until write started, before reading data to avoid race condition where
    //upper layer read callback functions are not ready
    loop_cnt = 10;
    while (loop_cnt-- > 0)
    {
        if (tpt_ctx->wr_started)
        {
            break;
        }
        plt_sleep(100);
    }

    //Initialize source address
    memset(&sa6, 0, sizeof(struct sockaddr_in6));
    sa6.sin6_family = AF_INET6;

    // the n param in select()
    max_fd = tpt_ctx->sock_fd + 1;

    while (tpt_ctx->rd_thrd_run)
    {
        // clear the set ahead of time
        FD_ZERO(&readfds);

        // add our descriptors to the set
        FD_SET(tpt_ctx->sock_fd, &readfds);

        // wait until either socket has data ready to be received or timeout
        tv.tv_sec = TLS_TRANSPORT_READ_TIMEOUT / 1000;
        tv.tv_usec = (TLS_TRANSPORT_READ_TIMEOUT % 1000) * 1000;

        rv = select(max_fd, &readfds, NULL, NULL, &tv);

        if (!tpt_ctx->rd_thrd_run)
        {   //Do nothing
        }
        else if (rv == TPT_SOCKET_ERR)
        {
#if defined(_WINDOWS) || defined(WIN32)
            debug_msg_show(tpt_ctx->plt_ctx, "select failed with error %d", WSAGetLastError());
#else
            debug_msg_show(tpt_ctx->plt_ctx, "select failed with error %d", errno);
#endif
            if (tpt_ctx->tpt_net_err_cb)
            {
                tpt_ctx->tpt_net_err_cb(tpt_ctx->user);
            }
            break;
        }
        else if (rv == 0)
        {
            //Timeout
            if (++inactivity_cnt > TLS_INACTIVITY_TIMEOUT)
            {
                debug_msg_ts_show(tpt_ctx->plt_ctx, "No keep alive message, network connection timeout");
                if (tpt_ctx->tpt_net_err_cb)
                {
                    tpt_ctx->tpt_net_err_cb(tpt_ctx->user);
                }
                break;
            }
        }
        else
        {
            if (FD_ISSET(tpt_ctx->sock_fd, &readfds))
            {
            	rcx_size = tls_rx(tpt_ctx->ssl, buf, MAX_TLS_TX_SIZE, &sa6);

                if (rcx_size > 0)
                {
#ifdef SHOW_PACKAT_INFO
                    debug_msg_show(tpt_ctx->plt_ctx, "udp rcx:");
                    debug_bin_show(tpt_ctx->plt_ctx, buf, rcx_size);
                    debug_msg_show(tpt_ctx->plt_ctx, "From:");
                    ip_addr_show(tpt_ctx->plt_ctx, (struct sockaddr *)&sa6, sizeof(struct sockaddr_in6));
#endif

                    //Callback frame layer
                    tpt_ctx->tpt_rd_cb(tpt_ctx, buf, rcx_size, (struct sockaddr *)&sa6, NULL);

                }
                else if (rcx_size < 0)
                {
                    if (rcx_size == ZIPR_KEEP_ALIVE)
                    {
                        debug_msg_ts_show(tpt_ctx->plt_ctx, "rcx keep alive msg");
                    }
                    else
                    {
                        debug_msg_ts_show(tpt_ctx->plt_ctx, "udp rcx error:%d", rcx_size);
                        if (tpt_ctx->tpt_net_err_cb)
                        {
                            tpt_ctx->tpt_net_err_cb(tpt_ctx->user);
                        }
                        break;
                    }
                }
                //Reset inactivity count
                inactivity_cnt = 0;
            }
        }
    }

    debug_msg_show(tpt_ctx->plt_ctx, "tpt_portal_rd_thrd exited");
    tpt_ctx->rd_thrd_sts = 0;

}

#else
/**
tpt_rd_thrd - thread to receive UDP packets
@param[in]	data		Context
@return
*/
static void tpt_rd_thrd(void *data)
{
    tpt_layer_ctx_t *tpt_ctx = (tpt_layer_ctx_t *)data;
    int             loop_cnt;
    int             rcx_size;
    int             max_fd;
    int             rv;
    socklen_t       sock_len;
    fd_set          read_fds;
    struct timeval  tv;
    uint8_t         buf[MAX_RCX_SIZE];
    struct          msghdr mh;
    struct          iovec iov[1];
    char            cmbuf[256];
#ifdef SHOW_PACKAT_INFO
    char            addr_str[128];
#endif
    uint8_t         srv_addr[IPV6_ADDR_LEN];
    sockaddr_stor_t peer_addr;

    tpt_ctx->rd_thrd_sts = 1;

    //Delay 1 second or until write started, before reading data to avoid race condition where
    //upper layer read callback functions are not ready
    loop_cnt = 10;
    while (loop_cnt-- > 0)
    {
        if (tpt_ctx->wr_started)
        {
            break;
        }
        plt_sleep(100);
    }

    iov[0].iov_base = buf;
    iov[0].iov_len = sizeof(buf);

    sock_len = (tpt_ctx->use_ipv4)? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);

    while (tpt_ctx->rd_thrd_run)
    {
        // clear the set ahead of time
        FD_ZERO(&read_fds);

        // add our descriptors to the set
        FD_SET(tpt_ctx->sock_fd, &read_fds);

        // the n param in select()
        max_fd = tpt_ctx->sock_fd + 1;

        // wait until either socket has data ready to be received or timeout
        tv.tv_sec = TRANSPORT_READ_TIMEOUT_MIN / 1000;
        tv.tv_usec = (TRANSPORT_READ_TIMEOUT_MIN % 1000) * 1000;

        rv = select(max_fd, &read_fds, NULL, NULL, &tv);

        if (rv == TPT_SOCKET_ERR)
        {
    #if defined(_WINDOWS) || defined(WIN32)
            debug_msg_show(tpt_ctx->plt_ctx, "select failed with error %d", WSAGetLastError());
    #else
            int error_num;
            error_num = errno;
            debug_msg_show(tpt_ctx->plt_ctx, "select failed with error number %d : %s", error_num, strerror(error_num));
            if ((error_num == EBADF)
        #ifdef ENOTSOCK
                || (error_num == ENOTSOCK)
        #endif
                )
            {   //Bad socket file descriptor
                //Close and create a new listening socket
                close(tpt_ctx->sock_fd);
                if (tpt_setup(tpt_ctx))
                {
                    continue;
                }
                else
                {
                    tpt_ctx->sock_fd = -1;
                    break;
                }
            }
    #endif
            break;
        }
        else if (rv == 0)
        {
            //Timeout
        }
        else
        {
            if (FD_ISSET(tpt_ctx->sock_fd, &read_fds))
            {
                //Get source address of the connection request
                mh.msg_name = &peer_addr;
                mh.msg_namelen = sock_len;
                mh.msg_iov = iov;
                mh.msg_iovlen = 1;
                mh.msg_control = cmbuf;
                mh.msg_controllen = sizeof(cmbuf);
                mh.msg_flags = 0;

                rcx_size = recvmsg(tpt_ctx->sock_fd, &mh, 0);

                if (rcx_size > 0)
                {
    #ifdef SHOW_PACKAT_INFO
                    if (tpt_ctx->use_ipv4)
                    {
                        if (util_ip_ntoa((uint8_t *)&peer_addr.s4.sin_addr.s_addr, addr_str, 128, 1) == 0)
                        {
                            debug_msg_show(tpt_ctx->plt_ctx, "Peer address: %s", addr_str);
                        }
                    }
                    else
                    {
                        if (util_ip_ntoa(peer_addr.s6.sin6_addr.s6_addr, addr_str, 128, 0) == 0)
                        {
                            debug_msg_show(tpt_ctx->plt_ctx, "Peer address: %s", addr_str);
                        }
                    }
    #endif

                    //Get received packet header destination address
                    if (!util_rx_pkt_dest_addr_get(&mh, srv_addr, tpt_ctx->use_ipv4))
                    {
                        debug_msg_show(tpt_ctx->plt_ctx, "Err: no received packet header destination address");
                        break;
                    }

    #ifdef SHOW_PACKAT_INFO
                    if (util_ip_ntoa(srv_addr, addr_str, 128, tpt_ctx->use_ipv4) == 0)
                    {
                        debug_msg_show(tpt_ctx->plt_ctx, "Packet info: Header destination address = %s", addr_str);
                    }
    #endif

                    //Callback frame layer
                    //djnakata
                    tpt_ctx->tpt_rd_cb(tpt_ctx, buf + 1, rcx_size, (struct sockaddr *)&peer_addr, srv_addr);

                }
                else if (rcx_size < 0)
                {
#ifdef OS_LINUX
                    if ((errno == EBADF)
            #ifdef ENOTSOCK
                        || (errno == ENOTSOCK)
            #endif
                        )
                    {   //Bad socket file descriptor
                        //Close and create a new listening socket
                        close(tpt_ctx->sock_fd);
                        debug_msg_show(tpt_ctx->plt_ctx, "recvmsg failed");
                        if (!tpt_setup(tpt_ctx))
                        {
                            tpt_ctx->sock_fd = -1;
                            break;
                        }
                    }
#endif
                }
            }
        }
    }

    tpt_ctx->rd_thrd_sts = 0;

}
#endif

#ifdef ZIP_V2
/**
tpt_dtls_rd_thrd - thread to receive DTLS packets
@param[in]	data		Context
@return
*/
static void tpt_dtls_rd_thrd(void *data)
{
    dtls_rd_thrd((tpt_layer_ctx_t *)data);
}


/**
tpt_dtls_wr_thrd - thread for sending DTLS packet
@param[in]	data		Context
@return
*/
static void tpt_dtls_wr_thrd(void *data)
{
    dtls_wr_thrd((tpt_layer_ctx_t *)data);
}

#endif

/**
tpt_thrd_start - Creates the read and write threads.
@param[in,out]	tpt_ctx		Context
@return     Return non-zero indicates success, zero indicates failure.
*/
static int tpt_thrd_start(tpt_layer_ctx_t *tpt_ctx)
{
#ifdef TCP_PORTAL
    //Start read thread
    tpt_ctx->rd_thrd_run = 1;
    if (plt_thrd_create(tpt_portal_rd_thrd, tpt_ctx) < 0)
    {
        debug_msg_show(tpt_ctx->plt_ctx, "Init transport read thread failed");
        return 0;
    }

    //Start write thread
    tpt_ctx->wr_thrd_run = 1;
    if (plt_cancel_thrd_create(tpt_portal_wr_thrd, tpt_ctx, &tpt_ctx->wr_thrd_ctx) < 0)
    {
        debug_msg_show(tpt_ctx->plt_ctx, "Init transport write thread failed");

        //Stop the read thread
        tpt_ctx->rd_thrd_run = 0;
        while (tpt_ctx->rd_thrd_sts)
        {
            plt_sleep(100);
        }

        return 0;
    }
#elif defined(ZIP_V2)
    void (*thrd_fn)(void *);

    //Start read thread
    thrd_fn = (tpt_ctx->dtls_psk_len)? tpt_dtls_rd_thrd : tpt_rd_thrd;

    tpt_ctx->rd_thrd_run = 1;
    if (plt_thrd_create(thrd_fn, tpt_ctx) < 0)
    {
        debug_msg_show(tpt_ctx->plt_ctx, "Init transport read thread failed");
        return 0;
    }

    //Start write thread
    thrd_fn = (tpt_ctx->dtls_psk_len)? tpt_dtls_wr_thrd : tpt_wr_thrd;

    tpt_ctx->wr_thrd_run = 1;
    if (plt_thrd_create(thrd_fn, tpt_ctx) < 0)
    {

        debug_msg_show(tpt_ctx->plt_ctx, "Init transport write thread failed");

        //Stop the read thread
        tpt_ctx->rd_thrd_run = 0;
        while (tpt_ctx->rd_thrd_sts)
        {
            plt_sleep(100);
        }

        return 0;
    }
#else
    //Start read thread
    tpt_ctx->rd_thrd_run = 1;
    if (plt_thrd_create(tpt_rd_thrd, tpt_ctx) < 0)
    {
        debug_msg_show(tpt_ctx->plt_ctx, "Init transport read thread failed");
        return 0;
    }

    //Start write thread
    tpt_ctx->wr_thrd_run = 1;
    if (plt_thrd_create(tpt_wr_thrd, tpt_ctx) < 0)
    {

        debug_msg_show(tpt_ctx->plt_ctx, "Init transport write thread failed");

        //Stop the read thread
        tpt_ctx->rd_thrd_run = 0;
        while (tpt_ctx->rd_thrd_sts)
        {
            plt_sleep(100);
        }

        return 0;
    }
#endif
    return 1;
}


/**
tpt_thrd_stop - Stop the read and write threads.
@param[in]	tpt_ctx		Context
@return     Return non-zero indicates success, zero indicates failure.
*/
static void tpt_thrd_stop(tpt_layer_ctx_t *tpt_ctx)
{
#ifdef TCP_PORTAL
    int  ret;
#endif

    //Stop the read thread
    tpt_ctx->rd_thrd_run = 0;
    //Stop the write thread
    tpt_ctx->wr_thrd_run = 0;

    plt_sem_post(tpt_ctx->wr_q_sem);

    //Wait until the received packet is processed
    while (tpt_ctx->rd_thrd_sts)
    {
        plt_sleep(100);
    }

    while (tpt_ctx->wr_thrd_sts)
    {
        plt_sleep(100);
    }

    //Extra time for the thread to fully exit
    //Needed to avoid program crash if calling init and exit in a very short interval
    plt_sleep(200);

#ifdef TCP_PORTAL
    if (tpt_ctx->wr_thrd_sts != 0)
    {   //Force thread to exit
        if (plt_cancel_thrd_exit(tpt_ctx->wr_thrd_ctx) != 0)
        {
            debug_msg_show(tpt_ctx->plt_ctx, "Cancel thread failed");
        }
        else
        {
            debug_msg_show(tpt_ctx->plt_ctx, "Cancel thread o.k.");
        }
    }

    ret = plt_cancel_thrd_join(tpt_ctx->wr_thrd_ctx);

    switch (ret)
    {
        case 0:
            debug_msg_show(tpt_ctx->plt_ctx, "thread terminated normally");
            break;

        case 1:
            debug_msg_show(tpt_ctx->plt_ctx, "thread was canceled");
            break;

        default:
            debug_msg_show(tpt_ctx->plt_ctx, "Join thread failed");

    }
#endif


#if defined(_WINDOWS) || defined(WIN32)
    closesocket(tpt_ctx->sock_fd);
#else
    if (tpt_ctx->sock_fd > 0)
    {
        close(tpt_ctx->sock_fd);
    }
#endif
}


#ifndef TCP_PORTAL
/**
tpt_bind_zwave - Bind a socket to the given port
@param[in] tpt_ctx		Context
@param[in] fd           The network socket
@param[in] port         Listening and sending port number
@param[in] bound_addr   IP address to bind to the UDP socket (optional)
@param[in] use_ipv4     Flag to indicate whether to use IPv4
@return          Zero on success; else negative number on error.
*/
#if defined(_WINDOWS) || defined(WIN32)
static int tpt_bind_zwave(tpt_layer_ctx_t *tpt_ctx, SOCKET fd, uint16_t port, uint8_t *bound_addr, int use_ipv4)
#else
static int tpt_bind_zwave(tpt_layer_ctx_t *tpt_ctx, int fd, uint16_t port, uint8_t *bound_addr, int use_ipv4)
#endif
{
    struct sockaddr_in6 sa6;
    struct sockaddr_in  sa4;
    int res;

    if (use_ipv4)
    {
        sa4.sin_family = AF_INET;
        sa4.sin_port = htons(port);
        if (bound_addr)
        {
            memcpy(&sa4.sin_addr.s_addr, bound_addr, 4);
        }
        else
        {
            sa4.sin_addr.s_addr = INADDR_ANY;
        }

        //Bind to listening address
        res = bind(fd, (const struct sockaddr *)&sa4, sizeof(struct sockaddr_in));
    }
    else    //IPv6
    {
        sa6.sin6_family = AF_INET6;
        sa6.sin6_port = htons(port);
        sa6.sin6_flowinfo = 0;
    	sa6.sin6_scope_id = 0;
        if (bound_addr)
        {
            memcpy(sa6.sin6_addr.s6_addr, bound_addr, IPV6_ADDR_LEN);
        }
        else
        {
            sa6.sin6_addr = in6addr_any;
        }

        //Bind to listening address
        res = bind(fd, (const struct sockaddr *)&sa6, sizeof(struct sockaddr_in6));
    }

    if (res < 0)
    {
        debug_msg_show(tpt_ctx->plt_ctx, "Binding to ip address failed.");
        return -2;
    }

    //Save the port number if it is system-assigned
    if (port == 0)
    {
        if (use_ipv4)
        {   //IpV4
            struct sockaddr_in  name = {0};
            socklen_t           namelen = sizeof(name);

            res = getsockname(fd, (struct sockaddr *)&name, &namelen);

            if (res == 0)
            {
                tpt_ctx->host_port = ntohs(name.sin_port);
            }
        }
        else
        {   //IPv6
            struct sockaddr_in6     name = {0};
            socklen_t               namelen = sizeof(name);

            res = getsockname(fd, (struct sockaddr *)&name, &namelen);

            if (res == 0)
            {
                tpt_ctx->host_port = ntohs(name.sin6_port);
            }
        }
    }

    debug_msg_show(tpt_ctx->plt_ctx, "Bound to port number:%u", tpt_ctx->host_port);
    return 0;
}


/**
tpt_setup - Create UDP socket
@param[in,out]	tpt_ctx		Context
@return     Return non-zero indicates success, zero indicates failure.
*/
static int tpt_setup(tpt_layer_ctx_t *tpt_ctx)
{
    int res;
#ifdef OS_LINUX
    int old_flg;
#endif
    int	fd_flg;
    int yes = 1;

    //Open socket and bind it to the source address
    tpt_ctx->sock_fd = socket((tpt_ctx->use_ipv4)? AF_INET : AF_INET6, SOCK_DGRAM, 0);
    if (tpt_ctx->sock_fd < 0)
    {
        debug_msg_show(tpt_ctx->plt_ctx, "Open udp socket failed");
        return 0;
    }

    if (setsockopt(tpt_ctx->sock_fd, IPPROTO_IP, IP_PKTINFO, &yes, sizeof(yes)) < 0)
    {
        debug_msg_show(tpt_ctx->plt_ctx, "setsockopt IP_PKTINFO failed");
        goto l_TPT_SETUP_ERR;
    }

    if (!tpt_ctx->use_ipv4)
    {   //IPv6
        if (setsockopt(tpt_ctx->sock_fd, IPPROTO_IPV6, IPV6_RECVPKTINFO, &yes, sizeof(yes)) < 0)
        {
            debug_msg_show(tpt_ctx->plt_ctx, "setsockopt IPV6_RECVPKTINFO failed");
            goto l_TPT_SETUP_ERR;
        }

#ifdef OS_LINUX
        //Windows default to IPV6_V6ONLY = TRUE, so don't need to setsockopt
        #ifdef IPV6_V6ONLY

        // Disable IPv4 mapped addresses for security reason.
        if (setsockopt(tpt_ctx->sock_fd, IPPROTO_IPV6, IPV6_V6ONLY, &yes, sizeof(yes)) < 0)
        {
            debug_msg_show(tpt_ctx->plt_ctx, "setsockopt IPV6_V6ONLY failed");
            goto l_TPT_SETUP_ERR;
        }

        #else
        debug_msg_show(tpt_ctx->plt_ctx, "Warning: couldn't disable IPv4 mapped addresses");
        #endif
#endif
    }

#ifdef OS_LINUX
    //Set the socket file descriptor with FD_CLOEXEC which specifies
    //that the file descriptor should be closed when an exec function is invoked

    old_flg = fcntl (tpt_ctx->sock_fd, F_GETFD, 0);

    old_flg |= FD_CLOEXEC;

	res = fcntl (tpt_ctx->sock_fd, F_SETFD, old_flg);

    if (res < 0)
    {
        debug_msg_show(tpt_ctx->plt_ctx, "set FD_CLOEXEC failed");
        goto l_TPT_SETUP_ERR;
    }
#endif

    //Set socket as non-blocking
    fd_flg = fcntl(tpt_ctx->sock_fd, F_GETFL, 0);

    fd_flg |= O_NONBLOCK;

    if (fcntl(tpt_ctx->sock_fd, F_SETFL, fd_flg) < 0)
    {
        debug_msg_show(tpt_ctx->plt_ctx, "set O_NONBLOCK failed");
        goto l_TPT_SETUP_ERR;
    }

    //Bind to host port
    debug_msg_show(tpt_ctx->plt_ctx, "Binding to host port:%u", tpt_ctx->host_port);

    res = tpt_bind_zwave(tpt_ctx, tpt_ctx->sock_fd, tpt_ctx->host_port,
                         NULL,  //listen from all interfaces
                         //(tpt_ctx->use_ipv4)? NULL : tpt_ctx->listen_addr,
                         tpt_ctx->use_ipv4);

    if (res < 0)
    {
        debug_msg_show(tpt_ctx->plt_ctx, "tpt_bind_zwave failed:%d", res);
        goto l_TPT_SETUP_ERR;
    }

    return 1;//O.k.

l_TPT_SETUP_ERR:
#if defined(_WINDOWS) || defined(WIN32)
    closesocket(tpt_ctx->sock_fd);
#else
    close(tpt_ctx->sock_fd);
#endif
    return 0;

}
#endif


/**
tpt_init - Init the transport layer.
Should be called once before calling the other transport layer functions
@param[in,out]	tpt_ctx		Context
@return     Return zero indicates success, non-zero indicates failure.
@post       Caller should not modify the tpt_ctx after this call
*/
int tpt_init(tpt_layer_ctx_t *tpt_ctx)
{
#if defined(_WINDOWS) || defined(WIN32)
    int result;
    WSADATA wsa_dat;

    // Initialize Winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsa_dat);
    if (result != NO_ERROR)
    {
        debug_msg_show(tpt_ctx->plt_ctx, "WSAStartup failed with error %d", result);
        return INIT_ERROR_TRANSPORT;
    }
#endif

    tpt_ctx->wr_req_hd = NULL;

    if (tpt_ctx->use_ipv4)
    {
        uint8_t zip_router_ipv4[IPV4_ADDR_LEN];
        //Save the Z/IP router IPv4 address
        memcpy(zip_router_ipv4, tpt_ctx->zip_router, IPV4_ADDR_LEN);

        //Convert the IPv4 address to IPv4-mapped IPv6 address
        memset(tpt_ctx->zip_router, 0, IPV4_ADDR_LEN);
        tpt_ctx->zip_router[10] = 0xFF;
        tpt_ctx->zip_router[11] = 0xFF;
        memcpy(&tpt_ctx->zip_router[12], zip_router_ipv4, IPV4_ADDR_LEN);
    }
#ifndef TCP_PORTAL
    else
    {   //IPv6
        //Get the source IPv6 address used to connect to Z/IP gateway
        if (util_local_addr_get(tpt_ctx->zip_router, tpt_ctx->listen_addr, 0) != 0)
        {
            goto l_TRANSPORT_INIT_ERROR;
        }
    }
#endif

    if (!plt_mtx_init(&tpt_ctx->wr_req_mtx))
        goto l_TRANSPORT_INIT_ERROR;

    if (!plt_sem_init(&tpt_ctx->wr_q_sem))
    {
        goto l_TRANSPORT_INIT_ERROR1;
    }

#ifndef TCP_PORTAL
    // Create UDP socket
    if (!tpt_setup(tpt_ctx))
        goto l_TRANSPORT_INIT_ERROR2;

    #ifdef ZIP_V2
    // Check whether to use DTLS as transport layer
    if (tpt_ctx->dtls_psk_len)
    {   //Close socket as this is unsuitable for DTLS
        close(tpt_ctx->sock_fd);
        tpt_ctx->sock_fd = -1;

        //Initialize DTLS
        if (dtls_init(tpt_ctx) != 0)
            goto l_TRANSPORT_INIT_ERROR3;
    }
    #endif
#endif

    // Start threads
    if (!tpt_thrd_start(tpt_ctx))
    {
#ifdef TCP_PORTAL
        goto l_TRANSPORT_INIT_ERROR2;
#else
    #ifdef ZIP_V2
        // Check for DTLS
        if (tpt_ctx->dtls_psk_len)
        {
            goto l_TRANSPORT_INIT_ERROR4;
        }
    #endif
        goto l_TRANSPORT_INIT_ERROR3;
#endif

    }

    return 0;

#ifndef TCP_PORTAL

    #ifdef ZIP_V2
l_TRANSPORT_INIT_ERROR4:
    dtls_exit(tpt_ctx);
    #endif
l_TRANSPORT_INIT_ERROR3:
    #if defined(_WINDOWS) || defined(WIN32)
    closesocket(tpt_ctx->sock_fd);
    #else
    close(tpt_ctx->sock_fd);
    #endif
#endif

l_TRANSPORT_INIT_ERROR2:
    plt_sem_destroy(tpt_ctx->wr_q_sem);

l_TRANSPORT_INIT_ERROR1:
    plt_mtx_destroy(tpt_ctx->wr_req_mtx);

l_TRANSPORT_INIT_ERROR:
#if defined(_WINDOWS) || defined(WIN32)
    //Clean up Winsock
    WSACleanup();
#endif

    return INIT_ERROR_TRANSPORT;

}

/**
tpt_exit - Clean up the transport layer
@param[in,out]	tpt_ctx		Context
@return
*/
void tpt_exit(tpt_layer_ctx_t *tpt_ctx)
{
    //Stop all the threads
    tpt_thrd_stop(tpt_ctx);

    plt_sem_destroy(tpt_ctx->wr_q_sem);
    util_list_flush(tpt_ctx->wr_req_mtx, &tpt_ctx->wr_req_hd);
    plt_mtx_destroy(tpt_ctx->wr_req_mtx);

#ifdef ZIP_V2
    if (tpt_ctx->dtls_psk_len)
    {
        dtls_exit(tpt_ctx);
    }
#endif

#if defined(_WINDOWS) || defined(WIN32)
    //Clean up Winsock
    WSACleanup();
#endif
}


#ifdef TCP_PORTAL
/**
tpt_svr_ipv6_addr_set - Set the server IPv6 address used in the tunnel
@param[in]	tpt_ctx		    Context
@param[in]	svr_ipv6_addr	server IPv6 address
@return
*/
void tpt_svr_ipv6_addr_set(tpt_layer_ctx_t *tpt_ctx, uint8_t *svr_ipv6_addr)
{
    memcpy(tpt_ctx->zipr_prof.svr_ipv6_addr, svr_ipv6_addr, IPV6_ADDR_LEN);
}
#endif


/**
@}
*/

