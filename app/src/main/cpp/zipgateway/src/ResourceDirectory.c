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
#include "ResourceDirectory.h"
#include "RD_DataStore.h"
#include "RD_internal.h"
#include "mDNSService.h"
#include "ZIP_Router.h"
#include "Serialapi.h"
#include "NodeCache.h"
#include "Mailbox.h"
#include "Bridge.h"
#include "S2.h"
#include "ZW_transport_api.h"
#include "ZW_SendRequest.h"
#include "ZW_SendDataAppl.h"
#include "security_layer.h"
#include <ctype.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>

#include "ipv46_nat.h"

#include "lib/memb.h"
#include "lib/list.h"

#include "dev/eeprom.h"
#include "smalloc.h"

#include "zwdb.h"

#include "ZW_classcmd_ex.h"
#include "ZW_ZIPApplication.h"
//This is 2000ms
#define REQUEST_TIMEOUT 200

typedef char rd_name_t[64];
int dont_set_cache_flush_bit = 0;
struct ctimer rd_node_probe_update_timer;
struct ctimer ep_probe_update_timer;
extern int tie_braking_won;
extern int conflict_when_not_in_qs;
/* RD probe lock. If this lock is set then the probe machine is locked. */
static uint8_t probe_lock = 0;
uint8_t denied_conflict_probes = 0;

#define BIT8_TST(bit,array) ((array[(bit)>>3] >> ((bit) & 7)) & 0x1)
/*#define BIT8_SET(bit,array) array[(bit)>>3] |= (1<<((bit) & 7))
 #define BIT8_CLR(bit,array) array[(bit)>>3] &= ~(1<<((bit) & 7))
 */
/* forward */
void
rd_node_probe_update(rd_node_database_entry_t* data);
void
rd_ep_probe_update(rd_ep_database_entry_t* ep);
static void
rd_check_for_dead_nodes_worker(void* v);

/** Check if endpoint supports a command class non-securely.
 *
 * \param ep Valid pointer to an endpoint.
 * \param class The command class to check for.
 * \return True is class is supported by ep, otherwise false.
 */
static int rd_ep_supports_cmd_class_nonsec(rd_ep_database_entry_t* ep, uint16_t class);

static int
SupportsCmdClassFlags(BYTE nodeid, WORD class);

static struct ctimer dead_node_timer;
static struct ctimer nif_request_timer;


typedef struct ep_probe_done_notifier
{
  void
  (*callback)(rd_ep_database_entry_t* ep, void* user);
  void* user;
  uint16_t epid;
  uint16_t node_id;
} ep_probe_done_notifier_t;
#define NUM_PROBES 1
static ep_probe_done_notifier_t ep_probe[NUM_PROBES];

#define DEFAULT_WAKE_UP_INTERVAL (70 * 60)
const char* rd_node_probe_state_name(rd_node_state_t state)
{
  static char str[25];

  switch(state)
  {
  case STATUS_CREATED               :return "STATUS_CREATED";
  case STATUS_PROBE_NODE_INFO       :return "STATUS_PROBE_NODE_INFO";
  case STATUS_PROBE_PRODUCT_ID      :return "STATUS_PROBE_PRODUCT_ID";
  case STATUS_ENUMERATE_ENDPOINTS   :return "STATUS_ENUMERATE_ENDPOINTS";
  case STATUS_CHECK_WU_CC_VERSION   :return "STATUS_CHECK_WU_CC_VERSION";
  case STATUS_GET_WU_CAP            :return "STATUS_GET_WU_CAP";            
  case STATUS_SET_WAKE_UP_INTERVAL  :return "STATUS_SET_WAKE_UP_INTERVAL";
  case STATUS_ASSIGN_RETURN_ROUTE   :return "STATUS_ASSIGN_RETURN_ROUTE";
  case STATUS_PROBE_WAKE_UP_INTERVAL:return "STATUS_PROBE_WAKE_UP_INTERVAL";
  case STATUS_PROBE_ENDPOINTS       :return "STATUS_PROBE_ENDPOINTS";
  case STATUS_MDNS_PROBE            :return "STATUS_MDNS_PROBE";
  case STATUS_MDNS_EP_PROBE         :return "STATUS_MDNS_EP_PROBE";
  case STATUS_DONE                  :return "STATUS_DONE";
  case STATUS_PROBE_FAIL            :return "STATUS_PROBE_FAIL";
  case STATUS_FAILING               :return "STATUS_FAILING";
  default:
    sprintf(str, "%d", state);
    return str;
  }
};


const char *ep_state_name(rd_ep_state_t state)
{
  static char str[25];
  switch (state)
  {
  case EP_STATE_PROBE_INFO              : return "EP_STATE_PROBE_INFO";
  case EP_STATE_PROBE_SEC2_C2_INFO      : return "EP_STATE_PROBE_SEC2_C2_INFO";
  case EP_STATE_PROBE_SEC2_C1_INFO      : return "EP_STATE_PROBE_SEC2_C1_INFO";
  case EP_STATE_PROBE_SEC2_C0_INFO      : return "EP_STATE_PROBE_SEC2_C0_INFO";
  case EP_STATE_PROBE_SEC0_INFO         : return "EP_STATE_PROBE_SEC0_INFO";
  case EP_STATE_PROBE_ZWAVE_PLUS        : return "EP_STATE_PROBE_ZWAVE_PLUS";
  case EP_STATE_MDNS_PROBE              : return "EP_STATE_MDNS_PROBE";
  case EP_STATE_MDNS_PROBE_IN_PROGRESS  : return "EP_STATE_MDNS_PROBE_IN_PROGRESS";
  case EP_STATE_PROBE_DONE              : return "EP_STATE_PROBE_DONE";
  case EP_STATE_PROBE_FAIL              : return "EP_STATE_PROBE_FAIL";
  default:
    sprintf(str, "%d", state);
    return str;
 }
};

/**
 * Increase the number at the end of the string. Add a 1 if there is no number
 * return the new length of the string
 *
 * s, as string buffer which must be large enough to contain the new string
 * ilen the inital length of s
 * maxlen the maximum lenth of the new string.
 */
static u8_t
add_or_increment_trailing_digit(char* s, u8_t ilen, u8_t maxlen)
{
  u8_t k, d;

  k = ilen - 1;
  while (isdigit(s[k]) && (k >=0) )
    k--;

  if (s[k] == '-')
  {
    d = atoi(&(s[k + 1])) + 1;
  }
  else
  {
    d = 1;
    k = ilen;
  }

  k += snprintf(&(s[k]), maxlen - k, "-%i", d);
  s[k] = 0;
  return k;
}

void
rd_set_failing(rd_node_database_entry_t* n, uint8_t failing)
{
  rd_ep_database_entry_t* ep;
  rd_node_state_t s;
  s = failing ? STATUS_FAILING : STATUS_DONE;

  if (n->state != s
      && ((n->state == STATUS_FAILING) || (n->state == STATUS_DONE)))
  {
    DBG_PRINTF("Node %d is now %s\n", n->nodeid, failing ? "failing" : "ok");

    n->state = s;
    n->mode &= ~MODE_FLAGS_FAILED;
    n->mode |= (n->state == STATUS_FAILING) ? MODE_FLAGS_FAILED : 0;

    for (ep = list_head(n->endpoints); ep != NULL; ep = list_item_next(ep))
    {
      mdns_endpoint_nofify(ep, 0);
    }

    /* Force node re-discovery */
    /*n->mode |= MODE_FLAGS_DELETED;
     n->mode &= ~MODE_FLAGS_DELETED;
     for (ep = list_head(n->endpoints); ep != NULL; ep = list_item_next(ep)) {
     mdns_endpoint_nofify(ep);
     }*/
  }
}

static void
rd_failing_node_probe(BYTE txStatus, void* user, TX_STATUS_TYPE *t)
{
  uint8_t node = user - (void*) 0;
  rd_node_database_entry_t* nd = rd_node_get_raw(node);

  if (nd)
  {
    rd_set_failing(nd, txStatus != TRANSMIT_COMPLETE_OK);

    if (txStatus == TRANSMIT_COMPLETE_OK)
      rd_node_is_alive(node);
  }
  /*Continue probe in 1 sec */
  ctimer_set(&dead_node_timer, 1000, rd_check_for_dead_nodes_worker,
      (void*) 0 + node + 1);
}

static void
rd_check_for_dead_nodes_worker(void* v)
{
  static const uint8_t nop = 0;
  uint8_t i;
  rd_node_database_entry_t *n;
  uint8_t start_with_node = v - (void*) 0;

  uint32_t limit = 0;

  for (i = start_with_node; i < ZW_MAX_NODES - 1; i++)
  {
    n = rd_node_get_raw(i);
    if (!n)
      continue;
    if (n->nodeid == MyNodeID)
      continue;

    /* Devices with a wakeup interval of 0 need special treatment.
     * They only wake up on external events, not on a regular schedule.
     * Since we have no expected wakeup time for these devices, they are never
     * marked failing for missing a wakeup. */
    if (n->wakeUp_interval > 0)    {
      limit = n->lastAwake + 3 * n->wakeUp_interval; //3 times wakeup interval
    }
    //  ERR_PRINTF("Node %i wakeup interval %i\n", n->nodeid, n->wakeUp_interval);
    /* if the wakeup interval is 0 (nodes never failing) or sleeping nodes which
              are already marked failed*/
    if (((n->wakeUp_interval == 0) || (n->mode & MODE_FLAGS_FAILED)) && (n->mode == MODE_MAILBOX))
    {
      //DBG_PRINTF("Node (%d)is already marked failed or has wakeup interval 0. Just purging mailbox messages for it\n", n->nodeid);
      mb_purge_messages(n->nodeid);
    }
    else if (limit < clock_seconds())
    {
      if (n->mode == MODE_MAILBOX)
      {
        if(mb_enabled()) {
            DBG_PRINTF("mailbox node %i is now failing, because it has not reported in\n",n->nodeid);
            /*Notify the mailbox*/
            /* Following function ends up purging messages for the mailbox. Only messages older 
               than 10 mins are purged there. As rd_check_for_dead_nodes_worker() is called every 
               minute if condition above mb_purge_messages() ends up periodically purging messages 
               which are not purged here */
            mb_failing_notify(n->nodeid);

            rd_set_failing(n, TRUE);
        }
      }
      else if (n->mode == MODE_ALWAYSLISTENING && (n->state == STATUS_DONE))
      {
        ts_param_t p;
        ts_set_std(&p, n->nodeid);
        if (!ZW_SendDataAppl(&p, (void*) &nop, sizeof(nop),
            rd_failing_node_probe, (void*) 0 + n->nodeid))
        {
          rd_failing_node_probe(TRANSMIT_COMPLETE_FAIL, (void*) 0 + n->nodeid, NULL);
        }
        return;
      }
    }
    else
    {
      rd_set_failing(n, FALSE);
    }
  }
  /*Re schedule probe in 1 minute */
  ctimer_set(&dead_node_timer, 60 * 1000, rd_check_for_dead_nodes_worker,
      (void*) 1);
}

int
rd_register_ep_probe_notifier(uint16_t node_id, uint16_t epid, void* user, void
(*callback)(rd_ep_database_entry_t* ep, void* user))
{

  int i;
  rd_ep_database_entry_t* ep;

  ep = rd_get_ep(node_id, epid);
  if (ep)
  {
    if (ep->state == EP_STATE_PROBE_DONE || ep->state == EP_STATE_PROBE_FAIL)
    {
      callback(ep, user);
      return 1;
    }
  }

  for (i = 0; i < NUM_PROBES; i++)
  {
    if (ep_probe[i].node_id == 0)
    {
      ep_probe[i].node_id = node_id;
      ep_probe[i].epid = epid;
      ep_probe[i].user = user;
      ep_probe[i].callback = callback;

      return 1;
    }
  }
  ASSERT(0);
  return 0;
}

static uint8_t
rd_add_endpoint(rd_node_database_entry_t* n, BYTE epid)
{
  rd_ep_database_entry_t* ep;

  ep = (rd_ep_database_entry_t*) rd_data_mem_alloc(
      sizeof(rd_ep_database_entry_t));
  if (!ep)
  {
    ERR_PRINTF("Out of memory\n");
    return 0;
  }

  memset(ep, 0, sizeof(rd_ep_database_entry_t));

  ep->endpoint_id = epid;
  ep->state = EP_STATE_PROBE_INFO;
  ep->node = n;

  /*A name of null means the default name should be used.*/
  ep->endpoint_name_len = 0;
  ep->endpoint_name = 0;
  list_add(n->endpoints, ep);
  n->nEndpoints++;

  return 1;
}

static void rd_wu_cc_version_callback(BYTE txStatus, BYTE rxStatus,
    ZW_APPLICATION_TX_BUFFER *pCmd, WORD cmdLength, void* user)
{

    rd_node_database_entry_t* e = (rd_node_database_entry_t*) user;
    if (txStatus == TRANSMIT_COMPLETE_OK) {
        if(pCmd->ZW_VersionCommandClassReportFrame.requestedCommandClass == COMMAND_CLASS_WAKE_UP) {
            if (pCmd->ZW_VersionCommandClassReportFrame.commandClassVersion >= 0x02) {
                e->state = STATUS_GET_WU_CAP;
                DBG_PRINTF("WAKEUP CC V2 or above\n");
            } else if (pCmd->ZW_VersionCommandClassReportFrame.commandClassVersion == 0x01) {
                e->state = STATUS_SET_WAKE_UP_INTERVAL;
                DBG_PRINTF("WAKEUP CC V1\n");
            } else {
                ERR_PRINTF("Unknown version of Wakeup command class\n");
                e->state = STATUS_PROBE_FAIL;
            }
        } else {
            ERR_PRINTF("version report is not of Wakeup command class\n");
            e->state = STATUS_PROBE_FAIL;
        }
    } else {
        ERR_PRINTF("Not TRANSMIT_COMPLETE_OK\n");
        e->state = STATUS_PROBE_FAIL;
    }
    rd_node_probe_update(e);
}

static void rd_wake_up_interval_get_callback(BYTE txStatus, BYTE rxStatus,
    ZW_APPLICATION_TX_BUFFER *pCmd, WORD cmdLength, void* user)
{
    rd_node_database_entry_t* e = (rd_node_database_entry_t*) user;
    if (txStatus == TRANSMIT_COMPLETE_OK)
    {
        e->wakeUp_interval = pCmd->ZW_WakeUpIntervalReportFrame.seconds1 << 16 |
                             pCmd->ZW_WakeUpIntervalReportFrame.seconds2 << 8 |
                             pCmd->ZW_WakeUpIntervalReportFrame.seconds3; 
  
        DBG_PRINTF("rd_wake_up_interval_get_callback: Wakeup Interval set to %d\n", e->wakeUp_interval);
        e->state = STATUS_SET_WAKE_UP_INTERVAL;
    } else {
        e->state = STATUS_PROBE_FAIL;
    }
    rd_node_probe_update(e);
}

static void rd_cap_wake_up_callback(BYTE txStatus, BYTE rxStatus,
    ZW_APPLICATION_TX_BUFFER *pCmd, WORD cmdLength, void* user)
{
  uint32_t min_interval = 0;
  uint32_t max_interval = 0;
  uint32_t step = 0;

  rd_node_database_entry_t* e = (rd_node_database_entry_t*) user;
  if (txStatus == TRANSMIT_COMPLETE_OK)
  {
    min_interval = pCmd->ZW_WakeUpIntervalCapabilitiesReportV2Frame.minimumWakeUpIntervalSeconds1 << 16 |
                   pCmd->ZW_WakeUpIntervalCapabilitiesReportV2Frame.minimumWakeUpIntervalSeconds2 << 8 |
                   pCmd->ZW_WakeUpIntervalCapabilitiesReportV2Frame.minimumWakeUpIntervalSeconds3;
    
    max_interval = pCmd->ZW_WakeUpIntervalCapabilitiesReportV2Frame.maximumWakeUpIntervalSeconds1 << 16 |
                   pCmd->ZW_WakeUpIntervalCapabilitiesReportV2Frame.maximumWakeUpIntervalSeconds2 << 8 |
                   pCmd->ZW_WakeUpIntervalCapabilitiesReportV2Frame.maximumWakeUpIntervalSeconds3;

    step = pCmd->ZW_WakeUpIntervalCapabilitiesReportV2Frame.wakeUpIntervalStepSeconds1 << 16 |
                   pCmd->ZW_WakeUpIntervalCapabilitiesReportV2Frame.wakeUpIntervalStepSeconds2 << 8 |
                   pCmd->ZW_WakeUpIntervalCapabilitiesReportV2Frame.wakeUpIntervalStepSeconds3;

    DBG_PRINTF("min: %d max: %d\n step:%d", min_interval, max_interval, step);
    if ((min_interval < DEFAULT_WAKE_UP_INTERVAL)  && (DEFAULT_WAKE_UP_INTERVAL < max_interval)) {
        e->wakeUp_interval = (DEFAULT_WAKE_UP_INTERVAL - (DEFAULT_WAKE_UP_INTERVAL % step ));
        if (e->wakeUp_interval < min_interval) {
            e->wakeUp_interval = (DEFAULT_WAKE_UP_INTERVAL + (DEFAULT_WAKE_UP_INTERVAL % step ));
        }
    } else if (min_interval > DEFAULT_WAKE_UP_INTERVAL) {
        e->wakeUp_interval = min_interval;
    } else if (max_interval < DEFAULT_WAKE_UP_INTERVAL) {
        e->wakeUp_interval = max_interval;
    }
    DBG_PRINTF("Wakeup Interval set to %d\n", e->wakeUp_interval);
    e->state = STATUS_SET_WAKE_UP_INTERVAL;
  }
  else
  {
  e->state = STATUS_PROBE_FAIL;
  }
  rd_node_probe_update(e);
}
static void
rd_probe_vendor_callback(BYTE txStatus, BYTE rxStatus,
    ZW_APPLICATION_TX_BUFFER *pCmd, WORD cmdLength, void* user)
{


  rd_node_database_entry_t* e = (rd_node_database_entry_t*) user;

  if (txStatus == TRANSMIT_COMPLETE_OK)
  {
    e->manufacturerID = pCmd->ZW_ManufacturerSpecificReportFrame.manufacturerId1
        << 8 | pCmd->ZW_ManufacturerSpecificReportFrame.manufacturerId2;
    e->productID = pCmd->ZW_ManufacturerSpecificReportFrame.productId1 << 8
        | pCmd->ZW_ManufacturerSpecificReportFrame.productId2;
    e->productType = pCmd->ZW_ManufacturerSpecificReportFrame.productTypeId1
        << 8 | pCmd->ZW_ManufacturerSpecificReportFrame.productTypeId2;

    e->state = STATUS_ENUMERATE_ENDPOINTS;
  }
  else
  {
    e->state = STATUS_PROBE_FAIL;
  }
  rd_node_probe_update(e);
}

static void
rd_ep_get_callback(BYTE txStatus, BYTE rxStatus, ZW_APPLICATION_TX_BUFFER *pCmd,
    WORD cmdLength, void* user)
{
  int n_end_points;
  int n_aggregated_endpoints;
  int epid;
  rd_node_database_entry_t* n = (rd_node_database_entry_t*) user;
  rd_ep_database_entry_t* ep;

  if (txStatus == TRANSMIT_COMPLETE_OK
      && n->state == STATUS_ENUMERATE_ENDPOINTS)
  {
    n_end_points = pCmd->ZW_MultiChannelEndPointReportV4Frame.properties2
        & 0x7f;

    if(cmdLength >=5) {
      n_aggregated_endpoints = pCmd->ZW_MultiChannelEndPointReportV4Frame.properties3 & 0x7f;
      n_end_points+=n_aggregated_endpoints;
    } else {
      n_aggregated_endpoints = 0;
    }

    /*If the old endpoint count does not match free up all endpoints old endpoints except ep0 and create some new ones */
    if (n->nEndpoints != n_end_points + 1)
    {
      rd_ep_database_entry_t*  ep0 =list_head(n->endpoints);

      while(ep0->list) {
        ep = ep0->list;
        ep0->list = ep->list;
        rd_store_mem_free_ep(ep);
      }

      n->nAggEndpoints = n_aggregated_endpoints;
      n->nEndpoints = 1; //Endpoint 0 is still there

      for (epid = 1; epid < n_end_points + 1; epid++)
      {
        if (!rd_add_endpoint(n, epid))
        {
          n->state = STATUS_PROBE_FAIL;
          rd_node_probe_update(n);
          return;
        }
      }
    }
    n->state = STATUS_CHECK_WU_CC_VERSION;
  }
  else
  {
    n->state = STATUS_PROBE_FAIL;
  }
  rd_node_probe_update(n);
}

static void
rd_probe_wakeup_callback(BYTE txStatus, BYTE rxStatus,
    ZW_APPLICATION_TX_BUFFER *pCmd, WORD cmdLength, void* user)
{
  rd_node_database_entry_t* e = (rd_node_database_entry_t*) user;

  if (txStatus == TRANSMIT_COMPLETE_OK
      && e->state == STATUS_PROBE_WAKE_UP_INTERVAL)
  {
    e->wakeUp_interval = pCmd->ZW_WakeUpIntervalReportFrame.seconds1 << 16
        | pCmd->ZW_WakeUpIntervalReportFrame.seconds2 << 8
        | pCmd->ZW_WakeUpIntervalReportFrame.seconds3 << 0;

    if (pCmd->ZW_WakeUpIntervalReportFrame.nodeid != MyNodeID)
    {
      WRN_PRINTF("WakeUP notifier NOT set to me!\n");
    }
    e->state = STATUS_PROBE_ENDPOINTS;
  }
  else
  {
    ERR_PRINTF("rd_probe_wakeup_callback fail!\n");
    e->state = STATUS_PROBE_FAIL;
  }
  rd_node_probe_update(e);
}


/*
 * https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetNaive
 * Counting bits set, Brian Kernighan's way
 */
static inline BYTE
bit8_count(BYTE n)
{
  unsigned int c; // c accumulates the total bits set in v
  for (c = 0; n; c++)
    n &= n - 1; // clear the least sigficant bit set
  return c;
}

static BYTE
count_array_bits(BYTE* a, BYTE length)
{
  BYTE c = 0;
  BYTE i;
  for (i = 0; i < length; i++)
    c += bit8_count(a[i]);
  return c;
}


static void
rd_ep_aggregated_members_get_callback(BYTE txStatus, BYTE rxStatus,
    ZW_APPLICATION_TX_BUFFER *pCmd, WORD cmdLength, void* user)
{
  rd_ep_database_entry_t* ep = (rd_ep_database_entry_t*) user;
  ZW_MULTI_CHANNEL_AGGREGATED_MEMBERS_REPORT_4BYTE_V4_FRAME* f = (ZW_MULTI_CHANNEL_AGGREGATED_MEMBERS_REPORT_4BYTE_V4_FRAME*) pCmd;

  int i,j,n;
  if( ep->state != EP_STATE_PROBE_AGGREGATED_ENDPOINTS) {
    return;
  }


  if(rxStatus == TRANSMIT_COMPLETE_OK) {
    n = count_array_bits(&f->aggregatedMembersBitMask1,f->numberOfBitMasks);
    if(ep->endpoint_agg) {
      rd_data_mem_free(ep->endpoint_agg);
    }
    ep->endpoint_agg = rd_data_mem_alloc(n);


    /*Calculate the index of the bits set in the mask*/
    n=0;
    for(i=0; i < f->numberOfBitMasks; i++) {
      uint8_t c = (&f->aggregatedMembersBitMask1)[i];

      j=0;
      while(c) {
        if(c & 1) {
          ep->endpoint_agg[ep->endpoint_aggr_len] = n + j+1;
          ep->endpoint_aggr_len++;
        }
        c = c >> 1;
        j++;
      }
      n=n+8;
    }

  }

  ep->state = EP_STATE_PROBE_SEC2_C2_INFO;
  rd_ep_probe_update(ep);
}

static void
rd_ep_capability_get_callback(BYTE txStatus, BYTE rxStatus,
    ZW_APPLICATION_TX_BUFFER *pCmd, WORD cmdLength, void* user)
{

  rd_ep_database_entry_t* ep = (rd_ep_database_entry_t*) user;
  //DBG_PRINTF(" %i\n",txStatus);
  if (txStatus == TRANSMIT_COMPLETE_OK
      && ep->endpoint_id
          == (pCmd->ZW_MultiChannelCapabilityReport1byteV4Frame.properties1
              & 0x7F))
  {

    if (ep->endpoint_info)
      rd_data_mem_free(ep->endpoint_info);
    ep->endpoint_info_len = 0;

    ep->endpoint_info = rd_data_mem_alloc(cmdLength - 3);
    WRN_PRINTF("Storing %i bytes epid = %i\n", cmdLength - 3, ep->endpoint_id);
    if (ep->endpoint_info)
    {
      memcpy(ep->endpoint_info,
          &(pCmd->ZW_MultiChannelCapabilityReport1byteV4Frame.genericDeviceClass),
          cmdLength - 3);
      ep->endpoint_info_len = cmdLength - 3;

      ep->state =  ep->endpoint_id > (ep->node->nEndpoints -1 - ep->node->nAggEndpoints)
          ? EP_STATE_PROBE_AGGREGATED_ENDPOINTS : EP_STATE_PROBE_SEC2_C2_INFO;
      DBG_PRINTF("State selected is %d\n", ep->state);
    }
    else
    {
      ep->state = EP_STATE_PROBE_FAIL;
    }
  }
  else
  {
    ep->state = EP_STATE_PROBE_FAIL;
  }
  rd_ep_probe_update(ep);
}
/* Fixme: May be this should be part of rd_ep_database_entry_t structure? 
   Not sure if we want to store this information at all*/
static int portable = 0;

static void rd_ep_zwave_plus_info_callback(BYTE txStatus, BYTE rxStatus,
    ZW_APPLICATION_TX_BUFFER *pCmd, WORD cmdLength, void* user)
{
  rd_ep_database_entry_t* ep = (rd_ep_database_entry_t*) user;

  if (txStatus == TRANSMIT_COMPLETE_OK)
  {
    if(cmdLength>= sizeof(ZW_ZWAVEPLUS_INFO_REPORT_V2_FRAME)) {
      ep->installer_iconID = pCmd->ZW_ZwaveplusInfoReportV2Frame.installerIconType1 << 8
          | pCmd->ZW_ZwaveplusInfoReportV2Frame.installerIconType2;

      ep->user_iconID = pCmd->ZW_ZwaveplusInfoReportV2Frame.userIconType1 << 8
          | pCmd->ZW_ZwaveplusInfoReportV2Frame.userIconType2;

      if (pCmd->ZW_ZwaveplusInfoReportV2Frame.roleType == 0x02) {
        portable = 1;
      }
    }
    ep->state++;
  }
  else
  {
    ep->state = EP_STATE_PROBE_FAIL;
  }
  rd_ep_probe_update( ep);
}

static void
rd_ep_secure_commands_get_callback(BYTE txStatus, BYTE rxStatus,
    ZW_APPLICATION_TX_BUFFER *pCmd, WORD cmdLength, void* user)
{
  uint8_t *p;
  rd_ep_database_entry_t* ep = (rd_ep_database_entry_t*) user;
  uint8_t header_len;
  uint8_t flag;

  if(ep->state == EP_STATE_PROBE_SEC0_INFO) {
    header_len = 3;
  } else {
    header_len = 2;
  }

  switch(ep->state)
  {
  case EP_STATE_PROBE_SEC2_C2_INFO:
    flag = NODE_FLAG_SECURITY2_ACCESS;
    break;
  case EP_STATE_PROBE_SEC2_C1_INFO:
    flag = NODE_FLAG_SECURITY2_AUTHENTICATED;
    break;
  case EP_STATE_PROBE_SEC2_C0_INFO:
    flag = NODE_FLAG_SECURITY2_UNAUTHENTICATED;
    break;
  case EP_STATE_PROBE_SEC0_INFO:
    flag = NODE_FLAG_SECURITY0;
    break;
  default:
    flag = 0;
    ASSERT(0);
    break;
  }

  DBG_PRINTF("Security flags 0x%02x\n", ep->node->security_flags);
  /* If ep is the real node and it was added by someone else and it
   * has not been probed before, we allow probe fail to down-grade the
   * security flags. */
  if ((ep->endpoint_id == 0)
      && !(ep->node->security_flags & NODE_FLAG_ADDED_BY_ME)
      && (ep->node->security_flags & NODE_FLAG_JUST_ADDED)) {
     if (ep->node->security_flags & flag) {
        DBG_PRINTF("Clearing flag 0x%02x\n", flag);
     }
     ep->node->security_flags &= ~flag;
  }

  //LOG_PRINTF("%s",__FUNCTION__);
  if (txStatus == TRANSMIT_COMPLETE_OK)
  {
    if(cmdLength >header_len )
    {
      /*Reallocate the node info to hold the security nif*/
      p = rd_data_mem_alloc(ep->endpoint_info_len + 2 + cmdLength - header_len);
      if (p)
      {
        if (ep->endpoint_info)
        {
          memcpy(p, ep->endpoint_info, ep->endpoint_info_len);
          rd_data_mem_free(ep->endpoint_info);
        }

        ep->endpoint_info = p;
        p += ep->endpoint_info_len;

        /* Insert security command class mark */
        *p++ = (COMMAND_CLASS_SECURITY_SCHEME0_MARK >> 8);
        *p++ = (COMMAND_CLASS_SECURITY_SCHEME0_MARK >> 0) & 0xFF;

        memcpy(p, &((BYTE*)pCmd)[header_len],cmdLength- header_len);
        ep->endpoint_info_len += 2 + cmdLength - header_len;
      }
    }

    if (ep->endpoint_id == 0) {
        DBG_PRINTF("Setting flag 0x%02x\n", flag);
        ep->node->security_flags |= flag;
    }
  }
  if (ep->endpoint_id > 0) {
    /* Endpoints are only queried about one security class */
    ep->state = EP_STATE_PROBE_ZWAVE_PLUS;
  } else {
    /* The node itself is dragged through all the states. */
    ep->state++;
  }
  rd_ep_probe_update(ep);
}

/*Used when a get node info is pending */
static rd_ep_database_entry_t* nif_request_ep = 0;

/*Used by assign return route*/
static rd_node_database_entry_t* current_probe_entry = 0;

/**
 * called from ApplicationController Update, when a node info is received or if the
 * ZW_RequestNodeInfo is failed.
 *
 * @param success tell if the request went ok*/
/*IN  Node id of the node that send node info */
/*IN  Pointer to Application Node information */
/*IN  Node info length                        */
void
rd_nif_request_notify(uint8_t sucsess, uint8_t bNodeID, uint8_t* nif,
    uint8_t nif_len)
{

  rd_ep_database_entry_t* ep = nif_request_ep;
  if (ep && ep->state == EP_STATE_PROBE_INFO)
  {
    nif_request_ep = 0;
    ctimer_stop(&nif_request_timer);

    ASSERT(ep->node);
    if (sucsess && ep->node->nodeid == bNodeID)
    {
      ep->node->nodeType = nif[0];
      if (ep->endpoint_info)
        rd_data_mem_free(ep->endpoint_info);
      ep->endpoint_info = rd_data_mem_alloc(nif_len+1);
      if (ep->endpoint_info)
      {
        memcpy(ep->endpoint_info, nif + 1, nif_len - 1);

        ep->endpoint_info[nif_len-1] = COMMAND_CLASS_ZIP_NAMING;
        ep->endpoint_info[nif_len  ] = COMMAND_CLASS_ZIP;

        ep->endpoint_info_len = nif_len+1;

        /* If node is just added and GW is inclusion controller, we
         * are still trying to determine security classes. */
        if ((ep->endpoint_id == 0)
            && !(ep->node->security_flags & NODE_FLAG_ADDED_BY_ME)
            && (ep->node->security_flags & NODE_FLAG_JUST_ADDED)) {
           /* Downgrade S2 flags if node does not support it. */
           if (!rd_ep_supports_cmd_class_nonsec(ep, COMMAND_CLASS_SECURITY_2)) {
              ep->node->security_flags &= ~NODE_FLAGS_SECURITY2;
           }
           /* Downgrade S0 if node does not support it. */
           if (!rd_ep_supports_cmd_class_nonsec(ep, COMMAND_CLASS_SECURITY)) {
              ep->node->security_flags &= ~NODE_FLAG_SECURITY0;
           }
        }

        ep->state++;
      }
      else
      {
        ep->state = EP_STATE_PROBE_FAIL;
      }
    }
    else
    {
      ep->state = EP_STATE_PROBE_FAIL;
    }
    rd_ep_probe_update(ep);
  }
}

void
AssignReturnRouteCallback(uint8_t status)
{
  if (current_probe_entry)
  {
    if (status == TRANSMIT_COMPLETE_OK)
    {
      current_probe_entry->state++;
    }
    else
    {
      current_probe_entry->state = STATUS_PROBE_FAIL;
    }
    rd_node_probe_update(current_probe_entry);
  }
  else
  {
    ASSERT(0);
  }
}

static void
node_info_request_timeout(void* d)
{
  if(!current_probe_entry) return;

  rd_ep_database_entry_t* ep = (rd_ep_database_entry_t*) d;

  ep->state = EP_STATE_PROBE_FAIL;
  nif_request_ep = 0;
  rd_ep_probe_update(ep);
}

static void
rd_endpoint_name_probe_done(int bStatus, void* ctx)
{
  char buf[64];
  int timer_value = 0;
  rd_ep_database_entry_t *ep = (rd_ep_database_entry_t*) ctx;
  u8_t k;
  ASSERT(ep);
  if(!current_probe_entry) return;
  if(!ep) return;

  if (ep->state == EP_STATE_MDNS_PROBE_IN_PROGRESS)
  {
    if (bStatus)
    {
      ep->state = EP_STATE_PROBE_DONE;
    }
    else
    {
      /* If this is the default node name, We need to allocate memory for this name*/
      if (ep->endpoint_name_len == 0)
      {
        ep->endpoint_name_len = rd_get_ep_name(ep, buf, sizeof(buf));
      }
      else
      { /**/
        /*Locate trailing digits in the name */
        memcpy(buf, ep->endpoint_name, ep->endpoint_name_len);
        rd_data_mem_free(ep->endpoint_name);
      }
      k = add_or_increment_trailing_digit(buf, ep->endpoint_name_len,
          sizeof(buf));
      ep->endpoint_name = rd_data_mem_alloc(k);
      ep->endpoint_name_len = k;
      memcpy(ep->endpoint_name, buf, k);
      timer_value = 250 + (750 * (denied_conflict_probes > 10));
    }
    if (timer_value) {
        ep->state = EP_STATE_MDNS_PROBE;
        ctimer_set(&ep_probe_update_timer, timer_value, (void (*)(void *)) rd_ep_probe_update, ep);
        timer_value = 0;
        denied_conflict_probes++;
    }
    else {
        rd_ep_probe_update(ep);
    }
   
  }
}

void
rd_ep_probe_update(rd_ep_database_entry_t* ep)
{
  const uint8_t secure_commands_supported_get[] = {COMMAND_CLASS_SECURITY,SECURITY_COMMANDS_SUPPORTED_GET};
  const uint8_t secure_commands_supported_get2[] = {COMMAND_CLASS_SECURITY_2,SECURITY_2_COMMANDS_SUPPORTED_GET};
  ts_param_t p;

  ASSERT(current_probe_entry);
  if(!current_probe_entry) return;

  static ZW_MULTI_CHANNEL_CAPABILITY_GET_V2_FRAME cap_get_frame =
    { COMMAND_CLASS_MULTI_CHANNEL_V2, MULTI_CHANNEL_CAPABILITY_GET_V2, 0 };

  if(ep->node == 0) {
    return;
  }
  DBG_PRINTF("EP probe nd=%i (flags 0x%02x) ep =%d state=%s\n",
             ep->node->nodeid, ep->node->security_flags,
             ep->endpoint_id, ep_state_name(ep->state));

  if(is_virtual_node(ep->node->nodeid)) {
    rd_remove_node(ep->node->nodeid);
    ASSERT(0);
    return;
  }

  ts_set_std(&p, ep->node->nodeid);
  p.dendpoint = ep->endpoint_id;

  switch (ep->state)
  {
    case EP_STATE_PROBE_AGGREGATED_ENDPOINTS:
      ts_set_std(&p, ep->node->nodeid);
      ZW_MULTI_CHANNEL_AGGREGATED_MEMBERS_GET_V4_FRAME f;
      f.cmdClass = COMMAND_CLASS_MULTI_CHANNEL_V4;
      f.cmd = MULTI_CHANNEL_AGGREGATED_MEMBERS_GET_V4;
      f.properties1 = ep->endpoint_id;

      if (!ZW_SendRequest(&p, (BYTE*) &f, sizeof(ZW_MULTI_CHANNEL_AGGREGATED_MEMBERS_GET_V4_FRAME),
          MULTI_CHANNEL_AGGREGATED_MEMBERS_REPORT_V4, REQUEST_TIMEOUT, ep,
          rd_ep_aggregated_members_get_callback))
      {
        goto fail_state;
      }
      break;
    case EP_STATE_PROBE_INFO:
      /*The Device Capabilities of Endpoint 1 must respond the normal NIF*/
      if (ep->endpoint_id > 0)
      {
        cap_get_frame.properties1 = ep->endpoint_id & 0x7F;

        ts_param_t p;
        ts_set_std(&p, ep->node->nodeid);

        if (!ZW_SendRequest(&p, (BYTE*) &cap_get_frame, sizeof(cap_get_frame),
            MULTI_CHANNEL_CAPABILITY_REPORT_V2, REQUEST_TIMEOUT, ep,
            rd_ep_capability_get_callback))
        {
          goto fail_state;
        }
      }
      else
      {
        /*ERR_PRINTF("Request node info %d\n",ep->node->nodeid);*/

        /*The routers own NIF */
        if (ep->node->nodeid == MyNodeID)
        {
          if (ep->endpoint_info)
            rd_data_mem_free(ep->endpoint_info);
          ep->endpoint_info = rd_data_mem_alloc(
              IPNIFLen - 4 + 2 + IPnSecureClasses);
          ep->user_iconID = ICON_TYPE_GENERIC_GATEWAY;
          ep->installer_iconID = ICON_TYPE_GENERIC_GATEWAY;
          if (ep->endpoint_info)
          {

            memcpy(ep->endpoint_info, IPNIF + 4, IPNIFLen - 4);
            ep->endpoint_info_len = IPNIFLen - 4;
            /*Add the command class mark -- No matter if we are secure or not */
            ep->endpoint_info[ep->endpoint_info_len] =
                (COMMAND_CLASS_SECURITY_SCHEME0_MARK >> 8) & 0xFF;
            ep->endpoint_info[ep->endpoint_info_len + 1] =
                (COMMAND_CLASS_SECURITY_SCHEME0_MARK >> 0) & 0xFF;
            ep->endpoint_info_len += 2;

            memcpy(ep->endpoint_info + ep->endpoint_info_len, IPSecureClasses,
                IPnSecureClasses);

            ep->endpoint_info_len += IPnSecureClasses;
            ep->state =  EP_STATE_MDNS_PROBE;
            rd_ep_probe_update(ep);
            return;
          }
          else
          {
            ERR_PRINTF("Nodeinfo fail(no memory)\n");
            goto fail_state;
          }
        }
        else
        { /* ep->node->nodeid == MyNodeID */
          //ASSERT(nif_request_ep == 0);
          /*This might just be a session resume */
          if (nif_request_ep)
          {
            return;
          }

          nif_request_ep = ep;
          /*Mask out all security flags as we will find them durring the probe */
          /* TODO : we set all the NODE_FLAGS_SECURITY in network management flags but then mask it out again here ? */
          /* Only increase security */
          /* ep->node->security_flags &= ~NODE_FLAGS_SECURITY; */

          if (ZW_RequestNodeInfo(ep->node->nodeid, 0))
          {
            if(ep->node->mode == MODE_FREQUENTLYLISTENING)
            {
                ctimer_set(&nif_request_timer, 1550, node_info_request_timeout,
                ep);
            } else {
                ctimer_set(&nif_request_timer, 250, node_info_request_timeout,
                ep);
            }
          }
          else
          {
            nif_request_ep = 0;
            ERR_PRINTF("Nodeinfo fail\n");
            goto fail_state;
            return;
          }
        }
      }
      break;
    case EP_STATE_PROBE_SEC2_C2_INFO:
      /*Dont probe security at all */
      if (ep->node->security_flags & NODE_FLAG_KNOWN_BAD)
      {
        ep->state = EP_STATE_PROBE_ZWAVE_PLUS - 1;
        goto next_state;
      }

      /* Don't probe more S2 */
      if (!SupportsCmdClass(ep->node->nodeid, COMMAND_CLASS_SECURITY_2))
      {
        ep->state = EP_STATE_PROBE_SEC0_INFO - 1;
        goto next_state;
      }

      /*If this is not the root NIF just use the auto scheme */
      if( ep->endpoint_id >0 )
      {
        p.scheme = AUTO_SCHEME;
        if (((GetCacheEntryFlag(ep->node->nodeid) & NODE_FLAGS_SECURITY2))
            && ((GetCacheEntryFlag(MyNodeID) & NODE_FLAGS_SECURITY2))) {
          /* S2 is supported and working on both node and GW.
           * Therefore ep must be S2. */
          if (!ZW_SendRequest(&p, secure_commands_supported_get2, sizeof(secure_commands_supported_get2),
                  SECURITY_2_COMMANDS_SUPPORTED_REPORT, 20, ep, rd_ep_secure_commands_get_callback))
          {
            goto fail_state;
          }
        } else {
          /* Node is not S2 or node is not included with S2 or GW is
           * not running S2. */
          /* If the ep or the GW do not support S0, either, we let the
           * PROBE_SEC0 state sort it out. */
          ep->state = EP_STATE_PROBE_SEC0_INFO - 1;
          goto next_state;
        }
      } else {
        /* Root device */
        if(GetCacheEntryFlag(MyNodeID) & NODE_FLAG_SECURITY2_ACCESS)
        {
          p.scheme = SECURITY_SCHEME_2_ACCESS;

          if (!ZW_SendRequest(&p, secure_commands_supported_get2, sizeof(secure_commands_supported_get2),
          SECURITY_2_COMMANDS_SUPPORTED_REPORT, 20, ep, rd_ep_secure_commands_get_callback))
          {
            goto fail_state;
          }
        }
        else
        {
          goto next_state;
        }
      }
      break;

    case EP_STATE_PROBE_SEC2_C1_INFO:
      /*If this is not the root NIF and the root does not support this class, move on*/
      if((ep->endpoint_id >0) && (0==(GetCacheEntryFlag(ep->node->nodeid) & NODE_FLAG_SECURITY2_AUTHENTICATED))) {
        goto next_state;
      }

      if (GetCacheEntryFlag(MyNodeID) & NODE_FLAG_SECURITY2_AUTHENTICATED)
      {
        p.scheme = SECURITY_SCHEME_2_AUTHENTICATED;

        if (!ZW_SendRequest(&p, secure_commands_supported_get2, sizeof(secure_commands_supported_get2),
        SECURITY_2_COMMANDS_SUPPORTED_REPORT, 20, ep, rd_ep_secure_commands_get_callback))
        {
          goto fail_state;
        }
      }
      else
      {
        goto next_state;
      }

      break;
    case EP_STATE_PROBE_SEC2_C0_INFO:
      /*If this is not the root NIF and the root does not support this class, move on*/
      if((ep->endpoint_id >0) && (0==(GetCacheEntryFlag(ep->node->nodeid) & NODE_FLAG_SECURITY2_UNAUTHENTICATED))) {
        goto next_state;
      }

      if (GetCacheEntryFlag(MyNodeID) & NODE_FLAG_SECURITY2_UNAUTHENTICATED)
      {
        p.scheme = SECURITY_SCHEME_2_UNAUTHENTICATED;

        if (!ZW_SendRequest(&p, secure_commands_supported_get2, sizeof(secure_commands_supported_get2),
        SECURITY_2_COMMANDS_SUPPORTED_REPORT, 20, ep, rd_ep_secure_commands_get_callback))
        {
          goto fail_state;
        }
      }
      else
      {
        goto next_state;
      }

      break;
    case EP_STATE_PROBE_SEC0_INFO:
      /* Do not probe S0 if it is not supported by the node. */
      if (!SupportsCmdClass(ep->node->nodeid, COMMAND_CLASS_SECURITY))
      {
        goto next_state;
      }

      /* Do not probe S0 on a real endpoint if the root device is not
       * using it.  */
      if ((ep->endpoint_id > 0) &&
          !(GetCacheEntryFlag(ep->node->nodeid) & NODE_FLAG_SECURITY0))
      {
        goto next_state;
      }

      /* Only probe S0 if the GW is using it. */
      if ((GetCacheEntryFlag(MyNodeID) & NODE_FLAG_SECURITY0))
      {
        p.scheme = SECURITY_SCHEME_0;

        if (!ZW_SendRequest(&p, secure_commands_supported_get, sizeof(secure_commands_supported_get),
        SECURITY_COMMANDS_SUPPORTED_REPORT, 3*20, ep, rd_ep_secure_commands_get_callback))
        {
          goto fail_state;
        }
      }
      else
      {
        goto next_state;
      }

      break;
    case EP_STATE_PROBE_ZWAVE_PLUS:
      ep->installer_iconID = 0;
      ep->user_iconID = 0;
      if ((rd_ep_class_support(ep, COMMAND_CLASS_ZWAVEPLUS_INFO) & SUPPORTED))
      {
        const uint8_t zwave_plus_info_get[] ={ COMMAND_CLASS_ZWAVEPLUS_INFO, ZWAVEPLUS_INFO_GET};
        p.scheme = AUTO_SCHEME;
        if (!ZW_SendRequest(&p, zwave_plus_info_get,
            sizeof(zwave_plus_info_get), ZWAVEPLUS_INFO_REPORT,
            REQUEST_TIMEOUT, ep, rd_ep_zwave_plus_info_callback))
        {
          goto fail_state;
        }
      } else {
        goto next_state;
      }
      break;
    case EP_STATE_MDNS_PROBE:
      ep->state = EP_STATE_MDNS_PROBE_IN_PROGRESS;
      if (!mdns_endpoint_name_probe(ep, rd_endpoint_name_probe_done, ep))
      {
        goto next_state;
      }
      break;
    case EP_STATE_MDNS_PROBE_IN_PROGRESS:
      break;
    case EP_STATE_PROBE_FAIL:
    case EP_STATE_PROBE_DONE:
      rd_node_probe_update(ep->node);
      break;
  }
  return;

  next_state: ep->state++;
  rd_ep_probe_update(ep);
  return;

  fail_state: ep->state = EP_STATE_PROBE_FAIL;
  rd_ep_probe_update(ep);
  return;
}

void
rd_probe_next_state(BYTE txStatus, void* user, TX_STATUS_TYPE *t)
{
  rd_node_database_entry_t* n = (rd_node_database_entry_t*) user;

  DBG_PRINTF("rd_probe_next_state\n");
  if (txStatus == TRANSMIT_COMPLETE_OK)
  {
    n->state++;
  }
  else
  {
    n->state = STATUS_PROBE_FAIL;
  }
  rd_node_probe_update(n);
}

/**
 * Read the protocol info from NVM
 */
static void
update_protocol_info(rd_node_database_entry_t* n)
{
  NODEINFO ni;

  ZW_GetNodeProtocolInfo(n->nodeid, &ni);
  n->nodeType = ni.nodeType.basic;

  if (ni.capability & NODEINFO_LISTENING_SUPPORT)
  {
    n->mode = MODE_ALWAYSLISTENING;
    //n->wakeUp_interval = 70*60; //70 Minutes, the node will be probed once each 70 minutes.
  }
  else if (ni.security
      & (NODEINFO_ZWAVE_SENSOR_MODE_WAKEUP_1000
          | NODEINFO_ZWAVE_SENSOR_MODE_WAKEUP_250))
  {
    n->mode = MODE_FREQUENTLYLISTENING;
  }
  else
  {
    n->mode = MODE_NONLISTENING;
  }
}


void
rd_probe_resume()
{
  uint8_t i;
  rd_node_database_entry_t* nd;

  if (current_probe_entry)
  {
     DBG_PRINTF("Resume probe of %u\n", current_probe_entry->nodeid);
    rd_node_probe_update(current_probe_entry);
    return;
  }

  for (i = 1; i <= ZW_MAX_NODES; i++)
  {
    nd = rd_node_get_raw(i);
    if (nd
        && (nd->state != STATUS_DONE && nd->state != STATUS_PROBE_FAIL
            && nd->state != STATUS_FAILING))
    {
      current_probe_entry = nd;
      rd_node_probe_update(nd);
      break;
    }
  }
  if ((i == (ZW_MAX_NODES+1)) && (probe_lock==0))
  {
    process_post(&zip_process, ZIP_EVENT_ALL_NODES_PROBED, 0);
  }
}

static void
rd_node_name_probe_done(int bStatus, void* ctx)
{
  char buf[255];
  rd_node_database_entry_t *n = (rd_node_database_entry_t*) ctx;
  u8_t k;
  ASSERT(n);
  int timer_value = 0;

  DBG_PRINTF("rd_node_name_probe_done status: %d\n", bStatus);
  if (n->state == STATUS_MDNS_PROBE)
  {
    if (bStatus)
    {
      n->state= STATUS_MDNS_EP_PROBE;
    }
    else
    {
      if (tie_braking_won == 1) { /* Need to probe with same name */
        DBG_PRINTF("rd_node_name_probe_done tie_braking_won\n");
        timer_value = 250;
        tie_braking_won = 0;
        goto exit;
      }
      if (tie_braking_won == 2) {
        DBG_PRINTF("rd_node_name_probe_done tie_braking_won = 2\n");
        timer_value = 1000;
        tie_braking_won = 0;
        goto exit;
      }

      /* If this is the default node name, We need to allocate memory for this name*/
      if (n->nodeNameLen == 0)
      {
        n->nodeNameLen = snprintf(buf, sizeof(buf), "zw%08X%02X-1", UIP_HTONL(homeID),
            n->nodeid);
        n->nodename = rd_data_mem_alloc(n->nodeNameLen);
        memcpy(n->nodename, buf, n->nodeNameLen);
      }
      else
      { /**/
        /*Locate trailing digits in the name */
        memcpy(buf, n->nodename, n->nodeNameLen);
        buf[n->nodeNameLen] = 0;
        rd_data_mem_free(n->nodename);
        k = add_or_increment_trailing_digit(buf, n->nodeNameLen, sizeof(buf));
        n->nodeNameLen = k;
        n->nodename = rd_data_mem_alloc(k);
        memcpy(n->nodename, buf, k);
      }
      timer_value = 250 + (750 * (denied_conflict_probes > 10));
      DBG_PRINTF("Delaying rd_node_probe_update by %d ms\n", timer_value);
    }
exit:
    if (timer_value) {
     ctimer_set(&rd_node_probe_update_timer, timer_value, (void (*)(void *)) rd_node_probe_update, n);
     timer_value = 0;
     denied_conflict_probes++;
    } else {
     rd_node_probe_update(n);
    }
  }
}

/**
 *  Lock/Unlock the node probe machine. When the node probe lock is enabled, all probing will stop.
 *  Probing is resumed when the lock is disabled. The probe lock is used during a add node process or during learn mode.
 */
void
rd_probe_lock(uint8_t enable)
{
  probe_lock = enable;
  DBG_PRINTF("Probe machine is %slocked\n", (enable) ? "" : "un");

  if (!probe_lock)
  {
    rd_probe_resume();
  }
}

u8_t
rd_probe_in_progress()
{
  return (current_probe_entry != 0);
}

void
rd_node_probe_update(rd_node_database_entry_t* n)
{
  uint8_t i;
  rd_ep_database_entry_t* ep;
  ts_param_t p;

  static const ZW_MANUFACTURER_SPECIFIC_GET_FRAME man_spec_get =
    { COMMAND_CLASS_MANUFACTURER_SPECIFIC, MANUFACTURER_SPECIFIC_GET };
  static const ZW_MULTI_CHANNEL_END_POINT_GET_V4_FRAME multi_ep_get =
    { COMMAND_CLASS_MULTI_CHANNEL_V4, MULTI_CHANNEL_END_POINT_GET_V4 };
  static const ZW_WAKE_UP_INTERVAL_GET_FRAME wakeup_get =
    { COMMAND_CLASS_WAKE_UP, WAKE_UP_INTERVAL_GET };

  if (probe_lock)
  {
    DBG_PRINTF("probe machine is locked\n");
    return;
  }

  if(bridge_state == booting) {
    DBG_PRINTF("Waiting for bridge\n");
    return;
  }

  if (!(process_is_running(&mDNS_server_process))) {
    DBG_PRINTF("Waiting for mDNS server process\n");
    return;
  }

  if(n->nodeid==0) {
    ASSERT(0);
    return;
  }

  if(is_virtual_node(n->nodeid)) {
    rd_remove_node(n->nodeid);
    ASSERT(0);
    return;
  }
  DBG_PRINTF("rd_node_probe_update state %s node =%d\n", rd_node_probe_state_name(n->state), n->nodeid);
  switch (n->state)
  {
    /*case STATUS_ADDING:
     update_protocol_info(n); //We do this here because the security layer needs to know if this is node is a controller
     break;*/
    case STATUS_CREATED:
      if (current_probe_entry && current_probe_entry != n)
      {
        DBG_PRINTF("another probe is in progress waiting\n");
        return;
      }
      current_probe_entry = n;
      goto next_state;
      break;
      /*case STATUS_PROBE_PROTOCOL_INFO:
       {
       update_protocol_info(n);
       goto next_state;
       break;
       }*/
    case STATUS_PROBE_NODE_INFO:
    {
      rd_ep_database_entry_t* ep = list_head(n->endpoints);

      if (!ep)
      { // Abort the probe the node might have been removed
         DBG_PRINTF("Abort probe\n");
        current_probe_entry = NULL;
        rd_probe_resume();
        return;
      }

      ASSERT(ep->node == n);

      if (ep->state == EP_STATE_PROBE_FAIL)
      {
        DBG_PRINTF("Endpoint probe fail\n");
        n->state = STATUS_PROBE_FAIL;
        rd_node_probe_update(n);
        return;
      }
      else if (ep->state != EP_STATE_PROBE_DONE)
      {
        rd_ep_probe_update(ep);
      }
      else
      {
        if (n->security_flags & NODE_FLAG_INFO_ONLY)
        {
          n->state = STATUS_DONE;
          rd_node_probe_update(n);
        }
        else
        {
          goto next_state;
        }
      }

    }
      break;
    case STATUS_PROBE_PRODUCT_ID:

      if(n->nodeid == MyNodeID) {
        n->productID = cfg.product_id;
        n->manufacturerID = cfg.manufacturer_id;
        n->productType = cfg.product_type;

        goto next_state;
      }



      if ((SupportsCmdClassFlags(n->nodeid, COMMAND_CLASS_MANUFACTURER_SPECIFIC) & SUPPORTED)
          == 0)
      {
        goto next_state;
      }
      ts_set_std(&p, n->nodeid);
      /**
       * Workaround for Schlage door locks with faulty security implementation.
       */
      p.scheme = SupportsCmdClassSecure(n->nodeid, COMMAND_CLASS_MANUFACTURER_SPECIFIC) ? AUTO_SCHEME : NO_SCHEME;
      /* Find manufacturer info */
      if (!ZW_SendRequest(&p, (BYTE*) &man_spec_get, sizeof(man_spec_get),
          MANUFACTURER_SPECIFIC_REPORT, REQUEST_TIMEOUT, n, rd_probe_vendor_callback))
      {
        goto fail_state;
      }

      break;
    case STATUS_ENUMERATE_ENDPOINTS:
      if (n->nodeid == MyNodeID
          || ((SupportsCmdClassFlags(n->nodeid, COMMAND_CLASS_MULTI_CHANNEL_V4) & SUPPORTED)
              == 0))
      {
        goto next_state;
      }

      ts_set_std(&p, n->nodeid);
      /* Query the number of endpoints */
      if (!ZW_SendRequest(&p, (BYTE*) &multi_ep_get, sizeof(multi_ep_get),
          MULTI_CHANNEL_END_POINT_REPORT_V4, 3*20, n, rd_ep_get_callback))
        goto fail_state;
      break;
    case STATUS_PROBE_ENDPOINTS:
    {
      for (ep = list_head(n->endpoints); ep != NULL; ep = list_item_next(ep))
      {
        /*if(ep->endpoint_id==1) continue;*/

        if (ep->state == EP_STATE_PROBE_FAIL)
        {
          n->state = STATUS_PROBE_FAIL;
          rd_node_probe_update(n);
          return;
        }
        else if (ep->state != EP_STATE_PROBE_DONE)
        {
          rd_ep_probe_update(ep);
          return;
        }
      }
      if (ep == NULL)
      {
        goto next_state;
      }
    }
      break;
    case STATUS_CHECK_WU_CC_VERSION:
      if ( (SupportsCmdClassFlags(n->nodeid, COMMAND_CLASS_WAKE_UP) & SUPPORTED) == 0 || !mb_enabled() )
      {
        n->state = STATUS_PROBE_ENDPOINTS;
        rd_node_probe_update(n);
        return;
      }

      /*This is a mailbox node */
      n->mode = MODE_MAILBOX;
      uint8_t suc_node = ZW_GetSUCNodeID();
      /*If the node was added by me then set the wakeup interval*/
      /* Three scenarios:
        1. If I am SIS 
                I set WUI and WAKE UP Destination to myself
        2. If I am inclusion controller(not SIS) and SIS supports COMMAND_CLASS_INCLUSION_CONTROLLER,
                I WONT set WUI and WAKE UP Destination
        3. If I inclusion controller (not SIS) and SIS does not support COMMAND_CLASS_INCLUSION_CONTROLLER
                I set WUI and WAKE UP Destination to SIS
      */
      if ((n->security_flags & NODE_FLAG_JUST_ADDED) &&
          (suc_node == MyNodeID || !SupportsCmdClass(suc_node, COMMAND_CLASS_INCLUSION_CONTROLLER)))
      {
          static ZW_VERSION_COMMAND_CLASS_GET_FRAME v;

          v.cmdClass = COMMAND_CLASS_VERSION;
          v.cmd = VERSION_COMMAND_CLASS_GET;
          v.requestedCommandClass = COMMAND_CLASS_WAKE_UP; 

          ts_set_std(&p, n->nodeid);
    
          if (!ZW_SendRequest(&p, (BYTE*) &v, sizeof(v),
              VERSION_COMMAND_CLASS_REPORT, 3*20, n, rd_wu_cc_version_callback))
          {
            goto fail_state;
          }
      }
      else
      {
        n->state = STATUS_PROBE_WAKE_UP_INTERVAL;
        rd_node_probe_update(n);
        return;
      }
      break;
 
    case STATUS_GET_WU_CAP:
          DBG_PRINTF("");
          static ZW_WAKE_UP_INTERVAL_CAPABILITIES_GET_V2_FRAME cf;
          cf.cmdClass = COMMAND_CLASS_WAKE_UP_V2;
          cf.cmd = WAKE_UP_INTERVAL_CAPABILITIES_GET_V2;
 
          ts_set_std(&p, n->nodeid);
     
          if (!ZW_SendRequest(&p, (BYTE*) &cf, sizeof(cf),
             WAKE_UP_INTERVAL_CAPABILITIES_REPORT_V2, 3*20, n, rd_cap_wake_up_callback))
          {
            goto fail_state;
          }
       break;
    case STATUS_SET_WAKE_UP_INTERVAL:
          DBG_PRINTF("");
          static ZW_WAKE_UP_INTERVAL_SET_V2_FRAME f;

          f.cmdClass = COMMAND_CLASS_WAKE_UP_V2;
          f.cmd = WAKE_UP_INTERVAL_SET_V2;

          if (portable) {
            portable = 0;
            n->wakeUp_interval = 0;
          }
          f.seconds1 = (n->wakeUp_interval >> 16) & 0xFF;
          f.seconds2 = (n->wakeUp_interval >> 8) & 0xFF;
          f.seconds3 = (n->wakeUp_interval >> 0) & 0xFF;
          f.nodeid = ZW_GetSUCNodeID();

          ts_set_std(&p, n->nodeid);
          if (!ZW_SendDataAppl(&p, &f, sizeof(ZW_WAKE_UP_INTERVAL_SET_V2_FRAME),
              rd_probe_next_state, n))
          {
            goto fail_state;
          }
      break;
    case STATUS_ASSIGN_RETURN_ROUTE:
      if (!ZW_AssignReturnRoute(n->nodeid, MyNodeID, AssignReturnRouteCallback))
      {
        goto fail_state;
      }
      break;
    case STATUS_PROBE_WAKE_UP_INTERVAL:
      ts_set_std(&p, n->nodeid);

      if (!ZW_SendRequest(&p, (BYTE*) &wakeup_get, sizeof(wakeup_get),
          WAKE_UP_INTERVAL_REPORT, REQUEST_TIMEOUT, n, rd_probe_wakeup_callback))
      {
        goto fail_state;
      }
      break;
    case STATUS_MDNS_PROBE:
      if (!mdns_node_name_probe(n, rd_node_name_probe_done, n))
      {
        goto next_state;
      }
      break;
    case STATUS_MDNS_EP_PROBE:
      /* This is used in initial probing. */
      for (ep = list_head(n->endpoints); ep != NULL; ep = list_item_next(ep))
      {
        if (ep->state == EP_STATE_MDNS_PROBE)
        {
          rd_ep_probe_update(ep);
          return;
        }
      }
      goto next_state;
      break;
    case STATUS_DONE:
      LOG_PRINTF("Probe of node %d is done\n", n->nodeid);
      for (ep = list_head(n->endpoints); ep != NULL; ep = list_item_next(ep))
      {
        LOG_PRINTF("Info len %i\n", ep->endpoint_info_len);
      }
      n->lastUpdate = clock_seconds();
      n->lastAwake = clock_seconds();
      n->security_flags &= ~NODE_FLAG_JUST_ADDED;
      n->security_flags &= ~NODE_FLAG_ADDED_BY_ME;
      goto probe_complete;
      break;
    case STATUS_PROBE_FAIL:
      ERR_PRINTF("Probe of node %d failed\n", n->nodeid);
      goto probe_complete;
      break;
    default:
      break;
  }
  return;

  next_state: n->state++;
  rd_node_probe_update(n);
  return;
  fail_state: n->state = STATUS_PROBE_FAIL;
  rd_node_probe_update(n);
  return;

  probe_complete:
  /* Store all node data in persistent memory */
  rd_data_store_nvm_free(n);
  rd_data_store_nvm_write(n);
  current_probe_entry = NULL;

  /*Send out notification for all endpoints */
  for (ep = list_head(n->endpoints); ep != NULL; ep = list_item_next(ep))
  {
    mdns_endpoint_nofify(ep, 0);

    /*Trigger endpoint probe done event*/
    for (i = 0; i < NUM_PROBES; i++)
    {
      if (ep_probe[i].node_id == ep->node->nodeid
          && ep_probe[i].epid == ep->endpoint_id)
      {
        ep_probe[i].node_id = 0;
        if (ep_probe[i].callback)
        {
          ep_probe[i].callback(ep, ep_probe[i].user);
        }
      }
    }
  }

  rd_probe_resume();
}

void
rd_register_new_node(uint8_t node, uint8_t probe_flags)
{
  rd_node_database_entry_t *n;
  rd_ep_database_entry_t * ep;
  //char name[16];

  DBG_PRINTF(" nodeid=%d 0x%02x\n", node, probe_flags);
  if (is_virtual_node(node))
  {
    return;
  }

  ASSERT((node>0) && (node < (ZW_MAX_NODES +1)));

  n = rd_node_get_raw(node);

  ipv46nat_add_entry(node);

  if (n)
  {
    if (n->state == STATUS_FAILING || n->state == STATUS_PROBE_FAIL
        || n->state == STATUS_DONE)
    {
      DBG_PRINTF("re-probing node %i old state %s\n",n->nodeid, rd_node_probe_state_name(n->state));
      ASSERT(n->nodeid == node);
      n->state = STATUS_CREATED;

      for (ep = list_head(n->endpoints); ep != NULL; ep = list_item_next(ep))
      {
        ep->state = EP_STATE_PROBE_INFO;
      }

      return rd_node_probe_update(n);
    }
    else
    {
       WRN_PRINTF("Node probe is already in progress on node %u.\n", n->nodeid);
      return;
    }
  }

  n = rd_node_entry_alloc(node);

  if (!n)
  {
    ERR_PRINTF("Unable to register new node Out of mem!\n");
    return;
  }

  memset(n,0,sizeof(rd_node_database_entry_t)),

  n->nodeid = node;
  ipOfNode(&(n->ipv6_address), n->nodeid);

  /*When node name is 0, then we use the default names*/
  n->nodeNameLen = 0;
  n->nodename = 0;
  n->state = STATUS_CREATED;
  n->mode = MODE_PROBING;
  n->security_flags = 0;
  update_protocol_info(n); //Get protocol info new because this is always sync

  n->security_flags |= probe_flags;

  n->wakeUp_interval = DEFAULT_WAKE_UP_INTERVAL ; //Default wakeup interval is 70 minutes

  LIST_STRUCT_INIT(n, endpoints);

  /*Endpoint 0 always exists*/
  if (!rd_add_endpoint(n, 0))
  {
    n->state = STATUS_PROBE_FAIL;
    rd_node_probe_update(n);
  } else {
    /* Since status is CREATED, this node will be set as
     * current_probe_entry if there is not already a current.  This
     * ensures that if the probe machine is currently locked, it will
     * resume probing as soon as it is unlocked and eventually get to
     * this node.  */
    rd_node_probe_update(n);
  }
}

/**
 * Called to instruct the RD to shutdown and send goodbye messages.
 * the function cbExitDone is called when there are no more messages to send.
 *
 */
void
rd_exit()
{

  rd_node_database_entry_t *n;
  rd_ep_database_entry_t *ep;
  uint8_t i;

  ctimer_stop(&dead_node_timer);
  ctimer_stop(&nif_request_timer);

  for (i = 1; i <= ZW_MAX_NODES; i++)
  {
    n = rd_node_get_raw(i);
    if (n)
    {
      n->mode |= MODE_FLAGS_DELETED;
      for (ep = list_head(n->endpoints); ep != NULL; ep = list_item_next(ep))
      {
        mdns_endpoint_nofify(ep, 0);
      }
    }
  }
  mdns_exit();
}

void
rd_remove_node(uint8_t node)
{
  rd_node_database_entry_t *n;
  rd_ep_database_entry_t *ep;

  ZW_Abort_SendRequest(node);
  if (node == 0)
    return;
  n = rd_node_get_raw(node);
  if (n == 0)
    return;

  /*
   * Abort probe if we have one in progress.
   */
  if(current_probe_entry == n) {
    current_probe_entry = 0;
  }

  DBG_PRINTF("Removing node %i %p\n", node, n);

  if (n->mode == MODE_MAILBOX )
  {
    /*Clear the mailbox for potential entries*/
    mb_failing_notify(n->nodeid);
  }

  n->mode |= MODE_FLAGS_DELETED;
  if (n->nodeid == 0) {
    return;
  }
  for (ep = list_head(n->endpoints); ep != NULL; ep = list_item_next(ep))
  {
    //DBG_PRINTF("Removing endpoint %s %p\n", ep->endpoint_name,ep);
    mdns_endpoint_nofify(ep, 1);
  }

  /* Clear node from data storage */
  rd_node_entry_free(node);

  ipv46nat_del_entry(node);
}

int
rd_init(uint8_t lock)
{
  uint8_t i;
  uint8_t nodelist[29];
  uint8_t ver, capabilities, len, chip_type, chip_version;
  rd_node_database_entry_t* nd;
  DWORD old_homeID;

  data_store_init();

  old_homeID = homeID;
  MemoryGetID((BYTE*) &homeID, &MyNodeID);
  LOG_PRINTF("HomeID is %08x node id %d\n",UIP_HTONL(homeID),MyNodeID);
  ipv46nat_init();
  /*Make sure the virtual node mask is up to date*/
  register_virtual_nodes();

  memset(&ep_probe, 0, sizeof(ep_probe));

  nif_request_ep = 0;
  if (rd_probe_in_progress()) {
     ERR_PRINTF("RD re-initialized while probing\n");
  }
  current_probe_entry = 0;
  probe_lock = lock;

  SerialAPI_GetInitData(&ver, &capabilities, &len, nodelist, &chip_type,
      &chip_version);
  /*Always update the entry for this node, since this is without cost, and network role
   * might have changed. */
  rd_register_new_node(MyNodeID, 0x00);
  DBG_PRINTF("Requesting probe of gw, node id %u\n", MyNodeID);

  /* i is a nodeid */
  for (i = 1; i <= ZW_MAX_NODES; i++)
  {
    if (BIT8_TST((i-1),nodelist))
    {
      if (i == MyNodeID)
        continue;

      DBG_PRINTF("Network has node %i\n", i);

      nd = rd_node_entry_import(i);
      
      if (nd == 0)
      {
        rd_register_new_node(i, 0x00);
      }
      else
      {
        rd_node_database_entry_t* n = rd_node_get_raw(i);
        rd_ep_database_entry_t *ep;

        /*Create nat entry for node */
        ipv46nat_add_entry(i);

        /*Schedule all names to be re-probed.*/
        if (n->state == STATUS_DONE)
        {
          /* Here we just fake that the nodes has been alive recently.
           * This is to prevent that the node becomes failing without
           * reason*/
          n->lastAwake = clock_seconds();
          n->state = STATUS_MDNS_PROBE;
          for (ep = list_head(n->endpoints); ep != NULL;
              ep = list_item_next(ep))
          {
            ep->state = EP_STATE_MDNS_PROBE;
          }
        }
        /* Mark wakeup nodes without fixed interval as recently
         * alive. This prevents frames queued to them from being
         * dropped too early after a gateway restart. */
        if ((n->mode == MODE_MAILBOX) && (n->wakeUp_interval == 0))
        {
          n->lastAwake = clock_seconds();
        }
      }
    }
    else
    {
      rd_remove_node(i);
    }
  }

  ctimer_set(&dead_node_timer, 60 * 1000, rd_check_for_dead_nodes_worker,
      (void*) 1);
  rd_probe_resume();
  DBG_PRINTF("Resource directory init done\n");
  return (old_homeID != homeID);
}

void
rd_full_network_discovery()
{
  uint8_t nodelist[29];
  uint8_t ver, capabilities, len, chip_type, chip_data;
  uint8_t i;

  SerialAPI_GetInitData(&ver, &capabilities, &len, nodelist, &chip_type,
      &chip_data);

  for (i = 0; i < ZW_MAX_NODES; i++)
  {
    if (BIT8_TST(i,nodelist))
    {
      rd_register_new_node(i + 1, 0x00);
    }
    else
    {
      rd_remove_node(i + 1);
    }
  }
}

u8_t
rd_probe_new_nodes()
{
  uint8_t nodelist[29];
  uint8_t ver, capabilities, len, chip_type, chip_data;
  uint8_t i, k;

  SerialAPI_GetInitData(&ver, &capabilities, &len, nodelist, &chip_type,
      &chip_data);
  k = 0;
  for (i = 1; i <= ZW_MAX_NODES; i++)
  {
    if (BIT8_TST(i-1,nodelist) && !rd_node_exists(i) && !is_virtual_node(i))
    {
      rd_register_new_node(i, 0x00);
      k++;
    }
  }
  return k;
}
/******************************************* Iterators ***************************************/

rd_ep_database_entry_t*
rd_ep_iterator_group_begin(rd_group_entry_t* ge)
{
  return 0;
}

rd_ep_database_entry_t*
rd_group_ep_iterator_next(rd_group_entry_t* ge, rd_ep_database_entry_t* ep)
{
  return 0;
}

/**
 *
 * Supported  non Sec
 * Controlled non Sec
 * Supported Sec
 * Controlled Sec
 *
 */
int
rd_ep_class_support(rd_ep_database_entry_t* ep, uint16_t cls)
{
  int i;
  int bSecureClass;
  int bControlled;
  u8_t result;
  u16_t c;
  if (!ep->endpoint_info)
    return 0;

  if(ep->node->mode & MODE_FLAGS_DELETED) {
    return 0;
  }

  bSecureClass = 0;
  bControlled = 0;
  result = 0;

  for (i = 2; i < ep->endpoint_info_len; i++)
  {
    c = ep->endpoint_info[i];
    if ((c & 0xF0) == 0xF0 && (i < ep->endpoint_info_len - 1))
    {
      i++;
      c = ((c & 0xFF) << 8) | ep->endpoint_info[i];
    }

    if (c == COMMAND_CLASS_SECURITY_SCHEME0_MARK)
    {
      bSecureClass = 1;
      bControlled = 0;
    }
    else if (c == COMMAND_CLASS_MARK)
    {
      bControlled = 1;
    }
    else if (c == cls)
    {
      result |= 1 << ((bSecureClass << 1) | bControlled);
    }
  }
  //DBG_PRINTF("Result %x\n", result);
  return result;
}

u8_t
rd_get_ep_name(rd_ep_database_entry_t* ep, char* buf, u8_t size)
{
  if (ep->endpoint_name && ep->endpoint_name_len)
  {
    if (size > ep->endpoint_name_len)
    {
      size = ep->endpoint_name_len;
    }
    memcpy(buf, ep->endpoint_name, size);
    return size;
  }
  else
  {

    if (ep->endpoint_info && (ep->endpoint_info_len > 2))
    {
      const char* type_str = get_gen_type_string(ep->endpoint_info[0]);
      return snprintf(buf, size, "%s [%04x%02x%02x]",
          type_str, UIP_HTONL(homeID), ep->node->nodeid,
          ep->endpoint_id);
    }
    else
    {
      return snprintf(buf, size, "(unknown) [%04x%02x%02x]", UIP_HTONL(homeID),
          ep->node->nodeid, ep->endpoint_id);
    }
  }
}
u8_t
rd_get_ep_location(rd_ep_database_entry_t* ep, char* buf, u8_t size)
{
  if (size > ep->endpoint_loc_len)
  {
    size = ep->endpoint_loc_len;
  }

  memcpy(buf, ep->endpoint_location, size);
  return size;
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

void
rd_update_ep_name_and_location(rd_ep_database_entry_t* ep, char* name,
    u8_t name_size, char* location, u8_t location_size)
{
  char* new_name;
  char* new_location;

  if (ep->node->state != STATUS_DONE)
  {
    WRN_PRINTF("Not setting the node name, because probe is not done\n");
    return;
  }

  if (!validate_name(name, name_size))
  {
    ERR_PRINTF("Invalid name %s\n", name);
    return;
  }

  if (location_size > 0 && !validate_location(location, location_size))
  {
    ERR_PRINTF("Invalid location %s\n", location);
    return;
  }

  if (((name_size + location_size) > 63) || name_size < 1)
  {
    ERR_PRINTF("Resource names must be between 1 and  63 bytes.\n");
    return;
  }

  if (rd_lookup_by_ep_name(name, location))
  {
    WRN_PRINTF("Already have an endpoint with this name\n");
    return;
  }

  new_name = rd_data_mem_alloc(name_size);
  memcpy(new_name, name, name_size);

  if (location_size > 0)
  {
    new_location = rd_data_mem_alloc(location_size);
    memcpy(new_location, location, location_size);
  }
  else
  {
    new_location = 0;
  }

  /*Send a mDNS goodbye for this endpoint right away */
  ep->node->mode |= MODE_FLAGS_DELETED;
  mdns_endpoint_nofify(ep, 1);
  ep->node->mode &= ~MODE_FLAGS_DELETED;

  if (ep->endpoint_name_len)
  {
    rd_data_mem_free(ep->endpoint_name);
  }
  if (ep->endpoint_loc_len)
  {
    rd_data_mem_free(ep->endpoint_location);
  }

  ep->endpoint_name_len = name_size;
  ep->endpoint_name = new_name;

  ep->endpoint_location = new_location;
  ep->endpoint_loc_len = location_size;

  ep->state = EP_STATE_MDNS_PROBE;
  ep->node->state = STATUS_MDNS_EP_PROBE;
  DBG_PRINTF("Setting cache-flush bit as its new name\n");
  dont_set_cache_flush_bit = 1;
  if (current_probe_entry == NULL) {
     current_probe_entry=ep->node;
     rd_node_probe_update(ep->node);
  }
  /* If there is a current probe entry, the probe machine will
   * eventually get around to this node, as well. */
}

void
rd_update_ep_name(rd_ep_database_entry_t* ep, char* name, u8_t size)
{
  rd_update_ep_name_and_location(ep, name, size, ep->endpoint_location,
      ep->endpoint_loc_len);
}

void
rd_update_ep_location(rd_ep_database_entry_t* ep, char* name, u8_t size)
{

  rd_update_ep_name_and_location(ep, ep->endpoint_name, ep->endpoint_name_len,
      name, size);
}

/*
 * Lookup an endpoint from its service name
 * \return NULL if no endpoint is found otherwise return the ep sructure.
 */
rd_ep_database_entry_t*
rd_lookup_by_ep_name(const char* name, const char* location)
{
  rd_ep_database_entry_t* ep;
  uint8_t j;
  char buf[64];

  for (ep = rd_ep_first(0); ep; ep = rd_ep_next(0, ep))
  {
    j = rd_get_ep_name(ep, buf, sizeof(buf));
    if (mdns_str_cmp(buf, name, j) == 0)
    {
      if (location)
      {
        if (mdns_str_cmp(ep->endpoint_location, location, ep->endpoint_loc_len))
        {
          return ep;
        }
      }
      else
      {
        return ep;
      }
    }
  }
  return 0;
}

rd_group_entry_t*
rd_lookup_group_by_name(const char* name)
{
  return 0;
}

/**
 * MUST be called when a node entry is no longer needed.
 */
void
rd_free_node_dbe(rd_node_database_entry_t* n)
{
  if (n)
  {
    ASSERT(n->refCnt>0);
    n->refCnt--;
  }
}

rd_ep_database_entry_t*
rd_get_ep(uint8_t nodeid, uint8_t epid)
{
  rd_node_database_entry_t* n = rd_get_node_dbe(nodeid);
  rd_ep_database_entry_t* ep = NULL;

  if (n == NULL)
  {
    return NULL;
  }
  for (ep = list_head(n->endpoints); ep != NULL; ep = list_item_next(ep))
  {
    if (ep->endpoint_id == epid)
    {
      break;
    }
  }
  rd_free_node_dbe(n);
  return ep;
}
rd_node_mode_t
rd_get_node_mode(uint8_t nodeid)
{
  /* FIXME: Choose a better default value in case lookup fails */
  rd_node_mode_t mode = MODE_FLAGS_DELETED;
  rd_node_database_entry_t *n = rd_get_node_dbe(nodeid);
  //ASSERT(n);
  if (n)
  {
    mode = n->mode;
    rd_free_node_dbe(n);
  }
  return mode;
}

void rd_mark_node_deleted(uint8_t nodeid) {
  /* FIXME: Choose a better default value in case lookup fails */
  rd_node_database_entry_t *n = rd_get_node_dbe(nodeid);
  //ASSERT(n);
  if (n)
  {
    n->mode |= MODE_FLAGS_DELETED;
    rd_free_node_dbe(n);
  }
}

rd_node_state_t
rd_get_node_state(uint8_t nodeid)
{
  /* FIXME: Choose a better default value in case lookup fails */
  rd_node_state_t state = STATUS_PROBE_FAIL;
  rd_node_database_entry_t *n = rd_get_node_dbe(nodeid);
  if (n)
  {
    state = n->state;
    rd_free_node_dbe(n);
  }
  return state;
}



/***************Reimplementaion of nodecache *****************************/
/*
 * Returns a bit field of Secure and non-secure support for given node
 */
static int
SupportsCmdClassFlags(BYTE nodeid, WORD class)
{
  rd_node_database_entry_t* n;
  int rc;

  rc = 0;
  n = rd_get_node_dbe(nodeid);
  if (n)
  {
    if (list_head(n->endpoints))
    {

      rc = rd_ep_class_support(
          (rd_ep_database_entry_t*) list_head(n->endpoints), class);
    }
    rd_free_node_dbe(n);
  }

  return rc;
}

int
SupportsCmdClass(BYTE nodeid, WORD class)
{
  return ((SupportsCmdClassFlags(nodeid, class) & SUPPORTED_NON_SEC) > 0);
}

int
SupportsCmdClassSecure(BYTE nodeid, WORD class)
{
  return ((SupportsCmdClassFlags(nodeid, class) & SUPPORTED_SEC) > 0);
}

static int rd_ep_supports_cmd_class_nonsec(rd_ep_database_entry_t* ep, uint16_t class)
{
   return ((rd_ep_class_support(ep, class) & SUPPORTED_NON_SEC) > 0);
}

/**
 * Set node attribute flags
 */BYTE
SetCacheEntryFlagMasked(BYTE nodeid, BYTE value, BYTE mask)
{
  rd_node_database_entry_t* n;

  n = rd_get_node_dbe(nodeid);
  if (n)
  {
    n->security_flags = (n->security_flags & (~mask)) | value;
    rd_data_store_update(n);

    rd_free_node_dbe(n);
  }
  else
  {
    ERR_PRINTF("Attempt to set flag of non existing node = %i\n", nodeid);
  }
  return 1;
}

/**
 * Retrieve Cache entry flag
 */BYTE
GetCacheEntryFlag(BYTE nodeid)
{
  rd_node_database_entry_t* n;
  uint8_t rc;

  if(is_virtual_node(nodeid)) {
    n = rd_get_node_dbe(MyNodeID);
  } else {
    n = rd_get_node_dbe(nodeid);
  }

  if (!n)
  {
    ERR_PRINTF("GetCacheEntryFlag: on non existing node=%i\n",nodeid);
    return 0;
  }
  rc = n->security_flags;
  rd_free_node_dbe(n);
  return rc;
}

int
isNodeController(BYTE nodeid)
{
  rd_node_database_entry_t* n;
  uint8_t rc = 0;

  n = rd_get_node_dbe(nodeid);
  if (n)
  {
    DBG_PRINTF("isNodeController(%d) type =%d\n", nodeid, n->nodeType);
    rc = (n->nodeType == BASIC_TYPE_CONTROLLER)
        || (n->nodeType == BASIC_TYPE_STATIC_CONTROLLER);
    rd_free_node_dbe(n);
  }
  else
  {
    ERR_PRINTF("isNodeController: on non existing node\n");
  }
  return rc;
}

void
rd_node_is_alive(uint8_t node)
{
  rd_node_database_entry_t *n;
  n = rd_get_node_dbe(node);
  if (n)
  {
    n->lastAwake = clock_seconds();
    rd_set_failing(n, FALSE);
    rd_free_node_dbe(n);
  }
}
