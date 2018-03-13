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
 * aes_cmac_unit_test.c
 *
 *  Created on: 25/06/2015
 *      Author: COlsen
 */
#include <stdint.h>
#include <stdio.h>
#include "unity.h"
#include "../crypto/aes-cmac/aes_cmac.c"

#ifndef NULL
#define NULL   ((void *) 0)
#endif


void test_subkey_generation(void)
{
  const uint8_t key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  const uint8_t subkey1[] = {0xfb, 0xee, 0xd6, 0x18, 0x35, 0x71, 0x33, 0x66, 0x7c, 0x85, 0xe0, 0x8f, 0x72, 0x36, 0xa8, 0xde};
  const uint8_t subkey2[] = {0xf7, 0xdd, 0xac, 0x30, 0x6a, 0xe2, 0x66, 0xcc, 0xf9, 0x0b, 0xc1, 0x1e, 0xe4, 0x6d, 0x51, 0x3b};
  uint8_t out1[16];
  uint8_t out2[16];

  generate_subkey(key, out1, out2);

  UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(subkey1, out1, 16, __LINE__, "");
  UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(subkey2, out2, 16, __LINE__, "");
}

void test_cmac_ietf_rfc4493_test_vectors_example_1(void)
{
  const uint8_t key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c}; // Valid

  const uint8_t expaected_mac[] = {0xbb, 0x1d, 0x69, 0x29, 0xe9, 0x59, 0x37, 0x28, 0x7f, 0xa3, 0x7d, 0x12, 0x9b, 0x75, 0x67, 0x46};
  uint8_t calculated_mac[16];
  aes_cmac_calculate(key, NULL, 0, calculated_mac);
  UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(expaected_mac, calculated_mac, 16, __LINE__, "");
}

void test_cmac_ietf_rfc4493_test_vectors_example_2(void)
{
  const uint8_t key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c}; // Valid

  const uint8_t message[] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a};
  uint8_t calculated_mac[16];
  const uint8_t expected_mac[] = {0x07, 0x0a, 0x16, 0xb4, 0x6b, 0x4d, 0x41, 0x44, 0xf7, 0x9b, 0xdd, 0x9d, 0xd0, 0x4a, 0x28, 0x7c};
  aes_cmac_calculate(key, message, sizeof(message), calculated_mac);
  UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_mac, calculated_mac, 16, __LINE__, "");
}

void test_cmac_ietf_rfc4493_test_vectors_example_3(void)
{
  const uint8_t key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c}; // Valid

  const uint8_t message[] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
                             0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
                             0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11};
  uint8_t calculated_mac[16];
  const uint8_t expected_mac[] = {0xdf, 0xa6, 0x67, 0x47, 0xde, 0x9a, 0xe6, 0x30, 0x30, 0xca, 0x32, 0x61, 0x14, 0x97, 0xc8, 0x27};
  aes_cmac_calculate(key, message, sizeof(message), calculated_mac);
  UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_mac, calculated_mac, 16, __LINE__, "");
}

void test_cmac_ietf_rfc4493_test_vectors_example_4(void)
{
  const uint8_t key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c}; // Valid

  const uint8_t message[] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
                             0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
                             0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
                             0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10};
  uint8_t calculated_mac[16];
  const uint8_t expected_mac[] = {0x51, 0xf0, 0xbe, 0xbf, 0x7e, 0x3b, 0x9d, 0x92, 0xfc, 0x49, 0x74, 0x17, 0x79, 0x36, 0x3c, 0xfe};
  aes_cmac_calculate(key, message, sizeof(message), calculated_mac);
  UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_mac, calculated_mac, 16, __LINE__, "");
}

void test_cmac_ietf_rfc4493_test_vectors_example_4_verify(void)
{
  const uint8_t key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c}; // Valid
  const uint8_t in[] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
                  0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
                  0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
                  0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10};
  const uint8_t mac[] = {0x51, 0xf0, 0xbe, 0xbf, 0x7e, 0x3b, 0x9d, 0x92, 0xfc, 0x49, 0x74, 0x17, 0x79, 0x36, 0x3c, 0xfe};
  CMAC_VERIFY_T cmac_verify_result;

  cmac_verify_result = aes_cmac_verify(key, in, sizeof(in), mac);

  //printf("%z", sizeof(CMAC_VERIFY_T));

  UNITY_TEST_ASSERT_EQUAL_UINT8(CMAC_VALID, cmac_verify_result, __LINE__, "");
}

void test_cmac_ietf_rfc4493_test_vectors_example_4_verify_negative(void)
{
  const uint8_t key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c}; // Valid
  const uint8_t in[] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
                  0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
                  0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
                  0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10};
  const uint8_t mac[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  CMAC_VERIFY_T cmac_verify_result;

  cmac_verify_result = aes_cmac_verify(key, in, sizeof(in), mac);

  UNITY_TEST_ASSERT_EQUAL_UINT8(CMAC_INVALID, cmac_verify_result, __LINE__, "");
}

void test_cmac_xor_120(void)
{
  const uint8_t value1[] = {0xaa, 0x51, 0x58, 0xb2, 0x58, 0x99, 0x80, 0x0b, 0x61, 0xaa, 0xa0, 0x42, 0x98, 0xf8, 0x38, 0xd0};
  const uint8_t value2[] = {0x0f, 0xe7, 0x09, 0x20, 0x26, 0x45, 0x76, 0xa4, 0xc7, 0xc7, 0xc2, 0xd2, 0x9b, 0x7a, 0x46, 0xb8};
  uint8_t output[16];
  const uint8_t expected[] = {0xa5, 0xb6, 0x51, 0x92, 0x7e, 0xdc, 0xf6, 0xaf, 0xa6, 0x6d, 0x62, 0x90, 0x3, 0x82, 0x7e, 0x68};

  UNITY_TEST_ASSERT_EQUAL_UINT8(16, sizeof(value1), __LINE__, "");
  UNITY_TEST_ASSERT_EQUAL_UINT8(16, sizeof(value2), __LINE__, "");
  UNITY_TEST_ASSERT_EQUAL_UINT8(16, sizeof(expected), __LINE__, "");

  xor_128(value1, value2, output);

  UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, output, sizeof(expected), __LINE__, "");
}

void test_cmac_leftshift_onebit(void)
{
  const uint8_t value[] = {0xaa, 0x51, 0x58, 0xb2, 0x58, 0x99, 0x80, 0x0b, 0x61, 0xaa, 0xa0, 0x42, 0x98, 0xf8, 0x38, 0xd0};
  uint8_t output[16];
  const uint8_t expected[] = {0x54, 0xa2, 0xb1, 0x64, 0xb1, 0x33, 0x0, 0x16, 0xc3, 0x55, 0x40, 0x85, 0x31, 0xf0, 0x71, 0xa0};

  UNITY_TEST_ASSERT_EQUAL_UINT8(16, sizeof(value), __LINE__, "");
  UNITY_TEST_ASSERT_EQUAL_UINT8(16, sizeof(expected), __LINE__, "");

  leftshift_onebit(value, output);

  UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, output, sizeof(expected), __LINE__, "");
}


void test_cmac_padding(void)
{
  uint8_t output[16];
  const uint8_t value[] = {0x38, 0xd0};
  const uint8_t expected[] = {0x38, 0xd0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  UNITY_TEST_ASSERT_EQUAL_UINT8(16, sizeof(output), __LINE__, "");
  UNITY_TEST_ASSERT_EQUAL_UINT8(16, sizeof(expected), __LINE__, "");

  padding(value, output, sizeof(value));

  UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, output, sizeof(expected), __LINE__, "");
}
