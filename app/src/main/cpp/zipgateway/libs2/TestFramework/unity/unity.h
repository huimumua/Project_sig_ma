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
/* ==========================================
    Unity Project - A Test Framework for C
    Copyright (c) 2007-14 Mike Karlesky, Mark VanderVoord, Greg Williams
    [Released under MIT License. Please refer to license.txt for details]
========================================== */

#ifndef UNITY_FRAMEWORK_H
#define UNITY_FRAMEWORK_H
#define UNITY

#ifdef __cplusplus
extern "C"
{
#endif

#include "unity_internals.h"

//-------------------------------------------------------
// Configuration Options
//-------------------------------------------------------
// All options described below should be passed as a compiler flag to all files using Unity. If you must add #defines, place them BEFORE the #include above.

// Integers/longs/pointers
//     - Unity attempts to automatically discover your integer sizes
//       - define UNITY_EXCLUDE_STDINT_H to stop attempting to look in <stdint.h>
//       - define UNITY_EXCLUDE_LIMITS_H to stop attempting to look in <limits.h>
//       - define UNITY_EXCLUDE_SIZEOF to stop attempting to use sizeof in macros
//     - If you cannot use the automatic methods above, you can force Unity by using these options:
//       - define UNITY_SUPPORT_64
//       - define UNITY_INT_WIDTH
//       - UNITY_LONG_WIDTH
//       - UNITY_POINTER_WIDTH

// Floats
//     - define UNITY_EXCLUDE_FLOAT to disallow floating point comparisons
//     - define UNITY_FLOAT_PRECISION to specify the precision to use when doing TEST_ASSERT_EQUAL_FLOAT
//     - define UNITY_FLOAT_TYPE to specify doubles instead of single precision floats
//     - define UNITY_FLOAT_VERBOSE to print floating point values in errors (uses sprintf)
//     - define UNITY_INCLUDE_DOUBLE to allow double floating point comparisons
//     - define UNITY_EXCLUDE_DOUBLE to disallow double floating point comparisons (default)
//     - define UNITY_DOUBLE_PRECISION to specify the precision to use when doing TEST_ASSERT_EQUAL_DOUBLE
//     - define UNITY_DOUBLE_TYPE to specify something other than double
//     - define UNITY_DOUBLE_VERBOSE to print floating point values in errors (uses sprintf)

// Output
//     - by default, Unity prints to standard out with putchar.  define UNITY_OUTPUT_CHAR(a) with a different function if desired

// Optimization
//     - by default, line numbers are stored in unsigned shorts.  Define UNITY_LINE_TYPE with a different type if your files are huge
//     - by default, test and failure counters are unsigned shorts.  Define UNITY_COUNTER_TYPE with a different type if you want to save space or have more than 65535 Tests.

// Test Cases
//     - define UNITY_SUPPORT_TEST_CASES to include the TEST_CASE macro, though really it's mostly about the runner generator script

// Parameterized Tests
//     - you'll want to create a define of TEST_CASE(...) which basically evaluates to nothing

//-------------------------------------------------------
// Basic Fail and Ignore
//-------------------------------------------------------

#define TEST_FAIL_MESSAGE(message)                                                                 UNITY_TEST_FAIL(__LINE__, message)
#define TEST_FAIL()                                                                                UNITY_TEST_FAIL(__LINE__, NULL)
#define TEST_IGNORE_MESSAGE(message)                                                               UNITY_TEST_IGNORE(__LINE__, message)
#define TEST_IGNORE()                                                                              UNITY_TEST_IGNORE(__LINE__, NULL)
#define TEST_ONLY()

//-------------------------------------------------------
// Test Asserts (simple)
//-------------------------------------------------------

//Boolean
#define TEST_ASSERT(condition)                                                                     UNITY_TEST_ASSERT(       (condition), __LINE__, " Expression Evaluated To FALSE")
#define TEST_ASSERT_TRUE(condition)                                                                UNITY_TEST_ASSERT(       (condition), __LINE__, " Expected TRUE Was FALSE")
#define TEST_ASSERT_UNLESS(condition)                                                              UNITY_TEST_ASSERT(      !(condition), __LINE__, " Expression Evaluated To TRUE")
#define TEST_ASSERT_FALSE(condition)                                                               UNITY_TEST_ASSERT(      !(condition), __LINE__, " Expected FALSE Was TRUE")
#define TEST_ASSERT_NULL(pointer)                                                                  UNITY_TEST_ASSERT_NULL(    (pointer), __LINE__, " Expected NULL")
#define TEST_ASSERT_NOT_NULL(pointer)                                                              UNITY_TEST_ASSERT_NOT_NULL((pointer), __LINE__, " Expected Non-NULL")

//Integers (of all sizes)
#define TEST_ASSERT_EQUAL_INT(expected, actual)                                                    UNITY_TEST_ASSERT_EQUAL_INT((expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_INT8(expected, actual)                                                   UNITY_TEST_ASSERT_EQUAL_INT8((expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_INT16(expected, actual)                                                  UNITY_TEST_ASSERT_EQUAL_INT16((expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_INT32(expected, actual)                                                  UNITY_TEST_ASSERT_EQUAL_INT32((expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_INT64(expected, actual)                                                  UNITY_TEST_ASSERT_EQUAL_INT64((expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL(expected, actual)                                                        UNITY_TEST_ASSERT_EQUAL_INT((expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_NOT_EQUAL(expected, actual)                                                    UNITY_TEST_ASSERT(((expected) !=  (actual)), __LINE__, " Expected Not-Equal")
#define TEST_ASSERT_EQUAL_UINT(expected, actual)                                                   UNITY_TEST_ASSERT_EQUAL_UINT( (expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_UINT8(expected, actual)                                                  UNITY_TEST_ASSERT_EQUAL_UINT8( (expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_UINT16(expected, actual)                                                 UNITY_TEST_ASSERT_EQUAL_UINT16( (expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_UINT32(expected, actual)                                                 UNITY_TEST_ASSERT_EQUAL_UINT32( (expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_UINT64(expected, actual)                                                 UNITY_TEST_ASSERT_EQUAL_UINT64( (expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_HEX(expected, actual)                                                    UNITY_TEST_ASSERT_EQUAL_HEX32((expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_HEX8(expected, actual)                                                   UNITY_TEST_ASSERT_EQUAL_HEX8( (expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_HEX16(expected, actual)                                                  UNITY_TEST_ASSERT_EQUAL_HEX16((expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_HEX32(expected, actual)                                                  UNITY_TEST_ASSERT_EQUAL_HEX32((expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_HEX64(expected, actual)                                                  UNITY_TEST_ASSERT_EQUAL_HEX64((expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_BITS(mask, expected, actual)                                                   UNITY_TEST_ASSERT_BITS((mask), (expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_BITS_HIGH(mask, actual)                                                        UNITY_TEST_ASSERT_BITS((mask), (_UU32)(-1), (actual), __LINE__, NULL)
#define TEST_ASSERT_BITS_LOW(mask, actual)                                                         UNITY_TEST_ASSERT_BITS((mask), (_UU32)(0), (actual), __LINE__, NULL)
#define TEST_ASSERT_BIT_HIGH(bit, actual)                                                          UNITY_TEST_ASSERT_BITS(((_UU32)1 << bit), (_UU32)(-1), (actual), __LINE__, NULL)
#define TEST_ASSERT_BIT_LOW(bit, actual)                                                           UNITY_TEST_ASSERT_BITS(((_UU32)1 << bit), (_UU32)(0), (actual), __LINE__, NULL)

//Integer Ranges (of all sizes)
#define TEST_ASSERT_INT_WITHIN(delta, expected, actual)                                            UNITY_TEST_ASSERT_INT_WITHIN(delta, expected, actual, __LINE__, NULL)
#define TEST_ASSERT_INT8_WITHIN(delta, expected, actual)                                           UNITY_TEST_ASSERT_INT8_WITHIN(delta, expected, actual, __LINE__, NULL)
#define TEST_ASSERT_INT16_WITHIN(delta, expected, actual)                                          UNITY_TEST_ASSERT_INT16_WITHIN(delta, expected, actual, __LINE__, NULL)
#define TEST_ASSERT_INT32_WITHIN(delta, expected, actual)                                          UNITY_TEST_ASSERT_INT32_WITHIN(delta, expected, actual, __LINE__, NULL)
#define TEST_ASSERT_INT64_WITHIN(delta, expected, actual)                                          UNITY_TEST_ASSERT_INT64_WITHIN(delta, expected, actual, __LINE__, NULL)
#define TEST_ASSERT_UINT_WITHIN(delta, expected, actual)                                           UNITY_TEST_ASSERT_UINT_WITHIN(delta, expected, actual, __LINE__, NULL)
#define TEST_ASSERT_UINT8_WITHIN(delta, expected, actual)                                          UNITY_TEST_ASSERT_UINT8_WITHIN(delta, expected, actual, __LINE__, NULL)
#define TEST_ASSERT_UINT16_WITHIN(delta, expected, actual)                                         UNITY_TEST_ASSERT_UINT16_WITHIN(delta, expected, actual, __LINE__, NULL)
#define TEST_ASSERT_UINT32_WITHIN(delta, expected, actual)                                         UNITY_TEST_ASSERT_UINT32_WITHIN(delta, expected, actual, __LINE__, NULL)
#define TEST_ASSERT_UINT64_WITHIN(delta, expected, actual)                                         UNITY_TEST_ASSERT_UINT64_WITHIN(delta, expected, actual, __LINE__, NULL)
#define TEST_ASSERT_HEX_WITHIN(delta, expected, actual)                                            UNITY_TEST_ASSERT_HEX32_WITHIN(delta, expected, actual, __LINE__, NULL)
#define TEST_ASSERT_HEX8_WITHIN(delta, expected, actual)                                           UNITY_TEST_ASSERT_HEX8_WITHIN(delta, expected, actual, __LINE__, NULL)
#define TEST_ASSERT_HEX16_WITHIN(delta, expected, actual)                                          UNITY_TEST_ASSERT_HEX16_WITHIN(delta, expected, actual, __LINE__, NULL)
#define TEST_ASSERT_HEX32_WITHIN(delta, expected, actual)                                          UNITY_TEST_ASSERT_HEX32_WITHIN(delta, expected, actual, __LINE__, NULL)
#define TEST_ASSERT_HEX64_WITHIN(delta, expected, actual)                                          UNITY_TEST_ASSERT_HEX64_WITHIN(delta, expected, actual, __LINE__, NULL)

//Structs and Strings
#define TEST_ASSERT_EQUAL_PTR(expected, actual)                                                    UNITY_TEST_ASSERT_EQUAL_PTR((expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_STRING(expected, actual)                                                 UNITY_TEST_ASSERT_EQUAL_STRING(expected, actual, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_STRING_LEN(expected, actual, len)                                        UNITY_TEST_ASSERT_EQUAL_STRING_LEN(expected, actual, len, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_MEMORY(expected, actual, len)                                            UNITY_TEST_ASSERT_EQUAL_MEMORY(expected, actual, len, __LINE__, NULL)

//Arrays
#define TEST_ASSERT_EQUAL_INT_ARRAY(expected, actual, num_elements)                                UNITY_TEST_ASSERT_EQUAL_INT_ARRAY(expected, actual, num_elements, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_INT8_ARRAY(expected, actual, num_elements)                               UNITY_TEST_ASSERT_EQUAL_INT8_ARRAY(expected, actual, num_elements, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_INT16_ARRAY(expected, actual, num_elements)                              UNITY_TEST_ASSERT_EQUAL_INT16_ARRAY(expected, actual, num_elements, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_INT32_ARRAY(expected, actual, num_elements)                              UNITY_TEST_ASSERT_EQUAL_INT32_ARRAY(expected, actual, num_elements, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_INT64_ARRAY(expected, actual, num_elements)                              UNITY_TEST_ASSERT_EQUAL_INT64_ARRAY(expected, actual, num_elements, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_UINT_ARRAY(expected, actual, num_elements)                               UNITY_TEST_ASSERT_EQUAL_UINT_ARRAY(expected, actual, num_elements, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, actual, num_elements)                              UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, actual, num_elements, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_UINT16_ARRAY(expected, actual, num_elements)                             UNITY_TEST_ASSERT_EQUAL_UINT16_ARRAY(expected, actual, num_elements, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, num_elements)                             UNITY_TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, num_elements, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_UINT64_ARRAY(expected, actual, num_elements)                             UNITY_TEST_ASSERT_EQUAL_UINT64_ARRAY(expected, actual, num_elements, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_HEX_ARRAY(expected, actual, num_elements)                                UNITY_TEST_ASSERT_EQUAL_HEX32_ARRAY(expected, actual, num_elements, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, actual, num_elements)                               UNITY_TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, actual, num_elements, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_HEX16_ARRAY(expected, actual, num_elements)                              UNITY_TEST_ASSERT_EQUAL_HEX16_ARRAY(expected, actual, num_elements, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_HEX32_ARRAY(expected, actual, num_elements)                              UNITY_TEST_ASSERT_EQUAL_HEX32_ARRAY(expected, actual, num_elements, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_HEX64_ARRAY(expected, actual, num_elements)                              UNITY_TEST_ASSERT_EQUAL_HEX64_ARRAY(expected, actual, num_elements, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_PTR_ARRAY(expected, actual, num_elements)                                UNITY_TEST_ASSERT_EQUAL_PTR_ARRAY(expected, actual, num_elements, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_STRING_ARRAY(expected, actual, num_elements)                             UNITY_TEST_ASSERT_EQUAL_STRING_ARRAY(expected, actual, num_elements, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_MEMORY_ARRAY(expected, actual, len, num_elements)                        UNITY_TEST_ASSERT_EQUAL_MEMORY_ARRAY(expected, actual, len, num_elements, __LINE__, NULL)

//Floating Point (If Enabled)
#define TEST_ASSERT_FLOAT_WITHIN(delta, expected, actual)                                          UNITY_TEST_ASSERT_FLOAT_WITHIN(delta, expected, actual, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_FLOAT(expected, actual)                                                  UNITY_TEST_ASSERT_EQUAL_FLOAT(expected, actual, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected, actual, num_elements)                              UNITY_TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected, actual, num_elements, __LINE__, NULL)
#define TEST_ASSERT_FLOAT_IS_INF(actual)                                                           UNITY_TEST_ASSERT_FLOAT_IS_INF(actual, __LINE__, NULL)
#define TEST_ASSERT_FLOAT_IS_NEG_INF(actual)                                                       UNITY_TEST_ASSERT_FLOAT_IS_NEG_INF(actual, __LINE__, NULL)
#define TEST_ASSERT_FLOAT_IS_NAN(actual)                                                           UNITY_TEST_ASSERT_FLOAT_IS_NAN(actual, __LINE__, NULL)
#define TEST_ASSERT_FLOAT_IS_DETERMINATE(actual)                                                   UNITY_TEST_ASSERT_FLOAT_IS_DETERMINATE(actual, __LINE__, NULL)
#define TEST_ASSERT_FLOAT_IS_NOT_INF(actual)                                                       UNITY_TEST_ASSERT_FLOAT_IS_NOT_INF(actual, __LINE__, NULL)
#define TEST_ASSERT_FLOAT_IS_NOT_NEG_INF(actual)                                                   UNITY_TEST_ASSERT_FLOAT_IS_NOT_NEG_INF(actual, __LINE__, NULL)
#define TEST_ASSERT_FLOAT_IS_NOT_NAN(actual)                                                       UNITY_TEST_ASSERT_FLOAT_IS_NOT_NAN(actual, __LINE__, NULL)
#define TEST_ASSERT_FLOAT_IS_NOT_DETERMINATE(actual)                                               UNITY_TEST_ASSERT_FLOAT_IS_NOT_DETERMINATE(actual, __LINE__, NULL)

//Double (If Enabled)
#define TEST_ASSERT_DOUBLE_WITHIN(delta, expected, actual)                                         UNITY_TEST_ASSERT_DOUBLE_WITHIN(delta, expected, actual, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_DOUBLE(expected, actual)                                                 UNITY_TEST_ASSERT_EQUAL_DOUBLE(expected, actual, __LINE__, NULL)
#define TEST_ASSERT_EQUAL_DOUBLE_ARRAY(expected, actual, num_elements)                             UNITY_TEST_ASSERT_EQUAL_DOUBLE_ARRAY(expected, actual, num_elements, __LINE__, NULL)
#define TEST_ASSERT_DOUBLE_IS_INF(actual)                                                          UNITY_TEST_ASSERT_DOUBLE_IS_INF(actual, __LINE__, NULL)
#define TEST_ASSERT_DOUBLE_IS_NEG_INF(actual)                                                      UNITY_TEST_ASSERT_DOUBLE_IS_NEG_INF(actual, __LINE__, NULL)
#define TEST_ASSERT_DOUBLE_IS_NAN(actual)                                                          UNITY_TEST_ASSERT_DOUBLE_IS_NAN(actual, __LINE__, NULL)
#define TEST_ASSERT_DOUBLE_IS_DETERMINATE(actual)                                                  UNITY_TEST_ASSERT_DOUBLE_IS_DETERMINATE(actual, __LINE__, NULL)
#define TEST_ASSERT_DOUBLE_IS_NOT_INF(actual)                                                      UNITY_TEST_ASSERT_DOUBLE_IS_NOT_INF(actual, __LINE__, NULL)
#define TEST_ASSERT_DOUBLE_IS_NOT_NEG_INF(actual)                                                  UNITY_TEST_ASSERT_DOUBLE_IS_NOT_NEG_INF(actual, __LINE__, NULL)
#define TEST_ASSERT_DOUBLE_IS_NOT_NAN(actual)                                                      UNITY_TEST_ASSERT_DOUBLE_IS_NOT_NAN(actual, __LINE__, NULL)
#define TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(actual)                                              UNITY_TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(actual, __LINE__, NULL)

//-------------------------------------------------------
// Test Asserts (with additional messages)
//-------------------------------------------------------

//Boolean
#define TEST_ASSERT_MESSAGE(condition, message)                                                    UNITY_TEST_ASSERT(       (condition), __LINE__, message)
#define TEST_ASSERT_TRUE_MESSAGE(condition, message)                                               UNITY_TEST_ASSERT(       (condition), __LINE__, message)
#define TEST_ASSERT_UNLESS_MESSAGE(condition, message)                                             UNITY_TEST_ASSERT(      !(condition), __LINE__, message)
#define TEST_ASSERT_FALSE_MESSAGE(condition, message)                                              UNITY_TEST_ASSERT(      !(condition), __LINE__, message)
#define TEST_ASSERT_NULL_MESSAGE(pointer, message)                                                 UNITY_TEST_ASSERT_NULL(    (pointer), __LINE__, message)
#define TEST_ASSERT_NOT_NULL_MESSAGE(pointer, message)                                             UNITY_TEST_ASSERT_NOT_NULL((pointer), __LINE__, message)

//Integers (of all sizes)
#define TEST_ASSERT_EQUAL_INT_MESSAGE(expected, actual, message)                                   UNITY_TEST_ASSERT_EQUAL_INT((expected), (actual), __LINE__, message)
#define TEST_ASSERT_EQUAL_INT8_MESSAGE(expected, actual, message)                                  UNITY_TEST_ASSERT_EQUAL_INT8((expected), (actual), __LINE__, message)
#define TEST_ASSERT_EQUAL_INT16_MESSAGE(expected, actual, message)                                 UNITY_TEST_ASSERT_EQUAL_INT16((expected), (actual), __LINE__, message)
#define TEST_ASSERT_EQUAL_INT32_MESSAGE(expected, actual, message)                                 UNITY_TEST_ASSERT_EQUAL_INT32((expected), (actual), __LINE__, message)
#define TEST_ASSERT_EQUAL_INT64_MESSAGE(expected, actual, message)                                 UNITY_TEST_ASSERT_EQUAL_INT64((expected), (actual), __LINE__, message)
#define TEST_ASSERT_EQUAL_MESSAGE(expected, actual, message)                                       UNITY_TEST_ASSERT_EQUAL_INT((expected), (actual), __LINE__, message)
#define TEST_ASSERT_NOT_EQUAL_MESSAGE(expected, actual, message)                                   UNITY_TEST_ASSERT(((expected) !=  (actual)), __LINE__, message)
#define TEST_ASSERT_EQUAL_UINT_MESSAGE(expected, actual, message)                                  UNITY_TEST_ASSERT_EQUAL_UINT( (expected), (actual), __LINE__, message)
#define TEST_ASSERT_EQUAL_UINT8_MESSAGE(expected, actual, message)                                 UNITY_TEST_ASSERT_EQUAL_UINT8( (expected), (actual), __LINE__, message)
#define TEST_ASSERT_EQUAL_UINT16_MESSAGE(expected, actual, message)                                UNITY_TEST_ASSERT_EQUAL_UINT16( (expected), (actual), __LINE__, message)
#define TEST_ASSERT_EQUAL_UINT32_MESSAGE(expected, actual, message)                                UNITY_TEST_ASSERT_EQUAL_UINT32( (expected), (actual), __LINE__, message)
#define TEST_ASSERT_EQUAL_UINT64_MESSAGE(expected, actual, message)                                UNITY_TEST_ASSERT_EQUAL_UINT64( (expected), (actual), __LINE__, message)
#define TEST_ASSERT_EQUAL_HEX_MESSAGE(expected, actual, message)                                   UNITY_TEST_ASSERT_EQUAL_HEX32((expected), (actual), __LINE__, message)
#define TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected, actual, message)                                  UNITY_TEST_ASSERT_EQUAL_HEX8( (expected), (actual), __LINE__, message)
#define TEST_ASSERT_EQUAL_HEX16_MESSAGE(expected, actual, message)                                 UNITY_TEST_ASSERT_EQUAL_HEX16((expected), (actual), __LINE__, message)
#define TEST_ASSERT_EQUAL_HEX32_MESSAGE(expected, actual, message)                                 UNITY_TEST_ASSERT_EQUAL_HEX32((expected), (actual), __LINE__, message)
#define TEST_ASSERT_EQUAL_HEX64_MESSAGE(expected, actual, message)                                 UNITY_TEST_ASSERT_EQUAL_HEX64((expected), (actual), __LINE__, message)
#define TEST_ASSERT_BITS_MESSAGE(mask, expected, actual, message)                                  UNITY_TEST_ASSERT_BITS((mask), (expected), (actual), __LINE__, message)
#define TEST_ASSERT_BITS_HIGH_MESSAGE(mask, actual, message)                                       UNITY_TEST_ASSERT_BITS((mask), (_UU32)(-1), (actual), __LINE__, message)
#define TEST_ASSERT_BITS_LOW_MESSAGE(mask, actual, message)                                        UNITY_TEST_ASSERT_BITS((mask), (_UU32)(0), (actual), __LINE__, message)
#define TEST_ASSERT_BIT_HIGH_MESSAGE(bit, actual, message)                                         UNITY_TEST_ASSERT_BITS(((_UU32)1 << bit), (_UU32)(-1), (actual), __LINE__, message)
#define TEST_ASSERT_BIT_LOW_MESSAGE(bit, actual, message)                                          UNITY_TEST_ASSERT_BITS(((_UU32)1 << bit), (_UU32)(0), (actual), __LINE__, message)

//Integer Ranges (of all sizes)
#define TEST_ASSERT_INT_WITHIN_MESSAGE(delta, expected, actual, message)                           UNITY_TEST_ASSERT_INT_WITHIN(delta, expected, actual, __LINE__, message)
#define TEST_ASSERT_INT8_WITHIN_MESSAGE(delta, expected, actual, message)                          UNITY_TEST_ASSERT_INT8_WITHIN(delta, expected, actual, __LINE__, message)
#define TEST_ASSERT_INT16_WITHIN_MESSAGE(delta, expected, actual, message)                         UNITY_TEST_ASSERT_INT16_WITHIN(delta, expected, actual, __LINE__, message)
#define TEST_ASSERT_INT32_WITHIN_MESSAGE(delta, expected, actual, message)                         UNITY_TEST_ASSERT_INT32_WITHIN(delta, expected, actual, __LINE__, message)
#define TEST_ASSERT_INT64_WITHIN_MESSAGE(delta, expected, actual, message)                         UNITY_TEST_ASSERT_INT64_WITHIN(delta, expected, actual, __LINE__, message)
#define TEST_ASSERT_UINT_WITHIN_MESSAGE(delta, expected, actual, message)                          UNITY_TEST_ASSERT_UINT_WITHIN(delta, expected, actual, __LINE__, message)
#define TEST_ASSERT_UINT8_WITHIN_MESSAGE(delta, expected, actual, message)                         UNITY_TEST_ASSERT_UINT8_WITHIN(delta, expected, actual, __LINE__, message)
#define TEST_ASSERT_UINT16_WITHIN_MESSAGE(delta, expected, actual, message)                        UNITY_TEST_ASSERT_UINT16_WITHIN(delta, expected, actual, __LINE__, message)
#define TEST_ASSERT_UINT32_WITHIN_MESSAGE(delta, expected, actual, message)                        UNITY_TEST_ASSERT_UINT32_WITHIN(delta, expected, actual, __LINE__, message)
#define TEST_ASSERT_UINT64_WITHIN_MESSAGE(delta, expected, actual, message)                        UNITY_TEST_ASSERT_UINT64_WITHIN(delta, expected, actual, __LINE__, message)
#define TEST_ASSERT_HEX_WITHIN_MESSAGE(delta, expected, actual, message)                           UNITY_TEST_ASSERT_HEX32_WITHIN(delta, expected, actual, __LINE__, message)
#define TEST_ASSERT_HEX8_WITHIN_MESSAGE(delta, expected, actual, message)                          UNITY_TEST_ASSERT_HEX8_WITHIN(delta, expected, actual, __LINE__, message)
#define TEST_ASSERT_HEX16_WITHIN_MESSAGE(delta, expected, actual, message)                         UNITY_TEST_ASSERT_HEX16_WITHIN(delta, expected, actual, __LINE__, message)
#define TEST_ASSERT_HEX32_WITHIN_MESSAGE(delta, expected, actual, message)                         UNITY_TEST_ASSERT_HEX32_WITHIN(delta, expected, actual, __LINE__, message)
#define TEST_ASSERT_HEX64_WITHIN_MESSAGE(delta, expected, actual, message)                         UNITY_TEST_ASSERT_HEX64_WITHIN(delta, expected, actual, __LINE__, message)

//Structs and Strings
#define TEST_ASSERT_EQUAL_PTR_MESSAGE(expected, actual, message)                                   UNITY_TEST_ASSERT_EQUAL_PTR(expected, actual, __LINE__, message)
#define TEST_ASSERT_EQUAL_STRING_LEN_MESSAGE(expected, actual, len, message)                       UNITY_TEST_ASSERT_EQUAL_STRING_LEN(expected, actual, len, __LINE__, message)
#define TEST_ASSERT_EQUAL_MEMORY_MESSAGE(expected, actual, len, message)                           UNITY_TEST_ASSERT_EQUAL_MEMORY(expected, actual, len, __LINE__, message)

//Arrays
#define TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(expected, actual, num_elements, message)               UNITY_TEST_ASSERT_EQUAL_INT_ARRAY(expected, actual, num_elements, __LINE__, message)
#define TEST_ASSERT_EQUAL_INT8_ARRAY_MESSAGE(expected, actual, num_elements, message)              UNITY_TEST_ASSERT_EQUAL_INT8_ARRAY(expected, actual, num_elements, __LINE__, message)
#define TEST_ASSERT_EQUAL_INT16_ARRAY_MESSAGE(expected, actual, num_elements, message)             UNITY_TEST_ASSERT_EQUAL_INT16_ARRAY(expected, actual, num_elements, __LINE__, message)
#define TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, actual, num_elements, message)             UNITY_TEST_ASSERT_EQUAL_INT32_ARRAY(expected, actual, num_elements, __LINE__, message)
#define TEST_ASSERT_EQUAL_INT64_ARRAY_MESSAGE(expected, actual, num_elements, message)             UNITY_TEST_ASSERT_EQUAL_INT64_ARRAY(expected, actual, num_elements, __LINE__, message)
#define TEST_ASSERT_EQUAL_UINT_ARRAY_MESSAGE(expected, actual, num_elements, message)              UNITY_TEST_ASSERT_EQUAL_UINT_ARRAY(expected, actual, num_elements, __LINE__, message)
#define TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected, actual, num_elements, message)             UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, actual, num_elements, __LINE__, message)
#define TEST_ASSERT_EQUAL_UINT16_ARRAY_MESSAGE(expected, actual, num_elements, message)            UNITY_TEST_ASSERT_EQUAL_UINT16_ARRAY(expected, actual, num_elements, __LINE__, message)
#define TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, actual, num_elements, message)            UNITY_TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, num_elements, __LINE__, message)
#define TEST_ASSERT_EQUAL_UINT64_ARRAY_MESSAGE(expected, actual, num_elements, message)            UNITY_TEST_ASSERT_EQUAL_UINT64_ARRAY(expected, actual, num_elements, __LINE__, message)
#define TEST_ASSERT_EQUAL_HEX_ARRAY_MESSAGE(expected, actual, num_elements, message)               UNITY_TEST_ASSERT_EQUAL_HEX32_ARRAY(expected, actual, num_elements, __LINE__, message)
#define TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected, actual, num_elements, message)              UNITY_TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, actual, num_elements, __LINE__, message)
#define TEST_ASSERT_EQUAL_HEX16_ARRAY_MESSAGE(expected, actual, num_elements, message)             UNITY_TEST_ASSERT_EQUAL_HEX16_ARRAY(expected, actual, num_elements, __LINE__, message)
#define TEST_ASSERT_EQUAL_HEX32_ARRAY_MESSAGE(expected, actual, num_elements, message)             UNITY_TEST_ASSERT_EQUAL_HEX32_ARRAY(expected, actual, num_elements, __LINE__, message)
#define TEST_ASSERT_EQUAL_HEX64_ARRAY_MESSAGE(expected, actual, num_elements, message)             UNITY_TEST_ASSERT_EQUAL_HEX64_ARRAY(expected, actual, num_elements, __LINE__, message)
#define TEST_ASSERT_EQUAL_PTR_ARRAY_MESSAGE(expected, actual, num_elements, message)               UNITY_TEST_ASSERT_EQUAL_PTR_ARRAY(expected, actual, num_elements, __LINE__, message)
#define TEST_ASSERT_EQUAL_STRING_ARRAY_MESSAGE(expected, actual, num_elements, message)            UNITY_TEST_ASSERT_EQUAL_STRING_ARRAY(expected, actual, num_elements, __LINE__, message)
#define TEST_ASSERT_EQUAL_MEMORY_ARRAY_MESSAGE(expected, actual, len, num_elements, message)       UNITY_TEST_ASSERT_EQUAL_MEMORY_ARRAY(expected, actual, len, num_elements, __LINE__, message)

//Floating Point (If Enabled)
#define TEST_ASSERT_FLOAT_WITHIN_MESSAGE(delta, expected, actual, message)                         UNITY_TEST_ASSERT_FLOAT_WITHIN(delta, expected, actual, __LINE__, message)
#define TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, message)                                 UNITY_TEST_ASSERT_EQUAL_FLOAT(expected, actual, __LINE__, message)
#define TEST_ASSERT_EQUAL_FLOAT_ARRAY_MESSAGE(expected, actual, num_elements, message)             UNITY_TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected, actual, num_elements, __LINE__, message)
#define TEST_ASSERT_FLOAT_IS_INF_MESSAGE(actual, message)                                          UNITY_TEST_ASSERT_FLOAT_IS_INF(actual, __LINE__, message)
#define TEST_ASSERT_FLOAT_IS_NEG_INF_MESSAGE(actual, message)                                      UNITY_TEST_ASSERT_FLOAT_IS_NEG_INF(actual, __LINE__, message)
#define TEST_ASSERT_FLOAT_IS_NAN_MESSAGE(actual, message)                                          UNITY_TEST_ASSERT_FLOAT_IS_NAN(actual, __LINE__, message)
#define TEST_ASSERT_FLOAT_IS_DETERMINATE_MESSAGE(actual, message)                                  UNITY_TEST_ASSERT_FLOAT_IS_DETERMINATE(actual, __LINE__, message)
#define TEST_ASSERT_FLOAT_IS_NOT_INF_MESSAGE(actual, message)                                      UNITY_TEST_ASSERT_FLOAT_IS_NOT_INF(actual, __LINE__, message)
#define TEST_ASSERT_FLOAT_IS_NOT_NEG_INF_MESSAGE(actual, message)                                  UNITY_TEST_ASSERT_FLOAT_IS_NOT_NEG_INF(actual, __LINE__, message)
#define TEST_ASSERT_FLOAT_IS_NOT_NAN_MESSAGE(actual, message)                                      UNITY_TEST_ASSERT_FLOAT_IS_NOT_NAN(actual, __LINE__, message)
#define TEST_ASSERT_FLOAT_IS_NOT_DETERMINATE_MESSAGE(actual, message)                              UNITY_TEST_ASSERT_FLOAT_IS_NOT_DETERMINATE(actual, __LINE__, message)

//Double (If Enabled)
#define TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(delta, expected, actual, message)                        UNITY_TEST_ASSERT_DOUBLE_WITHIN(delta, expected, actual, __LINE__, message)
#define TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(expected, actual, message)                                UNITY_TEST_ASSERT_EQUAL_DOUBLE(expected, actual, __LINE__, message)
#define TEST_ASSERT_EQUAL_DOUBLE_ARRAY_MESSAGE(expected, actual, num_elements, message)            UNITY_TEST_ASSERT_EQUAL_DOUBLE_ARRAY(expected, actual, num_elements, __LINE__, message)
#define TEST_ASSERT_DOUBLE_IS_INF_MESSAGE(actual, message)                                         UNITY_TEST_ASSERT_DOUBLE_IS_INF(actual, __LINE__, message)
#define TEST_ASSERT_DOUBLE_IS_NEG_INF_MESSAGE(actual, message)                                     UNITY_TEST_ASSERT_DOUBLE_IS_NEG_INF(actual, __LINE__, message)
#define TEST_ASSERT_DOUBLE_IS_NAN_MESSAGE(actual, message)                                         UNITY_TEST_ASSERT_DOUBLE_IS_NAN(actual, __LINE__, message)
#define TEST_ASSERT_DOUBLE_IS_DETERMINATE_MESSAGE(actual, message)                                 UNITY_TEST_ASSERT_DOUBLE_IS_DETERMINATE(actual, __LINE__, message)
#define TEST_ASSERT_DOUBLE_IS_NOT_INF_MESSAGE(actual, message)                                     UNITY_TEST_ASSERT_DOUBLE_IS_NOT_INF(actual, __LINE__, message)
#define TEST_ASSERT_DOUBLE_IS_NOT_NEG_INF_MESSAGE(actual, message)                                 UNITY_TEST_ASSERT_DOUBLE_IS_NOT_NEG_INF(actual, __LINE__, message)
#define TEST_ASSERT_DOUBLE_IS_NOT_NAN_MESSAGE(actual, message)                                     UNITY_TEST_ASSERT_DOUBLE_IS_NOT_NAN(actual, __LINE__, message)
#define TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE_MESSAGE(actual, message)                             UNITY_TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(actual, __LINE__, message)

//end of UNITY_FRAMEWORK_H
#ifdef __cplusplus
}
#endif
#endif
