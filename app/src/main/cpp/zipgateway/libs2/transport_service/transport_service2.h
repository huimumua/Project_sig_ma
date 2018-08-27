/* © 2017 Sigma Designs, Inc. This is an unpublished work protected by Sigma
 * Designs, Inc. as a trade secret, and is not to be used or disclosed except as
 * provided Z-Wave Controller Development Kit Limited License Agreement. All
 * rights reserved.
 *
 * Notice: All information contained herein is confidential and/or proprietary to
 * Sigma Designs and may be covered by U.S. and Foreign Patents, patents in
 * process, and are protected by trade secret or copyright law. Dissemination or
 * reproduction of the source code contained herein is expressly forbidden to
 * anyone except Licensees of Sigma Designs  who have executed a Sigma Designs’
 * Z-WAVE CONTROLLER DEVELOPMENT KIT LIMITED LICENSE AGREEMENT. The copyright
 * notice above is not evidence of any actual or intended publication of the
 * source code. THE RECEIPT OR POSSESSION OF  THIS SOURCE CODE AND/OR RELATED
 * INFORMATION DOES NOT CONVEY OR IMPLY ANY RIGHTS  TO REPRODUCE, DISCLOSE OR
 * DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE, USE, OR SELL A PRODUCT THAT IT  MAY
 * DESCRIBE.
 *
 * THE SIGMA PROGRAM AND ANY RELATED DOCUMENTATION OR TOOLS IS PROVIDED TO COMPANY
 * "AS IS" AND "WITH ALL FAULTS", WITHOUT WARRANTY OF ANY KIND FROM SIGMA. COMPANY
 * ASSUMES ALL RISKS THAT LICENSED MATERIALS ARE SUITABLE OR ACCURATE FOR
 * COMPANY’S NEEDS AND COMPANY’S USE OF THE SIGMA PROGRAM IS AT COMPANY’S
 * OWN DISCRETION AND RISK. SIGMA DOES NOT GUARANTEE THAT THE USE OF THE SIGMA
 * PROGRAM IN A THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICES ENVIRONMENT WILL
 * BE: (A) PERFORMED ERROR-FREE OR UNINTERRUPTED; (B) THAT SIGMA WILL CORRECT ANY
 * THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICE ENVIRONMENT ERRORS; (C) THE
 * THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICE ENVIRONMENT WILL OPERATE IN
 * COMBINATION WITH COMPANY’S CONTENT OR COMPANY APPLICATIONS THAT UTILIZE THE
 * SIGMA PROGRAM; (D) OR WITH ANY OTHER HARDWARE, SOFTWARE, SYSTEMS, SERVICES OR
 * DATA NOT PROVIDED BY SIGMA. COMPANY ACKNOWLEDGES THAT SIGMA DOES NOT CONTROL
 * THE TRANSFER OF DATA OVER COMMUNICATIONS FACILITIES, INCLUDING THE INTERNET,
 * AND THAT THE SERVICES MAY BE SUBJECT TO LIMITATIONS, DELAYS, AND OTHER PROBLEMS
 * INHERENT IN THE USE OF SUCH COMMUNICATIONS FACILITIES. SIGMA IS NOT RESPONSIBLE
 * FOR ANY DELAYS, DELIVERY FAILURES, OR OTHER DAMAGE RESULTING FROM SUCH ISSUES.
 * SIGMA IS NOT RESPONSIBLE FOR ANY ISSUES RELATED TO THE PERFORMANCE, OPERATION
 * OR SECURITY OF THE THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICES
 * ENVIRONMENT THAT ARISE FROM COMPANY CONTENT, COMPANY APPLICATIONS OR THIRD
 * PARTY CONTENT. SIGMA DOES NOT MAKE ANY REPRESENTATION OR WARRANTY REGARDING THE
 * RELIABILITY, ACCURACY, COMPLETENESS, CORRECTNESS, OR USEFULNESS OF THIRD PARTY
 * CONTENT OR SERVICE OR THE SIGMA PROGRAM, AND DISCLAIMS ALL LIABILITIES ARISING
 * FROM OR RELATED TO THE SIGMA PROGRAM OR THIRD PARTY CONTENT OR SERVICES. TO THE
 * EXTENT NOT PROHIBITED BY LAW, THESE WARRANTIES ARE EXCLUSIVE. SIGMA OFFERS NO
 * WARRANTY OF NON-INFRINGEMENT, TITLE, OR QUIET ENJOYMENT. NEITHER SIGMA NOR ITS
 * SUPPLIERS OR LICENSORS SHALL BE LIABLE FOR ANY INDIRECT, SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES OR LOSS (INCLUDING DAMAGES FOR LOSS OF BUSINESS, LOSS OF
 * PROFITS, OR THE LIKE), ARISING OUT OF THIS AGREEMENT WHETHER BASED ON BREACH OF
 * CONTRACT, INTELLECTUAL PROPERTY INFRINGEMENT, TORT (INCLUDING NEGLIGENCE),
 * STRICT LIABILITY, PRODUCT LIABILITY OR OTHERWISE, EVEN IF SIGMA OR ITS
 * REPRESENTATIVES HAVE BEEN ADVISED OF OR OTHERWISE SHOULD KNOW ABOUT THE
 * POSSIBILITY OF SUCH DAMAGES. THERE ARE NO OTHER EXPRESS OR IMPLIED WARRANTIES
 * OR CONDITIONS INCLUDING FOR SOFTWARE, HARDWARE, SYSTEMS, NETWORKS OR
 * ENVIRONMENTS OR FOR MERCHANTABILITY, NONINFRINGEMENT, SATISFACTORY QUALITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * The Sigma Program  is not fault-tolerant and is not designed, manufactured or
 * intended for use or resale as on-line control equipment in hazardous
 * environments requiring fail-safe performance, such as in the operation of
 * nuclear facilities, aircraft navigation or communication systems, air traffic
 * control, direct life support machines, or weapons systems, in which the failure
 * of the Sigma Program, or Company Applications created using the Sigma Program,
 * could lead directly to death, personal injury, or severe physical or
 * environmental damage ("High Risk Activities").  Sigma and its suppliers
 * specifically disclaim any express or implied warranty of fitness for High Risk
 * Activities.Without limiting Sigma’s obligation of confidentiality as further
 * described in the Z-Wave Controller Development Kit Limited License Agreement,
 * Sigma has no obligation to establish and maintain a data privacy and
 * information security program with regard to Company’s use of any Third Party
 * Service Environment or Cloud Service Environment. For the avoidance of doubt,
 * Sigma shall not be responsible for physical, technical, security,
 * administrative, and/or organizational safeguards that are designed to ensure
 * the security and confidentiality of the Company Content or Company Application
 * in any Third Party Service Environment or Cloud Service Environment that
 * Company chooses to utilize.
 */
/*
 * transport_service2.h
 *
 *  Created on: Oct 10, 2014
 *      Author: aes
 */

#ifndef TRANSPORT_SERVICE2_H_
#define TRANSPORT_SERVICE2_H_

//#include "ZW_typedefs.h"
#if defined(ZIPGW)
#include <TYPES.H>
#include "ZW_SendDataAppl.h" /* for ts_param_t */
#include <stdio.h>
#include <ZW_transport_api.h>
#else
#include "ts_types.h"
#endif

#include <ZW_classcmd.h> /* for ZW_APPLICATION_TX_BUFFER */
#include "transport2_fsm.h"
//#include <ZIP_Router.h>


extern TRANSPORT2_ST_T current_state;


#define FRAGMENT_FC_TIMEOUT          1000 /*ms*/
#define FRAGMENT_RX_TIMEOUT          800 /*ms*/

//#define DATAGRAM_SIZE_MAX       (UIP_BUFSIZE - UIP_LLH_LEN) /*1280*/
#define DATAGRAM_SIZE_MAX       (200) /*1280*/

#ifdef __C51__
#ifndef slash
#define slash /
#endif
#endif

#ifdef __C51__
#define T2_DBG slash/
#else
//#define DBG 1
#ifdef DBG
#define T2_DBG(format, args...) \
        printf("T2: %s sid: %d rid: %d, %s():%d: ",T2_STATES_STRING[current_state],scb.cmn.session_id, rcb.cmn.session_id,__func__, __LINE__);\
        printf(format, ## args); \
        printf("\n");
#else
#define T2_DBG(format, args...)
#endif
#endif

#ifdef __C51__
#define T2_ERR slash/
#else
#if defined(ZIPGW) || defined(DBG)
#define T2_ERR(format, args...) \
        printf("T2: %s sid: %d rid: %d, %s():%d: ", T2_STATES_STRING[current_state],scb.cmn.session_id, rcb.cmn.session_id,__func__, __LINE__);\
        printf(format, ## args); \
        printf("\n");
#else
#define T2_ERR(format, args...)
#endif
#endif

#define TIMER
//#define DEBUG_TIMER

#if !defined(ZIPGW)
#include <transport_service2_external.h>
#else
/**
 * Input function for the transport service module, this must be called when we receive a frame
 * of type COMMAND_CLASS_TRANSPORT_SERVICE.
 *
 * \param p structure containing the parameters of the transmission, like source node and destination node.
 * \param pCmd pointer to the received frame.
 * \param cmdLength Length of the received frame.
 */
void TransportService_ApplicationCommandHandler(ts_param_t* p, BYTE *pCmd, BYTE cmdLength);

/**
 * Initialize the Transport service state machine.
 * \param commandHandler Application command handler to be called when a full datagram has been received.
 *
 */
void
ZW_TransportService_Init(void
(*commandHandler)(ts_param_t* p, ZW_APPLICATION_TX_BUFFER *pCmd,
WORD cmdLength));

/**
 * \defgroup TransportService Transport service module
 * \{
 *
 * This module handles the Z-Wave Transport Service command class version 2.
 * The module is able handle a single TX session to a node, and able to handle
 * a number of RX session for some nodes.
 */


/**
 * Send a large frame from srcNodeID to dstNodeID using TRANSPORT_SERVICE V2. Only one
 * transmit session is allowed at any time.
 *
 * \param p structure containing the parameters of the transmission, like source node and destination node.
 * \param pData pointer to the data being sent. The contents of this buffer must not change
 * while the transmission is in progress.
 * \param txOption the Z-Wave transmit options to use in this transmission.
 * \param completedFunc A callback which is called when the transmission has completed. The status of the
 * transmission is given in txStatus. See \ref ZW_SendData.
 * \return
 *      - TRUE if the transmission is started, and callback will be called when the transmission is done.
 *      - FALSE Transmission is not started, because another transmission is already on progress.
 *        The callback function will not be called.
 */
BOOL ZW_TransportService_SendData(ts_param_t* p, const BYTE *pData, uint16_t dataLength,
    void (*completedFunc)(BYTE txStatus, TX_STATUS_TYPE *));


#endif

#endif /* TRANSPORT_SERVICE2_H_ */

