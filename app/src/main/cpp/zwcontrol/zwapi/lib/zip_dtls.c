/**
@file   zip_dtls.c - Z/IP host controller interface DTLS transport layer implementation.

        Platform specific implementation of Datagram Transport Layer Security (DTLS) function.

@author David Chow

@version    1.0 10-10-14  Initial release

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

#define _GNU_SOURCE         //For IPv6

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <pthread.h>

#define OPENSSL_THREAD_DEFINES
#include "openssl/opensslconf.h"
#include "openssl/ssl.h"
#include "openssl/bio.h"
#include "openssl/err.h"
#include "openssl/rand.h"
#include "../include/zip_transport.h"
#include "../include/zwave/ZW_classcmd.h"
#include "zip_api.h"

//#define  DEBUG_ZIP_DTLS     //Display DTLS debug messages

#define MAX_ZWAVE_NODE              232     ///< The maximum number of Z-wave nodes in a network
#define ZIPV2_HOME_CTL_PORT         41230   ///< Z/IP version 2 secure home controller UDP port
#define ZIPV2_KEEP_ALIVE_EXP        50      ///< Z/IP version 2 keep-alive timer expiry time in seconds for SSL type client
#define ZIPV2_SESS_EXPIRY_SERVER    60      ///< Z/IP version 2 secure session expiry time in seconds for SSL type server
#define ZIPV2_SESS_KEEP_ALIVE_CNT   5       ///< Z/IP version 2 secure session keep-alive count for SSL type client
#define ZIP_FRM_HDR_SIZE            7       ///< ZIP packet header size
#define ZIP_FRM_KEEP_ALIVE_SZ       3       ///< ZIP keep alive packet frame size
#define ZIPV2_KEEP_ALIVE_RESP_EXP   2       ///< Z/IP version 2 keep-alive response expiry time in seconds
#define DTLS_MAX_TIMEOUT_SEC        4       ///< DTLS maximum timeout in seconds. Note: timeout start from 1; multiply by 2 in each iteration.
#define DTLS_SHUTDOWN_TIMEOUT_SEC   1       ///< DTLS wait for close notify timeout in seconds.
#define COOKIE_KEY_LEN              16      ///< Cookie key length

#define SSL_SUCCESS                 1       ///< SSL success
#define SSL_FATAL_ERROR             -1      ///< SSL error

#ifdef OS_ANDROID
typedef uint16_t in_port_t;
#endif

///
/// dtls_select() return codes
typedef enum{
    DTLS_SELECT_FAIL,           ///< Failed
    DTLS_SELECT_TIMEOUT,        ///< Timeout
    DTLS_SELECT_RECV_READY,     ///< Received ready
    DTLS_SELECT_ERROR_READY     ///< Exceptions
} dtls_select_e;

///
/// DTLS SSL states
#define DTLS_SSL_STA_IDLE           0       ///< Idle state (SSL is uninitialized)
#define DTLS_SSL_STA_HANDSHAKE      1       ///< Handshaking state
#define DTLS_SSL_STA_DATA           2       ///< Data state
#define DTLS_SSL_STA_SESS_EXPIRED   3       ///< Session expired state

///
/// DTLS SSL type
#define DTLS_SSL_TYPE_CLIENT        0       ///< The local SSL connection is a client
#define DTLS_SSL_TYPE_SERVER        1       ///< The local SSL connection is a server

///
/// SSL and its associated socket data structure
typedef struct
{
    int       sock_fd;              ///< Socket file descriptor associated with ssl
    SSL       *ssl;                 ///< SSL context
    uint8_t   state;                ///< SSL state, DTLS_SSL_STA_XXX
    int8_t    keep_alive_cnt;       ///< Session keep-alive count for SSL type client
    uint16_t  tm_to_expire;         /**< Time to SSL session expire in seconds (SSL type server).
                                         Time to keep-alive expire in seconds (SSL type client) */
    int16_t   keep_alive_resp_exp;  ///< Time to keep-alive response expire in seconds
    int       use_host_port;        ///< Flag to indicate using host listening port
} ssl_sock_t;

///
/// "IP address - ssl" context table entry
typedef struct
{
    uint8_t     ipaddr_sz;            ///< IP address size in bytes
    uint8_t     ipaddr[IPV6_ADDR_LEN];///< IP address
    ssl_sock_t  ssl_sock[2];          /**< SSL and its associated socket data structure.
                                           The first entry is for client; the second is for server */
} ipaddr_ssl_t;


///
/// DTLS context
typedef struct _dtls_ctx
{
    volatile uint16_t   tmr_tick;           ///< Periodic timer tick, incremented every 1 second
    uint16_t            last_chk_tm;        ///< Last checking time of "IP address - ssl" context table
    SSL_CTX             *client_ctx;        ///< ssl client context
    SSL_CTX             *svr_ctx;           ///< ssl server context
    tpt_layer_ctx_t     *tpt_ctx;           ///< transport context
    struct plt_mtx_t    *rdwr_mtx;          ///< mutex for read/write access
    void                *tick_tmr_ctx;      ///< Tick timer context
    int                 sock_fd;            ///< Socket file descriptor for sending/receiving UDP packets
    ipaddr_ssl_t        ip_ssl_tbl[MAX_ZWAVE_NODE]; ///< "IP address - ssl" context table
    unsigned char       cookie_key[COOKIE_KEY_LEN]; ///< Cookie key that is used to generate cookie for "hello verify request" message

} dtls_ctx_t;

///
/// SSL function
typedef int (*ssl_func_t)(SSL *ssl);

///
/// DTLS context
typedef struct
{
    ssl_func_t  func;   ///< SSL function
    char        *name;  ///< SSL function name
} ssl_func_name_t;

///
/// SSL function constant
#define DTLS_SSL_CONNECT    0       ///< SSL_connect()
#define DTLS_SSL_ACCEPT     1       ///< SSL_accept()

static const ssl_func_name_t ssl_func_name[] = { {SSL_connect, "SSL_connect"},
                                                 {SSL_accept, "SSL_accept"}
                                               };


static const char  cipherList[] = "PSK-AES256-CBC-SHA:PSK-AES128-CBC-SHA";
#ifdef  DEBUG_ZIP_DTLS
static const char  *dtls_ssl_type_str[] = {"client", "server"};
#endif

extern void ip_addr_show(plt_ctx_t *plt_ctx, struct sockaddr *sock_addr, socklen_t sock_len);

/**
dtls_cookie_gen - Generate cookie based on peer's ip address and port
@param[in]  ssl         SSL context
@param[out] cookie      generated cookie
@param[out] cookie_len  cookie length
@return          1 on success; else zero on failure
*/
static int dtls_cookie_gen(SSL *ssl, unsigned char *cookie, unsigned int *cookie_len)
{
    tpt_layer_ctx_t *tpt_ctx = (tpt_layer_ctx_t *)SSL_get_app_data(ssl);
    dtls_ctx_t      *dtls_ctx = (dtls_ctx_t *)tpt_ctx->dtls_ctx;
    unsigned char   buffer[64];
    unsigned char   generated_cookie[EVP_MAX_MD_SIZE];
    unsigned int    length = 0;
    unsigned int    generated_cookie_len;
    socklen_t       peer_addr_len;
    union
    {
        struct sockaddr_storage ss;
        struct sockaddr_in6 s6;
        struct sockaddr_in s4;
    } peer;

    // Read peer information
    peer_addr_len = sizeof(peer);
    if (getpeername(SSL_get_fd(ssl), (struct sockaddr * __restrict__)&peer, &peer_addr_len) < 0)
    {
        return 0;
    }

    length = sizeof(in_port_t);
    switch (peer.ss.ss_family)
    {
        case AF_INET:
            length += sizeof(struct in_addr);
            memcpy(buffer,
                   &peer.s4.sin_port,
                   sizeof(in_port_t));
            memcpy(buffer + sizeof(peer.s4.sin_port),
                   &peer.s4.sin_addr,
                   sizeof(struct in_addr));
            break;
        case AF_INET6:
            length += sizeof(struct in6_addr);
            memcpy(buffer,
                   &peer.s6.sin6_port,
                   sizeof(in_port_t));
            memcpy(buffer + sizeof(in_port_t),
                   &peer.s6.sin6_addr,
                   sizeof(struct in6_addr));
            break;
        default:
            return 0;
    }

    // Calculate HMAC of buffer using the cookie key
    HMAC(EVP_sha1(), (const void *)dtls_ctx->cookie_key, COOKIE_KEY_LEN,
         (const unsigned char *)buffer, length, generated_cookie, &generated_cookie_len);

    memcpy(cookie, generated_cookie, generated_cookie_len);
    *cookie_len = generated_cookie_len;

    return 1;
}


/**
dtls_cookie_verify - Verify cookie based on peer's ip address and port
@param[in] ssl         SSL context
@param[in] cookie      received cookie
@param[in] cookie_len  cookie length
@return          1 on success; else zero on failure
*/
static int dtls_cookie_verify(SSL *ssl, unsigned char *cookie, unsigned int cookie_len)
{
    unsigned char   generated_cookie[EVP_MAX_MD_SIZE];
    unsigned int    length;

    if (dtls_cookie_gen(ssl,generated_cookie, &length))
    {
        return ((cookie_len == length) && (memcmp(generated_cookie, cookie, length) == 0))? 1 : 0;
    }

    return 0;
}


/**
dtls_ip_ssl_get - Get SSL context associated with the specified IP address
@param[in] dtls_ctx     DTLS context
@param[in] ipaddr       IP address
@param[in] ipaddr_sz    IP address size in bytes
@param[in] ssl_type     SSL type, DTLS_SSL_TYPE_XXX
@param[out] ssl         SSL context
@param[out] pos_found   The position of the IP address found in the table
@return         0 on found; else not found
@note           Even if the entry was found, the ssl may be invalid (i.e. NULL)
@pre            Caller must lock dtls_ctx->rdwr_mtx
*/
static int  dtls_ip_ssl_get(dtls_ctx_t *dtls_ctx, uint8_t *ipaddr, uint8_t ipaddr_sz, uint8_t ssl_type,
                            SSL **ssl, int *pos_found)
{
    int             i;
    ipaddr_ssl_t    *tbl = dtls_ctx->ip_ssl_tbl;

    for (i=0; i<MAX_ZWAVE_NODE; i++)
    {
        if (tbl[i].ipaddr_sz == ipaddr_sz
            && (memcmp(tbl[i].ipaddr, ipaddr, ipaddr_sz) == 0))
        {
            *pos_found = i;
            *ssl = tbl[i].ssl_sock[ssl_type].ssl;
            return 0;
        }
    }
    *pos_found = 0;
    return -1;
}


/**
dtls_ip_ssl_add - Add an entry into "IP address - ssl" table
@param[in] dtls_ctx     DTLS context
@param[in] ssl          SSL context
@param[in] fd           Socket file descriptor associated with ssl
@param[in] ipaddr       IP address
@param[in] ipaddr_sz    IP address size in bytes
@param[in] ssl_type     SSL type, DTLS_SSL_TYPE_XXX
@param[in] use_host_port   Flag to indicate the socket descriptor is using host listening port
@return          0 on success; negative value on error.
*/
static int  dtls_ip_ssl_add(dtls_ctx_t *dtls_ctx, SSL *ssl, int fd, uint8_t *ipaddr, uint8_t ipaddr_sz,
                            uint8_t ssl_type, int use_host_port)
{
    int             i;
    int             res;
    int             pos;
    int             ret = -1;
    SSL             *tmp_ssl;
    ipaddr_ssl_t    *tbl;

    plt_mtx_lck(dtls_ctx->rdwr_mtx);

    tbl = dtls_ctx->ip_ssl_tbl;

    res = dtls_ip_ssl_get(dtls_ctx, ipaddr, ipaddr_sz, ssl_type, &tmp_ssl, &pos);

    if (res != 0)
    {   //IP address not found
        //Find an empty slot for the new entry
        for (i=0; i<MAX_ZWAVE_NODE; i++)
        {
            if (tbl[i].ipaddr_sz == 0)
            {
                ret = 0;
                break;
            }
        }
    }
    else
    {   //Found
        //Free ssl & socket fd before replacing them
        if (tmp_ssl)
        {
            SSL_free(tmp_ssl);
        }
        if (tbl[pos].ssl_sock[ssl_type].sock_fd > 0)
        {
            close(tbl[pos].ssl_sock[ssl_type].sock_fd);
        }
        i = pos;
        ret = 0;
    }

    if (ret == 0)
    {
        ssl_sock_t  *ssl_sock;

        ssl_sock = &tbl[i].ssl_sock[ssl_type];

        if (ssl_type == DTLS_SSL_TYPE_CLIENT)
        {
            ssl_sock->tm_to_expire = ZIPV2_KEEP_ALIVE_EXP;
            ssl_sock->keep_alive_cnt = ZIPV2_SESS_KEEP_ALIVE_CNT;
            ssl_sock->use_host_port = use_host_port;
        }
        else    //DTLS_SSL_TYPE_SERVER
        {
            ssl_sock->tm_to_expire = ZIPV2_SESS_EXPIRY_SERVER;
            ssl_sock->keep_alive_cnt = 0;
            ssl_sock->use_host_port = 1;
        }
        ssl_sock->state = DTLS_SSL_STA_DATA;
        ssl_sock->ssl = ssl;
        ssl_sock->sock_fd = fd;
        ssl_sock->keep_alive_resp_exp = -1;
        tbl[i].ipaddr_sz = ipaddr_sz;
        memcpy(tbl[i].ipaddr, ipaddr, ipaddr_sz);
    }

    plt_mtx_ulck(dtls_ctx->rdwr_mtx);

    return ret;

}


/**
dtls_select - Wait for a socket is ready for reading or timeout
@param[in] sock_fd      Socket file descriptor
@param[in] tmout_sec    Timeout in seconds
@return    DTLS_SELECT_XXX
*/
static dtls_select_e dtls_select(int sock_fd, int tmout_sec)
{
    fd_set          recvfds, errfds;
    int             nfds = sock_fd + 1;
    struct timeval  timeout = { (tmout_sec > 0)? tmout_sec : 0, 0};//second, micro-second
    int             result;

    FD_ZERO(&recvfds);
    FD_SET(sock_fd, &recvfds);
    FD_ZERO(&errfds);
    FD_SET(sock_fd, &errfds);

    result = select(nfds, &recvfds, NULL, &errfds, &timeout);

    if (result == 0)
    {
        return DTLS_SELECT_TIMEOUT;
    }
    else if (result > 0)
    {
        if (FD_ISSET(sock_fd, &recvfds))
            return DTLS_SELECT_RECV_READY;
        else if(FD_ISSET(sock_fd, &errfds))
            return DTLS_SELECT_ERROR_READY;
    }

    return DTLS_SELECT_FAIL;
}


/**
dtls_func - DTLS handshake function
@param[in] tpt_ctx      Transport layer context
@param[in] ssl          SSL context
@param[in] func         SSL function, see DTLS_SSL_XXX
@param[in] max_timeout  Maximum timeout in seconds. Note: timeout start from 1; multiply by 2 in each iteration.
@return    SSL_SUCCESS on success; other value on error.
*/
static int dtls_func(tpt_layer_ctx_t *tpt_ctx, SSL *ssl, int func, int max_timeout)
{
    int         curr_tmout = 1;
    int         ret = ssl_func_name[func].func(ssl);
    int         error = SSL_get_error(ssl, ret);
    int         sockfd = SSL_get_fd(ssl);
    dtls_select_e select_ret;

    while (ret != SSL_SUCCESS &&
           (error == SSL_ERROR_WANT_READ || error == SSL_ERROR_WANT_WRITE))
    {

#ifdef  DEBUG_ZIP_DTLS
        debug_msg_show(tpt_ctx->plt_ctx, "... %s would %s block", ssl_func_name[func].name, (error == SSL_ERROR_WANT_READ)? "read" : "write");
        debug_msg_show(tpt_ctx->plt_ctx, "... curr_tmout = %d", curr_tmout);
#endif
        //Check if total timeout exceeded or exit is signaled
        if ((curr_tmout > max_timeout) || (tpt_ctx->wr_thrd_run == 0))
        {
            break;
        }

        //Wait for read/write ready
        select_ret = dtls_select(sockfd, curr_tmout);

#ifdef  DEBUG_ZIP_DTLS
        debug_msg_show(tpt_ctx->plt_ctx, "... dtls_select returns %d", select_ret);
#endif

        if ((select_ret == DTLS_SELECT_RECV_READY) || (select_ret == DTLS_SELECT_ERROR_READY))
        {
            ret = ssl_func_name[func].func(ssl);
            error = SSL_get_error(ssl, ret);
#ifdef  DEBUG_ZIP_DTLS
            debug_msg_show(tpt_ctx->plt_ctx, "... %s returns %d, error = %d", ssl_func_name[func].name, ret, error);
#endif
        }
        else if (select_ret == DTLS_SELECT_TIMEOUT)
        {
            //Timeout, retry with larger timeout value
            curr_tmout = curr_tmout << 1; //Increase next timeout by a factor of 2
            error = SSL_ERROR_WANT_READ;
        }
        else
        {
            error = SSL_FATAL_ERROR;
        }
    }
    return ret;
}


/**
dtls_ssl_create - Create a client SSL connection to the specified host
@param[in] tpt_ctx		Context
@param[in] fd           UDP socket to be associated to SSL connection
@param[in] dest_host    destination host IP address
@param[in] dest_port    destination host port number
@param[in] use_ipv4     Flag to indicate whether to use IPv4
@param[out] ssl_ctx     SSL context
@return          Zero on success; else negative number on error.
*/
static int dtls_ssl_create(tpt_layer_ctx_t *tpt_ctx, int fd, uint8_t *dest_host, uint16_t dest_port,
                           int use_ipv4, SSL **ssl_ctx)
{
    dtls_ctx_t          *dtls_ctx = (dtls_ctx_t *)tpt_ctx->dtls_ctx;
    SSL                 *ssl;
    struct sockaddr_in6 sa6;
    struct sockaddr_in  sa4;

    ssl = SSL_new(dtls_ctx->client_ctx);
    if (!ssl)
    {
        debug_msg_show(tpt_ctx->plt_ctx, "Failed to create SSL object");
        return -1;
    }

    SSL_set_app_data(ssl, tpt_ctx);

    *ssl_ctx = ssl;

    if (use_ipv4)
    {
        sa4.sin_family = AF_INET;
        sa4.sin_port = htons(dest_port);
        memcpy(&sa4.sin_addr.s_addr, dest_host, IPV4_ADDR_LEN);
        connect(fd, (struct sockaddr *)&sa4, sizeof(struct sockaddr_in));
    }
    else    //IPv6
    {
        sa6.sin6_family = AF_INET6;
        sa6.sin6_port = htons(dest_port);
        sa6.sin6_flowinfo = 0;
    	sa6.sin6_scope_id = 0;
        memcpy(sa6.sin6_addr.s6_addr, dest_host, IPV6_ADDR_LEN);
        connect(fd, (struct sockaddr *)&sa6, sizeof(struct sockaddr_in6));
    }

    SSL_set_fd(ssl, fd);

    return 0;
}


/**
dtls_ssl_close - Close SSL and its associated socket fd
@param[in] ipaddr_ssl_ent   Pointer to IP address - ssl context table entry
@param[in] ssl_type         SSL type, DTLS_SSL_TYPE_XXX
@return
@pre    Caller must lock dtls_ctx->rdwr_mtx
*/
static void  dtls_ssl_close(ipaddr_ssl_t *ipaddr_ssl_ent, uint8_t ssl_type)
{
    int         ret;
    ssl_sock_t  *ssl_sock = &ipaddr_ssl_ent->ssl_sock[ssl_type];

    //Check whether the SSL is valid
    if (!ssl_sock->ssl)
        return;

    ret = SSL_shutdown(ssl_sock->ssl);
    if (ret == 0)
    {   //Call SSL_shutdown() again to wait for peer's "close notify" alert
        ret = SSL_shutdown(ssl_sock->ssl);
    }

    if (ret != 1)
    {
        int retry_cnt = 2;
        int error;

        while (retry_cnt-- > 0)
        {
            //print_time_msg("Waiting for peer to send close_notify");
            error = SSL_get_error(ssl_sock->ssl, ret);
            if (((error == SSL_ERROR_WANT_READ) || (error == SSL_ERROR_WANT_WRITE))
                && (ssl_sock->sock_fd > 0))
            {
                dtls_select(ssl_sock->sock_fd, DTLS_SHUTDOWN_TIMEOUT_SEC);
            }

            if (!(SSL_get_shutdown(ssl_sock->ssl) & SSL_RECEIVED_SHUTDOWN))
            {
                //Get shutdown status as the SSL_get_shutdown() is not so accurate. This may be due to when
                //dtls_select() returns the DTLS read thread is not fast enough to process peer's "close notify"
                //and set the flag SSL_RECEIVED_SHUTDOWN

                ret  = SSL_shutdown(ssl_sock->ssl);
                if (ret == 1)
                {
                    //print_time_msg("Peer sent close_notify");
                    break;
                }
                else
                {
                    //Resend "close notify" alert
                    //print_time_msg("Resend close_notify");

                    //#define SSL_UNSET_SHUTDOWN
                    SSL_unset_shutdown(ssl_sock->ssl, SSL_SENT_SHUTDOWN);
                    //#endif

                    ret  = SSL_shutdown(ssl_sock->ssl);

                    if (ret == 0)
                    {   //Call SSL_shutdown() again to wait for peer's "close notify" alert
                        ret = SSL_shutdown(ssl_sock->ssl);
                    }

                    if (ret == 1)
                    {
                        //print_time_msg("Peer sent close_notify");
                        break;
                    }
                    else
                    {
                        //print_time_msg("Peer didn't sent close_notify");
                    }
                }
            }
        }
    }

    //Change SSL state to idle
    ssl_sock->state = DTLS_SSL_STA_IDLE;

    //Free resources
    SSL_free(ssl_sock->ssl);
    ssl_sock->ssl = NULL;
    if (ssl_sock->sock_fd > 0)
    {
        close(ssl_sock->sock_fd);
    }
    ssl_sock->sock_fd = -1;

    //Invalidate the entry
    if (!ipaddr_ssl_ent->ssl_sock[DTLS_SSL_TYPE_CLIENT].ssl
        && !ipaddr_ssl_ent->ssl_sock[DTLS_SSL_TYPE_SERVER].ssl)
    {
        ipaddr_ssl_ent->ipaddr_sz = 0;
    }
}


/**
dtls_sock_bind - Bind a socket to the given port
@param[in] tpt_ctx		Context
@param[in] fd           The network socket
@param[in] port         Listening and sending port number
@param[in] bound_addr   IP address to bind to the UDP socket (optional)
@param[in] use_ipv4     Flag to indicate whether to use IPv4
@return          Zero on success; else negative number on error.
*/
static int dtls_sock_bind(tpt_layer_ctx_t *tpt_ctx, int fd, uint16_t port, uint8_t *bound_addr, int use_ipv4)
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

    return 0;
}


/**
dtls_sock_create - Create a UDP socket
@param[in]	tpt_ctx		    Transport layer context
@param[in]  port            Listening and sending port number
@param[in]  use_ipv4        Flag to indicate whether to use IPv4
@param[in]  reuse_addr      Flag to indicate enable reuse of IP address/port
@param[in]  rcx_packet_info Flag to indicate enable socket to receive packet info
@param[in]  bound_addr      IP address to bind to the UDP socket (optional)
@param[out]	sock_fd		    Socket file descriptor
@return     Return non-zero indicates success, zero indicates failure.
*/
static int dtls_sock_create(tpt_layer_ctx_t *tpt_ctx, uint16_t port, int use_ipv4, int reuse_addr,
                            int rcx_packet_info, uint8_t *bound_addr, int *sock_fd)
{
    int res;
#ifdef OS_LINUX
    int old_flg;
    int fd;
#endif
    int	fd_flg;
    int yes = 1;
    char * err_str;

    //Open socket and bind it to the source address
    fd = socket((use_ipv4)? AF_INET : AF_INET6, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        err_str = "Create udp socket failed";
        goto DTLS_SOCK_CREATE_ERR1;
    }

    if (reuse_addr)
    {
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
        {
            err_str = "setsockopt SO_REUSEADDR failed";
            goto DTLS_SOCK_CREATE_ERR2;
        }
    }

    if (rcx_packet_info)
    {
        if (setsockopt(fd, IPPROTO_IP, IP_PKTINFO, &yes, sizeof(yes)) < 0)
        {
            err_str = "setsockopt IP_PKTINFO failed";
            goto DTLS_SOCK_CREATE_ERR2;
        }

        if (!use_ipv4)
        {   //IPv6
            if (setsockopt(fd, IPPROTO_IPV6, IPV6_RECVPKTINFO, &yes, sizeof(yes)) < 0)
            {
                err_str = "setsockopt IPV6_RECVPKTINFO failed";
                goto DTLS_SOCK_CREATE_ERR2;
            }
        }
    }

#ifdef OS_LINUX
    if (!use_ipv4)
    {
        //Windows default to IPV6_V6ONLY = TRUE, so don't need to setsockopt
        #ifdef IPV6_V6ONLY

        // Disable IPv4 mapped addresses for security reason.
        if (setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, &yes, sizeof(yes)) < 0)
        {
            err_str = "setsockopt IPV6_V6ONLY failed";
            goto DTLS_SOCK_CREATE_ERR2;
        }

        #else
        debug_msg_show(tpt_ctx->plt_ctx, "Warning: couldn't disable IPv4 mapped addresses");
        #endif
    }


    //Set the socket file descriptor with FD_CLOEXEC which specifies
    //that the file descriptor should be closed when an exec function is invoked

    old_flg = fcntl(fd, F_GETFD, 0);

    old_flg |= FD_CLOEXEC;

	res = fcntl(fd, F_SETFD, old_flg);

    if (res < 0)
    {
        err_str = "set FD_CLOEXEC failed";
        goto DTLS_SOCK_CREATE_ERR2;
    }
#endif

    //Set socket as non-blocking
    fd_flg = fcntl(fd, F_GETFL, 0);

    fd_flg |= O_NONBLOCK;

    if (fcntl(fd, F_SETFL, fd_flg) < 0)
    {
        err_str = "set O_NONBLOCK failed";
        goto DTLS_SOCK_CREATE_ERR2;
    }

    //Bind to host
    res = dtls_sock_bind(tpt_ctx, fd, port, bound_addr, use_ipv4);

    if (res < 0)
    {
        err_str = "dtls_sock_bind failed";
        goto DTLS_SOCK_CREATE_ERR2;
    }

    *sock_fd = fd;
    return 1;//O.k.

DTLS_SOCK_CREATE_ERR2:
    close(fd);
DTLS_SOCK_CREATE_ERR1:
    debug_msg_show(tpt_ctx->plt_ctx, "%s", err_str);
    return 0;

}


/**
dtls_sess_expiry_chk - Check for DTLS sessions expiry
@param[in]	tpt_ctx		Context
@return
*/
static void dtls_sess_expiry_chk(tpt_layer_ctx_t *tpt_ctx)
{
    static const uint8_t    keep_alive_msg[] = {0, COMMAND_CLASS_ZIP, COMMAND_ZIP_KEEP_ALIVE, 0x80}; //djnakata
    int             i;
    int             k;
    int             bytes_sent;
    dtls_ctx_t      *dtls_ctx = (dtls_ctx_t *)tpt_ctx->dtls_ctx;
    ipaddr_ssl_t    *tbl = dtls_ctx->ip_ssl_tbl;
    ssl_sock_t      *ssl_sock;
    uint16_t        tm_diff;

    tm_diff = util_time_diff(dtls_ctx->tmr_tick, dtls_ctx->last_chk_tm);
    if (tm_diff)
    {
        plt_mtx_lck(dtls_ctx->rdwr_mtx);

        for (i=0; i<MAX_ZWAVE_NODE; i++)
        {
            if (tbl[i].ipaddr_sz)
            {
                //Check client and server ssl
                for (k=0; k<2; k++)
                {
                    ssl_sock = &tbl[i].ssl_sock[k];
                    if ((ssl_sock->state == DTLS_SSL_STA_DATA || ssl_sock->state == DTLS_SSL_STA_SESS_EXPIRED)
                        && ssl_sock->tm_to_expire)
                    {
                        //
                        //Check for keep-alive response expiry time
                        //
                        if (ssl_sock->keep_alive_resp_exp > tm_diff)
                        {
                            ssl_sock->keep_alive_resp_exp -= tm_diff;
                        }
                        else if (ssl_sock->keep_alive_resp_exp >= 0)
                        {   //Keep-alive response was not received within the expiry time
                            dtls_ssl_close(&tbl[i], k);
                        }

                        //
                        //Check for keep-alive or session expiry time
                        //
                        if (ssl_sock->tm_to_expire > tm_diff)
                        {   //Not expired yet
                            ssl_sock->tm_to_expire -= tm_diff;
                        }
                        else
                        {   //Keep-alive timer or session expired
                            ssl_sock->tm_to_expire = 0;
        #ifdef  DEBUG_ZIP_DTLS
                            debug_msg_ts_show(tpt_ctx->plt_ctx, "DTLS %s session expired for IP:", dtls_ssl_type_str[k]);
                            debug_bin_show(tpt_ctx->plt_ctx, tbl[i].ipaddr, tbl[i].ipaddr_sz);
        #endif
                            if (k == DTLS_SSL_TYPE_SERVER)
                            {
                                dtls_ssl_close(&tbl[i], k);
                            }
                            else
                            {   //SSL type client
                                if (ssl_sock->keep_alive_cnt == 0)
                                {   //Don't send keep-alive to server
                                    ssl_sock->tm_to_expire = ZIPV2_KEEP_ALIVE_EXP;
                                    ssl_sock->keep_alive_cnt = -1;
                                }
                                else if (ssl_sock->keep_alive_cnt > 0)
                                {   //Reload keep-alive timer and send keep-alive to server
                                    ssl_sock->tm_to_expire = ZIPV2_KEEP_ALIVE_EXP;

                                    ssl_sock->keep_alive_cnt--;

                                    //Send keep-alive
                                    bytes_sent = SSL_write(ssl_sock->ssl, keep_alive_msg, sizeof(keep_alive_msg));
        #ifdef  DEBUG_ZIP_DTLS
                                    debug_msg_show(tpt_ctx->plt_ctx, "Send keep alive count: %d", (int)ssl_sock->keep_alive_cnt);
        #endif
                                    if (bytes_sent == sizeof(keep_alive_msg))
                                    {
                                        ssl_sock->keep_alive_resp_exp = ZIPV2_KEEP_ALIVE_RESP_EXP;
                                    }
                                    else
                                    {
                                        debug_msg_show(tpt_ctx->plt_ctx, "Send keep alive failed. Err: %d", bytes_sent);
                                    }
                                }
                                else //ssl_sock->keep_alive_cnt < 0
                                {   //DTLS session expired, doing clean up
                                    if (ssl_sock->state == DTLS_SSL_STA_DATA)
                                    {   //Wait for server to close connection
                                        ssl_sock->tm_to_expire = ZIPV2_KEEP_ALIVE_EXP;
                                        ssl_sock->state = DTLS_SSL_STA_SESS_EXPIRED;
                                    }
                                    else
                                    {   //Server didn't close connection, initiate closing of connection
        #ifdef  DEBUG_ZIP_DTLS
                                        debug_msg_show(tpt_ctx->plt_ctx, "Server did not close connection. Initiate closing ...");
        #endif
                                        dtls_ssl_close(&tbl[i], k);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        plt_mtx_ulck(dtls_ctx->rdwr_mtx);

        //Update last check time
        dtls_ctx->last_chk_tm += tm_diff;

    }
}


/**
dtls_rd_ready_hdlr - Handler of DTLS read ready
@param[in]	tpt_ctx		    Context
@param[in]  read_fds        Set containing the read ready socket fd
@param[in]  no_rd_ready_chk Flag to determine whether check for select() returned read ready.
                            1=do not check read ready; 0=check read ready
@pre            Caller must lock dtls_ctx->rdwr_mtx
*/
static void dtls_rd_ready_hdlr(tpt_layer_ctx_t *tpt_ctx, fd_set *read_fds, int no_rd_ready_chk)
{
    int                 i;
    int                 k;
    int                 rd_err;
    int                 rcx_size;
    dtls_ctx_t          *dtls_ctx = (dtls_ctx_t *)tpt_ctx->dtls_ctx;
    ipaddr_ssl_t        *tbl = dtls_ctx->ip_ssl_tbl;
    ssl_sock_t          *ssl_sock;
    struct sockaddr     *sock_addr;
    struct sockaddr_in6 sa6 = {0};
    struct sockaddr_in  sa4 = {0};
    uint8_t             buf[MAX_RCX_SIZE] = {0};

    for (i=0; i<MAX_ZWAVE_NODE; i++)
    {
        if (tbl[i].ipaddr_sz)
        {
            //Check client and server ssl
            for (k=0; k<2; k++)
            {
                ssl_sock = &tbl[i].ssl_sock[k];
                if (ssl_sock->state == DTLS_SSL_STA_DATA || ssl_sock->state == DTLS_SSL_STA_SESS_EXPIRED)
                {
                    //Find read fd
                    if (no_rd_ready_chk || (FD_ISSET(ssl_sock->sock_fd, read_fds)))
                    {
        #ifdef  DEBUG_ZIP_DTLS
                        debug_msg_ts_show(tpt_ctx->plt_ctx, "read ready fd:%d IP:", ssl_sock->sock_fd);
                        debug_bin_show(tpt_ctx->plt_ctx, tbl[i].ipaddr, tbl[i].ipaddr_sz);
        #endif
                        rcx_size = SSL_read(ssl_sock->ssl, buf, sizeof(buf)-1);
                        if (rcx_size > 0)
                        {   //Have valid data
        #ifdef SHOW_PACKAT_INFO
                            debug_msg_show(tpt_ctx->plt_ctx, "udp rcx:");
                            debug_bin_show(tpt_ctx->plt_ctx, buf, rcx_size);
        #endif
                            // ALOGI("************* Read udp rcx:\n");
                            // debug_bin_show(tpt_ctx->plt_ctx, buf, rcx_size);
                            // ALOGI("************* Read udp rcx end\n");
                            //djnakata
                            ssl_sock->keep_alive_cnt = ZIPV2_SESS_KEEP_ALIVE_CNT;

                            uint8_t buf2[MAX_RCX_SIZE] = {0};
                            memcpy(buf2, buf, rcx_size);
                            rcx_size = rcx_size - 1;
                            uint8_t nodeid = buf2[0];
                            memcpy(buf, buf2 + 1, rcx_size);

                            //Reset server time to session expiry
                            if (k == DTLS_SSL_TYPE_SERVER)
                            {
                                ssl_sock->tm_to_expire = ZIPV2_SESS_EXPIRY_SERVER;
                            }

                            //Check for Z/IP keep alive response message
                            if ((rcx_size >= 3) && (buf[0] == COMMAND_CLASS_ZIP) && (buf[1] == COMMAND_ZIP_KEEP_ALIVE)
                                && (buf[2] & 0x40))
                            {
                                ssl_sock->keep_alive_resp_exp = -1;
                            }
                            else
                            {   //Pass the message to upper layer
                                if (tpt_ctx->use_ipv4)
                                {
                                    sock_addr = (struct sockaddr *)&sa4;
                                    //djnakata
                                    char addr[50];
                                    if(nodeid == tpt_ctx->frm_layer_ctx->ssn_layer_ctx->appl_layer_ctx->ctl_node_id)
                                    {
                                        memcpy(&sa4.sin_addr.s_addr, tbl[i].ipaddr, tbl[i].ipaddr_sz);
										//sa4.sin_port = k;//Overload port to indicate whether the socket is using host listening port
                                    }
                                    else
                                    {
                                        sprintf(addr, "0.0.0.%d", nodeid);
                                        sa4.sin_addr.s_addr = inet_addr(addr);
										//sa4.sin_port = k;//Overload port to indicate whether the socket is using host listening port
                                    }
                                    //memcpy(&sa4.sin_addr.s_addr, tbl[i].ipaddr, tbl[i].ipaddr_sz);
									//sa4.sin_port = k;//Overload port to indicate whether the socket is using host listening port
                                }
                                else
                                {
                                    sock_addr = (struct sockaddr *)&sa6;
                                    memcpy(sa6.sin6_addr.s6_addr, tbl[i].ipaddr, tbl[i].ipaddr_sz);
                                    sa6.sin6_port = k;//Overload port to indicate whether the socket is using host listening port
                                }

                                //Callback frame layer
                                plt_mtx_ulck(dtls_ctx->rdwr_mtx);
                                tpt_ctx->tpt_rd_cb(tpt_ctx, buf, rcx_size, sock_addr, NULL);
                                plt_mtx_lck(dtls_ctx->rdwr_mtx);
                            }
                        }
                        else if (rcx_size < 0)
                        {
                            rd_err = SSL_get_error(ssl_sock->ssl, rcx_size);
                            if (rd_err != SSL_ERROR_WANT_READ)
                            {
                                debug_msg_ts_show(tpt_ctx->plt_ctx, "SSL_read failed: %d\n", rd_err);

                                if ((rd_err == SSL_ERROR_ZERO_RETURN)
                                    || (rd_err == SSL_ERROR_SYSCALL))
                                {   //Received "Close notify" alert, free ssl and close socket
                                    dtls_ssl_close(&tbl[i], k);
                                }
                                else
                                {
                                    //Read the buffer to prevent looping
                                    recvfrom(ssl_sock->sock_fd, buf, MAX_RCX_SIZE, 0, NULL, NULL);
                                }
                            }
                        }
                        else // rcx_size == 0
                        {   //Connection closed, free ssl and close socket
        #ifdef  DEBUG_ZIP_DTLS
                            debug_msg_ts_show(tpt_ctx->plt_ctx, "Peer closed connection");
        #endif
                            dtls_ssl_close(&tbl[i], k);
                        }
                    }
                }
            }
        }
    }
}


/**
dtls_new_conn_hdlr - Handler of new DTLS connection request
@param[in]	tpt_ctx		Context
@pre        Caller must lock dtls_ctx->rdwr_mtx
*/
static void dtls_new_conn_hdlr(tpt_layer_ctx_t *tpt_ctx)
{
    int                 res;
    int                 rcx_size;
    int                 new_fd;
    socklen_t           sock_len;
    uint8_t             *dst_addr;
    dtls_ctx_t          *dtls_ctx = (dtls_ctx_t *)tpt_ctx->dtls_ctx;
    SSL                 *new_ssl;
    uint8_t             buf[MAX_RCX_SIZE];
    uint8_t             dst_addr_len;
    struct              msghdr mh;
    struct              iovec iov[1];
    char                cmbuf[256];
#ifdef SHOW_PACKAT_INFO
    char                addr_str[128];
#endif
    uint8_t             srv_addr[IPV6_ADDR_LEN];
    sockaddr_stor_t     peer_addr;

    iov[0].iov_base = buf;
    iov[0].iov_len = sizeof(buf);

    sock_len = (tpt_ctx->use_ipv4)? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);

    //Get source address of the connection request
l_GET_UDP_MSG:

    mh.msg_name = &peer_addr;
    mh.msg_namelen = sock_len;
    mh.msg_iov = iov;
    mh.msg_iovlen = 1;
    mh.msg_control = cmbuf;
    mh.msg_controllen = sizeof(cmbuf);
    mh.msg_flags = 0;

    rcx_size = recvmsg(dtls_ctx->sock_fd, &mh, 0);

    if (rcx_size > 0)
    {
        //Check if this message is "client hello"
        if (!(buf[0]==0x16 && buf[1]==0xfe && buf[2]==0xff && buf[13]==0x01))
        {   //Non "client hello" message
            debug_msg_ts_show(tpt_ctx->plt_ctx, "Warning: received non client hello msg:");
            debug_bin_show(tpt_ctx->plt_ctx, buf, rcx_size);
            //Try to receive another UDP packet in case there are two packets in the receive buffer
            goto l_GET_UDP_MSG;
        }

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
            return;
        }

#ifdef SHOW_PACKAT_INFO
        if (util_ip_ntoa(srv_addr, addr_str, 128, tpt_ctx->use_ipv4) == 0)
        {
            debug_msg_show(tpt_ctx->plt_ctx, "Packet info: Header destination address = %s", addr_str);
        }
#endif
        //
        //Create connected socket for subsequent communication
        //
        if (!dtls_sock_create(tpt_ctx, tpt_ctx->host_port, tpt_ctx->use_ipv4, 1, 0, srv_addr, &new_fd))
        {
            return;
        }

        if (connect(new_fd, (const struct sockaddr *)&peer_addr, sock_len) != 0)
        {
            debug_msg_ts_show(tpt_ctx->plt_ctx, "New DTLS connect failed");
            close(new_fd);
            return;
        }

        //
        // Create SSL context with the newly created socket
        //
        new_ssl = SSL_new(dtls_ctx->svr_ctx);
        if (!new_ssl)
        {
            debug_msg_show(tpt_ctx->plt_ctx, "Failed to create server SSL object");
            close(new_fd);
            return;
        }
        SSL_set_app_data(new_ssl, tpt_ctx);

//#define   SSL_SET_FD_RCV_MSG
        //Set fd to SSL with the received message so the first read from fd will return this message
        //NOTE: The SSL_set_fd_rcv_msg() is a special function patched to openssl library to support
        //      multihomed IP addresses.
        if(!SSL_set_fd_rcv_msg(new_ssl, new_fd, buf, (unsigned)rcx_size))
        {
            debug_msg_show(tpt_ctx->plt_ctx, "SSL_set_fd_rcv_msg failed");
            SSL_free(new_ssl);
            close(new_fd);
            return;
        }
//#endif
        SSL_set_options(new_ssl, SSL_OP_COOKIE_EXCHANGE);//enable "hello verify request"

        //Do DTLS handshakes to accept the new connection
        res = dtls_func(tpt_ctx, new_ssl, DTLS_SSL_ACCEPT, 1);
        if (res != SSL_SUCCESS)
        {
            debug_msg_ts_show(tpt_ctx->plt_ctx, "ssl_accept failed. Err: %d", res);
            SSL_free(new_ssl);
            close(new_fd);
            return;
        }

        if (tpt_ctx->use_ipv4)
        {
            dst_addr = (uint8_t *)&peer_addr.s4.sin_addr.s_addr;
            dst_addr_len = IPV4_ADDR_LEN;
        }
        else
        {
            dst_addr = (uint8_t *)peer_addr.s6.sin6_addr.s6_addr;
            dst_addr_len = IPV6_ADDR_LEN;
        }

        //Add to "IP address - ssl" table
        res = dtls_ip_ssl_add(dtls_ctx, new_ssl, new_fd, dst_addr, dst_addr_len, DTLS_SSL_TYPE_SERVER, 0);
        if (res != 0)
        {
            SSL_free(new_ssl);
            close(new_fd);
            debug_msg_show(tpt_ctx->plt_ctx, "Error: \"IP address - ssl\" table is full!");
            return;
        }
    }
    else if (rcx_size < 0)
    {
        if ((errno == EBADF)
#ifdef ENOTSOCK
            || (errno == ENOTSOCK)
#endif
            )
        {   //Bad socket file descriptor
            //Close and create a new listening socket
            debug_msg_show(tpt_ctx->plt_ctx, "DTLS: new connection rcv error");
            close(dtls_ctx->sock_fd);
            if (!dtls_sock_create(tpt_ctx, tpt_ctx->host_port, tpt_ctx->use_ipv4, 1, 1,
                                  NULL, &dtls_ctx->sock_fd))

            {
                dtls_ctx->sock_fd = -1;
                return;
            }
        }
    }
}


/**
dtls_rd_thrd - thread to receive IPv4/IPv6 DTLS packets
@param[in]	tpt_ctx		Transport layer context
@return
*/
void dtls_rd_thrd(tpt_layer_ctx_t *tpt_ctx)
{
    dtls_ctx_t          *dtls_ctx = (dtls_ctx_t *)tpt_ctx->dtls_ctx;
    int                 loop_cnt;
    int                 i;
    int                 k;
    int                 fd_cnt;
    int                 max_fdn;    //Maximum file descriptor number
    ssl_sock_t          *ssl_sock;
    fd_set              rd_fds;
    struct timeval      tv;
    ipaddr_ssl_t        *tbl = dtls_ctx->ip_ssl_tbl;

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

    //Initialize DTLS session expiry time check value
    dtls_ctx->last_chk_tm = dtls_ctx->tmr_tick;

    while (tpt_ctx->rd_thrd_run)
    {
        //Clear the read set
        FD_ZERO(&rd_fds);

        //Prepare read set
        plt_mtx_lck(dtls_ctx->rdwr_mtx);

        FD_SET(dtls_ctx->sock_fd, &rd_fds);//Server listening socket fd
        max_fdn = dtls_ctx->sock_fd;

        for (i=0; i<MAX_ZWAVE_NODE; i++)
        {
            if (tbl[i].ipaddr_sz)
            {
                //Check client and server ssl
                for (k=0; k<2; k++)
                {
                    ssl_sock = &tbl[i].ssl_sock[k];
                    if (ssl_sock->state == DTLS_SSL_STA_DATA || ssl_sock->state == DTLS_SSL_STA_SESS_EXPIRED)
                    {
                        FD_SET(ssl_sock->sock_fd, &rd_fds);
                        if (ssl_sock->sock_fd > max_fdn)
                        {
                            max_fdn = ssl_sock->sock_fd;
                        }
                    }
                }
            }
        }
        plt_mtx_ulck(dtls_ctx->rdwr_mtx);

        //Wait until either socket has data ready to be received or timeout
        tv.tv_sec = 0;
        tv.tv_usec = 200000; //200 ms

        fd_cnt = select(max_fdn + 1, &rd_fds, NULL, NULL, &tv);

        if (fd_cnt == TPT_SOCKET_ERR)
        {
            perror("select failed");

            //Try to determine which socket is bad
            plt_mtx_lck(dtls_ctx->rdwr_mtx);

            dtls_rd_ready_hdlr(tpt_ctx, &rd_fds, 1);

            dtls_new_conn_hdlr(tpt_ctx);

            plt_mtx_ulck(dtls_ctx->rdwr_mtx);
            continue;
        }

        if (fd_cnt == 0)
        {
            //Timeout
            //Check each entry in the table for DTLS session expiry
            dtls_sess_expiry_chk(tpt_ctx);
            continue;
        }

        if (tpt_ctx->rd_thrd_run)
        {
            plt_mtx_lck(dtls_ctx->rdwr_mtx);

            dtls_rd_ready_hdlr(tpt_ctx, &rd_fds, 0);

            if (FD_ISSET(dtls_ctx->sock_fd, &rd_fds))
            {   //New DTLS connection request
                dtls_new_conn_hdlr(tpt_ctx);
            }

            plt_mtx_ulck(dtls_ctx->rdwr_mtx);
        }
    }

    tpt_ctx->rd_thrd_sts = 0;
}


/**
dtls_pkt_send - Send a DTLS packet
@param[in]	tpt_ctx		    Transport layer context
@param[in]	dst_addr		Destination address
@param[in]	dst_addr_len    Destination address length
@param[in]	buf		        Data buffer
@param[in]	len		        Data buffer length
@param[in]	ssl_type	    SSL type DTLS_SSL_TYPE_XXX
@return     Return non-zero indicates success, zero indicates failure.
*/
static int dtls_pkt_send(tpt_layer_ctx_t *tpt_ctx, uint8_t *dst_addr, uint8_t dst_addr_len, uint8_t *buf, uint16_t len, uint8_t ssl_type)
{
    dtls_ctx_t  *dtls_ctx = (dtls_ctx_t *)tpt_ctx->dtls_ctx;
    SSL         *ssl;
    ssl_sock_t  *ssl_sock;
    ipaddr_ssl_t *ipaddr_ssl_ent;
    int         res;
    int         ret = 0;
    int         pos;
    int         bytes_sent;

    plt_mtx_lck(dtls_ctx->rdwr_mtx);

    res = dtls_ip_ssl_get(dtls_ctx, dst_addr, dst_addr_len, ssl_type, &ssl, &pos);

    if (res == 0)
    {   //Found SSL socket
        ipaddr_ssl_ent = &dtls_ctx->ip_ssl_tbl[pos];
        ssl_sock = &ipaddr_ssl_ent->ssl_sock[ssl_type];

        if (ssl && (ssl_sock->state == DTLS_SSL_STA_DATA))
        {   //Send the packet
            bytes_sent = SSL_write(ssl, buf, len);

        #ifdef SHOW_PACKAT_INFO
            debug_msg_show(tpt_ctx->plt_ctx, "Send udp len: %d", bytes_sent);
        #endif

            if (bytes_sent == len)
            {   //Send O.K.
                //Reset client time to session expiry
                if (ssl_type == DTLS_SSL_TYPE_CLIENT)
                {
                    ssl_sock->tm_to_expire = ZIPV2_KEEP_ALIVE_EXP;
                    if (!ssl_sock->use_host_port)
                    {
                        ssl_sock->keep_alive_cnt = ZIPV2_SESS_KEEP_ALIVE_CNT;
                    }
                    ssl_sock->keep_alive_resp_exp = -1;
                }

                ret = 1;
            }
            else
            {
                //perror("Send udp failed");
                debug_msg_show(tpt_ctx->plt_ctx, "Send udp failed. Err: %d", bytes_sent);
            }
        }
    }
    plt_mtx_ulck(dtls_ctx->rdwr_mtx);

    return ret;
}


/**
dtls_sess_create - Create a new DTLS session
@param[in]	tpt_ctx		    Transport layer context
@param[in]	dst_addr		Destination address
@param[in]	dst_addr_len    Destination address length
@param[in]	ssl_type	    SSL type DTLS_SSL_TYPE_XXX
@param[in]	use_host_port	Flag to indicate whether the new UDP socket must use host listening port
@return     Return non-zero indicates success, zero indicates failure.
*/
static int dtls_sess_create(tpt_layer_ctx_t *tpt_ctx, uint8_t *dst_addr, uint8_t dst_addr_len, uint8_t ssl_type, int use_host_port)
{
    dtls_ctx_t  *dtls_ctx = (dtls_ctx_t *)tpt_ctx->dtls_ctx;
    int         res;
    int         dst_fd;
    SSL         *ssl;

    //Create socket
    if (use_host_port)
    {   //using host listening port
        res = dtls_sock_create(tpt_ctx, tpt_ctx->host_port, tpt_ctx->use_ipv4, 1, 0,
                               (tpt_ctx->use_ipv4)? NULL : tpt_ctx->listen_addr, &dst_fd);
    }
    else
    {
        res = dtls_sock_create(tpt_ctx, 0, tpt_ctx->use_ipv4, 0, 0,
                               (tpt_ctx->use_ipv4)? NULL : tpt_ctx->listen_addr, &dst_fd);
    }

    if (!res)
    {
        return 0;
    }

    //Create SSL connection using the newly create socket
    res = dtls_ssl_create(tpt_ctx, dst_fd, dst_addr, ZIPV2_HOME_CTL_PORT,
                          tpt_ctx->use_ipv4, &ssl);
    if (res != 0)
    {
        close(dst_fd);
        debug_msg_show(tpt_ctx->plt_ctx, "Create SSL failed. Err: %d", res);
        return 0;
    }

    //Connect to target host and perform DTLS handshakes
    res = dtls_func(tpt_ctx, ssl, DTLS_SSL_CONNECT, DTLS_MAX_TIMEOUT_SEC);
    if (res != SSL_SUCCESS)
    {
        SSL_free(ssl);
        close(dst_fd);
        debug_msg_show(tpt_ctx->plt_ctx, "SSL connect failed. Err: %d", res);
        return 0;
    }

    //Add to "IP address - ssl" table
    res = dtls_ip_ssl_add(dtls_ctx, ssl, dst_fd, dst_addr, dst_addr_len, ssl_type , use_host_port);
    if (res != 0)
    {
        SSL_free(ssl);
        close(dst_fd);
        debug_msg_show(tpt_ctx->plt_ctx, "Error: \"IP address - ssl\" table is full!");
        return 0;
    }

    return 1;
}


/**
dtls_pkt_send_new_sess - Send a DTLS packet using a new DTLS session
@param[in]	tpt_ctx		    Transport layer context
@param[in]	dst_addr		Destination address
@param[in]	dst_addr_len    Destination address length
@param[in]	buf		        Data buffer
@param[in]	len		        Data buffer length
@param[in]	ssl_type	    SSL type DTLS_SSL_TYPE_XXX
@param[in]	use_host_port	Flag to indicate whether the new UDP socket must use host listening port
@return     Return non-zero indicates success, zero indicates failure.
*/
static void dtls_pkt_send_new_sess(tpt_layer_ctx_t *tpt_ctx, uint8_t *dst_addr, uint8_t dst_addr_len, uint8_t *buf, uint16_t len,
                                  uint8_t ssl_type, int use_host_port)
{
    int res;

    res = dtls_sess_create(tpt_ctx, dst_addr, dst_addr_len, ssl_type, use_host_port);

    if (res)
    {
        dtls_pkt_send(tpt_ctx, dst_addr, dst_addr_len, buf, len, ssl_type);
    }
}


/**
dtls_wr_thrd - thread for sending IPv4/IPv6 DTLS packet
@param[in]	tpt_ctx		Transport layer context
@return
*/
void dtls_wr_thrd(tpt_layer_ctx_t *tpt_ctx)
{
    util_lst_t      *wr_req;
    send_pkt_t      *send_pkt;
    int             bytes_sent;
    int             res;
    int             pos;
    SSL             *ssl;
    dtls_ctx_t      *dtls_ctx = (dtls_ctx_t *)tpt_ctx->dtls_ctx;
    uint8_t         *dst_addr;
    ipaddr_ssl_t    *ipaddr_ssl_ent;
    ssl_sock_t      *ssl_sock;
    uint8_t         dst_addr_len;

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
            {
                dst_addr = (uint8_t *)&send_pkt->dst.ipv4.sin_addr.s_addr;
                dst_addr_len = IPV4_ADDR_LEN;
            }
            else
            {
                dst_addr = (uint8_t *)send_pkt->dst.ipv6.sin6_addr.s6_addr;
                dst_addr_len = IPV6_ADDR_LEN;
            }

            //Get SSL context
            plt_mtx_lck(dtls_ctx->rdwr_mtx);

            if ((send_pkt->flag & TPT_SEND_FLG_NEW_DTLS_SESS) == 0)
            {   //Use existing DTLS session if possible

                if ((send_pkt->flag & TPT_SEND_FLG_USE_HOST_PORT) == 0)
                {   //-----------------
                    //Use non-host port
                    //-----------------
                    res = dtls_ip_ssl_get(dtls_ctx, dst_addr, dst_addr_len, DTLS_SSL_TYPE_CLIENT, &ssl, &pos);
                    ipaddr_ssl_ent = &dtls_ctx->ip_ssl_tbl[pos];
                    ssl_sock = &ipaddr_ssl_ent->ssl_sock[DTLS_SSL_TYPE_CLIENT];

                    if ((res == 0) && ssl && (ssl_sock->state == DTLS_SSL_STA_DATA) && (!ssl_sock->use_host_port))
                    {   //Found client SSL socket which uses non-host port, send the packet
                        bytes_sent = SSL_write(ssl, send_pkt->buf, send_pkt->len);

#ifdef SHOW_PACKAT_INFO
                        debug_msg_show(tpt_ctx->plt_ctx, "Send udp len: %d", bytes_sent);
#endif
                        // ALOGI("************* Send buffer to zipgateway:\n");
                        // debug_bin_show(tpt_ctx->plt_ctx, send_pkt->buf, bytes_sent);
                        // ALOGI("************* Send buffer to zipgateway end\n");

                        if (bytes_sent == send_pkt->len)
                        {   //Send O.K.
                            //Reset client time to session expiry
                            ssl_sock->tm_to_expire = ZIPV2_KEEP_ALIVE_EXP;
                            ssl_sock->keep_alive_cnt = ZIPV2_SESS_KEEP_ALIVE_CNT;
                            ssl_sock->keep_alive_resp_exp = -1;
                        }
                        else
                        {
                            //perror("Send udp failed");
                            debug_msg_show(tpt_ctx->plt_ctx, "Send udp failed. Err: %d", bytes_sent);
                        }

                        plt_mtx_ulck(dtls_ctx->rdwr_mtx);
                    }
                    else
                    {   //Client SSL socket is either expired or using host port
                        if ((res == 0) && ssl)
                        {
                            dtls_ssl_close(ipaddr_ssl_ent, DTLS_SSL_TYPE_CLIENT);
                        }

                        //Create new DTLS session and send the packet
                        plt_mtx_ulck(dtls_ctx->rdwr_mtx);
                        dtls_pkt_send_new_sess(tpt_ctx, dst_addr, dst_addr_len, send_pkt->buf, send_pkt->len, DTLS_SSL_TYPE_CLIENT, 0);
                    }
                }
                else
                {   //-----------------------
                    //Use host listening port
                    //-----------------------

                    //Use SSL type server first (higher priority)
                    res = dtls_ip_ssl_get(dtls_ctx, dst_addr, dst_addr_len, DTLS_SSL_TYPE_SERVER, &ssl, &pos);
                    ipaddr_ssl_ent = &dtls_ctx->ip_ssl_tbl[pos];
                    ssl_sock = &ipaddr_ssl_ent->ssl_sock[DTLS_SSL_TYPE_SERVER];

                    if ((res == 0) && ssl && (ssl_sock->state == DTLS_SSL_STA_DATA))
                    {   //Found and send packet
                        res = dtls_pkt_send(tpt_ctx, dst_addr, dst_addr_len, send_pkt->buf, send_pkt->len, DTLS_SSL_TYPE_SERVER);
                        if (!res)
                        {
                            debug_msg_show(tpt_ctx->plt_ctx, "dtls_pkt_send using DTLS_SSL_TYPE_SERVER failed!");
                        }
                        plt_mtx_ulck(dtls_ctx->rdwr_mtx);
                    }
                    else
                    {
                        //Server SSL socket is expired
                        if ((res == 0) && ssl)
                        {
                            dtls_ssl_close(ipaddr_ssl_ent, DTLS_SSL_TYPE_SERVER);
                        }

                        //Create new DTLS session and send the packet
                        plt_mtx_ulck(dtls_ctx->rdwr_mtx);
                        dtls_pkt_send_new_sess(tpt_ctx, dst_addr, dst_addr_len, send_pkt->buf, send_pkt->len, DTLS_SSL_TYPE_SERVER, 1);

                    }
                }
            }
            else
            {   //---------------------
                //Use new DTLS session
                //---------------------

                uint8_t ssl_type;

                ssl_type = (send_pkt->flag & TPT_SEND_FLG_USE_HOST_PORT)? DTLS_SSL_TYPE_SERVER : DTLS_SSL_TYPE_CLIENT;
                //Close existing DTLS session
                res = dtls_ip_ssl_get(dtls_ctx, dst_addr, dst_addr_len, ssl_type, &ssl, &pos);
                ipaddr_ssl_ent = &dtls_ctx->ip_ssl_tbl[pos];

                if ((res == 0) && ssl)
                {
                    dtls_ssl_close(ipaddr_ssl_ent, ssl_type);
                }

                //Create new DTLS session and send the packet
                plt_mtx_ulck(dtls_ctx->rdwr_mtx);
                dtls_pkt_send_new_sess(tpt_ctx, dst_addr, dst_addr_len, send_pkt->buf, send_pkt->len, ssl_type,
                                       (send_pkt->flag & TPT_SEND_FLG_USE_HOST_PORT)? 1 : 0);
            }

            free(wr_req);
        }
    }
}


/**
dtls_close_conn - Close all DTLS connections
@param[in] tpt_ctx		Context
@param[in] ipaddr       IP address
@param[in] ipaddr_sz    IP address size in bytes
@return
*/
void dtls_close_conn(tpt_layer_ctx_t *tpt_ctx, uint8_t *ipaddr, uint8_t ipaddr_sz)
{
    dtls_ctx_t      *dtls_ctx = (dtls_ctx_t *)tpt_ctx->dtls_ctx;
    SSL             *ssl;
    ipaddr_ssl_t    *ipaddr_ssl_ent;
    ssl_sock_t      *ssl_sock;
    int             pos;
    int             res;

    if (!dtls_ctx)
    {
        return;
    }

    plt_mtx_lck(dtls_ctx->rdwr_mtx);

    //Server type
    res = dtls_ip_ssl_get(dtls_ctx, ipaddr, ipaddr_sz, DTLS_SSL_TYPE_SERVER, &ssl, &pos);

    ipaddr_ssl_ent = &dtls_ctx->ip_ssl_tbl[pos];
    ssl_sock = &ipaddr_ssl_ent->ssl_sock[DTLS_SSL_TYPE_SERVER];

    if ((res == 0) && ssl && (ssl_sock->state == DTLS_SSL_STA_DATA))
    {
        dtls_ssl_close(ipaddr_ssl_ent, DTLS_SSL_TYPE_SERVER);
    }
    plt_mtx_ulck(dtls_ctx->rdwr_mtx);

    //Let the other DTLS process to have a chance to run as the dtls_ssl_close may take up to 3 seconds if the other end does not ACK
    plt_sleep(100);

    plt_mtx_lck(dtls_ctx->rdwr_mtx);

    //Client type
    res = dtls_ip_ssl_get(dtls_ctx, ipaddr, ipaddr_sz, DTLS_SSL_TYPE_CLIENT, &ssl, &pos);

    ipaddr_ssl_ent = &dtls_ctx->ip_ssl_tbl[pos];
    ssl_sock = &ipaddr_ssl_ent->ssl_sock[DTLS_SSL_TYPE_CLIENT];

    if ((res == 0) && ssl && (ssl_sock->state == DTLS_SSL_STA_DATA))
    {
        dtls_ssl_close(ipaddr_ssl_ent, DTLS_SSL_TYPE_CLIENT);
    }
    plt_mtx_ulck(dtls_ctx->rdwr_mtx);

}


static unsigned int dtls_psk_client_cb(SSL* ssl, const char* hint,
                                       char* identity, unsigned int id_max_len,
                                       unsigned char* key, unsigned int key_max_len)
{
    tpt_layer_ctx_t *tpt_ctx = (tpt_layer_ctx_t *)SSL_get_app_data(ssl);

    //strncpy(identity, "Client_identity", id_max_len);
    *identity = '\0';

    if (tpt_ctx->dtls_psk_len <= key_max_len)
    {
        memcpy(key, tpt_ctx->dtls_psk, tpt_ctx->dtls_psk_len);
        return tpt_ctx->dtls_psk_len;
    }

    return 0; /* 0 for error */
}


static unsigned int dtls_psk_svr_cb(SSL* ssl, const char* identity,
                                     unsigned char* key, unsigned int key_max_len)
{
    tpt_layer_ctx_t *tpt_ctx = (tpt_layer_ctx_t *)SSL_get_app_data(ssl);

    if (tpt_ctx->dtls_psk_len <= key_max_len)
    {
        memcpy(key, tpt_ctx->dtls_psk, tpt_ctx->dtls_psk_len);
        return tpt_ctx->dtls_psk_len;
    }

    return 0; /* 0 for error */
}


/**
dtls_tmr_tick_cb - Timer tick timeout callback
@param[in] data     Pointer to security layer context
@return
*/
static void    dtls_tmr_tick_cb(void *data)
{
    dtls_ctx_t  *dtls_ctx = (dtls_ctx_t *)data;

    //Increment timer tick
    dtls_ctx->tmr_tick++;
}


/**
dtls_client_setup - Setup DTLS client
@param[in]	tpt_ctx		Context
@return     Return zero indicates success, non-zero indicates failure.
*/
static int dtls_client_setup(tpt_layer_ctx_t *tpt_ctx)
{
    dtls_ctx_t  *dtls_ctx = (dtls_ctx_t *)tpt_ctx->dtls_ctx;

    dtls_ctx->client_ctx = SSL_CTX_new(DTLSv1_client_method());

    if (!dtls_ctx->client_ctx)
    {
        return -1;
    }

    //Save transport context
    dtls_ctx->tpt_ctx = tpt_ctx;

    if (SSL_CTX_set_cipher_list(dtls_ctx->client_ctx, cipherList /*"PSK"*/) == 0)
    {
        SSL_CTX_free(dtls_ctx->client_ctx);
        return -2;
    }
    SSL_CTX_set_psk_client_callback(dtls_ctx->client_ctx, dtls_psk_client_cb);

    return 0;
}


/**
dtls_svr_setup - Setup DTLS server
@param[in]	tpt_ctx		Context
@return     Return zero indicates success, non-zero indicates failure.
*/
static int dtls_svr_setup(tpt_layer_ctx_t *tpt_ctx)
{
    dtls_ctx_t  *dtls_ctx = (dtls_ctx_t *)tpt_ctx->dtls_ctx;

    dtls_ctx->svr_ctx = SSL_CTX_new(DTLSv1_server_method());

    if (!dtls_ctx->svr_ctx)
    {
        return -1;
    }

    if (SSL_CTX_set_cipher_list(dtls_ctx->svr_ctx, cipherList /*"PSK"*/) == 0)
    {
        SSL_CTX_free(dtls_ctx->svr_ctx);
        return -2;
    }

    SSL_CTX_set_psk_server_callback(dtls_ctx->svr_ctx, dtls_psk_svr_cb);

    SSL_CTX_use_psk_identity_hint(dtls_ctx->svr_ctx, NULL);//Disable identity hint

    SSL_CTX_set_cookie_generate_cb(dtls_ctx->svr_ctx, dtls_cookie_gen);

    SSL_CTX_set_cookie_verify_cb(dtls_ctx->svr_ctx, dtls_cookie_verify);


    //Create listening socket
    if (!dtls_sock_create(tpt_ctx, tpt_ctx->host_port, tpt_ctx->use_ipv4, 1, 1,
                          NULL, &dtls_ctx->sock_fd))
    {
        SSL_CTX_free(dtls_ctx->svr_ctx);
        return -3;
    }

    return 0;
}


/**
dtls_init - Init the DTLS layer.
@param[in,out]	tpt_ctx		Context
@return     Return zero indicates success, non-zero indicates failure.
*/
int dtls_init(tpt_layer_ctx_t *tpt_ctx)
{
    dtls_ctx_t  *dtls_ctx;
    int         ret;

#if defined(OPENSSL_THREADS)
    // Thread support enabled
    #ifdef  DEBUG_ZIP_DTLS
    debug_msg_show(tpt_ctx->plt_ctx, "Openssl lib support threads");
    #endif
#else
    // No thread support
    debug_msg_show(tpt_ctx->plt_ctx, "Openssl lib doesn't support threads");
    return -1;
#endif

    dtls_ctx = calloc(1, sizeof(dtls_ctx_t));

    if (!dtls_ctx)
    {
        return -1;
    }

    //Save the context
    tpt_ctx->dtls_ctx = dtls_ctx;

    if (!plt_mtx_init(&dtls_ctx->rdwr_mtx))
    {
        ret = -2;
        goto l_DTLS_INIT_ERR0;
    }

    //Start periodic timer
    dtls_ctx->tick_tmr_ctx = plt_periodic_start(tpt_ctx->plt_ctx, 1000, dtls_tmr_tick_cb, tpt_ctx->dtls_ctx);
    if (!dtls_ctx->tick_tmr_ctx)
    {
        ret = -3;
        goto l_DTLS_INIT_ERR1;
    }

    SSL_library_init();
    SSL_load_error_strings();

    //Generate cookie key
    if (RAND_bytes(dtls_ctx->cookie_key, COOKIE_KEY_LEN) <= 0)
    {
        ret = -4;
        goto l_DTLS_INIT_ERR2;
    }


    if (dtls_client_setup(tpt_ctx) != 0)
    {
        ret = -5;
        goto l_DTLS_INIT_ERR2;
    }

    if (dtls_svr_setup(tpt_ctx) != 0)
    {
        ret = -6;
        goto l_DTLS_INIT_ERR3;
    }

    return 0;

l_DTLS_INIT_ERR3:
    SSL_CTX_free(dtls_ctx->client_ctx);
l_DTLS_INIT_ERR2:
    ERR_free_strings();
    plt_tmr_stop(tpt_ctx->plt_ctx, dtls_ctx->tick_tmr_ctx);
l_DTLS_INIT_ERR1:
    plt_mtx_destroy(dtls_ctx->rdwr_mtx);
l_DTLS_INIT_ERR0:
    free(dtls_ctx);
    return ret;
}


/**
dtls_exit - exit the DTLS layer.
@param[in,out]	tpt_ctx		Context
@return
*/
void dtls_exit(tpt_layer_ctx_t *tpt_ctx)
{
    int             i;
    dtls_ctx_t      *dtls_ctx = (dtls_ctx_t *)tpt_ctx->dtls_ctx;
    ipaddr_ssl_t    *tbl = dtls_ctx->ip_ssl_tbl;

    plt_mtx_lck(dtls_ctx->rdwr_mtx);
    for (i=0; i<MAX_ZWAVE_NODE; i++)
    {
        if (tbl[i].ipaddr_sz)
        {
            dtls_ssl_close(&tbl[i], DTLS_SSL_TYPE_CLIENT);
            dtls_ssl_close(&tbl[i], DTLS_SSL_TYPE_SERVER);
        }
    }

    if (dtls_ctx->sock_fd > 0)
    {
        close(dtls_ctx->sock_fd);
    }

    plt_mtx_ulck(dtls_ctx->rdwr_mtx);

    SSL_CTX_free(dtls_ctx->svr_ctx);
    SSL_CTX_free(dtls_ctx->client_ctx);
    ERR_free_strings();
    plt_tmr_stop(tpt_ctx->plt_ctx, dtls_ctx->tick_tmr_ctx);
    plt_mtx_destroy(dtls_ctx->rdwr_mtx);
    free(dtls_ctx);

}


