/**
@file   zip_wkup_poll.h - Z-wave High Level API wake-up notification poll header file.

@author David Chow

@version    1.0 4-1-16  Initial release

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

#ifndef _ZIP_WKUP_POLL_DAVID_
#define _ZIP_WKUP_POLL_DAVID_

#define  RM_0_WKUP_INTV_ENTRY   //< Remove zero wakeup interval entry

/**
@defgroup If_WkupPoll  Wake-up Notification Polling Interface APIs
Used to create and delete wake-up notification polling commands to a device
@ingroup zwarecapi
@{
*/

#define WUPOLL_TIMER_TICK             1000                         /**< Periodic timer tick interval in ms */
#define WUPOLL_TICK_PER_SEC           (1000/WUPOLL_TIMER_TICK)     /**< Number of timer ticks per second */
#define WUPOLL_CHK_INACTIVE_INTV      (180 * WUPOLL_TICK_PER_SEC)  /**< Polling for inactive entries check interval
                                                                        in terms of timer tick */
#define WUPOLL_CHK_DOWN_INTV          (600 * WUPOLL_TICK_PER_SEC)  /**< Polling for down entries check interval
                                                                        in terms of timer tick */
#ifdef  RM_0_WKUP_INTV_ENTRY
#define WUPOLL_CHK_DOWN_0_INTERVAL    (25 * 3600 * WUPOLL_TICK_PER_SEC)    /**< Polling for down entries check with 0 wakeup interval */
#else
#define WUPOLL_CHK_DOWN_0_INTERVAL    (2 * WUPOLL_TICK_PER_SEC)    /**< Polling for down entries check with 0 wakeup interval */
#endif
#define WUPOLL_CHK_ACTIVE_INTV        (150 * WUPOLL_TICK_PER_SEC)  /**< Polling for active entries interval
                                                                        in terms of timer tick */
#define WUPOLL_ACTIVE_TMOUT_RESEND    (3 * WUPOLL_TICK_PER_SEC)    /**< Active polling state timeout resend in terms of timer tick */
#define WUPOLL_CHK_INIT_INTV          (10 * WUPOLL_TICK_PER_SEC)   /**< First add node initial check interval in terms of timer tick */
#define WUPOLL_TX_STATUS_TMOUT        (10 * WUPOLL_TICK_PER_SEC)   /**< Transmit status timeout in terms of timer tick */

#define WUPOLL_WAKEUP_NOTIFICATION    8                            /**< Wakeup notification received */

/** Event definition */
#define WUPOLL_EVT_TYPE_TIMER         0       /**< Timer event*/
#define WUPOLL_EVT_TYPE_ADD           1       /**< Add to polling queue event*/
#define WUPOLL_EVT_TYPE_RM            2       /**< Remove from polling queue event*/
#define WUPOLL_EVT_TYPE_STATUS        3       /**< NOP poll transmit status event*/
#define WUPOLL_EVT_TYPE_RM_ALL        4       /**< Remove all polling entries event*/

/** Status definition */
#define WUPOLL_STA_INACTIVE           0       /**< Polling is inactive*/
#define WUPOLL_STA_NODE_DOWN          1       /**< Node is down*/
#define WUPOLL_STA_WAIT_TX_STS        2       /**< Waiting transmit status */
#define WUPOLL_STA_ACTIVE             3       /**< Polling is active*/


/** Polling queue entry*/
typedef struct _wupoll_q_ent
{
    struct _wupoll_q_ent *next;         /**< Next entry */
    struct _wupoll_q_ent *prev;         /**< Previous entry */
    uint8_t              node_id;       /**< Node id*/
    uint8_t              state;         /**< State, see WUPOLL_STA_XXX */
    uint32_t             state_tmout;   /**< State timeout in terms of timer tick */
    int32_t              wkup_intv;     /**< Wake up interval in seconds. Negative value = invalid or unknown */
}
wupoll_q_ent_t;

/** Add polling event message*/
typedef struct
{
    uint8_t     node_id;        /**< Node id*/
    int32_t     wkup_intv;      /**< Wake up interval in seconds. Negative value = invalid or unknown */
}
wupoll_ev_msg_add_t;

/** Remove polling event message*/
typedef struct
{
    uint8_t     node_id;         /**< Node id*/
}
wupoll_ev_msg_rm_t;

/** Status event message*/
typedef struct
{
    uint8_t     node_id;         /**< Node id*/
    int8_t      tx_sts;          /**< Transmit status */
}
wupoll_ev_msg_sts_t;

/** Wakeup and report arrival event message*/
typedef struct
{
    int         type;                   /**< Event type WUPOLL_EVT_TYPE_XXX */
    union
    {
        wupoll_ev_msg_add_t  add;       /**< Add event message*/
        wupoll_ev_msg_rm_t   rm;        /**< Remove event message*/
        wupoll_ev_msg_sts_t  status;    /**< Report arrival event message*/
    } ev_msg;
}
wupoll_evt_msg_t;


/** Polling context */
typedef struct  _wupoll_ctx
{
    volatile int        run;                /**< Control whether to run post-set polling facility. 1 = run, 0 = stop */
    volatile uint32_t   tmr_tick;           /**< Periodic timer tick, incremented every WUPOLL_TIMER_TICK ms */
    void                *tick_tmr_ctx;      /**< Tick timer context */
    wupoll_q_ent_t      *wupoll_lst_hd;     /**< Head of double linked list for polling requests */
    wupoll_q_ent_t      *wupoll_lst_tail;   /**< Tail of double linked list for polling requests */
    util_msg_loop_t     *evt_msg_loop_ctx;  /**< Context of message loop for wakeup and report arrival events*/
    zwnet_p             net;                /**< Network */
}
wupoll_ctx_t;

int wkuppoll_init(wupoll_ctx_t *wupoll_ctx);
/**<
Initialize the polling facility
@param[in]	wupoll_ctx	    Polling context
@return  0 on success; negative error number on failure
@note  Must call wkuppoll_shutdown followed by wkuppoll_exit to shutdown and clean up the polling facility
*/

void wkuppoll_shutdown(wupoll_ctx_t *wupoll_ctx);
/**<
Shutdown the polling facility
@param[in]	wupoll_ctx	    Polling context
@return
*/

void wkuppoll_exit(wupoll_ctx_t *wupoll_ctx);
/**<
Clean up
@param[in]	wupoll_ctx	    Polling context
@return
*/


int wkuppoll_add(wupoll_ctx_t *wupoll_ctx, uint8_t node_id, int32_t wkup_intv);
/**<
Add a wake-up notification polling request into polling queue
@param[in]	wupoll_ctx	context
@param[in]	node_id	    node id
@param[in]	wkup_intv	Wake up interval in seconds. Negative value = invalid or unknown
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/


void wkuppoll_add_all(wupoll_ctx_t *wupoll_ctx);
/**<
Add all sleeping nodes for polling
@param[in]	wupoll_ctx	context
@return
*/

int wkuppoll_rm(wupoll_ctx_t *wupoll_ctx, uint8_t node_id);
/**
Remove a wake-up notification polling entry
@param[in]	wupoll_ctx	context
@param[in]	node_id	    node id
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

void wkuppoll_rm_all(wupoll_ctx_t *wupoll_ctx);
/**<
Remove all polling requests
@param[in]	wupoll_ctx    Context
@return
*/


/**
@}
*/


#endif /* _ZIP_WKUP_POLL_DAVID_ */

