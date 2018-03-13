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
 * command_handler.h
 *
 *  Created on: Apr 7, 2016
 *      Author: aes
 */

#ifndef SRC_COMMAND_HANDLER_H_
#define SRC_COMMAND_HANDLER_H_

#include "ZW_typedefs.h"
#include "ZW_udp_server.h"


typedef enum {
  COMMAND_HANDLED,      /// the command was handled by the command handler
  COMMAND_PARSE_ERROR,  /// the command handler was unable to parse the command
  COMMAND_BUSY,         /// the command handler was unable to process to request because it is currently performing another operation.
  COMMAND_NOT_SUPPORTED,/// the command handler does not support this command
  CLASS_NOT_SUPPORTED,  /// there is no command handler registered for this class. This is an internal return code and should not be returned by any registered handler
  COMMAND_CLASS_DISABLED,  /// Command class has been disabled. This is an internal return code and should not be returned by any registered handler
} command_handler_codes_t;

typedef struct {
  /**
   * This is the function which will be executed when frame of the given command class is received.
   * The handler MUST return proper \ref command_handler_codes_t code.
   *
   * \param connection  a zwave_connection_t structure, this has info about the transport properties of
   * this frame.
   * \param payload  The data payload  of this  frame.
   * \param len      The length of this frame
   */
  command_handler_codes_t (*handler)(zwave_connection_t *, uint8_t* frame, uint16_t length); /// the command handler self

  /**
   * Initializer, this function initializes the command handler, ie. resetting state machines etc.
   */
  void (*init)(void);  /// Initialize the command handler
  uint16_t cmdClass;   /// command class that this handler implements
  uint8_t  version;    /// version of the implemented command class
  security_scheme_t  minimal_scheme; ///the minimal security level which this command is supported on.
} command_handler_t;

#ifdef __APPLE__
#define REGISTER_HANDLER(handler_func,init_func,cmdClass, version,scheme) \
  static const command_handler_t  __handler##cmdClass##__ \
    __attribute__(( __section__("__TEXT,__handlers") )) \
    __attribute__(( __used__ )) \
    = { handler_func,init_func,cmdClass,version,scheme};
#else
#define REGISTER_HANDLER(handler_func,init_func,cmdClass, version,scheme) \
  static const command_handler_t __handler##cmdClass##__ \
    __attribute__(( __section__("_handlers") )) \
    __attribute__(( __used__ )) \
    = { handler_func,init_func,cmdClass,version,scheme};
#endif

/**
 * Find a command handler in the database for the frame pointed to by \ref payload. If no command handler
 * is found \ref CLASS_NOT_SUPPORTED is returned. Otherwise the return code of the handler is returned.
 *
 * \param connection  a zwave_connection_t structure, this has info about the transport properties of
 * this frame.
 * \param payload  The data payload  of this  frame.
 * \param len      The length of this frame
 */
command_handler_codes_t ZW_command_handler_run(zwave_connection_t *connection, uint8_t* payload, uint16_t len);

/**
 * Get the version of a command handler.
 * \param secure    If this must be a secure handler.
 * \param cmdClass  Command class to query for.
 * \return Command handler version, if no handler is found, this function returns 0
 */
uint8_t ZW_comamnd_handler_version_get( security_scheme_t scheme, uint16_t cmdClass);


/**
 * Build the command class list for the NIF based on the registered command handlers
 *
 * \param scheme Security level to build the NIF for (secure NIF, non-secure NIF)
 * \param nif Where to store the command class list
 * \param max_len Maximum number of bytes to output. Output will be silently truncated if max_len is exceeded.
 */
uint8_t ZW_command_handler_get_nif( security_scheme_t scheme, uint8_t* nif,uint8_t max_len);

/**
 * Initialize the command handlers
 */
void ZW_command_handler_init();


/**
 * Set a list of commands which are disabled
 */
void ZW_command_handler_disable_list(uint16_t *cmdList,uint8_t cmdListLen);


#endif /* SRC_COMMAND_HANDLER_H_ */
