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
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "kderiv.h"
#include <unity.h>

void print_array(const char *name, uint8_t *arr, int len)
{
   int i;

   printf("%s", name);
   for ( i = 0; i < len; i++) {
       if (arr[i] <= 0xf)
           printf("0x0%x ", arr[i]);
       else
           printf("0x%x ", arr[i]);
   }
   printf("\n");
}

void test_key_derivation(void)
{
    uint8_t network_key[16]= {0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f};
    uint8_t pseudo_random_keymat_output[16]= {0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f};
    uint8_t ccm_key[16];
    uint8_t mpan_key[16];
    uint8_t pers_string[32];
    /* These samples are generated from the Python cmac code */
    uint8_t auth_tag[64] = {0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f};
    uint8_t ecdh_share_secret[32] = {0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47};

    /* following vectors are created by python code inside test/kderiv_python/generate*.py */
    /* test/kderiv_python/generate_networkkey_expand.py */ 
    uint8_t py_ccm_key[16] = { 0xaf,0x7f,0x0a,0x1e,0xfc,0xfe,0x8f,0x4b,0x1f,0xd2,0x84,0x5d,0xd2,0x85,0x6f,0x7d};
    uint8_t py_pers_string[32] = {0xcc,0xf8,0xb4,0x2a,0x4f,0x70,0x21,0x76,0xac,0x2b,0x91,0x94,0xdb,0xbd,0xb8,0x2c,0x47,0x43,0x02,0xff,0x12,0xe1,0xe8,0x26,0x64,0x22,0xf3,0xac,0x44,0x89,0x4b,0x87};
    // TODO: Update mpan key with real test vector
    uint8_t py_mpan_key[16] = {0xb8, 0x9d, 0xb1, 0x54, 0x3f, 0xd2, 0x82, 0x0e, 0xa9, 0x79, 0xc9, 0x6a, 0x30, 0x5a, 0x23, 0x0b};
        //0xaf,0x7f,0x0a,0x1e,0xfc,0xfe,0x8f,0x4b,0x1f,0xd2,0x84,0x5d,0xd2,0x85,0x6f,0x7d};
    /* test/kderiv_python/generate_tempkey_expand.py */ 
    uint8_t py_temp_ccm_key[16] = {0x47, 0xda, 0x9d, 0x15, 0x39, 0xec, 0x73, 0xe5, 0xd2, 0xa0, 0xf7, 0x37, 0xdb, 0xf2, 0x9b, 0x33};
    uint8_t py_temp_pers_string[32] = {0x94, 0xa5, 0x3e, 0xb9, 0xa1, 0x2d, 0x7c, 0xd9, 0x49, 0xea, 0x70, 0xff, 0xb8, 0xa6, 0xe7, 0x1e, 0xa6, 0xf7, 0x9c, 0xa7, 0xa0, 0x14, 0x5a, 0x08, 0xba, 0xd5, 0x82, 0xae, 0xb0, 0x9b, 0xd2, 0x8b};
    // TODO: Update py_temp_mpan_key with real test vector
    uint8_t py_temp_mpan_key[16] = {0xc2, 0x8e, 0x57, 0x5f, 0x24, 0x6f, 0x59, 0xa4, 0xa5, 0x85, 0x0f, 0x20, 0x66, 0xf1, 0x06, 0x2b};
    /* test/kderiv_python/generate_tempkey_extract.py */ 
    uint8_t py_pseudo_random_keymat_output[16] = {0x74,0x77,0x8f,0x89,0xd4,0xd8,0x8c,0x6e,0x46,0x59,0x63,0x4a,0x88,0xea,0x98,0x7a};

    networkkey_expand(network_key, ccm_key, pers_string, mpan_key);

    //print_array("ccm_key  : ", ccm_key, 16);
    //print_array("pers_string   : ", pers_string, 32);
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(py_ccm_key, ccm_key, 16, "ERROR: ccm_key is wrong");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(py_pers_string, pers_string, 32, "ERROR: pers_string is wrong");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(py_mpan_key, mpan_key, 16, "ERROR: mpan_key is wrong");

    tempkey_extract(ecdh_share_secret, auth_tag, pseudo_random_keymat_output);
    //print_array("pseudo_random_keymat_output: ", pseudo_random_keymat_output, 16);
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(py_pseudo_random_keymat_output, pseudo_random_keymat_output, 16, "ERROR: pseudo_random_keymat_output is wrong");

    tempkey_expand(pseudo_random_keymat_output, ccm_key, pers_string, mpan_key);
    //print_array("ccm_key  : ", ccm_key, 16);
    //print_array("pers_string   : ", pers_string, 32);
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(py_temp_ccm_key, ccm_key, 16, "ERROR: temp ccm_key is wrong");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(py_temp_pers_string, pers_string, 32, "ERROR: pers_string is wrong");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(py_temp_mpan_key, mpan_key, 16, "ERROR: temp_mpan_key is wrong");
}
