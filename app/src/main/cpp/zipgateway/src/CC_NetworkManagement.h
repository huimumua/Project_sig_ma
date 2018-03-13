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

#ifndef NETWORKMANAGEMENT_H_
#define NETWORKMANAGEMENT_H_

/**
 *  \ingroup CMD_handler
 *  \defgroup NW_CMD_hadler Handler for Network Management Command Classes
 *
 * Control of the Z-Wave network is carried out through a number of Network
 * Management Command Classes.
 *
 * The Z/IP Gateway MUST support all four Network Management Command Classes:
 * - Network Management Inclusion: Support for Adding/Removing, Set Default,
 *   Failed Node Replacement and Removal and Requesting Node Neighbor Update
 * - Network Management Basic: Support for entering Learn Mode, Node Information Send
 *   and Request Network Update
 * - Network Management Proxy: Support for Node List Get and Node Info Cached Get
 * - Network Management Primary: Support for Controller Change, only available
 *   when operating as Primary Controller without SIS functionality.
 *
 * These four command classes enable network management and access to simple network topology information.
 *
 * @{
 */
#include "contiki-net.h"
#include "TYPES.H"
#include "ZW_udp_server.h"
#include "s2_inclusion.h"
/**
 * State variable tracking it the network managemnet module has alread been initalized
 */
extern int network_management_init_done;

/**
 * State of the networkmanagement module.
 */
typedef enum {
  NM_IDLE,
  NM_WAITING_FOR_ADD,
  NM_NODE_FOUND,
  NM_WAIT_FOR_PROTOCOL,
  NM_NETWORK_UPDATE,
  NM_WAITING_FOR_PROBE,
  /** When changing to this state, we MUST call rd_exit() to advance the state machine. */
  NM_SET_DEFAULT,
  NM_LEARN_MODE,
  NM_LEARN_MODE_STARTED,
  NM_WAIT_FOR_SECURE_ADD,
  NM_SENDING_NODE_INFO,
  NM_WAITING_FOR_NODE_REMOVAL,
  NM_WAITING_FOR_FAIL_NODE_REMOVAL,
  NM_WAITING_FOR_NODE_NEIGH_UPDATE,
  NM_WAITING_FOR_RETURN_ROUTE_ASSIGN,
  NM_WAITING_FOR_RETURN_ROUTE_DELETE,

  NM_WAIT_FOR_PROBE_AFTER_ADD,
  NM_WAIT_FOR_SECURE_LEARN,
  /** When changing to this state, we MUST call rd_exit() to advance the state machine. */
  NM_WAIT_FOR_MDNS,
  NM_WAIT_FOR_PROBE_BY_SIS,
  NM_WAIT_FOR_OUR_PROBE,
  NM_WAIT_DHCP,
  NM_REMOVING_ASSOCIATIONS,

  NM_REPLACE_FAILED_REQ,
  NM_PREPARE_SUC_INCLISION,
  NM_WIAT_FOR_SUC_INCLUSION,
  NM_PROXY_INCLUSION_WAIT_NIF,
  NM_WAIT_FOR_SELF_DESTRUCT,
  NM_WAIT_FOR_TX_TO_SELF_DESTRUCT,
  NM_WAIT_FOR_SELF_DESTRUCT_REMOVAL,
} nm_state_t;




/**
 * The DHCP stage is completed
 */
#define NETWORK_UPDATE_FLAG_DHCPv4    0x1
/**
 * Node probe is completed
 */
#define NETWORK_UPDATE_FLAG_PROBE     0x2

#define NETWORK_UPDATE_FLAG_VIRTUAL   0x4
#define NETWORK_UPDATE_FLAG_DISABLED  0x80
/**
 * Notify the netowrk management module that some event has occurred.
 */
void NetworkManagement_NetworkUpdateStatusUpdate(u8_t flag);


/**
 * Initialize this module, and if possible send a message from the previous life of
 * the gateway.
 *
 * Some network management commands will cause the Z/IP gateway to reset, Ie it will restart
 * its IPv6 stack, cancel all pending processes, and reprobe the network. This does all take time. The
 * IP reply to the command should only be sent when all the probing is done etc.
 *
 * Allong the boot process NetworkManagement_Init will be called several times. Each time it will
 * check if it is able to send the pending ip package. If the package is sent this function will return 1
 * otherwise it will return 0.
 *
 * if network_management_init_done == 1 calling this function will have no effect.
 * if init is successful \ref network_management_init_done will be set to 1.
 * \return TRUE on success.
 */
int NetworkManagement_Init();


/**
 * Called when a node gets an Ipv4 address assigned.
 */
void NetworkManagement_IPv4_assigned(BYTE node);

/**
 * Called when mdns process has exited
 */
void NetworkManagement_mdns_exited();

/**
 * Get the state of the Network management module
 */
nm_state_t NetworkManagement_getState();

/**
 * Return TRUE if the current network management peer(if any) is the unsolicited destination
 */
BOOL NetworkManagement_is_Unsolicited_peer();

/**
 * Return TRUE if the current network management peer(if any) is the unsolicited destination
 */
BOOL NetworkManagement_is_Unsolicited2_peer();

/**
 * Send a Unsolicite NodeList report to the unsolicited destination
 */
BOOL NetworkManagement_SendNodeList_To_Unsolicited();


void NetworkManagement_VirtualNodes_removed();

/*
 * Event from security2 layer to the networkmanagement fsm to notify dsk of the node to be included.
 */
void NetworkManagement_dsk_challenge(s2_node_inclusion_challenge_t *challenge_evt);

/*
 * Event from security2 layer to the networkmanagement fsm to notify about requested keys
 */
void NetworkManagement_key_request(s2_node_inclusion_request_t* inclusion_request);


/**
 * Initiate proxy inclusion, requesting information about the node and ask unsolicited destination
 * for permission to proceed inclusion process.
 */
void  NetworkManagement_start_proxy_inclusion(uint8_t node_id);


/**
 * Initiate proxy replace, requesting information about the node and ask unsolicited destination
 * for permission to proceed inclusion process.
 */
void NetworkManagement_start_proxy_replace(uint8_t node_id) ;

/**
 * Notify the NM state machine about a incoming nif
 */
void NetworkManagement_nif_notify(uint8_t bNodeID,uint8_t* pCmd,uint8_t bLen);

/**
 * Notify the NM state machine about a incoming frame
 */
void NetworkManagement_frame_notify();

/**
 * Notify the NM state machine about all nodes probed
 */
void NetworkManagement_all_nodes_probed();

void
NetworkManagement_smart_start_inclusion(uint8_t inclusion_options, uint8_t *prekit_homeID);

/**
 * Notify that S0 inclusion has started
 */
void NetworkManagement_s0_started();

/**
 * Enable Smart Start mode if pending DSKs exist in provisioning list.
 */
void NetworkManagement_smart_start_init_if_pending();

/**
 *  Called when an Included NIF has been received from SerialAPI
 */
void NetworkManagement_INIF_Received(uint8_t bNodeID, uint8_t INIF_rxStatus,
    uint8_t *INIF_NWI_homeid);

/* Return newly included smart start nodeid only when we are waiting for
 * middleware to finish probing it. Ot
 *
 * \returns NodeID of newly included smart start node if we are waiting for middleware probe to finish.
 * \returns 0 otherwise.
 * */
uint8_t NM_get_newly_included_nodeid(void);

/* Called when node queue forwards a frame to a newly included smart start node.
 * We detect middleware has finished probing when 10 seconds pass without another
 * frame to the node.
 * */
void extend_middleware_probe_timeout(void);

/* Check if Network Management is idle
 *
 * \return TRUE if Network Management state is NM_IDLE
 * \return
 */
int nm_is_idle();
/** @} */
#endif /* NETWORKMANAGEMENT_H_ */
