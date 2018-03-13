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
 * s2_classcmd.h
 *
 *  Created on: Oct 16, 2015
 *      Author: trasmussen
 */
 
#ifndef _S2_CLASSCMD_H_
#define _S2_CLASSCMD_H_

/**
 * \defgroup s2cls Constants used by libs2
 *
 * @{
 */
// NOTE: It is expected that those values should go into ZW_classcmd.h some way.
//       Naming of fields might in that case have minor changes.

/* Values used for Kex Report command */
//#define KEX_REPORT_PROPERTIES1_ECHO_BIT_MASK                                             0x01
//#define KEX_REPORT_PROPERTIES1_RESERVED_MASK                                             0xFE
//#define KEX_REPORT_PROPERTIES1_RESERVED_SHIFT                                            0x01

/* Values used for Kex Set command */
//#define KEX_SET_PROPERTIES1_ECHO_BIT_MASK                                                0x01
//#define KEX_SET_PROPERTIES1_RESERVED_MASK                                                0xFE
//#define KEX_SET_PROPERTIES1_RESERVED_SHIFT                                               0x01

/* Values used for Public Key Report command */
//#define PUBLIC_KEY_REPORT_PROPERTIES1_INCLUDING_NODE_BIT_MASK                            0x01
//#define PUBLIC_KEY_REPORT_PROPERTIES1_RESERVED_MASK                                      0xFE
//#define PUBLIC_KEY_REPORT_PROPERTIES1_RESERVED_SHIFT                                     0x01
/* Values used for Security 2 Transfer End command */
//#define SECURITY_2_TRANSFER_END_PROPERTIES1_KEY_REQUEST_COMPLETE_BIT_MASK                0x01
//#define SECURITY_2_TRANSFER_END_PROPERTIES1_KEY_VERIFIED_BIT_MASK                        0x02
//#define SECURITY_2_TRANSFER_END_PROPERTIES1_RESERVED_MASK                                0xFC
//#define SECURITY_2_TRANSFER_END_PROPERTIES1_RESERVED_SHIFT                               0x02

#define SECURITY_2_SCHEME_1_SUPPORT          0x02     //< Value denoting support for scheme 1 in the sheme support field.
#define SECURITY_2_SCHEME_SUPPORT_MASK       0x02     //< Mask containing all schemes.

#define SECURITY_2_SECURITY_2_CLASS_0        0x01     //< Value denoting support/request/grant for security 2 class 0 key.
#define SECURITY_2_SECURITY_2_CLASS_1        0x02     //< Value denoting support/request/grant for security 2 class 1 key.
#define SECURITY_2_SECURITY_2_CLASS_2        0x04     //< Value denoting support/request/grant for security 2 class 2 key.
#define SECURITY_2_SECURITY_0_NETWORK_KEY    0x80     //< Value denoting support/request/grant for security 0 network key.
#define SECURITY_2_KEY_2_MASK                0x07     //< Mask containing all valid settings for seurity 2 keys.
#define SECURITY_2_KEY_MASK                  0x87     //< Mask containing all keys.

#define KEX_REPORT_CURVE_25519               0x01     //< Value denoting curve25519 used in ECDH.
#define KEX_REPORT_CURVE_MASK                0x01     //< Mask containing all curves.
#define KEX_REPORT_CURVE_RESERVED_MASK       0xFE
#define KEX_REPORT_CURVE_RESERVED_SHIFT      0x01

#define SECURITY_2_ECHO_OFF                  0x00     //< Value denoting that this is not an echo frame.
#define SECURITY_2_ECHO_ON                   0x01     //< Value denoting that this is an echo frame.

#define SECURITY_2_CSA_ON                    0x02     //< Value denoting that csa was requested


#define SECURITY_2_JOINING_NODE              0x00     //< Value denoting that this frame originates from the joining node.
#define SECURITY_2_INCLUDING_NODE            0x01     //< Value denoting that this frame originates from the including node.

#define SECURITY_2_KEY_REQ_CONTINUE          0x00     //< Value denoting that joining node has not received all keys.
#define SECURITY_2_KEY_REQ_COMPLETE          0x01     //< Value denoting that joining node has received all requested keys.
#define SECURITY_2_KEY_VERIFIED              0x02     //< Value denoting that the verify frame could be successfully decrypted using the exchanged network key.

/** Following defines are related to security 2 command class frames. */
#define SECURITY_2_COMMAND_CLASS_POS         0 //< Position of the command class byte.
#define SECURITY_2_COMMAND_POS               1 //< Position of the command byte identifying the security 2 command.

/** Following defines are related to the KEX Set Frame. */
#define SECURITY_2_KEX_SET_ECHO_POS          2 //< Position of the echo field in the KEX set frame.
#define SECURITY_2_KEX_SET_CSA_POS          2 //< Position of the echo field in the KEX set frame.
#define SECURITY_2_KEX_SET_SCHEME_POS        3 //< Position of the echo field in the KEX set frame.
#define SECURITY_2_KEX_SET_CURVE_POS         4 //< Position of the echo field in the KEX set frame.
#define SECURITY_2_KEX_SET_KEYS_POS          5 //< Position of the echo field in the KEX set frame.


/** Following defines are related to the KEX Report Frame. Note: The KEX Report fields are identical to the KEX Set frame. */
#define SECURITY_2_KEX_REP_ECHO_POS          SECURITY_2_KEX_SET_ECHO_POS   //< Position of the echo field in the KEX report frame.
#define SECURITY_2_KEX_REP_SCHEME_POS        SECURITY_2_KEX_SET_SCHEME_POS //< Position of the echo field in the KEX report frame.
#define SECURITY_2_KEX_REP_CURVE_POS         SECURITY_2_KEX_SET_CURVE_POS  //< Position of the echo field in the KEX report frame.
#define SECURITY_2_KEX_REP_KEYS_POS          SECURITY_2_KEX_SET_KEYS_POS   //< Position of the echo field in the KEX report frame.

/** Following defines are related to the Public Key Frame set. */
#define SECURITY_2_PUB_KEY_INC_FLAG_POS      2 //< Position of the including node flag in the public key exchange frame.
#define SECURITY_2_PUB_KEY_KEY_POS           3 //< Position of the key in the public key exchange frame.

/** Following define is related to the Network Key Get Frame. */
#define SECURITY_2_NET_KEY_GET_REQ_KEY_POS   2 //< Position of the key requested field in the network key get frame.

/** Following defines are related to the network key report frame. */
#define SECURITY_2_NET_KEY_REP_GRANT_KEY_POS 2 //< Position of the key granted field in the network key report frame.
#define SECURITY_2_NET_KEY_REP_KEY_POS       3 //< Position of the key in the network key report frame.

/** Following define is related to the Network Transfer End Frame. */
#define SECURITY_2_TRANSFER_END_FLAGS_POS    2 //< Position of the flag field in the transfer end frame.

/** Following define is related to the KEX Fail frame. */
#define SECURITY_2_KEX_FAIL_FAIL_TYPE_POS    2 //< Position of the fail type field in the KEX fail frame.

/** Length definitions */
#define SECURITY_2_KEX_GET_LENGTH            2
#define SECURITY_2_KEX_REPORT_LENGTH         6
#define SECURITY_2_KEX_SET_LENGTH            6
#define SECURITY_2_PUB_KEY_LENGTH            35
#define SECURITY_2_NET_KEY_GET_LENGTH        3
#define SECURITY_2_NET_KEY_REPORT_LENGTH     19
#define SECURITY_2_NET_KEY_VERIFY_LENGTH     2
#define SECURITY_2_TRANSFER_END_LENGTH       3
#define SECURITY_2_KEX_FAIL_LENGTH           3

#define SECURITY_2_EC_PUBLIC_KEY_LENGTH      32

#define SECURITY_2_KEX_SET_RESERVED_MASK 0xFC //< Bitmask of the reserved bits in Kex Set

/**
 * @}
 */
#endif // _S2_CLASSCMD_H_
