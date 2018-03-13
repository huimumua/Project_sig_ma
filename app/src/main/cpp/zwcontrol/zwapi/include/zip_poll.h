/**
@file   zip_poll.h - Z-wave High Level API poll facility header file.

@author David Chow

@version    1.0 26-3-13  Initial release

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

#ifndef _ZIP_POLL_DAVID_
#define _ZIP_POLL_DAVID_
/**
@defgroup If_Poll Polling Interface APIs
Used to create and delete polling commands to a device
@ingroup zwarecapi
@{
*/

#define POLL_TIMER_TICK             500                         /**< Periodic timer tick interval in ms */
#define POLL_TICK_PER_SEC           (1000/POLL_TIMER_TICK)      /**< Number of timer ticks per second */
#define MIN_POLL_TIME               (10 * POLL_TICK_PER_SEC)    /**< Minimum polling time in terms of timer tick */
#define CHECK_EXPIRY_INTERVAL       (1 * POLL_TICK_PER_SEC)     /**< Check for polling entries expiry interval
                                                                     in terms of timer tick */
#define POLL_RPT_TIMEOUT            (10 * POLL_TICK_PER_SEC)    /**< Poll report timeout */

/** Event definition */
#define POLL_EVT_TYPE_WAKEUP        0       /**< Wakeup event*/
#define POLL_EVT_TYPE_REPORT        1       /**< Report arrival event*/

/** Poll entry active status */
#define POLL_STS_INACTIVE_WAIT_RPT  1       /**< inactive and waiting for report to complete the poll process*/
#define POLL_STS_ACTIVE             2       /**< active and ready to poll*/

/** Wakeup and report arrival event message*/
typedef struct
{
    int         type;           /**< Event type POLL_EVT_TYPE_XXX */
    uint8_t     nodeid;         /**< Node ID */
    uint8_t     rpt;            /**< Report*/
    uint16_t    cls;            /**< Command class*/
}
poll_evt_msg_t;


/** Callback request message*/
typedef struct
{
    zwpoll_cmplt_fn cmplt_cb;       /**< Polling completion callback*/
    void            *usr_param;     /**< User parameter*/
    uint32_t        usr_token;      /**< User defined token to facilitate deletion of multiple polling requests */
    uint16_t        handle;         /**< Polling request handle*/
}
poll_cb_msg_t;


/** Polling queue entry*/
typedef struct _poll_q_ent
{
    struct _poll_q_ent *next;   /**< Next entry */
    struct _poll_q_ent *prev;   /**< Previous entry */
	zwifd_t     ifd;	        /**< Interface associated with the command */
    uint32_t    next_poll_tm;   /**< Next polling time */
    uint32_t    rpt_tout;       /**< Report timeout. Only valid for active_sts is POLL_STS_INACTIVE_WAIT_RPT */
    uint32_t    slp_node_tout;  /**< Sleeping node timeout. Only valid for sleeping==1 */
    uint32_t    usr_token;      /**< User defined token to facilitate deletion of multiple polling requests */
    uint32_t    interval;       /**< Polling interval in terms of timer tick*/
    zwpoll_cmplt_fn cmplt_cb;   /**< Polling completion callback. NULL if callback is not required*/
    void        *usr_param;     /**< User parameter of polling completion callback */
    uint16_t    active_sts;     /**< Active status, POLL_STS_XXX */
    uint16_t    expired;        /**< Flag to indicate the polling time is expired and ready to send poll command */
    uint16_t    sleeping;       /**< Flag to indicate the node is capable to sleep */
    uint16_t    wr_cmd_q;       /**< Flag to indicate the polling command has been written to sleeping node command queue */
    uint16_t    wait_rpt;       /**< Flag to indicate the node is waiting for report */
    uint16_t    poll_seq_num;   /**< Poll sequence number to avoid polling stuck at one particular node */
    uint16_t    poll_cnt;       /**< Number of times to poll; zero = unlimited times (repetitive poll)*/
    uint16_t    tot_poll_cnt;   /**< Number of polls being executed */
    uint16_t    handle;         /**< Polling request handle*/
    uint16_t    cmd_cls;        /**< Expected command class of the report*/
    uint8_t     rpt;            /**< Expected report command of the report*/
    uint8_t     node_id;        /**< Node id*/
    uint8_t     dat_len;        /**< Length of the dat_buf field */
    uint8_t     rsvd;           /**< Padding to make sure dat_buf is 4-byte aligned */
    uint8_t     dat_buf[1];     /**< Place holder for the data buffer */
}
poll_q_ent_t;


/** Polling context */
typedef struct  _poll_ctx
{
    volatile uint32_t   tmr_tick;           /**< Periodic timer tick, incremented every POLL_TIMER_TICK ms */
    volatile int        run;                /**< Control whether to run polling facility. 1 = run, 0 = stop */
    volatile int        tmr_chk_thrd_run;   /**< Control the timer check thread whether to run. 1 = run, 0 = stop */
    volatile int        tmr_chk_thrd_sts;   /**< Timer check thread status. 1 = running, 0 = thread exited */
    uint32_t            next_poll_tm;       /**< Next polling time */
    void                *tmr_sem;           /**< Semaphore for waiting timer tick event */
    struct plt_mtx_t    *poll_mtx;          /**< Mutex for the polling facility */
    void                *tick_tmr_ctx;      /**< Tick timer context */
    poll_q_ent_t        *poll_lst_hd;       /**< Head of double linked list for polling requests */
    poll_q_ent_t        *poll_lst_tail;     /**< Tail of double linked list for polling requests */
    util_msg_loop_t     *cb_msg_loop_ctx;   /**< Context of message loop for callback*/
    util_msg_loop_t     *evt_msg_loop_ctx;  /**< Context of message loop for wakeup and report arrival events*/
    zwnet_p             net;                /**< Network */
    uint16_t            handle_gen;         /**< Handle number generator */
    uint16_t            poll_seq_num;       /**< Poll sequence number to avoid polling stuck at one particular node */
    uint32_t            cur_start_tm;       /**< Start time of the current poll*/
    uint32_t            cur_cmd_tm;         /**< Command time of the current poll*/
    uint16_t            cur_cmd_cls;        /**< Expected command class of the report*/
    uint8_t             cur_rpt;            /**< Expected report command of the report*/
    uint8_t             cur_node_id;        /**< Node id of the current poll*/
}
zwpoll_ctx_t;

int zwpoll_init(zwpoll_ctx_t *poll_ctx);
/**<
Initialize the polling facility
@param[in]	poll_ctx	    Polling context
@return  0 on success; negative error number on failure
@note  Must call zwpoll_shutdown followed by zwpoll_exit to shutdown and clean up the polling facility
*/

void zwpoll_shutdown(zwpoll_ctx_t *poll_ctx);
/**<
Shutdown the polling facility
@param[in]	poll_ctx	    Polling context
@return
*/

void zwpoll_exit(zwpoll_ctx_t *poll_ctx);
/**<
Clean up
@param[in]	poll_ctx	    Polling context
@return
*/

void zwpoll_beam_tx_cb(appl_layer_ctx_t  *appl_ctx, int8_t tx_sts, void *user_prm, uint8_t node_id, zw_tx_cb_prm_t *param);
/**<
send command to node that requires wake up beam (FLIRS) callback function
@param[in]	appl_ctx    The application layer context
@param[in]	tx_sts		The transmit complete status
@param[in]	user_prm    The user specific parameter
@param[in]	node_id     Node id
@param[in]	param       Additional parameters
@return
*/

void zwpoll_on_rpt_rcv(zwif_p intf, uint8_t *cmd_buf, uint16_t cmd_len);
/**<
zwpoll_on_rpt_rcv - Callback when a report is received
@param[in]	intf	Interface
@param[in]	cmd_buf	The report command received
@param[in]	cmd_len	The length of cmd_buf
@return
*/

void zwpoll_on_wkup(zwnode_p node);
/**<
zwpoll_on_wkup - Callback when a sleeping node sends wake up notification and there are commands in the queue
@param[in]	node	Node
@return
*/

int zwpoll_rm(zwnet_p net, uint16_t handle);
/**<
remove a polling request
@param[in]	net	        network
@param[in]	handle	    handle of the polling request to remove
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

int zwpoll_rm_mul(zwnet_p net, uint32_t usr_token);
/**<
remove multiple polling requests
@param[in]	net	        network
@param[in]	usr_token	usr_token of the polling requests to remove
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

int zwpoll_add(zwnet_p net, poll_q_ent_t *poll_ent, uint16_t *handle);
/**<
add a polling request into polling queue
@param[in]	net	        network
@param[in]	poll_ent	polling request entry (allocated by caller). All unused fields must be zeroed.
@param[out]	handle	    Polling request handle
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
@post       If return error, caller must free poll_ent
*/


/**
@}
*/


#endif /* _ZIP_POLL_DAVID_ */

