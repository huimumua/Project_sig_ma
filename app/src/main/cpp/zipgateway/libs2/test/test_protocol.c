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
/*
 * test_protocol.c
 *
 *  Created on: Jun 24, 2015
 *      Author: aes
 */
#include "unity.h"
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<stdarg.h>
#include<stdio.h>

#include "../include/S2.h"
#include "../include/s2_protocol.h"

#include "ctr_drbg.h"
#include "ZW_typedefs.h"
#include "ZW_classcmd.h"

#ifdef __C51__
/* Local stub implementation of ZW_WatchDogKick() when compiling for C51. */
void ZW_WatchDogKick()
{
  // Do nothing. Stub implementation.
}
#endif // __C51__

extern int verbose;

struct test_state
{
  uint8_t frame[1280+8+4];
  uint16_t frame_len;

  int fcount; //number of frames sent

  int timeout_value;

  int s2_send_done;
  s2_tx_status_t s2_send_status;

  int send_data_return_fail; //Make send_data return fail

  uint8_t rx_frame[1280];
  uint16_t rx_frame_len;

  s2_connection_t last_trans; //The last transaction reversed
} ts;

uint8_t S2_send_frame_multi(struct S2* ctxt,s2_connection_t *peer, uint8_t* buf, uint16_t len) {


  if(verbose) {
      printf("********** MULTICAST ***************\n");
  }
  return S2_send_frame(ctxt,peer,buf,len);
}


uint8_t
S2_send_frame_no_cb(struct S2* ctxt, const s2_connection_t* dst, uint8_t* buf, uint16_t len)
{
  return S2_send_frame(ctxt,dst,buf,len);
}


uint8_t
S2_send_frame(struct S2* ctxt, const s2_connection_t* dst, uint8_t* buf, uint16_t len)
{

  int i;
  s2_connection_t swap;
  if(verbose) {
#if defined(__C51__) || defined (SINGLE_CONTEXT)
      printf("Sending  %02u -> %02u (len %04u) : ", dst->l_node, dst->r_node,len);
#else
      printf("Sending %p %02u -> %02u (len %04u) : ", ctxt, dst->l_node, dst->r_node,len);
#endif
      for (i = 0; i < len; i++)
        {
          printf("%02X", buf[i]);
        }
      printf("\n");
  }

  TEST_ASSERT(len < 1280+8+4);

  if (ts.send_data_return_fail)
    {
      if(verbose) printf("making send_data return fail\n");
      return 0;
    }

  memcpy(ts.frame, buf, len);
  ts.frame_len = len;
  ts.fcount++;

  swap = *dst;
  ts.last_trans.l_node = swap.r_node;
  ts.last_trans.r_node = swap.l_node;

  return 1;
}

void
S2_set_timeout(struct S2* ctxt, uint32_t interval)
{
  //ctxt = ctxt;
  if(verbose) printf("Timer set on interval %lu\n", (long unsigned)interval);
  ts.timeout_value = interval;
}

void
S2_send_done_event(struct S2* ctxt, s2_tx_status_t status)
{
  //ctxt=ctxt;
  if(verbose) printf("S2 send done status %d\n", (int)status);

  ts.s2_send_done++;
  ts.s2_send_status = status;
}

void
S2_msg_received_event(struct S2* ctxt, s2_connection_t* src, uint8_t* buf, uint16_t len)
{
  //int i;
  //ctxt = ctxt;

  if(verbose) {
      printf("Message received. from %d -> %d\n ", src->r_node, src->l_node);
      puts((char*)buf);
      /*for (i = 0; i < len; i++)
        {
          printf("%02X", buf[i]);
        }
      printf("\n");*/

  }

  memcpy(ts.rx_frame, buf, len);
  ts.rx_frame_len = len;
}

void
S2_get_hw_random(uint8_t* rnd,uint8_t len)
{
  int i;
  for (i = 0; i < len; i++)
    {
      rnd[i] = rand() % 0xFF;
    }
}

void S2_get_commands_supported(uint8_t lnode,uint8_t class_id, const uint8_t ** cmdClasses, uint8_t* length) {
  static uint8_t cmd_classes[] = {COMMAND_CLASS_SECURITY_2};
  *length = sizeof(cmd_classes);
  *cmdClasses = cmd_classes;
}
//---------------- global test state ---------------------
s2_connection_t conn12 =
  { 1, 2 }; //Sending form node 1 to 2
s2_connection_t conn21 =
  { 2, 1 }; //Sending form node 2 to 1

s2_connection_t conn13 =
  { 1, 3 }; //Sending form node 1 to 3
s2_connection_t conn31 =
  { 3, 1 }; //Sending form node 3 to 1


/*
 * Test that we a able to send a message from once instance to another
 */
const uint8_t key[] =
  { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
      0x0, };

const uint8_t key2[] = {0xab,0xab,0xab,0xab,0xab,0xab,0xab,0xab,0xab,0xab,0xab,0xab,0xab,0xab,0xab,0xab,};

const uint8_t key3[] = {0xac,0xac,0xac,0xac,0xac,0xac,0xac,0xac,0xac,0xac,0xac,0xac,0xac,0xac,0xac,0xac,};

const char hello[] = "HelloWorld";
const uint8_t nonce_get[] =
  { COMMAND_CLASS_SECURITY_2, SECURITY_2_NONCE_GET, 0 };
const uint8_t nonce_report[] =
  { COMMAND_CLASS_SECURITY_2, SECURITY_2_NONCE_REPORT };

const uint8_t commands_supported_report[] = {COMMAND_CLASS_SECURITY_2,SECURITY_2_COMMANDS_SUPPORTED_REPORT,COMMAND_CLASS_SECURITY_2};

struct S2* ctx1;
struct S2* ctx2;
struct S2* ctx3;

static void
my_setup()
{
  srand(0x44);
  //reset test state
  memset(&ts, 0, sizeof(ts));
  conn12.tx_options = 0;
  conn21.tx_options = 0;

  if(ctx1) {
      S2_destroy(ctx1);
  }
  ctx1 = S2_init_ctx( 0xAABBCCDD);

  if(ctx2) {
      S2_destroy(ctx2);
  }
  ctx2 = S2_init_ctx( 0xAABBCCDD);

  if(ctx3) {
      S2_destroy(ctx3);
  }
  ctx3 = S2_init_ctx( 0xAABBCCDD);

  S2_init_prng();

  S2_network_key_update(ctx1,0,key,0);
  S2_network_key_update(ctx2,0,key,0);
  S2_network_key_update(ctx3,0,key,0);

  S2_network_key_update(ctx1,1,key2,0);
  S2_network_key_update(ctx2,1,key2,0);
  S2_network_key_update(ctx3,1,key2,0);

  S2_network_key_update(ctx1,2,key3,0);
  S2_network_key_update(ctx2,2,key3,0);
  S2_network_key_update(ctx3,2,key3,0);

}

/*In general we should valgrind to make sure there is no buffer overruns*/


void wrap_test_s2_send_data(struct S2* ctx_dest, s2_connection_t* dst)
{
  ts.fcount = 0;
  ts.s2_send_done =0;
  S2_send_data(ctx1, dst, (uint8_t*) hello, sizeof(hello));

  TEST_ASSERT_EQUAL_STRING_LEN(nonce_get, ts.frame, 2);
  TEST_ASSERT_EQUAL(3, ts.frame_len);
  TEST_ASSERT_EQUAL(1, ts.fcount);
  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);

  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx_dest, &ts.last_trans, ts.frame, ts.frame_len);
  /*Expect that we get a nonce report*/
  TEST_ASSERT_EQUAL(2, ts.fcount);
  TEST_ASSERT_EQUAL(20, ts.frame_len);
  TEST_ASSERT_EQUAL_STRING_LEN(nonce_report, ts.frame, 2);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT_PROPERTIES1_SOS_BIT_MASK, ts.frame[3]);

  S2_send_frame_done_notify(ctx_dest, S2_TRANSMIT_COMPLETE_OK,0x42);

  /*Send the frame to ctx1*/
  S2_application_command_handler(ctx1, &ts.last_trans, ts.frame, ts.frame_len);
  TEST_ASSERT_EQUAL(3, ts.fcount);

  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  //TEST_ASSERT_EQUAL(ts.frame[2],COMMAND_CLASS_SECURITY_2); //seq
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION_PROPERTIES1_EXTENSION_BIT_MASK, ts.frame[3]);
  TEST_ASSERT_EQUAL(18, ts.frame[4]);
  TEST_ASSERT_EQUAL(S2_MSG_EXTHDR_CRITICAL_FLAG | S2_MSG_EXTHDR_TYPE_SN,
      ts.frame[5]);

  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);

  TEST_ASSERT_EQUAL(1, ts.s2_send_done); //Check we got a send done event
  TEST_ASSERT_EQUAL(S2_TRANSMIT_COMPLETE_OK, ts.s2_send_status); //Check we got a send done event

  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx_dest, &ts.last_trans, ts.frame, ts.frame_len);
  TEST_ASSERT_EQUAL(sizeof(hello), ts.rx_frame_len);
  TEST_ASSERT_EQUAL_STRING_LEN(hello, ts.rx_frame, sizeof(hello));
}

/**
 * Positive test. Check that we can send a single cast message between two instances, without
 * having a span established first.
 *
 * Verify the frame flow
 * - Nonce_get
 * - Nonce_raport, with SOS
 * - Encap_message with SN
 *
 * Verify the status code of the s2_send_data callback
 * Verify the frame received by the ctx2
 *
 */
void test_s2_send_data()
{
  my_setup();

  wrap_test_s2_send_data(ctx2, &conn12);
}



/**
 * Test that we can use single frame transmissions once SPAN
 * has been established.
 *
 */
void test_single_frame_transmission()
{
  const char hello[] = "HelloWorld Second Frame";

  test_s2_send_data();
  /* ----------- now send second frame with SPAN established --------- *
   */

  ts.fcount = 0; //Reset frame count
  ts.rx_frame_len = 0;

  S2_send_data(ctx1, &conn12, (uint8_t*) hello, sizeof(hello));
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  //TEST_ASSERT_EQUAL(ts.frame[2],COMMAND_CLASS_SECURITY_2); //seq
  TEST_ASSERT_EQUAL(0, ts.frame[3]); //Verify that there is no SN included
  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);
  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx2, &ts.last_trans, ts.frame, ts.frame_len);

  TEST_ASSERT_EQUAL(1, ts.fcount);
  TEST_ASSERT_EQUAL(sizeof(hello), ts.rx_frame_len);
  TEST_ASSERT_EQUAL_STRING_LEN(hello, ts.rx_frame, sizeof(hello));
}



/**
 * Make send data return FALSE in nonce get
 */
void test_send_data_fail_in_nonce_get()
{
  my_setup();
  ts.send_data_return_fail = 1;
  S2_send_data(ctx1, &conn12, (uint8_t*) hello, sizeof(hello));
  TEST_ASSERT_EQUAL(1, ts.s2_send_done);
  TEST_ASSERT_EQUAL(S2_TRANSMIT_COMPLETE_FAIL, ts.s2_send_status);
}

/**
 * Make send data return transmit complete no ack in nonce get
 */
void test_transmit_complete_no_ack_in_nonce_get()
{
  my_setup();
  S2_send_data(ctx1, &conn12, (uint8_t*) hello, sizeof(hello));
  TEST_ASSERT_EQUAL_STRING_LEN(nonce_get, ts.frame, 2);
  TEST_ASSERT_EQUAL(3, ts.frame_len);
  TEST_ASSERT_EQUAL(1, ts.fcount);
  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_NO_ACK,0x42);

  TEST_ASSERT_EQUAL(1, ts.s2_send_done);
  TEST_ASSERT_NOT_EQUAL(S2_TRANSMIT_COMPLETE_OK, ts.s2_send_status);
}

/**
 * Make make system timeout before nonce report is sent
 */
void test_transmit_timeout_after_sending_nonce_get()
{
  my_setup();
  S2_send_data(ctx1, &conn12, (uint8_t*) hello, sizeof(hello));
  TEST_ASSERT_EQUAL_STRING_LEN(nonce_get, ts.frame, 2);
  TEST_ASSERT_EQUAL(3, ts.frame_len);
  TEST_ASSERT_EQUAL(1, ts.fcount);
  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);
  S2_timeout_notify(ctx1);

  TEST_ASSERT_EQUAL(1, ts.s2_send_done);
  TEST_ASSERT_NOT_EQUAL(S2_TRANSMIT_COMPLETE_OK, ts.s2_send_status);
}

/**
 * Make make system timeout before before ACK of nonce get is sent.
 */
void test_transmit_timeout_before_sending_nonce_get()
{
  my_setup();
  S2_send_data(ctx1, &conn12, (uint8_t*) hello, sizeof(hello));
  TEST_ASSERT_EQUAL_STRING_LEN(nonce_get, ts.frame, 2);
  TEST_ASSERT_EQUAL(3, ts.frame_len);
  TEST_ASSERT_EQUAL(1, ts.fcount);
  S2_timeout_notify(ctx1);
  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);

  TEST_ASSERT_EQUAL(1, ts.s2_send_done);
  TEST_ASSERT_NOT_EQUAL(S2_TRANSMIT_COMPLETE_OK, ts.s2_send_status);
}



/* Test that duplicate nonce get frames are ignored
 * Test that duplicate nonce raport frames are ignored
 * Test that duplicate msg enap frames are ignored */
void test_dup_nonce_get()
{
  uint8_t backup_frame[sizeof(ts.frame)];
  uint8_t backup_len;

  my_setup();
  S2_send_data(ctx1, &conn12, (uint8_t*) hello, sizeof(hello));

  TEST_ASSERT_EQUAL_STRING_LEN(nonce_get, ts.frame, 2);
  TEST_ASSERT_EQUAL(3, ts.frame_len);
  TEST_ASSERT_EQUAL(1, ts.fcount);
  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);

  memcpy(backup_frame,ts.frame,ts.frame_len);
  backup_len = ts.frame_len;
  /*Send the frame to ctx2 twice */
  S2_application_command_handler(ctx2, &conn21, backup_frame, backup_len);
  S2_application_command_handler(ctx2, &conn21, backup_frame, backup_len);

  /*Expect that we get a nonce report*/
  TEST_ASSERT_EQUAL(2, ts.fcount);
  TEST_ASSERT_EQUAL(20, ts.frame_len);
  TEST_ASSERT_EQUAL_STRING_LEN(nonce_report, ts.frame, 2);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT_PROPERTIES1_SOS_BIT_MASK, ts.frame[3]);

  S2_send_frame_done_notify(ctx2, S2_TRANSMIT_COMPLETE_OK,0x42);

  memcpy(backup_frame,ts.frame,ts.frame_len);
  backup_len = ts.frame_len;
  /*Send the frame to ctx1 twice */
  S2_application_command_handler(ctx1, &conn12, backup_frame, backup_len);
  S2_application_command_handler(ctx1, &conn12, backup_frame, backup_len);

  TEST_ASSERT_EQUAL(3, ts.fcount);

  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  //TEST_ASSERT_EQUAL(ts.frame[2],COMMAND_CLASS_SECURITY_2); //seq
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION_PROPERTIES1_EXTENSION_BIT_MASK, ts.frame[3]);
  TEST_ASSERT_EQUAL(18, ts.frame[4]);
  TEST_ASSERT_EQUAL(S2_MSG_EXTHDR_CRITICAL_FLAG | S2_MSG_EXTHDR_TYPE_SN,
      ts.frame[5]);

  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);

  TEST_ASSERT_EQUAL(1, ts.s2_send_done); //Check we got a send done event
  TEST_ASSERT_EQUAL(S2_TRANSMIT_COMPLETE_OK, ts.s2_send_status); //Check we got a send done event

  memcpy(backup_frame,ts.frame,ts.frame_len);
  backup_len = ts.frame_len;
  /*Send the frame to ctx2 twice */
  S2_application_command_handler(ctx2, &conn21, backup_frame, backup_len);
  S2_application_command_handler(ctx2, &conn21, backup_frame, backup_len);


  TEST_ASSERT_EQUAL(sizeof(hello), ts.rx_frame_len);
  TEST_ASSERT_EQUAL_STRING_LEN(hello, ts.rx_frame, sizeof(hello));
}


/**
 * Test that we get transmissions complete fail when send_data
 * fails in encap_message
 *
 */
void test_send_data_fail_in_encap_message()
{
  const char hello[] = "HelloWorld Second Frame";

  test_s2_send_data();
  /* ----------- now send second frame with SPAN established --------- *
   */

  ts.fcount = 0; //Reset frame count
  ts.rx_frame_len = 0;
  ts.send_data_return_fail=1;
  ts.s2_send_done =0;

  S2_send_data(ctx1, &conn12, (uint8_t*) hello, sizeof(hello));
  TEST_ASSERT_EQUAL(1, ts.s2_send_done);
  TEST_ASSERT_EQUAL(S2_TRANSMIT_COMPLETE_FAIL, ts.s2_send_status);
}

/**
 * Test that we get transmissions complete fail when send_data
 * fails in encap_message
 *
 */
void test_send_data_no_ack_in_encap_message()
{
  const char hello[] = "HelloWorld Second Frame";

  test_s2_send_data();
  /* ----------- now send second frame with SPAN established --------- *
   */

  ts.fcount = 0; //Reset frame count
  ts.rx_frame_len = 0;
  ts.s2_send_done =0;

  S2_send_data(ctx1, &conn12, (uint8_t*) hello, sizeof(hello));
  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_NO_ACK,0x42);
  TEST_ASSERT_EQUAL(1, ts.s2_send_done);
  TEST_ASSERT_EQUAL(S2_TRANSMIT_COMPLETE_NO_ACK, ts.s2_send_status);
}


/* Test that sequence counter increases and test all message lengths */

  /*  loop len 0..1280
   *     send encap message with len
   *     verify message decryption
   *     verify seq increases
   */
void test_encap_seq_increase()
{
  uint8_t test_buf[1280];
  uint8_t seq;
  int i;

  for(i=0; i < sizeof(test_buf); i++) {
      test_buf[i] = rand() & 0xFF;
  }


  test_s2_send_data();
  /* ----------- now send second frame with SPAN established --------- *
   */

  ts.fcount = 0; //Reset frame count
  ts.rx_frame_len = 0;

  S2_send_data(ctx1, &conn12, (uint8_t*) hello, sizeof(hello));
  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);
  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx2, &ts.last_trans, ts.frame, ts.frame_len);
  seq = ts.frame[2];

  TEST_ASSERT_EQUAL(1, ts.fcount);
  TEST_ASSERT_EQUAL(sizeof(hello), ts.rx_frame_len);
  TEST_ASSERT_EQUAL_STRING_LEN(hello, ts.rx_frame, sizeof(hello));

  ts.fcount=0;
  for(i=1; i < 1280; i++) {
    S2_send_data(ctx1, &conn12, (uint8_t*) test_buf, i);
    TEST_ASSERT_EQUAL(i, ts.fcount);

    S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);
    /*Send the frame to ctx2*/
    S2_application_command_handler(ctx2, &ts.last_trans, ts.frame, ts.frame_len);
    TEST_ASSERT_EQUAL(++seq, ts.frame[2]);

    TEST_ASSERT_EQUAL(i, ts.rx_frame_len);
    TEST_ASSERT_EQUAL_STRING_LEN(test_buf, ts.rx_frame, i);
  }

  }

/*Validate nonce report */
void test_nonce_report()
{
  uint8_t nonce_get[] = {COMMAND_CLASS_SECURITY_2,SECURITY_2_NONCE_GET,42};
  int seq2;
  uint8_t nonce[8];
  my_setup();

  /*send nonce get */
  S2_application_command_handler(ctx2,&conn12,nonce_get,3);
  S2_send_frame_done_notify(ctx2,S2_TRANSMIT_COMPLETE_OK,0x42);

  /* verify nonce report save nonce for later */
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2,ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT,ts.frame[1]);
  seq2 = ts.frame[2];
  memcpy(nonce,&ts.frame[4],8);

  /*verify sequence number */
  nonce_get[2]++;
  S2_application_command_handler(ctx2,&conn12,nonce_get,3);
  S2_send_frame_done_notify(ctx2,S2_TRANSMIT_COMPLETE_OK,0x42);
  TEST_ASSERT_EQUAL(2,ts.fcount);
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2,ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT,ts.frame[1]);
  TEST_ASSERT_EQUAL(seq2+1,ts.frame[2]);


  /* verify reserved is 0 */
  /* verify SOS = 1 and MOS 0*
   */
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT_PROPERTIES1_SOS_BIT_MASK,ts.frame[3]);
  /* Verify nonce bytes */
  /* Verify nonce bytes different than in case 1 */
  TEST_ASSERT( memcmp(nonce,&ts.frame[4],8) != 0 )

  /* Verify frame length */
  TEST_ASSERT_EQUAL(4+16,ts.frame_len);

  /* send nonce get which is too short ie 2 bytes */
  /* Verify that no report is sent */
  ts.fcount =0;
  nonce_get[2]++;
  S2_application_command_handler(ctx2,&conn12,nonce_get,2);
  S2_send_done_event(ctx2,S2_TRANSMIT_COMPLETE_OK);
  TEST_ASSERT_EQUAL(0,ts.fcount);

  /* send nonce get which has some extra data appened */
  /* Verify we get a valid report*/
  nonce_get[2]++;
  S2_application_command_handler(ctx2,&conn12,nonce_get,6);
  S2_send_done_event(ctx2,S2_TRANSMIT_COMPLETE_OK);
  TEST_ASSERT_EQUAL(1,ts.fcount);
}

/**
 * Check recover after a node has sent SOS
 *
 * Scenario:
 *
 * A sends MSG1 to B
 * B is out of sync and sends Nonce report
 * A sends MSG2 to B, with SN
 * B receives MSG 2
 *
 */
void test_sos_recover_scenario1() {

  const char hello[] = "HelloWorld Second Frame";

  test_s2_send_data();

  ts.rx_frame_len=0;
  ts.s2_send_done = 0;
  ts.fcount = 0;
  /* ----------- now send second frame with SPAN established --------- *
   */
  S2_send_data(ctx1, &conn12, (uint8_t*) hello, sizeof(hello));
  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);
  TEST_ASSERT_EQUAL(1, ts.s2_send_done); //Check the callback

  /*Make the message invalid*/
  ts.frame[ts.frame_len-1] =0;
  ts.frame[ts.frame_len-2] =0;
  ts.frame[ts.frame_len-3] =0;
  ts.frame[ts.frame_len-4] =0;

  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx2, &ts.last_trans, ts.frame, ts.frame_len);

  /*Verify that the ctx2 sends a nonce report SOS, because it could not receive the frame.*/
  TEST_ASSERT_EQUAL(2, ts.fcount); //Check the callback
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT, ts.frame[1]);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT_PROPERTIES1_SOS_BIT_MASK, ts.frame[3]);
  S2_send_frame_done_notify(ctx2, S2_TRANSMIT_COMPLETE_OK,0x42);

  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx1, &ts.last_trans, ts.frame, ts.frame_len);

  /* send a new frame */
  S2_send_data(ctx1, &conn12, (uint8_t*) hello, sizeof(hello));


  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION_PROPERTIES1_EXTENSION_BIT_MASK, ts.frame[3]); /*Check that SN is included */
  TEST_ASSERT_EQUAL(18, ts.frame[4]);
  TEST_ASSERT_EQUAL(S2_MSG_EXTHDR_CRITICAL_FLAG | S2_MSG_EXTHDR_TYPE_SN,
      ts.frame[5]);

  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);


  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx2, &ts.last_trans, ts.frame, ts.frame_len);

  TEST_ASSERT_EQUAL(sizeof(hello), ts.rx_frame_len); //Verify that the frame has been received
  TEST_ASSERT_EQUAL_STRING_LEN(hello, ts.rx_frame, sizeof(hello));
}


/**
 * Check recover after a node has sent SOS
 *
 * Scenario:
 *
 * A sends MSG1 to B
 * B is out of sync and sends Nonce report
 * B sends MSG2, but is sending Nonce get first
 * A receives nonce get
 * A sends nonce report
 * B sends MSG 2
 * A Gets MSG 2
 *
 */
void test_sos_recover_scenario2() {

  const char hello[] = "HelloWorld Second Frame";

  test_s2_send_data();

  ts.rx_frame_len=0;
  ts.s2_send_done = 0;
  ts.fcount = 0;
  /* ----------- now send second frame with SPAN established --------- *
   */
  S2_send_data(ctx1, &conn12, (uint8_t*) hello, sizeof(hello));
  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);
  TEST_ASSERT_EQUAL(1, ts.s2_send_done); //Check the callback

  /*Make the message invalid*/
  ts.frame[ts.frame_len-1] =0;
  ts.frame[ts.frame_len-2] =0;
  ts.frame[ts.frame_len-3] =0;
  ts.frame[ts.frame_len-4] =0;

  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx2, &ts.last_trans, ts.frame, ts.frame_len);

  /*Verify that the ctx2 sends a nonce report SOS, because it could not receive the frame.*/
  TEST_ASSERT_EQUAL(2, ts.fcount); //Check the callback
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT, ts.frame[1]);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT_PROPERTIES1_SOS_BIT_MASK, ts.frame[3]);
  S2_send_frame_done_notify(ctx2, S2_TRANSMIT_COMPLETE_OK,0x42);

  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx1, &ts.last_trans, ts.frame, ts.frame_len);

  /* send a new frame from ctx 2*/
  S2_send_data(ctx2, &conn21, (uint8_t*) hello, sizeof(hello));

  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_GET, ts.frame[1]);
  S2_send_frame_done_notify(ctx2, S2_TRANSMIT_COMPLETE_OK,0x42);

  /*Send the frame to ctx1*/
  S2_application_command_handler(ctx1, &ts.last_trans, ts.frame, ts.frame_len);

  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT, ts.frame[1]);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT_PROPERTIES1_SOS_BIT_MASK, ts.frame[3]);
  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);

  /*Send the NONCE REPORT frame to ctx2*/
  S2_application_command_handler(ctx2, &ts.last_trans, ts.frame, ts.frame_len);

  /*Verify a ENCAP msg comes out */
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION_PROPERTIES1_EXTENSION_BIT_MASK, ts.frame[3]); /*Check that SN is included */
  TEST_ASSERT_EQUAL(18, ts.frame[4]);
  TEST_ASSERT_EQUAL(S2_MSG_EXTHDR_CRITICAL_FLAG | S2_MSG_EXTHDR_TYPE_SN,
      ts.frame[5]);

  S2_send_frame_done_notify(ctx2, S2_TRANSMIT_COMPLETE_OK,0x42);


  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx1, &ts.last_trans, ts.frame, ts.frame_len);

  /* verify that ctx1 gets the message */
  TEST_ASSERT_EQUAL(sizeof(hello), ts.rx_frame_len); //Verify that the frame has been received
  TEST_ASSERT_EQUAL_STRING_LEN(hello, ts.rx_frame, sizeof(hello));

}


/**Verify that message is ignored if MAC is invalid
 */

void test_encap_message_authcheck()
{
  test_s2_send_data();
  /* ----------- now send second frame with SPAN established --------- *
   */

  ts.fcount = 0; //Reset frame count
  ts.rx_frame_len = 0;

  S2_send_data(ctx1, &conn12, (uint8_t*) hello, sizeof(hello));
  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);

  /*Break the mac */
  ts.frame[ts.frame_len-3] = 0x1;
  ts.frame[ts.frame_len-2] = 0x2;
  ts.frame[ts.frame_len-1] = 0x3;
  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx2, &ts.last_trans, ts.frame, ts.frame_len);
  TEST_ASSERT_EQUAL(0, ts.rx_frame_len);
}

/* Verify we dont go into an infinite loop when parsing a zero-length header extension
 * and more-to-follow is set.*/
void test_hdr_ext_zero_length_and_more_to_follow()
{
  test_s2_send_data();
  /* ----------- now send second frame with SPAN established --------- *
   */

  ts.fcount = 0; //Reset frame count
  ts.rx_frame_len = 0;

  //uint8_t strange_frame[] = {0x9F, 0x03 , 0x17 , 0x01 , 0x02 , 0xBF , 0x00 , 0x89, 0xE , 0xDD , 0x69 , 0xFC , 0x92 , 0xA0 , 0xA5 , 0x09 , 0xF0 , 0xF4};
  uint8_t strange_frame[] = {0x9F, 0x03 , 0x17 , 0x01 , 0x02 , 0xBF , 0x00 , 0x89, 0 , 0 , 0 , 0, 0, 0, 0};

  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx2, &ts.last_trans, strange_frame, sizeof(strange_frame));
  TEST_ASSERT_EQUAL(0, ts.rx_frame_len);
}

/* Verify we dont go into an infinite loop when parsing a zero-length header extension
 * and more-to-follow is set.*/
void test_hdr_ext_zero_length_and_more_to_follow2()
{
  test_s2_send_data();
  /* ----------- now send second frame with SPAN established --------- *
   */

  ts.fcount = 0; //Reset frame count
  ts.rx_frame_len = 0;

  uint8_t strange_frame[] = {0x9F, 0x03 , 0x17 , 0x01 , 0x02 , 0xBF , 0x00 , 0x89, 0xE , 0xDD , 0x69 , 0xFC , 0x92 , 0xA0 , 0xA5 , 0x09 , 0xF0 , 0xF4};

  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx2, &ts.last_trans, strange_frame, sizeof(strange_frame));
  TEST_ASSERT_EQUAL(0, ts.rx_frame_len);
}

void test_encap_message_oversize()
{
  /*send message which is 1281 byes */
  /* verify rejecting by transport layer */

  /*construct message which is 1281 bytes long */
  /* verify that the message is ignored by receiver*/
  TEST_IGNORE_MESSAGE("Implement me");
 ;
}

void test_encap_message_lencheck()
{
  /* loop
   *   establish span
   *   send frame frame cropped from 0.. len bytes
   *   verify that message is ignored
   */
  TEST_IGNORE_MESSAGE("Implement me");
  ;
}

void test_encap_message_extcheck()
{
  /*construct message with two extensions which are non critical. One of them must be 255 bytes in len
   * verify the message is decrypted
   */
  TEST_IGNORE_MESSAGE("Implement me");
  ;
}

void test_encap_message_critical_extcheck()
{
  /* construct message with two extensions one are non critical and one is critical, and unknown.
   verify the message is ignored
   */
  TEST_IGNORE_MESSAGE("Implement me");
  ;
}

void test_encap_message_extlen()
{
  /* construct message with an extension of len 0.
   * verify that this message is ignored.
   */
  TEST_IGNORE_MESSAGE("Implement me");
  ;
}


void test_m_enacap_send()
{
  /* generate MPAN for nodes 2,3,4 */
  /* send secure single-cast message for node 2 and 3 (SPAN is now established) */
  /* push MPAN to node 2, out of band hack */

  /* enc_send_mcast to nodes 2,3,4*/
  /* verify that all three nodes got the message after transmission has completed. */

  /* enc_send_mcast to nodes 2,3,4*/
  /* verify that all three nodes got the message after transmission has completed but this time
   * after sending fewer frames. */

  TEST_IGNORE_MESSAGE("Implement me");
  ;
}

/**
 * Test that we can use single frame transmissions once SPAN
 * has been established.
 *
 */
void test_commands_supported_report()
{
  const uint8_t commands_supported_get[] = {COMMAND_CLASS_SECURITY_2,SECURITY_2_COMMANDS_SUPPORTED_GET};

  test_s2_send_data();
  /* ----------- now send second frame with SPAN established --------- *
   */

  ts.fcount = 0; //Reset frame count
  ts.rx_frame_len = 0;

  S2_send_data(ctx1, &conn12, (uint8_t*) commands_supported_get, sizeof(commands_supported_get));
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  //TEST_ASSERT_EQUAL(ts.frame[2],COMMAND_CLASS_SECURITY_2); //seq
  TEST_ASSERT_EQUAL(0, ts.frame[3]); //Verify that there is no SN included
  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);
  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx2, &ts.last_trans, ts.frame, ts.frame_len);

  S2_send_frame_done_notify(ctx2, S2_TRANSMIT_COMPLETE_OK,0x42);

  TEST_ASSERT_EQUAL(2, ts.fcount); //encap commands supported get + encap commands supported report
  S2_application_command_handler(ctx1, &ts.last_trans, ts.frame, ts.frame_len);


  /*Expect ctx2 to reply with command supported report*/

  TEST_ASSERT_EQUAL(sizeof(commands_supported_report), ts.rx_frame_len);
  TEST_ASSERT_EQUAL_MEMORY( commands_supported_report, ts.rx_frame, sizeof(commands_supported_report));
}

/**
 * Check that we receive S2_TRANSMIT_COMPLETE_OK if we are sending with verify,
 * if the receiving controller is able to decrypt the message
 */
void test_sd_verify_tx_ok() {

  const char hello[] = "HelloWorld Second Frame";

  test_s2_send_data();
  /* ----------- now send second frame with SPAN established --------- *
   */

  ts.fcount = 0; //Reset frame count
  ts.rx_frame_len = 0;
  ts.s2_send_status = S2_TRANSMIT_COMPLETE_NO_ACK;

  conn12.tx_options = S2_TXOPTION_VERIFY_DELIVERY;

  S2_send_data(ctx1, &conn12, (uint8_t*) hello, sizeof(hello));
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  //TEST_ASSERT_EQUAL(ts.frame[2],COMMAND_CLASS_SECURITY_2); //seq
  TEST_ASSERT_EQUAL(0, ts.frame[3]); //Verify that there is no SN included

  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);
  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx2, &ts.last_trans, ts.frame, ts.frame_len);

  TEST_ASSERT_EQUAL(1, ts.fcount);
  TEST_ASSERT_EQUAL(sizeof(hello), ts.rx_frame_len);
  TEST_ASSERT_EQUAL_STRING_LEN(hello, ts.rx_frame, sizeof(hello));


  TEST_ASSERT_NOT_EQUAL(S2_TRANSMIT_COMPLETE_OK, ts.s2_send_status);
  S2_timeout_notify(ctx1);
  TEST_ASSERT_EQUAL(S2_TRANSMIT_COMPLETE_OK, ts.s2_send_status);
}



/**
 * Check that we receive S2_TRANSMIT_COMPLETE_OK if we are sending with verify,
 * if the receiving controller is able to decrypt the message after the
 * second attempt
 */
void test_sd_verify_tx_ok_second_attempt() {
  const char hello[] = "HelloWorld Second Frame";
  uint8_t bad_frame[1500];
  test_s2_send_data();
  /* ----------- now send second frame with SPAN established --------- *
   */

  ts.fcount = 0; //Reset frame count
  ts.rx_frame_len = 0;
  ts.s2_send_status = S2_TRANSMIT_COMPLETE_NO_ACK;

  conn12.tx_options = S2_TXOPTION_VERIFY_DELIVERY;

  S2_send_data(ctx1, &conn12, (uint8_t*) hello, sizeof(hello));
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  TEST_ASSERT_EQUAL(0, ts.frame[3]); //Verify that there is no SN included

  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);

  /*make a bad frame */
  memcpy(bad_frame,ts.frame,ts.frame_len);
  bad_frame[ts.frame_len-1]++; // *this* will invalidate the MAC

  /*Send the BAD frame to ctx2*/
  S2_application_command_handler(ctx2, &ts.last_trans, bad_frame, ts.frame_len);

  /*We expect ctx2 to send a nonce report */
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT, ts.frame[1]);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT_PROPERTIES1_SOS_BIT_MASK, ts.frame[3]);

  /*Make ctx1 hear the nocne report*/
  S2_application_command_handler(ctx1, &ts.last_trans, ts.frame, ts.frame_len);

  /* ctx1 should now send a MSG,SN */
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION_PROPERTIES1_EXTENSION_BIT_MASK, ts.frame[3]);
  TEST_ASSERT_EQUAL(18, ts.frame[4]);
  TEST_ASSERT_EQUAL(S2_MSG_EXTHDR_CRITICAL_FLAG | S2_MSG_EXTHDR_TYPE_SN,
      ts.frame[5]);

  /*Make ctx1 hear the MSG,SN*/
  S2_application_command_handler(ctx2, &ts.last_trans, ts.frame, ts.frame_len);

  /*Checke that Ctx2 did hear the frame*/
  TEST_ASSERT_EQUAL(sizeof(hello), ts.rx_frame_len);
  TEST_ASSERT_EQUAL_STRING_LEN(hello, ts.rx_frame, sizeof(hello));

  /*Verify that the callback only comes after the timeout */
  TEST_ASSERT_NOT_EQUAL(S2_TRANSMIT_COMPLETE_OK, ts.s2_send_status);
  S2_timeout_notify(ctx1);
  TEST_ASSERT_EQUAL(S2_TRANSMIT_COMPLETE_OK, ts.s2_send_status);
}


/**
 * Check that we receive S2_TRANSMIT_COMPLETE_FIAL if we are sending with verify,
 * if the receiving controller is not able to decrypt, and sends a NONCE_REPORT -
 * after both first and second attempt.
 */
void test_sd_verify_tx_fail() {
  const char hello[] = "HelloWorld Second Frame";
  uint8_t bad_frame[1500];
  test_s2_send_data();
  /* ----------- now send second frame with SPAN established --------- *
   */

  ts.fcount = 0; //Reset frame count
  ts.rx_frame_len = 0;
  ts.s2_send_status = S2_TRANSMIT_COMPLETE_NO_ACK;

  conn12.tx_options = S2_TXOPTION_VERIFY_DELIVERY;

  S2_send_data(ctx1, &conn12, (uint8_t*) hello, sizeof(hello));
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  TEST_ASSERT_EQUAL(0, ts.frame[3]); //Verify that there is no SN included

  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);

  /*make a bad frame */
  memcpy(bad_frame,ts.frame,ts.frame_len);
  bad_frame[ts.frame_len-1]++; // *this* will invalidate the MAC

  /*Send the BAD frame to ctx2*/
  S2_application_command_handler(ctx2, &ts.last_trans, bad_frame, ts.frame_len);

  /*We expect ctx2 to send a nonce report */
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT, ts.frame[1]);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT_PROPERTIES1_SOS_BIT_MASK, ts.frame[3]);

  /*Make ctx1 hear the nocne report*/
  S2_application_command_handler(ctx1, &ts.last_trans, ts.frame, ts.frame_len);

  /* ctx1 should now send a MSG,SN */
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION_PROPERTIES1_EXTENSION_BIT_MASK, ts.frame[3]);
  TEST_ASSERT_EQUAL(18, ts.frame[4]);
  TEST_ASSERT_EQUAL(S2_MSG_EXTHDR_CRITICAL_FLAG | S2_MSG_EXTHDR_TYPE_SN,
      ts.frame[5]);

  /*make a bad frame */
  memcpy(bad_frame,ts.frame,ts.frame_len);
  bad_frame[ts.frame_len-1]++; // *this* will invalidate the MAC

  /*Make ctx1 hear the bad frame*/
  S2_application_command_handler(ctx2, &ts.last_trans, bad_frame, ts.frame_len);

  /*We expect ctx2 to send a nonce report */
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT, ts.frame[1]);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT_PROPERTIES1_SOS_BIT_MASK, ts.frame[3]);


  /*Verify that the callback only comes after the second frame */
  TEST_ASSERT_NOT_EQUAL(S2_TRANSMIT_COMPLETE_FAIL, ts.s2_send_status);

  /*Make ctx1 hear the nocne report*/
  S2_application_command_handler(ctx1, &ts.last_trans, ts.frame, ts.frame_len);

  TEST_ASSERT_EQUAL(S2_TRANSMIT_COMPLETE_FAIL, ts.s2_send_status);
}



/**
 * Check that we receive S2_TRANSMIT_COMPLETE_VERIFIED if we are sending with verify,
 * if the receiving controller is able to decrypt the message,and sends a reply
 */
void test_sd_verify_tx_verifify() {
  const char ping[] = "PING";
  const char pong[] = "PONG";

  test_s2_send_data();
  /* ----------- now send second frame with SPAN established --------- *
   */

  ts.fcount = 0; //Reset frame count
  ts.rx_frame_len = 0;
  ts.s2_send_status = S2_TRANSMIT_COMPLETE_NO_ACK;

  conn12.tx_options = S2_TXOPTION_VERIFY_DELIVERY;

  /* We just do this to make sure that the callback of ctx2 does not overwrite
   * the callback we are looking for.
    */
  conn21.tx_options = S2_TXOPTION_VERIFY_DELIVERY;

  S2_send_data(ctx1, &conn12, (uint8_t*) ping, sizeof(ping));
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  TEST_ASSERT_EQUAL(0, ts.frame[3]); //Verify that there is no SN included

  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);
  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx2, &ts.last_trans, ts.frame, ts.frame_len);

  TEST_ASSERT_EQUAL(1, ts.fcount);
  TEST_ASSERT_EQUAL(sizeof(ping), ts.rx_frame_len);
  TEST_ASSERT_EQUAL_STRING_LEN(ping, ts.rx_frame, sizeof(ping));

  S2_send_data(ctx2, &conn21, (uint8_t*) pong, sizeof(pong));
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  TEST_ASSERT_EQUAL(0, ts.frame[3]); //Verify that there is no SN included

  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);

  TEST_ASSERT_NOT_EQUAL(S2_TRANSMIT_COMPLETE_VERIFIED, ts.s2_send_status);

  /*Send the frame to ctx1*/
  S2_application_command_handler(ctx1, &ts.last_trans, ts.frame, ts.frame_len);

  TEST_ASSERT_EQUAL(2, ts.fcount);
  TEST_ASSERT_EQUAL(sizeof(pong), ts.rx_frame_len);
  TEST_ASSERT_EQUAL_STRING_LEN(pong, ts.rx_frame, sizeof(pong));

  TEST_ASSERT_EQUAL(S2_TRANSMIT_COMPLETE_VERIFIED, ts.s2_send_status);
}


/**
 * Check that we receive S2_TRANSMIT_COMPLETE_VERIFIED if we are sending with verify,
 * if the receiving controller is able to decrypt the message after the
 * second attempt, and sends a reply message
 */
void test_sd_verify_tx_verify_second_attempt() {
  const char ping[] = "PING";
  const char pong[] = "PONG";

  uint8_t bad_frame[1500];
  test_s2_send_data();
  /* ----------- now send second frame with SPAN established --------- *
   */

  ts.fcount = 0; //Reset frame count
  ts.rx_frame_len = 0;
  ts.s2_send_status = S2_TRANSMIT_COMPLETE_NO_ACK;

  conn12.tx_options = S2_TXOPTION_VERIFY_DELIVERY;
  /* We just do this to make sure that the callback of ctx2 does not overwrite
   * the callback we are looking for.
    */
  conn21.tx_options = S2_TXOPTION_VERIFY_DELIVERY;


  S2_send_data(ctx1, &conn12, (uint8_t*) ping, sizeof(ping));
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  TEST_ASSERT_EQUAL(0, ts.frame[3]); //Verify that there is no SN included

  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);

  /*make a bad frame */
  memcpy(bad_frame,ts.frame,ts.frame_len);
  bad_frame[ts.frame_len-1]++; // *this* will invalidate the MAC

  /*Send the BAD frame to ctx2*/
  S2_application_command_handler(ctx2, &ts.last_trans, bad_frame, ts.frame_len);

  /*We expect ctx2 to send a nonce report */
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT, ts.frame[1]);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT_PROPERTIES1_SOS_BIT_MASK, ts.frame[3]);

  /*Make ctx1 hear the nocne report*/
  S2_application_command_handler(ctx1, &ts.last_trans, ts.frame, ts.frame_len);

  /* ctx1 should now send a MSG,SN */
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION_PROPERTIES1_EXTENSION_BIT_MASK, ts.frame[3]);
  TEST_ASSERT_EQUAL(18, ts.frame[4]);
  TEST_ASSERT_EQUAL(S2_MSG_EXTHDR_CRITICAL_FLAG | S2_MSG_EXTHDR_TYPE_SN,
      ts.frame[5]);

  /*Make ctx1 hear the MSG,SN*/
  S2_application_command_handler(ctx2, &ts.last_trans, ts.frame, ts.frame_len);

  /*Checke that Ctx2 did hear the frame*/
  TEST_ASSERT_EQUAL(sizeof(ping), ts.rx_frame_len);
  TEST_ASSERT_EQUAL_STRING_LEN(ping, ts.rx_frame, sizeof(ping));

  /*Verify that the callback only comes after the reply */
  TEST_ASSERT_NOT_EQUAL(S2_TRANSMIT_COMPLETE_OK, ts.s2_send_status);

  /*Now make ctx2 send reply*/

  S2_send_data(ctx2, &conn21, (uint8_t*) pong, sizeof(pong));
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  TEST_ASSERT_EQUAL(0, ts.frame[3]); //Verify that there is no SN included

  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);

  TEST_ASSERT_NOT_EQUAL(S2_TRANSMIT_COMPLETE_VERIFIED, ts.s2_send_status);

  /*Send the frame to ctx1*/
  S2_application_command_handler(ctx1, &ts.last_trans, ts.frame, ts.frame_len);

  TEST_ASSERT_EQUAL(4, ts.fcount);
  TEST_ASSERT_EQUAL(sizeof(pong), ts.rx_frame_len);
  TEST_ASSERT_EQUAL_STRING_LEN(pong, ts.rx_frame, sizeof(pong));

  TEST_ASSERT_EQUAL(S2_TRANSMIT_COMPLETE_VERIFIED, ts.s2_send_status);
}

/**
 * Do a singlecast followup, where SPAN is ok, but node b is MOS
 */
void singlecast_followup_mos(struct S2* ctx_a, struct S2* ctx_b, s2_connection_t* conn) {
  char hello_mc[] = "Hello multicast";

  ts.fcount = 0;
  ts.s2_send_done=0;
  /* Send the single cast frame */
  S2_send_data(ctx_a, conn, (uint8_t*) hello_mc, sizeof(hello_mc));
  S2_send_frame_done_notify(ctx_a, S2_TRANSMIT_COMPLETE_OK,0x42);

  S2_application_command_handler(ctx_b, &ts.last_trans, ts.frame, ts.frame_len);
  S2_timeout_notify(ctx_b);

  /* Verify that ctx2 sends a NONCE_REPORT,MOS */
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT, ts.frame[1]);
  TEST_ASSERT_EQUAL(2, ts.frame[3]);
  TEST_ASSERT_EQUAL(4, ts.frame_len);

  S2_send_frame_done_notify(ctx_b, S2_TRANSMIT_COMPLETE_OK,0x42);

  /* Deliver NR,MOS to ctx 1*/
  S2_application_command_handler(ctx_a, &ts.last_trans, ts.frame, ts.frame_len);

  /* The CTX1 sends a MSG,MPAN */
  TEST_ASSERT_EQUAL(3, ts.fcount);
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  TEST_ASSERT_EQUAL(2, ts.frame[3]); //Verify that there is a ENC, EXT included
  TEST_ASSERT_EQUAL(4 + 19 + 8, ts.frame_len);

  S2_send_frame_done_notify(ctx_a, S2_TRANSMIT_COMPLETE_OK,0x42);
  S2_timeout_notify(ctx_a);
  TEST_ASSERT_EQUAL(1, ts.s2_send_done);
  TEST_ASSERT_EQUAL(S2_TRANSMIT_COMPLETE_OK, ts.s2_send_status);

  /* Deliver MSG,MPAN to ctx 2*/
  S2_application_command_handler(ctx_b, &ts.last_trans, ts.frame, ts.frame_len);
}

/**
 * Do a singlecast followup, where SPAN is ok, but node b is MOS, but
 * here its a supervision get, and receiver will send a report
 */
void supervision_singlecast_followup_mos(struct S2* ctx_a, struct S2* ctx_b, s2_connection_t* conn) {
  char supervision_get[] = "Supervision Get";
  char supervision_report[] = "Supervision Report";

  ts.fcount = 0;
  ts.s2_send_done=0;
  /* Send the single cast frame */
  S2_send_data(ctx_a, conn, (uint8_t*) supervision_get, sizeof(supervision_get));
  S2_send_frame_done_notify(ctx_a, S2_TRANSMIT_COMPLETE_OK,0x42);

  S2_application_command_handler(ctx_b, &ts.last_trans, ts.frame, ts.frame_len);

  /*Reply with supervison report */
  S2_send_data(ctx_b, &ts.last_trans, (uint8_t*) supervision_report, sizeof(supervision_report));
  S2_send_frame_done_notify(ctx_b, S2_TRANSMIT_COMPLETE_OK,0x42);

  /* Verify that ctx2 sends a message with the,MOS extension */
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  TEST_ASSERT_EQUAL(1, ts.frame[3]); //Verify that there is a ENC, EXT included
  TEST_ASSERT_EQUAL(2, ts.frame[4]); //length 2
  TEST_ASSERT_EQUAL(S2_MSG_EXTHDR_TYPE_MOS, ts.frame[5]); //length 2

  S2_send_frame_done_notify(ctx_b, S2_TRANSMIT_COMPLETE_OK,0x42);
  S2_application_command_handler(ctx_a, &ts.last_trans, ts.frame, ts.frame_len);

  /* The CTX1 sends a MSG,MPAN */
  TEST_ASSERT_EQUAL(3, ts.fcount);
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  TEST_ASSERT_EQUAL(2, ts.frame[3]); //Verify that there is a ENC, EXT included
  TEST_ASSERT_EQUAL(4 + 19 + 8, ts.frame_len);

  S2_send_frame_done_notify(ctx_a, S2_TRANSMIT_COMPLETE_OK,0x42);
  S2_timeout_notify(ctx_a);
  TEST_ASSERT_EQUAL(2, ts.s2_send_done); //both ctx_a and ctx_b have been sending a message
  TEST_ASSERT_EQUAL(S2_TRANSMIT_COMPLETE_OK, ts.s2_send_status);

  /* Deliver MSG,MPAN to ctx 2*/
  S2_application_command_handler(ctx_b, &ts.last_trans, ts.frame, ts.frame_len);
}

/*
 * Establish MPAN sync between ctx1 and ctx2, its ctx1 which is sending
 *
 * 2) Send multicast, verify that no frames are transmitted by the source node
 * 3) Send singlecast frame, verify that the node answers NR,MOS
 * 4) Verify that sender sends EMSG,MPAN
 *
 */
void
establish_mpan(uint8_t group)
{
  uint8_t buf[1024];
  char hello_mc[] = "Hello multicast";
  s2_connection_t mc_group =
    { 1, 0x42 };
  s2_connection_t mc_group_rx =
    { 0x42, 1, 0, S2_RXOPTION_MULTICAST };

  mc_group.class_id = 2;
  mc_group.r_node = group;
  mc_group_rx.l_node = group;

  /* Send the first multicast message */

  S2_send_data_multicast(ctx1, &mc_group, (uint8_t*) hello_mc,
      sizeof(hello_mc));

  TEST_ASSERT_EQUAL(ts.fcount, 1);
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  TEST_ASSERT_EQUAL(1, ts.frame[3]); //Verify that there is a MC group included
  TEST_ASSERT_EQUAL(3, ts.frame[4]);
  TEST_ASSERT_EQUAL(S2_MSG_EXTHDR_CRITICAL_FLAG|S2_MSG_EXTHDR_TYPE_MGRP,
      ts.frame[5]);
  TEST_ASSERT_EQUAL(mc_group.r_node, ts.frame[6]);

  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);
  TEST_ASSERT_EQUAL(1, ts.s2_send_done);
  TEST_ASSERT_EQUAL(S2_TRANSMIT_COMPLETE_OK, ts.s2_send_status);

  memcpy(buf,ts.frame,ts.frame_len);
  S2_application_command_handler(ctx2, &mc_group_rx, ts.frame, ts.frame_len);
  S2_application_command_handler(ctx3, &mc_group_rx, buf, ts.frame_len);


  /* Of ctx 2 */
  conn12.rx_options = 0;
  conn12.tx_options = S2_TXOPTION_VERIFY_DELIVERY | S2_TXOPTION_SINGLECAST_FOLLOWUP | S2_TXOPTION_FIRST_SINGLECAST_FOLLOWUP;

  supervision_singlecast_followup_mos(ctx1,ctx2,&conn12);

  conn13.rx_options = 0;
  conn13.tx_options = S2_TXOPTION_VERIFY_DELIVERY | S2_TXOPTION_SINGLECAST_FOLLOWUP;

  singlecast_followup_mos(ctx1,ctx3,&conn13);
}

/**
 * Send a multicast frame to a node
 *
 * 1) Establish span
 * 2) Send multicast to node 2 and 3, verify that no frames are transmitted by the source node
 * 3) Send singlecast frame, verify that the node answers NR,MOS
 * 4) Verify that sender sends EMSG,MPAN
 * 5) Send multicast again, verify that the message is received by destination
 * 6) Send singlecast, verify that we do not get NR,MOS
 */
void test_mulicast_send()
{
  uint8_t buf[1024];
  char hello_mc2[] = "Jello multicast 2";
  char hello_single[] = "hello singlecast";
  int i;
  int group_id = 2;
  s2_connection_t mc_group =
    { 1, group_id };
  s2_connection_t mc_group_rx =
    { group_id, 1, 0, S2_RXOPTION_MULTICAST };

  conn12.class_id=2;
  conn13.class_id=2;
  mc_group.class_id=2;
  /*Establish span*/
  //test_s2_send_data();
  my_setup();
  wrap_test_s2_send_data(ctx2,&conn12);
  wrap_test_s2_send_data(ctx3,&conn13);

  ts.fcount = 0;
  ts.s2_send_done = 0;
  ts.s2_send_status = S2_TRANSMIT_COMPLETE_NO_ACK;

  establish_mpan(group_id);


  for(i=0; i < 10; i++) {
    int j;
    printf("Round %i.....\n",i);

    /*Send 3 multicast frames */
    for(j=0; j<3; j++) {
      /*Send a new multicast and verify that ctx2 is able to decrypt the frame*/
      S2_send_data_multicast(ctx1, &mc_group, (uint8_t*) hello_mc2,
          sizeof(hello_mc2));
      S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);

      memcpy(buf,ts.frame,ts.frame_len);

      memset(ts.rx_frame,0,sizeof(ts.rx_frame));
      ts.rx_frame_len=0;
      S2_application_command_handler(ctx2, &mc_group_rx, ts.frame, ts.frame_len);
      TEST_ASSERT_EQUAL_STRING(hello_mc2, ts.rx_frame);
      TEST_ASSERT_EQUAL( ts.rx_frame_len, sizeof(hello_mc2));

      memset(ts.rx_frame,0,sizeof(ts.rx_frame));
      ts.rx_frame_len = 0;
      S2_application_command_handler(ctx3, &mc_group_rx, buf, ts.frame_len);
      TEST_ASSERT_EQUAL_STRING(hello_mc2, ts.rx_frame);
      TEST_ASSERT_EQUAL( ts.rx_frame_len, sizeof(hello_mc2));
    }


    /*Verify no NR send on next single cast */
    ts.fcount = 0;

    conn12.tx_options = S2_TXOPTION_VERIFY_DELIVERY |S2_TXOPTION_SINGLECAST_FOLLOWUP | S2_TXOPTION_FIRST_SINGLECAST_FOLLOWUP;

    memset(ts.rx_frame,0,sizeof(ts.rx_frame));
    S2_send_data(ctx1,&conn12, (uint8_t*)hello_single, sizeof(hello_single));
    S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);
    S2_application_command_handler(ctx2, &ts.last_trans, ts.frame, ts.frame_len);
    TEST_ASSERT_EQUAL_STRING(hello_single, ts.rx_frame);
    TEST_ASSERT_EQUAL(1,ts.fcount);
    S2_timeout_notify(ctx1);

    conn13.tx_options = S2_TXOPTION_VERIFY_DELIVERY | S2_TXOPTION_SINGLECAST_FOLLOWUP;

    /*Verify no NR send on next single cast */
    memset(ts.rx_frame,0,sizeof(ts.rx_frame));
    S2_send_data(ctx1,&conn13, (uint8_t*)hello_single, sizeof(hello_single));
    S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);
    S2_application_command_handler(ctx3, &ts.last_trans, ts.frame, ts.frame_len);
    TEST_ASSERT_EQUAL_STRING(hello_single, ts.rx_frame);
    TEST_ASSERT_EQUAL(2,ts.fcount);
    S2_timeout_notify(ctx1);
  }
}

/*
 * Test that we are able to MOS ans SOS sync in the same session.
 *
 */
void test_mulicast_send_mos_sos_sync()
{
  char hello_mc2[] = "Jello multicast 2";
  char hello_single[] = "hello singlecast";
  char hello_mc[] = "Hello multicast";
  int i;
  s2_connection_t mc_group =
    { 1, 0x42 };
  s2_connection_t mc_group_rx =
    { 0x42, 1, 0, S2_RXOPTION_MULTICAST };

  my_setup();

  ts.fcount = 0;
  ts.s2_send_done = 0;
  ts.s2_send_status = S2_TRANSMIT_COMPLETE_NO_ACK;

  mc_group.class_id=2;
  /* Send the first multicast message */

  S2_send_data_multicast(ctx1, &mc_group, (uint8_t*) hello_mc,
      sizeof(hello_mc));

  TEST_ASSERT_EQUAL(ts.fcount, 1);
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  TEST_ASSERT_EQUAL(1, ts.frame[3]); //Verify that there is a MC group included
  TEST_ASSERT_EQUAL(3, ts.frame[4]);
  TEST_ASSERT_EQUAL(S2_MSG_EXTHDR_CRITICAL_FLAG|S2_MSG_EXTHDR_TYPE_MGRP,
      ts.frame[5]);
  TEST_ASSERT_EQUAL(mc_group.r_node, ts.frame[6]);

  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);
  TEST_ASSERT_EQUAL(1, ts.s2_send_done);
  TEST_ASSERT_EQUAL(S2_TRANSMIT_COMPLETE_OK, ts.s2_send_status);

  S2_application_command_handler(ctx2, &mc_group_rx, ts.frame, ts.frame_len);

  conn12.rx_options = 0;
  conn12.tx_options = S2_TXOPTION_VERIFY_DELIVERY | S2_TXOPTION_FIRST_SINGLECAST_FOLLOWUP | S2_TXOPTION_SINGLECAST_FOLLOWUP;

  /* Send the single cast frame */
  S2_send_data(ctx1, &conn12, (uint8_t*) hello_mc, sizeof(hello_mc));
  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);

  S2_application_command_handler(ctx2, &ts.last_trans, ts.frame, ts.frame_len);

  /* Verify that ctx2 sends a NONCE_REPORT,SOS */
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT, ts.frame[1]);
  TEST_ASSERT_EQUAL(S2_NONCE_REPORT_SOS_FLAG, ts.frame[3]); /*SOS*/
  TEST_ASSERT_EQUAL(4+16, ts.frame_len);

  S2_send_frame_done_notify(ctx2, S2_TRANSMIT_COMPLETE_OK,0x42);

  /* Deliver NR,SOS to ctx 1*/
  S2_application_command_handler(ctx1, &ts.last_trans, ts.frame, ts.frame_len);

  /* The CTX1 sends a MSG,MPAN */
  TEST_ASSERT_EQUAL(ts.fcount, 4);
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  TEST_ASSERT_EQUAL(S2_MSG_FLAG_EXT, ts.frame[3]); //Verify that there is a non ENC EXT is included
  TEST_ASSERT_EQUAL(S2_MSG_EXTHDR_MORE_FLAG, ts.frame[5] & S2_MSG_EXTHDR_MORE_FLAG); //Verify that there is a non ENC EXT is included

  TEST_ASSERT_EQUAL(4 +3+ 18+ sizeof(hello_mc) + 8, ts.frame_len); //SH=4,  MC GRP=3,  SPAN=18, PAYLOAD , AUTH=8

  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);

  /* Deliver MSG, to ctx 2*/
  S2_application_command_handler(ctx2, &ts.last_trans, ts.frame, ts.frame_len);
  S2_timeout_notify(ctx2);

  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT, ts.frame[1]);
  TEST_ASSERT_EQUAL(S2_NONCE_REPORT_MOS_FLAG, ts.frame[3]); /*MOS*/

  /* Deliver NR, MOS to ctx 1*/
  S2_application_command_handler(ctx1, &ts.last_trans, ts.frame, ts.frame_len);
  S2_timeout_notify(ctx2);

  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  TEST_ASSERT_EQUAL(S2_MSG_FLAG_SEXT, ts.frame[3]); //Verify that there is a ENC, SEXT included
  TEST_ASSERT_EQUAL(4 + 19 + 8 , ts.frame_len); //SH, MPAN, AUTH

  /* Deliver ENC, MPAN to ctx 2*/
  S2_application_command_handler(ctx2, &ts.last_trans, ts.frame, ts.frame_len);

  TEST_ASSERT_EQUAL(1, ts.s2_send_done);
  S2_timeout_notify(ctx1);
  TEST_ASSERT_EQUAL(2, ts.s2_send_done);
  TEST_ASSERT_EQUAL(S2_TRANSMIT_COMPLETE_OK, ts.s2_send_status);

  ts.fcount = 0;
  ts.s2_send_done = 0;
  for(i =0; i < 10; i++) {

    /*Send a new multicast and verify that ctx2 is able to decrypt the frame*/
    S2_send_data_multicast(ctx1, &mc_group, (uint8_t*) hello_mc2,
        sizeof(hello_mc2));
    S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);

    S2_application_command_handler(ctx2, &mc_group_rx, ts.frame, ts.frame_len);
    TEST_ASSERT_EQUAL_STRING(hello_mc2, ts.rx_frame);

    conn12.tx_options = S2_TXOPTION_VERIFY_DELIVERY | S2_TXOPTION_FIRST_SINGLECAST_FOLLOWUP | S2_TXOPTION_SINGLECAST_FOLLOWUP;

    /*Verify no NR send on next single cast */
    S2_send_data(ctx1,&conn12, (uint8_t*)hello_single, sizeof(hello_single));
    S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);
    S2_application_command_handler(ctx2, &ts.last_trans, ts.frame, ts.frame_len);

    TEST_ASSERT_EQUAL(2*i+1,ts.s2_send_done);
    S2_timeout_notify(ctx1);
    TEST_ASSERT_EQUAL(2*i+2,ts.s2_send_done);
    TEST_ASSERT_EQUAL(S2_TRANSMIT_COMPLETE_OK,ts.s2_send_status);

    TEST_ASSERT_EQUAL_STRING(hello_single, ts.rx_frame);
    TEST_ASSERT_EQUAL((i+1)*2,ts.fcount);
  }
}


/**
 * Send a frame with sec group 1, verify that it goes just like group 0
 */
void test_s2_send_data_second_group()
{

  const char hello2[] = "HelloWorld Second Frame";

  my_setup();

  conn12.class_id = 0;
  S2_send_data(ctx1, &conn12, (uint8_t*) hello, sizeof(hello));

  TEST_ASSERT_EQUAL_STRING_LEN(nonce_get, ts.frame, 2);
  TEST_ASSERT_EQUAL(3, ts.frame_len);
  TEST_ASSERT_EQUAL(1, ts.fcount);
  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);

  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx2, &ts.last_trans, ts.frame, ts.frame_len);
  /*Expect that we get a nonce report*/
  TEST_ASSERT_EQUAL(2, ts.fcount);
  TEST_ASSERT_EQUAL(20, ts.frame_len);
  TEST_ASSERT_EQUAL_STRING_LEN(nonce_report, ts.frame, 2);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT_PROPERTIES1_SOS_BIT_MASK, ts.frame[3]);

  S2_send_frame_done_notify(ctx2, S2_TRANSMIT_COMPLETE_OK,0x42);

  /*Send the frame to ctx1*/
  S2_application_command_handler(ctx1, &ts.last_trans, ts.frame, ts.frame_len);
  TEST_ASSERT_EQUAL(3, ts.fcount);

  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  //TEST_ASSERT_EQUAL(ts.frame[2],COMMAND_CLASS_SECURITY_2); //seq
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION_PROPERTIES1_EXTENSION_BIT_MASK, ts.frame[3]);
  TEST_ASSERT_EQUAL(18, ts.frame[4]);
  TEST_ASSERT_EQUAL(S2_MSG_EXTHDR_CRITICAL_FLAG | S2_MSG_EXTHDR_TYPE_SN,
      ts.frame[5]);

  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);

  TEST_ASSERT_EQUAL(1, ts.s2_send_done); //Check we got a send done event
  TEST_ASSERT_EQUAL(S2_TRANSMIT_COMPLETE_OK, ts.s2_send_status); //Check we got a send done event

  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx2, &ts.last_trans, ts.frame, ts.frame_len);
  TEST_ASSERT_EQUAL(sizeof(hello), ts.rx_frame_len);
  TEST_ASSERT_EQUAL_STRING_LEN(hello, ts.rx_frame, sizeof(hello));


  /*Now verify that we can send single frames*/

  /* ----------- now send second frame with SPAN established --------- *
   */

  ts.fcount = 0; //Reset frame count
  ts.rx_frame_len = 0;

  S2_send_data(ctx1, &conn12, (uint8_t*) hello2, sizeof(hello2));
  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  TEST_ASSERT_EQUAL(0, ts.frame[3]); //Verify that there is no SN included
  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);
  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx2, &ts.last_trans, ts.frame, ts.frame_len);

  TEST_ASSERT_EQUAL(1, ts.fcount);
  TEST_ASSERT_EQUAL(sizeof(hello2), ts.rx_frame_len);
  TEST_ASSERT_EQUAL_STRING_LEN(hello2, ts.rx_frame, sizeof(hello2));


  /*Now change group again this should look exactly like the first transmission*/
  ts.fcount = 0; //Reset frame count
  ts.rx_frame_len = 0;
  ts.s2_send_done =0;

  conn21.class_id = 2;
  S2_send_data(ctx2, &conn21, (uint8_t*) hello, sizeof(hello));

  TEST_ASSERT_EQUAL_STRING_LEN(nonce_get, ts.frame, 2);
  TEST_ASSERT_EQUAL(3, ts.frame_len);
  TEST_ASSERT_EQUAL(1, ts.fcount);
  S2_send_frame_done_notify(ctx2, S2_TRANSMIT_COMPLETE_OK,0x42);

  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx1, &ts.last_trans, ts.frame, ts.frame_len);
  /*Expect that we get a nonce report*/
  TEST_ASSERT_EQUAL(2, ts.fcount);
  TEST_ASSERT_EQUAL(20, ts.frame_len);
  TEST_ASSERT_EQUAL_STRING_LEN(nonce_report, ts.frame, 2);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT_PROPERTIES1_SOS_BIT_MASK, ts.frame[3]);

  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);

  /*Send the frame to ctx1*/
  S2_application_command_handler(ctx2, &ts.last_trans, ts.frame, ts.frame_len);
  TEST_ASSERT_EQUAL(3, ts.fcount);

  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  //TEST_ASSERT_EQUAL(ts.frame[2],COMMAND_CLASS_SECURITY_2); //seq
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION_PROPERTIES1_EXTENSION_BIT_MASK, ts.frame[3]);
  TEST_ASSERT_EQUAL(18, ts.frame[4]);
  TEST_ASSERT_EQUAL(S2_MSG_EXTHDR_CRITICAL_FLAG | S2_MSG_EXTHDR_TYPE_SN,
      ts.frame[5]);

  S2_send_frame_done_notify(ctx2, S2_TRANSMIT_COMPLETE_OK,0x42);

  TEST_ASSERT_EQUAL(1, ts.s2_send_done); //Check we got a send done event
  TEST_ASSERT_EQUAL(S2_TRANSMIT_COMPLETE_OK, ts.s2_send_status); //Check we got a send done event

  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx1, &ts.last_trans, ts.frame, ts.frame_len);
  TEST_ASSERT_EQUAL(sizeof(hello), ts.rx_frame_len);
  TEST_ASSERT_EQUAL_STRING_LEN(hello, ts.rx_frame, sizeof(hello));

}




/**
 * Check that the sequence number check in verify delivery works
 */
void test_s2_verify_delivery_seq_no_check()
{
  my_setup();
  uint8_t dup_nr[64];
  struct S2* ctx_dest = ctx2;
  s2_connection_t* dst = &conn12;
  s2_connection_t nr_conn;
  ts.fcount = 0;
  ts.s2_send_done =0;

  dst->tx_options = S2_TXOPTION_VERIFY_DELIVERY;

  S2_send_data(ctx1, dst, (uint8_t*) hello, sizeof(hello));

  TEST_ASSERT_EQUAL_STRING_LEN(nonce_get, ts.frame, 2);
  TEST_ASSERT_EQUAL(3, ts.frame_len);
  TEST_ASSERT_EQUAL(1, ts.fcount);
  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);

  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx_dest, &ts.last_trans, ts.frame, ts.frame_len);
  /*Expect that we get a nonce report*/
  TEST_ASSERT_EQUAL(2, ts.fcount);
  TEST_ASSERT_EQUAL(20, ts.frame_len);
  TEST_ASSERT_EQUAL_STRING_LEN(nonce_report, ts.frame, 2);
  TEST_ASSERT_EQUAL(SECURITY_2_NONCE_REPORT_PROPERTIES1_SOS_BIT_MASK, ts.frame[3]);

  /*Remember the nonce report */
  memcpy(dup_nr,ts.frame,ts.frame_len);
  nr_conn= ts.last_trans;
  S2_send_frame_done_notify(ctx_dest, S2_TRANSMIT_COMPLETE_OK,0x42);

  /*Send the frame to ctx1*/
  S2_application_command_handler(ctx1, &ts.last_trans, ts.frame, ts.frame_len);
  TEST_ASSERT_EQUAL(3, ts.fcount);

  TEST_ASSERT_EQUAL(COMMAND_CLASS_SECURITY_2, ts.frame[0]);
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION, ts.frame[1]);
  //TEST_ASSERT_EQUAL(ts.frame[2],COMMAND_CLASS_SECURITY_2); //seq
  TEST_ASSERT_EQUAL(SECURITY_2_MESSAGE_ENCAPSULATION_PROPERTIES1_EXTENSION_BIT_MASK, ts.frame[3]);
  TEST_ASSERT_EQUAL(18, ts.frame[4]);
  TEST_ASSERT_EQUAL(S2_MSG_EXTHDR_CRITICAL_FLAG | S2_MSG_EXTHDR_TYPE_SN,
      ts.frame[5]);

  S2_send_frame_done_notify(ctx1, S2_TRANSMIT_COMPLETE_OK,0x42);


  /*Send the frame to ctx2*/
  S2_application_command_handler(ctx_dest, &ts.last_trans, ts.frame, ts.frame_len);
  TEST_ASSERT_EQUAL(sizeof(hello), ts.rx_frame_len);
  TEST_ASSERT_EQUAL_STRING_LEN(hello, ts.rx_frame, sizeof(hello));

  /*Send retransmitted nonce report to ctx1 */
  S2_application_command_handler(ctx1, &nr_conn, dup_nr, 20);

  S2_timeout_notify(ctx1);

  TEST_ASSERT_EQUAL(1, ts.s2_send_done); //Check we got a send done event
  TEST_ASSERT_EQUAL(S2_TRANSMIT_COMPLETE_OK, ts.s2_send_status); //Check we got a send done event

}


/* Stub function */
uint8_t s2_inclusion_set_timeout(struct S2* ctxt, uint32_t interval)
{
#ifndef SINGLE_CONTEXT
  UNUSED(ctxt);
#endif
  UNUSED(interval);
  return 0;
}

void s2_inclusion_stop_timeout(void)
{
}

void keystore_public_key_read(uint8_t *buf)
{
  UNUSED(buf);
}

void keystore_private_key_read(uint8_t *buf)
{
  UNUSED(buf);
}

bool keystore_network_key_read(uint8_t keyclass, uint8_t *buf)
{
  UNUSED(keyclass);
  UNUSED(buf);
  return true;
}

void keystore_network_key_write(uint8_t keyclass, uint8_t *keybuf)
{
  UNUSED(keyclass);
  UNUSED(keybuf);
}

void keystore_network_key_clear(uint8_t keyclass)
{
  UNUSED(keyclass);
}

void S2_dbg_printf(const char *format, ...)
{
    va_list argptr;

    va_start(argptr, format);
    vfprintf(stdout, format, argptr);
    va_end(argptr);

}
