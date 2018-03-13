/**
@file   zip_api_util.h - Z/IP High Level API utility/miscellaneous functions header file.

        To implement some utility/miscellaneous functions.

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
/*
node info file version: 0.1
comments: Initial release

node info file version: 0.2
comments: Added ZW_SUBTAG_BASIC

node info file version: 0.3
comments: -Added ZW_SUBTAG_EXT_VER_CAP, ZW_SUBTAG_HW_VER, ZW_SUBTAG_FW_VER, ZW_SUBTAG_ZWPLUS_VER,
           ZW_SUBTAG_ROLE_TYPE, ZW_SUBTAG_NODE_TYPE and ZW_SUBTAG_AGI_XXX.
          -When the bit-7 of subtag length field is set.  The length field is ignored, the next two
           bytes become the length field.
          -Changed MAX_SUBTAGS_SIZE  to  1200
          -Added storage of Association Group Information interface data.

node info file version: 0.4
comments: -Added ZW_SUBTAG_DEV_ID_TYPE

node info file version: 0.5
comments: -Added ZW_SUBTAG_GRP_NODE

node info file version: 0.6
comments: -Added ZW_SUBTAG_INSTR_ICON and ZW_SUBTAG_USER_ICON
          -Moved ZW_SUBTAG_ZWPLUS_VER, ZW_SUBTAG_ROLE_TYPE and ZW_SUBTAG_NODE_TYPE to endpoint tags.

node info file version: 0.7
comments: -Data structure for COMMAND_CLASS_ALARM has changed

node info file version: 0.8
comments: -Added ZW_SUBTAG_CCH_DAT_CNT, ZW_SUBTAG_CCH_DAT_ITEM_SZ and ZW_SUBTAG_CCH_DATA

node info file version: 0.9
comments: -Extended COMMAND_CLASS_DOOR_LOCK interface cached data to include door lock configuration.

node info file version: 0.10
comments: -Added ZW_SUBTAG_CCH_B2LVL and ZW_SUBTAG_CCH_1LVL_ELE_SZ
	      -Added 2-level cached data saving/loading

node info file version: 0.11
comments: -Added ZW_SUBTAG_S2_KEY_VALID and ZW_SUBTAG_S2_GRNT_KEY

node info file version: 0.12
comments: -Added ZW_SUBTAG_LISTEN, ZW_SUBTAG_AGGR_EP_CNT, ZW_SUBTAG_AGGR_MEMBER_CNT, ZW_SUBTAG_AGGR_MEMBERS

node info file version: 0.13
comments: -Added ZW_SUBTAG_NODE_PROPTY

node info file version: 0.14
comments: -Added ZW_SUBTAG_DSK_STR

node info file version: 0.15
comments: -Changed the data structure for supported thermostat setpoint types

node info file version: 0.16
comments: -Data structure for COMMAND_CLASS_CENTRAL_SCENE has changed

node info file version: 0.17
comments: -Added ZW_SUBTAG_WAKEUP_INTV

node info file version: 0.18
comments: -Removed Basic, doorlock, switch binary and multi-level cache data as formats have changed

node info file version: 0.19
comments: -Data structures for COMMAND_CLASS_THERMOSTAT_OPERATING_STATE, COMMAND_CLASS_THERMOSTAT_MODE have changed

node info file version: 0.20
comments: -Added ZW_SUBTAG_CFG_INFO_STRUCT, ZW_SUBTAG_CFG_NAME and ZW_SUBTAG_CFG_INFO

*/

#ifndef _ZIP_API_UTIL_DAVID_
#define _ZIP_API_UTIL_DAVID_
/**
@defgroup Util Utility/Miscellaneous Functions
Some utility/miscellaneous functions.
@ingroup zwarecapi
@{
*/

#define FILE_MAGIC_NUM_H  0x8A      /**< High byte of magic identifier of the node information file*/
#define FILE_MAGIC_NUM_L  0xB3      /**< Low byte of magic identifier of the node information file*/

#define FILE_VER_NUM_H    0         /**< High byte of node information file version*/
#define FILE_VER_NUM_L    20        /**< Low byte of node information file version*/

#define NI_FILE_HDR_LEN     4       /**< Node information file header length*/


/** Tag size definition */
#define ZW_TAG_ID_SIZE      2       /**< Tag identifier field size in bytes*/
#define ZW_TAG_LEN_SIZE     2       /**< Tag length field size in bytes*/
#define ZW_SUBTAG_ID_SIZE   1       /**< Sub-tag identifier field size in bytes*/
#define ZW_SUBTAG_LEN_SIZE  1       /**< Sub-tag length field size in bytes*/
#define MAX_SUBTAGS_SIZE    1200    /**< Maximum subtags size, used for pre-allocate memory */


/** Tag identifier definition */
#define ZW_TAG_NODE      0xA701     /**< Node tag id*/
#define ZW_TAG_EP        0xA702     /**< End point tag id*/
#define ZW_TAG_IF        0xA703     /**< Interface tag id*/
#define ZW_TAG_NW        0xA704     /**< Network tag id*/


/** Node sub-tag identifier definition */
#define ZW_SUBTAG_NODE_ID            0       /**< Node ID sub-tag id*/
#define ZW_SUBTAG_VID                1       /**< Vendor ID sub-tag id*/
#define ZW_SUBTAG_VTYPE              2       /**< Vendor Product Type sub-tag id*/
#define ZW_SUBTAG_PID                3       /**< Product ID sub-tag id*/
#define ZW_SUBTAG_NAME               4       /**< DEPRECATED:User configured name string of device sub-tag id*/
#define ZW_SUBTAG_LOC                5       /**< DEPRECATED:User configured location string of device sub-tag id*/
#define ZW_SUBTAG_MULCH_VER          6       /**< Multi-channel version sub-tag id*/
#define ZW_SUBTAG_EP_CNT             7       /**< Number of end points sub-tag id*/
#define ZW_SUBTAG_SLEEP_CAP          8       /**< Flag to indicate the node is capable to sleep sub-tag id*/
#define ZW_SUBTAG_SND_SEQ_N          9       /**< 4-bit sequence number in sending split secure packet*/
#define ZW_SUBTAG_UNUSED_1          10       /**< Unused*/
#define ZW_SUBTAG_CRC16_CAP         11       /**< Flag to indicate the node is capable to check/generate CRC-16 checksum*/
#define ZW_SUBTAG_DEV_ID_FMT        12       /**< Device id format*/
#define ZW_SUBTAG_DEV_ID_LEN        13       /**< Device id length*/
#define ZW_SUBTAG_DEV_ID_DAT        14       /**< Device id */
#define ZW_SUBTAG_LIB_TYPE          15       /**< Z-Wave Library Type sub-tag id*/
#define ZW_SUBTAG_PROT_VER          16       /**< Z-Wave Protocol Version sub-tag id*/
#define ZW_SUBTAG_APPL_VER          17       /**< Application Version sub-tag id*/
#define ZW_SUBTAG_MUL_CMD_CAP       18       /**< Flag to indicate the node is capable of multi command encapsulation*/
#define ZW_SUBTAG_SENSOR            19       /**< Flag to indicate the node is a sensor (FLIRs) that requires wake up beam*/
#define ZW_SUBTAG_CATEGORY          20       /**< Device category*/
#define ZW_SUBTAG_BASIC             21       /**< Basic Device Class*/
#define ZW_SUBTAG_EXT_VER_CAP       22       /**< Flag to indicate the node supports extended version information
                                                  and firmware versions count*/
#define ZW_SUBTAG_HW_VER            23       /**< Hardware version*/
#define ZW_SUBTAG_FW_VER            24       /**< Firmware versions*/
#define ZW_SUBTAG_UNUSED_2          25       /**< Unused*/
#define ZW_SUBTAG_UNUSED_3          26       /**< Unused*/
#define ZW_SUBTAG_UNUSED_4          27       /**< Unused*/
#define ZW_SUBTAG_DEV_ID_TYPE       28       /**< Device ID type */
#define ZW_SUBTAG_S2_KEY_VALID      29       /**< Flag to mark S2 granted keys bit-mask is valid */
#define ZW_SUBTAG_S2_GRNT_KEY       30       /**< S2 granted keys bit-mask */
#define ZW_SUBTAG_LISTEN            31       /**< Flag to indicate the node is always listening (awake)*/
#define ZW_SUBTAG_AGGR_EP_CNT       32       /**< Number of aggregated end points sub-tag id*/
#define ZW_SUBTAG_NODE_PROPTY       33       /**< Node properties bit mask*/
#define ZW_SUBTAG_DSK_STR           34       /**< DSK string */
#define ZW_SUBTAG_WAKEUP_INTV       35       /**< Wakeup interval */
#define ZW_SUBTAG_SE_STATUS         36       /**< Security status */


/** End point sub-tag identifier definition */
#define ZW_SUBTAG_EP_ID             0       /**< End point ID sub-tag id*/
#define ZW_SUBTAG_GEN               1       /**< Generic Device Class sub-tag id*/
#define ZW_SUBTAG_SPEC              2       /**< Specific Device Class sub-tag id*/
#define ZW_SUBTAG_EP_NAME           3       /**< User configured name string of endpoint sub-tag id*/
#define ZW_SUBTAG_EP_LOC            4       /**< User configured location string of endpoint sub-tag id*/
#define ZW_SUBTAG_ZWPLUS_VER        5       /**< Z-Wave+ version*/
#define ZW_SUBTAG_ROLE_TYPE         6       /**< Z-Wave+ role type*/
#define ZW_SUBTAG_NODE_TYPE         7       /**< Z-Wave+ node type*/
#define ZW_SUBTAG_INSTR_ICON        8       /**< Z-Wave+ installer icon type*/
#define ZW_SUBTAG_USER_ICON         9       /**< Z-Wave+ user icon type*/
#define ZW_SUBTAG_AGGR_MEMBER_CNT   10      /**< Aggregated end point member count*/
#define ZW_SUBTAG_AGGR_MEMBERS      11      /**< Aggregated end point members*/


/** Interface sub-tag identifier definition */
#define ZW_SUBTAG_CLS               0       /**< Command class sub-tag id*/
#define ZW_SUBTAG_VER               1       /**< Command class version sub-tag id*/
#define ZW_SUBTAG_PROPTY            2       /**< Command class properties sub-tag id*/
#define ZW_SUBTAG_DAT_CNT           3       /**< Number of data items*/
#define ZW_SUBTAG_DAT_ITEM_SZ       4       /**< Data item size in bytes*/
#define ZW_SUBTAG_DATA              5       /**< Data items which is specific to the command class*/
#define ZW_SUBTAG_AGI_GRP_CNT       6       /**< Association group information: number of groups*/
#define ZW_SUBTAG_AGI_GRP_DYNAMIC   7       /**< Association group information: flag to indicate the group info is dynamic*/
#define ZW_SUBTAG_AGI_GRP_ID        8       /**< Association group information: group id*/
#define ZW_SUBTAG_AGI_CMD_CNT       9       /**< Association group information: number of entries in command list*/
#define ZW_SUBTAG_AGI_PROFILE       10      /**< Association group information: profile*/
#define ZW_SUBTAG_AGI_EVENT         11      /**< Association group information: event code*/
#define ZW_SUBTAG_AGI_GRP_NAME      12      /**< Association group information: group name*/
#define ZW_SUBTAG_AGI_CMD_LIST      13      /**< Association group information: command list*/
#define ZW_SUBTAG_CCH_DAT_CNT       14      /**< Number of cached data items*/
#define ZW_SUBTAG_CCH_DAT_ITEM_SZ   15      /**< Cached data item size in bytes*/
#define ZW_SUBTAG_CCH_DATA          16      /**< Cached data items which is specific to the command class*/
#define ZW_SUBTAG_CCH_B2LVL			17      /**< whether cch_data is a 2 level cached data format*/
#define ZW_SUBTAG_CCH_1LVL_ELE_SZ	18      /**< whether cch_data is a 2 level cached data format*/
#define ZW_SUBTAG_CFG_INFO_STRUCT   19      /**< Configuration info capability struct "zwcfg_info_t" */
#define ZW_SUBTAG_CFG_NAME          20      /**< Configuration parameter name with terminating NUL character */
#define ZW_SUBTAG_CFG_INFO          21      /**< Configuration parameter usage info with terminating NUL character */


/** Network sub-tag identifier definition */
#define ZW_SUBTAG_HOME_ID   0       /**< Home ID sub-tag id*/
#define ZW_SUBTAG_GRP_NODE  1       /**< Group 1 node sub-tag id*/


/** Tag container */
typedef struct
{
    uint16_t    id;		    /**< Tag id */
    uint16_t    len;	    /**< The length of the data[] */
    uint8_t     data[1];    /**< Data storage place holder */
}
zwtag_t, *zwtag_p;


int zwutl_ni_load(zwnet_p nw, const char *ni_file);
/**<
load node information from file
@param[in]	nw	        network
@param[in]	ni_file	    node information file
@return         ZW_ERR_XXX.
*/


int zwutl_ni_save(zwnet_p nw, const char *ni_file);
/**<
save node information into file
@param[in]	nw	        network
@param[in]	ni_file	    node information file
@return         ZW_ERR_XXX.
*/

#define POLY        0x1021      /**< crc-ccitt mask */
#define CRC_INIT    0x1D0F      /**< crc-ccitt initialization value */


#define     CRC16_OVERHEAD      4   /**< CRC-16 Encapsulation Command Class overhead */

uint16_t zwutl_crc16_chk(uint16_t crc, uint8_t *data, uint32_t len);
/**<
CRC-CCITT (0x1D0F) calculation / check
@param[in]	crc	        CRC initialization value
@param[in]	data	    Buffer that store the byte string
@param[in]	len	        Length of byte string
@return         If data contains the 16-bit checksum field: return zero if check o.k.
                If data excludes the 16-bit checksum field: return CRC-16 checksum.
*/

int zwutl_file_load(const char *dat_file, uint8_t **buf);
/**<
Load a data file into a buffer
@param[in]	dat_file	Data file
@param[out]	buf	        Buffer for storing the file content
@return         ZW_ERR_XXX.
@post       Caller must free the buf
*/

#define PREF_FILE_MAGIC_NUM_H  0x18      /**< High byte of magic identifier of the preference file*/
#define PREF_FILE_MAGIC_NUM_L  0x3F      /**< Low byte of magic identifier of the preference file*/

#define PREF_FILE_VER_NUM_H    0         /**< High byte of preference file version*/
#define PREF_FILE_VER_NUM_L    0x01      /**< Low byte of preference file version*/

#define PREF_FILE_HDR_LEN      4         /**< Preference file header length*/
#define PREF_FILE_CHKSUM_LEN   2         /**< Preference file checksum length*/

int zwutl_pref_get(zwnet_p net, uint32_t client_id, void **buf, uint16_t *buf_size, int use_client_id);
/**<
Retrieve network/client preference from persistent storage
@param[in]	net		    Network
@param[in]	client_id	User-defined client id to identify the preference
@param[out]	buf		    Return buffer that contains the network preference
@param[out]	buf_size	Buffer size in bytes
@param[in]	use_client_id	Flag to indicate whether to use client_id
@return		ZW_ERR_XXX
@post       Caller MUST free the return buffer "buf" if the call is successful.
@see		zwutl_pref_set
*/

int zwutl_pref_set(zwnet_p net, void *buf, uint16_t buf_size, uint32_t client_id, int use_client_id);
/**<
Store network/client preference into persistent storage
@param[in]	net		    Network
@param[in]	buf		    Buffer that contains the network preference
@param[in]	buf_size	Buffer size in bytes
@param[in]	client_id	User-defined client id to identify the preference
@param[in]	use_client_id	Flag to indicate whether to use client_id
@return		ZW_ERR_XXX
@see		zwutl_pref_get
*/

int zwutl_file_name_get(char *non_client_pfx, char *dir_path, uint32_t homeid, uint32_t client_id,
                        char **file_name, int use_client_id);
/**<
Get file name based on home id and optionally client id
@param[in]	non_client_pfx	Non-client (i.e. use_client_id=0)file prefix
@param[in]	dir_path	Directory path of file name
@param[in]	homeid	    Home id
@param[in]	client_id	Client id
@param[out]	file_name	File name which is derived from network home id and optionally client id
@param[in]	use_client_id	Flag to indicate whether to use client_id
@return		ZW_ERR_XXX
@post   Caller must free the memory allocated to file_name if this call is successful
*/

int zwutil_dsk_chk(char *dsk);
/**<
Check Device Specific Key (DSK) format
@param[in]	dsk	    DSK to be checked
@return     Non-zero if check o.k.; else zero on error
*/

char *zwutil_dsk_to_str(uint8_t *dsk, uint8_t key_len);
/**<
Convert Device Specific Key (DSK) to string
@param[in]	dsk	    DSK
@param[in]	key_len	DSK key length in bytes
@return     Converted string if o.k.; else return NULL
@post   Caller MUST free the return string if return value is non-NULL
*/

int zwutil_str_to_dsk(char *dsk, uint8_t *dsk_bin, uint8_t *key_len);
/**<
Convert Device Specific Key (DSK) string to binary
@param[in]	dsk	    DSK string
@param[out]	dsk_bin	Buffer to store DSK binary
@param[out]	key_len	DSK binary length
@return     Non-zero if o.k.; else zero on error
*/

uint16_t zwutil_sec_encap_fmt_get(uint8_t grnt_keys);
/**<
Find the highest security key from the granted keys
@param[in]	grnt_keys	    S2 granted keys, bitmask of SEC_KEY_BITMSK_XXX
@return     highest security key formatted in Z/IP header extension Encapsulation Format Info.
*/

uint16_t zwutl_file_crc_get(FILE *file);
/**<
Get file CRC checksum
@param[in]	file	 File to be checked
@return         CRC checksum
*/

int zwutil_is_cmd_set(uint16_t cls, uint16_t cmd);
/**<
Check if the Z-wave command is type "set"
@param[in]	cls	    Z-wave command class
@param[in]	cmd	    Z-wave command
@return     non-zero if it is type "set"; else return 0
*/

/** Dynamic buffer */
typedef struct
{
    int         status;     /**< Status: 0=ok; -1=error, don't use the buffer*/
    uint8_t     *buf;       /**< Buffer */
    uint32_t    len;        /**< Buffer length */
    uint32_t    wr_ofs;     /**< Write offset (i.e. point to the empty location for writing) */
    uint32_t    alloc_sz;   /**< Memory block size to allocate if not enough memory*/
}
zwutil_buf_t;


zwutil_buf_t *zwutl_buf_init(uint32_t alloc_sz);
/**<
Initialize dynamic buffer
@param[in] alloc_sz     Memory block size to allocate for initial and subsequent allocation
@return Pointer to dynamic buffer if successful; else return NULL
@post  Caller is required to call zwutl_buf_exit to free the memory allocated if this call is successful
*/

void zwutl_buf_exit(zwutil_buf_t *buf);
/**<
Clean up dynamic buffer
@param[in] buf     Dynamic buffer
@return
*/

uint8_t *zwutl_buf_get_wptr(zwutil_buf_t *buf, uint32_t size);
/**<
Get buffer pointer for writing
@param[in] buf     Dynamic buffer
@param[in] size    Size to write in bytes
@return Write buffer pointer if successful; else return NULL
*/


/**
@}
*/



#endif /* _ZIP_API_UTIL_DAVID_ */

