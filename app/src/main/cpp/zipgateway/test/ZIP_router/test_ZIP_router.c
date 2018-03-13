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
 * Test of a tiny part of ZIP_Router.c. In particular, test the detection of
 * Wake Up Notifications inside multi cmd encapsulations.
 */
/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/
#include "test_helpers.h"

#include "../src/ZIP_Router.c"

/****************************************************************************/
/*                      PRIVATE TYPES and DEFINITIONS                       */
/****************************************************************************/

/****************************************************************************/
/*                              EXPORTED DATA                               */
/****************************************************************************/

/****************************************************************************/
/*                              PRIVATE DATA                                */
/****************************************************************************/

/****************************************************************************/
/*                               MOCKS FUNCTIONS                            */
/****************************************************************************/
static struct {
  uint8_t node;
  uint8_t is_broadcast;
} mb_wakeup_event_args = {0};

void
mb_wakeup_event(uint8_t node, uint8_t is_broadcast)
{
  mb_wakeup_event_args.node = node;
  mb_wakeup_event_args.is_broadcast = is_broadcast;
}

security_scheme_t
highest_scheme(uint8_t scheme_mask)
{
  return 0;
}

uint8_t GetCacheEntryFlag(uint8_t nodeid)
{
  return 0;
}
/****************************************************************************/
/*                              TEST FUNCTIONS                              */
/****************************************************************************/

static void init_test()
{
  memset(&mb_wakeup_event_args, 0, sizeof mb_wakeup_event_args);
}

void test_find_WUN_in_multi()
{
  uint8_t cmd1[] = {COMMAND_CLASS_MULTI_CMD,
    MULTI_CMD_ENCAP,
    255, /* no of commands */
    3,
    1,2,3,
    5,
    11,12,13,14,15,
    2,
    0x84, /* COMMAND_CLASS_WAKE_UP */
    0x07, /* WAKE_UP_NOTIFICATION */
  };

  ts_param_t tsp = {42, 0, 0, 0, 0, 0, 0};
  cfg.mb_conf_mode = ENABLE_MAILBOX_SERVICE;
  find_WUN_in_multi(&tsp, (ZW_APPLICATION_TX_BUFFER *)cmd1, sizeof cmd1);
  check_true(mb_wakeup_event_args.node == 42, "WUN not detected #1");

  init_test();

  uint8_t cmd2[] = {COMMAND_CLASS_MULTI_CMD,
    MULTI_CMD_ENCAP,
    3, /* no of commands */
    2,
    0x84,0,
    2,
    0x84,1,
    2,
    0x84, /* COMMAND_CLASS_WAKE_UP */
    0x06,
  };

  find_WUN_in_multi(&tsp, (ZW_APPLICATION_TX_BUFFER *)cmd2, sizeof cmd2);
  check_true(mb_wakeup_event_args.node == 0, "WUN not detected #2");

  init_test();

  uint8_t cmd3[] = {COMMAND_CLASS_MULTI_CMD,
    MULTI_CMD_ENCAP,
    3, /* no of commands */
    2,
    0x84,7, /* WAKE_UP_NOTIFICATION */
    2,
    0x84,7, /* WAKE_UP_NOTIFICATION */
    2,
    0x84, /* COMMAND_CLASS_WAKE_UP */
    0x07,
  };
  find_WUN_in_multi(&tsp, (ZW_APPLICATION_TX_BUFFER *)cmd3, sizeof cmd3);
  check_true(mb_wakeup_event_args.node == 42, "WUN not detected #2");

  init_test();

  /* Check for invalid no of commands */
  cmd1[2] = 255;
  find_WUN_in_multi(&tsp, (ZW_APPLICATION_TX_BUFFER *)cmd1, sizeof cmd1);
  check_true(mb_wakeup_event_args.node == 42, "WUN not detected #4");
}

int main()
{
   test_find_WUN_in_multi();

   if (numErrs) {
       test_print(0, "\nFAILED, %d errors\n", numErrs);
   } else {
       test_print(0, "\nPASSED\n");
   }
   return numErrs;
}
