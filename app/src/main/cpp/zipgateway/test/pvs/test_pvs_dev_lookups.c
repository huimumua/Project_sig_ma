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

#include "provisioning_list.h"
#include <string.h>
#include "test_helpers.h"

/**
\ingroup pvs_test 
* @{
*
*/

FILE *log_strm = NULL;

static uint8_t dsk5[] =       {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
static uint8_t challenge5[] = { 0,  0, 12, 13, 14, 15, 16, 17, 18, 19, 20};
static uint8_t dsk6[] =       {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 51, 62, 73, 76};
static uint8_t challenge6[] = {12,  0, 12, 13, 14, 15, 16, 17, 18, 19, 20};
static uint8_t challenge7[] = {12,  0, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22};
static uint8_t dskregular[] = {0xff,0, 12, 13, 14, 15, 16, 17, 18, 19, 20, 0xAB, 0x00, 0xFF, 0x17, 0x06};
static uint8_t challengexl[]= {12,  0, 12, 13, 14, 15, 16, 17, 18, 19, 20, 0xAB, 0x00, 0xFF, 0x17, 0x06, 0xCC, 0xC0};
static uint8_t *name3 = (uint8_t *)"Node3";
static uint8_t *location3 = (uint8_t *)"Location3";

static void test_provisioning_list_dev_match_challenge(void);

/* Test that we can match a S2 inclusion challenge with a provisioning_list DSK.
 * In this case, the first two bytes of the challenge may be zeroed out. */
static void test_provisioning_list_dev_match_challenge(void)
{
  struct provision *pvs = provisioning_list_dev_add(sizeof(dsk5), dsk5, PVS_BOOTMODE_S2);
  struct provision *pvsreg = NULL;

  test_print_suite_title(1, "Challenge lookup");

  start_case("Lookup device by challenge ---\n    Add DSK dsk5\n", log_strm);
  provisioning_list_tlv_set(pvs, PVS_TLV_TYPE_NAME, (uint8_t)strlen((char*)name3)+1, name3);
  provisioning_list_tlv_set(pvs, PVS_TLV_TYPE_LOCATION, (uint8_t)strlen((char*)location3)+1, location3);

  test_print(3, "Test with challenge 5, size %u\n", sizeof(challenge5));
  check_not_null((void*)provisioning_list_dev_match_challenge(sizeof(challenge5), challenge5),
                 "Challenge5 should match DSK5");

  check_not_null((void*)provisioning_list_dev_match_challenge(sizeof(challenge6), challenge6),
                 "Challenge6 should match DSK5");
  check_not_null((void*)provisioning_list_dev_match_challenge(11, challenge7),
                 "11 bytes from challenge7 should match DSK5");
  close_case("Lookup device by challenge ---\n    Add DSK dsk5\n");

  start_case("Challenge too long", log_strm);
  check_null((void*)provisioning_list_dev_match_challenge(sizeof(challenge7), challenge7),
             "Challenge 7 should not match DSK5 (too long)");
  check_not_null((void*)provisioning_list_dev_match_challenge(11, challengexl),
                 "11 bytes from challengexl should match DSK5");
  check_null((void*)provisioning_list_dev_match_challenge(sizeof(challengexl), challengexl),
             "16 bytes from challengexl should match DSK5");

  pvsreg = provisioning_list_dev_add(sizeof(dskregular), dskregular, PVS_BOOTMODE_S2);
  check_true(pvsreg == provisioning_list_dev_match_challenge(sizeof(challengexl), challengexl),
             "16 bytes from challengexl should match dskregular");

  pvs = provisioning_list_dev_add(sizeof(dsk6), dsk6, PVS_BOOTMODE_SMART_START);
  check_not_null((void*)provisioning_list_dev_match_challenge(sizeof(challenge5), challenge5),
                 "Challenge5 should match DSK6");
  check_not_null((void*)provisioning_list_dev_match_challenge(sizeof(challenge6), challenge6),
                 "Challenge6 should match DSK6");
  check_null((void*)provisioning_list_dev_match_challenge(sizeof(challenge7), challenge7),
             "Challenge 7 should not match DSK6 (character mismatch)");
  close_case("Challenge too long");

  start_case("Challenge too short", log_strm);
  check_null((void*)provisioning_list_dev_match_challenge(0, challenge6),
             "0 bytes from challenge6 should NOT match DSK6");
  check_null((void*)provisioning_list_dev_match_challenge(2, challenge6),
             "2 bytes from challenge6 should NOT match DSK6");
  close_case("Challenge too short");
}

int main()
{
    log_strm = test_create_log(TEST_CREATE_LOG_NAME);

    provisioning_list_init(NULL, NULL);

    provisioning_list_clear();

    test_provisioning_list_dev_match_challenge();

    close_run();
    fclose(log_strm);

    return numErrs;
}

/**
 * @}
 */
