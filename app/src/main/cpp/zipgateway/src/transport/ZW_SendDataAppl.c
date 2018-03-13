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
#include "Serialapi.h"
#include "contiki.h"
#include "ZW_SendDataAppl.h"
#include "Security_Scheme0.h"
#include "ZIP_Router.h"
#include "NodeCache.h"
//#include "ZW_Transport_Service.h"
#include "transport_service2.h"
#include "list.h"
#include "memb.h"
#include "ctimer.h"
#include "security_layer.h"
#include "CC_FirmwareUpdate.h"
#include "ClassicZIPNode.h"
#include "CC_InstalltionAndMaintenance.h"
#include "node_queue.h"
#include "S2_wrap.h"

#include<stdlib.h>
#include "ZW_transport_api.h"
/*
 * SendData Hierarchy, each, level wraps the previous. A higher level call MUST only call lower level calls.
 *
 * SendRequest          - available from application space, send a command and wait for a reply
 * SendDataAppl         - available from application space, send a command and put it in a queue
 * SendEndpoint         - internal call, single session call add endpoint header
 * SendSecurity         - internal call, single session call add security header
 * SendTransportService - internal call, single session call do transport service fragmentation
 * SendData             - internal call, single session send the data
 */

typedef struct
{
  void* next;
  const void *pData;
  uint16_t dataLength;
  ts_param_t param;
  void* user;
  ZW_SendDataAppl_Callback_t callback;
} send_data_appl_session_t;

static uint8_t lock = 0;
static struct etimer emergency_timer;
static struct ctimer backoff_timer;
static clock_time_t t_tx_start;

LIST(session_list);
MEMB(session_memb, send_data_appl_session_t, 8);

static uint8_t lock_ll = 0;
LIST(send_data_list);


static security_scheme_t
ZW_SendData_scheme_select(const ts_param_t* param, const u8_t* data, u8_t len);


PROCESS(ZW_SendDataAppl_process, "ZW_SendDataAppl_process process");

enum
{
  SEND_EVENT_SEND_NEXT, SEND_EVENT_SEND_NEXT_LL,
};

void
ts_set_std(ts_param_t* p, u8_t dnode)
{
  p->dendpoint = 0;
  p->sendpoint = 0;
  p->snode = MyNodeID;
  p->dnode = dnode;
  p->rx_flags = 0;
  p->tx_flags = TRANSMIT_OPTION_ACK | TRANSMIT_OPTION_AUTO_ROUTE
      | TRANSMIT_OPTION_EXPLORE;
  p->scheme = AUTO_SCHEME;
}

void
ts_param_make_reply(ts_param_t* dst, const ts_param_t* src)
{
  dst->snode = src->dnode;
  dst->dnode = src->snode;
  dst->sendpoint = src->dendpoint;
  dst->dendpoint = src->sendpoint;
  dst->scheme = ZW_SendData_scheme_select(src, 0, 2);


  dst->tx_flags =
      ((src->rx_flags & RECEIVE_STATUS_LOW_POWER) ?
          TRANSMIT_OPTION_LOW_POWER : 0) | TRANSMIT_OPTION_ACK
          | TRANSMIT_OPTION_EXPLORE | TRANSMIT_OPTION_AUTO_ROUTE;
}

/**
 * Returns true if source and destinations are identical
 */
u8_t
ts_param_cmp(ts_param_t* a1, const ts_param_t* a2)
{
  return (a1->snode == a2->snode && a1->dnode == a2->dnode
      && a1->sendpoint == a2->sendpoint && a1->dendpoint == a2->dendpoint);
}

static send_data_appl_session_t* current_session;
static send_data_appl_session_t* current_session_ll;

void
ZW_SendDataAppl_CallbackEx(uint8_t status, void*user, TX_STATUS_TYPE *t) REENTRANT
{
  send_data_appl_session_t* s = (send_data_appl_session_t*) user;

  if (!lock)
  {
    ERR_PRINTF("Double callback! ");
    return;
  }
  lock = FALSE;
  free((void*) s->pData);
  memb_free(&session_memb, s);

  /*if(status==TRANSMIT_COMPLETE_FAIL) {
   DBG_PRINTF("Actual transmitter fault!");
   }*/

  if (s->callback)
  {
    s->callback(status, s->user, t);
  }
  process_post(&ZW_SendDataAppl_process, SEND_EVENT_SEND_NEXT, NULL);
}

security_scheme_t
highest_scheme(uint8_t scheme_mask)
{
  if(scheme_mask & NODE_FLAG_SECURITY2_ACCESS) {
    return SECURITY_SCHEME_2_ACCESS;
  } else if(scheme_mask & NODE_FLAG_SECURITY2_AUTHENTICATED) {
    return SECURITY_SCHEME_2_AUTHENTICATED;
  } else if(scheme_mask & NODE_FLAG_SECURITY2_UNAUTHENTICATED) {
    return SECURITY_SCHEME_2_UNAUTHENTICATED;
  }else if(scheme_mask & NODE_FLAG_SECURITY0) {
    return SECURITY_SCHEME_0;
  } else {
    return NO_SCHEME;
  }
}

static security_scheme_t
ZW_SendData_scheme_select(const ts_param_t* param, const u8_t* data, u8_t len)
{
  u8_t dst_scheme_mask=0; //Mask of schemes supported by destination
  u8_t src_scheme_mask=0;
  static security_scheme_t scheme;
  dst_scheme_mask = GetCacheEntryFlag(param->dnode);
  src_scheme_mask = GetCacheEntryFlag(MyNodeID);

  /* Check that this node node has at least one security scheme */

  if (len < 2 || (dst_scheme_mask & NODE_FLAG_KNOWN_BAD) || (src_scheme_mask & NODE_FLAG_KNOWN_BAD) )
  {
    return NO_SCHEME;
  }

  //Common schemes
  dst_scheme_mask &= src_scheme_mask;

  switch (param->scheme) {
  case AUTO_SCHEME:

    scheme=  highest_scheme(dst_scheme_mask);
    if(scheme == NO_SCHEME && SupportsCmdClass(param->dnode, COMMAND_CLASS_CRC_16_ENCAP)) {
      return USE_CRC16;
    } else {
      return scheme;
    }
  case NO_SCHEME:
    return NO_SCHEME;
  case USE_CRC16:
    return USE_CRC16;
  case SECURITY_SCHEME_2_ACCESS:
    if(dst_scheme_mask & NODE_FLAG_SECURITY2_ACCESS) return param->scheme;
    break;
  case SECURITY_SCHEME_2_AUTHENTICATED:
    if(dst_scheme_mask & NODE_FLAG_SECURITY2_AUTHENTICATED) return param->scheme;
    break;
  case SECURITY_SCHEME_2_UNAUTHENTICATED:
    if(dst_scheme_mask & NODE_FLAG_SECURITY2_UNAUTHENTICATED) return param->scheme;
    break;
  case SECURITY_SCHEME_0:
    if(dst_scheme_mask & NODE_FLAG_SECURITY0) return param->scheme;
    break;
  default:
    break;
  }
  WRN_PRINTF("Scheme %x NOT supported by destination %i \n",param->scheme,param->dnode);
  return param->scheme;
}

static void
backoff_release(void * data)
{
  process_post(&ZW_SendDataAppl_process, SEND_EVENT_SEND_NEXT_LL, NULL);
}

void
send_data_callback_func(u8_t status, TX_STATUS_TYPE* ts)
{
  enum en_queue_state queue_state = get_queue_state();
  clock_time_t dt;


  if(!lock_ll) {
    ERR_PRINTF("Double callback?\n");
    return;
  }

  send_data_appl_session_t *s = list_pop(send_data_list);

  /* Call only if short queue (queue_state = 1) and status is TRANSMIT_COMPLETE_OK */
  /* Call all the time queue state is long queue (2)*/
  /* Do not call if queue state is idle(0) */
  //if (((queue_state == QS_SENDING_FIRST) && (status == TRANSMIT_COMPLETE_OK)) || (queue_state == QS_SENDING_LONG))
  if(status == TRANSMIT_COMPLETE_OK) //djnakata
  {
      ima_send_data_done(s->param.dnode,status);
  }
  etimer_stop(&emergency_timer);

  //if(status==TRANSMIT_COMPLETE_OK) {
#if NO_BRIDGE
  clasic_session_keepalive(s->param.dnode);
#endif

  //}

  memb_free(&session_memb, s);
  lock_ll = FALSE;
  if (s)
  {
    if (s->callback)
    {
      s->callback(status, s->user, ts);
    }
  }
  else
  {
    ASSERT(0);
  }

  dt = clock_time() - t_tx_start;
  if (dt > 1500)
  {
    WRN_PRINTF("Backing off....\n");
    ctimer_set(&backoff_timer, dt, backoff_release, 0);
  } else {
    process_post(&ZW_SendDataAppl_process, SEND_EVENT_SEND_NEXT_LL, NULL);
  }
}
/**
 * Low level send data. This call will not do any encapsulation except transport service encap
 */
u8_t
send_data(ts_param_t* p, const u8_t* data, u16_t len,
    ZW_SendDataAppl_Callback_t cb, void* user)
{
  send_data_appl_session_t* s;

  s = memb_alloc(&session_memb);
  if (s == 0)
  {
    DBG_PRINTF("OMG! No more queue space");
    return FALSE;
  }
  s->pData = data;
  s->dataLength = len;
  s->param = *p;
  s->user = user;
  s->callback = cb;

  list_add(send_data_list, s);
  process_post(&ZW_SendDataAppl_process, SEND_EVENT_SEND_NEXT_LL, NULL);
  return TRUE;
}

/**
 * Send data to an endpoint and do endpoint encap security encap CRC16 or transport service encap
 * if needed. This function is not reentrant. It will only be called from the ZW_SendDataAppl event tree
 * @param p
 * @param data
 * @param len
 * @param cb
 * @param user
 * @return
 */
u8_t
send_endpoint(ts_param_t* p, const u8_t* data, u16_t len, ZW_SendDataAppl_Callback_t cb, void* user)
{
  u16_t new_len;
  security_scheme_t scheme;
  static u8_t new_buf[UIP_BUFSIZE]; //Todo we should have some max frame size

  new_len = len;

  if (len > sizeof(new_buf))
    return FALSE;

  if (p->dendpoint || p->sendpoint)
  {
    new_buf[0] = COMMAND_CLASS_MULTI_CHANNEL_V2;
    new_buf[1] = MULTI_CHANNEL_CMD_ENCAP_V2;
    new_buf[2] = p->sendpoint;
    new_buf[3] = p->dendpoint;
    new_len += 4;

    if (new_len > sizeof(new_buf))
      return FALSE;

    memcpy(&new_buf[4], data, len);
  }
  else
  {
    memcpy(new_buf, data, len);
  }

  /*Select the right security shceme*/
  scheme = ZW_SendData_scheme_select(p, data, len);
  LOG_PRINTF("Sending with scheme %d\n", scheme);
  switch (scheme)
  {
  case USE_CRC16:
    /* CRC16 Encap frame if destination supports it and if its a single fragment frame
     *
     *
     * */
    if (new_len < META_DATA_MAX_DATA_SIZE && new_buf[0] != COMMAND_CLASS_TRANSPORT_SERVICE
        && new_buf[0] != COMMAND_CLASS_SECURITY && new_buf[0] != COMMAND_CLASS_SECURITY_2
        && new_buf[0] != COMMAND_CLASS_CRC_16_ENCAP)
    {
      WORD crc;
      memmove(new_buf + 2, new_buf, new_len);
      new_buf[0] = COMMAND_CLASS_CRC_16_ENCAP;
      new_buf[1] = CRC_16_ENCAP;
      crc = ZW_CheckCrc16(CRC_INIT_VALUE, (BYTE*) new_buf, new_len + 2);
      new_buf[2 + new_len] = (crc >> 8) & 0xFF;
      new_buf[2 + new_len + 1] = (crc >> 0) & 0xFF;
      new_len += 4;
    }
    return send_data(p, new_buf, new_len, cb, user);
  case NO_SCHEME:
    return send_data(p, new_buf, new_len, cb, user);
    break;
  case SECURITY_SCHEME_0:
    return sec0_send_data(p, new_buf, new_len, cb, user);
    break;
  case SECURITY_SCHEME_2_ACCESS:
  case SECURITY_SCHEME_2_AUTHENTICATED:
  case SECURITY_SCHEME_2_UNAUTHENTICATED:
    p->scheme = scheme;
    return sec2_send_data(p, new_buf, new_len, cb, user);
    break;
  default:
    return FALSE;
    break;
  }
}

static void
send_first()
{
  current_session = list_pop(session_list);

  if (!current_session)
  {
    return;
  }

  lock = TRUE;

  if (!send_endpoint(&current_session->param, current_session->pData,
      current_session->dataLength, ZW_SendDataAppl_CallbackEx, current_session))
  {
    ZW_SendDataAppl_CallbackEx(TRANSMIT_COMPLETE_ERROR, current_session, NULL);
  }
}

uint8_t
ZW_SendDataAppl(ts_param_t* p, const void *pData, uint16_t dataLength,
    ZW_SendDataAppl_Callback_t callback, void* user)
{
  send_data_appl_session_t* s;
  unsigned char *c = (unsigned char *)pData;

  DBG_PRINTF("Sending %d->%d, class: 0x%x, cmd: 0x%x\n", p->snode, p->dnode, c[0], c[1]);
  s = memb_alloc(&session_memb);
  if (s == 0)
  {
    DBG_PRINTF("OMG! No more queue space");
    return FALSE;
  }

  s->pData = malloc(dataLength);
  if (s->pData == NULL)
  {
    memb_free(&session_memb, s);
    ERR_PRINTF("ZW_SendDataAppl: malloc failed\r\n");
    return FALSE;
  }
  memcpy((void*)s->pData, pData, dataLength);

  s->dataLength = dataLength;
  s->param = *p;
  s->user = user;
  s->callback = callback;

  list_add(session_list, s);
  process_post(&ZW_SendDataAppl_process, SEND_EVENT_SEND_NEXT, NULL);

  /*return a handle that we may use to abort the tranmission */
  return ( ((void*)s -session_memb.mem ) /session_memb.size) + 1;
}


void
ZW_SendDataApplAbort(uint8_t handle ) {
  send_data_appl_session_t* s;
  uint8_t h = handle -1;

  if(handle==0 || handle > session_memb.num) {
    ERR_PRINTF("ZW_SendDataAppl_Abort: Invalid handle\n");
    return;
  }

  s = (send_data_appl_session_t*)( session_memb.mem + session_memb.size*h);


  if(s == current_session) {
    /*Transmission is in progress so stop the module from making more routing attempts
     * This will trigger a transmit complete fail or ok for the current transmission. At some
     * point this will call ZW_SendDataAppl_CallbackEx which will free the session */
    ZW_SendDataAbort();

    /*Cancel security timers in case we are waiting for some frame from target node*/
    sec0_abort_all_tx_sessions();

    /* Cancel transport service timer, in case we are waiting for some frame from target node.
     * TODO*/
    //TransportService_SendDataAbort();
  } else {
    if (!list_contains(session_list, s))
    {
      /* This is an orphaned callback. Ignore it.*/
      return;
    }

    /*Remove the session from the list */
    list_remove(session_list,s);

    /*De-allocate the session */
    memb_free(&session_memb, s);
    if (s->callback)
    {
      s->callback(TRANSMIT_COMPLETE_FAIL, s->user, NULL);
    }
  }
}

void
ZW_SendDataAppl_init()
{
  lock = FALSE;
  lock_ll = FALSE;
  list_init(session_list);
  list_init(send_data_list);
  memb_init(&session_memb);

  process_exit(&ZW_SendDataAppl_process);
  process_start(&ZW_SendDataAppl_process, NULL);
}


/**
 * Calculate a priority for the scheme
 */
static int
scheme_prio(security_scheme_t a)
{
  switch (a)
  {
  case SECURITY_SCHEME_UDP:
    return 0xFFFF;
  case SECURITY_SCHEME_2_ACCESS:
    return 4;
  case SECURITY_SCHEME_2_AUTHENTICATED:
    return 3;
  case SECURITY_SCHEME_2_UNAUTHENTICATED:
    return 2;
  case SECURITY_SCHEME_0:
    return 1;
  default:
    return 0;
  }
}

/**
 * return true if a has a larger or equal scheme to b
 */
int
scheme_compare(security_scheme_t a, security_scheme_t b)
{
  return scheme_prio(a) >= scheme_prio(b);
}


PROCESS_THREAD(ZW_SendDataAppl_process, ev, data)
{
  register BYTE rc;
  PROCESS_BEGIN()
  ;

  while (1)
  {
    switch (ev)
    {
      case PROCESS_EVENT_TIMER:
        if (data == (void*) &emergency_timer)
        {
          ERR_PRINTF("Missed serialAPI callback!");
          send_data_callback_func(TRANSMIT_COMPLETE_FAIL,0);
        }
        break;
      case SEND_EVENT_SEND_NEXT:
        if (!lock)
        {
          send_first();
        }
        break;
      case SEND_EVENT_SEND_NEXT_LL:
        current_session_ll = list_head(send_data_list);
        if (current_session_ll && lock_ll == FALSE)
        {
          lock_ll = TRUE;
          t_tx_start = clock_time();

          if (current_session_ll->dataLength >= META_DATA_MAX_DATA_SIZE)
          {
            if (SupportsCmdClass(current_session_ll->param.dnode,
                COMMAND_CLASS_TRANSPORT_SERVICE))
            {
              //ASSERT(current_session_ll->param.snode == MyNodeID); //TODO make transport service bridge aware
              rc = ZW_TransportService_SendData(&current_session_ll->param,
                  (u8_t*) current_session_ll->pData,
                  current_session_ll->dataLength,
                  send_data_callback_func);
            }
            else
            {
              WRN_PRINTF("Frame is too long for target.\n");
              rc = FALSE;
            }
          }
          else if (current_session_ll->param.snode != MyNodeID)
          {
            rc = ZW_SendData_Bridge(current_session_ll->param.snode,
                current_session_ll->param.dnode,
                (BYTE*) current_session_ll->pData,
                current_session_ll->dataLength,
                current_session_ll->param.tx_flags, send_data_callback_func);
          }
          else
          {
            rc = ZW_SendData(current_session_ll->param.dnode,
                (BYTE*) current_session_ll->pData,
                current_session_ll->dataLength,
                current_session_ll->param.tx_flags, send_data_callback_func);
          }

          if (!rc)
          {
            ERR_PRINTF("Send data returned false\n");
            send_data_callback_func(TRANSMIT_COMPLETE_FAIL,0);
          }
          else
          {
            ima_send_data_start(current_session_ll->param.dnode);
            etimer_set(&emergency_timer, 65 * 1000UL);
          }
        }
    }
    PROCESS_WAIT_EVENT()
    ;
  }
PROCESS_END();
}
