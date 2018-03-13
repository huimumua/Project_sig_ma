/**
@file   zip_error.h - Z/IP interface error code header file.

        Definition of error codes.

@author David Chow

@version    1.0 7-6-11  Initial release

@copyright © 2014 SIGMA DESIGNS, INC. THIS IS AN UNPUBLISHED WORK PROTECTED BY SIGMA DESIGNS, INC.
AS A TRADE SECRET, AND IS NOT TO BE USED OR DISCLOSED EXCEPT AS PROVIDED Z-WAVE CONTROLLER DEVELOPMENT KIT
LIMITED LICENSE AGREEMENT. ALL RIGHTS RESERVED.

NOTICE: ALL INFORMATION CONTAINED HEREIN IS CONFIDENTIAL AND/OR PROPRIETARY TO SIGMA DESIGNS
AND MAY BE COVERED BY U.S. AND FOREIGN PATENTS, PATENTS IN PROCESS, AND ARE PROTECTED BY TRADE SECRET
OR COPYRIGHT LAW. DISSEMINATION OR REPRODUCTION OF THE SOURCE CODE CONTAINED HEREIN IS EXPRESSLY FORBIDDEN
TO ANYONE EXCEPT LICENSEES OF SIGMA DESIGNS  WHO HAVE EXECUTED A SIGMA DESIGNS' Z-WAVE CONTROLLER DEVELOPMENT KIT
LIMITED LICENSE AGREEMENT. THE COPYRIGHT NOTICE ABOVE IS NOT EVIDENCE OF ANY ACTUAL OR INTENDED PUBLICATION OF
THE SOURCE CODE. THE RECEIPT OR POSSESSION OF  THIS SOURCE CODE AND/OR RELATED INFORMATION DOES NOT CONVEY OR
IMPLY ANY RIGHTS  TO REPRODUCE, DISCLOSE OR DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE, USE, OR SELL A PRODUCT
THAT IT  MAY DESCRIBE.


THE SIGMA PROGRAM AND ANY RELATED DOCUMENTATION OR TOOLS IS PROVIDED TO COMPANY "AS IS" AND "WITH ALL FAULTS",
WITHOUT WARRANTY OF ANY KIND FROM SIGMA. COMPANY ASSUMES ALL RISKS THAT LICENSED MATERIALS ARE SUITABLE OR ACCURATE
FOR COMPANY'S NEEDS AND COMPANY'S USE OF THE SIGMA PROGRAM IS AT COMPANY'S OWN DISCRETION AND RISK. SIGMA DOES NOT
GUARANTEE THAT THE USE OF THE SIGMA PROGRAM IN A THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICES ENVIRONMENT WILL BE:
(A) PERFORMED ERROR-FREE OR UNINTERRUPTED; (B) THAT SIGMA WILL CORRECT ANY THIRD PARTY SERVICE ENVIRONMENT OR
CLOUD SERVICE ENVIRONMENT ERRORS; (C) THE THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICE ENVIRONMENT WILL
OPERATE IN COMBINATION WITH COMPANY'S CONTENT OR COMPANY APPLICATIONS THAT UTILIZE THE SIGMA PROGRAM;
(D) OR WITH ANY OTHER HARDWARE, SOFTWARE, SYSTEMS, SERVICES OR DATA NOT PROVIDED BY SIGMA. COMPANY ACKNOWLEDGES
THAT SIGMA DOES NOT CONTROL THE TRANSFER OF DATA OVER COMMUNICATIONS FACILITIES, INCLUDING THE INTERNET, AND THAT
THE SERVICES MAY BE SUBJECT TO LIMITATIONS, DELAYS, AND OTHER PROBLEMS INHERENT IN THE USE OF SUCH COMMUNICATIONS
FACILITIES. SIGMA IS NOT RESPONSIBLE FOR ANY DELAYS, DELIVERY FAILURES, OR OTHER DAMAGE RESULTING FROM SUCH ISSUES.
SIGMA IS NOT RESPONSIBLE FOR ANY ISSUES RELATED TO THE PERFORMANCE, OPERATION OR SECURITY OF THE THIRD PARTY SERVICE
ENVIRONMENT OR CLOUD SERVICES ENVIRONMENT THAT ARISE FROM COMPANY CONTENT, COMPANY APPLICATIONS OR THIRD PARTY CONTENT.
SIGMA DOES NOT MAKE ANY REPRESENTATION OR WARRANTY REGARDING THE RELIABILITY, ACCURACY, COMPLETENESS, CORRECTNESS, OR
USEFULNESS OF THIRD PARTY CONTENT OR SERVICE OR THE SIGMA PROGRAM, AND DISCLAIMS ALL LIABILITIES ARISING FROM OR RELATED
TO THE SIGMA PROGRAM OR THIRD PARTY CONTENT OR SERVICES. TO THE EXTENT NOT PROHIBITED BY LAW, THESE WARRANTIES ARE EXCLUSIVE.
SIGMA OFFERS NO WARRANTY OF NON-INFRINGEMENT, TITLE, OR QUIET ENJOYMENT. NEITHER SIGMA NOR ITS SUPPLIERS OR LICENSORS
SHALL BE LIABLE FOR ANY INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES OR LOSS (INCLUDING DAMAGES FOR LOSS OF
BUSINESS, LOSS OF PROFITS, OR THE LIKE), ARISING OUT OF THIS AGREEMENT WHETHER BASED ON BREACH OF CONTRACT,
INTELLECTUAL PROPERTY INFRINGEMENT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY, PRODUCT LIABILITY OR OTHERWISE,
EVEN IF SIGMA OR ITS REPRESENTATIVES HAVE BEEN ADVISED OF OR OTHERWISE SHOULD KNOW ABOUT THE POSSIBILITY OF SUCH DAMAGES.
THERE ARE NO OTHER EXPRESS OR IMPLIED WARRANTIES OR CONDITIONS INCLUDING FOR SOFTWARE, HARDWARE, SYSTEMS, NETWORKS OR
ENVIRONMENTS OR FOR MERCHANTABILITY, NONINFRINGEMENT, SATISFACTORY QUALITY AND FITNESS FOR A PARTICULAR PURPOSE.

The Sigma Program  is not fault-tolerant and is not designed, manufactured or intended for use or resale as on-line control
equipment in hazardous environments requiring fail-safe performance, such as in the operation of nuclear facilities,
aircraft navigation or communication systems, air traffic control, direct life support machines, or weapons systems,
in which the failure of the Sigma Program, or Company Applications created using the Sigma Program, could lead directly
to death, personal injury, or severe physical or environmental damage ("High Risk Activities").  Sigma and its suppliers
specifically disclaim any express or implied warranty of fitness for High Risk Activities.Without limiting Sigma's obligation
of confidentiality as further described in the Z-Wave Controller Development Kit Limited License Agreement, Sigma has no
obligation to establish and maintain a data privacy and information security program with regard to Company's use of any
Third Party Service Environment or Cloud Service Environment. For the avoidance of doubt, Sigma shall not be responsible
for physical, technical, security, administrative, and/or organizational safeguards that are designed to ensure the
security and confidentiality of the Company Content or Company Application in any Third Party Service Environment or
Cloud Service Environment that Company chooses to utilize.
*/

#ifndef _ZIP_ERROR_DAVID_
#define _ZIP_ERROR_DAVID_

#define     DEBUG_ZIPAPI     ///< Enable debug message
#define     DEBUG_PORTAL     ///< Enable debug portal message

/// Error codes definition
#define     ZWHCI_NO_ERROR                     0 ///< No error
#define     ZWHCI_ERROR_MEMORY              -100 ///< Out of memory
#define     ZWHCI_ERROR_RESOURCE            -101 ///< Out of resource
#define     TRANSPORT_ERROR_SIGNALING       -102 ///< Could not signal write thread to perform actual write
#define     FRAME_ERROR_MULTIPLE_WRITE      -103 ///< Write while the previous write in progress is not allowed
#define     FRAME_ERROR_SEND_TIMER          -104 ///< Send timer not functioning
#define     SESSION_ERROR_PREVIOUS_COMMAND_UNCOMPLETED  -105 ///< Previous command uncompleted, retry later
#define     SESSION_ERROR_DEST_BUSY         -106 ///< The message has not timed out yet.The destination host may have a long response time
#define     SESSION_ERROR_UNREACHABLE       -107 ///< Frame failed to reach destination host
#define     SESSION_ERROR_SND_FRM_TMOUT     -108 ///< Send frame timeout due to no ACK received
#define     SESSION_ERROR_SYSTEM            -109 ///< System error, the program should exit.
#define     SESSION_ERROR_INVALID_RESP      -110 ///< The response command id doesn't match with the sent command id
#define     SESSION_ERROR_SEND_BUSY         -111 ///< Send error at lower layer due to controller busy
#define     APPL_TX_STATUS_TIMEOUT          -112 ///< There is no transmit status callback from lower layer
#define     APPL_OPER_ALREADY_ACTIVE        -113 ///< The requested operation is already active
#define     APPL_OPER_NOT_STARTED           -114 ///< The requested operation fails to start
#define     APPL_ERROR_WAIT_CB              -115 ///< Waiting for transmit complete callback function, retry later
#define     APPL_INVALID_ADDR               -116 ///< Invalid IPv6 address
#define     APPL_INVALID_NODE_ID            -117 ///< Invalid node id
#define     APPL_ERROR_RSLV_NODE_ID         -118 ///< Couldn't resolve node id to IP address
#define     ZWHCI_ERROR_TOO_LARGE           -119 ///< The value supplied is too large
#define     ZWHCI_ERROR_WRITE               -120 ///< Write failed
#define     ZWHCI_ERROR_READ                -121 ///< Read failed
#define     ZWHCI_ERROR_TIMEOUT             -122 ///< Time out
#define     ZWHCI_ERROR_VERIFY              -123 ///< Verification failed
#define     ZWHCI_ERROR_SHUTDOWN            -124 ///< System is shutting down

// Initialization error code
#define     INIT_ERROR_TRANSPORT        1   ///< Initialization error on transport layer
#define     INIT_ERROR_FRAME            2   ///< Initialization error on frame layer
#define     INIT_ERROR_SESSION          3   ///< Initialization error on session layer
#define     INIT_ERROR_APPL             4   ///< Initialization error on application layer

#ifdef  DEBUG_ZIPAPI
#define     debug_msg_show      plt_msg_show    ///< show message to the user
#define     debug_msg_ts_show   plt_msg_ts_show ///< show message to the user with time stamp
#define     debug_bin_show      plt_bin_show    ///< show binary string to the user
#define     debug_gw_discvr     printf          ///< show gateway discovery debug message
#else
#define     debug_msg_show(...)
#define     debug_msg_ts_show(...)
#define     debug_bin_show(...)
#define     debug_gw_discvr(...)
#endif

#ifdef  DEBUG_PORTAL
#define     debug_tls_svr       printf          ///< show TLS debug message

#include "openssl/err.h"
		#define 	debug_tls_svr_err(errorcode)		\
		{\
			debug_tls_svr("Error: %s\n", ERR_error_string(errorcode, NULL));\
		}

#else
    #define     debug_tls_svr(...)
	#define 	debug_tls_svr_err(errorcode)
#endif

#endif /* _ZIP_ERROR_DAVID_ */
