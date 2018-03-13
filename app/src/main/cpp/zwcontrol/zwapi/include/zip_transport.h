/**
@file   zip_transport.h - Z/IP interface transport layer implementation header file.

        Platform specific implementation of ipV6 UDP function.

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

#ifndef _ZIP_TRANSPORT_DAVID_
#define _ZIP_TRANSPORT_DAVID_
//#define WIN32
//#define OS_LINUX

#include <stdint.h>
#include <stdio.h>

#if defined(_WINDOWS) || defined(WIN32)
#include <winsock2.h>
#include <Ws2tcpip.h>
#else
#define _GNU_SOURCE         //For IPv6
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#endif
#include "zip_error.h"
#include "zip_platform.h"
#include "zip_util.h"
#include "zip_tls_server.h"

//#define SHOW_PACKAT_INFO    ///< To control whether to show packet information

/**
@defgroup Transport Transport layer APIs
Platform specific implementation of ipV6 UDP function.
@ingroup zwarecapi
@{
*/

//Minimum read time out value
#ifdef TCP_PORTAL
#define TLS_TRANSPORT_READ_TIMEOUT  3000    ///< Read time out value in milliseconds for select() loop

#define TLS_INACTIVITY_TIMEOUT      120     ///< Inactivity time out value in terms of multiple of TLS_TRANSPORT_READ_TIMEOUT

#endif

#define TRANSPORT_READ_TIMEOUT_MIN  2000    ///< Minimum read time out value in milliseconds

#define ZWNET_GW_DISCVR_TMOUT       2000    ///< Gateway discovery time out in milliseconds

#define MAX_IPV6_INTF_NAME_LEN      8       ///< Maximum network interface name length; including terminating Null character.

#define MAX_IPV6_ADDR_LEN           56      ///< Maximum IPv6 address string length; including terminating Null character.

#define MAX_IPV4_ADDR_LEN           16      ///< Maximum IPv4 address string length; including terminating Null character.

#define IPV6_ADDR_LEN               16      ///< IPv6 address length in bytes

#define IPV4_ADDR_LEN               4       ///< IPv4 address length in bytes

#define ZWAVE_HOME_CTL_PORT         4123    ///< Z-wave home controller UDP port

#define MAX_RCX_SIZE                1280    ///< Maximum UDP payload

#if defined(_WINDOWS) || defined(WIN32)
///
/// Read thread state
#define     RD_THRD_STATE_ENT_SELECT    0   ///< Entering select loop
#define     RD_THRD_STATE_LV_SELECT     1   ///< Leaving select loop
#define     RD_THRD_STATE_ENT_CALLBACK  2   ///< Entering read callback function
#define     RD_THRD_STATE_LV_CALLBACK   3   ///< Leaving read callback function
#endif

///
/// Flag for additional option to send packet (bit-mask)
#define     TPT_SEND_FLG_NEW_DTLS_SESS  0x01   ///< Terminate the current DTLS session and use a new session to send packet
#define     TPT_SEND_FLG_USE_HOST_PORT  0x02   ///< Bind to host listening port.
#define     TPT_SEND_FLG_USE_SRC_ADDR   0x04   ///< Use specified source address when sending packet.

#if defined(_WINDOWS) || defined(WIN32)
#define TPT_SOCKET_ERR      SOCKET_ERROR    ///< Socket error
#else
#define TPT_SOCKET_ERR      -1              ///< Socket error
#endif

///
/// IPv4/IPv6 address storage
typedef union
{
    struct sockaddr_storage ss;
    struct sockaddr_in s4;
    struct sockaddr_in6 s6;
} sockaddr_stor_t;


///
/// IPv4/IPv6 send packet
typedef struct
{
    union
    {
        struct sockaddr_in  ipv4;
        struct sockaddr_in6 ipv6;
    } dst;                          ///< Destination address
    uint8_t                 src_addr[IPV6_ADDR_LEN];///< Source address to use when sending packet; valid if TPT_SEND_FLG_USE_SRC_ADDR is set
    uint16_t                flag;   ///< Additional options, see TPT_SEND_FLG_XXX
    uint16_t                len;    ///< Length of data in the buf
    uint8_t                 buf[1]; ///< place holder for the data buffer

} send_pkt_t;



//Forward declaration of frame layer context
struct _frm_layer_ctx;

//Forward declaration of transport layer context
struct _tpt_layer_ctx;

typedef void (*tpt_rd_fn)(struct _tpt_layer_ctx *tpt_ctx, uint8_t *buf, uint16_t data_len,
                          struct sockaddr *src_addr, uint8_t *dst_addr);
/**<
Data received callback function
@param[in]	tpt_ctx	    context
@param[in]	buf	        buffer that stores the received bytes
@param[in]	data_len    number of bytes received
@param[in]	src_addr    source address of the received packet
@param[in]	dst_addr    optional destination address (as in the packet header) of the received packet
@return
*/


///
/// Z/IP transport layer context
typedef struct _tpt_layer_ctx
{
    //Callback functions
    tpt_rd_fn   tpt_rd_cb;///< Data received callback function
    #ifdef TCP_PORTAL
    void    (*tpt_net_err_cb)(void *user);///< Network error callback function
    #endif

    //Data
    struct _frm_layer_ctx* frm_layer_ctx; ///< Pointer to frame layer context

    uint8_t         zip_router[IPV6_ADDR_LEN];///< Z/IP router address
    uint16_t        host_port;          ///< Listening and sending port used by the host
    int             use_ipv4;           ///< Flag to indicate whether to use IPv4 as transport IP protocol
    #if defined(_WINDOWS) || defined(WIN32)
    SOCKET          sock_fd;            ///< Socket file descriptor for sending/receiving UDP packets
    #else
    int             sock_fd;            ///< Socket file descriptor for sending/receiving UDP packets
    #endif
    #ifdef TCP_PORTAL
    void            *ssl;               ///< SSL object pointer
    void            *user;              ///< User parameter when tpt_net_err_cb is invoked
    tls_clnt_prof_t zipr_prof;          ///< ZIPR profile
    #endif
    struct plt_mtx_t *wr_req_mtx;       ///< mutex for accessing write request list
    void            *wr_q_sem;          ///< semaphore for waiting requests to write to the comm port
    util_lst_t      *wr_req_hd;         ///< head of linked list for write requests
    plt_ctx_t       *plt_ctx;           ///< Platform context
    volatile int    rd_thrd_run;        ///< control the read thread whether to run. 1 = run, 0 = stop
    #ifdef OS_LINUX
    void            *wr_thrd_ctx;       ///< write thread context
    #endif
    #ifndef TCP_PORTAL
    void            *dtls_ctx;          ///< DTLS context
    uint8_t         listen_addr[IPV6_ADDR_LEN]; ///< Listening IPv6 address
    uint8_t         dtls_psk[32];       /**< DTLS pre-shared key */
    uint8_t         dtls_psk_len;       /**< DTLS pre-shared key length (bytes). If length is zero,
                                             no DTLS will be used, i.e. communication will be insecured */
    #endif

    volatile int    rd_thrd_sts;        ///< read thread status. 1 = running, 0 = thread exited
    volatile int    wr_thrd_run;        ///< control the write thread whether to run. 1 = run, 0 = stop
    volatile int    wr_thrd_sts;        ///< write thread status. 1 = running, 0 = thread exited
    volatile int    wr_started;         ///< flag to indicate write has started.

} tpt_layer_ctx_t;

int tpt_snd_req_create(tpt_layer_ctx_t *tpt_ctx, struct sockaddr *dst, uint8_t *src_addr,
                       uint8_t *buf, uint16_t dat_sz, uint16_t flag);
/**<
create and queue a send request to the write thread.
@param[in,out]	tpt_ctx		Context
@param[in]      dst         Destination address
@param[in]      src_addr    Optional source address to use when sending the packet; valid if TPT_SEND_FLG_USE_SRC_ADDR is set
@param[in]      buf         Buffer that store the data to be sent
@param[in]      dat_sz      Size of data to be sent
@param[in]      flag        Additional options, see TPT_SEND_FLG_XXX
@return                     Return 0 on success, negative error number on failure.
*/

int tpt_init(tpt_layer_ctx_t *tpt_ctx);
/**<
Init the transport layer.
Should be called once before calling the other transport layer functions
@param[in,out]	tpt_ctx		Context
@return     Return zero indicates success, non-zero indicates failure.
@post       Caller should not modify the tpt_ctx after this call
*/

void tpt_exit(tpt_layer_ctx_t *tpt_ctx);
/**<
Clean up the transport layer
@param[in,out]	tpt_ctx		Context
@return
*/

#ifdef TCP_PORTAL
void tpt_svr_ipv6_addr_set(tpt_layer_ctx_t *tpt_ctx, uint8_t *svr_ipv6_addr);
/**<
Set the server IPv6 address used in the tunnel
@param[in]	tpt_ctx		    Context
@param[in]	svr_ipv6_addr	server IPv6 address
@return
*/
#endif


/**
@}
*/


#endif /* _ZIP_TRANSPORT_DAVID_ */
