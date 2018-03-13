/**
@file   zip_supervision.h - Set and poll supervision facility header file.

@author David Chow

@version    1.0 1-9-16  Initial release

@copyright © 2016 SIGMA DESIGNS, INC. THIS IS AN UNPUBLISHED WORK PROTECTED BY SIGMA DESIGNS, INC.
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

#ifndef _ZIP_SUPERVISION_DAVID_
#define _ZIP_SUPERVISION_DAVID_
/**
@defgroup If_Spv Supervision Interface APIs
Used to create and delete set and poll supervision commands to a device
@ingroup zwarecapi
@{
*/

#define SPV_TIMER_TICK             500                      /**< Periodic timer tick interval in ms */
#define SPV_TICK_PER_SEC           (1000/SPV_TIMER_TICK)    /**< Number of timer ticks per second */
#define SPV_RESP_TIMEOUT           (8 * SPV_TICK_PER_SEC)   /**< Timeout for waiting supervision report */
#define SPV_ADDTNL_TIMEOUT         (5 * SPV_TICK_PER_SEC)   /**< Additional timeout for waiting subsequent supervision report */
#define SPV_DEFAULT_TIMEOUT        (30 * SPV_TICK_PER_SEC)  /**< Default timeout for waiting subsequent supervision report */
#define SPV_MAX_CMD_LEN            40                       /**< Maximum command length*/

/** Event definition */
#define SPV_EVT_TYPE_TIMER         0       /**< Timer event*/
#define SPV_EVT_TYPE_ADD           1       /**< Add to supervision queue */
#define SPV_EVT_TYPE_REPORT        2       /**< Report arrival */
#define SPV_EVT_TYPE_RMV           3       /**< Remove entries of a node */
#define SPV_EVT_TYPE_RMV_ALL       4       /**< Remove all entries */

/** Supervision report status */
#define SPV_STATUS_NO_SUPPORT       0x00    /** Command is not supported by the receiver*/
#define SPV_STATUS_WORKING          0x01    /** Command was accepted by the receiver and processing has started*/
#define SPV_STATUS_FAIL             0x02    /** Command execution was accepted but failed for some reason */
#define SPV_STATUS_BUSY             0x03    /** The node is already busy supervising the operation of this or other commands*/
#define SPV_STATUS_SUCCESS          0xFF    /** The requested command has been completed and the application status is as the supervised command requested*/


/** Report buffer content type */
#define SUPERVSN_RPT_TYPE_GET   0   ///< Buffer content is report get command
#define SUPERVSN_RPT_TYPE_RPT   1   ///< Buffer content is simulated report


/** Polling queue entry*/
typedef struct _spv_q_ent
{
    struct _spv_q_ent   *next;        /**< Next entry */
    struct _spv_q_ent   *prev;        /**< Previous entry */
    zw_postset_fn       cb;           /**< Callback function*/
    void                *usr_param;   /**< User-defined parameter*/
	zwifd_t             ifd;	      /**< Interface associated with the command */
    uint16_t            tmout;        /**< timeout */
    uint8_t             ssn_id;       /**< Session id */
    uint8_t             rpt_buf_len;  /**< Length of rpt_cmd_buf in bytes */
    uint8_t             rpt_cmd_buf[SPV_MAX_CMD_LEN]; /**< Buffer to store the z-wave interface report command and parameters,
                                                           used when target is hit */
    int                 rpt_cmd_buf_type;/**<The content type in rpt_cmd_buf, SUPERVSN_RPT_TYPE_XXX */
}
spv_q_ent_t;

/** Add supervision event message*/
typedef struct
{
	zwifd_t         ifd;	        /**< Interface associated with the command */
    uint8_t         ssn_id;         /**< Session id */
    uint8_t         set_buf_len;    /**< Length of set_cmd_buf in bytes */
    uint8_t         set_cmd_buf[SPV_MAX_CMD_LEN]; /**< Buffer to store the z-wave interface set command and parameters */
    uint8_t         rpt_buf_len;    /**< Length of rpt_cmd_buf in bytes */
    uint8_t         rpt_cmd_buf[SPV_MAX_CMD_LEN]; /**< Buffer to store the z-wave interface report command and parameters,
                                                       used when target is hit */
    int             rpt_cmd_buf_type;/**<The content type in rpt_cmd_buf, SUPERVSN_RPT_TYPE_XXX */
    zw_postset_fn   cb;             /**< Callback function*/
    void            *usr_param;     /**< User-defined parameter*/
}
spv_ev_msg_add_t;

/** Report event message*/
typedef struct
{
	zwifd_t     ifd;	        /**< Interface associated with the command */
    uint8_t     ssn_id;         /**< Session id */
    uint8_t     more_rpt;       /**< More report to follow */
    uint8_t     status;         /**< Status.See SPV_STATUS_XXX */
    uint8_t     duration;       /**< Duration: 0 = already at the target; 0x01~0x7F = seconds; 0x80~0xFD = 1~126 minutes;
                                               0xFE = Unknown duration; 0xFF = reserved*/
}
spv_ev_msg_rpt_t;

/** Event message*/
typedef struct
{
    int         type;                   /**< Event type SPV_EVT_TYPE_XXX */
    union
    {
        spv_ev_msg_add_t  add;          /**< Add event message*/
        spv_ev_msg_rpt_t  rpt;          /**< Report arrival event message*/
        uint8_t           rm_node_id;   /**< Node id of the entries to be removed*/
    } ev_msg;
}
spv_evt_msg_t;


/** Supervision context */
typedef struct  _spv_ctx
{
    volatile int        run;                /**< Control whether to run post-set supervision facility. 1 = run, 0 = stop */
    volatile uint16_t   tmr_tick;           /**< Periodic timer tick, incremented every SPV_TIMER_TICK ms */
    void                *tick_tmr_ctx;      /**< Tick timer context */
    spv_q_ent_t       *spv_lst_hd;      /**< Head of double linked list for supervision requests */
    spv_q_ent_t       *spv_lst_tail;    /**< Tail of double linked list for supervision requests */
    util_msg_loop_t     *evt_msg_loop_ctx;  /**< Context of message loop for wakeup and report arrival events*/
    zwnet_p             net;                /**< Network */
}
zwspv_ctx_t;


int zwspv_init(zwspv_ctx_t *spv_ctx);
/**<
Initialize the supervision facility
@param[in]	spv_ctx	    supervision context
@return  0 on success; negative error number on failure
@note  Must call zwspv_shutdown followed by zwspv_exit to shutdown and clean up the supervision facility
*/

void zwspv_shutdown(zwspv_ctx_t *spv_ctx);
/**<
Shutdown the supervision facility
@param[in]	spv_ctx	    supervision context
@return
*/

void zwspv_exit(zwspv_ctx_t *spv_ctx);
/**<
Clean up
@param[in]	spv_ctx	    supervision context
@return
*/

int zwspv_on_rpt_rcv(zwif_p intf, uint8_t *cmd_buf, uint16_t cmd_len);
/**<
Callback when a report is received
@param[in]	intf	Interface
@param[in]	cmd_buf	The report command received
@param[in]	cmd_len	The length of cmd_buf
@return  Non-zero if the report is processed; else return zero
*/

int zwspv_add(zwifd_p ifd, uint8_t *set_cmd_buf, int set_buf_len, uint8_t *rpt_cmd_buf, int rpt_buf_len, int rpt_cmd_buf_type,
              uint8_t ssn_id, zw_postset_fn cb, void *usr_param);
/**<
Add a post-set supervision request into supervision queue
@param[in]	ifd	        interface
@param[in]	set_cmd_buf	command buffer which contains set command and parameters
@param[in]	set_buf_len	length of set_cmd_buf in bytes
@param[in]	rpt_cmd_buf	command buffer which contains report command and parameters, used when target is hit
@param[in]	rpt_buf_len	length of rpt_cmd_buf in bytes
@param[in]	rpt_cmd_buf_type	The content type in rpt_cmd_buf, SUPERVSN_RPT_TYPE_XXX
@param[in]	ssn_id	    session id
@param[in]	cb		    Optioanl post-set supervision callback function.
@param[in]	usr_param   Optional user-defined parameter passed in callback.
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

void zwspv_rm(zwspv_ctx_t *spv_ctx, uint8_t node_id);
/**<
Remove all post-set supervision requests of a node
@param[in]	spv_ctx	supervision context
@param[in]	node_id	    Node id
@return
*/

void zwspv_rm_all(zwspv_ctx_t *spv_ctx);
/**<
Remove all post-set supervision requests
@param[in]	spv_ctx	supervision context
@param[in]	node_id	    Node id
@return
*/

void zwspv_cb(zwifd_p ifd, zw_postset_fn cb, void *usr_param, int reason);
/**<
Create a callback request to the callback thread
@param[in]	ifd         interface descriptor
@param[in]	cb		    post-set supervision callback function.  NULL if no callback required.
@param[in]	usr_param   user-defined parameter passed in callback.
@param[in]  reason      reason of the callback, see ZWPSET_REASON_XXX
@return
*/


/**
@}
*/


#endif /* _ZIP_SUPERVISION_DAVID_ */

