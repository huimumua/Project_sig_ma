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
 * ZW_timer_mock.c
 *
 *  Created on: Aug 14, 2015
 *      Author: trasmussen
 */

#include "example_test_mock.h"
#include <stdint.h>
#include <string.h>
#include "mock_control.h"
#include "unity.h"

#define MOCK_FILE "example_test_mock.c"

uint32_t mock_function_simple_input(uint8_t arg0, uint16_t arg1, uint32_t arg2)
{
  mock_t * p_mock;

  MOCK_CALL_RETURN_IF_USED_AS_STUB(0x00);
  MOCK_CALL_FIND_RETURN_ON_FAILURE(p_mock, 0xFF);
  MOCK_CALL_ACTUAL(p_mock, arg0, arg1, arg2);
  MOCK_CALL_RETURN_IF_ERROR_SET(p_mock, uint32_t);
  MOCK_CALL_COMPARE_INPUT_UINT8(p_mock, ARG0, arg0);
  MOCK_CALL_COMPARE_INPUT_UINT16(p_mock, ARG1, arg1);
  MOCK_CALL_COMPARE_INPUT_UINT32(p_mock, ARG2, arg2);
  MOCK_CALL_RETURN_VALUE(p_mock, uint32_t);
}

uint32_t mock_function_pointer_input(void * void_pointer , uint32_t * num_pointer, mock_callback_function_t func_pointer)
{
  mock_t * p_mock;

  MOCK_CALL_RETURN_IF_USED_AS_STUB(0x00);
  MOCK_CALL_FIND_RETURN_ON_FAILURE(p_mock, 0xFF);
  MOCK_CALL_ACTUAL(p_mock, void_pointer, num_pointer, func_pointer);
  MOCK_CALL_RETURN_IF_ERROR_SET(p_mock, uint32_t);
  MOCK_CALL_COMPARE_INPUT_POINTER(p_mock, ARG0, void_pointer);
  MOCK_CALL_COMPARE_INPUT_POINTER(p_mock, ARG1, num_pointer);
  MOCK_CALL_COMPARE_INPUT_POINTER(p_mock, ARG2, func_pointer);
  MOCK_CALL_RETURN_VALUE(p_mock, uint32_t);
}

/**
 * @brief This function provides a fake implementation example when using the mock function
 *        \ref mock_function_array as fake.
 *
 * @param[in]     p_in_array   Pointer to an array.
 * @param[in]     in_length  Length of array.
 * @param[out]    p_out_array  Pointer to an array.
 * @param[in,out] p_in_out_length Pointer where size of incoming array is specified, used for actual length when outgoing.
 *
 * @return Return value represented by uint32_t.

 */
static uint32_t mock_function_array_fake(uint8_t * p_in_array,  uint32_t in_length,
                                  uint8_t * p_out_array, uint32_t *p_in_out_length)
{
    memcpy(p_out_array, p_in_array, in_length);
    return 0;
}

/**@brief This function provides a function for mock calls that takes arrays with length information as input/output.
 *
 * @param[in]     p_in_array   Pointer to an array.
 * @param[in]     in_length  Length of array.
 * @param[out]    p_out_array  Pointer to an array.
 * @param[in,out] p_in_out_length Pointer where size of incoming array is specified, used for actual length when outgoing.
 *
 * @return Return value represented by uint32_t.
 */
uint32_t mock_function_array(uint8_t * p_in_array,  uint32_t in_length,
                             uint8_t * p_out_array, uint32_t *p_in_out_length)
{
  mock_t * p_mock;

  MOCK_CALL_RETURN_IF_USED_AS_STUB(0x00);
  MOCK_CALL_RETURN_IF_USED_AS_FAKE(mock_function_array_fake, p_in_array, in_length, p_out_array, p_in_out_length);
  MOCK_CALL_FIND_RETURN_ON_FAILURE(p_mock, 0xFF);
  MOCK_CALL_ACTUAL(p_mock, p_in_array, in_length, p_out_array, p_in_out_length);
  MOCK_CALL_RETURN_IF_ERROR_SET(p_mock, uint32_t);

  // Validating array input according to expectation setup in unit test.
  MOCK_CALL_COMPARE_INPUT_UINT8_ARRAY(p_mock, ARG0, p_mock->expect_arg[1].v, p_in_array, in_length);
  MOCK_CALL_COMPARE_INPUT_UINT8(p_mock, ARG3, *p_in_out_length);

  // Setting data output  according to rules setup in unit test.
  MOCK_CALL_SET_OUTPUT_ARRAY(p_mock->output_arg[2].p, p_out_array, p_mock->output_arg[3].v, uint8_t);
  *p_in_out_length = p_mock->output_arg[3].v;

  MOCK_CALL_RETURN_VALUE(p_mock, uint32_t);
}

/**
 * Fake implementation example when using the mock function \ref mock_function_struct as fake.
 */
static void mock_function_struct_fake(test_mock_struct_t * p_test_struct)
{
  p_test_struct->member_byte              = p_test_struct->member_byte + 1;
  p_test_struct->member                   = p_test_struct->member + 2;
  p_test_struct->union_member.submember_b = p_test_struct->union_member.submember_b + 3;
}

void mock_function_struct(test_mock_struct_t * p_test_struct)
{
  mock_t * p_mock;

  MOCK_CALL_RETURN_VOID_IF_USED_AS_STUB();
  MOCK_CALL_RETURN_VOID_IF_USED_AS_FAKE(mock_function_struct_fake, p_test_struct);
  MOCK_CALL_FIND_RETURN_VOID_ON_FAILURE(p_mock);
  MOCK_CALL_ACTUAL(p_mock, p_test_struct);

  // This type of compare is comparing raw memory.
  // It is usefull if it can be ensure that ALL mem is filled out as expected.
  // If only certain fields are to be examined, used the example below.
  //MOCK_CALL_COMPARE_INPUT_POINTER(p_mock, ARG0, p_test_struct);

  // This validation checks only fields required in the struct - recommended.
  MOCK_CALL_COMPARE_STRUCT_MEMBER_UINT8(p_mock, ARG0, p_test_struct, test_mock_struct_t,  member_byte);
  MOCK_CALL_COMPARE_STRUCT_MEMBER_UINT32(p_mock, ARG0, p_test_struct, test_mock_struct_t, member);
  MOCK_CALL_COMPARE_STRUCT_MEMBER_UINT32(p_mock, ARG0, p_test_struct, test_mock_struct_t, union_member.submember_a);
  MOCK_CALL_COMPARE_STRUCT_MEMBER_UINT32(p_mock, ARG0, p_test_struct, test_mock_struct_t, union_member.submember_b);
}


void mock_function_void(void)
{
  mock_t * p_mock;

  MOCK_CALL_RETURN_VOID_IF_USED_AS_STUB();
  MOCK_CALL_FIND_RETURN_VOID_ON_FAILURE(p_mock);
}
