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
#include <uip.h>
#include <contiki.h>
#include <list.h>
#include <memb.h>
#ifndef ZIP_NATIVE
#include <stdio.h>
#endif
#include "ClassicZIPNode.h"
#include <uip-debug.h>
#include "ZW_controller_bridge_api.h"
#include "ZW_zip_classcmd.h"
#include "ZIP_Router.h"
#include "Bridge.h"
#include "ZW_udp_server.h"
#include "ResourceDirectory.h"
#include "eeprom_layout.h"
#include "DataStore.h"
#include "security_layer.h"
#include "sys/ctimer.h"
#include "DTLS_server.h"
#define BACKUP_UIP_IP_BUF                ((struct uip_ip_hdr *)backup_buf)
#define BACKUP_UIP_UDP_BUF          ((struct uip_udp_hdr *)&backup_buf[UIP_IPH_LEN])
#define BACKUP_ZIP_PKT_BUF               ((ZW_COMMAND_ZIP_PACKET*)&backup_buf[UIP_IPUDPH_LEN])
// Broke due to variable length extensions to zipheader
//#define BACKUP_IP_ASSOC_PKT_BUF          ((ZW_COMMAND_IP_ASSOCIATION_SET*)&backup_buf[UIP_IPUDPH_LEN + ZIP_HEADER_SIZE])

#define BIT8_TST(bit,array) ((array[(bit)>>3] >> ((bit) & 7)) & 0x1)
#define BIT8_SET(bit,array) array[(bit)>>3] |= 1<<((bit) & 7)
#define BIT8_CLR(bit,array) array[(bit)>>3] &= ~(1<<((bit) & 7))

#define MAX_ENDPOINTS 127

/* Timeout before retrying ZW_SetSlaveLearnMode */
#define VIRTUAL_ADD_RETRY_TIMEOUT 1000
#define VIRTUAL_ADD_RETRY_MAX 10

/* Cooldown period after each succesful virtual node add */
/* 2 sec due to TO#3682 */
#define VIRTUAL_ADD_COOLDOWN_DELAY 2000

int bulk_ipa_remove = 0;
enum bridge_state bridge_state;

static void
virtual_node_allocated(struct IP_association *new_assoc, ZW_COMMAND_IP_ASSOCIATION_SET *ip_assoc_set_cmd);
static void
temporary_virtual_add_callback(uint8_t bStatus, uint8_t orgID, uint8_t newID);

nodeid_t temp_assoc_virtual_nodeids[PREALLOCATED_VIRTUAL_NODE_COUNT];
uint8_t temp_assoc_virtual_nodeid_count;
static int virtual_add_retry_count;

LIST(association_table);
MEMB(association_pool, struct IP_association, MAX_IP_ASSOCIATIONS);

uint8_t virtual_nodes_mask[MAX_NODEMASK_LENGTH];
struct ctimer virtual_add_timer;

static int virtual_nodeid; /* Holds nodeid of new virtual node
 * Note that some endpoints of the nodeid may be used, but the
 * singlechannel "non-endpoint" is guaranteed to be available.
 */
static int multichannel_permanent_nodeid; /* The virtual nodeid we endpoint multiplex when
 * assoc source is multichannel */
static int temporary_association_count; /* How many temporary associations exist */

/* Callback function to call when association creation is over */
/* Also acts as access control to the bridge module, since no new
 * calls to create_bridge_association() is allowed when non-null */
static void
(*bridge_cb)(struct IP_association *);

/* Used for passing IP Association Set across asynchronous call to callback func */
/* Set .cmd to NULL when not in use */
static struct
{
  ZW_COMMAND_IP_ASSOCIATION_SET *cmd;
  enum ASSOC_TYPE type;
  uint8_t was_dtls;
} virtual_add_callback_state;

extern uint16_t zip_payload_len;

void
do_bridge_callback(struct IP_association *a)
{
  void
  (*temp_cb)(struct IP_association*);
  temp_cb = bridge_cb;
  bridge_cb = NULL;
  if (temp_cb)
    temp_cb(a);
}

static char
ip_assoc_matches(struct IP_association *a, nodeid_t src_id, nodeid_t dst_id, uint8_t endpoint)
{
  if (src_id == a->han_nodeid && dst_id == a->virtual_id && endpoint == a->virtual_endpoint)
  {
    return TRUE;
  }
  return FALSE;
}

/* Inspect a Z-Wave frame payload and extract the destination multichannel endpoint.
 * Return 0 if the frame is not multichannel encapsultaed.
 * Return 0xFF if the frame bit-addresses several endpoints.
 *
 * Known bugs:
 * Returns 0 if fail if there are other encapsulations outside the multichannel
 * (e.g. multi command class).
 *
 * TODO: Unpack other encapsulations while searching for multichannel.
 */
uint8_t
get_multichannel_dest_endpoint(unsigned char *p)
{
  ZW_MULTI_CHANNEL_CMD_ENCAP_V2_FRAME *pCmd = (void*) p;
  if (pCmd->cmdClass == COMMAND_CLASS_MULTI_CHANNEL_V2 && pCmd->cmd == MULTI_CHANNEL_CMD_ENCAP_V2)
  {
    if (pCmd->properties2 & MULTI_CHANNEL_CMD_ENCAP_PROPERTIES2_BIT_ADDRESS_BIT_MASK_V2)
    {
      return 0xFF;
    }
    return pCmd->properties2 & MULTI_CHANNEL_CMD_ENCAP_PROPERTIES2_DESTINATION_END_POINT_MASK_V2;
  }
  return 0;
}

BOOL
bridge_virtual_node_commandhandler(ts_param_t* p, BYTE *__pCmd, BYTE cmdLength)
{
  ZW_APPLICATION_TX_BUFFER* pCmd = (ZW_APPLICATION_TX_BUFFER*) __pCmd;

#if 0
  zwave_connection_t c;
  memset(&c, 0, sizeof(c));

  ipOfNode(&c.ripaddr,p->snode);
  ipOfNode(&c.lipaddr,p->dnode);

  c.scheme = p->scheme;
  c.rendpoint = p->sendpoint;
  c.lendpoint = p->dendpoint;
#endif

  switch (pCmd->ZW_Common.cmdClass)
  {
#if 0
  case COMMAND_CLASS_MULTI_CHANNEL_V3:
  switch (pCmd->ZW_Common.cmd)
  {
    case MULTI_CHANNEL_END_POINT_GET_V3:
    txBuf.ZW_MultiChannelEndPointReportV3Frame.cmdClass =
    COMMAND_CLASS_MULTI_CHANNEL_V3;
    txBuf.ZW_MultiChannelEndPointReportV3Frame.cmd =
    MULTI_CHANNEL_END_POINT_REPORT_V3;
    txBuf.ZW_MultiChannelEndPointReportV3Frame.properties1 = 0x40;
    txBuf.ZW_MultiChannelEndPointReportV3Frame.properties2 = 1;
    ZW_SendDataZIP(&c, (BYTE*) &txBuf,
        sizeof(ZW_MULTI_CHANNEL_END_POINT_REPORT_V3_FRAME), NULL);
    return TRUE;
    break;
    case MULTI_CHANNEL_CAPABILITY_GET_V3:
    if (pCmd->ZW_MultiChannelCapabilityGetV3Frame.properties1 == 1)
    {
      txBuf.ZW_MultiChannelCapabilityReport1byteV2Frame.cmdClass =
      COMMAND_CLASS_MULTI_CHANNEL_V3;
      txBuf.ZW_MultiChannelCapabilityReport1byteV2Frame.cmd =
      MULTI_CHANNEL_CAPABILITY_REPORT_V3;
      txBuf.ZW_MultiChannelCapabilityReport1byteV2Frame.properties1 = 0x1;
      txBuf.ZW_MultiChannelCapabilityReport1byteV2Frame.genericDeviceClass =
      GENERIC_TYPE_GENERIC_CONTROLLER;
      txBuf.ZW_MultiChannelCapabilityReport1byteV2Frame.specificDeviceClass =
      SPECIFIC_TYPE_GATEWAY;
      ZW_SendDataZIP(&c, (BYTE*) &txBuf,
          sizeof(ZW_MULTI_CHANNEL_CAPABILITY_REPORT_1BYTE_V2_FRAME) - 1,
          NULL);
      return TRUE;
    }
    break;
    case MULTI_CHANNEL_END_POINT_FIND_V3:
    txBuf.ZW_MultiChannelEndPointFindReport1byteV2Frame.cmdClass =
    COMMAND_CLASS_MULTI_CHANNEL_V3;
    txBuf.ZW_MultiChannelEndPointFindReport1byteV2Frame.cmd =
    MULTI_CHANNEL_END_POINT_FIND_REPORT_V3;
    txBuf.ZW_MultiChannelEndPointFindReport1byteV2Frame.reportsToFollow =
    0;
    txBuf.ZW_MultiChannelEndPointFindReport1byteV2Frame.genericDeviceClass =
    pCmd->ZW_MultiChannelEndPointFindV3Frame.genericDeviceClass;
    txBuf.ZW_MultiChannelEndPointFindReport1byteV2Frame.specificDeviceClass =
    pCmd->ZW_MultiChannelEndPointFindV3Frame.specificDeviceClass;

    if ((pCmd->ZW_MultiChannelEndPointFindV3Frame.genericDeviceClass
            == GENERIC_TYPE_GENERIC_CONTROLLER
            && pCmd->ZW_MultiChannelEndPointFindV3Frame.specificDeviceClass
            == SPECIFIC_TYPE_GATEWAY)
        || (pCmd->ZW_MultiChannelEndPointFindV3Frame.genericDeviceClass
            == 0xFF
            && pCmd->ZW_MultiChannelEndPointFindV3Frame.specificDeviceClass
            == 0xFF))
    {
      txBuf.ZW_MultiChannelEndPointFindReport1byteV2Frame.variantgroup1.properties1 =
      1;
      ZW_SendDataZIP(&c, (BYTE*) &txBuf,
          sizeof(ZW_MULTI_CHANNEL_END_POINT_FIND_REPORT_1BYTE_V2_FRAME),
          NULL);
    }
    else
    {
      ZW_SendDataZIP(&c, (BYTE*) &txBuf,
          sizeof(ZW_MULTI_CHANNEL_END_POINT_FIND_REPORT_1BYTE_V2_FRAME)
          - 1, NULL);
    }
    break;
  }
  break;
#endif
  default:
    CreateLogicalUDP(p, __pCmd, cmdLength);
    break;
  }

  return TRUE;
}

struct IP_association*
get_ip_assoc_by_nodes(u8_t snode, u8_t dnode, u8_t endpoint)
{
  struct IP_association* a;
  for (a = list_head(association_table); a != NULL; a = list_item_next(a))
  {
    if (ip_assoc_matches(a, snode, dnode, endpoint))
    {
      return a;
    }
  }
  return NULL;
}

struct IP_association*
get_ip_assoc_by_virtual_node(u8_t virtnode)
{
  struct IP_association* a;
  for (a = list_head(association_table); a != NULL; a = list_item_next(a))
  {
    if (a->virtual_id == virtnode)
    {
      return a;
    }
  }
  return NULL;
}

/*
 * We have created a new virtual node. Proceed to create a permanent or
 * proxy IP association on it.
 */
static void
virtual_add_callback(uint8_t bStatus, uint8_t orgID, uint8_t newID)
{
  struct IP_association *new_assoc;
  ASSERT(NULL != virtual_add_callback_state.cmd);
  ZW_COMMAND_IP_ASSOCIATION_SET *set_cmd = virtual_add_callback_state.cmd;
  virtual_add_callback_state.cmd = NULL;

  DBG_PRINTF("virtual_add_callback bStatus=%d\n", bStatus);
  switch (bStatus)
  {
  case ASSIGN_NODEID_DONE:
    if (set_cmd == NULL)
    {
      ASSERT(0);
      return;
    }
    virtual_nodeid = newID;
    if (!multichannel_permanent_nodeid)
    {
      multichannel_permanent_nodeid = newID;
    }
    DBG_PRINTF("Succesfully preallocated a virtual node %d for permanent association\n", newID);
    BIT8_SET(newID - 1, virtual_nodes_mask);
    /* We have a virtual node, process an IP Association Set from txQueue
     * Note: This does not have to be the same IP Assoc Set that triggered virtual node
     * creation. */
    new_assoc = (struct IP_association*) memb_alloc(&association_pool);
    if (new_assoc)
    {
      memset(new_assoc, 0, sizeof(struct IP_association));
      new_assoc->type = virtual_add_callback_state.type;
      new_assoc->was_dtls = virtual_add_callback_state.was_dtls;
      virtual_node_allocated(new_assoc, set_cmd);
    }
    else
    {
      do_bridge_callback(NULL);
    }
    break;

  default:
  case ASSIGN_COMPLETE:
    /* It looks like the SerialAPI target emits this callback when virtual add fails
     * with no free nodeids. */
    /* Signal to txQueue module that we have permanently failed
     * to process this IP Association Set packet */
    do_bridge_callback(NULL);
    break;
  }
}

static void
virtual_add_timeout(void *user)
{

  if (temp_assoc_virtual_nodeid_count < PREALLOCATED_VIRTUAL_NODE_COUNT)
  {
    if (virtual_add_retry_count < VIRTUAL_ADD_RETRY_MAX)
    {
      WRN_PRINTF("Creating virtual node\n");
      virtual_add_retry_count++;
      if (!ZW_SetSlaveLearnMode(0, VIRTUAL_SLAVE_LEARN_MODE_ADD, temporary_virtual_add_callback))
      {
        WRN_PRINTF("create failed, will retry\n");
        ctimer_set(&virtual_add_timer, VIRTUAL_ADD_RETRY_TIMEOUT, virtual_add_timeout, 0);
      }
    }
    else
    {
      WRN_PRINTF("Giving up retrying to create virtual node");
      bridge_state = initfail;
      process_post(&zip_process, ZIP_EVENT_BRIDGE_INITIALIZED, 0);
    }
  }
  else
  {
    bridge_state = initialized;
    process_post(&zip_process, ZIP_EVENT_BRIDGE_INITIALIZED, 0);
  }
}

void
persist_temp_virtual_nodes()
{
  DS_EEPROM_SAVE(temp_assoc_virtual_nodeid_count, temp_assoc_virtual_nodeid_count);
  DS_EEPROM_SAVE(temp_assoc_virtual_nodeids, temp_assoc_virtual_nodeids);
}

/* Save association table to persistent storage */
void
persist_association_table()
{
  struct IP_association *a;
  u32_t offset = offsetof(rd_eeprom_static_hdr_t, association_table);
  uint16_t len = 0;
  for (a = list_head(association_table); a != NULL; a = list_item_next(a))
  {
    if (a->type != temporary_assoc)
    {
        rd_eeprom_write(offset, sizeof(*a), a);
        LOG_PRINTF("Persisting association at address 0x%x, length %u\n", offset + 0x40, (u32_t )sizeof(*a));
        offset += sizeof(*a);
        len++;
    }
  }
  rd_eeprom_write(offsetof(rd_eeprom_static_hdr_t, association_table_length), sizeof(len), (unsigned char*) &len);
  persist_temp_virtual_nodes();
}

/*
 * Callback function from ZW_SetSlaveLearnMode(VIRTUAL_SLAVE_LEARN_MODE_ADD)
 * Called during initialization when creating virtual nodes for temporary
 * IP associations.
 */
static void
temporary_virtual_add_callback(uint8_t bStatus, uint8_t orgID, uint8_t newID)
{
  switch (bStatus)
  {
  case ASSIGN_NODEID_DONE:
    /* Dont double-register if someone already invoked register_virtual_nodes() */
    if (!is_virtual_node(newID))
    {
      temp_assoc_virtual_nodeids[temp_assoc_virtual_nodeid_count] = newID;
      temp_assoc_virtual_nodeid_count++;
      BIT8_SET(newID - 1, virtual_nodes_mask);
      persist_association_table();
    }
    
    {
      virtual_add_retry_count = 0;
      /* FIXME: Only use COOLDOWN_DELAY if an external SIS is present */
      if(ZW_GetSUCNodeID() == MyNodeID) {
        virtual_add_timeout(0);
      } else {
        ctimer_set(&virtual_add_timer, VIRTUAL_ADD_COOLDOWN_DELAY, virtual_add_timeout, 0);
      }
    }
    /*else
    {
      LOG_PRINTF("Succesfully preallocated %d virtual nodes\n", temp_assoc_virtual_nodeid_count);
      process_post(&zip_process, ZIP_EVENT_BRIDGE_INITIALIZED, 0);
      bridge_state = initialized;
    }*/
    break;

  case ASSIGN_COMPLETE:
    /* This can happen if the SIS does not respond to the Reserve NodeID frame, see TO#3596 */
    /* Abort without creating any more temporary virtual nodeids */
    /* TODO: Instead of aborting here, we could choose to revert to the non-bridge library
     * session handling system.*/
    ctimer_set(&virtual_add_timer, VIRTUAL_ADD_RETRY_TIMEOUT, virtual_add_timeout, 0);
    break;

  case ASSIGN_RANGE_INFO_UPDATE:
  default:
    ASSERT(0); /* Should never receive these callback when using VIRTUAL_SLAVE_LEARN_MODE_ADD */
    break;
  }
}

/* Find and register first N virtual nodes for temporary association use */
void
register_virtual_nodes()
{
  ZW_GetVirtualNodes(virtual_nodes_mask);
  /* TODO: Delete all remaining virtual nodes that are not used in association_table */
}

uint8_t
update_temp_assoc_count()
{
  struct IP_association *a;

  temporary_association_count = 0;
  for (a = list_head(association_table); a != NULL; a = list_item_next(a))
  {
    if (a->type == temporary_assoc)
    {
      temporary_association_count++;
    }
  }
  return temporary_association_count;
}

/* Determine if assoc is a duplicate of an item already in association_table */
static uint8_t
is_duplicate(struct IP_association *assoc)
{
  struct IP_association *a;
  void *org_next = assoc->next;

  for (a = list_head(association_table); a != NULL; a = list_item_next(a))
  {
    assoc->next = a->next; /* dont compare the next field */
    if (0 == memcmp(&assoc->virtual_id, &a->virtual_id, sizeof(struct IP_association) - sizeof(void*)))
    {
      break;
    }
  }
  assoc->next = org_next;
  return a != NULL;
}

static void
association_add(struct IP_association *a)
{
  /* Check for duplicates before adding. No need to release virtual nodeids, only
   * local associations (which dont use virtual nodes) and temp associations
   * (which recycle the same virtual nodes) are stopped here.
   * Permanent associations already have their own virtual id at this point, so
   * they would not be flagged as duplicates.
   * */
  if (is_duplicate(a))
  {
    memb_free(&association_pool, a);
  }
  else
  {
    list_add(association_table, a);
  }
}

/* Load association table from persistent storage */
void
unpersist_association_table()
{
  uint16_t len;
  int addr;
  int i;
  ASSERT(*association_table == NULL); /* Make sure we dont leak memory */
  rd_eeprom_read(offsetof(rd_eeprom_static_hdr_t, association_table_length), sizeof(uint16_t), &len);
  //eeprom_read(EEOFSET_ASSOCIATION_TABLE_START, (unsigned char*)&len, sizeof(len));
  for (i = 0; i < len; i++)
  {
    struct IP_association* a = (struct IP_association*) memb_alloc(&association_pool);
    if (!a)
    {
      ERR_PRINTF("Out of memory during unpersist of association table\n");
      return;
    }
    //addr = EEOFSET_ASSOCIATION_TABLE_START + sizeof(len) + i * sizeof(struct IP_association);
    addr = offsetof(rd_eeprom_static_hdr_t, association_table) + i * sizeof(struct IP_association);
    LOG_PRINTF("Unpersisting association %u at address 0x%x, length %u\n", i, addr + 0x40,
        (u32_t )sizeof(struct IP_association));
    rd_eeprom_read(addr, sizeof(struct IP_association), a);
    //ASSERT(a->type < ASSOC_TYPE_COUNT);
    association_add(a);
  }
  update_temp_assoc_count();
  DS_EEPROM_LOAD(temp_assoc_virtual_nodeid_count, temp_assoc_virtual_nodeid_count);
  if (temp_assoc_virtual_nodeid_count > PREALLOCATED_VIRTUAL_NODE_COUNT)
  {
    temp_assoc_virtual_nodeid_count = PREALLOCATED_VIRTUAL_NODE_COUNT;
  }
  DS_EEPROM_LOAD(temp_assoc_virtual_nodeids, temp_assoc_virtual_nodeids);
  printf("temp_assoc_virtual_nodeids (%u): %u %u %u ... %u\n", temp_assoc_virtual_nodeid_count,
      temp_assoc_virtual_nodeids[0], temp_assoc_virtual_nodeids[1], temp_assoc_virtual_nodeids[2],
      temp_assoc_virtual_nodeids[PREALLOCATED_VIRTUAL_NODE_COUNT - 1]);
}

static struct ctimer delay_timeout_ctimer;

void
bridge_init()
{
  uint8_t cap;
  /*TODO*/
  const APPL_NODE_TYPE virtual_node_type =
    { GENERIC_TYPE_REPEATER_SLAVE, SPECIFIC_TYPE_VIRTUAL_NODE };
  const uint8_t virtual_cmd_classes[] =
    { COMMAND_CLASS_SECURITY,COMMAND_CLASS_SECURITY_2 };
  const uint8_t virtual_nop_classes[] =
    { COMMAND_CLASS_NO_OPERATION };

  list_init(association_table);
  memb_init(&association_pool);
  memset(temp_assoc_virtual_nodeids, 0, sizeof(temp_assoc_virtual_nodeids));
  bridge_state = booting;
  virtual_nodeid = 0;
  multichannel_permanent_nodeid = 0;
  temporary_association_count = 0;
  bridge_cb = NULL;
  virtual_add_retry_count = 0;
  virtual_add_callback_state.cmd = NULL;

  if (get_net_scheme() > 0)
  {
    SerialAPI_ApplicationSlaveNodeInformation(0, 1, virtual_node_type, (uint8_t *) virtual_cmd_classes,
        sizeof(virtual_cmd_classes));
  }
  else
  {
    SerialAPI_ApplicationSlaveNodeInformation(0, 1, virtual_node_type, (uint8_t *) virtual_nop_classes,
        sizeof(virtual_nop_classes));
  }

  unpersist_association_table();
  register_virtual_nodes();
  LOG_PRINTF("virtual_nodeid_init_count: %d\n", temp_assoc_virtual_nodeid_count);

  cap = ZW_GetControllerCapabilities();

  if ((cap & CONTROLLER_NODEID_SERVER_PRESENT) || (cap & CONTROLLER_IS_SECONDARY) == 0)
  {
    // Doing add node stop before virtual node creation
    // to avoid race condition after setdefault.
    // Otherwise we would enable Smart Start lean mode before creating
    // virtual nodes, and that would disrupt the virtual node creation.
    ZW_AddNodeToNetwork(ADD_NODE_STOP, NULL);
    ctimer_set(&delay_timeout_ctimer, 0, virtual_add_timeout, NULL);
    //virtual_add_timeout(0);
  }
  else
  {
    ERR_PRINTF("Gateway is included into a NON SIS network\n");
    ERR_PRINTF("there will be limited functionality available\n");
    bridge_state = initfail;
    process_post(&zip_process, ZIP_EVENT_BRIDGE_INITIALIZED, 0);
  }
}

int
is_assoc_source_multichannel()
{
  /* inspect UIP_BUF to decide if assoc source is multi-channel capable */
  return BACKUP_ZIP_PKT_BUF->dEndpoint != 0;
}

struct IP_association*
get_oldest_temporary_association()
{
  struct IP_association *a;

  /* create a new assoc, if we have fewer than expected */
  if (update_temp_assoc_count() < PREALLOCATED_VIRTUAL_NODE_COUNT)
  {
    a = (struct IP_association*) memb_alloc(&association_pool);
    a->type = temporary_assoc;
    return a;
  }

  for (a = list_head(association_table); a != NULL; a = list_item_next(a))
  {
    if (a->type == temporary_assoc)
    {
      list_remove(association_table, a);
      return a;
    }
  }

  return NULL; /* FIXME: Alloc a new temp_assoc if none is found */
}

static struct IP_association*
find_existing_assoc(enum ASSOC_TYPE type,uint8_t was_dtls)
{
  struct IP_association *a;
  uint8_t destination_node ;


  destination_node = nodeOfIP(&(BACKUP_UIP_IP_BUF->destipaddr));

  for (a = list_head(association_table); a != NULL; a = list_item_next(a))
  {
    if(
        a->type == type &&
        destination_node == a->han_nodeid &&
        uip_ipaddr_cmp(&a->resource_ip,&(BACKUP_UIP_IP_BUF->srcipaddr)) &&
        a->resource_port == BACKUP_UIP_UDP_BUF->srcport &&
        a->resource_endpoint == BACKUP_ZIP_PKT_BUF->sEndpoint &&
        a->was_dtls == was_dtls
    ) {
      return a;
    }
  }
  return NULL;
}


/* Choose an nodeid/endpoint tupple for new bridge association
 * Take input from a and store result in a.
 * Return a->virtual_id = 0 to signal error.
 */
void
next_temp_virtual_nodeid_and_endpoint(struct IP_association* a)
{
  static int next_temp_id = 0;

  if (temp_assoc_virtual_nodeid_count == 0)
  {
    a->virtual_id = 0;
    a->virtual_endpoint = 0;
    return;
  }
  /* Idea: Reuse virtual nodeids for _temporary associations_
   * by using different endpoints */

  if (next_temp_id >= temp_assoc_virtual_nodeid_count)
    next_temp_id = 0;

  a->virtual_endpoint = 0;
  a->virtual_id = temp_assoc_virtual_nodeids[next_temp_id++];

}

/*
 * Create IP association of local type to use as backup
 * for replacing failed node, and for storing assoc
 * source/destination during async setup of return
 * routes.
 * Must be called synchronously while the IP Association Set
 * command is still in uip_buf.
 */
struct IP_association*
create_local_association(ZW_COMMAND_IP_ASSOCIATION_SET *cmd)
{
  struct IP_association *new_assoc = (struct IP_association*) memb_alloc(&association_pool);
  if (!new_assoc)
  {
    DBG_PRINTF("failed to alloc new struct IP_association\n");
    return NULL;
  }
  ASSERT(is_local_address(&cmd->resourceIP));
  ASSERT(is_local_address(&BACKUP_UIP_IP_BUF->destipaddr));
  new_assoc->type = local_assoc;
  new_assoc->resource_ip = cmd->resourceIP;
  new_assoc->resource_port = UIP_HTONS(DTLS_PORT);
  new_assoc->resource_endpoint = cmd->endpoint;
  new_assoc->grouping = cmd->groupingIdentifier;
  new_assoc->han_nodeid = nodeOfIP(&BACKUP_UIP_IP_BUF->destipaddr);
  new_assoc->han_endpoint = BACKUP_ZIP_PKT_BUF->dEndpoint;

  association_add(new_assoc);
  persist_association_table();

  return new_assoc;
}

int
check_temp_assoc_count()
{
  return list_length(association_table) >= temporary_association_count;
}

static char*
assoc2str(const enum ASSOC_TYPE type)
{
  switch (type)
  {
  case permanent_assoc:
    return "permanent";
    break;

  case temporary_assoc:
    return "temporary";
    break;

  case proxy_assoc:
    return "proxy";
    break;

  case local_assoc:
    return "local";
    break;
  }
  return "ERROR unknown assoc type";
}

return_status_t /* RET: Status code BRIDGE_OK or BRIDGE_FAIL */
create_bridge_association(enum ASSOC_TYPE type, void
(*cb_func)(struct IP_association *), ZW_COMMAND_IP_ASSOCIATION_SET *ip_assoc_set_cmd, uint8_t was_dtls)
{
  if (type != temporary_assoc && ip_assoc_set_cmd == NULL)
  {
    /* ip_assoc_set_cmd must be nonnull, except for temporary associations */
    return BRIDGE_FAIL;
  }
  struct IP_association* new_assoc = NULL;

  if (bridge_cb)
  {
    /* Its an error to get here while a bridge session is already ongoing*/
    ASSERT(0);
    return BRIDGE_FAIL;
  }
  bridge_cb = cb_func;

  /* TODO: Check if we can reuse an existing association */
  new_assoc = find_existing_assoc(type, was_dtls);
  if (new_assoc)
  {
    new_assoc->han_endpoint = new_assoc->han_endpoint = BACKUP_ZIP_PKT_BUF->dEndpoint;
    do_bridge_callback(new_assoc);
    return BRIDGE_OK;
  }

  if (type == local_assoc)
  {
    /* Unsupported type, call create_local_association() instead */
    ASSERT(0);
    return BRIDGE_FAIL;
  }

  DBG_PRINTF("create_bridge_association of %s type\n", assoc2str(type));

  if (type == temporary_assoc)
  {
    if ((new_assoc = get_oldest_temporary_association()) == NULL)
    {
      ERR_PRINTF("failed to alloc or recycle a struct IP_assoc\n");
      /* Can happen if a non-responding SIS causes virtual node creation to fail */
      do_bridge_callback(NULL);
      return BRIDGE_FAIL;
    }
    DBG_PRINTF("Re-using oldest temporary association with virtual nodeid %d\n", new_assoc->virtual_id);
  }
  else if (type == permanent_assoc || type == proxy_assoc)
  {
    if (is_assoc_source_multichannel())
    {
      DBG_PRINTF("multichannel assoc source\n");
      /*if (multichannel_permanent_nodeid != 0)
       {
       new_assoc = (struct IP_association*)memb_alloc(&association_pool);
       }*/

      /* TODO: Check if there are unused endpoints on previously allocated virtual nodeids
       * before we allocate a new virtual node.
       * This could happen when the endpoints are fragmented across multiple virtual nodeids
       * due to mixed single and multichannel association sources.
       */
    }
    DBG_PRINTF("creating a new virtual node\n");
    /* TODO: Add a guard timer in case this callback is lost */
    BYTE retVal;
    ASSERT(virtual_add_callback_state.cmd == NULL);
    virtual_add_callback_state.cmd = ip_assoc_set_cmd;
    virtual_add_callback_state.type = type;
    virtual_add_callback_state.was_dtls = was_dtls;
    retVal = ZW_SetSlaveLearnMode(0, VIRTUAL_SLAVE_LEARN_MODE_ADD, virtual_add_callback);
    if (!retVal)
    {
      virtual_add_callback_state.cmd = NULL;
      DBG_PRINTF("ZW_SetSlaveLearnMode() returned false\n");
      do_bridge_callback(NULL);
      return BRIDGE_FAIL;
    }
    return BRIDGE_OK;
  }
  else
  {
    ERR_PRINTF("Unknown IP association type.\n");
    return BRIDGE_FAIL;
  }
  //ASSERT(check_temp_assoc_count());
  memset(new_assoc, 0, sizeof(struct IP_association));
  new_assoc->type = type;
  new_assoc->was_dtls = was_dtls;
  virtual_node_allocated(new_assoc, ip_assoc_set_cmd);
  return BRIDGE_OK;
}

/*
 *  Finish setup of an IP association.
 *
 *  Preconditions:
 *   - an IP_association memblock has been allocated and zeroed.
 *   - new_assoc->type has been set.
 *   - (for permanent assoc) a virtual node has been allocated and the node id
 *     is stored in unused_virtual_nodeid.
 *
 */
static void
virtual_node_allocated(struct IP_association *new_assoc, ZW_COMMAND_IP_ASSOCIATION_SET *ip_assoc_set_cmd)
{
  //DBG_PRINTF("virtual node allocated, filling out struct IP_association\n");

  new_assoc->han_nodeid = nodeOfIP(&(BACKUP_UIP_IP_BUF->destipaddr));
  ASSERT(new_assoc->han_nodeid != 0);
  new_assoc->han_endpoint = BACKUP_ZIP_PKT_BUF->dEndpoint;

  if (new_assoc->type == permanent_assoc || new_assoc->type == proxy_assoc)
  {
    if (zip_payload_len < IP_ASSOC_SET_FIXED_SIZE)
    {
      ERR_PRINTF("IP Association message invalid: too short\n");
      memb_free(&association_pool, new_assoc);
      do_bridge_callback(NULL);
      return;
    }
    new_assoc->grouping = ip_assoc_set_cmd->groupingIdentifier;
    uip_ipaddr_copy(&(new_assoc->resource_ip), (uip_ipaddr_t* )&(ip_assoc_set_cmd->resourceIP));
    new_assoc->resource_endpoint = ip_assoc_set_cmd->endpoint;
    /* resource_port controls if forwardings via this association are
     * DTLS encrypted. Use DTLS_PORT for resources on the LAN and
     * ZWAVE_PORT for resources in the PAN. */
    if (new_assoc->type == proxy_assoc)
    {
      new_assoc->resource_port = UIP_HTONS(ZWAVE_PORT);
    }
    else
    {
      new_assoc->resource_port = UIP_HTONS(DTLS_PORT);
    }
    if (is_assoc_source_multichannel())
    {
      /* We can multiplex several associations on the same virtual nodeid
       * and distinguish them by the endpoint on the virtual nodeid. */
      new_assoc->virtual_id = virtual_nodeid;
    }
    else
    {
      ASSERT(virtual_nodeid);
      new_assoc->virtual_id = virtual_nodeid;
    }
    rd_register_new_node(new_assoc->virtual_id, 0x00);
  }
  else if (new_assoc->type == temporary_assoc)
  {
    new_assoc->resource_port = BACKUP_UIP_UDP_BUF->srcport;
    new_assoc->resource_endpoint = BACKUP_ZIP_PKT_BUF->sEndpoint;
    uip_ipaddr_copy(&(new_assoc->resource_ip), &(BACKUP_UIP_IP_BUF->srcipaddr));
    //ERR_PRINTF("port: %d, cfg_port: %d\n", UIP_HTONS(BACKUP_UIP_UDP_BUF->srcport), cfg.unsolicited_port);
    if(uip_ipaddr_cmp(&cfg.unsolicited_dest,&(BACKUP_UIP_IP_BUF->srcipaddr)) && (UIP_HTONS(BACKUP_UIP_UDP_BUF->srcport) == cfg.unsolicited_port))
    {
        DBG_PRINTF("This packet was received from Unsolicited destination. Source node will be GW's Node id \n");
        new_assoc->virtual_id = MyNodeID;
    } else {
        next_temp_virtual_nodeid_and_endpoint(new_assoc);
    }
    if (new_assoc->virtual_id == 0)
    {
      /* could not get virtual nodeid, abort */
      memb_free(&association_pool, new_assoc);
      do_bridge_callback(NULL);
      return;
    }
  }

  association_add(new_assoc);
  persist_association_table();

  do_bridge_callback(new_assoc);
}

static void
virtual_remove_callback(uint8_t bStatus, uint8_t orgID, uint8_t newID)
{
  decrememnt_bulk_ipa_remove();

  if (bulk_ipa_remove_val())
  {
//    ERR_PRINTF("------Calling remove_from_bulk_list\n");
    remove_from_bulk_list();
  }
  else
  {
    send_zipack_ipassoc(TRANSMIT_COMPLETE_OK);
    reset_ip_association_state(TRANSMIT_COMPLETE_OK);
    process_post(&zip_process, ZIP_EVENT_NM_VIRT_NODE_REMOVE_DONE, 0);
  }

}

void
remove_bridge_association(struct IP_association* a)
{
  BYTE retVal;

  if (a)
  {
    rd_remove_node(a->virtual_id);
    list_remove(association_table, a);
    if (a->type == permanent_assoc)
    {
      DBG_PRINTF("Removing a permanent virtual node: %d\n", a->virtual_id);
      retVal = ZW_SetSlaveLearnMode(a->virtual_id, VIRTUAL_SLAVE_LEARN_MODE_REMOVE, virtual_remove_callback);
      if (!retVal)
      {
        DBG_PRINTF("ZW_SetSlaveLearnMode() returned false\n");
        virtual_remove_callback(FALSE, 0, 0);
      }
    }
    else
    {
      virtual_remove_callback(TRUE, 0, 0);
    }
    /* TODO: Use refcount to allow using the same struct IP_association for several sessions */
    memb_free(&association_pool, a);
  }
}

void
remove_bridge_association_and_persist(struct IP_association *a)
{
  remove_bridge_association(a);
  persist_association_table();
}

void
list_bridge_associations()
{
  const char assoc_types[] =
    { 'T', 'P', 'L', 'O' };
  struct IP_association *s;
  for (s = list_head(association_table); s != NULL; s = list_item_next(s))
  {
    LOG_PRINTF("LAN assoc dest endpoint: %d\t %d  ", s->resource_endpoint, uip_ntohs(s->resource_port));
    uip_debug_ipaddr_print(&s->resource_ip);

    LOG_PRINTF("Virt-%d:%d \t HAN-(assoc source node_id: %x: assoc source endpoint %d \t %c\n", s->virtual_id,
        s->virtual_endpoint, s->han_nodeid, s->han_endpoint, assoc_types[s->type]);
  }
}

void
print_bridge_status()
{
  uint8_t i, j;
  uint8_t k = 1;

  ZW_GetVirtualNodes(virtual_nodes_mask);

  list_bridge_associations();
  LOG_PRINTF("\n");
#define PRINT_INT(VAR) LOG_PRINTF(#VAR ": %u\n", VAR)
  PRINT_INT(temp_assoc_virtual_nodeid_count);
  PRINT_INT(bridge_state);
  PRINT_INT(virtual_nodeid);
  PRINT_INT(multichannel_permanent_nodeid);
  PRINT_INT(temporary_association_count);
  PRINT_INT(list_length(association_table));
#undef PRINT_INT

  printf("\nTemporary virtual nodes: ");
  for (i = 0; i < temp_assoc_virtual_nodeid_count; i++)
    printf("%d, ", temp_assoc_virtual_nodeids[i]);
  printf("\n");
  printf("virtual_nodes_mask:");
  for (i = 0; i < sizeof(virtual_nodes_mask); i++)
  {
    printf("%x ", virtual_nodes_mask[i]);
  }
  printf("\n");
  printf("Virtual nodes:(in decimal)");
  printf("\n");
  for (i = 0; i < sizeof(virtual_nodes_mask); i++)
  {
    if (virtual_nodes_mask[i] == 0)
      continue;

    for (j = 0; j < 8; j++)
    {
      if (virtual_nodes_mask[i] & (1 << (j)))
        printf("%d ", k);
      k++;
    }
    printf("\n");
  }
  printf("\n\n");
}

char
is_virtual_node(nodeid_t nid)
{
  NODEINFO ni;
  if (BIT8_TST(nid - 1, virtual_nodes_mask))
  {
    return 1;
  }
  return 0;
  /*
   ZW_GetNodeProtocolInfo(nid, &ni);
   if(ni.nodeType.generic == GENERIC_TYPE_REPEATER_SLAVE && ni.nodeType.specific == SPECIFIC_TYPE_VIRTUAL_NODE) {
   return 1;
   } else {
   return 0;
   }
   */
}

void
bridge_clear_association_table()
{ /* FIXME: memb_free all existing struct IP_associations first? */
  uint16_t i = 0;
  rd_eeprom_write(offsetof(rd_eeprom_static_hdr_t, association_table_length), sizeof(i), (void*) &i);
  list_init(association_table);
  temporary_association_count = 0;
}

void
decrememnt_bulk_ipa_remove()
{
  if (bulk_ipa_remove > 0)
  {
    bulk_ipa_remove--;
  }
  return;
}
int
bulk_ipa_remove_val()
{
  return bulk_ipa_remove;
}

void
mark_bulk_removals(nodeid_t assoc_source_id, uint8_t assoc_source_endpoint, uint8_t grouping, uip_ip6addr_t *res_ip,
    uint8_t res_endpoint)
{
  struct IP_association *ipa = list_head(association_table);
  bulk_ipa_remove = 0;
  uip_ip6addr_t zero_ip;
  /* TODO: Check lenght of frame */

  memset(&zero_ip, 0, sizeof(uip_ip6addr_t));

  while (ipa)
  {
    DBG_PRINTF("-------type: 1\n");
    if (ipa->han_nodeid == assoc_source_id
        && ((ipa->han_endpoint == assoc_source_endpoint) || (assoc_source_endpoint == 0))
        && ipa->type != temporary_assoc && ((ipa->grouping == grouping) || (grouping == 0))
        && ((((res_ip == 0) || uip_ipaddr_cmp(res_ip, &zero_ip))) || (uip_ipaddr_cmp(res_ip, &ipa->resource_ip))))
    {
      ipa->mark_removal = 1;
      bulk_ipa_remove++;
      DBG_PRINTF("--------- ipa marked for removal node id: %x assoc dest endpoint:%d\n ip:", ipa->han_nodeid,
          ipa->resource_endpoint);
      uip_debug_ipaddr_print(&ipa->resource_ip);
    }
    ipa = ipa->next;
  }
}

/* Return a pointer to IP Association requested by IP Assoc Get,
 * and also return the count of nodes associated in this grouping */
uint8_t
find_ip_assoc_for_report(uint8_t nid, uint8_t han_endpoint, uint8_t grouping, uint8_t index,
    struct IP_association **result)
{
  struct IP_association *a;
  uint8_t nodes_in_grouping = 0;
  *result = NULL;
  for (a = list_head(association_table); a != NULL; a = list_item_next(a))
  {
    if ((nid == a->han_nodeid) && (a->han_endpoint == han_endpoint) && (a->grouping == grouping)
        && (a->type != temporary_assoc))
    {
      nodes_in_grouping++;
      if (--index == 0)
      {
        *result = a;
      }
    }
  }
  return nodes_in_grouping;
}

struct IP_association *
get_ipa_marked_for_removal()
{
  struct IP_association *ipa = list_head(association_table);
  while (ipa)
  {
    if (ipa->mark_removal == 1)
    {
      DBG_PRINTF("--------- get_ipa_marked_for_removal found ipa ipa->resource_endpoint:%d\n ip:",
          ipa->resource_endpoint);
      uip_debug_ipaddr_print(&ipa->resource_ip);
      return ipa;
    }
    ipa = ipa->next;
  }
  return NULL;
}
/* helper functions to inspect association table from debugger */
struct IP_association *
at_head()
{
  return (struct IP_association*) list_head(association_table);
}

struct IP_association *
at_next(struct IP_association *iap)
{
  return (struct IP_association*) iap->next;
}

void
resume_bridge_init()
{
  /* only resume if no retry timer is running */
  if (virtual_add_retry_count >= VIRTUAL_ADD_RETRY_MAX)
  {
    virtual_add_retry_count = 0;
    virtual_add_timeout(NULL);
  }
}

void
bridge_reset()
{
  bridge_clear_association_table();
  temp_assoc_virtual_nodeid_count = 0;
  memset(temp_assoc_virtual_nodeids, 0, sizeof(temp_assoc_virtual_nodeids));
  persist_temp_virtual_nodes();
  bridge_state = booting;
}

