/**
@file   zip_tls_server.h - Z/IP interface TLS implementation header file.

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

#ifndef _ZIP_TLS_DAVID_
#define _ZIP_TLS_DAVID_


/**
@defgroup TLS TLS APIs
Platform specific implementation of TLS function.
@ingroup zwarecapi
@{
*/

#define TLS_SERVER_PORT			44123           ///< TLS server listening TCP port
#define MAX_TLS_Q_LEN			128             ///< Maximum queue length for pendig TLS connection
#define MAX_CLIENT_TLS			MAX_TLS_Q_LEN   ///< Maximum clients waiting for TLS connection handshaking
#define MAX_TLS_TX_SIZE         1200            ///< Maximum TLS transmit payload size
#define TLS_TIMER_TICK          1000            ///< Periodic timer tick interval in ms
#define TLS_TIMER_CHK           3               ///< Check for timer expiry interval in terms of TLS_TIMER_TICK
#define TLS_HANDSHAKE_TIMEOUT   (8*60)          ///< TLS and ZIPR handshaking timeout in terms of TLS_TIMER_TICK
#define TLS_GW_DISCVR_TIMEOUT   2               ///< ZIP gateway discovery timeout in terms of TLS_TIMER_TICK
#define TLS_UNREGISTER_TIMEOUT  5               ///< Time to wait for ZIP gateway unregister in terms of TLS_TIMER_TICK
#define ZIPR_ID_LEN	            8               ///< ZIPR id (IEEE EUI-64 identifier) length
#define ZIPR_PIN_LEN	        8               ///< ZIPR PIN length
#define ZIPR_KEEP_ALIVE	        -5120           ///< Return value from tls_rx() to indicate keep alive message received
#define SSL_ERROR_CONN_LOST     -256            ///< TLS connection lost


///
/// TLS client profile
typedef struct
{
    uint8_t         clnt_id[ZIPR_ID_LEN];   ///< Client's ID (currently using MAC address)
    uint8_t         clnt_pin[ZIPR_PIN_LEN]; ///< Client's PIN (password)
    uint8_t         clnt_ipv6_addr[16];     ///< Client's IPv6 address. May be all-zeroes IPv6 address
    uint8_t         clnt_dflt_gw[16];       ///< Client's default IPv6 gateway
    uint8_t         clnt_pan_prefix[16];    ///< Client's PAN interface prefix with /64 prefix length. May be all-zeroes IPv6 address
    uint8_t         clnt_unsolicited_dst[16];///< Client's forwarding destination address for unsolicited message
    uint8_t         svr_ipv6_addr[16];      ///< Server's IPv6 address
    uint16_t        clnt_unsolicited_port;  ///< Client's forwarding destination port for unsolicited message. Should be 4123
    uint8_t         clnt_ipv6_prefix;       ///< Client's IPv6 address prefix length
    uint8_t         svr_ipv6_prefix;        ///< Server's IPv6 address prefix length
} tls_clnt_prof_t;

///
/// TLS client profile linked-list
typedef struct _tls_clnt_prof_lst
{
    struct _tls_clnt_prof_lst   *next;      ///< Next TLS client profile
    tls_clnt_prof_t             clnt_prof;  ///< Client profile
} tls_clnt_prof_lst;

typedef int (*tls_svr_cb_t)(int clnt_fd, void *clnt_ssl, tls_clnt_prof_t *clnt_prof, void *usr_param);
/**<
new client connection callback from TLS server
@param[in]	clnt_fd	    client socket file descriptor
@param[in]	clnt_ssl	client SSL object pointer
@param[in]	clnt_prof	client profile used
@param[in]	usr_param   user defined parameter
@return     Non-zero if the new client connection is accepted; zero if it is rejected.
*/

///
/// TLS server context
typedef struct
{
    volatile int        svr_thrd_run;   ///< control the TLS server thread whether to run. 1 = run, 0 = stop
    volatile int        svr_thrd_sts;   ///< TLS server thread status. 1 = running, 0 = thread exited
    int                 sock_fd;        ///< server socket descriptor
    void                *ssl_ctx;       ///< SSL context
    struct plt_mtx_t    *clnt_prof_mtx; ///< mutex for accessing client profiles list
    util_lst_t          *clnt_prof_hd;  ///< head of linked list for client profiles
    tls_svr_cb_t        cb;             ///< new client connection callback function
    void                *usr_param;     ///< user defined parameter used in callback function
    void                *tmr_tick_ctx;  ///< Periodic timer tick context
    char                *ca_file;       /**< File path to CA certificate that is used to sign Z/IP gateway
                                             public certificate*/
    char                *ssl_file;      ///< File path to SSL certificate (portal public certificate)
    char                *pvt_key_file;  ///< File path to portal private key
    volatile uint16_t   tmr_tick;       ///< Periodic timer tick, updated every TLS_TIMER_TICK ms
    uint16_t            clean_up_tick;  ///< Clean up timer tick
    uint16_t            svr_port;       ///< Server listening port
    volatile int        chk_tmr;        ///< Flag to determine whether to check for timer expiry
    plt_ctx_t           plt_ctx;        ///< Platform context
} tls_svr_ctx_t;

///
/// Function prototypes
void *tls_svr_start(tls_clnt_prof_t *clnt_prof, int prof_cnt, tls_svr_cb_t cb, void *usr_param,
                    char *ca_file, char *ssl_file, char *pvt_key_file, uint16_t port);
/**<
Start TLS server to listen for ZIPR connection
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

int tls_svr_shutdown(void *ctx);
/**<
Shutdown TLS server listening socket
@param[in]	    ctx		    The context returned from the call to tls_svr_start()
@return Zero on success, non-zero on failure.
*/

int tls_svr_stop(void *ctx);
/**<
Stop TLS server
@param[in]	    ctx		    The context returned from the call to tls_svr_start()
@return Zero on success, non-zero on failure.
@post After the call, the ctx is invalid and should not be used
*/

int tls_rx(void *ssl, uint8_t *buf, int ilength, struct sockaddr_in6 *src_addr);
/**<
Receive data from the SSL connection
@param[in]	    ssl	        SSL object pointer
@param[in]	    buf	        User supplied buffer to store the data
@param[in]	    ilength	    Length of the buffer
@param[in]	    src_addr    Source IPv6 address
@return Size of the data in bytes if success. Zero if the received data if for handshaking/keep alive. Negative error number if failed.
*/

int tls_tx(void *ssl, uint8_t *data, uint16_t len, struct sockaddr_in6 *src_addr, struct sockaddr_in6 *dst_addr);
/**<
Send data to the other end of SSL connection
@param[in]	    ssl	        SSL object pointer
@param[in]	    data	    Data buffer
@param[in]	    len	        Length of data
@param[in]	    src_addr    Source IPv6 address
@param[in]	    dst_addr    Destination IPv6 address
@return Number of bytes sent if success. Negative error number if failed.
*/

void tls_conn_close(int sfd, void *ssl);
/**<
Close client's socket fd and SSL
@param[in]	    sfd	 Client socket fd
@param[in]	    ssl	 Client SSL object pointer
@return
*/

int tls_clnt_add(void *ctx, tls_clnt_prof_t *clnt_prof);
/**<
Add a client profile to the TLS server internal list, overwriting old entry with the same gateway id.
@param[in]	    ctx		    The context returned from the call to tls_svr_start()
@param[in]	    clnt_prof	Client profile
@return                     Return non-zero on success, zero on failure.
*/

int tls_clnt_rm(void *ctx, uint8_t *gw_id);
/**<
Remove a client profile from the TLS server internal list
@param[in]	    ctx		    The context returned from the call to tls_svr_start()
@param[in]	    gw_id	    Gateway id
@return                     Return non-zero on success, zero on failure.
*/

int tls_clnt_find(void *ctx, tls_clnt_prof_t *clnt_prof);
/**<
Find a client profile in the TLS server internal list based on gateway id.
@param[in]	    ctx		    The context returned from the call to tls_svr_start()
@param[in,out]	clnt_prof	Client profile buffer with the gateway id set as input key to be searched.
                            On success, this buffer will be returned with found entry.
@return                     Return non-zero on success, zero on failure.
*/

int tls_clnt_list_get(void *ctx, tls_clnt_prof_lst **lst_hd);
/**<
Get all the client profiles in the TLS server internal list
@param[in]	    ctx		    The context returned from the call to tls_svr_start()
@param[out]	    lst_hd		List head of the client profiles list
@return                     Number of client profiles in the list; negative value on failure.
@post Caller must call tls_clnt_list_free() to free the linked-list if return value is greater than zero.
*/

void tls_clnt_list_free(tls_clnt_prof_lst *lst_hd);
/**<
Free client profiles list
@param[in]	    lst_hd		List head of the client profiles list
@return
*/

/**
@}
*/


#endif /* _ZIP_TLS_DAVID_ */
