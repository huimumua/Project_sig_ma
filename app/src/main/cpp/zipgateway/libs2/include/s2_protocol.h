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
 * s2_protocol.h
 *
 *  Created on: Jun 30, 2015
 *      Author: aes
 */

#ifndef PROTOCOL_S2_PROTOCOL_H_
#define PROTOCOL_S2_PROTOCOL_H_

#include "S2.h"
#include "ctr_drbg.h"
#include "s2_classcmd.h"
#include "ZW_typedefs.h"
#include "ZW_classcmd.h"

#define UNENCRYPTED_CLASS 0xFF
#ifdef __C51__
#define SPAN_TABLE_SIZE 5
#define MPAN_TABLE_SIZE 5
#else
#define SPAN_TABLE_SIZE 10
#define MPAN_TABLE_SIZE 10
#endif
#define MOS_LIST_LENGTH 3
#ifdef ZW050x
#define WORKBUF_SIZE 200
#else
#define WORKBUF_SIZE 1280
#endif
#define S2_MULTICAST

typedef uint8_t nonce_t[16];

#ifdef ZW_CONTROLLER
#define N_SEC_CLASS 5
#else
#define N_SEC_CLASS 3
#endif
#define DSK_SSA_CHALLENGE_LENGTH 2
#define DSK_CSA_CHALLENGE_LENGTH 4

typedef uint8_t public_key_t[32];

typedef enum
{
  SPAN_NOT_USED,
  SPAN_NO_SEQ,
  SPAN_SOS,
  SPAN_SOS_LOCAL_NONCE,
  SPAN_SOS_REMOTE_NONCE,
  SPAN_INSTANTIATE,
  SPAN_NEGOTIATED
} span_state_t;


typedef enum{
  S2_INC_IDLE,
  S2_AWAITING_KEX_GET,
  S2_AWAITING_KEX_REPORT,
  S2_AWAITING_KEY_USER_ACCEPT,
  S2_AWAITING_KEX_SET,
  S2_AWAITING_PUB_KEY_A,
  S2_AWAITING_PUB_KEY_B,
  S2_AWAITING_USER_ACCEPT,
  S2_AWAITING_USER_A_ACCEPT,
  S2_PENDING_ECHO_KEX_REPORT,
  S2_ECHO_KEX_SET_SENDING,
  S2_AWAITING_ECHO_KEX_SET,
  S2_AWAITING_ECHO_KEX_REPORT,
  S2_AWAITING_NET_KEY_GET,
  S2_AWAITING_NET_KEY_REPORT,
  S2_AWAITING_NET_KEY_VERIFY,
  S2_AWAITING_TRANSFER_END,
  S2_KEY_EXCHANGED,
  S2_SENDING_FINAL_TRANSFER_END,
  S2_ERROR_SENT,
  S2_INC_STATE_ANY,
}s2_inclusion_state_t;


struct SPAN
{
  union
  {
    CTR_DRBG_CTX rng;
    uint8_t r_nonce[16];
  } d;
  node_t lnode;
  node_t rnode;

  uint8_t rx_seq; // sequence number of last received message
  uint8_t tx_seq; // sequence number of last sent message

  security_class_t class_id; //The id of the security group in which this span is negotiated.
  span_state_t state;
};

struct MPAN
{
  uint8_t owner_id; //this is the node id of the node maintaining mcast group members
  uint8_t group_id; //a unique id generated by the group maintainer(sender)
  uint8_t inner_state[16]; //The Multicast  pre-agreed nonce inner state
  security_class_t class_id;

  enum
  {
    MPAN_NOT_USED, MPAN_SET, MPAN_MOS
  } state; //State of this entry
};

struct MOS_LIST {
  uint8_t node_id; //node_id if reserved to "not used"
  uint8_t group_id;
};

typedef enum {
  IDLE,
  WAIT_NONCE_RAPORT,
  SENDING_MSG,
  SENDING_MULTICAST,
  VERIFYING_DELIVERY,
  IS_MOS_WAIT_REPLY, //Wait for reply from application layer...
} states_t;

typedef struct {
  const s2_connection_t *con;
  union {
    struct {
      const uint8_t* buffer;
      uint16_t len;
    } buf;
    struct {
      uint8_t status;
      uint16_t time;
    } tx;
  } d;
} event_data_t;

typedef enum {
  SEND_MSG,
  SEND_MULTICAST,
  SEND_DONE,
  GOT_NONCE_GET,
  GOT_NONCE_RAPORT,
  GOT_ENC_MSG,
  GOT_BAD_ENC_MSG,
  GOT_ENC_MSG_MOS,
  TIMEOUT,
  ABORT,
} event_t;

typedef enum {
   AUTH_OK,
   PARSE_FAIL,
   AUTH_FAIL,
   SEQUENCE_FAIL,
} decrypt_return_code_t;

//#define S2_MULTICAST
struct S2
{
  struct sec_group {
    network_key_t enc_key; //Ke 16 bytes
    network_key_t mpan_key; //Ke 16 bytes
    uint8_t nonce_key[32]; //Knonce 32 bytes
  } sg[N_SEC_CLASS];

  uint8_t csa_support;
  uint8_t kex_set_byte2;
  uint8_t scheme_support;
  uint8_t curve_support;
  uint8_t key_granted;
  uint8_t key_exchange;
  uint8_t key_requested;

  uint8_t loaded_keys; //Bit mask of S2 keys in use
  uint32_t my_home_id;

  const uint8_t* sec_commands_supported_frame;
  uint8_t sec_commands_supported_frame_size;

  s2_connection_t peer;
  s2_connection_t inclusion_peer;

  const uint8_t * buf;
  uint16_t length;

//  struct SPAN xdata *span; //The current span
  struct MPAN xdata *mpan; //The current mpan


  struct SPAN span_table[SPAN_TABLE_SIZE];
#ifdef S2_MULTICAST
  struct MPAN mpan_table[MPAN_TABLE_SIZE];
  struct MOS_LIST mos_list[MOS_LIST_LENGTH];
#endif
  states_t fsm;
  uint8_t retry;
  s2_inclusion_state_t inclusion_state;
  enum {INCLUSION_MODE_CSA, INCLUSION_MODE_SSA} inclusion_mode;
  uint8_t workbuf[WORKBUF_SIZE + 10 + 8 + 4];
  union {
    uint8_t inclusion_buf[40];
    uint8_t commands_sup_report_buf[40];
    uint8_t capabilities_report_buf[40];
  } u;
  uint8_t inclusion_buf_length;

  public_key_t public_key;

  //network_key_t temp_network_key;
};

#ifdef SINGLE_CONTEXT
#define s2_inclusion_post_event(a)                  s2_inclusion_post_event()
#endif

/**
 * Must be called from s2_inclusion_send_done_callback implementation in glue layer
 * when a transmission success occured.
 *
 * @param[in] p_context Structure identifying the context for current inclusion.
 * @param[in] status    TRUE on sucsess
 */
void s2_inclusion_send_done(struct S2 *p_context, uint8_t status);

/** @brief This function notifies the S2 inclusion state machine that a frame could not be decrypted.
 *
 *  @details During inclusion and exchange of public keys there is a risk that an invalid public
 *           key is used for calculating a common shared secret. This may happen in case an
 *           end-user provides wrond digits for the mising part of the DSK. If a frame cannot be
 *           decrypted the protocol can notify the inclusion state machine through this function.
 *
 * @param[in] p_context  Structure identifying the context for current inclusion.
 */
void s2_inclusion_decryption_failure(struct S2 *p_context);

/** @brief This function handles events received during secure inclusion of a node.
 *
 *  @param[in] context Pointer to the context to which the event is related.
 */
void s2_inclusion_post_event(struct S2 *p_context);

/**
 * Load all keys from keystore
 */
void s2_restore_keys(struct S2 *p_context);


/** Update the network key of a context
 * A context is unique to a node id, ie a bridge module will have multiple contexts.
 *
 * \param ctx Pointer to the context to update.
 *
 * \param class_id security class to update
 *
 * \param net_key Network key to use for this context. Only the derived keys are stored in SRAM. Remember for clear the
 * net_key from SRAM after calling this function.
 *
 * \param temp_key_expand Set to true if temp key is being expanded.
 *
 */
uint8_t
S2_network_key_update(struct S2 *ctxt,security_class_t class_id, const network_key_t net_key, uint8_t temp_key_expand);

#endif /* PROTOCOL_S2_PROTOCOL_H_ */
