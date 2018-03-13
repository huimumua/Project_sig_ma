/**
@file   zip_api.h - Z/IP host controller API header file.

        Z/IP API public definitions.

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

#ifndef _ZIP_API_DAVID_
#define _ZIP_API_DAVID_

//--------------- testing control ---------------------------------
//#define  TEST_HANDLE_BASIC_COMMAND_CLASS
//#define  TEST_EXT_CMD_CLASS

//-----------------------------------------------------------------
#define     ZW_CONTROLLER           ///<For controller
#define     REPLACE_FAILED          ///<For failed node replacement
//#define     IGNORE_INIT_ERROR       ///<Ignore initialization error (for testing purposes only)
#include "zip_application.h"
#include "zwave/ZW_transport_api.h"
#include "zwave/ZW_classcmd.h"
#include "../include/cjson.h"

#include <android/log.h>

#define LOG_TAG "ZWCONTROL"

#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

#define  ZIP_API            ///<For Z/IP controller implementation

#define  ZW_PLUS            ///<To support Z-wave+

#define  CRC16_ENCAP        ///<CRC-16 encapsulation

#define HIDE_EP0_FUNC_IF	            ///<To enable hiding of functional interfaces in ep0 for multi-channel slave device
#ifdef TCP_PORTAL
	#define HIDE_FW_UPD_IF_FOREIGN_NODE	///<To enable hiding of COMMAND_CLASS_FIRMWARE_UPDATE_MD in all foreign nodes for portal version
#endif
#define ALARM_2_DLOCK_RPT_CONVERSION	///<To enable alarm report to doorlock report conversion

//#define USE_IP_ASSOCIATION              ///<To support use of IP Association CC
//#define MAP_IP_ASSOC_TO_ASSOC           ///<To map IP assoc CC to assoc CC (Temporary measure while waiting Z/IP gateway new code)

#define DEBUG_HCAPI         ///<Print debug messages

#ifdef  DEBUG_HCAPI
#define     debug_zwapi_msg         plt_msg_show        ///< Print debug message
#define     debug_zwapi_ts_msg      plt_msg_ts_show     ///< Print debug message with time
#define     debug_zwapi_bin_msg     plt_bin_show        ///< Print binary debug message
#else
#define     debug_zwapi_msg(...)
#define     debug_zwapi_ts_msg(...)
#define     debug_zwapi_bin_msg(...)
#endif


#define APPL_VERSION        8   ///<Application version
#define APPL_SUBVERSION     12  ///<Application sub-version

#ifndef _ZIP_API_PTE_DAVID_
struct _zwnet;
typedef struct _zwnet	*zwnet_p;///<Opaque network handle
#endif

#define ZW_DEV_LOC_STR_MAX	16  ///<Actual device name & location maximum string length
#define ZW_LOC_STR_MAX	    32  ///<Endpoint name & location maximum string length

struct  _zwnoded;
struct  _zwepd;
struct  _zwifd;
typedef struct _zwnoded	    *zwnoded_p;		/**<Node descriptor */
typedef struct _zwepd	    *zwepd_p;		/**<Endpoint descriptor */
typedef struct _zwifd	    *zwifd_p;		/**<Interface descriptor */


/**
@defgroup Net Network APIs
A Z-Wave Network is seen through a local controller node which can manage
control and data in the mesh network.
@ingroup zwarecapi
@{
*/


/** @name Zw_error_codes
* Z-Wave error code definition
@{
*/
#define ZW_ERR_CACHE_AVAIL        2    /**< The data is available in the cache (no live get from the target device) */
#define ZW_ERR_QUEUED			  1	   /**< The command is queued (no transmission status callback) */
#define ZW_ERR_NONE				  0    /**< Operation succeeded */
#define ZW_ERR_FAILED			 -1	   /**< Operation failed */
#define ZW_ERR_WRONG_IF         -10    /**< Wrong interface */
#define ZW_ERR_NO_RESP          -11    /**< No response from controller */
#define ZW_ERR_MEMORY           -12    /**< Out of memory */
#define ZW_ERR_NODE_NOT_FOUND   -13    /**< Node not found*/
#define ZW_ERR_CLASS_NOT_FOUND  -14    /**< Command class not found*/
#define ZW_ERR_INTF_NOT_FOUND   -15    /**< Interface to a class not found*/
#define ZW_ERR_INTF_NO_REP_HDLR -16    /**< Interface report handler not found*/
#define ZW_ERR_LAST_OP_NOT_DONE -17    /**< Last operation uncompleted yet, try again*/
#define ZW_ERR_NOT_IN_LIST      -18    /**< Node not in protocol layer failed node ID list*/
#define ZW_ERR_OP_FAILED        -19    /**< The requested operation failed*/
#define ZW_ERR_EP_NOT_FOUND     -20    /**< Endpoint not found*/
#define ZW_ERR_RPT_NOT_FOUND    -21    /**< The report command of an interface not found*/
#define ZW_ERR_NET_NOT_FOUND    -22    /**< Network not found*/
#define ZW_ERR_CMD_VERSION      -23    /**< Incorrect command class version*/
#define ZW_ERR_PENDING			-24	   /**< Operation pending, it cannot be canceled now */
#define ZW_ERR_VALUE			-25	   /**< The value is invalid */
#define ZW_ERR_QUEUE_FULL		-26	   /**< The queue is full */
#define ZW_ERR_UNSUPPORTED		-27	   /**< The requested function is unsupported for this node */
#define ZW_ERR_FILE_OPEN	    -28	   /**< Open file error */
#define ZW_ERR_FILE_WRITE	    -29	   /**< Write file error */
#define ZW_ERR_FILE_EOF	        -30	   /**< The end-of-file was reached*/
#define ZW_ERR_FILE	            -31	   /**< File is corrupted */
#define ZW_ERR_FILE_HOME_ID     -32	   /**< File home id doesn't match */
#define ZW_ERR_EXPIRED          -33    /**< Expired */
#define ZW_ERR_NO_RES           -34    /**< No resource for mutex, semaphore, timer, etc */
#define ZW_ERR_EVENT            -35    /**< Event is not processed */
#define ZW_ERR_TOO_LARGE        -36    /**< Data size is too large */
#define ZW_ERR_TOO_SMALL        -37    /**< Data size is too small */
#define ZW_ERR_TIMEOUT          -38    /**< Timeout */
#define ZW_ERR_TRANSMIT         -39    /**< Transmission failed */
#define ZW_ERR_NONCE_NOT_FOUND  -40    /**< Security nonce not found */
#define ZW_ERR_AUTH             -41    /**< Authentication error */
#define ZW_ERR_SEQ_NUMBER       -42    /**< Incorrect sequence number */
#define ZW_ERR_BUSY             -43    /**< Busy, try again later */
#define ZW_ERR_SEC_SCHEME       -44    /**< Security scheme unsupported */
#define ZW_ERR_TRANSPORT_INI    -45    /**< Initialization error on transport layer */
#define ZW_ERR_FRAME_INI        -46    /**< Initialization error on frame layer */
#define ZW_ERR_SESSION_INI      -47    /**< Initialization error on session layer */
#define ZW_ERR_APPL_INI         -48    /**< Initialization error on application layer */
#define ZW_ERR_UNEXPECTED       -49    /**< The error was unexpected under normal circumstances*/
#define ZW_ERR_NETWORK_IF       -50    /**< Network interface not configured properly */
#define ZW_ERR_IP_ADDR			-51	   /**< The IP address is invalid */
#define ZW_ERR_VERSION          -52    /**< Wrong version number*/
#define ZW_ERR_INTF_NO_DATA     -53    /**< Interface data is missing*/
#define ZW_ERR_FILE_READ	    -54	   /**< Read file error */
#define ZW_ERR_PARSE_FILE	    -55	   /**< Parsing file failed */
#define ZW_ERR_MISSING_ENTRY	-56	   /**< Missing mandatory entry */
#define ZW_ERR_DEVCFG_NOT_FOUND	-57	   /**< Device specific configuration record not found */
#define ZW_ERR_DISALLOWED       -58    /**< The operation is disallowed under certain circumstances*/
#define ZW_ERR_PSK_TOO_SHORT    -59    /**< DTLS pre-shared key length is too short*/
#define ZW_ERR_NO_CACHE_AVAIL   -60    /**< The data is unavailable in the cache */
#define ZW_ERR_NOT_APPLICABLE   -61    /**< Not applicable and should be skipped or ignored */
#define ZW_ERR_SHUTDOWN         -62    /**< The system is shutting down */
#define ZW_ERR_POST_SET_POLL    -63    /**< Post-set poll error */
#define ZW_ERR_ORDER            -64    /**< Out of order */
#define ZW_ERR_CMD_UNSUPP_TGT   -65    /**< Command sent by the association group is unsupported at target node or endpoint */

/**
@}
*/


/** @name Zw_if_cmd_id
* Z-Wave interface command id for command queuing
@{
*/
#define ZW_CID_BSENSOR_RPT_GET	  1	   /**< zwif_bsensor_get */
#define ZW_CID_CONFIG_RPT_GET	  2	   /**< zwif_config_get */
#define ZW_CID_CONFIG_SET	      3	   /**< zwif_config_set */
#define ZW_CID_GRP_ACTV_GET	      4	   /**< zwif_group_actv_get */
#define ZW_CID_GRP_ADD	          5	   /**< zwif_group_add */
#define ZW_CID_GRP_CMD_GET	      6	   /**< zwif_group_cmd_get */
#define ZW_CID_GRP_CMD_SET	      7	   /**< zwif_group_cmd_set */
#define ZW_CID_GRP_CMD_SUP_GET    8	   /**< zwif_group_cmd_sup_get */
#define ZW_CID_GRP_DEL	          9	   /**< zwif_group_del */
#define ZW_CID_GRP_GET	         10	   /**< zwif_group_get */
#define ZW_CID_GRP_SUP_GET       11	   /**< zwif_group_sup_get */
#define ZW_CID_LEVEL_RPT_GET	 12	   /**< zwif_level_get */
#define ZW_CID_LEVEL_SET	     13	   /**< zwif_level_set */
#define ZW_CID_LEVEL_START	     14	   /**< zwif_level_start */
#define ZW_CID_LEVEL_STOP	     15	   /**< zwif_level_stop */
#define ZW_CID_LEVEL_SUP_GET     16	   /**< zwif_level_sup_get */
#define ZW_CID_NAME_LOC_SET	     17	   /**< zwif_nameloc_set */
#define ZW_CID_SENSOR_RPT_GET	 18	   /**< zwif_sensor_get */
#define ZW_CID_SWITCH_RPT_GET	 19	   /**< zwif_switch_get */
#define ZW_CID_SWITCH_SET	     20	   /**< zwif_switch_set */
#define ZW_CID_WAKE_UP_GET       21	   /**< zwif_wakeup_get */
#define ZW_CID_WAKE_UP_SET	     22	   /**< zwif_wakeup_set */
#define ZW_CID_METER_DESC_GET    23	   /**< zwif_meter_get_desc */
#define ZW_CID_METER_RESET       24	   /**< zwif_meter_reset */
#define ZW_CID_METER_RPT_GET     25	   /**< zwif_meter_get */
#define ZW_CID_METER_SUP_GET     26	   /**< zwif_meter_sup_get */
#define ZW_CID_METER_ADMIN_SET   27	   /**< zwif_meter_set_admin */
#define ZW_CID_AV_CAP_GET        28	   /**< zwif_av_caps */
#define ZW_CID_AV_SET            29	   /**< zwif_av_set */
#define ZW_CID_PULSE_METER_RPT_GET     30	   /**< zwif_pulsemeter_get */
#define ZW_CID_BASIC_RPT_GET	 31	   /**< zwif_level_get */
#define ZW_CID_BASIC_SET	     32	   /**< zwif_level_set */
#define ZW_CID_DL_OP_SET	     33	   /**< zwif_dlck_op_set */
#define ZW_CID_DL_OP_RPT_GET	 34	   /**< zwif_dlck_op_get */
#define ZW_CID_DL_CFG_SET	     35	   /**< zwif_dlck_cfg_set */
#define ZW_CID_DL_CFG_RPT_GET	 36	   /**< zwif_dlck_cfg_get */
#define ZW_CID_USRCOD_SET	     37	   /**< zwif_usrcod_set */
#define ZW_CID_USRCOD_GET	     38	   /**< zwif_usrcod_get */
#define ZW_CID_USRCOD_SUP_GET    39	   /**< zwif_usrcod_sup_get */
#define ZW_CID_ALRM_RPT_GET	     40	   /**< zwif_alrm_get */
#define ZW_CID_ALRM_SET	         41	   /**< zwif_alrm_set */
#define ZW_CID_ALRM_SUP_GET      42	   /**< zwif_alrm_sup_get */
#define ZW_CID_BATTERY_RPT_GET	 43	   /**< zwif_battery_get */
#define ZW_CID_THRMO_FAN_MD_GET	 44	   /**< zwif_thrmo_fan_md_get */
#define ZW_CID_THRMO_FAN_MD_SET	 45	   /**< zwif_thrmo_fan_md_set */
#define ZW_CID_THRMO_FAN_MD_SUP_GET      46	   /**< zwif_thrmo_fan_md_sup_get */
#define ZW_CID_THRMO_FAN_STA_GET 47	   /**< zwif_thrmo_fan_sta_get */
#define ZW_CID_THRMO_MD_GET	     48	   /**< zwif_thrmo_md_get */
#define ZW_CID_THRMO_MD_SET	     49	   /**< zwif_thrmo_md_set */
#define ZW_CID_THRMO_MD_SUP_GET  50	   /**< zwif_thrmo_md_sup_get */
#define ZW_CID_THRMO_OP_STA_GET  51	   /**< zwif_thrmo_op_sta_get */
#define ZW_CID_THRMO_SETB_GET	 52	   /**< zwif_thrmo_setb_get */
#define ZW_CID_THRMO_SETB_SET	 53	   /**< zwif_thrmo_setb_set */
#define ZW_CID_THRMO_SETP_GET	 54	   /**< zwif_thrmo_setp_get */
#define ZW_CID_THRMO_SETP_SET	 55	   /**< zwif_thrmo_setp_set */
#define ZW_CID_THRMO_SETP_SUP_GET       56	   /**< zwif_thrmo_setp_sup_get */
#define ZW_CID_CLOCK_RPT_GET	 57	   /**< zwif_clock_get */
#define ZW_CID_CLOCK_SET	     58	   /**< zwif_clock_set */
#define ZW_CID_CLMT_CTL_SCHD_GET 59	   /**< zwif_clmt_ctl_schd_get */
#define ZW_CID_CLMT_CTL_SCHD_SET 60	   /**< zwif_clmt_ctl_schd_set */
#define ZW_CID_CLMT_CTL_SCHD_CHG_GET    61	   /**< zwif_clmt_ctl_schd_chg_get */
#define ZW_CID_CLMT_CTL_SCHD_OVR_GET    62	   /**< zwif_clmt_ctl_schd_ovr_get */
#define ZW_CID_CLMT_CTL_SCHD_OVR_SET    63	   /**< zwif_clmt_ctl_schd_ovr_set */
#define ZW_CID_WKUP_NO_INFO      64	   /**< wake up no more info (for internal use)*/
#define ZW_CID_PROT_GET	         65	   /**< zwif_prot_get */
#define ZW_CID_PROT_SET	         66	   /**< zwif_prot_set */
#define ZW_CID_PROT_SUP_GET      67	   /**< zwif_prot_sup_get */
#define ZW_CID_PROT_EC_GET	     68	   /**< zwif_prot_ec_get */
#define ZW_CID_PROT_EC_SET	     69	   /**< zwif_prot_ec_set */
#define ZW_CID_PROT_TIMEOUT_GET	 70	   /**< zwif_prot_tmout_get */
#define ZW_CID_PROT_TIMEOUT_SET	 71	   /**< zwif_prot_tmout_set */
#define ZW_CID_INDICATOR_GET	 72	   /**< zwif_ind_get */
#define ZW_CID_INDICATOR_SET	 73	   /**< zwif_ind_set */
#define ZW_CID_SENSOR_SUP_GET    74	   /**< zwif_sensor_sup_get */
#define ZW_CID_SENSOR_UNIT_GET   75	   /**< zwif_sensor_unit_get */
#define ZW_CID_FW_INFO_GET       76	   /**< zwif_fw_info_get */
#define ZW_CID_FW_UPDT_GET       77	   /**< zwif_fw_updt_req */
#define ZW_CID_GW_MODE_SET       78	   /**< zwif_gw_mode_set */
#define ZW_CID_GW_CONF_LOCK      79	   /**< zwif_gw_cfg_lock */
#define ZW_CID_GW_MODE_GET       80	   /**< zwif_gw_mode_get */
#define ZW_CID_GW_UNSOLICIT_SET  81	   /**< zwif_gw_unsolicit_set */
#define ZW_CID_GW_UNSOLICIT_GET  82	   /**< zwif_gw_unsolicit_get */
#define ZW_CID_GW_CFG_SET        83	   /**< zwif_gw_cfg_set */
#define ZW_CID_GW_CFG_GET        84	   /**< zwif_gw_cfg_get */
#define ZW_CID_POWER_LVL_GET	 85	   /**< zwif_power_level_get */
#define ZW_CID_POWER_LVL_TST_GET 86	   /**< zwif_power_level_test_get */
#define ZW_CID_POWER_LVL_SET     87	   /**< zwif_power_level_set */
#define ZW_CID_POWER_LVL_TST_SET 88	   /**< zwif_power_level_test_set */
#define ZW_CID_ALRM_SUP_EVT_GET  89	   /**< zwif_alrm_sup_evt_get */
#define ZW_CID_BSENSOR_SUP_GET   90	   /**< zwif_bsensor_sup_get */
#define ZW_CID_GW_WIFI_CFG_SET   91	   /**< zwif_gw_wifi_cfg_set */
#define ZW_CID_GW_WIFI_CFG_GET   92	   /**< zwif_gw_wifi_cfg_get */
#define ZW_CID_GW_WIFI_SEC_GET   93	   /**< zwif_gw_wifi_sec_sup_get */
#define ZW_CID_CSC_SUP_GET		 94	   /**< zwif_csc_sup_get */
#define ZW_CID_ALRM_SNSR_RPT_GET 95	   /**< zwif_alrm_snsr_get */
#define ZW_CID_ALRM_SNSR_SUP_GET 96	   /**< zwif_alrm_snsr_sup_get */
#define ZW_CID_BARRIER_SET       97	   /**< zwif_barrier_set */
#define ZW_CID_BARRIER_GET       98	   /**< zwif_barrier_get */
#define ZW_CID_BARRIER_NOTIF_SET 99	   /**< zwif_barrier_notif_cfg_set */
#define ZW_CID_BARRIER_NOTIF_GET 100   /**< zwif_barrier_notif_cfg_get */
#define ZW_CID_BARRIER_SUP_GET   101   /**< zwif_barrier_notif_sup_get */
#define ZW_CID_SWITCH_COLOR_RPT_GET	 102   /**< zwif_color_sw_get */
#define ZW_CID_SWITCH_COLOR_SET	     103   /**< zwif_color_sw_set */
#define ZW_CID_SWITCH_COLOR_START	 104   /**< zwif_color_sw_start */
#define ZW_CID_SWITCH_COLOR_STOP	 105   /**< zwif_color_sw_stop */
#define ZW_CID_SWITCH_COLOR_SUP_GET  106   /**< zwif_color_sw_sup_get */
#define ZW_CID_THRMO_SETP_RANGE_GET  107   /**< zwif_thrmo_setp_sup_range_get */
#define ZW_CID_CSC_CONFIG_SET	     108   /**< zwif_csc_cfg_set */
#define ZW_CID_CSC_CONFIG_GET	     109   /**< zwif_csc_cfg_get */
#define ZW_CID_THRMO_OP_STA_SUP_LOG_GET  110    /**< zwif_thrmo_op_sta_log_sup_get */
#define ZW_CID_THRMO_OP_STA_LOG_GET  111   /**< zwif_thrmo_op_sta_log_get */
#define ZW_CID_CFG_BULK_SET	      112	   /**< zwif_config_bulk_set */
#define ZW_CID_CFG_BULK_RPT_GET	  113	   /**< zwif_config_bulk_get */
#define ZW_CID_CFG_PARAM_RESET	  114	   /**< zwif_config_prm_reset */

/****************************************************************************/
// skysoft modified start
#define ZW_CID_NOTIFICATION_RPT_GET	     115	   /**< zwif_notification_get */
#define ZW_CID_NOTIFICATION_SET	         116	   /**< zwif_notification_set */
#define ZW_CID_NOTIFICATION_SUP_GET      117	   /**< zwif_notification_sup_get */
#define ZW_CID_NOTIFICATION_SUP_EVT_GET  118	   /**< zwif_notification_sup_evt_get */
#define ZW_CID_SWITCH_ALL_ON     119	   /**< zwif_switch_all_on */
#define ZW_CID_SWITCH_ALL_OFF    120       /**< zwif_switch_all_off */
#define ZW_CID_SWITCH_ALL_SET    121       /**< zwif_switch_all_set */
#define ZW_CID_SWITCH_ALL_GET    122       /**< zwif_switch_all_get */
#define ZW_CID_SCENE_ACT_CONF_GET        123       /**< zwif_scene_actuator_conf_get */
#define ZW_CID_SCENE_ACT_CONF_SET        124       /**< zwif_scene_actuator_conf_set */
#define ZW_CID_MUL_CMD                   125       /**< zwif_multi_cmd_encap */
#define ZW_CID_NET_RSSI          126       /**< zwif_network_rssi_rep_set */

// skysoft modified end
/****************************************************************************/

#define ZW_CID_POLL              0xFFFF /**< The command is meant fo polling purposes only */
/**
@}
*/


/** @name Device_categories
* Device Categories definition
@{
*/
#define DEV_CATEGORY_UNKNOWN        0	   /**< Unknown or unassigned category */
#define DEV_SENSOR_ALARM            1	   /**< Sensor alarm */
#define DEV_ON_OFF_SWITCH           2	   /**< On/off switch */
#define DEV_POWER_STRIP             3	   /**< Power strip */
#define DEV_SIREN                   4	   /**< Siren */
#define DEV_VALVE                   5	   /**< Valve */
#define DEV_SIMPLE_DISPLAY          6	   /**< Simple display */
#define DEV_DOORLOCK_KEYPAD         7	   /**< Door lock with keypad */
#define DEV_SUB_ENERGY_METER        8	   /**< Sub energy meter */
#define DEV_ADV_WHL_HOME_ENER_METER 9	   /**< Advanced whole home energy meter */
#define DEV_SIM_WHL_HOME_ENER_METER 10	   /**< Simple whole home energy meter */
#define DEV_SENSOR                  11	   /**< Sensor */
#define DEV_LIGHT_DIMMER            12	   /**< Light dimmer switch */
#define DEV_WIN_COVERING_NO_POS     13	   /**< Window covering no position/endpoint */
#define DEV_WIN_COVERING_EP         14	   /**< Window covering end point aware */
#define DEV_WIN_COVERING_POS_EP     15	   /**< Window covering position/end point aware */
#define DEV_FAN_SWITCH              16	   /**< Fan switch */
#define DEV_RMT_CTL_MULTIPURPOSE    17	   /**< Remote control - multipurpose */
#define DEV_RMT_CTL_AV              18	   /**< Remote control - AV */
#define DEV_RMT_CTL_SIMPLE          19	   /**< Remote control - simple */
#define DEV_UNRECOG_GATEWAY         20	   /**< Gateway (unrecognized by client) */
#define DEV_CENTRAL_CTLR            21	   /**< Central controller */
#define DEV_SET_TOP_BOX             22	   /**< Set top box */
#define DEV_TV                      23	   /**< TV */
#define DEV_SUB_SYS_CTLR            24	   /**< Sub system controller */
#define DEV_GATEWAY                 25	   /**< Gateway */
#define DEV_THERMOSTAT_HVAC         26	   /**< Thermostat - HVAC */
#define DEV_THERMOSTAT_SETBACK      27	   /**< Thermostat - setback */
#define DEV_WALL_CTLR               28	   /**< Wall controller */
/**
@}
*/


/** Z/IP network management definition */
#define NODE_ADD_STATUS_SECURITY_FAILED     9   /**< Node has been included but the secure inclusion failed.*/


/** @name Network_operation
* Network operation definition
@{
*/
#define ZWNET_OP_NONE           0   /**< No operation is executing*/
#define ZWNET_OP_INITIALIZE     1   /**< Initialization operation*/
#define ZWNET_OP_ADD_NODE       2   /**< Add node operation*/
#define ZWNET_OP_RM_NODE        3   /**< Remove node operation*/
#define ZWNET_OP_RP_NODE        4   /**< Replace failed node operation*/
#define ZWNET_OP_RM_FAILED_ID   5   /**< Remove failed node id operation*/
#define ZWNET_OP_INITIATE       6   /**< Initiation operation by controller*/
#define ZWNET_OP_UPDATE         7   /**< Update network topology from the SUC/SIS*/
#define ZWNET_OP_RESET          8   /**< Restore to factory default setting*/
#define ZWNET_OP_MIGRATE_SUC    9   /**< Create primary controller by a SUC*/
#define ZWNET_OP_MIGRATE        10  /**< Migrate primary controller operation*/
#define ZWNET_OP_ASSIGN         11  /**< assign or deassign SUC/SIS operation*/
//#define ZWNET_OP_LOAD_NW_INFO   12  /**< Load detailed network and node info operation*/
#define ZWNET_OP_NODE_UPDATE    13  /**< Update node info*/
#define ZWNET_OP_SEND_NIF       14  /**< Send node info frame*/
#define ZWNET_OP_NW_CHANGED     15  /**< Network change detection*/
#define ZWNET_OP_NODE_CACHE_UPT 16  /**< Update node cache info. (For internal use only)*/
#define ZWNET_OP_SAVE_NW        17  /**< Save network and node information to persistent storage. (For internal use only)*/
#define ZWNET_OP_SLEEP_NODE_UPT 18  /**< Update sleeping detailed node information when it is awake. (For internal use only)*/
#define ZWNET_OP_FW_UPDT        19  /**< Firmware update*/
#define ZWNET_OP_HEALTH_CHK     20  /**< Network health check*/

/**
@}
*/

/** Final progress status for all operations */
#define     OP_DONE                     0       /**< Operation completed*/
#define     OP_FAILED                   0x00FF  /**< Operation failed*/

/** Progress status of all getting node information */
#define     OP_GET_NI_TOTAL_NODE_MASK   0xFF00  /**< Mask to get the total number of nodes*/
#define     OP_GET_NI_NODE_CMPLT_MASK   0x00FF  /**< Mask to get the number of completed nodes*/

/** Progress status of adding node and migrating primary controller */
#define     OP_ADD_NODE_PROTOCOL_DONE   1   /**< Protocol part of adding node done*/
#define     OP_ADD_NODE_GET_NODE_INFO   2   /**< Getting node detailed information*/
#define     OP_ADD_NODE_PROTOCOL_START  3   /**< Smart Start add node Z-wave protocol started*/

/** Progress status of replacing failed node */
#define     OP_RP_NODE_PROTOCOL_DONE    1   /**< Protocol part of replacing node done*/
#define     OP_RP_NODE_GET_NODE_INFO    2   /**< Getting node detailed information*/

/** Progress status of initiating */
#define     OP_INI_PROTOCOL_DONE        1   /**< Protocol part of initiating done*/
#define     OP_INI_GET_NODE_INFO        2   /**< Getting node detailed information*/

/** Progress status of network update */
#define     OP_NU_TOPOLOGY              1   /**< Network topology update started*/
#define     OP_NU_NEIGHBOR              2   /**< Node neighbor update started*/
#define     OP_NU_GET_NODE_INFO         3   /**< Node information update started*/

/** Progress status of firmware update */
#define     OP_FW_UPLOAD_STARTED        1   /**< Uploading firmware to device started */
#define     OP_FW_UPLOADING             2   /**< Uploading firmware to device in progress with additional info,
                                                 see ZWNET_STS_INFO_FW_UPDT_PRG */

/** Progress status of network health check */
#define     OP_HEALTH_CHK_STARTED       1   /**< Network health check started */
#define     OP_HEALTH_CHK_PROGRESS      2   /**< Network health check in progress with additional info,
                                                 see ZWNET_STS_INFO_HEALTH_CHK_PRG*/
#define     OP_HEALTH_CHK_CMPLT         3   /**< Network health check completed with additional info,
                                                 see ZWNET_STS_INFO_HEALTH_CHK_RPT*/

/** Controller role bit-mask*/
#define ZWNET_CTLR_ROLE_PROXY           0x01   /**< Support Network Management Proxy*/
#define ZWNET_CTLR_ROLE_INCL            0x02   /**< Support Network Management Inclusion*/
#define ZWNET_CTLR_ROLE_PRI             0x04   /**< Support Network Management Primary*/

/** Controller capabilities bit-mask*/
#define ZWNET_CTLR_CAP_S2               0x01   /**< Support Security 2 protocol */
#define ZWNET_CTLR_CAP_INC_ON_BEHALF    0x02   /**< Support inclusion on-behalf */
#define ZWNET_CTLR_CAP_SMART_START      0x04   /**< Support Smart Start */
#define ZWNET_CTLR_CAP_IMA              0x08   /**< Support IMA (Z-Wave Network Installation and maintenance) */

/** Controller Z-wave role*/
#define ZW_ROLE_UNKNOWN         0   /**< Unknown */
#define ZW_ROLE_SIS             1   /**< SIS*/
#define ZW_ROLE_INCLUSION       2   /**< Inclusion*/
#define ZW_ROLE_PRIMARY         3   /**< Primary*/
#define ZW_ROLE_SECONDARY       4   /**< Secondary*/

/** Network descriptor */
typedef struct
{
	uint32_t	    id;			/**< home id */
	uint8_t	        ctl_id;		/**< Z/IP controller node id*/
	uint8_t	        ctl_role;   /**< Z/IP controller role. A bit-mask of ZWNET_CTLR_ROLE_XXX */
    uint8_t	        ctl_cap;    /**< Z/IP controller capabilities. A bit-mask of ZWNET_CTLR_CAP_XXX */
	uint8_t	        ctl_zw_role;/**< Z/IP controller Z-wave role, ZW_ROLE_XXX */
    void            *user;      /**< User specified information that was passed to zwnet_init() */
    void            *plt_ctx;   /**< Platform context for printing of output text messages*/
}
zwnetd_t, *zwnetd_p;

/** @name Network_operation_sts_info_type
* Network operation status information type definition
@{
*/
#define ZWNET_STS_INFO_SS_START         0   /**< Smart Start add node protocol started (for ZWNET_OP_ADD_NODE->OP_ADD_NODE_PROTOCOL_START) */
#define ZWNET_STS_INFO_FW_UPDT_PRG      1   /**< Firmware update progress in percentage */
#define ZWNET_STS_INFO_HEALTH_CHK_PRG   2   /**< Network health check progress */
#define ZWNET_STS_INFO_HEALTH_CHK_RPT   3   /**< Network health check completion report */
#define ZWNET_STS_INFO_NODE_ID          4   /**< Node id involved in the corresponding network operation */

/**
@}
*/
#ifndef MAX_DSK_STR_LEN
#define MAX_DSK_STR_LEN      47      /**< Maximum DSK string length */
#endif

#define MAX_NODE_STS         232     /**< The maximum number of Z-wave node health status */

/** Network health check progress */
typedef struct
{
	uint8_t	        node_cnt;   /**< Number of health check completed nodes */
	uint8_t	        total;      /**< Total number of nodes scheduled for health check */
}
zw_health_prg_t;

/** @name Network_health_category
* Network health category definition
@{
*/
#define NW_HEALTH_GREEN     0   /**< Network health is good */
#define NW_HEALTH_YELLOW    1   /**< Network health is acceptable but latency can be observed occasionally */
#define NW_HEALTH_RED       2   /**< Network health is insufficient because frames are dropped */
#define NW_HEALTH_CRITICAL  3   /**< Network health is critical because Z-Wave node is not responding at all */

/**
@}
*/

/** Network health check individual node status */
typedef struct
{
	uint8_t	        node_id;    /**< Node id */
	uint8_t	        sts_cat;    /**< Status category which is derived from network health value, see NW_HEALTH_XXX */
	uint8_t	        value;      /**< Calculated network health value */
}
zw_health_sts_t;

/** Network health check status report */
typedef struct
{
	uint8_t	        sts_cnt;            /**< Total number of node health status in the array */
	zw_health_sts_t sts[MAX_NODE_STS];  /**< Node health status array*/
}
zw_health_rpt_t;


/** Additional info for network operation status */
typedef struct
{
    int                 type;       /**< Type of info, ZWNET_STS_INFO_XXX */
    union
    {
        char		    s2_dsk[MAX_DSK_STR_LEN + 1];     /**< S2 DSK (for ZWNET_STS_INFO_SS_START) */
        uint8_t		    progress_percent;                /**< Firmware update progress in % (for ZWNET_STS_INFO_FW_UPDT_PRG) */
        zw_health_prg_t	health_chk_progress;             /**< Network health check progress (for ZWNET_STS_INFO_HEALTH_CHK_PRG) */
        zw_health_rpt_t	health_chk_rpt;                  /**< Network health check completion report (for ZWNET_STS_INFO_HEALTH_CHK_RPT) */
        uint8_t		    node_id;                         /**< Node id (for ZWNET_STS_INFO_NODE_ID) */

    } info;
}
zwnet_sts_t;


typedef void (*zwnet_notify_fn)(void *user, uint8_t op, uint16_t sts, zwnet_sts_t *info);
/**<
network status notification callback
@param[in]	user	    user context
@param[in]	op		    network operation ZWNET_OP_XXX
@param[in]	sts		    status of current operation
@param[in]	info	    additional information for the specified op and sts; NULL if there is no additional info
*/

/** @name Node_status
* Node status definition
@{
*/
#define ZWNET_NODE_ADDED	        0		/**< Node is added */
#define ZWNET_NODE_REMOVED	        1		/**< Node is removed */
#define ZWNET_NODE_UPDATED	        2		/**< Node is updated */
#define ZWNET_NODE_STATUS_ALIVE     3		/**< Node status has changed to "alive" */
#define ZWNET_NODE_STATUS_DOWN      4		/**< Node status has changed to "down" */
#define ZWNET_NODE_STATUS_SLEEP     5		/**< Node status has changed to "sleeping" */
/**
@}
*/

typedef void (*zwnet_node_fn)(void *user, zwnoded_p noded, int status);
/**<
network node status notification callback
@param[in]	user	user context
@param[in]	noded	newly added or removed node
@param[in]	status	status, ZWNET_NODE_XXX
*/

/** @name Network_tx_status
* Network transmission status definition
@{
*/
#define ZWNET_TX_OK                 0x00  /**< transmission is o.k.*/
#define ZWNET_TX_NO_ACK             0x01  /**< send frame timeout due to no ACK received */
#define ZWNET_TX_SYS_ERR            0x02  /**< system error, the program should exit */
#define ZWNET_TX_DEST_BUSY          0x03  /**< message has not timed out yet.The destination host may have a long response time (e.g. sleeping node)*/
#define ZWNET_TX_NOROUTE            0x04  /**< frame failed to reach destination host */
/**
@}
*/

typedef void (*zwnet_appl_fn)(void *user, uint8_t tx_sts);
/**<
application transmit data status callback
@param[in]	user	user context
@param[in]	tx_sts	transmit status ZWNET_TX_xx
*/

#ifdef SUPPORT_SIMPLE_AV_CONTROL
typedef void (*zwnet_av_set_fn)(zwepd_p ep, uint16_t *cmds, uint8_t num_cmds, uint8_t key_attr);
/**<
set callback for AV button press
@param[in]  cmds        list of AV commands
@param[in]  num_cmds    number of AV commands in the list
@param[in]  key_attr    key attribute
*/
#endif

#ifdef TCP_PORTAL
typedef void (*zwnet_net_err_fn)(void *user);
/**<
unrecoverable network error callback
@param[in]	user	user context
*/
#endif

typedef void (*zwnet_unhandled_cmd_fn)(void *user, uint8_t src_node, uint8_t src_ep, uint8_t *cmd, uint16_t cmd_len);
/**<
unhandled command callback
@param[in]	user	    user context
@param[in]  src_node    source node id
@param[in]  src_ep      source endpoint id
@param[in]  cmd         buffer that stores the unhandled command
@param[in]  cmd_len     length of the unhandled command
*/

struct _dev_record;
struct _dev_rec_srch_key;
typedef int (*zwnet_dev_rec_find_fn)(void *dev_cfg_ctx, struct _dev_rec_srch_key *srch_key, struct _dev_record *matched_rec);
/**<
search for a match in device specific configuration records
@param[in]	dev_cfg_ctx	    User specified device configuration context
@param[in]	srch_key	    Search key
@param[out]	matched_rec	    The matched record; either exact match or partial match as explained in the note below
@return     Non-zero if a match is found; else returns zero
@note       This function support "don't care" cases in device specific configuration records.
            The search priority is as follows (in the format (Manf id, Product Type, Product id))
            : (V, V, V), (V, V, X), (V, X, X), (X, X, X) where V="valid value"; X="don't care".
*/


#ifdef TCP_PORTAL
/** Portal client profile */
typedef struct
{
    uint8_t         clnt_id[8];             ///< Client's ID (currently using MAC address formatted as an IEEE EUI-64 identifier)
    uint8_t         clnt_pin[8];            ///< Client's PIN (password)
    uint8_t         clnt_ipv6_addr[16];     ///< Client's IPv6 address. May be all-zeroes IPv6 address
    uint8_t         clnt_dflt_gw[16];       ///< Client's default IPv6 gateway
    uint8_t         clnt_pan_prefix[16];    ///< Client's PAN interface prefix with /64 prefix length. May be all-zeroes IPv6 address
    uint8_t         clnt_unsolicited_dst[16];///< Client's forwarding destination address for unsolicited message
    uint8_t         svr_ipv6_addr[16];      ///< Server's IPv6 address
    uint16_t        clnt_unsolicited_port;  ///< Client's forwarding destination port for unsolicited message. Should be 4123
    uint8_t         clnt_ipv6_prefix;       ///< Client's IPv6 address prefix length
    uint8_t         svr_ipv6_prefix;        ///< Server's IPv6 address prefix length
} clnt_prof_t;
#endif


/** @name If_rec_type
* Interface device database record type definition
@{
*/
#define IF_REC_TYPE_GROUP       1   /**< Group */
#define IF_REC_TYPE_CONFIG      2   /**< Configuration */
#define IF_REC_TYPE_BIN_SENSOR  3   /**< Binary sensor */
#define IF_REC_TYPE_SENSOR      4   /**< Sensor */
#define IF_REC_TYPE_METER       5   /**< Meter */
#define IF_REC_TYPE_ALARM       6   /**< Alarm */
#define IF_REC_TYPE_BASIC       7   /**< Basic */
#define IF_REC_TYPE_ALRM_SNSR   8   /**< Alarm sensor */
#define IF_REC_TYPE_THRMO_SETP  9   /**< Thermostat setpoint */
#define IF_REC_TYPE_MAX         IF_REC_TYPE_THRMO_SETP   /**< Maximun interface record type number */
/**
@}
*/

/** Configuration parameter set record */
typedef struct
{
    uint8_t     param_num;	    /**< Parameter number */
    uint8_t     param_size;	    /**< Data size: 1,2,or 4 bytes*/
    int32_t     param_val;      /**< Configuration parameter value to set (signed integer)*/
} if_rec_cfg_set_t;

/** Configuration parameter information record */
typedef struct
{
    uint8_t     param_num;	    /**< Parameter number */
    uint8_t     param_size;	    /**< Data size: 1,2,or 4 bytes*/
    int32_t     param_min;      /**< Minimum configuration parameter value (signed integer)*/
    int32_t     param_max;      /**< Maximum configuration parameter value (signed integer)*/
    int32_t     param_deflt;    /**< Default configuration parameter value (signed integer)*/
    char        *param_name;    /**< Parameter name*/
    char        *param_info;    /**< Parameter information*/
} if_rec_cfg_info_t;


#define CONFIG_REC_TYPE_SET     0   /**< Configuration record type set*/
#define CONFIG_REC_TYPE_INFO    1   /**< Configuration record type information*/

/** Configuration record */
typedef struct config_rec
{
    struct config_rec     *next;    /**< Next record */

    uint16_t              rec_type; /**< Record type CONFIG_REC_TYPE_XXX */
    union
    {
        if_rec_cfg_set_t  cfg_set;  /**< Configuration parameter set record; for rec_type of CONFIG_REC_TYPE_SET*/
        if_rec_cfg_info_t cfg_info; /**< Configuration parameter information record; for rec_type of CONFIG_REC_TYPE_INFO*/
    } rec;
} if_rec_config_t;

#define IF_REC_MAX_GROUPS       8   /**< Maximum number of groups for the controller to set its node id into the groups*/

/** Group record */
typedef struct
{
    uint8_t     grp_cnt;                  /**< Number of group id in the grp_id array*/
    uint8_t     grp_id[IF_REC_MAX_GROUPS];/**< Group id for the controller to set its node id into the group*/

} if_rec_grp_t;

/** @name Central Scene Key attributes
* Central Scene Key attributes definition
@{
*/
#define ZW_CSC_KEY_ATTRIB_KEY_PRESSED_1_TIME	0x00    /**< A key is pressed and released before time out */
#define ZW_CSC_KEY_ATTRIB_KEY_RELEASED			0x01    /**< A key is released. Termination of a Key Held Down sequence.*/
#define ZW_CSC_KEY_ATTRIB_KEY_HELDDOWN			0x02    /**< a key is pressed and not released before time out
															Event used to signal continuation of key held down operation.*/
#define ZW_CSC_KEY_ATTRIB_KEY_PRESSED_2_TIME	0x03    /**< A key is pressed twice and no more key presses follow */
#define ZW_CSC_KEY_ATTRIB_KEY_PRESSED_3_TIME	0x04    /**< A key is pressed 3 times  and no more key presses follow */
#define ZW_CSC_KEY_ATTRIB_KEY_PRESSED_4_TIME	0x05    /**< A key is pressed 4 times  and no more key presses follow */
#define ZW_CSC_KEY_ATTRIB_KEY_PRESSED_5_TIME	0x06    /**< A key is pressed 5 times  and no more key presses follow */
/**
@}
*/

/** @name Bin_sensor_type
* Binary sensor Type definition
@{
*/
#define ZW_BSENSOR_TYPE_GP       0x01    /**< General purpose sensor*/
#define ZW_BSENSOR_TYPE_SMOKE    0x02    /**< Smoke sensor*/
#define ZW_BSENSOR_TYPE_CO       0x03    /**< CO sensor*/
#define ZW_BSENSOR_TYPE_CO2      0x04    /**< CO2 sensor*/
#define ZW_BSENSOR_TYPE_HEAT     0x05    /**< Heat sensor*/
#define ZW_BSENSOR_TYPE_WATER    0x06    /**< Water sensor*/
#define ZW_BSENSOR_TYPE_FREEZE   0x07    /**< Freeze sensor*/
#define ZW_BSENSOR_TYPE_TAMPER   0x08    /**< Tamper sensor*/
#define ZW_BSENSOR_TYPE_AUX      0x09    /**< Aux sensor*/
#define ZW_BSENSOR_TYPE_DR_WIN   0x0A    /**< Door/Window sensor*/
#define ZW_BSENSOR_TYPE_TILT     0x0B    /**< Tilt sensor*/
#define ZW_BSENSOR_TYPE_MOTION   0x0C    /**< Motion sensor*/
#define ZW_BSENSOR_TYPE_GLASS_BRK 0x0D    /**< Glass break sensor*/

#define ZW_BSENSOR_TYPE_1ST_SUP  0xFF    /**<  Return first sensor type on supported list*/
/**
@}
*/


/** Device specific binary sensor record matching condition */
typedef struct bsnsr_mch_rec
{
	struct bsnsr_mch_rec  *next;/**< Next match record */
	void		*pResult;		/**< The result record that maps to. eg. if_rec_alarm_result_t* */

	int			type;			/**< Binary sensor type ZW_BSENSOR_TYPE_XXX */
	int			state;		    /**< Binary sensor state */

	int			resultType;		/**< Type of the resulted conversion.  IF_REC_TYPE_xxx*/

} if_rec_bsnsr_match_t;

/** Binary sensor record */
typedef struct
{
    uint8_t                 type;           /**< Supported binary sensor type ZW_BSENSOR_TYPE_XXX (for version 1 only) */
    if_rec_bsnsr_match_t    *rec_match;     /**< Record matching condition*/

} if_rec_bsnsr_t;

/** Sensor record */
typedef struct
{
    uint8_t     type;           /**< Sensor type ZW_SENSOR_TYPE_XXX */
    uint8_t     unit;           /**< Sensor unit ZW_SENSOR_UNIT_XXX */

} if_rec_snsr_t;

/** Meter record */
typedef struct
{
    uint8_t     type;		    /**< ZW_METER_TYPE_XXX */
    uint8_t     unit_supp;      /**< supported unit bit-mask : ZW_METER_SUP_UNIT_XXX */
} if_rec_meter_t;

struct zwsetp_temp_range;

/** Thermostat setpoint record */
typedef struct
{
    uint8_t                     type_cnt;       /**< Number of thermostat setpoint supported temparature range */
    struct zwsetp_temp_range    *temp_range;    /**< Temparature ranges */

} if_rec_thrmo_setp_t;

#define MAX_ALRM_NAME_LEN       80      /**< Maximum alarm name length*/
#define MAX_ALRM_LVL_NAME_LEN   80      /**< Maximum alarm level name length*/

/** Device specific alarm result record */
typedef struct alarm_result_rec
{
	int			type;	        /**< Vendor proprietary alarm type */
	int			level;			/**< Vendor proprietary alarm level */

	int			ex_type;		/**< Z-wave alarm type (ZW_ALRM_XXX) */
	char        *type_name;	    /**< Device specific alarm type name */
	int			ex_event;	    /**< Z-wave alarm event (ZW_ALRM_EVT_XXX) */
	char        *level_name;    /**< Device specific alarm level name */

	int			ex_evt_len;     /**< Z-wave alarm event parameter length. Zero if the event has no parameter */
	int			ex_evt_type;    /**< Z-wave alarm event parameter type (ZW_ALRM_PARAM_XXX) */
	uint8_t		*pEx_evt_prm;	/**< pointer to Z-wave alarm event parameter*/
} if_rec_alarm_result_t;

/** Device specific alarm reverse-matching record (It is the OR result of match and result struct*/
typedef struct alarm_rev_match_rec
{
	int			type;	        /**< Vendor proprietary alarm type */

	int			ex_type;		/**< Z-wave alarm type (ZW_ALRM_XXX) */
	int			ex_event;	    /**< Z-wave alarm event (ZW_ALRM_EVT_XXX) */
} if_rec_alarm_rev_match_t;

/** Device specific alarm record matching condition */
typedef struct alarm_match_rec
{   //NOTE: Modifying this structure requires modifying 'alarm_match_rec_init'
	struct alarm_match_rec      *next;      /**< Next match record */
	if_rec_alarm_result_t	    *pResult;   /**< The result alarm record that maps to*/
	if_rec_alarm_rev_match_t	*pRevMatch; /**< The RevMatch alarm record */

    int         match_valid;    /**< Flag to indicate whether the match entries below are valid*/

	int			type;	        /**< Vendor proprietary alarm type */
	int			level_low;		/**< Vendor proprietary alarm level or lower limit of a range */
	int			level_high;     /**< Vendor proprietary alarm level higher limit of a range */
	int			isRange;        /**< Flag to indicate vendor proprietary alarm level is a range */

	int			ex_type;		/**< Z-wave alarm type (ZW_ALRM_XXX) */
	int			ex_event;	    /**< Z-wave alarm event (ZW_ALRM_EVT_XXX) */

	int			ex_evt_len;     /**< Z-wave alarm event parameter length. Zero if the event has no parameter */
	uint8_t		*pEx_evt_prm;	/**< pointer to Z-wave alarm event parameter*/
} if_rec_alarm_match_t;

/** Device specific basic record matching condition */
typedef struct basic_match_rec
{
	struct basic_match_rec  *next;   /**< Next match record */
	void		*pResult;		/**< The result record that maps to. eg. if_rec_alarm_result_t* */

	int			command;	    /**< Basic command class command. eg. BASIC_SET */
	int			value;			/**< Command value 0-255. -1 means don't care. */
	int			resultType;		/**< Type of the resulted conversion.  IF_REC_TYPE_xxx*/
} if_rec_basic_match_t;

/** @name Alarm_sensor_type
* Alarm sensor Type definition
@{
*/
#define ZW_ALRM_SNSR_TYPE_GP			0x00    /**< General Purpose Alarm*/
#define ZW_ALRM_SNSR_TYPE_SMOKE			0x01    /**< Smoke Alarm*/
#define ZW_ALRM_SNSR_TYPE_CO			0x02    /**< CO Alarm*/
#define ZW_ALRM_SNSR_TYPE_CO2			0x03    /**< CO2 Alarm*/
#define ZW_ALRM_SNSR_TYPE_HEAT			0x04    /**< Heat Alarm*/
#define ZW_ALRM_SNSR_TYPE_WATER_LEAK	0x05    /**< Water Leak Alarm*/

#define ZW_ALRM_SNSR_TYPE_1ST_SUP		0xFF    /**<  Return first Alarm on supported list*/
/**
@}
*/

/** Device specific alarm sensor record matching condition */
typedef struct alrm_snsr_match_rec
{
	struct alrm_snsr_match_rec  *next;   /**< Next match record */
	void		*pResult;		/**< The result record that maps to. eg. if_rec_alarm_result_t* */

	int			type;			/**< Alarm sensor type. (ZW_ALRM_SNSR_TYPE_XXX) */
	int			state_low;		/**< Alarm sensor state or lower limit of a range. -1 means don't care. */
	int			state_high;     /**< Alarm sensor state higher limit of a range */
	int			isRange;        /**< Flag to indicate Alarm sensor state is a range */

	int			resultType;		/**< Type of the resulted conversion.  IF_REC_TYPE_xxx*/
} if_rec_alrm_snsr_match_t;

/** Device specific interface record */
typedef struct if_record
{
    struct if_record   *next;   /**< Next interface record */
    uint8_t     type;           /**< Interface type, IF_REC_TYPE_XXX */
    uint8_t     usr_def_ver;    /**< User defined version */
    uint8_t     propty;         /**< Interface type specific property (bit-mask) */
    uint16_t    tmout;          /**< Interface type specific timeout value in seconds */

    union {                     /**< Record as indicated by interface type */
        if_rec_grp_t                *grp;
        if_rec_config_t             *config;
        if_rec_bsnsr_t              *bsnsr;
        if_rec_snsr_t               *snsr;
        if_rec_meter_t              *meter;
		if_rec_alarm_match_t        *alarm;
		if_rec_basic_match_t        *basic;
		if_rec_alrm_snsr_match_t    *alrm_snsr;
        if_rec_thrmo_setp_t         *thrmo_setp;
    } rec;                      /**< Interface record*/
} if_rec_t;

/** Device specific command redirection record */
typedef struct redirec_record
{
	struct redirec_record   *next;  /**< Next redirection record */
	uint16_t    if_id;              /**< Interface id / command class id*/
	int			cmd;			    /**< Command value 0-255. -1 means don't care. */
	uint8_t     target_ep;		    /**< Target endpoint id to redirect to*/
} redir_rec_t;

/** Device specific endpoint record */
typedef struct ep_record
{
    struct ep_record   *next;   /**< Next endpoint record */
    uint8_t     id;             /**< Endpoint id (starting from 0 for virtual endpoint, 1 and onwards for real endpoints)*/
	uint16_t    new_if;         /**< New command class to be created/added to the endpoint, if any*/
	uint8_t     new_if_ver;     /**< The "user defined version" for the new command class, if any*/
    uint8_t     new_if_propty;  /**< New command class (interface) property (bit-mask) */
	redir_rec_t	*redir_rec;		/**< Command redirection records */
	if_rec_t	*if_rec;	    /**< Interface records */
} ep_rec_t;

/** Device specific record search parameters*/
typedef struct _dev_rec_srch_key
{
	uint32_t	vid;			/**< Vendor ID */
	uint32_t	ptype;			/**< Product Type ID */
	uint32_t	pid;			/**< Product ID */
} dev_rec_srch_key_t;

#define DEV_REC_ID_WILDCARD     0xFFFFFFFF    /**< Wildcard for Vendor ID, Product Type ID or Product ID.*/

/** Device specific record */
typedef struct _dev_record
{
	uint32_t	vid;			/**< Vendor ID. May use DEV_REC_ID_WILDCARD to indicate "don't care" */
	uint32_t	ptype;			/**< Product Type ID. May use DEV_REC_ID_WILDCARD to indicate "don't care" */
	uint32_t	pid;			/**< Product ID. May use DEV_REC_ID_WILDCARD to indicate "don't care" */
    uint8_t     category;	    /**< Device category, DEV_XXX */
    ep_rec_t    *ep_rec;        /**< Endpoint records*/
} dev_rec_t;

/** Global setting types*/
#define GLOB_SET_TYPE_WKUP_INTV         1   /**< Wakeup interval to set for a newly added sleeping node*/
#define GLOB_SET_TYPE_WUNMI_DELAY       2   /**< Send wakeup no more info delay for a newly added sleeping node*/

/** Device database global settings */
typedef struct
{
    uint16_t            type;           /**< Setting type, GLOB_SET_TYPE_XXX */
    union {                             /**< Record as indicated by setting type */
        uint32_t        wkup_intv;      /**< For GLOB_SET_TYPE_WKUP_INTV */
        uint16_t        wunmi_delay;    /**< For GLOB_SET_TYPE_WUNMI_DELAY */
    } sett;                             /**< Settings*/
} dev_global_sett_t;

/** Device specific configuration records */
typedef struct
{
    dev_rec_t   *dev_cfg_records;   /**< pointer to device specific configuration records*/
	uint16_t	dev_cfg_cnt;	    /**< number of records in dev_cfg_records */
} dev_cfg_rec_t;

/** Device specific configurations (managed by user application) */
typedef struct
{
    zwnet_dev_rec_find_fn   dev_rec_find_fn;    /**< User supplied function to find device record*/
    void                    *dev_cfg_ctx;       /**< User specified device configuration context for use in dev_rec_find_fn*/
    dev_global_sett_t       *dev_glob_sett;     /**< Optional device global settings */
    uint16_t                global_sett_cnt;    /**< Global settings count in dev_glob_sett*/
} dev_cfg_usr_t;

/** Device specific configuration error location*/
typedef struct
{
    unsigned    dev_ent;    /**< Device entry number (starting from 1)*/
    unsigned    ep_ent;     /**< Endpoint entry number (starting from 1)*/
    unsigned    if_ent;     /**< Interface entry number (starting from 1)*/
} dev_cfg_error_t;

/** Supported command class property (bit-mask) */
#define BITMASK_CMD_CLS_INSECURE    0x01    /**< Command class is insecure*/
#define BITMASK_CMD_CLS_SECURE      0x02    /**< Command class is secure*/

/** Supported command class*/
typedef struct
{
    uint16_t    cls;			/**< Command class */
    uint8_t		ver;			/**< Version of the command class */
	uint8_t		propty;	        /**< Properties of the interface (bit-mask): BITMASK_CMD_CLS_XXX */
} sup_cmd_cls_t;


struct sec2_add_cb_prm;
typedef void (*add_node_sec2_fn)(void *usr_param, struct sec2_add_cb_prm *cb_param);
/**<
callback for add node with security 2 operation to report joining device requested keys and/or Device Specific Key (DSK)
@param[in]	usr_param  user supplied parameter when calling zwnet_add
@param[in]	cb_param   DSK related callback parameters
*/

struct pl_lst_ent;
typedef void (*pl_info_fn)(void *usr_ctx, struct pl_lst_ent *lst_ent);
/**<
callback to report provisioning list entry information
@param[in]	usr_ctx     user context
@param[in]	lst_ent     provisioning list entry
*/


/** network client initialization parameters */
typedef struct
{
	int				        instance;           /**< 0 for now */
	void			        *user;		        /**< User context used in callbacks */
	uint16_t	            host_port;          /**< Host listening and sending port.
                                                     For system assigned port, enter port number 0 */
	int	                    use_ipv4;           /**< Flag to indicate whether to use IPv4 as transport IP protocol. 1=use IPv4; 0=use IPv6 */
    uint8_t                 zip_router[16];     /**< Z/IP router (gateway) IPv4/IPv6 address in numeric format */
	zwnet_notify_fn	        notify;		        /**< Command status callback */
	zwnet_node_fn	        node;		        /**< Node add/del/status callback */
    zwnet_appl_fn           appl_tx;            /**< Application transmit data status callback*/
    pl_info_fn              inif_cb;            /**< Unsolicited included node information frame (INIF) callback*/
    zwnet_unhandled_cmd_fn  unhandled_cmd;      /**< Unhandled command callback*/
    print_fn                print_txt_fn;       /**< Print text function */
#ifdef SUPPORT_SIMPLE_AV_CONTROL
    zwnet_av_set_fn         av_set;             /**< AV Set commands callback */
    uint8_t                 *av_sup_bitmask;    /**< Bitmask of supported AV commands */
    uint16_t                av_sup_bitmask_len; /**< Length of bitmask for supported AV commands (in bytes) */
#endif
#ifdef TCP_PORTAL
	int	                    portal_fd;          /**< File descriptor to connect to Z/IP gateway/router using TLS */
	void			        *portal_ssl;		/**< SSL object pointer to connect to Z/IP gateway/router using TLS */
    void			        *display_ctx;		/**< Display context for the print_txt_fn */
	zwnet_net_err_fn        net_err;		    /**< Unrecoverable network error callback, application should close
                                                     this instance of Z-ware, i.e. call zwnet_exit() */
    clnt_prof_t             portal_prof;        /**< Profile of the Z/IP gateway/router that is connected to the portal */
#endif
    const char              *net_info_dir;      /**< Full path of directory for storing network and node information file */
    const char              *pref_dir;          /**< Full path of directory for storing network/user preference files*/
    const char              *dev_cfg_file;      /**< Device specific configurations file in JSON format. If
                                                     it is NULL, device specific configurations will be managed by
                                                     user application. In this case dev_cfg_usr must be valid  */
    const char              *cmd_cls_cfg_file;  /**< Optional command classes configuration file. Enable specific command classes probing
                                                     after a new node inclusion and during background polling. If NULL, ALL supported
                                                     command classes are enabled. */
    dev_cfg_usr_t           *dev_cfg_usr;       /**< Device specific configurations (managed by user application). If
                                                     it is NULL, device specific configurations will be managed by
                                                     HCAPI library internally. In this case dev_cfg_file must be valid */
    dev_cfg_error_t         err_loc;            /**< Error location while parsing device specific configuration file.
                                                     Note: The error is not due to JSON format parsing error.*/
    sup_cmd_cls_t           *sup_cmd_cls;       /**< User application implemented command classes.NOTE: if the controller
                                                     has already implemented the command class, the user's request for that
                                                     command class will be ignored*/
    add_node_sec2_fn        s2_unsolicited_cb;  /**< Callback to report unsolicited joining device requested keys and/or
                                                     status of Device Specific Key (DSK)*/
    uint8_t                 sup_cmd_cls_cnt;    /**< User application implemented command classes count*/
#ifndef TCP_PORTAL
    uint8_t                 dtls_psk[32];       /**< DTLS pre-shared key in binary format, MUST be at least 16 bytes*/
    uint8_t                 dtls_psk_len;       /**< DTLS pre-shared key length (bytes). If length is zero,
                                                     no DTLS will be used, i.e. communication will be insecured */
#endif
}
zwnet_init_t, *zwnet_init_p;

int zwnet_init(const zwnet_init_p init, zwnet_p *net);
/**<
initialize network and return this network
@param[in]	init	        client initialization parameters
@param[out]	net	            network handle
@return		ZW_ERR_XXX
*/

void zwnet_exit(zwnet_p net);
/**<
clean up  network
@param[in]	net		        network
*/

int zwnet_save(zwnet_p net, const char *node_info_file);
/**<
save network node information and security layer key and setting into a file
@param[in]	net		        network
@param[in]	node_info_file	node information file name
@return  ZW_ERR_XXX.
*/

int zwnet_load(zwnet_p nw, const char *node_info_file);
/**<
zwnet_load - Load network node information and security layer key and settings
@param[in]	nw		        Network
@param[in]	node_info_file	Node information file name.  If this parameter is NULL,
                            node information will be acquired through Z-Wave network
@return		ZW_ERR_XXX
*/

int zwnet_get_node(zwnet_p net, zwnoded_p noded);
/**<
get first node (local controller) in the network
@param[in]	net		network
@param[out]	noded	node
@return		ZW_ERR_NONE if successful
*/

int zwnet_get_node_by_id(zwnet_p net, uint8_t nodeid, zwnoded_p noded);
/**<
get node with specified node id in the network
@param[in]	net		    Network
@param[in]	nodeid		Node id
@param[out]	noded	    Node descriptor (Can be NULL, if the purpose is to verify the existence of a node)
@return		ZW_ERR_NONE if successful
*/

int zwnet_get_ep_by_id(zwnet_p net, uint8_t nodeid, uint8_t epid, zwepd_p epd);
/**<
get endpoint with specified node and endpoint id in the network
@param[in]	net		    Network
@param[in]	nodeid		Node id
@param[in]	epid		Endpoint id
@param[out]	epd	        Endpoint descriptor (Can be NULL, if the purpose is to verify the existence of an endpoint)
@return		ZW_ERR_NONE if successful
*/

int zwnet_get_if_by_id(zwnet_p net, uint8_t nodeid, uint8_t epid, uint16_t cls, zwifd_p ifd);
/**<
get interface with specified node, endpoint and interface id in the network
@param[in]	net		    Network
@param[in]	nodeid		Node id
@param[in]	epid		Endpoint id
@param[in]	cls		    Interface id (command class)
@param[out]	ifd	        Interface descriptor (Can be NULL, if the purpose is to verify the existence of an interface)
@return		ZW_ERR_NONE if successful
*/

void    zwnet_node_info_dump(zwnet_p net);
/**<
dump nodes information (for debugging only)
@param[in]	net		The network
@return
*/

zwnetd_p zwnet_get_desc(zwnet_p net);
/**<
get read-only network descriptor
@param[in]	net		network
@return		network descriptor
*/

void *zwnet_get_user(zwnet_p net);
/**<
zwnet_get_user - get user context
@param[in]	net	        Network
@return	user context passed during initialization
*/

int zwnet_abort(zwnet_p net);
/**<
abort current action/transmission
@param[in]	net		network
@return		ZW_ERR_XXX
*/

/** @name Security_key_bitmask
* Security key bitmask
@{
*/
#define SEC_KEY_BITMSK_S2_K0     0x01    /**< S2: class key 0 (Unauthenticated)*/
#define SEC_KEY_BITMSK_S2_K1     0x02    /**< S2: class key 1 (Authenticated)*/
#define SEC_KEY_BITMSK_S2_K2     0x04    /**< S2: class key 2 (Access Control)*/
#define SEC_KEY_BITMSK_S0        0x80    /**< S0: legacy security network key (Unauthenticated)*/
#define SEC_KEY_ALL_S2  (SEC_KEY_BITMSK_S2_K0 | SEC_KEY_BITMSK_S2_K1 | SEC_KEY_BITMSK_S2_K2)    /**< All S2 keys*/
#define SEC_KEY_ALL_S2_SO       (SEC_KEY_ALL_S2 | SEC_KEY_BITMSK_S0)        /**< All security keys*/
/**
@}
*/

#define ADD_NODE_ANY_S2                 7               /**< Add any type of node and allow security 2*/
#define START_FAILED_NODE_REPLACE_S2    ADD_NODE_ANY_S2 /**< Replace any type of node and allow security 2*/

#define S2_CB_TYPE_REQ_KEY              0   /**< Callback type for joining device requested keys*/
#define S2_CB_TYPE_DSK                  1   /**< Callback type for joining device DSK keys*/

/** Security 2 DSK callback parameters*/
typedef struct
{
	uint8_t		pin_required;	/**< Indicate whether user is required to enter 5-digit pin. 1=required; 0=not required */
    char        *dsk;           /**< Device Specific Key (DSK) of the joining node for user to verify.  Note that
                                     the first 5 digits of DSK are missing if pin_required=1.
                                     Example of complete DSK:
                                     34028-23669-20938-46346-33746-07431-56821-14553
                                     Example of DSK with first 5 digits missing:
                                     -23669-20938-46346-33746-07431-56821-14553 */

} sec2_dsk_cb_prm_t;

/** Security 2 keys and CSA request callback parameters*/
typedef struct
{
    uint8_t     req_keys;   /**< Requested keys (bit mask) by the joining node */
    uint8_t     req_csa;    /**< Flag to indicate joining node is requesting Client-side Authentication (CSA) */
    char        csa_pin[12];/**< CSA 10-digit pin to be entered into the joining node if req_csa is non-zero.
                                 Example: 34028-23669  (Note: The hyphen is for display purposes, it is not part of the pin)
                                 */
} sec2_keys_req_cb_prm_t;

/** Add node with security 2 callback parameters*/
typedef struct sec2_add_cb_prm
{
	uint8_t		cb_type;	    /**< Callback type, see S2_CB_TYPE_XXX */
    union
    {
        sec2_keys_req_cb_prm_t  req_key;    /**< For cb_type=S2_CB_TYPE_REQ_KEY; the joining device requested keys and CSA request*/
        sec2_dsk_cb_prm_t       dsk;        /**< For cb_type=S2_CB_TYPE_DSK; the joining device DSK keys*/
    } cb_prm;

} sec2_add_cb_prm_t;

typedef void (*get_dsk_fn)(void *usr_ctx, char *dsk);
/**<
callback to report  Z/IP gateway DSK
@param[in]	usr_ctx     user context
@param[in]	dsk         Z/IP gateway DSK string
*/

/** Add node with security 2 parameters*/
typedef struct
{
    void                *usr_param;     /**< User defined parameter used in callback */
    add_node_sec2_fn    cb;             /**< Callback to report joining device requested keys and/or
                                             status of Device Specific Key (DSK)*/
    char                *dsk;           /**< Optional Device Specific Key (DSK) of the joining node for verification.
                                             Note that if this is not NULL, no DSK callback will be executed.
                                             The format of the DSK must be as shown in the example:
                                             34028-23669-20938-46346-33746-07431-56821-14553 */
} sec2_add_prm_t;


int zwnet_add(zwnet_p net, uint8_t add, sec2_add_prm_t *sec2_param, int incl_on_behalf);
/**<
called by inclusion controller to add/remove initiating node to network
@param[in] net              Network
@param[in] add              0=remove; else add node
@param[in] sec2_param       Parameters for adding node with security 2 protocol.
                            This parameter is ignored when non-security 2 Z/IP gateway is detected or when removing node.
@param[in] incl_on_behalf   Flag to indicate enter into inclusion on-behalf (iob) mode; 1=enter into iob mode, 0=normal add node.
                            This parameter is ignored when parameter add = 0
@return		ZW_ERR_XXX, otherwise node callback will be called.
@see		zwnet_initiate
*/

int zwnet_add_sec2_accept(zwnet_p net, int accept, char *dsk);
/**<
accept/reject newly added node into security 2 mode
@param[in]	net		    network
@param[in]	accept		1=accept; 0=reject
@param[in]	dsk	        Complete Device Specific Key (DSK) of the added node if accept=1; else this can be NULL.
@return		ZW_ERR_XXX
@pre        The function should be called only after receiving security 2 DSK callback.
@see		zwnet_add
*/

int zwnet_add_sec2_grant_key(zwnet_p net, uint8_t granted_keys, uint8_t grant_csa);
/**<
grant keys and CSA to the newly added node in security 2 mode
@param[in]	net		        network
@param[in]	granted_keys    security 2 granted keys (bit-mask), see SEC_KEY_BITMSK_XXX
@param[in]	grant_csa       grant client-side authentication (CSA); 1=grant, 0=reject. If joining node
                            didn't request CSA, it is ignored.
@return		ZW_ERR_XXX
@pre        The function should be called only after receiving security 2 requested keys callback.
@see		zwnet_add
*/

int zwnet_sec2_get_dsk(zwnet_p net, get_dsk_fn cb, void *usr_ctx);
/**<
get Z/IP gateway security 2 DSK
@param[in]	net	    network
@param[in]	cb      callback to report Z/IP gateway DSK
@param[in]	usr_ctx user context used in callback
@return		ZW_ERR_XXX
*/

int zwnet_fail(zwnet_p net, uint8_t nodeid, uint8_t replace, sec2_add_prm_t *sec2_param, int rplc_on_behalf);
/**<
called by inclusion controller to remove failed node from the routing table or replace failed node with initiating node
@param[in]	net		        network
@param[in]	nodeid	        failed node id. This parameter is ignored when rplc_on_behalf = 1.
@param[in]	replace	        replace failed node if non-zero; else remove failed node if zero
@param[in]	sec2_param	    Optional parameters for replacing node with security 2 protocol.  This parameter is ignored
                            when removing failed node (i.e. replace = 0).
@param[in]	rplc_on_behalf	flag to indicate enter into "replace failed node" on-behalf (rob) mode; 1=enter into rob mode,
                            0=normal replace failed node. This parameter is ignored when parameter replace = 0.
@return		ZW_ERR_XXX, otherwise node callback will be called.
@see		zwnet_initiate
*/

int zwnet_migrate(zwnet_p net);
/**<
called by primary controller or SUC to make initiating controller primary, and itself secondary.
@param[in]	net		network
@return		ZW_ERR_XXX, otherwise node callback will be called
@note       The controller invoking this function will become secondary.
*/

int zwnet_initiate(zwnet_p net, get_dsk_fn cb, void *usr_ctx);
/**<
called by a controller to join/leave a Z-wave network
@param[in]	net		network
@param[in]	cb      callback to report Z/IP gateway DSK when joining another S2 capable Z-wave network.
                    Note that the callback will not be invoked if the Z/IP gateway is not S2 capable
                    or this API is invoked to leave a Z-wave network.
@param[in]	usr_ctx user context used in callback
@return		ZW_ERR_XXX, otherwise node callback will be called
@see		zwnet_add, zwnet_fail, zwnet_migrate
*/

int zwnet_initiate_classic(zwnet_p net, get_dsk_fn cb, void *usr_ctx);
/**<
called by a controller to join/leave a Z-wave network in classic mode
@param[in]	net		network
@param[in]	cb      callback to report Z/IP gateway DSK when joining another Z-wave network.
                    Note that the callback will not be invoked if the Z/IP gateway is not S2 capable
                    or this API is invoked to leave a Z-wave network.
@param[in]	usr_ctx user context used in callback
@return		ZW_ERR_XXX, otherwise node callback will be called
@note   This API is provided mainly for backward compatibility with old Z-wave controller which only supports classic mode
*/

int zwnet_update(zwnet_p net);
/**<
update network status and information
@param[in]	net		network
@return		ZW_ERR_XXX
*/

int zwnet_reset(zwnet_p net);
/**<
detach self from network ie. forget all nodes.  A power-cycle is required after the reset.
@param[in]	net		network
@return		ZW_ERR_XXX
*/

int zwnet_health_chk(zwnet_p net);
/**<
start health check on all but sleeping nodes
@param[in]	net		        network
@return		ZW_ERR_XXX. If ZW_ERR_NONE is returned network notify callback will be called.
*/

void zwnet_version(uint8_t *ver, uint8_t *subver);
/**<
get the home controller API version and subversion
@param[out]	ver		Version
@param[out]	subver	Sub version
@return
*/

int zwnet_send_nif(zwnet_p net, zwnoded_p noded, uint8_t broadcast);
/**<
send node information frame to a node or broadcast it
@param[in]	net		    Network
@param[in]	noded	    Destination node to receive the node information frame
@param[in]	broadcast	Broadcast flag. 1= broadcast; 0= single cast
@return		ZW_ERR_XXX.
*/

int zwnet_pan_prefix_get(zwnet_p net, uint8_t *pan_prefix, uint8_t *prefix_len);
/**<
get PAN prefix
@param[in]	net	        Network
@param[out] pan_prefix  Buffer to store the PAN prefix, it must be at least 16 bytes in size
@param[out] prefix_len  PAN prefix length
@return     ZW_ERR_XXX
*/

#define ZWNET_NODE_STS_SLEEP      2   ///< Node is sleeping
#define ZWNET_NODE_STS_DOWN       1   ///< Node is down
#define ZWNET_NODE_STS_UP         0   ///< Node is alive

void zwnet_all_node_sts_get(zwnet_p net, uint8_t *node_sts_buf);
/**<
get all node status
@param[in]	net		        Network
@param[out]	node_sts_buf	Buffer (min. size of 233 bytes)to store all the node status. Individual node status
                            (ZWNET_NODE_STS_XXX) can be access using the node id as index to the buffer.
@return
*/

uint8_t zwnet_node_sts_get(zwnet_p net, uint8_t node_id);
/**<
get node status
@param[in]	net		        Network
@param[in]	node_id		    Node id (ranging from 1 to 232)
@return node status (ZWNET_NODE_STS_XXX)
*/

/**
@}
@defgroup Netutil Network Utility APIs
Utilities to get gateway IP addresses
@ingroup zwarecapi
@{
*/

int zwnet_ip_aton(char *addr_str, uint8_t *addr_buf, int *ipv4);
/**<
convert IPv4 / IPv6 address string to numeric equivalent
@param[in]  addr_str     Null terminated IPv4 / IPv6 address string
@param[out] addr_buf     Buffer that should be at least 16-byte long to store the result
@param[out] ipv4         Flag to indicate the converted numeric IP address is IPv4 or IPv6. 1=IPv4; 0=IPv6.
@return     Zero if successful; otherwise non-zero
*/

int zwnet_ip_ntoa(uint8_t *addr, char *addr_str, int addr_str_len, int ipv4);
/**<
convert IPv4 / IPv6 address in numerical form to string equivalent
@param[in]  addr         IPv4 / IPv6 address in numerical form
@param[out] addr_str     Buffer to store the converted address string
@param[in]  addr_str_len The size of the addr_str in bytes
@param[in]  ipv4         Flag to indicate the addr parameter is IPv4 or IPv6. 1=IPv4; 0=IPv6.
@return     Zero if successful; otherwise non-zero
*/

int zwnet_local_addr_get(zwnet_p net, uint8_t *dest_ip, uint8_t *local_ip, int use_ipv4);
/**<
get local address that is reachable by destination host
@param[in]	net	        network
@param[in]  dest_ip     Destination host address either IPv4 or IPv6 according to use_ipv4 flag
@param[in]  use_ipv4    Flag to indicate IP address type. 1= IPv4; 0= IPv6
@param[out] local_ip    Buffer of 16-byte to store the local address (either IPv4 or IPv6 according to use_ipv4 flag)
@return     Zero if successful; otherwise non-zero
*/

uint16_t zwnet_listen_port_get(zwnet_p net);
/**<
get local Z/IP listening port number
@param[in]	net	        network
@return     The Z/IP client listening port number
@pre  Must call zwnet_init() before calling this function
*/

typedef void (*zwnet_gw_discvr_cb_t)(uint8_t *gw_addr, uint8_t gw_cnt, int ipv4, void *usr_param,
                                     int rpt_num, int total_rpt, char **gw_name);
/**<
gateway discovery callback
@param[in]	gw_addr	    Gateway addresses
@param[in]	gw_cnt	    Number of gateway addresses returned in gw_addr
@param[in]	ipv4        Flag to indicate the gw_addr parameter is IPv4 or IPv6. 1=IPv4; 0=IPv6
@param[in]	usr_param   User defined parameter used in callback
@param[in]	rpt_num     Report number that this callback is delivering the gateway addresses report; start from 1
@param[in]	total_rpt   Total reports that will be delivered by callbacks. Each callback delivers one report. Zero
                        is returned if there is no valid IP to facilitate gateway discovery.
@param[in]	gw_name	    Gateway names corresponding to the gw_ip.  If NULL, it means gateway name information is unavailable.
*/

void *zwnet_gw_discvr_start(zwnet_gw_discvr_cb_t cb, void *usr_param, int ipv4, int use_mdns);
/**<
start Z/IP gateway discovery
@param[in]	    cb		    Callback function when the gateway discovery has completed
@param[in]	    usr_param	User defined parameter used in callback function
@param[in]	    ipv4	    Flag to indicate whether to use IPv4 as transport IP protocol. 1= use IPv4; 0= use IPv6
@param[in]	    use_mdns	Flag to indicate whether to use MDNS for gateway discovery.  Note: MDNS gateway discovery
                            is only supported in Z/IP gateway version 2
@return Context on success, null on failure.
@post   Caller is required to call zwnet_gw_discvr_stop() with the returned context if it is not null.
*/

int zwnet_gw_discvr_stop(void *ctx);
/**<
stop Z/IP gateway discovery
@param[in]	    ctx		    The context returned from the call to zwnet_gw_discvr_start()
@return Zero on success, non-zero on failure.
@post After the call, the ctx is invalid and should not be used
*/

/**
@}
*/


#ifdef TCP_PORTAL
/**
@defgroup TLSutil TLS Network Utility APIs
Utilities to communicate securely using TLS
@ingroup zwarecapi
@{
*/
///
/// Portal client profile linked-list
typedef struct _clnt_prof_lst
{
    struct _clnt_prof_lst   *next;  ///< Next portal client profile
    clnt_prof_t         clnt_prof;  ///< Client profile
} clnt_prof_lst;

typedef int (*zwportal_cb_t)(int clnt_fd, void *clnt_ssl, clnt_prof_t *clnt_prof, void *usr_param);
/**<
new client connection callback from portal
@param[in]	clnt_fd	    client socket file descriptor
@param[in]	clnt_ssl	client SSL object pointer
@param[in]	clnt_prof	client profile used
@param[in]	usr_param   user defined parameter
@return     Non-zero if the new client connection is accepted; zero if it is rejected.
*/

/** portal initialization parameters */
typedef struct
{
    clnt_prof_t     *clnt_prof;     /**< An array of client profiles*/
    int             prof_cnt;       /**< Number of client profiles in clnt_prof*/
    zwportal_cb_t   cb;             /**< Callback function when a new gateway has completed the TLS handshake*/
    void            *usr_param;     /**< User defined parameter used in callback function*/
    char            *ca_file;       /**< File path to CA certificate that is used to sign Z/IP gateway
                                         public certificate*/
    char            *ssl_file;      /**< File path to SSL certificate (portal public certificate)*/
    char            *pvt_key_file;  /**< File path to portal private key*/
    uint16_t        svr_port;       /**< Portal listening port */
} zwportal_init_t;

void *zwportal_init(zwportal_init_t *init_prm);
/**<
start portal to listen for secure TLS connection
@param[in]	    init_prm	Portal initialization parameters
@return Context on success, null on failure.
@post   Caller is required to call zwportal_shutdown() and zwportal_exit() with the returned context if it is not null.
*/

int zwportal_shutdown(void *ctx);
/**<
shutdown portal listening socket
@param[in]	    ctx		    The context returned from the call to zwportal_init()
@return Zero on success, non-zero on failure.
@post   Caller is required to call zwportal_exit() to free up resources.
*/

int zwportal_exit(void *ctx);
/**<
stop portal and free up resources
@param[in]	    ctx		    The context returned from the call to zwportal_init()
@return Zero on success, non-zero on failure.
@post After the call, the ctx is invalid and should not be used
*/

void zwportal_clnt_conn_close(int sfd, void *ssl);
/**<
close client's socket fd and SSL
@param[in]	    sfd	 Client socket fd
@param[in]	    ssl	 Client SSL object pointer
@return
*/

int zwportal_clnt_add(void *ctx, clnt_prof_t *clnt_prof);
/**<
add a client profile to the portal internal list, overwriting old entry with the same gateway id.
@param[in]	    ctx		    The context returned from the call to zwportal_init()
@param[in]	    clnt_prof	Client profile
@return                     Return non-zero on success, zero on failure.
*/

int zwportal_clnt_rm(void *ctx, uint8_t *gw_id);
/**<
remove a client profile from the portal internal list
@param[in]	    ctx		    The context returned from the call to zwportal_init()
@param[in]	    gw_id	    Gateway id
@return                     Return non-zero on success, zero on failure.
*/

int zwportal_clnt_find(void *ctx, clnt_prof_t *clnt_prof);
/**<
find a client profile in the portal internal list based on gateway id.
@param[in]	    ctx		    The context returned from the call to zwportal_init()
@param[in,out]	clnt_prof	Client profile buffer with the gateway id set as input key to be searched.
                            On success, this buffer will be returned with found entry.
@return                     Return non-zero on success, zero on failure.
*/

void zwportal_clnt_list_free(clnt_prof_lst *lst_hd);
/**<
free client profiles list
@param[in]	    lst_hd		List head of the client profiles list
@return
*/

int zwportal_clnt_list_get(void *ctx, clnt_prof_lst **lst_hd);
/**<
get all the client profiles in the portal internal list
@param[in]	    ctx		    The context returned from the call to zwportal_init()
@param[out]	    lst_hd		List head of the client profiles list
@return                     Number of client profiles in the list; negative value on failure.
@post Caller must call zwportal_clnt_list_free() to free the linked-list if return value is greater than zero.
*/


/**
@}
*/
#endif


/**
@defgroup If_SPoll Post-set Polling Interface APIs
Used to create and delete post-set polling commands to a device
@ingroup zwarecapi
@{
*/
#define ZWPSET_REASON_TGT_HIT       0   ///< Set target was hit
#define ZWPSET_REASON_TIMEOUT       1   ///< Timeout, set target was not hit
#define ZWPSET_REASON_UNSUPPORTED   2   ///< Post-set polling is unsupported for this interface
#define ZWPSET_REASON_DEVICE_RMV    3   ///< Device removed
#define ZWPSET_REASON_BUSY          4   ///< Device is busy.  Try again the zwif_xxx_set() function call later.
#define ZWPSET_REASON_SEND_FAILED   5   ///< Failed to send command.
#define ZWPSET_REASON_FAILED        6   ///< Failed to hit the target for some reason.


typedef void (*zw_postset_fn)(zwnet_p net, uint8_t node_id, uint8_t ep_id, uint16_t cls_id, void *usr_param, int reason);
/**<
post set polling callback function
@param[in]	net	        network
@param[in]	node_id	    node id
@param[in]	ep_id	    endpoint id
@param[in]	cls_id	    commadn class (interface) id
@param[in]	usr_param   user defined parameter
@param[in]	reason      reason of the callback, see ZWPSET_REASON_XXX
@return
*/

/**
@}
*/


/**
@defgroup If_Vendor Manufacturer Specific Interface APIs
Used to get manufacturer and product info
@ingroup zwarecapi
@{
*/

#define MAX_DEV_ID_LEN      31      /**< Maximum device id length */

/** @name Device_id_type_format
* Device id type and format definition
@{
*/
#define DEV_ID_TYPE_OEM     0       /**< Device id type is OEM factory default */
#define DEV_ID_TYPE_SN      1       /**< Device id type is serial number */
#define DEV_ID_TYPE_RANDOM  2       /**< Device id type is a pseudo random number */

#define DEV_ID_FMT_UTF      0       /**< Device id format is UTF-8 and MUST be displayed as is. */
#define DEV_ID_FMT_BIN      1       /**< Device id format is binary and MUST be displayed as hexadecimal values
                                         e.g. 0x30, 0x31, 0x32, 0x33 MUST be displayed as h’30313233. */

/**
@}
*/

/** device id */
typedef struct
{
    uint8_t   type;                         ///< device id type: DEV_ID_TYPE_XXX
    uint8_t   format;                       ///< device id data format: DEV_ID_FMT_XXX
    uint8_t   len;                          ///< device id length
    uint8_t   dev_id[MAX_DEV_ID_LEN + 1];   ///< device id

} dev_id_t;

/**
@}
@defgroup Node Node APIs
Nodes have certain attributes indicating its role and location in the network
and endpoints which indicate its utility to the user
@ingroup zwarecapi
@{
*/

/** Extended version information */
typedef struct
{
	uint8_t		hw_ver;		    /**< Hardware version*/
	uint8_t		fw_cnt;		    /**< Number of firmwares in the device, excluding the Z-wave firmware*/
    uint16_t    rsvd;           /**< Padding to make sure fw_ver is 4-byte aligned */
	uint16_t	fw_ver[1];		/**< Firmware versions place holder as indicated in fw_cnt*/
}
ext_ver_t;

/** Node property bit-mask definitions */
#define NODE_PROPTY_SECURE_CAP_S0   0x01    /**< Node capable to be included securely S0*/
#define NODE_PROPTY_SECURE_CAP_S2   0x02    /**< Node capable to be included securely S2*/
#define NODE_PROPTY_ADD_SECURE      0x04    /**< Node is included securely*/
#define NODE_PROPTY_ADD_INSECURE    0x08    /**< Node is included insecurely*/


/** Node Descriptor - read only for client */
typedef struct  _zwnoded
{
	uint8_t		nodeid;			/**< Node ID */
    uint8_t		propty;	        /**< Properties of the node (bit-mask): NODE_PROPTY_XXX */
	uint16_t	vid;			/**< Vendor ID */
	uint16_t	type;			/**< Vendor Product Type */
	uint16_t	pid;			/**< Product ID */
    zwnet_p     net;            /**< Network that the node belongs to */
    dev_id_t    dev_id;         /**< Device ID */
	uint16_t	proto_ver;		/**< Z-Wave Protocol Version*/
	uint16_t	app_ver;		/**< Application Version*/
	uint8_t		lib_type;		/**< Z-Wave Library Type*/
    uint8_t     category;	    /**< Device category, DEV_XXX */
	uint8_t		sensor;		    /**< Flag to indicate whether the node is a sensor (FLIRS) */
    uint8_t     sleep_cap;      /**< Flag to indicate the node is capable to sleep  (i.e. non-listening and support Wake up command class) */
    uint8_t     listen;         /**< Flag to indicate the node is always listening */
    int32_t     wkup_intv;      /**< Wake up interval in seconds. Negative value = invalid or unknown */
	uint8_t		s2_keys_valid;  /**< Flag to indicate whether s2_grnt_keys is valid */
	uint8_t		s2_grnt_keys;   /**< Security 2 granted keys (bit-mask), see SEC_KEY_BITMSK_XXX
                                     NOTE: This is valid only s2_keys_valid = 1*/
	uint8_t     security_incl_status;     /**< node security add status */   
	char		s2_dsk[MAX_DSK_STR_LEN + 1];     /**< S2 DSK. If s2_dsk[0] == '\0', the DSK is unavailable for this node */

}
zwnoded_t;


zwnet_p zwnode_get_net(zwnoded_p noded);
/**<
get node's associated network
@param[in]	noded	node
@return		network
*/

int zwnode_get_next(zwnoded_p noded, zwnoded_p nxt_noded);
/**<
get next node in network
@param[in]	noded	    The initial node for searching the next node
@param[out]	nxt_noded	The next node if found
@return		ZW_ERR_XXX
*/

int zwnode_get_ep(zwnoded_p noded, zwepd_p epd);
/**<
get first endpoint in device
@param[in]	noded	    node
@param[out]	epd	first endpoint
@return		ZW_ERR_XXX
*/

int zwnode_update(zwnoded_p noded);
/**<
update node status and information
@param[in]	noded	node
@return		ZW_ERR_XXX
*/

int zwnode_mul_cmd_ctl_set(zwnoded_p noded, uint8_t on);
/**<
start/stop multi command encapsulation
@param[in]	noded	node
@param[in]	on	    1=turn on multi command encapsulation;  0=turn off and flush
                    the commands in the buffer.
@return		ZW_ERR_xxx
*/

int zwnode_mul_cmd_ctl_get(zwnoded_p noded, uint8_t *on);
/**<
get the current state of multi command encapsulation
@param[in]	noded	    node
@param[out]	on	        state: 1=on; 0=off
return      ZW_ERR_XXX
*/

void zwnode_set_user(zwnoded_p noded, void *user);
/**<
set user specific information for node
@param[in]	noded	node
@param[in]	user	user specific information
*/

void *zwnode_get_user(zwnoded_p noded);
/**<
get user specific information for node
@param[in]	noded	node
@return	user specific information if found; else return NULL
*/

ext_ver_t *zwnode_get_ext_ver(zwnoded_p noded);
/**<
get extended version information
@param[in]	noded	Node
@return	extended version information if the node supports it; else return NULL
@post   Caller has to free the returned extended version information
*/


int zwnode_cmd_q_ena_set(zwnoded_p noded, uint8_t enable);
/**<
control the command queue
@param[in]	noded	    node
@param[in]	enable	    1=enable command queuing; 0=disable command queuing and
                        delete the commands in the queue.
return      ZW_ERR_XXX
*/


int zwnode_cmd_q_ena_get(zwnoded_p noded, uint8_t *enable);
/**<
get the setting of command queue control
@param[in]	noded	    node
@param[out]	enable	    current setting of command queue control;
                        1=enable command queuing; 0=disable command queuing
return      ZW_ERR_XXX
*/


uint8_t zwnode_cmd_q_get(zwnoded_p noded, uint16_t **cmd_q);
/**<
get command queue associated with the specified node
@param[in]	noded	node
@param[out]	cmd_q	command queue
@return     number of entries in cmd_q
@post       caller must free the cmd_q after this call if return value > 0
*/

int zwnode_cmd_q_cancel(zwnoded_p noded);
/**<
cancel all pending commands in command queue
@param[in]	noded	node
@return		ZW_ERR_XXX
*/

//Forward declaration of name location descriptor
struct _zw_nameloc;

int zwep_nameloc_set(zwepd_p epd, struct _zw_nameloc *nameloc);
/**<
zwep_nameloc_set - Set endpoint name and location string for device regardless whether the endpoint
                   has Node Naming And Location interface.  If it does, the strings will be sent to
                   the interface physically.
@param[in]	    epd	        The endpoint
@param[in,out]	nameloc	    Input:null terminated name & location string
                            Output:null terminated name & location string with proper UTF-8 delineation
@return		ZW_ERR_XXX
*/

/**
@}
@defgroup EP Endpoint APIs
Endpoints have interfaces that can received commands for a particular attribute of a utility
@ingroup zwarecapi
@{
*/

/** Z-wave+ info */
typedef struct
{
	uint8_t		zwplus_ver;		/**< Z-Wave+ version. Zero indicates this node is non-Z-Wave+ */
	uint8_t		node_type;		/**< Z-Wave+ node type*/
	uint8_t		role_type;		/**< Z-Wave+ role type*/
	uint16_t	instr_icon;		/**< Installer icon type */
	uint16_t	usr_icon;		/**< User icon type */

}
zwplus_info_t;

#define MAX_AGGR_MBR    126     ///<Max number of members in an aggregated end point

/** Endpoint Descriptor - read only for client */
typedef struct _zwepd
{
	uint8_t		    generic;		            /**< Generic Device Class */
	uint8_t		    specific;		            /**< Specific Device Class */
	uint8_t		    epid;			            /**< Endpoint ID */
	uint8_t		    nodeid;			            /**< Node ID */
	uint8_t		    aggr_ep_cnt;		        /**< Total number of end point members that are
                                                     represented by this aggregated end point. Zero means
                                                     this endpoint is NOT an aggregated end point*/
	uint8_t		    aggr_members[MAX_AGGR_MBR]; /**< Buffer to store the end point members that are
                                                     represented by this aggregated end point*/
    zwnet_p         net;                        /**< Network that the node belongs to */
    char		    name[ZW_LOC_STR_MAX + 1];   /**< user configured name string of the endpoint */
    char		    loc[ZW_LOC_STR_MAX + 1];	/**< user configured location string of the endpoint */
    zwplus_info_t   zwplus_info;                /**< Z-wave+ information */
}
zwepd_t;


int zwep_get_node(zwepd_p epd, zwnoded_p noded);
/**<
get ep's associated node
@param[in]	epd		endpoint
@param[out]	noded	node
@return		ZW_ERR_XXX
*/

int zwep_get_if(zwepd_p epd, zwifd_p ifd);
/**<
get first interface in endpoint
@param[in]	epd	    endpoint
@param[out]	ifd	    interface
@return		ZW_ERR_XXX
@post   Caller must free ifd->data if ifd->data_cnt > 0
*/

int zwep_get_next(zwepd_p epd, zwepd_p nxt_epd);
/**<
get next endpoint in node
@param[in]	epd	    endpoint
@param[out]	nxt_epd	next endpoint
@return		ZW_ERR_XXX
*/


void zwep_set_user(zwepd_p epd, void *user);
/**<
set user specific information for endpoint
@param[in]	epd	    endpoint
@param[in]	user	user specific information
*/

void *zwep_get_user(zwepd_p epd);
/**<
get user specific information for endpoint
@param[in]	epd	    Endpoint
@return	    user specific information
*/



/**
@}
@defgroup If Interface APIs
Interface with functionalities that correspond to Z-wave command class
@ingroup zwarecapi
@{
*/

/** interface property bit-mask definitions */
#define IF_PROPTY_SECURE            0x01    /**< Interface can be access securely bit-mask*/
#define IF_PROPTY_UNSECURE          0x02    /**< Interface can be access unsecurely bit-mask*/
#define IF_PROPTY_SECURE_S2         0x08    /**< Interface can be access securely S2 bit-mask*/
#define IF_PROPTY_ALRM_EVT_CLR      0x04    /**< Interface is capable to sent event clear notification. This bit-mask
                                                 is only valid for alarm/notification command class */
#define IF_PROPTY_BSNSR_EVT_CLR     0x04    /**< Interface is capable to sent event clear. This bit-mask
                                                 is only valid for binary sensor command class */
#define IF_PROPTY_ALRM_SNSR_EVT_CLR 0x04    /**< Interface is capable to sent event clear. This bit-mask
                                                 is only valid for alarm sensor command class */
#define IF_PROPTY_HIDDEN_POLL       0x40    /**< Interface is hidden but device polling is enabled. NOTE: This property is for internal use only,
                                                 user application MUST NOT use this.*/
#define IF_PROPTY_HIDDEN            0x80    /**< Interface is hidden. NOTE: This property is for internal use only,
                                                 user application MUST NOT use this.*/


/** interface descriptor */
typedef struct _zwifd
{
    uint16_t    cls;			/**< Command class */
    uint8_t		ver;			/**< Version of the command class. Could be overwritten by Database*/
	uint8_t		real_ver;		/**< Real Version of the command class that the device supports*/
	uint8_t		propty;	        /**< Properties of the interface (bit-mask): IF_PROPTY_XXX */
    uint8_t		epid;			/**< Endpoint ID */
    uint8_t		nodeid;			/**< Node ID */
    uint8_t	    data_cnt;	    /**< number of data items */
    void        *data;          /**< data items which is specific to the command class*/
    uint8_t     rsvd;           /**< Padding to make sure the structure is 4-byte aligned */
    zwnet_p     net;            /**< Network that the node belongs to */
}
zwifd_t;

/** Multi-level sensor interface data */
typedef struct
{
	uint8_t		sensor_type;    /**< supported sensor type */
	uint8_t		sensor_unit;    /**< supported sensor units (bit-mask)*/
} if_sensor_data_t;

/** Thermostat Fan Mode interface data */
typedef struct
{
	uint8_t		off;            /**< flag to indicate whether off mode is supported */
	uint8_t		mode_len;       /**< number of entries in mode buffer*/
    uint8_t     mode[1];        /**< mode place holder*/
} if_thrmo_fan_mode_data_t;

/** Thermostat Mode interface data */
typedef struct
{
	uint8_t		mode_len;       /**< number of entries in mode buffer*/
    uint8_t     mode[1];        /**< mode place holder*/
} if_thrmo_mode_data_t;

/** Thermostat Setpoint interface data */
typedef struct
{
	uint8_t		setp_len;       /**< number of entries in setp buffer*/
    uint8_t     setp[1];        /**< setpoint types place holder*/
} if_thrmo_setp_data_t;

/** Multilevel switch interface data */
typedef struct
{
	uint8_t		pri_type;       /**< primary switch type, SW_TYPE_XX */
    uint8_t     sec_type;       /**< secondary switch type , SW_TYPE_XX*/
} if_mul_switch_data_t;


#define MAX_EVT_BITMASK_LEN     10   /**< Maximum bitmask length*/
/** Alarm type and its associated events */
typedef struct
{
    uint8_t     ztype;                      /**< alarm type*/
    uint8_t     evt_len;                    /**< supported event bitmask length*/
	uint8_t		evt[MAX_EVT_BITMASK_LEN];   /**< event bitmask. Bit-0 in evt[0] is unused.
                                            Bit-1 in evt[0] set means event 0x01 is supported, and so on...*/
} if_alarm_type_evt_t;

/** Alarm interface data */
typedef struct
{
    uint8_t                 have_vtype;     /**< flag to indicate whether vendor specific alarm type supported*/
	uint8_t		            type_evt_cnt;   /**< number of entries in type_evt buffer*/
    uint16_t                rsvd;           /**< Padding to make sure type_evt is 4-byte aligned */
    if_alarm_type_evt_t     type_evt[1];    /**< alarm type-event place holder*/
} if_alarm_data_t;

/** Central scene interface data */
typedef struct
{
	uint8_t		scene_cnt;					/**< maximum number of supported scenes. */
	uint8_t		slow_rfsh;				    /**< flag to indicate support slow refresh of Key Held Down notification. Non-zero=support; 0=unsupported*/
	uint8_t		sameKA;						/**< if all scenes are supporting the same Key Attributes. 1=same; 0=different*/
	uint8_t     KA_array_len;				/**< length/width of key attribute array per scene (including the
                                                 leading byte to indicate valid KA length for each scene)*/
	uint8_t     KA_array[1];				/**< key attribute array of scenes place holder, see examples below*/
   /* KA Array format:
    * v1 interface -- sameKA 1, scene_cnt 5, KA_array_len 0, KA Array is NULL
    * V2 interface or above --
    * eg 1.
    * sameKA 1, scene_cnt 5, KA_array_len 4
    * KA_array: [3] [0] [1] [2]
    *
    * eg 2.
    * sameKA 0, scene_cnt 5, KA_array_len 4
    * KA_array: [3] [0] [1] [2]     //3 valid KAs in this row
    *			[2] [0] [5] [X]		//Only have 2 valid KAs in this row
    *			[0] [X] [X] [X]		//No valid KA in this row
    *			[3] [0] [3] [5]
    *			[1] [1] [X] [X]		//Only have 1 valid KA in this row
    *            ^
    *            |
    *           number of valid KAs in the row
    */
} if_csc_data_t;

/** Group member */
typedef struct
{
    uint8_t     node_id;    /**< node id*/
    uint8_t     ep_id;      /**< endpoint id*/
} grp_member_t;

typedef void (*zwrep_fn)(zwifd_p ifd, uint8_t level);
/**<
generic report callback for switch, binary sensor, battery, lock
@param[in]	ifd	    interface
@param[in]	level	level. Interpretation of level is device specific
*/

typedef void (*zwrep_ts_fn)(zwifd_p ifd, uint8_t level, time_t ts);
/**<
generic report callback with timestamp
@param[in]	ifd	    interface
@param[in]	level	level. Interpretation of level is device specific
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
                    values should be ignored.
*/

typedef void (*zwrep_group_fn)(zwifd_p ifd, uint8_t group, uint8_t max_cnt, uint8_t cnt, grp_member_t *grp_member);
/**<
group report callback
@param[in]	ifd	        Interface
@param[in]	group	    Grouping identifier
@param[in]	max_cnt	    Maximum number of members the grouping identifier above supports
@param[in]	cnt	        The number of members in the grouping in this report
@param[in]	grp_member	An array of cnt group members in the grouping. Note that the group members may contain
                        non-existence node/endpoint.
*/


int zwif_get_ep(zwifd_p ifd, zwepd_p epd);
/**<
get endpoint containing this interface
@param[in]	ifd		interface
@param[out]	epd		endpoint
@return		ZW_ERR_XXX
*/

int zwif_get_next(zwifd_p ifd, zwifd_p nxt_ifd);
/**<
get next interface in containing endpoint
@param[in]	ifd			interface
@param[out]	nxt_ifd		next interface
@return		ZW_ERR_XXX
@post   Caller must free nxt_ifd->data if nxt_ifd->data_cnt > 0
*/

int zwif_exec(zwifd_p ifdesc, uint8_t *cmd_buf, int buf_len, tx_cmplt_cb_t cb);
/**<
execute action on an interface
@param[in]	ifdesc      interface
@param[in]	cmd_buf		command and parameters
@param[in]	buf_len		length of cmd in bytes
@param[in]	cb		    callback function for transmit status
@return	ZW_ERR_XXX
*/

void zwif_set_user(zwifd_p ifd, void *user);
/**<
set user specific information for interface
@param[in]	ifd	    Interface
@param[in]	user	user specific information
*/

void *zwif_get_user(zwifd_p ifd);
/**<
get user specific information for interface
@param[in]	ifd	    interface
@return	user specific information else NULL
*/



/**
@}
@defgroup If_Loc Location Interface APIs
Clients can assign descriptive name and location strings
Their state can be read back by the generic zwif_get_report.
@ingroup zwarecapi
@{
*/

/** name location descriptor */
typedef struct _zw_nameloc
{
	char name[ZW_LOC_STR_MAX + 1];  /**< name string in UTF-8 */
	char loc[ZW_LOC_STR_MAX + 1];   /**< location string in UTF-8 */
}
zw_nameloc_t, *zw_nameloc_p;


/**
@}
@defgroup If_Grp Group Interface APIs
Groups are application-specific, and normally specify report recipients eg. for a sensor
@ingroup zwarecapi
@{
*/

int zwif_group_get(zwifd_p ifd, uint8_t group, zwrep_group_fn cb);
/**<
get information on specified group of interface through report callback
@param[in]		ifd	        interface
@param[in]		group	    group id
@param[in]		cb	        report callback function
@return  0 on success, else ZW_ERR_XXX
*/


int zwif_group_add(zwifd_p ifd, uint8_t group, zwepd_p ep, uint8_t cnt);
/**<
add endpoints to this group
@param[in]	ifd		interface
@param[in]	group	grouping identifier
@param[in]	ep		array of cnt end points in the grouping
@param[in]	cnt		number of end points
@return		ZW_ERR_XXX
*/


int zwif_group_del(zwifd_p ifd, uint8_t group, grp_member_t *grp_member, uint8_t cnt);
/**<
zwif_group_del - remove members from specified group of interface
@param[in]	ifd	        interface
@param[in]	group	    group id
@param[in]	grp_member  An array of cnt members to be removed from the grouping
@param[in]	cnt	        The number of members in the array grp_member. If cnt is zero,
                        all the members in the group may be removed.
@return  0 on success, else ZW_ERR_XXX
*/


typedef void (*zwrep_group_sup_fn)(zwifd_p ifd,  uint8_t max_grp, int valid);
/**<
report callback for maximum number of groupings the given node supports
@param[in]	ifd	      interface
@param[in]	max_grp   maximum number of groupings
@param[in]	valid     validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/


int zwif_group_sup_get(zwifd_p ifd,  zwrep_group_sup_fn cb, int cache);
/**<
get information on the maximum number of groupings the given node supports through report callback
@param[in]		ifd	    interface
@param[in]		cb	    report callback function
@param[in]	    cache	flag: to get data from cached only. If set, no fetching from real device when cache unavailable.
@return  0 on success, else ZW_ERR_XXX
*/


typedef void (*zwrep_group_actv_fn)(zwifd_p ifd,  uint8_t group);
/**<
report callback for current active group
@param[in]	ifd	    interface
@param[in]	group   current active group
*/


int zwif_group_actv_get(zwifd_p ifd,  zwrep_group_actv_fn cb);
/**<
get information on the current active group from a node through report callback
@param[in]		ifd	    interface
@param[in]		cb	    report callback function
@return  0 on success, else ZW_ERR_XXX
*/

/** Group command capabilities*/
typedef struct
{
    uint8_t     configurable;    /**< 1=command record is configurable, 0=not configurable */
    uint8_t     config_type;    /**< Configuration type: 1=value type (only support Basic Set command),
                                     0=command type (support any command)*/
    uint8_t     max_len;        /**< maximum command length that can be set */
    uint16_t    free_rec;       /**< number of free command records that can be set */
    uint16_t    total_rec;	    /**< total number of command records supported */
}
zwgrp_cmd_cap_t, * zwgrp_cmd_cap_p;


typedef void (*zwrep_grp_cmd_sup_fn)(zwifd_p ifd, zwgrp_cmd_cap_p  cmd_cap);
/**<
report callback for command records supporting capabilities
@param[in]	ifd	        interface
@param[in]	cmd_cap     command records supporting capabilities
*/


int zwif_group_cmd_sup_get(zwifd_p ifd,  zwrep_grp_cmd_sup_fn cb);
/**<
get information on command records supporting capabilities through report callback
@param[in]		ifd	    interface
@param[in]		cb	    report callback function
@return  0 on success, else ZW_ERR_XXX
*/


typedef void (*zwrep_grp_cmd_fn)(zwifd_p ifd, uint8_t group, uint8_t nodeid, uint8_t *cmd_buf, uint8_t len);
/**<
report callback for command record for a node within a given grouping identifier.
@param[in]	ifd	        interface
@param[in]	group	grouping identifier
@param[in]	nodeid	node id of the node within the grouping specified
@param[in]	cmd_buf	command and parameters
@param[in]	len     length of cmd_buf
*/


int zwif_group_cmd_get(zwifd_p ifd, uint8_t group, uint8_t nodeid, zwrep_grp_cmd_fn cb);
/**<
get command record for a node within a given grouping identifier through report callback
@param[in]	ifd	    interface
@param[in]	group	grouping identifier
@param[in]	nodeid	node id of the node within the grouping specified
@param[in]  cb	    report callback function
@return  0 on success, else ZW_ERR_XXX
*/


int zwif_group_cmd_set(zwifd_p ifd, uint8_t group, zwnoded_p node, uint8_t *cmd_buf, uint8_t len);
/**<
configure the command to send to the node in this group
@param[in]	ifd		interface
@param[in]	group	grouping identifier
@param[in]	node	node within the grouping specified, that should receive the command specified in cmd_buf
@param[in]	cmd_buf	command and parameters
@param[in]	len     length of cmd_buf
@return		ZW_ERR_XXX
*/

#define ZW_GRP_NAME_MAX	    42  ///< Group name maximum string length

/** group command entry*/
typedef struct
{
    uint16_t  cls;          ///< command class
    uint8_t   cmd;          ///< command
} grp_cmd_ent_t;

/** group information */
typedef struct _zw_grp_info
{
	uint8_t         grp_num;        /**< group number */
	uint8_t         cmd_ent_cnt;    /**< Number of entries in command list (cmd_lst) */
	uint16_t        profile;        /**< profile */
	uint16_t        evt_code;       /**< event code */
    uint16_t        rsvd;           /**< Padding to make sure cmd_lst is 4-byte aligned */
	char            name[ZW_GRP_NAME_MAX + 2];   /**< null terminated group name string in UTF-8 */
    grp_cmd_ent_t   cmd_lst[1];     /**< place holder for command list */
}
zw_grp_info_t, *zw_grp_info_p;

/** Association group info interface data */
typedef struct
{
	uint16_t	    group_cnt;      /**< Number of supported groups */
	uint16_t	    valid_grp_cnt;  /**< Number of valid group info in grp_info[]*/
    uint8_t         dynamic;        /**< Flag to indicate the group info is dynamic. 1=dynamic; 0=static */
    uint8_t         rsvd1;          /**< Padding to make sure grp_info is 4-byte aligned */
    uint16_t        rsvd2;          /**< Padding to make sure grp_info is 4-byte aligned */
    zw_grp_info_p   grp_info[1];    /**< Place holder for storing pointers to group information */

} if_grp_info_dat_t;

int zwif_group_info_get(zwifd_p ifd, if_grp_info_dat_t **grp_info);
/**<
get detailed group information
@param[in]	ifd	        interface
@param[out]	grp_info	grouping information if success; NULL on failure
@return  0 on success, else ZW_ERR_XXX
@post  Caller is required to call zwif_group_info_free to free the memory allocated to grp_info
*/

void zwif_group_info_free(if_grp_info_dat_t *grp_info);
/**<
free group information
@param[in]	grp_info	grouping information returned by zwif_group_info_get()
@return
@post   Caller should not use the grp_info after this call.
*/

/**
@}
@defgroup If_Wku Wake Up Interface APIs
Wake up APIs are for battery powered device that sleep most of the time
@ingroup zwarecapi
@{
*/

/** wake up interval settings in seconds */
typedef struct
{
	uint32_t	min;		/**< minimum */
	uint32_t	max;		/**< maximum */
	uint32_t	def;		/**< default */
	uint32_t	interval;	/**< steps between min and max */
	uint32_t	cur;		/**< current */
	zwnoded_t	node;       /**< node to notify wake up */
}
zwif_wakeup_t, *zwif_wakeup_p;

typedef int (*zwrep_wakeup_fn)(zwifd_p ifd, zwif_wakeup_p cap);
/**<
wake up report or notification callback
@param[in]	ifd	    interface
@param[in]	cap		capabilities report, null for notification
@return	    Only apply to notification: 0=no command pending to send; 1=commands pending to send.
*/


int  zwif_wakeup_set(zwifd_p ifd, uint32_t secs, zwnoded_p node);
/**<
set wake up interval and node to notify on wake up
@param[in]	ifd	    interface
@param[in]	secs	interval in seconds (24 bit)
@param[in]	node	node to notify
@return		ZW_ERR_XXX
*/

int  zwif_wakeup_get(zwifd_p ifd, zwrep_wakeup_fn cb);
/**<
get wake up report
@param[in]	ifd	    interface
@param[in]	cb	    report callback function
@return		ZW_ERR_XXX
*/


/**
@}
@defgroup If_Swt Switch Interface APIs
Switches can be switched on/off
@ingroup zwarecapi
@{
*/

/** Switch data */
typedef struct
{
    uint8_t curr_val;  	/**< current value: 0 = off; 0xFE = Unknown; 0xFF = On. */
    uint8_t tgt_val;	/**< target value with same interpretation as curr_val */
    uint8_t dur;	    /**< Duration: 0 = already at the target; 0x01~0x7F = seconds; 0x80~0xFD = 1~126 minutes;
                                       0xFE = Unknown duration; 0xFF = reserved*/
}
zwswitch_t, *zwswitch_p;

typedef void (*zwrep_switch_fn)(zwifd_p ifd, zwswitch_p val, time_t ts);
/**<
report callback for switch
@param[in]	ifd	    interface
@param[in]	val		switch data
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
                    values should be ignored.
*/

int zwif_switch_set(zwifd_p ifd, uint8_t on, zw_postset_fn cb, void *usr_param);
/**<
turn on/off switch
@param[in]	ifd		    interface
@param[in]	on		    0=off, else on
@param[in]	cb		    Optional post-set polling callback function.  NULL if no callback required.
@param[in]	usr_param   Optional user-defined parameter passed in callback.
@return		ZW_ERR_XXX
*/

int zwif_switch_rpt_set(zwifd_p ifd, zwrep_switch_fn rpt_cb);
/**<
setup a switch report callback function
@param[in]	ifd         interface
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/


int zwif_switch_get(zwifd_p ifd, int flag);
/**<
get switch report through report callback
@param[in]	ifd	        interface
@param[in]	flag	    flag, see ZWIF_GET_BMSK_XXX
@return		ZW_ERR_XXX
*/


/**
@}
@defgroup If_Lvl Level Interface APIs
Levels can be set and auto-(in/de)cremented with start/stop
@ingroup zwarecapi
@{
*/

/** Multilevel switch data */
typedef struct
{
    uint8_t curr_val;  	/**< current value: 0 = off; 0x01~0x63 = percentage (%);0xFE = Unknown; 0xFF = On. */
    uint8_t tgt_val;	/**< target value with same interpretation as curr_val */
    uint8_t dur;	    /**< Duration: 0 = already at the target; 0x01~0x7F = seconds; 0x80~0xFD = 1~126 minutes;
                                       0xFE = Unknown duration; 0xFF = reserved*/
}
zwlevel_dat_t, *zwlevel_dat_p;

typedef void (*zwrep_level_fn)(zwifd_p ifd, zwlevel_dat_p val, time_t ts);
/**<
multilevel switch report callback
@param[in]	ifd     interface
@param[in]	val	    multilevel switch data
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
                    values should be ignored.
*/

int zwif_level_set(zwifd_p ifd, uint8_t v, uint8_t dur, zw_postset_fn cb, void *usr_param);
/**<
set level
@param[in]	ifd		    interface
@param[in]	v		    0=off, 0xFF=on(previous level), 1-99=%
@param[in]	dur	        Dimming duration.  0=instantly;  0x01 to 0x7F = 1 second (0x01) to 127 seconds (0x7F);
                        0x80 to 0xFE = 1 minute (0x80) to 127 minutes (0xFE); 0xFF = factory default rate.
@param[in]	cb		    Optional post-set polling callback function.  NULL if no callback required.
@param[in]	usr_param   Optional user-defined parameter passed in callback.
@return	ZW_ERR_XXX
*/

/** Multilevel control*/
typedef struct
{
    uint8_t     pri_dir;        /**< primary switch direction.  0 = increase level; 1 = decrease level;3=no change */
    uint8_t     pri_level;      /**< primary switch start level */
    uint8_t     pri_ignore_lvl; /**< flag for ignoring primary switch start level.  0=use the start level as in pri_level;
                                     1=don’t use the start level and instead start from the actual level in the device. */
    uint8_t     sec_dir;        /**< secondary switch direction.  0 = increase level; 1 = decrease level;3=no change */
    uint8_t     sec_step;       /**< secondary switch step size. 0 to 99 = percentage of steps the increment or decrement
                                     function should execute. 255 (0xFF) = fixed step size defined by the OEM */
    uint8_t     dur;            /**< Dimming duration in seconds which is the interval it takes to dim from level 0 to 99 */
}
zwlevel_t, *zwlevel_p;

int zwif_level_start(zwifd_p ifd, zwlevel_p  level_ctrl);
/**<
start modifying levels
@param[in]	ifd	        interface
@param[in]	level_ctrl	level control of switches
@return	ZW_ERR_XXX
*/

int zwif_level_stop(zwifd_p ifd);
/**<
stop changing level
@param[in]	ifd		interface
@return		ZW_ERR_XXX
*/

int zwif_level_rpt_set(zwifd_p ifd, zwrep_level_fn rpt_cb);
/**<
setup a level report callback function
@param[in]	ifd         interface
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_level_get(zwifd_p ifd, int flag);
/**<
get level report through report callback
@param[in]	ifd	        interface
@param[in]	flag	    flag, see ZWIF_GET_BMSK_XXX
@return		ZW_ERR_XXX
*/


/** @name Level_switch_type
* Switch type definition
@{
*/
#define    SW_TYPE_NOT_SUP	        0x00	/**< Not present or supported */
#define    SW_TYPE_OFF_ON	        0x01	/**< When level =0 the switch is off,  level=0x63/0xFF the switch is on*/
#define    SW_TYPE_DOWN_UP	        0x02	/**< When level =0 the switch is down,  level=0x63/0xFF the switch is up*/
#define    SW_TYPE_CLOSE_OPEN	    0x03	/**< When level =0 the switch is close,  level=0x63/0xFF the switch is open*/
#define    SW_TYPE_COUNTER_CLOCKWISE 0x04	/**< When level =0 the switch is turning counter-clockwise,
                                                 level=0x63/0xFF the switch is turning clockwise*/
#define    SW_TYPE_LEFT_RIGHT	    0x05	/**< When level =0 the switch is left,  level=0x63/0xFF the switch is right*/
#define    SW_TYPE_REV_FWD	        0x06	/**< When level =0 the switch is reverse,  level=0x63/0xFF the switch is forward*/
#define    SW_TYPE_PULL_PUSH	    0x07	/**< When level =0 the switch is pulled,  level=0x63/0xFF the switch is pushed*/
/**
@}
*/

typedef void (*zwrep_lvl_sup_fn)(zwifd_p ifd,  uint8_t pri_type, uint8_t sec_type, int valid);
/**<
report callback for supported multilevel switch types
@param[in]	ifd	        interface
@param[in]	pri_type    primary switch type, SW_TYPE_XX
@param[in]	sec_type    secondary switch type , SW_TYPE_XX.
@param[in]	valid       validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/

int zwif_level_sup_get(zwifd_p ifd, zwrep_lvl_sup_fn cb, int cache);
/**<
get a switch type report through report callback
@param[in]	ifd	        interface
@param[in]	cb	        callback function to receive the supported switch type report
@param[in]	cache	    flag: to get data from cached only. If set, no fetching from real device when cache unavailable.
@return		ZW_ERR_XXX
*/

int zwif_level_sup_cache_get(zwifd_p ifd, uint8_t *pri_type, uint8_t *sec_type);
/**<
get the supported switch types from cache
@param[in]	ifd	        interface
@param[out]	pri_type    primary switch type, SW_TYPE_XX
@param[out]	sec_type    secondary switch type , SW_TYPE_XX.
@return ZW_ERR_XXX
*/

/**
@}
@defgroup If_Color_Swt Color Switch Interface APIs
Used to control color capable device
@ingroup zwarecapi
@{
*/

/** @name Color_sw_component_id
* Color switch component id definition
@{
*/
#define    COL_SW_COMP_ID_WARM_WHITE	0   /**< Warm white */
#define    COL_SW_COMP_ID_COLD_WHITE	1   /**< Cold white */
#define    COL_SW_COMP_ID_RED	        2   /**< Red */
#define    COL_SW_COMP_ID_GREEN	        3   /**< Green */
#define    COL_SW_COMP_ID_BLUE	        4   /**< Blue */
#define    COL_SW_COMP_ID_AMBER	        5   /**< Amber (for 6 channels color mixing) */
#define    COL_SW_COMP_ID_CYAN	        6   /**< Cyan (for 6 channels color mixing) */
#define    COL_SW_COMP_ID_PURPLE	    7   /**< Purple (for 6 channels color mixing) */
#define    COL_SW_COMP_ID_INDEX	        8   /**< Indexed color */
/**
@}
*/

/** @name Color_sw_dir
* Color switch component level change direction definition
@{
*/
#define    COL_SW_LVL_UP	0   /**< increase level */
#define    COL_SW_LVL_DOWN	1   /**< decrease level */
/**
@}
*/

typedef void (*zwrep_color_sw_sup_fn)(zwifd_p ifd, uint8_t comp_cnt, uint8_t *comp_id, int valid);
/**<
report callback for supported color components
@param[in]	ifd	        interface
@param[in]	comp_cnt    number of color components in comp_id array
@param[in]	comp_id     color component id (see COL_SW_COMP_ID_XXX )array
@param[in]	valid       validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/

int zwif_color_sw_sup_get(zwifd_p ifd, zwrep_color_sw_sup_fn cb, int cache);
/**<
get supported color components through report callback
@param[in]	ifd	        interface
@param[in]	cb	        callback function
@param[in]	cache	    flag: to get data from cached only. If set, no fetching from real device when cache unavailable.
@return		ZW_ERR_XXX
*/

int zwif_color_sw_sup_cache_get(zwifd_p ifd, uint8_t *comp_cnt, uint8_t *comp_id);
/**<
get supported color components from cache
@param[in]	ifd	        interface
@param[out]	comp_cnt    number of color components
@param[out]	comp_id     caller supplied buffer of size 255 bytes to store supported color component id (see COL_SW_COMP_ID_XXX )
@return ZW_ERR_XXX
*/

/** color switch data */
typedef struct
{
    uint8_t     id;			/**< color component id, COL_SW_COMP_ID_XXX */
    uint8_t     value;  	/**< value or current value */
    uint8_t     target_val;	/**< target value */
    uint8_t     dur;	    /**< Duration: 0 = already at the target; 0x01~0x7F = seconds; 0x80~0xFD = 1~126 minutes;
                                 0xFE = Unknown duration; 0xFF = reserved*/
}
zwcolor_t, *zwcolor_p;

typedef void (*zwrep_color_sw_get_fn)(zwifd_p ifd, zwcolor_p data, time_t ts);
/**<
report callback with timestamp for color switch
@param[in]	ifd	        interface
@param[in]	data        color component data
@param[in]	ts      	time stamp.  If this is zero, the callback has no data and hence other parameter
						values should be ignored.
*/

int zwif_color_sw_rpt_set(zwifd_p ifd, zwrep_color_sw_get_fn rpt_cb);
/**<
setup a color switch report callback function
@param[in]	ifd         interface
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_color_sw_get(zwifd_p ifd, uint8_t id, int flag);
/**<
get a color component value through report callback
@param[in]	ifd	        interface
@param[in]	id	        color component id, COL_SW_COMP_ID_XXX
@param[in]	flag	    flag, see ZWIF_GET_BMSK_XXX
@return		ZW_ERR_XXX
*/

int zwif_color_sw_set(zwifd_p ifd, uint8_t cnt, uint8_t *id, uint8_t *val, uint8_t dur);
/**<
set color component values
@param[in]	ifd	        interface
@param[in]	cnt	        color component count (maximum 31)
@param[in]	id	    	pointer to first element of the color component id (see COL_SW_COMP_ID_XXX) array
@param[in]	val	    	pointer to first element of the color component value array
@param[in]	dur	        Level change duration.  0=instantly;  0x01 to 0x7F = 1 second (0x01) to 127 seconds (0x7F);
                        0x80 to 0xFE = 1 minute (0x80) to 127 minutes (0xFE); 0xFF = factory default rate.
@@return		ZW_ERR_XXX
*/

/** Color switch level control*/
typedef struct
{
    uint8_t     dir;        	/**< color switch direction, COL_SW_LVL_XXX */
	uint8_t     id;    			/**< color component id, COL_SW_COMP_ID_XXX  */
    uint8_t     use_start_lvl;  /**< flag to indicate whether to start changing level from user supplied start_level.
                                     1=use user supplied start level; 0=start from current level */
    uint8_t     start_level;    /**< color switch start level (only valid if use_start_lvl=1) */
	uint8_t     dur;            /**< level change duration */
}
zwcol_ctl_t, *zwcol_ctl_p;

int zwif_color_sw_start(zwifd_p ifd, zwcol_ctl_p  color_ctrl);
/**<
start increasing/decreasing color switch level
@param[in]	ifd	        interface
@param[in]	color_ctrl	level control of color switch
@return	ZW_ERR_XXX
*/

int zwif_color_sw_stop(zwifd_p ifd, uint8_t id);
/**<
stop changing color switch level
@param[in]	ifd		interface
@param[in]	id	    color component id, COL_SW_COMP_ID_XXX
@return		ZW_ERR_XXX
*/


/**
@}
@defgroup BSns Binary Sensor Interface APIs
Binary sensors state can be idle (no event) or event detected
Their state can be read back by the generic zwif_get_report.
@ingroup zwarecapi
@{
*/

typedef void (*zwrep_bsensor_fn)(zwifd_p ifd, uint8_t state, uint8_t type, time_t ts);
/**<
report callback for binary sensor
@param[in]	ifd	    interface
@param[in]	state	0=idle, else event detected
@param[in]	type	sensor type, ZW_BSENSOR_TYPE_XXX. If type equals to zero, sensor type is unknown.
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
                    values should be ignored.
*/

int zwif_bsensor_rpt_set(zwifd_p ifd, zwrep_bsensor_fn rpt_cb);
/**<
setup a binary sensor report callback function
@param[in]	ifd         interface
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_bsensor_get(zwifd_p ifd, uint8_t type, int flag);
/**<
get binary sensor report through report callback
@param[in]	ifd	        interface
@param[in]	type	    preferred sensor type, ZW_BSENSOR_TYPE_XXX. If type equals to zero, the
                        sensor report will return the factory default sensor type.
@param[in]	flag	    flag, see ZWIF_GET_BMSK_XXX
@return		ZW_ERR_XXX
*/

typedef void (*zwrep_bsensor_sup_fn)(zwifd_p ifd, uint8_t type_len, uint8_t *type, int valid);
/**<
report callback for supported binary sensor types
@param[in]	ifd	       interface
@param[in]	type_len   size of sensor type buffer
@param[in]	type       buffer to store supported sensor types (ZW_BSENSOR_TYPE_XXX)
@param[in]	valid      validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/

int zwif_bsensor_sup_get(zwifd_p ifd, zwrep_bsensor_sup_fn cb, int cache);
/**<
get the supported binary sensor types through report callback
@param[in]	ifd	    interface
@param[in]	cb	    report callback function
@param[in]	cache	flag: to get data from cached only. If set, no fetching from real device when cache unavailable.
@return ZW_ERR_XXX
*/

int zwif_bsensor_sup_cache_get(zwifd_p ifd, uint8_t *snsr_cnt, uint8_t *sup_snsr);
/**<
get supported sensor types from cache
@param[in]	ifd	        interface
@param[out]	snsr_cnt	supported sensor counts
@param[out]	sup_snsr    caller supplied buffer of size 255 bytes to store supported sensors (ZW_BSENSOR_TYPE_XXX)
@return ZW_ERR_XXX
*/

/**
@}
@defgroup Sns Sensor Interface APIs
Multi-level sensors value can be 1, 2 or 4 bytes signed number
Their value can be read back by the generic zwif_get_report.
@ingroup zwarecapi
@{
*/

/** @name Sensor_type
* Sensor Type definition
@{
*/
#define ZW_SENSOR_TYPE_TEMP      0x01    /**< Temperature sensor*/
#define ZW_SENSOR_TYPE_GP        0x02    /**< General purpose sensor*/
#define ZW_SENSOR_TYPE_LUM       0x03    /**< Luminance sensor*/
#define ZW_SENSOR_TYPE_POW       0x04    /**< Power sensor*/
#define ZW_SENSOR_TYPE_HUMD      0x05    /**< Relative humidity sensor*/
#define ZW_SENSOR_TYPE_VELO      0x06    /**< Velocity sensor*/
#define ZW_SENSOR_TYPE_DIR       0x07    /**< Direction sensor*/
#define ZW_SENSOR_TYPE_ATM       0x08    /**< Atmospheric pressure sensor*/
#define ZW_SENSOR_TYPE_BARO      0x09    /**< Barometric pressure sensor*/
#define ZW_SENSOR_TYPE_SLR       0x0A    /**< Solar radiation sensor*/
#define ZW_SENSOR_TYPE_DEW       0x0B    /**< Dew point sensor*/
#define ZW_SENSOR_TYPE_RAIN      0x0C    /**< Rain rate sensor*/
#define ZW_SENSOR_TYPE_TIDE      0x0D    /**< Tide level sensor*/
#define ZW_SENSOR_TYPE_WGT       0x0E    /**< Weight sensor*/
#define ZW_SENSOR_TYPE_VOLT      0x0F    /**< Voltage sensor*/
#define ZW_SENSOR_TYPE_CUR       0x10    /**< Current sensor*/
#define ZW_SENSOR_TYPE_CO2       0x11    /**< CO2-level sensor*/
#define ZW_SENSOR_TYPE_AIR       0x12    /**< Air flow sensor*/
#define ZW_SENSOR_TYPE_TANK      0x13    /**< Tank capacity sensor*/
#define ZW_SENSOR_TYPE_DIST      0x14    /**< Distance sensor*/
#define ZW_SENSOR_TYPE_AGL       0x15    /**< Angle Position sensor*/
#define ZW_SENSOR_TYPE_ROT       0x16    /**< Rotation sensor*/
#define ZW_SENSOR_TYPE_WTR_TEMP  0x17    /**< Water temperature sensor*/
#define ZW_SENSOR_TYPE_SOIL_TEMP 0x18    /**< Soil temperature sensor*/
#define ZW_SENSOR_TYPE_SEIS_INT  0x19    /**< Seismic intensity sensor*/
#define ZW_SENSOR_TYPE_SEIS_MAG  0x1A    /**< Seismic magnitude sensor*/
#define ZW_SENSOR_TYPE_UV        0x1B    /**< Ultraviolet sensor*/
#define ZW_SENSOR_TYPE_ELEC_RES  0x1C    /**< Electrical resistivity sensor*/
#define ZW_SENSOR_TYPE_ELEC_COND 0x1D    /**< Electrical conductivity sensor*/
#define ZW_SENSOR_TYPE_LOUDNESS  0x1E    /**< Loudness sensor*/
#define ZW_SENSOR_TYPE_MOIST     0x1F    /**< Moisture sensor*/
#define ZW_SENSOR_TYPE_FREQ      0x20    /**< Frequency sensor*/
#define ZW_SENSOR_TYPE_TIME      0x21    /**< Time sensor*/
#define ZW_SENSOR_TYPE_TGT_TEMP  0x22    /**< Target temperature sensor*/
#define ZW_SENSOR_TYPE_PM_2_5    0x23    /**< Particulate matter 2.5 sensor*/
#define ZW_SENSOR_TYPE_F_CH2O    0x24    /**< Formaldehyde CH2O-level sensor*/
#define ZW_SENSOR_TYPE_RAD_CONT  0x25    /**< Radon Concentration sensor*/
#define ZW_SENSOR_TYPE_METH_DENS 0x26    /**< Methane Density CH4 sensor*/
#define ZW_SENSOR_TYPE_VOC       0x27    /**< Volatile Organic Compound sensor*/
#define ZW_SENSOR_TYPE_CO_LVL    0x28    /**< Carbon Monoxide CO-level sensor*/
#define ZW_SENSOR_TYPE_SOIL_HUMD 0x29    /**< Soil Humidity sensor*/
#define ZW_SENSOR_TYPE_SOIL_REAC 0x2A    /**< Soil Reactivity sensor*/
#define ZW_SENSOR_TYPE_SOIL_SAL  0x2B    /**< Soil Salinity sensor*/
#define ZW_SENSOR_TYPE_HEART_RT  0x2C    /**< Heart Rate sensor*/
#define ZW_SENSOR_TYPE_BLOOD_PRS 0x2D    /**< Blood Pressure sensor*/
#define ZW_SENSOR_TYPE_MUSCLE_MS 0x2E    /**< Muscle Mass sensor*/
#define ZW_SENSOR_TYPE_FAT_MS    0x2F    /**< Fat Mass sensor*/
#define ZW_SENSOR_TYPE_BONE_MS   0x30    /**< Bone Mass sensor*/
#define ZW_SENSOR_TYPE_TBW       0x31    /**< Total Body Water sensor*/
#define ZW_SENSOR_TYPE_BMR       0x32    /**< Basic Metabolic Rate sensor*/
#define ZW_SENSOR_TYPE_BMI       0x33    /**< Body Mass Index sensor*/
#define ZW_SENSOR_TYPE_ACCEL_X   0x34    /**< Acceleration, X-axis sensor*/
#define ZW_SENSOR_TYPE_ACCEL_Y   0x35    /**< Acceleration, Y-axis sensor*/
#define ZW_SENSOR_TYPE_ACCEL_Z   0x36    /**< Acceleration, Z-axis sensor*/
#define ZW_SENSOR_TYPE_SMOKE_DEN 0x37    /**< Smoke Density sensor*/
#define ZW_SENSOR_TYPE_WATER_FLW 0x38    /**< Water Flow sensor*/
#define ZW_SENSOR_TYPE_WATER_PRS 0x39    /**< Water Pressure sensor*/
#define ZW_SENSOR_TYPE_RF_SGN    0x3A    /**< RF Signal Strength sensor*/
#define ZW_SENSOR_TYPE_PM_10     0x3B    /**< Particulate Matter 10 sensor*/
#define ZW_SENSOR_TYPE_RESPI_RATE       0x3C    /**< Respiratory rate sensor*/
#define ZW_SENSOR_TYPE_REL_MOD          0x3D    /**< Relative Modulation level sensor*/
#define ZW_SENSOR_TYPE_BOILER_WTR_TEMP  0x3E    /**< Boiler water temperature sensor*/
#define ZW_SENSOR_TYPE_DHW_TEMP         0x3F    /**< Domestic Hot Water (DHW) temperature sensor*/
#define ZW_SENSOR_TYPE_OUTSIDE_TEMP     0x40    /**< Outside temperature sensor*/
#define ZW_SENSOR_TYPE_EXHAUST_TEMP     0x41    /**< Exhaust temperature sensor*/
#define ZW_SENSOR_TYPE_WATER_CHLOR_LVL  0x42    /**< Water Chlorine level sensor*/
#define ZW_SENSOR_TYPE_WATER_ACID       0x43    /**< Water acidity sensor*/
#define ZW_SENSOR_TYPE_WATER_OXI_RED    0x44    /**< Water Oxidation reduction potential sensor*/

///@}

/** @name Sensor_unit
* Sensor Unit definition
@{
*/
#define ZW_SENSOR_UNIT_TEMP_CEL       0x00    /**< Air temperature unit: Celsius (C)*/
#define ZW_SENSOR_UNIT_TEMP_FAHR      0x01    /**< Air temperature unit: Fahrenheit (F)*/

#define ZW_SENSOR_UNIT_GP_PERC        0x00    /**< General purpose unit: Percentage value*/
#define ZW_SENSOR_UNIT_GP_NODIM       0x01    /**< General purpose unit: Dimensionless value*/

#define ZW_SENSOR_UNIT_LUM_PERC       0x00    /**< Luminance unit: Percentage value*/
#define ZW_SENSOR_UNIT_LUM_LUX        0x01    /**< Luminance unit: Lux*/

#define ZW_SENSOR_UNIT_POW_W          0x00    /**< Power unit: W*/
#define ZW_SENSOR_UNIT_POW_BTU        0x01    /**< Power unit: Btu/h*/

#define ZW_SENSOR_UNIT_HUMD_PERC      0x00    /**< Relative humidity unit: Percentage value*/
#define ZW_SENSOR_UNIT_HUMD_ABS       0x01    /**< Absolute humidity unit: (g/m3) */

#define ZW_SENSOR_UNIT_VELO_MS        0x00    /**< Velocity unit: m/s*/
#define ZW_SENSOR_UNIT_VELO_MPH       0x01    /**< Velocity unit: mph*/

#define ZW_SENSOR_UNIT_DIR_DEG        0x00    /**< Direction unit: 0 to 360 degrees. 0 = no wind, 90 = east,
                                                   180 = south, 270 = west, and 360 = north*/

#define ZW_SENSOR_UNIT_ATM_KPA        0x00    /**< Atmospheric pressure unit: kPa*/
#define ZW_SENSOR_UNIT_ATM_HG         0x01    /**< Atmospheric pressure unit: inches of Mercury*/

#define ZW_SENSOR_UNIT_BARO_KPA       0x00    /**< Barometric pressure unit: kPa*/
#define ZW_SENSOR_UNIT_BARO_HG        0x01    /**< Barometric pressure unit: inches of Mercury*/

#define ZW_SENSOR_UNIT_SLR_WM2        0x00    /**< Solar radiation unit: W/m2*/

#define ZW_SENSOR_UNIT_DEW_CEL        0x00    /**< Dew point unit: Celsius (C)*/
#define ZW_SENSOR_UNIT_DEW_FAHR       0x01    /**< Dew point unit: Fahrenheit (F)*/

#define ZW_SENSOR_UNIT_RAIN_MMH       0x00    /**< Rain rate unit: mm/h*/
#define ZW_SENSOR_UNIT_RAIN_INH       0x01    /**< Rain rate unit: in/h*/

#define ZW_SENSOR_UNIT_TIDE_M         0x00    /**< Tide level unit: m*/
#define ZW_SENSOR_UNIT_TIDE_FT        0x01    /**< Tide level unit: feet*/

#define ZW_SENSOR_UNIT_WGT_KG         0x00    /**< Weight unit: kg*/
#define ZW_SENSOR_UNIT_WGT_LBS        0x01    /**< Weight unit: pounds*/

#define ZW_SENSOR_UNIT_VOLT_V         0x00    /**< Voltage unit: V*/
#define ZW_SENSOR_UNIT_VOLT_MV        0x01    /**< Voltage unit: mV*/

#define ZW_SENSOR_UNIT_CUR_A          0x00    /**< Current unit: A*/
#define ZW_SENSOR_UNIT_CUR_MA         0x01    /**< Current unit: mA*/

#define ZW_SENSOR_UNIT_CO2_PPM        0x00    /**< CO2-level unit: ppm*/

#define ZW_SENSOR_UNIT_AIR_M3H        0x00    /**< Air flow unit: m3/h*/
#define ZW_SENSOR_UNIT_AIR_CFM        0x01    /**< Air flow unit: cfm (cubic feet per minute)*/

#define ZW_SENSOR_UNIT_TANK_L         0x00    /**< Tank capacity unit: l*/
#define ZW_SENSOR_UNIT_TANK_CBM       0x01    /**< Tank capacity unit: cbm*/
#define ZW_SENSOR_UNIT_TANK_GAL       0x02    /**< Tank capacity unit: US gallons*/

#define ZW_SENSOR_UNIT_DIST_M         0x00    /**< Distance unit: m*/
#define ZW_SENSOR_UNIT_DIST_CM        0x01    /**< Distance unit: cm*/
#define ZW_SENSOR_UNIT_DIST_FT        0x02    /**< Distance unit: feet*/

#define ZW_SENSOR_UNIT_AGL_PERC       0x00    /**< Angle Position unit: Percentage value*/
#define ZW_SENSOR_UNIT_AGL_DEGN       0x01    /**< Angle Position unit: Degrees relative to
                                                   north pole of standing eye view*/
#define ZW_SENSOR_UNIT_AGL_DEGS       0x02    /**< Angle Position unit: Degrees relative to
                                                   south pole of standing eye view*/

#define ZW_SENSOR_UNIT_ROT_RPM        0x00    /**< Rotation unit: rpm (revolutions per minute)*/
#define ZW_SENSOR_UNIT_ROT_HZ         0x01    /**< Rotation unit: Hz (Hertz)*/

#define ZW_SENSOR_UNIT_WTR_TEMP_CEL   0x00    /**< Water temperature unit: Celsius (C)*/
#define ZW_SENSOR_UNIT_WTR_TEMP_FAHR  0x01    /**< Water temperature unit: Fahrenheit (F)*/

#define ZW_SENSOR_UNIT_SOIL_TEMP_CEL  0x00    /**< Soil temperature unit: Celsius (C)*/
#define ZW_SENSOR_UNIT_SOIL_TEMP_FAHR 0x01    /**< Soil temperature unit: Fahrenheit (F)*/

#define ZW_SENSOR_UNIT_SEIS_INT_MERC  0x00    /**< Seismic intensity unit: Mercalli*/
#define ZW_SENSOR_UNIT_SEIS_INT_EMCRO 0x01    /**< Seismic intensity unit: European Macroseismic*/
#define ZW_SENSOR_UNIT_SEIS_INT_LIEDU 0x02    /**< Seismic intensity unit: Liedu*/
#define ZW_SENSOR_UNIT_SEIS_INT_SHNDO 0x03    /**< Seismic intensity unit: Shindo*/

#define ZW_SENSOR_UNIT_SEIS_MAG_LOCAL 0x00    /**< Seismic magnitude unit: Local (ML)*/
#define ZW_SENSOR_UNIT_SEIS_MAG_MOM   0x01    /**< Seismic magnitude unit: Moment (MW)*/
#define ZW_SENSOR_UNIT_SEIS_MAG_SWAVE 0x02    /**< Seismic magnitude unit: Surface wave (MS)*/
#define ZW_SENSOR_UNIT_SEIS_MAG_BWAVE 0x03    /**< Seismic magnitude unit: Body wave (MB)*/

#define ZW_SENSOR_UNIT_UV_INDEX       0x00    /**< Ultraviolet unit: UV index*/

#define ZW_SENSOR_UNIT_ELEC_RES_OHMM  0x00    /**< Electrical resistivity unit: ohm metre*/

#define ZW_SENSOR_UNIT_ELEC_COND_SIEM 0x00    /**< Electrical conductivity unit: siemens per metre (S/m)*/

#define ZW_SENSOR_UNIT_LOUDNESS_ABS   0x00    /**< Loudness unit: Absolute loudness (dB)*/
#define ZW_SENSOR_UNIT_LOUDNESS_A_WT  0x01    /**< Loudness unit: A-weighted decibels (dBA)*/

#define ZW_SENSOR_UNIT_MOIST_PERC     0x00    /**< Moisture unit: Percentage value*/
#define ZW_SENSOR_UNIT_MOIST_VOL_WTR  0x01    /**< Moisture unit: Volume water content (m3/m3)*/
#define ZW_SENSOR_UNIT_MOIST_IMPD     0x02    /**< Moisture unit: Impedance (k ohm)*/
#define ZW_SENSOR_UNIT_MOIST_WTR_ACT  0x03    /**< Moisture unit: Water activity (aw)*/

#define ZW_SENSOR_UNIT_FREQ_HZ        0x00    /**< Frequency unit: Hz - Must be used until 4.294967295 GHz*/
#define ZW_SENSOR_UNIT_FREQ_KHZ       0x01    /**< Frequency unit: kHz- Must be used until 4.294967295 GHz*/

#define ZW_SENSOR_UNIT_TIME_SEC       0x00    /**< Time unit: seconds*/

#define ZW_SENSOR_UNIT_TGT_TEMP_CEL   0x00    /**< Target temperature unit: Celsius (C)*/
#define ZW_SENSOR_UNIT_TGT_TEMP_FAHR  0x01    /**< Target temperature unit: Fahrenheit (F)*/

#define ZW_SENSOR_UNIT_PM_2_5_MM3     0x00    /**< Particulate matter 2.5 unit: mole per cubic meter (mol/m3) */
#define ZW_SENSOR_UNIT_PM_2_5_UGM3    0x01    /**< Particulate matter 2.5 unit: Absolute microgram/cubic meter (ug/m3)*/

#define ZW_SENSOR_UNIT_F_CH2O_MM3     0x00    /**< Formaldehyde CH2O-level unit: mole per cubic meter (mol/m3) */

#define ZW_SENSOR_UNIT_RAD_CONT_BQM3  0x00    /**< Radon Concentration unit: Becquerel/cubic meter (bq/m3)*/
#define ZW_SENSOR_UNIT_RAD_CONT_PCIL  0x01    /**< Radon Concentration unit: picocuries/liter (pCi/L)*/

#define ZW_SENSOR_UNIT_METH_DENS_MM3  0x00    /**< Methane Density CH4 unit: mole per cubic meter (mol/m3) */

#define ZW_SENSOR_UNIT_VOC_MM3        0x00    /**< Volatile Organic unit: mole per cubic meter (mol/m3) */

#define ZW_SENSOR_UNIT_CO_LVL_MM3     0x00    /**< Carbon Monoxide CO-level unit: mole per cubic meter (mol/m3) */

#define ZW_SENSOR_UNIT_SOIL_HUMD_PERC 0x00    /**< Soil Humidity unit: Percentage value */

#define ZW_SENSOR_UNIT_SOIL_REAC_PH   0x00    /**< Soil Reactivity unit: acidity (pH) */

#define ZW_SENSOR_UNIT_SOIL_SAL_MM3   0x00    /**< Soil Salinity unit: mole per cubic meter (mol/m3) */

#define ZW_SENSOR_UNIT_HEART_RT_BPM   0x00    /**< Heart Rate unit: beats/minute (Bpm) */

#define ZW_SENSOR_UNIT_BLOOD_PRS_SYS  0x00    /**< Blood Pressure unit: Systolic mmHg (upper number) */
#define ZW_SENSOR_UNIT_BLOOD_PRS_DIA  0x01    /**< Blood Pressure unit: Diastolic mmHg (lower number)*/

#define ZW_SENSOR_UNIT_MUSCLE_MS_KG   0x00    /**< Muscle Mass unit: kilogram (kg) */

#define ZW_SENSOR_UNIT_FAT_MS_KG      0x00    /**< Fat Mass unit: kilogram (kg) */

#define ZW_SENSOR_UNIT_BONE_MS_KG     0x00    /**< Bone Mass unit: kilogram (kg) */

#define ZW_SENSOR_UNIT_TBW_KG         0x00    /**< Total Body Water unit: kilogram (kg) */

#define ZW_SENSOR_UNIT_BMR_J          0x00    /**< Basic Metabolic Rate unit: joule (J)*/

#define ZW_SENSOR_UNIT_BMI_IDX        0x00    /**< Body Mass Index unit: BMI Index */

#define ZW_SENSOR_UNIT_ACCEL_X_MS2    0x00    /**< Acceleration, X-axis unit: (m/s2) */

#define ZW_SENSOR_UNIT_ACCEL_Y_MS2    0x00    /**< Acceleration, Y-axis unit: (m/s2) */

#define ZW_SENSOR_UNIT_ACCEL_Z_MS2    0x00    /**< Acceleration, Z-axis unit: (m/s2) */

#define ZW_SENSOR_UNIT_SMOKE_DEN_PERC 0x00    /**< Smoke Density unit: Percentage value */

#define ZW_SENSOR_UNIT_WATER_FLW_LHR  0x00    /**< Water Flow unit: liter/hour (l/h) */

#define ZW_SENSOR_UNIT_WATER_PRS_KPA  0x00    /**< Water Pressure unit: kilopascal (kPa)*/

#define ZW_SENSOR_UNIT_RF_SGN_RSSI    0x00    /**< RF Signal Strength unit: RSSI (Percentage value) */
#define ZW_SENSOR_UNIT_RF_SGN_DBM     0x01    /**< RF Signal Strength unit: (dBm) */

#define ZW_SENSOR_UNIT_PM_10_MOLE     0x00    /**< Mole per cubic meter (mol/m3)*/
#define ZW_SENSOR_UNIT_PM_10_UG       0x01    /**< Microgram per cubic meter (µg/m3)*/

#define ZW_SENSOR_UNIT_RESPI_RATE_BPM 0x00    /**< Breaths per minute (bpm)*/

#define ZW_SENSOR_UNIT_REL_MOD_PERC   0x00    /**< Percentage value (%)*/

#define ZW_SENSOR_UNIT_BOILER_WTR_TEMP_C 0x00  /**< Celcius (C)*/

#define ZW_SENSOR_UNIT_DHW_TEMP_C     0x00     /**< Celcius (C)*/

#define ZW_SENSOR_UNIT_OUTSIDE_TEMP_C 0x00     /**< Celcius (C)*/

#define ZW_SENSOR_UNIT_EXHAUST_TEMP_C 0x00     /**< Celcius (C)*/

#define ZW_SENSOR_UNIT_WATER_CHLOR_LVL_MGL    0x00    /**< Milligram per liter (mg/l)*/

#define ZW_SENSOR_UNIT_WATER_ACID_PH  0x00     /**< Acidity (pH)*/

#define ZW_SENSOR_UNIT_WATER_OXI_RED_MV       0x00    /**< MilliVolt (mV)*/

///@}

/** sensor data */
typedef struct
{
    uint8_t     type;		/**< ZW_SENSOR_TYPE_XXX */
    uint8_t     precision;  /**< Decimal places of the value.  The decimal
                                 value 1025 with precision 2 is therefore equal to 10.25.*/
    uint8_t     unit;	    /**< ZW_SENSOR_UNIT_XXX */
    uint8_t     size;	    /**< data size: 1,2,or 4 bytes*/
    uint8_t     data[4];	/**< sensor data (a signed number) with the first byte
                                 is the most significant byte*/
}
zwsensor_t, *zwsensor_p;

typedef void (*zwrep_sensor_fn)(zwifd_p ifd, zwsensor_p data, time_t ts);
/**<
report callback for multi-level sensor
@param[in]	ifd	    interface
@param[in]	data	current value and unit of the sensor
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
                    values should be ignored.
*/

int zwif_sensor_rpt_set(zwifd_p ifd, zwrep_sensor_fn rpt_cb);
/**<
setup a multilevel sensor report callback function
@param[in]	ifd         interface
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_sensor_get(zwifd_p ifd, uint8_t type, uint8_t unit, int flag);
/**<
get multilevel sensor report through report callback
@param[in]	ifd	        interface
@param[in]	type	    preferred sensor type, ZW_SENSOR_TYPE_XXX. If type equals to zero, the
                        sensor report will return the factory default sensor type.
@param[in]	unit	    preferred sensor unit, ZW_SENSOR_UNIT_XXX.  This parameter is ignored
                        if type=0.
@param[in]	flag	    flag, see ZWIF_GET_BMSK_XXX
@return		ZW_ERR_XXX
@note  Preferred sensor type and unit are not guaranteed to be returned in the report callback.  It
       depends on the multilevel sensor command class version number and the device supported.
*/

typedef void (*zwrep_sensor_sup_fn)(zwifd_p ifd, uint8_t type_len, uint8_t *type, int valid);
/**<
report callback for supported sensor types
@param[in]	ifd	       interface
@param[in]	type_len   size of sensor type buffer
@param[in]	type       buffer to store supported sensor types (ZW_SENSOR_TYPE_XXX)
@param[in]	valid      validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/

int zwif_sensor_sup_get(zwifd_p ifd, zwrep_sensor_sup_fn cb, int cache);
/**<
get the supported sensor types through report callback
@param[in]	ifd	    interface
@param[in]	cb	    report callback function
@param[in]	cache	flag: to get data from cached only. If set, no fetching from real device when cache unavailable.
@return ZW_ERR_XXX
*/

typedef void (*zwrep_sensor_unit_fn)(zwifd_p ifd, uint8_t sensor_type, uint8_t sensor_unit_msk, int valid);
/**<
report callback for supported sensor types
@param[in]	ifd	            interface
@param[in]	sensor_type     sensor type, ZW_SENSOR_TYPE_XXX
@param[in]	sensor_unit_msk bitmask of units supported for the sensor_type, ZW_SENSOR_UNIT_XXX
@param[in]	valid           validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/

int zwif_sensor_unit_get(zwifd_p ifd, uint8_t sensor_type, zwrep_sensor_unit_fn cb, int cache);
/**<
get the supported sensor units through report callback
@param[in]	ifd	            interface
@param[in]	sensor_type	    sensor type
@param[in]	cb	            report callback function
@param[in]	cache	        flag: to get data from cached only. If set, no fetching from real device when cache unavailable.
@return ZW_ERR_XXX
*/

int zwif_sensor_unit_cache_get(zwifd_p ifd, uint8_t sensor_type, uint8_t *unit_cnt, uint8_t *sup_unit);
/**<
get supported sensor units from cache
@param[in]	ifd	        interface
@param[in]	sensor_type	sensor type
@param[out]	unit_cnt	supported sensor unit count
@param[out]	sup_unit	caller supplied buffer of size 255 bytes to store supported sensor units (ZW_SENSOR_UNIT_XXX)
@return ZW_ERR_XXX
*/

int zwif_sensor_sup_cache_get(zwifd_p ifd, uint8_t *snsr_cnt, uint8_t *sup_snsr);
/**<
get supported sensor types from cache
@param[in]	ifd	        interface
@param[out]	snsr_cnt	supported sensor counts
@param[out]	sup_snsr	caller supplied buffer of size 255 bytes to store supported sensor types (ZW_SENSOR_TYPE_XXX)
@return ZW_ERR_XXX
*/


/**
@}
@defgroup If_Cfg    Device Configuration APIs
Configure device specific parameters
@ingroup zwarecapi
@{
*/

/** Generic data type*/
typedef union
{
    uint8_t     u8_data;    /**< unsigned data for size=1*/
    int8_t      s8_data;    /**< signed data for size=1*/
    uint16_t    u16_data;   /**< unsigned data for size=2*/
    int16_t     s16_data;   /**< signed data for size=2*/
    uint32_t    u32_data;   /**< unsigned data for size=4*/
    int32_t     s32_data;   /**< signed data for size=4*/
} gen_dat_u;

/** Device configuration single parameter*/
typedef struct
{
    uint8_t     param_num;	    /**< parameter number */
    uint8_t     size;	        /**< data size: 1,2,or 4 bytes*/
    uint8_t     data[4];	    /**< data (a signed or unsigned number, depending on parameter number data format) with the
                                     first byte (i.e. data[0]) is the most significant byte*/
    uint8_t     use_default;	/**< only valid for configuration set command. Parameter flag: 1=use default factory setting and ignore data[];
                                     0=use the value in data[]*/
} zwconfig_t, *zwconfig_p;

#define MAX_CFG_PARAM_CNT   255  /**< Maximum configuration parameter counts*/

/** Device configuration multiple parameters*/
typedef struct
{
    uint16_t    param_start;	/**< starting parameter number */
    uint8_t     param_cnt;	    /**< parameter count*/
    uint8_t     size;	        /**< data size: 1,2,or 4 bytes*/
    uint8_t     handshake;	    /**< For bulk_set command: request the device to send Configuration Bulk Report once the parameters have been written.
                                                           1=request report; 0=do not request report.
                                     For bulk report command: 1=indicate that all configuration parameters have been stored in non-volatile memory;
                                                              0=indicate that this report is returned in response to a Configuration Bulk Get command*/
    uint8_t     deflt_data;     /**< For bulk_set command: 1=use default factory setting and ignore data[]; 0=use the value in data[].
                                     For bulk report command: 1=all advertised configuration parameters in data[] have the factory default value;
                                     0=one or more of the advertised configuration parameters in data[] do not have the factory default value*/
    union {
        uint8_t     u8_data;    /**< unsigned data for size=1*/
        int8_t      s8_data;    /**< signed data for size=1*/
        uint16_t    u16_data;   /**< unsigned data for size=2*/
        int16_t     s16_data;   /**< signed data for size=2*/
        uint32_t    u32_data;   /**< unsigned data for size=4*/
        int32_t     s32_data;   /**< signed data for size=4*/
    } data[MAX_CFG_PARAM_CNT];  /**< configuration data array with number of parameters specified by param_cnt */
} zwcfg_bulk_t, *zwcfg_bulk_p;

/**  Device configuration capabilities bit-mask*/
#define CFG_CAP_BULK        0x01    /**< Support bulk set and bulk report get APIs*/
#define CFG_CAP_DEFAULT_SET 0x02    /**< Support API to reset all configuration parameters to their default value */

/**  Cconfiguration parameter data format*/
#define CFG_FMT_SIGN_INT    0x00    /**< Signed integer */
#define CFG_FMT_UNSIGN_INT  0x01    /**< Unsigned integer*/
#define CFG_FMT_ENUM        0x02    /**< Enumerated (Radio buttons). It must be treated as unsigned integer*/
#define CFG_FMT_BIT_FIELD   0x03    /**< Bit field (Checkboxes). It must be treated as bit field where each individual bit can be set or reset.*/

/** Device configuration parameter information*/
typedef struct
{
    char        *name;          /**< Name of a parameter. NULL if unavailable.*/
    char        *info;          /**< Usage information of a parameter. NULL if unavailable.*/
    uint16_t    param_num;      /**< Parameter number */
    uint8_t     adv;            /**< Flag to indicate if the parameter is to be presented in the “Advanced” parameter section
                                     in the controller GUI. 1=advanced; 0=normal */
    uint8_t     read_only;      /**< Flag to indicate if the parameter is read-only. 1=read only; 0=read-write */
    uint8_t     re_incl_req;	/**< Flag to indicate if the advertised parameter requires the node to be re-included in the
                                     network before the parameter value change takes effect. 1=required re-inclusion; 0=not required re-inclusion */
    uint8_t     format;         /**< Format of data,  CFG_FMT_XXX */
    uint8_t     size;	        /**< Data size: 1,2,or 4 bytes. If zero, the "param_num" is invalid and all the data fields must be ignored*/
    gen_dat_u   min_val;        /**< Minimum value of data. Zero if format is bit field */
    gen_dat_u   max_val;        /**< Maximum value of data. If the format is “bit field”, each individual supported bit
                                     MUST be set to ‘1’, while each un-supported bit MUST be set to ‘0’. */
    gen_dat_u   dflt_val;       /**< Default value of data. */

} zwcfg_info_t, *zwcfg_info_p;

/** Device configuration parameters information and capabilities*/
typedef struct
{
    uint8_t         cap;        /**< Device configuration capabilities, CFG_CAP_XXX */
    uint16_t        cnt;        /**< Number of parameters info pointed by 'info' */
    zwcfg_info_p    info;       /**< Pointer to an array of parameters info*/
} zwcfg_info_cap_t, *zwcfg_info_cap_p;


int zwif_config_set(zwifd_p ifd, zwconfig_p param);
/**<
set configuration parameter
@param[in]	ifd	    interface
@param[in]	param	parameter to set.
@return	ZW_ERR_XXX
*/

typedef void (*zwrep_config_fn)(zwifd_p ifd, zwconfig_p param);
/**<
report callback for configuration parameter
@param[in]	ifd	    interface
@param[in]	param   parameter value
*/

int zwif_config_rpt_set(zwifd_p ifd, zwrep_config_fn rpt_cb);
/**<
setup a configuration parameter report callback function
@param[in]	ifd         interface
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_config_get(zwifd_p ifd, uint8_t param_num);
/**<
get configuration parameter report through report callback
@param[in]	ifd	        interface
@param[in]	param_num	parameter number of the value to get
@return		ZW_ERR_XXX
*/

typedef void (*zwrep_cfg_bulk_fn)(zwifd_p ifd, zwcfg_bulk_p param);
/**<
report callback for multiple configuration parameters
@param[in]	ifd	    interface
@param[in]	param   parameter value
*/

int zwif_config_bulk_rpt_set(zwifd_p ifd, zwrep_cfg_bulk_fn rpt_cb);
/**<
setup a multiple configuration parameters report callback function
@param[in]	ifd         interface
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_config_bulk_set(zwifd_p ifd, zwcfg_bulk_p param);
/**<
set multiple configuration parameters
@param[in]	ifd	    interface
@param[in]	param	parameters to set.
@return	ZW_ERR_XXX
*/

int zwif_config_bulk_get(zwifd_p ifd, uint16_t param_start, uint8_t param_cnt);
/**<
get multiple configuration parameters report through report callback
@param[in]	ifd	        interface
@param[in]	param_start	starting parameter number
@param[in]	param_cnt	number of parameters to retrieve
@return		ZW_ERR_XXX
*/

int zwif_config_prm_reset(zwifd_p ifd);
/**<
reset all configuration parameters to their default value
@param[in]	ifd	        interface
@return ZW_ERR_XXX
*/

int zwif_config_info_get(zwifd_p ifd, zwcfg_info_cap_t **cfg_info);
/**<
get configuration parameters information
@param[in]	ifd	        interface
@param[out]	cfg_info	configuration parameters information if success; NULL on failure
@return ZW_ERR_XXX
@post  Caller is required to call zwif_config_info_free to free the memory allocated to cfg_info
*/

void zwif_config_info_free(zwcfg_info_cap_t *cfg_info);
/**<
free configuration parameters information
@param[in]	cfg_info	configuration parameters information to be free
@return
@post   Caller should not use the cfg_info after this call.
*/

/**
@}
@defgroup If_Mtr Meter Interface APIs
Meters can be reset
@ingroup zwarecapi
@{
*/

/** Meter Type definition */
#define ZW_METER_TYPE_ELEC           0x01    /**< Electric meter*/
#define ZW_METER_TYPE_GAS            0x02    /**< Gas meter*/
#define ZW_METER_TYPE_WATER          0x03    /**< Water meter*/


/** Meter Unit definition */
#define ZW_METER_UNIT_ELEC_KWH       0x00    /**< Electric meter unit: kWh*/
#define ZW_METER_UNIT_ELEC_KVAH      0x01    /**< Electric meter unit: kVAh*/
#define ZW_METER_UNIT_ELEC_W         0x02    /**< Electric meter unit: W*/
#define ZW_METER_UNIT_ELEC_PULSE     0x03    /**< Electric meter unit: pulse count*/
#define ZW_METER_UNIT_ELEC_V         0x04    /**< Electric meter unit: V*/
#define ZW_METER_UNIT_ELEC_A         0x05    /**< Electric meter unit: A*/
#define ZW_METER_UNIT_ELEC_PF        0x06    /**< Electric meter unit: power factor*/

#define ZW_METER_UNIT_GAS_CM         0x00    /**< Gas meter unit: cubic meters*/
#define ZW_METER_UNIT_GAS_CF         0x01    /**< Gas meter unit: cubic feet*/
#define ZW_METER_UNIT_GAS_PULSE      0x03    /**< Gas meter unit: pulse count*/

#define ZW_METER_UNIT_WATER_CM       0x00    /**< Water meter unit: cubic meters*/
#define ZW_METER_UNIT_WATER_CF       0x01    /**< Water meter unit: cubic feet*/
#define ZW_METER_UNIT_WATER_GAL      0x02    /**< Water meter unit: US gallons*/
#define ZW_METER_UNIT_WATER_PULSE    0x03    /**< Water meter unit: pulse count*/


/** Meter Supported Unit Bit-mask definition : 1=supported, 0=not supported */
#define ZW_METER_SUP_UNIT_ELEC_KWH       0x01    /**< Electric meter unit: kWh*/
#define ZW_METER_SUP_UNIT_ELEC_KVAH      0x02    /**< Electric meter unit: kVAh*/
#define ZW_METER_SUP_UNIT_ELEC_W         0x04    /**< Electric meter unit: W*/
#define ZW_METER_SUP_UNIT_ELEC_PULSE     0x08    /**< Electric meter unit: pulse count*/
#define ZW_METER_SUP_UNIT_ELEC_V         0x10    /**< Electric meter unit: V*/
#define ZW_METER_SUP_UNIT_ELEC_A         0x20    /**< Electric meter unit: A*/
#define ZW_METER_SUP_UNIT_ELEC_PF        0x40    /**< Electric meter unit: power factor*/

#define ZW_METER_SUP_UNIT_GAS_CM         0x01    /**< Gas meter unit: cubic meters*/
#define ZW_METER_SUP_UNIT_GAS_CF         0x02    /**< Gas meter unit: cubic feet*/
#define ZW_METER_SUP_UNIT_GAS_PULSE      0x08    /**< Gas meter unit: pulse count*/

#define ZW_METER_SUP_UNIT_WATER_CM       0x01    /**< Water meter unit: cubic meters*/
#define ZW_METER_SUP_UNIT_WATER_CF       0x02    /**< Water meter unit: cubic feet*/
#define ZW_METER_SUP_UNIT_WATER_GAL      0x04    /**< Water meter unit: US gallons*/
#define ZW_METER_SUP_UNIT_WATER_PULSE    0x08    /**< Water meter unit: pulse count*/


/** Meter Rate Type definition */
#define ZW_METER_RATE_IMPORT           0x01    /**< Import: Meter Value is a consumed measurement*/
#define ZW_METER_RATE_EXPORT           0x02    /**< Export: Meter Value is a produced measurement*/


#define ZW_ADMIN_STR_MAX    32  /**< maximum number of characters in the administration number*/
#define ZW_ID_STR_MAX       32  /**< maximum number of characters in the identification of customer*/
#define ZW_METER_CAP_MON	1	/**< id & admin number available */
#define ZW_METER_CAP_CFG	2	/**< admin number can be set */

/** meter descriptor */
typedef struct
{
	uint8_t	caps;			                /**< ZW_METER_CAP_xxx or-mask */
	char	id[ZW_ID_STR_MAX + 1];			/**< id, used for identification of customer and metering device */
	char	admin[ZW_ADMIN_STR_MAX + 1];    /**< admin number used to identify customer */
}
zwmeter_t, *zwmeter_p;


/** meter data */
typedef struct
{
    uint8_t     type;		/**< ZW_METER_TYPE_XXX */
    uint8_t     precision;  /**< Decimal places of the value.  The decimal
                                 value 1025 with precision 2 is therefore equal to 10.25.*/
    uint8_t     unit;	    /**< ZW_METER_UNIT_XXX */
    uint8_t     size;	    /**< data size: 1,2,or 4 bytes*/
    uint8_t     data[4];	/**< meter data (a signed number) with the first byte
                                 is the most significant byte*/
    uint16_t    delta_time; /**< elapsed time in seconds between the ‘Meter Value’ and the ‘Previous
                                 Meter Value’ measurements.  If delta_time = 0, it means no ‘Previous
                                 Meter Value’ measurement*/
    uint8_t     prv_data[4];/**< previous meter data (a signed number) with the first byte
                                 is the most significant byte. This field is valid only if delta_time > 0*/
    uint8_t     rate_type;	/**< rate type: ZW_METER_RATE_XXX */

}
zwmeter_dat_t, *zwmeter_dat_p;


/** meter capabilities */
typedef struct
{
    uint8_t     type;		/**< ZW_METER_TYPE_XXX */
    uint8_t     reset_cap;	/**< capability to reset all accumulated values stored in the meter device.
                                 1=reset supported, 0=not supported. See zwif_meter_reset()*/
    uint8_t     unit_sup;   /**< supported unit bit-mask : ZW_METER_SUP_UNIT_XXX */
}
zwmeter_cap_t, *zwmeter_cap_p;


typedef void (*zwrep_meter_fn)(zwifd_p ifd, zwmeter_dat_p data, time_t ts);
/**<
report callback for meter reading
@param[in]	ifd	    interface
@param[in]	data    current value and unit of the meter
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
                    values should be ignored.
*/

typedef void (*zwrep_meterd_fn)(zwifd_p ifd, zwmeter_p meter, int valid);
/**<
report callback for meter descriptor
@param[in]	ifd	    interface
@param[in]	meter	meter descriptor
@param[in]	valid   validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/

typedef void (*zwrep_meter_sup_fn)(zwifd_p ifd, zwmeter_cap_p meter_cap, int valid);
/**<
report callback for meter capabilities
@param[in]	ifd	        interface
@param[in]	meter_cap	meter capabilities
@param[in]	valid       validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/

int zwif_meter_rpt_set(zwifd_p ifd, zwrep_meter_fn rpt_cb);
/**<
setup a meter report callback function
@param[in]	ifd         interface
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_meter_get(zwifd_p ifd, uint8_t unit, int flag);
/**<
get meter report through report callback
@param[in]	ifd	        interface
@param[in]	unit	    the preferred unit (ZW_METER_UNIT_XXX). The report may not return
                        the preferred unit if the device doesn't support it.
@param[in]	flag	    flag, see ZWIF_GET_BMSK_XXX
@return		ZW_ERR_XXX
*/

int zwif_meter_sup_get(zwifd_p ifd,  zwrep_meter_sup_fn cb, int cache);
/**<
get information on the meter capabilities through report callback
@param[in]		ifd	    interface
@param[in]		cb	    report callback function
@param[in]	    cache	flag: to get data from cached only. If set, no fetching from real device when cache unavailable.
@return  0 on success, else ZW_ERR_XXX
*/

int zwif_meter_sup_cache_get(zwifd_p ifd, zwmeter_cap_t *meter_cap);
/**<
get information on the meter capabilities from cache
@param[in]	ifd	        interface
@param[out]	meter_cap	meter capabilities
@return ZW_ERR_XXX
*/

int zwif_meter_reset(zwifd_p ifd);
/**<
reset all accumulated values stored in the meter device
@param[in]	ifd	interface
@return	ZW_ERR_XXX
*/

int zwif_meter_get_desc(zwifd_p ifd, zwrep_meterd_fn cb, int cache);
/**<
get meter admin name
@param[in]	ifd	interface
@param[in]	cb	meter descriptor report callback function
@param[in]	cache	flag: to get data from cached only. If set, no fetching from real device when cache unavailable.
@return	ZW_ERR_XXX
*/

int zwif_meter_set_admin(zwifd_p ifd, char *name);
/**<
set meter admin name
@param[in]	ifd	interface
@param[in]	name	admin number
@return	ZW_ERR_XXX
*/


/**
@}
@defgroup If_PlsMtr Pulse Meter Interface APIs
Intended for all kinds of meters that generate pulses.
@ingroup zwarecapi
@{
*/

typedef void (*zwrep_pulsemeter_fn)(zwifd_p ifd, uint32_t cnt);
/**<
report callback for pulsemeter
@param[in]	ifd	interface
@param[in]	cnt	number of pulses detected in the device
*/

int zwif_pulsemeter_rpt_set(zwifd_p ifd, zwrep_pulsemeter_fn rpt_cb);
/**<
setup a pulse meter report callback function
@param[in]	ifd         interface
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_pulsemeter_get(zwifd_p ifd);
/**<
get pulse meter report through report callback
@param[in]	ifd	        interface
@return		ZW_ERR_XXX
*/

/**
@}
@defgroup If_Av Audio-Video Interface APIs
Audio Video Interface is meant to replace TV/player remotes.
@ingroup zwarecapi
@{
*/

/** @name Button_type
* Button Type definition
@{
*/
#define ZW_AV_BTN_MUTE		0x0001
#define ZW_AV_BTN_VOL_DN	0x0002
#define ZW_AV_BTN_VOL_UP	0x0003
#define ZW_AV_BTN_CH_UP		0x0004
#define ZW_AV_BTN_CH_DN		0x0005
#define ZW_AV_BTN_0 		0x0006
#define ZW_AV_BTN_1 		0x0007
#define ZW_AV_BTN_2 		0x0008
#define ZW_AV_BTN_3 		0x0009
#define ZW_AV_BTN_4 		0x000A
#define ZW_AV_BTN_5 		0x000B
#define ZW_AV_BTN_6 		0x000C
#define ZW_AV_BTN_7 		0x000D
#define ZW_AV_BTN_8 		0x000E
#define ZW_AV_BTN_9 		0x000F
#define ZW_AV_BTN_LAST_CH   0x0010
#define ZW_AV_BTN_DISP_INFO	0x0011
#define ZW_AV_BTN_FAV		0x0012
#define ZW_AV_BTN_PLAY		0x0013
#define ZW_AV_BTN_STOP		0x0014
#define ZW_AV_BTN_PAUSE		0x0015
#define ZW_AV_BTN_FWD		0x0016
#define ZW_AV_BTN_REV		0x0017
#define ZW_AV_BTN_REPLAY	0x0018
#define ZW_AV_BTN_REC		0x0019
#define ZW_AV_BTN_AC3		0x001A
#define ZW_AV_BTN_PVR 		0x001B
#define ZW_AV_BTN_EPG		0x001C
#define ZW_AV_BTN_MENU_SET	0x001D
#define ZW_AV_BTN_MENU_UP	0x001E
#define ZW_AV_BTN_MENU_DN	0x001F
#define ZW_AV_BTN_MENU_L	0x0020
#define ZW_AV_BTN_MENU_R	0x0021
#define ZW_AV_BTN_PGUP		0x0022
#define ZW_AV_BTN_PGDN		0x0023
#define ZW_AV_BTN_SEL		0x0024
#define ZW_AV_BTN_EXIT		0x0025
#define ZW_AV_BTN_INPUT		0x0026
#define ZW_AV_BTN_POWER		0x0027
#define ZW_AV_BTN_ENTER		0x0028
#define ZW_AV_BTN_10		0x0029
#define ZW_AV_BTN_11		0x002A
#define ZW_AV_BTN_12		0x002B
#define ZW_AV_BTN_13		0x002C
#define ZW_AV_BTN_14		0x002D
#define ZW_AV_BTN_15		0x002E
#define ZW_AV_BTN_16		0x002F
#define ZW_AV_BTN_INC10		0x0030
#define ZW_AV_BTN_INC20		0x0031
#define ZW_AV_BTN_INC100	0x0032
#define ZW_AV_BTN_CH1_2		0x0033
#define ZW_AV_BTN_3CH		0x0034
#define ZW_AV_BTN_3D		0x0035
#define ZW_AV_BTN_6CH		0x0036
#define ZW_AV_BTN_A		    0x0037
#define ZW_AV_BTN_ADD		0x0038
#define ZW_AV_BTN_ALARM		0x0039
#define ZW_AV_BTN_AM		0x003A
#define ZW_AV_BTN_ANALOG	0x003B
#define ZW_AV_BTN_ANGLE		0x003C
#define ZW_AV_BTN_ANT_EXT	0x003D
#define ZW_AV_BTN_ANT_EAST	0x003E
#define ZW_AV_BTN_ANT_WEST	0x003F
#define ZW_AV_BTN_ASPECT	0x0040
#define ZW_AV_BTN_AUDIO1	0x0041
#define ZW_AV_BTN_AUDIO2	0x0042
#define ZW_AV_BTN_AUDIO3	0x0043
#define ZW_AV_BTN_AUDIO_DUB	0x0044
#define ZW_AV_BTN_AUDIO_DN	0x0045
#define ZW_AV_BTN_AUDIO_UP	0x0046
#define ZW_AV_BTN_AUTO		0x0047
#define ZW_AV_BTN_AUX1		0x0048
#define ZW_AV_BTN_AUX2		0x0049
#define ZW_AV_BTN_B		    0x004A
#define ZW_AV_BTN_BACK		0x004B
#define ZW_AV_BTN_BACKLIGHT	0x004C
#define ZW_AV_BTN_BALANCE	0x004D
#define ZW_AV_BTN_BALANCE_L	0x004E
#define ZW_AV_BTN_BALANCE_R	0x004F
#define ZW_AV_BTN_BW_FM_AM	0x0050
#define ZW_AV_BTN_BW_W_N	0x0051
#define ZW_AV_BTN_BASS		0x0052
#define ZW_AV_BTN_BASS_DN	0x0053
#define ZW_AV_BTN_BASS_UP	0x0054
#define ZW_AV_BTN_BLANK		0x0055
#define ZW_AV_BTN_BREEZE	0x0056
#define ZW_AV_BTN_BRIGHTEN	0x0057
#define ZW_AV_BTN_BRIGHTNESS	0x0058
#define ZW_AV_BTN_BRIGHT_DN	0x0059
#define ZW_AV_BTN_BRIGHT_UP	0x005A
#define ZW_AV_BTN_BUY		0x005B
#define ZW_AV_BTN_C		    0x005C
#define ZW_AV_BTN_CAMERA	0x005D
#define ZW_AV_BTN_CAT_DOWN	0x005E
#define ZW_AV_BTN_CAT_UP	0x005F
#define ZW_AV_BTN_CENTER		0x0060
#define ZW_AV_BTN_CENTER_VOL_DN	0x0061
#define ZW_AV_BTN_CENTER_MODE	0x0062
#define ZW_AV_BTN_CENTER_VOL_UP	0x0063
#define ZW_AV_BTN_CHANNEL	0x0064
#define ZW_AV_BTN_CANCEL	0x0065
#define ZW_AV_BTN_CLOSE		0x0066
#define ZW_AV_BTN_CC		0x0067
#define ZW_AV_BTN_COLD		0x0068
#define ZW_AV_BTN_COLOR		0x0069
#define ZW_AV_BTN_COLOR_DN	0x006A
#define ZW_AV_BTN_COLOR_UP	0x006B
#define ZW_AV_BTN_COMPO1	0x006C
#define ZW_AV_BTN_COMPO2	0x006D
#define ZW_AV_BTN_COMPO3	0x006E
#define ZW_AV_BTN_CONCERT	0x006F
#define ZW_AV_BTN_CONFIRM	0x0070
#define ZW_AV_BTN_CONTINUE	0x0071
#define ZW_AV_BTN_CONTRAST	0x0072
#define ZW_AV_BTN_CONTRAST_DN	0x0073
#define ZW_AV_BTN_CONTRAST_UP	0x0074
#define ZW_AV_BTN_COUNTER	    0x0075
#define ZW_AV_BTN_COUNTER_RESET	0x0076
#define ZW_AV_BTN_D		    0x0077
#define ZW_AV_BTN_DAY_DN	0x0078
#define ZW_AV_BTN_DAY_UP	0x0079
#define ZW_AV_BTN_DELAY		0x007A
#define ZW_AV_BTN_DELAY_DN	0x007B
#define ZW_AV_BTN_DELAY_UP	0x007C
#define ZW_AV_BTN_ERASE		0x007D
#define ZW_AV_BTN_DELIMIT	0x007E
#define ZW_AV_BTN_DIGEST	0x007F
#define ZW_AV_BTN_DIGITAL	0x0080
#define ZW_AV_BTN_DIM		0x0081
#define ZW_AV_BTN_DIRECT	0x0082
#define ZW_AV_BTN_DISARM	0x0083
#define ZW_AV_BTN_DISC		0x0084
#define ZW_AV_BTN_DISC1		0x0085
#define ZW_AV_BTN_DISC2		0x0086
#define ZW_AV_BTN_DISC3		0x0087
#define ZW_AV_BTN_DISC4		0x0088
#define ZW_AV_BTN_DISC5		0x0089
#define ZW_AV_BTN_DISC6		0x008A
#define ZW_AV_BTN_DISC_DN	0x008B
#define ZW_AV_BTN_DISC_UP	0x008C
#define ZW_AV_BTN_DISCO		0x008D
#define ZW_AV_BTN_EDIT		0x008E
#define ZW_AV_BTN_EFFECT_DN	0x008F
#define ZW_AV_BTN_EFFECT_UP	0x0090
#define ZW_AV_BTN_EJECT		0x0091
#define ZW_AV_BTN_END		0x0092
#define ZW_AV_BTN_EQ		0x0093
#define ZW_AV_BTN_FADER		0x0094
#define ZW_AV_BTN_FAN		0x0095
#define ZW_AV_BTN_FAN_HIGH	0x0096
#define ZW_AV_BTN_FAN_LOW	0x0097
#define ZW_AV_BTN_FAN_MEDIUM	0x0098
#define ZW_AV_BTN_FAN_SPEED	    0x0099
#define ZW_AV_BTN_FASTEXT_BLUE	0x009A
#define ZW_AV_BTN_FASTEXT_GREEN	0x009B
#define ZW_AV_BTN_FASTEXT_PURPLE	0x009C
#define ZW_AV_BTN_FASTEXT_RED		0x009D
#define ZW_AV_BTN_FASTEXT_WHITE	    0x009E
#define ZW_AV_BTN_FASTEXT_YELLOW	0x009F
#define ZW_AV_BTN_FAV_CH_DN	0x00A0
#define ZW_AV_BTN_FAV_CH_UP	0x00A1
#define ZW_AV_BTN_FINALIZE	0x00A2
#define ZW_AV_BTN_FINE TUNE	0x00A3
#define ZW_AV_BTN_FLAT		0x00A4
#define ZW_AV_BTN_FM		0x00A5
#define ZW_AV_BTN_FOCUS_DN	0x00A6
#define ZW_AV_BTN_FOCUS_UP	0x00A7
#define ZW_AV_BTN_FREEZE	0x00A8
#define ZW_AV_BTN_FRONT		0x00A9
#define ZW_AV_BTN_GAME		0x00AA
#define ZW_AV_BTN_GOTO		0x00AB
#define ZW_AV_BTN_HALL		0x00AC
#define ZW_AV_BTN_HEAT		0x00AD
#define ZW_AV_BTN_HELP		0x00AE
#define ZW_AV_BTN_HOME		0x00AF
#define ZW_AV_BTN_INDEX_VISS	0x00B0
#define ZW_AV_BTN_INDEX_FWD	0x00B1
#define ZW_AV_BTN_INDEX_REV	0x00B2
#define ZW_AV_BTN_PLANNER	0x00B3
#define ZW_AV_BTN_INTRO_SCAN   0x00B4
#define ZW_AV_BTN_JAZZ		0x00B5
#define ZW_AV_BTN_KARAOKE	0x00B6
#define ZW_AV_BTN_KEYSTONE	0x00B7
#define ZW_AV_BTN_KEYSTONE_DN	0x00B8
#define ZW_AV_BTN_KEYSTONE_UP	0x00B9
#define ZW_AV_BTN_LANGUAGE	0x00BA
#define ZW_AV_BTN_LCLICK	0x00BB
#define ZW_AV_BTN_VOLUME	0x00BC
#define ZW_AV_BTN_LIGHT		0x00BD
#define ZW_AV_BTN_LIST		0x00BE
#define ZW_AV_BTN_LIVE		0x00BF
#define ZW_AV_BTN_LOCAL		0x00C0
#define ZW_AV_BTN_LOUDNESS	0x00C1
#define ZW_AV_BTN_MAIL		0x00C2
#define ZW_AV_BTN_MARK		0x00C3
#define ZW_AV_BTN_RECALL	0x00C4
#define ZW_AV_BTN_MONITOR	0x00C5
#define ZW_AV_BTN_MOVIE		0x00C6
#define ZW_AV_BTN_MULTI		0x00C7
#define ZW_AV_BTN_MUSIC		0x00C8
#define ZW_AV_BTN_MUSIC_SCAN	0x00C9
#define ZW_AV_BTN_NATURAL	0x00CA
#define ZW_AV_BTN_NIGHT		0x00CB
#define ZW_AV_BTN_DOLBYNR	0x00CC
#define ZW_AV_BTN_NORMALIZE	0x00CD
#define ZW_AV_BTN_INPUT_CATV	0x00CE
#define ZW_AV_BTN_INPUT_CD1 	0x00CF
#define ZW_AV_BTN_INPUT_CD2	0x00D0
#define ZW_AV_BTN_INPUT_CDR	0x00D1
#define ZW_AV_BTN_INPUT_DAT	0x00D2
#define ZW_AV_BTN_INPUT_DVD	0x00D3
#define ZW_AV_BTN_INPUT_DVI	0x00D4
#define ZW_AV_BTN_INPUT_HDTV	0x00D5
#define ZW_AV_BTN_INPUT_LD	0x00D6
#define ZW_AV_BTN_INPUT_MD	0x00D7
#define ZW_AV_BTN_INPUT_PC	0x00D8
#define ZW_AV_BTN_INPUT_PVR	0x00D9
#define ZW_AV_BTN_INPUT_TV	0x00DA
#define ZW_AV_BTN_INPUT_TV_VCR	0x00DB
#define ZW_AV_BTN_INPUT_VCR	0x00DC
#define ZW_AV_BTN_OTPB		0x00DD
#define ZW_AV_BTN_OTR		0x00DE
#define ZW_AV_BTN_OPEN		0x00DF
#define ZW_AV_BTN_OPTICAL	0x00E0
#define ZW_AV_BTN_OPTIONS	0x00E1
#define ZW_AV_BTN_ORCHESTRA	0x00E2
#define ZW_AV_BTN_PAL_NTSC	0x00E3
#define ZW_AV_BTN_PARENTAL	0x00E4
#define ZW_AV_BTN_PBC		0x00E5
#define ZW_AV_BTN_PHONO		0x00E6
#define ZW_AV_BTN_PHOTO		0x00E7
#define ZW_AV_BTN_PIC_MENU	0x00E8
#define ZW_AV_BTN_PIC_MODE	0x00E9
#define ZW_AV_BTN_PIC_MUTE	0x00EA
#define ZW_AV_BTN_PIP_CHDN	0x00EB
#define ZW_AV_BTN_PIP_CHUP	0x00EC
#define ZW_AV_BTN_PIP_FREEZE	0x00ED
#define ZW_AV_BTN_PIP_INPUT	0x00EE
#define ZW_AV_BTN_PIP_MOVE	0x00EF
#define ZW_AV_BTN_PIP_OFF	0x00F0
#define ZW_AV_BTN_PIP_ON	0x00F1
#define ZW_AV_BTN_PIP_SIZE	0x00F2
#define ZW_AV_BTN_PIP_SPLIT	0x00F3
#define ZW_AV_BTN_PIP_SWAP	0x00F4
#define ZW_AV_BTN_PLAY_MODE	0x00F5
#define ZW_AV_BTN_PLAY_REV	0x00F6
#define ZW_AV_BTN_PWR_OFF	0x00F7
#define ZW_AV_BTN_PWR_ON	0x00F8
#define ZW_AV_BTN_PPV		0x00F9
#define ZW_AV_BTN_PRESET	0x00FA
#define ZW_AV_BTN_PROGRAM	0x00FB
#define ZW_AV_BTN_PROGRESSIVE	0x00FC
#define ZW_AV_BTN_PROLOGIC	0x00FD
#define ZW_AV_BTN_PTY		0x00FE
#define ZW_AV_BTN_SKIP		0x00FF
#define ZW_AV_BTN_SHUFFLE	0x0100
#define ZW_AV_BTN_RDS		0x0101
#define ZW_AV_BTN_REAR		0x0102
#define ZW_AV_BTN_REAR_VOLDN	0x0103
#define ZW_AV_BTN_REAR_VOLUP	0x0104
#define ZW_AV_BTN_REC_MUTE	0x0105
#define ZW_AV_BTN_REC_PAUSE	0x0106
#define ZW_AV_BTN_REPEAT	0x0107
#define ZW_AV_BTN_REPEAT_AB	0x0108
#define ZW_AV_BTN_RESUME	0x0109
#define ZW_AV_BTN_RGB		0x010A
#define ZW_AV_BTN_RCLICK	0x010B
#define ZW_AV_BTN_ROCK		0x010C
#define ZW_AV_BTN_ROT_L		0x010D
#define ZW_AV_BTN_ROT_R		0x010E
#define ZW_AV_BTN_SAT		0x010F
#define ZW_AV_BTN_CH_SCAN	0x0110
#define ZW_AV_BTN_SCART		0x0111
#define ZW_AV_BTN_SCENE		0x0112
#define ZW_AV_BTN_SCROLL	0x0113
#define ZW_AV_BTN_SERVICES	0x0114
#define ZW_AV_BTN_SETUP			0x0115
#define ZW_AV_BTN_SHARP			0x0116
#define ZW_AV_BTN_SHARPNESS		0x0117
#define ZW_AV_BTN_SHARP_DN		0x0118
#define ZW_AV_BTN_SHARP_UP		0x0119
#define ZW_AV_BTN_SIDE_AB		0x011A
#define ZW_AV_BTN_NEXT			0x011B
#define ZW_AV_BTN_PREV			0x011C
#define ZW_AV_BTN_SLEEP_OFF		0x011D
#define ZW_AV_BTN_SLOW			0x011E
#define ZW_AV_BTN_SLOW_FWD		0x011F
#define ZW_AV_BTN_SLOW_REV		0x0120
#define ZW_AV_BTN_SOUND_MENU	0x0121
#define ZW_AV_BTN_SOUND_MODE	0x0122
#define ZW_AV_BTN_SPEED_RECORD	0x0123
#define ZW_AV_BTN_SPEED_DN		0x0124
#define ZW_AV_BTN_SPEED_UP		0x0125
#define ZW_AV_BTN_SPORTS		0x0126
#define ZW_AV_BTN_STADIUM		0x0127
#define ZW_AV_BTN_START			0x0128
#define ZW_AV_BTN_START_ERASE		0x0129
#define ZW_AV_BTN_START_RENUMBER	0x012A
#define ZW_AV_BTN_START_WRITE		0x012B
#define ZW_AV_BTN_STEP			0x012C
#define ZW_AV_BTN_STEREO_MONO	0x012D
#define ZW_AV_BTN_STILL_FWD		0x012E
#define ZW_AV_BTN_STILL_REV		0x012F
#define ZW_AV_BTN_SUBTITLE		0x0130
#define ZW_AV_BTN_SUBWOOFER_DN	0x0131
#define ZW_AV_BTN_SUBWOOFER_UP	0x0132
#define ZW_AV_BTN_SUPER_BASS	0x0133
#define ZW_AV_BTN_SURROUND		0x0134
#define ZW_AV_BTN_SURROUND_MODE	0x0135
#define ZW_AV_BTN_SVIDEO		0x0136
#define ZW_AV_BTN_SWEEP			0x0137
#define ZW_AV_BTN_SYNCHRO		0x0138
#define ZW_AV_BTN_TAPE1DECK 1		0x0139
#define ZW_AV_BTN_TAPE1_2DECK1_2	0x013A
#define ZW_AV_BTN_TAPE2DECK2		0x013B
#define ZW_AV_BTN_TEMPERATURE_DOWN	0x013C
#define ZW_AV_BTN_TEMPERATURE_UP	0x013D
#define ZW_AV_BTN_TEST_TONE		0x013E
#define ZW_AV_BTN_TEXT_TELETEXT	0x013F
#define ZW_AV_BTN_TEXT_EXPAND	0x0140
#define ZW_AV_BTN_TEXT_HOLD		0x0141
#define ZW_AV_BTN_TEXT_INDEX	0x0142
#define ZW_AV_BTN_TEXT_MIX		0x0143
#define ZW_AV_BTN_TEXT_OFF		0x0144
#define ZW_AV_BTN_TEXT_REVEAL	0x0145
#define ZW_AV_BTN_TEXT_SUBPAGE	0x0146
#define ZW_AV_BTN_TEXT_TIMED_PAGE	0x0147
#define ZW_AV_BTN_TEXT CANCEL		0x0148
#define ZW_AV_BTN_THEATER 	0x0149
#define ZW_AV_BTN_THEME		0x014A
#define ZW_AV_BTN_THUMBS_DN	0x014B
#define ZW_AV_BTN_THUMBS_UP	0x014C
#define ZW_AV_BTN_TILT_DN	0x014D
#define ZW_AV_BTN_TILT_UP	0x014E
#define ZW_AV_BTN_TIME		0x014F
#define ZW_AV_BTN_TIMER		0x0150
#define ZW_AV_BTN_TIMER_DN	0x0151
#define ZW_AV_BTN_TIMER_UP	0x0152
#define ZW_AV_BTN_TINT		0x0153
#define ZW_AV_BTN_TINT_DN	0x0154
#define ZW_AV_BTN_TINT_UP	0x0155
#define ZW_AV_BTN_TITLE		0x0156
#define ZW_AV_BTN_TRACK		0x0157
#define ZW_AV_BTN_TRACKING	0x0158
#define ZW_AV_BTN_TRACKING_DN	0x0159
#define ZW_AV_BTN_TRACKING_UP	0x015A
#define ZW_AV_BTN_TREBLE	0x015B
#define ZW_AV_BTN_TREBLE_DN	0x015C
#define ZW_AV_BTN_TREBLE_UP	0x015D
#define ZW_AV_BTN_TUNE_DN	0x015E
#define ZW_AV_BTN_TUNE_UP	0x015F
#define ZW_AV_BTN_TUNER		0x0160
#define ZW_AV_BTN_SHOWVIEW	0x0161
#define ZW_AV_BTN_AV1		0x0162
#define ZW_AV_BTN_AV2		0x0163
#define ZW_AV_BTN_AV3		0x0164
#define ZW_AV_BTN_AV4		0x0165
#define ZW_AV_BTN_AV5		0x0166
#define ZW_AV_BTN_VIEW		0x0167
#define ZW_AV_BTN_VOICE		0x0168
#define ZW_AV_BTN_ZOOM		0x0169
#define ZW_AV_BTN_ZOOM_IN	0x016A
#define ZW_AV_BTN_ZOOM_OUT	0x016B
#define ZW_AV_BTN_365		0x016C
#define ZW_AV_BTN_DVD_MENU	0x016E
#define ZW_AV_BTN_MYTV		0x016F
#define ZW_AV_BTN_RECORDEDTV	0x0170
#define ZW_AV_BTN_MYVIDEOS2	    0x0171
#define ZW_AV_BTN_DVDANGLE2	    0x0172
#define ZW_AV_BTN_DVDAUDIO2	    0x0173
#define ZW_AV_BTN_DVDSUB	    0x0174
#define ZW_AV_BTN_RADIO			0x0175
#define ZW_AV_BTN_HASH			0x0176
#define ZW_AV_BTN_STAR			0x0177
#define ZW_AV_BTN_OEM1			0x0178
#define ZW_AV_BTN_OEM2			0x0179
#define ZW_AV_BTN_RQT_INFO		0x017A
#define ZW_AV_BTN_CAPS_NUM		0x017B
#define ZW_AV_BTN_TV_MODE		0x017C
#define ZW_AV_BTN_SOURCE		0x017D
#define ZW_AV_BTN_FILE			0x017E
#define ZW_AV_BTN_SEEK			0x017F
#define ZW_AV_BTN_MOUSEON		0x0180
#define ZW_AV_BTN_MOUSEOFF		0x0181
#define ZW_AV_BTN_VOD			0x0182
#define ZW_AV_BTN_THUMBS_UP_GUI	0x0183
#define ZW_AV_BTN_THUMBS_DN_GUI	0x0184
#define ZW_AV_BTN_APPS			0x0185
#define ZW_AV_BTN_MOUSE			0x0186
#define ZW_AV_BTN_TV			0x0187
#define ZW_AV_BTN_DVD			0x0188
#define ZW_AV_BTN_STB			0x0189
#define ZW_AV_BTN_AUX			0x018A
#define ZW_AV_BTN_BLURAY		0x018B
#define ZW_AV_BTN_STANDBY1		0x0194
#define ZW_AV_BTN_STANDBY2		0x0195
#define ZW_AV_BTN_STANDBY3		0x0196
#define ZW_AV_BTN_HDMI1			0x0197
#define ZW_AV_BTN_HDMI2			0x0198
#define ZW_AV_BTN_HDMI3			0x0199
#define ZW_AV_BTN_HDMI4			0x019A
#define ZW_AV_BTN_HDMI5			0x019B
#define ZW_AV_BTN_HDMI6			0x019C
#define ZW_AV_BTN_HDMI7			0x019D
#define ZW_AV_BTN_HDMI8			0x019E
#define ZW_AV_BTN_HDMI9			0x019F
#define ZW_AV_BTN_USB1			0x01A0
#define ZW_AV_BTN_USB2			0x01A1
#define ZW_AV_BTN_USB3			0x01A2
#define ZW_AV_BTN_USB4			0x01A3
#define ZW_AV_BTN_USB5			0x01A4
#define ZW_AV_BTN_ZOOM4_3_NORMAL	0x01A5
#define ZW_AV_BTN_ZOOM4_3_ZOOM	    0x01A6
#define ZW_AV_BTN_ZOOM16_9_NORMAL	0x01A7
#define ZW_AV_BTN_ZOOM16_9_ZOOM	    0x01A8
#define ZW_AV_BTN_ZOOM16_9_WIDE1	0x01A9
#define ZW_AV_BTN_ZOOM16_9_WIDE2	0x01AA
#define ZW_AV_BTN_ZOOM16_9_WIDE3	0x01AB
#define ZW_AV_BTN_ZOOM16_9_CINEMA	0x01AC
#define ZW_AV_BTN_ZOOM_DEFAULT	    0x01AD
#define ZW_AV_BTN_AUTO_ZOOM		    0x01B0
#define ZW_AV_BTN_ZOOM_SET_DEFAULT	0x01B1
#define ZW_AV_BTN_MUTE_ON		    0x01B2
#define ZW_AV_BTN_MUTE_OFF		    0x01B3
#define ZW_AV_BTN_AUMODE_AUDYSSEY_OFF	0x01B4
#define ZW_AV_BTN_AUMODE_AUDYSSEY_LO	0x01B5
#define ZW_AV_BTN_AUMODE_AUDYSSEY_MED	0x01B6
#define ZW_AV_BTN_AUMODE_AUDYSSEY_HI	0x01B7
#define ZW_AV_BTN_AUMODE_SRS_ON	    0x01BA
#define ZW_AV_BTN_AUMODE_SRS_OFF	0x01BB
#define ZW_AV_BTN_PICMODE_HOME	    0x01BF
#define ZW_AV_BTN_PICMODE_RETAIL	0x01C0
#define ZW_AV_BTN_PICMODE_VIVID	    0x01C1
#define ZW_AV_BTN_PICMODE_STANDARD	0x01C2
#define ZW_AV_BTN_PICMODE_THEATER	0x01C3
#define ZW_AV_BTN_PICMODE_SPORTS	0x01C4
#define ZW_AV_BTN_PICMODE_ENERGY	0x01C5
#define ZW_AV_BTN_PICMODE_CUSTOM	0x01C6
#define ZW_AV_BTN_COOL			0x01C7
#define ZW_AV_BTN_MEDIUM		0x01C8
#define ZW_AV_BTN_WARM_D65		0x01C9
#define ZW_AV_BTN_CCON			0x01CA
#define ZW_AV_BTN_CCOFF			0x01CB
#define ZW_AV_BTN_VMUTE			0x01CC
#define ZW_AV_BTN_VUNMUTE		0x01CD
#define ZW_AV_BTN_NEXTEV		0x01CE
#define ZW_AV_BTN_PREVEV		0x01CF
#define ZW_AV_BTN_CEC			0x01D0
#define ZW_AV_BTN_MTS			0x01D1
///@}

typedef void (*zwrep_av_fn)(zwifd_p ifd, uint16_t length, uint8_t *mask, int valid);
/**<
report callback for av capabilities
@param[in]	ifd	    interface
@param[in]	length	mask byte length
@param[in]	mask	mask buffer pointer
@param[in]	valid   validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/

int zwif_av_caps(zwifd_p ifd, zwrep_av_fn cb, int cache);
/**<
get supported AV commands
@param[in]	ifd	    interface
@param[in]	cb	    av capabilities report callback function.
@param[in]	cache	flag: to get data from cached only. If set, no fetching from real device when cache unavailable.
@return	ZW_ERR_XXX
*/

int zwif_av_set(zwifd_p ifd, uint16_t ctl, uint8_t down);
/**<
press AV button
@param[in]	ifd	    interface
@param[in]	ctl		button number ZW_AV_BTN_XX
@param[in]	down	0=button up, else button down
@return	ZW_ERR_XXX
*/

/**
@}
@defgroup If_Basic Basic Interface APIs
Basic command that can be used to control the basic functionality of a device
@ingroup zwarecapi
@{
*/

int zwif_basic_set(zwifd_p ifd, uint8_t v);
/**<
set basic value
@param[in]	ifd		interface
@param[in]	v		value (the range of value is device specific)
@return	ZW_ERR_XXX
*/

/** Basic data */
typedef struct
{
    uint8_t curr_val;  	/**< current value: 0 = off; 0x01~0x63 = percentage (%);0xFE = Unknown; 0xFF = On. */
    uint8_t tgt_val;	/**< target value with same interpretation as curr_val */
    uint8_t dur;	    /**< Duration: 0 = already at the target; 0x01~0x7F = seconds; 0x80~0xFD = 1~126 minutes;
                                       0xFE = Unknown duration; 0xFF = reserved*/
}
zwbasic_t, *zwbasic_p;

typedef void (*zwrep_basic_fn)(zwifd_p ifd, zwbasic_p val, time_t ts);
/**<
basic report callback
@param[in]	ifd	        interface
@param[in]	val	        basic data
@param[in]	ts          time stamp.  If this is zero, the callback has no data and hence other parameter
                        values should be ignored.
*/

int zwif_basic_rpt_set(zwifd_p ifd, zwrep_basic_fn rpt_cb);
/**<
setup a basic report callback function
@param[in]	ifd         Interface descriptor
@param[in]	rpt_cb	    Report callback function
return      ZW_ERR_XXX
*/

int zwif_basic_get(zwifd_p ifd, int flag);
/**<
get basic report through report callback
@param[in]	ifd	        interface
@param[in]	flag	    flag, see ZWIF_GET_BMSK_XXX
@return		ZW_ERR_XXX
*/


/**
@}
@defgroup If_Doorlock Door Lock Interface APIs
Used to secure/unsecure a lock type as well as setting the configuration of an advanced Z-Wave door lock device
@ingroup zwarecapi
@{
*/

/** @name Door_lock_op_mode
* Door lock operation mode definition
@{
*/
#define ZW_DOOR_UNSEC           0x00    /**< Door unsecured*/
#define ZW_DOOR_UNSEC_TMOUT     0x01    /**< Door unsecured with timeout. Fallback to secured mode after timeout has expired*/
#define ZW_DOOR_UNSEC_IN        0x10    /**< Door unsecured for inside door handles*/
#define ZW_DOOR_UNSEC_IN_TMOUT  0x11    /**< Door unsecured for inside door handles with timeout*/
#define ZW_DOOR_UNSEC_OUT       0x20    /**< Door unsecured for outside door handles*/
#define ZW_DOOR_UNSEC_OUT_TMOUT 0x21    /**< Door unsecured for outside door handles with timeout*/
#define ZW_DOOR_UNKNOWN         0xFE    /**< Unknown state. This could happend while in transition to a new mode.
                                             Example: Bolt is not fully retracted/engaged*/
#define ZW_DOOR_SEC             0xFF    /**< Door secured*/
/**
@}
*/

int zwif_dlck_op_set(zwifd_p ifd, uint8_t mode, zw_postset_fn cb, void *usr_param);
/**<
set door lock operation
@param[in]	ifd	        interface
@param[in]	mode	    operation mode (ZW_DOOR_XXX).
@param[in]	cb		    Optional post-set polling callback function.  NULL if no callback required.
@param[in]	usr_param   Optional user-defined parameter passed in callback.
@return	ZW_ERR_XXX
*/

/** Door condition bit-mask */
#define ZW_COND_DOOR_MASK       0x01    /**< Bit-mask for door status.
                                             After masking bit=0 means door open; else closed*/
#define ZW_COND_BOLT_MASK       0x02    /**< Bit-mask for bolt status.
                                             After masking bit=0 means bolt locked; else unlocked*/
#define ZW_COND_LATCH_MASK      0x04    /**< Bit-mask for latch status.
                                             After masking bit=0 means latch open; else closed*/
/** Door lock operation mode*/
typedef struct
{
    uint8_t     mode;           /**< Current door lock mode (ZW_DOOR_XXX) */
    uint8_t     out_mode;       /**< Outside door handles mode. It's a 4-bit mask; bit=0 for disabled, bit=1 for enabled.
                                     When disabled, the actual handle cannot open the door locally.
                                     When enabled, the actual handle can open the door locally*/
    uint8_t     in_mode;        /**< Inside door handles mode. It's a 4-bit mask; bit=0 for disabled, bit=1 for enabled*/
    uint8_t     cond;           /**< Door condition. See ZW_COND_XXX_MASK */
    uint8_t     tmout_min;      /**< The remaining time in minutes before the door lock will automatically be locked again.
                                     Value of 0xFE means timeout is not supported*/
    uint8_t     tmout_sec;      /**< The remaining time in seconds before the door lock will automatically be locked again.
                                     Value of 0xFE means timeout is not supported*/
    uint8_t     tgt_mode;	    /**< Target door lock mode (ZW_DOOR_XXX) */
    uint8_t     dur;	        /**< Duration: 0 = already at the target; 0x01~0x7F = seconds; 0x80~0xFD = 1~126 minutes;
                                     0xFE = Unknown duration; 0xFF = reserved*/
}
zwdlck_op_t, *zwdlck_op_p;


typedef void (*zwrep_dlck_op_fn)(zwifd_p ifd, zwdlck_op_p  op_sts, time_t ts);
/**<
report callback for door lock operation
@param[in]	ifd	        interface
@param[in]	op_sts      operation status
@param[in]	ts          time stamp.  If this is zero, the callback has no data and hence other parameter
                        values should be ignored.
*/

int zwif_dlck_op_rpt_set(zwifd_p ifd, zwrep_dlck_op_fn rpt_cb);
/**<
setup a door lock operation report callback function
@param[in]	ifd         interface descriptor
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_dlck_op_get(zwifd_p ifd, int flag);
/**<
get the state of the door lock device through report callback
@param[in]	ifd	        interface
@param[in]	flag	    flag, see ZWIF_GET_BMSK_XXX
@return		ZW_ERR_XXX
*/

/** @name Door_lock_op_type
* Door lock operation type definition
@{
*/
#define ZW_DOOR_OP_CONST    0x01    /**< Constant operation*/
#define ZW_DOOR_OP_TIMED    0x02    /**< Timed operation*/
/**
@}
*/

/** Door lock configuration */
typedef struct
{
    uint8_t     type;           /**< Door lock operation type (ZW_DOOR_OP_XXX) */
    uint8_t     out_sta;        /**< Outside door handles state. It's a 4-bit mask; bit=0 for disable, bit=1 for enable
                                     When disabled, the actual handle cannot open the door locally.
                                     When enabled, the actual handle can open the door locally*/
    uint8_t     in_sta;         /**< Inside door handles state. It's a 4-bit mask; bit=0 for disable, bit=1 for enable*/
    uint8_t     tmout_min;      /**< Lock timeout in minutes. Valid value: 0 to 253. Value of 0xFE means timeout is not supported*/
    uint8_t     tmout_sec;      /**< Lock timeout in seconds. Valid value: 0 to 59. Value of 0xFE means timeout is not supported*/
}
zwdlck_cfg_t, *zwdlck_cfg_p;

typedef void (*zwrep_dlck_cfg_fn)(zwifd_p ifd, zwdlck_cfg_p  config, time_t ts);
/**<
report callback for door lock configuration
@param[in]	ifd	    interface
@param[in]	config	configuration
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
                    values should be ignored.
*/

int zwif_dlck_cfg_set(zwifd_p ifd, zwdlck_cfg_p config);
/**<
set the configuration of the door lock device
@param[in]	ifd	    interface
@param[in]	config	configuration
@return	ZW_ERR_XXX
*/

int zwif_dlck_cfg_get(zwifd_p ifd, zwrep_dlck_cfg_fn cb, int flag);
/**<
get configuration parameter through report callback
@param[in]	ifd	    interface
@param[in]	cb	    report callback function
@param[in]	flag	flag, see ZWIF_GET_BMSK_XXX
@return  0 on success, else ZW_ERR_XXX
*/


/**
@}
@defgroup If_Usrcode User Code Interface APIs
Used to manage user codes required to unlock a lock
@ingroup zwarecapi
@{
*/

/** @name User_id_status
* User ID status definition
@{
*/
#define ZW_USRCOD_AVAIL         0x00    /**< Available (not set)*/
#define ZW_USRCOD_UNAVAIL       0x01    /**< Occupied*/
#define ZW_USRCOD_RSVD          0x02    /**< Reserved by administrator*/
#define ZW_USRCOD_NO_STS        0xFE    /**< Status unavailable*/
/**
@}
*/

#define MAX_USRCOD_LENGTH       10      /**< Max. user code length*/
/** User code */
typedef struct
{
    uint8_t     id;                     /**< User identifier */
    uint8_t     id_sts;                 /**< User id status*/
    uint8_t     code_len;               /**< User code length*/
    uint8_t     code[MAX_USRCOD_LENGTH];/**< User code; minimum length = 4, maximum length = 10*/
}
zwusrcod_t, *zwusrcod_p;

typedef void (*zwrep_usr_cod_fn)(zwifd_p ifd, zwusrcod_p  usr_cod);
/**<
report callback for user code
@param[in]	ifd	        interface
@param[in]	usr_cod     user code and its status
*/

typedef void (*zwrep_usr_sup_fn)(zwifd_p ifd, uint8_t  usr_num, int valid);
/**<
report callback for number of supported user codes
@param[in]	ifd	        interface
@param[in]	usr_num     number of supported user codes
@param[in]	valid       validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/

int zwif_usrcod_set(zwifd_p ifd, zwusrcod_p usr_cod);
/**<
set the user code
@param[in]	ifd	    interface
@param[in]	usr_cod user code and its status
@return	ZW_ERR_XXX
*/

int zwif_usrcod_get(zwifd_p ifd, uint8_t usr_id, zwrep_usr_cod_fn cb);
/**<
get the specified user code and its status
@param[in]	ifd	    interface
@param[in]	usr_id  user identifier
@param[in]	cb	    report callback function
@return ZW_ERR_XXX
*/

int zwif_usrcod_sup_get(zwifd_p ifd, zwrep_usr_sup_fn cb, int cache);
/**<
get the number of supported user codes
@param[in]	ifd	    interface
@param[in]	cb	    report callback function
@param[in]	cache	flag: to get data from cached only. If set, no fetching from real device when cache unavailable.
@return ZW_ERR_XXX
*/

int zwif_usrcod_sup_cache_get(zwifd_p ifd, uint8_t *usr_num);
/**<
get the number of supported user codes from cache
@param[in]	ifd	        interface
@param[out]	usr_num     number of supported user codes
@return ZW_ERR_XXX
*/

/**
@}
@defgroup If_Alarm Alarm Interface APIs
Used to report alarm or service conditions
@ingroup zwarecapi
@{
*/

/** @name Alarm_type
* Z-wave alarm type definition
@{
*/
#define ZW_ALRM_SMOKE       0x01    /**< Smoke alarm*/
#define ZW_ALRM_CO          0x02    /**< Carbon monoxide alarm*/
#define ZW_ALRM_CO2         0x03    /**< Carbon dioxide alarm*/
#define ZW_ALRM_HEAT        0x04    /**< Heat alarm*/
#define ZW_ALRM_WATER       0x05    /**< Water alarm*/
#define ZW_ALRM_LOCK        0x06    /**< Lock access control alarm*/
#define ZW_ALRM_BURGLAR     0x07    /**< Burglar alarm or home security*/
#define ZW_ALRM_POWER       0x08    /**< Power management alarm*/
#define ZW_ALRM_SYSTEM      0x09    /**< System alarm*/
#define ZW_ALRM_EMERGENCY   0x0A    /**< Emergency alarm*/
#define ZW_ALRM_CLOCK       0x0B    /**< Alarm clock*/
#define ZW_ALRM_APPLIANCE   0x0C    /**< Home appliance alarm*/
#define ZW_ALRM_HEALTH      0x0D    /**< Home health alarm*/
#define ZW_ALRM_SIREN       0x0E    /**< Siren alarm*/
#define ZW_ALRM_WATER_VLV   0x0F    /**< Water Valve alarm*/
#define ZW_ALRM_WEATHER     0x10    /**< Weather alarm*/
#define ZW_ALRM_IRRIGATION  0x11    /**< Irrigation alarm*/
#define ZW_ALRM_GAS         0x12    /**< Gas alarm*/
#define ZW_ALRM_PEST_CTL    0x13    /**< Pest control*/
#define ZW_ALRM_LIGHT_SNSR  0x14    /**< Light sensor*/

#define ZW_ALRM_REQ_PEND    0xFF    /**< Request pending notification*/
#define ZW_ALRM_FIRST       0xFF    /**< Used by the zwif_alrm_get() to retrieve the
                                         first alarm detection from the supported list*/
/**
@}
*/

/** @name Alarm_event
* Z-wave alarm event definition
@{
*/
/** Z-wave alarm event for all types of alarms*/
#define ZW_ALRM_EVT_UNKNOWN             0xFE    /**< Unknown event*/
#define ZW_ALRM_EVT_INACTIVE_CLEAR      0x00    /**< Event inactive (push mode) / Previous events cleared (pull mode)*/

/** Z-wave alarm event for smoke alarm*/
#define ZW_ALRM_EVT_SMOKE_L             0x01    /**< Smoke detected with location info*/
#define ZW_ALRM_EVT_SMOKE               0x02    /**< Smoke detected with unknown location info*/
#define ZW_ALRM_EVT_SMOKE_TEST          0x03    /**< Smoke alarm test*/
#define ZW_ALRM_EVT_SMOKE_REPLA         0x04    /**< Replacement required*/
#define ZW_ALRM_EVT_SMOKE_REPLA_EOL     0x05    /**< Replacement required, End-of-life*/
#define ZW_ALRM_EVT_SMOKE_MAINTNC_PLAN  0x07    /**< Maintenance required, planned periodic inspection*/
#define ZW_ALRM_EVT_SMOKE_MAINTNC_DUST  0x08    /**< Maintenance required, dust in device*/

/** Z-wave alarm event for carbon monoxide alarm*/
#define ZW_ALRM_EVT_CO_L                0x01    /**< Carbon monoxide detected with location info*/
#define ZW_ALRM_EVT_CO                  0x02    /**< Carbon monoxide detected with unknown location info*/
#define ZW_ALRM_EVT_CO_TEST             0x03    /**< Carbon monoxide test. Event parameter value: 1=OK, 2=Failed*/
#define ZW_ALRM_EVT_CO_REPLA            0x04    /**< Replacement required*/
#define ZW_ALRM_EVT_CO_REPLA_EOL        0x05    /**< Replacement required, End-of-life*/
#define ZW_ALRM_EVT_CO_SILENCED         0x06    /**< Alarm silenced*/
#define ZW_ALRM_EVT_CO_MAINTNC_PLAN     0x07    /**< Maintenance required, planned periodic inspection*/

/** Z-wave alarm event for carbon dioxide alarm*/
#define ZW_ALRM_EVT_CO2_L               0x01    /**< Carbon dioxide detected with location info*/
#define ZW_ALRM_EVT_CO2                 0x02    /**< Carbon dioxide detected with unknown location info*/
#define ZW_ALRM_EVT_CO2_TEST            0x03    /**< Carbon dioxide test. Event parameter value: 1=OK, 2=Failed*/
#define ZW_ALRM_EVT_CO2_REPLA           0x04    /**< Replacement required*/
#define ZW_ALRM_EVT_CO2_REPLA_EOL       0x05    /**< Replacement required, End-of-life*/
#define ZW_ALRM_EVT_CO2_SILENCED        0x06    /**< Alarm silenced*/
#define ZW_ALRM_EVT_CO2_MAINTNC_PLAN    0x07    /**< Maintenance required, planned periodic inspection*/

/** Z-wave alarm event for heat alarm*/
#define ZW_ALRM_EVT_OVERHEAT_L          0x01    /**< Overheat detected with location info*/
#define ZW_ALRM_EVT_OVERHEAT            0x02    /**< Overheat detected with unknown location info*/
#define ZW_ALRM_EVT_TEMP_RISE_L         0x03    /**< Rapid temperature rise detected with location info*/
#define ZW_ALRM_EVT_TEMP_RISE           0x04    /**< Rapid temperature rise detected with unknown location info*/
#define ZW_ALRM_EVT_UNDRHEAT_L          0x05    /**< Underheat detected with location info*/
#define ZW_ALRM_EVT_UNDRHEAT            0x06    /**< Underheat detected with unknown location info*/
#define ZW_ALRM_EVT_HT_TEST             0x07    /**< Heat alarm test*/
#define ZW_ALRM_EVT_HT_REPLA_EOL        0x08    /**< Replacement required, End-of-life*/
#define ZW_ALRM_EVT_HT_SILENCED         0x09    /**< Alarm silenced*/
#define ZW_ALRM_EVT_HT_MAINTNC_DUST     0x0A    /**< Maintenance required, dust in device*/
#define ZW_ALRM_EVT_HT_MAINTNC_PLAN     0x0B    /**< Maintenance required, planned periodic inspection*/
#define ZW_ALRM_EVT_RAPID_TEMP_FALL_L   0x0C    /**< Rapid temparature fall with location info*/
#define ZW_ALRM_EVT_RAPID_TEMP_FALL     0x0D    /**< Rapid temparature fall*/

/** Z-wave alarm event for water alarm*/
#define ZW_ALRM_EVT_LEAK_L              0x01    /**< Water leak detected with location info*/
#define ZW_ALRM_EVT_LEAK                0x02    /**< Water leak detected with unknown location info*/
#define ZW_ALRM_EVT_LVL_L               0x03    /**< Water level dropped detected with location info*/
#define ZW_ALRM_EVT_LVL                 0x04    /**< Water level dropped detected with unknown location info*/
#define ZW_ALRM_EVT_REPLACE_WATER_FILTER    0x05    /**< Replace Water Filter*/
#define ZW_ALRM_EVT_WATER_FLOW          0x06    /**< Water flow. Event parameter value: 1=no data, 2=below low treshold,
                                                     3=above high treshold, 4=max*/
#define ZW_ALRM_EVT_WATER_PRESSURE      0x07    /**< Water pressure. Event parameter meaning same as those for water flow. */
#define ZW_ALRM_EVT_WATER_TEMP          0x08    /**< Water temparature. Event parameter value: 1=no data, 2=below low treshold,
                                                     3=above high treshold*/
#define ZW_ALRM_EVT_WATER_LEVEL         0x09    /**< Water level. Event parameter meaning same as those for water temparature. */
#define ZW_ALRM_EVT_SUMP_PUMP_ACTV      0x0A    /**< Sump pump active*/
#define ZW_ALRM_EVT_SUMP_PUMP_FAIL      0x0B    /**< Sump pump failure*/

/** Z-wave alarm event for lock access control alarm*/
#define ZW_ALRM_EVT_MANUAL_LCK          0x01    /**< Manual lock operation*/
#define ZW_ALRM_EVT_MANUAL_ULCK         0x02    /**< Manual unlock operation*/
#define ZW_ALRM_EVT_RF_LCK              0x03    /**< RF lock operation*/
#define ZW_ALRM_EVT_RF_ULCK             0x04    /**< RF unlock operation*/
#define ZW_ALRM_EVT_KEYPAD_LCK          0x05    /**< Keypad lock operation with user identifier info*/
#define ZW_ALRM_EVT_KEYPAD_ULCK         0x06    /**< Keypad unlock operation with user identifier info*/
#define ZW_ALRM_EVT_MANUAL_NOT_FUL_LCK  0x07    /**< Manual not fully locked operation*/
#define ZW_ALRM_EVT_RF_NOT_FUL_LCK      0x08    /**< RF not fully locked operation*/
#define ZW_ALRM_EVT_AUTO_LCK            0x09    /**< Auto lock locked operation*/
#define ZW_ALRM_EVT_AUTO_NOT_FUL_OPER   0x0A    /**< Auto lock not fully operation*/
#define ZW_ALRM_EVT_LCK_JAMMED          0x0B    /**< Lock jammed*/
#define ZW_ALRM_EVT_ALL_CODE_DEL        0x0C    /**< All user codes deleted*/
#define ZW_ALRM_EVT_1_CODE_DEL          0x0D    /**< Single user code deleted*/
#define ZW_ALRM_EVT_CODE_ADDED          0x0E    /**< New user code added*/
#define ZW_ALRM_EVT_CODE_DUP            0x0F    /**< New user code not added due to duplicate code*/
#define ZW_ALRM_EVT_KEYPAD_DISABLED     0x10    /**< Keypad temporary disabled*/
#define ZW_ALRM_EVT_KEYPAD_BUSY         0x11    /**< Keypad busy*/
#define ZW_ALRM_EVT_NEW_PROG_CODE       0x12    /**< New program code entered - unique code for lock configuration*/
#define ZW_ALRM_EVT_USR_CODE_LIMIT      0x13    /**< Manually enter user access code exceeds code limit*/
#define ZW_ALRM_EVT_RF_ULCK_INVLD_CODE  0x14    /**< Unlock by RF with invalid user code*/
#define ZW_ALRM_EVT_RF_LCK_INVLD_CODE   0x15    /**< Locked by RF with invalid user code*/
#define ZW_ALRM_EVT_WINDOW_DOOR_OPEN    0x16    /**< Window/door is open*/
#define ZW_ALRM_EVT_WINDOW_DOOR_CLOSED  0x17    /**< Window/door is closed*/
#define ZW_ALRM_EVT_WIN_DR_HDL_OPEN     0x18    /**< Window/door handle is open*/
#define ZW_ALRM_EVT_WIN_DR_HDL_CLOSED   0x19    /**< Window/door handle is closed*/
/**< 0x1A - 0x3F Reserved*/
#define ZW_ALRM_EVT_BARRIER_INIT					0x40    /**< Barrier performing Initialization process. Event parameter value:
                                                                 0=Completed, 0xFF=In progress*/
#define ZW_ALRM_EVT_BARRIER_OP_FORCE_EXCEED			0x41    /**< Barrier operation (Open / Close) force has been exceeded.*/
#define ZW_ALRM_EVT_BARRIER_MOTOR_TIME_EXCEED		0x42    /**< Barrier motor has exceeded manufacturer’s operational time limit.
                                                                 Event parameter value: 0 to 0x7F = 0 to 127 seconds; 0x80 to 0xFE = 1 to 127 minutes*/
#define ZW_ALRM_EVT_BARRIER_MECHANIC_EXCEED			0x43    /**< Barrier operation has exceeded physical mechanical limits.
                                                                 (For example: barrier has opened past the open limit)*/
#define ZW_ALRM_EVT_BARRIER_OP_FAILED				0x44    /**< Barrier unable to perform requested operation due to UL requirements.*/
#define ZW_ALRM_EVT_BARRIER_OP_DISABLED				0x45    /**< Barrier Unattended operation has been disabled per UL requirements*/
#define ZW_ALRM_EVT_BARRIER_MALFUNC					0x46    /**< Barrier failed to perform Requested operation, device malfunction*/
#define ZW_ALRM_EVT_BARRIER_VACAT_MODE				0x47    /**< Barrier Vacation Mode. Event parameter value: 0=disabled, 0xFF=enabled*/
#define ZW_ALRM_EVT_BARRIER_BEAM_OBST				0x48    /**< Barrier Safety Beam Obstacle. Event parameter value: 0=no obstruction, 0xFF=obstruction*/
#define ZW_ALRM_EVT_BARRIER_SNR_ERR					0x49    /**< Barrier Sensor Not Detected / Supervisory Error. Event parameter value:
                                                                 0=sensor not defined, 1 to 0xFF=sensor ID*/
#define ZW_ALRM_EVT_BARRIER_LOW_BATT				0x4A    /**< Barrier Sensor Low Battery Warning. Event parameter value: 0=sensor not defined,
                                                                 1 to 0xFF=sensor ID*/
#define ZW_ALRM_EVT_BARRIER_SHORT					0x4B    /**< Barrier detected short in Wall Station wires*/
#define ZW_ALRM_EVT_BARRIER_NON_ZWAVE				0x4C    /**< Barrier associated with non-Z-wave remote control.*/


/** Z-wave alarm event for burglar alarm*/
#define ZW_ALRM_EVT_INTRUSION_L         0x01    /**< Intrusion detected with location info*/
#define ZW_ALRM_EVT_INTRUSION           0x02    /**< Intrusion detected with unknown location info*/
#define ZW_ALRM_EVT_TMPR_COVER          0x03    /**< Tampering, product covering removed*/
#define ZW_ALRM_EVT_TMPR_CODE           0x04    /**< Tampering, Invalid Code*/
#define ZW_ALRM_EVT_GLASS_L             0x05    /**< Glass breakage detected with location info*/
#define ZW_ALRM_EVT_GLASS               0x06    /**< Glass breakage detected with unknown location info*/
#define ZW_ALRM_EVT_MOTION_DET_L        0x07    /**< Motion detected with location info*/
#define ZW_ALRM_EVT_MOTION_DET          0x08    /**< Motion detected with unknown location info*/

/** Z-wave alarm event for power management alarm*/
#define ZW_ALRM_EVT_POWER               0x01    /**< Power has been applied*/
#define ZW_ALRM_EVT_AC_OFF              0x02    /**< AC mains disconnected*/
#define ZW_ALRM_EVT_AC_ON               0x03    /**< AC mains re-connected*/
#define ZW_ALRM_EVT_SURGE               0x04    /**< Surge Detection*/
#define ZW_ALRM_EVT_VOLT_DROP           0x05    /**< Voltage Drop/Drift detected*/
#define ZW_ALRM_EVT_OVER_CURRENT        0x06    /**< Over-current detected*/
#define ZW_ALRM_EVT_OVER_VOLT           0x07    /**< Over-voltage detected*/
#define ZW_ALRM_EVT_OVER_LOAD           0x08    /**< Over-load detected*/
#define ZW_ALRM_EVT_LOAD_ERR            0x09    /**< Load error*/
#define ZW_ALRM_EVT_REPLACE_BATT_SOON   0x0A    /**< Replace battery soon*/
#define ZW_ALRM_EVT_REPLACE_BATT_NOW    0x0B    /**< Replace battery now*/
#define ZW_ALRM_EVT_BATT_CHARGING       0x0C    /**< Battery is charging*/
#define ZW_ALRM_EVT_BATT_CHARGED        0x0D    /**< Battery is fully charged*/
#define ZW_ALRM_EVT_CHARGE_BATT_SOON    0x0E    /**< Charge battery soon*/
#define ZW_ALRM_EVT_CHARGE_BATT_NOW     0x0F    /**< Charge battery now!*/
#define ZW_ALRM_EVT_BKUP_BATT_LOW       0x10    /**< Back-up battery is low*/
#define ZW_ALRM_EVT_BATT_FLUID_LOW      0x11    /**< Battery fluid is low*/

/** Z-wave alarm event for system alarm*/
#define ZW_ALRM_EVT_HW                  0x01    /**< System hardware failure*/
#define ZW_ALRM_EVT_SW                  0x02    /**< System software failure*/
#define ZW_ALRM_EVT_HW_OEM_CODE         0x03    /**< System hardware failure with OEM proprietary failure code*/
#define ZW_ALRM_EVT_SW_OEM_CODE         0x04    /**< System software failure with OEM proprietary failure code*/

/** Z-wave alarm event for emergency alarm*/
#define ZW_ALRM_EVT_POLICE              0x01    /**< Contact police*/
#define ZW_ALRM_EVT_FIRE                0x02    /**< Contact fire service*/
#define ZW_ALRM_EVT_MEDICAL             0x03    /**< Contact medical service*/

/** Z-wave alarm event for alarm clock*/
#define ZW_ALRM_EVT_WKUP                0x01    /**< Wake up alert*/
#define ZW_ALRM_EVT_TIMER_ENDED         0x02    /**< Timer ended*/
#define ZW_ALRM_EVT_TIME_REMAINING      0x03    /**< Time remaining. Event parameter value (3 bytes): byte-0 unit = hours,
                                                     byte-1 unit = minutes, byte-2 unit = seconds*/

/** Z-wave alarm event for Appliance*/
#define ZW_ALRM_EVT_PROG_STARTED		0x01    /**< Program started*/
#define ZW_ALRM_EVT_PROG_IN_PROGRESS	0x02    /**< Program in progress*/
#define ZW_ALRM_EVT_PROG_END			0x03    /**< Program completed*/
#define ZW_ALRM_EVT_REPLACE_FILTER		0x04    /**< Replace main filter*/
#define ZW_ALRM_EVT_SET_TEMP_ERR		0x05    /**< Failure to set target temperature*/
#define ZW_ALRM_EVT_SUPPLYING_WATER		0x06    /**< Supplying water*/
#define ZW_ALRM_EVT_WATER_SUPPLY_ERR	0x07    /**< Water supply failure*/
#define ZW_ALRM_EVT_BOILING				0x08    /**< Boiling*/
#define ZW_ALRM_EVT_BOILING_ERR			0x09    /**< Boiling failure*/
#define ZW_ALRM_EVT_WASHING				0x0A    /**< Washing*/
#define ZW_ALRM_EVT_WASHING_ERR			0x0B    /**< Washing failure*/
#define ZW_ALRM_EVT_RINSING				0x0C    /**< Rinsing*/
#define ZW_ALRM_EVT_RINSING_ERR			0x0D    /**< Rinsing failure*/
#define ZW_ALRM_EVT_DRAINING			0x0E    /**< Draining*/
#define ZW_ALRM_EVT_DRAINING_ERR		0x0F    /**< Draining failure*/
#define ZW_ALRM_EVT_SPINNING			0x10    /**< Spinning*/
#define ZW_ALRM_EVT_SPINNING_ERR		0x11    /**< Spinning failure*/
#define ZW_ALRM_EVT_DRYING				0x12    /**< Drying*/
#define ZW_ALRM_EVT_DRYING_ERR			0x13    /**< Drying failure*/
#define ZW_ALRM_EVT_FAN_ERR				0x14    /**< Fan failure*/
#define ZW_ALRM_EVT_COMPRESSOR_ERR		0x15    /**< Compressor failure*/

/** Z-wave alarm event for Home Health*/
#define ZW_ALRM_EVT_LEAVING_BED						0x01    /**< Leaving Bed*/
#define ZW_ALRM_EVT_SITTING_ON_BED					0x02    /**< Sitting on bed*/
#define ZW_ALRM_EVT_LYING_ON_BED					0x03    /**< Lying on bed*/
#define ZW_ALRM_EVT_POSTURE_CHANGED					0x04    /**< Posture changed*/
#define ZW_ALRM_EVT_SITTING_ON_BED_EDGE				0x05    /**< Sitting on edge of bed*/
#define ZW_ALRM_EVT_VOLATILE_ORGANIC_COMPOUND_LVL   0x06    /**< Volatile Organic Compound level. Event parameter value (pollution level):
                                                                 1=clean, 2=Slightly polluted, 3=Moderately polluted, 4=Highly polluted,*/

/** Z-wave alarm event for Siren*/
#define ZW_ALRM_EVT_SIREN_ACT						0x01    /**< Siren Active*/

/** Z-wave alarm event for Water Valve*/
#define ZW_ALRM_EVT_VALVE_OP						0x01    /**< Valve Operation. Event parameter value: 0=closed, 1=open*/
#define ZW_ALRM_EVT_MSTR_VALVE_OP					0x02    /**< Master Valve Operation. Event parameter value: 0=closed, 1=open*/
#define ZW_ALRM_EVT_SHORT_CCT						0x03    /**< Valve Short Circuit*/
#define ZW_ALRM_EVT_MSTR_SHORT_CCT					0x04    /**< Master Valve Short Circuit*/
#define ZW_ALRM_EVT_CUR_ALRM						0x05    /**< Valve Current Alarm. Event parameter value: 1=no data, 2=below low treshold,
                                                                 3=above high treshold, 4=max*/
#define ZW_ALRM_EVT_MSTR_CUR_ALRM					0x06    /**< Master Valve Current Alarm. Event parameter value same as Valve Current Alarm*/

/** Z-wave alarm event for Weather*/
#define ZW_ALRM_EVT_RAIN							0x01    /**< Rain*/
#define ZW_ALRM_EVT_MOIST							0x02    /**< Moisture*/
#define ZW_ALRM_EVT_FREEZE							0x03    /**< Freeze*/

/** Z-wave alarm event for Irrigation*/
#define ZW_ALRM_EVT_SCHED_START						0x01    /**< Schedule Started. Event parameter value is schedule ID*/
#define ZW_ALRM_EVT_SCHED_FIN						0x02    /**< Schedule Finished. Event parameter value is schedule ID*/
#define ZW_ALRM_EVT_VLV_TBL_RUN_START				0x03    /**< Valve Table Run Started. Event parameter value is valve table ID*/
#define ZW_ALRM_EVT_VLV_TBL_RUN_FIN					0x04    /**< Valve Table Run Finished. Event parameter value is valve table ID*/
#define ZW_ALRM_EVT_DEV_UNCONFIG					0x05    /**< Device is not Configured*/

/** Z-wave alarm event for Gas*/
#define ZW_ALRM_EVT_COMBUST_GAS_DET_L				0x01    /**< Combustible Gas detected with location info*/
#define ZW_ALRM_EVT_COMBUST_GAS_DET					0x02    /**< Combustible Gas detected with unknown location info*/
#define ZW_ALRM_EVT_TOXIC_GAS_DET_L					0x03    /**< Toxic Gas detected with location info*/
#define ZW_ALRM_EVT_TOXIC_GAS_DET					0x04    /**< Toxic Gas detected with unknown location info*/
#define ZW_ALRM_EVT_GAS_ALRM_TEST					0x05    /**< Gas Alarm Test*/
#define ZW_ALRM_EVT_GAS_ALRM_REPLACE				0x06    /**< Replacement Required*/

/** Z-wave alarm event for Pest Control*/
#define ZW_ALRM_EVT_TRAP_ARMED_L					0x01    /**< Trap armed with location info*/
#define ZW_ALRM_EVT_TRAP_ARMED						0x02    /**< Trap armed*/
#define ZW_ALRM_EVT_TRAP_REARM_REQ_L				0x03    /**< Trap re-arm required with location info*/
#define ZW_ALRM_EVT_TRAP_REARM_REQ					0x04    /**< Trap re-arm required*/
#define ZW_ALRM_EVT_PEST_DET_L						0x05    /**< Pest detected with location info*/
#define ZW_ALRM_EVT_PEST_DET						0x06    /**< Pest detected*/
#define ZW_ALRM_EVT_PEST_EXTERMINATED_L				0x07    /**< Pest exterminated with location info*/
#define ZW_ALRM_EVT_PEST_EXTERMINATED				0x08    /**< Pest exterminated*/

/** Z-wave alarm event for Light Sensor*/
#define ZW_ALRM_EVT_LIGHT_DET						0x01    /**< Light detected*/
#define ZW_ALRM_EVT_COLOR_TRANS_DET					0x02    /**< Light color transition detected*/

/**
@}
*/

/** @name Alarm_param
* Z-wave alarm parameter type definition
@{
*/
#define ZW_ALRM_PARAM_LOC               1    /**< Node location UTF-8 string (NULL terminated)*/
#define ZW_ALRM_PARAM_USRID             2    /**< User id*/
#define ZW_ALRM_PARAM_OEM_ERR_CODE      3    /**< OEM proprietary system failure code */
#define ZW_ALRM_PARAM_PROPRIETARY		4	 /**< Proprietary event parameters */
#define ZW_ALRM_PARAM_EVENT_ID		    5	 /**< Event id which is no more active*/
#define ZW_ALRM_PARAM_UNKNOWN			0xFF /**< Unknown alarm event parameters. It could be from a higher version of CC,
												  or the device violates the spec and send parameters when Spec does not define*/
/**
@}
*/

/** @name Alarm_status
* Z-wave alarm status definition
@{
*/
#define ZW_ALRM_STS_DEACTIVATED         0    /**< Unsolicited alarm report is deactivated (push mode) or
                                                  report message carries valid notification information (pull mode)*/
#define ZW_ALRM_STS_ACTIVATED           0xFF /**< Unsolicited alarm report is activated (push mode) */
#define ZW_ALRM_STS_NO_PEND_NOTICE      0xFE /**< Report message does not carry valid notification information. The queue is empty (pull mode) */
/**
@}
*/

/** Alarm */
typedef struct
{
    uint8_t     type;           /**< Vendor specific alarm type*/
    uint8_t     level;          /**< Vendor specific alarm level*/
    uint8_t     ex_info;        /**< Flag to indicate the following extended info fields are valid.
                                     1= valid; 0= invalid (no extended info fields)*/
    /*extended info fields*/
    uint8_t     ex_zensr_nodeid;/**< Zensor Net source node id. This field is 0 if device is not based on Zensor Net*/
    uint8_t     ex_status;      /**< Z-wave alarm status (ZW_ALRM_STS_XXX) */
    uint8_t     ex_type;        /**< Z-wave alarm type (ZW_ALRM_XXX) */
    uint8_t     ex_event;       /**< Z-wave alarm event (ZW_ALRM_EVT_XXX) */
	uint8_t		ex_has_sequence;/**< Flag to indicate whether this report has sequence number as stored in "ex_sequence_no".
									 1=valid; 0 = invalid */
	uint8_t		ex_sequence_no;	/**< Z-wave alarm sequence number. Only valid if field ex_has_sequence is 1.*/
    uint8_t     ex_evt_len;     /**< Z-wave alarm event parameter length. Zero if the event has no parameter */
    uint8_t     ex_evt_type;    /**< Z-wave alarm event parameter type (ZW_ALRM_PARAM_XXX) */
    uint8_t     ex_evt_prm[1];  /**< Z-wave alarm event parameter place holder*/
}
zwalrm_t, *zwalrm_p;

typedef void (*zwrep_alrm_fn)(zwifd_p ifd, zwalrm_p  alarm_info, time_t ts);
/**<
report callback for alarm
@param[in]	ifd	        interface
@param[in]	alarm_info  alarm info
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
                    values should be ignored.
*/

int zwif_alrm_rpt_set(zwifd_p ifd, zwrep_alrm_fn rpt_cb);
/**<
setup an alarm report callback function
@param[in]	ifd         interface descriptor
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_alrm_get(zwifd_p ifd, int16_t vtype, uint8_t ztype, uint8_t evt, int flag);
/**<
get the state of the alarm device (push mode) or the pending notification (pull mode) through report callback
@param[in]	ifd	        interface
@param[in]	vtype	    vendor specific alarm type. Zero if this field is not used; -1 to indicate "don't care" for cache get.
@param[in]	ztype	    Z-wave alarm type (ZW_ALRM_XXX). Zero if this field is not used; 0xFF = select a supported Notification Type (push mode),
                        or retrieve the first alarm detection (pull mode).
@param[in]	evt	        Event corresponding to Z-wave alarm type. Zero if this field is not used. This parameter is valid for push mode only
@param[in]	flag	    flag, see ZWIF_GET_BMSK_XXX
@return		ZW_ERR_XXX
*/

int zwif_alrm_set(zwifd_p ifd, uint8_t ztype, uint8_t sts);
/**<
set the state of the specified Z-Wave alarm type (push mode) or clear a persistent notification (pull mode)
@param[in]	ifd	    interface
@param[in]	ztype	Z-wave alarm type (ZW_ALRM_XXX)
@param[in]	sts     Z-wave alarm status. For alarm operating in push mode: 0= disable unsolicited report ; 0xFF= enable.
                    For pull mode: 0=clear a persistent notification.
@return	ZW_ERR_XXX
*/

typedef void (*zwrep_alrm_sup_fn)(zwifd_p ifd, uint8_t have_vtype, uint8_t ztype_len, uint8_t *ztype, int valid);
/**<
report callback for supported alarm types
@param[in]	ifd	        interface
@param[in]	have_vtype  flag to indicate whether vendor specific alarm type supported. 1=supported; else 0=unsupported
@param[in]	ztype_len   size of ztype buffer
@param[in]	ztype       buffer to store supported Z-wave alarm types (ZW_ALRM_XXX)
@param[in]	valid       validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/

int zwif_alrm_sup_get(zwifd_p ifd, zwrep_alrm_sup_fn cb, int cache);
/**<
get the supported alarm types
@param[in]	ifd	    interface
@param[in]	cb	    report callback function
@param[in]	cache	flag: to get data from cached only. If set, no fetching from real device when cache unavailable.
@return ZW_ERR_XXX
*/

int zwif_alrm_sup_cache_get(zwifd_p ifd, uint8_t *have_vtype, uint8_t *ztype_cnt, uint8_t *ztype_buf);
/**<
get the supported alarm types from cache
@param[in]	ifd	        interface
@param[out]	have_vtype  flag to indicate whether vendor specific alarm type supported. 1=supported; else 0=unsupported
@param[out]	ztype_cnt   number of supported Z-wave alarm types
@param[out]	ztype_buf   caller supplied buffer of size 255 bytes to store supported Z-wave alarm types (ZW_ALRM_XXX)
@return ZW_ERR_XXX
*/

typedef void (*zwrep_alrm_evt_fn)(zwifd_p ifd, uint8_t ztype, uint8_t evt_len, uint8_t *evt, int valid);
/**<
report callback for supported alarm types
@param[in]	ifd	        interface
@param[in]	ztype       Z-wave alarm type (ZW_ALRM_XXX)
@param[in]	evt_len     size of evt buffer
@param[in]	evt         buffer to store supported event of the alarm type specified in ztype
@param[in]	valid       validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/

int zwif_alrm_sup_evt_get(zwifd_p ifd, uint8_t ztype, zwrep_alrm_evt_fn cb, int cache);
/**<
get the supported events of a specified alarm type
@param[in]	ifd	    interface
@param[in]	ztype   Z-wave alarm type (ZW_ALRM_XXX)
@param[in]	cb	    report callback function
@param[in]	cache	flag: to get data from cached only. If set, no fetching from real device when cache unavailable.
@return ZW_ERR_XXX
*/

int zwif_alrm_sup_evt_cache_get(zwifd_p ifd, uint8_t ztype, uint8_t *evt_cnt, uint8_t *evt_buf);
/**<
get the supported events of a specified alarm type from cache
@param[in]	ifd	        interface
@param[in]	ztype       Z-wave alarm type (ZW_ALRM_XXX)
@param[out]	evt_cnt     number of supported events
@param[out]	evt_buf     caller supplied buffer of size 255 bytes to store supported events
@return ZW_ERR_XXX
*/

int zwif_alrm_vtype_sup_get(zwifd_p ifd, if_rec_alarm_match_t **rec_head);
/**<
get the supported vendor specific alarm types
@param[in]	ifd	        interface
@param[out]	rec_head	head of the alarm record link-list
@return ZW_ERR_XXX
*/

/**
@}
@defgroup If_Battery  Battery Interface APIs
Battery command that can be used to read the remaining level of a device
@ingroup zwarecapi
@{
*/

int zwif_battery_rpt_set(zwifd_p ifd, zwrep_ts_fn rpt_cb);
/**<
setup a battery report callback function
@param[in]	ifd         Interface descriptor
@param[in]	rpt_cb	    Report callback function
return      ZW_ERR_XXX
*/

int zwif_battery_get(zwifd_p ifd, int flag);
/**<
get battery report through report callback
@param[in]	ifd	        interface
@param[in]	flag	    flag, see ZWIF_GET_BMSK_XXX
@return		ZW_ERR_XXX
*/

/**
@}
@defgroup If_Thrm_fan Thermostat Fan Mode and State Interface APIs
Used to report thermostat fan operating conditions
@ingroup zwarecapi
@{
*/

/** @name Thermostat_fan_mode
* Thermostat Fan Mode definition
@{
*/
#define ZW_THRMO_FAN_MD_AUTO_LO     0       /**< Turn the manual fan operation off unless turned on by the furnace or AC.
                                                 Lower speed is selected in case it is a two-speed fan.*/
#define ZW_THRMO_FAN_MD_LOW         1       /**< Turn the manual fan to low speed*/
#define ZW_THRMO_FAN_MD_AUTO_HI     2       /**< Turn the manual fan operation off unless turned on by the furnace or AC.
                                                 High speed is selected in case it is a two-speed fan.*/
#define ZW_THRMO_FAN_MD_HI          3       /**< Turn the manual fan to high speed*/
#define ZW_THRMO_FAN_MD_AUTO_MID    4       /**< Turn the manual fan operation off unless turned on by the furnace or AC.
                                                 Medium speed is selected in case it is a three-speed fan.*/
#define ZW_THRMO_FAN_MD_MID         5       /**< Turn the manual fan to medium speed*/
#define ZW_THRMO_FAN_MD_CIR         6       /**< Turn the manual fan operation off unless turned on by the circulation
                                                 algorithms.*/
#define ZW_THRMO_FAN_MD_HUM_CIR     7       /**< Turn the manual fan operation off unless turned on by the humidity
                                                 circulation algorithms.*/
#define ZW_THRMO_FAN_MD_LEFT_RIGHT  8       /**< Turn the manual fan operation off unless turned on by the manufacturer specific
                                                 "left & right" circulation algorithms.*/
#define ZW_THRMO_FAN_MD_UP_DOWN     9       /**< Turn the manual fan operation off unless turned on by the manufacturer specific
                                                 "up & down" circulation algorithms.*/
#define ZW_THRMO_FAN_MD_QUIET       10      /**< Turn the manual fan operation off unless turned on by the manufacturer specific
                                                 "quiet" algorithms.*/
/**
@}
*/

typedef void (*zwrep_thrmo_fan_md_fn)(zwifd_p ifd, uint8_t off, uint8_t mode, time_t ts);
/**<
report callback for thermostat fan operating mode
@param[in]	ifd	    interface
@param[in]	off     fan off mode flag. Non-zero indicates that the fan is fully OFF,
                    “0” indicates that it is possible to change between Fan Modes.
@param[in]	mode    fan operating mode, ZW_THRMO_FAN_MD_XXX
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
                    values should be ignored.
*/

int zwif_thrmo_fan_md_rpt_set(zwifd_p ifd, zwrep_thrmo_fan_md_fn rpt_cb);
/**<
setup an alarm report callback function
@param[in]	ifd         interface descriptor
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_thrmo_fan_md_get(zwifd_p ifd, int flag);
/**<
get the thermostat fan operating mode through report callback
@param[in]	ifd	        interface
@param[in]	flag	    flag, see ZWIF_GET_BMSK_XXX
@return		ZW_ERR_XXX
*/

int zwif_thrmo_fan_md_set(zwifd_p ifd, uint8_t off, uint8_t mode);
/**<
set the fan mode in the device
@param[in]	ifd	    interface
@param[in]	off     fan off mode flag. Non-zero will switch the fan fully OFF.
                    In order to activate a fan mode this flag must be set to “0”.
@param[in]	mode    fan operating mode, ZW_THRMO_FAN_MD_XXX
@return	ZW_ERR_XXX
*/

typedef void (*zwrep_thrmo_fan_md_sup_fn)(zwifd_p ifd, uint8_t off, uint8_t mode_len, uint8_t *mode, int valid);
/**<
report callback for supported thermostat fan operating modes
@param[in]	ifd	        interface
@param[in]	off         flag to indicate whether off mode is supported.
@param[in]	mode_len    size of mode buffer
@param[in]	mode        buffer to store supported thermostat fan operating modes (ZW_THRMO_FAN_MD_XXX)
@param[in]	valid       validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/

int zwif_thrmo_fan_md_sup_get(zwifd_p ifd, zwrep_thrmo_fan_md_sup_fn cb, int cache);
/**<
get the supported thermostat fan operating modes
@param[in]	ifd	    interface
@param[in]	cb	    report callback function
@param[in]	cache	flag: to get data from cached only. If set, no fetching from real device when cache unavailable.
@return ZW_ERR_XXX
*/

int zwif_thrmo_fan_md_sup_cache_get(zwifd_p ifd, uint8_t *off, uint8_t *mode_cnt, uint8_t *mode);
/**<
get supported thermostat fan operating modes from cache
@param[in]	ifd	        interface
@param[out]	off         flag to indicate whether off mode is supported
@param[out]	mode_cnt    supported thermostat fan operating modes count
@param[out]	mode        caller supplied buffer of size 255 bytes to store supported thermostat fan operating modes (ZW_THRMO_FAN_MD_XXX)
@return ZW_ERR_XXX
*/

/** @name Thermostat_fan_state
*  Thermostat Fan State definition
@{
*/
#define ZW_THRMO_FAN_STA_IDLE       0       /**< Idle */
#define ZW_THRMO_FAN_STA_LOW        1       /**< Running / Running Low - Lower speed is selected in case it is a two-speed fan*/
#define ZW_THRMO_FAN_STA_HI         2       /**< Running High - High speed is selected in case it is a two-speed fan*/
#define ZW_THRMO_FAN_STA_MEDIUM     3       /**< Running Medium*/
#define ZW_THRMO_FAN_STA_CIR        4       /**< Circulation Mode*/
#define ZW_THRMO_FAN_STA_CIR_HUMID  5       /**< Humidity Circulation Mode*/
#define ZW_THRMO_FAN_STA_CIR_RL     6       /**< Right-Left Circulation Mode*/
#define ZW_THRMO_FAN_STA_CIR_UP_DN  7       /**< Up-down Circulation Mode*/
#define ZW_THRMO_FAN_STA_CIR_QUIET  8       /**< Quiet Circulation Mode*/

/**
@}
*/

typedef void (*zwrep_thrmo_fan_sta_fn)(zwifd_p ifd, uint8_t state, time_t ts);
/**<
report callback for thermostat fan operating state
@param[in]	ifd	    interface
@param[in]	state   fan operating state, ZW_THRMO_FAN_STA_XXX
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
                    values should be ignored.
*/

int zwif_thrmo_fan_sta_rpt_set(zwifd_p ifd, zwrep_thrmo_fan_sta_fn rpt_cb);
/**<
setup a thermostat fan operating state report callback function
@param[in]	ifd         interface descriptor
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_thrmo_fan_sta_get(zwifd_p ifd, int flag);
/**<
get the thermostat fan operating state through report callback
@param[in]	ifd	        interface
@param[in]	flag	    flag, see ZWIF_GET_BMSK_XXX
@return		ZW_ERR_XXX
*/

/** @name Thermostat_mode
*  Thermostat Mode definition
@{
*/
#define ZW_THRMO_MD_OFF             0       /**< System is off */
#define ZW_THRMO_MD_HEAT            1       /**< Only heating will occur*/
#define ZW_THRMO_MD_COOL            2       /**< Only cooling will occur */
#define ZW_THRMO_MD_AUTO            3       /**< Auto*/
#define ZW_THRMO_MD_AUX_HEAT        4       /**< Auxiliary/Emergency Heat- the thermostat may be put into Aux
                                                 heat mode simply to use a more efficient secondary heat source when
                                                 there are no failures of the compressor or heat pump unit itself.*/
#define ZW_THRMO_MD_RESUME          5       /**< The system will resume from last active mode.*/
#define ZW_THRMO_MD_FAN             6       /**< Only cycle fan to circulate air. */
#define ZW_THRMO_MD_FURNACE         7       /**< Only furnace */
#define ZW_THRMO_MD_DRY_AIR         8       /**< The system will cycle cooling in relation to the room and set point
                                                 temperatures in order to remove moisture from ambient*/
#define ZW_THRMO_MD_MOIST_AIR       9       /**< Humidification */
#define ZW_THRMO_MD_AUTO_CHANGEOVER 10      /**< Heating or cooling will come on according to the auto changeover setpoint*/
#define ZW_THRMO_MD_ENE_SAVE_HEAT   11      /**< Energy Save Mode Heating will occur */
#define ZW_THRMO_MD_ENE_SAVE_COOL   12      /**< Energy Save Mode Cooling will occur.*/
#define ZW_THRMO_MD_AWAY            13      /**< special Heating Mode, i.e. preventing water from freezing in
                                                 forced water systems.*/
#define ZW_THRMO_MD_FULL_PWR        15      /**< SPEED UP / FULL POWER heating or cooling mode will be activated when
                                                 temperature exceeds FULL POWER set point*/
#define ZW_THRMO_MD_MANF_SPECIFIC   31      /**< Vendor specific thermostat mode*/


/**
@}
*/

typedef void (*zwrep_thrmo_md_fn)(zwifd_p ifd, uint8_t mode, uint8_t *manf_dat, uint8_t manf_dat_cnt, time_t ts);
/**<
report callback for thermostat mode
@param[in]	ifd	            interface
@param[in]	mode            mode, ZW_THRMO_MD_XXX
@param[in]	manf_dat        Manufacturer data when the mode is ZW_THRMO_MD_MANF_SPECIFIC
@param[in]	manf_dat_cnt    Manufacturer data count in bytes. Valid range is from 0 to 7 bytes.
@param[in]	ts              time stamp.  If this is zero, the callback has no data and hence other parameter
                            values should be ignored.
*/

int zwif_thrmo_md_rpt_set(zwifd_p ifd, zwrep_thrmo_md_fn rpt_cb);
/**<
setup a thermostat mode report callback function
@param[in]	ifd         interface descriptor
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_thrmo_md_get(zwifd_p ifd, int flag);
/**<
get the thermostat mode through report callback
@param[in]	ifd	        interface
@param[in]	flag	    flag, see ZWIF_GET_BMSK_XXX
@return		ZW_ERR_XXX
*/

int zwif_thrmo_md_set(zwifd_p ifd, uint8_t mode, uint8_t *manf_dat, uint8_t manf_dat_cnt);
/**<
set the mode in the device
@param[in]	ifd	            interface
@param[in]	mode            mode, ZW_THRMO_MD_XXX
@param[in]	manf_dat        Manufacturer data when the mode is ZW_THRMO_MD_MANF_SPECIFIC
@param[in]	manf_dat_cnt    Manufacturer data count in bytes. Valid range is from 0 to 7 bytes.
@return	ZW_ERR_XXX
*/

typedef void (*zwrep_thrmo_md_sup_fn)(zwifd_p ifd, uint8_t mode_len, uint8_t *mode, int valid);
/**<
report callback for supported thermostat modes
@param[in]	ifd	        interface
@param[in]	mode_len    size of mode buffer
@param[in]	mode        buffer to store supported thermostat modes (ZW_THRMO_MD_XXX except ZW_THRMO_MD_MANF_SPECIFIC)
@param[in]	valid       validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/

int zwif_thrmo_md_sup_get(zwifd_p ifd, zwrep_thrmo_md_sup_fn cb, int cache);
/**<
get the supported thermostat modes
@param[in]	ifd	    interface
@param[in]	cb	    report callback function
@param[in]	cache	flag: to get data from cached only. If set, no fetching from real device when cache unavailable.
@return ZW_ERR_XXX
*/

int zwif_thrmo_md_sup_cache_get(zwifd_p ifd, uint8_t *mode_buf, uint8_t *mode_cnt);
/**<
get the supported thermostat modes from cache
@param[in]	ifd	        interface
@param[out]	mode_buf    caller supplied buffer of size 255 bytes to store supported thermostat modes (ZW_THRMO_MD_XXX except ZW_THRMO_MD_MANF_SPECIFIC)
@param[out]	mode_cnt    number of supported modes
@return ZW_ERR_XXX
*/

/** @name Thermostat_op_state
*  Thermostat Operating State definition
@{
*/
#define ZW_THRMO_OP_STA_IDLE       0       /**< Idle */
#define ZW_THRMO_OP_STA_HEAT       1       /**< Heating*/
#define ZW_THRMO_OP_STA_COOL       2       /**< Cooling*/
#define ZW_THRMO_OP_STA_FAN        3       /**< Fan only */
#define ZW_THRMO_OP_STA_PD_HEAT    4       /**< Pending Heat*/
#define ZW_THRMO_OP_STA_PD_COOL    5       /**< Pending Cool*/
#define ZW_THRMO_OP_STA_VENT       6       /**< Vent/Economizer */
#define ZW_THRMO_OP_STA_AUX_HEAT   7       /**< Aux heating*/
#define ZW_THRMO_OP_STA_HEAT_2     8       /**< Second stage heating*/
#define ZW_THRMO_OP_STA_COOL_2     9       /**< Second stage cooling*/
#define ZW_THRMO_OP_STA_AUX_HEAT_2 10      /**< Second stage aux heating*/
#define ZW_THRMO_OP_STA_AUX_HEAT_3 11      /**< Third stage aux heating*/
/**
@}
*/

/** Thermostat operating state log */
typedef struct
{
	time_t	    tmstamp;	/**< time stamp */
    uint8_t     log_type;	/**< operating state log type, ZW_THRMO_OP_STA_XXX */
    uint8_t     today_hr;	/**< The number of hours (00 ~ 24) the thermostat has been in the indicated operating state since 12:00 am of the current day*/
    uint8_t     today_mn;	/**< The number of minutes (00 ~ 59) the thermostat has been in the indicated operating state since 12:00 am of the current day*/
    uint8_t     prev_hr;	/**< The number of hours (00 ~ 24) the thermostat has been in the indicated operating state since 12:00 am of the previous day*/
    uint8_t     prev_mn;	/**< The number of minutes (00 ~ 59) the thermostat has been in the indicated operating state since 12:00 am of the previous day*/
} zwthrmo_op_sta_log_t;

typedef void (*zwrep_thrmo_op_sta_fn)(zwifd_p ifd, uint8_t state, time_t ts);
/**<
report callback for thermostat operating state
@param[in]	ifd	    interface
@param[in]	state   operating state, ZW_THRMO_OP_STA_XXX
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
                    values should be ignored.
*/

int zwif_thrmo_op_sta_rpt_set(zwifd_p ifd, zwrep_thrmo_op_sta_fn rpt_cb);
/**<
setup a thermostat operating state report callback function
@param[in]	ifd         interface descriptor
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_thrmo_op_sta_get(zwifd_p ifd, int flag);
/**<
get the thermostat operating state through report callback
@param[in]	ifd	        interface
@param[in]	flag	    flag, see ZWIF_GET_BMSK_XXX
@return		ZW_ERR_XXX
*/

typedef void (*zwrep_thrmo_op_sta_log_sup_fn)(zwifd_p ifd, uint8_t op_sta_len, uint8_t *op_sta, int valid);
/**<
report callback for supported thermostat operating state logging
@param[in]	ifd	        interface
@param[in]	op_sta_len  size of op_sta buffer
@param[in]	op_sta      buffer to store supported thermostat operating state logging (ZW_THRMO_OP_STA_XXX)
@param[in]	valid       validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/

int zwif_thrmo_op_sta_log_sup_get(zwifd_p ifd, zwrep_thrmo_op_sta_log_sup_fn cb, int cache);
/**<
get the supported thermostat operating state logging
@param[in]	ifd	    interface
@param[in]	cb	    report callback function
@param[in]	cache	flag: to get data from cached only. If set, no fetching from real device when cache unavailable.
@return ZW_ERR_XXX
*/

int zwif_thrmo_op_sta_log_sup_cache_get(zwifd_p ifd, uint8_t *op_sta_cnt, uint8_t *op_sta);
/**<
get the supported thermostat operating state logging from cache
@param[in]	ifd	        interface
@param[out]	op_sta_cnt  supported thermostat operating state logging count
@param[out]	op_sta      caller supplied buffer of size 255 bytes to store supported thermostat operating state logging (ZW_THRMO_OP_STA_XXX)
@return ZW_ERR_XXX
*/

typedef void (*zwrep_thrmo_op_sta_log_fn)(zwifd_p ifd, zwthrmo_op_sta_log_t *sta_log, uint8_t sta_log_cnt, int valid);
/**<
report callback for thermostat operating state logs
@param[in]	ifd	        interface
@param[in]	sta_log     state logging array
@param[in]	sta_log_cnt number of state loggings in sta_log
@param[in]	valid       Validity. If this is zero, the callback has no data and hence other parameter
                        values should be ignored.
*/

int zwif_thrmo_op_sta_log_rpt_set(zwifd_p ifd, zwrep_thrmo_op_sta_log_fn rpt_cb);
/**<
setup a thermostat operating state logging report callback function
@param[in]	ifd         interface descriptor
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_thrmo_op_sta_log_get(zwifd_p ifd, uint8_t *sta_log_type, uint8_t sta_log_type_cnt, int flag);
/**<
get the thermostat operating state logging through report callback
@param[in]	ifd	                interface
@param[in]	sta_log_type        operating state logging type array, ZW_THRMO_OP_STA_XXX, except ZW_THRMO_OP_STA_IDLE.
@param[in]	sta_log_type_cnt    number of operating state logging type in sta_log_type array
@param[in]	flag	            flag, see ZWIF_GET_BMSK_XXX
@return		ZW_ERR_XXX
*/

/**
@}
@defgroup If_Thrm_Setback Thermostat Setback Interface APIs
Used to control and read settings of thermostat setback
@ingroup zwarecapi
@{
*/

/** @name Thermostat_setback_type
*  Thermostat Setback Type definition
@{
*/
#define ZW_THRMO_SETB_TYP_NO_OVERRIDE       0       /**< No override */
#define ZW_THRMO_SETB_TYP_TEMP_OVR          1       /**< Temporary override */
#define ZW_THRMO_SETB_TYP_PERM_OVR          2       /**< Permanent override */
/**
@}
*/

/** @name Thermostat_setback_state
*  Thermostat Setback State definition
@{
*/
#define ZW_THRMO_SETB_STA_SETB              0       /**< Setback in 1/10 degrees (Kelvin)*/
#define ZW_THRMO_SETB_STA_FROST_PROCT       1       /**< Frost Protection */
#define ZW_THRMO_SETB_STA_ENER_SAVE         2       /**< Energy Saving Mode */
#define ZW_THRMO_SETB_STA_UNUSED            3       /**< Unused */
/**
@}
*/

typedef void (*zwrep_thrmo_setb_fn)(zwifd_p ifd, uint8_t type, uint8_t state, int8_t tenth_degree);
/**<
report callback for thermostat setback
@param[in]	ifd	            interface
@param[in]	type            setback type, ZW_THRMO_SETB_TYP_XXX
@param[in]	state           setback state, ZW_THRMO_SETB_STA_XXX
@param[in]	tenth_degree	1/10 of a degree.  This parameter is valid only if state equals to ZW_THRMO_SETB_STA_SETB
*/

int zwif_thrmo_setb_rpt_set(zwifd_p ifd, zwrep_thrmo_setb_fn rpt_cb);
/**<
setup a thermostat setback report callback function
@param[in]	ifd         interface descriptor
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_thrmo_setb_get(zwifd_p ifd);
/**<
get the thermostat setback state through report callback
@param[in]	ifd	        interface
@return		ZW_ERR_XXX
*/

int zwif_thrmo_setb_set(zwifd_p ifd, uint8_t type, uint8_t state, int8_t tenth_degree);
/**<
set the state in the device
@param[in]	ifd	            interface
@param[in]	type            setback type, ZW_THRMO_SETB_TYP_XXX
@param[in]	state           setback state, ZW_THRMO_SETB_STA_XXX
@param[in]	tenth_degree	1/10 of a degree (Kelvin).  This parameter is valid if state equals to ZW_THRMO_SETB_STA_SETB.
                            Valid values: -128 to 120 (inclusive). i.e. setback temperature ranges from -12.8 degree K to 12 degree K.
@return	ZW_ERR_XXX
*/

/**
@}
@defgroup If_Thrm_Setpoint Thermostat Setpoint Interface APIs
Used to control and read settings of thermostat setpoint
@ingroup zwarecapi
@{
*/

/** @name Thermostat_setpoint_type
*  Thermostat Setpoint Type definition
@{
*/
#define ZW_THRMO_SETP_TYP_HEATING           1       /**< Heating */
#define ZW_THRMO_SETP_TYP_COOLING           2       /**< Cooling */
#define ZW_THRMO_SETP_TYP_FURNACE           7       /**< Furnace */
#define ZW_THRMO_SETP_TYP_DRY               8       /**< Dry air */
#define ZW_THRMO_SETP_TYP_MOIST             9       /**< Moist air */
#define ZW_THRMO_SETP_TYP_AUTO_CHANGEOVER   10      /**< Auto changeover */
#define ZW_THRMO_SETP_TYP_ENE_SAVE_HEAT     11      /**< Energy Save Heating */
#define ZW_THRMO_SETP_TYP_ENE_SAVE_COOL     12      /**< Energy Save Cooling */
#define ZW_THRMO_SETP_TYP_AWAY_HEAT         13      /**< Away heating */
#define ZW_THRMO_SETP_TYP_AWAY_COOL         14      /**< Away cooling */
#define ZW_THRMO_SETP_TYP_FULL_POWER        15      /**< Full power */
/**
@}
*/

/** @name Thermostat_setpoint_unit
*  Thermostat Setpoint Unit definition
@{
*/
#define ZW_THRMO_SETP_UNIT_C           0       /**< Celsius */
#define ZW_THRMO_SETP_UNIT_F           1       /**< Fahrenheit */
/**
@}
*/


/** Thermostat setpoint data */
typedef struct
{
    uint8_t     type;		/**< ZW_THRMO_SETP_TYP_XXX */
    uint8_t     precision;  /**< Decimal places of the value.  The decimal
                                 value 1025 with precision 2 is therefore equal to 10.25.*/
    uint8_t     unit;	    /**< ZW_THRMO_SETP_UNIT_XXX */
    uint8_t     size;	    /**< Data size: 1,2,or 4 bytes*/
    uint8_t     data[4];	/**< Setpoint data (a signed number) with the first byte as MSB */
}
zwsetp_t, *zwsetp_p;

/** Thermostat setpoint temparature range */
typedef struct zwsetp_temp_range
{
    zwsetp_t    min;        /**< Minimum setpoint temparature */
    zwsetp_t    max;        /**< Maximum setpoint temparature */
}
zwsetp_temp_range_t;


typedef void (*zwrep_thrmo_setp_fn)(zwifd_p ifd, zwsetp_p data, time_t ts);
/**<
report callback for thermostat setpoint
@param[in]	ifd	    interface
@param[in]	data    setpoint data
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
                    values should be ignored.
*/

int zwif_thrmo_setp_rpt_set(zwifd_p ifd, zwrep_thrmo_setp_fn rpt_cb);
/**<
setup a thermostat setpoint report callback function
@param[in]	ifd         interface descriptor
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_thrmo_setp_get(zwifd_p ifd, uint8_t type, int flag);
/**<
get the thermostat setpoint through report callback
@param[in]	ifd	        interface
@param[in]	type	    setpoint type, ZW_THRMO_SETP_TYP_XXX
@param[in]	flag	    flag, see ZWIF_GET_BMSK_XXX
@return		ZW_ERR_XXX
*/

int zwif_thrmo_setp_set(zwifd_p ifd, zwsetp_p data);
/**<
set the setpoint in the device
@param[in]	ifd	    interface
@param[in]	data    setpoint data
@return	ZW_ERR_XXX
*/

typedef void (*zwrep_thrmo_setp_sup_fn)(zwifd_p ifd, uint8_t type_len, uint8_t *type, int valid);
/**<
report callback for supported thermostat modes
@param[in]	ifd	        interface
@param[in]	type_len    size of type buffer
@param[in]	type        buffer to store supported thermostat setpoint types (ZW_THRMO_SETP_TYP_XXX)
@param[in]	valid       validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/

int zwif_thrmo_setp_sup_get(zwifd_p ifd, zwrep_thrmo_setp_sup_fn cb, int cache);
/**<
get the supported thermostat setpoint types
@param[in]	ifd	    interface
@param[in]	cb	    report callback function
@param[in]	cache	flag: to get data from cached only. If set, no fetching from real device when cache unavailable.
@return ZW_ERR_XXX
*/

int zwif_thrmo_setp_sup_cache_get(zwifd_p ifd, uint8_t *type_cnt, uint8_t *type);
/**<
get supported thermostat setpoint types from cache
@param[in]	ifd	        interface
@param[out]	type_cnt    supported setpoint types count
@param[out]	type        caller supplied buffer of size 255 bytes to store supported thermostat setpoint types (ZW_THRMO_SETP_TYP_XXX)
@return ZW_ERR_XXX
*/

typedef void (*zwrep_thrmo_setp_range_fn)(zwifd_p ifd, uint8_t type, zwsetp_p min, zwsetp_p max, int valid);
/**<
report callback for supported thermostat temparature range
@param[in]	ifd	        interface
@param[in]	type	    setpoint type, ZW_THRMO_SETP_TYP_XXX
@param[in]	min         minimum value supported
@param[in]	max         maximum value supported
@param[in]	valid       validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/

int zwif_thrmo_setp_sup_range_get(zwifd_p ifd, uint8_t type, zwrep_thrmo_setp_range_fn cb, int cache);
/**<
get the supported thermostat temparature range
@param[in]	ifd	    interface
@param[in]	type	setpoint type, ZW_THRMO_SETP_TYP_XXX
@param[in]	cb	    report callback function
@param[in]	cache	flag: to get data from cached only. If set, no fetching from real device when cache unavailable.
@return ZW_ERR_XXX
*/

int zwif_thrmo_setp_sup_range_cache_get(zwifd_p ifd, uint8_t setp_type, zwsetp_p min, zwsetp_p max);
/**<
get supported thermostat temparature range from cache
@param[in]	ifd	        interface
@param[in]	setp_type   setpoint type, ZW_THRMO_SETP_TYP_XXX
@param[out]	min         minimum value supported
@param[out]	max         maximum value supported
@return ZW_ERR_XXX
*/

/**
@}
@defgroup If_Clock Clock Interface APIs
Clock command that can be used to control the clock functionality of a device
@ingroup zwarecapi
@{
*/
/** @name Day_of_week
*  Day of week definition
@{
*/
#define ZW_CLOCK_UNDEFINED          0       /**< Undefined */
#define ZW_CLOCK_MONDAY             1       /**< Monday */
#define ZW_CLOCK_TUESDAY            2       /**< Tuesday */
#define ZW_CLOCK_WEDNESDAY          3       /**< Wednesday */
#define ZW_CLOCK_THURSDAY           4       /**< Thursday */
#define ZW_CLOCK_FRIDAY             5       /**< Friday */
#define ZW_CLOCK_SATURDAY           6       /**< Saturday */
#define ZW_CLOCK_SUNDAY             7       /**< Sunday */
/**
@}
*/

typedef void (*zwrep_clock_fn)(zwifd_p ifd, uint8_t weekday, uint8_t hour, uint8_t minute);
/**<
report callback for time of day
@param[in]	ifd	    interface
@param[in]	weekday	Day of week, ZW_CLOCK_XXX
@param[in]	hour	Hour (in 24 hours format)
@param[in]	minute	Minute
*/

int zwif_clock_set(zwifd_p ifd, uint8_t weekday, uint8_t hour, uint8_t minute);
/**<
set clock value
@param[in]	ifd		interface
@param[in]	weekday	Day of week, ZW_CLOCK_XXX
@param[in]	hour	Hour (in 24 hours format)
@param[in]	minute	Minute
@return	ZW_ERR_XXX
*/

int zwif_clock_rpt_set(zwifd_p ifd, zwrep_clock_fn rpt_cb);
/**<
setup a clock report callback function
@param[in]	ifd         interface
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_clock_get(zwifd_p ifd);
/**<
get clock report through report callback
@param[in]	ifd	        interface
@return		ZW_ERR_XXX
*/

/**
@}
@defgroup If_Climate_Ctl Climate Control Schedule Interface APIs
Climate Control Schedule command that can be used to control a thermostat setback operation schedule
@ingroup zwarecapi
@{
*/

/** Thermostat Climate Control Schedule Entry (Switchpoint)*/
typedef struct
{
    uint8_t     hour;       /**< Hour: 0 ~ 23*/
    uint8_t     minute;		/**< Minute: 0 ~ 59 */
    uint8_t     state;      /**< Schedule state, ZW_THRMO_SETB_STA_XXX */
    int8_t      tenth_deg;  /**< 1/10 of a degree (Kelvin).  This parameter is valid if state
                                 equals to ZW_THRMO_SETB_STA_SETB. Valid values: -128 to 120 (inclusive).
                                 i.e. setback temperature ranges from -12.8 degree K to 12 degree K.*/
}
zwcc_shed_swpt_t;

/** Thermostat Climate Control Schedule */
typedef struct
{
    uint8_t             weekday;    /**< Day of week. Valid from ZW_CLOCK_MONDAY to ZW_CLOCK_SUNDAY*/
    uint8_t             total;      /**< Total number of valid schedule entries*/
    zwcc_shed_swpt_t    swpts[9];   /**< Schedule entries (switchpoints). The entries must be ordered by time, ascending
                                         from 00:00 towards 23:59. No duplicates of time shall be allowed*/
}
zwcc_shed_t, *zwcc_shed_p;


typedef void (*zwrep_clmt_ctl_schd_fn)(zwifd_p ifd, zwcc_shed_p sched);
/**<
report callback for climate control schedule
@param[in]	ifd	    interface
@param[in]	sched   climate control schedule
*/

int zwif_clmt_ctl_schd_rpt_set(zwifd_p ifd, zwrep_clmt_ctl_schd_fn rpt_cb);
/**<
setup a climate control schedule report callback function
@param[in]	ifd         interface descriptor
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_clmt_ctl_schd_get(zwifd_p ifd, uint8_t weekday);
/**<
get the climate control schedule through report callback
@param[in]	ifd	        interface
@param[in]	weekday	    day of week, ZW_CLOCK_XXX
@return		ZW_ERR_XXX
*/

int zwif_clmt_ctl_schd_set(zwifd_p ifd, zwcc_shed_p sched);
/**<
set the climate control schedule in a device for a specific weekday
@param[in]	ifd	            interface
@param[in]	sched           climate control schedule
@return	ZW_ERR_XXX
*/

typedef void (*zwrep_clmt_ctl_schd_chg_fn)(zwifd_p ifd, uint8_t chg_counter);
/**<
report callback for climate control schedule change
@param[in]	ifd	            interface
@param[in]	chg_counter     climate control schedule change counter.If the ChangeCounter
                            is different from last time, this indicates a change in a
                            climate control schedule.
                            Value from 1 to 255 indicates the climate control schedule change mechanism is enabled.
                            Value of 0 indicates the climate control schedule change mechanism is temporarily
                            disabled by the override function.
*/

int zwif_clmt_ctl_schd_chg_rpt_set(zwifd_p ifd, zwrep_clmt_ctl_schd_chg_fn rpt_cb);
/**<
setup a climate control schedule change report callback function
@param[in]	ifd         interface descriptor
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_clmt_ctl_schd_chg_get(zwifd_p ifd);
/**<
get the climate control schedule change counter through report callback
@param[in]	ifd	        interface
@return		ZW_ERR_XXX
*/

/** Thermostat Climate Control Schedule Override*/
typedef struct
{
    uint8_t     type;		/**< Schedule override type, ZW_THRMO_SETB_TYP_XXX */
    uint8_t     state;      /**< Schedule override state, ZW_THRMO_SETB_STA_XXX */
    int8_t      tenth_deg;  /**< 1/10 of a degree (Kelvin).  This parameter is valid if state
                                 equals to ZW_THRMO_SETB_STA_SETB. Valid values: -128 to 120 (inclusive).
                                 i.e. setback temperature ranges from -12.8 degree K to 12 degree K.*/
}
zwcc_shed_ovr_t, *zwcc_shed_ovr_p;

typedef void (*zwrep_clmt_ctl_schd_ovr_fn)(zwifd_p ifd, zwcc_shed_ovr_p schd_ovr);
/**<
report callback for climate control schedule override
@param[in]	ifd	            interface
@param[in]	schd_ovr        climate control schedule override
*/

int zwif_clmt_ctl_schd_ovr_rpt_set(zwifd_p ifd, zwrep_clmt_ctl_schd_ovr_fn rpt_cb);
/**<
setup a climate control schedule override report callback function
@param[in]	ifd         interface descriptor
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_clmt_ctl_schd_ovr_get(zwifd_p ifd);
/**<
get the climate control schedule override through report callback
@param[in]	ifd	        interface
@return		ZW_ERR_XXX
*/

int zwif_clmt_ctl_schd_ovr_set(zwifd_p ifd, zwcc_shed_ovr_p schd_ovr);
/**<
set the climate control schedule override in a device
@param[in]	ifd	            interface
@param[in]	schd_ovr        climate control schedule override
@return	ZW_ERR_XXX
*/

/**
@}
@defgroup If_Protection  Protection Interface APIs
Protection command that can be used to protect a device from unauthorized control
@ingroup zwarecapi
@{
*/

/** @name Local_protect_state
*  Local Protection State definition
@{
*/
#define ZW_LPROT_UNPROT         0       /**< Unprotected - The device is not protected,
                                             and can be operated normally via the user interface. */
#define ZW_LPROT_SEQ            1       /**< Protection by sequence - The device is protected by
                                             altering the way the device normally is operated into a more
                                             complicated sequence of actions */
#define ZW_LPROT_NO_CTL         2       /**< No operation possible - It is not possible at all to control
                                             a device directly via the user interface.*/

/**
@}
*/

/** @name RF_protect_state
*  RF Protection State (version 2) definition
@{
*/
#define ZW_RFPROT_UNPROT        0       /**< Unprotected - The device must accept and respond to all RF Commands.*/
#define ZW_RFPROT_NO_CTL        1       /**< No RF control - all runtime Commands are ignored by the device.
                                             The device must still respond with status on requests.*/
#define ZW_RFPROT_NO_RESP       2       /**< No RF response at all. The device will not even reply to status requests.*/
/**
@}
*/

typedef void (*zwrep_prot_fn)(zwifd_p ifd, uint8_t local_prot, uint8_t rf_prot, time_t ts);
/**<
report callback for protection states
@param[in]	ifd	        interface
@param[in]	local_prot  local protection state, ZW_LPROT_XXX
@param[in]	rf_prot     RF protection state, ZW_RFPROT_XXX.
@param[in]	ts          time stamp.  If this is zero, the callback has no data and hence other parameter
                        values should be ignored.
*/

int zwif_prot_rpt_set(zwifd_p ifd, zwrep_prot_fn rpt_cb);
/**<
setup a protection report callback function
@param[in]	ifd         interface descriptor
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_prot_get(zwifd_p ifd, int flag);
/**<
get the protection states through report callback
@param[in]	ifd	        interface
@param[in]	flag	    flag, see ZWIF_GET_BMSK_XXX
@return		ZW_ERR_XXX
*/

int zwif_prot_set(zwifd_p ifd, uint8_t local_prot, uint8_t rf_prot);
/**<
set the protection states in the device
@param[in]	ifd	        interface
@param[in]	local_prot  local protection state, ZW_LPROT_XXX
@param[in]	rf_prot     RF protection state, ZW_RFPROT_XXX. For device
                        that supports only version 1, this field will be ignored
@return	ZW_ERR_XXX
*/


/** Supported Protection States*/
typedef struct
{
    uint8_t     excl_ctl;       /**< Flag to indicates whether the device supports Exclusive Control */
    uint8_t     tmout;          /**< Flag to indicates whether the device supports timeout for RF Protection State */
    uint8_t     lprot[16];      /**< Supported Local Protection States */
    uint8_t     lprot_len;      /**< Number of supported Local Protection States*/
    uint8_t     rfprot[16];     /**< Supported RF Protection States */
    uint8_t     rfprot_len;     /**< Number of supported RF Protection States*/

}
zwprot_sup_t, *zwprot_sup_p;


typedef void (*zwrep_prot_sup_fn)(zwifd_p ifd, zwprot_sup_p sup_sta, int valid);
/**<
report callback for supported protection states
@param[in]	ifd	        interface
@param[in]	sup_sta     supported Protection States
@param[in]	valid       validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/

int zwif_prot_sup_get(zwifd_p ifd, zwrep_prot_sup_fn cb, int cache);
/**<
get the supported protections
@param[in]	ifd	    interface
@param[in]	cb	    report callback function
@param[in]	cache	flag: to get data from cached only. If set, no fetching from real device when cache unavailable.
@return ZW_ERR_XXX
*/

int zwif_prot_sup_cache_get(zwifd_p ifd, zwprot_sup_t *sup_prot);
/**<
get supported protection states from cache
@param[in]	ifd	        interface
@param[out]	sup_prot	supported protection states
@return ZW_ERR_XXX
*/


typedef void (*zwrep_prot_ec_fn)(zwifd_p ifd, uint8_t node_id, time_t ts);
/**<
report callback for protection exclusive control node
@param[in]	ifd	        interface
@param[in]	node_id     node ID that has exclusive control can override the RF protection state
                        of the device and can control it regardless of the protection state.
                        Node id of zero is used to reset the protection exclusive control state.
@param[in]	ts          time stamp.  If this is zero, the callback has no data and hence other parameter
                        values should be ignored.
*/

int zwif_prot_ec_rpt_set(zwifd_p ifd, zwrep_prot_ec_fn rpt_cb);
/**<
setup a protection exclusive control report callback function
@param[in]	ifd         interface descriptor
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_prot_ec_set(zwifd_p ifd, uint8_t node_id);
/**<
set the protection exclusive control node in the device
@param[in]	ifd	        interface
@param[in]	node_id     node ID that has exclusive control can override the RF protection state
                        of the device and can control it regardless of the protection state.
                        Node id of zero is used to reset the protection exclusive control state.
@return	ZW_ERR_XXX
*/

int zwif_prot_ec_get(zwifd_p ifd, int flag);
/**<
get the protection exclusive control node through report callback
@param[in]	ifd	        interface
@param[in]	flag	    flag, see ZWIF_GET_BMSK_XXX
@return		ZW_ERR_XXX
*/

typedef void (*zwrep_prot_tmout_fn)(zwifd_p ifd, uint8_t remain_tm, time_t ts);
/**<
report callback for the remaining time that a device will remain in protection mode.
@param[in]	ifd	        interface
@param[in]	remain_tm   remaining time. 0x00 = No timer is set. All “normal operation” Commands must be accepted.
                        0x01 to 0x3C = 1 second (0x01) to 60 seconds (0x3C);
                        0x41 to 0xFE = 2 minutes (0x41) to 191 minutes (0xFE);
                        0xFF = No Timeout - The Device will remain in RF Protection mode infinitely.
@param[in]	ts          time stamp.  If this is zero, the callback has no data and hence other parameter
                        values should be ignored.
*/

int zwif_prot_tmout_rpt_set(zwifd_p ifd, zwrep_prot_tmout_fn rpt_cb);
/**<
setup a RF protection timeout report callback function
@param[in]	ifd         interface descriptor
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_prot_tmout_get(zwifd_p ifd, int flag);
/**<
get the remaining time that a device will remain in RF protection mode through report callback
@param[in]	ifd	        interface
@param[in]	flag	    flag, see ZWIF_GET_BMSK_XXX
@return		ZW_ERR_XXX
*/

int zwif_prot_tmout_set(zwifd_p ifd, uint8_t tmout);
/**<
set the RF protection timeout in the device
@param[in]	ifd	        interface
@param[in]	tmout	    Timeout specifies the time (in different resolutions) a device will remain in RF Protection mode.
                        0x01 to 0x3C = 1 second (0x01) to 60 seconds (0x3C);
                        0x41 to 0xFE = 2 minutes (0x41) to 191 minutes (0xFE);
                        0xFF = No Timeout - The Device will remain in RF Protection mode infinitely.
@return	ZW_ERR_XXX
*/

/**
@}
@defgroup If_Status  Status Interface APIs
Application status command that can be used to inform the outcome of a request
@ingroup zwarecapi
@{
*/

/** Application Busy Status */
#define ZW_BSY_STS_TRY          0       /**< Try again later */
#define ZW_BSY_STS_TRY_WAIT     1       /**< Try again in Wait Time seconds */
#define ZW_BSY_STS_Q            2       /**< Request queued, will be executed later */

typedef void (*zwrep_appl_busy_fn)(zwifd_p ifd, uint8_t status, uint8_t wait_tm);
/**<
report callback for application busy status
@param[in]	ifd	        interface
@param[in]	status      busy status, ZW_BSY_STS_XXX
@param[in]	wait_tm     wait time in seconds; only valid if status = ZW_BSY_STS_TRY_WAIT
*/

int zwif_appl_busy_rep_set(zwifd_p ifd, zwrep_appl_busy_fn rpt_cb);
/**<
setup an application busy report callback function
@param[in]	ifd         interface descriptor
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

/** Application Rejected Status */
#define ZW_RJ_STS_REJECT        0       /**< Supported command rejected by the application in the receiving node */

typedef void (*zwrep_appl_reject_fn)(zwifd_p ifd, uint8_t status);
/**<
report callback for application rejected request status
@param[in]	ifd	        interface
@param[in]	status      application rejected request status, ZW_RJ_STS_XXX
*/

int zwif_appl_reject_rep_set(zwifd_p ifd, zwrep_appl_reject_fn rpt_cb);
/**<
setup an application rejected request report callback function
@param[in]	ifd         interface descriptor
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/


/**
@}
@defgroup If_Indicator  Indicator Interface APIs
Indicator command that can be used to show the actual state, level etc. on a device
@ingroup zwarecapi
@{
*/

int zwif_ind_rpt_set(zwifd_p ifd, zwrep_fn rpt_cb);
/**<
setup an indicator report callback function
@param[in]	ifd         Interface descriptor
@param[in]	rpt_cb	    Report callback function
return      ZW_ERR_XXX
*/

int zwif_ind_get(zwifd_p ifd);
/**<
get indicator report through report callback
@param[in]	ifd	        interface
@return		ZW_ERR_XXX
*/

int zwif_ind_set(zwifd_p ifd, uint8_t val);
/**<
set indicator value
@param[in]	ifd		interface
@param[in]	val		value. The value can be either 0x00 (off/disable) or 0xFF (on/enable).
                    Furthermore it can take values from 1 to 99 (0x01 - 0x63).

@return	ZW_ERR_XXX
*/


/**
@}
@defgroup If_Firmware Firmware update Interface APIs
Used to update firmwares on a device
@ingroup zwarecapi
@{
*/

/** @name Firmware_upgrd_flag
*  Firmware upgradable flag definition
@{
*/
#define ZW_FW_UPGD_NO           0       /**< Firmware is not upgradable */
#define ZW_FW_UPGD_YES          0xFF    /**< Firmware is upgradable */
/**
@}
*/

#define ZW_FW_FRAG_SZ_V1_V2     40      /**< Default firmware fragment size for version 1 & 2*/

/** Firmwares information */
typedef struct
{
    uint16_t    vid;            /**< Vendor/Manufacturer id */
    uint16_t    zw_fw_id;       /**< Z-wave firmware id*/
    uint16_t    chksum;         /**< CRC-CCITT checksum of Z-wave firmware*/
    uint16_t    max_frag_sz;    /**< Maximum meta data fragment size for firmware update */
    uint8_t     fixed_frag_sz;  /**< Flag to indicate whether the max_frag_sz is fixed, i.e. firmware update
                                     request MUST use the given size. 1=fixed size; 0=variable size */
    uint8_t     upgrade_flg;    /**< Firmware upgradable flag. See ZW_FW_UPGD_XXX */
    uint8_t     hw_ver_valid;   /**< Flag to indicate whether the hw_ver is valid. 1=valid; 0=invalid */
    uint8_t     hw_ver;         /**< Hardware version */
    uint8_t     other_fw_cnt;   /**< Number of other firmware IDs*/
    uint16_t    *other_fw_id;   /**< Pointer to array of other firmware IDs with the count given by other_fw_cnt */
}
zwfw_info_t;

typedef void (*zwrep_fw_info_fn)(zwifd_p ifd, zwfw_info_t  *fw_info);
/**<
report callback for firmwares information
@param[in]	ifd	        interface
@param[in]	fw_info     firmwares information
*/

int zwif_fw_info_get(zwifd_p ifd, zwrep_fw_info_fn cb);
/**<
get firmwares information through report callback
@param[in]	ifd	        interface
@param[in]	cb	        report callback
@return		ZW_ERR_XXX
*/

/** @name Firmware_update_req_status
*  Firmware update request status definition
@{
*/
#define ZW_FW_UPDT_ERR_INVALID      0       /**< Invalid combination of vendor id and firmware id */
#define ZW_FW_UPDT_ERR_AUTHEN       0x01    /**< Need out-of-band authentication event to enable firmware update */
#define ZW_FW_UPDT_ERR_FRAG_SZ      0x02    /**< The requested Fragment Size exceeds the Max Fragment Size */
#define ZW_FW_UPDT_ERR_UPGRD        0x03    /**< This firmware target is not upgradable */
#define ZW_FW_UPDT_ERR_HW_VER       0x04    /**< Invalid hardware version */
#define ZW_FW_UPDT_ERR_VALID        0xFF    /**< Valid combination of vendor id and firmware id; the device will
                                                 start to request firmware data from the requester*/
/**
@}
*/

/** @name Firmware_update_completion_status
*  Firmware update completion status definition
@{
*/
#define ZW_FW_UPDT_CMPLT_ERR_CHKS       0       /**< Cheksum error in requested firmware */
#define ZW_FW_UPDT_CMPLT_ERR_DOWNLOAD   0x01    /**< Download of the requested firmware failed */
#define ZW_FW_UPDT_CMPLT_ERR_VID        0x02    /**< Vendor/Manufacturer ID mismatched */
#define ZW_FW_UPDT_CMPLT_ERR_FW_ID      0x03    /**< Firmware ID mismatched */
#define ZW_FW_UPDT_CMPLT_ERR_FW_TGT     0x04    /**< Firmware target mismatched */
#define ZW_FW_UPDT_CMPLT_ERR_FILE_HDR   0x05    /**< Invalid file header information */
#define ZW_FW_UPDT_CMPLT_ERR_FILE_H_FMT 0x06    /**< Invalid file header format */
#define ZW_FW_UPDT_CMPLT_ERR_MEM        0x07    /**< Out of memory */
#define ZW_FW_UPDT_CMPLT_ERR_HW_VER     0x08    /**< Hardware version mismatched */
#define ZW_FW_UPDT_CMPLT_OK_WAIT        0xFD    /**< Image downloaded, waiting for activation command */
#define ZW_FW_UPDT_CMPLT_OK_NO_RESTART  0xFE    /**< New image was successfully stored in temporary non-volatile memory.
                                                     The device does not restart itself. */
#define ZW_FW_UPDT_CMPLT_OK_RESTART     0xFF    /**< New image was successfully stored in temporary non-volatile memory.
                                                     The device will now start storing the new image in primary non-volatile
                                                     memory dedicated to executable code. Then the device will restart itself.*/
/**
@}
*/

/** @name Firmware_update_target_restart_status
*  Firmware update target restart status definition
@{
*/
#define ZW_FW_UPDT_RESTART_OK       0       /**< Restart o.k. */
#define ZW_FW_UPDT_RESTART_FAILED   0x01    /**< Restart failed due to no response from the target. */
/**
@}
*/

typedef void (*zwrep_fw_updt_sts_fn)(zwifd_p ifd, uint8_t status);
/**<
report callback for firmware update status
@param[in]	ifd	        interface
@param[in]	status      firmware update status, ZW_FW_UPDT_ERR_XXX
*/

typedef void (*zwrep_fw_updt_cmplt_fn)(zwifd_p ifd, uint8_t status, uint16_t wait_tm, int wait_tm_valid);
/**<
report callback for firmware update completion status
@param[in]	ifd	            interface
@param[in]	status          firmware update completion status, ZW_FW_UPDT_CMPLT_XXX
@param[in]	wait_tm         time (in seconds) that is needed before the receiving node becomes
                            available again for communication after the transfer of an image. This parameter is
                            valid only if wait_tm_valid=1
@param[in]	wait_tm_valid   flag to indicate the wait_tm parameter is valid.
*/

typedef void (*zwrep_fw_tgt_restart_fn)(zwnoded_p node, uint8_t status);
/**<
report callback for firmware update target restart status
@param[in]	node    node
@param[in]	status  firmware update target restart status, ZW_FW_UPDT_RESTART_XXX
*/

/** Firmware update request */
typedef struct
{
    uint16_t                vid;            /**< Vendor/Manufacturer id the firmware is intended for */
    uint16_t                fw_id;          /**< Firmware id the firmware is intended for */
    uint8_t                 hw_ver;         /**< Hardware version the firmware is intended for; zero if inapplicable */
    uint8_t                 fw_tgt;         /**< Firmware target to update. 0= Z-wave firmware, 0x01 to 0xFF for
                                                 firmware target returned by zwif_fw_info_get(). For example,
                                                 to update firmware target in other_fw_id[0], use fw_tgt=1. */
    const char              *fw_file;       /**< Firmware file path. For Intel hex file format, the extension must be ".hex",
                                                 other extension will be treated as binary file. */
    zwrep_fw_updt_sts_fn    sts_cb;         /**< Firmware update request status callback function. See ZW_FW_UPDT_ERR_XXX */
    zwrep_fw_updt_cmplt_fn  cmplt_cb;       /**< Firmware update request completion status callback function.
                                                 See ZW_FW_UPDT_CMPLT_XXX. This function will only be invoked if update
                                                 request status callback sts_cb() reported ZW_FW_UPDT_ERR_VALID*/
    zwrep_fw_tgt_restart_fn restart_cb;     /**< Optional: Firmware update target restart completion callback function.
                                                 Can be NULL if callback is not required.  Note that the callback function
                                                 works only if the cmplt_cb() invoked with parameter wait_tm_valid=1*/
}
zwfw_updt_req_t;

int zwif_fw_updt_req(zwifd_p ifd, zwfw_updt_req_t *req);
/**<
request firmware update
@param[in]	ifd	        interface
@param[in]	req	        firmware update request
@return	ZW_ERR_XXX
@note Caller should call zwif_fw_info_get() first before calling this function.
*/


/**
@}
@defgroup If_ZIP_Gateway Configure ZIP Gateway Interface APIs
Used to configure Z/IP gateway into either stand-alone or portal mode
@ingroup zwarecapi
@{
*/

/** Z/IP gateway operating mode */
#define ZW_GW_STAND_ALONE   0x01    /**< Stand alone mode as Z/IP gateway */
#define ZW_GW_PORTAL        0x02    /**< Portal mode with its configuration pushed from a portal server */

/** Portal profile */
typedef struct
{
    uint16_t                port;           /**< Port number that the portal is listening on */
    uint8_t                 addr6[16];      /**< Full IPv6 address with no compression. The address SHOULD be in the
                                                 ULA IPv6 prefix or in a globally routable IPv6 prefix. The address MAY
                                                 be an IPv4-mapped IPv6 address. The field MUST NOT carry a link-local
                                                 IPv6 address. The IPv6 address MAY be specified as ::/128 (all zeros),
                                                 i.e. the unspecified address. If setting the IPv6 address field to the
                                                 unspecified IPv6 address, the portal_name field MUST be set to a
                                                 DNS-resolvable FQDN. */
    uint8_t                 portal_name[64];/**< Optional portal name field MUST be formatted as a UTF-8 based
                                                 FQDN string such as “example.com” without null terminated character */
    uint8_t                 name_len;       /**< portal_name length. Valid value: 0 to 63*/
}
zwgw_portal_prof_t;


int zwif_gw_mode_set(zwifd_p ifd, uint8_t mode, zwgw_portal_prof_t *portal_profile);
/**<
configure Z/IP gateway operating mode
@param[in]	ifd	            interface
@param[in]	mode	        operating mode; ZW_GW_XXX
@param[in]	portal_profile	portal profile; must be valid if mode is ZW_GW_PORTAL
@return	ZW_ERR_XXX
*/

typedef void (*zwrep_gw_mode_fn)(zwifd_p ifd, uint8_t status, uint8_t mode, zwgw_portal_prof_t *portal_profile);
/**<
report callback for gateway operating mode
@param[in]	ifd	            interface
@param[in]	status          0=success; 1=failure due to either timeout or the gateway is locked with its parameter hidden.
                            2=transmit error.
@param[in]	mode	        operating mode; ZW_GW_XXX
@param[in]	portal_profile	portal profile; only valid if mode is ZW_GW_PORTAL. If it is NULL, no profile is
                            stored in the Z/IP gateway.
*/

int zwif_gw_mode_get(zwifd_p ifd, zwrep_gw_mode_fn cb);
/**<
get Z/IP gateway operating mode through report callback
@param[in]	ifd	        interface
@param[in]	cb	        report callback
@return		ZW_ERR_XXX
*/

int zwif_gw_cfg_lock(zwifd_p ifd, uint8_t lock, uint8_t show);
/**<
lock Z/IP gateway configuration parameters
@param[in]	ifd	  interface
@param[in]	lock  lock configuration parameters. 1= lock; 0= unlock.
@param[in]	show  control whether to allow configuration parameters to be read back. 1= allow read back; 0= disallow
@return	ZW_ERR_XXX
@note    Once the Z/IP Gateway has been locked, it MUST NOT be possible to unlock the device unless :-
         *A factory default reset unlock the Z/IP Gateway and revert all settings to default
         *An unlock command received via an authenticated secure connection with the portal
*/

int zwif_gw_unsolicit_set(zwifd_p ifd, uint8_t *dst_ip, uint16_t dst_port);
/**<
configure Z/IP gateway unsolicited message destination
@param[in]	ifd	            interface
@param[in]	dst_ip	        unsolicited destination IPv6 address
@param[in]	dst_port	    unsolicited destination port
@return	ZW_ERR_XXX
*/

typedef void (*zwrep_gw_unsolicit_fn)(zwifd_p ifd, uint8_t *dst_ip, uint16_t dst_port);
/**<
report callback for gateway unsolicited message destination
@param[in]	ifd	            interface
@param[in]	dst_ip	        unsolicited destination IPv6 address
@param[in]	dst_port	    unsolicited destination port
*/

int zwif_gw_unsolicit_get(zwifd_p ifd, zwrep_gw_unsolicit_fn cb);
/**<
get Z/IP gateway unsolicited message destination through report callback
@param[in]	ifd	        interface
@param[in]	cb	        report callback
@return		ZW_ERR_XXX
*/

#ifdef SUPPORT_GW_WIFI
/** Wifi Configuration */
typedef struct
{
    uint8_t         ssid_len;               /**< SSID length (maximum 32 bytes) */
    uint8_t         key_len;                /**< Key/password length (maximum 64 bytes) */
    uint8_t         sec_type;               /**< Wifi security types (0 to 7) */
    uint8_t         wep_idx;                /**< WEP key index (1 to 4). Valid if sec_type is OPEN_SYSTEM_WEP64 or OPEN_SYSTEM_WEP128.*/
    uint8_t         ssid[32];               /**< SSID */
    uint8_t         key[64];                /**< Key/password */
}
zw_wifi_cfg_t;

/** @name Wifi_security_type
*  Wifi security types definition
@{
*/
#define NO_ENCRYPTION             0    ///< No encryption
#define OPEN_SYSTEM_WEP64         1    ///< WEP 64
#define OPEN_SYSTEM_WEP128        2    ///< WEP 128
#define WPA_WPA2_PSK_MIXED_TKIP   3    ///< WPA/WPA2 PSK and TKIP
#define WPA_WPA2_PSK_MIXED_AES    4    ///< WPA/WPA2 PSK and AES
#define WPA_WPA2_PSK_MIXED_AUTO   5    ///< WPA/WPA2 PSK Auto
#define WIFI_SECURE_TYPE_UNKNOWN   (WPA_WPA2_PSK_MIXED_AUTO + 1)    ///< Unknown security type
/**
@}
*/

typedef void (*zwrep_gw_wifi_cfg_fn)(zwifd_p ifd, zw_wifi_cfg_t *cfg);
/**<
report callback for gateway wifi configuration
@param[in]	ifd     interface
@param[in]	cfg	    configuration
*/

typedef void (*zwrep_gw_wifi_sec_fn)(zwifd_p ifd, uint8_t *sec_type, uint8_t type_cnt, int valid);
/**<
report callback for gateway wifi supported security types
@param[in]	ifd         interface
@param[in]	sec_type	buffer to store the supported security types
@param[in]	type_cnt	number of supported security types
@param[in]	valid       validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/

int zwif_gw_wifi_cfg_set(zwifd_p ifd, zw_wifi_cfg_t *cfg);
/**<
set Z/IP gateway wifi configuration
@param[in]	ifd interface
@param[in]	cfg	configuration parameters
@return	ZW_ERR_XXX
*/

int zwif_gw_wifi_cfg_get(zwifd_p ifd, zwrep_gw_wifi_cfg_fn cb);
/**<
get Z/IP gateway wifi configuration through report callback
@param[in]	ifd	        interface
@param[in]	cb	        report callback
@return		ZW_ERR_XXX
*/

int zwif_gw_wifi_sec_sup_get(zwifd_p ifd, zwrep_gw_wifi_sec_fn cb, int cache);
/**<
get Z/IP gateway wifi supported security types through report callback
@param[in]	ifd	        interface
@param[in]	cb	        report callback
@param[in]	cache	    flag: to get data from cached only. If set, no fetching from real device when cache unavailable.
@return		ZW_ERR_XXX
*/
#endif

/**
@}
@defgroup If_Poll Polling Interface APIs
Used to create and delete polling commands to a device
@ingroup zwarecapi
@{
*/

typedef void (*zwpoll_cmplt_fn)(zwnet_p net, uint16_t handle, uint32_t usr_token, void *usr_param);
/**<
callback on polling completion
@param[in]	net	        network
@param[in]	handle	    handle of the polling request
@param[in]	usr_token	user defined token to facilitate deletion of multiple polling requests
@param[in]	usr_param	user parameter
*/

/** Polling request */
typedef struct
{   /* Input*/
    uint32_t                usr_token;      /**< User defined token to facilitate deletion of multiple polling requests */
    uint16_t                interval;       /**< Polling interval in seconds; zero = the smallest possible interval*/
    uint16_t                poll_cnt;       /**< Number of times to poll; zero = unlimited times (i.e. repetitive polling)*/
    zwpoll_cmplt_fn         cmplt_cb;       /**< Polling completion callback. NULL if callback is not required*/
    void                    *usr_param;     /**< User parameter of polling completion callback */

    /* Output*/
    uint16_t                handle;         /**< Handle if the request is accepted into the polling queue. The handle
                                                 can be used to facilitate deletion of the polling request */
}
zwpoll_req_t;

int zwnet_poll_rm(zwnet_p net, uint16_t handle);
/**<
remove a polling request
@param[in]	net	        network
@param[in]	handle	    handle of the polling request to remove
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

int zwnet_poll_rm_mul(zwnet_p net, uint32_t usr_token);
/**<
remove multiple polling requests
@param[in]	net	        network
@param[in]	usr_token	usr_token of the polling requests to remove
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

int zwif_alrm_get_poll(zwifd_p ifd, int16_t vtype, uint8_t ztype, uint8_t evt, zwpoll_req_t *poll_req);
/**<
get the state of the alarm device (push mode) or the pending notification (pull mode) through report callback
@param[in]	ifd	        interface
@param[in]	vtype	    vendor specific alarm type. Zero if this field is not used; -1 to indicate "don't care" for cache get.
@param[in]	ztype	    Z-wave alarm type (ZW_ALRM_XXX). Zero if this field is not used; 0xFF = select a supported Notification Type (push mode),
                        or retrieve the first alarm detection (pull mode).
@param[in]	evt	        Event corresponding to Z-wave alarm type. Zero if this field is not used. This parameter is valid for push mode only
@param[in, out] poll_req Poll request
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

int zwif_basic_get_poll(zwifd_p ifd, zwpoll_req_t *poll_req);
/**<
get basic report through report callback
@param[in]	ifd	        interface
@param[in, out] poll_req Poll request
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

int zwif_battery_get_poll(zwifd_p ifd, zwpoll_req_t *poll_req);
/**<
get battery report through report callback
@param[in]	ifd	        interface
@param[in, out] poll_req Poll request
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

int zwif_bsensor_get_poll(zwifd_p ifd, uint8_t type, zwpoll_req_t *poll_req);
/**<
get binary sensor report through report callback
@param[in]	ifd	        interface
@param[in]	type	    preferred sensor type, ZW_BSENSOR_TYPE_XXX. If type equals to zero, the
                        sensor report will return the factory default sensor type.
@param[in, out] poll_req Poll request
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

int zwif_clmt_ctl_schd_chg_get_poll(zwifd_p ifd, zwpoll_req_t *poll_req);
/**<
get the climate control schedule change counter through report callback
@param[in]	ifd	        interface
@param[in, out] poll_req Poll request
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

int zwif_dlck_op_get_poll(zwifd_p ifd, zwpoll_req_t *poll_req);
/**<
get the state of the door lock device through report callback
@param[in]	ifd	        interface
@param[in, out] poll_req Poll request
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

int zwif_ind_get_poll(zwifd_p ifd, zwpoll_req_t *poll_req);
/**<
get indicator report through report callback
@param[in]	ifd	        interface
@param[in, out] poll_req Poll request
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

int zwif_level_get_poll(zwifd_p ifd, zwpoll_req_t *poll_req);
/**<
get level report through report callback
@param[in]	ifd	        interface
@param[in, out] poll_req Poll request
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

int zwif_color_sw_get_poll(zwifd_p ifd, uint8_t id, zwpoll_req_t *poll_req);
/**<
get color switch value report through report callback
@param[in]	    ifd	        interface
@param[in]	    id	        color component id, COL_SW_COMP_ID_XXX
@param[in, out] poll_req    Poll request
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

int zwif_meter_get_poll(zwifd_p ifd, uint8_t unit, zwpoll_req_t *poll_req);
/**<
get meter report through report callback
@param[in]	ifd	        interface
@param[in]	unit	    the preferred unit (ZW_METER_UNIT_XXX). The report may not return
                        the preferred unit if the device doesn't support it.
@param[in, out] poll_req Poll request
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

int zwif_prot_get_poll(zwifd_p ifd, zwpoll_req_t *poll_req);
/**<
get the protection states through report callback
@param[in]	ifd	        interface
@param[in, out] poll_req Poll request
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

int zwif_sensor_get_poll(zwifd_p ifd, uint8_t type, uint8_t unit, zwpoll_req_t *poll_req);
/**<
get multilevel sensor report through report callback
@param[in]	ifd	        interface
@param[in]	type	    preferred sensor type, ZW_SENSOR_TYPE_XXX. If type equals to zero, the
                        sensor report will return the factory default sensor type.
@param[in]	unit	    preferred sensor unit, ZW_SENSOR_UNIT_XXX.  This parameter is ignored
                        if type=0.
@param[in, out] poll_req Poll request
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
@note  Preferred sensor type and unit are not guaranteed to be returned in the report callback.  It
       depends on the multilevel sensor command class version number and the device supported.
*/

int zwif_switch_get_poll(zwifd_p ifd, zwpoll_req_t *poll_req);
/**<
get switch report through report callback
@param[in]	ifd	        interface
@param[in, out] poll_req Poll request
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

int zwif_thrmo_setp_get_poll(zwifd_p ifd, uint8_t type, zwpoll_req_t *poll_req);
/**<
get the thermostat setpoint through report callback
@param[in]	ifd	            interface
@param[in]	type	        setpoint type, ZW_THRMO_SETP_TYP_XXX
@param[in, out] poll_req    Poll request
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

int zwif_alrm_snsr_get_poll(zwifd_p ifd, uint8_t type, zwpoll_req_t *poll_req);
/**<
get alarm sensor report through report callback
@param[in]	ifd	        interface
@param[in]	type	    preferred alarm sensor type. ZW_ALRM_SNSR_TYPE_XXX. If type equals to 0xFF, the alarm
sensor report will return the first supported sensor type report.
@param[in, out] poll_req Poll request
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

int zwif_barrier_get_poll(zwifd_p ifd, zwpoll_req_t *poll_req);
/**<
get barrier operator state through report callback
@param[in]	    ifd	        interface
@param[in, out] poll_req    Poll request
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

/**
@}
@defgroup If_ZIP_Portal Configure ZIP Portal Interface APIs
Used to configure Z/IP gateway operating in portal mode
@ingroup zwarecapi
@{
*/

/** Portal Configuration */
typedef struct
{
    uint8_t         lan_ipv6_addr[16];      /**< LAN IPv6 address of Z/IP gateway.
                                                 May be all-zeroes IPv6 address for auto-configuration */
    uint8_t         dflt_gw[16];            /**< Default IPv6 gateway */
    uint8_t         pan_prefix[16];         /**< PAN interface prefix with /64 prefix length.
                                                 May be all-zeroes IPv6 address for auto-configuration */
    uint8_t         portal_ipv6_prefix[16]; /**< Portal IPv6 address. The Z/IP gateway must route all IP traffic
                                                 for the portal ipv6 prefix into the secure TLS connection */
    uint8_t         lan_ipv6_prefix_len;    /**< LAN IPv6 address prefix length */
    uint8_t         portal_ipv6_prefix_len; /**< Portal IPv6 prefix length */
}
zwportal_cfg_t;


typedef void (*zwrep_cfg_sts_fn)(zwifd_p ifd, uint8_t sts);
/**<
gateway configuration status callback
@param[in]	ifd	    interface
@param[in]	sts	    configuration status. 0xFF = o.k.; 0x01 = invalid configuration block.
*/

typedef void (*zwrep_gw_cfg_fn)(zwifd_p ifd, zwportal_cfg_t *cfg);
/**<
report callback for gateway portal mode configuration
@param[in]	ifd     interface
@param[in]	cfg	    configuration parameters
*/

int zwif_gw_cfg_set(zwifd_p ifd, zwportal_cfg_t *cfg, zwrep_cfg_sts_fn cb);
/**<
set Z/IP gateway portal mode configuration
@param[in]	ifd interface
@param[in]	cfg	configuration parameters
@param[in]	cb	configuration status callback function
@return	ZW_ERR_XXX
*/

int zwif_gw_cfg_get(zwifd_p ifd, zwrep_gw_cfg_fn cb);
/**<
get Z/IP gateway portal mode configuration through report callback
@param[in]	ifd interface
@param[in]	cb	report callback
@return		ZW_ERR_XXX
*/

/**
@}
@defgroup If_Power_level Power level Interface APIs
Used to control power level and test power level on target device
@ingroup zwarecapi
@{
*/

typedef void (*zwrep_power_level_fn)(zwifd_p ifd, uint8_t lvl, uint8_t timeout);
/**<
report callback for power level get
@param[in]	ifd	    interface
@param[in]	lvl     Current power level indicator value in effect on the node. Ranges
                    from 0 to 9. 0=normal power; 1= -1dbm; 2= -2dbm, etc.
@param[in]	timeout Time out value, in seconds.
*/

int zwif_power_level_rpt_set(zwifd_p ifd, zwrep_power_level_fn rpt_cb);
/**<
setup power level report callback function
@param[in]	ifd         interface
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_power_level_get(zwifd_p ifd);
/**<
get the power level value in use by the node through report callback
@param[in]	ifd	        interface
@return		ZW_ERR_XXX
*/

int zwif_power_level_set(zwifd_p ifd, uint8_t lvl, uint8_t timeout);
/**<
set the power level which should be used by the node when transmitting RF
@param[in]	ifd	      interface
@param[in]	lvl		  Power level. Ranges from 0 to 9. 0=normal power; 1= -1dbm; 2= -2dbm, etc.
@param[in]	timeout   Time out value (in seconds) ranges from 1-255 before resetting to normal power level.
@return	ZW_ERR_XXX
*/

/** @name Power_test_status
*  Power level test status definition
@{
*/
#define POWERLEVEL_TEST_FAILED      0x00    /**< No test frame transmissions has been acknowledged*/
#define POWERLEVEL_TEST_SUCCES      0x01    /**< At least 1 test frame transmission has been acknowledged*/
#define POWERLEVEL_TEST_INPROGRESS  0x02    /**< Test is still in progress*/
/**
@}
*/

typedef void (*zwrep_power_level_test_fn)(zwifd_p ifd, uint8_t node_id, uint8_t status, uint16_t frame_cnt);
/**<
report callback for power level test
@param[in]	ifd	        interface
@param[in]	node_id     Test node ID. If node id is 0, it means no test has been made and the rest of the
                        parameters should be ignored.
@param[in]	status      Status of the test operation. POWERLEVEL_TEST_XXX
@param[in]	frame_cnt   Number of frame count which has been acknowledged by the node as specified by node_id.
*/

int zwif_power_level_test_rpt_set(zwifd_p ifd, zwrep_power_level_test_fn rpt_cb);
/**<
setup power level test report callback function
@param[in]	ifd         interface
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_power_level_test_get(zwifd_p ifd);
/**<
get the result of power level test through report callback
@param[in]	ifd	        interface
@return		ZW_ERR_XXX
*/

int zwif_power_level_test_set(zwifd_p ifd, zwnoded_p noded, uint8_t lvl, uint16_t frame_cnt);
/**<
set the power level test information and start the test
@param[in]	ifd			interface
@param[in]	noded		Node descriptor of the test node which should recieves the transmitted test frames.
@param[in]	lvl		    Power level. Ranges from 0 to 9. 0=normal power; 1= -1dbm; 2= -2dbm, etc.
@param[in]	frame_cnt   Test frame count to be carried out. (1-65535)
@return	ZW_ERR_XXX
*/

/**
@}
@defgroup DevCfg Z-wave Device Configuration Functions
Some device configuration functions.
@ingroup zwarecapi
@{
*/

int zwdev_cfg_load(const char *cfg_file, dev_rec_t **records, uint16_t *record_cnt,
                   dev_global_sett_t **global_sett, uint16_t *global_sett_cnt, dev_cfg_error_t *err_loc);
/**<
load and store device specific configurations and device global settings
@param[in]	cfg_file	    Device specific configurations file in JSON format
@param[out]	records	        Device specific configuration records sorted according to manufacturer id,
                            product type id and product id
@param[out]	record_cnt	    Number of device specific configuration records stored in "records"
@param[out]	global_sett	    Device global settings
@param[out]	global_sett_cnt	Number of entries in device global settings
@param[out]	err_loc	        Parse error location for "device_records" in JSON file
@return		ZW_ERR_xxx
@post       Caller must free the memory allocated to "records" using zwdev_cfg_free() and memory allocated to "global_sett"
            using zwdev_global_sett_free() if return value is ZW_ERR_NONE

*/

void zwdev_cfg_free(dev_rec_t *records, int record_cnt);
/**<
free device specific configuration records
@param[in]	records	    Device specific configuration records
@param[in]	record_cnt	Number of records stored in "records" array
@return
*/

void zwdev_global_sett_free(dev_global_sett_t *global_sett, uint16_t global_sett_cnt);
/**<
free device global settings
@param[in]	global_sett	    Device setting records
@param[in]	global_sett_cnt	Number of records stored in "global_sett" array
@return
*/

int zwdev_cfg_find(dev_rec_srch_key_t *srch_key, const dev_rec_t *records, int record_cnt, dev_rec_t *matched_rec);
/**<
search for a match in device specific configuration records
@param[in]	srch_key	Search key
@param[in]	records	    Device specific configuration records sorted according to manufacturer id,
                        product type id and product id
@param[in]	record_cnt	Number of records stored in "records" array
@param[out]	matched_rec	The matched record; either exact match or partial match as explained in the note below
@return     Non-zero if a match is found; else returns zero
@note       This function support "don't care" cases in device specific configuration records.
            The search priority is as follows (in the format (Manf id, Product Type, Product id))
            : (V, V, V), (V, V, X), (V, X, X), (X, X, X) where V="valid value"; X="don't care".
*/

/**
@}
@defgroup If_Cache Get interface cached data APIs
Used to get interface cached data and/or live report data
@ingroup zwarecapi
@{
*/
#define ZWIF_GET_BMSK_CACHE     0x01    /**< Get cached data. */
#define ZWIF_GET_BMSK_LIVE      0x02    /**< Get live report data by invoking the zwif_XXX_get command */

/**
@}
@defgroup NW_Pref_storage Preference storage APIs
Used to store and retrieve preferences
@ingroup zwarecapi
@{
*/

int zwnet_pref_set(zwnet_p net, void *buf, uint16_t buf_size);
/**<
store network preference into persistent storage
@param[in]	net		    Network
@param[in]	buf		    Buffer that contains the network preference
@param[in]	buf_size	Buffer size in bytes
@return		ZW_ERR_XXX
@see		zwnet_pref_get
*/

int zwnet_pref_get(zwnet_p net, void **buf, uint16_t *buf_size);
/**<
retrieve network preference from persistent storage
@param[in]	net		    Network
@param[out]	buf		    Return buffer that contains the network preference
@param[out]	buf_size	Buffer size in bytes
@return		ZW_ERR_XXX
@post       Caller MUST free the return buffer "buf" if the call is successful.
@see		zwnet_pref_set
*/

int zwnet_client_pref_set(zwnet_p net, uint32_t client_id, void *buf, uint16_t buf_size);
/**<
store client preference into persistent storage
@param[in]	net		    Network
@param[in]	client_id	User-defined client id to identify the preference
@param[in]	buf		    Buffer that contains the client preference
@param[in]	buf_size	Buffer size in bytes
@return		ZW_ERR_XXX
@see		zwnet_client_pref_get
*/

int zwnet_client_pref_get(zwnet_p net, uint32_t client_id, void **buf, uint16_t *buf_size);
/**<
retrieve client preference from persistent storage
@param[in]	net		    Network
@param[in]	client_id	User-defined client id to identify the preference
@param[out]	buf		    Return buffer that contains the client preference
@param[out]	buf_size	Buffer size in bytes
@return		ZW_ERR_XXX
@post       Caller MUST free the return buffer "buf" if the call is successful.
@see		zwnet_client_pref_set
*/

/**
@}
@defgroup Csc Central Scene Interface APIs
Central Scene notification includes sequence number, scene number and key attributes.
Central Scene notification is only available through unsolicited report. There is no get command
to retrieve this report.
@ingroup zwarecapi
@{
*/

/** Central Scene Notification */
typedef struct
{
	uint8_t     seqNo;          /**< Sequence number. Incremented each time a new report is issued by the device. */
	uint8_t     keyAttr;        /**< Key attribute. ZW_CSC_KEY_ATTRIB_XXX.*/
	uint8_t     sceneNo;        /**< Scene Number. Actual scene identifier.*/
    uint8_t     slow_rfsh;      /**< Slow refresh of "Key Held Down" notification. Non-zero=enable; 0=disable.
                                     If disabled:
                                     A new "Key Held Down" notification MUST be sent by CSC every 200ms until the key is released.
                                     If not receiving a new "Key Held Down" notification within 400ms after the most recent
                                     "Key Held Down" notification, a "Key Release" notification must be assumed.

                                     If enabled:
                                     A new "Key Held Down" notification MUST be sent by CSC every 55 seconds until the key is released.
                                     If not receiving a new "Key Held Down" notification within 60 seconds after the most recent
                                     "Key Held Down" notification, a "Key Release" notification must be assumed.
                                */
}
zwcsc_notif_t, *zwcsc_notif_p;

typedef void(*zwrep_csc_fn)(zwifd_p ifd, zwcsc_notif_p data, time_t ts);
/**<
report callback for central scene
@param[in]	ifd	    interface
@param[in]	data	Central scene notification data.
@param[in]	ts		time stamp.  If this is zero, the callback has no data and hence other parameter
                    values should be ignored.
*/

int zwif_csc_rpt_set(zwifd_p ifd, zwrep_csc_fn rpt_cb);
/**<
setup a central scene notification report callback function
@param[in]	ifd         interface
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

typedef void(*zwrep_csc_sup_fn)(zwifd_p ifd, uint8_t scene_cnt, uint8_t sameKA, uint8_t KA_array_len, uint8_t *KA_array, uint8_t slow_rfsh, int valid);
/**<
report callback for max number of supported scenes and the key attributes supported per scene
@param[in]	ifd	       interface
@param[in]	scene_cnt  maximum number of supported scenes.
@param[in]	sameKA	   it indicates if all scenes are supporting the same Key Attributes. 1=same, 0=different
@param[in]	KA_array_len    length/width of key attribute array. For v1 COMMAND_CLASS_CENTRAL_SCENE, KA_array_len will be 0.
@param[in]	KA_array   key attribute array. It contains all the supported key attributes for each scene. \n
					   When KA_array_len is 0, KA_array should be ignored.\n
					   If sameKA is 1, KA_array is a one-dimensional array with length indicated by 'KA_array_len'.\n
					   All the scenes support the same set of Key Attribute.\n
					   For each row of the array, the first element is the number of valid key attributes given in this row.
					   If a particular scene does not support any key attribute, the first element for that row will be 0.\n
					   If sameKA is 0, KA_array is a two-dimensional array with size KA_array_len x scene_cnt.\n
					   For each row of the array, the first element is the number of valid key attributes given in this row.
					   If a particular scene does not support any key attribute, the first element for that row will be 0.\n
@param[in]	slow_rfsh  status for slow refresh of Key Held Down notification. Non-zero=enable; 0=disable
@param[in]	valid      validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
@remarks  KA Array returned in the following format:
 v1 interface -- sameKA 1, scene_cnt 5, KA_array_len 0, KA Array is NULL	\n
 V2 interface or above --													\n
 eg 1.																		\n
 sameKA 1, scene_cnt 5, KA_array_len 4										\n
  KA_array: [3] [0] [1] [2] 												\n
																			\n
 eg 2.																		\n
 sameKA 0, scene_cnt 5, KA_array_len 4										\n
 KA_array:  [3] [0] [1] [2]													\n
			[2] [0] [5] [X]		//Only have 2 valid KA in this row			\n
			[0] [X] [X] [X]		//No valid KA in this row					\n
			[3] [0] [3] [5]													\n
			[1] [1] [X] [X]		//Only have 1 valid KA in this row			\n
*/

int zwif_csc_sup_get(zwifd_p ifd, zwrep_csc_sup_fn cb, int cache);
/**<
get the max number of supported scenes and the key attributes supported per scene through report callback
@param[in]	ifd	    interface
@param[in]	cb	    report callback function
@param[in]	cache	flag: to get data from cached only. If set, no fetching from real device when cache unavailable.
@return ZW_ERR_XXX
*/

typedef void(*zwrep_csc_cfg_fn)(zwifd_p ifd, int slow_refresh, time_t ts);
/**<
report callback for central scene configuration of optional node capabilities for scene notifications
@param[in]	ifd	            interface
@param[in]	slow_refresh	status for slow refresh of Key Held Down notification. Non-zero=enable; 0=disable
@param[in]	ts		        time stamp of when the report is received
*/

int zwif_csc_cfg_rpt_set(zwifd_p ifd, zwrep_csc_cfg_fn rpt_cb);
/**<
setup configuration for scene configuration report callback function
@param[in]	ifd         Interface descriptor
@param[in]	rpt_cb	    Report callback function
return      ZW_ERR_XXX
*/

int zwif_csc_cfg_get(zwifd_p ifd);
/**<
get configuration for scene notifications report through report callback
@param[in]	ifd	        interface
@return		ZW_ERR_XXX
*/

int zwif_csc_cfg_set(zwifd_p ifd, int slow_refresh);
/**<
configure the use of slow refresh
@param[in]	ifd		        interface
@param[in]	slow_refresh	flag to enable slow refresh of Key Held Down notification. Non-zero=enable; 0=disable
@return	ZW_ERR_XXX
*/


/**
@}
@defgroup AlrmSnsr Alarm Sensor Interface APIs
Alarm sensors state can be no alarm, alarm, or alarm severity represented in percentage
@ingroup zwarecapi
@{
*/

/** Alarm sensor data */
typedef struct
{
	uint8_t     src_node_id;/**< Source node ID with the alarm condition. Not valid in Zensor Net. */
	uint8_t     type;		/**< Alarm sensor types. See ZW_ALRM_SNSR_TYPE_xx */
	uint8_t     state;	    /**< sensor state: 0=no alarm; 0xFF=alarm; 1-99: alarm severity in percentage  */
	uint16_t	activetime;	/**< Alarm active time since last received report in seconds. Zero means
                                 this field must be ignored  */
}
zw_alrm_snsr_t, *zw_alrm_snsr_p;

typedef void(*zwrep_alrm_snsr_fn)(zwifd_p ifd, zw_alrm_snsr_p data, time_t ts);
/**<
report callback for alarm sensor
@param[in]	ifd	    interface
@param[in]	data	Alarm sensor report data
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
values should be ignored.
*/

int zwif_alrm_snsr_rpt_set(zwifd_p ifd, zwrep_alrm_snsr_fn rpt_cb);
/**<
setup an alarm sensor report callback function
@param[in]	ifd         interface
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_alrm_snsr_get(zwifd_p ifd, uint8_t type, int flag);
/**<
get alarm sensor report through report callback
@param[in]	ifd	        interface
@param[in]	type	    preferred alarm sensor type. ZW_ALRM_SNSR_TYPE_XXX. If type equals to 0xFF, the alarm
sensor report will return the first supported sensor type report.
@param[in]	flag	    flag, see ZWIF_GET_BMSK_XXX
@return		ZW_ERR_XXX
*/

typedef void(*zwrep_alrm_snsr_sup_fn)(zwifd_p ifd, uint8_t type_len, uint8_t *type, int valid);
/**<
report callback for supported alarm sensor types
@param[in]	ifd	       interface
@param[in]	type_len   size of alarm sensor type buffer
@param[in]	type       buffer to store supported alarm sensor types (ZW_ALRM_SNSR_TYPE_XXX)
@param[in]	valid      validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/

int zwif_alrm_snsr_sup_get(zwifd_p ifd, zwrep_alrm_snsr_sup_fn cb, int cache);
/**<
get the supported alarm sensor types through report callback
@param[in]	ifd	    interface
@param[in]	cb	    report callback function
@param[in]	cache	flag: to get data from cached only. If set, no fetching from real device when cache unavailable.
@return ZW_ERR_XXX
*/

int zwif_alrm_snsr_sup_cache_get(zwifd_p ifd, uint8_t *snsr_cnt, uint8_t *sup_snsr);
/**<
get supported sensor types from cache
@param[in]	ifd	        interface
@param[out]	snsr_cnt	supported sensor counts
@param[out]	sup_snsr    caller supplied buffer of size 255 bytes to store supported sensors (ZW_ALRM_SNSR_TYPE_XXX)
@return ZW_ERR_XXX
*/

/**
@}
@defgroup Barrier Barrier operator Interface APIs
Barrier operator state can be open, opening, closed, closing, stopped, and optional 1 to 99 (%) open
Their states can be read back by the generic zwif_get_report.
@ingroup zwarecapi
@{
*/

/** @name Barrier_state
* Barrier Operator state definition
@{
*/
#define ZW_BAR_STA_CLOSED       0       /**< The barrier is in the closed position*/
#define ZW_BAR_STA_CLOSING      0xFC    /**< The barrier is closing; current position is unknown */
#define ZW_BAR_STA_STOPPED      0xFD    /**< The barrier is stopped; current position is unknown */
#define ZW_BAR_STA_OPENING      0xFE    /**< The barrier is opening; current position is unknown */
#define ZW_BAR_STA_OPEN         0xFF    /**< The barrier is in the open position */

/**
@}
*/

/** @name Notification subsystem_type
* Barrier Operator Notification Subsystem Type definition
@{
*/
#define ZW_BAR_NOTIF_TYP_UNSUPPORTED     0       /**< Not supported*/
#define ZW_BAR_NOTIF_TYP_AUDIBLE         0x01    /**< Audible Notification subsystem (e.g. siren)*/
#define ZW_BAR_NOTIF_TYP_VISUAL          0x02    /**< Visual Notification subsystem (e.g. flashing light)*/

/**
@}
*/

typedef void (*zwrep_barrier_fn)(zwifd_p ifd, uint8_t state, time_t ts);
/**<
report callback for barrier operator state
@param[in]	ifd	    interface
@param[in]	state	barrier operator state, ZW_BAR_STA_XXX. It may also be in a range from 1 to 99 (%) open.
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
                    values should be ignored.
*/

typedef void (*zwrep_barrier_subsys_fn)(zwifd_p ifd, uint8_t subsys_type, uint8_t subsys_sta, time_t ts);
/**<
report callback for barrier operator subsystem state
@param[in]	ifd	        interface
@param[in]	subsys_type	barrier operator subsystem type, ZW_BAR_NOTIF_TYP_XXX.
@param[in]	subsys_sta	barrier operator subsystem state: 0=off; 0xFF=on.
@param[in]	ts          time stamp.  If this is zero, the callback has no data and hence other parameter
                        values should be ignored.
*/

int zwif_barrier_set(zwifd_p ifd, uint8_t tgt_state, zw_postset_fn cb, void *usr_param);
/**<
set barrier operator state
@param[in]	ifd		    interface
@param[in]	tgt_state   target state: 0=close, 0xFF=open
@param[in]	cb		    Optional post-set polling callback function.  NULL if no callback required.
@param[in]	usr_param   Optional user-defined parameter passed in callback.
@return	ZW_ERR_XXX
*/

int zwif_barrier_rpt_set(zwifd_p ifd, zwrep_barrier_fn rpt_cb);
/**<
setup a barrier operator state report callback function
@param[in]	ifd         Interface descriptor
@param[in]	rpt_cb	    Report callback function
return      ZW_ERR_XXX
*/

int zwif_barrier_get(zwifd_p ifd, int flag);
/**<
get barrier operator state through report callback
@param[in]	ifd	        interface
@param[in]	flag	    flag, see ZWIF_GET_BMSK_XXX
@return		ZW_ERR_XXX
*/

int zwif_barrier_notif_cfg_set(zwifd_p ifd, uint8_t type, uint8_t state);
/**<
enable/disable barrier operator notification subsystem
@param[in]	ifd		    interface
@param[in]	type        notification subsystem type ZW_BAR_NOTIF_TYP_XXX; except ZW_BAR_NOTIF_TYP_UNSUPPORTED
@param[in]	state		state : 0=disable; 0xFF=enable
@return	ZW_ERR_XXX
*/

int zwif_barrier_notif_rpt_set(zwifd_p ifd, zwrep_barrier_subsys_fn rpt_cb);
/**<
setup a barrier operator notification subsystem configuration report callback function
@param[in]	ifd         Interface descriptor
@param[in]	rpt_cb	    Report callback function
return      ZW_ERR_XXX
*/

int zwif_barrier_notif_cfg_get(zwifd_p ifd, uint8_t type, int flag);
/**<
get barrier operator notification subsystem configuration through report callback
@param[in]	ifd	        interface
@param[in]	type        notification subsystem type ZW_BAR_NOTIF_TYP_XXX; except ZW_BAR_NOTIF_TYP_UNSUPPORTED
@param[in]	flag	    flag, see ZWIF_GET_BMSK_XXX
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

typedef void (*zwrep_barrier_notif_sup_fn)(zwifd_p ifd, uint8_t type_len, uint8_t *type, int valid);
/**<
report callback for supported barrier operator notification subsystem types
@param[in]	ifd	       interface
@param[in]	type_len   size of barrier operator notification subsystem type buffer
@param[in]	type       buffer to store supported barrier operator notification subsystem types (ZW_BAR_NOTIF_TYP_XXX)
@param[in]	valid      validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/

int zwif_barrier_notif_sup_get(zwifd_p ifd, zwrep_barrier_notif_sup_fn cb, int cache);
/**<
get supported barrier operator notification subsystem report through report callback
@param[in]	ifd	        interface
@param[in]	cb	        callback function
@param[in]	cache	    flag: to get data from cached only. If set, no fetching from real device when cache unavailable.
@return		ZW_ERR_XXX
*/

int zwif_barrier_notif_sup_cache_get(zwifd_p ifd, uint8_t *type_cnt, uint8_t *sup_type);
/**<
get supported barrier operator notification subsystem from cache
@param[in]	ifd	        interface
@param[out]	type_cnt	supported barrier operator notification subsystem types counts
@param[out]	sup_type    caller supplied buffer of size 255 bytes to store barrier operator notification subsystem types (ZW_BAR_NOTIF_TYP_XXX)
@return ZW_ERR_XXX
*/

/**
@}
@defgroup SmartStart Smart Start APIs
Support Smart Start where devices in provisioning list are added automatically to the Z-wave network without user interaction.
@ingroup zwarecapi
@{
*/
#ifndef SMARTSTART_APPL

/** Meta data information type*/
#define PL_INFO_TYPE_PROD_TYPE  0      ///< Product type
#define PL_INFO_TYPE_PROD_ID    1      ///< Product ID
#define PL_INFO_TYPE_INC_INTV   2      ///< Smart Start inclusion request interval used by the node
#define PL_INFO_TYPE_UUID16     3      ///< UUID assigned to the node
#define PL_INFO_TYPE_NAME       0x32   ///< Device name
#define PL_INFO_TYPE_LOC        0x33   ///< Device location
#define PL_INFO_TYPE_INCL_STS   0x34   ///< Inclusion status of the provisioning list entry
#define PL_INFO_TYPE_S2_GRNT    0x35   ///< S2 keys to be granted
#define PL_INFO_TYPE_BOOT_MODE  0x36   ///< Bootstrapping mode
#define PL_INFO_TYPE_NW_STS     0x37   ///< Network status of the provisioning list entry

#define PL_MAX_INFO_TYPE        10     ///< Maximum number of defined meta data information types

#define PL_MAX_NAME             62     ///< Maximum name length in bytes
#define PL_MAX_LOC              62     ///< Maximum location length in bytes

#define PL_UUID_LEN             16     ///< UUID length

/** Inclusion status of the provisioning list entry */
#define PL_INCL_STS_PENDING     0      ///< The node will be included in the network when it issues SmartStart inclusion requests.
#define PL_INCL_STS_PASSIVE     2      /**< The node is in the Provisioning List but it has been decided by the supporting or
                                            controlling node that the node is unlikely to issue SmartStart inclusion requests in
                                            the near future.
                                            SmartStart Inclusion requests will be ignored by the Z/IP Gateway.
                                            All entries with this status MUST be updated to the “Pending” status (PL_INCL_STS_PENDING) when a
                                            Provisioning List Iteration Get Command is received.*/
#define PL_INCL_STS_IGNORED     3      /**< SmartStart inclusion requests sent by the node in the Provisioning List entry will be
                                            ignored until the status is changed again by a Z/IP Client or controlling node.*/

/** Network status of the provisioning list entry */
#define PL_NW_STS_NOT_INCL      0      ///< The node in the Provisioning List is not currently included (added) in the network.
#define PL_NW_STS_ADDED         1      ///< The node in the Provisioning List is included in the network and is functional.
#define PL_NW_STS_FAILED        2      /**< The node in the Provisioning List has been included in the Z-Wave network
                                            but is now marked as failing. */

/** Bootstrapping mode */
#define PL_BOOT_MODE_S2         0      ///< The node MUST manually be set to Learn Mode and follow the S2 bootstrapping instructions (if any).
#define PL_BOOT_MODE_SMART_STRT 1      ///< The node will be included and S2 bootstrapped automatically using the Smart Start functionality.

/** UUID presentation format */
#define UUID_PRES_32HEX         0      ///< 32 hex digits, no delimiters
#define UUID_PRES_16ASCII       1      ///< 16 ASCII chars, no delimiters
#define UUID_PRES_SN_32HEX      2      ///< "sn:" followed by 32 hex digits, no delimiters
#define UUID_PRES_SN_16ASCII    3      ///< "sn:" followed by 16 ASCII chars, no delimiters
#define UUID_PRES_UUID_32HEX    4      ///< "UUID:" followed by 32 hex digits, no delimiters
#define UUID_PRES_UUID_16ASCII  5      ///< "UUID:" followed by 16 ASCII chars, no delimiters
#define UUID_PRES_RFC4122       6      ///< RFC4122 compliant presentation (e.g. “58D5E212-165B-4CA0-909B-C86B9CEE0111”)

/** Product type meta data */
typedef struct
{
	uint8_t     generic_cls;    /**< Generic device class*/
	uint8_t     specific_cls;	/**< Specific device class*/
    uint16_t    icon_type;      /**< Installer icon type*/
} pl_prod_type_t;

/** Product ID meta data */
typedef struct
{
	uint16_t    manf_id;        /**< Manufacturer ID*/
	uint16_t    prod_type;	    /**< Product type*/
    uint16_t    prod_id;        /**< Product ID*/
	uint8_t     app_ver;        /**< Application version*/
	uint8_t     app_sub_ver;    /**< Application sub version*/
} pl_prod_id_t;

/** UUID */
typedef struct
{
	uint8_t pres_fmt;           /**< Presentation format, UUID_PRES_XXX */
    uint8_t uuid[PL_UUID_LEN];  /**< UUID assigned to the node as defined by IETF RFC 4122 */
} pl_uuid_t;

/** Network status meta data */
typedef struct
{
	uint8_t node_id;            /**< NodeID that has been assigned to the Provisioning List entry during network inclusion. 0 indicates
                                     that the NodeID is not assigned*/
    uint8_t status;             /**< network status of the Provisioning List entry, PL_NW_STS_XXX */
} pl_nw_sts_t;

/** Provisioning list meta data information */
typedef struct
{
	uint8_t     type;	                /**< Info type, PL_INFO_TYPE_XXX */
    union
    {
        char    name[PL_MAX_NAME + 1];  /**< For type PL_INFO_TYPE_NAME; device name in UTF-8 encoding. It must not contains period character '.',
                                             underscore character '_' and must not end with the dash character '-' and must be terminated by
                                             NUL character*/
        char    loc[PL_MAX_LOC + 1];    /**< For type PL_INFO_TYPE_LOC; device location in UTF-8 encoding with same restrictions as name field. */
        pl_prod_type_t  prod_type;      /**< For type PL_INFO_TYPE_PROD_TYPE; product type*/
        pl_prod_id_t    prod_id;        /**< For type PL_INFO_TYPE_PROD_ID; product id*/
        uint8_t         interval;       /**< For type PL_INFO_TYPE_INC_INTV; Smart Start inclusion request interval in unit of 128 seconds.
                                             This field must have value ranging from 5 to 99. */
        pl_uuid_t       uuid;           /**< For type PL_INFO_TYPE_UUID16; UUID assigned to the node as defined by IETF RFC 4122 */
        pl_nw_sts_t     nw_sts;         /**< For type PL_INFO_TYPE_NW_STS; network status of the provisioning list entry  */
        uint8_t         incl_sts;       /**< For type PL_INFO_TYPE_INCL_STS; inclusion status of the provisioning list entry, PL_INCL_STS_XXX */
        uint8_t         grnt_keys;      /**< For type PL_INFO_TYPE_S2_GRNT; S2 keys to be granted, see SEC_KEY_BITMSK_XXX. Use only for SmartStart inclusions. */
        uint8_t         boot_mode;      /**< For type PL_INFO_TYPE_BOOT_MODE; Bootstrapping mode, see PL_BOOT_MODE_XXX */
    } info;

} pl_info_t;

#endif

/** Provisioning list entry*/
typedef struct pl_lst_ent
{
	char        dsk[MAX_DSK_STR_LEN + 1];/**< Device Specific Key (DSK). The format of the DSK must be 8 groups of 5 digits separated
                                              by '-' as shown in the example: 34028-23669-20938-46346-33746-07431-56821-14553 */
    pl_info_t   info[PL_MAX_INFO_TYPE]; /**< Information of the device*/
    uint8_t     info_cnt;               /**< Number of information stored in "info". If zero, the dsk is not found in the provisioning list */
} pl_lst_ent_t;


typedef void (*pl_list_fn)(void *usr_ctx, pl_lst_ent_t *pl_list, uint8_t ent_cnt, int lst_cmplt);
/**<
callback to report all entries in provisioning list
@param[in]	usr_ctx     user context
@param[in]	pl_list     provisioning list
@param[in]	ent_cnt     number of entries in provisioning list 'pl_list'
@param[in]	lst_cmplt   flag to indicate whether all the entries in 'pl_list' represent the whole list in Z/IP gateway. 1=all the entries
                        have been retrieved from Z/IP gateway; 0=some of the entries were not retrieved due to error.
*/

int zwnet_pl_add(zwnet_p net, char *dsk, pl_info_t *info, uint8_t info_cnt);
/**<
Add a provisioning list entry
@param[in]	net		    Network
@param[in]	dsk		    Device Specific Key (DSK). The format of the DSK must be 8 groups of 5 digits separated by '-' as shown in the example:
                        34028-23669-20938-46346-33746-07431-56821-14553
@param[in]	info		Buffer to store additional information of the device (optional). Info type PL_INFO_TYPE_NW_STS must not be
                        used in this function.
@param[in]	info_cnt    Number of additional information stored in "info".
@return		ZW_ERR_XXX
*/

int zwnet_pl_get(zwnet_p net, char *dsk, pl_info_fn cb, void *usr_ctx);
/**<
Get a provisioning list entry information through callback
@param[in]	net		    Network
@param[in]	dsk		    Device Specific Key (DSK). The format of the DSK must be 8 groups of 5 digits separated by '-' as shown in the example:
                        34028-23669-20938-46346-33746-07431-56821-14553
@param[in]	cb          Callback to report provisioning list entry information
@param[in]	usr_ctx     User context used in callback
@return		ZW_ERR_XXX
*/

int zwnet_pl_del(zwnet_p net, char *dsk);
/**<
Delete a provisioning list entry
@param[in]	net		    Network
@param[in]	dsk		    Device Specific Key (DSK). The format of the DSK must be 8 groups of 5 digits separated by '-' as shown in the example:
                        34028-23669-20938-46346-33746-07431-56821-14553
@return		ZW_ERR_XXX
*/

int zwnet_pl_list_get(zwnet_p net, pl_list_fn cb, void *usr_ctx);
/**<
Get all provisioning list entries through callback
@param[in]	net		    Network
@param[in]	cb          Callback to report provisioning list entries
@param[in]	usr_ctx     User context used in callback
@return		ZW_ERR_XXX
*/

int zwnet_pl_list_del(zwnet_p net);
/**<
Delete all provisioning list entries
@param[in]	net		network
@return		ZW_ERR_XXX
*/

#ifdef PL_S2_GRANT_MODE_API

int zwnet_pl_s2_grnt_mode_set(zwnet_p net, uint8_t mode);
/**<
set whether to ask for confirmation before granting S2 keys to non-Smart Start node in provisioning list
@param[in]	net		    Network
@param[in]	mode		1 = ask for confirmation; 0 = do not ask for confirmation
@return		ZW_ERR_XXX
@note       The setting only applies to non-Smart Start node present in the provisioning list
*/

typedef void (*pl_s2_grnt_mode_fn)(void *usr_ctx, uint8_t mode);
/**<
callback to report provisioning list S2 keys grant mode setting
@param[in]	usr_ctx     user context
@param[in]	mode		1 = ask for confirmation; 0 = do not ask for confirmation
*/

int zwnet_pl_s2_grnt_mode_get(zwnet_p net, pl_s2_grnt_mode_fn cb, void *usr_ctx);
/**<
get S2 keys grant mode setting through callback
@param[in]	net		    Network
@param[in]	cb          Callback to report S2 keys grant mode setting
@param[in]	usr_ctx     User context used in callback
@return		ZW_ERR_XXX
@note       The setting only applies to non-Smart Start node present in the provisioning list
*/
#endif


/****************************************************************************/
// skysoft modified start

/*
 **  Command Class Basic
 */
typedef void (*zwrep_basic_v2_fn)(zwifd_p ifd, zwbasic_p val, time_t ts);
/**<
basic report v2 callback
@param[in]	ifd	     interface
@param[in]	cur_val	 current value
@param[in]	tar_val	 target value
@param[in]	duration
*/

int zwif_basic_rpt_set_v2(zwifd_p ifd, zwrep_basic_v2_fn rpt_cb);
/**<
setup a basic version 2 report callback function
@param[in]	ifd         Interface descriptor
@param[in]	rpt_cb	    Report callback function
return      ZW_ERR_XXX
*/


/*
 **  Command Class Notification
 */
int zwif_notification_info_cache_get(zwifd_p ifd, if_alarm_data_t **sup_snsr, uint8_t *alarm_cnt);

/**
@}
@defgroup If_Notification Notification Interface APIs
Used to report notification or service conditions
@{
*/

/** Z-wave notification type */
#define ZW_NOTIFICATION_SMOKE       0x01    /**< Smoke notification*/
#define ZW_NOTIFICATION_CO          0x02    /**< Carbon monoxide notification*/
#define ZW_NOTIFICATION_CO2         0x03    /**< Carbon dioxide notification*/
#define ZW_NOTIFICATION_HEAT        0x04    /**< Heat notification*/
#define ZW_NOTIFICATION_WATER       0x05    /**< Water notification*/
#define ZW_NOTIFICATION_LOCK        0x06    /**< Lock access control notification*/
#define ZW_NOTIFICATION_BURGLAR     0x07    /**< Burglar notification or home security*/
#define ZW_NOTIFICATION_POWER       0x08    /**< Power management notification*/
#define ZW_NOTIFICATION_SYSTEM      0x09    /**< System notification*/
#define ZW_NOTIFICATION_EMERGENCY   0x0A    /**< Emergency notification*/
#define ZW_NOTIFICATION_CLOCK       0x0B    /**< Notification clock*/
#define ZW_NOTIFICATION_FIRST       0xFF    /**< Used by the zwif_notification_get() to retrieve the
                                         first notification detection from the supported list*/
/** Z-wave notification event for all*/
#define ZW_NOTIFICATION_EVT_UNKNOWN             0xFE    /**< Unknown event*/

/** Z-wave notification event for smoke notification*/
#define ZW_NOTIFICATION_EVT_SMOKE_L             0x01    /**< Smoke detected with location info*/
#define ZW_NOTIFICATION_EVT_SMOKE               0x02    /**< Smoke detected with unknown location info*/
#define ZW_NOTIFICATION_EVT_SMOKE_TEST          0x03    /**< Smoke notification test*/

/** Z-wave notification event for carbon monoxide notification*/
#define ZW_NOTIFICATION_EVT_CO_L                0x01    /**< Carbon monoxide detected with location info*/
#define ZW_NOTIFICATION_EVT_CO                  0x02    /**< Carbon monoxide detected with unknown location info*/

/** Z-wave notification event for carbon dioxide notification*/
#define ZW_NOTIFICATION_EVT_CO2_L               0x01    /**< Carbon dioxide detected with location info*/
#define ZW_NOTIFICATION_EVT_CO2                 0x02    /**< Carbon dioxide detected with unknown location info*/

/** Z-wave notification event for heat notification*/
#define ZW_NOTIFICATION_EVT_OVERHEAT_L          0x01    /**< Overheat detected with location info*/
#define ZW_NOTIFICATION_EVT_OVERHEAT            0x02    /**< Overheat detected with unknown location info*/
#define ZW_NOTIFICATION_EVT_TEMP_RISE_L         0x03    /**< Rapid temperature rise detected with location info*/
#define ZW_NOTIFICATION_EVT_TEMP_RISE           0x04    /**< Rapid temperature rise detected with unknown location info*/
#define ZW_NOTIFICATION_EVT_UNDRHEAT_L          0x05    /**< Underheat detected with location info*/
#define ZW_NOTIFICATION_EVT_UNDRHEAT            0x06    /**< Underheat detected with unknown location info*/

/** Z-wave notification event for water notification*/
#define ZW_NOTIFICATION_EVT_LEAK_L              0x01    /**< Water leak detected with location info*/
#define ZW_NOTIFICATION_EVT_LEAK                0x02    /**< Water leak detected with unknown location info*/
#define ZW_NOTIFICATION_EVT_LVL_L               0x03    /**< Water level dropped detected with location info*/
#define ZW_NOTIFICATION_EVT_LVL                 0x04    /**< Water level dropped detected with unknown location info*/

/** Z-wave notification event for lock access control notification*/
#define ZW_NOTIFICATION_EVT_MANUAL_LCK          0x01    /**< Manual lock operation*/
#define ZW_NOTIFICATION_EVT_MANUAL_ULCK         0x02    /**< Manual unlock operation*/
#define ZW_NOTIFICATION_EVT_RF_LCK              0x03    /**< RF lock operation*/
#define ZW_NOTIFICATION_EVT_RF_ULCK             0x04    /**< RF unlock operation*/
#define ZW_NOTIFICATION_EVT_KEYPAD_LCK          0x05    /**< Keypad lock operation with user identifier info*/
#define ZW_NOTIFICATION_EVT_KEYPAD_ULCK         0x06    /**< Keypad unlock operation with user identifier info*/
#define ZW_NOTIFICATION_EVT_MANUAL_NOT_FUL_LCK  0x07    /**< Manual not fully locked operation*/
#define ZW_NOTIFICATION_EVT_RF_NOT_FUL_LCK      0x08    /**< RF not fully loced operation*/
#define ZW_NOTIFICATION_EVT_AUTO_LCK            0x09    /**< Auto lock locked operation*/
#define ZW_NOTIFICATION_EVT_AUTO_NOT_FUL_OPER   0x0A    /**< Auto lock not fully operation*/
#define ZW_NOTIFICATION_EVT_LCK_JAMMED          0x0B    /**< Lock jammed*/
#define ZW_NOTIFICATION_EVT_ALL_CODE_DEL        0x0C    /**< All user codes deleted*/
#define ZW_NOTIFICATION_EVT_1_CODE_DEL          0x0D    /**< Single user code deleted*/
#define ZW_NOTIFICATION_EVT_CODE_ADDED          0x0E    /**< New user code added*/
#define ZW_NOTIFICATION_EVT_CODE_DUP            0x0F    /**< New user code not added due to duplicate code*/
#define ZW_NOTIFICATION_EVT_KEYPAD_DISABLED     0x10    /**< Keypad temporary disabled*/
#define ZW_NOTIFICATION_EVT_KEYPAD_BUSY         0x11    /**< Keypad busy*/
#define ZW_NOTIFICATION_EVT_NEW_PROG_CODE       0x12    /**< New program code entered - unique code for lock configuration*/
#define ZW_NOTIFICATION_EVT_USR_CODE_LIMIT      0x13    /**< Manually enter user access code exceeds code limit*/
#define ZW_NOTIFICATION_EVT_RF_ULCK_INVLD_CODE  0x14    /**< Unlock by RF with invalid user code*/
#define ZW_NOTIFICATION_EVT_RF_LCK_INVLD_CODE   0x15    /**< Locked by RF with invalid user code*/
#define ZW_NOTIFICATION_EVT_WINDOW_DOOR_OPEN    0x16    /**< Window/door is open*/
#define ZW_NOTIFICATION_EVT_WINDOW_DOOR_CLOSED  0x17    /**< Window/door is closed*/


/** Z-wave notification event for burglar notification*/
#define ZW_NOTIFICATION_EVT_INTRUSION_L         0x01    /**< Intrusion detected with location info*/
#define ZW_NOTIFICATION_EVT_INTRUSION           0x02    /**< Intrusion detected with unknown location info*/
#define ZW_NOTIFICATION_EVT_TMPR_COVER          0x03    /**< Tampering, product covering removed*/
#define ZW_NOTIFICATION_EVT_TMPR_CODE           0x04    /**< Tampering, Invalid Code*/
#define ZW_NOTIFICATION_EVT_GLASS_L             0x05    /**< Glass breakage detected with location info*/
#define ZW_NOTIFICATION_EVT_GLASS               0x06    /**< Glass breakage detected with unknown location info*/
#define ZW_NOTIFICATION_EVT_MOTION_DET_L        0x07    /**< Motion detected with location info*/

/** Z-wave notification event for power management notification*/
#define ZW_NOTIFICATION_EVT_POWER               0x01    /**< Power has been applied*/
#define ZW_NOTIFICATION_EVT_AC_OFF              0x02    /**< AC mains disconnected*/
#define ZW_NOTIFICATION_EVT_AC_ON               0x03    /**< AC mains re-connected*/
#define ZW_NOTIFICATION_EVT_SURGE               0x04    /**< Surge Detection*/
#define ZW_NOTIFICATION_EVT_VOLT_DROP           0x05    /**< Voltage Drop/Drift detected*/
#define ZW_NOTIFICATION_EVT_OVER_CURRENT        0x06    /**< Over-current detected*/
#define ZW_NOTIFICATION_EVT_OVER_VOLT           0x07    /**< Over-voltage detected*/
#define ZW_NOTIFICATION_EVT_OVER_LOAD           0x08    /**< Over-load detected*/
#define ZW_NOTIFICATION_EVT_LOAD_ERR            0x09    /**< Load error*/
#define ZW_NOTIFICATION_EVT_REPLACE_BATT_SOON   0x0A    /**< Replace battery soon*/
#define ZW_NOTIFICATION_EVT_REPLACE_BATT_NOW    0x0B    /**< Replace battery now*/

/** Z-wave notification event for system notification*/
#define ZW_NOTIFICATION_EVT_HW                  0x01    /**< System hardware failure*/
#define ZW_NOTIFICATION_EVT_SW                  0x02    /**< System software failure*/
#define ZW_NOTIFICATION_EVT_HW_OEM_CODE         0x03    /**< System hardware failure with OEM proprietary failure code*/
#define ZW_NOTIFICATION_EVT_SW_OEM_CODE         0x04    /**< System software failure with OEM proprietary failure code*/

/** Z-wave notification event for emergency notification*/
#define ZW_NOTIFICATION_EVT_POLICE              0x01    /**< Contact police*/
#define ZW_NOTIFICATION_EVT_FIRE                0x02    /**< Contact fire service*/
#define ZW_NOTIFICATION_EVT_MEDICAL             0x03    /**< Contact medical service*/

/** Z-wave notification event for notification clock*/
#define ZW_NOTIFICATION_EVT_WKUP                0x01    /**< Wake up aler*/
#define ZW_NOTIFICATION_EVT_TIMER_ENDED         0x02    /**< Timer ended*/

/** Z-wave notification parameter type*/
#define ZW_NOTIFICATION_PARAM_LOC               1    /**< node location UTF-8 string (NULL terminated)*/
#define ZW_NOTIFICATION_PARAM_USRID             2    /**< user id*/
#define ZW_NOTIFICATION_PARAM_OEM_ERR_CODE      3    /**< OEM proprietary system failure code */

/** Z-wave notification status*/
#define ZW_NOTIFICATION_STS_DEACTIVATED         0    /**< Unsolicited notification report is deactivated*/
#define ZW_NOTIFICATION_STS_ACTIVATED           0xFF /**< Unsolicited notification report is activated or a pending
                                                  notification is present*/
#define ZW_NOTIFICATION_STS_NO_PEND_NOTICE      0xFE /**< No pending notification */

typedef struct
{
    uint8_t     type;           /**< Vendor specific alarm type*/
    uint8_t     level;          /**< Vendor specific alarm level*/
    uint8_t     reserved;        
    /*extended info fields*/
    uint8_t     ex_status;      /**< Z-wave alarm status (ZW_ALRM_STS_XXX) */
    uint8_t     ex_type;        /**< Z-wave alarm type (ZW_ALRM_XXX) */
    uint8_t     ex_event;       /**< Z-wave alarm event (ZW_ALRM_EVT_XXX) */
    uint8_t     ex_evt_len;     /**< Z-wave alarm event parameter length. Zero if the event has no parameter */
    uint8_t     ex_evt_type;    /**< Z-wave alarm event parameter type (ZW_ALRM_PARAM_XXX) */
    uint8_t     ex_evt_prm[1];  /**< Z-wave alarm event parameter place holder*/
}
zwnotification_t, *zwnotification_p;

typedef void (*zwrep_notification_fn)(zwifd_p ifd, zwnotification_p  notification_info, time_t ts);
/**<
report callback for notification
@param[in]	ifd	        interface
@param[in]	notification_info  notification info
*/

int zwif_notification_rpt_set(zwifd_p ifd, zwrep_notification_fn rpt_cb);
/**<
setup an notification report callback function
@param[in]	ifd         interface descriptor
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_notification_get(zwifd_p ifd, uint8_t vtype, uint8_t ztype, uint8_t evt);
/**<
get the state of the notification device through report callback
@param[in]	ifd	        interface
@param[in]	vtype	    vendor specific notification type. Zero if this field is not used
@param[in]	ztype	    Z-wave notification type (ZW_NOTIFICATION_XXX). Zero if this field is not used; 0xFF=to retrieve the first notification detection.
@param[in]	evt	        Event corresponding to Z-wave notification type. Zero if this field is not used.
@return		ZW_ERR_XXX
*/

int zwif_notification_set(zwifd_p ifd, uint8_t ztype, uint8_t sts);
/**<
set the activity of the specified Z-Wave Notification Type
@param[in]	ifd	    interface
@param[in]	ztype	Z-wave notification type (ZW_NOTIFICATION_XXX)
@param[in]	sts     Z-wave notification status. 0= deactivated; 0xFF= activated
@return	ZW_ERR_XXX
*/

typedef void (*zwrep_notification_sup_fn)(zwifd_p ifd, uint8_t have_vtype, uint8_t ztype_len, uint8_t *ztype, int valid);
/**<
report callback for supported notification types
@param[in]	ifd	        interface
@param[in]	have_vtype  flag to indicate whether vendor specific notification type supported. 1=supported; else 0=unsupported
@param[in]	ztype_len   size of ztype buffer
@param[in]	ztype       buffer to store supported Z-wave notification types (ZW_NOTIFICATION_XXX)
*/

int zwif_notification_sup_get(zwifd_p ifd, zwrep_notification_sup_fn cb);
/**<
get the supported notification types
@param[in]	ifd	    interface
@param[in]	cb	    report callback function
@return ZW_ERR_XXX
*/

typedef void (*zwrep_notification_evt_fn)(zwifd_p ifd, uint8_t ztype, uint8_t evt_len, uint8_t *evt, int valid);
/**<
report callback for supported notification types
@param[in]	ifd	        interface
@param[in]	ztype       Z-wave notification type (ZW_NOTIFICATION_XXX)
@param[in]	evt_len     size of evt buffer
@param[in]	evt         buffer to store supported event of the notification type specified in ztype
*/

int zwif_notification_sup_evt_get(zwifd_p ifd, uint8_t ztype, zwrep_notification_evt_fn cb);
/**<
get the supported events of a specified notification type
@param[in]	ifd	    interface
@param[in]	ztype   Z-wave notification type (ZW_NOTIFICATION_XXX)
@param[in]	cb	    report callback function
@return ZW_ERR_XXX
*/

typedef void (*zwrep_notification_get_fn)(zwifd_p ifd, uint8_t mode);
/**<
report callback for notification get
@param[in]	ifd	    interface
@param[in]	mode current mode,The 'Mode' can return the following values
			0x00	excluded from the all on/all off functionality
			0x01	excluded from the all on functionality but not all off
			0x02	excluded from the all off functionality but not all on
			0xFF	included in the all on/all off functionality
*/

int zwif_notification_get_rpt_set(zwifd_p ifd, zwrep_notification_get_fn rpt_cb);
/**<
setup notification get report callback function
@param[in]	ifd         interface
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

/*
 **  Command Class Meter
 */
// dump meter cache info
int zwif_meter_info_cache_get(zwifd_p ifd, zwmeter_cap_t **sup_snsr, uint8_t *snsr_cnt);


/*
 **  Command Class Central Scene version 2
 */
typedef struct
{
    uint8_t     seq_num;		           // sequence number
    uint8_t     key_attr;                  // key attribute
    uint8_t     scene_num;                 // scene number
}
zwcentral_scene_notify_t, *zwcentral_scene_notify_p;


/*
 **  Command Class Switch All
 */

/**<
zwif_switch_all_on - set switch all on
@param[in]	net	     internet
@return		ZW_ERR_XXX
*/
int zwif_switch_all_on(zwifd_p ifd);

// For broadcast
int zwif_switch_all_on_broadcast(zwnet_p net);

/**<
zwif_switch_all_off - set switch all off
@param[in]  net     internet
@return ZW_ERR_XXX
*/
int zwif_switch_all_off(zwifd_p ifd);

// For broadcast
int zwif_switch_all_off_broadcast(zwnet_p net);

/**
zwif_switch_all_set - set switch all value
@param[in]  ifd     interface
@param[in]  v       value (the range of value is device specific)
@return ZW_ERR_XXX
*/
int zwif_switch_all_set(zwifd_p ifd, uint8_t v);

typedef void (*zwrep_switch_all_get_fn)(zwifd_p ifd, uint8_t mode);
/**<
report callback for switch all get
@param[in]	ifd	    interface
@param[in]	mode current mode,The 'Mode' can return the following values
			0x00	excluded from the all on/all off functionality
			0x01	excluded from the all on functionality but not all off
			0x02	excluded from the all off functionality but not all on
			0xFF	included in the all on/all off functionality
*/

int zwif_switch_all_get_rpt_set(zwifd_p ifd, zwrep_switch_all_get_fn rpt_cb);
/**<
setup switch all get report callback function
@param[in]	ifd         interface
@param[in]	rpt_cb	    report callback function
return      ZW_ERR_XXX
*/

int zwif_switch_all_get(zwifd_p ifd);
/**<
get the switch all mode in use by the node through report callback
@param[in]	ifd	        interface
@return		ZW_ERR_XXX
*/

/*
 **  Command Class Scene Actuator Conf ver 1
 */
typedef void (*zwrep_scene_actuator_conf_get_fn)(zwifd_p ifd, uint8_t sceneId, uint8_t level, uint8_t dimDuration);
int zwif_scene_actuator_conf_get(zwifd_p ifd, uint8_t sceneId, zwrep_scene_actuator_conf_get_fn rpt_cb);
int zwif_scene_actuator_conf_set(zwifd_p ifd, uint8_t sceneId, uint8_t dimDuration, uint8_t override, uint8_t level);


/*
 **  Command Class Multi Cmd
 */
int zwif_multi_cmd_encap(zwifd_p ifd);

/*
 **  Command Class Network Management IMA 
 */
typedef void (*zwrep_network_rssi_get_fn)(zwifd_p ifd, uint8_t channel_1, uint8_t channel_2, uint8_t channel_3);
int zwif_network_rssi_rep_set_get(zwifd_p ifd, zwrep_network_rssi_get_fn cb);

// skysoft modified end
/****************************************************************************/


/**
@}
*/

#endif
