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
 * s2.h
 *
 *  Created on: Jun 25, 2015
 *      Author: aes
 */

#ifndef INCLUDE_S2_H_
#define INCLUDE_S2_H_

#include<stdint.h>
#include<platform.h>

#ifndef DllExport
#define DllExport extern
#endif

#include "ctr_drbg.h"

#define S2_MSG_FLAG_EXT      0x1
#define S2_MSG_FLAG_SEXT     0x2

#define S2_NONCE_REPORT_SOS_FLAG 1
#define S2_NONCE_REPORT_MOS_FLAG 2

#define S2_MSG_EXTHDR_CRITICAL_FLAG 0x40
#define S2_MSG_EXTHDR_MORE_FLAG 0x80
#define S2_MSG_EXTHDR_TYPE_MASK 0x3F
#define S2_MSG_EXTHDR_TYPE_SN 1
#define S2_MSG_EXTHDR_TYPE_MPAN 2
#define S2_MSG_EXTHDR_TYPE_MGRP 3
#define S2_MSG_EXTHDR_TYPE_MOS  4

/* TODO move above into ZW_classcmd.h */


/**
 * \defgroup S2trans S2 transport system
 * The S2 transport system defines all the interfaces which are needed for
 * normal S2 singlecast and multicast frame transmission. This module will
 * interface the the underlying Z-Wave system calls such as ZW_SendData and
 * timer functions. The Z-Wave application command handler will also hook into
 * this module to provide frame input to the S2 layer.
 *
 * @{
 **/


/**
 * This flag will activate frame delivery.
 *
 * In this transmission mode the S2_send_data will try
 * to verify that the receiver understood the sent message.
 * This is done by waiting a little to see if the node will
 * respond nonce report to the encrypted message. If the node
 * does respond with a nonce report then the S2_send_data
 * call will automatically cause the system to re-sync the node,
 * and deliver the message
 *
 */
#define S2_TXOPTION_VERIFY_DELIVERY 1

/**
 * This flag must be present on all single cast followup messages.
 */
#define S2_TXOPTION_SINGLECAST_FOLLOWUP 2

/**
 * This flag must be present on the first, and only the first single
 * cast followup message in a multicast transmission.
 */
#define S2_TXOPTION_FIRST_SINGLECAST_FOLLOWUP 4

/**
 * Set this flag if the received message was a multicast message.
 */
#define S2_RXOPTION_MULTICAST 1

/**
 * Set this flag if the received message was a multicast message.
 */
#define S2_RXOPTION_FOLLOWUP 2

/* parameter used by the send function */

/**
 * The node type.
 */
typedef uint8_t node_t;

/**
 * Security class which is used, in S2_send_data and is set in S2_network_key_update
 * The class must be less than S2_NUM_KEY_CLASSES
 */
typedef uint8_t security_class_t;


/**
 * The structure holds information about the sender and receiver in a S2
 * Transmission.
 */
typedef struct peer {
  /**
   * Local node, if sending this a the node sending the message, when receiving this is the node receiving the message.
   */
  node_t l_node; //local   node

  /**
   * Remote node, when sending this is the node that should receive the message. When receiving a message, this is the
   * node for whom the message is targeted. In the special case of multicast transmission, whic must be the multicast
   * group id.
   */
  node_t r_node;

  /**
   * This is the S2 tx flags see \ref s2txopt
   */
  uint8_t tx_options;
  /**
   * This is the S2 rx flags see \ref s2rxopt
   */
  uint8_t rx_options;

  /**
   * These is just passed transparent through the S2 transport system.
   */
  uint8_t zw_tx_options;
  uint8_t zw_rx_status;
  uint8_t zw_rx_RSSIval;
  //uint8_t zw_multicast_nodelist[29];

  /**
   * The security class which is used to send this frame, or the the class used to decrypt this frame.
   */
  security_class_t class_id;
} s2_connection_t;

/**
 * Status codes for \ref S2_send_data
 */
typedef enum {
  /**
   * Frame was sent to node, we the node should have been able to decrypt the frame
   * but we don't know for sure.
   */
  S2_TRANSMIT_COMPLETE_OK,

  /**
   * We are sure that the node has decrypted the frame, because it has sent a message
   * the other way which it would only be able to send if it could decrypt this frame
   */
  S2_TRANSMIT_COMPLETE_VERIFIED,

  /**
   * Frame was not sent due to a MAC layer failure
   */
  S2_TRANSMIT_COMPLETE_NO_ACK,

  /**
   * Frame was not sent because some other failure, i.e the node does not support S2
   */
  S2_TRANSMIT_COMPLETE_FAIL,

} s2_tx_status_t;


/**
 * Anonymous declaration of the S2 context, the data inside S2 context must not be
 * modified outside libs2 */
struct S2;

/**
 *  Network key type used in S2
 */
typedef uint8_t network_key_t[16];

#ifdef SINGLE_CONTEXT
#define  S2_send_data(a, b ,c, d)                   __S2_send_data(b,c,d)
#define  S2_is_send_data_busy(a)                    __S2_is_send_data_busy()
#define  S2_send_data_multicast(a, b ,c, d)         __S2_send_data_multicast(b,c,d)
#define  S2_is_send_data_multicast_busy(a)          __S2_is_send_data_multicast_busy()
#define  S2_network_key_update(a, b ,c, d)          __S2_network_key_update(b,c,d)
#define  S2_destroy(a)                              __S2_destroy()
#define  S2_application_command_handler(a, b ,c, d) __S2_application_command_handler(b,c,d)
#define  S2_timeout_notify(a)                       __S2_timeout_notify()
#define  S2_send_frame_done_notify(a, b ,c)         __S2_send_frame_done_notify(b,c)
#define  S2_is_busy(a)                              __S2_is_busy()
#define  S2_set_inclusion_peer(a, b ,c)             __S2_set_inclusion_peer(b,c)

#endif
/**
 * Send singlecast security s2 encrypted frame. Upon completion this call will call \ref S2_send_done_event. Only one transmission
 * may be active at a time. If this function is called twice without waiting to the S2_send_done_event it will return FALSE
 *
 * \param ctxt   the security context.
 * \param peer   transmit parameters, destination node, ack req etc. see \ref s2_connection_t
 * \param buf   plaintext to which is going to be sent. The the data in this pointer must be valid until the S2_send_done_event has been called.
 * \param len   length of data to be sent.
 * \return TRUE if the frame transmission has been initiated. FALSE the transmission has not begun and \ref S2_send_done_event will not be called.
 *
 *
 */
uint8_t S2_send_data(struct S2* ctxt, const s2_connection_t* peer ,const uint8_t* buf, uint16_t len);

/**
* Check if S2 is ready to receive a new frame for transmission
*
* Use to ensure S2_send_data will accept a frame before calling it.
* \param[in] ctxt The S2 context
* \return False if the S2_send_data method will accept a new frame for transmission
*/
uint8_t S2_is_send_data_busy(struct S2* ctxt);

/**
 * Send multicast security s2 encrypted frame.
 * This will only send the multicast frame itself. There will be no single cast follow ups.
 * a S2_send_done_event will be emitted when the transmission is done.
 *
 * \param ctxt           the security context.
 * \param dst           connection handle a handle for the mulicast group to use, this group id will also be used when calling S2_send_frame_multi
 * \param buf           plaintext to which is going to be sent.
 * \param len           length of data to be sent.
 *
 */
uint8_t S2_send_data_multicast(struct S2* ctxt,  const s2_connection_t* dst, const uint8_t* buf, uint16_t len);

/**
* Check if S2 is ready to receive a new multicast frame for transmission
*
* Use to ensure S2_send_data_multicast will accept a frame before calling it.
* \param[in] ctxt The S2 context
* \return False if the S2_send_data_multicast method will accept a new frame for transmission
*/
uint8_t S2_is_send_data_multicast_busy(struct S2* ctxt);

/**
 * Allocates and Initializes a context. This must be done on every power up, or when the homeID changes.
 * The allocated context must be freed when its not longer needed, ie. when home ID changes. See \ref S2_destroy.
 * This function will also read in the network keys using the S2_keysotre. See \ref keystore.
 *
 * \param home HomeID of the context
 *
 * \return pointer to the allocated context. If libs2 has been compiled with SINGLE_CONTEXT the context is statically allocated.
 * libs2 in multi-context mode is only strictly need in test mode.

 * \callgraph
 */
struct S2*
S2_init_ctx(uint32_t home);


/**
 * Free's resources associated with context, the context will be invalid after a call to this function
 *
 * \param ctxt the S2 context
 */
void S2_destroy(struct S2* ctxt);

/**
 * Command handler for all incoming COMMAND_CLASS_SECURITY2 frames.
 * \param ctxt the S2 context
 * \param peer Information about the frame transaction. Rember to fill in the rx_options.
 * \param buf  pointing to the received data. The S2 machine may alter the data in this buffer.
 * \param len  The length of the received data.
 */
void S2_application_command_handler(struct S2* ctxt, s2_connection_t* peer , uint8_t* buf, uint16_t len);

/**
 * This must be called when the timer set by \ref S2_set_timeout has expired.
 * \param ctxt the S2 context
 */
void S2_timeout_notify(struct S2* ctxt);

/**
 * Notify the security stack that sent with \ref S2_send_frame has completed.
 * \param ctxt the S2 context
 * \param status  status code of the transmission
 * \param tx_time the time used for this transmission i milliseconds.
 */
void S2_send_frame_done_notify(struct S2* ctxt, s2_tx_status_t status,uint16_t tx_time);

/**
* Check if the S2 FSM or inclusion FSM are busy.
*
* \param ctxt the S2 context
* \return False if the FSM and inclusion FSM are both idle
*/
uint8_t S2_is_busy(struct S2* ctxt);

/**
 * Set the inclusion peer. Use on slave nodes only.
 * They do not have this information available when starting secure learn mode.
 * \param ctxt the S2 context
 * \param inclusion_peer_nodeid Nodeid of peer performing inclusion
 * \param my_nodeid The nodeid of the node itself.
 */
void S2_set_inclusion_peer(struct S2* ctxt, uint8_t inclusion_peer_nodeid, uint8_t my_nodeid);

#include "S2_external.h"
#include "s2_inclusion.h"

/**
* Initialize the S2 PRNG. Must be called on powerup.
* Do not call when starting S2 inclusion. Disables the radio briefly while running.
*/
void S2_init_prng(void);

/**
 * The random number generator used by S2
 */
extern CTR_DRBG_CTX s2_ctr_drbg;


/**
 * @}
 */
#endif /* INCLUDE_S2_H_ */
