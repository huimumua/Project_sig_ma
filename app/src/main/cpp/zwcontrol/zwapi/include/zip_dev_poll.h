/**
@file   zip_dev_poll.h - Z-wave High Level API device poll facility header file.

@author David Chow

@version    1.0 29-9-15  Initial release

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

#ifndef _ZIP_DEV_POLL_DAVID_
#define _ZIP_DEV_POLL_DAVID_
/**
@defgroup If_DevPoll Device Polling Interface APIs
Used to create and delete device polling
@ingroup zwarecapi
@{
*/

#define DEV_POLL_TIMER_TICK             500                         /**< Periodic timer tick interval in ms */
#define DEV_POLL_TICK_PER_SEC           (1000/DEV_POLL_TIMER_TICK)  /**< Number of timer ticks per second */
#define AO_DEV_POLL_TIME                (10 * DEV_POLL_TICK_PER_SEC)/**< Always-on device polling time in terms of timer tick */
#define FLIRS_DEV_POLL_TIME             (12 * 3600 * DEV_POLL_TICK_PER_SEC) /**< FLIRS device polling time in terms of timer tick */

/** Event definition */
#define DEV_POLL_EVT_ADD_NODE           0       /**< Add node event*/
#define DEV_POLL_EVT_RM_NODE            1       /**< Remove node event*/
#define DEV_POLL_EVT_RM_ALL             2       /**< Remove all node event*/
#define DEV_POLL_EVT_WAKEUP             3       /**< Node wakeup event*/
#define DEV_POLL_EVT_OP_CMPLT           4       /**< Node cache update operation complete event*/

/** Device type */
#define DEV_POLL_DEV_NONE               0       /**< None*/
#define DEV_POLL_DEV_SLEEPING           1       /**< Sleeping device */
#define DEV_POLL_DEV_FLIRS              2       /**< FLIRS device */
#define DEV_POLL_DEV_AO                 3       /**< Always-on device */


/** Event message*/
typedef struct
{
    int         type;           /**< Event type DEV_POLL_EVT_XXX */
    zwnoded_t   noded;          /**< Node descriptor */
}
dpoll_evt_msg_t;


/** FLIRS poll entry*/
typedef struct
{
    uint32_t    next_poll_tm;   /**< Next polling time */
    zwnoded_t   noded;          /**< Node descriptor */
}
dpoll_flirs_t;


/** Device polling context */
typedef struct  _zwdpoll_ctx
{
    volatile int        run;                /**< Control whether to run device polling facility. 1 = run, 0 = stop */
    volatile uint32_t   tmr_tick;           /**< Periodic timer tick, incremented every DEV_POLL_TIMER_TICK ms */
    volatile int        tmr_chk_thrd_run;   /**< Control the timer check thread whether to run. 1 = run, 0 = stop */
    volatile int        tmr_chk_thrd_sts;   /**< Timer check thread status. 1 = running, 0 = thread exited */
    void                *tmr_sem;           /**< Semaphore for waiting timer tick event */
    struct plt_mtx_t    *poll_mtx;          /**< Mutex for the polling facility */
    uint32_t            next_poll_tm;       /**< Next polling time */
    util_lst_t          *priority_hd;       /**< Head of linked list for priority queue */
    util_lst_t          *flirs_hd;          /**< Head of linked list for FLIRS device queue */
    util_lst_t          *ao_hd;             /**< Head of linked list for always-on device queue */
    void                *tick_tmr_ctx;      /**< Tick timer context */
    util_msg_loop_t     *evt_msg_loop_ctx;  /**< Context of message loop for wakeup and report arrival events*/
    zwnet_p             net;                /**< Network */
    int                 cur_device_type;    /**< Current device type DEV_POLL_DEV_XXX */
	uint8_t		        cur_ao_nodeid;	    /**< Current polling always-on device node id */
}
zwdpoll_ctx_t;


int zwdpoll_init(zwdpoll_ctx_t *dpoll_ctx);
/**<
Initialize the polling facility
@param[in]	dpoll_ctx	    Polling context. The content must be cleared except the member net must point to network context.
@return  0 on success; negative error number on failure
@note  Must call zwdpoll_shutdown followed by zwdpoll_exit to shutdown and clean up the polling facility
*/

void zwdpoll_shutdown(zwdpoll_ctx_t *dpoll_ctx);
/**<
Shutdown the polling facility
@param[in]	dpoll_ctx	    Polling context
@return
*/

void zwdpoll_exit(zwdpoll_ctx_t *dpoll_ctx);
/**<
Clean up
@param[in]	dpoll_ctx	    Polling context
@return
*/

void zwdpoll_add_all(zwdpoll_ctx_t *dpoll_ctx);
/**<
Add all nodes except sleeping nodes for polling
@param[in]	dpoll_ctx   Polling context
@param[in]	noded	    Node
return
*/

void zwdpoll_add_node(zwdpoll_ctx_t *dpoll_ctx, zwnoded_p noded);
/**<
Add node for polling
@param[in]	dpoll_ctx   Polling context
@param[in]	noded	    Node
return
*/

void zwdpoll_rm_node(zwdpoll_ctx_t *dpoll_ctx, zwnoded_p noded);
/**<
Remove node from polling
@param[in]	dpoll_ctx   Polling context
@param[in]	noded	    Node
return
*/

void zwdpoll_rm_all(zwdpoll_ctx_t *dpoll_ctx);
/**<
zwdpoll_rm_all - Remove all nodes from polling
@param[in]	dpoll_ctx   Polling context
return
*/

void zwdpoll_on_wkup(zwdpoll_ctx_t *dpoll_ctx, zwnoded_p noded);
/**<
Callback when a sleeping node sends wake up notification and there are commands in the queue
@param[in]	dpoll_ctx   Polling context
@param[in]	noded	    Node
@return
*/

void zwdpoll_on_complt(zwdpoll_ctx_t *dpoll_ctx, zwnoded_p noded);
/**<
Callback when node cache update operation has completed
@param[in]	dpoll_ctx   Polling context
@param[in]	noded	    Node
@return
*/


/**
@}
*/


#endif /* _ZIP_DEV_DEV_POLL_DAVID_ */

