/**
@file   zip_frame.h - Z/IP interface frame layer implementation header file.

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
#ifndef _ZIP_FRAME_DAVID_
#define _ZIP_FRAME_DAVID_

#include "zip_transport.h"

/**
@defgroup Frame Frame layer APIs
Assemble frame from the bytes received from transport layer
and handling of frame resend if required.
@ingroup zwarecapi
@{
*/

#define ZIP_FRM_HDR_SIZE             7   ///< ZIP packet header size

#define ZIP_FRM_KEEP_ALIVE_SZ        3   ///< ZIP keep alive packet frame size

#define FRAME_MAX_RESEND             1   ///< The maximum number of frame resend

#define ZIP_FRM_ND_SOLICIT_SZ        20  ///< ZIP packet ZIP_NODE_SOLICITATION frame size

#define ZIP_FRM_ND_INV_SOLICIT_SZ     4  ///< ZIP packet ZIP_INV_NODE_SOLICITATION frame size

#define ZIP_FRM_ND_ADVT_SZ           24  ///< ZIP packet ZIP_NODE_ADVERTISEMENT frame size

#define ZIP_FRM_ENCAP_FMT_SZ          4  ///< ZIP packet extension for encapsulation format info size
#define ZIP_FRM_IMA_GET_SZ            2  ///< ZIP packet extension for IMA_GET size
#define ZIP_FRM_EXT_TOTAL_SZ          1  ///< ZIP packet extension "total size" field size

//Minimum send frame time out value
//#define     FRAME_SEND_TIMEOUT_MIN      6400    ///< Minimum send frame time out value in milliseconds
#define     FRAME_SEND_TIMEOUT_MIN      2500    ///< Minimum send frame time out value in milliseconds

//#define     FRAME_SEND_MIN_BUSY_WAIT    7000    ///< Minimum time out value when host indicates that it is busy in milliseconds

//#define     FRAME_SEND_DFLT_BUSY_WAIT   10000   ///< Default time out value when host indicates that it is busy in milliseconds

///
/// Z-wave Serial API send frame status
typedef enum
{
    FRAME_SEND_UNKNOWN,      ///< Unknown
    FRAME_SEND_OK,           ///< Send frame succeeded with ACK received.
    FRAME_SEND_TIMEOUT,      ///< Send frame timeout or no response from receiver
    FRAME_SEND_NACK,         ///< Frame failed to reach destination host
    FRAME_SEND_BUSY,         ///< The message has not timed out yet.The destination host may have a long response time
    FRAME_SEND_SYSTEM        ///< Internal system error.
} frm_snd_sts_t;

/** Byte-2 of Z/IP frame bit definitions */
#define ZIP_FRM_ACK_REQ     0x80    ///< request receiver to send back an ACK
#define ZIP_FRM_ACK_RESP    0x40    ///< ACK response to the previous frame
#define ZIP_FRM_NACK_RESP   0x20    ///< Previously sent Z/IP packet didn't reach destination host
#define ZIP_FRM_WAIT_RESP   0x10    ///< The destination host may have a long response time
#define ZIP_FRM_Q_FULL_RESP 0x08    ///< Queue is full
#define ZIP_FRM_OP_ERR_RESP 0x04    ///< Critical option is unrecognized

/** Byte-3 of Z/IP frame bit definitions */
#define ZIP_FRM_HDR_EXT     0x80    ///< header extension included
#define ZIP_FRM_ZWAVE_CMD   0x40    ///< Z-wave command included
#define ZIP_FRM_MORE_INFO   0x20    ///< more information
#define ZIP_FRM_SECURE      0x10    ///< Z/IP packet is secure

/** Z/IP frame header extension option types */
#define ZIP_OPT_EXP_DELAY   0x01    ///< Expected delay
#define ZIP_OPT_IMA_GET     0x02    ///< Installation and maintenance get
#define ZIP_OPT_IMA_RPT     0x03    ///< Installation and maintenance report
#define ZIP_OPT_ENCAP_FMT   0x04    ///< Encapsulation format info
#define ZIP_OPT_MULTICAST   0x05    ///< Multicast Addressing has been used by the sending node in the Z-Wave network

/** Z/IP encapsulation format information bitmask */
#define ZIP_ENCAP_FMT_INFO_CRC16   0x0001   ///< CRC-16

/** Z/IP frame header extension installation and maintenance report IME types */
#define ZIP_IME_ROUTE_CHG   0       ///< Route changed
#define ZIP_IME_TX_TIME     0x01    ///< Transmission time
#define ZIP_IME_LWR         0x02    ///< Last working route
#define ZIP_IME_RSSI        0x03    ///< Receiver Signal Strength Indicator
#define ZIP_IME_ACK_CH      0x04    ///< ACK channel
#define ZIP_IME_TX_CH       0x05    ///< Transmit channel
#define ZIP_IME_ROUTE_SCH   0x06    ///< Routing scheme
#define ZIP_IME_ROUTE_ATMP  0x07    ///< Routing attempts
#define ZIP_IME_LST_F_LNK   0x08    ///< Last failed link

/** Flag bit-mask definition for Z/IP frame used by the library */
#define ZIP_FLAG_SECURE         0x0001    ///< frame is secure
#define ZIP_FLAG_ACK_REQ        0x0002    ///< set the Z/IP frame ACK request bit
#define ZIP_FLAG_ACK_RESP       0x0004    ///< set the Z/IP frame ACK response bit
#define ZIP_FLAG_NACK_OPT_RESP  0x0008    ///< set the Z/IP frame NACK response bit and "option error" bit
#define ZIP_FLAG_HDR_EXT        0x0010    ///< set the Z/IP frame header extension included bit
#define ZIP_FLAG_ENCAP_INFO     0x0020    ///< frame contains Z-wave encapsulation format info
#define ZIP_FLAG_MORE_INFO      0x0040    ///< set the Z/IP frame "more information" bit
#define ZIP_FLAG_IMA_GET        0x0080    ///< request IMA report from the ACK of this frame
#define ZIP_FLAG_MULTICAST      0x0100    ///< multicast addressing has been used by the sending node in the Z-Wave network
#define ZIP_FLAG_NO_NEW_DTLS    0x0200    ///< not to create new DTLS session when re-send a Z/IP packet
#define ZIP_FLAG_HOST_PORT      0x0400    ///< use host listening port socket to send a Z/IP packet


///
/// Endpoints address
typedef struct
{
    uint8_t src_ep;             ///< source endpoint (1 to 127); zero if the device does not support multi-channel
    uint8_t dst_ep;             ///< destination endpoint (1 to 127); zero if the device does not support multi-channel
    uint8_t dst_ep_type;        /**< destination endpoint type; 0=individual endpoint; 1=bit address endpoints where
                                     bit-0 of dst_ep represents endpoint 1, bit-6 represents endpoint 7.
                                    */
} frm_ep_t;

///
/// Parameters for sending a frame
typedef struct
{
    struct sockaddr *dst;       ///< destination address
    uint8_t         *src_addr;  ///< optional source address to use when sending the frame
    uint8_t         *cmd_buf;   ///< the command buffer, NULL if no command to be sent
    uint16_t        cmd_len;    ///< length of command
    uint16_t        flag;       ///< flag for the frame: ZIP_FLAG_XXX
    uint16_t        encap_fmt;  ///< Z-wave encapsulation format (valid if flag ZIP_FLAG_ENCAP_INFO is set).
    uint8_t         seq_num;    ///< sequence number of frame to the specific destination node
    uint8_t         wkup_poll;  ///< flag to indicate this frame is sent for the purpose of wakeup notification polling
    frm_ep_t        ep_addr;    ///< source and destination endpoint addresses
} frm_snd_prm_t;


//Forward declaration of session layer context
struct _ssn_layer_ctx;

typedef void (*snd_frm_sts_fn)(struct _frm_layer_ctx *frm_ctx, frm_snd_sts_t status, uint8_t seq_num, struct sockaddr *src_addr,
                               uint8_t ima_rpt_len, uint8_t *ima_rpt);
/**<
send frame status callback function
@param[in]	frm_ctx	    context
@param[in]	status	    status, see FRAME_SEND_XXX
@param[in]	seq_num	    Z/IP sequence number
@param[in]	src_addr	source address
@param[in]	ima_rpt_len	Z/IP header extension IMA report length
@param[in]	ima_rpt	    Buffer to store the Z/IP header extension IMA report; this field is valid only if ima_rpt_len > 0
@return
*/

typedef void (*rx_frm_fn)(struct _frm_layer_ctx *frm_ctx,
                         uint8_t   *buf, uint16_t len,
                         struct sockaddr *src_addr, frm_ep_t *ep_addr,
                         uint16_t flag, uint16_t encap_fmt);
/**<
received frame callback function
@param[in]	frm_ctx	    context
@param[in]	buf	        buffer to stored the received frame
@param[in]	len	        length of buf
@param[in]	src_addr    source address of the received frame
@param[in]	ep_addr     source and destination endpoint of the received frame
@param[in]	flag	    flag, see ZIP_FLAG_XXX
@param[in]	encap_fmt   Z-wave encapsulation format (valid if flag ZIP_FLAG_ENCAP_INFO is set).
                        This format should be passed back when responding
                        to unsolicited command with the flag ZIP_FLAG_ENCAP_INFO is set.

@return
*/


///
/// Z-wave HCI frame layer context
typedef struct _frm_layer_ctx
{
    //Callback functions to session layer
    snd_frm_sts_fn      snd_frm_sts_cb;     ///< send frame status callback
    rx_frm_fn           rx_frm_cb;          ///< received frame callback

    //Data
    volatile int        run;                ///< Control whether to run. 1 = run, 0 = stop
    struct plt_mtx_t    *wr_mtx;            ///< Mutex for writing to transport layer
    void                *snd_tmr_ctx;       ///< Send frame timer context
    uint8_t             *resnd_frm_buf;     ///< Backup frame for resend
    uint16_t            resnd_frm_len;      ///< Backup frame length
    uint8_t             resnd_frm_wkup_poll;///< Flag to indicate this frame is sent for the purpose of wakeup notification polling
    uint8_t             resend_cnt;         ///< Number of resend of the last frame
    uint16_t            frm_rcvd;           ///< Flag to indicate the frame has been received by Z/IP gateway
    uint8_t             resnd_new_dtls;     ///< Flag to indicate whether to create new DTLS session when re-send a Z/IP packet
    uint8_t             resnd_host_port;    ///< Flag to indicate whether to use host listening port socket re-send a Z/IP packet
    union
    {
        struct sockaddr_in  ipv4;
        struct sockaddr_in6 ipv6;
    } resnd_dst;                            ///< Destination IPv6 address for resend
    uint32_t            resnd_tmout;        ///< Resend frame timeout value in milliseconds
    tpt_layer_ctx_t     tpt_ctx;            ///< Transport layer context
    plt_ctx_t           *plt_ctx;           ///< Platform context
    struct _ssn_layer_ctx *ssn_layer_ctx;   ///< Pointer to session layer context

} frm_layer_ctx_t;


int frm_gw_send(frm_layer_ctx_t *frm_ctx, uint8_t cls, uint8_t cmd, uint8_t *data, uint8_t data_len);
/**<
Send frame to Z/IP router/gateway without Z/IP encapsulation
@param[in] frm_ctx      Context
@param[in] cls          Command class
@param[in] cmd          Command
@param[in] data         Data
@param[in] data_len     Data length
@return  0 on success, negative error number on failure
*/

int frm_node_id_get(frm_layer_ctx_t *frm_ctx, struct sockaddr *zip_gw, uint8_t *tgt_ip, int retry);
/**<
Resolve IPv6 address into node id using ZIP-ND
@param[in] frm_ctx  Context
@param[in] zip_gw   Z/IP gateway address
@param[in] tgt_ip   Target IPv6 address of the Z-wave node
@param[in] retry    Flag to indicate if this call is second attempt to get node id
@return  0 on success, negative error number on failure
*/

int frm_node_ipv6_get(frm_layer_ctx_t *frm_ctx, struct sockaddr *zip_gw, uint8_t node_id);
/**<
Resolve a node id into IPv6 address using ZIP-ND
@param[in] frm_ctx  Context
@param[in] zip_gw   Z/IP gateway address
@param[in] node_id  Z-wave node id
@return  0 on success, negative error number on failure
*/

int frm_send(frm_layer_ctx_t *frm_ctx, frm_snd_prm_t *prm);
/**<
Send a frame through transport layer
@param[in] frm_ctx      Context
@param[in] prm          Parameter
@return  0 on success, negative error number on failure
*/

int frm_init(frm_layer_ctx_t   *frm_ctx);
/**<
Init the frame layer.
Should be called once before calling the other frame layer functions
@param[in,out]	frm_ctx		Context
@return     Return zero indicates success, non-zero indicates failure.
@post       Caller should not modify the context after this call
*/

void frm_exit(frm_layer_ctx_t   *frm_ctx);
/**<
Clean up the frame layer
@param[in,out]	frm_ctx		Context
@return
*/

/**
@}
*/


#endif /* _ZIP_FRAME_DAVID_ */
