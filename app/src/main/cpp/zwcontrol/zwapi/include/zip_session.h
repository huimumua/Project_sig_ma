/**
@file   zip_session.h - Z/IP interface session layer implementation header file.

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
#ifndef _ZIP_SESSION_DAVID_
#define _ZIP_SESSION_DAVID_

#include "zip_frame.h"

/**
@defgroup Session Session layer APIs
To handle commands sending and the corresponding responses and/or callback.
@ingroup zwarecapi
@{
*/

//Forward declaration of session layer context
struct _ssn_layer_ctx;

typedef void    (*cmd_cb_t)(struct _ssn_layer_ctx *ssn_ctx, struct sockaddr *src_addr,
                            frm_ep_t *ep_addr, uint8_t *buf, uint16_t len, uint16_t flag, uint16_t encap_fmt);
/**<
command callback function
@param[in]	ssn_ctx	    context
@param[in]	src_addr    source address of the received frame
@param[in]	ep_addr     source and destination endpoint of the received frame
@param[in]	buf	        buffer to stored the received frame
@param[in]	len	        length of buf
@param[in]	flag	    flag, see ZIP_FLAG_XXX
@param[in]	encap_fmt   Z-wave encapsulation format (valid if flag ZIP_FLAG_ENCAP_INFO is set).
                        This format should be passed back when responding
                        to unsolicited command with the flag ZIP_FLAG_ENCAP_INFO is set.

@return
*/

typedef void (*snd_sts_cb_t)(struct _ssn_layer_ctx *ssn_ctx, int8_t tx_sts, void *user_prm, uint8_t node_id,
                              uint8_t seq_num, struct sockaddr *src_addr, uint8_t ima_rpt_len, uint8_t *ima_rpt);
/**<
send status callback function
@param[in]	ssn_ctx	    context
@param[in]	tx_sts	    transmit status, see SESSION_ERROR_XXX or zero if no error
@param[in]	user_prm	user-defined parameter used in callback
@param[in]	node_id	    original send packet destination node id. If 0xFF, node id is unknown.
@param[in]	seq_num	    Z/IP sequence number
@param[in]	src_addr	source address
@param[in]	ima_rpt_len	Z/IP header extension IMA report length
@param[in]	ima_rpt	    Buffer to store the Z/IP header extension IMA report; this field is valid only if ima_rpt_len > 0
@return
*/


///
/// Send command parameters
typedef struct
{
    struct sockaddr     *dst;       ///< destination address
    frm_ep_t            ep_addr;    ///< source and destination endpoint addresses
    uint8_t             *cmd_buf;   ///< command buffer
    uint16_t            cmd_len;    ///< length of command
    uint16_t            flag;       ///< flag, see ZIP_FLAG_XXX
    uint16_t            encap_fmt;  ///< Z-wave encapsulation format (valid if flag ZIP_FLAG_ENCAP_INFO is set).
    snd_sts_cb_t        sts_cb;     ///< send status callback function; null if no acknowledgement is required
    void                *cb_prm;    ///< send status callback function parameter
    uint8_t             node_id;    ///< send status callback to report destination node
    uint8_t             seq_num;    ///< sequence number of frame to the specific destination node
    uint8_t             wkup_poll;  ///< flag to indicate this frame is sent for the purpose of wakeup notification polling
} ssn_cmd_snd_param_t;

///
/// Callback request
typedef struct  _ssn_cb_req
{
    union
    {
        struct sockaddr_in  ipv4;
        struct sockaddr_in6 ipv6;
    } src_addr;                             ///< Source address of the sender
    frm_ep_t                ep_addr;        ///< Source and destination endpoint addresses
    uint16_t                len;            ///< Length of the data in the dat_buf
    uint16_t                flag;           ///< Flag, see ZIP_FLAG_XXX
    uint16_t                encap_fmt;      ///< Z-wave encapsulation format (valid if flag ZIP_FLAG_ENCAP_INFO is set)
    uint8_t                 dat_buf[1];     ///< Place holder for buffer to store the Z-wave command

} ssn_cb_req_t;


//Forward declaration of application layer context
struct _appl_layer_ctx;

///
/// Z-wave Serial API session layer context
typedef struct _ssn_layer_ctx
{
    //Callback functions to application layer
    cmd_cb_t        unsolicited_cmd_cb;   ///< Pointer to unsolicited command receiving callback function.
    snd_sts_cb_t    sts_cb;         ///< Send status callback function pointer
    snd_sts_cb_t    old_sts_cb;     ///< Old (non current) send status callback function pointer

    //Data
    struct plt_mtx_t    *wr_mtx;       ///< Mutex for sending command
    struct plt_mtx_t    *cb_thrd_mtx;  ///< Mutex for callback thread
    void                *cb_thrd_sem;  ///< Semaphore for waiting of callback requests
    void                *cb_prm;       ///< Send status callback parameter
    void                *old_cb_prm;   ///< Old (non current) send status callback parameter
    uint8_t             node_id;       ///< The original send packet destination node
    uint8_t             seq_num;       ///< sequence number of frame to the specific destination node
    util_lst_t          *cb_req_hd;    ///< head of linked list for callback requests
    volatile int        cb_thrd_run;   ///< Control the callback thread whether to run. 1 = run, 0 = stop
    volatile int        cb_thrd_sts;   ///< Callback thread status. 1 = run, 0 = thread exited
    frm_layer_ctx_t     frm_ctx;       ///< Frame layer context
    plt_ctx_t           *plt_ctx;      ///< Platform context
    struct _appl_layer_ctx  *appl_layer_ctx;    ///< Pointer to application layer context
} ssn_layer_ctx_t;

int ssn_cmd_snd(ssn_layer_ctx_t *ssn_ctx, ssn_cmd_snd_param_t *param);
/**<
Send a command
@param[in,out] ssn_ctx      Context.
@param[in] param            The parameters related to the command.
@return  0 on success, negative error number on failure
*/

int ssn_init(ssn_layer_ctx_t   *ssn_ctx);
/**<
Init the session layer.
Should be called once before calling the other session layer functions
@param[in,out]	ssn_ctx		Context
@return     Return zero indicates success, non-zero indicates failure.
@post       Caller should not modify the context after this call
*/

void ssn_exit(ssn_layer_ctx_t   *ssn_ctx);
/**<
Clean up the session layer
@param[in,out]	ssn_ctx		Context
@return
*/

void ssn_old_snd_status_cb_set(ssn_layer_ctx_t *ssn_ctx, snd_sts_cb_t sts_cb, void *usr_prm);
/**<
Set callback function to receive old (non current) send status
@param[in]	ssn_ctx     Context
@param[in]	sts_cb      Status callback function
@param[in]	usr_prm     Use-defined parameter when callback
@return
*/


/**
@}
*/

#endif /* _ZIP_SESSION_DAVID_ */
