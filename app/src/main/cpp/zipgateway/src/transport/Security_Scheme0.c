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
#include "contiki.h"
#include "Serialapi.h"
#include "ZIP_Router.h"
#include "security_layer.h"
#include "Security_Scheme0.h"
#include "ZW_SendDataAppl.h"
#include "ZW_PRNG.h"
#define NONCE_OPT 0

/**/
#define NONCE_TABLE_SIZE 5*3
#define NUM_TX_SESSIONS 2
#define MAX_ENCRYPTED_MSG_SIZE 128
#define MAX_NONCES 10
#define MAX_RXSESSIONS 2

#define NONCE_TIMEOUT 10
#define NONCE_REQUEST_TIMEOUT_MSEC 500

typedef enum {
  NONCE_GET,
  NONCE_GET_SENT,
  ENC_MSG,
  ENC_MSG_SENT,
  ENC_MSG2,
  ENC_MSG2_SENT,
  TX_DONE,
  TX_FAIL} tx_state_t;


typedef struct sec_tx_session {
  ts_param_t param;
  const uint8_t* data;            //Pointer the data to be encrypted
  uint8_t data_len;         //Length of the data to be encrypted
  tx_state_t state;
  uint8_t tx_code;
  clock_time_t transition_time;
  struct ctimer timer;      //Session timer
  uint8_t crypted_msg[46];
  uint8_t seq;              //Sequence number used in multisegment messages
  void*   user;             //User supplied pointer which is returned in the callback
  ZW_SendDataAppl_Callback_t callback;//Callback function, called when the session has been terminated
} sec_tx_session_t;

typedef struct _AUTHDATA_ {
  uint8_t sh; /* Security Header for authentication */
  uint8_t senderNodeID; /* Sender ID for authentication */
  uint8_t receiverNodeID; /* Receiver ID for authentication */
  uint8_t payloadLength; /* Length of authenticated payload */
} auth_data_t;

extern u8_t send_data(ts_param_t* p, const u8_t* data, u16_t len,ZW_SendDataAppl_Callback_t cb,void* user);

static sec_tx_session_t tx_sessions[NUM_TX_SESSIONS];
u8_t networkKey[16];                    /* The master key */

/************************ AES Helper functions ********************************************/
static uint8_t aes_key[16];
static uint8_t aes_iv[16];

void aes_encrypt(uint8_t *in, uint8_t* out) {
  SerialAPI_AES128_Encrypt(in,out,aes_key);
}

void aes_set_key_tpt(uint8_t* key,uint8_t* iv) {
  memcpy(aes_key,key,16);
  memcpy(aes_iv,iv,16);
}

void aes_ofb(uint8_t* data,uint8_t len) {
  uint8_t i;
  for (i = 0; i < len; i++) {
    if ((i & 0xF) == 0x0) {
      aes_encrypt(aes_iv, aes_iv);
    }
    data[i] ^= aes_iv[(i & 0xF)];
  }
}

/*
 * Caclucalte the authtag for the message,
 */
void aes_cbc_mac(uint8_t* data,uint8_t len,uint8_t* mac) {
  uint8_t i;

  aes_encrypt(aes_iv,mac);
  for (i = 0; i < len; i++) {
    mac[i & 0xF] ^= data[i];
    if ((i & 0xF) == 0xF) {
      aes_encrypt(mac, mac);
    }
  }

  /*if len is not divisible by 16 do the final pass, this is the padding described in the spec */
  if(len & 0xF) {
    aes_encrypt(mac, mac);
  }
}

/**
 * Generate 8 random bytes
 */
void aes_random8(uint8_t*d ) {
  PRNGOutput(d);
}

/******************************** Nonce Management **********************************************/

typedef struct nonce {
  u8_t src;
  u8_t dst;
  u8_t timeout;
  u8_t reply_nonce; //indicate if this nonce from a enc message sent by me
  u8_t nonce[8];
} nonce_t;

static nonce_t nonce_table[NONCE_TABLE_SIZE]; //Nonces received or sent
static struct ctimer nonce_timer;

/**
 * Register a new nonce from sent from src to dst
 */
static u8_t register_nonce(u8_t src, u8_t dst,u8_t reply_nonce, const u8_t nonce[8]) {
  u8_t i;

  if(reply_nonce) {
    /*Only one reply nonce is allowed*/
    for (i=0; i< NONCE_TABLE_SIZE; i++) {
      if( nonce_table[i].reply_nonce &&
          nonce_table[i].timeout > 0 &&
          nonce_table[i].src == src &&
          nonce_table[i].dst == dst) {
        DBG_PRINTF("Reply nonce overwritten\n");
        memcpy(nonce_table[i].nonce,nonce,8);
        nonce_table[i].timeout = NONCE_TIMEOUT;
        return 1;
      }
    }
  }

  for (i=0; i< NONCE_TABLE_SIZE; i++) {
    if(nonce_table[i].timeout == 0) {
      nonce_table[i].src = src;
      nonce_table[i].dst = dst;
      nonce_table[i].reply_nonce = reply_nonce;
      memcpy(nonce_table[i].nonce,nonce,8);
      nonce_table[i].timeout = NONCE_TIMEOUT;
      return 1;
    }
  }

  ERR_PRINTF("Nonce table is full\n");
  return 0;
}

/**
 * Receive nonce sent from src to dst, if th ri. If a nonce
 * is found the remove all entries from that src->dst combination
 * from the table.
 *
 * If any_nonce is set then ri is ignored
 */
static u8_t get_nonce(u8_t src, u8_t dst,u8_t ri, u8_t nonce[8],u8_t any_nonce) {
  u8_t i;
  u8_t rc;
  rc = 0;
  for (i=0; i< NONCE_TABLE_SIZE; i++) {
    if(nonce_table[i].timeout>0 && nonce_table[i].src == src && nonce_table[i].dst == dst) {
      if(any_nonce ||  nonce_table[i].nonce[0] == ri) {
        memcpy(nonce,nonce_table[i].nonce,8);
        return 1;
      }
    }
  }
  return 0;
}

/**
 * Remove all nonces from nonce table sent from src to dst
 * @param src
 * @param dst
 */
static void nonce_clear(u8_t src, u8_t dst)
{
  u8_t i;
  /*Remove entries from table from that source dest combination */
  for (i=0; i< NONCE_TABLE_SIZE; i++) {
    if(nonce_table[i].timeout && nonce_table[i].src == src && nonce_table[i].dst == dst) {
      nonce_table[i].timeout = 0;
    }
  }
}

static void nonce_timer_timeout(void* data) {
  u8_t i;
  for (i=0; i< NONCE_TABLE_SIZE; i++) {
    if(nonce_table[i].timeout > 0) {
      nonce_table[i].timeout--;
    }
  }
  ctimer_set(&nonce_timer,1000,nonce_timer_timeout,0);
}

/********************************Security TX Code ***************************************************************/
static void tx_session_state_set(sec_tx_session_t* s,tx_state_t state);
static uint8_t enckey[16];
static uint8_t authkey[16];
static uint8_t enckeyz[16];
static uint8_t authkeyz[16];

/**
 * Set the network key
 */
void sec0_set_key(const uint8_t* netkey)REENTRANT {
  uint8_t p[16];
  uint8_t temp[16] ={ 0};

  if (memcmp(netkey, temp, 16) == 0) {
    ERR_PRINTF("sec0_set_key with all zero key\n");
  }else{
    ERR_PRINTF("sec0_set_key \n");
  }

  memcpy(aes_key,netkey,16);
  memset(p,0x55,16);
  aes_encrypt(p,authkey);
  memset(p,0xAA,16);
  aes_encrypt(p,enckey);

  memcpy(aes_key,temp,16);
  memset(p,0x55,16);
  aes_encrypt(p,authkeyz);
  memset(p,0xAA,16);
  aes_encrypt(p,enckeyz);


}


static void tx_timeout(void* user) {
  sec_tx_session_t* s = (sec_tx_session_t*) user;
  ERR_PRINTF("Security0 transmit timeout\n");
  s->tx_code = TRANSMIT_COMPLETE_FAIL;
  tx_session_state_set(s,TX_FAIL);
}

/**
 * Lookup a tx session by nodeid
 */
static sec_tx_session_t* get_tx_session_by_node(uint8_t snode, uint8_t dnode) {
  uint8_t i;
  for(i=0; i < NUM_TX_SESSIONS;i++) {
    if(tx_sessions[i].param.dnode == dnode && tx_sessions[i].param.snode == snode) {
      return &tx_sessions[i];
    }
  }
  return 0;
}

/**
 * Get the maximum frame size supported by a node.
 */
static uint8_t get_node_max_frame_size(u8_t dnode) {
  /*FIXME This should be made more clever*/
  return 46;
}

/**
 * Encrypt a message and write the encrypted data into s->crypted_msg
 */
static uint8_t encrypt_msg(sec_tx_session_t* s,uint8_t pass2) REENTRANT{
  uint8_t iv[16],tmp[8]; /* Initialization vector for enc, dec,& auth */
  uint8_t mac[16];
  uint8_t len; // Length of the encrypted part
  uint8_t more_to_send;
  uint8_t* enc_data;
  auth_data_t* auth;
  uint8_t maxlen = get_node_max_frame_size(s->param.dnode);

  len = s->data_len;
  more_to_send = 0;

  /*Check if we should break this message in two */
  if(len + 20 > maxlen) {
    len = maxlen-20;
    more_to_send = 1;
  }

  ASSERT(len + 20 <= sizeof(s->crypted_msg));
  /* Make the IV */

  do {
    aes_random8(iv);
  } while( get_nonce(s->param.dnode,s->param.snode,iv[0],tmp,FALSE) );

  /*Choose a nonce from sender */
  if(!get_nonce(s->param.dnode,s->param.snode,0,iv+8,TRUE)) {
    ERR_PRINTF("Nonce for node %u --> %u is not found\n",s->param.dnode,s->param.snode);
    return 0;
  } else {
    nonce_clear(s->param.dnode,s->param.snode);
  }

  /*Register my nonce */
  register_nonce(s->param.snode,s->param.dnode,TRUE,iv);


  /* Setup pointers */
  enc_data = s->crypted_msg + 10;
  auth = (auth_data_t* )(s->crypted_msg + 6);

  /* Copy data into a second buffer Insert security flags */

  if(pass2) {
    *enc_data = SECURITY_MESSAGE_ENCAPSULATION_PROPERTIES1_SEQUENCED_BIT_MASK | SECURITY_MESSAGE_ENCAPSULATION_PROPERTIES1_SECOND_FRAME_BIT_MASK | (s->seq & 0xF);
  } else if(more_to_send) {
    *enc_data = SECURITY_MESSAGE_ENCAPSULATION_PROPERTIES1_SEQUENCED_BIT_MASK  | (s->seq & 0xF);
  } else {
    *enc_data = 0;
  }

  memcpy(enc_data+1, s->data, len);


  if ((s->data[0] == COMMAND_CLASS_SECURITY) && (s->data[1] == NETWORK_KEY_SET)) {
    DBG_PRINTF("COMMAND_CLASS_SECURITY, NETWORK_KEY_SET\n");
   /*Encrypt */
    aes_set_key_tpt(enckeyz,iv);
  } else {
   /*Encrypt */
    aes_set_key_tpt(enckey,iv);
  }

  aes_ofb(enc_data,len+1);

  /*Fill in the auth structure*/
  auth->sh = more_to_send ? SECURITY_MESSAGE_ENCAPSULATION_NONCE_GET : SECURITY_MESSAGE_ENCAPSULATION;
  auth->senderNodeID = s->param.snode;
  auth->receiverNodeID = s->param.dnode;
  auth->payloadLength = len + 1;

  if ((s->data[0] == COMMAND_CLASS_SECURITY) && (s->data[1] == NETWORK_KEY_SET)) {
    DBG_PRINTF("COMMAND_CLASS_SECURITY, NETWORK_KEY_SET\n");
    /* Authtag */
    aes_set_key_tpt(authkeyz,iv);
  } else {
    /* Authtag */
    aes_set_key_tpt(authkey,iv);
  }
  aes_cbc_mac((uint8_t*)auth,4 + len+1,mac);
  s->crypted_msg[0] = COMMAND_CLASS_SECURITY;
  s->crypted_msg[1] = auth->sh;
  memcpy(s->crypted_msg+2, iv,8);

  s->crypted_msg[2 +8 + len + 1] = iv[8];
  memcpy(s->crypted_msg+2 + 8 + len + 2,mac,8);

  s->data += len;
  s->data_len -=len;
  return len + 20;
}

static void noce_get_callback(uint8_t status,void* user, TX_STATUS_TYPE *t) {
  sec_tx_session_t* s = (sec_tx_session_t*) user;

  //ASSERT(s->state == NONCE_GET);

  s->tx_code = status;
  if(s->state == NONCE_GET)
    tx_session_state_set(s, status == TRANSMIT_COMPLETE_OK ? NONCE_GET_SENT : TX_FAIL);
}

static void msg1_callback(uint8_t status,void* user, TX_STATUS_TYPE *t) {
  sec_tx_session_t* s = (sec_tx_session_t*) user;
  s->tx_code = status;

  //ASSERT(s->state == ENC_MSG);
  if(s->state == ENC_MSG)
    tx_session_state_set(s, status == TRANSMIT_COMPLETE_OK ? ENC_MSG_SENT : TX_FAIL);
}

static void msg2_callback(uint8_t status,void* user, TX_STATUS_TYPE *t) {
  sec_tx_session_t* s = (sec_tx_session_t*) user;
  s->tx_code = status;

  //ASSERT(s->state == ENC_MSG2);
  if(s->state == ENC_MSG2)
    tx_session_state_set(s, status == TRANSMIT_COMPLETE_OK ? ENC_MSG2_SENT : TX_FAIL);
}

static void tx_session_state_set(sec_tx_session_t* s,tx_state_t state) {
  uint8_t len;
#if NONCE_OPT
  u8_t dummy[8];
#endif

  static const uint8_t nonce_get[] =  { COMMAND_CLASS_SECURITY, SECURITY_NONCE_GET };

  s->state = state;	  
  s->transition_time = clock_time();
  //DBG_PRINTF("State %d\n",state);

  switch(s->state) {
  case NONCE_GET:
#if NONCE_OPT
    if(get_nonce(s->dnode.nodeid,s->snode.nodeid,0,dummy)) {
      tx_session_state_set(s,ENC_MSG);
      return;
    }
#endif
    if(!send_data(&s->param,nonce_get,sizeof(nonce_get), noce_get_callback,s) ) {
      s->tx_code = TRANSMIT_COMPLETE_FAIL;
      tx_session_state_set(s,TX_FAIL);
    } else if(secure_learn_active()) {
        ctimer_set(&s->timer,10000,tx_timeout,s);
    }
    break;
  case NONCE_GET_SENT:
    if(!secure_learn_active()) {
        ctimer_set(&s->timer, NONCE_REQUEST_TIMEOUT_MSEC + clock_time()-s->transition_time,tx_timeout,s);
    }
    break;
  case ENC_MSG:
    ctimer_stop(&s->timer);
    len = encrypt_msg(s,0);
    if(len==0 || !send_data(&s->param,s->crypted_msg,len, msg1_callback,s) ) {
      s->tx_code = TRANSMIT_COMPLETE_FAIL;
      tx_session_state_set(s,TX_FAIL);
    }
    break;
  case ENC_MSG_SENT:
    /*If there is no more data to send*/
    if(s->data_len==0) {
      tx_session_state_set(s,TX_DONE);
    } else {
      ctimer_set(&s->timer,NONCE_REQUEST_TIMEOUT_MSEC + clock_time()-s->transition_time,tx_timeout,s);
    }
    break;
  case ENC_MSG2:
    ctimer_stop(&s->timer);
    len = encrypt_msg(s,1);
    if(len==0 || !send_data(&s->param,s->crypted_msg,len, msg2_callback,s) ) {
      s->tx_code = TRANSMIT_COMPLETE_FAIL;
      tx_session_state_set(s,TX_FAIL);
    }
    break;
  case ENC_MSG2_SENT:
    tx_session_state_set(s,TX_DONE);
    break;
  case TX_DONE:
    s->param.dnode = 0;
    ctimer_stop(&s->timer);
    if(s->callback) s->callback(TRANSMIT_COMPLETE_OK,s->user, NULL);
    break;
  case TX_FAIL:
    s->param.dnode= 0;
    ctimer_stop(&s->timer);
    if(s->callback) s->callback(s->tx_code,s->user, NULL);
    break;
  }
}

/**
 * Get the next sequence number no node may receive the same sequence number in two concurrent transmissions
 */
static uint8_t get_seq(uint8_t src, uint8_t dst) {
  static uint8_t s=0;
  s++; //FIXME make this routine do the right thing.
  return s;
}

uint8_t sec0_send_data(ts_param_t* p,const void* data, uint8_t len,ZW_SendDataAppl_Callback_t callback,void* user) {
  sec_tx_session_t* s;
  uint8_t i;

  s = get_tx_session_by_node(p->snode,p->dnode);
  if(s) {
    ERR_PRINTF("Already have one tx session from node %d to %d\n", p->snode,p->dnode);
    return FALSE;
  }

  for(i=0; i < NUM_TX_SESSIONS;i++) {
    if(tx_sessions[i].param.dnode == 0) {
      s = &tx_sessions[i];
      break;
    }
  }

  if(!s) {
    ERR_PRINTF("No more security TX sessions available\n");
    return FALSE;
  }

  s->param = *p;
  s->data = data;
  s->data_len = len;
  s->callback = callback;
  s->user = user;
  s->seq = get_seq(s->param.snode,s->param.dnode);

  DBG_PRINTF("New sessions for src %d dst %d\n",s->param.snode,s->param.dnode);
  tx_session_state_set(s,NONCE_GET);
  return TRUE;
}

/**
 * Register a nonce by source and destination
 */
void sec0_register_nonce(uint8_t src, uint8_t dst, const uint8_t* nonce) {
  sec_tx_session_t* s;
  s = get_tx_session_by_node(dst,src);
  if(s) {
    register_nonce(src,dst,FALSE,nonce);
    if( s->state == NONCE_GET_SENT || s->state == NONCE_GET ) {
      //memcpy(s->nonce,nonce,8);
      tx_session_state_set(s,ENC_MSG);
    } else if (s->state == ENC_MSG_SENT  || (s->state == ENC_MSG && s->data_len>0)){
      //memcpy(s->nonce,nonce,8);
      tx_session_state_set(s,ENC_MSG2);
    }
  } else {
    ERR_PRINTF("Nonce report but not for me src %d dst %d\n",src,dst);
  }
}

/******************************** Security RX code ***********************************/


/*
 *
 * Jeg kunne lave en ordenlig rx session hvis man m���tte rapotere samme nonce flere gange
 * hvis den ikke var brugt.
 *
 * Jeg sender NONCE_GET, men jeg misser en ack s��� den bliver sendt flere gange
 * Jeg modtager derfor flere reports. Kunne disse reports ikke indholde samme nonce?
 * Jeg kan ikke se problemet, jeg koder
 *
 */
typedef enum {
  RX_INIT,
  RX_ENC1,
  RX_ENC2,
  RX_SESSION_DONE
} rx_session_state_t;


typedef struct {
  uint8_t snode;
  uint8_t dnode;
  rx_session_state_t state;
  uint8_t seq_nr;
  uint8_t msg[MAX_ENCRYPTED_MSG_SIZE];
  uint8_t msg_len;
  clock_time_t timeout;
} rx_session_t;



uint8_t is_free(rx_session_t* e) {
  return  (e->state == RX_SESSION_DONE) || (e->timeout < clock_time()) ;
}
rx_session_t rxsessions[MAX_RXSESSIONS];

/**
 * Get a new free RX session.
 */
rx_session_t* new_rx_session(uint8_t snode,uint8_t dnode) {
  uint8_t i;
  for(i=0; i < MAX_RXSESSIONS; i++) {
    if( is_free(&rxsessions[i])) {
      rxsessions[i].snode=snode;
      rxsessions[i].dnode=dnode;
      rxsessions[i].timeout = clock_time() + CLOCK_SECOND*10; //Timeout in 10s
      return &rxsessions[i];
    }
  }
  return 0;
}

void free_rx_session(rx_session_t* s) {
  //memset(s->nonce,0,sizeof(s->nonce));
  s->state = RX_SESSION_DONE;
}

void sec0_init() {
  uint8_t i;

  for(i=0; i < MAX_RXSESSIONS; i++) {
    free_rx_session(&rxsessions[i]);
  }
  for(i=0; i < NUM_TX_SESSIONS; i++) {
    ctimer_stop(&tx_sessions[i].timer);
    memset(&tx_sessions[i],0,sizeof(sec_tx_session_t));
  }

  /* We do not re-initialize the nonce table here, bacause it is
   * actually desirable to keep the nonces after a gateway reset. The nonces will
   * timeout anyway. This fixes a situation like TO#05875, where the gateway get a nonce
   * get just before it resets after learnmode has completed.
   */
  /*for(i =0; i < NONCE_TABLE_SIZE; i++) {
    nonce_table[i].timeout = 0;
  }*/
  ctimer_stop(&nonce_timer);
  ctimer_set(&nonce_timer,1000,nonce_timer_timeout,0);
}

void sec0_abort_all_tx_sessions() {
  u8_t i;
  for(i=0; i < NUM_TX_SESSIONS; i++) {
    if(tx_sessions[i].param.dnode) {
      tx_sessions[i].tx_code = TRANSMIT_COMPLETE_FAIL;
      tx_session_state_set(&tx_sessions[i],TX_FAIL);
    }
  }
}

/**
 * Get a specific nonce from the nonce table. The session must not be expired
 */
rx_session_t* get_rx_session_by_nodes(uint8_t snode, uint8_t dnode) {
  uint8_t i;
  rx_session_t* e;
  for(i=0; i < MAX_RXSESSIONS; i++) {
    e = &rxsessions[i];
    if( !is_free(e) &&
        e->dnode == dnode &&
        e->snode == snode) {
      return e;
    }
  }
  return 0;
}

/**
 * Send a nonce from given source to given destination. The nonce is registered internally
 */
void sec0_send_nonce(ts_param_t *src)REENTRANT {
  u8_t nonce[8],tmp[8];
  ts_param_t dst;
  static ZW_SECURITY_NONCE_REPORT_FRAME nonce_res;

  do {
    aes_random8(nonce);
  } while( get_nonce(src->dnode,src->snode,nonce[0],tmp,FALSE) );

  nonce_res.cmdClass = COMMAND_CLASS_SECURITY;
  nonce_res.cmd = SECURITY_NONCE_REPORT;
  memcpy(&nonce_res.nonceByte1,nonce,8);

  /*Swap source and destination*/
  ts_param_make_reply(&dst, src);

  if( send_data(&dst,(u8_t*)&nonce_res,sizeof(nonce_res), 0,0)) {
    register_nonce(src->dnode,src->snode,FALSE,nonce);
  }
}

uint8_t sec0_decrypt_message(uint8_t snode, uint8_t dnode, uint8_t* enc_data, uint8_t enc_data_length,uint8_t* dec_message) REENTRANT {
    uint8_t iv[16]; /* Initialization vector for enc, dec,& auth */
    uint8_t mac[16];
    rx_session_t *s;
    uint8_t *enc_payload;
    uint8_t ri;
    auth_data_t* auth;
    uint8_t flags;

    if(enc_data_length < 19) {
      ERR_PRINTF("Encrypted message is too short\n");
      return 0;
    }

    ri =  enc_data[enc_data_length-9];

    /*Build the IV*/
    memcpy(iv,enc_data+2,8);

    /*Find the nonce in the nonce table */
    if(!get_nonce(dnode,snode,ri,&iv[8],FALSE)) {
      WRN_PRINTF("Nonce for %d -> %d not found",(int)dnode, (int)snode);
      return 0;
    } else {
      nonce_clear(dnode,snode);
    }

#if NONCE_OPT
    register_nonce(snode,dnode,TRUE,enc_data+2);
#endif

    /*TODO don't create sessions for single fragment frames*/
    s = get_rx_session_by_nodes(snode,dnode);
    if(!s)
    {
      s = new_rx_session(snode,dnode);
      if(s) {
        s->state = RX_INIT;
      } else {
        WRN_PRINTF("no more RX sessions available\n");
        return 0;
      }
    }

    enc_payload = enc_data+2+8;

    /*Temporargit y use dec_message for auth verification*/
    auth = (auth_data_t*)dec_message;
    /*Fill in the auth structure*/
    auth->sh = enc_data[1];
    auth->senderNodeID = snode;
    auth->receiverNodeID = dnode;
    auth->payloadLength = enc_data_length - 19;
    memcpy( (uint8_t*)auth+4, enc_payload, auth->payloadLength);

    /* Authtag */
    aes_set_key_tpt(authkey,iv);
    aes_cbc_mac((uint8_t*)auth,4 + auth->payloadLength,mac);

    if(memcmp(mac, enc_data+ enc_data_length-8,8) !=0 ) {
      ERR_PRINTF("Unable to verify auth tag\n");
      return 0;
    }
    DBG_PRINTF("Authentication verified\n");
    /*Decrypt */
    aes_set_key_tpt(enckey,iv);
    aes_ofb(enc_payload,auth->payloadLength);

    flags = *enc_payload;

    if(flags & SECURITY_MESSAGE_ENCAPSULATION_PROPERTIES1_SEQUENCED_BIT_MASK ) {
      if((flags & SECURITY_MESSAGE_ENCAPSULATION_PROPERTIES1_SECOND_FRAME_BIT_MASK)==0) {
        //First frame
        s->seq_nr = flags & 0xF;
#ifndef __ASIX_C51__
        ERR_PRINTF("State is %d seq %u expecetd %u\n", (int)s->state,flags & 0xF,s->seq_nr);
#endif
        s->msg_len = enc_data_length - 20;
        s->state=RX_ENC1;
        memcpy(s->msg, enc_payload+1,s->msg_len);
        return 0;
      } else {
        //Second frame
        if((s->state != RX_ENC1) || (flags & 0xF) != s->seq_nr) {
          ERR_PRINTF("State is %d seq %u expecetd %u\n", (int)s->state,flags & 0xF,s->seq_nr);
          goto state_error;
        } else {
          s->state  = RX_ENC2;
        }
        memcpy(dec_message,s->msg, s->msg_len);
        memcpy(dec_message + s->msg_len,enc_payload+1 ,enc_data_length - 20);

        free_rx_session(s);
        return (s->msg_len + enc_data_length - 20);
      }
    } else {
      /* Single frame message */
      memcpy(dec_message,enc_payload+1 ,enc_data_length - 20);
      free_rx_session(s);
      return (enc_data_length - 20);
    }
    return 0;
state_error:
  ERR_PRINTF("Security RX session is not in the right state\n");
  return 0;
}


void sec0_reset_netkey() {
  LOG_PRINTF("Reinitializing network key\n");
  aes_random8( &networkKey[0] );
  aes_random8( &networkKey[8] );

  nvm_config_set(security_netkey,networkKey);
}

