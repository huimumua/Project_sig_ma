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
 * aes_cmac_unit_test.c
 *
 *  Created on: 25/06/2015
 *      Author: COlsen
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "unity.h"
#include <curve25519.h>
#include <wc_util.h>
#include <bigint.h>

#ifdef __C51__
/* Local stub implementation of ZW_WatchDogKick() when compiling for C51. */
void ZW_WatchDogKick()
{
  // Do nothing. Stub implementation.
}
#endif // __C51__

#ifndef NULL
#define NULL   ((void *) 0)
#endif

#define KEY_SIZE (32) // In bytes

static const uint8_t alice_secret_key[KEY_SIZE] = {
        0x77,0x07,0x6d,0x0a,0x73,0x18,0xa5,0x7d
        ,0x3c,0x16,0xc1,0x72,0x51,0xb2,0x66,0x45
        ,0xdf,0x4c,0x2f,0x87,0xeb,0xc0,0x99,0x2a
        ,0xb1,0x77,0xfb,0xa5,0x1d,0xb9,0x2c,0x2a
};

/*static const uint8_t bob_secret_key[KEY_SIZE] = {
        0x5d,0xab,0x08,0x7e,0x62,0x4a,0x8a,0x4b
        ,0x79,0xe1,0x7f,0x8b,0x83,0x80,0x0e,0xe6
        ,0x6f,0x3b,0xb1,0x29,0x26,0x18,0xb6,0xfd
        ,0x1c,0x2f,0x8b,0x27,0xff,0x88,0xe0,0xeb
};*/

static uint8_t alice_public_key[KEY_SIZE];
#ifdef NOT_USED
static uint8_t bob_public_key[KEY_SIZE];
#endif

void test_alice_calculation_of_public_key(void)
{
  const uint8_t expected_alice_public_key[KEY_SIZE] = {
          0x85,0x20,0xf0,0x09,0x89,0x30,0xa7,0x54
          ,0x74,0x8b,0x7d,0xdc,0xb4,0x3e,0xf7,0x5a
          ,0x0d,0xbf,0x3a,0x0d,0x26,0x38,0x1a,0xf4
          ,0xeb,0xa4,0xa9,0x8e,0xaa,0x9b,0x4e,0x6a
  };

  crypto_scalarmult_curve25519_base(alice_public_key, alice_secret_key);

  UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_alice_public_key, alice_public_key, KEY_SIZE, __LINE__, "");
}

void test_bigint_calc(void)
{
  uint32_t i = 0x12345678;

  const uint8_t some_data1[8] = {
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12
  };
  const uint8_t some_data2[8] = {
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12
  };

  uint8_t result[16];

  memset(result, 0x00, sizeof(result));

  bigint_mul(result, some_data1, some_data2, 8);

  print32(result);

  print_txt("------------------------------------");

  if ( *(char *)&i == 0x12 )
  {
    print_txt("Big endian\n");
//    ZW_DEBUG_MYPRODUCT_SEND_STR("BIG endian.");
  }
  else if ( *(char *)&i == 0x78 )
  {
    print_txt("Little endian\n");
//    ZW_DEBUG_MYPRODUCT_SEND_STR("LITTLE endian.");
  }
}

#ifdef NOT_USED

//void test_bob_calculation_of_public_key(void)
{
  const uint8_t expected_bob_public_key[KEY_SIZE] = {
          0xde,0x9e,0xdb,0x7d,0x7b,0x7d,0xc1,0xb4
          ,0xd3,0x5b,0x61,0xc2,0xec,0xe4,0x35,0x37
          ,0x3f,0x83,0x43,0xc8,0x5b,0x78,0x67,0x4d
          ,0xad,0xfc,0x7e,0x14,0x6f,0x88,0x2b,0x4f
  };

  crypto_scalarmult_curve25519_base(bob_public_key, bob_secret_key);

  UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_bob_public_key, bob_public_key, KEY_SIZE, __LINE__, "");
}

//void test_alice_bob_shared_secret(void)
{
  const uint8_t expected_shared_secret[KEY_SIZE] = {
          0x4a,0x5d,0x9d,0x5b,0xa4,0xce,0x2d,0xe1
          ,0x72,0x8e,0x3b,0xf4,0x80,0x35,0x0f,0x25
          ,0xe0,0x7e,0x21,0xc9,0x47,0xd1,0x9e,0x33
          ,0x76,0xf0,0x9b,0x3c,0x1e,0x16,0x17,0x42
  };
  uint8_t k[KEY_SIZE];

  memset(k, 0x00, sizeof(k));

  crypto_scalarmult_curve25519(
          k,
          alice_secret_key,
          bob_public_key);

  UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_shared_secret, k, KEY_SIZE, __LINE__, "");

  memset(k, 0x00, sizeof(k));

  crypto_scalarmult_curve25519(
          k,
          bob_secret_key,
          alice_public_key);

  UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_shared_secret, k, KEY_SIZE, __LINE__, "");
}

//void test_10_shared_secrets(void)
{
  uint8_t person1_secret_key[KEY_SIZE];
  uint8_t person1_public_key[KEY_SIZE];
  uint8_t person1_shared_key_calc[KEY_SIZE];

  uint8_t person2_secret_key[KEY_SIZE];
  uint8_t person2_public_key[KEY_SIZE];
  uint8_t person2_shared_key_calc[KEY_SIZE];

  uint16_t count;
  uint8_t key_count;

  for (count = 0; count < 10; count++)
  {
    for (key_count = 0; key_count < KEY_SIZE; key_count++)
    {
      person1_secret_key[key_count] = (uint8_t)(rand() & 0xFF);
      person2_secret_key[key_count] = (uint8_t)(rand() & 0xFF);
    }
    crypto_scalarmult_curve25519_base(person1_public_key, person1_secret_key);
    crypto_scalarmult_curve25519_base(person2_public_key, person2_secret_key);

    crypto_scalarmult_curve25519(
            person1_shared_key_calc,
            person1_secret_key,
            person2_public_key);

    crypto_scalarmult_curve25519(
            person2_shared_key_calc,
            person2_secret_key,
            person1_public_key);

    UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(person1_shared_key_calc, person2_shared_key_calc, KEY_SIZE, __LINE__, "");

    memset(person1_shared_key_calc, 0x00, KEY_SIZE);
    memset(person2_shared_key_calc, 0x01, KEY_SIZE);
  }
}

#endif
