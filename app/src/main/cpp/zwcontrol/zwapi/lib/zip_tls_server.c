/**
@file   zip_tls_server.c - Z/IP interface TLS implementation.

        Platform specific implementation of TLS function.

@author David Chow

@version    1.0 11-7-12  Initial release

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
#ifdef TCP_PORTAL

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/ip6.h>
#include <netinet/udp.h>
#include <netdb.h>
#include <ifaddrs.h>


#include "../include/zip_frame.h"
#include "../include/zip_tls_server.h"
#include "../include/zwave/ZW_classcmd.h"

#define OPENSSL_THREAD_DEFINES
#include "openssl/opensslconf.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

//#define ENABLE_DECRYPT_FRAME    //Use cipher that can be decrypted by Wireshark

/**
@defgroup TLS TLS APIs
Platform specific implementation of TLS function.
@{
*/

#define IP6_HDRLEN      40          ///< IPv6 header length
#define UDP_HDRLEN      8           ///< UDP header length, excludes data

#define ZIPR_READY_OK	            0xFF    ///<ZIPR is ready
#define ZIPR_READY_INVALID_CONFIG	0x01    ///<ZIPR rejected the configuration sent


#define TLS_HANDSHAKE_COMPLETED         0   ///<SSL and ZIPR handshakes completed
#define TLS_HANDSHAKE_IN_PROGRESS       1   ///<SSL handshake in progress
#define TLS_HANDSHAKE_SSL_OK            2   ///<SSL handshake completed, ZIPR handshake in progress
#define TLS_HANDSHAKE_WAIT_ZIPR_READY   3   ///<ZIPR handshake waiting for ZIPR ready
#define TLS_HANDSHAKE_ZIP_GW_DISCVR     4   ///<Z/IP gateway discovery in progress
#define TLS_HANDSHAKE_AUTH_ERR          5   ///<SSL authentication error, caller should close the SSL connection
#define TLS_HANDSHAKE_ERR               6   ///<SSL handshake error, caller should close the SSL connection
#define TLS_HANDSHAKE_UNKNOWN_CMD       7   ///<Received unknown command
                                   //123456789
#define ZIPR_KEEPALIVE_STR          "KEEPALIVE" ///<ZIPR keep alive string
/*
struct pushed_configuration {
   uip_ip6addr_t lan_address; //may be 0
   u8_t lan_prefix_length;

   uip_ip6addr_t tun_prefix;
   u8_t tun_prefix_length;

   uip_ip6addr_t gw_address; //default gw address, may be 0.
   uip_ip6addr_t pan_prefix; //pan prefix may be 0... prefix length is always /64
   uip_ip6addr_t unsolicited_dest;
   u16_t unsolicited_destination_port;
} CC_ALIGN_PACK;
*/

///
/// TLS client configuration offset
#define TLS_CFG_LAN_IP_OFFSET           2
#define TLS_CFG_LAN_PREFIX_OFFSET       18
#define TLS_CFG_PORTAL_IP_OFFSET        19
#define TLS_CFG_PORTAL_PREFIX_OFFSET    35
#define TLS_CFG_DFLT_GW_IP_OFFSET       36
#define TLS_CFG_PAN_PREFIX_OFFSET       52
#define TLS_CFG_LENGTH                  68

///
/// TLS client context
typedef struct
{
    int                 clnt_fd;        ///< client socket file descriptor
    int                 clnt_state;     ///< client TLS handshaking state
    SSL*                clnt_ssl;       ///< client SSL object pointer
    uint16_t            clnt_expire;    ///< client TLS handshaking timeout
    tls_clnt_prof_t     clnt_prof;      ///< client profile

    int                 clnt_flag_want_write;   ///< client socket file descriptor for writing during SSL handshake
} tls_clnt_ctx_t;

static int s_server_session_id_context = 1;

/**
tls_checksum - Calculate checksum
@param[in]	    buf16	 Buffer that store 16-bits data
@param[in]	    len	     Length of buffer in terms of bytes
@return     Checksum
*/
static uint16_t tls_checksum (uint16_t *buf16, int len)
{
    int         bytes_left = len;
    int         sum = 0;
    uint16_t    *w = buf16;
    uint16_t    answer = 0;

    while (bytes_left > 1)
    {
        sum += *w++;
        bytes_left -= sizeof(uint16_t);
    }

    if (bytes_left == 1)
    {   //odd byte
        *(uint8_t *) (&answer) = *(uint8_t *) w;
        sum += answer;
    }

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    answer = ~sum;

    return answer;
}


/**
tls_udp6_checksum - Build IPv6 UDP pseudo-header and calculate its checksum (Section 8.1 of RFC 2460)
@param[in]	    iphdr	 IP header
@param[in]	    udphdr	 UDP header
@param[in]	    payload	 Buffer that store the payload
@param[in]	    len 	 Payload length
@return     Checksum
*/
static uint16_t tls_udp6_checksum (struct ip6_hdr iphdr, struct udphdr udphdr, uint8_t *payload, int len)
{
    char buf[MAX_TLS_TX_SIZE + IP6_HDRLEN + UDP_HDRLEN];
    char *ptr;
    int  chksumlen = 0;
    int  i;

    ptr = &buf[0];  // ptr points to beginning of buffer buf

    // Copy source IP address into buf (128 bits)
    memcpy (ptr, &iphdr.ip6_src.s6_addr, sizeof (iphdr.ip6_src.s6_addr));
    ptr += sizeof (iphdr.ip6_src.s6_addr);
    chksumlen += sizeof (iphdr.ip6_src.s6_addr);

    // Copy destination IP address into buf (128 bits)
    memcpy (ptr, &iphdr.ip6_dst.s6_addr, sizeof (iphdr.ip6_dst.s6_addr));
    ptr += sizeof (iphdr.ip6_dst.s6_addr);
    chksumlen += sizeof (iphdr.ip6_dst.s6_addr);

    // Copy UDP length into buf (32 bits)
    memcpy (ptr, &udphdr.len, sizeof (udphdr.len));
    ptr += sizeof (udphdr.len);
    chksumlen += sizeof (udphdr.len);

    // Copy zero field to buf (24 bits)
    *ptr++ = 0;
    *ptr++ = 0;
    *ptr++ = 0;
    chksumlen += 3;

    // Copy next header field to buf (8 bits)
    memcpy (ptr, &iphdr.ip6_nxt, sizeof (iphdr.ip6_nxt));
    ptr += sizeof (iphdr.ip6_nxt);
    chksumlen += sizeof (iphdr.ip6_nxt);

    // Copy UDP source port to buf (16 bits)
    memcpy (ptr, &udphdr.source, sizeof (udphdr.source));
    ptr += sizeof (udphdr.source);
    chksumlen += sizeof (udphdr.source);

    // Copy UDP destination port to buf (16 bits)
    memcpy (ptr, &udphdr.dest, sizeof (udphdr.dest));
    ptr += sizeof (udphdr.dest);
    chksumlen += sizeof (udphdr.dest);

    // Copy UDP length again to buf (16 bits)
    memcpy (ptr, &udphdr.len, sizeof (udphdr.len));
    ptr += sizeof (udphdr.len);
    chksumlen += sizeof (udphdr.len);

    // Copy UDP checksum to buf (16 bits)
    // Zero, since we don't know it yet
    *ptr++ = 0;
    *ptr++ = 0;
    chksumlen += 2;

    // Copy payload to buf
    memcpy (ptr, payload, len);
    ptr += len;
    chksumlen += len;

    // Pad to the next 16-bit boundary
    for (i=0; i<len%2; i++, ptr++)
    {
        *ptr++ = 0;
        chksumlen++;
    }

    return tls_checksum ((uint16_t *) buf, chksumlen);
}


/**
tls_conn_close - Close client's socket fd and SSL
@param[in]	    sfd	 Client socket fd
@param[in]	    ssl	 Client SSL object pointer
@return
*/
void tls_conn_close(int sfd, void *ssl)
{
    if (ssl)
    {
        SSL_shutdown((SSL *)ssl);
        SSL_free((SSL *)ssl);
    }

    if (sfd > 0)
    {
        close(sfd);
    }
}


/**
tls_rx - Receive data from the SSL connection
@param[in]	    ssl	        SSL object pointer
@param[in]	    buf	        User supplied buffer to store the data
@param[in]	    ilength	    Length of the buffer
@param[in]	    src_addr    Source IPv6 address
@return Size of the data in bytes if success. Zero for SSL handshaking. ZIPR_KEEP_ALIVE for keep alive. Negative error number if failed.
*/
int tls_rx(void *ssl, uint8_t *buf, int ilength, struct sockaddr_in6 *src_addr)
{

    int                 res;
    int                 zip_len;
    uint8_t             read_buf[MAX_TLS_TX_SIZE + IP6_HDRLEN + UDP_HDRLEN] = {0}; //SSL read buf pointer
    struct ip6_hdr      *ipv6_hdr;
//  struct sockaddr_in6 sa6;
//  char                addr_str[80];
    struct udphdr       *udp_hdr;

    res = SSL_read((SSL *)ssl, read_buf, MAX_TLS_TX_SIZE + IP6_HDRLEN + UDP_HDRLEN);
    if (res > 0)
    {
        //debug_tls_svr("tls_rx got %d bytes\n", res);

        //IPv6 header len(40) + UDP header len(8)
        if (res > (IP6_HDRLEN + UDP_HDRLEN))
        {

            zip_len = res - IP6_HDRLEN - UDP_HDRLEN;

            ipv6_hdr = (struct ip6_hdr *)read_buf;

            udp_hdr = (struct udphdr *)(read_buf + IP6_HDRLEN);

            //plt_bin_show(&read_buf[IP6_HDRLEN + UDP_HDRLEN], zip_len);

            //Drop non-UDP packets
//          if (ipv6_hdr->ip6_nxt != IPPROTO_UDP)
//          {
//              //debug_tls_svr("Drop non-udp packet\n");
//              return 0;
//          }

            //Fill in the source addr
            src_addr->sin6_addr = ipv6_hdr->ip6_src;

            //Fill in the source port
            src_addr->sin6_port = udp_hdr->source;

            src_addr->sin6_family = AF_INET6;
            src_addr->sin6_flowinfo = 0;
            src_addr->sin6_scope_id = 0;

            //Check destination addr
//          sa6.sin6_addr = ipv6_hdr->ip6_dst;
//          if (util_ip_ntoa(sa6.sin6_addr.s6_addr, addr_str, 80, 0) == 0)
//          {
//              debug_tls_svr("tls_rx dest addr:%s\n", addr_str);
//          }

            zip_len = (zip_len > ilength) ? ilength : zip_len; //min(zip_len, ilength);
            memcpy(buf, read_buf + IP6_HDRLEN + UDP_HDRLEN, zip_len);

            return zip_len;
        }
        else
        {
            return(strncmp((const char *)read_buf, ZIPR_KEEPALIVE_STR, 9) == 0)? ZIPR_KEEP_ALIVE : 0;
            //debug_tls_svr("Keep alive packet received:'%s'\n", read_buf);
        }
    }
    else
    {
        int ssl_err_code;

        ssl_err_code = SSL_get_error((SSL *)ssl, res);

        debug_tls_svr("ssl_read err = %d, error code = %d\n", res, ssl_err_code);

        if (res == 0)
        {
            //The reason may either be a clean shutdown due to a "close notify" alert sent by the peer (in which case the
            //SSL_RECEIVED_SHUTDOWN flag in the ssl shutdown state is set (see SSL_shutdown(3), SSL_set_shutdown(3)). It is
            //also possible, that the peer simply shut down the underlying transport and the shutdown is incomplete.
            return -2;
        }

        switch(ssl_err_code)
        {
            case SSL_ERROR_WANT_READ:
            case SSL_ERROR_WANT_WRITE:
            case SSL_ERROR_WANT_CONNECT:
            case SSL_ERROR_WANT_ACCEPT:
            case SSL_ERROR_WANT_X509_LOOKUP:
                res = 0;
                break;
        }

        return res;
    }
}


/**
tls_tx - Send data to the other end of SSL connection
@param[in]	    ssl	        SSL object pointer
@param[in]	    data	    Data buffer
@param[in]	    len	        Length of data
@param[in]	    src_addr    Source IPv6 address
@param[in]	    dst_addr    Destination IPv6 address
@return Number of bytes sent if success. Negative error number if failed.
*/
int tls_tx(void *ssl, uint8_t *data, uint16_t len, struct sockaddr_in6 *src_addr, struct sockaddr_in6 *dst_addr)
{
    struct ip6_hdr  *ipv6_hdr;
    struct udphdr   *udp_hdr;
    uint8_t         tx_buf[MAX_TLS_TX_SIZE + IP6_HDRLEN + UDP_HDRLEN];
    int             res;

    if (len > MAX_TLS_TX_SIZE)
    {
        debug_tls_svr("tls_tx: data size too large\n");
        return -1;
    }

    ipv6_hdr = (struct ip6_hdr *)tx_buf;

    //IPV6 Header

    // IPv6 version (4 bits), Traffic class (8 bits), Flow label (20 bits)
    ipv6_hdr->ip6_flow = htonl((6 << 28) | (0 << 20) | 0);

    // Payload length (16 bits)
    ipv6_hdr->ip6_plen = htons(UDP_HDRLEN + len);

    // Next header (8 bits)
    ipv6_hdr->ip6_nxt = IPPROTO_UDP;

    // Hop limit  (8 bits) - use 255 (RFC 4861)
    ipv6_hdr->ip6_hops = 255;

    // Source IPv6 address (128 bits)
    ipv6_hdr->ip6_src = src_addr->sin6_addr;

    // Destination IPv6 address (128 bits)
    ipv6_hdr->ip6_dst = dst_addr->sin6_addr;

    //Fill in the UDP header
    // Define next part of buffer outpack to be a udphdr struct.
    udp_hdr = (struct udphdr *)(tx_buf + IP6_HDRLEN);

    // Source port number (16 bits): pick a number
    udp_hdr->source = src_addr->sin6_port;

    // Destination port number (16 bits): Z-wave prot number
    udp_hdr->dest = dst_addr->sin6_port;

    // Length of UDP datagram (16 bits): UDP header + UDP data
    udp_hdr->len = htons(UDP_HDRLEN + len);

    // UDP checksum (16 bits)
    udp_hdr->check = tls_udp6_checksum(*ipv6_hdr, *udp_hdr, data, len);

    // Append data to end of udphdr struct.
    memcpy(&tx_buf[IP6_HDRLEN + UDP_HDRLEN], data, len);

    //debug_tls_svr("App requested data len = %d\r\n", len);
    //debug_tls_svr("Total to be transfered len = %d\r\n", (len+IP6_HDRLEN + UDP_HDRLEN));

    //debug_tls_svr("tls_tx:\n");
    //plt_bin_show(data, len);

    res = SSL_write((SSL *)ssl, tx_buf, len + IP6_HDRLEN + UDP_HDRLEN);

    if (res >= (IP6_HDRLEN + UDP_HDRLEN))
    {
        res -= (IP6_HDRLEN + UDP_HDRLEN);
    }
    else if (res > 0)
    {
        res = 0;
    }
    else if (res < 0)
    {
        debug_tls_svr("tls_tx: failed = %d\n", res);
    }

    return res;
}


/**
tls_client_add - Add client's socket fd and SSL object pointer into the given buffers
@param[in]	    ctx	        Context
@param[in]	    clnt_ctx    Client contexts
@param[in]	    new_fd	    Client fd to be added
@param[in]	    new_ssl		Client SSL object pointer
@param[in]	    size	    Number of entries in clnt_ctx
@return Zero on success, non-zero on failure.
*/
static int tls_client_add(tls_svr_ctx_t *ctx, tls_clnt_ctx_t *clnt_ctx, int new_fd, SSL* new_ssl,
                          int size)
{
    int i;

    for (i=0; i<size; i++)
    {
        if (clnt_ctx[i].clnt_fd < 0)
        {
            clnt_ctx[i].clnt_fd = new_fd;
            clnt_ctx[i].clnt_ssl = new_ssl;
            clnt_ctx[i].clnt_state = TLS_HANDSHAKE_IN_PROGRESS;
            clnt_ctx[i].clnt_expire = ctx->tmr_tick + TLS_HANDSHAKE_TIMEOUT;
            return 0;
        }
    }

    return -1;
}


/**
tls_gw_discvr_start - Start Z/IP gateway discovery
@param[in]      tls_svr_ipv6    TLS server IPv6 address
@param[in]      ssl             SSL object pointer
@return         Return 0 on success, negative error number on failure.
*/
static int tls_gw_discvr_start(SSL *ssl, uint8_t *tls_svr_ipv6)
{
    static const uint8_t    all_routers_multicast[IPV6_ADDR_LEN] = { 0xFF, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2} ;
    uint8_t                 zip_frm[] = {COMMAND_CLASS_ZIP, COMMAND_ZIP_PACKET, 0, 0x40, 0xaa, 0, 0,
        COMMAND_CLASS_NETWORK_MANAGEMENT_PROXY, NODE_INFO_CACHED_GET, 0xbb, 0/*Max age*/, 0/*Node id*/};

    int                     bytes_sent;
    struct sockaddr_in6     sa_multicast;
    struct sockaddr_in6     sa_src;

    zip_frm[4] = rand() & 0xFF;
    zip_frm[9] = rand() & 0xFF;

    //source address
    sa_src.sin6_family = AF_INET6;
    sa_src.sin6_port = htons(ZWAVE_HOME_CTL_PORT);
    memcpy(sa_src.sin6_addr.s6_addr, tls_svr_ipv6, IPV6_ADDR_LEN);

    //multicast address
    sa_multicast.sin6_family = AF_INET6;
    sa_multicast.sin6_port = htons(ZWAVE_HOME_CTL_PORT);
    memcpy(sa_multicast.sin6_addr.s6_addr, all_routers_multicast, IPV6_ADDR_LEN);

    bytes_sent = tls_tx(ssl, zip_frm, sizeof(zip_frm), &sa_src, &sa_multicast);

    if (bytes_sent == sizeof(zip_frm))
    {
        return 0;
    }

    debug_tls_svr("tls_tx failed with error %d", bytes_sent);
    return -1;
}


/**
tls_gw_id_cmp - Compare two gateway id
@param[in]	s1	gw_id 1
@param[in]	s2	gw_id 2
@return     zero if s1 equals to s2
*/
static int tls_gw_id_cmp(uint8_t *s1, uint8_t *s2)
{
    tls_clnt_prof_t   *gw_id_1 = (tls_clnt_prof_t *)s1;
    tls_clnt_prof_t   *gw_id_2 = (tls_clnt_prof_t *)s2;

    return memcmp(gw_id_1->clnt_id, gw_id_2->clnt_id, ZIPR_ID_LEN);
}

/**
tls_handshake_chk - Check the SSL and ZIPR handshaking status
@param[in]	    svr_ctx	    TLS server context
@param[in]	    clnt_ctx	TLS client context
@return TLS_HANDSHAKE_XXX.
*/
static int tls_handshake_chk(tls_svr_ctx_t *svr_ctx, tls_clnt_ctx_t *clnt_ctx)
{
    int     res;
    int     ret;
    uint8_t *read_buf = NULL; //SSL read buffer pointer

    ret = clnt_ctx->clnt_state;   //default to current state

    if (ret == TLS_HANDSHAKE_IN_PROGRESS)
    {
        //SSL_set_accept_state(clnt_ctx->clnt_ssl);
        //SSL_read(clnt_ctx->clnt_ssl, )
        res = SSL_accept(clnt_ctx->clnt_ssl);
        debug_tls_svr("tls_handshake_chk: res = %d\n", res);

        res = SSL_get_error(clnt_ctx->clnt_ssl, res);

        switch (res)
        {
            case SSL_ERROR_WANT_WRITE :
                {
                    debug_tls_svr("SSL_ERROR_WANT_WRITE.\n");
                    clnt_ctx->clnt_flag_want_write = 1;
                    return TLS_HANDSHAKE_IN_PROGRESS;
                }
                break;

            case SSL_ERROR_WANT_READ :
                {
                    debug_tls_svr("SSL_ERROR_WANT_READ.\n");
                    clnt_ctx->clnt_flag_want_write = 0;
                    return TLS_HANDSHAKE_IN_PROGRESS;
                }
                break;

            case SSL_ERROR_WANT_X509_LOOKUP :
                {
                    //SSL Handshake is in progress
                    //No change to the clnt_ctx->clnt_state and return code. (Supposed to be TLS_HANDSHAKE_IN_PROGRESS.)
                    debug_tls_svr("SSL_ERROR_WANT_X509_LOOKUP.\n");
                    clnt_ctx->clnt_flag_want_write = 0;
                    return TLS_HANDSHAKE_IN_PROGRESS;
                }
                break;

            case SSL_ERROR_NONE :
                {
                    //SSL Handshake is done. Now check serial number and start other ZIPR init stuff
                    X509        *client_cert = NULL;
                    uint8_t     serial_num[4 + ZIPR_ID_LEN + ZIPR_PIN_LEN] = {0}; //which should be 20
                    char        pin[ZIPR_PIN_LEN + 1];
                    tls_clnt_prof_t clnt_prof;

                    clnt_ctx->clnt_flag_want_write = 0;

                    //ret = TLS_HANDSHAKE_SSL_OK;

                    //The handshake is complete and ok
                    debug_tls_svr("SSL hand shake was successful.\n");

                    res = SSL_get_verify_result(clnt_ctx->clnt_ssl);

                    if (res != X509_V_OK)
                    {
                        debug_tls_svr("Client cert SSL verification fail: %d\n", res);
                        return TLS_HANDSHAKE_ERR;
                    }

                    client_cert = SSL_get_peer_certificate(clnt_ctx->clnt_ssl);

                    if (client_cert != NULL)
                    {
                        int             minlen = 0;
                        ASN1_INTEGER*   pASN_Serial = X509_get_serialNumber(client_cert);

                        debug_tls_svr("%s Serial number length: %d\n",
                                      pASN_Serial->length == 20 ? "" : "Error:",
                                      pASN_Serial->length);

                        minlen = pASN_Serial->length > 20 ? 20 : pASN_Serial->length;
                        memcpy(serial_num, pASN_Serial->data, minlen);
                        X509_free(client_cert);

                        if (pASN_Serial->length != 20)
                        {
                            ret = TLS_HANDSHAKE_ERR;
                        }
                        else
                        {
                            //memcpy(serial_num, pASN_Serial->data, 4 + ZIPR_ID_LEN + ZIPR_PIN_LEN);

                            //serial_num = pASN_Serial->data;

                            debug_tls_svr("ZIPR ID: %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\n",
                                          serial_num[4], serial_num[5], serial_num[6], serial_num[7],
                                          serial_num[8], serial_num[9], serial_num[10], serial_num[11]);
                            memcpy(pin, serial_num + 12, ZIPR_PIN_LEN);
                            pin[ZIPR_PIN_LEN] = '\0';
                            debug_tls_svr("ZIPR PIN:%s\n", pin);

                            //
                            //Send configuration to ZIPR.
                            //
                            memcpy(clnt_prof.clnt_id, serial_num + 4, ZIPR_ID_LEN);
                            if (util_list_find(svr_ctx->clnt_prof_mtx,
                                               svr_ctx->clnt_prof_hd, (uint8_t *) &clnt_prof,
                                               tls_gw_id_cmp))
                            {
                                uint8_t buf[TLS_CFG_LENGTH] = {0};

                                buf[0] = COMMAND_CLASS_ZIP_PORTAL;

                                //Check PIN
                                if (memcmp(clnt_prof.clnt_pin, serial_num + 4 + ZIPR_ID_LEN, ZIPR_PIN_LEN) != 0)
                                {
                                    debug_tls_svr("PIN error\n");
                                    buf[1] = GATEWAY_CONFIGURATION_UNREGISTER;
                                    SSL_write(clnt_ctx->clnt_ssl, buf, 2);
                                    //ssl_write(clnt_ctx->clnt_ssl, buf, 2);
                                    return TLS_HANDSHAKE_AUTH_ERR;
                                }

                                buf[1] = GATEWAY_CONFIGURATION_SET;

                                //copy the Z/IP gateway LAN Ipv6 addr
                                memcpy(buf + TLS_CFG_LAN_IP_OFFSET, clnt_prof.clnt_ipv6_addr, IPV6_ADDR_LEN);

                                //copy the prefix length of gateway LAN ipv6 addr -- 1byte
                                buf[TLS_CFG_LAN_PREFIX_OFFSET] = clnt_prof.clnt_ipv6_prefix;

                                //copy the portal Ipv6 addr (tunnel prefix)
                                memcpy(buf + TLS_CFG_PORTAL_IP_OFFSET, clnt_prof.svr_ipv6_addr, IPV6_ADDR_LEN);

                                //copy the prefix length of portal ipv6 addr -- 1byte
                                buf[TLS_CFG_PORTAL_PREFIX_OFFSET] = clnt_prof.svr_ipv6_prefix;

                                //copy default gateway Ipv6 addr
                                memcpy(buf + TLS_CFG_DFLT_GW_IP_OFFSET, clnt_prof.clnt_dflt_gw, IPV6_ADDR_LEN);

                                //copy PAN prefix Ipv6 addr (with 64 prefix length)
                                memcpy(buf + TLS_CFG_PAN_PREFIX_OFFSET, clnt_prof.clnt_pan_prefix, IPV6_ADDR_LEN );

                                res = SSL_write(clnt_ctx->clnt_ssl, buf, TLS_CFG_LENGTH);

                                if (res == TLS_CFG_LENGTH)
                                {
                                    //Copy the client profile used
                                    clnt_ctx->clnt_prof = clnt_prof;
                                    ret = TLS_HANDSHAKE_WAIT_ZIPR_READY;
                                }
                                else
                                {
                                    debug_tls_svr("Write config to ZIPR err: %d:\n", res);

                                    ret = TLS_HANDSHAKE_ERR;
                                }

                            }
                            else
                            {
                                uint8_t buf[2];

                                buf[0] = COMMAND_CLASS_ZIP_PORTAL;
                                buf[1] = GATEWAY_CONFIGURATION_UNREGISTER;
                                SSL_write(clnt_ctx->clnt_ssl, buf, 2);
                                debug_tls_svr("ZIPR id not found\n");
                                ret = TLS_HANDSHAKE_AUTH_ERR;
                            }
                        }
                    }
                    else
                    {
                        debug_tls_svr("No Client cert is found. \n");
                        return TLS_HANDSHAKE_ERR;
                    }
                }
                break;

            case SSL_ERROR_SYSCALL:
                {
                    debug_tls_svr("SSL_ERROR_SYSCALL error: %lu\n", ERR_get_error());
                    debug_tls_svr("SSL Handshake error = %d\n", res);

                    clnt_ctx->clnt_flag_want_write = 0;
                    ret = TLS_HANDSHAKE_ERR;
                }
                break;

            case SSL_ERROR_SSL:
                {
                    long lerr = ERR_get_error();
                    //ERR_print_errors_fp(stderr);
                    debug_tls_svr("SSL_ERROR_SSL\n");

                    debug_tls_svr("error string: %s\n", ERR_error_string(lerr, NULL));
                    debug_tls_svr("Reason string: %s\n", ERR_reason_error_string(lerr));
                    debug_tls_svr("SSL Handshake error = %d\n", res);
                    clnt_ctx->clnt_flag_want_write = 0;
                    ret = TLS_HANDSHAKE_ERR;
                }
                break;

            case SSL_ERROR_ZERO_RETURN:
                {
                    debug_tls_svr("SSL_ERROR_ZERO_RETURN error: %lu\n", ERR_get_error());
                    debug_tls_svr("SSL Handshake error = %d\n", res);
                    clnt_ctx->clnt_flag_want_write = 0;
                    ret = TLS_HANDSHAKE_ERR;
                }
                break;
        }
    }
    //else if(ret == TLS_HANDSHAKE_WAIT_ZIPR_READY)
    else
    {
        //SSL_set_accept_state(clnt_ctx->clnt_ssl);
        //SSL_read(clnt_ctx->clnt_ssl, )
        read_buf = OPENSSL_malloc(MAX_TLS_TX_SIZE + IP6_HDRLEN + UDP_HDRLEN);
        if (read_buf == NULL)
        {
            debug_tls_svr("OPENSSL_malloc fail\n");
            return TLS_HANDSHAKE_ERR;
        }
        res = SSL_read(clnt_ctx->clnt_ssl, read_buf, MAX_TLS_TX_SIZE);
        debug_tls_svr("SSL_read: res = %d\n", res);

        if (res <= 0)
        {
            res = SSL_get_error(clnt_ctx->clnt_ssl,res);
            debug_tls_svr("SSL_read err = %d\n", res);
            ret = TLS_HANDSHAKE_ERR;
        }
        else
        {
            switch (clnt_ctx->clnt_state)
            {
                //----------------------------------------------
                case TLS_HANDSHAKE_SSL_OK:
                //----------------------------------------------
                    if (res == 20)
                    {
                    }
                    break;

                //----------------------------------------------
                case TLS_HANDSHAKE_WAIT_ZIPR_READY:
                //----------------------------------------------
                    if (res >= 3)
                    {
                        if ((read_buf[0] == COMMAND_CLASS_ZIP_PORTAL) && (read_buf[1] == GATEWAY_CONFIGURATION_STATUS))
                        {
                            if (read_buf[2] == 0xFF)
                            { //O.k.
                                debug_tls_svr("Gateway configuration status: o.k.\n");

                                //Check whether Z/IP gateway discovery is required
                                if (util_ipv6_zero_cmp(clnt_ctx->clnt_prof.clnt_ipv6_addr) == 0)
                                {
                                    debug_tls_svr("Start Z/IP gateway discovery\n");
                                    tls_gw_discvr_start(clnt_ctx->clnt_ssl, clnt_ctx->clnt_prof.svr_ipv6_addr);
                                    //Set timeout
                                    clnt_ctx->clnt_expire = svr_ctx->tmr_tick + TLS_GW_DISCVR_TIMEOUT;
                                    ret = TLS_HANDSHAKE_ZIP_GW_DISCVR;
                                }
                                else
                                {
                                    debug_tls_svr("Configuration to ZIPR done!\n");
                                    ret = TLS_HANDSHAKE_COMPLETED;
                                }
                            }
                            else
                            { //Failed
                                debug_tls_svr( "Gateway configuration status: failed\n");
                                ret = TLS_HANDSHAKE_ERR;
                            }
                            break;
                        }
                    }
                    ret = TLS_HANDSHAKE_UNKNOWN_CMD;
                    break;

                //----------------------------------------------
                case TLS_HANDSHAKE_ZIP_GW_DISCVR:
                //----------------------------------------------
                    if (res > (IP6_HDRLEN + UDP_HDRLEN))
                    {
                        struct ip6_hdr *ipv6_hdr;

                        ipv6_hdr = (struct ip6_hdr *) read_buf;

                        //udp_hdr = (struct udphdr *)(read_buf + IP6_HDRLEN);

                        //plt_bin_show(&read_buf[IP6_HDRLEN + UDP_HDRLEN], zip_len);

                        //Check for UDP packets
                        if (ipv6_hdr->ip6_nxt == IPPROTO_UDP)
                        {
                            int rcx_size;
                            uint8_t *buf;
                            struct sockaddr_in6 sa_src;
                            char addr_str[80];

                            sa_src.sin6_addr = ipv6_hdr->ip6_src;
                            rcx_size = res - IP6_HDRLEN - UDP_HDRLEN;

                            if (util_ip_ntoa(sa_src.sin6_addr.s6_addr, addr_str, 80, 0) == 0)
                            {
                                debug_tls_svr("GW discovery response from:%s\n", addr_str);
                            }

                            //Copy the buffer pointer
                            buf = read_buf + IP6_HDRLEN + UDP_HDRLEN;

                            //check the response
                            if (rcx_size >= 14)
                            {
                                //Check Z/IP header
                                if ((buf[0] == COMMAND_CLASS_ZIP) && (buf[1] == COMMAND_ZIP_PACKET))
                                {
                                    //Check for Z-wave command
                                    if (buf[3] & ZIP_FRM_ZWAVE_CMD)
                                    {
                                        unsigned cmd_offset = 7;

                                        if (buf[3] & ZIP_FRM_HDR_EXT)
                                        {   //Adjust for header extension
                                            cmd_offset += buf[7];
                                        }

                                        if (rcx_size >= (cmd_offset + 2))
                                        {
                                            if ((buf[cmd_offset] == COMMAND_CLASS_NETWORK_MANAGEMENT_PROXY)
                                                && (buf[cmd_offset + 1] == NODE_INFO_CACHED_REPORT))
                                            { //Save the gateway IP
                                                memcpy(clnt_ctx->clnt_prof.clnt_ipv6_addr,
                                                       sa_src.sin6_addr.s6_addr, IPV6_ADDR_LEN);
                                                debug_tls_svr("Configuration to ZIPR done!\n");
                                                ret = TLS_HANDSHAKE_COMPLETED;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    break;
            }
        }
    }

    //Update the client state
    clnt_ctx->clnt_state = ret;

    if (read_buf != NULL)
        OPENSSL_free(read_buf);

    return ret;
}


/**
tls_new_clnt_conn_handler - New client connection handler
@param[in]	ctx         TLS server context
@param[in]	clnt_ctx    TLS client context
@param[in]	clnt_cnt    Number of TLS clients who still haven't completed handshaking
@return Zero on success, -1 on accept() failure, -2 on other failure.
*/
static int tls_new_clnt_conn_handler(tls_svr_ctx_t *ctx, tls_clnt_ctx_t *clnt_ctx, int clnt_cnt)
{
#if defined(_WINDOWS) || defined(WIN32)
    SOCKET              new_fd;
#else
    int                 new_fd;
#endif
    int                 fd_flg;
    int                 yes = 1;
    SSL                 *new_ssl;
    socklen_t           clnt_addr_len;
    struct sockaddr_in  clnt_saddr;
    //struct linger       so_linger;

    clnt_addr_len = sizeof(struct sockaddr_in);

    if ((new_fd = accept(ctx->sock_fd,(struct sockaddr *)&clnt_saddr, &clnt_addr_len)) < 0)
    {
        int sys_err_num = errno;
        //perror("tls_svr_thrd:accpet()");
        debug_tls_svr("tls_new_clnt_conn_handler:accept with errno:%d\n", sys_err_num);
        return -1;
    }

    if (clnt_cnt >= MAX_CLIENT_TLS)
    {   //Reject connection
        debug_tls_svr("Max TLS client exceeded, reject connection\n");
        goto l_NEW_CONN_ERR1;
    }

    //Set keep alive option
    if (setsockopt(new_fd, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(yes)) < 0)
    {
        debug_tls_svr("setsockopt SO_KEEPALIVE failed\n");
        goto l_NEW_CONN_ERR1;
    }

    //Set the client fds to be non-blocking as well
    fd_flg = fcntl(new_fd, F_GETFL, 0);

    fd_flg |= O_NONBLOCK;

    if (fcntl(new_fd, F_SETFL, fd_flg) < 0)
    {
        debug_tls_svr("set client fd O_NONBLOCK failed\n");
        goto l_NEW_CONN_ERR1;
    }

    //Set linger sock option
//  so_linger.l_onoff = 1;
//  so_linger.l_linger = 5;//wait for 5 seconds
//  if (setsockopt(new_fd, SOL_SOCKET, SO_LINGER, (const char *)&so_linger, sizeof(so_linger)) < 0)
//  {
//      debug_tls_svr("Setsockopt SO_LINGER failed\n");
//      goto l_NEW_CONN_ERR1;
//  }

    new_ssl = SSL_new((SSL_CTX *)ctx->ssl_ctx);

    if (!new_ssl)
    {
        debug_tls_svr("SSL_new failed\n");
        goto l_NEW_CONN_ERR1;
    }

    if (!SSL_set_fd(new_ssl, new_fd))
    {
        debug_tls_svr("SSL_set_fd failed\n");
        goto l_NEW_CONN_ERR2;
    }

    //SSL_set_mode(new_ssl, SSL_MODE_AUTO_RETRY);
    debug_tls_svr("ZIPR Connected !!!\n");

    //Store the client fd, SSL object pointer, etc
    if (tls_client_add(ctx, clnt_ctx, new_fd, new_ssl, MAX_CLIENT_TLS) == 0)
    {
        return 0;
    }

l_NEW_CONN_ERR2:
    SSL_shutdown(new_ssl);
    SSL_free(new_ssl);
l_NEW_CONN_ERR1:
#if defined(_WINDOWS) || defined(WIN32)
    closesocket(new_fd);
#else
    close(new_fd);
#endif
    return -2;

}


/**
tls_handshake_handler - TLS handshake handler
@param[in]	ctx         TLS server context
@param[in]	clnt_ctx    TLS client context
@return Zero on handshake completed or failed; else return non-zero on handshake in progress
*/
static int tls_handshake_handler(tls_svr_ctx_t *ctx, tls_clnt_ctx_t *clnt_ctx)
{
    int result;

    result = tls_handshake_chk(ctx, clnt_ctx);

    switch (result)
    {
        case TLS_HANDSHAKE_COMPLETED:
            //Callback to hand over the fd and SSL object for secure read and write
            if (!ctx->cb(clnt_ctx->clnt_fd, clnt_ctx->clnt_ssl, &clnt_ctx->clnt_prof, ctx->usr_param))
            {   //Hand over failed, the connection was rejected
                tls_conn_close(clnt_ctx->clnt_fd, clnt_ctx->clnt_ssl);
            }
            //Free up client context slot
            clnt_ctx->clnt_fd = -1;
            result = 0;
            break;

        case TLS_HANDSHAKE_AUTH_ERR:
            //Set timeout
            clnt_ctx->clnt_expire = ctx->tmr_tick + TLS_UNREGISTER_TIMEOUT;
            result = 1;
            break;

        case TLS_HANDSHAKE_ERR:
            //Close the fd and SSL object
            tls_conn_close(clnt_ctx->clnt_fd, clnt_ctx->clnt_ssl);
            //Free up client context slot
            clnt_ctx->clnt_fd = -1;
            result = 0;
            break;

        default:
            result = 1;
            break;
    }

    return result;
}


/**
tls_svr_thrd - TLS server thread
@param[in]	data    Context
@return
*/
static void tls_svr_thrd(void *data)
{
    tls_svr_ctx_t   *ctx = (tls_svr_ctx_t *)data;
    int             res;
    int             rate_limit = 0; //Flag to turn on rate-limit for select loop
    int             i;
    int             sys_err_num;
    int             max_fdn;    //Maximum file descriptor number
    int             clnt_cnt = 0;
    int             fd_cnt;
    tls_clnt_ctx_t  clnt_ctx[MAX_CLIENT_TLS];
    fd_set          rd_fds;
    fd_set          wr_fds;   //For some of the fds encountered "ERROR_WANT_WRITE" during handshaking
    struct timeval  tv;

    //Update thread status
    ctx->svr_thrd_sts = 1;

    //Init client fd buffer
    for (i=0; i<MAX_CLIENT_TLS; i++)
    {
        clnt_ctx[i].clnt_fd = -1;
        clnt_ctx[i].clnt_flag_want_write = 0;
    }

    FD_ZERO(&wr_fds);

    while (ctx->svr_thrd_run)
    {
        //Rate limit the select loop
        if (rate_limit)
        {
            plt_sleep(1000);
            rate_limit = 0;
        }

        //Waiting for connection

        //Clear the read set
        FD_ZERO(&rd_fds);
        //Clear the write set
        FD_ZERO(&wr_fds);

        //Add server descriptor to the set
        FD_SET(ctx->sock_fd, &rd_fds);

        max_fdn = ctx->sock_fd;

        //Add client descriptors to the set
        if (clnt_cnt > 0)
        {
            for (i=0; i<MAX_CLIENT_TLS; i++)
            {
                if (clnt_ctx[i].clnt_fd > 0)
                {
                    FD_SET(clnt_ctx[i].clnt_fd, &rd_fds);
                    if (clnt_ctx[i].clnt_fd > max_fdn)
                    {
                        max_fdn = clnt_ctx[i].clnt_fd;
                    }

                    //Add the client fd to write set if the last operation needs write
                    if (clnt_ctx[i].clnt_flag_want_write == 1)
                    {
                        FD_SET(clnt_ctx[i].clnt_fd, &wr_fds);
                    }
                }
            }
        }

        // Wait until either socket has data ready to be received or timeout
        tv.tv_sec = 3;
        tv.tv_usec = 0;

        fd_cnt = select(max_fdn + 1, &rd_fds, &wr_fds, NULL, &tv);

        if (fd_cnt == TPT_SOCKET_ERR)
        {
            //debug_gw_discvr("select failed with error %d\n", WSAGetLastError());
            sys_err_num = errno;
            debug_tls_svr("************************ select failed: errno=%d\n", sys_err_num);
            //perror("************************  select failed:");

            //Try to eliminate bad file descriptor
            for (i=0; i<MAX_CLIENT_TLS; i++)
            {
                if (clnt_ctx[i].clnt_fd > 0)
                {
                    if (tls_handshake_handler(ctx, &clnt_ctx[i]) == 0)
                    {
                        clnt_cnt--;
                    }
                }
            }
        }

        //Check for handshaking timeout
        if (ctx->chk_tmr)
        {
            ctx->clean_up_tick = 0;
            for (i=0; i<MAX_CLIENT_TLS; i++)
            {
                if ((clnt_ctx[i].clnt_fd > 0) && (util_tmr_exp_chk(ctx->tmr_tick, clnt_ctx[i].clnt_expire)))
                {
                    debug_tls_svr("Handshaking timeout, fd=%d\n", clnt_ctx[i].clnt_fd);
                    //Close the fd and SSL object
                    tls_conn_close(clnt_ctx[i].clnt_fd, clnt_ctx[i].clnt_ssl);
                    //Free up client context slot
                    clnt_ctx[i].clnt_fd = -1;
                    clnt_cnt--;
                }
            }
            ctx->chk_tmr = 0;
        }

        if (fd_cnt == 0)
        {
            //Timeout
            //debug_tls_svr("select timeout\n");
            continue;
        }
        else if (fd_cnt > 0)
        {
            if (FD_ISSET(ctx->sock_fd, &rd_fds))
            {
                fd_cnt--;

                res = tls_new_clnt_conn_handler(ctx, clnt_ctx, clnt_cnt);
                if (res == 0)
                {
                    clnt_cnt++;
                }
                else if (res == -1)
                {   //accept() failed
                    rate_limit = 1;
                }
            }

            //Check for client file descriptors that signal read or write ready
            if (fd_cnt > 0)
            {
                for (i=0; i<MAX_CLIENT_TLS; i++)
                {
                    if ((clnt_ctx[i].clnt_fd > 0)
                        && (FD_ISSET(clnt_ctx[i].clnt_fd, &rd_fds) || FD_ISSET(clnt_ctx[i].clnt_fd, &wr_fds)))
                    {
                        if (tls_handshake_handler(ctx, &clnt_ctx[i]) == 0)
                        {
                            clnt_cnt--;
                        }
                    }
                }
            }
        }
    }

    //Clean up
    for (i=0; i<MAX_CLIENT_TLS; i++)
    {
        if (clnt_ctx[i].clnt_fd > 0)
        {
            tls_conn_close(clnt_ctx[i].clnt_fd, clnt_ctx[i].clnt_ssl);
        }
    }
    debug_tls_svr("tls_svr_thrd exited\n");
    ctx->svr_thrd_sts = 0;
    return;

}


/**
tls_svr_setup - Setup TLS server to listen at TCP port 44123
@param[in]	    ctx		    Context
@return Zero on success, non-zero on failure.
*/
static int tls_svr_setup(tls_svr_ctx_t *ctx)
{
    int                 sock_fd;
    int                 res;
    int                 ret = 0;
    int                 old_flg;
    SSL_CTX             *ssl_ctx = NULL;
    struct sockaddr_in  sa4;
    struct linger       so_linger;

#if defined(OPENSSL_THREADS)
    // Thread support enabled
    debug_tls_svr("Openssl lib support threads\n");
#else
    // No thread support
    debug_tls_svr("Openssl lib doesn't support threads\n");
    return -1;
#endif


    //Open socket and bind it to the source address
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        debug_tls_svr("Open TCP socket failed\n");
        ret = -1;
        goto l_TLS_SVR_setup_ERROR;
    }

    //Set socket as non-blocking
    old_flg = fcntl (sock_fd, F_GETFL, 0);

    old_flg |= O_NONBLOCK;

    res = fcntl(sock_fd, F_SETFL, old_flg);

    if (res < 0)
    {
        debug_tls_svr("set O_NONBLOCK failed\n");
        ret = -1;
        goto l_TLS_SVR_setup_ERROR;

    }

    //Set sock option to abort socket when closed to avoid TCP socket goes into TIME_WAIT state.
    //Any pending data is immediately discarded upon close()
    so_linger.l_onoff = 1;
    so_linger.l_linger = 0;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_LINGER, (const char *)&so_linger, sizeof(so_linger)) < 0)
    {
        debug_tls_svr("Setsockopt SO_LINGER failed\n");
        ret = -1;
        goto l_TLS_SVR_setup_ERROR;
    }

    sa4.sin_family           =   AF_INET;
    sa4.sin_port             =   htons(ctx->svr_port);
    sa4.sin_addr.s_addr      =   INADDR_ANY;

    res = bind(sock_fd, (const struct sockaddr *)&sa4, sizeof(sa4));

    if (res < 0)
    {
        //perror("tls_svr_setup: bind()");
        debug_tls_svr("Bind failed with errno:%d\n", errno);
        ret = -1;
        goto l_TLS_SVR_setup_ERROR;
    }

    res = listen(sock_fd, MAX_TLS_Q_LEN);

    if (res < 0)
    {
        //perror("tls_svr_setup: listen()");
        debug_tls_svr("Listen failed with err:%d\n", res);
        ret = -1;
        goto l_TLS_SVR_setup_ERROR;
    }

    SSL_load_error_strings();
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    //OpenSSL_add_all_ciphers();

    //Create SSL context
    //Since ZIPR is using TLS v1.1, we also use this.
    if ((ssl_ctx = SSL_CTX_new(TLSv1_1_server_method())) == NULL)
    {
        debug_tls_svr_err(ERR_get_error());
        debug_tls_svr("Error: SSL Server context is invalid\n");
        ret = -1;
        goto l_TLS_SVR_setup_ERROR;
    }
#ifdef ENABLE_DECRYPT_FRAME
    //AES256-SHA256:AES256-SHA:AES128-GCM-SHA256:
    if (SSL_CTX_set_cipher_list(ssl_ctx, "AES128-SHA256:AES128-SHA") == 0)
    {
        debug_tls_svr("Error: SSL Server cipher list is invalid\n");
        ret = -1;
        goto l_TLS_SVR_setup_ERROR;
    }
#endif

    if (SSL_CTX_use_certificate_file(ssl_ctx, ctx->ssl_file, SSL_FILETYPE_ASN1) != 1)
    {
        debug_tls_svr_err(ERR_get_error());
        debug_tls_svr("SSL Certificate  is undefined.\n");
        ret = -1;
        goto l_TLS_SVR_setup_ERROR;
    }

    if (SSL_CTX_use_PrivateKey_file(ssl_ctx, ctx->pvt_key_file, SSL_FILETYPE_ASN1) != 1)
    {
        debug_tls_svr_err(ERR_get_error());
        debug_tls_svr("Private Key is undefined.\n");
        ret = -1;
        goto l_TLS_SVR_setup_ERROR;
    }

    /* Check if the server certificate and private-key matches */
    if (!SSL_CTX_check_private_key(ssl_ctx))
    {
        debug_tls_svr_err(ERR_get_error());
        debug_tls_svr("Private key does not match the certificate public key.\n");
        ret = -1;
        goto l_TLS_SVR_setup_ERROR;
    }

    if (SSL_CTX_load_verify_locations(ssl_ctx, ctx->ca_file, NULL) != 1)
    {
        debug_tls_svr_err(ERR_get_error());
        debug_tls_svr("CA Certificate  is undefined.\n");
        ret = -1;
        goto l_TLS_SVR_setup_ERROR;
    }

    SSL_CTX_set_client_CA_list(ssl_ctx, SSL_load_client_CA_file(ctx->ca_file));
    /* Set to require peer (client) certificate verification */
    SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT | SSL_VERIFY_CLIENT_ONCE, NULL);

    //Enable server session caching
    {
        long lmode = SSL_CTX_get_session_cache_mode(ssl_ctx);
        lmode |= SSL_SESS_CACHE_SERVER;
        SSL_CTX_set_session_cache_mode(ssl_ctx, lmode);

        SSL_CTX_set_session_id_context(ssl_ctx, (void*)&s_server_session_id_context, sizeof(s_server_session_id_context));

        //debug_tls_svr("Original session time out is: %ld\n", SSL_CTX_get_timeout(ssl_ctx));
        //Set the session time out to be 24 hours
        SSL_CTX_set_timeout(ssl_ctx, 24*3600);
    }

l_TLS_SVR_setup_ERROR:
    if (ret == 0)
    {
        //Save the result
        ctx->sock_fd = sock_fd;
        ctx->ssl_ctx = ssl_ctx;
    }
    else
    {
        if (sock_fd >= 0)
        {
            close(sock_fd);
        }

        if (ssl_ctx != NULL)
        {
            EVP_cleanup();
            SSL_CTX_free(ssl_ctx);
        }
    }

    return ret;
}


/**
tls_tmout_cb - Timer tick timeout callback
@param[in] data     Pointer to TLS server context
@return
*/
static void    tls_tmout_cb(void *data)
{
    tls_svr_ctx_t   *ctx = (tls_svr_ctx_t *)data;

    //Increment timer tick
    ctx->tmr_tick++;

    //Increment internal clean up tick
    if (ctx->chk_tmr == 0)
    {
        ctx->clean_up_tick++;
        if (ctx->clean_up_tick >= TLS_TIMER_CHK)
        {
            ctx->chk_tmr++;
        }
    }
}


/**
tls_msg_show - show message to the user
@param[in] msg   The output message to printf.
@return
*/
static void tls_msg_show(void *msg, void *display_ctx)
{
#if defined(_WINDOWS) || defined(WIN32)
#ifdef  _UNICODE
    wprintf(L"%s", (wchar_t *)msg);
#else
    printf("%s", (char *)msg);
#endif
#else
    fputs((const char *)msg, stdout);
#endif
}


/**
tls_svr_start - Start TLS server to listen for ZIPR connection
@param[in]	    clnt_prof	Client profiles
@param[in]	    prof_cnt	Number of client profiles
@param[in]	    cb		    Callback function when a new gateway has completed the TLS handshake
@param[in]	    usr_param	User defined parameter used in callback function
@param[in]	    ca_file	    File path to CA certificate
@param[in]	    ssl_file	File path to SSL certificate
@param[in]	    pvt_key_file	File path to private key
@param[in]	    port	    TLS server listening port. If zero, default port will be used.
@return Context on success, null on failure.
@post   Caller is required to call tls_svr_shutdown() and tls_svr_stop() with the returned context if it is not null.
*/
void *tls_svr_start(tls_clnt_prof_t *clnt_prof, int prof_cnt, tls_svr_cb_t cb, void *usr_param,
                    char *ca_file, char *ssl_file, char *pvt_key_file, uint16_t port)
{
    tls_svr_ctx_t *ctx;
#if defined(_WINDOWS) || defined(WIN32)
    WSADATA     wsa_dat;
#endif
    int         result;
    int         wait_cnt;
    sigset_t    sig_set;

    //Block SIGPIPE signal
    sigemptyset(&sig_set);
    sigaddset(&sig_set, SIGPIPE);
    result = pthread_sigmask(SIG_BLOCK, &sig_set, NULL);
    if (result != 0)
    {
        debug_tls_svr("Block SIGPIPE signal with error:%d\n", result);
        return NULL;
    }

    ctx = (tls_svr_ctx_t *)calloc(1, sizeof(tls_svr_ctx_t));

    if (!ctx)
    {
        return NULL;
    }

    if (!plt_mtx_init(&ctx->clnt_prof_mtx))
    {
        free(ctx);
        return NULL;
    }

#if defined(_WINDOWS) || defined(WIN32)
    // Initialize Winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsa_dat);
    if (result != NO_ERROR)
    {
        debug_tls_svr("WSAStartup failed with error %d", result);
        plt_mtx_destroy(ctx->clnt_prof_mtx);
        free(ctx);
        return NULL;
    }
#endif

    //Save certificate file names
    ctx->ca_file = malloc(strlen(ca_file) + 1);
    ctx->ssl_file = malloc(strlen(ssl_file) + 1);
    ctx->pvt_key_file = malloc(strlen(pvt_key_file) + 1);

    if (!ctx->ca_file || !ctx->ssl_file || !ctx->pvt_key_file)
    {
        goto l_TLS_SVR_ERROR;
    }

    strcpy(ctx->ca_file, ca_file);
    strcpy(ctx->ssl_file, ssl_file);
    strcpy(ctx->pvt_key_file, pvt_key_file);

    //Save the TLS server listening port
    ctx->svr_port = (port == 0)? TLS_SERVER_PORT : port;

    //Initialize platform
    if (plt_init(&ctx->plt_ctx, tls_msg_show, NULL) != 0)
    {
        goto l_TLS_SVR_ERROR;
    }

    //Setup network connection
    if (tls_svr_setup(ctx) != 0)
    {
        goto l_TLS_SVR_ERROR1;
    }

    //Save the callback function
    ctx->cb = cb;
    ctx->usr_param = usr_param;

    //Save the client profiles
    while (clnt_prof && (prof_cnt-- > 0))
    {
        result = util_list_add(ctx->clnt_prof_mtx, &ctx->clnt_prof_hd, (uint8_t *)clnt_prof, sizeof(tls_clnt_prof_t));
        if (result < 0)
        {
            goto l_TLS_SVR_ERROR2;
        }
        clnt_prof++;
    }

    //Start periodic timer
    ctx->tmr_tick_ctx = plt_periodic_start(&ctx->plt_ctx, TLS_TIMER_TICK, tls_tmout_cb, ctx);

    if (!ctx->tmr_tick_ctx)
        goto l_TLS_SVR_ERROR2;

    //Start TLS server thread
    ctx->svr_thrd_run = 1;
    if (plt_thrd_create(tls_svr_thrd, ctx) < 0)
    {
        goto l_TLS_SVR_ERROR3;
    }

    //Wait for thread start to run
    wait_cnt = 50;
    while (wait_cnt-- > 0)
    {
        if (ctx->svr_thrd_sts == 1)
            break;
        plt_sleep(100);
    }
    return ctx;

    l_TLS_SVR_ERROR3:
    plt_tmr_stop(&ctx->plt_ctx, ctx->tmr_tick_ctx);
    l_TLS_SVR_ERROR2:
    util_list_flush(ctx->clnt_prof_mtx, &ctx->clnt_prof_hd);
    close(ctx->sock_fd);
    EVP_cleanup();
    SSL_CTX_free((SSL_CTX *)ctx->ssl_ctx);
    l_TLS_SVR_ERROR1:
    plt_exit(&ctx->plt_ctx);
    l_TLS_SVR_ERROR:
    free(ctx->ca_file);
    free(ctx->ssl_file);
    free(ctx->pvt_key_file);
#if defined(_WINDOWS) || defined(WIN32)
    WSACleanup(); //Clean up Winsock
#endif
    plt_mtx_destroy(ctx->clnt_prof_mtx);
    free(ctx);
    return NULL;

}


/**
tls_svr_shutdown - Shutdown TLS server listening socket
@param[in]	    ctx		    The context returned from the call to tls_svr_start()
@return Zero on success, non-zero on failure.
*/
int tls_svr_shutdown(void *ctx)
{
    tls_svr_ctx_t *svr_ctx = (tls_svr_ctx_t *)ctx;

    if (!ctx)
    {
        return -1;
    }

    plt_tmr_stop(&svr_ctx->plt_ctx, svr_ctx->tmr_tick_ctx);

    if (svr_ctx->svr_thrd_sts)
    {
        //Stop the thread
        int  wait_count = 50;

        svr_ctx->svr_thrd_run = 0;
        while (wait_count-- > 0)
        {
            if (svr_ctx->svr_thrd_sts == 0)
                break;
            plt_sleep(100);
        }
    }
    plt_exit(&svr_ctx->plt_ctx);

    close(svr_ctx->sock_fd);

    return 0;
}


/**
tls_svr_stop - Stop TLS server
@param[in]	    ctx		    The context returned from the call to tls_svr_start()
@return Zero on success, non-zero on failure.
@post After the call, the ctx is invalid and should not be used
*/
int tls_svr_stop(void *ctx)
{
    tls_svr_ctx_t *svr_ctx = (tls_svr_ctx_t *)ctx;

    if (!ctx)
    {
        return -1;
    }

    //Free all the TLS connections under this server
    EVP_cleanup();
    SSL_CTX_free((SSL_CTX *)svr_ctx->ssl_ctx);

    util_list_flush(svr_ctx->clnt_prof_mtx, &svr_ctx->clnt_prof_hd);
    plt_mtx_destroy(svr_ctx->clnt_prof_mtx);
    free(svr_ctx->ca_file);
    free(svr_ctx->ssl_file);
    free(svr_ctx->pvt_key_file);
    free(svr_ctx);
#if defined(_WINDOWS) || defined(WIN32)
    WSACleanup(); //Clean up Winsock
#endif
    return 0;
}


/**
tls_clnt_add - Add a client profile to the TLS server internal list, overwriting old entry with the same gateway id.
@param[in]	    ctx		    The context returned from the call to tls_svr_start()
@param[in]	    clnt_prof	Client profile
@return                     Return non-zero on success, zero on failure.
*/
int tls_clnt_add(void *ctx, tls_clnt_prof_t *clnt_prof)
{
    tls_svr_ctx_t   *svr_ctx = (tls_svr_ctx_t *)ctx;
    int             result;
    result = util_list_rplc(svr_ctx->clnt_prof_mtx, &svr_ctx->clnt_prof_hd, (uint8_t *)clnt_prof,
                            sizeof(tls_clnt_prof_t), tls_gw_id_cmp);

    return(result >= 0)? 1 : 0;

}


/**
tls_clnt_rm - Remove a client profile from the TLS server internal list
@param[in]	    ctx		    The context returned from the call to tls_svr_start()
@param[in]	    gw_id	    Gateway id
@return                     Return non-zero on success, zero on failure.
*/
int tls_clnt_rm(void *ctx, uint8_t *gw_id)
{
    tls_svr_ctx_t   *svr_ctx = (tls_svr_ctx_t *)ctx;
    tls_clnt_prof_t     clnt_prof;

    memcpy(clnt_prof.clnt_id, gw_id, ZIPR_ID_LEN);

    return util_list_rm(svr_ctx->clnt_prof_mtx, &svr_ctx->clnt_prof_hd, (uint8_t *)&clnt_prof, tls_gw_id_cmp);
}


/**
tls_clnt_find - Find a client profile in the TLS server internal list based on gateway id.
@param[in]	    ctx		    The context returned from the call to tls_svr_start()
@param[in,out]	clnt_prof	Client profile buffer with the gateway id set as input key to be searched.
                            On success, this buffer will be returned with found entry.
@return                     Return non-zero on success, zero on failure.
*/
int tls_clnt_find(void *ctx, tls_clnt_prof_t *clnt_prof)
{
    tls_svr_ctx_t *svr_ctx = (tls_svr_ctx_t *)ctx;

    return util_list_find(svr_ctx->clnt_prof_mtx, svr_ctx->clnt_prof_hd, (uint8_t *)clnt_prof, tls_gw_id_cmp);
}


/**
tls_clnt_list_free - Free client profiles list
@param[in]	    lst_hd		List head of the client profiles list
@return
*/
void tls_clnt_list_free(tls_clnt_prof_lst *lst_hd)
{
    tls_clnt_prof_lst   *del_entry;    //Entry to be deleted

    while (lst_hd)
    {
        del_entry = lst_hd;
        lst_hd = lst_hd->next;
        free(del_entry);
    }
}


/**
tls_clnt_list_get - Get all the client profiles in the TLS server internal list
@param[in]	    ctx		    The context returned from the call to tls_svr_start()
@param[out]	    lst_hd		List head of the client profiles list
@return                     Number of client profiles in the list; negative value on failure.
@post Caller must call tls_clnt_list_free() to free the linked-list if return value is greater than zero.
*/
int tls_clnt_list_get(void *ctx, tls_clnt_prof_lst **lst_hd)
{
    tls_svr_ctx_t       *svr_ctx = (tls_svr_ctx_t *)ctx;
    tls_clnt_prof_lst   *ent;
    tls_clnt_prof_lst   *prev_ent;
    util_lst_t          *temp;
    tls_clnt_prof_t     *clnt_prof;
    int                 prof_cnt = 0;

    plt_mtx_lck(svr_ctx->clnt_prof_mtx);

    if (!svr_ctx->clnt_prof_hd)
    {   //list is empty
        plt_mtx_ulck(svr_ctx->clnt_prof_mtx);
        return 0;
    }

    temp = svr_ctx->clnt_prof_hd;

    while (temp)
    {
        ent = malloc(sizeof(tls_clnt_prof_lst));

        if (prof_cnt == 0)
        {
            *lst_hd = ent;
            prev_ent = ent;
        }

        if (!ent)
        {
            tls_clnt_list_free(*lst_hd);
            plt_mtx_ulck(svr_ctx->clnt_prof_mtx);
            return -1;
        }

        clnt_prof = (tls_clnt_prof_t *)temp->wr_buf;
        //Copy the profile
        ent->clnt_prof = *clnt_prof;
        ent->next = NULL;

        if (prof_cnt > 0)
        {
            prev_ent->next = ent;
        }

        prev_ent = ent;
        prof_cnt++;

        temp = temp->next;
    }

    plt_mtx_ulck(svr_ctx->clnt_prof_mtx);
    return prof_cnt;
}

#endif  //TCP_PORTAL

/**
@}
*/

