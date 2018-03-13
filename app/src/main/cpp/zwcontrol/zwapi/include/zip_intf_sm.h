/**
@file   zip_intf_sm.h - Z/IP Interface state-machines header file.

@author David Chow

@version    1.0 16-8-2017  Initial release

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

#ifndef _ZIP_INTF_SM_DAVID_
#define _ZIP_INTF_SM_DAVID_

/**
@defgroup Interface State-machine APIs
Implementation of interface state-machines
@ingroup zwarecapi
@{
*/

/** Configuration state-machine events */
typedef enum
{
    EVT_CONFIG_TMOUT,          ///< Timeout
    EVT_CONFIG_START,          ///< Start state-machine
    EVT_CONFIG_BULK_RPT,       ///< Received config bulk report
    EVT_CONFIG_TX_STS,         ///< Received transmit status
    EVT_CONFIG_SHUTDOWN        ///< Shutdown state-machine

} ifsm_config_evt_t;

/** Configuration state-machine states */
typedef enum
{
    CONFIG_STA_IDLE,            ///< Idle
    CONFIG_STA_WAIT_BULK_RPT,   ///< Waiting for config bulk report
    CONFIG_STA_WAIT_TX_STS,     ///< Waiting for transmit status
    CONFIG_STA_SHUTDOWN         ///< Shutdown

} ifsm_config_sta_t;


/** Configuration state-machine context */
typedef struct
{
    ifsm_config_sta_t   sta;        /**< state*/
    void                *tmr_ctx;   /**< State machine timer context*/
    zwifd_t             ifd;        /**< interface*/
    uint8_t             retry_cnt;  /**< retry count*/
} cfg_sm_ctx_t;


void ifsm_config_bulk_cb(zwifd_p ifd, zwcfg_bulk_p param);
/**<
report callback for multiple configuration parameters
@param[in]	ifd	    interface
@param[in]	param   parameter value
*/

int ifsm_config_sm(zwnet_p nw, ifsm_config_evt_t evt, uint8_t *data);
/**<
Configuration state-machine
@param[in] nw		Network
@param[in] evt      The event for the state-machine
@param[in] data     The data associated with the event
@return   Non-zero when the state-machine is started from idle state; otherwise return zero
@pre        Caller must lock the nw->mtx before calling this function.
*/


/** Interface state-machines context */
typedef struct _ifsm_ctx
{
    zwnet_p         nw;         /**< Network context*/
    cfg_sm_ctx_t    cfg_ctx;    /**< Configuration state-machine context*/
} ifsm_ctx_t;

ifsm_ctx_t *ifsm_ctx_init(zwnet_p nw);
/**<
Initialize interface state-machines context
@param[in]	nw		    Network context
@return Interface state-machines context on success; else returns NULL
@post Caller must call ifsm_ctx_shutdown() followed by ifsm_ctx_exit() to shutdown and cleanp up if this call is successfully
*/

void ifsm_ctx_shutdown(ifsm_ctx_t *ctx);
/**<
Shutdown interface state-machines context
@param[in]	ctx		    Interface state-machines context
@return
*/

void ifsm_ctx_exit(ifsm_ctx_t *ctx);
/**<
Clean up and free resources for interface state-machines context
@param[in]	ctx		    Interface state-machines context
@return
@post ctx must not be used again after this call
*/


/**
@}
*/



#endif /* _ZIP_INTF_SM_DAVID_ */

