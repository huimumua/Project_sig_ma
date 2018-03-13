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
 * ZIP_Router_logging.h
 *
 *  Created on: Aug 11, 2015
 *      Author: aes
 */

#ifndef SRC_ZIP_ROUTER_LOGGING_H_
#define SRC_ZIP_ROUTER_LOGGING_H_



#if defined(WIN32)
#define LOG_PRINTF(f, ...) printf(f,  __VA_ARGS__ )
#define ERR_PRINTF(f, ...) printf(f,  __VA_ARGS__ )
#define WRN_PRINTF(f, ...) printf(f,  __VA_ARGS__ )
#define DBG_PRINTF(f, ...) printf(f,  __VA_ARGS__ )
#elif defined(__ASIX_C51__)
#define LOG_PRINTF
#define ERR_PRINTF printf
#define WRN_PRINTF printf
#define DBG_PRINTF
#elif defined(__BIONIC__)
#include <android/log.h>
#define LOG_PRINTF(f, ...) __android_log_print(ANDROID_LOG_INFO , "zgw", f , ## __VA_ARGS__ )
#define ERR_PRINTF(f, ...) __android_log_print(ANDROID_LOG_ERROR, "zgw", f , ## __VA_ARGS__ )
#define WRN_PRINTF(f, ...) __android_log_print(ANDROID_LOG_WARN , "zgw", f , ## __VA_ARGS__ )
#ifdef ZIPGATEWAY_DEBUG //djnakata
#define DBG_PRINTF(f, ...) __android_log_print(ANDROID_LOG_DEBUG, "zgw", f , ## __VA_ARGS__ )
#else
#define DBG_PRINTF
#endif
#elif defined(SYSLOG)
#include <syslog.h>

#define LOG_PRINTF(f, ...) syslog(LOG_INFO , f , ## __VA_ARGS__ )
#define ERR_PRINTF(f, ...) syslog(LOG_ERR,  f , ## __VA_ARGS__ )
#define WRN_PRINTF(f, ...) syslog(LOG_WARNING , f , ## __VA_ARGS__ )
#define DBG_PRINTF(f, ...) syslog(LOG_DEBUG,  f , ## __VA_ARGS__ )


#else
#include <stdio.h>
#include "sys/clock.h"
/**
 * Information level logging.
 * \param f argument similar to the one passed to printf
 */
#define LOG_PRINTF(f, ...) printf("\033[32;1m%lu " f "\033[0m",  clock_time(), ## __VA_ARGS__ )
/**
 * Error level logging.
 * \param f argument similar to the one passed to printf
 */
#define ERR_PRINTF(f, ...) printf("\033[31;1m%lu " f "\033[0m",  clock_time(), ## __VA_ARGS__ )
/**
 * Warning level logging.
 * \param f argument similar to the one passed to printf
 */
#define WRN_PRINTF(f, ...) printf("\033[33;1m%lu " f "\033[0m",  clock_time(), ## __VA_ARGS__ )
/**
 * Debug level logging.
 * \param f argument similar to the one passed to printf
 */
#define DBG_PRINTF(f, ...) printf("\033[34;1m%lu " f "\033[0m", clock_time(), ## __VA_ARGS__ )
#endif


#endif /* SRC_ZIP_ROUTER_LOGGING_H_ */
