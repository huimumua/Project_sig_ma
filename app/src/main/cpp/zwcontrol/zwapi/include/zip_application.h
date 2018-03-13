/**
@file   zip_application.h - Z/IP interface application layer implementation header file.

        To provide network management and controller functions.

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
#ifndef _ZIP_APPLICATION_DAVID_
#define _ZIP_APPLICATION_DAVID_

#include "zip_session.h"

#ifdef OS_LINUX
#if !defined(OS_MAC_X) && !defined(OS_ANDROID)
#define NI_NUMERICHOST	1	/**< Don't try to look up hostname.  */
#define NI_NUMERICSERV  2	/**< Don't convert port number to name.  */
#endif
#endif

/**
@defgroup Application Application layer APIs
To provide network management and controller functions.
@ingroup zwarecapi
@{
*/
///
/// Boolean definition
#ifndef TRUE
#define TRUE  1         ///< True
#endif
#ifndef FALSE
#define FALSE 0         ///< False
#endif


#define APPL_CB_TMOUT_MIN        (FRAME_SEND_TIMEOUT_MIN * (FRAME_MAX_RESEND + 1) + 100)  ///< Min. callback timeout from session layer
//#define APPL_WAIT_SEND_TIMEOUT      7000    ///< The maximum wait time for sending a command in milliseconds
#define APPL_WAIT_SEND_TIMEOUT   (APPL_CB_TMOUT_MIN + 100)    ///< The maximum wait time for sending a command in milliseconds
#define MAX_ZWAVE_NODE           232     ///< The maximum number of Z-wave nodes in a network
#define UNSOLICITED_NODE_ID      (MAX_ZWAVE_NODE + 1)  ///< The temporary node id assigned to unsolicited node

#define START_FAILED_NODE_REPLACE       0x01    ///< Start failed node replace
#define STOP_FAILED_NODE_REPLACE        0x05    ///< Stop failed node replace

/** @name Nw_mgmnt_op_id
* Network management operation id definition
@{
*/
#define NM_OP_NODE_LIST_GET             0   ///< Get node list
#define NM_OP_NODE_INFO_CACHED_GET      1   ///< Get cached node info
#define NM_OP_DEFAULT_SET               2   ///< Reset z-wave controller
#define NM_OP_LEARN_MODE_SET            3   ///< Start learn mode
#define NM_OP_NETWORK_UPDATE_REQUEST    4   ///< Start network update
#define NM_OP_NODE_ADD                  5   ///< Add node
#define NM_OP_NODE_REMOVE               6   ///< Remove node
#define NM_OP_FAILED_NODE_ID_REMOVE     7   ///< Remove failed node
#define NM_OP_FAILED_NODE_REPLACE       8   ///< Replace failed node
#define NM_OP_REQ_NODE_NBR_UPDATE       9   ///< Request node neighbor update
#define NM_OP_CONTROLLER_CHANGE         10  ///< Start controller change
#define NM_OP_ASSIGN_RET_ROUTE          11  ///< Assign return route
#define NM_OP_DELETE_RET_ROUTE          12  ///< Delete return route
#define NM_OP_DSK_GET                   13  ///< Get Z/IP gateway DSK
#define NM_OP_EP_CNT_GET                14  ///< Get node endpoint counts
#define NM_OP_EP_CAP_GET                15  ///< Get endpoint capabilities
#define NM_OP_AGGR_EP_GET               16  ///< Get aggregate endpoint members
#define NM_OP_PL_ITER_GET               17  ///< Get a provisioning list entry info through iteration
#define NM_OP_PL_GET                    18  ///< Get a provisioning list entry info
#define NM_OP_PL_S2_GRANT_MODE          19  ///< Get S2 keys grant mode setting
#define NM_OP_IMA_STAT_GET              20  ///< Get IMA node statistic
#define NM_OP_GW_RSSI_GET               21  ///< Get background RSSI from Z/IP gateway

#define NM_OP_TOTAL                     22  ///< Total number of operations
///@}

/** @name Node_status
* Node status definition
@{
*/
#define APPL_NODE_STS_FORCE_DOWN        0xFF///< Force node status to down.  Normally, sleeping can't be set to down without further investigation.
#define APPL_NODE_STS_SLEEP             2   ///< Node is sleeping
#define APPL_NODE_STS_DOWN              1   ///< Node is down
#define APPL_NODE_STS_UP                0   ///< Node is alive
///@}

///
/// Node information
typedef struct
{
    uint8_t   node_id;       ///< node identifier
    uint8_t   listen;        ///< flag to indicate whether the node is always listening (awake)
    uint8_t   optional;      /**< flag to indciate whether the node supports more command classes
                                  in addition to the ones covered by the device classes listed in this message.*/
    uint8_t   sensor;        ///< flag to indicate whether the node is a sensor
    uint8_t   status;        ///< status indicating the progress
    uint8_t   age;           ///< age of the NodeInfo frame. Time is given in 2^n minutes
    uint8_t   resrvd;        ///< reserved for version 1, grant keys for version 2
    uint8_t   basic;         ///< basic device class
    uint8_t   gen;           ///< generic device class
    uint8_t   spec;          ///< specific device class
    uint8_t   cmd_cnt;       ///< the number of command classes in the unsecure cmd_cls field
    uint8_t   cmd_cnt_sec;   ///< the number of command classes in the secure cmd_cls field
    uint16_t  *cmd_cls;      ///< unsecure command classes
    uint16_t  *cmd_cls_sec;  ///< secure command classes

} appl_node_info_t;


///
/// Endpoint information
typedef struct
{
    uint8_t   node_id;       ///< node identifier
    uint8_t   ep_id;         ///< endpoint identifier
    uint8_t   gen;           ///< generic device class
    uint8_t   spec;          ///< specific device class
    uint8_t   cmd_cnt;       ///< the number of command classes in the unsecure cmd_cls field
    uint8_t   cmd_cnt_sec;   ///< the number of command classes in the secure cmd_cls field
    uint16_t  *cmd_cls;      ///< unsecure command classes
    uint16_t  *cmd_cls_sec;  ///< secure command classes

} appl_ep_info_t;


///
/// Send data parameters
typedef struct
{
    uint8_t   node_id;      ///< node identifier; zero for Z/IP gateway
    uint8_t   wkup_poll;    ///< flag to indicate this frame is sent for the purpose of wakeup notification polling
    uint16_t  flag;         ///< flag, see ZIP_FLAG_XXX
    uint16_t  encap_fmt;    ///< Z-wave encapsulation format (valid if flag ZIP_FLAG_ENCAP_INFO is set).
    frm_ep_t  ep_addr;      ///< source and destination endpoint addresses
    uint16_t  dat_len;      ///< length of the dat_buf field
    uint8_t   *dat_buf;     ///< data buffer

} appl_snd_data_t;

///
/// Network management callback information
typedef struct
{
    void      *cb;          ///< callback function pointer
    uint8_t   node_id;      ///< node identifier; zero for Z/IP gateway
    uint8_t   seq_num;      ///< sequence number of the request

} nm_cb_info_t;

///
/// Node-IP address translation table entry
typedef struct
{
    uint8_t   valid;                ///< Flag to indicate the IP address is valid
    uint8_t   ipaddr_sz;            ///< IP address size in bytes
    uint8_t   ipaddr[IPV6_ADDR_LEN];///< IP address
} node_ipaddr_t;

#define ZIP_ND_INFORMATION_OK           0   /**< valid information in both the IPv6 Address and Node ID fields*/
#define ZIP_ND_INFORMATION_OBSOLETE     1   /**< information in the IPv6 Address and Node ID fields is obsolete.
                                                 No node exists in the network with this address information.*/
#define ZIP_ND_INFORMATION_NOT_FOUND    2   /**< responding Z/IP Router could not locate valid information.*/

#define MAX_DSK_STR_LEN      47      /**< Maximum DSK string length */
#define MAX_DSK_KEY_LEN      16      /**< Maximum DSK key length*/

///
/// Z/IP ND node advertisement report
typedef struct
{
    uint8_t addr_local;     ///< flag to indicate that the requester asked for the site-local address (a.k.a. ULA).
    uint8_t status;         ///< status of the address/node id resolution query (ZIP_ND_INFORMATION_XXX)
    uint8_t node_id;        ///< node id
    uint8_t ipv6_addr[IPV6_ADDR_LEN];        ///< IPv6 address that corresponds to the node id
    uint32_t    homeid;     ///< Z-wave home id

} nd_advt_rpt_t;

///
/// Node information with security 2 support
typedef struct
{
    appl_node_info_t    node_info;  ///< node information
    int                 sec2_valid; ///< flag to indicate whether the following security 2 parameters are valid
    uint8_t             grnt_keys;  ///< security 2: granted keys
    uint8_t             key_xchg_fail_type;  ///< security 2: key exchange failed type. Zero means success.
    uint8_t             dsk_len;    ///< DSK length
    uint8_t             dsk[MAX_DSK_KEY_LEN];  ///< security 2: DSK bytes.
} appl_node_info_sec2_t;


///
/// Add node DSK report
typedef struct
{
    uint8_t             dsk_keys[MAX_DSK_KEY_LEN];  ///< DSK buffer
    uint8_t             obscure_key_len;            ///< Obscure DSK key length in bytes in the dsk_keys buffer
} appl_dsk_rpt_t;

///
/// Add node keys report
typedef struct
{
    uint8_t     req_keys;   ///< Requested keys (bit mask) by the joining node
    uint8_t     req_csa;    ///< Flag to indicate joining node is requesting Client-side Authentication (CSA)
} appl_node_keys_rpt_t;

///
/// Node replace with security 2 support
typedef struct
{
    uint8_t     sts;        ///< status
    uint8_t     node_id;    ///< node id of replaced node
    int         sec2_valid; ///< flag to indicate whether the following security 2 parameters are valid
    uint8_t     grnt_keys;  ///< security 2: granted keys
    uint8_t     key_xchg_fail_type;  ///< security 2: key exchange failed type. Zero means success.
} appl_node_rp_sec2_t;

///
/// Learnt mode callback parameters with security 2 support
typedef struct
{
    uint8_t     sts;        ///< status
    uint8_t     node_id;    ///< node id assigned by including controller
    int         sec2_valid; ///< flag to indicate whether the following security 2 parameters are valid
    uint8_t     grnt_keys;  ///< security 2: granted keys
    uint8_t     dsk[MAX_DSK_KEY_LEN];  ///< security 2: DSK bytes.
} appl_lrn_mod_sec2_t;


///
/// Node add status callback parameters
typedef struct
{
    uint16_t    rpt_type;     ///< report types : NODE_ADD_STATUS, FAILED_NODE_REPLACE_STATUS, NODE_ADD_KEYS_REPORT_V2, or NODE_ADD_DSK_REPORT_V2
    uint16_t    flag;         ///< Flag, see ZIP_FLAG_XXX
    union
    {
        appl_node_info_sec2_t   node_info;      ///< For NODE_ADD_STATUS
        appl_node_rp_sec2_t     node_rp_info;   ///< For FAILED_NODE_REPLACE_STATUS
        appl_node_keys_rpt_t    req_keys_rpt;   ///< For NODE_ADD_KEYS_REPORT_V2
        appl_dsk_rpt_t          dsk_rpt;        ///< For NODE_ADD_DSK_REPORT_V2
    } rpt_prm;

} appl_node_add_cb_prm_t;

#define PL_INFO_CRITICAL_MASK  0x01    ///< Bitmask for critical meta data information type

#ifndef SMARTSTART_APPL
#define SMARTSTART_APPL          //Define to prevent re-definition in zip_api.h

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

/** DSK */
typedef struct
{
	uint8_t dsk_len;	            /**< DSK length*/
    uint8_t dsk[MAX_DSK_KEY_LEN];   /**< DSK*/
} zw_dsk_t;


/** RSSI value definition */
#define RSSI_NOT_AVAILABLE      127   /**< No RSSI measurement is available*/
#define RSSI_MAX_PWR_SATURATED  126   /**< RSSI is above the maximum power*/
#define RSSI_BELOW_SENSITIVITY  125   /**< RSSI is above the maximum power*/

/** Type of IMA statistics */
#define ZW_IMA_STAT_RC      0   /**< Route change*/
#define ZW_IMA_STAT_TC      1   /**< Transmission count*/
#define ZW_IMA_STAT_NB      2   /**< Neighbor count*/
#define ZW_IMA_STAT_PEC     3   /**< Packet error count*/


/** Installation and maintenance statistics */
typedef struct
{
    uint8_t		type;   /**< Type of statistics, ZW_IMA_STAT_XXX */
    uint8_t		val;    /**< Value */
//  int             type;       /**< Type of statistics, ZW_IMA_STAT_XXX */
//  union
//  {
//      uint8_t		rc;         /**< Route change (for ZW_IMA_STAT_RC) */
//      uint8_t		tc;         /**< Transmission count (for ZW_IMA_STAT_TC) */
//      uint8_t		nb_cnt;     /**< Neighbors count (for ZW_IMA_STAT_NB) */
//      uint8_t		pec;        /**< Packet error count (for ZW_IMA_STAT_PEC) */
//
//  } stat;
} zw_ima_stat_t;

#define MAX_XTD_IMA_RPT_LEN     48  ///< Maximum Z/IP header extension IMA report length
/** Transmit completion status callback additional parameters */
typedef struct
{
    uint8_t     ima_rpt_len;                    /**< Z/IP header extension IMA report length*/
    uint8_t     ima_rpt[MAX_XTD_IMA_RPT_LEN];   /**< Buffer to store the Z/IP header extension IMA report;
                                                     this field is valid only if ima_rpt_len > 0*/
} zw_tx_cb_prm_t;

//Forward declaration of application layer context
struct _appl_layer_ctx;

///
/// Transmit completion status callback function
typedef void (*tx_cmplt_cb_t)(struct _appl_layer_ctx *appl_ctx, int8_t tx_sts, void *user_prm, uint8_t node_id, zw_tx_cb_prm_t *param);

///
/// Node list get callback function
typedef void (*node_list_get_cb_t)(struct _appl_layer_ctx *appl_ctx, uint8_t sts, uint8_t node_list_ctlr_id, uint8_t *node_list, uint8_t nl_len);

///
/// Application status callback function
typedef void (*appl_status_cb_t)(struct _appl_layer_ctx *appl_ctx, uint8_t cmd, uint8_t status);

///
/// Node info cached get callback function
typedef void (*node_info_chd_get_cb_t)(struct _appl_layer_ctx *appl_ctx, appl_node_info_t *cached_node_info);

///
/// Node endpoint count get callback function
typedef void (*ep_cnt_get_cb_t)(struct _appl_layer_ctx *appl_ctx, uint8_t node_id, uint8_t ep_cnt, uint8_t aggr_ep_cnt);

///
/// Endpont capabilities get callback function
typedef void (*ep_cap_get_cb_t)(struct _appl_layer_ctx *appl_ctx, appl_ep_info_t *ep_info);

///
/// Get members of an aggregated endpoint callback function
typedef void (*ep_mbr_get_cb_t)(struct _appl_layer_ctx *appl_ctx, uint8_t node_id, uint8_t ep_id, uint8_t member_cnt, uint8_t *members);

///
/// Add node to network callback function
typedef void (*add_node_nw_cb_t)(struct _appl_layer_ctx *appl_ctx, appl_node_add_cb_prm_t *cb_prm);

///
/// Controller change callback function
typedef void (*ctlr_chg_nw_cb_t)(struct _appl_layer_ctx *appl_ctx, appl_node_info_t *node_info);

///
/// Remove node callback function
typedef void (*rm_node_nw_cb_t)(struct _appl_layer_ctx *appl_ctx, uint8_t sts, uint8_t org_node_id);

///
/// Set learn mode callback function
typedef void (*set_lrn_mod_cb_t)(struct _appl_layer_ctx *appl_ctx, appl_lrn_mod_sec2_t *cb_prm);

///
/// Get Z/IP gateway DSK callback function
typedef void (*get_dsk_cb_t)(struct _appl_layer_ctx *appl_ctx, uint8_t *dsk, uint8_t dsk_len, int type);

///
/// Remove failed node callback function
typedef void (*rm_failed_node_cb_t)(struct _appl_layer_ctx *appl_ctx, uint8_t sts, uint8_t node_id);

///
/// Replace failed node callback function
//typedef void (*rplc_failed_node_cb_t)(struct _appl_layer_ctx *appl_ctx, uint8_t sts, uint8_t node_id);

///
/// Request node neighbor update callback function
typedef void (*req_node_nbr_updt_cb_t)(struct _appl_layer_ctx *appl_ctx, uint8_t sts, uint8_t node_id);

///
/// Assign/delete return route completion callback function
typedef void (*ret_route_cb_t)(struct _appl_layer_ctx *appl_ctx, uint8_t sts, uint8_t src_node_id);

///
/// Request network topology update callback function
typedef void (*req_nw_updt_cb_t)(struct _appl_layer_ctx *appl_ctx, uint8_t sts);

///
/// Set default callback function
typedef void (*set_deflt_cb_t)(struct _appl_layer_ctx *appl_ctx, uint8_t sts, uint8_t nodeid);

///
/// Node advertisement callback function
typedef void (*node_advt_cb_t)(struct _appl_layer_ctx *appl_ctx, nd_advt_rpt_t *rpt);

#ifdef TCP_PORTAL
///
/// Network error callback function
typedef void (*net_err_cb_t)(void *user);

#endif

///
/// Firmware version report callback function
typedef void (*fw_ver_cb_t)(uint8_t major, uint8_t minor, void *user);

///
/// Firmware update status report callback function
typedef void (*fw_sts_cb_t)(uint8_t status, void *user);


typedef void (*node_sts_cb_t)(struct _appl_layer_ctx *appl_ctx, uint8_t node_id, uint8_t sts);
/**<
Callback function to report node alive status change
@param[in]	appl_ctx    The application layer context
@param[in]	node_id	    Node
@param[in]	sts		    Status (APPL_NODE_STS_XXX)
@return
*/


typedef void (*appl_command_hdlr_fn)(struct _appl_layer_ctx *appl_ctx, struct sockaddr *src_addr,
                                     frm_ep_t *ep_addr, uint8_t *cmd_buf, uint16_t cmd_len,
                                     uint16_t flag, uint16_t encap_fmt);
/**<
application command handler callback function
@param[in]	appl_ctx    context
@param[in]	src_addr    source address of the received command
@param[in]	ep_addr     destination endpoint of the received command
@param[in]	cmd_buf	    buffer to stored the received command
@param[in]	cmd_len	    length of buf
@param[in]	flag	    flag, see ZIP_FLAG_XXX
@param[in]	encap_fmt   Z-wave encapsulation format (valid if flag ZIP_FLAG_ENCAP_INFO is set).
                        This format should be passed back when responding
                        to unsolicited command with the flag ZIP_FLAG_ENCAP_INFO is set.

@return
*/

typedef void (*zw_pl_info_iter_fn)(struct _appl_layer_ctx *appl_ctx, uint8_t remain_cnt, zw_dsk_t *dsk, pl_info_t *info, uint8_t info_cnt);
/**<
callback to report provisioning list entry iteration information
@param[in]	appl_ctx    context
@param[in]	remain_cnt	Remaining counter
@param[in]	dsk         DSK
@param[in]	info		Buffer to store additional information of the device (optional).
@param[in]	info_cnt    Number of additional information stored in "info".
*/

typedef void (*zw_pl_info_fn)(struct _appl_layer_ctx *appl_ctx, zw_dsk_t *dsk, pl_info_t *info, uint8_t info_cnt);
/**<
callback to report provisioning list entry information
@param[in]	appl_ctx    context
@param[in]	dsk         DSK
@param[in]	info		Buffer to store additional information of the device (optional).
@param[in]	info_cnt    Number of additional information stored in "info".
*/

typedef void (*zw_ss_started_fn)(struct _appl_layer_ctx *appl_ctx, zw_dsk_t *dsk);
/**<
callback to report Smart Start join started
@param[in]	appl_ctx    context
@param[in]	dsk         DSK
*/

#ifdef PL_S2_GRANT_MODE_API
typedef void (*zw_pl_s2_grnt_mode_fn)(struct _appl_layer_ctx *appl_ctx, uint8_t mode);
/**<
callback to report S2 keys grant mode setting
@param[in]	appl_ctx    context
@param[in]	mode		1 = ask for confirmation; 0 = do not ask for confirmation
*/
#endif

typedef void (*zw_ima_stat_fn)(struct _appl_layer_ctx *appl_ctx, zw_ima_stat_t *stat, uint8_t stat_cnt, uint8_t node_id);
/**<
callback to report installation and maintenance statistics of a node
@param[in]	appl_ctx    context
@param[in]	stat        statistics array
@param[in]	stat_cnt	number of element in 'stat' array
@param[in]	node_id     node id
*/

typedef void (*zw_gw_rssi_fn)(struct _appl_layer_ctx *appl_ctx, int8_t *rssi, uint8_t rssi_cnt);
/**<
callback to report background RSSI from Z/IP gateway
@param[in]	appl_ctx    context
@param[in]	rssi        RSSI, valid values are -32 to -94 and RSSI_XXX; first value is for channel 0, second value is for channel 1, etc.
@param[in]	rssi_cnt	number of RSSI in the rssi array
*/

typedef void (*zw_inif_fn)(struct _appl_layer_ctx *appl_ctx, zw_dsk_t *dsk);
/**<
callback to report included node information frame (INIF)
@param[in]	appl_ctx    context
@param[in]	dsk         DSK
*/

///
/// Send node information frame parameters
typedef struct
{
    uint8_t         fr_node_id;   ///< Node that sends the node information frame; zero for Z/IP gateway
    uint8_t         to_node_id;   ///< Desination node to receive the node information frame. 0xFF = broadcast
    uint8_t         tx_opt;       ///< Transmit options TRANSMIT_OPTION_XXX
    tx_cmplt_cb_t   cb;           ///< The callback function on transmit completion
    void            *cb_prm;      ///< Callback function

} appl_snd_nif_t;

///
/// Status callback request
typedef struct
{
    tx_cmplt_cb_t   cb;             ///< The status callback function
    void            *user_prm;      ///< The parameters of cb
    int8_t          tx_sts;         ///< Transmit completion status
    uint8_t         node_id;        ///< Node id
    zw_tx_cb_prm_t  param;          ///< Additional parameters
} appl_cb_req_t;

/// Send data transmit completion timeout callback info
typedef struct
{
    tx_cmplt_cb_t   send_data_cb;     ///< callback function
    void            *snd_dat_cb_prm;  ///< callback user parameter
    uint8_t         node_id;          ///< destination node
    uint8_t         seq_num;          ///< sequence number of frame to the specific destination node
} appl_tmout_ctx_t;

/// Z/IP frame sequence number
typedef struct
{
    uint16_t  seq_num_valid;    ///< flag to indicate this sequence number for the Z/IP frame is valid or in use
    uint8_t   node_id;          ///< destination node
    uint8_t   wkup_poll;        ///< flag to indicate this sequence number is assigned for wakeup notification polling
} appl_zip_seq_num_t;

///
/// Z-wave HCI application layer context
typedef struct _appl_layer_ctx
{
    //Callback functions to high-level application layer
    appl_command_hdlr_fn    application_command_handler_cb;///< application command handler
    node_sts_cb_t           node_sts_cb;    ///< Node alive status change callback function
    node_list_get_cb_t      nodelst_cb;     ///< Unsolicited node list callback function
    add_node_nw_cb_t        add_node_sts_cb;///< Unsolicited add node status callback function
    tx_cmplt_cb_t           old_sts_cb;     ///< Callback function to receive old send status
    appl_status_cb_t        appl_sts_cb;    ///< Application status callback function
    zw_ss_started_fn        ss_started_cb;  ///< Smart Start join started callback
    zw_inif_fn              inif_cb;        /**< Included node information frame (INIF) callback when a Smart Start device
                                            (which has joined a foreign network but it's listed in the local provisioning list) is powering up */
    zw_pl_info_fn           pl_entry_cb;    ///< Unsolicited provisioning list callback function

    /** Callback function information for Z/IP network management operations */
    nm_cb_info_t    nm_cb_info[NM_OP_TOTAL];


    //Data
    void      *data;                ///< For high-level application layer to store data/context
    struct plt_mtx_t *snd_mtx;      ///< Mutex for sending command
    struct plt_mtx_t *zip_mtx;      ///< Mutex for zip network management operation
    struct plt_mtx_t *zip_seq_num_mtx;///< Mutex for zip sequence number
    void      *snd_cv;              ///< Condition variable for sending command
    void      *cb_tmr_ctx;          ///< Callback waiting timer context
    void      *old_sts_prm;         ///< Old send status callback user-defined parameter
    node_advt_cb_t node_advt_cb;    ///< Node advertisement callback function
	int	      multi_client;         ///< Flag to indicate whether to support multiple Z/IP clients
    uint32_t  cb_tmout_ms;          ///< Callback timeout from session layer in milliseconds
    uint8_t   node_slp[232+1];      ///< Nodes capability to sleep
    uint8_t   node_sts[232+1];      ///< Nodes status (APPL_NODE_STS_XXX)
    uint8_t   pl_dsk[MAX_DSK_KEY_LEN];///<Saved DSK for provision get request
    uint8_t   pl_seq_num;           ///< Saved sequence number for provision get request
    uint8_t   ctl_node_id;          ///< Controller's node id
    struct plt_mtx_t *cb_thrd_mtx;  ///< Mutex for status callback thread
    void      *cb_thrd_sem;         ///< Semaphore for waiting of status callback requests
    util_lst_t   *cb_req_hd;        ///< Head of linked list for status callback requests
    int       use_ipv4;             ///< Flag to indicate whether to use IPv4 as transport IP protocol
    volatile int cb_thrd_run;       ///< Control the callback thread whether to run. 1 = run, 0 = stop
    volatile int cb_thrd_sts;       ///< Callback thread status. 1 = run, 0 = thread exited
    appl_zip_seq_num_t zip_seq_num_db[256];///< Sequence number database for the Z/IP frame
    uint8_t   zip_next_seq_num;     ///< Next Z/IP sequence number
    uint8_t   zip_op_seq_num;       ///< Sequence number for the Z/IP network management operation
    volatile int  wait_cmd_cb;      ///< Flag to indicate whether to wait for command callback
                                    ///< in order for the next command to be sent.
    volatile int  wait_nm_cb;       ///< Flag to indicate whether to wait for network management command callback
                                    ///< in order for the next command to be sent.
    appl_tmout_ctx_t  snd_tmout_ctx;///< Send data transmit completion status timeout callback info (used by timer callback only)
    ssn_layer_ctx_t   ssn_ctx;      ///< Session layer context
    plt_ctx_t       *plt_ctx;       ///< Platform context
    node_ipaddr_t   node_ip_tbl[MAX_ZWAVE_NODE+1];    ///< Node-IP address translation table
    node_ipaddr_t   node_ip_cache;  ///< Cache the last query of Node-IP address translation

} appl_layer_ctx_t;

#ifdef TCP_PORTAL

/// Initialization parameters for portal
typedef struct
{
    int             portal_fd;    ///< File descriptor to connect to Z/IP gateway/router using TLS
    void            *portal_ssl;  ///< SSL object pointer to connect to Z/IP gateway/router using TLS
    tls_clnt_prof_t *portal_prof; ///< Profile of the Z/IP gateway/router that is connected to the portal
    uint8_t         *zip_router;  ///< Z/IP gateway address
    net_err_cb_t    err_cb;       ///< Callback when there is unrecoverable network error occurred
    void            *user;        ///< User parameter that will passed when err_cb is invoked
    int             multi_client; ///< Flag to indicate whether to support multiple Z/IP clients
} appl_portal_prm_t;


int     zwhci_init(appl_layer_ctx_t *appl_ctx, appl_portal_prm_t *init_prm);
/**<
Init the application layer.
Should be called once before calling the other application layer functions
@param[in,out]	appl_ctx    Context
@param[in]	init_prm        Initialization parameters
@return     Return zero indicates success, non-zero indicates failure.
@post       Caller should call zwhci_shutdown(), followed by zwhci_exit() to properly clean up this layer.
*/
#else

/// Initialization parameters for stand-alone Z/IP client
typedef struct
{
    uint8_t         *zip_router;  ///< Z/IP gateway address
    uint16_t        host_port;    ///< Host listening and sending port
    int16_t         use_ipv4;     ///< Flag to indicate whether to use IPv4 as transport IP protocol
    uint8_t         *dtls_psk;    ///< DTLS pre-shared key
    uint8_t         dtls_psk_len; ///< DTLS pre-shared key length (in bytes)
    int             multi_client; ///< Flag to indicate whether to support multiple Z/IP clients
} appl_ce_prm_t;

int     zwhci_init(appl_layer_ctx_t *appl_ctx, appl_ce_prm_t *init_prm);
/**<
Init the application layer.
Should be called once before calling the other application layer functions
@param[in,out]	appl_ctx		Context
@param[in]	    init_prm        Initialization parameters
@return     Return zero indicates success, non-zero indicates failure.
@post       Caller should call zwhci_shutdown(), followed by zwhci_exit() to properly clean up this layer.
*/
#endif

void zwhci_shutdown(appl_layer_ctx_t *appl_ctx);
/**<
Shutdown the application layer, stop sending any data
@param[in]	appl_ctx    Context
@return
*/

void    zwhci_exit(appl_layer_ctx_t *appl_ctx);
/**<
Clean up the application layer
@param[in,out]	appl_ctx		Context
@return
*/

int     zw_send_data(appl_layer_ctx_t *appl_ctx, appl_snd_data_t  *prm, tx_cmplt_cb_t cb, void *cb_prm);
/**<
Send data to a node
@param[in]	appl_ctx		Context
@param[in]	prm             The parameters
@param[in]	cb              The callback function on transmit completion
@param[in]	cb_prm          The parameter to be passed when invoking cb callback function
@return  0 on success, negative error number on failure
*/

int     zip_node_list_get(appl_layer_ctx_t *appl_ctx, node_list_get_cb_t cb);
/**<
Get node list from Z/IP gateway through callback function
@param[in]	appl_ctx		Context
@param[in]	cb              The callback function for receiving node list
@return  0 on success, negative error number on failure
*/

int     zip_node_info_chd_get(appl_layer_ctx_t *appl_ctx, node_info_chd_get_cb_t cb, uint8_t nodeid, uint8_t max_age);
/**<
Get cached node info from Z/IP gateway through callback function
@param[in]	appl_ctx		Context
@param[in]	cb              The callback function for receiving node info
@param[in]	nodeid          Node id
@param[in]	max_age         The maximum age of the NodeInfo frame, given in 2^n minutes.
                            If the cache entry does not exist or if it is older than the value given in this field,
                            the ZipRouter will attempt to get a Fresh NodeInfo frame before
                            responding to the Node Info Cached Get command. A value of 15 means infinite,
                            i.e. No Cache Refresh.
@return  0 on success, negative error number on failure
*/

int zip_ep_cnt_get(appl_layer_ctx_t *appl_ctx, ep_cnt_get_cb_t cb, uint8_t nodeid);
/**<
Get number of endpoints supported by the node through callback function
@param[in]	appl_ctx		Context
@param[in]	cb              The callback function for receiving node endpoint info
@param[in]	nodeid          Node id
@return  0 on success, negative error number on failure
*/

int zip_ep_cap_get(appl_layer_ctx_t *appl_ctx, ep_cap_get_cb_t cb, uint8_t nodeid, uint8_t epid);
/**<
Get endpoint capabilities through callback function
@param[in]	appl_ctx    Context
@param[in]	cb          Callback function
@param[in]	nodeid      Node id
@param[in]	epid        Endpoint id
@return  0 on success, negative error number on failure
*/

int zip_aggr_ep_mbr_get(appl_layer_ctx_t *appl_ctx, ep_mbr_get_cb_t cb, uint8_t nodeid, uint8_t epid);
/**<
Get members of an aggregated endpoint through callback function
@param[in]	appl_ctx    Context
@param[in]	cb          Callback function
@param[in]	nodeid      Node id
@param[in]	epid        Endpoint id
@return  0 on success, negative error number on failure
*/

int     zip_node_id_get(appl_layer_ctx_t *appl_ctx, node_advt_cb_t cb, uint8_t *tgt_ip, int retry);
/**<
Resolve IPv6 address into node id through callback function
@param[in]	appl_ctx		Context
@param[in]  tgt_ip          Target IPv6 address of the Z-wave node
@param[in]	cb              The callback function for receiving node id
@param[in]	retry           Flag to indicate if this call is second attempt to get node id
@return  0 on success, negative error number on failure
*/

int     zip_node_ipv6_get(appl_layer_ctx_t *appl_ctx, node_advt_cb_t cb, uint8_t node_id);
/**<
Resolve a node id into IPv6 address through callback function
@param[in]	appl_ctx		Context
@param[in]  node_id         Z-wave node id
@param[in]	cb              The callback function for receiving node list
@return  0 on success, negative error number on failure
*/

int     zw_set_default(appl_layer_ctx_t *appl_ctx, set_deflt_cb_t cb, uint8_t nodeid);
/**<
Set the Controller back to the factory default state
@param[in]	appl_ctx		Context
@param[in]	nodeid          Node id. If zero, the destination is Z/IP gateway
@param[in]	cb              The callback function to report completion.
@return  0 on success, negative error number on failure
*/

int     zw_set_learn_mode(appl_layer_ctx_t *appl_ctx, set_lrn_mod_cb_t cb, uint8_t nodeid, uint8_t  mode);
/**<
Set learn mode, used to add or remove the controller to/from a Z-Wave network.
@param[in]	appl_ctx		Context
@param[in]	nodeid          Node id. If zero, the destination is Z/IP gateway
@param[in]	mode            Mode (ZW_SET_LEARN_MODE_XXX) could be to start the learn mode in classic or NWI or to stop the learn mode.
@param[in]	cb              The callback function to report status.
@return  0 on success, negative error number on failure
*/

int     zip_node_info_send(appl_layer_ctx_t *appl_ctx, appl_snd_nif_t  *nif_prm);
/**<
Send node information from a node to another node or all nodes.
@param[in]	appl_ctx    Context
@param[in]	nif_prm     Send node information frame parameters
@return  0 on success, negative error number on failure
*/

int     zw_request_network_update(appl_layer_ctx_t *appl_ctx, req_nw_updt_cb_t cb);
/**<
Request network topology updates from the SUC/SIS node.
Secondary controllers can only use this call when a SUC is present in the network.
All controllers can use this call in case a SUC ID Server (SIS) is available.
Routing Slaves can only use this call, when a SUC is present in the network.
@param[in]	appl_ctx		Context
@param[in]	cb              The callback function on completion.
@return  0 on success, negative error number on failure.
*/

int    zw_sec2_key_set(appl_layer_ctx_t *appl_ctx, uint8_t granted_key, uint8_t grant_csa, uint16_t flag);
/**<
Security 2 key set
@param[in]	appl_ctx		Context
@param[in]	granted_key     Granted key
@param[in]	grant_csa       Grant client-side authentication (CSA); 1=grant, 0=reject
@param[in]  flag            Send packet flag, see ZIP_FLAG_XXX
@return  0 on success, negative error number on failure
*/

int    zw_sec2_dsk_set(appl_layer_ctx_t *appl_ctx, int accept, uint8_t dsk_len, uint8_t *dsk, uint16_t flag);
/**<
Security 2 dsk set
@param[in]	appl_ctx	Context
@param[in]	accept      Flag to indicate whether to accept the joining node DSK
@param[in]	dsk_len     Number of bytes (max. 16 bytes) of DSK to send.
@param[in]	dsk         DSK to send
@param[in]  flag        Send packet flag, see ZIP_FLAG_XXX
@return  0 on success, negative error number on failure
*/

#define GW_DSK_TYPE_JOIN    0   /**< Static Z/IP gateway DSK key for joining other network*/
#define GW_DSK_TYPE_ADD     1   /**< Dynamic Z/IP gateway DSK key for adding new node*/
#define GW_DSK_TYPE_UNKNOWN 0xFF/**< Unknown Z/IP gateway DSK key type*/

int    zw_sec2_dsk_get(appl_layer_ctx_t *appl_ctx, get_dsk_cb_t cb, int type);
/**<
Get Z/IP gateway DSK
@param[in]	appl_ctx		Context
@param[in]	cb              The callback function to report Z/IP gateway DSK.
@param[in]	type            Z/IP gateway DSK type, see GW_DSK_TYPE_XXX
@return  0 on success, negative error number on failure
*/

int     zw_add_node_to_network(appl_layer_ctx_t *appl_ctx, add_node_nw_cb_t cb, uint8_t  mode, uint8_t tx_opt);
/**<
Add node to a network
@param[in]	appl_ctx		Context
@param[in]	cb              The callback function to report status.
@param[in]	mode            Mode (ADD_NODE_XXX) to control the add node process.
@param[in]	tx_opt          Transmit options (TRANSMIT_OPTION_XXX) to control if transmissions must use special properties.
@return  0 on success, negative error number on failure
*/

int     zw_remove_node_from_network(appl_layer_ctx_t *appl_ctx, rm_node_nw_cb_t cb, uint8_t  mode);
/**<
Remove node from a network
@param[in]	appl_ctx		Context
@param[in]	cb              The callback function to report status.
@param[in]	mode            Mode (REMOVE_NODE_XXX) to control the remove node process.
@return  0 on success, negative error number on failure
*/

int     zw_remove_failed_node_id(appl_layer_ctx_t *appl_ctx, rm_failed_node_cb_t cb, uint8_t node_id);
/**<
Remove a non-responding node from the routing table in the requesting controller.
@param[in]	appl_ctx		Context
@param[in]	node_id         The Node ID (1...232) of the failed node to be deleted.
@param[in]	cb              The callback function on remove process completion.
@return  0 on success, negative error number on failure.
@pre    The node must be on the failed node ID list and as an extra precaution also fail to respond before it is removed.
*/

int     zw_replace_failed_node(appl_layer_ctx_t *appl_ctx, add_node_nw_cb_t cb, uint8_t node_id, uint8_t tx_opt, uint8_t mode);
/**<
Replace a non-responding node with a new one in the requesting controller.
@param[in]	appl_ctx		Context
@param[in]	cb              The callback function on replace process completion.
@param[in]	node_id         The Node ID (1...232) of the failed node to be deleted.
@param[in]	tx_opt          Transmit options: TRANSMIT_OPTION_LOW_POWER
@param[in]	mode            Mode to control start (START_FAILED_NODE_REPLACE)/stop (STOP_FAILED_NODE_REPLACE)
@return  0 on success, negative error number on failure. If APPL_OPER_NOT_STARTED is returned, check the resp_flg output.
*/

int     zw_request_node_neighbor_update(appl_layer_ctx_t *appl_ctx, req_node_nbr_updt_cb_t cb, uint8_t node_id);
/**<
Get the neighbors from the specified node. Can only be called by a primary/inclusion controller.
@param[in]	appl_ctx		Context
@param[in]	node_id         The Node ID (1...232) of the node that the controller wants to get new neighbors from.
@param[in]	cb              The callback function with the status of request.
@return  0 on success, negative error number on failure.
*/

int     zw_controller_change(appl_layer_ctx_t *appl_ctx, ctlr_chg_nw_cb_t cb, uint8_t  mode, uint8_t tx_opt);
/**<
Add a controller to the Z-Wave network and transfer the role as primary controller to it.
The controller invoking this function will become secondary.
@param[in]	appl_ctx		Context
@param[in]	cb              The callback function to report status.
@param[in]	mode            Mode (CONTROLLER_CHANGE_XXX)to start/stop operation.
@param[in]	tx_opt          Transmit options (TRANSMIT_OPTION_EXPLORE  and/or TRANSMIT_OPTION_LOW_POWER)
@return  0 on success, negative error number on failure
*/

int     zw_assign_return_route(appl_layer_ctx_t *appl_ctx, uint8_t src_node_id, uint8_t dst_node_id, ret_route_cb_t cb);
/**<
Assign static return routes (up to 4) to a Routing Slave node or Enhanced Slave node.
@param[in]	appl_ctx		Context
@param[in]	src_node_id     The Node ID (1...232) of the routing slave that should get the return routes.
@param[in]	dst_node_id     The Destination node ID (1...232).
@param[in]	cb              The callback function.
@return  0 on success, negative error number on failure
*/

int     zw_delete_return_route(appl_layer_ctx_t *appl_ctx, uint8_t node_id, ret_route_cb_t cb);
/**<
Delete all static return routes from a Routing Slave or Enhanced Slave.
@param[in]	appl_ctx		Context
@param[in]	node_id         The Node ID (1...232) of the routing slave node.
@param[in]	cb              The callback function.
@return  0 on success, negative error number on failure
*/

int     zip_node_ipaddr_set(appl_layer_ctx_t *appl_ctx, uint8_t *ipaddr, uint8_t ipaddr_sz, uint8_t node_id);
/**<
Add a node-IP address entry into the translation table
@param[in]	appl_ctx		Context
@param[in]	ipaddr          IP address
@param[in]	ipaddr_sz       IP address size in bytes
@param[in]  node_id         Z-wave node id
@return  1 on success, zero on failure
*/

int     zip_node_ipaddr_rm(appl_layer_ctx_t *appl_ctx, uint8_t node_id);
/**<
Remove a node-IP address entry from the translation table
@param[in]	appl_ctx		Context
@param[in]  node_id         Z-wave node id
@return  1 on success, zero on failure
*/

int     zip_translate_ipaddr(appl_layer_ctx_t *appl_ctx, uint8_t *ipaddr, uint8_t ipaddr_sz, uint8_t *node_id);
/**<
Resolve an IP address to the node id of a node
@param[in]	appl_ctx		Context
@param[in]	ipaddr          IP address
@param[in]	ipaddr_sz       IP address size in bytes
@param[out] node_id         Z-wave node id
@return  1 on success, zero on failure
*/

void    zip_close_dtls_conn(appl_layer_ctx_t *appl_ctx, uint8_t node_id);
/**<
Close all DTLS connections
@param[in]	appl_ctx		Context
@param[in]	node_id         Node identifier; zero for Z/IP gateway
@return
*/

void    appl_updt_node_sts(appl_layer_ctx_t *appl_ctx, uint8_t node_id, uint8_t sts, int cb_on_change);
/**<
Update node status
@param[in]	appl_ctx		Context
@param[in]	node_id		    Node
@param[in]	sts		        Status (APPL_NODE_STS_XXX)
@param[in]	cb_on_change    Flag to determine whether to callback to registered function on status change
@return
*/

uint8_t appl_get_node_sts(appl_layer_ctx_t *appl_ctx, uint8_t node_id);
/**<
Get node status
@param[in]	appl_ctx		Context
@param[in]	node_id		    Node id (ranging from 1 to 232)
@return node status (APPL_NODE_STS_XXX)
*/

void    appl_get_all_node_sts(appl_layer_ctx_t *appl_ctx, uint8_t *node_sts_buf);
/**<
Get all node status
@param[in]	appl_ctx		Context
@param[out]	node_sts_buf	Buffer (min. size of 233 bytes)to store all the node status. Individual node status
                            can be access using the node id as index to the buffer.
@return
*/

void    appl_updt_node_slp_cap(appl_layer_ctx_t *appl_ctx, uint8_t node_id, uint8_t slp_cap);
/**<
Update node sleeping capability
@param[in]	appl_ctx		Context
@param[in]	node_id		    Node
@param[in]	slp_cap		    Sleeping capability. 1=capable to sleep; 0=otherwise
@return
*/


void  zip_old_status_cb_set(appl_layer_ctx_t *appl_ctx, tx_cmplt_cb_t cb, void *cb_prm);
/**<
Set callback function to receive old send status
@param[in]	appl_ctx		Context
@param[in]	cb              The callback function to receive old send status
@param[in]	cb_prm          The parameter to be passed when invoking cb callback function
@return
*/

void appl_seq_num_rm_by_nodeid(appl_layer_ctx_t *appl_ctx, uint8_t node_id);
/**<
Remove Z/IP frame sequence numbers by node id
@param[in]	appl_ctx    Context
@param[in]	node_id     Node id. If 0xFF, all sequence numbers will be removed
@return
*/

int  zw_pl_set(appl_layer_ctx_t *appl_ctx, uint8_t *dsk, uint8_t dsk_len,
               pl_info_t *info, uint8_t info_cnt);
/**<
Provisioning list set
@param[in]	appl_ctx	Context
@param[in]	dsk		    Device Specific Key (DSK)
@param[in]	dsk_len		Device Specific Key length
@param[in]	info		Buffer to store additional information of the device (optional).
@param[in]	info_cnt    Number of additional information stored in "info".
@return  0 on success, negative error number on failure
*/

int  zw_pl_del(appl_layer_ctx_t *appl_ctx, uint8_t *dsk, uint8_t dsk_len);
/**<
Provisioning list delete
@param[in]	appl_ctx	Context
@param[in]	dsk		    Device Specific Key (DSK)
@param[in]	dsk_len		Device Specific Key length. If zero, all entries will be deleted.
@return  0 on success, negative error number on failure
*/

int  zw_pl_iter_get(appl_layer_ctx_t *appl_ctx, uint8_t remain_cnt, zw_pl_info_iter_fn cb);
/**<
Get provisioning list entry iteration through callback
@param[in]	appl_ctx	Context
@param[in]	remain_cnt	Remaining counter
@param[in]	cb          Callback to report provisioning list entry info
@return  0 on success, negative error number on failure
*/

int  zw_pl_get(appl_layer_ctx_t *appl_ctx, uint8_t *dsk, uint8_t dsk_len, zw_pl_info_fn cb);
/**<
Get provisioning list entry through callback
@param[in]	appl_ctx	Context
@param[in]	dsk		    Device Specific Key (DSK)
@param[in]	dsk_len		Device Specific Key length
@param[in]	cb          Callback to report provisioning list entry info
@return  0 on success, negative error number on failure
*/

#ifdef PL_S2_GRANT_MODE_API
int zw_pl_s2_grnt_mode_get(appl_layer_ctx_t *appl_ctx, zw_pl_s2_grnt_mode_fn cb);
/**<
Get S2 keys grant mode setting
@param[in]	appl_ctx	Context
@param[in]	cb          The callback function to report S2 keys grant mode setting
@return  0 on success, negative error number on failure
*/

int  zw_pl_s2_grnt_mode_set(appl_layer_ctx_t *appl_ctx, uint8_t mode);
/**<
Set whether to ask for confirmation before granting S2 keys to non-Smart Start node in provisioning list
@param[in]	appl_ctx	Context
@param[in]	mode		1 = ask for confirmation; 0 = do not ask for confirmation
@return  0 on success, negative error number on failure
*/
#endif

int zw_ima_stat_get(appl_layer_ctx_t *appl_ctx, zw_ima_stat_fn cb, uint8_t node_id);
/**<
Get installation and maintenance statistics from a node
@param[in]	appl_ctx		Context
@param[in]	node_id         Node id
@param[in]	cb              The callback function to report the statistics.
@return  0 on success, negative error number on failure
*/

int zw_gw_rssi_get(appl_layer_ctx_t *appl_ctx, zw_gw_rssi_fn cb);
/**<
Get the measured background RSSI from Z/IP gateway
@param[in]	appl_ctx		Context
@param[in]	cb              The callback function to report the statistics.
@return  0 on success, negative error number on failure
*/

/**
@}
*/

#endif /* _ZIP_APPLICATION_DAVID_ */
