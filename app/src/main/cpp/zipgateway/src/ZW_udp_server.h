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
#ifndef ZW_UDP_SERVER_H_
#define ZW_UDP_SERVER_H_

/** \ingroup processes
 * \defgroup ZIP_Udp Z/IP UDP process
 * Handles the all Z/IP inbound and outbout UDP communication
 * Manages the Z/IP sessions for the Zipgateway
 * @{
 */
/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/
#include "net/uip.h"
#include "ZW_SendDataAppl.h"
#define ZWAVE_PORT 4123

typedef struct {
  union {
    /**
     * Convenience attribute.
     */
    struct uip_udp_conn conn;
    struct {
      /**
       * Local ip address
       */
      uip_ipaddr_t lipaddr;
      /**
       * Remote IP address
       */
      uip_ipaddr_t ripaddr;
      /**
       *  The local port number in network byte order.
       */
      u16_t lport;
      /**
       * The remote port number in network byte order.
       */
      u16_t rport;
    };
  };
  /**
   * remote endpoint
   */
  uint8_t rendpoint;

  /**
   * local endpoint
   *
   */
  uint8_t lendpoint;
  /**
   * Sequence number when sending UDP frames
   */
  uint8_t seq;
  /**
   * Security scheme when sending Z-Wave frames
   */
  security_scheme_t scheme;
  /**
   * rx flags when receive as a Z-Wave frame
   */
  u8_t rx_flags;
  /**
   * tx flags when sending as a Z-Wave frame
   */
  u8_t tx_flags;
} zwave_udp_session_t;

/**
 * Structure describing a Z-Wave or Z/IP connection, with source and destination
 * ips/nodids/endpoint. Together with specific parameter such as receiver flags.
 */
typedef zwave_udp_session_t zwave_connection_t;

/**
 * Returns true if the two connections are identical, i has the same source and destination addresses.
 */
int zwave_connection_compare(zwave_connection_t* a,zwave_connection_t *b);


typedef enum {
  RES_ACK,RES_NAK,RES_WAITNG,RES_OPT_ERR,
} zwave_udp_response_t;

/**
 * Send a udp ACK NAK or waiting
 */
void send_udp_ack(zwave_udp_session_t* s, zwave_udp_response_t res  );


/**
 * The generic application command handler for Z/IP frames.
 * \param c The connection describing the source and destination of the message
 * \param pData pointer the Z-Wave command to be handled
 * \param bDatalen length of the Z-Wave command to be handled
 */
extern void ApplicationIpCommandHandler(zwave_connection_t *c, void *pData, u16_t bDatalen);


/**
 * Wrapper function to Send package at an ordinary Z-wave package or as a
 * Z/IP UDP package depending on destination address.
 *
 * If the first 15 bytes of the destination address is all zeroes then
 * the package is sent as a ordinary Z-wave package. In this case
 * \see ZW_SendData_UDP
 * \see ZW_SendData
 *
 * \param c connection describing the source and destination for this package
 * \param dataptr pointer to the data being sent
 * \param datalen length of the data being sent
 * \param cbFunc Callback to be called when transmission is complete
 */
extern void
ZW_SendDataZIP(zwave_connection_t *c,  const  void *dataptr, u16_t datalen,
    ZW_SendDataAppl_Callback_t cbFunc);


/**
 * Convenience function which calls \ref ZW_SendDataZIP
 *
 * \param src Source ip of package
 * \param  dst Destination ip of package
 * \param  port Destination port to send to
 * \param dataptr pointer to the data being sent
 * \param datalen length of the data being sent
 * \param cbFunc Callback to be called when transmission is complete
 * \deprecated
 */
extern void __ZW_SendDataZIP(
    uip_ip6addr_t* src,uip_ip6addr_t* dst, WORD port,
    const void *dataptr,
    u16_t datalen,
    ZW_SendDataAppl_Callback_t cbFunc);


/**
 * Convenience function which calls \ref ZW_SendDataZIP_ack
 *
 * \param src Source ip of package
 * \param  dst Destination ip of package
 * \param  port Destination port to send to in network byte order
 * \param dataptr pointer to the data being sent
 * \param datalen length of the data being sent
 * \param cbFunc Callback to be called when transmission is complete
 * \deprecated
 */
extern void __ZW_SendDataZIP_ack(
    uip_ip6addr_t* src,uip_ip6addr_t* dst, WORD port,
    const void *dataptr,
    u16_t datalen,
    ZW_SendDataAppl_Callback_t cbFunc);

/**
 * Send a Z-Wave udp frame, but with the ACK flag set. No retransmission will be attempted
 * See \ref ZW_SendDataZIP
 */
extern void
ZW_SendDataZIP_ack(zwave_connection_t *c,const void *dataptr, u8_t datalen, void
    (*cbFunc)(u8_t, void*, TX_STATUS_TYPE *));

extern void (*uip_completedFunc)(u8_t,void*);


/**
 * Create a udp con structure from the package in uip_buf
 */
struct uip_udp_conn* get_udp_conn();

/**
 * \return TRUE if the specified address is a Z-Wave address ie a 0\::node address
 */
#define ZW_IsZWAddr(a)     \
  (                        \
   (((a)->u8[1]) == 0) &&                        \
   (((a)->u16[1]) == 0) &&                          \
   (((a)->u16[2]) == 0) &&                          \
   (((a)->u16[3]) == 0) &&                          \
   (((a)->u16[4]) == 0) &&                          \
   (((a)->u16[5]) == 0) &&                          \
   (((a)->u16[6]) == 0) &&                          \
   (((a)->u8[14]) == 0) &&                          \
   (((a)->u8[15]) == 0))

/**
 * Input function for RAW Z-Wave udp frames. This function parses the COMMAND_CLASS_ZIP header
 * and manages UDP sessions.
 * \param c UDP connection the frame ware recieved on.
 * \param data pointer to the udp data
 * \param len length of the udp data
 * \param received_secure TRUE if this was a UDP frame which has been decrypted with DTLS.
 */
void UDPCommandHandler(struct uip_udp_conn* c,const u8_t* data, u16_t len,u8_t received_secure);


/**
 * Send package using the given UDP connection. The package may be DTLS encrypted.
 */

void udp_send_wrap(struct uip_udp_conn* c, const void* data, u16_t len);


typedef struct
{
  u8_t type;
  u8_t length;
  u8_t value[1];
} tlv_t;


/**
 * Convert zwave udp packet EFI extensions to a security scheme
 * \param ext1 fist ext byte
 * \param ext2 second ext byte
 */
security_scheme_t efi_to_shceme(uint8_t ext1, uint8_t ext2);



/**
 * Check if we have anything outstanding in the IPv4 queue
 */
void
udp_server_check_ipv4_queue();


/**
 * This process handles all Z/ZIP UDP communication.
 */
PROCESS_NAME(udp_server_process);

/** @} */
#endif /* ZW_UDP_SERVER_H_ */

