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
#ifndef RD_BASIC_H
#define RD_BASIC_H

#include "lib/list.h" /* Contiki lists */
#include "net/uip.h" /* Contiki uip_ip6addr_t */

typedef enum {
  MODE_PROBING,
  MODE_NONLISTENING,
  MODE_ALWAYSLISTENING,
  MODE_FREQUENTLYLISTENING,
  MODE_MAILBOX,
} rd_node_mode_t;

typedef enum {
  //STATUS_ADDING,
  STATUS_CREATED,
  //STATUS_PROBE_PROTOCOL_INFO,
  STATUS_PROBE_NODE_INFO,
  STATUS_PROBE_PRODUCT_ID,
  STATUS_ENUMERATE_ENDPOINTS,
  STATUS_CHECK_WU_CC_VERSION,
  STATUS_GET_WU_CAP,
  STATUS_SET_WAKE_UP_INTERVAL,
  STATUS_ASSIGN_RETURN_ROUTE,
  STATUS_PROBE_WAKE_UP_INTERVAL,
  STATUS_PROBE_ENDPOINTS,
  STATUS_MDNS_PROBE,
  STATUS_MDNS_EP_PROBE,
  STATUS_DONE,
  STATUS_PROBE_FAIL,
  STATUS_FAILING,
} rd_node_state_t;

typedef struct rd_node_database_entry {

  uint32_t wakeUp_interval;
  uint32_t lastAwake;
  uint32_t lastUpdate;

  uip_ip6addr_t ipv6_address;

  uint8_t nodeid;
  uint8_t security_flags;
  /*uint32_t homeID;*/

  rd_node_mode_t mode;
  rd_node_state_t state;

  uint16_t manufacturerID;
  uint16_t productType;
  uint16_t productID;

  uint8_t nodeType; // Is this a controller, routing slave ... etc

  uint8_t refCnt;
  uint8_t nEndpoints; //Total number of endpointss
  uint8_t nAggEndpoints; //Number of aggregated endpoints

  LIST_STRUCT(endpoints);

  uint8_t nodeNameLen;
  uint8_t dskLen; /**< Currently 16 or 0. */
  char* nodename;
  uint8_t *dsk; /**< Key to connect with provisioning list. */
} rd_node_database_entry_t;

typedef enum {
  EP_STATE_PROBE_AGGREGATED_ENDPOINTS,
  EP_STATE_PROBE_INFO, /** Initial state */
  EP_STATE_PROBE_SEC2_C2_INFO,
  EP_STATE_PROBE_SEC2_C1_INFO, /* Skipped when probing proper endpoints (>0). */
  EP_STATE_PROBE_SEC2_C0_INFO, /* Skipped when probing proper endpoints (>0). */
  EP_STATE_PROBE_SEC0_INFO,
  EP_STATE_PROBE_ZWAVE_PLUS,
  EP_STATE_MDNS_PROBE,
  EP_STATE_MDNS_PROBE_IN_PROGRESS,
  EP_STATE_PROBE_DONE,
  EP_STATE_PROBE_FAIL
} rd_ep_state_t;



typedef struct rd_ep_data_store_entry {
  uint8_t endpoint_info_len;
  uint8_t endpoint_name_len;
  uint8_t endpoint_loc_len;
  uint8_t endpoint_aggr_len;
  uint8_t endpoint_id;
  rd_ep_state_t state;
  uint16_t iconID;
} rd_ep_data_store_entry_t;


typedef struct rd_ep_database_entry {
  struct rd_ep_database_entry* list;
  rd_node_database_entry_t* node;
  char *endpoint_location;
  char *endpoint_name;
  uint8_t *endpoint_info;
  uint8_t *endpoint_agg;

  uint8_t endpoint_info_len;
  uint8_t endpoint_name_len;
  uint8_t endpoint_loc_len;
  uint8_t endpoint_aggr_len; //Length of aggregations
  uint8_t endpoint_id;
  rd_ep_state_t state;
  uint16_t installer_iconID;
  uint16_t user_iconID;
} rd_ep_database_entry_t;



typedef struct rd_group_entry {
  char name[64];
  list_t* endpoints;
} rd_group_entry_t;

rd_node_database_entry_t* rd_node_entry_alloc(uint8_t nodeid);

rd_node_database_entry_t* rd_node_entry_import(uint8_t nodeid);

void rd_node_entry_free(uint8_t nodeid);

rd_node_database_entry_t* rd_node_get_raw(uint8_t nodeid);

/** Set the DSK of a node.  
 * The node must support S2 or Smart Start to have a DSK.  The DSK
 * should only be added here if it has been authenticated in S2.
 *
 * If memory allocation fails, a warning is printed and 0 length is stored.
 * 
 * @param node The node id.  Should not be MyNodeId or a virtual node.
 * @param dsklen Length of the dsk.
 * @param dsk Pointer to the memory we should copy the dsk from.
 */
void rd_node_add_dsk(u8_t node, uint8_t dsklen, uint8_t *dsk);

/**
 * Finally free memory allocated by RD
 */
void rd_destroy(void);

/**
 * Returns true if node is registred in database
 * @param node
 * @return
 */
u8_t rd_node_exists(u8_t node);

rd_ep_database_entry_t* rd_ep_first(uint8_t node);

rd_ep_database_entry_t* rd_ep_next(uint8_t node, rd_ep_database_entry_t* ep);

/**
 * function to get the node name, use the auto generated name if no name is set
 * \param n Given node entry
 * \param buf buffer to write into
 * \param size maximum number of bytes to copy
 */
u8_t rd_get_node_name(rd_node_database_entry_t* n,char* buf,u8_t size);

/**
 * Find node by node name
 */
rd_node_database_entry_t* rd_lookup_by_node_name(const char* name);

rd_node_database_entry_t* rd_lookup_by_dsk(uint8_t dsklen, const uint8_t* dsk);

/**
 * Get nodedatabase entry from data store. free_node_dbe, MUST
 * be called when the node entry if no longer needed.
 */
rd_node_database_entry_t* rd_get_node_dbe(uint8_t nodeid);

#endif
