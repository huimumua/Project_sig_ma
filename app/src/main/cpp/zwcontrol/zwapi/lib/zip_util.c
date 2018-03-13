/**
@file   zip_util.c - Z/IP host controller interface utility/miscellaneous functions.

        To implement some utility/miscellaneous functions.

@author David Chow

@version    1.0 5-7-10  Initial release

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

#define _GNU_SOURCE         //For IPv6

#include <stdlib.h>
#ifndef OS_MAC_X
#include <malloc.h>
#endif
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <ctype.h>
#if defined(_WINDOWS) || defined(WIN32)
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <iphlpapi.h>
#else
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#ifndef OS_ANDROID
#include <ifaddrs.h>
#endif

#include <unistd.h>
#endif

#include "../include/zip_error.h"
#include "../include/zip_platform.h"
#include "../include/zip_util.h"
#include "../include/zip_frame.h"
#include "../include/zwave/ZW_classcmd.h"

#ifdef OS_ANDROID
#include "../include/zip_netutil.h"
extern int getifaddrs(struct ifaddrs **ifap);
extern void freeifaddrs(struct ifaddrs *ifa);
#endif

//#define SHOW_NETMASK

/**
@defgroup Util Utility/Miscellaneous Functions
Some utility/miscellaneous functions.
@{
*/

#if defined(_WINDOWS) || defined(WIN32)
#define TPT_SOCKET_ERR      SOCKET_ERROR     ///< Socket error
#else
#define TPT_SOCKET_ERR      -1               ///< Socket error
#endif

#define MAX_DISCOVERY_GW    100         ///< Maximum number of gateways to discover

/**
util_list_add - add an entry into the end of the list.
@param[in]      mtx_ctx     Mutex context; optional, if NULL no locking will be used.
@param[in,out]	head		Pointer to the list head
@param[in]      buf         Buffer that store the data
@param[in]      dat_sz      Size of data to be stored
@return                     Return 0 on success, negative error number on failure.
*/
int util_list_add(struct plt_mtx_t *mtx_ctx, util_lst_t **head, uint8_t  *buf, uint16_t dat_sz)
{
    util_lst_t   *ent;     //Pointer to list entry
    util_lst_t   *temp;

    ent = (util_lst_t   *)malloc(sizeof(util_lst_t) + dat_sz);

    if (!ent)
        return ZWHCI_ERROR_MEMORY;

    ent->dat_sz = dat_sz;
    memcpy(ent->wr_buf, buf, dat_sz);

    if (mtx_ctx)
    {
        plt_mtx_lck(mtx_ctx);
    }

    if (*head == NULL)
    {
        ent->next = NULL;
        *head = ent;
        if (mtx_ctx)
        {
            plt_mtx_ulck(mtx_ctx);
        }
        return ZWHCI_NO_ERROR;
    }

    temp = *head;
    while (temp->next)
    {
        temp = temp->next;
    }

    temp->next = ent;
    ent->next = NULL;

    if (mtx_ctx)
    {
        plt_mtx_ulck(mtx_ctx);
    }
    return ZWHCI_NO_ERROR;
}


/**
util_list_add_no_dup - add an entry into the end of the list if the content of the buffer is
                       different from the last entry.
@param[in]      mtx_ctx     Mutex context; optional, if NULL no locking will be used.
@param[in,out]	head		Pointer to the list head
@param[in]      buf         Buffer that store the data
@param[in]      dat_sz      Size of data to be stored
@param[in]      cmp_fn      The comparison function to identify the entry is identical with the last entry
@return                     Return 0 on success, 1 if the contents are same; else negative error number on failure.
*/
int util_list_add_no_dup(struct plt_mtx_t *mtx_ctx, util_lst_t **head, uint8_t  *buf, uint8_t dat_sz, util_list_cmp_fn cmp_fn)
{
    int          ret;
    util_lst_t   *ent;     //Pointer to list entry
    util_lst_t   *temp;

    ent = (util_lst_t   *)malloc(sizeof(util_lst_t) + dat_sz - 1);

    if (!ent)
        return ZWHCI_ERROR_MEMORY;

    ent->dat_sz = dat_sz;
    memcpy(ent->wr_buf, buf, dat_sz);

    if (mtx_ctx)
    {
        plt_mtx_lck(mtx_ctx);
    }

    if (*head == NULL)
    {
        ent->next = NULL;
        *head = ent;
        if (mtx_ctx)
        {
            plt_mtx_ulck(mtx_ctx);
        }
        return ZWHCI_NO_ERROR;
    }

    temp = *head;
    while (temp->next)
    {
        temp = temp->next;
    }

    //Check if last entry is identical with the new entry
    if (cmp_fn(temp->wr_buf, buf) == 0)
    {
        free(ent);
        ret = 1;
    }
    else
    {
        temp->next = ent;
        ent->next = NULL;
        ret = 0;
    }

    if (mtx_ctx)
    {
        plt_mtx_ulck(mtx_ctx);
    }
    return ret;
}


/**
util_list_get - get the entry from the beginning of the list.
@param[in]      mtx_ctx     Mutex context; optional, if NULL no locking will be used.
@param[in, out]	head		List head
@return     The first entry in the list if the list is not empty; otherwise, NULL.
@post       The caller should free the returned entry.
*/
util_lst_t *util_list_get(struct plt_mtx_t *mtx_ctx, util_lst_t **head)
{
    util_lst_t   *first_entry;  //The entry at the beginning of the list

    if (mtx_ctx)
    {
        plt_mtx_lck(mtx_ctx);
    }
    first_entry = *head;

    if (*head == NULL)
    {
        if (mtx_ctx)
        {
            plt_mtx_ulck(mtx_ctx);
        }
        return NULL;
    }

    *head = (*head)->next;
    if (mtx_ctx)
    {
        plt_mtx_ulck(mtx_ctx);
    }
    return first_entry;
}


/**
util_list_flush - flush the list.
@param[in]      mtx_ctx     Mutex context; optional, if NULL no locking will be used.
@param[in, out]	head		List head
@return
*/
void util_list_flush(struct plt_mtx_t *mtx_ctx, util_lst_t **head)
{
    util_lst_t   *first_entry;  //The entry at the beginning of the list
    util_lst_t   *del_entry;    //Entry to be deleted

    if (mtx_ctx)
    {
        plt_mtx_lck(mtx_ctx);
    }

    first_entry = *head;

    while (first_entry)
    {
        del_entry = first_entry;
        first_entry = first_entry->next;
        free(del_entry);
    }

    *head = NULL;
    if (mtx_ctx)
    {
        plt_mtx_ulck(mtx_ctx);
    }
}


/**
util_list_find - find an entry from the list without modifying the list.
@param[in]      mtx_ctx     Mutex context; optional, if NULL no locking will be used.
@param[in]	    head		Pointer to the list head
@param[in]      cmp_fn      The comparison function to identify the entry using the key stored in buf
@param[in,out]  buf         Buffer that stores the key to be searched. On success, this buffer will be
                            returned with found entry.
@return                     Return non-zero on success, zero on failure.
*/
int util_list_find(struct plt_mtx_t *mtx_ctx, util_lst_t *head, uint8_t  *buf, util_list_cmp_fn cmp_fn)
{
    util_lst_t   *temp;

    if (!head)
        return 0;

    if (mtx_ctx)
    {
        plt_mtx_lck(mtx_ctx);
    }

    temp = head;

    while (temp)
    {
        if (cmp_fn(temp->wr_buf, buf) == 0)
        {
            memcpy(buf, temp->wr_buf, temp->dat_sz);
            if (mtx_ctx)
            {
                plt_mtx_ulck(mtx_ctx);
            }
            return 1;
        }
        temp = temp->next;
    }

    if (mtx_ctx)
    {
        plt_mtx_ulck(mtx_ctx);
    }
    return 0;
}


/**
util_list_rm - remove the specified entry from the list.
@param[in]      mtx_ctx     Mutex context; optional, if NULL no locking will be used.
@param[in]	    head		List head
@param[in]      cmp_fn      The comparison function to identify the entry using the key stored in buf
@param[in]      buf         Buffer that stores the key to be searched.
@return                     Return non-zero on success, zero on failure.
*/
int util_list_rm(struct plt_mtx_t *mtx_ctx, util_lst_t **head, uint8_t  *buf, util_list_cmp_fn cmp_fn)
{
    util_lst_t   *cur_ent;
    util_lst_t   *prev_ent;

    if (*head == NULL)
        return 0;

    if (mtx_ctx)
    {
        plt_mtx_lck(mtx_ctx);
    }

    cur_ent = *head;

    if (cmp_fn(cur_ent->wr_buf, buf) == 0)
    {   //List head to be removed
        *head = cur_ent->next;
        free(cur_ent);
        if (mtx_ctx)
        {
            plt_mtx_ulck(mtx_ctx);
        }
        return 1;
    }

    prev_ent = cur_ent;
    cur_ent = cur_ent->next;

    while (cur_ent)
    {
        if (cmp_fn(cur_ent->wr_buf, buf) == 0)
        {
            prev_ent->next = cur_ent->next;
            free(cur_ent);
            if (mtx_ctx)
            {
                plt_mtx_ulck(mtx_ctx);
            }
            return 1;
        }
        prev_ent = cur_ent;
        cur_ent = cur_ent->next;
    }

    if (mtx_ctx)
    {
        plt_mtx_ulck(mtx_ctx);
    }

    return 0;
}


/**
util_list_rplc - replace the specified entry from the list if found; else add the entry to the end of the list.
@param[in]      mtx_ctx     Mutex context; optional, if NULL no locking will be used.
@param[in]	    head		List head
@param[in]      buf         Buffer that stores the entry to be added.
@param[in]      dat_sz      Size of data to be stored
@param[in]      cmp_fn      The comparison function to identify the entry using the key stored in buf
@return     1=entry has been replaced, 0=entry has been added; negative value on error.
*/
int util_list_rplc(struct plt_mtx_t *mtx_ctx, util_lst_t **head, uint8_t  *buf, uint16_t dat_sz, util_list_cmp_fn cmp_fn)
{
    util_lst_t   *cur_ent;
    util_lst_t   *prev_ent = NULL;

    if (mtx_ctx)
    {
        plt_mtx_lck(mtx_ctx);
    }

    if (*head == NULL)
    {
        cur_ent = (util_lst_t *)malloc(sizeof(util_lst_t) + dat_sz - 1);

        if (!cur_ent)
        {
            if (mtx_ctx)
            {
                plt_mtx_ulck(mtx_ctx);
            }
            return ZWHCI_ERROR_MEMORY;
        }

        cur_ent->dat_sz = dat_sz;
        memcpy(cur_ent->wr_buf, buf, dat_sz);

        cur_ent->next = NULL;
        *head = cur_ent;
        if (mtx_ctx)
        {
            plt_mtx_ulck(mtx_ctx);
        }
        return 0;
    }

    cur_ent = *head;

    while (cur_ent)
    {
        if (cmp_fn(cur_ent->wr_buf, buf) == 0)
        {   //Found and replace
            cur_ent->dat_sz = dat_sz;
            memcpy(cur_ent->wr_buf, buf, dat_sz);
            if (mtx_ctx)
            {
                plt_mtx_ulck(mtx_ctx);
            }
            return 1;
        }
        prev_ent = cur_ent;
        cur_ent = cur_ent->next;
    }

    //Add new entry at the end of list
    cur_ent = (util_lst_t *)malloc(sizeof(util_lst_t) + dat_sz - 1);

    if (!cur_ent)
    {
        if (mtx_ctx)
        {
            plt_mtx_ulck(mtx_ctx);
        }
        return ZWHCI_ERROR_MEMORY;
    }

    cur_ent->dat_sz = dat_sz;
    memcpy(cur_ent->wr_buf, buf, dat_sz);

    prev_ent->next = cur_ent;
    cur_ent->next = NULL;
    if (mtx_ctx)
    {
        plt_mtx_ulck(mtx_ctx);
    }
    return 0;
}


/**
util_list_updt - find an entry from the list and update its content.
@param[in]      mtx_ctx     Mutex context; optional, if NULL no locking will be used.
@param[in]	    head		Pointer to the list head
@param[in]      cmp_fn      The comparison function to identify the entry using the key stored in buf
@param[in]      buf         Buffer that stores the key to be searched and the entry's new content
@return                     Return non-zero on success, zero on failure.
*/
int util_list_updt(struct plt_mtx_t *mtx_ctx, util_lst_t *head, uint8_t  *buf, util_list_cmp_fn cmp_fn)
{
    util_lst_t   *temp;

    if (!head)
        return 0;

    if (mtx_ctx)
    {
        plt_mtx_lck(mtx_ctx);
    }

    temp = head;

    while (temp)
    {
        if (cmp_fn(temp->wr_buf, buf) == 0)
        {
            memcpy(temp->wr_buf, buf, temp->dat_sz);
            if (mtx_ctx)
            {
                plt_mtx_ulck(mtx_ctx);
            }
            return 1;
        }
        temp = temp->next;
    }

    if (mtx_ctx)
    {
        plt_mtx_ulck(mtx_ctx);
    }
    return 0;
}


/**
util_num_string_add - append an unsigned number to a string
@param[in,out]	src		    The string where an unsigned number is to be appended
@param[in]	    src_size	The size of the src buffer
@param[in]	    num		    The unsigned number that is to be converted to number string
@return
*/
void    util_num_string_add(char *src, unsigned src_size, unsigned num)
{
    char    num_str[20];
#ifdef USE_SAFE_VERSION
    sprintf_s(num_str, 20, "%u, ", num);
    strcat_s(src, src_size, num_str);
#else
    sprintf(num_str, "%u, ", num);
    strcat(src, num_str);
#endif
}


/**
util_hex_string_add - append an hex number to a string
@param[in,out]	src		    The string where an hex number is to be appended
@param[in]	    src_size	The size of the src buffer
@param[in]	    num		    The unsigned number that is to be converted to hex number string
@return
*/
void    util_hex_string_add(char *src, unsigned src_size, unsigned num)
{
    char    num_str[20];
#ifdef USE_SAFE_VERSION
    sprintf_s(num_str, 20, "%02X, ", num);
    strcat_s(src, src_size, num_str);
#else
    sprintf(num_str, "%02X, ", num);
    strcat(src, num_str);
#endif
}


/**
util_ntohs - conversion from big-endian to host byte order
@param[in,out]	src		    The string for conversion from big-endian to host byte order
@param[in]	    elem_cnt	The number of elements (2-byte words) in the src
@return
*/
void    util_ntohs(uint16_t *src, unsigned elem_cnt)
{
    if (!util_is_host_big_endian())
    {
        unsigned i;
        for (i=0; i<elem_cnt; i++)
        {
            *src = (*src << 8) | (*src >> 8);
            src++;
        }
    }
}


/**
util_tmr_exp_chk - Check whether the timer has expired
@param[in] now     Current time
@param[in] timer   The time of the timer to check
@return 1=timer has expired; otherwise 0
*/
int    util_tmr_exp_chk(uint16_t now, uint16_t timer)
{
    uint16_t    time_diff;
    if (now == timer)
    {
        return 1;
    }

    //Handle wrap over case
    if (now > timer)
    {
        time_diff = now - timer;
        if (time_diff < 0xF000)
        {
            return 1;
        }
    }
    else //now < timer
    {
        time_diff = timer - now;
        if (time_diff >= 0xF000)
        {
            return 1;
        }
    }
    //Not expired
    return 0;
}


/**
util_time_diff - Calculate time difference; taking wrap-over case into consideration
@param[in] now      Current time
@param[in] old_time Previous time
@return time difference
*/
uint16_t    util_time_diff(uint16_t now, uint16_t old_time)
{
    uint16_t    time_diff;
    if (now == old_time)
    {
        return 0;
    }


    if (now > old_time)
    {
        time_diff = now - old_time;
    }
    else //now < old_time
    {   //Handle wrap over case

        time_diff = (0xFFFF - old_time) + now + 1;
    }

    return time_diff;
}


/**
util_tmr_exp_chk32 - Check whether the 32-bit timer has expired
@param[in] now     Current time
@param[in] timer   The time of the timer to check
@return 1=timer has expired; otherwise 0
*/
int util_tmr_exp_chk32(uint32_t now, uint32_t timer)
{
    uint32_t    time_diff;
    if (now == timer)
    {
        return 1;
    }

    //Handle wrap over case
    if (now > timer)
    {
        time_diff = now - timer;
        if (time_diff < 0xF0000000)
        {
            return 1;
        }
    }
    else //now < timer
    {
        time_diff = timer - now;
        if (time_diff >= 0xF0000000)
        {
            return 1;
        }
    }
    //Not expired
    return 0;
}


/**
util_time_diff32 - Calculate 32-bit time difference (i.e. tm1 - tm2) in terms of timer ticks
@param[in] tm1     Time 1
@param[in] tm2     Time 2
@return time different in terms of timer tick
*/
uint32_t   util_time_diff32(uint32_t tm1, uint32_t tm2)
{
    uint32_t    time_diff;
    uint32_t    temp;

    if (tm1 == tm2)
    {
        return 0;
    }

    if (tm1 < tm2)
    {   //swap
        temp = tm1;
        tm1 = tm2;
        tm2 = temp;
    }

    time_diff = tm1 - tm2;
    if (time_diff >= 0xF0000000)
    {   //wrap over
        time_diff = (0xFFFFFFFF - tm1) + 1 + tm2;
    }
    return time_diff;

}


/**
util_is_host_big_endian - Get host endianess
@return 1 if host is big endian; otherwise 0
*/
int util_is_host_big_endian(void)
{
    union
    {
        long l; char c[sizeof (long)];
    } u;
    u.l = 1;
    return(u.c[sizeof (long) - 1] == 1);
}


#define CMD_CLS_PARSE_MODE_INSECURE     0   ///< Command class parser is in insecure mode
#define CMD_CLS_PARSE_MODE_CONTROL      1   ///< Command class parser is in control mode
#define CMD_CLS_PARSE_MODE_SECURE       2   ///< Command class parser is in secure mode
/**
util_cmd_cls_parse - Parse buffer that stores mixture of 1-byte and 2-byte command classes
@param[in] cmd_buf      Command buffer
@param[in] cmd_buf_len  Size of command buffer
@param[out] cmd_cls     Output insecure command classes without any marker
@param[out] cmd_cnt     Output insecure command classes count
@param[out] cmd_cls_sec Output secure command classes without any marker
@param[out] cmd_cnt_sec Output secure command classes count
@return     Non-zero if no error; otherwise return zero
@post       If return value is non-zero, caller needs to free the cmd_cls and cmd_cls_sec buffers.
*/
int util_cmd_cls_parse(uint8_t *cmd_buf, uint8_t cmd_buf_len, uint16_t **cmd_cls, uint8_t *cmd_cnt,
                       uint16_t **cmd_cls_sec, uint8_t *cmd_cnt_sec)
{
    unsigned    i;
    int         mode; // 0=insecure supported command classes; 1=insecure controlled command classes;
                      // 2=secure supported command classes
    uint16_t    *cls_buf;
    uint16_t    *cls_sec_buf;
    uint8_t     cnt;
    uint8_t     cnt_sec;

    if ((cmd_buf_len == 0) || !cmd_buf)
    {
        return 0;
    }

    //Allocate memory for the buffers
    cls_buf = (uint16_t *)malloc(cmd_buf_len * sizeof(uint16_t));

    if (!cls_buf)
    {
        return 0;
    }

    cls_sec_buf = (uint16_t *)malloc(cmd_buf_len * sizeof(uint16_t));

    if (!cls_sec_buf)
    {
        free(cls_buf);
        return 0;
    }

    //Initialize the parser
    cnt_sec = cnt = i = 0;
    mode = CMD_CLS_PARSE_MODE_INSECURE;

    while (i < cmd_buf_len)
    {
        if (*cmd_buf >= 0xF1)
        {   //Check whether this is the security scheme 0 marker
            if (*(cmd_buf + 1) == 0)
            {   //Change to secure mode
                mode = CMD_CLS_PARSE_MODE_SECURE;

                cmd_buf += 2;
                i += 2;
                continue;
            }

            //Extended command class
            if (mode == CMD_CLS_PARSE_MODE_INSECURE)
            {
                cls_buf[cnt] = *cmd_buf++;
                cls_buf[cnt] = (cls_buf[cnt] << 8) | (*cmd_buf++);
                cnt++;
            }
            else if (mode == CMD_CLS_PARSE_MODE_SECURE)
            {
                cls_sec_buf[cnt_sec] = *cmd_buf++;
                cls_sec_buf[cnt_sec] = (cls_sec_buf[cnt_sec] << 8) | (*cmd_buf++);
                cnt_sec++;
            }
            else
            {
                cmd_buf += 2;
            }

            i += 2;
        }
        else
        {
            //Check whether this is the controlled command class marker
            if (*cmd_buf == 0xEF)
            {   //Change mode
                mode = CMD_CLS_PARSE_MODE_CONTROL;

                cmd_buf++;
                i++;
                continue;
            }


            //Normal command class
            if (mode == CMD_CLS_PARSE_MODE_INSECURE)
            {
                cls_buf[cnt++] = *cmd_buf++;
            }
            else if (mode == CMD_CLS_PARSE_MODE_SECURE)
            {
                cls_sec_buf[cnt_sec++] = *cmd_buf++;
            }
            else
            {
                cmd_buf++;
            }
            i++;
        }
    }
    //Parsing done
    *cmd_cnt = cnt;
    *cmd_cnt_sec = cnt_sec;
    *cmd_cls = cls_buf;
    *cmd_cls_sec = cls_sec_buf;
    return 1;
}


#ifdef SHOW_NETMASK
/**
util_netmask_get - Get the netmask from prefix length
@param[in] prefix_len   Prefix length
@return     Netmask
*/
static uint32_t util_netmask_get(uint8_t prefix_len)
{
    uint32_t    mask;
    int         i;
    int         num_of_1s;

    if (prefix_len < 32)
    {
        mask = 0;
        num_of_1s = prefix_len;
        for (i=0; i<num_of_1s; i++) {
            mask = (mask >> 1) | 0x80000000;
        }

    }
    else
    {
        mask = 0xFFFFFFFF;
    }

    return mask;
}
#endif


/**
util_ip_addr_get - Get addresses of all the IPv4 / IPv6 network in the system
@param[in]  use_ipv4     Flag to indicate whether to get IPv4 addresses. 1= IPv4; 0= IPv6
@param[in]  num_addr     The number of addresses the ip_addr_buf buffer can store
@param[out] ip_addr_buf  Output buffer to store the IPv4 / IPv6 addresses.
                         For IPv4, it should be a buffer of 4-byte for each entry.
                         For IPv6, it should be an array of type util_ipv6_addr_t
@return     The number of IPv4 / IPv6 addresses actually found.
*/
#if defined(_WINDOWS) || defined(WIN32)
int util_ip_addr_get(int num_addr, void *ip_addr_buf, int use_ipv4)
{
    static const uint8_t ipv6_zero_addr[IPV6_ADDR_LEN] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ;
    static const uint8_t ipv6_loopback_addr[IPV6_ADDR_LEN] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1} ;
    //static const uint8_t ipv6_lnk_lc_v4_eb_addr[12] = { 0xfe, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0x5e, 0xfe};//IPv6 link local ipv4 embedded address; e.g. fe80::5efe:169.254.127.114
    static const uint8_t ipv6_lnk_lc_addr[8] = { 0xfe, 0x80, 0, 0, 0, 0, 0, 0};//IPv6 link local address
    static const uint8_t ipv4_loopback_addr[IPV4_ADDR_LEN] = { 127, 0, 0, 1 };
    static const uint8_t ipv4_lnk_lc_addr[2] = {169, 254};//IPv4 link-local address 169.254.0.0/16

    uint8_t             *ipv4_addr_buf = (uint8_t *)ip_addr_buf;
    util_ipv6_addr_t    *ipv6_addr_buf = (util_ipv6_addr_t *)ip_addr_buf;
    DWORD               dwRetVal = 0;
    ULONG               outBufLen;
    int                 try_cnt = 0;
    int                 i;
    int                 result;
    int                 ip_cnt = 0;
#ifdef SHOW_NETMASK
    uint32_t            netmask;
    struct sockaddr_in  sock_netmask;
#endif

    // Set the flags to pass to GetAdaptersAddresses
    ULONG               flags = GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER
                                | GAA_FLAG_SKIP_FRIENDLY_NAME;

    PIP_ADAPTER_ADDRESSES       pAddresses = NULL;
    PIP_ADAPTER_ADDRESSES       pCurrAddresses;
    PIP_ADAPTER_UNICAST_ADDRESS pUnicast;

    // Allocate a 8 KB buffer to start with
    outBufLen = 8192;

    do
    {
        pAddresses = (IP_ADAPTER_ADDRESSES *) malloc(outBufLen);
        if (pAddresses == NULL)
        {
            debug_gw_discvr("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");

            return 0;
        }

        dwRetVal = GetAdaptersAddresses((use_ipv4)? AF_INET : AF_INET6, flags, NULL, pAddresses, &outBufLen);

        if (dwRetVal == ERROR_BUFFER_OVERFLOW)
        {   //Try again with bigger buffer
            //debug_gw_discvr("Buf overflow, try using buffer size:%lu\n", outBufLen);
            free(pAddresses);
            pAddresses = NULL;
        }
        else
        {
            break;
        }

        try_cnt++;

    } while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (try_cnt < 3));

    if (dwRetVal == NO_ERROR)
    {
        struct sockaddr_in  *sa4;
        struct sockaddr_in6 *sa6;
        char* FAR addr_str;
        char  addr_string[80];
        DWORD addr_str_len;

        pCurrAddresses = pAddresses;

        while (pCurrAddresses)
        {
            pUnicast = pCurrAddresses->FirstUnicastAddress;
            if (pUnicast)
            {
                for (i = 0; pUnicast != NULL; i++)
                {
                    if (pUnicast->Address.lpSockaddr->sa_family == AF_INET)
                    {
                        sa4 = (struct sockaddr_in  *)pUnicast->Address.lpSockaddr;

                        if ((memcmp(&sa4->sin_addr.s_addr, ipv4_loopback_addr, IPV4_ADDR_LEN) != 0)
                            && (memcmp(&sa4->sin_addr.s_addr, ipv4_lnk_lc_addr, 2) != 0)
                            && sa4->sin_addr.s_addr)
                        {
                            addr_str = inet_ntoa(sa4->sin_addr);
                            debug_gw_discvr("\tIPv4 Address: %s\n", addr_str);

#ifdef SHOW_NETMASK
                            netmask = util_netmask_get(pUnicast->OnLinkPrefixLength);
                            netmask = htonl(netmask);
                            memcpy(&sock_netmask.sin_addr.s_addr, &netmask, IPV4_ADDR_LEN);
                            sock_netmask.sin_family = AF_INET;
                            addr_str = inet_ntoa(sock_netmask.sin_addr);
                            debug_gw_discvr("\tIPv4 Address netmask: %s\n", addr_str);
#endif
                            if (ip_cnt < num_addr)
                            {
                                memcpy(ipv4_addr_buf + (ip_cnt*IPV4_ADDR_LEN), &sa4->sin_addr.s_addr, IPV4_ADDR_LEN);
                                ip_cnt++;
                            }
                        }
                    }
                    else if (pUnicast->Address.lpSockaddr->sa_family == AF_INET6)
                    {
                        sa6 = (struct sockaddr_in6 *)pUnicast->Address.lpSockaddr;

                        if ((memcmp(sa6->sin6_addr.s6_addr, ipv6_loopback_addr, IPV6_ADDR_LEN) != 0)
                            && (memcmp(sa6->sin6_addr.s6_addr, ipv6_zero_addr, IPV6_ADDR_LEN) != 0)
                            && (memcmp(sa6->sin6_addr.s6_addr, ipv6_lnk_lc_addr, 8) != 0)
                            /*&& (memcmp(sa6->sin6_addr.s6_addr, ipv6_lnk_lc_v4_eb_addr, 12) != 0)*/)
                        {
                            addr_str_len = 80;
                            result = WSAAddressToStringA(pUnicast->Address.lpSockaddr, pUnicast->Address.iSockaddrLength,
                                                         NULL, addr_string, &addr_str_len);
                            if (result == 0)
                            {
                                debug_gw_discvr("\tIPv6 Address: %s\n", addr_string);
                            }
#ifdef SHOW_NETMASK
                            debug_gw_discvr("\tIPv6 Address prefix length: %u\n", pUnicast->OnLinkPrefixLength);
#endif

                            if ((ip_cnt < num_addr)
                                && (pUnicast->OnLinkPrefixLength < 128))//Skip Temporary IPv6 Address
                            {
                                memcpy(ipv6_addr_buf[ip_cnt].addr, sa6->sin6_addr.s6_addr, IPV6_ADDR_LEN);
                                ipv6_addr_buf[ip_cnt].if_idx = pCurrAddresses->Ipv6IfIndex;
                                ip_cnt++;
                            }
                        }
                    }
                    pUnicast = pUnicast->Next;
                }
            }
            pCurrAddresses = pCurrAddresses->Next;
        }
    }
    else
    {
        debug_gw_discvr("Call to GetAdaptersAddresses failed with error: %d\n", dwRetVal);
    }

    if (pAddresses)
    {
        free(pAddresses);
    }

    return ip_cnt;
}
#else
int util_ip_addr_get(int num_addr, void *ip_addr_buf, int use_ipv4)
{
    static const uint8_t ipv6_zero_addr[IPV6_ADDR_LEN] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ;
    static const uint8_t ipv6_loopback_addr[IPV6_ADDR_LEN] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1} ;
    //static const uint8_t ipv6_lnk_lc_v4_eb_addr[12] = { 0xfe, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0x5e, 0xfe};//IPv6 link local ipv4 embedded address; e.g. fe80::5efe:169.254.127.114
    static const uint8_t ipv6_lnk_lc_addr[8] = { 0xfe, 0x80, 0, 0, 0, 0, 0, 0};//IPv6 link local address
    static const uint8_t ipv4_loopback_addr[IPV4_ADDR_LEN] = { 127, 0, 0, 1 };
    static const uint8_t ipv4_lnk_lc_addr[2] = {169, 254};//IPv4 link-local address 169.254.0.0/16

    uint8_t             *ipv4_addr_buf = (uint8_t *)ip_addr_buf;
    util_ipv6_addr_t    *ipv6_addr_buf = (util_ipv6_addr_t *)ip_addr_buf;
    struct sockaddr_in  *sa4;
    struct sockaddr_in6 *sa6;
    struct ifaddrs      *ifaddr;
    struct ifaddrs      *ifa;
    int                 family;
    int                 result;
    int                 ip_cnt = 0;
    char                host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1)
    {
        return 0;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;

        if (use_ipv4)
        {
            if (family == AF_INET)
            {
                //Store the ip address
                sa4 = (struct sockaddr_in *)ifa->ifa_addr;

                if ((memcmp(&sa4->sin_addr.s_addr, ipv4_loopback_addr, IPV4_ADDR_LEN) != 0)
                    && (memcmp(&sa4->sin_addr.s_addr, ipv4_lnk_lc_addr, 2) != 0)
                    && sa4->sin_addr.s_addr)
                {
                    result = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                                         host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
                    if (result == 0)
                    {
                        debug_gw_discvr("\tIPv4 Address: %s\n", host);
                    }
#ifdef SHOW_NETMASK
                    result = getnameinfo(ifa->ifa_netmask, sizeof(struct sockaddr_in),
                                         host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
                    if (result == 0)
                    {
                        debug_gw_discvr("\tIPv4 Address netmask: %s\n", host);
                    }
#endif

                    if (ip_cnt < num_addr)
                    {
                        memcpy(ipv4_addr_buf + (ip_cnt*IPV4_ADDR_LEN), &sa4->sin_addr.s_addr, IPV4_ADDR_LEN);
                        ip_cnt++;
                    }
                }
            }
        }
        else
        {
            if (family == AF_INET6)
            {
                sa6 = (struct sockaddr_in6 *)ifa->ifa_addr;

                if ((memcmp(sa6->sin6_addr.s6_addr, ipv6_loopback_addr, IPV6_ADDR_LEN) != 0)
                    && (memcmp(sa6->sin6_addr.s6_addr, ipv6_zero_addr, IPV6_ADDR_LEN) != 0)
                    && (memcmp(sa6->sin6_addr.s6_addr, ipv6_lnk_lc_addr, 8) != 0)
                    /*&& (memcmp(sa6->sin6_addr.s6_addr, ipv6_lnk_lc_v4_eb_addr, 12) != 0)*/)
                {
                    result = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in6),
                                         host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
                    if (result == 0)
                    {
                        debug_gw_discvr("\tIPv6 Address: %s\n", host);
                    }
#ifdef SHOW_NETMASK
                    result = getnameinfo(ifa->ifa_netmask, sizeof(struct sockaddr_in6),
                                         host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
                    if (result == 0)
                    {
                        debug_gw_discvr("\tIPv6 Address netmask: %s\n", host);
                    }
#endif
                    if (ip_cnt < num_addr)
                    {
                        memcpy(ipv6_addr_buf[ip_cnt].addr, sa6->sin6_addr.s6_addr, IPV6_ADDR_LEN);
                        ipv6_addr_buf[ip_cnt].if_idx = if_nametoindex(ifa->ifa_name);
                        ip_cnt++;
                    }
                }
            }
        }
    }

    freeifaddrs(ifaddr);
    return ip_cnt;
}
#endif


/**
util_ip_aton - Convert IPv4 / IPv6 address string to numeric equivalent
@param[in]  addr_str     Null terminated IPv4 / IPv6 address string
@param[out] addr_buf     Buffer that should be at least 16-byte long to store the result
@param[out] ipv4         Flag to indicate the converted neumeric is IPv4 or IPv6. 1=IPv4; 0=IPv6.
@return     Zero if successful; otherwise non-zero
*/
#if defined(_WINDOWS) || defined(WIN32)
int util_ip_aton(char *addr_str, uint8_t *addr_buf, int *ipv4)
{
    struct sockaddr_in  sa4;
    int                 sock_size;
    int                 result;
    WSADATA             wsa_dat;

    // Initialize Winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsa_dat);
    if (result != NO_ERROR)
    {
        debug_gw_discvr("WSAStartup failed with error %d\n", result);
        return -1;
    }

    //Start conversion by assuming IPv4 address string
    sock_size = sizeof(struct sockaddr_in);
    sa4.sin_family = AF_INET;

    if (WSAStringToAddressA(addr_str, AF_INET, NULL, (LPSOCKADDR)&sa4, &sock_size) != 0)
    {
        //Try second conversion by assuming IPv6 address string
        struct sockaddr_in6 sa6;

        sock_size = sizeof(struct sockaddr_in6);
        sa6.sin6_family = AF_INET6;

        if (WSAStringToAddressA(addr_str, AF_INET6, NULL, (LPSOCKADDR)&sa6, &sock_size) != 0)
        {
            //Clean up Winsock
            WSACleanup();
            return -2; //Failed
        }
        memcpy(addr_buf, sa6.sin6_addr.s6_addr, IPV6_ADDR_LEN);
        *ipv4 = 0;
    }
    else
    {   //Address string is IPv4
        memcpy(addr_buf, &sa4.sin_addr.s_addr, IPV4_ADDR_LEN);
        *ipv4 = 1;
    }

    //Clean up Winsock
    WSACleanup();
    return 0;
}
#else
int util_ip_aton(char *addr_str, uint8_t *addr_buf, int *ipv4)
{
    //Start conversion by assuming IPv4 address string
    if (inet_pton(AF_INET, addr_str, addr_buf) <= 0)
    {
        //Try second conversion by assuming IPv6 address string
        if (inet_pton(AF_INET6, addr_str, addr_buf) <= 0)
        {
            return -2; //Failed
        }
        *ipv4 = 0;
    }
    else
    {   //Address string is IPv4
        *ipv4 = 1;
    }
    return 0;
}
#endif


/**
util_ip_ntoa - Convert IPv4 / IPv6 address in numerical form to string equivalent
@param[in]  addr         IPv4 / IPv6 address in numerical form
@param[out] addr_str     buffer to store the converted address string
@param[in]  addr_str_len the size of the addr_str in bytes
@param[in]  ipv4         Flag to indicate the addr parameter is IPv4 or IPv6. 1=IPv4; 0=IPv6.
@return     Zero if successful; otherwise non-zero
*/
#if defined(_WINDOWS) || defined(WIN32)
int util_ip_ntoa(uint8_t *addr, char *addr_str, int addr_str_len, int ipv4)
{
    struct sockaddr     *sock_addr;
    struct sockaddr_in  sa4;
    struct sockaddr_in6 sa6;
    DWORD               sock_size;
    DWORD               addr_str_sz;
    int                 result;
    WSADATA             wsa_dat;

    // Initialize Winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsa_dat);
    if (result != NO_ERROR)
    {
        debug_gw_discvr("WSAStartup failed with error %d\n", result);
        return -1;
    }

    if (ipv4)
    {
        sock_size = sizeof(struct sockaddr_in);
        sock_addr = (struct sockaddr *)&sa4;
        memset(&sa4, 0, sock_size);
        sa4.sin_family = AF_INET;
        memcpy(&sa4.sin_addr.s_addr, addr, IPV4_ADDR_LEN);
    }
    else
    {
        sock_size = sizeof(struct sockaddr_in6);
        sock_addr = (struct sockaddr *)&sa6;
        memset(&sa6, 0, sock_size);
        sa6.sin6_family = AF_INET6;
        memcpy(sa6.sin6_addr.s6_addr, addr, IPV6_ADDR_LEN);

    }

    addr_str_sz = addr_str_len;

    result = WSAAddressToStringA(sock_addr, sock_size,
                                 NULL, addr_str, &addr_str_sz);

    //Clean up Winsock
    WSACleanup();
    return result;

}
#else
int util_ip_ntoa(uint8_t *addr, char *addr_str, int addr_str_len, int ipv4)
{
    struct sockaddr     *sock_addr;
    struct sockaddr_in  sa4;
    struct sockaddr_in6 sa6;
    socklen_t           sock_size;
    int                 result;

    if (ipv4)
    {
        sock_size = sizeof(struct sockaddr_in);
        sock_addr = (struct sockaddr *)&sa4;
        memset(&sa4, 0, sock_size);
        sa4.sin_family = AF_INET;
        memcpy(&sa4.sin_addr.s_addr, addr, IPV4_ADDR_LEN);
    }
    else
    {
        sock_size = sizeof(struct sockaddr_in6);
        sock_addr = (struct sockaddr *)&sa6;
        memset(&sa6, 0, sock_size);
        sa6.sin6_family = AF_INET6;
        memcpy(sa6.sin6_addr.s6_addr, addr, IPV6_ADDR_LEN);

    }
    result = getnameinfo(sock_addr, sock_size,
                         addr_str, addr_str_len, NULL, 0, NI_NUMERICHOST);

    return result;
}
#endif


/**
util_ipv4_broadcast - broadcast message to a specific network
@param[in]      host_ip     Host IP from which the network corresponding to the host IP message is broadcasted.
@param[in]      buf         Buffer that store the data to be sent
@param[in]      dat_sz      Size of data to be sent
@param[out]     sfd         Socket handler which can be used to received response to the broadcast message
@return                     Return 0 on success, negative error number on failure.
@post           Caller has to close the socket handler (sfd) if the return value is 0 (success)
*/
#if defined(_WINDOWS) || defined(WIN32)
static int util_ipv4_broadcast(struct sockaddr_in *host_ip, uint8_t *buf, uint16_t dat_sz, SOCKET *sfd)
#else
static int util_ipv4_broadcast(struct sockaddr_in *host_ip, uint8_t *buf, uint16_t dat_sz, int *sfd)
#endif
{
    #if defined(_WINDOWS) || defined(WIN32)
    SOCKET              sock_fd;
    int                 bytes_sent;
    BOOL                sock_opt = TRUE;
    #else
    int                 sock_fd;
    ssize_t             bytes_sent;
    int                 sock_opt = 1;
    #endif
    int res;

    struct sockaddr_in  sa_bdcast;

    //Open socket and bind it to the host ip address
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0)
    {
        return -1;
    }

    // Enable sending of broadcast message
    if (setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, (const char *)&sock_opt, sizeof(sock_opt)) < 0)
    {
#if defined(_WINDOWS) || defined(WIN32)
        closesocket(sock_fd);
#else
        close(sock_fd);
#endif
        return -2;
    }

    //Bind to listening address
    host_ip->sin_family = AF_INET;
    host_ip->sin_port = 0;//use any available port
    debug_gw_discvr("Binding to host IP:%s\n", inet_ntoa(host_ip->sin_addr));

    res = bind(sock_fd, (const struct sockaddr *)host_ip, sizeof(struct sockaddr_in));

    if (res == 0)
    {
        sa_bdcast.sin_family = AF_INET;
        sa_bdcast.sin_port = htons(ZWAVE_HOME_CTL_PORT);
        sa_bdcast.sin_addr.s_addr = 0xFFFFFFFF;

        bytes_sent = sendto(sock_fd, buf, dat_sz,
                            0, (const struct sockaddr *)&sa_bdcast, sizeof(struct sockaddr_in));

        if (bytes_sent == dat_sz)
        {
            *sfd = sock_fd;
            return 0;
        }
    }
    else
    {
        debug_gw_discvr("Bind failed\n");
    }

#if defined(_WINDOWS) || defined(WIN32)
    closesocket(sock_fd);
#else
    close(sock_fd);
#endif
    return -3;
}


/**
util_ipv6_multicast - send multicast message to a specific IPv6 network
@param[in]      host_ip     Host IP from which the network corresponding to the host IP message is sent.
@param[in]      host_if     Host IP interface index
@param[in]      buf         Buffer that store the data to be sent
@param[in]      dat_sz      Size of data to be sent
@param[out]     sfd         Socket handler which can be used to received response to the broadcast message
@return                     Return 0 on success, negative error number on failure.
@post           Caller has to close the socket handler (sfd) if the return value is 0 (success)
*/
#if defined(_WINDOWS) || defined(WIN32)
static int util_ipv6_multicast(struct sockaddr_in6 *host_ip, uint32_t host_if,
                              uint8_t *buf, uint16_t dat_sz, SOCKET *sfd)
#else
static int util_ipv6_multicast(struct sockaddr_in6 *host_ip, uint32_t host_if,
                              uint8_t *buf, uint16_t dat_sz, int *sfd)
#endif
{
    static const uint8_t all_routers_multicast[IPV6_ADDR_LEN] = { 0xFF, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2} ;

    #if defined(_WINDOWS) || defined(WIN32)
    SOCKET              sock_fd;
    int                 bytes_sent;
    DWORD               addr_str_len;
    #else
    int                 sock_fd;
    ssize_t             bytes_sent;
    #endif
    char                addr_string[80];
    int res;

    struct sockaddr_in6  sa_multicast;

    //Open socket and bind it to the host ip address
    sock_fd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sock_fd < 0)
    {
        debug_gw_discvr("Open socket failed\n");
        return -1;
    }

    //Bind to listening address
    host_ip->sin6_family = AF_INET6;
    host_ip->sin6_port = 0;//use any available port
    host_ip->sin6_flowinfo = 0;
    host_ip->sin6_scope_id = 0;

    #if defined(_WINDOWS) || defined(WIN32)
    addr_str_len = 80;
    res = WSAAddressToStringA((struct sockaddr *)host_ip, sizeof(struct sockaddr_in6),
                                 NULL, addr_string, &addr_str_len);
    #else
    res = getnameinfo((const struct sockaddr *)host_ip, sizeof(struct sockaddr_in6),
                         addr_string, 80, NULL, 0, NI_NUMERICHOST);
    #endif
    if (res == 0)
    {
        debug_gw_discvr("Binding to host IP:%s, interface index: %u\n", addr_string, host_if);
    }

    if (setsockopt(sock_fd, IPPROTO_IPV6, IPV6_MULTICAST_IF, (const char *)&host_if, sizeof(uint32_t)) < 0)
    {
#if defined(_WINDOWS) || defined(WIN32)
        closesocket(sock_fd);
#else
        close(sock_fd);
#endif
        return -2;
    }

    res = bind(sock_fd, (const struct sockaddr *)host_ip, sizeof(struct sockaddr_in6));

    if (res == 0)
    {
        sa_multicast.sin6_family = AF_INET6;
        sa_multicast.sin6_port = htons(ZWAVE_HOME_CTL_PORT);
        sa_multicast.sin6_flowinfo = 0;
        sa_multicast.sin6_scope_id = 0;
        memcpy(sa_multicast.sin6_addr.s6_addr, all_routers_multicast, IPV6_ADDR_LEN);
        bytes_sent = sendto(sock_fd, buf, dat_sz,
                            0, (const struct sockaddr *)&sa_multicast, sizeof(struct sockaddr_in6));

        if (bytes_sent == dat_sz)
        {
            *sfd = sock_fd;
            return 0;
        }
        else
        {
            debug_gw_discvr("Send to IPv6 'all routers' address failed\n");
        }
    }
    else
    {
        debug_gw_discvr("Bind failed\n");
    }

#if defined(_WINDOWS) || defined(WIN32)
    closesocket(sock_fd);
#else
    close(sock_fd);
#endif
    return -3;
}


/**
util_gw_discvr_wait_resp - Wait for gateway discovery response
@param[in]	sfd		        Socket handler
@param[in]	nif_seq		    Node information cached sequence number
@param[in]	gw_ip_buf_sz	The number of gateway addresses could be stored in the gw_ip
@param[in,out]	gw_ip	    (Input) The sin_family member indicates which IP family to use. (Output) Gateway IP addresses
@return The number of gateway IP addresses found
*/
#if defined(_WINDOWS) || defined(WIN32)
static int util_gw_discvr_wait_resp(SOCKET sfd, uint8_t nif_seq, uint8_t gw_ip_buf_sz, struct sockaddr *gw_ip)
#else
static int util_gw_discvr_wait_resp(int sfd, uint8_t nif_seq, uint8_t gw_ip_buf_sz, struct sockaddr *gw_ip)
#endif
{
    int                 rcx_size;
    int                 n;
    int                 rv;
    int                 use_ipv4;
    fd_set              readfds;
    struct timeval      tv;
    struct sockaddr     *sock_addr;
    struct sockaddr_in6 *gw_ip6 = NULL;
    struct sockaddr_in  *gw_ip4 = NULL;
    struct sockaddr_in6 sa6;
    struct sockaddr_in  sa4;
    socklen_t           sock_len;
    uint8_t             buf[MAX_RCX_SIZE];
    uint8_t             idx = 0;

    if (gw_ip->sa_family == AF_INET)
    {
        use_ipv4 = 1;
        gw_ip4 = (struct sockaddr_in *)gw_ip;
    }
    else
    {
        use_ipv4 = 0;
        gw_ip6 = (struct sockaddr_in6 *)gw_ip;
    }

    while (idx < gw_ip_buf_sz)
    {
        // clear the set ahead of time
        FD_ZERO(&readfds);

        // add our descriptors to the set
        FD_SET(sfd, &readfds);

        // the n param in select()
        n = sfd + 1;

        // wait until either socket has data ready to be received or timeout
        tv.tv_sec = ZWNET_GW_DISCVR_TMOUT / 1000;
        tv.tv_usec = (ZWNET_GW_DISCVR_TMOUT % 1000) * 1000;

        rv = select(n, &readfds, NULL, NULL, &tv);

        if (rv == TPT_SOCKET_ERR)
        {
            //debug_gw_discvr("select failed with error %d\n", WSAGetLastError());
            debug_gw_discvr("select failed\n");
            break;
        }
        else if (rv == 0)
        {
            //Timeout
            //debug_gw_discvr("select timeout\n");
            break;
        }
        else
        {
            if (FD_ISSET(sfd, &readfds))
            {
                if (use_ipv4)
                {
                    sock_len = sizeof(struct sockaddr_in);
                    sock_addr = (struct sockaddr *)&sa4;
                }
                else
                {
                    sock_len = sizeof(struct sockaddr_in6);
                    sock_addr = (struct sockaddr *)&sa6;
                }

                rcx_size = recvfrom(sfd, buf, MAX_RCX_SIZE, 0,
                                    sock_addr, &sock_len);

                if (rcx_size > 0)
                {
                    //check the response
                    if (rcx_size >= 14)
                    {
                        //Check Z/IP header
                        if ((buf[0] == COMMAND_CLASS_ZIP) && (buf[1] == COMMAND_ZIP_PACKET))
                        {
                            //Check for Z-wave command
                            if (buf[3] & ZIP_FRM_ZWAVE_CMD)
                            {
                                unsigned cmd_offset = 7;

                                if (buf[3] & ZIP_FRM_HDR_EXT)
                                {   //Adjust for header extension
                                    cmd_offset += buf[7];
                                }

                                if (rcx_size >= (int)(cmd_offset + 3))
                                {
                                    if ((buf[cmd_offset] == COMMAND_CLASS_NETWORK_MANAGEMENT_PROXY)
                                        && (buf[cmd_offset + 1] == NODE_INFO_CACHED_REPORT)
                                        && (buf[cmd_offset + 2] == nif_seq))
                                    {
                                        if (use_ipv4)
                                        {
                                            gw_ip4[idx++] = sa4;
                                        }
                                        else
                                        {
                                            gw_ip6[idx++] = sa6;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else if (rcx_size == TPT_SOCKET_ERR)
                {
                    //debug_gw_discvr("udp rcx error:%d", WSAGetLastError());
                    debug_gw_discvr("udp rcx error\n");
                    break;
                }
            }
        }
    }

    return idx;
}


/**
util_gw_discvr_thrd - thread to perform gateway discovery
@param[in]	data    Context
@return
*/
static void util_gw_discvr_thrd(void *data)
{
    gw_discvr_ctx_t *ctx = (gw_discvr_ctx_t *)data;
    uint8_t         zip_frm[] = {COMMAND_CLASS_ZIP, COMMAND_ZIP_PACKET, 0, 0x40, 0xaa, 0, 0,
                                 COMMAND_CLASS_NETWORK_MANAGEMENT_PROXY, NODE_INFO_CACHED_GET, 0xbb, 0/*Max age*/, 0/*Node id*/};

#if defined(_WINDOWS) || defined(WIN32)
    WSADATA             wsa_dat;
    SOCKET              sock_fd;
#else
    int                 sock_fd;
#endif
    int                 result;
    int                 i;
    uint8_t             gw_ip_addr[MAX_DISCOVERY_GW * IPV6_ADDR_LEN];
    uint8_t             host_ipv4_addr[MAX_DISCOVERY_GW * IPV4_ADDR_LEN];
    util_ipv6_addr_t    host_ipv6_addr[MAX_DISCOVERY_GW];
    int                 host_ip_cnt;        //Total number of host IPv4 addresses
    int                 host_ip_idx = 0;    //Read index to the host IPv4 addresses
    struct sockaddr_in  sa4;
    struct sockaddr_in  gw_ip4[MAX_DISCOVERY_GW];
    struct sockaddr_in6 sa6;
    struct sockaddr_in6 gw_ip6[MAX_DISCOVERY_GW];
    struct sockaddr     *gw_ip;
    uint8_t             gw_ip_addr_cnt;

#if defined(_WINDOWS) || defined(WIN32)
    // Initialize Winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsa_dat);
    if (result != NO_ERROR)
    {
        debug_gw_discvr("WSAStartup failed with error %d", result);
        return;
    }
#endif

    //Update thread status
    ctx->gw_thrd_sts = 1;

    //Assign sequence numbers
    //Init random number seed
    srand((unsigned)time(NULL));
    zip_frm[4] = rand() & 0xFF;
    zip_frm[9] = rand() & 0xFF;

    if (ctx->use_ipv4)
    {
        host_ip_cnt = util_ip_addr_get(MAX_DISCOVERY_GW, host_ipv4_addr, 1);
    }
    else
    {
        host_ip_cnt = util_ip_addr_get(MAX_DISCOVERY_GW, host_ipv6_addr, 0);
    }

    //testing IPv4 ---------------
//  memcpy(&sa4.sin_addr.s_addr, host_ipv4_addr + (1*IPV4_ADDR_LEN), IPV4_ADDR_LEN);
//  memcpy(host_ipv4_addr + (1*IPV4_ADDR_LEN), host_ipv4_addr, IPV4_ADDR_LEN);
//  memcpy(host_ipv4_addr, &sa4.sin_addr.s_addr, IPV4_ADDR_LEN);
//  //testing --------------------
//  //testing IPv6 ---------------
//  host_ipv6_addr[19] = host_ipv6_addr[1];
//  host_ipv6_addr[1] = host_ipv6_addr[0];
//  host_ipv6_addr[0] = host_ipv6_addr[19];
    //testing --------------------

    if (host_ip_cnt == 0)
    {   //No valid IP
        if (ctx->gw_thrd_run)
        {
            ctx->cb(NULL, 0, ctx->use_ipv4, ctx->usr_param, 0, 0, NULL);
        }
    }
    else
    {
        while (host_ip_idx < host_ip_cnt)
        {
            //Check whether to exit the thread due to user cancels the operation
            if (ctx->gw_thrd_run == 0)
            {
                //No callback
    #if defined(_WINDOWS) || defined(WIN32)
                WSACleanup(); //Clean up Winsock
    #endif
                ctx->gw_thrd_sts = 0;
                return;
            }

            if (ctx->use_ipv4)
            {
                gw_ip4[0].sin_family = AF_INET;
                gw_ip = (struct sockaddr *)gw_ip4;
                memcpy(&sa4.sin_addr.s_addr, host_ipv4_addr + (host_ip_idx*IPV4_ADDR_LEN), IPV4_ADDR_LEN);
                result = util_ipv4_broadcast(&sa4, zip_frm, sizeof(zip_frm), &sock_fd);
            }
            else
            {
                gw_ip6[0].sin6_family = AF_INET6;
                gw_ip = (struct sockaddr *)gw_ip6;
                memcpy(sa6.sin6_addr.s6_addr, host_ipv6_addr[host_ip_idx].addr, IPV6_ADDR_LEN);
                result = util_ipv6_multicast(&sa6, host_ipv6_addr[host_ip_idx].if_idx, zip_frm, sizeof(zip_frm), &sock_fd);
            }

            host_ip_idx++;
            gw_ip_addr_cnt = 0;

            if (result == 0)
            {
                //Get responses
                result = util_gw_discvr_wait_resp(sock_fd, zip_frm[9], MAX_DISCOVERY_GW, gw_ip);

    #if defined(_WINDOWS) || defined(WIN32)
                closesocket(sock_fd);
    #else
                close(sock_fd);
    #endif
                if (result > 0)
                {
                    //Save the gateway ip found
                    debug_gw_discvr("Gw discovery received %d responses\n", result);
                    if (ctx->use_ipv4)
                    {
                        for (i=0; i<result; i++)
                        {
                            if (gw_ip_addr_cnt < MAX_DISCOVERY_GW)
                            {
                                memcpy(&gw_ip_addr[gw_ip_addr_cnt * IPV4_ADDR_LEN], &gw_ip4[i].sin_addr.s_addr, IPV4_ADDR_LEN);
                                gw_ip_addr_cnt++;
                            }
                        }
                    }
                    else
                    {
                        for (i=0; i<result; i++)
                        {
                            if (gw_ip_addr_cnt < MAX_DISCOVERY_GW)
                            {
                                memcpy(&gw_ip_addr[gw_ip_addr_cnt * IPV6_ADDR_LEN], gw_ip6[i].sin6_addr.s6_addr, IPV6_ADDR_LEN);
                                gw_ip_addr_cnt++;
                            }
                        }
                    }
                }
            }

            //Callback to report gateway IP addresses for each host IP
            if (ctx->gw_thrd_run)
            {
                ctx->cb(gw_ip_addr, gw_ip_addr_cnt, ctx->use_ipv4, ctx->usr_param, host_ip_idx, host_ip_cnt, NULL);
            }
        }
    }


#if defined(_WINDOWS) || defined(WIN32)
    WSACleanup(); //Clean up Winsock
#endif
    ctx->gw_thrd_sts = 0;
    return;

}


/**
util_gw_discvr_start - Start Z/IP gateway discovery
@param[in]	    cb		    Callback function when the gateway discovery has completed
@param[in]	    usr_param	User defined parameter used in callback function
@param[in]	    use_ipv4	Flag to indicate whether to use IPv4 as transport IP protocol. 1= use IPv4; 0= use IPv6
@return Context on success, null on failure.
@post   Caller is required to call util_gw_discvr_stop() with the returned context if it is not null.
*/
void *util_gw_discvr_start(util_gw_discvr_cb_t cb, void *usr_param, int use_ipv4)
{
    int             wait_cnt;
    gw_discvr_ctx_t *ctx;

    ctx = (gw_discvr_ctx_t *)calloc(1, sizeof(gw_discvr_ctx_t));

    if (!ctx)
    {
        return NULL;
    }

    //Save the callback function
    ctx->cb = cb;
    ctx->usr_param = usr_param;

    ctx->use_ipv4 = use_ipv4;

    //Start Z/IP gateway discovery thread
    ctx->gw_thrd_run = 1;
    if (plt_thrd_create(util_gw_discvr_thrd, ctx) < 0)
    {
        return NULL;
    }

    //Wait for thread start to run
    wait_cnt = 50;
    while (wait_cnt-- > 0)
    {
        if (ctx->gw_thrd_sts == 1)
            break;
        plt_sleep(100);
    }

    return ctx;
}


/**
util_gw_discvr_stop - Stop Z/IP gateway discovery
@param[in]	    ctx		    The context returned from the call to util_gw_discvr_start()
@return Zero on success, non-zero on failure.
@post After the call, the ctx is invalid and should not be used
*/
int util_gw_discvr_stop(void *ctx)
{
    gw_discvr_ctx_t *gw_discvr_ctx = (gw_discvr_ctx_t *)ctx;

    if (!ctx)
    {
        return -1;
    }

    if (gw_discvr_ctx->gw_thrd_sts)
    {
        //Stop the thread
        int  wait_count = 50;

        gw_discvr_ctx->gw_thrd_run = 0;
        while (wait_count-- > 0)
        {
            if (gw_discvr_ctx->gw_thrd_sts == 0)
                break;
            plt_sleep(100);
        }
    }

    free(gw_discvr_ctx);

    return 0;
}


/**
util_local_addr_get - Get local address that is reachable by destination host
@param[in]  dest_ip     Destination host address either IPv4 or IPv6 according to use_ipv4 flag
@param[in]  use_ipv4    Flag to indicate IP address type. 1= IPv4; 0= IPv6
@param[out] local_ip    Buffer of 16-byte to store the local address (either IPv4 or IPv6 according to use_ipv4 flag)
@return     Zero if successful; otherwise non-zero
*/
int util_local_addr_get(uint8_t *dest_ip, uint8_t *local_ip, int use_ipv4)
{
    #if defined(_WINDOWS) || defined(WIN32)
    WSADATA             wsa_dat;
    SOCKET              sock_fd;
    #else
    int                 sock_fd;
    #endif
    int                 res;

    #if defined(_WINDOWS) || defined(WIN32)
    // Initialize Winsock
    res = WSAStartup(MAKEWORD(2, 2), &wsa_dat);
    if (res != NO_ERROR)
    {
        return -1;
    }
    #endif

    //Open socket and bind it to the source address
    sock_fd = socket((use_ipv4)? AF_INET : AF_INET6, SOCK_DGRAM, 0);
    if (sock_fd < 0)
    {
        res = -2;
        goto l_ADDR_GET_ERR1;
    }

    if (use_ipv4)
    {
        struct sockaddr_in  sa4 = {0};

        sa4.sin_family = AF_INET;
        sa4.sin_port = htons(ZWAVE_HOME_CTL_PORT);
        memcpy(&sa4.sin_addr.s_addr, dest_ip, IPV4_ADDR_LEN);

        //Connect to destination address
        res = connect(sock_fd, (const struct sockaddr *)&sa4, sizeof(struct sockaddr_in));
    }
    else    //IPv6
    {
        struct sockaddr_in6 sa6 = {0};

        sa6.sin6_family = AF_INET6;
        sa6.sin6_port = htons(ZWAVE_HOME_CTL_PORT);
        memcpy(sa6.sin6_addr.s6_addr, dest_ip, IPV6_ADDR_LEN);

        //Connect to destination address
        res = connect(sock_fd, (const struct sockaddr *)&sa6, sizeof(struct sockaddr_in6));
    }

    if (res < 0)
    {
        res = -3;
        goto l_ADDR_GET_ERR2;
    }

    if (use_ipv4)
    {   //IpV4
        struct sockaddr_in  name;
        socklen_t           namelen = sizeof(name);

        res = getsockname(sock_fd, (struct sockaddr *)&name, &namelen);

        if (res == 0)
        {
            memcpy(local_ip, &name.sin_addr.s_addr, IPV4_ADDR_LEN);
        }
        else
        {
            res = -4;
        }

    }
    else
    {   //IPv6
        struct sockaddr_in6     name;
        socklen_t               namelen = sizeof(name);

        res = getsockname(sock_fd, (struct sockaddr *)&name, &namelen);

        if (res == 0)
        {
            memcpy(local_ip, name.sin6_addr.s6_addr, IPV6_ADDR_LEN);
        }
        else
        {
            res = -4;
        }
    }

l_ADDR_GET_ERR2:
#if defined(_WINDOWS) || defined(WIN32)
    closesocket(sock_fd);
#else
    close(sock_fd);
#endif

l_ADDR_GET_ERR1:
#if defined(_WINDOWS) || defined(WIN32)
    //Clean up Winsock
    WSACleanup();
#endif
    return res;
}


/**
util_ipv6_zero_cmp - Compare the specified IPv6 address to all-zero IPv6 address
@param[in]  ip          IPv6 address
@return     Zero if same ; otherwise non-zero
*/
int util_ipv6_zero_cmp(uint8_t *ip)
{
    uint8_t zero_addr[IPV6_ADDR_LEN];

    memset(zero_addr, 0, IPV6_ADDR_LEN);

    return memcmp(ip, zero_addr, IPV6_ADDR_LEN);

}


/**
util_ipv4_zero_cmp - Compare the specified IPv4 address to 0.0.0.0
@param[in]  ip          IPv4 address
@return     Zero if same ; otherwise non-zero
*/
int util_ipv4_zero_cmp(uint8_t *ip)
{
    uint8_t zero_addr[IPV4_ADDR_LEN];

    memset(zero_addr, 0, IPV4_ADDR_LEN);

    return memcmp(ip, zero_addr, IPV4_ADDR_LEN);

}


/**
util_is_ipv4_mapped_ipv6 - Check if the address is IPv4-mapped-IPv6
@param[in]  ip          IPv6 address
@return     non-zero if the address is IPv4-mapped-IPv6; otherwise zero
*/
int util_is_ipv4_mapped_ipv6(uint8_t *ip)
{
    static const uint8_t ipv4_mapped_addr[12] = {0,0,0,0,0,0,0,0,0,0,0xFF,0xFF};

    return !(memcmp(ip, ipv4_mapped_addr, 12));

}


/**
util_msg_loop_thrd - Thread to wait for message
@param[in]	ctx		Context
@return
*/
static void util_msg_loop_thrd(void *ctx)
{
    util_msg_loop_t     *msg_ctx = (util_msg_loop_t *)ctx;
    util_lst_t          *lst_ent;
    int                 exit_thrd;
#ifdef CONFIG_DEBUG
    unsigned            id = msg_ctx->id;
#endif

    msg_ctx->thrd_sts = 1;

#ifdef CONFIG_DEBUG
    //Just to make the compiler happy
    if (id == 0)
    {
        msg_ctx->id = 0;
    }
#endif

    while (1)
    {
        //Wait for a message
        plt_sem_wait(msg_ctx->sem);

        //Check whether to exit the thread
        if (msg_ctx->thrd_run == 0)
        {
            msg_ctx->thrd_sts = 0;
            return;
        }

        while ((lst_ent = util_list_get(msg_ctx->mtx, &msg_ctx->msg_hd)) != NULL)
        {
            exit_thrd = msg_ctx->cb(msg_ctx->usr_prm, (void *)lst_ent->wr_buf);

            if (msg_ctx->free_fn)
            {
                msg_ctx->free_fn((void *)lst_ent->wr_buf);
            }
            free(lst_ent);

            //Check whether to exit the thread
            if ((msg_ctx->thrd_run == 0) || exit_thrd)
            {
                msg_ctx->thrd_sts = 0;
                return;
            }
        }
    }
}


/**
util_msg_loop_init - Initialize message loop
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
int util_msg_loop_init(util_msg_loop_fn cb, void *usr_prm, util_msg_free_fn free_fn, unsigned id, util_msg_loop_t **ctx)
{
    util_msg_loop_t     *msg_ctx;
    int                 wait_count;

    msg_ctx = calloc(1, sizeof(util_msg_loop_t));

    if (!msg_ctx)
    {
        return ZWHCI_ERROR_MEMORY;
    }

    msg_ctx->cb = cb;
    msg_ctx->usr_prm = usr_prm;
    msg_ctx->free_fn = free_fn;
    msg_ctx->id = id;

    if (!plt_sem_init(&msg_ctx->sem))
    {
        goto l_MSG_INIT_ERROR1;
    }

    if (!plt_mtx_init(&msg_ctx->mtx))
    {
        goto l_MSG_INIT_ERROR2;
    }

    msg_ctx->thrd_run = 1;
    if (plt_thrd_create(util_msg_loop_thrd, msg_ctx) < 0)
    {
        goto l_MSG_INIT_ERROR3;
    }
    //Wait for the thread to be ready
    wait_count = 20;
    while (wait_count-- > 0)
    {
        if (msg_ctx->thrd_sts)
            break;
        plt_sleep(100);
    }

    //ok
    *ctx = msg_ctx;
    return 0;

l_MSG_INIT_ERROR3:
    plt_mtx_destroy(msg_ctx->mtx);
l_MSG_INIT_ERROR2:
    plt_sem_destroy(msg_ctx->sem);
l_MSG_INIT_ERROR1:
    free(msg_ctx);
    return  ZWHCI_ERROR_RESOURCE;

}


/**
util_msg_loop_shutdown - Stop the message loop
@param[in]  msg_ctx Context
@param[in]  tm_out	Timeout (in milliseconds) to wait for message loop to stop. If zero, no timeout.
@return     Return 0 on success, negative error number on failure.
*/
int util_msg_loop_shutdown(util_msg_loop_t *msg_ctx, uint32_t tm_out)
{
    int res = ZWHCI_ERROR_TIMEOUT;

    msg_ctx->thrd_run = 0;
    plt_sem_post(msg_ctx->sem);

    //Wait for thread to exit
    if (tm_out == 0)
    {
        while (1)
        {
            if (msg_ctx->thrd_sts == 0)
            {
                res = 0;
                break;
            }
            plt_sleep(100);
        }
    }
    else
    {
        int wait_count;

        wait_count = tm_out/100;
        while (wait_count-- > 0)
        {
            if (msg_ctx->thrd_sts == 0)
            {
                res = 0;
                break;
            }
            plt_sleep(100);
        }
    }

    return res;
}


/**
util_msg_loop_exit - Free the allocated memory
@param[in]  msg_ctx Context
@return
@pre  Should call util_msg_loop_shutdown() first, then wait for the appropriate time to call this function safely
@post Do not call util_msg_loop_send() after calling this function
*/
void util_msg_loop_exit(util_msg_loop_t *msg_ctx)
{
    if (msg_ctx->free_fn)
    {   //Free one entry at a time
        util_lst_t  *lst_ent;
        while ((lst_ent = util_list_get(msg_ctx->mtx, &msg_ctx->msg_hd)) != NULL)
        {
            msg_ctx->free_fn((void *)lst_ent->wr_buf);
            free(lst_ent);
        }
    }
    else
    {
        util_list_flush(msg_ctx->mtx, &msg_ctx->msg_hd);
    }

    plt_sem_destroy(msg_ctx->sem);
    plt_mtx_destroy(msg_ctx->mtx);
    free(msg_ctx);
}


/**
util_msg_loop_send - Send a message to message loop
@param[in]  msg_ctx Context
@param[in]  msg	    Message
@param[in]  msg_sz  Message size
@return     Return 0 on success, negative error number on failure.
*/
int util_msg_loop_send(util_msg_loop_t *msg_ctx, void *msg, uint8_t msg_sz)
{
    int res;

    if (msg_ctx->thrd_sts == 0)
    {
        return ZWHCI_ERROR_WRITE;
    }

    res = util_list_add(msg_ctx->mtx, &msg_ctx->msg_hd, (uint8_t *)msg, msg_sz);

    if (res == 0)
    {
        plt_sem_post(msg_ctx->sem);
    }

    return res;
}


/**
util_time_stamp - Get the current calendar time as time stamp
@param[out]  ts     Time stamp which represents the number of seconds elapsed since 00:00:00 on January 1, 1970,
                    Coordinated Universal Time
@return     Return 0 on success; else negative value.
*/
int util_time_stamp(time_t *ts)
{
    if (time(ts) != (time_t)(-1))
    {
        return 0;
    }
    return -1;
}


/**
util_time_get - Get the current calendar time
@return     The number of seconds elapsed since 00:00:00 on January 1, 1970, Coordinated Universal Time
*/
time_t util_time_get(void)
{
    return time(NULL);
}


/**
util_path_dup - Duplicate and add appropriate path separator of a directory path
@param[in]  path    Directory path
@return     Return duplicated path if successful; else return NULL
@post       Caller MUST free the returned path if it is non NULL
*/
char *util_path_dup(const char *path)
{
    if (path)
    {
        size_t path_len;
        char   *dup_path;

        path_len = strlen(path);

        if (path_len > 0)
        {
            dup_path = malloc(path_len + 2);

            if (dup_path)
            {
                memcpy(dup_path, path, path_len);

                //Add path separator as needed
#if defined(_WINDOWS) || defined(WIN32)
                if (dup_path[path_len - 1] != '\\')
                {
                    dup_path[path_len++] = '\\';
                }
#else
                if (dup_path[path_len - 1] != '/')
                {
                    dup_path[path_len++] = '/';
                }
#endif
                dup_path[path_len] = '\0';

                return dup_path;

            }
        }
    }

    return NULL;
}


/**
util_tmr_tick_thrd - Timer tick thread
@param[in]	ctx		Context
@return
*/
static void util_tmr_tick_thrd(void *ctx)
{
    util_tmr_tick_t     *tmr_tick_ctx = (util_tmr_tick_t *)ctx;

    tmr_tick_ctx->thrd_sts = 1;

    while (tmr_tick_ctx->thrd_run)
    {
        plt_sleep(tmr_tick_ctx->tick_intv_ms);
        tmr_tick_ctx->tick++;
    }

    tmr_tick_ctx->thrd_sts = 0;
}


/**
util_tmr_tick_init - Initialize timer tick generator
@param[in]  tick_intv_ms    Timer tick interval in milliseconds
@param[out] ctx             Context
@return     Return 0 on success, negative error number on failure.
@post       Caller should call util_tmr_tick_exit() to free the allocated memory
*/
int util_tmr_tick_init(uint32_t tick_intv_ms, util_tmr_tick_t **ctx)
{
    util_tmr_tick_t     *tmr_tick_ctx;
    int                 wait_count;

    tmr_tick_ctx = calloc(1, sizeof(util_tmr_tick_t));

    if (!tmr_tick_ctx)
    {
        return ZWHCI_ERROR_MEMORY;
    }

    tmr_tick_ctx->tick_intv_ms = tick_intv_ms;

    tmr_tick_ctx->thrd_run = 1;
    if (plt_thrd_create(util_tmr_tick_thrd, tmr_tick_ctx) < 0)
    {
        free(tmr_tick_ctx);
        return  ZWHCI_ERROR_RESOURCE;
    }

    //Wait for the thread to be ready
    wait_count = 20;
    while (wait_count-- > 0)
    {
        if (tmr_tick_ctx->thrd_sts)
            break;
        plt_sleep(10);
    }

    //ok
    *ctx = tmr_tick_ctx;
    return 0;

}


/**
util_tmr_tick_exit - Stop timer tick and free the allocated memory
@param[in]  tmr_tick_ctx Context
@return
*/
void util_tmr_tick_exit(util_tmr_tick_t *tmr_tick_ctx)
{
    tmr_tick_ctx->thrd_run = 0;

    while (tmr_tick_ctx->thrd_sts)
    {
        plt_sleep(100);
    }

    free(tmr_tick_ctx);
}


/**
util_tmr_tick_get - Get current timer tick count
@param[in]  tmr_tick_ctx Context
@return timer tick count
*/
uint32_t util_tmr_tick_get(util_tmr_tick_t *tmr_tick_ctx)
{
    return tmr_tick_ctx->tick;
}


/**
util_rx_pkt_dest_addr_get - Get received packet header destination address
@param[in]	mh		        Received message header
@param[out]	hdr_dest_addr	Buffer to store header destination address
@param[in] use_ipv4         Flag to indicate whether to use IPv4
@return          Non-zero on success; otherwise zero on failure.
*/
int util_rx_pkt_dest_addr_get(struct msghdr *mh, uint8_t *hdr_dest_addr, int use_ipv4)
{
    struct cmsghdr *cmsg;

    for ( // Iterate through all the control headers
         cmsg = CMSG_FIRSTHDR(mh);
         cmsg != NULL;
         cmsg = CMSG_NXTHDR(mh, cmsg))
    {
        if (use_ipv4)
        {
            if ((cmsg->cmsg_level == IPPROTO_IP) && (cmsg->cmsg_type == IP_PKTINFO))
            {
                struct in_pktinfo *pi = (struct in_pktinfo *)CMSG_DATA(cmsg);

                memcpy(hdr_dest_addr, &pi->ipi_addr.s_addr, 4);

                return 1;
            }
        }
        else
        {   //IPv6
            if ((cmsg->cmsg_level == IPPROTO_IPV6) && (cmsg->cmsg_type == IPV6_PKTINFO))
            {
                struct in6_pktinfo *pi6 = (struct in6_pktinfo *)CMSG_DATA(cmsg);

                memcpy(hdr_dest_addr, pi6->ipi6_addr.s6_addr, 16);

                return 1;
            }
        }
    }

    return 0;
}


/**
util_file_ext_get - get file name extension
@param[in]  file_name   File path
@return     File extension; otherwise NULL.
*/
char *util_file_ext_get(const char *file_name)
{
    char *dot_ptr = (char *)file_name;     //Point to the "."
    char *last_dot_ptr = NULL;     //Point to the last "."

    while ((dot_ptr = strchr(dot_ptr, '.')) != NULL)
    {
        dot_ptr++;
        last_dot_ptr = dot_ptr;
    }

    return last_dot_ptr;
}


/**
util_file_write - Write a buffer info file
@param[in]      file_name     file name
@param[in]      buf           buffer that contains data to be written
@param[in]      len           buffer length
@return		Non-zero on success; otherwise zero.
*/
int util_file_write(const char *file_name, uint8_t *buf, size_t len)
{
    FILE *file;

    if (!file_name)
    {
        return 0;
    }

#ifdef USE_SAFE_VERSION
    if (fopen_s(&file, file_name, "wb") != 0)
    {
        return 0;
    }
#else
    file = fopen(file_name, "wb");
    if (!file)
    {
        return 0;
    }
#endif

    if (file)
    {
        size_t elm_cnt;

        elm_cnt = fwrite(buf, len, 1, file);
        fclose(file);

        if (elm_cnt == 1)
        {
            return 1;
        }
    }

    return 0;
}


#define H2BIN_MIN_RECORD_LEN    11      /**< Minimum record length*/
#define H2BIN_LINE_BUF_SZ       (H2BIN_MIN_RECORD_LEN + 260*2)      /**< Line buffer size*/

/** Intel hex to binary conversion error code */
#define H2BIN_ERR_TOO_LARGE     -1      /**< Binary size is too large*/
#define H2BIN_ERR_PARSE         -2      /**< Parse Intel hex file error*/
#define H2BIN_ERR_TOO_SMALL     -3      /**< Binary size is too small*/
#define H2BIN_ERR_MEM           -4      /**< No memory*/
#define H2BIN_ERR_CHKSUM        -5      /**< Checksum error*/

/** Intel hex to binary conversion addressing mode */
#define H2BIN_ADDR_MODE_LINEAR   0      /**< Linear */
#define H2BIN_ADDR_MODE_SEG      1      /**< Segment */


/** Intel hex record type */
#define H2BIN_TYPE_DATA             0      /**< Data*/
#define H2BIN_TYPE_EOF              1      /**< End of file*/
#define H2BIN_TYPE_SEGMENT_ADDR     2      /**< Extended segment address*/
#define H2BIN_TYPE_LINEAR_ADDR      4      /**< Extended linear address*/


/**
util_hex_str_2_bin - Convert hex string to binary
@param[in]  in_buf     Input buffer
@param[out] out_buf    Output binary buffer
@param[in]  data_len   Number of input data bytes to read
@param[out] chksum     Checksum of all input data + one more byte which is the checksum byte
@return     Non-zero on success; otherwise zero
*/
static int util_hex_str_2_bin(char *in_buf, uint8_t *out_buf, uint16_t data_len, uint16_t *chksum)
{
    unsigned    bin_byte;
    int         result;
    uint16_t    byte_rd = 0;        //Number of bytes read
    uint16_t    check_sum = 0;

	while (data_len-- > 0)
	{
		result = sscanf (in_buf, "%2x",&bin_byte);
		if (result != 1)
        {
            //fprintf(stderr,"util_hex_str_2_bin: error reading a hex byte\n");
            return 0;;
        }

        out_buf[byte_rd++] = bin_byte;
        check_sum += bin_byte;

		in_buf += 2;
	}

    //Read and calculate checksum byte
    result = sscanf (in_buf, "%2x",&bin_byte);
    if (result != 1)
    {
        //fprintf(stderr,"util_hex_str_2_bin: error reading a checksum byte\n");
        return 0;;
    }

    check_sum += bin_byte;

    *chksum = check_sum & 0xFF;

	return 1;
}


/**
util_intel_hex_bin_parse - Parse Intel hex to binary
@param[in]  hex_file   File pointer of the input file
@param[out] buf        Binary buffer
@param[in]  buf_sz     Binary buffer size
@return     Zero on success; otherwise H2BIN_ERR_XXX.
@pre        Must call util_intel_hex_bin_sz() to calculate the buffer size before calling this function
*/
static int util_intel_hex_bin_parse(FILE *hex_file, uint8_t *buf, uint32_t buf_sz)
{
    uint32_t lowest_addr = 0xFFFFFFFF;
    uint32_t highest_addr = 0;  //Highest address that contains the last byte
    uint32_t seg_addr = 0;
    uint32_t linear_addr = 0;
    uint32_t rec_num = 0;
    uint32_t phy_addr;  //Physical address
    uint32_t last_byte_addr;
    uint16_t chksum;
    char     line[H2BIN_LINE_BUF_SZ];
    uint8_t	 data_str[H2BIN_LINE_BUF_SZ];
    size_t   rec_len;
    int      i;
    int      result;
    int      addr_mode = H2BIN_ADDR_MODE_LINEAR;
    unsigned data_len;
    unsigned rec_start_addr;
    unsigned rec_type;
    unsigned chksum_tmp;
    char     *p;

    while (fgets(line, H2BIN_LINE_BUF_SZ, hex_file))
    {
        rec_num++;

        rec_len = strlen(line);

        if (rec_len < H2BIN_MIN_RECORD_LEN)
        {
            continue;
        }

        i = rec_len - 1;

        //Remove line feed and carriage return characters
        while ((i >= 0) &&
               ((line[i] == '\n') || (line[i] == '\r')))
        {
            line[i--] = '\0';
        }

        // Intel hex format:  ":llaaaatt[dd....]cc
        // where ll = length of data dd
        //       aaaa = starting address of data
        //       tt = record type
        //       dd = data
        //       cc = checksum
        result = sscanf (line, ":%2x%4x%2x%s", &data_len, &rec_start_addr, &rec_type, data_str);
        if (result != 4)
        {
            //fprintf(stderr,"Error in line %d of hex file\n", rec_num);
            return H2BIN_ERR_PARSE;
        }

        p = (char *)data_str;

        switch (rec_type)
        {
            case H2BIN_TYPE_DATA:

                if (data_len == 0)
                    break;

                phy_addr = ((addr_mode == H2BIN_ADDR_MODE_SEG)? seg_addr : linear_addr) + rec_start_addr;

                // Set the lowest address
                if (phy_addr < lowest_addr)
                    lowest_addr = phy_addr;

                // Set the highest address
                last_byte_addr = phy_addr + data_len - 1;

                if (last_byte_addr > highest_addr)
                {
                    highest_addr = last_byte_addr;
                }

                if (!util_hex_str_2_bin(p, buf + phy_addr, data_len, &chksum))
                {
                    return H2BIN_ERR_PARSE;
                }

                chksum = data_len + (rec_start_addr >> 8) + (rec_start_addr & 0xFF) + rec_type + chksum;

                if ((chksum & 0xFF) != 0)
                {
                    return H2BIN_ERR_CHKSUM;
                }
                break;

            case H2BIN_TYPE_EOF:
                return 0;
                break;

            case H2BIN_TYPE_SEGMENT_ADDR:

                addr_mode = H2BIN_ADDR_MODE_SEG;
                sscanf (p, "%4x%2x",&seg_addr,&chksum_tmp);
                seg_addr = (seg_addr << 4);

                break;

            case H2BIN_TYPE_LINEAR_ADDR:

                addr_mode = H2BIN_ADDR_MODE_LINEAR;
                sscanf (p, "%4x%2x",&linear_addr,&chksum_tmp);
                linear_addr = (linear_addr << 16);

                break;

            default:
                break;
        }

    }

    //EOF, return o.k. even if no EOF record found
    return 0;
}


/**
util_intel_hex_bin_sz - calculate the Intel hex to binary size
@param[in]  hex_file   File pointer of the input file
@param[out] bin_sz     Binary size
@return     Zero on success; otherwise H2BIN_ERR_XXX.
*/
static int util_intel_hex_bin_sz(FILE *hex_file, uint32_t *bin_sz)
{
    uint32_t lowest_addr = 0xFFFFFFFF;
    uint32_t highest_addr = 0;  //Highest address that contains the last byte
    uint32_t seg_addr = 0;
    uint32_t linear_addr = 0;
    uint32_t rec_num = 0;
    uint32_t phy_addr;  //Physical address
    uint32_t last_byte_addr;
    char     line[H2BIN_LINE_BUF_SZ];
    uint8_t	 data_str[H2BIN_LINE_BUF_SZ];
    size_t   rec_len;
    int      i;
    int      result;
    int      addr_mode = H2BIN_ADDR_MODE_LINEAR;
    unsigned data_len;
    unsigned rec_start_addr;
    unsigned rec_type;
    unsigned chksum_tmp;
    char     *p;

    while (fgets(line, H2BIN_LINE_BUF_SZ, hex_file))
    {
        rec_num++;

        rec_len = strlen(line);

        if (rec_len < H2BIN_MIN_RECORD_LEN)
        {
            continue;
        }

        i = rec_len - 1;

        //Remove line feed and carriage return characters
        while ((i >= 0) &&
               ((line[i] == '\n') || (line[i] == '\r')))
        {
            line[i--] = '\0';
        }

        // Intel hex format:  ":llaaaatt[dd....]cc
        // where ll = length of data dd
        //       aaaa = starting address of data
        //       tt = record type
        //       dd = data
        //       cc = checksum
        result = sscanf (line, ":%2x%4x%2x%s", &data_len, &rec_start_addr, &rec_type, data_str);
        if (result != 4)
        {
            //fprintf(stderr,"Error in line %d of hex file\n", rec_num);
            rewind(hex_file);
            return H2BIN_ERR_PARSE;
        }

        p = (char *)data_str;

        switch (rec_type)
        {
            case H2BIN_TYPE_DATA:

                if (data_len == 0)
                    break;

                phy_addr = ((addr_mode == H2BIN_ADDR_MODE_SEG)? seg_addr : linear_addr) + rec_start_addr;

                // Set the lowest address
                if (phy_addr < lowest_addr)
                    lowest_addr = phy_addr;

                // Set the highest address
                last_byte_addr = phy_addr + data_len - 1;

                if (last_byte_addr > highest_addr)
                {
                    highest_addr = last_byte_addr;
                }
                break;

            case H2BIN_TYPE_EOF:
                *bin_sz = highest_addr + 1;
                rewind(hex_file);
                return 0;
                break;

            case H2BIN_TYPE_SEGMENT_ADDR:

                addr_mode = H2BIN_ADDR_MODE_SEG;
                result = sscanf (p, "%4x%2x",&seg_addr,&chksum_tmp);
                if (result != 2)
                {
                    rewind(hex_file);
                    return H2BIN_ERR_PARSE;
                }
                seg_addr = (seg_addr << 4);

                break;

            case H2BIN_TYPE_LINEAR_ADDR:

                addr_mode = H2BIN_ADDR_MODE_LINEAR;

                result = sscanf (p, "%4x%2x",&linear_addr,&chksum_tmp);
                if (result != 2)
                {
                    rewind(hex_file);
                    return H2BIN_ERR_PARSE;

                }

                linear_addr = (linear_addr << 16);

                break;

            default:
                break;
        }

    }
    //EOF, return o.k. even if no EOF record found
    *bin_sz = highest_addr + 1;
    rewind(hex_file);
    return 0;
}


/**
util_intel_hex_2_bin - convert Intel hex to binary
@param[in]  hex_file   File pointer of the input file
@param[in]  max_sz     Maximum output buffer size
@param[out] out_buf    Output buffer
@param[out] out_len    Output buffer size
@return     Zero on success; otherwise H2BIN_ERR_XXX
@post       Caller MUST free the out_buf if return value is zero
*/
int util_intel_hex_2_bin(FILE *hex_file, uint32_t max_sz, uint8_t **out_buf, uint32_t *out_len)
{
    int         res;
    uint32_t    bin_sz;
    uint8_t     *buf;

    res = util_intel_hex_bin_sz(hex_file, &bin_sz);

    if (res < 0)
    {
        return res;
    }

    if (bin_sz > max_sz)
    {
        return H2BIN_ERR_TOO_LARGE;
    }

    if (bin_sz < 10)
    {
        return H2BIN_ERR_TOO_SMALL;
    }

    buf = (uint8_t *) malloc(bin_sz);

    if (!buf)
    {
        return H2BIN_ERR_MEM;
    }

    //Pad unused bytes with 0xFF for EEPROM usage
    memset(buf, 0xFF, bin_sz);

    //Parse the Intel hex into binary
    res = util_intel_hex_bin_parse(hex_file, buf, bin_sz);

    if (res < 0)
    {
        free(buf);
        return res;
    }

    *out_buf = buf;
    *out_len = bin_sz;

    return 0;

}


/**
util_str_2_num_type - convert string to number type that supports floating point
@param[in]  num_str    Number string that should contain only '0' to '9', '-' or '.'
@param[in]  max_prec   Maximum precision (decimal places)
@param[out] out_num    Number type
@return     Zero on success; otherwise negative number
*/
int util_str_2_num_type(char *num_str, uint8_t max_prec, zwnum_type_t *out_num)
{
    char    *decimal_pt;
    int     int_num;
    int     dec_pt_cnt = 0;
    int     i = 0;
    size_t  str_len;

    while (num_str[i])
    {
        if (isdigit(num_str[i]) || (num_str[i] == '-') || (num_str[i] == '.'))
        {
            if ((num_str[i] == '-') && (i > 0))
            {   //Minus sign is not at the beginning of string
                return -1;
            }

            if ((num_str[i] == '.') && (dec_pt_cnt++ > 0))
            {   //More than one decimal point
                return -2;
            }

            //Increment index
            i++;
        }
        else
        {
            return -3;
        }
    }

    if ((decimal_pt = strchr(num_str, '.')) != NULL)
    {   //Have decimal point
        str_len = strlen(decimal_pt + 1);

        if (str_len > max_prec)
        {
            *(decimal_pt + 1 + max_prec) = '\0';
        }

        //Save precision
        out_num->precision = strlen(decimal_pt + 1);

        //Remove decimal point
        memmove(decimal_pt, decimal_pt + 1, out_num->precision + 1);
    }
    else
    {   //Integer
        out_num->precision = 0;
    }

    if (sscanf(num_str, "%d", &int_num) != 1)
    {
        return -4;
    }

    if ((int_num >= -128) && (int_num <= 127))
    {
        out_num->size = 1;
        out_num->data[0] = (uint8_t)(int_num & 0xFF);
    }
    else if ((int_num >= -32768) && (int_num <= 32767))
    {
        out_num->size = 2;
        out_num->data[0] = (uint8_t)((int_num >> 8) & 0xFF);
        out_num->data[1] = (uint8_t)(int_num & 0xFF);
    }
    else
    {
        out_num->size = 4;
        out_num->data[0] = (uint8_t)((int_num >> 24) & 0xFF);
        out_num->data[1] = (uint8_t)((int_num >> 16) & 0xFF);
        out_num->data[2] = (uint8_t)((int_num >> 8) & 0xFF);
        out_num->data[3] = (uint8_t)(int_num & 0xFF);

    }

    return 0;

}


/**
util_bitmsk_str_decode - decode a bitmask string into number string
@param[in]  bitmsk_buf      Bitmask buffer
@param[in]  bitmsk_buf_sz   Number of bitmask bytes in bitmsk_buf
@param[out] out_buf         Output number buffer
@param[in,out] out_buf_sz   Input: the length of out_buf in bytes. Output: the decoded numbers stored in out_buf in bytes
@param[in]  first_bit_0_eq_0      Flag to indicate whether the first byte bit-0 should be decoded as 0; else it is decoded as 1.
@return     Non-zero on success; otherwise zero
*/
int util_bitmsk_str_decode(uint8_t *bitmsk_buf, uint8_t bitmsk_buf_sz, uint8_t *out_buf, uint8_t *out_buf_sz, int first_bit_0_eq_0)
{
    uint16_t    i;
    uint16_t    max_num_byte;
    uint8_t     num_cnt = 0;

    if (!bitmsk_buf || !out_buf)
    {
        return 0;
    }

    max_num_byte = bitmsk_buf_sz * 8;

    if (max_num_byte > (*out_buf_sz))
    {   //Cap the maximum number bytes
        max_num_byte = (*out_buf_sz);
    }

    for (i=0; i < max_num_byte; i++)
    {
        if ((bitmsk_buf[i>>3] >> (i & 0x07)) & 0x01)
        {
            out_buf[num_cnt++] = (first_bit_0_eq_0)? i : (i + 1);
        }
    }
    *out_buf_sz = num_cnt;

    return 1;

}


/**
util_bitmsk_str_encode - encode a number string into a bitmask string
@param[in]      num_buf         Number string buffer
@param[in]      num_buf_sz      Number of bytes in num_buf
@param[out      bitmsk_buf      Bitmask buffer
@param[in,out]  bitmsk_buf_sz   Input: the length of bitmsk_buf in bytes. Output: the encoded bitmask stored in bitmsk_buf in bytes
@param[in]  first_bit_0_eq_0    Flag to indicate whether the first byte bit-0 should be decoded as 0; else it is decoded as 1.
@return     zero on success; otherwise negative error code
*/
int util_bitmsk_str_encode(uint8_t *num_buf, uint8_t num_buf_sz, uint8_t *bitmsk_buf, uint8_t *bitmsk_buf_sz, int first_bit_0_eq_0)
{
    uint16_t    i;
    uint16_t    max_num_byte;
    uint8_t     num_cnt = 0;
    uint8_t     number_byte;
    uint8_t     idx;

    if (!num_buf || !bitmsk_buf)
    {
        return -1;
    }

    max_num_byte = *bitmsk_buf_sz;

    memset(bitmsk_buf, 0, max_num_byte);

    for (i=0; i < num_buf_sz; i++)
    {
        number_byte = num_buf[i];

        if (!first_bit_0_eq_0)
        {
            if (number_byte == 0)
            {
                return -2;
            }
            number_byte--;
        }

        idx = number_byte >> 3;

        if (idx >= max_num_byte)
        {
            return -3;//bitmask buffer is too small
        }
        //Set the bit
        bitmsk_buf[idx] |= (0x01 << (number_byte & 0x07));

    }

    //Find the maximum number of bitmask bytes required
    for (i=0; i < max_num_byte; i++)
    {
        if (bitmsk_buf[i])
        {
            num_cnt = i + 1;
        }
    }

    *bitmsk_buf_sz = num_cnt;

    return 0;

}


/**
util_tlv_get - Get a specific TLV (type-length-value)in a series of TLVs
@param[in] buf      Buffer that stores the TLVs
@param[in] len      Number of bytes in buf
@param[in] type     Type to get
@param[out] tlv_len The length of the TLV value field
@return             Pointer to the TLV value field; NULL if the TLV is not found or error
*/
uint8_t *util_tlv_get(uint8_t *buf, uint16_t len, uint8_t type, uint8_t *tlv_len)
{
    uint8_t     *ptr = buf;
    uint16_t    remain_len = len;
    uint8_t     val_len;

    while (remain_len >= 2)
    {
        if (*ptr++ == type)
        {
            //check the length field
            if ((*ptr + 2) <= remain_len)
            {
                *tlv_len = *ptr;
                return (ptr + 1);
            }
            break;
        }

        val_len = *ptr;
        //Point to next TLV
        ptr += (val_len + 1);

        if ((val_len + 2) < remain_len)
        {
            remain_len -= (val_len + 2);
        }
        else
        {
            break;
        }
    }
    return NULL;
}


/**
@}
*/



