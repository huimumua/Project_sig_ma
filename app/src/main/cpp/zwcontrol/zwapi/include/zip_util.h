/**
@file   zip_util.h - Z/IP interface utility/miscellaneous functions header file.

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

#ifndef _ZIP_UTIL_DAVID_
#define _ZIP_UTIL_DAVID_
/**
@defgroup Util Utility/Miscellaneous Functions
Some utility/miscellaneous functions.
@ingroup zwarecapi
@{
*/

///
/// List entry
typedef struct  _util_lst
{
    struct _util_lst  *next;        ///< Point to the next list entry
    uint16_t          dat_sz;       ///< Data size
    uint16_t          rsvd;         ///< Padding to make sure wr_buf is 4-byte aligned
    uint8_t           wr_buf[1];    ///< Place holder for the data buffer. MUST be 4-byte aligned.

} util_lst_t;


///
/// List entry
typedef struct
{
    uint32_t          if_idx;       ///< Interface index for the IPv6 IP address
    uint8_t           addr[16];     ///< IPv6 IP address

} util_ipv6_addr_t;


///
/// List compare function that return 0 if s1 is same as s2; else return non-zero
typedef int (*util_list_cmp_fn)(uint8_t *s1, uint8_t *s2);

typedef void (*util_gw_discvr_cb_t)(uint8_t *gw_addr, uint8_t gw_cnt, int use_ipv4, void *usr_param, int rpt_num, int total_rpt, char **gw_name);
/**<
@param[in]	gw_addr	    Gateway addresses
@param[in]	gw_cnt	    Number of gateway addresses returned in gw_addr
@param[in]	use_ipv4    Flag to indicate the addr parameter is IPv4 or IPv6. 1=IPv4; 0=IPv6
@param[in]	usr_param   User defined parameter used in callback function
@param[in]	rpt_num     Report number that this callback is delivering the gateway addresses report; start from 1
@param[in]	total_rpt   Total reports that will be delivered by callbacks. Each callback delivers one report.
@param[in]	gw_name	    Gateway names corresponding to the gw_ip.  If NULL, it means gateway name information is unavailable.
*/

///
/// Z/IP Gateway discovery context
typedef struct
{
    volatile int        gw_thrd_run;    ///< control the gateway discovery thread whether to run. 1 = run, 0 = stop
    volatile int        gw_thrd_sts;    ///< gateway discovery thread status. 1 = running, 0 = thread exited
    int                 use_ipv4;       ///< flag to indicate whether to use IPv4 as transport IP protocol
    util_gw_discvr_cb_t cb;             ///< gateway discovery callback function
    void                *usr_param;     ///< user defined parameter used in callback function
} gw_discvr_ctx_t;


typedef int (*util_msg_loop_fn)(void *usr_prm, void *msg);
/**<
message loop on message arrives callback
@param[in]	usr_prm	    user parameter supplied during initialization
@param[in]	msg	        message
return      1 to exit message loop; else return 0 to continue the message loop
*/

typedef void (*util_msg_free_fn)(void *msg);
/**<
function to free the content of message apart from the message itself
@param[in]	msg	        message
return
@note    The function should not free the "msg" itself, it is freed by the message loop related function.
*/

///
/// Message loop context
typedef struct
{
    volatile int            thrd_run;   /**< Control the thread whether to run. 1 = run, 0 = stop*/
    volatile int            thrd_sts;   /**< Thread status. 1 = run, 0 = thread exited*/
    void                    *sem;       /**< Semaphore for waiting messages*/
    struct plt_mtx_t        *mtx;       /**< Mutex for message queue */
    util_lst_t              *msg_hd;    /**< Head of linked list of messages*/
    util_msg_free_fn        free_fn;    /**< Function to free message content (optional)*/
    unsigned                id;         /**< Identifier for the message loop (for debug use)*/

    //Callback
    util_msg_loop_fn        cb;         /**< Callback function on message arrives*/
    void                    *usr_prm;   /**< User supplied parameter*/

} util_msg_loop_t;


///
/// Timer tick context
typedef struct
{
    volatile int            thrd_run;   /**< Control the thread whether to run. 1 = run, 0 = stop*/
    volatile int            thrd_sts;   /**< Thread status. 1 = run, 0 = thread exited*/
    volatile uint32_t       tick;       /**< Timer tick*/
    uint32_t                tick_intv_ms;/**< Tick interval in milliseconds*/
} util_tmr_tick_t;

/** Number type that supports floating point */
typedef struct
{
    uint8_t     precision;  /**< Decimal places of the value.  The decimal
                                 value 1025 with precision 2 is therefore equal to 10.25 */
    uint8_t     size;	    /**< Data size: 1,2,or 4 bytes*/
    uint8_t     data[4];	/**< Setpoint data (a signed number) with the first byte as MSB */
} zwnum_type_t;

int     util_list_add(struct plt_mtx_t *mtx_ctx, util_lst_t **head, uint8_t  *buf, uint16_t dat_sz);
/**<
add an entry into the end of the list.
@param[in]      mtx_ctx     Mutex context; optional, if NULL no locking will be used.
@param[in,out]	head		Pointer to the list head
@param[in]      buf         Buffer that store the data
@param[in]      dat_sz      Size of data to be stored
@return                     Return 0 on success, negative error number on failure.
*/

int     util_list_add_no_dup(struct plt_mtx_t *mtx_ctx, util_lst_t **head, uint8_t  *buf, uint8_t dat_sz, util_list_cmp_fn cmp_fn);
/**<
add an entry into the end of the list if the content of the buffer is different from the last entry.
@param[in]      mtx_ctx     Mutex context; optional, if NULL no locking will be used.
@param[in,out]	head		Pointer to the list head
@param[in]      buf         Buffer that store the data
@param[in]      dat_sz      Size of data to be stored
@param[in]      cmp_fn      The comparison function to identify the entry is identical with the last entry
@return                     Return 0 on success, 1 if the contents are same; else negative error number on failure.
*/

util_lst_t *util_list_get(struct plt_mtx_t *mtx_ctx, util_lst_t **head);
/**<
get the entry from the beginning of the list.
@param[in]      mtx_ctx     Mutex context; optional, if NULL no locking will be used.
@param[in, out]	head		List head
@return     The first entry in the list if the list is not empty; otherwise, NULL.
@post       The caller should free the returned entry.
*/

void    util_list_flush(struct plt_mtx_t *mtx_ctx, util_lst_t **head);
/**<
flush the list.
@param[in]      mtx_ctx     Mutex context; optional, if NULL no locking will be used.
@param[in, out]	head		List head
@return
*/

int     util_list_find(struct plt_mtx_t *mtx_ctx, util_lst_t *head, uint8_t  *buf, util_list_cmp_fn cmp_fn);
/**<
find an entry from the list without modifying the list.
@param[in]      mtx_ctx     Mutex context; optional, if NULL no locking will be used.
@param[in]	    head		Pointer to the list head
@param[in]      cmp_fn      The comparison function to identify the entry using the key stored in buf
@param[in,out]  buf         Buffer that stores the key to be searched. On success, this buffer will be
                            returned with found entry.
@return                     Return non-zero on success, zero on failure.
*/

int     util_list_rm(struct plt_mtx_t *mtx_ctx, util_lst_t **head, uint8_t  *buf, util_list_cmp_fn cmp_fn);
/**<
remove the specified entry from the list.
@param[in]      mtx_ctx     Mutex context; optional, if NULL no locking will be used.
@param[in]	    head		List head
@param[in]      cmp_fn      The comparison function to identify the entry using the key stored in buf
@param[in]      buf         Buffer that stores the key to be searched.
@return                     Return non-zero on success, zero on failure.
*/

int     util_list_rplc(struct plt_mtx_t *mtx_ctx, util_lst_t **head, uint8_t  *buf, uint16_t dat_sz, util_list_cmp_fn cmp_fn);
/**<
replace the specified entry from the list if found; else add the entry to the end of the list.
@param[in]      mtx_ctx     Mutex context; optional, if NULL no locking will be used.
@param[in]	    head		List head
@param[in]      buf         Buffer that stores the entry to be added.
@param[in]      dat_sz      Size of data to be stored
@param[in]      cmp_fn      The comparison function to identify the entry using the key stored in buf
@return     1=entry has been replaced, 0=entry has been added; negative value on error.
*/

int     util_list_updt(struct plt_mtx_t *mtx_ctx, util_lst_t *head, uint8_t  *buf, util_list_cmp_fn cmp_fn);
/**<
find an entry from the list and update its content.
@param[in]      mtx_ctx     Mutex context; optional, if NULL no locking will be used.
@param[in]	    head		Pointer to the list head
@param[in]      cmp_fn      The comparison function to identify the entry using the key stored in buf
@param[in]      buf         Buffer that stores the key to be searched and the entry's new content
@return                     Return non-zero on success, zero on failure.
*/

void    util_hex_string_add(char *src, unsigned src_size, unsigned num);
/**<
append an hex number to a string
@param[in,out]	src		    The string where an hex number is to be appended
@param[in]	    src_size	The size of the src buffer
@param[in]	    num		    The unsigned number that is to be converted to hex number string
@return
*/

void    util_num_string_add(char *src, unsigned src_size, unsigned num);
/**<
append an unsigned number to a string
@param[in,out]	src		    The string where an unsigned number is to be appended
@param[in]	    src_size	The size of the src buffer
@param[in]	    num		    The unsigned number that is to be converted to number string
@return
*/

void    util_ntohs(uint16_t *src, unsigned elem_cnt);
/**<
conversion from big-endian to host byte order
@param[in,out]	src		    The string for conversion from big-endian to host byte order
@param[in]	    elem_cnt	The number of elements (2-byte words) in the src
@return
*/

int     util_tmr_exp_chk(uint16_t now, uint16_t timer);
/**<
Check whether the timer has expired
@param[in] now     Current time
@param[in] timer   The time of the timer to check
@return 1=timer has expired; otherwise 0
*/

uint16_t    util_time_diff(uint16_t now, uint16_t old_time);
/**<
Calculate time difference; taking wrap-over case into consideration
@param[in] now      Current time
@param[in] old_time Previous time
@return time difference
*/

int util_tmr_exp_chk32(uint32_t now, uint32_t timer);
/**<
Check whether the 32-bit timer has expired
@param[in] now     Current time
@param[in] timer   The time of the timer to check
@return 1=timer has expired; otherwise 0
*/

uint32_t   util_time_diff32(uint32_t tm1, uint32_t tm2);
/**<
Calculate 32-bit time difference (i.e. tm1 - tm2) in terms of timer ticks
@param[in] tm1     Time 1
@param[in] tm2     Time 2
@return time different in terms of timer tick
*/

int     util_is_host_big_endian(void);
/**<
Get host endianess
@return 1 if host is big endian; otherwise 0
*/

int     util_cmd_cls_parse(uint8_t *cmd_buf, uint8_t cmd_buf_len, uint16_t **cmd_cls, uint8_t *cmd_cnt,
                           uint16_t **cmd_cls_sec, uint8_t *cmd_cnt_sec);
/**<
Parse buffer that stores mixture of 1-byte and 2-byte command classes
@param[in] cmd_buf      Command buffer
@param[in] cmd_buf_len  Size of command buffer
@param[out] cmd_cls     Output insecure command classes without any marker
@param[out] cmd_cnt     Output insecure command classes count
@param[out] cmd_cls_sec Output secure command classes without any marker
@param[out] cmd_cnt_sec Output secure command classes count
@return     Non-zero if no error; otherwise return zero
@post       If return value is non-zero, caller needs to free the cmd_cls and cmd_cls_sec buffers.
*/

int     util_ip_aton(char *addr_str, uint8_t *addr_buf, int *ipv4);
/**<
Convert IPv4 / IPv6 address string to numeric equivalent
@param[in]  addr_str     Null terminated IPv4 / IPv6 address string
@param[out] addr_buf     Buffer that should be at least 16-byte long to store the result
@param[out] ipv4         Flag to indicate the converted neumeric is IPv4 or IPv6. 1=IPv4; 0=IPv6.
@return     Zero if successful; otherwise non-zero
*/

int     util_ip_ntoa(uint8_t *addr, char *addr_str, int addr_str_len, int ipv4);
/**<
Convert IPv4 / IPv6 address in numerical form to string equivalent
@param[in]  addr         IPv4 / IPv6 address in numerical form
@param[out] addr_str     buffer to store the converted address string
@param[in]  addr_str_len the size of the addr_str in bytes
@param[in]  ipv4         Flag to indicate the addr parameter is IPv4 or IPv6. 1=IPv4; 0=IPv6.
@return     Zero if successful; otherwise non-zero
*/

void    *util_gw_discvr_start(util_gw_discvr_cb_t cb, void *usr_param, int use_ipv4);
/**<
Start Z/IP gateway discovery
@param[in]	    cb		    Callback function when the gateway discovery has completed
@param[in]	    usr_param	User defined parameter used in callback function
@param[in]	    use_ipv4	Flag to indicate whether to use IPv4 as transport IP protocol. 1= use IPv4; 0= use IPv6
@return Context on success, null on failure.
@post   Caller is required to call util_gw_discvr_stop() with the returned context if it is not null.
*/

int     util_gw_discvr_stop(void *ctx);
/**<
Stop Z/IP gateway discovery
@param[in]	    ctx		    The context returned from the call to util_gw_discvr_start()
@return Zero on success, non-zero on failure.
@post After the call, the ctx is invalid and should not be used
*/

int     util_local_addr_get(uint8_t *dest_ip, uint8_t *local_ip, int use_ipv4);
/**<
Get local address that is reachable by destination host
@param[in]  dest_ip     Destination host address either IPv4 or IPv6 according to use_ipv4 flag
@param[in]  use_ipv4    Flag to indicate IP address type. 1= IPv4; 0= IPv6
@param[out] local_ip    Buffer of 16-byte to store the local address (either IPv4 or IPv6 according to use_ipv4 flag)
@return     Zero if successful; otherwise non-zero
*/

int     util_ipv6_zero_cmp(uint8_t *ip);
/**<
Compare the specified IPv6 address to all-zero IPv6 address
@param[in]  ip          IPv6 address
@return     Zero if same ; otherwise non-zero
*/

int     util_ipv4_zero_cmp(uint8_t *ip);
/**<
Compare the specified IPv4 address to 0.0.0.0
@param[in]  ip          IPv4 address
@return     Zero if same ; otherwise non-zero
*/

int     util_is_ipv4_mapped_ipv6(uint8_t *ip);
/**<
Check if the address is IPv4-mapped-IPv6
@param[in]  ip          IPv6 address
@return     non-zero if the address is IPv4-mapped-IPv6; otherwise zero
*/

int     util_ip_addr_get(int num_addr, void *ip_addr_buf, int use_ipv4);
/**<
Get addresses of all the IPv4 / IPv6 network in the system
@param[in]  use_ipv4     Flag to indicate whether to get IPv4 addresses. 1= IPv4; 0= IPv6
@param[in]  num_addr     The number of addresses the ip_addr_buf buffer can store
@param[out] ip_addr_buf  Output buffer to store the IPv4 / IPv6 addresses.
                         For IPv4, it should be a buffer of 4-byte for each entry.
                         For IPv6, it should be an array of type util_ipv6_addr_t
@return     The number of IPv4 / IPv6 addresses actually found.
*/

int util_msg_loop_init(util_msg_loop_fn cb, void *usr_prm, util_msg_free_fn free_fn, unsigned id, util_msg_loop_t **ctx);
/**<
Initialize message loop
@param[in]  cb      Callback function on message arrival
@param[in]  usr_prm	User supplied parameter which will be used on callback
@param[in]  free_fn	User supplied function to free the content of message. This is optional.
                    If not used, set it to NULL.
@param[in]  id	    Identifier for the message loop (for debug use)
@param[out] ctx     Context
@return     Return 0 on success, negative error number on failure.
@post       Caller should call util_msg_loop_shutdown() to stop the message loop and util_msg_loop_exit()
            to free the allocated memory
*/

int util_msg_loop_shutdown(util_msg_loop_t *msg_ctx, uint32_t tm_out);
/**<
Stop the message loop
@param[in]  msg_ctx Context
@param[in]  tm_out	Timeout (in milliseconds) to wait for message loop to stop. If zero, no timeout.
@return     Return 0 on success, negative error number on failure.
*/

void util_msg_loop_exit(util_msg_loop_t *msg_ctx);
/**<
Free the allocated memory
@param[in]  msg_ctx Context
@return
@pre  Should call util_msg_loop_shutdown() first, then wait for the appropriate time to call this function safely
@post Do not call util_msg_loop_send() after calling this function
*/

int util_msg_loop_send(util_msg_loop_t *msg_ctx, void *msg, uint8_t msg_sz);
/**<
Send a message to message loop
@param[in]  msg_ctx Context
@param[in]  msg	    Message
@param[in]  msg_sz  Message size
@return     Return 0 on success, negative error number on failure.
*/

int util_time_stamp(time_t *ts);
/**<
Get the current calendar time as time stamp
@param[out]  ts     Time stamp which represents the number of seconds elapsed since 00:00:00 on January 1, 1970,
                    Coordinated Universal Time
@return     Return 0 on success; else negative value.
*/

time_t util_time_get(void);
/**<
Get the current calendar time
@return     The number of seconds elapsed since 00:00:00 on January 1, 1970, Coordinated Universal Time
*/

char *util_path_dup(const char *path);
/**<
Duplicate and add appropriate path separator of a directory path
@param[in]  path    Directory path
@return     Return duplicated path if successful; else return NULL
@post       Caller MUST free the returned path if it is non NULL
*/

#define UNIQUELY_ADD_TO_ARRAY(pArray, data, arrayLen)								\
{																					\
	int k = 0;																		\
	for(k = 0; k < arrayLen; k++)													\
	{																			\
		if(pArray[k] == data)														\
		{																	\
			break;																	\
		}																	\
	}																			\
	if(k == arrayLen)																\
	{																			\
		pArray[arrayLen++] = data;													\
	}																			\
}
/**<
Add data into array if the data is not already in.
@param[in,out]  pArray    The array to be examined.
@param[in]		data	  The data to be added to array
@param[in,out]  arrayLen   The length of the current array. If the data is added, it will be updated as well.
@note     If the data already exists in the array, no add will be performed to the array.
*/

#define UNIQUELY_ADD_TO_BITMASK(pBitMaskArray, data, startPos, maxArrayLen)			\
{																					\
	int longBitMask = data - 1 + startPos;											\
	if (longBitMask < maxArrayLen*8)												\
	{																				\
		pBitMaskArray[longBitMask / 8] |=  1 << (longBitMask % 8);					\
	}																				\
}
/**<
Convert data into bitmask and add into bitmask array.
@param[in,out]  pBitMaskArray    The bitmask array to be examined.
@param[in]		data			 The data to be converted and added to array
@param[in]		startPos		 Start bit position of the bitmask array (usually 0 or 1)
@param[in]		maxArrayLen		 The maximum length of the bitmask array in bytes.
*/

#define REMOVE_STRUCTLIST(pHead, type)												\
{																					\
	type *pTemp = pHead;															\
	while(pTemp)																	\
	{																				\
		pHead = pHead->next;														\
		if(pTemp)																	\
			free(pTemp);															\
		pTemp = pHead;																\
	}																				\
}
/**<
Free/remove a struct list with the next member in the struct is named 'next'.
@param[in]		pHead			 Head of the struct list
@param[in]		type			 Type of the structure
*/

#define FIND_LASTSTRUCT(pHead, pLast)											    \
{																					\
	pLast = pHead;																	\
	if(pLast != NULL)																\
	{																			    \
		while(pLast->next != NULL)													\
		{																			\
			pLast = pLast->next;													\
		}																			\
	}																				\
}
/**<
Find the last struct in a linked-list with the next member named 'next'.
@param[in]		pHead			 Head of the struct list
@param[out]		pLast			 Last element in the struct
*/

int util_tmr_tick_init(uint32_t tick_intv_ms, util_tmr_tick_t **ctx);
/**<
Initialize timer tick generator
@param[in]  tick_intv_ms    Timer tick interval in milliseconds
@param[out] ctx             Context
@return     Return 0 on success, negative error number on failure.
@post       Caller should call util_tmr_tick_exit() to free the allocated memory
*/

void util_tmr_tick_exit(util_tmr_tick_t *tmr_tick_ctx);
/**<
Stop timer tick and free the allocated memory
@param[in]  tmr_tick_ctx Context
@return
*/

uint32_t util_tmr_tick_get(util_tmr_tick_t *tmr_tick_ctx);
/**<
Get current timer tick count
@param[in]  tmr_tick_ctx Context
@return timer tick count
*/

struct msghdr;
int util_rx_pkt_dest_addr_get(struct msghdr *mh, uint8_t *hdr_dest_addr, int use_ipv4);
/**<
Get received packet header destination address
@param[in]	mh		        Received message header
@param[out]	hdr_dest_addr	Buffer to store header destination address
@param[in] use_ipv4         Flag to indicate whether to use IPv4
@return          Non-zero on success; otherwise zero on failure.
*/

char *util_file_ext_get(const char *file_name);
/**<
Get file name extension
@param[in]  file_name   File path
@return     File extension; otherwise NULL.
*/

int util_file_write(const char *file_name, uint8_t *buf, size_t len);
/**<
Write a buffer info file
@param[in]      file_name     file name
@param[in]      buf           buffer that contains data to be written
@param[in]      len           buffer length
@return		Non-zero on success; otherwise zero.
*/

int util_intel_hex_2_bin(FILE *hex_file, uint32_t max_sz, uint8_t **out_buf, uint32_t *out_len);
/**<
Convert Intel hex to binary
@param[in]  hex_file   File pointer of the input file
@param[in]  max_sz     Maximum output buffer size
@param[out] out_buf    Output buffer
@param[out] out_len    Output buffer size
@return     Zero on success; otherwise H2BIN_ERR_XXX
@post       Caller MUST free the out_buf if return value is zero
*/

int util_str_2_num_type(char *num_str, uint8_t max_prec, zwnum_type_t *out_num);
/**<
Convert string to number type that supports floating point
@param[in]  num_str    Number string that should contain only '0' to '9', '-' or '.'
@param[in]  max_prec   Maximum precision (decimal places)
@param[out] out_num    Number type
@return     Zero on success; otherwise negative number
*/

int util_bitmsk_str_decode(uint8_t *bitmsk_buf, uint8_t bitmsk_buf_sz, uint8_t *out_buf, uint8_t *out_buf_sz, int first_bit_0_eq_0);
/**<
Decode a bitmask string into number string
@param[in]  bitmsk_buf      Bitmask buffer
@param[in]  bitmsk_buf_sz   Number of bitmask bytes in bitmsk_buf
@param[out] out_buf         Output number buffer
@param[in,out] out_buf_sz   Input: the length of out_buf in bytes. Output: the decoded numbers stored in out_buf in bytes
@param[in]  first_bit_0_eq_0      Flag to indicate whether the first byte bit-0 should be decoded as 0; else it is decoded as 1.
@return     Non-zero on success; otherwise zero
*/

int util_bitmsk_str_encode(uint8_t *num_buf, uint8_t num_buf_sz, uint8_t *bitmsk_buf, uint8_t *bitmsk_buf_sz, int first_bit_0_eq_0);
/**<
Encode a number string into a bitmask string
@param[in]      num_buf         Number string buffer
@param[in]      num_buf_sz      Number of bytes in num_buf
@param[out      bitmsk_buf      Bitmask buffer
@param[in,out]  bitmsk_buf_sz   Input: the length of bitmsk_buf in bytes. Output: the encoded bitmask stored in bitmsk_buf in bytes
@param[in]  first_bit_0_eq_0    Flag to indicate whether the first byte bit-0 should be decoded as 0; else it is decoded as 1.
@return     zero on success; otherwise negative error code
*/

uint8_t *util_tlv_get(uint8_t *buf, uint16_t len, uint8_t type, uint8_t *tlv_len);
/**<
Get a specific TLV (type-length-value)in a series of TLVs
@param[in] buf      Buffer that stores the TLVs
@param[in] len      Number of bytes in buf
@param[in] type     Type to get
@param[out] tlv_len The length of the TLV value field
@return             Pointer to the TLV value field; NULL if the TLV is not found or error
*/


/**
Get the MSB of uint16_t
*/
#define HI_OF_16BIT(a)     (uint8_t)((a)>>8)

/**
Get the LSB of uint16_t
*/
#define LO_OF_16BIT(a)     (uint8_t)((a) & 0xFF)

/**
Convert of a two uint8_t bytes pointed by p into uint16_t
*/
#define U8_TO_16BIT(p)     ((((uint16_t)(*(p)))<< 8) | (*(p+1)))

/**
Convert of a four uint8_t bytes pointed by p into uint32_t
*/
#define U8_TO_32BIT(p)     ((((uint32_t)(*(p)))<< 24) | (((uint32_t)(*(p+1)))<< 16) | (((uint32_t)(*(p+2)))<< 8) | (*(p+3)))


/**
@}
*/



#endif /* _ZIP_UTIL_DAVID_ */

