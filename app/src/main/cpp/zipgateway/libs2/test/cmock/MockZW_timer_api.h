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
#ifndef _MOCKZW_TIMER_API_H
#define _MOCKZW_TIMER_API_H

#include "ZW_timer_api.h"

/* Ignore the following warnings, since we are copying code */
#if defined(__GNUC__) && !defined(__ICC)
#if !defined(__clang__)
#pragma GCC diagnostic ignored "-Wpragmas"
#endif
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wduplicate-decl-specifier"
#endif

void MockZW_timer_api_Init(void);
void MockZW_timer_api_Destroy(void);
void MockZW_timer_api_Verify(void);




#define TimerStart_IgnoreAndReturn(cmock_retval) TimerStart_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void TimerStart_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, BYTE cmock_to_return);
#define TimerStart_ExpectAndReturn(cb, timerTicks, repeats, cmock_retval) TimerStart_CMockExpectAndReturn(__LINE__, cb, timerTicks, repeats, cmock_retval)
void TimerStart_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, func cb, BYTE timerTicks, BYTE repeats, BYTE cmock_to_return);
typedef BYTE (* CMOCK_TimerStart_CALLBACK)(func cb, BYTE timerTicks, BYTE repeats, int cmock_num_calls);
void TimerStart_StubWithCallback(CMOCK_TimerStart_CALLBACK Callback);
#define TimerStart_IgnoreArg_cb() TimerStart_CMockIgnoreArg_cb(__LINE__)
void TimerStart_CMockIgnoreArg_cb(UNITY_LINE_TYPE cmock_line);
#define TimerStart_IgnoreArg_timerTicks() TimerStart_CMockIgnoreArg_timerTicks(__LINE__)
void TimerStart_CMockIgnoreArg_timerTicks(UNITY_LINE_TYPE cmock_line);
#define TimerStart_IgnoreArg_repeats() TimerStart_CMockIgnoreArg_repeats(__LINE__)
void TimerStart_CMockIgnoreArg_repeats(UNITY_LINE_TYPE cmock_line);
#define TimerCancel_IgnoreAndReturn(cmock_retval) TimerCancel_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void TimerCancel_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, BYTE cmock_to_return);
#define TimerCancel_ExpectAndReturn(timerHandle, cmock_retval) TimerCancel_CMockExpectAndReturn(__LINE__, timerHandle, cmock_retval)
void TimerCancel_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BYTE timerHandle, BYTE cmock_to_return);
typedef BYTE (* CMOCK_TimerCancel_CALLBACK)(BYTE timerHandle, int cmock_num_calls);
void TimerCancel_StubWithCallback(CMOCK_TimerCancel_CALLBACK Callback);
#define TimerCancel_IgnoreArg_timerHandle() TimerCancel_CMockIgnoreArg_timerHandle(__LINE__)
void TimerCancel_CMockIgnoreArg_timerHandle(UNITY_LINE_TYPE cmock_line);
#define getTickTime_IgnoreAndReturn(cmock_retval) getTickTime_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void getTickTime_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, WORD cmock_to_return);
#define getTickTime_ExpectAndReturn(cmock_retval) getTickTime_CMockExpectAndReturn(__LINE__, cmock_retval)
void getTickTime_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, WORD cmock_to_return);
typedef WORD (* CMOCK_getTickTime_CALLBACK)(int cmock_num_calls);
void getTickTime_StubWithCallback(CMOCK_getTickTime_CALLBACK Callback);
#define getTickTimePassed_IgnoreAndReturn(cmock_retval) getTickTimePassed_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void getTickTimePassed_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, WORD cmock_to_return);
#define getTickTimePassed_ExpectAndReturn(wStartTickTime, cmock_retval) getTickTimePassed_CMockExpectAndReturn(__LINE__, wStartTickTime, cmock_retval)
void getTickTimePassed_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, WORD wStartTickTime, WORD cmock_to_return);
typedef WORD (* CMOCK_getTickTimePassed_CALLBACK)(WORD wStartTickTime, int cmock_num_calls);
void getTickTimePassed_StubWithCallback(CMOCK_getTickTimePassed_CALLBACK Callback);
#define getTickTimePassed_IgnoreArg_wStartTickTime() getTickTimePassed_CMockIgnoreArg_wStartTickTime(__LINE__)
void getTickTimePassed_CMockIgnoreArg_wStartTickTime(UNITY_LINE_TYPE cmock_line);

#endif
