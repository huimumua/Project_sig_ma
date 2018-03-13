/* © 2014 Sigma Designs, Inc. This is an unpublished work protected by Sigma
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

#ifndef ZW_SENDDATAAPPL_H_
#define ZW_SENDDATAAPPL_H_
#include <ZW_transport_api.h>

#define TRANSMIT_OPTION_SECURE 0x0100

/** \ingroup transport
 * \defgroup Send_Data_Appl Send Data Appl
 * SendDataAppl is the top level module for sending encapsulated packages on
 * the Z-Wave network.
 * @{
 */

/**
 * Enumeration of security schemes, this must map to the bit numbers in NODE_FLAGS
 */
typedef enum SECURITY_SCHEME {
  /**
   * Do not encrypt message
   */
  NO_SCHEME = 0xFF,

  /**
   * Let the SendDataAppl layer decide how to send the message
   */
  AUTO_SCHEME = 0xFE,

  NET_SCHEME = 0xFC, //Highest network shceme
  USE_CRC16 = 0xFD, //Force use of CRC16, not really a security scheme..

  /**
   * Send the message with security Scheme 0
   */
  SECURITY_SCHEME_0 = 7,

  /**
   * Send the message with security Scheme 2
   */
  SECURITY_SCHEME_2_UNAUTHENTICATED = 1,
  SECURITY_SCHEME_2_AUTHENTICATED = 2,
  SECURITY_SCHEME_2_ACCESS = 3,
  SECURITY_SCHEME_UDP = 4,

} security_scheme_t ;

/**
 * Structure describing how a package was received / should be transmitted
 */
typedef struct tx_param {
  /**
   * Source node
   */
  uint8_t snode;
  /**
   * Destination node
   */
  uint8_t dnode;

  /**
   * Source endpoint
   */
  uint8_t sendpoint;

  /**
   * Destination endpoint
   */
  uint8_t dendpoint;

  /**
   * Transmit flags
   * see txOptions in \ref ZW_SendData
   */
  uint8_t tx_flags;

  /**
   * Receive flags
   * see rxOptions in \ref ApplicationCommandHandler
   */
  uint8_t rx_flags;

  /**
   * Security scheme used for this package
   */
  security_scheme_t scheme; // Security scheme
} ts_param_t;


/**
 * Get standard transmit parameters
 * @param p
 * @param dnode
 */
void ts_set_std(ts_param_t* p, uint8_t dnode);

/**
 * Copy transport parameter structure
 * @param dst
 * @param src
 */
void ts_param_copy(ts_param_t* dst, const ts_param_t* src);

/**
 * Convert transport parameters to a reply
 * @param dst
 * @param src
 */
void ts_param_make_reply(ts_param_t* dst, const ts_param_t* src);

/**
 * \return true if source and destinations are identical
 */
uint8_t ts_param_cmp(ts_param_t* a1, const ts_param_t* a2);

/**
 * \param txStatus, see \ref ZW_SendData
 * \param user User defined pointer
 */
typedef void( *ZW_SendDataAppl_Callback_t)(BYTE txStatus,void* user, TX_STATUS_TYPE *txStatEx);

/**
 * Old style send data callback
 * @param txStatus  see \ref ZW_SendData
 */
typedef void( *ZW_SendData_Callback_t)(BYTE txStatus);



/**
 * This function will automatically
 * send a Z-Wave frame with the right encapsulation, like Multichannel, Security, CRC16, transport
 * service or no encapsulation at all.
 *
 * The decision is done based in the parameter given in \ref p, \ref dataLength and the
 * know capabilities of the destination node.
 *
 * \param p Parameters used for the transmission
 * \param pData Data to send
 * \param dataLength Length of frame
 * \param callback function to be called in completion
 * \param user user defined pointer which will provided a the second argument of the callback
 * see \ref ZW_SendDataAppl_Callback_t
 *
 */
uint8_t ZW_SendDataAppl(ts_param_t* p,
  const void *pData,
  uint16_t  dataLength,
  ZW_SendDataAppl_Callback_t callback,
  void* user) __attribute__((warn_unused_result));
/**
 * Initialize senddataAppl module
 */
void ZW_SendDataAppl_init();

/**
 * Abort the transmission
 */
void
ZW_SendDataApplAbort(uint8_t handle ) ;


/**
 * return true if a has a larger or equal scheme to b
 */
int scheme_compare(security_scheme_t a, security_scheme_t b);

/* Returns the highest security scheme of */
security_scheme_t highest_scheme(uint8_t scheme_mask);

/** @} */

#endif /* ZW_SENDDATAAPPL_H_ */
