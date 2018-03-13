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
/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "MockZW_timer_api.h"

typedef struct _CMOCK_TimerStart_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BYTE ReturnVal;
  func Expected_cb;
  BYTE Expected_timerTicks;
  BYTE Expected_repeats;
  int IgnoreArg_cb;
  int IgnoreArg_timerTicks;
  int IgnoreArg_repeats;

} CMOCK_TimerStart_CALL_INSTANCE;

typedef struct _CMOCK_TimerCancel_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BYTE ReturnVal;
  BYTE Expected_timerHandle;
  int IgnoreArg_timerHandle;

} CMOCK_TimerCancel_CALL_INSTANCE;

typedef struct _CMOCK_getTickTime_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  WORD ReturnVal;

} CMOCK_getTickTime_CALL_INSTANCE;

typedef struct _CMOCK_getTickTimePassed_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  WORD ReturnVal;
  WORD Expected_wStartTickTime;
  int IgnoreArg_wStartTickTime;

} CMOCK_getTickTimePassed_CALL_INSTANCE;

static struct MockZW_timer_apiInstance
{
  int TimerStart_IgnoreBool;
  BYTE TimerStart_FinalReturn;
  CMOCK_TimerStart_CALLBACK TimerStart_CallbackFunctionPointer;
  int TimerStart_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE TimerStart_CallInstance;
  int TimerCancel_IgnoreBool;
  BYTE TimerCancel_FinalReturn;
  CMOCK_TimerCancel_CALLBACK TimerCancel_CallbackFunctionPointer;
  int TimerCancel_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE TimerCancel_CallInstance;
  int getTickTime_IgnoreBool;
  WORD getTickTime_FinalReturn;
  CMOCK_getTickTime_CALLBACK getTickTime_CallbackFunctionPointer;
  int getTickTime_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE getTickTime_CallInstance;
  int getTickTimePassed_IgnoreBool;
  WORD getTickTimePassed_FinalReturn;
  CMOCK_getTickTimePassed_CALLBACK getTickTimePassed_CallbackFunctionPointer;
  int getTickTimePassed_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE getTickTimePassed_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void MockZW_timer_api_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  if (Mock.TimerStart_IgnoreBool)
    Mock.TimerStart_CallInstance = CMOCK_GUTS_NONE;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.TimerStart_CallInstance, cmock_line, "Function 'TimerStart' called less times than expected.");
  if (Mock.TimerStart_CallbackFunctionPointer != NULL)
    Mock.TimerStart_CallInstance = CMOCK_GUTS_NONE;
  if (Mock.TimerCancel_IgnoreBool)
    Mock.TimerCancel_CallInstance = CMOCK_GUTS_NONE;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.TimerCancel_CallInstance, cmock_line, "Function 'TimerCancel' called less times than expected.");
  if (Mock.TimerCancel_CallbackFunctionPointer != NULL)
    Mock.TimerCancel_CallInstance = CMOCK_GUTS_NONE;
  if (Mock.getTickTime_IgnoreBool)
    Mock.getTickTime_CallInstance = CMOCK_GUTS_NONE;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.getTickTime_CallInstance, cmock_line, "Function 'getTickTime' called less times than expected.");
  if (Mock.getTickTime_CallbackFunctionPointer != NULL)
    Mock.getTickTime_CallInstance = CMOCK_GUTS_NONE;
  if (Mock.getTickTimePassed_IgnoreBool)
    Mock.getTickTimePassed_CallInstance = CMOCK_GUTS_NONE;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.getTickTimePassed_CallInstance, cmock_line, "Function 'getTickTimePassed' called less times than expected.");
  if (Mock.getTickTimePassed_CallbackFunctionPointer != NULL)
    Mock.getTickTimePassed_CallInstance = CMOCK_GUTS_NONE;
}

void MockZW_timer_api_Init(void)
{
  MockZW_timer_api_Destroy();
}

void MockZW_timer_api_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
  Mock.TimerStart_CallbackFunctionPointer = NULL;
  Mock.TimerStart_CallbackCalls = 0;
  Mock.TimerCancel_CallbackFunctionPointer = NULL;
  Mock.TimerCancel_CallbackCalls = 0;
  Mock.getTickTime_CallbackFunctionPointer = NULL;
  Mock.getTickTime_CallbackCalls = 0;
  Mock.getTickTimePassed_CallbackFunctionPointer = NULL;
  Mock.getTickTimePassed_CallbackCalls = 0;
}

BYTE TimerStart(func cb, BYTE timerTicks, BYTE repeats)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_TimerStart_CALL_INSTANCE* cmock_call_instance = (CMOCK_TimerStart_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.TimerStart_CallInstance);
  Mock.TimerStart_CallInstance = CMock_Guts_MemNext(Mock.TimerStart_CallInstance);
  if (Mock.TimerStart_IgnoreBool)
  {
    if (cmock_call_instance == NULL)
      return (BYTE)Mock.TimerStart_FinalReturn;
    memcpy(&Mock.TimerStart_FinalReturn, &cmock_call_instance->ReturnVal, sizeof(BYTE));
    return (BYTE)cmock_call_instance->ReturnVal;
  }
  if (Mock.TimerStart_CallbackFunctionPointer != NULL)
  {
    return Mock.TimerStart_CallbackFunctionPointer(cb, timerTicks, repeats, Mock.TimerStart_CallbackCalls++);
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'TimerStart' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  if (!cmock_call_instance->IgnoreArg_cb)
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_cb), (void*)(&cb), sizeof(func), cmock_line, "Function 'TimerStart' called with unexpected value for argument 'cb'.");
  }
  if (!cmock_call_instance->IgnoreArg_timerTicks)
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_timerTicks), (void*)(&timerTicks), sizeof(BYTE), cmock_line, "Function 'TimerStart' called with unexpected value for argument 'timerTicks'.");
  }
  if (!cmock_call_instance->IgnoreArg_repeats)
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_repeats), (void*)(&repeats), sizeof(BYTE), cmock_line, "Function 'TimerStart' called with unexpected value for argument 'repeats'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_TimerStart(CMOCK_TimerStart_CALL_INSTANCE* cmock_call_instance, func cb, BYTE timerTicks, BYTE repeats)
{
  memcpy(&cmock_call_instance->Expected_cb, &cb, sizeof(func));
  cmock_call_instance->IgnoreArg_cb = 0;
  memcpy(&cmock_call_instance->Expected_timerTicks, &timerTicks, sizeof(BYTE));
  cmock_call_instance->IgnoreArg_timerTicks = 0;
  memcpy(&cmock_call_instance->Expected_repeats, &repeats, sizeof(BYTE));
  cmock_call_instance->IgnoreArg_repeats = 0;
}

void TimerStart_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, BYTE cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_TimerStart_CALL_INSTANCE));
  CMOCK_TimerStart_CALL_INSTANCE* cmock_call_instance = (CMOCK_TimerStart_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.TimerStart_CallInstance = CMock_Guts_MemChain(Mock.TimerStart_CallInstance, cmock_guts_index);
  Mock.TimerStart_IgnoreBool = (int)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.TimerStart_IgnoreBool = (int)1;
}

void TimerStart_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, func cb, BYTE timerTicks, BYTE repeats, BYTE cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_TimerStart_CALL_INSTANCE));
  CMOCK_TimerStart_CALL_INSTANCE* cmock_call_instance = (CMOCK_TimerStart_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.TimerStart_CallInstance = CMock_Guts_MemChain(Mock.TimerStart_CallInstance, cmock_guts_index);
  Mock.TimerStart_IgnoreBool = (int)0;
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_TimerStart(cmock_call_instance, cb, timerTicks, repeats);
  memcpy(&cmock_call_instance->ReturnVal, &cmock_to_return, sizeof(BYTE));
}

void TimerStart_StubWithCallback(CMOCK_TimerStart_CALLBACK Callback)
{
  Mock.TimerStart_CallbackFunctionPointer = Callback;
}

void TimerStart_CMockIgnoreArg_cb(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_TimerStart_CALL_INSTANCE* cmock_call_instance = (CMOCK_TimerStart_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.TimerStart_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "cb IgnoreArg called before Expect on 'TimerStart'.");
  cmock_call_instance->IgnoreArg_cb = 1;
}

void TimerStart_CMockIgnoreArg_timerTicks(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_TimerStart_CALL_INSTANCE* cmock_call_instance = (CMOCK_TimerStart_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.TimerStart_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "timerTicks IgnoreArg called before Expect on 'TimerStart'.");
  cmock_call_instance->IgnoreArg_timerTicks = 1;
}

void TimerStart_CMockIgnoreArg_repeats(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_TimerStart_CALL_INSTANCE* cmock_call_instance = (CMOCK_TimerStart_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.TimerStart_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "repeats IgnoreArg called before Expect on 'TimerStart'.");
  cmock_call_instance->IgnoreArg_repeats = 1;
}

BYTE TimerCancel(BYTE timerHandle)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_TimerCancel_CALL_INSTANCE* cmock_call_instance = (CMOCK_TimerCancel_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.TimerCancel_CallInstance);
  Mock.TimerCancel_CallInstance = CMock_Guts_MemNext(Mock.TimerCancel_CallInstance);
  if (Mock.TimerCancel_IgnoreBool)
  {
    if (cmock_call_instance == NULL)
      return (BYTE)Mock.TimerCancel_FinalReturn;
    memcpy(&Mock.TimerCancel_FinalReturn, &cmock_call_instance->ReturnVal, sizeof(BYTE));
    return (BYTE)cmock_call_instance->ReturnVal;
  }
  if (Mock.TimerCancel_CallbackFunctionPointer != NULL)
  {
    return Mock.TimerCancel_CallbackFunctionPointer(timerHandle, Mock.TimerCancel_CallbackCalls++);
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'TimerCancel' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  if (!cmock_call_instance->IgnoreArg_timerHandle)
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_timerHandle), (void*)(&timerHandle), sizeof(BYTE), cmock_line, "Function 'TimerCancel' called with unexpected value for argument 'timerHandle'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_TimerCancel(CMOCK_TimerCancel_CALL_INSTANCE* cmock_call_instance, BYTE timerHandle)
{
  memcpy(&cmock_call_instance->Expected_timerHandle, &timerHandle, sizeof(BYTE));
  cmock_call_instance->IgnoreArg_timerHandle = 0;
}

void TimerCancel_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, BYTE cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_TimerCancel_CALL_INSTANCE));
  CMOCK_TimerCancel_CALL_INSTANCE* cmock_call_instance = (CMOCK_TimerCancel_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.TimerCancel_CallInstance = CMock_Guts_MemChain(Mock.TimerCancel_CallInstance, cmock_guts_index);
  Mock.TimerCancel_IgnoreBool = (int)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.TimerCancel_IgnoreBool = (int)1;
}

void TimerCancel_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BYTE timerHandle, BYTE cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_TimerCancel_CALL_INSTANCE));
  CMOCK_TimerCancel_CALL_INSTANCE* cmock_call_instance = (CMOCK_TimerCancel_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.TimerCancel_CallInstance = CMock_Guts_MemChain(Mock.TimerCancel_CallInstance, cmock_guts_index);
  Mock.TimerCancel_IgnoreBool = (int)0;
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_TimerCancel(cmock_call_instance, timerHandle);
  memcpy(&cmock_call_instance->ReturnVal, &cmock_to_return, sizeof(BYTE));
}

void TimerCancel_StubWithCallback(CMOCK_TimerCancel_CALLBACK Callback)
{
  Mock.TimerCancel_CallbackFunctionPointer = Callback;
}

void TimerCancel_CMockIgnoreArg_timerHandle(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_TimerCancel_CALL_INSTANCE* cmock_call_instance = (CMOCK_TimerCancel_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.TimerCancel_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "timerHandle IgnoreArg called before Expect on 'TimerCancel'.");
  cmock_call_instance->IgnoreArg_timerHandle = 1;
}

WORD getTickTime(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_getTickTime_CALL_INSTANCE* cmock_call_instance = (CMOCK_getTickTime_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.getTickTime_CallInstance);
  Mock.getTickTime_CallInstance = CMock_Guts_MemNext(Mock.getTickTime_CallInstance);
  if (Mock.getTickTime_IgnoreBool)
  {
    if (cmock_call_instance == NULL)
      return (WORD)Mock.getTickTime_FinalReturn;
    memcpy(&Mock.getTickTime_FinalReturn, &cmock_call_instance->ReturnVal, sizeof(WORD));
    return (WORD)cmock_call_instance->ReturnVal;
  }
  if (Mock.getTickTime_CallbackFunctionPointer != NULL)
  {
    return Mock.getTickTime_CallbackFunctionPointer(Mock.getTickTime_CallbackCalls++);
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'getTickTime' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  return cmock_call_instance->ReturnVal;
}

void getTickTime_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, WORD cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_getTickTime_CALL_INSTANCE));
  CMOCK_getTickTime_CALL_INSTANCE* cmock_call_instance = (CMOCK_getTickTime_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.getTickTime_CallInstance = CMock_Guts_MemChain(Mock.getTickTime_CallInstance, cmock_guts_index);
  Mock.getTickTime_IgnoreBool = (int)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.getTickTime_IgnoreBool = (int)1;
}

void getTickTime_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, WORD cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_getTickTime_CALL_INSTANCE));
  CMOCK_getTickTime_CALL_INSTANCE* cmock_call_instance = (CMOCK_getTickTime_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.getTickTime_CallInstance = CMock_Guts_MemChain(Mock.getTickTime_CallInstance, cmock_guts_index);
  Mock.getTickTime_IgnoreBool = (int)0;
  cmock_call_instance->LineNumber = cmock_line;
  memcpy(&cmock_call_instance->ReturnVal, &cmock_to_return, sizeof(WORD));
}

void getTickTime_StubWithCallback(CMOCK_getTickTime_CALLBACK Callback)
{
  Mock.getTickTime_CallbackFunctionPointer = Callback;
}

WORD getTickTimePassed(WORD wStartTickTime)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_getTickTimePassed_CALL_INSTANCE* cmock_call_instance = (CMOCK_getTickTimePassed_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.getTickTimePassed_CallInstance);
  Mock.getTickTimePassed_CallInstance = CMock_Guts_MemNext(Mock.getTickTimePassed_CallInstance);
  if (Mock.getTickTimePassed_IgnoreBool)
  {
    if (cmock_call_instance == NULL)
      return (WORD)Mock.getTickTimePassed_FinalReturn;
    memcpy(&Mock.getTickTimePassed_FinalReturn, &cmock_call_instance->ReturnVal, sizeof(WORD));
    return (WORD)cmock_call_instance->ReturnVal;
  }
  if (Mock.getTickTimePassed_CallbackFunctionPointer != NULL)
  {
    return Mock.getTickTimePassed_CallbackFunctionPointer(wStartTickTime, Mock.getTickTimePassed_CallbackCalls++);
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'getTickTimePassed' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  if (!cmock_call_instance->IgnoreArg_wStartTickTime)
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_wStartTickTime), (void*)(&wStartTickTime), sizeof(WORD), cmock_line, "Function 'getTickTimePassed' called with unexpected value for argument 'wStartTickTime'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_getTickTimePassed(CMOCK_getTickTimePassed_CALL_INSTANCE* cmock_call_instance, WORD wStartTickTime)
{
  memcpy(&cmock_call_instance->Expected_wStartTickTime, &wStartTickTime, sizeof(WORD));
  cmock_call_instance->IgnoreArg_wStartTickTime = 0;
}

void getTickTimePassed_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, WORD cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_getTickTimePassed_CALL_INSTANCE));
  CMOCK_getTickTimePassed_CALL_INSTANCE* cmock_call_instance = (CMOCK_getTickTimePassed_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.getTickTimePassed_CallInstance = CMock_Guts_MemChain(Mock.getTickTimePassed_CallInstance, cmock_guts_index);
  Mock.getTickTimePassed_IgnoreBool = (int)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.getTickTimePassed_IgnoreBool = (int)1;
}

void getTickTimePassed_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, WORD wStartTickTime, WORD cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_getTickTimePassed_CALL_INSTANCE));
  CMOCK_getTickTimePassed_CALL_INSTANCE* cmock_call_instance = (CMOCK_getTickTimePassed_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.getTickTimePassed_CallInstance = CMock_Guts_MemChain(Mock.getTickTimePassed_CallInstance, cmock_guts_index);
  Mock.getTickTimePassed_IgnoreBool = (int)0;
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_getTickTimePassed(cmock_call_instance, wStartTickTime);
  memcpy(&cmock_call_instance->ReturnVal, &cmock_to_return, sizeof(WORD));
}

void getTickTimePassed_StubWithCallback(CMOCK_getTickTimePassed_CALLBACK Callback)
{
  Mock.getTickTimePassed_CallbackFunctionPointer = Callback;
}

void getTickTimePassed_CMockIgnoreArg_wStartTickTime(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_getTickTimePassed_CALL_INSTANCE* cmock_call_instance = (CMOCK_getTickTimePassed_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.getTickTimePassed_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "wStartTickTime IgnoreArg called before Expect on 'getTickTimePassed'.");
  cmock_call_instance->IgnoreArg_wStartTickTime = 1;
}

