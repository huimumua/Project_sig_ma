/**
@file   zip_node_sts.h - Z-wave High Level API sleeping node status polling header file.

@author David Chow

@version    1.0 20-2-17  Initial release

@copyright © 2017 SIGMA DESIGNS, INC. THIS IS AN UNPUBLISHED WORK PROTECTED BY SIGMA DESIGNS, INC.
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

#ifndef _ZIP_NODE_STATUS_DAVID_
#define _ZIP_NODE_STATUS_DAVID_
/**
@defgroup NodeStsPoll  Sleeping Node Status Polling APIs
Used to create and delete sleeping node status polling commands to a device
@ingroup zwarecapi
@{
*/

#define NODSTS_TIMER_TICK             1000          /**< Periodic timer tick interval in ms */
#define NODSTS_CHK_SHORT_INTV         20            /**< Node UP short check interval in seconds */
#define NODSTS_CHK_LONG_INTV          200           /**< Node UP long check interval in seconds */
#define NODSTS_DEFAULT_WKUP_INTV      (24 * 3600)   /**< Default wake up interval in seconds */
#define NODSTS_TX_STATUS_TMOUT        10            /**< Transmit status timeout in seconds */
#define NODSTS_WK_NOTIF_MARGIN        20            /**< Expected wakeup notification margin in seconds */
#define NODSTS_WK_NOTIF_TOO_EARLY     30            /**< Time threshold to determine wakeup notification arrives too early in seconds */


/** Event definition */
#define NODSTS_EVT_TYPE_TIMER         0       /**< Timer event*/
#define NODSTS_EVT_TYPE_ADD           1       /**< Add to polling queue event*/
#define NODSTS_EVT_TYPE_RM            2       /**< Remove from polling queue event*/
#define NODSTS_EVT_TYPE_TX_STS        3       /**< NOP poll transmit status event*/
#define NODSTS_EVT_TYPE_RM_ALL        4       /**< Remove all polling entries event*/
#define NODSTS_EVT_TYPE_WAKEUP        5       /**< Node wakeup event*/
#define NODSTS_EVT_TYPE_WKUP_INTV     6       /**< Wake up interval*/

/** State definition */
#define NODSTS_STA_UP                 0     ///< Node is alive
#define NODSTS_STA_DOWN               1     ///< Node is down
#define NODSTS_STA_SLEEP              2     ///< Node is sleeping
#define NODSTS_STA_WAIT_TX_STS        3     ///< Waiting transmit status


/** Polling queue entry*/
typedef struct _nodests_q_ent
{
    struct _nodests_q_ent *next;        /**< Next entry */
    struct _nodests_q_ent *prev;        /**< Previous entry */
    uint8_t               node_id;      /**< Node id*/
    uint8_t               state;        /**< State, see NODSTS_STA_XXX */
    uint32_t              state_tmout;  /**< State timeout in seconds */
    uint32_t              exp_wkup;     /**< Expected wakeup interval in seconds */
    int32_t               wkup_intv;    /**< Wake up interval in seconds. Negative value = invalid or unknown */
}
nodests_q_ent_t;


/** Add polling event message*/
typedef struct
{
    uint8_t     node_id;    /**< Node id*/
    uint8_t     state;      /**< Current state of the node, see NODSTS_STA_XXX */
    int32_t     wkup_intv;  /**< Wake up interval in seconds. Negative value = invalid or unknown */
}
nodests_ev_msg_add_t;

/** Remove polling event message*/
typedef struct
{
    uint8_t     node_id;         /**< Node id*/
}
nodests_ev_msg_rm_t;

/** Transmit status event message*/
typedef struct
{
    uint8_t     node_id;         /**< Node id*/
    int8_t      tx_sts;          /**< Transmit status */
}
nodests_ev_msg_sts_t;

/** Wakeup interval event message*/
typedef struct
{
    uint8_t     node_id;   /**< Node id*/
    uint32_t	wkup_intv; /**< Wakeup interval */
}
nodests_ev_msg_intv_t;

/** Wakeup and report arrival event message*/
typedef struct
{
    int         type;                   /**< Event type NODSTS_EVT_TYPE_XXX */
    union
    {
        nodests_ev_msg_add_t  add;       /**< Add event message*/
        nodests_ev_msg_rm_t   rm;        /**< Remove event message*/
        nodests_ev_msg_sts_t  status;    /**< Report arrival event message*/
        zwnoded_t             node;      /**< Node */
        nodests_ev_msg_intv_t wk_intv;   /**< Wakeup interval event message*/
    } ev_msg;
}
nodests_evt_msg_t;


/** Node status polling context */
typedef struct  _nodests_ctx
{
    volatile int        run;                /**< Control whether to run polling facility. 1 = run, 0 = stop */
    volatile uint32_t   tmr_tick;           /**< Periodic timer tick, incremented every NODSTS_TIMER_TICK ms */
    void                *tick_tmr_ctx;      /**< Tick timer context */
    nodests_q_ent_t     *nodests_lst_hd;    /**< Head of double linked list for polling requests */
    nodests_q_ent_t     *nodests_lst_tail;  /**< Tail of double linked list for polling requests */
    util_msg_loop_t     *evt_msg_loop_ctx;  /**< Context of message loop for wakeup and report arrival events*/
    zwnet_p             net;                /**< Network */
}
nodests_ctx_t;


int nodests_init(nodests_ctx_t *nodests_ctx);
/**<
Initialize the node status polling facility
@param[in]	nodests_ctx	    Polling context
@return  0 on success; negative error number on failure
@note  Must call nodests_shutdown followed by nodests_exit to shutdown and clean up the polling facility
*/

void nodests_shutdown(nodests_ctx_t *nodests_ctx);
/**<
Shutdown the polling facility
@param[in]	nodests_ctx	    Polling context
@return
*/

void nodests_exit(nodests_ctx_t *nodests_ctx);
/**<
Clean up
@param[in]	nodests_ctx	    Polling context
@return
*/

void nodests_on_wkup(nodests_ctx_t *nodests_ctx, zwnoded_p noded);
/**<
Callback when a sleeping node sends wake up notification
@param[in]	dpoll_ctx   Polling context
@param[in]	noded	    Node
@return
*/

int nodests_add(nodests_ctx_t *nodests_ctx, uint8_t node_id, uint8_t state, int32_t wkup_intv);
/**<
Add a node status polling request into polling queue
@param[in]	nodests_ctx	context
@param[in]	node_id	    node id
@param[in]	state	    current state of the node, see NODSTS_STA_XXX. Only NODSTS_STA_UP and NODSTS_STA_SLEEP are supported
@param[in]	wkup_intv	Wake up interval in seconds. Negative value = invalid or unknown
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

void nodests_add_all(nodests_ctx_t *nodests_ctx);
/**<
Add all sleeping nodes for polling
@param[in]	nodests_ctx	context
@return
*/

int nodests_rm(nodests_ctx_t *nodests_ctx, uint8_t node_id);
/**
Remove a node status polling entry
@param[in]	nodests_ctx	context
@param[in]	node_id	    node id
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

void nodests_rm_all(nodests_ctx_t *nodests_ctx);
/**<
Remove all polling entries
@param[in]	nodests_ctx    Context
@return
*/

/**
@}
*/

#endif /* _ZIP_NODE_STATUS_DAVID_ */

