/**
@file   zip_event_gen.h - Z-wave High Level API event generation facility header file.

@author David Chow

@version    1.0 10-10-16  Initial release

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

#ifndef _ZIP_EVT_GEN_DAVID_
#define _ZIP_EVT_GEN_DAVID_
/**
@defgroup If_Evt_Gen Event Generation APIs
Used to generate event to interface
@ingroup zwarecapi
@{
*/

#define EVTG_TIMER_TICK             500                         /**< Periodic timer tick interval in ms */
#define EVTG_TICK_PER_SEC           (1000/EVTG_TIMER_TICK)      /**< Number of timer ticks per second */
#define EVTG_MAX_RPT_LEN            42                          /**< Maximum report length */

/** Event definition */
#define EVTG_EVT_TYPE_TIMER         0       /**< Timer event*/
#define EVTG_EVT_TYPE_ADD           1       /**< Add to event generation queue */
#define EVTG_EVT_TYPE_RMV           2       /**< Remove entries of a node */
#define EVTG_EVT_TYPE_RMV_ALL       3       /**< Remove all entries */

/** Binary sensor report parameters*/
typedef struct
{
    uint8_t     state;      /**< 0=idle, else event detected*/
    uint8_t     type;       /**< sensor type, ZW_BSENSOR_TYPE_XXX. If type equals to zero, sensor type is unknown*/
}
evtg_bsnsr_rpt_t;

/** Event generation queue entry*/
typedef struct _evtg_q_ent
{
    struct _evtg_q_ent *next;                   /**< Next entry */
    struct _evtg_q_ent *prev;                   /**< Previous entry */
    zwifd_t             ifd;                    /**< Interface associated with the command */
    uint8_t             rpt[EVTG_MAX_RPT_LEN];  /**< Report*/
    uint16_t            rpt_len;                /**< Report length*/
    uint16_t            tmout;                  /**< Event generation time out */
}
evtg_q_ent_t;

/** Add event generation event message*/
typedef struct
{
    zwifd_t         ifd;                    /**< Interface */
    uint8_t         rpt[EVTG_MAX_RPT_LEN];  /**< Report*/
    uint16_t        rpt_len;                /**< Report length*/
    uint16_t        tmout;                  /**< Event generation time out in timer ticks */
}
evtg_ev_msg_add_t;

/** Event message*/
typedef struct
{
    int         type;           /**< Event type EVTG_EVT_TYPE_XXX */
    union
    {
        evtg_ev_msg_add_t  add;         /**< Add event message*/
        uint8_t            rm_node_id;  /**< Node id of the entries to be removed*/
    } ev_msg;
}
evtg_evt_msg_t;


/** Event generation context */
typedef struct  _evtg_ctx
{
    volatile int        run;                /**< Control whether to run event generation facility. 1 = run, 0 = stop */
    volatile uint16_t   tmr_tick;           /**< Periodic timer tick, incremented every EVTG_TIMER_TICK ms */
    void                *tick_tmr_ctx;      /**< Tick timer context */
    evtg_q_ent_t        *evtg_lst_hd;       /**< Head of double linked list for event generation requests */
    evtg_q_ent_t        *evtg_lst_tail;     /**< Tail of double linked list for event generation requests */
    util_msg_loop_t     *evt_msg_loop_ctx;  /**< Context of message loop for events*/
    zwnet_p             net;                /**< Network */
}
zwevtg_ctx_t;

int zwevtg_init(zwevtg_ctx_t *evtg_ctx);
/**<
Initialize the event generation facility
@param[in]	evtg_ctx	    Event generation context
@return  0 on success; negative error number on failure
@note  Must call zwevtg_shutdown followed by zwevtg_exit to shutdown and clean up the event generation facility
*/

void zwevtg_shutdown(zwevtg_ctx_t *evtg_ctx);
/**<
Shutdown the event generation facility
@param[in]	evtg_ctx	    Event generation context
@return
*/

void zwevtg_exit(zwevtg_ctx_t *evtg_ctx);
/**<
Clean up
@param[in]	evtg_ctx	    Event generation context
@return
*/

int zwevtg_add(zwifd_p ifd, uint8_t *rpt, uint16_t rpt_len, uint16_t tmout_s);
/**<
Add an event generation request into event generation queue
@param[in]	ifd	    interface
@param[in]	rpt     report for callback when event generation time out occurs
@param[in]	rpt_len report length
@param[in]	tmout   time out in seconds (max: 3600 s)
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

void zwevtg_rm(zwevtg_ctx_t *evtg_ctx, uint8_t node_id);
/**<
Remove all event generation requests of a node
@param[in]	evtg_ctx	Event generation context
@param[in]	node_id	    Node id
@return
*/

void zwevtg_rm_all(zwevtg_ctx_t *evtg_ctx);
/**<
Remove all event generation requests
@param[in]	evtg_ctx	Event generation context
@param[in]	node_id	    Node id
@return
*/


/**
@}
*/


#endif /* _ZIP_EVT_GEN_DAVID_ */

