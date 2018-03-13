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
 * S2_external.h
 *
 *  Created on: Sep 24, 2015
 *      Author: aes
 */

#ifndef INCLUDE_S2_EXTERNAL_H_
#define INCLUDE_S2_EXTERNAL_H_

#include "S2.h"
/**
 * \ingroup S2trans
 * \defgroup s2external Extenal functions used by S2
 *
 * All of these function must be implemented elsewhere. These are
 * the hooks that libs2 use for actually sending frames, and to timer tracking.
 * @{
 */
#ifdef SINGLE_CONTEXT
#define S2_send_done_event(a,b) __S2_send_done_event(b)
#define S2_msg_received_event(a,b,c,d) __S2_msg_received_event(b,c,d)
#define S2_send_frame(a,b,c,d) __S2_send_frame(b,c,d)
#define S2_send_frame_multi(a,b,c,d) __S2_send_frame_multi(b,c,d)
#define S2_set_timeout(a,b) __S2_set_timeout(b)
#endif
/**
 * Emitted when the security engine is done.
 * Note that this is also emitted when the security layer sends a SECURE_COMMANDS_SUPPORTED_REPORT
 *
 * \param ctxt the S2 context
 * \param status The status of the S2 transmisison
 */
void S2_send_done_event(struct S2* ctxt, s2_tx_status_t status);

/**
 * Emitted when a messages has been received and decrypted
 * \param ctxt the S2 context
 * \param peer Transaction parameters. peer->class_id will indicate which security class was used for
 * decryption.
 * \param buf The received message
 * \param len The length of the received message
 */
void S2_msg_received_event(struct S2* ctxt,s2_connection_t* peer , uint8_t* buf, uint16_t len);


/**
 * Must be implemented elsewhere maps to ZW_SendData or ZW_SendDataBridge note that ctxt is
 * passed as a handle. The ctxt MUST be provided when the \ref S2_send_frame_done_notify is called
 *
 * \param ctxt the S2 context
 * \param peer Transaction parameters.
 * \param buf The received message
 * \param len The length of the received message
 *
 */
uint8_t S2_send_frame(struct S2* ctxt,const s2_connection_t* peer, uint8_t* buf, uint16_t len);

#ifdef ZW_CONTROLLER
/**
 * Send without emitting a callback into S2 and by caching the buffer
 *
 * Must be implemented elsewhere maps to ZW_SendData or ZW_SendDataBridge note that ctxt is
 * passed as a handle. The ctxt MUST be provided when the \ref S2_send_frame_done_notify is called.
 *
 * \param ctxt the S2 context
 * \param peer Transaction parameters.
 * \param buf The received message
 * \param len The length of the received message
 *
 */
uint8_t S2_send_frame_no_cb(struct S2* ctxt,const s2_connection_t* peer, uint8_t* buf, uint16_t len);
#endif

/**
 * This must send a broadcast frame.
 * \ref S2_send_frame_done_notify must be called when transmisison is done.
 *
 * \param ctxt the S2 context
 * \param peer Transaction parameters.
 * \param buf The received message
 * \param len The length of the received message
 */
uint8_t S2_send_frame_multi(struct S2* ctxt,s2_connection_t* peer, uint8_t* buf, uint16_t len);

/**
 * Must be implemented elsewhere maps to ZW_TimerStart. Note that this must start the same timer every time.
 * Ie. two calls to this function must reset the timer to a new value. On timeout \ref S2_timeout_notify must be called.
 *
 * \param ctxt the S2 context
 * \param interval Timeout in milliseconds.
 */
void S2_set_timeout(struct S2* ctxt, uint32_t interval);


/**
 * Get a number of bytes from a hardware random number generator
 *
 * \param buf pointer to buffer where the random bytes must be written.
 * \param len number of bytes to write.
 */
void S2_get_hw_random(uint8_t *buf, uint8_t len);

/**
 * Get the command classes supported by S2
 * \param lnode The node id to which the frame was sent.
 * \param class_id the security class this request was on
 * \param cmdClasses points to the first command class
 * \param length the length of the command class list
 */
void S2_get_commands_supported(uint8_t lnode, uint8_t class_id, const uint8_t ** cmdClasses, uint8_t* length);

/**
 * External function for printing debug output
 * Takes printf arguments
 */
void S2_dbg_printf(const char *str, ...);

/**
 * Makes time in ms available to LibS2
 * \return Timer tick in MS
 */
#ifdef ZIPGW
unsigned long clock_time(void);
#else
uint16_t clock_time(void);
#endif

/**
 * @}
 */
#endif /* INCLUDE_S2_EXTERNAL_H_ */
