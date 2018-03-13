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
 * mock_control.c
 *
 *  Created on: Aug 14, 2015
 *      Author: trasmussen
 */

#include "mock_control.h"
#include <string.h>
#include "unity.h"

#define MOCK_CALL_DB_SIZE  1024 /**< Size of the mock call database. */
#define FAKE_CALL_DB_SIZE  255  /**< Size of the fake call database. */
#define STUB_CALL_DB_SIZE  255  /**< Size of the stub call database. */
#define MAX_MESSAGE_LENGTH 255  /**< Size of error message length, increase value if long messages are needed. */

const mock_t default_entry = {
  .p_func_name      = "default",
  .expect_arg       = {{0}, {0}, {0}, {0}},
  .actual_arg       = {{0}, {0}, {0}, {0}},
  .compare_rule_arg = {COMPARE_STRICT, COMPARE_STRICT, COMPARE_STRICT, COMPARE_STRICT},
  .error_code       = {0},
  .return_code      = {0},
  .executed         = false
};

static mock_t   mock_call_db[MOCK_CALL_DB_SIZE];
static uint32_t mock_db_idx;

static stub_t   stub_call_db[STUB_CALL_DB_SIZE];
static uint32_t stub_db_idx;

static fake_t   fake_call_db[FAKE_CALL_DB_SIZE];
static uint32_t fake_db_idx;

uint32_t g_mock_index;

void mock_call_expect_ex(uint32_t line_number, char * p_func_name, mock_t ** pp_mock)
{
  // Reset the entry before usage.
  mock_call_db[mock_db_idx] = default_entry;

  // Assign entry and increment.
  *pp_mock                = &mock_call_db[mock_db_idx];
  (*pp_mock)->p_func_name = p_func_name;
  (*pp_mock)->executed    = false;
  (*pp_mock)->line_number = line_number;
  mock_db_idx++;
}

void mock_calls_clear(void)
{
  mock_db_idx = 0;
  stub_db_idx = 0;
  fake_db_idx = 0;
}

void mock_calls_verify(void)
{
  bool     failed = false;
  uint16_t i;
  for (i = 0; i < mock_db_idx; i++)
  {
    if (false == mock_call_db[i].executed)
    {
      char error_msg[MAX_MESSAGE_LENGTH] = "- Expected mock call never occurred: ";
      sprintf(error_msg, "- Expected mock call never occurred: %s(...) at %s:%lu - ", mock_call_db[i].p_func_name, Unity.CurrentTestName, (long unsigned)mock_call_db[i].line_number);
      UnityPrint(error_msg);

      failed = true;
    }
    mock_call_db[i] = default_entry;
  }
  mock_db_idx = 0;
  stub_db_idx = 0;
  fake_db_idx = 0;

  if (failed)
  {
    TEST_FAIL_MESSAGE("Expected mock calls never occurred, see list for details.");
  }
}

bool mock_call_find(char const * const p_function_name, mock_t ** pp_mock)
{
  uint32_t i;
  for (i = 0; i < mock_db_idx; i++)
  {
    if ((false == mock_call_db[i].executed) &&
        strcmp(mock_call_db[i].p_func_name, p_function_name) == 0)
    {
      *pp_mock = &mock_call_db[i];
      g_mock_index = i;
      (*pp_mock)->executed = true;
      return true;
    }
  }
  mock_db_idx = 0;
  stub_db_idx = 0;
  fake_db_idx = 0;

  static char error_msg[MAX_MESSAGE_LENGTH] = "Unexpected mock call occurred: ";
  strcat(error_msg, p_function_name);

  TEST_FAIL_MESSAGE(error_msg);
  return false;
}

bool mock_call_used_as_stub(char const * const p_file_name, char const * const p_function_name)
{
  uint32_t i;
  for (i = 0; i < stub_db_idx; i++)
  {
    if ((strcmp(stub_call_db[i].p_name, p_function_name) == 0) ||
        (strcmp(stub_call_db[i].p_name, p_file_name) == 0))
    {
      return true;
    }
  }
  return false;
}

void mock_call_use_as_stub(char * const p_name)
{
  // Assign entry and increment.
  stub_call_db[stub_db_idx++].p_name = p_name;
}

bool mock_call_used_as_fake(char const * const p_file_name, char const * const p_function_name)
{
  uint32_t i;
  for (i = 0; i < fake_db_idx; i++)
  {
    if ((strcmp(fake_call_db[i].p_name, p_function_name) == 0) ||
        (strcmp(fake_call_db[i].p_name, p_file_name) == 0))
    {
      return true;
    }
  }
  return false;
}

void mock_call_use_as_fake(char * const p_name)
{
  // Assign entry and increment.
  fake_call_db[fake_db_idx++].p_name = p_name;
}

