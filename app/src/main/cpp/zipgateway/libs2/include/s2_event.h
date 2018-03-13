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
 * s2_event.h
 *
 *  Created on: Oct 16, 2015
 *      Author: trasmussen
 */

#ifndef _S2_EVENT_H_
#define _S2_EVENT_H_

/**
 * \defgroup s2_event libs2 inclusion events.
 *
 * @{
 */

#include "S2.h"
#include "platform.h"

/**
 * This enum contains codes for all type of S2 events.
 */
typedef enum
{
  S2_NODE_INCLUSION_INITIATED_EVENT,            ///< Security 2 event, a KEX Get has been received from including node, thus indicating that Security 2 inclusion is now in progress..
  S2_NODE_INCLUSION_KEX_REPORT_EVENT,           ///< Security 2 event, a node is in progress of being included to the network. This event contains the requested key set from the KEX report. This event MUST be followed by a call to \ref s2_inclusion_key_grant.
  S2_NODE_INCLUSION_PUBLIC_KEY_CHALLENGE_EVENT, ///< Security 2 event, a node is in progress of being included to the network. This event is a request to the system to verify the challenge and progress with inclusion. This event MUST be followed by a call to \ref s2_inclusion_challenge_response.
  S2_NODE_INCLUSION_COMPLETE_EVENT,             ///< Security 2 event, inclusion of the node  to the network has been completed.
  S2_NODE_JOINING_COMPLETE_EVENT,               ///< Security 2 event, the node has completed its inclusion.
  S2_NODE_INCLUSION_FAILED_EVENT,   ///< Security 2 event, inclusion of the node  to the network has failed. See the details
} zwave_event_codes_t;

typedef struct{
  struct S2     * s2_handle;      ///< Handle reference to the S2 structure to which this event relates.
  uint8_t         security_keys;  ///< Security keys requested by the joining node.
  uint8_t         csa;            ///< Client side authentication was requested
}s2_node_inclusion_request_t;

typedef struct{
  struct S2     * s2_handle;     ///< Handle reference to the S2 structure to which this event relates.
  uint8_t         granted_keys;  ///< Security keys granted earlier in inclusion process.
  uint8_t         dsk_length;    ///< Number of missing bytes in the DSK which is requested by the protocol and must be returned with \ref s2_inclusion_challenge_response.
  uint16_t        length;        ///< Length of public key following below.
  uint8_t         public_key[1]; ///< Public key, length of key is describe in length field.
}s2_node_inclusion_challenge_t;

typedef struct{
  uint8_t         exchanged_keys; ///< Keys actually given/granted by/to node
} s2_node_inclusion_complete_t;

typedef struct{
  uint8_t        kex_fail_type;
} s2_node_inclusion_fail_t;

typedef struct
{
  s2_connection_t peer;
  union {
    s2_node_inclusion_request_t      kex_report;
    s2_node_inclusion_challenge_t    challenge_req;
    s2_node_inclusion_complete_t     inclusion_complete;
    s2_node_inclusion_fail_t         inclusion_fail;
  } s2_data;

//  uint16_t        length;
//  uint8_t         buffer[1]; // Variable byte array at end to allow for different packets. How to free the memory ?
}s2_event_t;


typedef struct
{
  uint32_t event_type;
  // The below struct is inspired from SKB in Linux. Do we need it ?
  // Discussed with Anders. We should be concerned not to expose the linked list externally, but
  // maybe just keep it internally so that app cannot mess up/invalidate the list.
  // This also means the app must call an event_get every time we have sent a notification.
  /*
  struct
  {
    struct event_t * next;
    struct event_t * prev;
    uint32_t         timestamp;
  };
  */
  union
  {
    s2_event_t s2_event;
  }evt;
  // TODO: should we put the buffer here ?
  // Some events may not need a buffer, or at least a small one, in which case having the variable
  // buffer at end of struct in union allow dynamic growth.
  // Linux has a bit more flexibility for free form data, but in z-wave we might decide that we
  // know all supported form to ensure better name checking.
} zwave_event_t; // TODO: Should this be named zwave_event_t ?

/** @brief S2 event handler.
 * 
 *  @details The event handler is used for notifying upper layers about events from S2 securoity layers.
 *           Such events may be, but are not limited to, public key exchange during inclusions,
 *           transmission reports.
 *
 * @param[in] zwave_event_t  Z-Wave event identifying the event and associated data.
 */
typedef void (CODE *s2_event_handler_t)(zwave_event_t *);

/**
 * @}
 */
#endif // _S2_EVENT_H_
