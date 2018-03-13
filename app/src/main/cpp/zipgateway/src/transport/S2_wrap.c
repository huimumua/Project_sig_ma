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
/*
 * S2_wrap.c
 *
 *  Created on: Aug 25, 2015
 *      Author: aes
 */
#include "ZIP_Router.h"

#define DllExport
#undef BYTE
#undef UNUSED
#define x86
#include "NodeCache.h"
#include "S2.h"
#include "s2_inclusion.h"
#include "s2_keystore.h"
#include "s2_classcmd.h"

#include "S2_wrap.h"
#include "security_layer.h"
#include "ctimer.h"
#include "curve25519.h"
#include "CC_NetworkManagement.h"
#include "Mailbox.h"
#include "CommandAnalyzer.h"
#include "s2_protocol.h"
#include <stdarg.h>
#include <stdio.h>

#define S2_NUM_KEY_CLASSES 4 /* Includes the S0 key */
extern u8_t send_data(ts_param_t* p, const u8_t* data, u16_t len,ZW_SendDataAppl_Callback_t cb,void* user);
extern void print_hex(uint8_t* buf, int len);


struct S2* s2_ctx;

/**
 * Multicast transmission
 */
struct {
  uint8_t l_node;
  const uint8_t* node_list;
  uint8_t  node_list_len;
  const uint8_t* data;
  uint8_t  data_len;
  enum {MC_SEND_STATE_IDLE,MC_SEND_STATE_SEND_FIRST,MC_SEND_STATE_SEND} state;
} mc_state;


static ZW_SendDataAppl_Callback_t s2_send_callback;
static sec2_inclusion_cb_t sec_incl_cb;
/* Holds the TX_STATUS_TYPE of ZW_SendDataXX() callback for the most recent S2 frame */
static TX_STATUS_TYPE s2_send_tx_status;
static TX_STATUS_TYPE zeroed_tx_status_type; /* static are always initialized to zero*/

static void* s2_send_user;
static struct ctimer s2_timer;
static struct ctimer s2_inclusion_timer;
clock_time_t transmit_start_time;

static uint8_t
keystore_flags_2_node_flags(uint8_t key_store_flags)
{
  uint8_t flags =0;
  if (key_store_flags & KEY_CLASS_S0)
  {
    flags |= NODE_FLAG_SECURITY0;
  }
  if (key_store_flags & KEY_CLASS_S2_ACCESS)
  {
    flags |= NODE_FLAG_SECURITY2_ACCESS;
  }
  if (key_store_flags & KEY_CLASS_S2_AUTHENTICATED)
  {
    flags |= NODE_FLAG_SECURITY2_AUTHENTICATED;
  }
  if (key_store_flags & KEY_CLASS_S2_UNAUTHENTICATED)
  {
    flags |= NODE_FLAG_SECURITY2_UNAUTHENTICATED;
  }
  return flags;
}


static void sec2_event_handler(zwave_event_t* ev) {
  uint16_t response;
  uint8_t flags;
  LOG_PRINTF("S2 inclusion event %i\n",ev->event_type);
  switch(ev->event_type) {
    case S2_NODE_INCLUSION_INITIATED_EVENT:
      sec0_abort_inclusion(); //Here we abort S0 inclusion
      break;
    case S2_NODE_INCLUSION_PUBLIC_KEY_CHALLENGE_EVENT:
      NetworkManagement_dsk_challenge(&(ev->evt.s2_event.s2_data.challenge_req));
      break;
    case S2_NODE_INCLUSION_KEX_REPORT_EVENT:
      LOG_PRINTF("csa %i keys %i\n",ev->evt.s2_event.s2_data.kex_report.csa, ev->evt.s2_event.s2_data.kex_report.security_keys) ;
      NetworkManagement_key_request(&ev->evt.s2_event.s2_data.kex_report);
      break;
    case S2_NODE_INCLUSION_COMPLETE_EVENT:
      if(sec_incl_cb) {

        sec_incl_cb(keystore_flags_2_node_flags(ev->evt.s2_event.s2_data.inclusion_complete.exchanged_keys));
      }
      //< Security 2 event, inclusion of the node  to the network has been completed.
      break;
    case S2_NODE_JOINING_COMPLETE_EVENT:
      nvm_config_get(assigned_keys,&flags);
      if(sec_incl_cb) {
        sec_incl_cb(keystore_flags_2_node_flags(flags));
      }
      break;
    case S2_NODE_INCLUSION_FAILED_EVENT:
      if(sec_incl_cb) {
        sec_incl_cb(NODE_FLAG_KNOWN_BAD | (ev->evt.s2_event.s2_data.inclusion_fail.kex_fail_type << 16));
      }
      //< Security 2 event, inclusion of the node  to the network has failed. See the details
      break;
  }
}

void sec2_key_grant(uint8_t accept, uint8_t keys,uint8_t csa) {
  s2_inclusion_key_grant(s2_ctx, accept,keys,csa);
}

void sec2_dsk_accept(uint8_t accept, uint8_t* dsk, uint8_t len ) {
  s2_inclusion_challenge_response(s2_ctx, accept,  dsk,len);
}

void S2_get_commands_supported(uint8_t lnode,uint8_t class_id, const uint8_t** cmdClasses, uint8_t* length) {
  /*Only answer to ACCESS class*/

  if(
      ((net_scheme == SECURITY_SCHEME_2_ACCESS) && class_id==2 ) ||
      ((net_scheme == SECURITY_SCHEME_2_AUTHENTICATED) && class_id==1 ) ||
      ((net_scheme == SECURITY_SCHEME_2_UNAUTHENTICATED) && class_id==0 )
  ) {
    if(lnode == MyNodeID)  {
      *cmdClasses = &SecureClasses[0];
      /* libs2 buffer (ctxt->u.commands_sup_report_buf) can hold only (40 - 2) bytes so truncate
        the number of s2 commands supported */
      if (nSecureClasses > 38) {
         ERR_PRINTF("Truncating the number of command classes supported to 38.\n");
         *length = 38;
      } else {
         *length  = nSecureClasses;
      }
      return;
    }
  }

  *length = 0;
}


void sec2_init() {
  static uint8_t s2_cmd_class_sup_report[64];

  ctimer_stop(&s2_timer);
  ctimer_stop(&s2_inclusion_timer);
  if(s2_ctx) S2_destroy(s2_ctx);

  s2_inclusion_init(SECURITY_2_SCHEME_1_SUPPORT,KEX_REPORT_CURVE_25519,
      SECURITY_2_SECURITY_2_CLASS_0|SECURITY_2_SECURITY_2_CLASS_1 | SECURITY_2_SECURITY_2_CLASS_2 | SECURITY_2_SECURITY_0_NETWORK_KEY);

  s2_ctx = S2_init_ctx(UIP_HTONL(homeID));

  s2_inclusion_set_event_handler(&sec2_event_handler);
  sec2_create_new_dynamic_ecdh_key();
  //memset(&s2_send_tx_status, 0, sizeof s2_send_tx_status); //Redundant: Static vars are always initialized to zero
  mc_state.state = MC_SEND_STATE_IDLE;
}


uint8_t ecdh_dynamic_key[32];

void sec2_create_new_static_ecdh_key() {
  sec2_create_new_dynamic_ecdh_key();
  nvm_config_set(ecdh_priv_key,ecdh_dynamic_key);
  sec2_create_new_dynamic_ecdh_key();
}


void sec2_create_new_dynamic_ecdh_key()
{
  AES_CTR_DRBG_Generate(&s2_ctr_drbg, ecdh_dynamic_key);
  AES_CTR_DRBG_Generate(&s2_ctr_drbg, &ecdh_dynamic_key[16]);
}


uint8_t sec2_get_my_node_flags() {
  uint8_t flags;
  nvm_config_get(assigned_keys,&flags);

  return keystore_flags_2_node_flags(flags);
}

void sec2_create_new_network_keys() {
  uint8_t net_key[16];
  int i;
  int c;
  LOG_PRINTF("Creating new network keys\n");
  for(c=0; c < S2_NUM_KEY_CLASSES-1; c++) {
    S2_get_hw_random(net_key,16);
    print_hex(net_key,16);
    keystore_network_key_write(1<<c,net_key);
  }

  memset(net_key,0, sizeof(net_key));
}

void
sec2_start_learn_mode(uint8_t node_id, sec2_inclusion_cb_t cb)
{

  s2_connection_t s2_con;
  ZW_SendDataAppl_Callback_t cb_save;

  /*We update the context here becase the homeID has changed */
  sec2_init();

  s2_con.l_node = MyNodeID;
  s2_con.r_node = node_id;
  s2_con.zw_tx_options = TRANSMIT_OPTION_ACK | TRANSMIT_OPTION_AUTO_ROUTE | TRANSMIT_OPTION_EXPLORE;
  sec_incl_cb = cb;

  s2_inclusion_joining_start(s2_ctx,&s2_con,0);
}


void sec2_start_add_node( uint8_t node_id, sec2_inclusion_cb_t cb ) {

  s2_connection_t s2_con;

  ZW_SendDataAppl_Callback_t cb_save;

  s2_con.l_node = MyNodeID;
  s2_con.r_node = node_id;
  s2_con.zw_tx_options = TRANSMIT_OPTION_ACK | TRANSMIT_OPTION_AUTO_ROUTE | TRANSMIT_OPTION_EXPLORE;

  sec_incl_cb = cb;
  s2_inclusion_including_start(s2_ctx,&s2_con);
}

uint8_t sec2_send_data(ts_param_t* p, uint8_t* data, uint16_t len,ZW_SendDataAppl_Callback_t callback,void* user) {
  s2_connection_t s2_con;

  ZW_SendDataAppl_Callback_t cb_save;
  void* user_save;

  s2_con.l_node = p->snode;
  s2_con.r_node = p->dnode;
  s2_con.zw_tx_options = p->tx_flags;
  s2_con.tx_options = 0;
  /* This enables mailbox to set S2_TXOPTION_VERIFY_DELIVERY which waits for the sleeping ndoe to send SOS/resync if
    needed. It puts delay of 500ms, but only for first frame popped out of Mailbox after the node is woken up*/
  if(mb_is_busy() || CommandAnalyzerIsGet(data[0],data[1]))
  {
     s2_con.tx_options |= S2_TXOPTION_VERIFY_DELIVERY;
  }

  s2_con.class_id = p->scheme-SECURITY_SCHEME_2_UNAUTHENTICATED;

  cb_save = s2_send_callback;
  user_save = s2_send_user;

  s2_send_callback = callback;
  s2_send_user = user;
  if(S2_send_data(s2_ctx,&s2_con,data,len)) {
      return 1;
  } else {
      s2_send_callback = cb_save;
      s2_send_user = user_save;
      return 0;
  }
}


uint8_t sec2_send_multicast(uint8_t src_node, const uint8_t* node_list, uint8_t node_list_len,const uint8_t * data, uint8_t data_len,ZW_SendDataAppl_Callback_t callback) {
  s2_connection_t s2_con;
  uint8_t group_id =0x42;

  if(mc_state.state == MC_SEND_STATE_IDLE) {
    mc_state.l_node = src_node;
    mc_state.node_list = node_list;
    mc_state.node_list_len = node_list_len;
    mc_state.data = data;
    mc_state.data_len = data_len;
    mc_state.state = MC_SEND_STATE_SEND_FIRST;
    s2_send_callback = callback;

    s2_con.l_node = src_node;
    s2_con.r_node = group_id;
    s2_con.zw_tx_options = 0;
    s2_con.tx_options = 0;
    s2_con.class_id = 0;

    DBG_PRINTF("Sending Multicast\n");
    if( S2_send_data_multicast(s2_ctx,&s2_con,mc_state.data,mc_state.data_len) ) {
      return 1;
    }
    /* Transmission failed */
    mc_state.state = MC_SEND_STATE_IDLE;
  }
  return 0;
}

#ifdef TEST_MULTICAST_TX
uint8_t sec2_send_multicast_nofollowup(uint8_t src_node, const uint8_t* node_list, uint8_t node_list_len,const uint8_t * data, uint8_t data_len,ZW_SendDataAppl_Callback_t callback) {
  s2_connection_t s2_con;
  uint8_t group_id =0x42;

  if(mc_state.state == MC_SEND_STATE_IDLE) {
    mc_state.l_node = src_node;
    mc_state.node_list = node_list;
    mc_state.node_list_len = node_list_len;
    mc_state.data = data;
    mc_state.data_len = data_len;
    /* Disable single-cast followup*/
    mc_state.state = MC_SEND_STATE_IDLE;
    s2_send_callback = callback;

    s2_con.l_node = src_node;
    s2_con.r_node = group_id;
    s2_con.zw_tx_options = 0;
    s2_con.tx_options = 0;
    s2_con.class_id = 0;

    DBG_PRINTF("Sending Multicast\n");
    if( S2_send_data_multicast(s2_ctx,&s2_con,mc_state.data,mc_state.data_len) ) {
      return 1;
    }
    /* Transmission failed */
    mc_state.state = MC_SEND_STATE_IDLE;
  }
  return 0;
}
#endif


/**
 * Emitted when the security engine is done.
 * Note that this is also emitted when the security layer sends a SECURE_COMMANDS_SUPPORTED_REPORT
 */
void S2_send_done_event(struct S2* ctxt, s2_tx_status_t status) {
  const uint8_t s2zw_codes[] = {TRANSMIT_COMPLETE_OK,TRANSMIT_COMPLETE_OK,TRANSMIT_COMPLETE_NO_ACK,TRANSMIT_COMPLETE_FAIL};
  ZW_SendDataAppl_Callback_t cb_save;

  ctimer_stop(&s2_timer);

  if(mc_state.state != MC_SEND_STATE_IDLE) {
    if( mc_state.node_list_len ==0) {
      mc_state.state = MC_SEND_STATE_IDLE;
      DBG_PRINTF("Multicast transmission is done\n");
    } else {
      s2_connection_t s2_con;

      s2_con.l_node = mc_state.l_node;
      s2_con.r_node = *mc_state.node_list;
      s2_con.zw_tx_options = TRANSMIT_OPTION_ACK | TRANSMIT_OPTION_AUTO_ROUTE | TRANSMIT_OPTION_EXPLORE ;
      s2_con.tx_options = (mc_state.state ==MC_SEND_STATE_SEND)  ?
          S2_TXOPTION_SINGLECAST_FOLLOWUP  | S2_TXOPTION_VERIFY_DELIVERY :
          S2_TXOPTION_FIRST_SINGLECAST_FOLLOWUP | S2_TXOPTION_VERIFY_DELIVERY;
      s2_con.class_id = 0;

      mc_state.node_list++;
      mc_state.node_list_len--;

      DBG_PRINTF("Sending Multicast followup to node %i\n",s2_con.r_node);
      mc_state.state = MC_SEND_STATE_SEND;


      if(S2_send_data(s2_ctx,&s2_con,mc_state.data,mc_state.data_len)) {
        return;
      } else {
        status = S2_TRANSMIT_COMPLETE_FAIL;
        mc_state.state = MC_SEND_STATE_IDLE;
      }
    }
  }

  cb_save = s2_send_callback;
  s2_send_callback = 0;
  if(cb_save) {
    cb_save(s2zw_codes[status],s2_send_user, &s2_send_tx_status);
  }
  memset(&s2_send_tx_status, 0, sizeof s2_send_tx_status);

}

/**
 * Emitted when a messages has been received and decrypted
 */
void S2_msg_received_event(struct S2* ctxt,s2_connection_t* src , uint8_t* buf, uint16_t len) {
  ts_param_t p;
  const int scheme_map[] = {
      SECURITY_SCHEME_2_UNAUTHENTICATED,
      SECURITY_SCHEME_2_AUTHENTICATED,
      SECURITY_SCHEME_2_ACCESS};
  ts_set_std(&p,0);


  p.scheme = scheme_map[src->class_id];
  p.snode = src->r_node;
  p.dnode = src->l_node;
  p.rx_flags = src->zw_rx_status;

  ApplicationCommandHandlerZIP(&p,(ZW_APPLICATION_TX_BUFFER*)buf,len);
}

//This is the time it takes a z-wave node to do 3*S2 frame decryption + generating a nonce report
//#define NONCE_REP_TIME 50
#define NONCE_REP_TIME 250
static void S2_send_frame_callback(BYTE txStatus,void* user, TX_STATUS_TYPE *t) {
  /* If this is non-zeroed we are overwriting a non-processed TX_STATUS */
  /* ... except during network management we dont except anyone to process it */
  assert((memcmp(&s2_send_tx_status, &zeroed_tx_status_type, sizeof s2_send_tx_status) == 0)
      || !nm_is_idle());
  if (t) {
    s2_send_tx_status = *t;
  }
  S2_send_frame_done_notify((struct S2*) user,
      txStatus == TRANSMIT_COMPLETE_OK ? S2_TRANSMIT_COMPLETE_OK : S2_TRANSMIT_COMPLETE_NO_ACK,
          clock_time()-transmit_start_time + NONCE_REP_TIME);
}

/** Must be implemented elsewhere maps to ZW_SendData or ZW_SendDataBridge note that ctxt is
 * passed as a handle. The ctxt MUST be provided when the \ref S2_send_frame_done_notify is called */
uint8_t S2_send_frame(struct S2* ctxt,const s2_connection_t* conn, uint8_t* buf, uint16_t len) {
  ts_param_t p;
  ts_set_std(&p,0);
  p.snode = conn->l_node;
  p.dnode = conn->r_node;
  p.tx_flags = conn->zw_tx_options;
  LOG_PRINTF(" Sending S2_send_frame %i\n", len);
  transmit_start_time = clock_time();
  return send_data(&p, buf,  len,S2_send_frame_callback,ctxt);
}

typedef struct
{
  uint8_t buf[50];  /* TODO: This should be enough for a nonce report. Can be shrunk to save memory. */
} nonce_rep_tx_t;

/**
 *  This memblock caches outgoing nonce reports so we dont have to care if s2 overwrites the single buffer in there
 */
MEMB(nonce_rep_memb, nonce_rep_tx_t, 8);

/**
 * Callback function for S2_send_frame_no_cb()
 *
 * Cleans up the transmission
 */
static void nonce_rep_callback(BYTE txStatus,void* user, TX_STATUS_TYPE *t)
{
  nonce_rep_tx_t *m = (nonce_rep_tx_t*)user;
  if(txStatus != TRANSMIT_COMPLETE_OK) {
    DBG_PRINTF("Nonce rep tx failed, status %u, seqno %u", txStatus, m->buf[2]);
  }
  memb_free(&nonce_rep_memb, m);
}

/** Does not provide callback to libs2 (because send_done can be confused with Msg Encapsulations)
 *  Caches the buf pointer outside libs2 so we dont have to care if s2 reuses the single buffer in there.
 *
 * Must be implemented elsewhere maps to ZW_SendData or ZW_SendDataBridge note that ctxt is
 * passed as a handle. The ctxt MUST be provided when the \ref S2_send_frame_done_notify is called */
uint8_t S2_send_frame_no_cb(struct S2* ctxt,const s2_connection_t* conn, uint8_t* buf, uint16_t len) {
  ts_param_t p;
  nonce_rep_tx_t *m;
  m = memb_alloc(&nonce_rep_memb);
  if (m == 0)
  {
    DBG_PRINTF("OMG! No more queue space for nonce reps\n");
    return FALSE;
  }
  ts_set_std(&p,0);
  p.snode = conn->l_node;
  p.dnode = conn->r_node;
  p.tx_flags = conn->zw_tx_options;
  if(len > sizeof(m->buf)) {
    WRN_PRINTF("nonce_rep_tx_t buf is too small, needed %u\n", len);
  }
  memcpy(m->buf, buf, len);
  DBG_PRINTF("S2_send_frame_no_cb len %i, seqno %u\n", len, m->buf[2]);
  return send_data(&p, buf,  len, nonce_rep_callback, m);
}


/**
 * TODO
 */
uint8_t S2_send_frame_multi(struct S2* ctxt,s2_connection_t* conn, uint8_t* buf, uint16_t len){
  ts_param_t p;
  ts_set_std(&p,0);
  p.snode = conn->l_node;
  p.dnode = 0xFF;
  p.tx_flags = conn->zw_tx_options;
  LOG_PRINTF(" Sending S2_send_frame %i\n", len);
  transmit_start_time = clock_time();
  return send_data(&p, buf,  len,S2_send_frame_callback,ctxt);
}


static void timeout(void* ctxt) {
  S2_timeout_notify((struct S2*)ctxt);
}
/**
 * Must be implemented elsewhere maps to ZW_TimerStart. Note that this must start the same timer every time.
 * Ie. two calls to this function must reset the timer to a new value. On timout \ref S2_timeout_event must be called.
 *
 */
void S2_set_timeout(struct S2* ctxt, uint32_t interval) {
  DBG_PRINTF("S2_set_timeout interval =%i ms\n",interval );

  ctimer_set(&s2_timer,interval,timeout,ctxt);
}


static void incl_timeout(void* ctxt) {
  DBG_PRINTF("incl_timeout\n");
  s2_inclusion_notify_timeout((struct S2*)ctxt);
}


uint8_t s2_inclusion_set_timeout(struct S2* ctxt, uint32_t interval) {
  DBG_PRINTF("s2_inclusion_set_timeout interval =%i ms\n",interval * 10);
  ctimer_set(&s2_inclusion_timer,interval * 10,incl_timeout,ctxt);
  return 0;
}


/**
 * Get a number of bytes from a hardware random number generator
 */
void S2_get_hw_random(uint8_t *buf, uint8_t len) {
  uint8_t rnd[8];
  int n,left,pos;

  left = len;
  pos = 0;
  while( left > 0) {
      ZW_GetRandomWord(rnd,FALSE);
      n = left > 8 ? 8 : left;
      memcpy(&buf[pos],rnd, n);
      left -= n;
      pos += n;
  }
}

void /*RET Nothing                  */
security2_CommandHandler(ts_param_t* p,
const ZW_APPLICATION_TX_BUFFER *pCmd, uint16_t cmdLength) /* IN Number of command bytes including the command */
{
  const uint8_t s2_cap_report[] = {COMMAND_CLASS_SECURITY_2, SECURITY_2_CAPABILITIES_REPORT,0};

  s2_connection_t conn;
  conn.r_node = p->snode;
  conn.l_node = p->dnode;
  conn.zw_tx_options = p->tx_flags;
  conn.zw_rx_status = p->rx_flags;
  conn.tx_options = 0;
  conn.rx_options = p->rx_flags & ( RECEIVE_STATUS_TYPE_BROAD |  RECEIVE_STATUS_TYPE_MULTI) ? S2_RXOPTION_MULTICAST : 0;

  if( (pCmd->ZW_Common.cmd == SECURITY_2_CAPABILITIES_GET)  && (p->scheme == SECURITY_SCHEME_2_ACCESS)) {
    S2_send_data(s2_ctx,&conn,s2_cap_report,sizeof(s2_cap_report));
  } else {
    S2_application_command_handler(s2_ctx,&conn,(uint8_t*) pCmd,cmdLength);
  }
}

uint8_t sec2_gw_node_flags2keystore_flags(uint8_t gw_flags) {
  uint8_t f =0;

  if(gw_flags & NODE_FLAG_SECURITY0) {
    f |= KEY_CLASS_S0;
  }
  if(gw_flags & NODE_FLAG_SECURITY2_ACCESS) {
    f |= KEY_CLASS_S2_ACCESS;
  }
  if(gw_flags & NODE_FLAG_SECURITY2_AUTHENTICATED) {
    f |= KEY_CLASS_S2_AUTHENTICATED;
  }
  if(gw_flags & NODE_FLAG_SECURITY2_UNAUTHENTICATED) {
    f |= KEY_CLASS_S2_UNAUTHENTICATED;
  }
  return f;
}

void sec2_abort_join() {
  DBG_PRINTF("S2 inclusion was aborted\n");
  if(!s2_ctx) return;

  s2_inclusion_abort(s2_ctx);
}

void sec2_refresh_homeid(void)
{
  s2_ctx->my_home_id = UIP_HTONL(homeID);
}

void sec2_set_inclusion_peer(uint8_t remote_nodeID, uint8_t local_nodeID)
{
  S2_set_inclusion_peer(s2_ctx, remote_nodeID, local_nodeID);
}

void S2_dbg_printf(const char *format, ...)
{
    va_list argptr;
    printf("\033[34;1m%lu ", clock_time());

    va_start(argptr, format);
    vfprintf(stdout, format, argptr);
    va_end(argptr);

    printf("\033[0m");
}

extern void s2_inclusion_stop_timeout(void)
{
  ctimer_stop(&s2_inclusion_timer);
}
