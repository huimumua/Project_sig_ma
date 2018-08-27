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

#include "sc_types.h"
#include "Secure_learnRequired.h"
#include "Secure_learn.h"

#include "contiki.h"
#include "ctimer.h"

#include "Serialapi.h"
#include "serial_api_process.h"
#include "ZIP_Router.h"
#include "security_layer.h"
#include "Security_Scheme0.h"
#include "CC_NetworkManagement.h"

#include "NodeCache.h"
#include "ZW_ZIPApplication.h"
#include "S2_wrap.h"
#include "S2.h"
#include "s2_keystore.h"
#include "ZW_PRNG.h"
#define SECURITY_SCHEME_0_BIT 0x1

//Number of implemented schemes
#define N_SCHEMES 1

static Secure_learn ctx;
static struct ctimer timer;
static sec_learn_complete_t callback;

static const ZW_NETWORK_KEY_VERIFY_FRAME key_verify ={ COMMAND_CLASS_SECURITY, NETWORK_KEY_VERIFY };


static u8_t *secureCommandClassesList[N_SCHEMES] = {0}; /* List of supported command classes, when node communicate by this transport */
static u8_t secureCommandClassesListCount[N_SCHEMES]; /* Count of elements in supported command classes list */


/*Forward */
static void send_and_raise(BYTE node, BYTE *pBufData, BYTE dataLength, BYTE txOptions,security_scheme_t scheme);

ts_param_t cur_tsparm;
static void send_and_raise_ex(BYTE *pBufData, BYTE dataLength);
/**
 * These frames are never used at the same time.
 */
static union
{
  ZW_SECURITY_SCHEME_INHERIT_FRAME scheme_inherit_frame;
  ZW_SECURITY_SCHEME_REPORT_FRAME scheme_report_frame;
  ZW_SECURITY_SCHEME_GET_FRAME scheme_get_frame;
  u8_t key_set_frame[16 + 2];
  u8_t cmd[64];
} tx;

/************************************* Common functions *******************************************/

/*
 * Load security states from eeprom
 */
void security_load_state()
{
  int8_t scheme;
  keystore_network_key_read(KEY_CLASS_S0,networkKey);
  nvm_config_get(security_scheme, &scheme);
  secure_learnIface_set_net_scheme(&ctx, scheme);
  sec0_set_key(networkKey);
}

/*
 * Save security states to eeprom
 */
void security_save_state()
{
  int8_t scheme;
  scheme = secure_learnIface_get_net_scheme(&ctx);
  DBG_PRINTF("Setting scheme %x\n", scheme);
  nvm_config_set(security_scheme, &scheme);
  keystore_network_key_write(KEY_CLASS_S0, networkKey);
}

void security_init()
{
  static int prng_initialized = 0;
  if(!prng_initialized) {
    S2_init_prng();
    InitPRNG();
    prng_initialized = 1;
  }


  secure_learn_init(&ctx);
  secure_learn_enter(&ctx);

  /* Initialize the transport layer*/

  sec0_init();

  security_load_state();

  sec2_init();
}

void security_set_supported_classes(u8_t* classes, u8_t n_classes) {
  secureCommandClassesList[0] = classes;
  secureCommandClassesListCount[0] = n_classes;
}

/**
 * TODO maybe this should be a part of the state machine
 */
void security_set_default()
{
  u8_t scheme = 0;

  if (ZW_Type_Library() & (ZW_LIB_CONTROLLER_STATIC | ZW_LIB_CONTROLLER | ZW_LIB_CONTROLLER_BRIDGE))
  {
    scheme = secure_learnIface_get_supported_schemes(&ctx);
    sec0_reset_netkey();
  }
  secure_learnIface_set_net_scheme(&ctx, scheme);
  security_save_state();
  security_init();
}

static void send_data_callback(BYTE status,void*user, TX_STATUS_TYPE *t)
{
  if (status == TRANSMIT_COMPLETE_OK)
  {
    DBG_PRINTF("TX done ok\n");
    secure_learnIface_raise_tx_done(&ctx);
  } else
  {
    DBG_PRINTF("TX done fail\n");
    secure_learnIface_raise_tx_fail(&ctx);
  }
  process_poll(&serial_api_process);
}

/**
 * timeout function used by the statemachine timer
 */
static void timeout(void* user)
{
  WRN_PRINTF("Security timer triggered\n.");
  secure_learn_raiseTimeEvent(&ctx, user);
  process_poll(&serial_api_process);
}


void secure_learn_setTimer(Secure_learn* handle,const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic)
{
  ctimer_set(&timer, time_ms, timeout, evid);
}

void secure_learn_unsetTimer(Secure_learn* handle,const sc_eventid evid)
{
  ctimer_stop(&timer);
}

void secure_poll()
{
  int s = -1;

  while (s!=ctx.stateConfVector[0]) {
    //LOG_PRINTF("sec state %i\n",ctx.stateConfVector[0]);
    s=ctx.stateConfVector[0];

    secure_learn_runCycle(&ctx);
  }
}


void secure_learnIface_send_commands_supported(const sc_integer node,const sc_integer snode, const sc_integer txOptions) CC_REENTRANT_ARG
{
  u8_t scheme = 0;
  BYTE roomLeft;
  BYTE copyLen;
  int len;

  tx.cmd[0] = COMMAND_CLASS_SECURITY;
  tx.cmd[1] = SECURITY_COMMANDS_SUPPORTED_REPORT;
  tx.cmd[2] = 0;

  DBG_PRINTF("Sending commands supported report net_scheme is %i\n",net_scheme);

  if(
      (cur_tsparm.snode == MyNodeID) && (net_scheme == SECURITY_SCHEME_0)
    )
  {
    memcpy( &tx.cmd[3], secureCommandClassesList[scheme], secureCommandClassesListCount[0]);
    len = secureCommandClassesListCount[scheme];

    /* Add Unsolicited Destination CCs */
    roomLeft = sizeof(tx.cmd) - 3 - len;
    copyLen = nSecureClassesPAN < roomLeft ? nSecureClassesPAN : roomLeft;
    memcpy(&tx.cmd[3+secureCommandClassesListCount[scheme]], SecureClassesPAN, copyLen);
    len += copyLen;
  } else {
    secureCommandClassesListCount[scheme]=0;
    len =0;
  }
  send_and_raise_ex( (BYTE *) tx.cmd, 3 +len);
}

void /*RET Nothing                  */
security_CommandHandler(ts_param_t* p,
const ZW_APPLICATION_TX_BUFFER *pCmd, /* IN Payload from the received frame, the union */
/*    should be used to access the fields */BYTE cmdLength) /* IN Number of command bytes including the command */
{
  BYTE txOption;

  txOption = ((p->rx_flags & RECEIVE_STATUS_LOW_POWER) ? TRANSMIT_OPTION_LOW_POWER : 0) | TRANSMIT_OPTION_ACK
      | TRANSMIT_OPTION_EXPLORE;

  if (pCmd->ZW_Common.cmdClass != COMMAND_CLASS_SECURITY && p->dnode != MyNodeID)
  {
    return;
  }

  /*If we are not in any inclusion state, and have no scheme, then ignore all s0 frames */
  if(secure_learn_isActive(&ctx, Secure_learn_main_region_Idle) && get_net_scheme()<=0) {
      return;
  }

  switch (pCmd->ZW_Common.cmd)
  {
  /*Learn mode */
  case NETWORK_KEY_SET:
    if(p->scheme == SECURITY_SCHEME_0 &&
        secure_learn_isActive(&ctx, Secure_learn_main_region_LearnMode_r1_Scheme_report) &&
        cmdLength >= (sizeof(ZW_NETWORK_KEY_SET_1BYTE_FRAME) +15)
    ) {
      memcpy(networkKey, &pCmd->ZW_NetworkKeySet1byteFrame.networkKeyByte1, 16);
      sec0_set_key(networkKey);
      secure_learnIface_set_txOptions(&ctx, txOption);
      secure_learnIface_raise_key_set(&ctx, p->snode);
    }
    break;
  case SECURITY_SCHEME_GET:
    NetworkManagement_s0_started();
    MyNodeID = p->dnode;

    if(p->scheme == NO_SCHEME && cmdLength >= sizeof(ZW_SECURITY_SCHEME_GET_FRAME) ) {
      secure_learnIface_set_scheme(&ctx, (pCmd->ZW_SecuritySchemeGetFrame.supportedSecuritySchemes & 0xFE) | 0x1
          );
      secure_learnIface_set_txOptions(&ctx, txOption);
      secure_learnIface_raise_scheme_get(&ctx, p->snode);
    }
    break;
  case SECURITY_SCHEME_INHERIT:
    /* MUST be received securely */
    if(p->scheme == SECURITY_SCHEME_0 && cmdLength>= sizeof(ZW_SECURITY_SCHEME_INHERIT_FRAME) ) {
      /* Must support SCHEME_0*/
      if ((pCmd->ZW_SecuritySchemeInheritFrame.supportedSecuritySchemes & SECURITY_SCHEME_0_BIT) == 0) {
        secure_learnIface_set_txOptions(&ctx, txOption);
        secure_learnIface_raise_scheme_inherit(&ctx, p->snode);
      }
    }
    break;

    /* Add node */
  case SECURITY_SCHEME_REPORT:
    if(p->scheme == SECURITY_SCHEME_0 || secure_learn_isActive(&ctx, Secure_learn_main_region_InclusionMode_r1_SchemeRequest)) {
      if(cmdLength >= sizeof( ZW_SECURITY_SCHEME_REPORT_FRAME) ) {
        secure_learnIface_set_scheme(&ctx, (pCmd->ZW_SecuritySchemeReportFrame.supportedSecuritySchemes & 0xFE) |0x1
            );
      secure_learnIface_raise_scheme_report(&ctx, p->snode);
      }
    }
    break;
  case NETWORK_KEY_VERIFY:
    if(p->scheme == SECURITY_SCHEME_0) {
      secure_learnIface_raise_key_verify(&ctx, p->snode);
    }
    break;

    /* General */
  case SECURITY_COMMANDS_SUPPORTED_GET:
    if(p->scheme == SECURITY_SCHEME_0) {
      ts_param_make_reply(&cur_tsparm,p);
      secure_learnIface_raise_commandsSupportedRequest(&ctx);
    }
    break;
  case SECURITY_NONCE_GET:
    if(p->scheme == NO_SCHEME) {
      sec0_send_nonce(p);
    }
    break;
  case SECURITY_NONCE_REPORT:
    if(p->scheme == NO_SCHEME && cmdLength >= sizeof(ZW_SECURITY_NONCE_REPORT_FRAME)) {
      sec0_register_nonce(p->snode,p->dnode,&pCmd->ZW_SecurityNonceReportFrame.nonceByte1 );
    }
    break;
  case SECURITY_MESSAGE_ENCAPSULATION_NONCE_GET:
  case SECURITY_MESSAGE_ENCAPSULATION:
  {
   /*TODO This is not ok for Large Z/IP frames */
    uint8_t rxBuffer[64];
    uint8_t len;

    if(secure_learn_isActive(&ctx, Secure_learn_main_region_Idle) &&
        (isNodeBad(p->dnode) || isNodeBad(p->snode)) ) {
      WRN_PRINTF("Dropping security package from KNWON BAD NODE\n");
      return;
    }

    len = sec0_decrypt_message(p->snode,p->dnode,(uint8_t*)pCmd,cmdLength,rxBuffer);
    if(len) {
      if(!isNodeSecure(p->snode)) {
        secure_learnIfaceI_register_scheme(p->snode,1<<SECURITY_SCHEME_0); /*TODO Is this ok ?*/
      }
      p->scheme = SECURITY_SCHEME_0;
      ApplicationCommandHandlerZIP(p,(ZW_APPLICATION_TX_BUFFER*)rxBuffer,len);
    }
    if(pCmd->ZW_Common.cmd == SECURITY_MESSAGE_ENCAPSULATION_NONCE_GET) {
      sec0_send_nonce(p);
    }
  }
  break;
  }

  process_poll(&serial_api_process);
}

/**
 * Store security state, ie. save scheme and keys.
 */
void secure_learnIfaceL_save_state()
{
  security_save_state();
}
/**
 * New keys should be generated.
 */
void secure_learnIfaceL_new_keys()
{
  sec0_reset_netkey();
}

void secure_learnIface_complete(const sc_integer scheme_mask)
{
  uint8_t flags = 0;
  /*Update eeprom with negotiated scheme*/
  if (scheme_mask > 0)
  {
    LOG_PRINTF("Secure add/inclusion succeeded, with schemes 0x%x\n", scheme_mask);
    flags = NODE_FLAG_SECURITY0;
  } else if(scheme_mask < 0)
  {
    flags = NODE_FLAG_KNOWN_BAD;
    ERR_PRINTF("Secure add/inclusion failed\n");
  }

  if (callback)
  {
    callback(flags);
  }
}

static void send_and_raise_ex(BYTE *pBufData, BYTE dataLength) {
  if (!ZW_SendDataAppl(&cur_tsparm,pBufData,dataLength, send_data_callback,0))
  {
    secure_learnIface_raise_tx_fail(&ctx);
    process_poll(&serial_api_process);

  }
}
/**
 * SendData wrapper that raises a tx_done or tx_fial on completion.
 * Send using highest available security scheme
 */
static void send_and_raise(BYTE node, BYTE *pBufData, BYTE dataLength, BYTE txOptions,security_scheme_t scheme)
{
  ts_param_t p;
  ts_set_std(&p,node);
  p.scheme = scheme;
  p.tx_flags = txOptions;

  if (!ZW_SendDataAppl(&p,pBufData,dataLength, send_data_callback,0))
  {
    secure_learnIface_raise_tx_fail(&ctx);
    process_poll(&serial_api_process);
  }
}

int8_t get_net_scheme()
{
  return secure_learnIface_get_net_scheme(&ctx);
}

/****************************** Learn mode related functions *************************************/

/**
 * Enter learn mode
 */
void security_learn_begin(sec_learn_complete_t __cb)
{
  if (secure_learn_isActive(&ctx, Secure_learn_main_region_Idle))
  { // Anoying check but needed to protect the isController variable
    callback = __cb;

    if (ZW_Type_Library() & (ZW_LIB_CONTROLLER_STATIC | ZW_LIB_CONTROLLER | ZW_LIB_CONTROLLER_BRIDGE))
    {
      secure_learnIface_set_isController(&ctx, TRUE);
    } else
    {
      secure_learnIface_set_isController(&ctx, FALSE);
    }
    LOG_PRINTF("security_learn_begin\n");
    secure_learnIface_raise_learnRequest(&ctx);
    process_poll(&serial_api_process);
  }
}

void secure_learnIfaceL_send_scheme_report(const sc_integer node, const sc_integer txOptions)
{

  u8_t ctrlScheme; // The including controllers scheme
  u8_t my_schemes;
  ctrlScheme = secure_learnIface_get_scheme(&ctx);
  my_schemes = secure_learnIface_get_supported_schemes(&ctx) & ctrlScheme; //Common schemes

  /* Check if controller supports schemes that I don't */
  if (ctrlScheme ^ my_schemes)
  {
    /* I'm not allowed to include nodes */

    /*FIXME ... what to do? */
  }

  tx.scheme_report_frame.cmdClass = COMMAND_CLASS_SECURITY;
  tx.scheme_report_frame.cmd = SECURITY_SCHEME_REPORT;
  tx.scheme_report_frame.supportedSecuritySchemes = 0;//my_schemes;


  if(secure_learnIface_get_net_scheme(&ctx) == 0) {
    send_and_raise(node, (BYTE *) &tx.scheme_report_frame, sizeof(ZW_SECURITY_SCHEME_REPORT_FRAME), txOptions,NO_SCHEME);
  } else {
    send_and_raise(node, (BYTE *) &tx.scheme_report_frame, sizeof(ZW_SECURITY_SCHEME_REPORT_FRAME), txOptions,SECURITY_SCHEME_0);
  }
}

void secure_learnIfaceL_set_inclusion_key() REENTRANT
{
  BYTE key[16];
  memset(key, 0, sizeof(key));
  sec0_set_key(key);
}

void secure_learnIfaceL_send_key_verify(const sc_integer node, const sc_integer txOptions)
{
  send_and_raise(node, (BYTE *) &key_verify, sizeof(key_verify), txOptions,SECURITY_SCHEME_0);
}

/********************************** Add node related ******************************************/

u8_t security_add_begin(u8_t node, u8_t txOptions, BOOL controller, sec_learn_complete_t __cb)
{
  if (secure_learn_isActive(&ctx, Secure_learn_main_region_Idle)  && !isNodeSecure(node))
  { // Annoying check but needed to protect the isController variable
    LOG_PRINTF("Secure add begin\n");
    callback = __cb;
    secure_learnIface_set_isController(&ctx, controller);
    secure_learnIface_set_txOptions(&ctx, txOptions);
    secure_learnIface_raise_inclusionRequest(&ctx, node);
    process_poll(&serial_api_process);
    return TRUE;
  } else {
    return FALSE;
  }
}

void secure_learnIfaceI_send_scheme_get(const sc_integer node, const sc_integer txOptions)
{
  tx.scheme_get_frame.cmdClass = COMMAND_CLASS_SECURITY;
  tx.scheme_get_frame.cmd = SECURITY_SCHEME_GET;
  tx.scheme_get_frame.supportedSecuritySchemes = 0;
  //tx.scheme_get_frame.supportedSecuritySchemes = secure_learnIface_get_net_scheme(&ctx);
  send_and_raise(node, (BYTE *) &tx.scheme_get_frame, sizeof(tx.scheme_get_frame), txOptions,NO_SCHEME);
}

void secure_learnIfaceI_send_key(const sc_integer node, const sc_integer txOptions) REENTRANT
{
  u8_t inclusionKey[16];
  tx.key_set_frame[0] = COMMAND_CLASS_SECURITY;
  tx.key_set_frame[1] = NETWORK_KEY_SET;
  memcpy(tx.key_set_frame + 2, networkKey, 16);

  /*TODO It might be better to do this in some other way, meybe a txOption .... */
  memset(inclusionKey, 0, 16);
  sec0_set_key(inclusionKey);

  send_and_raise(node, (BYTE *) &tx.key_set_frame, sizeof(tx.key_set_frame), txOptions,SECURITY_SCHEME_0);
}

void secure_learnIfaceI_send_scheme_inherit(const sc_integer node, const sc_integer txOptions)
{
  tx.scheme_inherit_frame.cmdClass = COMMAND_CLASS_SECURITY;
  tx.scheme_inherit_frame.cmd = SECURITY_SCHEME_INHERIT;
  tx.scheme_inherit_frame.supportedSecuritySchemes = 0;
  //tx.scheme_inherit_frame.supportedSecuritySchemes = secure_learnIface_get_net_scheme(&ctx);
  send_and_raise(node, (BYTE *) &tx.scheme_inherit_frame, sizeof(tx.scheme_inherit_frame), txOptions,SECURITY_SCHEME_0);
}

void secure_learnIfaceI_restore_key()
{
  sec0_set_key(networkKey);
}

/**
 * Register which schemes are supported by a node
 */
void secure_learnIfaceI_register_scheme(const sc_integer node, const sc_integer scheme)
{

  //UpdateCacheEntry(node,0,0,0); //Make sure node exists in node cache
  /*TODO .... */
  if (scheme & SECURITY_SCHEME_0_BIT)
  {
    LOG_PRINTF("Registering node %d with schemes %d\n",node,SECURITY_SCHEME_0_BIT);
    SetCacheEntryFlagMasked(node, NODE_FLAG_SECURITY0, NODE_FLAG_SECURITY0);
  } else
  {
    SetCacheEntryFlagMasked(node, 0, NODE_FLAG_SECURITY0);
  }
}

uint8_t secure_learn_active() {
  return !secure_learn_isActive(&ctx, Secure_learn_main_region_Idle);
}

void sec0_abort_inclusion() {

  if(!secure_learn_isActive(&ctx, Secure_learn_main_region_Idle)) {
    DBG_PRINTF("S0 inclusion was aborted");
    secure_learn_init(&ctx);
    secure_learn_enter(&ctx);
    ctimer_stop(&timer);
  }
}
