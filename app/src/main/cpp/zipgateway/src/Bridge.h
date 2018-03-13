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
#ifndef BRIDGE_H_
#define BRIDGE_H_
#include "ZW_SendDataAppl.h"
#include "ZW_udp_server.h"
#include "ZW_zip_classcmd.h"

/**
 * \ingroup ip_emulation
 *
 * @{
 */

#define PREALLOCATED_VIRTUAL_NODE_COUNT 4
#define MAX_IP_ASSOCIATIONS (200 + PREALLOCATED_VIRTUAL_NODE_COUNT)

/* EEPROM layout:
 * 2 bytes: element count
 * X bytes: IP_association 1
 * X bytes: IP_association 2
 * ....
 * X bytes: IP_association N */
/* TODO: Remove sizeof(uint16_t) from ASSOCIATION_TABLE_EEPROM_SIZE. It has been factored out as a separate field. */
#define ASSOCIATION_TABLE_EEPROM_SIZE (sizeof(uint16_t) + MAX_IP_ASSOCIATIONS * sizeof(struct IP_association))

/* Bridge association types */
enum ASSOC_TYPE {temporary_assoc, permanent_assoc, local_assoc, proxy_assoc};
#define ASSOC_TYPE_COUNT 4 /* Number of values in enum ASSOC_TYPE */

typedef uint8_t nodeid_t;

typedef enum {BRIDGE_FAIL, BRIDGE_OK} return_status_t;

    /* Logical structure of IP associations are tree as follows with association source node id as roof of the tree
     * second level leaves are association source endpoints and the last level is groupings
     *           association source node-id
     *              /                 \
     *             /                   \
     *       association            association
     *      source endpoint        source endpoint
     *        /      \               /       \
     *       /        \             /         \
     *grouping      grouping    grouping    grouping
     *  | | |       | | |          | | |       | | |
     *associations associations associations associations
     *
     * But following data structure storing it in memory is like a flat table 
     */
 
struct IP_association {
  void *next;
  nodeid_t virtual_id;
  enum ASSOC_TYPE type; /* unsolicited or association */
  uip_ip6addr_t resource_ip; /*Association Destination IP */
  uint8_t resource_endpoint;  /* From the IP_Association command. Association destination endpoint */
  u16_t resource_port;
  uint8_t virtual_endpoint;   /* From the ZIP_Command command */
  uint8_t grouping;
  uint8_t han_nodeid; /* Association Source node ID*/
  uint8_t han_endpoint; /* Association Source endpoint*/
  uint8_t was_dtls;
  uint8_t mark_removal;
}; // __attribute__((packed));   /* Packed because we copy byte-for-byte from mem to eeprom */

enum bridge_state {
  booting,
  initialized,
  initfail,
};

extern enum bridge_state bridge_state;

/* Last error status for bridge */
extern return_status_t bridge_error;
extern uint8_t virtual_nodes_mask[MAX_NODEMASK_LENGTH];

/*
 * Create a bridge association and corresponding virtual node
 */
return_status_t /* RET: Status code BRIDGE_OK or BRIDGE_FAIL */
create_bridge_association(enum ASSOC_TYPE type,
    void (*cb_func)(struct IP_association *),
    ZW_COMMAND_IP_ASSOCIATION_SET *ip_assoc_set_cmd,uint8_t was_dtls);

/*
 * Create a local bridge association between two HAN nodes
 */
struct IP_association* create_local_association(ZW_COMMAND_IP_ASSOCIATION_SET *cmd);

/*
 *  List active bridge associations
 */
void list_bridge_associations(void);

/*
 *   Print bridge status to console
 */
void print_bridge_status(void);

/*
 *  Remove a bridge association
 */
void remove_bridge_association(struct IP_association* a);

void remove_bridge_association_and_persist(struct IP_association *a);

/*
 *  Initialize the bridge
 */
void bridge_init(void);

void ApplicationCommandHandler_Bridge(
    BYTE  rxStatus,
    BYTE destNode,
    BYTE  sourceNode,
    ZW_APPLICATION_TX_BUFFER *pCmd,
    BYTE cmdLength);


char is_virtual_node(nodeid_t nid);

/**
 * Update virtual nodes mask
 */
void register_virtual_nodes(void);

/**
 * Boolean, TRUE if this is a bridge library.
 */
extern uint8_t is_bridge_library;


/**
 * Returns true if a matching virtual node session was found
 */
BOOL bridge_virtual_node_commandhandler(
    ts_param_t* p,
    BYTE *pCmd,
    BYTE cmdLength);

/*
 * Clear and persist the association table.
 */
void bridge_clear_association_table(void);

struct IP_association *find_ip_associ_to_remove_by_node_id(nodeid_t assoc_source_id);
uint8_t find_ip_assoc_for_report(
    uint8_t nid,
    uint8_t han_endpoint,
    uint8_t grouping,
    uint8_t index,
    struct IP_association **result);

/**
 * Locate an IP association by its source and destination nodes + desitnation endpoint
 */
struct IP_association* get_ip_assoc_by_nodes(u8_t snode,u8_t dnode, u8_t endpoint);

void resume_bridge_init(void);

void bridge_reset(void);
/**
 * Locate an IP association by its virtual nodeid.
 */
struct IP_association* get_ip_assoc_by_virtual_node(u8_t virtnode);

/**
 * mark the in-memory ipa for removal in response to one of the bulk remove ipa
 * command or in response to the removal of a node which has multiple associations
 */
void mark_bulk_removals(nodeid_t assoc_source_id, uint8_t assoc_source_endpoint, uint8_t grouping,uip_ip6addr_t *res_ip,uint8_t res_endpoint);

/**
 * Return the in-memory ipa marked for removal in response to one of the bulk remove ipa
 * command or in response to the removal of a node which has multiple associations
 */
struct IP_association *get_ipa_marked_for_removal();
void decrememnt_bulk_ipa_remove();

/**
 * Return the value of global variable bulk_ipa_remove. This variable is used to count the
 * ipa association which are marked to be removed in response to one of the bulk remove ipa
 * command or in response to the removal of a node which has multiple associations 
 */
int bulk_ipa_remove_val();

/* Decrement the bulk_ipa_remove counter. See bulk_ipa_remove_val() for description of bulk_ipa_remove variable */
void decrememnt_bulk_ipa_remove();


/* Return value of the flag which differentiates between bulk ipa removal command and node removal */
int node_removal_val();

/**
 * }@
 */
#endif /* BRIDGE_H_ */
