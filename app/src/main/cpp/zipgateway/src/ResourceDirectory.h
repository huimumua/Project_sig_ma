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
#ifndef RESOURCEDIRECTORY_H
#define RESOURCEDIRECTORY_H

/**
 * \ingroup ZIP_MDNS
 * \defgroup ZIP_Resource Z/IP Resource directory
 *
 * The resource directory is a database containing information about all nodes in the network.
 * This modules will probe nodes in the network for their supported command class multi-endpoint
 * Capabilities etc.
 *
 * The resource directory works closely with the \ref ZIP_MDNS.
 *
 * @{
 */
#include "RD_internal.h"

//#include "contiki-net.h"
#include "sys/ctimer.h"
#include "sys/clock.h"

#define MODE_FLAGS_DELETED 0x0100
#define MODE_FLAGS_FAILED  0x0200
#define MODE_FLAGS_LOWBAT  0x0400


#define RD_ALL_NODES 0


/**
 * Register a new node into database, this will cause the resource
 * database engine to query nodes for new information.
 *
 * Set NODE_FLAG_JUST_ADDED on probe_flags if the controller is still adding node.
 * Set NODE_FLAG_ADDED_BY_ME on probe_flags if the controller is SIS.
 *
 * @param node node is to probe
 * @param probe_flags addition mode flags 
 */
void rd_register_new_node(uint8_t node,uint8_t probe_flags);

/**
 * Do a complete re-discovery and "hijack" wakeup nodes
 */
void rd_full_network_discovery();

/**
 * Re-discover nodes which are not in the database
 * @return the number of new nodes to probe
 */
u8_t rd_probe_new_nodes();

/**
 * Remove a node from resource database
 */
void rd_remove_node(uint8_t node);


/**
 * Initialize the resource directory. Return TRUE if the
 * homeID has changed since the last rd_init
 *
 * @param lock boolean, set true if node probe machine should be locked after init
 */
int rd_init(uint8_t);

/**
 * Gracefully send goodbye packages for all endpoints
 */
void rd_exit();

/**
 * Finally free memory allocated by RD
 */
void rd_destroy();

/**
 *  Lock/Unlock the node probe machine. When the node probe lock is enabled, all probing will stop.
 *  Probing is resumed when the lock is disabled. The probe lock is used during a add node process or during learn mode.
 */
void rd_probe_lock(uint8_t enable);

/**
 * Retrieve a
 */
rd_ep_database_entry_t* rd_lookup_by_ep_name(const char* name,const char* location);
rd_node_database_entry_t* rd_lookup_by_node_name(const char* name);

rd_group_entry_t* rd_lookup_group_by_name(const char* name);

rd_ep_database_entry_t* rd_ep_first(uint8_t node);
rd_ep_database_entry_t* rd_ep_iterator_group_begin(rd_group_entry_t*);
rd_ep_database_entry_t* rd_ep_next(uint8_t node,rd_ep_database_entry_t* ep);
rd_ep_database_entry_t* rd_group_ep_iterator_next(rd_group_entry_t* ge,rd_ep_database_entry_t* ep);

#define SUPPORTED_NON_SEC   0x1
#define CONTROLLED_NON_SEC  0x2
#define SUPPORTED_SEC       0x4
#define CONTROLLED_SEC      0x8
#define SUPPORTED  0x5
#define CONTROLLED 0xA

int rd_ep_class_support(rd_ep_database_entry_t* ep, uint16_t cls);

/*Called when a nif is received or when a nif request has timed out*/
void rd_nif_request_notify(uint8_t bStatus,uint8_t bNodeID,uint8_t* pCmd,uint8_t bLen);

/* Register an endpoint notifier. The endpoint notifier will be called when the probe of an endpoint has completed.
 * If the endpoints is already in state PROBE_DONE or PROBE_FAIL, the callback is called synchronously. */
int rd_register_ep_probe_notifier(
    uint16_t node_id,
    uint16_t epid,
    void* user,
    void (*callback)(rd_ep_database_entry_t* ep, void* user));
/**
 * Get nodedatabase entry from data store. free_node_dbe, MUST
 * be called when the node entry if no longer needed.
 */
rd_node_database_entry_t* rd_get_node_dbe(uint8_t nodeid);

/**
 * MUST be called when a node entry is no longer needed.
 */
void rd_free_node_dbe(rd_node_database_entry_t* n);


/**
 * Returns true if node is registered in database
 * @param node
 * @return
 */
u8_t rd_node_exists(u8_t node);

/**
 * Get an endpoint id entry from nodeID and epid,
 */
rd_ep_database_entry_t* rd_get_ep(uint8_t nodeid, uint8_t epid);

rd_node_mode_t rd_get_node_mode(uint8_t nodeid);
rd_node_state_t rd_get_node_state(uint8_t nodeid);
u8_t rd_get_node_schemes(uint8_t nodeid); // reutrn the security scheme suported by a node
/**
 * Should be called whenever a node has been found to be alive, ie. when
 * a command was successfully sent to the node or when we received a command from the node.
 */
void rd_node_is_alive(uint8_t node);


void rd_set_failing(rd_node_database_entry_t* n,uint8_t failing);

u8_t rd_get_ep_name(rd_ep_database_entry_t* ep,char* buf,u8_t size);
u8_t rd_get_ep_location(rd_ep_database_entry_t* ep,char* buf,u8_t size);
/**
 * Sets name/location of an endpoint. This will trigger a mdns probing. Note that
 * the name might be changed if the probe fails.
 */
void rd_update_ep_name(rd_ep_database_entry_t* ep,char* name,u8_t size);
void rd_update_ep_location(rd_ep_database_entry_t* ep,char* name,u8_t size);

/**
 * Update both the name and location of an endpoint.
 *
 * For creating a name/location during inclusion use \ref rd_add_name_and_location().
 */
void
rd_update_ep_name_and_location(rd_ep_database_entry_t* ep, char* name,
    u8_t name_size, char* location, u8_t location_size);

/**
 * return true if a probe is in progress
 */
u8_t rd_probe_in_progress();

/**
 * Mark the mode of node MODE_FLAGS_DELETED so that
 */
void rd_mark_node_deleted(uint8_t nodeid);

/**
 * Check node database to see if there are any more nodes to probe.
 *
 * A node will (eventually) be probed if it fulfills one of the following conditions:
 *
 * - it is the current probe
 *
 * - it is not in any of the states STATUS_DONE, STATUS_PROBE_FAIL, or
 *   STATUS_FAILING.
 */
void rd_probe_resume();

void rd_node_probe_update(rd_node_database_entry_t* n);
u8_t validate_name(char* name, u8_t len);
u8_t validate_location(char* name, u8_t len);
/**  @} */
#endif

