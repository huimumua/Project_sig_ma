/**
@file   zip_set_poll.h - Z-wave High Level API post-set poll facility header file.

@author David Chow

@version    1.0 9-9-15  Initial release

@copyright © 2015 SIGMA DESIGNS, INC. THIS IS AN UNPUBLISHED WORK PROTECTED BY SIGMA DESIGNS, INC.
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

#ifndef _ZIP_SET_POLL_DAVID_
#define _ZIP_SET_POLL_DAVID_
/**
@defgroup If_Poll Polling Interface APIs
Used to create and delete polling commands to a device
@ingroup zwarecapi
@{
*/

#define SPOLL_TIMER_TICK             500                         /**< Periodic timer tick interval in ms */
#define SPOLL_TICK_PER_SEC           (1000/SPOLL_TIMER_TICK)     /**< Number of timer ticks per second */
#define SPOLL_WAIT_REPORT_TIME       (3 * SPOLL_TICK_PER_SEC)    /**< Time to wait for a report
                                                                      in terms of timer tick */
#define SPOLL_TIMEOUT_CHECK          3                           /**< Time in seconds to wait before checking for timeout */

#define SPOLL_DOORLOCK_MAX_COUNT     3                           /**< Maximum poll count for door lock before timeout */
#define SPOLL_MOTOR_MAX_COUNT        4                           /**< Maximum poll count for motor before timeout */
#define SPOLL_BARRIER_OPRT_MAX_COUNT 7                           /**< Maximum poll count for barrier operator before timeout */
#define SPOLL_INVALID_MAX_COUNT      0                           /**< Invalid maximum poll count */

/** Event definition */
#define SPOLL_EVT_TYPE_TIMER         0       /**< Timer event*/
#define SPOLL_EVT_TYPE_ADD           1       /**< Add to polling queue */
#define SPOLL_EVT_TYPE_REPORT        2       /**< Report arrival */
#define SPOLL_EVT_TYPE_RMV           3       /**< Remove entries of a node */
#define SPOLL_EVT_TYPE_RMV_ALL       4       /**< Remove all entries */


/** Polling queue entry*/
typedef struct _spoll_q_ent
{
    struct _spoll_q_ent *next;      /**< Next entry */
    struct _spoll_q_ent *prev;      /**< Previous entry */
    zw_postset_fn       cb;         /**< Callback function*/
    void                *usr_param; /**< User-defined parameter*/
	zwifd_t     ifd;	            /**< Interface associated with the command */
    uint16_t    last_rpt_dur;       /**< Last reported estimated duration from the device */
    uint16_t    next_spoll_tm;      /**< Next polling time */
    uint16_t    max_spoll_cnt;      /**< Max number of times to poll */
    uint16_t    cur_spoll_cnt;      /**< Current poll count*/
    uint8_t     final_tmout;        /**< Flag to indicate the coming timeout is the final timeout*/
    uint8_t     target;             /**< Expected target value of the report*/
    uint8_t     rpt_get_len;        /**< Length of the rpt_get_buf field */
    uint8_t     rpt_get_buf[1];     /**< Place holder for the buffer to store the command for report get and its parameters */
}
spoll_q_ent_t;

/** Add polling event message*/
typedef struct
{
	zwifd_t         ifd;	        /**< Interface associated with the command */
    uint8_t         target;         /**< Expected target value of the report*/
    uint8_t         rpt_get_len;    /**< Length of the rpt_get_buf field */
    uint8_t         rpt_get_buf[8]; /**< Buffer to store the command for report get and its parameters */
    zw_postset_fn   cb;             /**< Callback function*/
    void            *usr_param;     /**< User-defined parameter*/
}
spoll_ev_msg_add_t;

/** Report event message*/
typedef struct
{
	zwifd_t     ifd;	        /**< Interface associated with the command */
    uint8_t     value;          /**< Report value */
    uint8_t     dur_valid;      /**< Flag to indicate whether the "dur" is valid */
    uint16_t    dur;            /**< Expected time in seconds to reach the target*/
}
spoll_ev_msg_rpt_t;

/** Wakeup and report arrival event message*/
typedef struct
{
    int         type;                   /**< Event type SPOLL_EVT_TYPE_XXX */
    union
    {
        spoll_ev_msg_add_t  add;        /**< Add event message*/
        spoll_ev_msg_rpt_t  rpt;        /**< Report arrival event message*/
        uint8_t             rm_node_id; /**< Node id of the entries to be removed*/
    } ev_msg;
}
spoll_evt_msg_t;


/** Polling context */
typedef struct  _spoll_ctx
{
    volatile int        run;                /**< Control whether to run post-set polling facility. 1 = run, 0 = stop */
    volatile uint16_t   tmr_tick;           /**< Periodic timer tick, incremented every SPOLL_TIMER_TICK ms */
    void                *tick_tmr_ctx;      /**< Tick timer context */
    spoll_q_ent_t       *spoll_lst_hd;      /**< Head of double linked list for polling requests */
    spoll_q_ent_t       *spoll_lst_tail;    /**< Tail of double linked list for polling requests */
    util_msg_loop_t     *evt_msg_loop_ctx;  /**< Context of message loop for wakeup and report arrival events*/
    zwnet_p             net;                /**< Network */
}
zwspoll_ctx_t;

int zwspoll_init(zwspoll_ctx_t *spoll_ctx);
/**<
Initialize the polling facility
@param[in]	spoll_ctx	    Polling context
@return  0 on success; negative error number on failure
@note  Must call zwspoll_shutdown followed by zwspoll_exit to shutdown and clean up the polling facility
*/

void zwspoll_shutdown(zwspoll_ctx_t *spoll_ctx);
/**<
Shutdown the polling facility
@param[in]	spoll_ctx	    Polling context
@return
*/

void zwspoll_exit(zwspoll_ctx_t *spoll_ctx);
/**<
Clean up
@param[in]	spoll_ctx	    Polling context
@return
*/

void zwspoll_on_rpt_rcv(zwif_p intf, uint8_t *cmd_buf, uint16_t cmd_len);
/**<
Callback when a report is received
@param[in]	intf	Interface
@param[in]	cmd_buf	The report command received
@param[in]	cmd_len	The length of cmd_buf
@return
*/

int zwspoll_add(zwifd_p ifd, uint8_t target, uint8_t *rpt_get_buf, uint8_t rpt_get_len, zw_postset_fn cb, void *usr_param);
/**<
Add a post-set polling request into polling queue
@param[in]	ifd	        interface
@param[in]	target	    expected report target value
@param[in]	rpt_get_buf	buffer to store the command for report get and its parameters
@param[in]	rpt_get_len	length of the rpt_get_buf field
@param[in]	cb		    Optional post-set polling callback function.  NULL if no callback required.
@param[in]	usr_param   Optional user-defined parameter passed in callback.
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

void zwspoll_rm(zwspoll_ctx_t *spoll_ctx, uint8_t node_id);
/**<
Remove all post-set polling requests of a node
@param[in]	spoll_ctx	Polling context
@param[in]	node_id	    Node id
@return
*/

void zwspoll_rm_all(zwspoll_ctx_t *spoll_ctx);
/**<
Remove all post-set polling requests
@param[in]	spoll_ctx	Polling context
@param[in]	node_id	    Node id
@return
*/

void zwspoll_cb(zwifd_p ifd, zw_postset_fn cb, void *usr_param, int reason);
/**<
Create a callback request to the callback thread
@param[in]	ifd         interface descriptor
@param[in]	cb		    post-set polling callback function.  NULL if no callback required.
@param[in]	usr_param   user-defined parameter passed in callback.
@param[in]  reason      reason of the callback, see ZWPSET_REASON_XXX
@return
*/


/**
@}
*/


#endif /* _ZIP_SET_POLL_DAVID_ */

