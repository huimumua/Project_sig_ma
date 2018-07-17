/**
@file   zip_api_pte.h - Z/IP host controller API header file.

        Z/IP API private definitions.

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

#ifndef _ZIP_API_PTE_DAVID_
#define _ZIP_API_PTE_DAVID_

#include <stdint.h>
#include <time.h>

/* forward references */
struct _zwnet;
struct _zwnode;
struct _zwep;
struct _zwif;
typedef struct _zwnet	*zwnet_p;		/**< network handle */
typedef struct _zwnode	*zwnode_p;		/**< node handle */
typedef struct _zwep	*zwep_p;		/**< endpoint handle */
typedef struct _zwif	*zwif_p;		/**< interface handle */

#include "zip_api.h"

/** min/max macro */
#define zwmin(a, b)  (((a)<(b))?(a):(b))
#define zwmax(a, b)  (((a)>(b))?(a):(b))

/**
@defgroup Base Base object APIs
To be embedded by other structure as object
@ingroup zwarecapi
@{
*/

#define MAX_ZWAVE_PKT_SIZE  44  /**< Maximum Z-wave packet size */

/** Maximum payload size for S2, S0 and unsecure packet*/
#define PKT_S2_MAX_SZ       26  ///< S2 packet maximum payload size
#define PKT_S0_MAX_SZ       46  ///< S0 packet maximum payload size
#define PKT_ZW_MAX_SZ       44  ///< Z-wave packet maximum payload size


/** Z-Wave base object for linking and user context */
typedef struct _zwobj
{
	struct _zwobj	*next;		/**< next object in list */
	void			*ctx;		/**< user context (opaque to server) */
}
zwobj_t, *zwobj_p;


void zwobj_add(zwobj_p *head, zwobj_p obj);
/**<
Add object to list
@param[in,out]	head	list head
@param[in]		obj		object
*/

int32_t zwobj_del(zwobj_p *head, zwobj_p obj);
/**<
Remove object from list
@param[in,out]	head	list head
@param[in]      obj     object
@return		Non-zero on success; otherwise zero if object not found in the list.
*/

int32_t zwobj_rplc(zwobj_p *head, zwobj_p obj, zwobj_p new_obj);
/**<
Replace an object with a new one
@param[in,out]	head	list head
@param[in]      obj     object to be replaced
@param[in]      new_obj new object
@return		Non-zero on success; otherwise zero if object not found in the list.
@post       The obj will be invalid on success, don't use it hereafter.
*/

/**
@}
@addtogroup EP Endpoint APIs
@{
*/

#define     VIRTUAL_EP_ID       0       /**< virtual endpoint id */

/** Z-Wave channel/instance/endpoint abstraction */
typedef struct _zwep
{
	zwobj_t		    obj;			            /**< base link object */
	struct _zwnode	*node;			            /**< back link to associated device */
	zwif_p          intf;			            /**< list of interfaces */
	uint8_t		    epid;		                /**< Endpoint id: ranging from 1 to 0xff */
	uint8_t		    generic;		            /**< Generic Device Class */
	uint8_t		    specific;		            /**< Specific Device Class */
	uint8_t		    aggr_ep_cnt;		        /**< Total number of end point members that are
                                                     represented by this aggregated end point. Zero means
                                                     this endpoint is NOT an aggregated end point*/
	uint8_t		    aggr_members[MAX_AGGR_MBR]; /**< Buffer to store the end point members that are
                                                     represented by this aggregated end point*/

    char		    name[ZW_LOC_STR_MAX + 1];   /**< user configured name string of the endpoint */
    char		    loc[ZW_LOC_STR_MAX + 1];    /**< user configured location string of the endpoint */
    zwplus_info_t   zwplus_info;                /**< Z-wave+ information */
}
zwep_t;

int zwep_get_desc(zwep_p ep, zwepd_p desc);
/**<
get endpoint descriptor
@param[in]	ep		endpoint
@param[out]	desc	descriptor
@return		ZW_ERR_XXX
*/

zwep_p zwep_get_ep(zwepd_p epd);
/**<
Get the endpoint of the specified endpoint descriptor
@param[in]	epd	    Endpoint descriptor
@return		The endpoint if found; else return NULL
@pre        Caller must lock the nw->mtx before calling this function.
*/

zwep_p zwep_get_by_id(zwnet_p nw, uint8_t node_id, uint8_t ep_id);
/**<
Get endpoint by node and endpoint id
@param[in]	nw	    Network handle
@param[in]	node_id	Node id
@param[in]	ep_id	Endpoint id
@return		The endpoint if found; else return NULL
@pre        Caller must lock the nw->mtx before calling this function.
*/

void zwep_intf_rm_all(zwep_p ep);
/**<
Remove all interfaces of an endpoint
@param[in]	ep	The endpoint
@pre        Caller must lock the nw->mtx before calling this function.
*/

zwep_p zwep_find(zwep_p first_ep, uint8_t epid);
/**<
find an endpoint based on endpoint id
@param[in]	first_ep	first endpoint in the node
@param[in]	epid	    endpoint id
@return		endpoint if found; else return NULL
@pre        Caller must lock the nw->mtx before calling this function.
*/

uint8_t zwep_count(zwnode_p node);
/**<
count the number of real endpoints in a node
@param[in]	node	Node
@return		The number of real endpoints in a node
@pre        Caller must lock the nw->mtx before calling this function.
*/

/**
@}
@addtogroup Node Node APIs
@{
*/

#define     ZWNODE_MUL_CMD_TMOUT    1000       ///< Multi command time out in milliseconds
#define     ZWNODE_WAIT_RPT_TMOUT   10000      ///< Wait for report time out in milliseconds


/** Command queue request for thread to execute */
typedef struct
{
    uint8_t     node_id;        /**< Node id to which the request should be sent*/
    util_lst_t  *req_hd;        /**< Head of the request list*/
}
cmd_q_req_t;


#define     EXEC_ACT_PROBE_FAILED_NODE  0        ///< Send NOP to failed node
#define     EXEC_ACT_RMV_FAILED         1        ///< Remove failed node
#define     EXEC_ACT_RMV_FAILED_RETRY   2        ///< Re-try to remove failed node
#define     EXEC_ACT_PROBE_RESTART_NODE 3        ///< Send NOP to restarted node
#define     EXEC_ACT_ADD_NODE_STS_CB    4        ///< Callback add node status handler
#define     EXEC_ACT_RPLC_NODE_STS_CB   5        ///< Callback replace node status handler
#define     EXEC_ACT_WKUP_NO_MORE_INFO  6        ///< Send wakeup no more information

#define     FW_UPDT_RESTART_POLL_INTERVAL   3000 ///< Firmware update target node poll interval (in ms)
#define     FW_UPDT_RESTART_POLL_MAX        10   ///< Firmware update target node poll maximum counts


/** Request for thread to execute replace node callback */
typedef struct
{
    uint16_t                rcx_flag;   /**< Received packet flag, see ZIP_FLAG_XXX */
    appl_node_rp_sec2_t     rp_dat;     /**< Node replace with security 2 support */
} zwnet_exec_req_rpS2_t;

/** Request for thread to execute add node callback */
typedef struct
{
    uint16_t                rcx_flag;   /**< Received packet flag, see ZIP_FLAG_XXX */
    appl_node_info_sec2_t   add_dat;     /**< Node add with security 2 support */
} zwnet_exec_req_addS2_t;

/** Request for thread to execute */
typedef struct
{
    uint8_t     node_id;        /**< Node id */
    uint8_t     action;         /**< Action to perform, EXEC_ACT_XXX */
    uint16_t    retry_cnt;      /**< Retry count */
    void        *user_dat;      /**< User data */
}
zwnet_exec_req_t;

/** Execute wakeup no more info context */
typedef struct
{
    int         timeout;        /**< Flag to indicate time out has occurred */
    void        *tmr_ctx;       /**< Timer context*/
    zwnet_p     nw;             /**< Network*/
}
zwnet_exec_wunmi_t;

/** Callback request type */
#define     CB_TYPE_DAT_CACHED_REPORT   0   ///< Report callback using cached data report
#define     CB_TYPE_SUP_CACHED_REPORT   1   ///< Report callback using cached supported report
#define     CB_TYPE_DB_REPORT           2   ///< Report callback using device specific configuration database
#define     CB_TYPE_NET_OP              3   ///< Network operation callback
#define     CB_TYPE_NODE                4   ///< Node status callback
#define     CB_TYPE_1_EP_SM             5   ///< Callback to single endpoint info state-machine
#define     CB_TYPE_S2_DSK              6   ///< Security 2 DSK callback
#define     CB_TYPE_S2_REQ_KEY          7   ///< Security 2 requested keys callback
#define     CB_TYPE_GW_DSK              8   ///< Z/IP Gateway DSK callback
#define     CB_TYPE_POST_SET            9   ///< Post-set polling callback
#define     CB_TYPE_SIMULATED_REPORT    10  ///< Callback report handler using simulated report (i.e. not from the network)
#define     CB_TYPE_INIF                11  ///< Unsolicited included node information frame (INIF) callback
#define     CB_TYPE_NET_ERR             12  ///< Network error callback


/**  Supported report type used for callback */
#define     CB_RPT_TYP_THRMO_FAN_MD     0   ///< Supported thermostat fan operating modes
#define     CB_RPT_TYP_THRMO_MD         1   ///< Supported thermostat modes
#define     CB_RPT_TYP_THRMO_SETPOINT   2   ///< Supported thermostat setpoint types
#define     CB_RPT_TYP_MAX_GROUP        3   ///< Maximum supported groupings
#define     CB_RPT_TYP_MUL_SWITCH       4   ///< Supported multilevel switch
#define     CB_RPT_TYP_AV               5   ///< Supported simple AV controls
#define     CB_RPT_TYP_ALARM_TYPE       6   ///< Supported alarm types
#define     CB_RPT_TYP_ALARM_EVENT      7   ///< Supported alarm events
#define     CB_RPT_TYP_PROT             8   ///< Supported protection states
#define     CB_RPT_TYP_MAX_USR_CODES    9   ///< Maximum supported user codes
#define     CB_RPT_TYP_METER            10  ///< Supported Meter
#define     CB_RPT_TYP_METER_DESC       11  ///< Meter descriptor
#define     CB_RPT_TYP_MAX_DRLOG_REC    12  ///< Maximum supported door lock logging records
#define     CB_RPT_TYP_BIN_SENSOR       13  ///< Supported binary sensor type
#define     CB_RPT_TYP_SENSOR           14  ///< Supported sensor type
#define     CB_RPT_TYP_SENSOR_UNIT      15  ///< Supported sensor unit
#define     CB_RPT_TYP_CENTRAL_SCENE    16  ///< Maximum supported scenes and supported key attributes per scene
#define     CB_RPT_TYP_ALARM_SENSOR		17  ///< Supported alarm sensor type
#define     CB_RPT_TYP_BAR_NOTIF_TYPE	18  ///< Supported barrier operator notification subsystem type
#define     CB_RPT_TYP_COL_SWITCH       19  ///< Supported color switch
#define     CB_RPT_TYP_THRMO_SETP_RANGE 20  ///< Supported thermostat setpoint temparature range
#define     CB_RPT_TYP_THRMO_OP_STA_LOG 21  ///< Supported thermostat operating state logging

#define     MAX_XTR_PRM_CNT             16  ///< Maximum extra parameters count for cached report callback

/** Supported report callback request (using cached interface data)*/
typedef struct
{
    zwifd_t     ifd;	        /**< Interface associated with the callback */
    uint16_t    rpt_type;       /**< Report type CB_RPT_TYP_XXX */
    uint16_t    extra;          /**< Extra parameter for the rpt_type*/
}
zwnet_sup_rpt_cb_req_t;

/** Data report callback request (using cached interface data)*/
typedef struct
{
    zwifd_t     ifd;	                /**< Interface associated with the callback */
    uint8_t     zw_rpt;                 /**< Z-wave report */
    uint8_t     extra_cnt;              /**< Number of extra parameter */
    uint16_t    extra[MAX_XTR_PRM_CNT]; /**< Extra parameters for the zw_rpt*/
}
zwnet_dat_rpt_cb_req_t;

/** Report callback request (using device specific configuration database)*/
typedef struct
{
    zwifd_t     ifd;            /**< Interface associated with the callback */
    uint16_t    rpt_type;       /**< Report type CB_RPT_TYP_XXX */
    uint16_t    extra;          /**< Extra parameter for the rpt_type*/
    uint8_t     if_type;        /**< Interface record type, IF_REC_TYPE_XXX*/
}
zwnet_db_rpt_cb_req_t;

/** Notify user application on network operation callback request*/
typedef struct
{
    void        *user;              /**< user context */
    uint8_t     op;                 /**< network operation ZWNET_OP_XXX */
    uint8_t     sts_info_valid;     /**< flag to indicate whether additional status information is valid */
    uint16_t    sts;                /**< status of network operation */
    zwnet_sts_t sts_info;           /**< additional status information */

}
zwnet_op_cb_req_t;

/** Notify user application on node status callback request*/
typedef struct
{
    void        *user;      /**< user context */
    int         mode;       /**< ZWNET_NODE_XXX */
    uint8_t     node_id;    /**< newly added or removed or updated node id */
}
zwnet_node_cb_req_t;

/** Single endpoint info state-machine callback request*/
typedef struct
{
    zwifd_t             ifd;    /**< Interface associated with the callback */
    int                 evt;    /**< Event*/
}
zwnet_1_ep_sm_cb_req_t;

/** Notify user application on network error*/
typedef struct
{
    void        *user;  /**< user context */
}
zwnet_net_err_cb_req_t;

/** Security 2 DSK callback parameters*/
typedef struct
{
    void        *user;          /**< user context */
	uint8_t		pin_required;	/**< Indicate whether user is required to enter 5-digit pin. 1=required; 0=not required */
    char        dsk[48];        /**< Device Specific Key (DSK) of the joining node for user to verify */
}
zwnet_dsk_cb_req_t;

/** Security 2 requested keys callback parameters*/
typedef struct
{
    void                    *user;          /**< user context */
	appl_node_keys_rpt_t    req_key;	    /**< requested keys report */
}
zwnet_req_key_cb_req_t;

/** Security 2 Z/IP gateway DSK callback parameters*/
typedef struct
{
    void    *user;      /**< user context */
    char    dsk[48];    /**< Z/IP gateway DSK */
}
zwnet_gw_dsk_cb_req_t;

/** Post-set polling callback request*/
typedef struct
{
    zwifd_t     ifd;        /**< Interface associated with the callback */
    int         reason;     /**< Reason of the callback, ZWPSET_REASON_XXX*/
    void        *user;      /**< user context */
}
zwnet_post_set_cb_req_t;

/** Simulated report callback*/
typedef struct
{
    zwifd_t     ifd;	                /**< Interface */
    uint16_t    rpt_len;                /**< Report length*/
    uint8_t     rpt[MAX_ZWAVE_PKT_SIZE];/**< Report*/
}
zwnet_sim_rpt_req_t;

/** Unsolicited included node information frame (INIF) callback parameters*/
typedef struct
{
    void            *user;      /**< user context */
    pl_lst_ent_t    lst_ent;    /**< List entry*/
}
zwnet_inif_cb_req_t;


/** Request for thread to callback*/
typedef struct
{
	void        *cb;	        /**< Callback function */
    int         type;           /**< Callback type (CB_TYPE_XXX) */
    union {
                zwnet_sup_rpt_cb_req_t  sup_rpt;
                zwnet_dat_rpt_cb_req_t  dat_rpt;
                zwnet_db_rpt_cb_req_t   db_rpt;
                zwnet_op_cb_req_t       net_op;
                zwnet_node_cb_req_t     node;
                zwnet_1_ep_sm_cb_req_t  ep_cb;
                zwnet_net_err_cb_req_t  net_err;
                zwnet_dsk_cb_req_t      dsk_cb;
                zwnet_req_key_cb_req_t  req_key;
                zwnet_gw_dsk_cb_req_t   gw_dsk;
                zwnet_post_set_cb_req_t post_set;
                zwnet_sim_rpt_req_t     sim_rpt;
                zwnet_inif_cb_req_t     inif;

          }     param;          /**< Callback parameter*/
}
zwnet_cb_req_t;


/** Multi Command queue entry for node that supports either Multi Command or Wake Up command classes */
typedef struct
{
	zwifd_t             ifd;	        /**< Interface associated with the command */
    util_lst_t          *extra;         /**< list of extra data for executing other functions*/
    uint16_t            cmd_id;         /**< interface API command id */
    uint8_t             cmd_num;        /**< the n th command class command of the interface API; start from 1, ... */
    uint8_t             wait_rpt;       /**< flag to indicate whether to wait for report (for node that requires wakeup beam) */
    uint8_t             dat_len;        /**< the length of the dat_buf field */
    uint8_t             rsvd1;          /**< Padding to make sure dat_buf is 4-byte aligned */
    uint16_t            rsvd2;          /**< Padding to make sure dat_buf is 4-byte aligned */
    uint8_t             dat_buf[1];     /**< place holder for the data buffer */
}
mul_cmd_q_ent_t;


/** Command queue extra handler parameter */
typedef struct
{
    void                *extra;         /**< extra data for executing other functions*/
    uint16_t            cmd_id;         /**< interface API command id */
    uint8_t             node_id;        /**< node id*/
}
cmd_q_xtra_t;


/** Return route for adding node to a group */
typedef struct
{
    uint8_t dst_node[4];                ///<Destination node of the return route
    uint8_t num_ent;                    ///<Number of entries in the buffer
    uint8_t rd_pos;                     ///<Read position
}
ret_route_t;


/** Command class, get command and report command table entry*/
typedef struct
{
    uint16_t    cls;        ///< Command class
    uint8_t     get_cmd;    ///< Get command
    uint8_t     rpt;        ///< Report command
}
cmd_get_resp_t;

/** Command class, set command table entry*/
typedef struct
{
    uint16_t    cls;        ///< Command class
    uint8_t     set_cmd;    ///< Set command
}
cmd_set_ent_t;


/** Z-Wave node abstraction */
typedef struct _zwnode
{
	zwobj_t		    obj;	    /**< base link object */
	struct _zwnet   *net;	    /**< back link to associated network */
	zwep_t		    ep;		    /**< head in list of endpoints */
    uint8_t		    propty;	    /**< Properties of the node (bit-mask): NODE_PROPTY_XXX */

	/* Manufacturer specific report */
    uint8_t     category;	    /**< Device category, DEV_XXX */
	uint16_t	vid;			/**< Vendor ID */
	uint16_t	vtype;			/**< Vendor Product Type */
	uint16_t	pid;			/**< Product ID */
    dev_id_t    dev_id;         /**< Device ID */

	/* Version class report */
	uint16_t	proto_ver;		/**< Z-Wave Protocol Version*/
	uint16_t	app_ver;		/**< Application Version*/
	uint8_t		lib_type;		/**< Z-Wave Library Type*/
    ext_ver_t   *ext_ver;       /**< Extended version information*/

	/* Node information report */
	uint8_t		nodeid;			/**< Node ID */
	uint8_t		sensor;		    /**< Flag to indicate whether the node is a sensor */
    uint8_t     listen;         /**< Flag to indicate whether the node is always listening (awake) */
	uint8_t		basic;			/**< Basic Device Class */

    /* Multi-channel report*/
    uint8_t		mul_ch_ver;	    /**< Multi-channel version */
	uint8_t		num_of_ep;	    /**< Number of endpoints */
    uint8_t     num_of_aggr_ep; /**< Number of aggregated endpoints */
	uint8_t		dynamic_ep;	    /**< Flag to indicate whether the number of endpoints is dynamic */
	uint8_t		ident_ep;	    /**< Flag to indicate whether the endpoints are identical */

    /* Sleeping node (wake up command class)*/
    uint16_t    cmd_id;         /**< Interface API command id */
    uint8_t     cmd_num;        /**< The n th command class command of the interface API; start from 1, ...
                                     A value of zero indicates that the command should not be queued */
    uint8_t     enable_cmd_q;   /**< Flag to enable command queuing */
    uint8_t     sleep_cap;      /**< Flag to indicate the node is capable to sleep (i.e. non-listening and support Wake up command class) */
    uint8_t     nif_pending;    /**< Flag to indicate the query of detailed node info is pending */
    int32_t     wkup_intv;      /**< Wake up interval in seconds. Negative value = invalid or unknown */

	/* Multi Command Encapsulation*/
	uint8_t		mul_cmd_cap;	/**< Flag to indicate the node is capable to encapsulate multiple commands */
    uint8_t		mul_cmd_ctl;	/**< Multi Command Encapsulation control: 0 = off, 1 = on*/
    void        *mul_cmd_tmr_ctx;/**< Multi command timer context*/

    /* Command queue for both sleeping node and multi command encapsulation*/
    util_lst_t  *mul_cmd_q_hd;  /**< List head of command queue */
    uint8_t     mul_cmd_q_cnt;  /**< The number of entries in the command queue */

	/* CRC-16 Encapsulation*/
	uint8_t		crc_cap;	    /**< Flag to indicate the node is capable to check/generate CRC-16 checksum */

	/* Supervision capability*/
	uint8_t		spv_cap;	    /**< Flag to indicate the node is capable to support supervision command class */

	/* Node that requires wakeup beam (FLIRS)*/
    util_lst_t  *wait_cmd_q_hd; /**< List head of command queue */
    void        *wait_tmr_ctx;  /**< Wait REPORT command timer context*/
    uint8_t     wait_cmd_q_cnt; /**< The number of entries in the command queue */
	uint8_t		wait_rpt_flg;	/**< Flag to indicate the node is waiting for report */
	uint16_t	wait_cmd;	    /**< The command class of the report */
	uint8_t		wait_rpt;	    /**< The report command of the report */

    /* Grouping (association command class)*/
    ret_route_t add_grp_rr;     /**< Return route to configure after the add group command */

	/* Security 2 */
	uint8_t		s2_keys_valid;  /**< Flag to indicate whether s2_grnt_keys is valid */
	uint8_t		s2_grnt_keys;   /**< Security 2 granted keys (bit-mask), see SEC_KEY_BITMSK_XXX
                                     NOTE: This is valid only s2_keys_valid = 1*/
	char		s2_dsk[MAX_DSK_STR_LEN + 1];     /**< S2 DSK. If s2_dsk[0] == '\0', the DSK is unavailable for this node */

	/* Firmware update version 3 */
    zwrep_fw_tgt_restart_fn restart_cb;     /**< Firmware update target restart completion callback function */
    int                     poll_tgt_cnt;   /**< Poll count*/

    /* Device configuration */
    int         dev_cfg_valid;  /**< Flag to indicate dev_cfg_rec is valid*/
    dev_rec_t   dev_cfg_rec;    /**< Device configuration record*/
    uint8_t     security_incl_status;     /**< node security add status */ 
    uint8_t     isNew;

}
zwnode_t;

/** Clear controller variables */
#define zwnode_ctl_clr(nw)  do{ \
                                (nw)->ctl.nodeid = 0;           \
                                (nw)->ctl.mul_ch_ver = 0;       \
                                (nw)->ctl.sleep_cap = 0;        \
                                (nw)->ctl.listen = 1;           \
                                (nw)->ctl.enable_cmd_q = 0;     \
                                (nw)->ctl.crc_cap = 0;          \
                                (nw)->ctl.mul_cmd_cap = 0;      \
                                (nw)->ctl.dev_id.len = 0;       \
                                (nw)->ctl.s2_keys_valid = 0;    \
                                (nw)->ctl.propty = 0;           \
                                (nw)->ctl.wkup_intv = -1;       \
                                (nw)->ctl.s2_dsk[0] = '\0';     \
                                (nw)->ctl.security_incl_status = 2;\
                              } while(0)


int zwnode_intf_reset(zwnode_p first_node);
/**<
reset a node intf data struct, if not do this, zwnet_load will not detailed.
@param[in]  first_node  first node in the network
*/


zwnode_p zwnode_find(zwnode_p first_node, uint8_t nodeid);
/**<
find a node based on node id
@param[in]	first_node	first node in the network
@param[in]	nodeid	    node id
@return		node if found; else return NULL
@pre        Caller must lock the nw->mtx before calling this function.
*/

int  zwnode_get_desc(zwnode_p node, zwnoded_p desc);
/**<
get device descriptor
@param[in]	node	node
@param[out]	desc	descriptor
@return		ZW_ERR_XXX
@pre        Caller must lock the nw->mtx before calling this function.
*/

void zwnode_rm(zwnet_p nw, uint8_t node_id);
/**<
Remove a node from the network.
@param[in]	nw	        The network
@param[in]	node_id     The node id to be removed
*/

void zwnode_ep_rm_all(zwnode_p node);
/**<
Remove all endpoints of a node
@param[in]	node	The node
@pre        Caller must lock the nw->mtx before calling this function.
*/

void zwnode_cmd_q_rm(zwnode_p node);
/**<
Remove all commands in command queue
@param[in]	node	The node
@pre        Caller must lock the nw->mtx before calling this function.
*/

void zwnode_cmd_q_thrd(void   *data);
/**<
Thread to serve requests to execute queued commands for sleeping nodes
@param[in]	data		Network
@return
*/

void zwnode_mul_cmd_rm_q(zwnet_p net, util_lst_t  **cmd_q_hd);
/**<
Remove and free resources in the multi-command queue
@param[in]	net	        Network
@param[in]	cmd_q_hd	Command queue list head
@return
*/

void zwnode_wait_rpt_chk(zwif_p intf, uint8_t *cmd_buf, uint16_t cmd_len);
/**<
Check whether the report is the one which the node is waiting for
@param[in]	intf	Interface
@param[in]	cmd_buf	The report command received
@param[in]	cmd_len	The length of cmd_buf
@return
*/

void zwnode_wait_q_thrd(void   *data);
/**<
Thread to serve requests to execute queued commands for nodes that require wakeup beam (FLIRS)
@param[in]	data		Network
@return
*/

void zwnode_wait_cmd_q_rm(zwnode_p node);
/**<
Remove all commands in command queue
@param[in]	node	The node
@pre        Caller must lock the nw->mtx before calling this function.
*/

int  zwnode_probe(zwnode_p node, tx_cmplt_cb_t cb, void *user);
/**<
send a "no operation" command to a node to test if it's reachable
@param[in]	node        node
@param[in]	cb		    callback function for transmit status
@param[in]	user	    user parameter of callback function
@return	ZW_ERR_xxx
*/

int zwnode_probe_by_id(zwnet_p nw, uint8_t node_id, tx_cmplt_cb_t cb, void *user, int req_ima_rpt);
/**<
send a "no operation" command to a node to test if it's reachable
@param[in]	nw          network
@param[in]	node_id     node id
@param[in]	cb		    callback function for transmit status
@param[in]	user	    user parameter of callback function
@param[in]	req_ima_rpt	flag for requesting IMA report
@return	ZW_ERR_xxx
*/


int zwnode_get_rpt(uint8_t* cmd, uint8_t len, uint16_t *cmd_cls, uint8_t *rpt);
/**<
Check the input command to find the corresponding REPORT
@param[in]	cmd		    Command
@param[in]  len	        The length of command in bytes
@param[out] cmd_cls	    The command class of the report
@param[out]	rpt	        The report
@return     1 if the corresponding report is found; otherwise return 0
*/

void zwnode_wait_tx_cb(appl_layer_ctx_t  *appl_ctx, int8_t tx_sts, void *user_prm, uint8_t node_id, zw_tx_cb_prm_t *param);
/**<
send command callback function
@param[in]	appl_ctx    The application layer context
@param[in]	tx_sts		The transmit complete status
@param[in]	user_prm    The user specific parameter
@param[in]	node_id     Node id
@param[in]	param       Additional parameters
@return
*/

int zwpoll_node_rm(zwnet_p net, uint8_t node_id);
/**<
remove polling requests that belong to the specified node
@param[in]	net	        network
@param[in]	node_id	    node id
@return		ZW_ERR_NONE if success; else ZW_ERR_XXX on error
*/

int zwnode_secure_propty_set(zwnode_p node);
/**<
Set node security properties
@param[in]	node		    Node
@return		Non-zero on set successfully; else return zero
*/

///
/// Node information specific to security 2
typedef struct
{
    int         grnt_keys_valid;    ///< flag to indicate whether the grant keys are valid
    uint8_t     grnt_keys;          ///< security 2: granted keys
    char        *dsk;               ///< optional DSK
} sec2_node_info_t;

int zwnet_node_info_update(zwnet_p nw, appl_node_info_t *node_info, sec2_node_info_t *s2_node_info);
/**<
update or create a new node in the network structure
@param[in]	nw		        Network
@param[in]	node_info       Node information
@param[in]	s2_node_info    Optional node information specific to security 2
@return		ZW_ERR_xxx.
*/


/**
@}
@addtogroup Net Network APIs
@{
*/

#define ZWNET_IP_NW_TMOUT               (APPL_WAIT_SEND_TIMEOUT + 100) ///< Network time out for sending Z/IP packet destines to Z/IP gateway and receiving response in milliseconds
#define ZWNET_NODE_INFO_TMOUT           (1000 + ZWNET_IP_NW_TMOUT) ///< Get node info time out in milliseconds
#define ZWNET_TMOUT                     (APPL_WAIT_SEND_TIMEOUT + 400) ///< Network time out for sending Z-wave packet and receiving response in milliseconds
#define ZWNET_SHORT_TMOUT               800         ///< Short timeout
#define ZWNET_FW_GET_TMOUT              26000       ///< Wait for firmware get from device in milliseconds
#define ZWNET_AV_KEEP_ALIVE_TMOUT       400         ///< "key hold" keep alive in milliseconds
#define ZWNET_NODE_NBR_UPDT_TMOUT       20000       ///< Get node neighbor update time out in milliseconds
#define ZWNET_NODE_INFO_GET_DELAY       500         ///< Get node info delay in milliseconds
#define ZWNET_N2IP_TMOUT                1500        ///< Z/IP gateway response to resolve node to ip address time out in milliseconds
#define ZWNET_N2IP_RETRY_TMOUT          2000        ///< Resolve node to ip address retry (due to invalid ip) time out in milliseconds
#define ZWNET_N2IP_RETRANSMIT_MAX       3           ///< Resolve node to ip address retransmission maximum count
#define ZWNET_N2IP_RETRY_MAX            5           ///< Resolve node to ip address retry (due to invalid ip) maximum count
#define ZWNET_NI_CACHE_GET_AGE          1           ///< Node info cache get maximum age in 2^X minutes
#define ZWNET_NI_CACHE_GET_REFRESH      0           ///< Node info cache get force refresh
#define ZWNET_NW_CHANGE_RETRY_INTV      5000        ///< Network change retry interval in milliseconds
#define ZWNET_APPL_BUSY_RETRY_INTV      2000        ///< Application busy retry interval in milliseconds
#define ZWNET_CTLR_SM_RETRY             1           ///< Controller info state-machine retry count
#define ZWNET_CTLR_BUSY_RETRY           5           ///< Controller busy retry count
#define ZWNET_VER_GET_RETRY             1           ///< Get version retry count

//Sasidhar: ZIPR is taking around 8.5 to 9 seconds to exccute the reset
//          500 series ZIPR, it is taking around  ~9.88 seconds
#define ZWNET_RST_IP_NW_TMOUT           15000       ///< Z/IP router Reset response time out in milliseconds

#define ZWNET_ZIPGW_VER_EP_NIF          0x023C      ///< Z/IP gateway version that supports endpoint info query
#define ZWNET_ZIPGW_VER_MULTI_CLIENT    0x023A      ///< Z/IP gateway version that supports multiple Z/IP clients
#define ZWNET_ZIPGW_VER_SEC_HDR_EXT     0x023A      ///< Z/IP gateway version that uses Encapsulation Format Information header option
#define ZWNET_ZIPGW_VER_HDR_EXT_LO      0x0219      ///< Z/IP gateway version that uses Encapsulation Format Information header option (lower range)
#define ZWNET_ZIPGW_VER_HDR_EXT_HI      0x0231      ///< Z/IP gateway version that uses Encapsulation Format Information header option (upper range)
#define ZWNET_ZIPGW_VER_IMA             0x0250      ///< Z/IP gateway version that supports IMA

///
/// Network initialization state-machine events
typedef enum
{
    EVT_INI_TMOUT,                       ///< Timeout
    EVT_INI_START,                       ///< Start network initialization
    EVT_INI_CTLR_INFO_DONE,              ///< The process of getting Z/IP router controller info done
    EVT_INI_CTLR_UNSOLICIT,              ///< Received Z/IP router unsolicited destination address
    EVT_INI_NODE_ID_ADDR,                ///< Received node-ip address discovery completion status
    EVT_INI_NODE_LIST,                   ///< Received network node list
    EVT_INI_NODE_LIST_RETRY,             ///< Retry to get node list
    EVT_INI_NI_UPDT_DONE,                ///< Node info update done
    EVT_INI_APPL_STATUS                  ///< Application status was received

} zwnet_init_evt_t;

///
/// Network initialization state-machine's states
typedef enum
{
    INI_STA_IDLE,                ///< Waiting for event
    INI_STA_CTLR_INFO,           ///< Getting Z/IP router controller info
    INI_STA_EXE_UNHANDLE_CMD,    ///< Execute unhandled commands in learn mode
    INI_STA_CTLR_UNSOLICIT,      ///< Getting Z/IP router unsolicited destination address
    INI_STA_NODE_LIST,           ///< Getting node list (all the node id in the network)
    INI_STA_NODE_IP_DSCVRY,      ///< Discovering node-ip addresses
    INI_STA_NODE_INFO            ///< Getting node info

} zwnet_init_state_t;

/** Initiate context */
typedef struct
{
	uint32_t	homeid;         /**< Network Home ID before initiate operation */
    uint8_t		join_new_nw;    /**< Flag to indicate whether initiate operation is to join a new network*/
    uint8_t		ctlr_node;      /**< Controller node id*/
} initiate_ctx_t;

///
/// Network initialization state-machine parameter
typedef struct
{
    uint8_t         nm_op;              ///< Network management operation
    uint8_t         enable_file_load;   ///< Flag to enable loading of detailed node info from file
    initiate_ctx_t  *initiate_ctx;      ///< Initiate context if the nm_op = ZWNET_OP_INITIATE and not joining a new network
} zwnet_init_sm_prm_t;

///
/// Controller reset state-machine events
typedef enum
{
    EVT_RST_TMOUT,                       ///< Timeout
    EVT_RST_START,                       ///< Start reset controller
    EVT_RST_DRL_TX_STS,                  ///< Transmit status of "device reset locally"
    EVT_RST_DONE,                        ///< Reset done
    EVT_RST_FAILED,                      ///< Reset failed
    EVT_RST_CTLR_INFO_DONE,              ///< The process of getting Z/IP router controller info done
    EVT_RST_NODE_LIST                    ///< Get node list
} zwnet_rst_evt_t;

///
/// Controller reset state-machine's states
typedef enum
{
    RST_STA_IDLE,                ///< Waiting for event
    RST_STA_DEV_RST_LOCAL,       ///< Waiting for "device reset locally" transmit status
    RST_STA_RESET,               ///< Waiting for reset done
    RST_STA_CTLR_INFO,           ///< Getting Z/IP router controller info
    RST_STA_NODE_LIST            ///< Waiting for node list
} zwnet_rst_state_t;

///
/// Controller info state-machine events
typedef enum
{
    EVT_CTLR_TMOUT,                  ///< Timeout
    EVT_CTLR_START,                  ///< Start getting controller information
    EVT_CTLR_NODE_ID,                ///< Received Z/IP router controller node id
    EVT_CTLR_PAN_ADDR,               ///< Received Z/IP router controller IPv6 PAN address
    EVT_CTLR_ID_ADDR_FAILED,         ///< Received Z/IP router controller node id failed
    EVT_CTLR_NODE_INFO,              ///< Received Z/IP router controller node info
    EVT_CTLR_NODE_INFO_RETRY,        ///< Retry to get controller node info
    EVT_CTLR_NODE_ID_ADDR,           ///< Received node-ip address discovery completion status
    EVT_CTLR_VER_REPORT,             ///< Detailed version report
    EVT_CTLR_CMD_VER_REPORT,         ///< Command version report
    EVT_CTLR_DSK_REPORT,             ///< S2 DSK report
    EVT_CTLR_MANF_SPEC_REPORT,       ///< Manufacturer specific report
    EVT_CTLR_DEVICE_ID_REPORT,       ///< Manufacturer specific device id report
    EVT_CTLR_MAILBOX_CFG_REPORT,     ///< Mailbox configuration report
    EVT_CTLR_APPL_STATUS             ///< Application status was received

} zwnet_ctlr_evt_t;

///
/// Controller info state-machine's states
typedef enum
{
    CTLR_STA_IDLE,                ///< Waiting for event
    CTLR_STA_CTLR_NODE_ID,        ///< Getting Z/IP router controller node id
    //CTLR_STA_CTLR_PAN_ADDR,       ///< Getting Z/IP router controller IPv6 PAN address
    CTLR_STA_CTLR_NODE_INFO,      ///< Getting Z/IP router controller node info
    CTLR_STA_CTLR_VERSION,        ///< Getting Z/IP router controller version report
    CTLR_STA_CTLR_DSK,            ///< Getting Z/IP router DSK
    CTLR_STA_MAILBOX,             ///< Getting and setting mailbox service
    CTLR_STA_CTLR_MANF_SPEC       ///< Getting Z/IP router controller manufacturer specific report

} zwnet_ctlr_state_t;

///
/// Node info state-machine events
typedef enum
{
    EVT_NW_TMOUT,                       ///< Network timeout
    EVT_GET_NODE_INFO,                  ///< Start the get node info process
    EVT_DELAYED_GET_NODE_INFO,          ///< Start the get node info process at specified time later
    EVT_GW_DSK,                         ///< Z/IP gateway DSK was received
    EVT_NODE_INFO,                      ///< Node info was received
    EVT_NODE_INFO_RETRY,                ///< Retry to get node info
    EVT_NODE_PROBE_REPORT,              ///< NOP ack was received
    EVT_RR_TX_STS,                      ///< Assign return route transmit status was received
    EVT_RR_RETRY,                       ///< Retry assign return route
    EVT_VER_REPORT,                     ///< Version report of a command class was received
    EVT_ZWVER_REPORT,                   ///< Z-wave protocol and application version report was received
    EVT_EP_INFO_DONE,                   ///< Endpoint info state-machine has completed
    EVT_GET_OTHERS_DONE,                ///< Get other command classes info done
    EVT_ZWPLUS_INFO_REPORT,             ///< Z-wave+ information was received
    EVT_MANF_PDT_ID_REPORT,             ///< Manufacturer and product id report was received
    EVT_DEVICE_ID_REPORT,               ///< Device id report was received
    EVT_WKUP_CAP_REP,                   ///< Wake up capabilities report was received
    EVT_MULTI_INSTANCE_REP,             ///< Multi-instance report was received
    EVT_APPL_STATUS,                    ///< Application status was received
    EVT_EP_NUM_REPORT,                  ///< Number of endpoints in a node report was received
    EVT_EP_AGGR_MEMBER_REPORT,          ///< Aggregated end point members report was received
    EVT_EP_CAP_REPORT                   ///< Endpoint capabilities report was received

} zwnet_ni_evt_t;

///
/// Node info state-machine's states
typedef enum
{
    ZWNET_STA_IDLE,             ///< Waiting for event
    ZWNET_STA_GET_GW_DSK,       ///< Getting Z/IP gateway DSK
    ZWNET_STA_GET_NODE_INFO,    ///< Getting node info
    ZWNET_STA_ASSIGN_RR,        ///< Assign controller's return route
    ZWNET_STA_GET_CMD_VER,      ///< Getting command class version
    ZWNET_STA_GET_OTHERS,       ///< Getting other command classes info
    ZWNET_STA_GET_ZWPLUS_INFO,  ///< Get Z-wave+ information
    ZWNET_STA_GET_MANF_PDT_ID,  ///< Getting manufacturer and product id
    ZWNET_STA_WKUP_CFG,         ///< Configuring wake up interval and notification receiving node
    ZWNET_STA_MULTI_INSTANCE,   ///< Multi-instance command class handling
    ZWNET_STA_MULTI_CHANNEL,    ///< Multi-channel command class handling
    ZWNET_STA_GET_EP_INFO       ///< Getting endpoint info

} zwnet_ni_state_t;

///
/// State-machine's sub-states
typedef enum
{
    SENSOR_SUBSTA_DATA_TYPE,    ///< Get sensor reading and type (For version 1 to 4)
    SENSOR_SUBSTA_TYPE,         ///< Get sensor types
    SENSOR_SUBSTA_UNIT,         ///< Get sensor units
    SENSOR_SUBSTA_DATA,         ///< Get sensor readings for each type and unit (For version 5 and greater)

    GRP_INFO_SUBSTA_CMD_LST,    ///< Get group command lists
    GRP_INFO_SUBSTA_NAME,       ///< Get group name
    GRP_INFO_SUBSTA_INFO,       ///< Get group info

    AV_SUBSTA_RPT_CNT,          ///< Get total simple AV control reports
    AV_SUBSTA_CTL_RPT,          ///< Get simple AV control report

    MS_SUBSTA_PDT_ID,           ///< Get manufacturer, product type and id
    MS_SUBSTA_DEV_ID_OEM,       ///< Get OEM factory default device ID

    MC_SUBSTA_EP_CNT,           ///< Get total number of endpoints
    MC_SUBSTA_EP_CAP,           ///< Get endpoint capabilities
    MC_SUBSTA_AGGR_EP_MBR,      ///< Get aggregated endpoint members

    ALARM_SUBSTA_TYPE,          ///< Get alarm types
    ALARM_SUBSTA_EVENT,         ///< Get alarm events

    GRP_SUBSTA_MAX_SUPP,        ///< Get maximum supported groups
    GRP_SUBSTA_SET,             ///< Set controller node id to group
    GRP_SUBSTA_GET,             ///< Get group members

    LEVEL_SUBSTA_DATA,          ///< Get multilevel switch level
    LEVEL_SUBSTA_SUPP,          ///< Get supported multilevel switch

    COLOR_SUBSTA_DATA,          ///< Get color switch level
    COLOR_SUBSTA_SUPP,          ///< Get supported color switch

    THRMO_MD_SUBSTA_DATA,       ///< Get thermostat modes
    THRMO_MD_SUBSTA_SUPP,       ///< Get supported thermostat modes

    THRMO_SETP_SUBSTA_DATA,     ///< Get thermostat setpoint value
    THRMO_SETP_SUBSTA_SUPP,     ///< Get supported thermostat setpoint type
    THRMO_SETP_SUBSTA_RANGE,    ///< Get supported thermostat setpoint temparature range

    BSENSOR_SUBSTA_TYPE,        ///< Get binary sensor types
    BSENSOR_SUBSTA_DATA,        ///< Get binary sensor readings for each type (For version 2 and greater)

    METER_SUBSTA_UNIT,          ///< Get meter units
    METER_SUBSTA_DATA,          ///< Get meter readings for each unit (For version 2 and greater)

    DOORLCK_SUBSTA_OP,          ///< Get door lock operating state
    DOORLCK_SUBSTA_CFG,         ///< Get door lock configuration

    THRMO_FAN_MD_SUBSTA_DATA,   ///< Get thermostat fan mode
    THRMO_FAN_MD_SUBSTA_SUPP,   ///< Get supported thermostat fan modes

 	ALRM_SNSR_SUBSTA_TYPE,      ///< Get alarm sensor types
	ALRM_SNSR_SUBSTA_DATA,      ///< Get alarm sensor readings for each type

 	BAR_OPRT_SUBSTA_DATA,       ///< Get barrier operator state
 	BAR_OPRT_SUBSTA_SUPP,       ///< Get supported barrier operator notification subsystem

    VER_SUBSTA_CC,              ///< Get command classs version
    VER_SUBSTA_DEVICE,          ///< Get device version

    CSC_SUBSTA_CFG,             ///< Set Central Scene Controller configuration
    CSC_SUBSTA_SUPP,            ///< Get supported Central Scene Controller scene and key attributes

    NAMELOC_SUBSTA_NAME,        ///< Get name
    NAMELOC_SUBSTA_LOC,         ///< Get location

 	THRMO_OP_SUBSTA_DATA,       ///< Get thermostat operating state
 	THRMO_OP_SUBSTA_SUPP_LOG,   ///< Get supported thermostat operating state logging
 	THRMO_OP_SUBSTA_LOG,        ///< Get thermostat operating state log

 	PROT_SUBSTA_STATE,          ///< Get protection states
 	PROT_SUBSTA_SUPP,           ///< Get supported protection states and capabilities
 	PROT_SUBSTA_EC,             ///< Get protection exclusive control node
 	PROT_SUBSTA_REMAIN_TM,      ///< Get remaining time that a device will remain in RF protection mode

    CFG_SUBSTA_SET,             ///< Set parameter value
    CFG_SUBSTA_NAME,            ///< Get parameter name
    CFG_SUBSTA_INFO,            ///< Get parameter info
    CFG_SUBSTA_PROPERTY,        ///< Get parameter number properties

    SUBSTA_RESERVED             ///< Reserved

} zwnet_substa_t;



///
/// Endpoint info state-machine events
typedef enum
{
    EVT_EP_NW_TMOUT,                    ///< Network timeout
    EVT_EP_SM_START,                    ///< Start endpoint get node info state-machine
    EVT_EP_SEC_SUP_REPORT,              ///< Supported security command classes was received
    EVT_EP_VER_REPORT,                  ///< Version report of a endpoint command class was received
    EVT_EP_S_EP_INFO_DONE               ///< Getting single endpoint information done

} zwnet_ep_evt_t;

///
/// Endpoint info state-machine's states
typedef enum
{
    EP_STA_IDLE,             ///< Waiting for event
    EP_STA_GET_SEC_CMD_SUP,  ///< Getting supported secure command classes
    EP_STA_GET_CMD_VER,      ///< Getting command class version
    EP_STA_GET_EP_INFO       ///< Getting single endpoint information

} zwnet_ep_state_t;

///
/// Action for single endpoint information state-machine command handler
/// Note: this must be in sync with the array sequence defined in zwnet_1_ep_sm_cls_hdlr()
typedef enum
{
    S_EP_ACT_CFG_PARAM,     ///< Set configuration parameters
    S_EP_ACT_MAX_GROUP,     ///< Get maximum number of supported groupings
	S_EP_ACT_CSC,			///< Get Central scene supported scenes and key attributes
    S_EP_ACT_ZWPLUS_INFO,   ///< Get Z-wave+ information
    S_EP_ACT_SNSR,          ///< Get multi-level sensor information
    S_EP_ACT_AGI,           ///< Get group information
    S_EP_ACT_THRMO_FAN_MD,  ///< Get supported thermostat fan modes
    S_EP_ACT_THRMO_MD,      ///< Get supported thermostat modes
    S_EP_ACT_THRMO_SETP,    ///< Get supported thermostat setpoint types
    S_EP_ACT_MUL_SWITCH,    ///< Get supported multilevel switch
    S_EP_ACT_AV,            ///< Get supported simple AV controls
    S_EP_ACT_ALARM,         ///< Get supported alarm types
    S_EP_ACT_PROT,          ///< Get supported protection states
    S_EP_ACT_MAX_USR_CODES, ///< Get maximum number of supported user codes
    S_EP_ACT_METER,         ///< Get supported meter
    S_EP_ACT_METER_DESC,    ///< Get meter descriptor
    S_EP_ACT_BIN_SNSR,      ///< Get binary sensor information
    S_EP_ACT_DOORLOCK,      ///< Get door lock operating state
	S_EP_ACT_ALRM_SNSR,     ///< Get alarm sensor information
	S_EP_ACT_BARRIER_OPRT,  ///< Get barrier operator state
    S_EP_ACT_COL_SWITCH,    ///< Get supported color switch
    S_EP_ACT_THRMO_OP,      ///< Get thermostat operating state
    S_EP_ACT_SWITCH,        ///< Get binary switch state
    S_EP_ACT_BATTERY,       ///< Get battery level
    S_EP_ACT_THRMO_FAN_STA, ///< Get thermostat fan state
    S_EP_ACT_NAME_LOC,      ///< Get node naming and location information
    S_EP_ACT_END            ///< No more action

} zwnet_1_ep_act_t;

#define MAX_CC_EN   (S_EP_ACT_END + 4)  ///< Maximum number of command classes configuration enabled for probing

///
/// Single endpoint info state-machine events
typedef enum
{
    EVT_S_EP_NW_TMOUT,                    ///< Network timeout
    EVT_S_EP_SM_START,                    ///< Start endpoint get node info state-machine
    EVT_S_EP_INFO_REPORT,                 ///< Z-wave+ information report was received
    EVT_S_EP_SENSOR_INFO_REPORT,          ///< Sensor type and unit report was received
    EVT_S_EP_SENSOR_TYPE_REPORT,          ///< Sensor supported types report was received
    EVT_S_EP_SENSOR_UNIT_REPORT,          ///< Sensor supported units report was received
    EVT_S_EP_GRP_CNT_REPORT,              ///< Maximum number of supported groupings report was received
    EVT_S_EP_GRP_SET_STATUS,              ///< Set controller id to group transmit status
    EVT_S_EP_GRP_1_MEMBER_REPORT,         ///< Group 1 members report was received
    EVT_S_EP_GRP_CMD_LST_REPORT,          ///< Group command list report was received
    EVT_S_EP_GRP_NAME_REPORT,             ///< Group name report was received
    EVT_S_EP_GRP_INFO_REPORT,             ///< Group information report was received
    EVT_S_EP_THRMO_MD_SUP,                ///< Supported thermostat modes report was received
    EVT_S_EP_THRMO_MD_DAT,                ///< Thermostat mode report was received
    EVT_S_EP_THRMO_SETP_SUP,              ///< Supported thermostat setpoint types report was received
    EVT_S_EP_THRMO_SETP_DAT,              ///< Thermostat setpoint report was received
    EVT_S_EP_THRMO_SETP_RANGE,            ///< Thermostat setpoint supported temparature range report was received
    EVT_S_EP_MUL_SWITCH_SUP,              ///< Supported multilevel switch report was received
    EVT_S_EP_MUL_SWITCH_DAT,              ///< Multilevel switch report was received
    EVT_S_EP_COL_SWITCH_SUP,              ///< Supported color switch report was received
    EVT_S_EP_COL_SWITCH_DAT,              ///< Color switch report was received
    EVT_S_EP_AV_REPORT_CNT,               ///< Total number of AV reports was received
    EVT_S_EP_AV_CTL_REPORT,               ///< Supported simple AV control report was received
    EVT_S_EP_ALARM_TYPE,                  ///< Supported alarm types report was received
    EVT_S_EP_ALARM_EVENT,                 ///< Supported alarm events report was received
    EVT_S_EP_PROT_SUP,                    ///< Supported protection states report was received
    EVT_S_EP_PROT_STATE,                  ///< Protection states report was received
    EVT_S_EP_PROT_EC,                     ///< Protection exclusive control node report was received
    EVT_S_EP_PROT_REMAIN_TM,              ///< Protection remaining time report was received
    EVT_S_EP_USR_CODE_CNT_REPORT,         ///< Maximum number of supported user codes report was received
    EVT_S_EP_METER,                       ///< Supported meter report was received
    EVT_S_EP_METER_DAT,                   ///< Meter reading report was received
    EVT_S_EP_NAME_REPORT,                 ///< Endpoint name report was received
    EVT_S_EP_LOC_REPORT,                  ///< Endpoint location report was received
    EVT_S_EP_METER_TBL_DESC,              ///< Meter table id and table point admin number were received
    EVT_S_EP_CFG_SET_STATUS,              ///< Set configuration parameters transmit status
    EVT_S_EP_CFG_PRM_PROPTY,              ///< Configuration parameter properties
    EVT_S_EP_CFG_PRM_NAME,                ///< Configuration parameter name
    EVT_S_EP_CFG_PRM_INFO,                ///< Configuration parameter info
    EVT_S_EP_BSENSOR_INFO_REPORT,         ///< Sensor type and state report was received
    EVT_S_EP_BSENSOR_TYPE_REPORT,         ///< Binary sensor supported types report was received
    EVT_S_EP_SWITCH_REPORT,               ///< Binary switch report was received
    EVT_S_EP_THRMO_OP_REPORT,             ///< Thermostat operating state report was received
    EVT_S_EP_THRMO_OP_LOG_SUP,            ///< Supported thermostat operating state logging report was received
    EVT_S_EP_THRMO_OP_LOG_REPORT,         ///< Thermostat operating state log report was received
    EVT_S_EP_BATTERY_REPORT,              ///< Battery report was received
    EVT_S_EP_DOORLOCK_OP_REPORT,          ///< Doorlock operating state report was received
    EVT_S_EP_DOORLOCK_CFG_REPORT,         ///< Doorlock configuration report was received
    EVT_S_EP_THRMO_FAN_MD_SUP,            ///< Supported thermostat fan modes report was received
    EVT_S_EP_THRMO_FAN_MD_DAT,            ///< Thermostat fan mode report was received
    EVT_S_EP_THRMO_FAN_STA,               ///< Thermostat fan state report was received
	EVT_S_EP_CENTRAL_SCENE_SUP,           ///< Supported central scene report was received
    EVT_S_EP_CENTRAL_SCENE_SET_STATUS,    ///< Set central scene configuration transmit status
	EVT_S_EP_ALRM_SNSR_TYPE_REPORT,       ///< Alarm sensor supported types report was received
	EVT_S_EP_ALRM_SNSR_INFO_REPORT,       ///< Alarm Sensor type and state report was received
    EVT_S_EP_BARRIER_OPTR_STA_REPORT,     ///< Barrier operator state report was received
    EVT_S_EP_BARRIER_OPTR_SUPP_NOTIF,     ///< Supported barrier operator notification subsystem report was received
    EVT_S_EP_RESERVED                     ///< Reserved

} zwnet_1_ep_evt_t;

///
/// Single endpoint info state-machine's states
typedef enum
{
    S_EP_STA_IDLE,             ///< Waiting for event
    S_EP_STA_GET_ZWPLUS_INFO,  ///< Getting Z-wave+ information
    S_EP_STA_GET_SENSOR,       ///< Getting multilevel sensor supported types and units
    S_EP_STA_GET_MAX_GROUP,    ///< Getting maximum supported group
    S_EP_STA_GET_GROUP_INFO,   ///< Getting group information
    S_EP_STA_GET_THRMO_FAN_MD, ///< Getting supported thermostat fan operating modes
    S_EP_STA_GET_THRMO_MD,     ///< Getting supported thermostat modes
    S_EP_STA_GET_THRMO_SETP,   ///< Getting supported thermostat setpoint types
    S_EP_STA_GET_MUL_SWITCH,   ///< Getting supported multilevel switch
    S_EP_STA_GET_COL_SWITCH,   ///< Getting supported color switch
    S_EP_STA_GET_AV,           ///< Getting supported simple AV control
    S_EP_STA_GET_ALARM,        ///< Getting supported alarm
    S_EP_STA_GET_PROT,         ///< Getting supported protection states
    S_EP_STA_GET_MAX_USR_CODES,///< Getting maximum supported user codes
    S_EP_STA_GET_METER,        ///< Getting supported meter
    S_EP_STA_GET_METER_DESC,   ///< Getting meter descriptor
    S_EP_STA_CFG_PARAM,        ///< Set and get configuration parameters
    S_EP_STA_GET_BIN_SENSOR,   ///< Getting binary sensor supported types
    S_EP_STA_GET_DOORLOCK,     ///< Getting door lock operating state
    S_EP_STA_GET_SWITCH,       ///< Getting binary switch state
    S_EP_STA_GET_THRMO_OP,     ///< Getting thermostat operating state
    S_EP_STA_GET_BATTERY,      ///< Getting battery level
    S_EP_STA_GET_THRMO_FAN_STA,///< Getting thermostat fan state
	S_EP_STA_GET_CENTRAL_SCENE,///< Getting max supported scenes and key attributes
	S_EP_STA_GET_ALRM_SENSOR,  ///< Getting alarm sensor supported types
	S_EP_STA_GET_BARRIER_OPRT, ///< Getting barrier operator state
    S_EP_STA_GET_NODE_NAMELOC  ///< Getting name and location

} zwnet_1_ep_sta_t;

///
/// Action for node information state-machine command handler
typedef enum
{
    ACT_START,          ///< Start checking for features like wake up, multi-command, crc16
    ACT_VERSION,        ///< Get version information
    ACT_ZWPLUS_INFO,    ///< Get Z-wave+ information
    ACT_MANF_SPEC,      ///< Get manufacturer specific information
    ACT_DEVICE_CFG,     ///< Configure device specific based on device information database
    ACT_OTHERS,         ///< Get other command classes information
    ACT_END_POINT       ///< Get endpoints information

} zwnet_info_sm_act_t;

///
/// Network update state-machine events
typedef enum
{
    EVT_NU_TMOUT,                       ///< Timeout
    EVT_NU_CTLR_INFO_DONE,              ///< The process of getting Z/IP router controller info done
    EVT_NU_START,                       ///< Start network update
    EVT_NU_TOPOLOGY_FAILED,             ///< Network topology update failed
    EVT_NU_TOPOLOGY_DONE,               ///< Network topology update done
    EVT_NU_NODE_LIST,                   ///< Received network node list
    EVT_NU_NODE_ID_ADDR,                ///< Received node-ip address discovery completion status
    EVT_NB_UPDT_FAILED,                 ///< Neighbor update failed
    EVT_NB_UPDT_DONE,                   ///< Neighbor update done
    EVT_NI_UPDT_DONE                    ///< Node info update done

} zwnet_nu_evt_t;

///
/// Network update state-machine's states
typedef enum
{
    NU_STA_IDLE,                ///< Waiting for event
    NU_STA_CTLR_INFO,           ///< Getting Z/IP router controller info
    NU_STA_TOPOLOGY_UPDT,       ///< Getting network topology update
    NU_STA_NODE_LIST,           ///< Getting node list (all the node id in the network)
    NU_STA_NODE_IP_DSCVRY,      ///< Discovering node-ip addresses
    NU_STA_NEIGHBOR_UPDT,       ///< Getting node neighbor info
    NU_STA_NODE_INFO_UPDT       ///< Getting node info

} zwnet_nu_state_t;

///
/// Network change state-machine events
typedef enum
{
    EVT_NC_TMOUT,                       ///< Timeout
    EVT_NC_CTLR_INFO,                   ///< The process of getting Z/IP router controller info done
    EVT_NC_START,                       ///< Start network change checking
    EVT_NC_START_RETRY,                 ///< Start network change checking retry
    //EVT_NC_NODE_LIST,                   ///< Received network node list
    EVT_NC_NODE_ID_ADDR,                ///< Received node-ip address discovery completion status
    EVT_NC_NODE_INFO_DONE               ///< Node info update done

} zwnet_nc_evt_t;

///
/// Network change state-machine's states
typedef enum
{
    NC_STA_IDLE,                ///< Waiting for event
    NC_STA_RETRY,               ///< Waiting for other network operation to complete
    NC_STA_CTLR_INFO,           ///< Getting Z/IP router controller info
    NC_STA_NODE_IP_DSCVRY,      ///< Discovering node-ip addresses
    NC_STA_NODE_LIST,           ///< Getting node list (all the node id in the network)
    NC_STA_NODE_INFO_UPDT       ///< Getting node info

} zwnet_nc_state_t;

///
/// Multi-command handler state-machine events
typedef enum
{
    EVT_MUL_CMD_TMOUT,      ///< Timeout
    EVT_MUL_CMD_REPORT,     ///< Report is received
    EVT_MUL_CMD_TX_STS,     ///< Packet transmit status
    EVT_MUL_CMD_RESEND,     ///< Resend packet
    EVT_MUL_CMD_START,      ///< Start state-machine
    EVT_MUL_CMD_SHUTDOWN    ///< Shutdown state-machine

} zwnet_mul_cmd_evt_t;

///
/// Multi-command handler state-machine's states
typedef enum
{
    MUL_CMD_STA_IDLE,           ///< Idle
    MUL_CMD_STA_WAIT_REPORT,    ///< Waiting for report
    MUL_CMD_STA_SET_CMD_SENT,   ///< Set command has been sent
    MUL_CMD_STA_WAIT_FOR_SEND,  ///< Waiting for sending another packet
    MUL_CMD_STA_SHUTDOWN        ///< Shutdown, no event will be processed

} zwnet_mul_cmd_state_t;

/** Multi-command encapsulated single command type*/
#define MUL_CMD_TYPE_SET         0   /**< Set or command that does not require a response from Z/IP gateway*/
#define MUL_CMD_TYPE_RPT_GET     1   /**< Report get command that requires a response from Z/IP gateway*/
#define MUL_CMD_TYPE_RPT         2   /**< Report that is sent directly to the sender of multi-command encapsulated packet*/

#define MUL_CMD_MAX_BUF_SZ      200  /**< Multi-command response maximum buffer size*/
///
/// Multi-command encapsulated single command entry
typedef struct
{
    uint8_t     type;       /**< command type, see MUL_CMD_TYPE_XXX */
    uint8_t     rpt;        /**< expected report if type is MUL_CMD_TYPE_RPT_GET */
    uint16_t    rpt_cls;    /**< expected command class of report if type is MUL_CMD_TYPE_RPT_GET */
    uint8_t     len;        /**< command length */
    uint8_t     cmd[1];     /**< command */

} mul_cmd_ent_t;

/** Request for multi-command thread to execute */
typedef struct
{
    util_lst_t          *mul_cmd_hd;    /**< Head of linked list for creating reply for multi-command encapsulated requests*/
    int                 mcmd_resp;      /**< Flag to determine whether to use multi-command encapsulation in response*/
    appl_snd_data_t     snd_prm;        /**< Send parameters */
} zwnet_mcmd_sm_req_t;

///
/// Multi-command handler state-machine context
typedef struct
{
    zwnet_mul_cmd_state_t   sta;            /**< Current state of the state-machine */
    void                    *tmr_ctx;       /**< State machine timer context*/
    uint8_t                 *mcmd_buf;      /**< Multi Command Encapsulation buffer with size MUL_CMD_MAX_BUF_SZ */
    uint8_t                 wr_idx;         /**< Index to the buffer for writing */
    uint8_t                 rd_idx;         /**< Index to the buffer for reading */
    uint8_t                 rpt;            /**< expected report */
    uint16_t                rpt_cls;        /**< expected command class of report */
    uint16_t                resnd_cnt;      /**< resend count */
    zwnet_mcmd_sm_req_t     req;            /**< Request*/

} mcmd_sm_ctx_t;

///
/// Add node state-machine events
typedef enum
{
    EVT_ADD_TMOUT,              ///< Timeout
    EVT_ADD_START,              ///< Start
    EVT_ADD_DSK_RPT             ///< Gateway DSK report received

} zwnet_add_evt_t;

///
/// Add node state-machine's states
typedef enum
{
    ADD_STA_IDLE,           ///< Waiting for event
    ADD_STA_CTLR_DSK        ///< Getting Z/IP gateway DSK

} zwnet_add_state_t;

#define SM_MODE_ADD     0   ///< Add node state-machine to add node
#define SM_MODE_REPLACE 1   ///< Add node state-machine to replace node

///
/// Join other network state-machine events
typedef enum
{
    EVT_JOIN_TMOUT,              ///< Timeout
    EVT_JOIN_START,              ///< Start
    EVT_JOIN_DSK_RPT             ///< Gateway DSK report received

} zwnet_join_nw_evt_t;

///
/// Join other network state-machine's states
typedef enum
{
    JOIN_STA_IDLE,           ///< Waiting for event
    JOIN_STA_CTLR_DSK        ///< Getting Z/IP gateway DSK

} zwnet_join_nw_sta_t;

///
/// Add node state-machine context
typedef struct
{
    uint8_t         mode;           /**< Mode, SM_MODE_XXX */
    uint8_t         s2_security;    /**< Flag to indicate whether to use S2 security */
    uint8_t         incl_on_behalf; /**< Flag to indicate whether to enter inclusion on-behalf mode */
    uint8_t         node_id;        /**< Node id. Valid if mode is SM_MODE_REPLACE */
} add_sm_ctx_t;

///
/// Join other network state-machine context
typedef struct
{
    void            *user;      /**< user context */
    get_dsk_fn      cb;         /**< callback to report Z/IP gateway DSK */
} join_sm_ctx_t;

///
/// Cache data state-machine table
typedef struct
{
    zwnet_1_ep_sta_t    state;  ///< State
    zwnet_1_ep_evt_t    evt;    ///< Event
    zwnet_1_ep_act_t    act;    ///< Action
} zwnet_1_ep_tab_t;

///
/// Firmware update state-machine events
typedef enum
{
    EVT_FW_UPDT_TMOUT,          ///< Timeout
    EVT_FW_UPDT_START,          ///< Start state-machine
    EVT_FW_UPDT_MD_RQ_RPT,      ///< Received MD request report
    EVT_FW_UPDT_FW_GET,         ///< Device request firmware MD
    EVT_FW_UPDT_FW_RESEND,      ///< Resend the last firmware MD
    EVT_FW_UPDT_FW_TX_STS,      ///< Firmware MD transmit status
    EVT_FW_UPDT_COMPLT_STS      ///< Received firmware update completion status

} zwnet_fw_updt_evt_t;

///
/// Firmware update state-machine states
typedef enum
{
    FW_UPDT_STA_IDLE,               ///< Idle
    FW_UPDT_STA_WAIT_MD_RQ_RPT,     ///< Waiting for MD request report
    FW_UPDT_STA_WAIT_FW_GET,        ///< Waiting for device to get firmware MD
    FW_UPDT_STA_WAIT_FW_TX_STS,     ///< Waiting for firmware MD transmit status
    FW_UPDT_STA_WAIT_COMPLT_STS     ///< Waiting for firmware update completion status

} zwnet_fw_updt_sta_t;

void zwnet_fw_updt_sts_cb(zwifd_p ifd, uint8_t status);
/**<
report callback for firmware update status
@param[in]	ifd	        interface
@param[in]	status      firmware update status
*/

void zwnet_fw_updt_cmplt_cb(zwifd_p ifd, uint8_t status, uint16_t wait_tm, int wait_tm_valid);
/**<
report callback for firmware update completion status
@param[in]	ifd	            interface
@param[in]	status          firmware update completion status, ZW_FW_UPDT_CMPLT_XXX
@param[in]	wait_tm         time (in seconds) that is needed before the receiving node becomes
                            available again for communication after the transfer of an image. This parameter is
                            valid only if wait_tm_valid=1
@param[in]	wait_tm_valid   flag to indicate the wait_tm parameter is valid.
*/

///
/// Network health check state-machine events
typedef enum
{
    EVT_NH_CHK_TMOUT,       ///< Timeout
    EVT_NH_CHK_PWRLVL_TMOUT,///< Powerlevel test timeout
    EVT_NH_CHK_RETRY,       ///< Retry last operation
    EVT_NH_CHK_START,       ///< Start
    EVT_NH_CHK_S_CLEAR,     ///< Statistics clear has been received by Z/IP gateway
    EVT_NH_CHK_TX_STS,      ///< Transmit status
    EVT_NH_CHK_S_RPT,       ///< Statistics report
    EVT_NH_CHK_PWRLVL_RPT,  ///< Powerlevel test report
    EVT_NH_CHK_GW_RSSI,     ///< Gateway background RSSI
    EVT_NH_CHK_START_CAL    ///< Start network health value calculation

} zwnet_nh_chk_evt_t;

///
/// Network health check state-machine's states
typedef enum
{
    NH_CHK_STA_IDLE,                ///< Waiting for event
    NH_CHK_STA_S_CLEAR,             ///< Waiting for statistics clear
    NH_CHK_STA_SEND_NOP,            ///< Send NOP to node
    NH_CHK_STA_S_RPT,               ///< Waiting for statistics report
    NH_CHK_STA_WAIT_PWRLVL_CMPT,    ///< Waiting for powerlevel test complete
    NH_CHK_STA_WAIT_PWRLVL_RPT,     ///< Waiting for powerlevel test report
    NH_CHK_STA_BKGND_RSSI_RPT,      ///< Waiting for background RSSI report
    NH_CHK_STA_WAIT_ALL_TEST        ///< Waiting for all test data in order to calculate network health value

} zwnet_nh_chk_state_t;

#define NHCHK_MAX_BUSY_RETRY    3       ///< Network health check state-machine maximum busy retry count
#define NHCHK_RSSI_GET_TMOUT    1000    ///< Get background RSSI timeout
#define NHCHK_BUSY_TIME         3000    ///< Time to retry transmission after a busy status is received
#define NHCHK_NOP_PER_CHK       10      ///< Number of NOP transmissions per network health check
#define NHCHK_CHK_PER_NODE      6       ///< Number of network health checks per node
#define NHCHK_MAX_REPEATER      4       ///< Maximum number of repeaters in LWR
#define NHCHK_MAX_RSSI_HOP      5       ///< Maximum number of RSSI hops
#define NHCHK_UNKNOWN_LWR       0xFF    ///< Unknown LWR
#define NHCHK_UNKNOWN_ACK_CH    0xFF    ///< Unknown ACK channel
#define NHCHK_UNKNOWN_TX_CH     0xFF    ///< Unknown transmit channel
#define NHCHK_PWRLVL_FRM_CNT    3       ///< Powerlevel test frame count
#define NHCHK_PWRLVL_WAIT_TIME  200     ///< Powerlevel test wait for completion time in milliseconds
#define NHCHK_PWRLVL_WAIT_RETRY 2       ///< Powerlevel test wait for valid result retry count
#define NHCHK_RETRY             3       ///< Generic retry count
#define NHCHK_BKGND_RSSI_CNT    10      ///< Number of background RSSI needed for calculation per channel

///
/// Network health check IMA statistics
typedef struct
{
    uint8_t     per;    ///< Packet error count
    uint8_t     rc;     ///< Route changes
    uint8_t     nb;     ///< Neighbors count
    uint8_t     tc;     ///< Transmission count
} nhchk_ima_stat_t;


///
/// Network health check state-machine context
typedef struct
{
    uint8_t             total_node; /**< Total number of nodes to check */
    uint8_t             node_idx;   /**< Index to node in health_rpt->sts[] */
    uint8_t             tx_cnt;     /**< Number of times NOP has been transmited per node*/
    uint8_t             chk_cnt;    /**< Number of times health check has been done per node */
    uint8_t             notify_snd; /**< Flag to indicate whether OP_HEALTH_CHK_STARTED has been sent */
    uint8_t             busy_retry; /**< Transmit busy retry count */
    uint8_t             lwr_rssi_ok;/**< Flag to indicate the node has at least 17 dB LWR RSSI*/
    uint8_t             lwr_pwrlvl_ok;/**< Flag to indicate all the repeaters and destination node have 6 dB power reduction margin*/
    uint8_t             lwr_src_node;/**< LWR powerlevel test source node*/
    uint8_t             bkgnd_rssi_idx;/**< Index to background RSSI array*/
    uint8_t             lwr_idx;    /**< Index to lwr*/
    uint8_t             ack_ch;     /**< ACK channel or NHCHK_UNKNOWN_ACK_CH if unknown*/
    uint8_t             tx_ch;      /**< Transmit channel or NHCHK_UNKNOWN_TX_CH if unknown*/
    uint8_t             rssi_cnt;   /**< Number of entries in rssi[]*/
    uint8_t             lwr_cnt;    /**< Number of node id in lwr[] or NHCHK_UNKNOWN_LWR if unknown*/
    uint8_t             lwr[NHCHK_MAX_REPEATER]; /**< Last working route node id */
    int8_t              rssi[NHCHK_MAX_RSSI_HOP];/**< Last working route RSSI array */
    int8_t              bkgnd_rssi[3][NHCHK_BKGND_RSSI_CNT];/**< Background RSSI array */
    uint16_t            nhv_sum;    /**< Accumulated network health value per node */
    zwnet_nh_chk_state_t state;     /**< State-machine state*/
    nhchk_ima_stat_t    ima_sta;    /**< Network health check IMA statistics */
    zw_health_rpt_t     health_rpt; /**< Health report */
} nhchk_sm_ctx_t;

///
/// Provisioning list state-machine events
typedef enum
{
    EVT_PLIST_TMOUT,    ///< Timeout
    EVT_PLIST_START,    ///< Start state-machine
    EVT_PLIST_RPT,      ///< Received provisioning list report
    EVT_PLIST_SHUTDOWN  ///< Shutdown the state-machine
} zwnet_plst_evt_t;

///
/// Provisioning list state-machine states
typedef enum
{
    PLIST_STA_IDLE,     ///< Idle
    PLIST_STA_WAIT_RPT, ///< Waiting for provisioning list report
    PLIST_STA_SHUTDOWN  ///< Shutdown
} zwnet_plst_sta_t;

#define PLIST_RPT_TMOUT     1100    ///< Get provisioning list report timeout
#define PLIST_MAX_RETRY     3       ///< Get provisioning list maximum retry count

///
/// AV "key hold" keep alive storage
typedef struct
{
    zwifd_t     ifd;        ///< Interface descriptor
    uint8_t     cmd[8];     ///< The "key hold" keep alive command
    uint8_t     run_tmr;    ///< Flag to control whether to continue running timer
    void        *tmr_ctx;   ///< Timer context
} av_hold_t;


///
/// Gateway mode query storage
typedef struct
{
    zwifd_t     ifd;        ///< Interface descriptor
    void        *tmr_ctx;   ///< Timer context
    void        *cb;        ///< Timeout callback
} gw_mode_qry_t;


///
/// Association report storage
typedef struct
{
    uint8_t         group_id;       ///< Group id the report refers to
    uint8_t         max_node;       ///< Maximum node the group id supported
    uint8_t         grp_mbr_cnt;    ///< Number of members in the buffer
    grp_member_t    *grp_mbr_buf;   ///< Buffer to store members
} asc_rpt_t;


///
/// AV capabilities report
typedef struct
{
    uint8_t total_rpt;      ///< The number of reports necessary to report the entire list of supported AV Commands
    uint8_t rpt_num;        ///< The last report number (starting from 1) stored in the buffer
    uint8_t *bit_mask_buf;  ///< The buffer to store the bit mask of supported AV Control Commands by the device
    uint16_t bit_mask_len;  ///< The length of the bit mask in the bit_mask_buf
} zwif_av_cap_t;


/** Association Group Information / Association data structure (to support) */
#define ZW_AGI_GROUPNAME_STR_MAX	42

/** Command Class and command id */
typedef struct  _CommandClass_Command_ID
{
	uint8_t CCMSB;	///< MSB byte of the command class
	uint8_t CCLSB;	///< LSB byte of the command class
	uint8_t CCID;	///< value of the command
	//struct _CommandClass_Command_ID	*pNext;
}CC_CCID;

/** Endpoint id and its associated node id*/
typedef struct _NodeEpInfo
{
	uint8_t byNodeID;   ///< Node id
	uint8_t byEpID;     ///< Endpoint id
}NodeEpInfo;

/** Association group info group*/
typedef struct  _Association_Group_Info_Group
{
	uint8_t	 byGroupID;                                 ///< Group Identifieer
	uint16_t wProfile;                                  ///< Profile
	uint16_t wEventCode;                                ///< Event code
	//CC_CCID *pCCList;
	CC_CCID stCCList;                                   ///< Command class and command
	char	cchGroupName[ZW_AGI_GROUPNAME_STR_MAX + 1]; ///< Group name
	uint8_t	byMaxNodes;                                 ///< max nodes supported
	NodeEpInfo stNodeEpInfo;                            ///< endpoint id and its node
	//SimpleArray_NodeEpInfo *pNodeArray;
	struct _Association_Group_Info_Group *pNext;        ///< next AGI group
}AGI_Group;

/** Association group info all groups*/
typedef struct _zwAGI_t
{
	uint8_t			byNofGroups;    /**< number of groups supported */
	AGI_Group		*pAGIGroupList; /**< AGI group list */
}
zwAGI_t, *zwAGI_p;

///
/// Supported secure command classes report
typedef struct
{
    uint8_t *cmd_cls_buf;   ///< The buffer to store the supported secure command classes
    uint8_t buf_len;        ///< The length of the secure command classes in the cmd_cls_buf
} zwif_sec_cmd_cls_t;


typedef void (*sm_fn)(zwnet_p nw, uint8_t first_node_id);  ///< Controller info state-machine callback when get node info has completed
typedef void (*ctlr_sm_fn)(zwnet_p nw, int status); ///< Controller info state-machine callback. status=0 (o.k.) else failed.

///
/// The job for the node info state-machine to execute
typedef struct
{
    sm_fn           cb;             /**< The callback function when get node info has completed */
    uint8_t         *node_id;       /**< The array to store the node id of nodes */
    uint32_t        delay;          /**< The time delay to start the get node information process */
    uint8_t         num_node;       /**< The number of node in the node_id array */
    uint8_t         op;             /**< The operation parameter for notify callback to pass
                                         once the get node information process is completed */
    uint8_t         auto_cfg;       /**< Flag to determine whether to auto configure wake up and sensor command classes */
    uint8_t         skip_nif;       /**< Flag to determine whether to skip node info cached get */
    uint8_t         assign_rr;      /**< Flag to determine whether to assign return route */
    uint8_t         get_gw_dsk;     /**< Flag to determine whether to get Z/IP gateway DSK. NOTE: only applicable for
                                         event=EVT_DELAYED_GET_NODE_INFO */
    uint8_t         delay_slp_node; /**< Flag to determine whether to delay detailed node info query of sleeping nodes
                                         until they are awake */

    //The following fields are used by state-machine internally
    uint8_t         rd_idx;         /**< The index to the node_id array for the current node */
} sm_job_t;

///
/// The job for the endpoint info state-machine to execute
typedef struct
{
    sm_fn       cb;             /**< The callback function when endpoint info state-machine has completed */
    uint8_t     node_id;        /**< Node id of endpoint*/
    uint8_t     cur_ep_id;      /**< Current endpoint id to query for info*/
    int         use_cache;      /**< Except for data readings, use cache if available */
    int         sts;            /**< State-machine status: 0=idle; 1=running*/
} ep_sm_job_t;

#define MAX_NEIGHBOR_UPDT_RETRIES   3   /**< maximum number of retries to find a failed node*/
///
/// Network update state-machine context
typedef struct
{
    uint8_t         failed_id[256]; /**< The array to store the node id of failed nodes */
    uint8_t         node_id[256];   /**< The array to store the node id of nodes */
    zwnet_nu_state_t    sta;        /**< The state of the network update state machine */
    uint8_t         num_node;       /**< The number of node in the node_id array */
    uint8_t         rd_idx;         /**< The index to the node_id array for the current node */
    uint8_t         nw_role;        /**< The network role of the controller */
    uint8_t         num_failed_node;/**< The number of node in the failed_id array */
    uint8_t         num_retry;      /**< The number of retries to find the failed nodes */

} nu_sm_ctx_t;

///
/// Network change state-machine context
typedef struct
{
    zwnet_nc_state_t    sta;                /**< The state of the network change state machine */
    void                *tmr_ctx;           /**< State machine timer context*/
    uint16_t            retry_cnt;          /**< Retry count*/
    uint8_t             sis_status;         /**< Z-wave role SIS status.0=none; 1=has been assigned SIS; 2=Role change handled*/
    uint8_t             new_node_num;       /**< Number of new nodes in new_node_list */
    uint8_t             new_node_list[232]; /**< Newly added node ids. */

} nc_sm_ctx_t;

///
/// Resolve node to ip address state-machine events
typedef enum
{
    EVT_N2IP_TMOUT,                       ///< Timeout
    EVT_N2IP_START,                       ///< Start resolving node to ip address
    EVT_N2IP_NODE_ID_ADDR                 ///< Received node-ip address discovery report

} n2ip_evt_t;

///
/// Resolve node to ip address state-machine's states
typedef enum
{
    N2IP_STA_IDLE,                ///< Waiting for event
    N2IP_STA_NODE_IP_DSCVRY,      ///< Discovering node-ip addresses
    N2IP_STA_WAIT_VALID_IP        ///< Waiting for valid ip address

} n2ip_sta_t;

///
/// Resolve node to ip address state-machine context
typedef struct
{
    int             retry_on_obsolete;      /**< Flag to indicate retry on INFORMATION_OBSOLETE */
    uint8_t         node_id[MAX_ZWAVE_NODE];/**< The array to store the node id of nodes */
    uint8_t         num_node;               /**< The number of node in the node_id array */
    uint8_t         rd_idx;                 /**< The read index to the node_id array for the current node */
    uint8_t         retry_cnt;              /**< Retry count due to the address obtained is invalid */
    uint8_t         retx_cnt;               /**< Retransmit retry count */
    n2ip_sta_t      state;                  /**< The state of state machine */
    void    (*cb)(zwnet_p nw, int status, void **prm);/**< Completion callback with status: 0=ok; non-zero = failed. */
    void            *params[3];             /**< Parameters used in the callback function*/

} n2ip_sm_ctx_t;

///
/// Cache set through context
typedef struct
{
    struct plt_mtx_t    *mtx;           /**< Mutex */
    zwifd_t     ifd;                    /**< Interface of the last set command that supports data caching */
    unsigned    id;                     /**< Identifier for checking of rpt integrity */
    uint16_t    rpt_len;                /**< Report length of set_cch_rpt */
    uint8_t     rpt[MAX_ZWAVE_PKT_SIZE];/**< Report generated based on the last set command */
} cch_set_ctx_t;


/** Inclusion on-behalf mode*/
#define IOB_USER_INITIATE       1   /**< User initiated inclusion on-behalf */
#define IOB_UNSOLICITED         2   /**< Unsolicited inclusion on-behalf */

/** Controller properties bit-mask*/
#define CTLR_PROPTY_NIF_GRANT_KEYS      0x01   /**< Node info cached report contains grant keys */
#define CTLR_PROPTY_EP_NIF              0x02   /**< Z/IP gateway supports endpoint info query */
#define CTLR_PROPTY_INTVW_CMPLT_STS     0x04   /**< Z/IP gateway supports LEARN_MODE_INTERVIEW_COMPLETED */


#define DSK_STR_LEN             48  /**< DSK string length, including the NUL character */

/** Provisioning list context */
typedef struct
{
    zwnet_plst_sta_t    sta;            /**< Provisioning list state-machine state*/
    void                *tmr_ctx;       /**< State machine timer context*/
	pl_info_fn          info_cb;	    /**< Callback to report provisioning list entry information*/
    pl_list_fn          list_cb;        /**< Callback to report all entries in provisioning list*/
#ifdef PL_S2_GRANT_MODE_API
    pl_s2_grnt_mode_fn  grnt_mode_cb;   /**< Callback to report S2 keys grant mode setting*/
#endif
    void                *info_usr_ctx;  /**< Provisioning list entry information callback user context*/
    void                *list_usr_ctx;  /**< Provisioning list callback user context*/
#ifdef PL_S2_GRANT_MODE_API
    void                *grnt_mode_usr_ctx;  /**< S2 keys grant mode setting callback user context*/
#endif
    pl_lst_ent_t        *pl_list;       /**< Provisioning list entries */
    uint8_t             pl_list_cnt;    /**< Number of entries in pl_list */
    uint8_t             save_remain_cnt;/**< Saved remaining counter used in iter_get command */
    uint8_t             retry_cnt;      /**< retry count for state-machine use*/
} pl_ctx_t;

///
/// Controller state-machine context
typedef struct
{
    ctlr_sm_fn      ctlr_sm_cb;         /**< Completion callback */
    initiate_ctx_t  *initiate_ctx;      /**< Initiate context if network operation = ZWNET_OP_INITIATE and not joining a new network */

} ctlr_sm_ctx_t;

//Forward declaration of contexts
struct _poll_ctx;
struct _spoll_ctx;
struct _zwsave_ctx;
struct _zwdpoll_ctx;
struct _wupoll_ctx;
struct _spv_ctx;
struct _evtg_ctx;
struct _nodests_ctx;
struct _ifsm_ctx;
struct _appl_cmd_saved;

/** Network */
typedef struct _zwnet
{
	uint32_t	        homeid;         /**< Network Home ID */
	zwnode_t		    ctl;	        /**< controller device, head in device list */
	zwnet_init_t	    init;		    /**< client initialization parameters */
    appl_layer_ctx_t    appl_ctx;       /**< Z-wave HCI application layer context */
    struct plt_mtx_t    *mtx;           /**< Mutex to access zwnet_t structure*/
	int	                multi_client;   /**< Flag to indicate whether to support multiple Z/IP clients connected to
                                             a same Z/IP gateway. 1=supported ; 0=not supported */
	int	                use_encap_hdr;  /**< Flag to indicate whether to use encapsulation header in Z/IP packet */
	int	                init_done;      /**< Flag to indicate call to zwnet_init has completed */
    uint8_t             zip_pan[IPV6_ADDR_LEN];/**< PAN network address of Z/IP nodes, to which packet has
                                                    to be routed through Z/IP router */
    uint8_t             zip_router_addr[IPV6_ADDR_LEN]; /**< Z/IP router IPv6 address */
    uint8_t             curr_op;        /**< Current executing operation*/
	uint8_t	            ctl_role;       /**< Z/IP controller role. A bit-mask of ZWNET_CTLR_ROLE_XXX */
    uint8_t	            ctl_cap;        /**< Z/IP controller capabilities. A bit-mask of ZWNET_CTLR_CAP_XXX */
	uint8_t	            zwave_role;     /**< Z/IP controller Z-wave role, ZW_ROLE_XXX */
	uint8_t	            nlist_ctl_id;   /**< Node list controller id (The controller who keeps latest node list) */
    uint8_t             failed_id;      /**< Failed node id either to be removed or replaced*/
    uint8_t             added_ctlr_id;  /**< Node id of the just added controller*/
    uint8_t             added_node_new; /**< Flag to determine whether the just added node is new*/
    uint8_t             added_node_id;  /**< Node id of the just added node*/
    uint8_t             added_node_op;  /**< Current add node operation*/
    uint8_t             nu_progress_sts;/**< Network update progress status*/
    uint8_t             av_seq_num;     /**< AV sequence number to detect duplicates*/
    uint8_t             poll_enable;    /**< Flag to indicate poll facility is enabled*/
    uint8_t             thrmo_rd_idx;   /**< Get thermostat value read index*/
    uint8_t             thrmo_setp_intp;/**< Supported thermostat setpoint bit-mask interpretation; 0=A, 1=B*/
    uint8_t             snsr_rd_idx;    /**< Get sensor units read index*/
    uint8_t             snsr_unit_idx;  /**< Get sensor report unit read index*/
    uint8_t             alrm_rd_idx;    /**< Get alarm events read index*/
    uint8_t             grp_rd_idx;     /**< Get group info read index*/
    uint8_t             sm_gen_counter; /**< State-machine generic counter to detect duplicates */
    uint8_t             sm_gen_retry;   /**< State-machine generic retry count */
    uint8_t             sm_busy_retry;  /**< State-machine application busy retry count */
    uint8_t             nw_init_failed; /**< Flag to indicate network initialization failed */
    uint8_t             ctlr_sm_sub_sta;/**< The sub-state of the controller info state machine */
    uint8_t             csa_requested;  /**< Flag to indicate joining node has requested S2 CSA */
    uint8_t             incl_on_behalf; /**< Inclusion on-behalf mode, see IOB_XXX */
    uint8_t             fw_updt_prg;    /**< firmware update progress in percentage */
    uint16_t            rcx_flag;       /**< Received packet flag, see ZIP_FLAG_XXX*/
    uint16_t            ni_sm_cls;      /**< Last command class of version get in the node info state-machine*/
    uint16_t	        ctl_propty;     /**< Z/IP controller properties. A bit-mask of CTLR_PROPTY_XXX */
    uint16_t            dflt_wunmi_dly; /**< Default wakeup no more info (in seconds) delay for post-inclusion interview. 0 = don't send WUNMI*/
    zwnet_substa_t      ni_sm_sub_sta;  /**< The sub-state of the node info state machine */
    add_sm_ctx_t        add_sm_ctx;     /**< Add node state machine context */
    join_sm_ctx_t       join_sm_ctx;    /**< Join other network state machine context */
    n2ip_sm_ctx_t       n2ip_sm_ctx;    /**< Resolve node to ip address state-machine context*/
    nhchk_sm_ctx_t      nh_chk_sm_ctx;  /**< Network health check state-machine context*/
    sec2_add_prm_t      sec2_add_prm;   /**< Add node using security 2 parameters*/
    zwnet_ctlr_state_t  ctlr_sm_sta;    /**< The state of the controller information state machine */
    zwnet_init_state_t  init_sm_sta;    /**< The state of the network initialization state machine */
    zwnet_ni_state_t    ni_sm_sta;      /**< The state of the node info state machine */
    zwnet_rst_state_t   rst_sm_sta;     /**< The state of the controller reset state machine */
    zwnet_add_state_t   add_sm_sta;     /**< The state of the add node state machine */
    zwnet_join_nw_sta_t join_sm_sta;    /**< The state of the join other network state machine */
    zwnet_ep_state_t    ep_sm_sta;      /**< The state of the endpoint info state machine */
    zwnet_1_ep_sta_t    single_ep_sta;  /**< The state of the single endpoint info state machine */
    zwnet_fw_updt_sta_t fw_updt_sta;    /**< The state of the firmware update state machine */
    sm_job_t            sm_job;         /**< The job for the node info state-machine to execute */
    ep_sm_job_t         ep_sm_job;      /**< The job for the endpoint info state-machine to execute */
    ep_sm_job_t         single_ep_job;  /**< The job for the single endpoint info state-machine to execute */
    nu_sm_ctx_t         nu_sm_ctx;      /**< The context of the network update state machine */
    nc_sm_ctx_t         nc_sm_ctx;      /**< The context of the network change state machine */
    ctlr_sm_ctx_t       ctlr_sm_ctx;    /**< Controller info state-machine context */
    initiate_ctx_t      initiate_ctx;   /**< The context of initiate operation */
    zwnet_init_sm_prm_t init_sm_prm;    /**< The parameters for the network initialization state-machine to execute */
    void                *sm_tmr_ctx;    /**< State machine timer context*/
    zwepd_t             ni_sm_ep;       /**< End point descriptor to use when time out occured in node info state machine*/
    zwif_wakeup_t       wake_up;        /**< Store the wake up setting*/
    zwif_av_cap_t       av_cap;         /**< Store the av capabilities setting*/
    zwif_sec_cmd_cls_t  sec_cmd_cls;    /**< Store the supported secure command classes*/
    asc_rpt_t           asc_rpt;        /**< Store association (group) report which is split into multiple reports*/
    void                *cmd_q_sem;     /**< Semaphore for waiting requests to execute queued commands*/
    struct plt_mtx_t    *cmd_q_mtx;     /**< Mutex for command queue thread */
    util_lst_t          *cmd_q_req_hd;  /**< Head of linked list for requests to execute queued commands*/
    void                *wait_q_sem;    /**< Semaphore for waiting requests to execute queued commands for nodes
                                             that require wakeup beam*/
    util_lst_t          *wait_q_req_hd; /**< Head of linked list for requests to execute queued commands for nodes
                                             that require wakeup beam*/
    void                *nw_exec_sem;   /**< Semaphore for waiting requests*/
    struct plt_mtx_t    *nw_exec_mtx;   /**< Mutex for requests to execute commands */
    util_lst_t          *nw_exec_req_hd;/**< Head of linked list for requests to execute commands*/
    void                *cb_sem;        /**< Semaphore for callback requests*/
    struct plt_mtx_t    *cb_mtx;        /**< Mutex for requests to execute callback */
    util_lst_t          *cb_req_hd;     /**< Head of linked list for requests to execute callback*/
    struct plt_mtx_t    *mul_cmd_sm_mtx;/**< Mutex for multi-command state-machine request queue */
    util_lst_t          *mul_cmd_sm_hd; /**< Head of linked list for requests to run multi-command state-machine*/
    util_lst_t          *mul_cmd_hd;    /**< Head of linked list for creating reply for multi-command encapsulated requests*/
    mcmd_sm_ctx_t       mul_cmd_sm_ctx; /**< Multi-command handler state-machine context*/
    char                *net_info_dir;  /**< Full path of directory for storing network and node information file */
    char                *pref_dir;      /**< Full path of directory for storing network/user preference files*/
    volatile int        wait_q_thrd_run;/**< Control the command queue thread whether to run. 1 = run, 0 = stop*/
    volatile int        wait_q_thrd_sts;/**< Command queue thread status. 1 = run, 0 = thread exited*/
    volatile int        cmd_q_thrd_run; /**< Control the command queue thread whether to run. 1 = run, 0 = stop*/
    volatile int        cmd_q_thrd_sts; /**< Command queue thread status. 1 = run, 0 = thread exited*/
    volatile int        nw_exec_thrd_run;/**< Control the thread whether to run. 1 = run, 0 = stop*/
    volatile int        nw_exec_thrd_sts;/**< Remove failed node thread status. 1 = run, 0 = thread exited*/
    volatile int        cb_thrd_run;    /**< Control the thread whether to run. 1 = run, 0 = stop*/
    volatile int        cb_thrd_sts;    /**< Callback thread status. 1 = run, 0 = thread exited*/
    zwmeter_t           meter_desc;     /**< Store the meter descriptor*/
    zwifd_t             fw_updt_ifd;    /**< Firmware update interface used by firmware update state-machine*/
    av_hold_t           av_key_hold;    /**< Store the "key hold" keep alive*/
    gw_mode_qry_t       gw_mode_qry;    /**< Store the gateway mode query context*/
    plt_ctx_t           plt_ctx;        /**< Platform context */
    zwnetd_t            net_desc;       /**< Network descriptor */
    uint32_t            dflt_wkup_intv; /**< Default wakeup interval (in seconds) to be configured on all devices. 0 = don't configure*/
    struct _poll_ctx    *poll_ctx;      /**< Polling context */
    struct _spoll_ctx   *spoll_ctx;     /**< Post-set polling context */
    struct _spv_ctx     *spv_ctx;       /**< Supervision context */
    struct _zwdpoll_ctx *dpoll_ctx;     /**< Device polling context */
    struct _wupoll_ctx  *wupoll_ctx;    /**< Wakeup notification polling context */
    struct _zwsave_ctx  *zwsave_ctx;    /**< Network save context */
    struct _evtg_ctx    *evtg_ctx;      /**< Event generation context */
    struct _nodests_ctx *nsts_ctx;      /**< Sleeping node status polling context*/
    struct _ifsm_ctx    *ifsm_ctx;      /**< Interface state-machines context*/
    get_dsk_fn          dsk_cb;         /**< Callback function to report Z/IP gateway DSK */
    void                *dsk_cb_prm;    /**< Z/IP gateway DSK callback parameter*/
    dev_cfg_rec_t       dev_cfg_rec_buf;/**< Storage for device specific configuration records */
    zwnet_dev_rec_find_fn   dev_rec_find_fn;/**< User supplied function to find device record*/
    void                    *dev_cfg_ctx;   /**< User specified device configuration context for use in dev_rec_find_fn*/
    int                 ctx_ref_cnt;        /**< Reference count for this context*/
    char                *node_info_file;    /**< File name for saving node information to persistent storage. If this
                                                 parameter is NULL,  node information will not be saved */
    char                *inif_dsk;          /**< DSK string from unsolicited included node information frame (INIF) callback*/
    char                gw_dsk[DSK_STR_LEN];/**< DSK string*/
    cch_set_ctx_t       cch_set_ctx;        /**< Cache set through context*/
#ifdef TCP_PORTAL
    uint8_t             tls_tunnel_svr_ipv6[16];    /**< TLS tunnel server IPv6 address set by user */
    uint8_t             tls_pan_prefix[16];         /**< TLS PAN network address set by user */
#endif
#ifdef SUPPORT_SIMPLE_AV_CONTROL
    uint8_t             *av_sup_bitmask;    /**< Bitmask of supported AV commands*/
    uint16_t            av_sup_bitmask_len; /**< Length of bitmask for supported AV commands (in bytes)*/
#endif
#ifdef  TEST_HANDLE_BASIC_COMMAND_CLASS
    uint8_t             basic_val;          /**< Basic command class value*/
#endif

	zwAGI_t				stAGIData;          /**< Association CC and AGI CC data */
    pl_ctx_t            pl_ctx;             /**< Provisioning list context*/

    //Device specific configuration
    if_rec_grp_t        dev_cfg_grp;        /**< Groups to which the controller id is to be set */
    if_rec_config_t     dev_cfg_param;      /**< Configuration parameter records */
    uint16_t            dev_cfg_rd_idx;     /**< Read index for device configuration*/

    //Configuration command class
    uint16_t            cfg_prev_prm_num;   /**< Last parameter number to query */
    uint16_t            cfg_prm_num;        /**< Current parameter number to query */
    int16_t             cfg_rpt_to_flw;     /**< Last "Reports to follow" field for name and info report. Must be initialized to -1 */
    uint16_t            cfg_name_len;       /**< Parameter name length*/
    uint16_t            cfg_info_len;       /**< Parameter info length*/
    char                *cfg_name;          /**< Temporary storage for parameter name */
    char                *cfg_info;          /**< Temporary storage for parameter info */

    //Z/IP gateway supported command classes
    sup_cmd_cls_t       *gw_sup_cmd_cls;    /**< supported command classes */
    uint8_t             gw_sup_cmd_cls_cnt; /**< supported command classes count*/

    //Supported command classes (including user application implemented command classes)
    sup_cmd_cls_t       *sup_cmd_cls;       /**< supported command classes */
    uint8_t             sup_cmd_cls_cnt;    /**< supported command classes count*/

    //Command classes configuration to enable for probing
    uint16_t            cmd_cls_cfg[MAX_CC_EN];/**< command classes that are enable for probing*/
    uint16_t            cmd_cls_cfg_cnt;       /**< command classes count*/

    //For unhandled commands during learn mode
    struct _appl_cmd_saved *appl_cmd_saved;     /**< Saved unhandled commands during learn mode*/
    uint16_t            appl_cmd_cnt;           /**< Saved unhandled commands count*/

}
zwnet_t;

void zwnet_alrm_evt_rpt_cb(zwifd_p ifd, uint8_t ztype, uint8_t evt_len, uint8_t *evt_msk, int valid);
/**<
alarm supported events report callback
@param[in]	ifd	        interface
@param[in]	ztype       Z-wave alarm type (ZW_ALRM_XXX)
@param[in]	evt_len     size of evt buffer
@param[in]	evt_msk     buffer to store supported event bit-masks of the ztype
@param[in]	valid       validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
@return
*/

void zwnet_dev_if_propty_tmout_get(zwep_p ep, uint16_t cmd_cls, uint8_t real_ver,
                             uint8_t propty, uint8_t *new_propty, uint16_t *tmout);
/**<
Get interface property based on interface version and from device database (higher priority)
@param[in]	ep	        Endpoint
@param[in]	cmd_cls	    Command class
@param[in]	real_ver	Real command class version as reported by the device
@param[in]	propty	    Original interface property
@param[out]	new_propty	New interface property
@param[out]	tmout	    Interface timeout value
@return
*/


/**
@}
@addtogroup If Interface APIs
@{
*/

/** Report in the interface */
typedef struct
{
	uint8_t		rpt_cmd;		/**< report command */
    uint8_t     rsvd1;          /**< Padding to make sure the structure is 4-byte aligned */
    uint16_t    rsvd2;          /**< Padding to make sure the structure is 4-byte aligned */
    void        *rpt_cb;        /**< pointer to report callback function */
    void        *sm_rpt_cb;     /**< pointer to state-machine report callback function */

} if_rpt_t;


/** interfaces within endpoint */
typedef struct _zwif
{
    zwobj_t		    obj;	        /**< base link object */
    struct _zwep    *ep;	        /**< back link to associated endpoint */
    uint16_t	    cls;	        /**< command class */
    uint8_t		    ver;	        /**< command class version to be reported to user application, this version
                                         may differ from the real interface version*/
    uint8_t		    propty;	        /**< Properties of the interface (bit-mask): IF_PROPTY_XXX */
    uint16_t        tmout;          /**< command class specific timeout value in seconds */
    uint8_t		    real_ver;	    /**< real command class version as reported by the device */
    uint8_t	        data_cnt;	    /**< number of data items */

    uint8_t	        data_item_sz;	/**< data item size in bytes*/
	uint8_t			b2lvlcch;		/**< Indicate whether cch_data is a 2 level cached data format. 0 - No, 1 - Yes*/
    uint8_t	        cch_dat_cnt;	/**< number of cached data items */
    uint8_t	        cch_dat_item_sz;/**< cached data item size in bytes*/
    void            *tmp_data;      /**< temporary data which is specific to the command class*/
    void            *data;          /**< data items which is specific to the command class*/
    void            *cch_data;      /**< cached data items which is specific to the command class*/
    uint8_t		    rpt_num;        /**< number of valid reports in rpt[]  */
    uint8_t         rsvd1;          /**< Padding to make sure rpt is 4-byte aligned */
    uint16_t        rsvd2;          /**< Padding to make sure rpt is 4-byte aligned */
    if_rpt_t        rpt[1];         /**< reports*/
}
zwif_t;

/** Some CC would need to store an array of cached data, each data could be fix-length or variable-length.
For this type of CC, developer can choose to create 2 level cached data format:
cch_data->an array of  ->timestamp + actual data

This is the first level data in a 2 level cached data format */
typedef struct
{
	uint8_t			cch_ele_sz;		/**< size of the data pointed by pcch_ele, 1 cached data size + timestamp size */
    void			*pcch_ele;		/**< points to 1 cached data element, which consists of a timestamp and the actual data*/
} cch_1lvl_t;


#define FW_MD_SRC_UNKNOWN   0   /**< Firmware meta data source is unknown*/
#define FW_MD_SRC_FILE      1   /**< Firmware meta data source is from file*/
#define FW_MD_SRC_BUF       2   /**< Firmware meta data source is from memory buffer*/

#define MAX_FW_SIZE                 1000000 /**< Maximum firmware size*/

#define FW_UPDT_PRG_PERCENT_INTV    2 /**< Firmware update progress percentage interval. Must be a factor of 100 */

/** Firmware update interface temporary data */
typedef struct
{
	uint16_t	max_frag_sz;    /**< Maximum meta data fragment size allowed for firmware update */
	uint16_t	frag_sz;        /**< Fragment size used in actual firmware update. Zero means stop any fragment transfer*/
	uint16_t	rpt_num;        /**< The report number that has been sent to the device*/
	uint8_t	    rpt_cnt;        /**< Pending report count to be sent to the device*/
    uint8_t     last_frag;      /**< Flag to indicate this is the last fragment*/
    uint8_t     fixed_frag_sz;  /**< Flag to indicate whether the max_frag_sz is fixed, i.e. firmware update
                                     request MUST use the given size. 1=fixed size; 0=variable size */
    int         fw_src;         /**< Firmware meta data source, see FW_MD_SRC_XXX */
    FILE        *fw_file;       /**< Firmware file in binary format*/
    long        fw_file_sz;     /**< Firmware file size */
    uint8_t     *fw_buf;        /**< Firmware buffer*/
    uint32_t    fw_len;         /**< Firmware meta data length*/

    //Firmware update target restart
    //
    zwnet_p     nw;             /**< Network*/
    zwnoded_t   node;           /**< Firmware update target node*/
    void        *restrt_tmr_ctx;/**< Firmware update target restart timer context*/

} if_fw_tmp_dat_t;

/** Configuration interface temporary data */
typedef struct
{
    uint8_t         remain_prm_cnt; /**< Remaining parameter count that have not been sent*/
    uint8_t         prev_prm_cnt;   /**< Previous remaining parameter count*/
    uint16_t        prev_prm_start;	/**< Previous bulk report starting parameter number */
    uint16_t        max_pkt_sz;     /**< Maximum packet size for the interface */
    zwcfg_bulk_t    bulk_param;     /**< Bulk set configuration parameters*/
    zwcfg_bulk_t    bulk_rpt_hs;    /**< Bulk report with handshake bit set*/
    //
    //The following fields are reserved for bulk report due to bulk get command
    //
    zwcfg_bulk_t    bulk_rpt;       /**< Bulk report due to bulk get command*/
    int16_t         bulk_rpt_to_flw;/**< Last "Reports to follow" field for bulk report. Must be initialized to -1 */

} if_cfg_tmp_dat_t;

/** Sleeping device interface data */
typedef struct
{
	uint32_t	min;		/**< minimum */
	uint32_t	max;		/**< maximum */
	uint32_t	def;		/**< default */
	uint32_t	interval;	/**< steps between min and max (in seconds)*/
} if_wkup_data_t;

/** Doorlock interface cached data */
typedef struct
{
	zwdlck_op_t	    op;		/**< Door lock operation mode */
	time_t	        cfg_ts;	/**< Door lock configuration cached data time stamp */
	zwdlck_cfg_t	cfg;	/**< Door lock configuration */
} if_dlck_cch_data_t;

/** Binary sensor cached data */
typedef struct
{
    uint8_t     type;		/**< ZW_BSENSOR_TYPE_XXX .If type equals to zero, sensor type is unknown */
    uint8_t     state;	    /**< sensor state: 0=idle; 0xFF=event detected */
} zwbsnsr_t;

/** Barrier operator cached data */
typedef struct
{
    uint8_t subsys_type;    /**< Subsystem type ZW_BAR_NOTIF_TYP_XXX .If type equals to zero, subsys_sta
                                 represents barrier operator state */
    uint8_t subsys_sta;	    /**< Subsystem state: 0=off; 0xFF=on */
} zwbarrier_t;

/** Thermostat operating state cached data */
typedef struct
{
	time_t	    tmstamp;	/**< cached data time stamp either for op_sta or log_type, but not for both */
    uint8_t     op_sta;		/**< operating state, ZW_THRMO_OP_STA_XXX. Invalid if value is 0xFF */
    uint8_t     log_type;	/**< operating state log type, ZW_THRMO_OP_STA_XXX. Invalid if value is 0 */
    uint8_t     today_hr;	/**< The number of hours (00 ~ 24) the thermostat has been in the indicated operating state since 12:00 am of the current day*/
    uint8_t     today_mn;	/**< The number of minutes (00 ~ 59) the thermostat has been in the indicated operating state since 12:00 am of the current day*/
    uint8_t     prev_hr;	/**< The number of hours (00 ~ 24) the thermostat has been in the indicated operating state since 12:00 am of the previous day*/
    uint8_t     prev_mn;	/**< The number of minutes (00 ~ 59) the thermostat has been in the indicated operating state since 12:00 am of the previous day*/
} zwthrmo_op_sta_t;

/** Thermostat mode cached data */
typedef struct
{
    uint8_t     mode;		    /**< mode, ZW_THRMO_MD_XXX */
    uint8_t     manf_dat_cnt;	/**< Manufacturer data lenght */
    uint8_t     manf_dat[8];	/**< Manufacturer data when the mode is ZW_THRMO_MD_MANF_SPECIFIC */
} zwthrmo_mode_t;

/** Protection cached data */
typedef struct
{
	time_t	    tmstp_prot;	/**< time stamp for protection state. 0 = no cache */
	time_t	    tmstp_ec;	/**< time stamp for exclusive control node id. 0 = no cache */
	time_t	    tmstp_remain;/**< time stamp for remaining time. 0 = no cache */
    uint8_t     local_prot; /**< local protection state, ZW_LPROT_XXX */
    uint8_t     rf_prot;    /**< RF protection state, ZW_RFPROT_XXX */
    uint8_t     ec_nodeid;  /**< NodeID that has exclusive control */
    uint8_t     remain_tm;  /**< Remaining time. 0x00 = No timer is set. All “normal operation” Commands must be accepted.
                                 0x01 to 0x3C = 1 second (0x01) to 60 seconds (0x3C);
                                 0x41 to 0xFE = 2 minutes (0x41) to 191 minutes (0xFE);
                                 0xFF = No Timeout - The Device will remain in RF Protection mode infinitely.*/
} zwprot_cch_t;

/** End point capability */
typedef struct
{
    uint8_t   ep;            ///< end point id
    uint8_t   ep_dynamic;    ///< flag to indicate whether end point is a dynamic end point
    uint8_t   gen;           ///< generic device class
    uint8_t   spec;          ///< specific device class
    uint8_t   cmd_cnt;       ///< the number of commands in the cmd_cls field
    uint16_t  *cmd_cls;      ///< command classes

} ep_cap_t;


#define CHAR_PRES_ASCII     0       /**< ASCII*/
#define CHAR_PRES_OEM       1       /**< OEM Extended ASCII*/
#define CHAR_PRES_UTF16     2       /**< Unicode UTF-16*/

typedef void (*zwrep_info_fn)(zwif_p intf, zwplus_info_t *info);
/**<
ZWave+ info report callback
@param[in]	intf	    interface
@param[in]	info	    Z-Wave+ information
*/

typedef void (*zwrep_name_fn)(zwif_p intf, char *name, uint8_t len, uint8_t char_pres);
/**<
node name report callback
@param[in]	intf	    interface
@param[in]	name	    Node name
@param[in]	len	        Length of node name
@param[in]	char_pres   Char presentation
*/

typedef void (*zwrep_ver_fn)(zwif_p intf, uint16_t cls, uint8_t ver);
/**<
version report callback
@param[in]	intf	interface
@param[in]	cls	    The command class the version is referred
@param[in]	ver	    The version of cls
*/

typedef void (*zwrep_zwver_fn)(zwif_p intf, uint16_t proto_ver, uint16_t app_ver, uint8_t lib_type, ext_ver_t *ext_ver);
/**<
Z-wave versions report callback
@param[in]	intf	    interface
@param[in]	proto_ver	Z-Wave Protocol Version
@param[in]	app_ver	    Application Version
@param[in]	lib_type	Z-Wave Library Type
@param[in]	ext_ver	    Extended version information if this parameter is not NULL
*/

typedef void (*zwrep_mul_inst_fn)(zwif_p intf, uint8_t cls, uint8_t inst);
/**<
multi instance report callback
@param[in]	intf	interface
@param[in]	cls	    The command class the report is referred to
@param[in]	inst	The number of instances of a given command class
*/

typedef void (*zwrep_mc_cap_fn)(zwif_p intf, ep_cap_t *ep_cap);
/**<
multi channel capability report callback
@param[in]	intf	        interface
@param[in]	ep_cap	        Endpoint capability.
*/

typedef void (*zwrep_mc_ep_fn)(zwif_p intf, uint8_t num_of_ep, uint8_t num_of_aggr_ep, uint8_t flag);
/**<
multi channel end point report callback
@param[in]	intf	        interface
@param[in]	num_of_ep	    Number of end points embedded in the node. The maximum number of end points is 127.
@param[in]	num_of_aggr_ep	Number of aggregated end points embedded in the node.
@param[in]	flag	        Bit-7: 1 if the device has a dynamic number of end points.
                            Bit-6: 1 if all the end points in the node has the same generic and specific command class.
*/

typedef void (*zwrep_aggr_ep_fn)(zwif_p intf, uint8_t ep_id, uint8_t member_cnt, uint8_t *members);
/**<
aggregated end point members report callback
@param[in]	intf	        interface
@param[in]	ep_id	        aggregated end point id
@param[in]	member_cnt	    member count in the array members
@param[in]	members	        array to store the members endpoint id
*/

typedef void (*zwrep_nameloc_fn)(zwif_p, zw_nameloc_p nameloc);
/**<
callback for node name and location string get_report
@param[in]	intf	interface
@param[in]	nameloc	name & location string null terminated
@return	ZW_ERR_xxx
@see zwif_get_report, zwif_set_reporter
*/

typedef void (*zwrep_manf_fn)(zwif_p intf, uint16_t *manf_pdt_id);
/**<
manufacturer and product id report callback
@param[in]	intf	        The interface that received the report
@param[in]	manf_pdt_id	    Array that stores manufacturer, product type and product id
@return
*/

typedef void (*zwrep_devid_fn)(zwif_p intf, dev_id_t *dev_id);
/**<
device id report callback
@param[in]	intf	        The interface that received the report
@param[in]	dev_id	        Device ID
@return
*/

typedef void (*zwrep_grp_cmd_lst_fn)(zwif_p intf, uint8_t grp_id, uint8_t cmd_ent_cnt, grp_cmd_ent_t *cmd_lst);
/**<
group command list report callback
@param[in]	intf	        The interface that received the report
@param[in]	grp_id	        Grouping identifier
@param[in]	cmd_ent_cnt	    Number of entries in the command list
@param[in]	cmd_lst	        Command list
@return
*/

typedef void (*zwrep_grp_name_fn)(zwif_p intf, uint8_t grp_id, uint8_t name_len, uint8_t *name);
/**<
group name report callback
@param[in]	intf	        The interface that received the report
@param[in]	grp_id	        Grouping identifier
@param[in]	name	        Group name in UTF-8 format
@param[in]	name_len        String length of group name
@return
*/

typedef void (*zwrep_av_raw_fn)(zwif_p intf, uint16_t cmd_len, uint8_t *cmd_buf);
/**<
simple AV raw report callback
@param[in]	intf	        The interface that received the report
@param[in]	cmd_buf	        Command buffer
@param[in]	cmd_len         Length of command buffer
@return
*/

typedef void (*zwrep_cfg_propty_fn)(zwif_p intf, zwcfg_info_p info, int bulk_sup, uint16_t next_prm);
/**<
configuration parameter properties report callback
@param[in]	intf	        The interface that received the report
@param[in]	info	        Parameter properties information
@param[in]	bulk_sup	    Flag to indicate support of bulk set and bulk report get APIs
@param[in]	next_prm	    Next parameter.  Zero if info->param_num is the last valid parameter number
@return
*/

typedef void (*zwrep_cfg_name_fn)(zwif_p intf, uint16_t prm_num, char *name);
/**<
configuration parameter name report callback
@param[in]	intf	        The interface that received the report
@param[in]	prm_num	        Parameter number
@param[in]	name	        Parameter name
@return
*/

typedef void (*zwrep_cfg_info_fn)(zwif_p intf, uint16_t prm_num, char *info);
/**<
configuration parameter usage info report callback
@param[in]	intf	        The interface that received the report
@param[in]	prm_num	        Parameter number
@param[in]	info	        Parameter info
@return
*/

/** group information entry*/
typedef struct
{
	uint8_t         grp_num;        /**< group number */
	uint16_t        profile;        /**< profile */
	uint16_t        evt_code;       /**< event code */
}
zw_grp_info_ent_t;

typedef void (*zwrep_grp_info_fn)(zwif_p intf, uint8_t grp_cnt, uint8_t dynamic, zw_grp_info_ent_t *grp_info);
/**<
group info report callback
@param[in]	intf	        The interface that received the report
@param[in]	grp_cnt	        Group count
@param[in]	dynamic	        Flag to indicate dynamic info. 1=dynamic; 0=static
@param[in]	grp_info        Group information
@return
*/

/** Options for zwif_exec_ex */
#define ZWIF_OPT_Q_EXTRA        0x01    /**< bit-mask to indicate there is extra parameter for queuing */
#define ZWIF_OPT_SKIP_CHK       0x02    /**< bit-mask to skip chekcing of input size */
#define ZWIF_OPT_SKIP_MUL_CH    0x04    /**< bit-mask to skip multi-channel encapsulation */
#define ZWIF_OPT_POLL           0x08    /**< bit-mask to indicate whether this is a poll frame */
#define ZWIF_OPT_SKIP_MUL_Q     0x10    /**< bit-mask to skip command queuing and multi-command encapsulation */
#define ZWIF_OPT_SKIP_WK_BEAM   0x20    /**< bit-mask to skip checking for node that requires wakeup beam (FLIRS device)*/
#define ZWIF_OPT_CMD_ENTRY      0x40    /**< bit-mask to indicate the command entry mul_cmd_q_ent_t is in the xtra param */
#define ZWIF_OPT_WAIT_REPORT    0x80    /**< bit-mask to indicate whether to wait for report (for FLIRS device)
                                             Note: MUST be used together with ZWIF_OPT_SKIP_WK_BEAM  */
#define ZWIF_OPT_WAIT_RPT_SET   0x0100  /**< bit-mask to set wait for report (for FLIRS device)
                                             Note: MUST be used together with ZWIF_OPT_SKIP_ALL_IMD  */
#define ZWIF_OPT_SECURE         0x0200  /**< bit-mask to set secure origin bit in the Z/IP packet */
#define ZWIF_OPT_NO_NEW_DTLS    0x0400  /**< bit-mask to indicate not to create new DTLS session when re-send a Z/IP packet */

#define ZWIF_OPT_SKIP_ALL_IMD   (ZWIF_OPT_SKIP_WK_BEAM | ZWIF_OPT_SKIP_CHK | ZWIF_OPT_SKIP_MUL_CH | ZWIF_OPT_SKIP_MUL_Q)
                                /**< Skip all intermediate levels except CRC-16, security and raw levels.*/

#define ZWIF_OPT_GW_PORTAL      (ZWIF_OPT_SKIP_WK_BEAM | ZWIF_OPT_SKIP_CHK | ZWIF_OPT_SKIP_MUL_CH | ZWIF_OPT_SECURE | ZWIF_OPT_SKIP_MUL_Q)
                                /**< Option for Z/IP Gateway and Portal command classes*/

/** Flags for zwif_get_report_ex */
#define ZWIF_RPT_GET_NORM       0    /**< Get report from Z-wave or Z/IP node  */
#define ZWIF_RPT_GET_ZIP        1    /**< Get report from Z/IP node in secure mode */


typedef int (*zwnet_sm_get_fn)(zwnet_p nw, zwif_p intf);
/**<
state-machine get report function
@param[in]	nw	            Network
@param[in]	intf	        The interface that received the report
@return
*/

/** state-machine handler data structure*/
typedef struct
{
	zwnet_sm_get_fn get_fn;     /**< state-machine get report function */
	uint16_t        cls;        /**< command class */
	uint16_t        alt_cls;    /**< alternate command class, 0= not in used */
}
zw_sm_hdlr_dat_t;


zwif_p zwif_find_cls(zwif_p first_intf, uint16_t cls);
/**<
find an interface based on its command class
@param[in]	first_intf	first interface in an endpoint
@param[in]	cls	        the command class to search
@return		interface if found; else return NULL
@pre        Caller must lock the nw->mtx before calling this function.
*/

uint8_t zwif_count(zwif_p first_intf);
/**<
count the number of unsecure interfaces in an endpoint
@param[in]	first_intf	first interface in an endpoint
@return		The number of interfaces in an endpoint
@pre        Caller must lock the nw->mtx before calling this function.
*/

int zwif_rep_hdlr(zwif_p intf, uint8_t *cmd_buf, uint16_t cmd_len, int nw_lck_sts);
/**<
Interface report handler
@param[in]	intf	Interface
@param[in]	cmd_buf	The report command received
@param[in]	cmd_len	The length of cmd_buf
@param[in]	nw_lck_sts  Network lock status. 1=locked; 0=unlocked
@return     Non-zero if the report is handled; else return zero
*/

void zwif_get_desc(zwif_p intf, zwifd_p desc);
/**<
get interface descriptor without the data portion
@param[in]	intf	interface
@param[out]	desc	descriptor
*/

void zwif_exec_cb(appl_layer_ctx_t *appl_ctx, int8_t tx_sts, void *user_prm, uint8_t node_id, zw_tx_cb_prm_t *param);
/**<
zwif_exec_cb - execute action on an interface callback function
@param[in]	appl_ctx    The application layer context
@param[in]	tx_sts		The transmit complete status
@param[in]	user_prm    The user specific parameter
@param[in]	node_id     Node id
@param[in]	param       Additional parameters
@return
*/

zwif_p zwif_get_if(zwifd_p ifd);
/**<
zwif_get_if - Get the interface of the specified interface descriptor
@param[in]	ifd	    Interface descriptor
@return		The interface if found; else return NULL
@pre        Caller must lock the nw->mtx before calling this function.
*/

zwif_p zwif_create(uint16_t cls, uint8_t ver, uint8_t propty);
/**<
Create interface based on command class
@param[in]	cls	        class
@param[in]	ver	        class version
@param[in]	propty      command class properties
@return		interface; else return NULL
@pre        Caller must lock the nw->mtx before calling this function.
*/

int zwif_get_report_ex(zwifd_p ifd, uint8_t *param, uint8_t len, uint8_t get_rpt_cmd, tx_cmplt_cb_t cb, int flag);
/**<
get interface report through report callback
@param[in]	ifd	        interface
@param[in]	param	    Parameter for the report get command
@param[in]	len     	Length of param
@param[in]	get_rpt_cmd Command to get the report
@param[in]	cb		    callback function for transmit status
@param[in]	flag		flag, ZWIF_RPT_GET_XXX
@return		ZW_ERR_xxx
*/

int zwif_get_report(zwifd_p ifd, uint8_t *param, uint8_t len, uint8_t get_rpt_cmd, tx_cmplt_cb_t cb);
/**<
get interface report through report callback
@param[in]	ifd	        interface
@param[in]	param	    Parameter for the report get command
@param[in]	len     	Length of param
@param[in]	get_rpt_cmd Command to get the report
@param[in]	cb		    callback function for transmit status
@return		ZW_ERR_xxx
*/

int zwif_set_report(zwifd_p ifd, void *rpt_cb, uint8_t rpt);
/**<
Setup a generic report callback function
@param[in]	ifd         Interface descriptor
@param[in]	rpt_cb	    Report callback function
@param[in]	rpt 	    Report command
@return      ZW_ERR_XXX
*/

int zwif_set_sm_report(zwifd_p ifd, void *rpt_cb, uint8_t rpt);
/**<
zwif_set_sm_report - Setup a report callback function to state-machine
@param[in]	ifd         Interface descriptor
@param[in]	rpt_cb	    Report callback function
@param[in]	rpt 	    Report command
@return      ZW_ERR_XXX
@note   This function is reserved for use by state-machine only
*/

void zwif_tx_sts_cb(appl_layer_ctx_t  *appl_ctx, int8_t tx_sts, void *user_prm, uint8_t node_id, zw_tx_cb_prm_t *param);
/**<
callback function to display transmit status
@param[in]	appl_ctx    The application layer context
@param[in]	tx_sts		The transmit complete status
@param[in]	user_prm    The user specific parameter
@param[in]	node_id     Node id
@param[in]	param       Additional parameters
@return
*/

void zwif_grp_cb(appl_layer_ctx_t *appl_ctx, uint8_t sts, uint8_t node_id);
/**<
group return route callback function
@param[in]	appl_ctx    The application layer context
@param[in]	sts		    The complete status
@param[in]	node_id     Source node
@return
*/

int zwif_nameloc_set(zwifd_p ifd, const zw_nameloc_p nameloc);
/**<
Set node name and location string
@param[in]	ifd	        Interface
@param[in]	nameloc	    Name & location string null terminated
@return	ZW_ERR_XXX
*/

int zwif_exec_ex(zwifd_p ifd, uint8_t *cmd_buf, int buf_len, tx_cmplt_cb_t cb, void *user, int opt, void *xtra);
/**<
execute action on an interface with extra parameters
@param[in]	ifd         interface
@param[in]	cmd_buf		command and parameters
@param[in]	buf_len		length of cmd_buf in bytes
@param[in]	cb		    callback function for transmit status
@param[in]	user	    user parameter of callback function
@param[in]	opt		    option, ZWIF_OPT_XXX
@param[in]	xtra		extra parameter
@return	ZW_ERR_xxx
*/

void zwif_cmd_q_cb(appl_layer_ctx_t  *appl_ctx, int8_t tx_sts, void *user_prm, uint8_t node_id, zw_tx_cb_prm_t *param);
/**<
send command from command queue callback function
@param[in]	appl_ctx    The application layer context
@param[in]	tx_sts		The transmit complete status
@param[in]	user_prm    The user specific parameter
@param[in]	node_id     Node id
@param[in]	param       Additional parameters
@return
*/

void zwif_cmd_q_xtra_rm(zwnet_p nw, util_lst_t **xtra_lst_hd);
/**<
remove xtra command queue
@param[in]	nw		    Network
@param[in]	xtra_lst_hd List head
@return
*/

void zwif_dat_rm(zwif_p intf);
/**<
Free command class specific data attached to the interface
@param[in]	intf	Interface
@pre        Caller must lock the nw->mtx before calling this function.
*/

void zwif_dat_mv(zwif_p src_intf, zwif_p dst_intf);
/**<
Move command class specific data attached to the interface
@param[in]	src_intf	Source interface
@param[in]	dst_intf	Destination interface
@pre        Caller must lock the nw->mtx before calling this function.
*/

int zwif_cls_cmd_get(uint8_t* cmd_buf, uint16_t len, uint16_t *cmd_cls, uint8_t *cmd);
/**<
parse a command buffer to get its class and command
@param[in]	cmd_buf		Command buffer
@param[in]  len	        The length of command buffer in bytes
@param[out] cmd_cls	    The command class
@param[out]	cmd	        The command
@return     1 if the command buffer is successfully parsed; otherwise return zero
*/

void zwnet_av_raw_rpt_cb(zwif_p intf, uint16_t cmd_len, uint8_t *cmd_buf);
/**<
simple AV raw report callback
@param[in]	intf	        The interface that received the report
@param[in]	cmd_buf	        Command buffer
@param[in]	cmd_len         Length of command buffer
@return
*/

int zwif_ifd_get(zwifd_p ifd, uint16_t cmd_cls, zwifd_p cmd_cls_ifd);
/**<
get the interface descriptor of the specified command class
@param[in]	ifd	        interface which has the same endpoint as the cmd_cls_ifd
@param[in]	cmd_cls	    specified command class
@param[out]	cmd_cls_ifd	interface descriptor of the specified command class
@return  0 on success, else ZW_ERR_XXX
*/

int zwif_max_pkt_sz(zwif_p intf, uint16_t *max_pkt_sz);
/**<
get maximum allowed packet payload size based on interface security property
@param[in]	intf        interface
@param[out]	max_pkt_sz	maximum allowed packet payload size
@return	ZW_ERR_XXX
*/


/**
@}
@defgroup If_Sec  Security Interface APIs
Security encapsulation packet
@ingroup zwarecapi
@{
*/

typedef void (*zwrep_sup_sec_fn)(zwif_p intf, uint16_t *cls, uint8_t cnt);
/**<
supported security commands report callback
@param[in]	intf	The interface that received the report
@param[in]	cls	    The buffer that store the command classes
@param[in]	cnt	    Command class count
@return
*/

/**
@}
@defgroup If_Firmware Firmware update Interface APIs
Used to update firmwares on a device
@ingroup zwarecapi
@{
*/

typedef void (*zw_fw_tx_fn)(zwif_p intf, uint16_t frag_num, uint8_t rpt_cnt);
/**<
send firmware fragment function
@param[in]	    intf        interface
@param[in]	    frag_num    fragment number requested (fragment number starts from 1)
@param[in]	    rpt_cnt     number of fragments requested
@return     ZW_ERR_XXX
*/

void zwif_fw_updt_clean_up(if_fw_tmp_dat_t *fw_data);
/**<
clean up firmware update process
@param[in]	fw_data	    firmware interface data
@return
@pre        Caller must lock the nw->mtx before calling this function.
*/

void zwnet_fw_updt_free(zwnet_p nw);
/**<
free firmware update allocated memory
@param[in] nw		Network
@return
@pre        Caller must lock the nw->mtx before calling this function.
*/


/**
@}
@defgroup DevCfg Z-wave Device Configuration Functions
Some device configuration functions.
@ingroup zwarecapi
@{
*/

#define DFLT_EVT_CLR_TIMEOUT   300     /**< Default event clear timeout in seconds */
#define MAX_EVT_CLR_TIMEOUT    3600    /**< Maximum event clear timeout in seconds */


void *zwdev_if_get(ep_rec_t *ep_rec, uint8_t ep_id, uint8_t if_type);
/**<
Get interface record
@param[in]	ep_rec	    Device specific configuration endpoint records
@param[in]	ep_id	    Endpoint id
@param[in]	if_type	    Interface record type, IF_REC_TYPE_XXX
@return Interface record pointer on success; else returns NULL
*/

uint8_t zwdev_if_ver_get(ep_rec_t *ep_rec, uint8_t ep_id, uint16_t cmd_cls);
/**<
Get user-defined interface version
@param[in]	ep_rec	    Device specific configuration endpoint records
@param[in]	ep_id	    Endpoint id
@param[in]	cmd_cls	    Command class
@return Interface version (>= 1); 0 if no user-defined interface version found
*/

if_rec_t *zwdev_if_get_by_cmd_cls(ep_rec_t *ep_rec, uint8_t ep_id, uint16_t cmd_cls);
/**<
Get interface record by command class
@param[in]	ep_rec	    Device specific configuration endpoint records
@param[in]	ep_id	    Endpoint id
@param[in]	cmd_cls	    Command class
@return Interface record pointer on success; else returns NULL
*/

uint8_t zwdev_redir_ep_get(ep_rec_t *ep_rec, uint8_t ep_id, uint16_t cmd_cls, uint8_t cmd, uint8_t *ptarget_ep);
/**<
Get redirection target endpoint
@param[in]	ep_rec	    Device specific configuration endpoint records
@param[in]	ep_id	    Endpoint id
@param[in]	cmd_cls	    Command class
@param[in]	cmd			Command
@param[out]	ptarget_ep	The target endpoint id if found
@return 1 -- redirection record is found. Target endpoint ID is stored in ptarget_ep; 0 -- No redirection record found
*/

/**
@}
@defgroup If_IP_Grp IP Group Interface APIs
Groups are application-specific, and normally specify report recipients eg. for a sensor
@ingroup zwarecapi
@{
*/

/** IP association group interface data */
typedef struct
{
    uint8_t         group;          /**< Grouping identifier */
    uint8_t         index;          /**< Current index of report.  Start from 1 up to total number of entries in the group*/
	uint8_t	        tot_entries;    /**< Total number of entries in the group */
	uint8_t	        valid_entries;  /**< Number of valid entries  (those IPv6 in the entry that could be resolved into node id)*/
    grp_member_t    grp_mbr[1];     /**< Place holder for the array of members in the grouping */

} if_ip_grp_dat_t;


/**
@}
@defgroup If_Mailbox Mailbox Service Interface APIs
Used to configure mailbox service
@ingroup zwarecapi
@{
*/

#define MBX_SUPP_BMSK_SERVICE   0x01   ///< Supported mailbox service (bitmask)
#define MBX_SUPP_BMSK_PROXY     0x02   ///< Supported mailbox proxy (bitmask)

#define MBX_MODE_DISABLE        0   ///< Disable both mailbox service and proxy forwarding
#define MBX_MODE_SERVICE        1   ///< Enable mailbox service
#define MBX_MODE_PROXY          2   ///< Enable mailbox proxy forwarding

/** mailbox service configuration */
typedef struct
{
    uint8_t   supp_mode;                    ///< Supported mode: MBX_SUPP_BMSK_XXX
    uint8_t   mode;                         ///< Current operating mode: MBX_MODE_XXX
    uint16_t  frame_cap;                    ///< Number of frames that the mailbox can store
    uint8_t   forward_dest[IPV6_ADDR_LEN];  ///< Destination IPv6 address to forward mail (valid if mode=MBX_MODE_PROXY)
    uint8_t   forward_port;                 ///< UDP forwaring port (valid if mode=MBX_MODE_PROXY)

} mailbox_cfg_t;

typedef void (*mbx_cfg_fn)(zwif_p intf, mailbox_cfg_t *mbx_cfg);
/**<
mailbox configuration report callback function
@param[in]	    intf        interface
@param[in]	    mbx_cfg     mailbox configuration
@return
*/

/**
@}
@defgroup If_Thrm_Setpoint Thermostat Setpoint Interface APIs
Used to control and read settings of thermostat setpoint
@ingroup zwarecapi
@{
*/

/** Thermostat setpoint temparature range */
typedef struct
{
    int         valid;      /**< Flag to indicate whether the range is valid*/
    zwsetp_t    min;        /**< Minimum setpoint temparature */
    zwsetp_t    max;        /**< Maximum setpoint temparature */
}
zwsetp_range_t;

/** Thermostat Setpoint interface data for storing supported types and temparature ranges */
typedef struct
{
	uint8_t		    setp_cnt;       /**< number of supported setpoint types*/
    uint8_t         rsvd1;          /**< Padding to make sure setp_cap is 4-byte aligned */
    uint16_t        rsvd2;          /**< Padding to make sure setp_cap is 4-byte aligned */
    zwsetp_range_t  setp_cap[1];    /**< setpoint types and capabilities place holder*/
} if_thrmo_setp_cap_t;


/**
@}
*/

#endif

