/* © ${year} Sigma Designs, Inc. This is an unpublished work protected by Sigma
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



/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/
#include <ZW_classcmd_ex.h>
#include "test_helpers.h"
#include <CC_provisioning_list.h>
#include <provisioning_list.h>
#include "RD_internal.h"
#include <stdlib.h>
#include <ZW_SendDataAppl.h>

/****************************************************************************/
/*                      PRIVATE TYPES and DEFINITIONS                       */
/****************************************************************************/
#define MOCK_CACHE_DEPTH 10 /* How many calls does the mocks cache. Note: Only some mocks implement this. Others default to 1. */

/* Use real provisioning_list module for tests */
//#define MOCK_PROV_LIST_MODULE

/****************************************************************************/
/*                              EXPORTED DATA                               */
/****************************************************************************/
struct {
  uint8_t buf[2000];
  uint8_t seq;
} nms;

/****************************************************************************/
/*                              PRIVATE DATA                                */
/****************************************************************************/

/****************************************************************************/
/*                              PRIVATE FUNCTIONS                           */
/****************************************************************************/
void NetworkManagement_smart_start_init_if_pending(void)
{
  //stub
}

/**
 * The location may contain . but not start or end with them
 */
u8_t
validate_location(char* name, u8_t len)
{
  if (name[0] == '_')
    return 0;
  if (name[0] == '.')
    return 0;
  if (name[len - 1] == '.')
    return 0;
  return 1;
}

/**
 * Validate a the Name part of the service name
 */
u8_t
validate_name(char* name, u8_t len)
{
  int i;
  if (name[0] == '_')
    return 0;

  for (i = 0; i < len; i++)
  {
    if (name[i] == '.')
      return 0;
  }
  return 1;
}

//#define MOCK_PROV_LIST_MODULE /* Undef to use real PVL module */
#ifdef MOCK_PROV_LIST_MODULE

struct  {
  struct provision *pvs;
  uint8_t type;
  uint8_t len;
  uint8_t *val;
} provisioning_list_tlv_set_args[MOCK_CACHE_DEPTH];
static unsigned int provisioning_list_tlv_set_call_count = 0;

int provisioning_list_tlv_set(struct provision *pvs, uint8_t type, uint8_t len, uint8_t *val)
{
  static uint8_t val_buf[MOCK_CACHE_DEPTH][2000];
  unsigned int idx = provisioning_list_tlv_set_call_count++;
  /* Cache the value of the pointer, so we dont fail when being called with ptr to shortlived stack
   * objects */
  memcpy(val_buf[idx], val, len);

  provisioning_list_tlv_set_args[idx].pvs = pvs;
  provisioning_list_tlv_set_args[idx].type = type;
  provisioning_list_tlv_set_args[idx].len = len;
  provisioning_list_tlv_set_args[idx].val = val_buf[idx];
  return 1;
}

int provisioning_list_clear_called = 0;
void provisioning_list_clear() {
  provisioning_list_clear_called++;
}

struct {
  uint8_t dsk_len;
  uint8_t *dsk;
} provisioning_list_dev_remove_args;

int provisioning_list_dev_remove(uint8_t dsk_len, uint8_t *dsk)
{
  provisioning_list_dev_remove_args.dsk_len = dsk_len;
  provisioning_list_dev_remove_args.dsk = dsk;
  return 1;
}

struct  {
  uint8_t dsk_len;
  uint8_t *dsk;
  provisioning_bootmode_t class;
} provisioning_list_dev_add_args;

struct provision * provisioning_list_dev_add(uint8_t dsk_len, uint8_t *dsk,
                                             provisioning_bootmode_t class)
{
  provisioning_list_dev_add_args.dsk_len = dsk_len;
  provisioning_list_dev_add_args.dsk = dsk;
  provisioning_list_dev_add_args.class = class;

  return (struct provision*)0x42;
}

void provisioning_list_iterator_delete(provisioning_list_iter_t *iter)
{
  UNUSED(iter);
  return;
}

provisioning_list_iter_t * provisioning_list_iterator_get()
{
  return (provisioning_list_iter_t*)0x42;
}

struct provision * provisioning_list_iter_get_next(provisioning_list_iter_t *iter)
{
  UNUSED(iter);
  return (struct provision *)0x42;
}

struct {
  uint8_t dsk_len;
  uint8_t *dsk;
} provisioning_list_dev_get_args;

struct provision * provisioning_list_dev_get(uint8_t dsk_len, uint8_t *dsk)
{
  provisioning_list_dev_get_args.dsk_len = dsk_len;
  provisioning_list_dev_get_args.dsk = dsk;
  return (struct provision*)0x42;
}

struct {
  struct provision *pvs;
  uint8_t type;
} provisioning_list_tlv_get_args;
static unsigned int provisioning_list_tlv_get_call_count = 0;

struct pvs_tlv * provisioning_list_tlv_get(struct provision *pvs, uint8_t type)
{
  provisioning_list_tlv_get_call_count++;
  provisioning_list_tlv_get_args.pvs = pvs;
  provisioning_list_tlv_get_args.type = type;
  return (struct pvs_tlv*)0x42;
}
#endif /* MOCK_PROV_LIST_MODULE */

struct {
 zwave_connection_t *c;
 void *dataptr;
 uint16_t datalen;
 ZW_SendDataAppl_Callback_t cbFunc;
} ZW_SendDataZIP_args;

void
ZW_SendDataZIP(zwave_connection_t *c,  const  void *dataptr, u16_t datalen,
    ZW_SendDataAppl_Callback_t cbFunc)
{
  ZW_SendDataZIP_args.c = c;
  ZW_SendDataZIP_args.dataptr = (void*)dataptr;
  ZW_SendDataZIP_args.datalen = datalen;
  ZW_SendDataZIP_args.cbFunc = cbFunc;
}



//static void _check_true(uint8_t x, char *msg, int lineno)
//{
//  char buf[2000];
//  snprintf(buf, sizeof buf,  "%s%i", msg, lineno);
//  check_true(x, buf);
//}
//#define CT(x) _check_true(x, "test in line: ", __LINE__)
//#define CT2(x, msg) _check_true(x, msg ". Test in line: ", __LINE__)

static void mock_reset(void)
{
#ifdef MOCK_PROV_LIST_MODULE
  provisioning_list_tlv_set_call_count = 0;
  provisioning_list_tlv_get_call_count = 0;
#endif /* MOCK_PROV_LIST_MODULE */
}



/****************************************************************************/
/*                              EXPORTED FUNCTIONS                          */
/****************************************************************************/

zwave_connection_t dummy_connection;

static void test_CC_pvl_basic(void)
{
  command_handler_codes_t ret;
  static uint8_t cmd_pl_iter_get[] = {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_ITER_GET,
      0xFF, 0xFF,
  };
  uint8_t nodeid;
  rd_node_database_entry_t *nde;

  // Iter GET empty list
  ret = PVL_CommandHandler(&dummy_connection, cmd_pl_iter_get, sizeof cmd_pl_iter_get);
  check_true(ret == COMMAND_HANDLED, "command failed cmd_pl_iter_get");

  static uint8_t cmd_pl_iter_rep0[] = {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_ITER_REPORT,
          0xFF, 0x00, // seqno and remaining count
          0x00, // DSK length
    };

  check_true(
      ZW_SendDataZIP_args.datalen == sizeof cmd_pl_iter_rep0  &&
      memcmp(ZW_SendDataZIP_args.dataptr, cmd_pl_iter_rep0, ZW_SendDataZIP_args.datalen) == 0,
      "Iter get response 0 wrong (empty list)");

  // Parse error
  static uint8_t cmd_pl_set_error[] = {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_SET,
    0x03, 0x10,        // Seqno, DSK length
    0x7a, 0x55, 0x49, 0x94, 0x55, 0xae, 0x8f, 0x57, 0x29, 0x69, 0x45, 0x4c, 0x23, 0x3a, 0x72, 0x69,
    100, 0x03, 'a', 'b', 'c',    // Name
    102, 0x08, '1', '2', '3', '1', '2', '3', '1', '2',  // Location
    6, 0x10, 0x01, 0xf0, 0xef, 0xde, 0xcd, 0xbc, 0xab, 0x9a, 0x89, 0x78, 0x67, 0x56, 0x45, 0x34, 0x23, 0x12, // error in uuid
  };
  ret = PVL_CommandHandler(&dummy_connection, cmd_pl_set_error, sizeof cmd_pl_set_error);
  check_true(ret == COMMAND_PARSE_ERROR, "command failed: cmd_pl_set with parse error");

  // SET device 7a55
  static uint8_t cmd_pl_set[] = {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_SET,
    0x03, 0x10,        // Seqno, DSK length
    0x7a, 0x55, 0x49, 0x94, 0x55, 0xae, 0x8f, 0x57, 0x29, 0x69, 0x45, 0x4c, 0x23, 0x3a, 0x72, 0x69,
    100, 0x03, 'a', 'b', 'c',    // Name
    102, 0x08, '1', '2', '3', '1', '2', '3', '1', '2',  // Location
    6, 0x11, 0x01, 0xf0, 0xef, 0xde, 0xcd, 0xbc, 0xab, 0x9a, 0x89, 0x78, 0x67, 0x56, 0x45, 0x34, 0x23, 0x12, 0x01, // uuid
  };

  ret = PVL_CommandHandler(&dummy_connection, cmd_pl_set, sizeof cmd_pl_set);
  check_true(ret == COMMAND_HANDLED, "command failed cmd_pl_set");

  check_true(provisioning_list_get_count() == 1 &&
      provisioning_list_pending_count() == 1,
      "Adding 1 pvl entry failed");


  // Iter GET 1-element list (device 7a55)
  ret = PVL_CommandHandler(&dummy_connection, cmd_pl_iter_get, sizeof cmd_pl_iter_get);
  check_true(ret == COMMAND_HANDLED, "command failed cmd_pl_iter_get");
  static uint8_t cmd_pl_iter_rep1[] = {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_ITER_REPORT,
        0xFF, 0x00, // seqno and remaining count
        0x10, // DSK length
        0x7a, 0x55, 0x49, 0x94, 0x55, 0xae, 0x8f, 0x57, 0x29, 0x69, 0x45, 0x4c, 0x23, 0x3a, 0x72, 0x69,
        PVS_TLV_TYPE_BOOTSTRAP_MODE << 1 | 1, 1, PVS_BOOTMODE_SMART_START,
        PVS_TLV_TYPE_STATUS << 1 | 1, 1, PVS_STATUS_PENDING, //MODE pending
                                       PVS_TLV_TYPE_NETWORK_INFO << 1, 2, 0, PVS_NETSTATUS_NOT_IN_NETWORK,
        PVS_TLV_TYPE_UUID16 << 1, 0x11, 0x01, 0xf0, 0xef, 0xde, 0xcd, 0xbc, 0xab, 0x9a, 0x89, 0x78, 0x67, 0x56, 0x45, 0x34, 0x23, 0x12, 0x01, // uuid
        PVS_TLV_TYPE_LOCATION << 1, 0x08, '1', '2', '3', '1', '2', '3', '1', '2',  // Location
        PVS_TLV_TYPE_NAME << 1, 0x03, 'a', 'b', 'c',    // Name
  };

  test_print(4, "Expect ZW_SendDataZIP_args.datalen(%u) == sizeof cmd_pl_rep1(%u)\n",
             ZW_SendDataZIP_args.datalen, sizeof cmd_pl_iter_rep1);
  uint8_t* data = (uint8_t*) ZW_SendDataZIP_args.dataptr;
  // 0x6c is bootmode
  // 0x68 is status
  // 0x66 is location
  // 0x64 is name
  test_print(4, "dsk 0x%02x%02x boot 0x%02x%02x%02x status 0x%02x%02x%02x loc(66)%02x%02x%02x%02x name(64)0x%02x%02x%02x%02x %02x%02x%02x 0x%02x%02x%02x%02x\n", 
             data[5], data[6],
             data[21], data[22], data[23],
             data[24], data[25], data[26],
             data[27], data[28], data[29],  data[30],data[31],data[32],data[33],  data[34],data[35],data[36],
             data[37], data[38], data[39], data[40], data[41], data[42]);
  check_true(
      (ZW_SendDataZIP_args.datalen == sizeof cmd_pl_iter_rep1) &&
      memcmp(ZW_SendDataZIP_args.dataptr, cmd_pl_iter_rep1, ZW_SendDataZIP_args.datalen) == 0,
      "Iter get response 1 wrong");


  // SET second element, device 1234
  static uint8_t cmd_pl_set2[] = {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_SET,
     0x03, 0x10,
     0x12, 0x34, 0x56, 0x94, 0x55, 0xae, 0x8f, 0x57, 0x29, 0x69, 0x45, 0x4c, 0x23, 0x3a, 0x88, 0x99,
     100, 0x03, 'x', 'y', 'z',
     102, 0x04, '5', '6', ' ', '7',
   };
  ret = PVL_CommandHandler(&dummy_connection, cmd_pl_set2, sizeof cmd_pl_set2);
  check_true(ret == COMMAND_HANDLED, "command failed: cmd_pl_set2");


  // Iter GET 2-element list
  ret = PVL_CommandHandler(&dummy_connection, cmd_pl_iter_get, sizeof cmd_pl_iter_get);
  check_true(ret == COMMAND_HANDLED, "command failed: cmd_pl_iter_get");
  static uint8_t cmd_pl_iter_rep2a[] = {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_ITER_REPORT,
        0xFF, 0x01, // seqno and remaining count
        0x10, // DSK length
        0x7a, 0x55, 0x49, 0x94, 0x55, 0xae, 0x8f, 0x57, 0x29, 0x69, 0x45, 0x4c, 0x23, 0x3a, 0x72, 0x69,
        PVS_TLV_TYPE_BOOTSTRAP_MODE << 1 | 1, 1, PVS_BOOTMODE_SMART_START,
        PVS_TLV_TYPE_STATUS << 1 | 1, 1, PVS_STATUS_PENDING, //MODE pending
                                        PVS_TLV_TYPE_NETWORK_INFO << 1, 2, 0, PVS_NETSTATUS_NOT_IN_NETWORK,
        PVS_TLV_TYPE_UUID16 << 1, 0x11, 0x01, 0xf0, 0xef, 0xde, 0xcd, 0xbc, 0xab, 0x9a, 0x89, 0x78, 0x67, 0x56, 0x45, 0x34, 0x23, 0x12, 0x01, // uuid
        PVS_TLV_TYPE_LOCATION << 1, 0x08, '1', '2', '3', '1', '2', '3', '1', '2',  // Location
        PVS_TLV_TYPE_NAME << 1, 0x03, 'a', 'b', 'c',    // Name
  };
  check_true(
      (ZW_SendDataZIP_args.datalen == sizeof cmd_pl_iter_rep2a) &&
      memcmp(ZW_SendDataZIP_args.dataptr, cmd_pl_iter_rep2a, ZW_SendDataZIP_args.datalen) == 0,
      "Iter get response 2a wrong");

  static uint8_t cmd_pl_iter_get2[] = {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_ITER_GET,
        0x00, 0x01,
  };
  ret = PVL_CommandHandler(&dummy_connection, cmd_pl_iter_get2, sizeof cmd_pl_iter_get);
  check_true(ret == COMMAND_HANDLED, "command failed: cmd_pl_iter_get2");
  static uint8_t cmd_pl_iter_rep2b[] = {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_ITER_REPORT,
        0x00, 0x00, // seqno and remaining count
        0x10, // DSK length
        0x12, 0x34, 0x56, 0x94, 0x55, 0xae, 0x8f, 0x57, 0x29, 0x69, 0x45, 0x4c, 0x23, 0x3a, 0x88, 0x99,
        PVS_TLV_TYPE_BOOTSTRAP_MODE << 1 | 1, 1, PVS_BOOTMODE_SMART_START,
        PVS_TLV_TYPE_STATUS << 1 | 1, 1, PVS_STATUS_PENDING, //MODE pending
                                        PVS_TLV_TYPE_NETWORK_INFO << 1, 2, 0, PVS_NETSTATUS_NOT_IN_NETWORK,
        102, 0x04, '5', '6', ' ', '7',
        100, 0x03, 'x', 'y', 'z',
  };
  check_true(
      (ZW_SendDataZIP_args.datalen == sizeof cmd_pl_iter_rep2b) &&
      memcmp(ZW_SendDataZIP_args.dataptr, cmd_pl_iter_rep2b, ZW_SendDataZIP_args.datalen) == 0,
      "Iter get response 2b wrong");


  // DELETE device 7a55
  static uint8_t cmd_pl_delete[] = {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_DELETE,
     0x00, 0x10,
     0x7a, 0x55, 0x49, 0x94, 0x55, 0xae, 0x8f, 0x57, 0x29, 0x69, 0x45, 0x4c, 0x23, 0x3a, 0x72, 0x69,
  };
  ret = PVL_CommandHandler(&dummy_connection, cmd_pl_delete, sizeof cmd_pl_delete);
  check_true(ret == COMMAND_HANDLED, "command failed: cmd_pl_delete");

  check_true(provisioning_list_get_count() == 1 &&
        provisioning_list_pending_count() == 1,
        "Del 1 pvl entry failed");

  // Now change an entry status to DISABLED on device 1234
  struct provision *pvs_to_change;
  uint8_t dsk_to_change[] = { 0x12, 0x34, 0x56, 0x94, 0x55, 0xae, 0x8f, 0x57, 0x29, 0x69, 0x45, 0x4c, 0x23, 0x3a, 0x88, 0x99, };
  check_not_null(pvs_to_change = provisioning_list_dev_get(16, dsk_to_change),
                 "DSK-to-change not found");
  if((pvs_to_change) != NULL) {
      check_true(pvs_to_change->status == PVS_STATUS_PENDING, "Initial status must be pending");
      provisioning_list_status_set(pvs_to_change, PVS_STATUS_DISABLED);
  }

  // Iter GET remaining list - this will also update DISABLED to PENDING
  ret = PVL_CommandHandler(&dummy_connection, cmd_pl_iter_get, sizeof cmd_pl_iter_get);
  check_true(ret == COMMAND_HANDLED, "command failed: cmd_pl_iter_get after delete");

  static uint8_t cmd_pl_iter_rep3[] = {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_ITER_REPORT,
        0xFF, 0x00, // seqno and remaining count
        0x10, // DSK length
        0x12, 0x34, 0x56, 0x94, 0x55, 0xae, 0x8f, 0x57, 0x29, 0x69, 0x45, 0x4c, 0x23, 0x3a, 0x88, 0x99,
        PVS_TLV_TYPE_BOOTSTRAP_MODE << 1 | 1, 1, PVS_BOOTMODE_SMART_START,
        PVS_TLV_TYPE_STATUS << 1 | 1, 1, PVS_STATUS_PENDING, //MODE is PENDING again
                                       PVS_TLV_TYPE_NETWORK_INFO << 1, 2, 0, PVS_NETSTATUS_NOT_IN_NETWORK,
        102, 0x04, '5', '6', ' ', '7',
        100, 0x03, 'x', 'y', 'z',
  };

  check_true(
      (ZW_SendDataZIP_args.datalen == sizeof cmd_pl_iter_rep3) &&
      memcmp(ZW_SendDataZIP_args.dataptr, cmd_pl_iter_rep3, ZW_SendDataZIP_args.datalen) == 0,
      "Iter get response 3 after delete - only one device (1234), with DISABLED updated to PENDING");

  ret = PVL_CommandHandler(&dummy_connection, cmd_pl_iter_get2, sizeof cmd_pl_iter_get2);
  check_true(ret == COMMAND_PARSE_ERROR, "command failed: extra cmd_pl_iter_get");

  // Now change an entry status to ignored
  nodeid = 8;
  nde = rd_node_entry_alloc(nodeid);
  memset(nde,0,sizeof(rd_node_database_entry_t)),
  nde->nodeid = nodeid;
  nde->nodeNameLen = 0;
  nde->nodename = 0;
  nde->state = STATUS_CREATED;
  nde->mode = MODE_PROBING;
  nde->security_flags = 0;
  LIST_STRUCT_INIT(nde, endpoints);
  rd_node_add_dsk(nodeid, 16, dsk_to_change);

  check_not_null(pvs_to_change = provisioning_list_dev_get(16, dsk_to_change),
                 "Looking up DSK-to-change");
  if((pvs_to_change) != NULL) {
     /* Restriction removed */
     /* check_true(pvs_to_change->status == PVS_STATUS_PENDING, "Status must be pending"); */
     provisioning_list_status_set(pvs_to_change, PVS_STATUS_IGNORED);
  }

  // Iter GET remaining list - this will NOT change IGNORED to PENDING
  ret = PVL_CommandHandler(&dummy_connection, cmd_pl_iter_get, sizeof cmd_pl_iter_get);
  check_true(ret == COMMAND_HANDLED, "command failed: cmd_pl_iter_get after status change");

  static uint8_t cmd_pl_iter_rep4[] = {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_ITER_REPORT,
        0xFF, 0x00, // seqno and remaining count
        0x10, // DSK length
        0x12, 0x34, 0x56, 0x94, 0x55, 0xae, 0x8f, 0x57, 0x29, 0x69, 0x45, 0x4c, 0x23, 0x3a, 0x88, 0x99,
        PVS_TLV_TYPE_BOOTSTRAP_MODE << 1 | 1, 1, PVS_BOOTMODE_SMART_START,
        PVS_TLV_TYPE_STATUS << 1 | 1, 1, PVS_STATUS_IGNORED,
                                       PVS_TLV_TYPE_NETWORK_INFO << 1, 2, 8, PVS_NETSTATUS_INCLUDED,
        102, 0x04, '5', '6', ' ', '7',
        100, 0x03, 'x', 'y', 'z',
  };

  test_print(4, "ZW_SendDataZIP_args.datalen(%u) == sizeof cmd_pl_rep1(%u)\n",
             ZW_SendDataZIP_args.datalen, sizeof cmd_pl_iter_rep4);
  data = (uint8_t*) ZW_SendDataZIP_args.dataptr;
  test_print(4, "0x%02x%02x%02x%02x 0x%02x%02x%02x%02x\n", 
             data[21], data[22], data[23],
             data[5], data[6],
             data[27], data[28], data[29]);
  check_true(
      ZW_SendDataZIP_args.datalen == sizeof cmd_pl_iter_rep4 &&
      memcmp(ZW_SendDataZIP_args.dataptr, cmd_pl_iter_rep4, ZW_SendDataZIP_args.datalen) == 0,
      "Iter get response 4 wrong");

  // Happy case _GET

  static uint8_t cmd_pl_get1[] = {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_GET,
          0x22, // seqno
          0x10, // DSK length
          0x12, 0x34, 0x56, 0x94, 0x55, 0xae, 0x8f, 0x57, 0x29, 0x69, 0x45, 0x4c, 0x23, 0x3a, 0x88, 0x99,
    };

  static uint8_t cmd_pl_rep1[] = {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_REPORT,
        0x22,  // seqno
        0x10, // DSK length
        0x12, 0x34, 0x56, 0x94, 0x55, 0xae, 0x8f, 0x57, 0x29, 0x69, 0x45, 0x4c, 0x23, 0x3a, 0x88, 0x99,
        PVS_TLV_TYPE_BOOTSTRAP_MODE << 1 | 1, 1, PVS_BOOTMODE_SMART_START,
        PVS_TLV_TYPE_STATUS << 1 | 1, 1, PVS_STATUS_IGNORED,
                                  PVS_TLV_TYPE_NETWORK_INFO << 1, 2, 8, PVS_NETSTATUS_INCLUDED,
        102, 0x04, '5', '6', ' ', '7',
        100, 0x03, 'x', 'y', 'z',
  };

  ret = PVL_CommandHandler(&dummy_connection, cmd_pl_get1, sizeof cmd_pl_get1);
  check_true(ret == COMMAND_HANDLED, "command failed: cmd_pl_get after status change");

  test_print(4, "ZW_SendDataZIP_args.datalen(%u) == sizeof cmd_pl_rep1(%u)\n",
             ZW_SendDataZIP_args.datalen, sizeof cmd_pl_rep1);
  check_true(
      ZW_SendDataZIP_args.datalen == sizeof cmd_pl_rep1 &&
      memcmp(ZW_SendDataZIP_args.dataptr, cmd_pl_rep1, ZW_SendDataZIP_args.datalen) == 0,
      "Get response 1 wrong");

  // Nonexistent DSK _GET

  static uint8_t cmd_pl_get2[] = {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_GET,
          0x23, // seqno
          0x10, // DSK length
          0x55, 0x34, 0x56, 0x94, 0x55, 0xae, 0x8f, 0x57, 0x28,
    };

  static uint8_t cmd_pl_rep2[] = {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_REPORT,
        0x23,  // seqno
        0x00, // DSK length
  };


  ret = PVL_CommandHandler(&dummy_connection, cmd_pl_get2, sizeof cmd_pl_get2);
    check_true(ret == COMMAND_HANDLED, "command failed: cmd_pl_get non-existent DSK");

  check_true(
      ZW_SendDataZIP_args.datalen == sizeof cmd_pl_rep2 &&
      memcmp(ZW_SendDataZIP_args.dataptr, cmd_pl_rep2, ZW_SendDataZIP_args.datalen) == 0,
      "Get response 2 wrong");


  // Set an existing DSK again (changing a name) - device 1234
  static uint8_t cmd_pl_set3[] = {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_SET,
     0x03, 0x10,
     0x12, 0x34, 0x56, 0x94, 0x55, 0xae, 0x8f, 0x57, 0x29, 0x69, 0x45, 0x4c, 0x23, 0x3a, 0x88, 0x99,
     100, 0x03, 'B', 'o', 'o',
     102, 0x04, '5', '6', ' ', '7',
   };

  ret = PVL_CommandHandler(&dummy_connection, cmd_pl_set3, sizeof cmd_pl_set3);
  check_true(ret == COMMAND_HANDLED, "command failed: cmd_pl_set3");

  // Check that the name has changed on device 1234
  // Check that all other parameters have not changed (ZGW-996)
  static uint8_t cmd_pl_rep3[] = {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_REPORT,
        0x24,  // seqno
        0x10, // DSK length
        0x12, 0x34, 0x56, 0x94, 0x55, 0xae, 0x8f, 0x57, 0x29, 0x69, 0x45, 0x4c, 0x23, 0x3a, 0x88, 0x99,
        PVS_TLV_TYPE_BOOTSTRAP_MODE << 1 | 1, 1, PVS_BOOTMODE_SMART_START,
        PVS_TLV_TYPE_STATUS << 1 | 1, 1, PVS_STATUS_IGNORED,
                                  PVS_TLV_TYPE_NETWORK_INFO << 1, 2, 8, PVS_NETSTATUS_INCLUDED,
        102, 0x04, '5', '6', ' ', '7',
        100, 0x03, 'B', 'o', 'o',
  };

  cmd_pl_get1[2] = 0x24;
  ret = PVL_CommandHandler(&dummy_connection, cmd_pl_get1, sizeof cmd_pl_get1);
  check_true(ret == COMMAND_HANDLED, "command failed: cmd_pl_get");
  test_print(4, "Expected size of reply (sizeof cmd_pl_rep3): %u, got size (datalen): %u\n",
             sizeof(cmd_pl_rep3), ZW_SendDataZIP_args.datalen);
  check_true(
      ZW_SendDataZIP_args.datalen == sizeof cmd_pl_rep3 &&
      memcmp(ZW_SendDataZIP_args.dataptr, cmd_pl_rep3, ZW_SendDataZIP_args.datalen) == 0,
      "Get response 3, only name tlv has changed.");


  // Create a device with bootmode S2
  // SET device 7a56
  static uint8_t cmd_pl_set7a56[] = 
     {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_SET,
      0x03, 0x10,        // Seqno, DSK length
      0x7a, 0x56, 0x49, 0x94, 0x55, 0xae, 0x8f, 0x57, 0x29, 0x69, 0x45, 0x4c, 0x23, 0x3a, 0x72, 0x69,
      100, 0x03, 'a', 'b', 'c',    // Name
      102, 0x08, '1', '2', '3', '1', '2', '3', '1', '2',  // Location
      0x6d, 1, PVS_BOOTMODE_S2, //bootmode
  };

  ret = PVL_CommandHandler(&dummy_connection, cmd_pl_set7a56, sizeof cmd_pl_set7a56);
  check_true(ret == COMMAND_HANDLED, "command cmd_pl_set on 7a56");

  check_true(provisioning_list_get_count() == 2 &&
      provisioning_list_pending_count() == 0,
      "Adding 2nd pvl entry failed");
  provisioning_list_print();
  static uint8_t cmd_pl_get7a56[] = 
     {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_GET,
      0x22, // seqno
      0x10, // DSK length
      0x7a, 0x56, 0x49, 0x94, 0x55, 0xae, 0x8f, 0x57, 0x29, 0x69, 0x45, 0x4c, 0x23, 0x3a, 0x72, 0x69
    };

  static uint8_t cmd_pl_rep7a56[] = 
     {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_REPORT,
      0x22,  // seqno
      0x10, // DSK length
      0x7a, 0x56, 0x49, 0x94, 0x55, 0xae, 0x8f, 0x57, 0x29, 0x69, 0x45, 0x4c, 0x23, 0x3a, 0x72, 0x69,
      PVS_TLV_TYPE_BOOTSTRAP_MODE << 1 | 1, 1, PVS_BOOTMODE_S2,
      PVS_TLV_TYPE_STATUS << 1 | 1, 1, PVS_STATUS_PENDING,
      PVS_TLV_TYPE_NETWORK_INFO << 1, 2, 0, PVS_NETSTATUS_NOT_IN_NETWORK,
      102, 0x08, '1', '2', '3', '1', '2', '3', '1', '2',  // Location
      100, 0x03, 'a', 'b', 'c',    // Name
  };

  ret = PVL_CommandHandler(&dummy_connection, cmd_pl_get7a56, sizeof cmd_pl_get7a56);
  check_true(ret == COMMAND_HANDLED, "cmd_pl_get after S2 set");
  check_true(
      ZW_SendDataZIP_args.datalen == sizeof cmd_pl_rep7a56 &&
      memcmp(ZW_SendDataZIP_args.dataptr, cmd_pl_rep7a56, ZW_SendDataZIP_args.datalen) == 0,
      "Get device 7a56, bootmode S2.");


//
  ///**
  // * PL Set - No TLV
  // */
  //static uint8_t cmd_pl_set3[] = {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_SET,
  //    0x03, 0x10,
  //    0x88, 0x99, 0x56, 0x94, 0x55, 0xae, 0x8f, 0x57, 0x29, 0x69, 0x45, 0x4c, 0x23, 0x3a, 0x88, 0x99,
  //};
  //
  //
  //
  // static uint8_t cmd_pl_get2[] = {COMMAND_CLASS_PROVISIONING_LIST, COMMAND_PROVISION_GET,
  //    0x00, 0x10,
  //    0x12, 0x34, 0x56, 0x94, 0x55, 0xae, 0x8f, 0x57, 0x29, 0x69, 0x45, 0x4c, 0x23, 0x3a, 0x88, 0x99,
  // };


  mock_reset();
}

int main()
{
#define PVL_FILENAME  "./provisioning_list_store_unittest.dat"

  /* Delete persistent PVL file if left over from previous run */
  system("/bin/sh -c '[ -e " PVL_FILENAME " ] && rm " PVL_FILENAME "'");

  CC_provisioning_list_init();
  provisioning_list_init(PVL_FILENAME, NULL);

  test_CC_pvl_basic();

  if (numErrs) {
      test_print(0, "\nFAILED, %d errors\n", numErrs);
  } else {
      test_print(0, "\nPASSED\n");
  }
  return numErrs;
}
