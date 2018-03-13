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

#ifndef CLASICZIPNODE_H_
#define CLASICZIPNODE_H_
#include"Serialapi.h"
#include"contiki-net.h"
#include "Bridge.h"
#include "ZW_SendDataAppl.h"
#include "ZW_udp_server.h"
#include "ZIP_Router.h"

/**
 * \defgroup ip_emulation IP Emulation Layer
 * This layer implements IP emulation for all Z-Wave nodes.
 *
 * \section IP Support for Z-Wave Nodes
 * - The Z/IP Gateway must perform an inspection of each IP Packet received
 * to check if the receiving(destination) node in the packet is a classic Z-Wave node.
 * Z/IP Gateway does not support IP enabled nodes in this release.
 * If incase the receiving(destination) node in the IP packet is
 * Z-Wave node, the Z/IP Gateway must intercept all IP packets and if possible,
 * emulate the requested service by using equivalent features of Z-Wave.
 *
 * - The Z/IP Gateway MUST emulate IP Ping (ICMP Echo). If a Ping request
 * is received by the Z/IP Gateway for a Z-Wave node, the Z/IP Gateway
 * must use the Z-Wave NOP command to emulate the ping, and respond using
 * ICMP reply to the requesting address.
 *
 * - The Z/IP Gateway MUST forward the Z-Wave payload of any Z/IP Packet
 * received for a classic Z-Wave node to the node. It must also handle Z/IP
 * ack, and perform same ACK check on Z-Wave if requested.
 *
 * \section asproxy Association Proxy
 * The IP Association Proxy extends your network beyond the Z-Wave PAN, by allowing
 * Z-Wave devices to communicate with any IPv6 enabled device that supports the
 * Z/IP Framework on either another Z-Wave network or a separate IP application.
 * IP Associations are created between two Z/IP resources identified by a resource name –
 * which may be resolved to an IP address and an endpoint ID. The Full Z/IP Gateway
 * emulates the IP properties of Z/IP Nodes for classic nodes residing in the network:
 *
 *   - Association from one simple node to another: Send a normal Association Set to the
 *     association source in a normal frame.
 *   - Association from a multichannel endpoint to another multichannel endpoint: Send a
 *     multichannel Association Set to the association source encapsulated in a multichannel frame.
 *   - Association from a multichannel endpoint to a simple node: Send an Association Set
 *     to the association source encapsulated in a multichannel frame.
 *   - Association from a simple node to a multichannel endpoint: Send an Association Set
 *     to the association source encapsulated in a normal frame. The association targets a
 *     virtual node in the Z/IP Gateway. Create a companion association from the virtual node to the multichannel endpoint.
 *
 * @{
 */


extern uint8_t backup_buf[UIP_BUFSIZE];
extern uint16_t backup_len;
/**
 * Input for IP packages destined for Classic Z-Wave nodes.
 *
 * Assumes the package in uip_buf has node as it destination.
 * This routine will do asynchronous processing, ie. it will return before any IP reply
 * is ready.
 *
 * @param node node to send this package to
 * @param completedFunc callback to be called when frame has been sent.
 * @param bFromMailbox Boolean should be TRUE if this frame is sent from mailbox
 * @return true if the package has been processed.
 */
int ClassicZIPNode_input(int node, void(*completedFunc)(BYTE,BYTE*,uint16_t),int bFromMailbox);


/**
 * Function for receiving de-crypted udp frames from the DTLS service.
 */
int ClassicZIPNode_dec_input(void * data, int len);


/**
 * Input an udp package to a classic node.
 * @param c The udp connection
 * @param data porinter to the UDP data.
 * @param len the length of the UDP payload
 * @param secure was the command received secure
 */
int ClassicZIPUDP_input(struct uip_udp_conn* c, u8_t* data, u16_t len,BOOL secure);


void ClassicZIPNode_AbortSending();

/**
 * Init data structures
 */
void ClassicZIPNode_init();


int ClassicZIPNode_UnsolicitedHandler(ts_param_t *p,
    ZW_APPLICATION_TX_BUFFER *pCmd, BYTE cmdLength, uip_ipaddr_t *pDestAddr);
/*
 * Send a IP encapsulated to an ip host, from sourceNode to port@destIP
 */
void ClassicZIPNode_SendUnsolicited(zwave_connection_t* __c,
     ZW_APPLICATION_TX_BUFFER *pCmd,
    BYTE cmdLength, uip_ip6addr_t* destIP, u16_t port, BOOL bAck);

void send_zipack_ipassoc(uint8_t bStatus);
void reset_ip_association_state(uint8_t bStatus);
void abort_current_ip_assoc(uint8_t bStatus);

/* Asynchronously cancel an ongoing attempt to parse an IP Association Set.
 *
 * and blacklist the IP Association packet so we can discard it when
 * it reappears from the txQueue.
 *
 */
void async_abort_current_ip_assoc();

void forward_proxy_data(struct IP_association* a, unsigned char *data, char datalen);

uint8_t is_local_address(uip_ipaddr_t *ip);

/**
 * Set the txOptions used in transmissions
 */
void ClassicZIPNode_setTXOptions(u8_t opt);

/**
 * Returns TRUE if the pkt is a ZIP command, pkt must point to the IP header
 */
void ClassicZIPNode_sendNACK(BOOL __sendAck);
void classic_session_keepalive(u8_t node);

void CreateLogicalUDP(ts_param_t* p, unsigned char *pCmd, uint8_t cmdLength);

void SendUDPStatus(int flags0, int flags1, zwave_connection_t *c);

void remove_ip_association_by_nodeid(BYTE);
/**
 * Start processing of removing nodes which are marked for removal 
 */
void remove_from_bulk_list();

void  ZW_SendData_UDP(zwave_connection_t* c,
    const BYTE *dataptr, u16_t datalen, void
    (*cbFunc)(BYTE, void* user),BOOL ackreq);
/**
 * @}
 */
#endif /* CLASICZIPNODE_H_ */
