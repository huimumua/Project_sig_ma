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

#ifndef ZIP_ROUTER_H_
#define ZIP_ROUTER_H_

/**
 * \ingroup processes
 *
 * \defgroup ZIP_Router Z/IP Router Main process for zipgateway
 * This process is main process which spawns of series of sub processes
 * It is responsible for
 * handling all incoming events and perform actions from sub processes.
 * @{
 */
#include "sys/process.h"
#include "net/uip.h"
#include "TYPES.H"
#include "ZW_SendDataAppl.h"
#include "pkgconfig.h"

#include "assert.h"

#include "router_events.h"
#include "zip_router_config.h"

#include "ZIP_Router_logging.h"

/**
 * Virtual mac address pf the tunnel interface
 */
extern const uip_lladdr_t tun_lladdr;

/**
 * Callback LAN output function.
 * \param a destination link layer (MAC) address. If zero the packet must be broadcasted
 *          packet to be transmitted is in uip_buf buffer and the length of the packet is
 *          uip_len
 *
 */
typedef  u8_t(* outputfunc_t)(uip_lladdr_t *a);
/**
 * The set output function for the LAN
 * Output function for the IP packets
 */
void set_landev_outputfunc(outputfunc_t f);

/* TODO: Remove? */
extern BYTE Transport_SendRequest(BYTE tnodeID, BYTE *pBufData, BYTE dataLength,
    BYTE txOptions, void (*completedFunc)(BYTE));

/*Forward declaration*/
union _ZW_APPLICATION_TX_BUFFER_;

/** Application command handler for Z-wave packets. The packets might be the decrypted packets
 * \param p description of the send,receive params
 * \param pCmd Pointer to the command received
 * \param cmdLength Length of the command received
 */

void ApplicationCommandHandlerZIP(ts_param_t *p,
		union _ZW_APPLICATION_TX_BUFFER_ *pCmd, WORD cmdLength) CC_REENTRANT_ARG;

/** Application command handler for raw Z-wave packets. 
 * \see ApplicationCommandHandler
 */
void ApplicationCommandHandlerSerial(BYTE rxStatus,BYTE destNode, BYTE sourceNode,
    union _ZW_APPLICATION_TX_BUFFER_ *pCmd, BYTE cmdLength)CC_REENTRANT_ARG;

/** ID of this node
 */
extern BYTE MyNodeID; 
/** Home ID of this node
 */
extern DWORD homeID;

typedef enum {
    SECONDARY,
    SUC,
    SLAVE
} controller_role_t;
    
extern controller_role_t controller_role;

/**
 * Check on the expression.
 */
#define ASSERT assert

/**
 * The Z/IP router process
 */
PROCESS_NAME(zip_process);

/**
 * Get the ipaddress of a given node
 * \param dst output
 * \param nodeID ID of the node
 */
void ipOfNode(uip_ip6addr_t* dst, BYTE nodeID);

/**
 * Return the node is corresponding to an ip address
 * \param ip IP address
 * \return ID of the node 
 */
BYTE nodeOfIP(uip_ip6addr_t *ip);


/**
 * Returns True is address is a classic zwave address.
 * \param ip IP address
 */
BYTE isClassicZWAddr(uip_ip6addr_t* ip);

/**
 * The zip_process is the main process in the Z/IP gateway application.
 * allmost all
 */
PROCESS_NAME(zip_process);


/* TODO: use is_4to6_addr()? */
u8_t is_4to6_addr_check(uip_ip6addr_t* ip);

/* TODO: Move the declaration somewhere else? May be in CC_Gateway.c*/
void parse_gw_profile(Gw_PeerProfile_St_t *pGwPeerProfile)REENTRANT;

/* TODO: Move ? */
void Reset_Gateway(void) CC_REENTRANT_ARG;

/* TODO: Move ? */
uint8_t validate_defaultconfig(uint32_t  addr, uint8_t isInt, uint16_t *pOutGwSettingsLen) CC_REENTRANT_ARG;
/* TODO: Move ? */
uint8_t MD5_ValidatFwImg(uint32_t baseaddr, uint32_t len) REENTRANT;
/* TODO: Do it in a different way. Use another code from the Resource directory*/
BYTE IsCCInNodeInfoSetList(BYTE cmdclass,BOOL secure) CC_REENTRANT_ARG;

void macOfNode(uip_lladdr_t* dst, u8_t nodeID);


extern BYTE SecureClasses[];
extern BYTE nSecureClasses;

void refresh_ipv6_addresses();

/**
 * The highest security scheme supported by this node.
 */
extern security_scheme_t net_scheme;

/** @} */ // end of ZIP_Router
#endif /* ZIP_ROUTER_H_ */
