/**
@file   zip_mdns.c - Z/IP multicast DNS functions.

        To implement multicast DNS functions.

@author David Chow

@version    1.0 11-11-14  Initial release

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

#define _GNU_SOURCE         //For strcasestr()

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
#if defined(_WINDOWS) || defined(WIN32)
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <iphlpapi.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#ifndef OS_ANDROID
#include <ifaddrs.h>
#endif
#include <unistd.h>
#endif

#include "../include/zip_transport.h"
#include "../include/zip_mdns.h"
#include "../include/zwave/ZW_classcmd.h"

#ifdef OS_MAC_X
    #ifndef IPV6_ADD_MEMBERSHIP
#define IPV6_ADD_MEMBERSHIP IPV6_JOIN_GROUP
    #endif
#endif

//#define  MDNS_GET_ZWAVE_RESOURCE_INFO   //Get DNS type : DNS_TYPE_TXT

/**
@defgroup MDNS multicast DNS Functions
Some multicast DNS functions.
@{
*/
#define MDNS_PORT                   5353        ///< MDNS server port
#define DNS_MAX_LABEL_SIZE          63          ///< Maximum label size
#define DNS_PACKET_HEADER_SIZE      12          ///< DNS packet header size
#define ZIP_RESOURCE_DOMAIN         "_z-wave._udp.local"    ///< Z/IP resource domain

#define MAX_DISCOVERY_GW            100         ///< Maximum number of gateways to discover
#define MAX_NW_INTF                 10          ///< Maximum number of network interfaces for sending mdns multicast
#define MDNS_CTX_ID                 0x01BABA38  ///< MDNS context magic number for identification purposes
#define MAX_MULTICAST_RCX_SZ        1472        ///< Maximum multicast receive size

///
/// DNS type
#define DNS_TYPE_A          1       ///< IPv4 host address
#define DNS_TYPE_NS         2       ///< an authoritative name server
#define DNS_TYPE_CNAME      5       ///< the canonical name for an alias
#define DNS_TYPE_SOA        6       ///< marks the start of a zone of authority
#define DNS_TYPE_WKS        11      ///< a well known service description
#define DNS_TYPE_PTR        12      ///< a domain name pointer
#define DNS_TYPE_HINFO      13      ///< host information
#define DNS_TYPE_TXT        16      ///< text strings
#define DNS_TYPE_AAAA       28      ///< IPv6 host address
#define DNS_TYPE_SRV        33      ///< location and service

///
/// DNS class
#define DNS_CLS_IN          1       ///< the Internet

///
/// Z/IP host record
typedef struct
{
    char                *srv_name;      ///< null terminate utf-8 service name, e.g.  Static Controller [5d9624d60100]._z-wave._udp.local
    char                *host_name;     ///< null terminate utf-8 host name, e.g.  zw5D9624D601.local
    uint8_t             host_ip[IPV6_ADDR_LEN];      ///< host ip address
} mdns_host_rec_t;

///
/// DNS location and service (SRV) resource record
typedef struct
{
    uint16_t    priority;           ///< The priority of this target host; lowest is the highest priority
    uint16_t    weight;             ///< The weight of this target host; specifies a relative weight for entries with the same priority
    uint16_t    port;               ///< The port on the target host of this service
    char        *target_host;       ///< The domain name of the target host.

} mdns_srv_rr_t;

///
/// DNS resource record
typedef struct
{
    uint16_t            type;           ///< type, DNS_TYPE_XXX
    uint32_t            ttl;            ///< time to live in seconds
    char                *name;          ///< null terminate utf-8 record name, e.g.  _z-wave._udp.local
    void                *rr_data;       ///< data structure pertinent to the resource record type.
} mdns_rr_t;

///
/// Z/IP Gateway discovery context
typedef struct
{
    int                 ctx_id;         ///< contact id for run time identifying this context
    volatile int        gw_thrd_run;    ///< control the gateway discovery thread whether to run. 1 = run, 0 = stop
    volatile int        gw_thrd_sts;    ///< gateway discovery thread status. 1 = running, 0 = thread exited
    int                 use_ipv4;       ///< flag to indicate whether to use IPv4 as transport IP protocol
    mdns_gw_discvr_cb_t cb;             ///< gateway discovery callback function
    void                *usr_param;     ///< user defined parameter used in callback function
} mdns_discvr_ctx_t;

static const uint8_t ipv4_mdns_mcast_addr[IPV4_ADDR_LEN] = { 224, 0, 0, 251 };
static const uint8_t ipv6_mdns_mcast_addr[IPV6_ADDR_LEN] = { 0xFF, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFB} ;


/**
mdns_mcast4 - multicast message to mdns link-local multicast address 224.0.0.251
@param[in]      sfd         Socket file descriptor
@param[in]      buf         Buffer that store the data to be sent
@param[in]      dat_sz      Size of data to be sent
@return    On  success,  return  the number of characters sent.  On error, -1 is returned.
*/
#if defined(_WINDOWS) || defined(WIN32)
static ssize_t mdns_mcast4(SOCKET sfd, uint8_t *buf, uint16_t dat_sz)
#else
static ssize_t mdns_mcast4(int sfd, uint8_t *buf, uint16_t dat_sz)
#endif
{
    struct sockaddr_in  sa_mcast;

    sa_mcast.sin_family = AF_INET;
    sa_mcast.sin_port = htons(MDNS_PORT);
    memcpy(&sa_mcast.sin_addr.s_addr, ipv4_mdns_mcast_addr, IPV4_ADDR_LEN);

    return sendto(sfd, buf, dat_sz,
                  0, (const struct sockaddr *)&sa_mcast, sizeof(struct sockaddr_in));
}


/**
mdns_mcast6 - send multicast message to a specific IPv6 network
@param[in]      sfd         Socket handler which can be used to received response to the multicast message
@param[in]      buf         Buffer that store the data to be sent
@param[in]      dat_sz      Size of data to be sent
@return    On  success,  return  the number of characters sent.  On error, -1 is returned.
*/
#if defined(_WINDOWS) || defined(WIN32)
static ssize_t mdns_mcast6(SOCKET sfd, uint8_t *buf, uint16_t dat_sz)
#else
static ssize_t mdns_mcast6(int sfd, uint8_t *buf, uint16_t dat_sz)
#endif
{
    struct sockaddr_in6 sa_multicast;

    sa_multicast.sin6_family = AF_INET6;
    sa_multicast.sin6_port = htons(MDNS_PORT);
    sa_multicast.sin6_flowinfo = 0;
    sa_multicast.sin6_scope_id = 0;
    memcpy(sa_multicast.sin6_addr.s6_addr, ipv6_mdns_mcast_addr, IPV6_ADDR_LEN);

    return sendto(sfd, buf, dat_sz,
                  0, (const struct sockaddr *)&sa_multicast, sizeof(struct sockaddr_in6));

}


/**
mdns_encode_hdr - Encode DNS header
@param[in]      id          message ID
@param[in]      flags       flags
@param[in]      qdcnt       number of entries in the question section
@param[in]      ancnt       number of resource records (RR) in the answer section
@param[in]      nscnt       number of name server resource records (RR) in authority records section
@param[in]      arcnt       number of resource records (RR) in the additional records section
@param[out]     buf         Buffer to store the encoded DNS header
@return
*/
static void mdns_encode_hdr(uint16_t id, uint16_t flags, uint16_t qdcnt, uint16_t ancnt, uint16_t nscnt, uint16_t arcnt, uint8_t *buf)
{
    uint16_t *u16_ptr = (uint16_t *)buf;

    *u16_ptr++ = htons(id);
    *u16_ptr++ = htons(flags);
    *u16_ptr++ = htons(qdcnt);
    *u16_ptr++ = htons(ancnt);
    *u16_ptr++ = htons(nscnt);
    *u16_ptr++ = htons(arcnt);
}


/**
mdns_encode_dname - Encode domain name
@param[in]      qname       a domain name string, e.g. _z-wave._udp.local
@param[out]     buf         Buffer to store the encoded DNS header
@param[in,out]  buf_len     Buffer length as input; number of bytes stored in the buffer as output
@return     Return 0 on success, negative error number on failure.
*/
static int mdns_encode_dname(const char *qname, uint8_t *buf, int *buf_len)
{
    char    *dname;
    char    *next_token;
    char    *label;
    int     initial_buf_len;
    int     cur_buf_len = 0;
    int     label_len;

    dname = strdup(qname);

    if (!dname)
    {
        return -1;
    }

    initial_buf_len = *buf_len;

    while (1)
    {
        label = strtok_r((cur_buf_len == 0)? dname : NULL, ".", &next_token);

        if (label)
        {
            label_len = strlen(label);
            if (label_len > DNS_MAX_LABEL_SIZE)
            {
                free(dname);
                return -2;
            }

            if (initial_buf_len < (cur_buf_len + label_len + 1))
            {
                free(dname);
                return -3;
            }

            //copy the label
            *buf++ = label_len;
            memcpy(buf, label, label_len);

            //Adjustment
            cur_buf_len += (label_len + 1);
            buf += label_len;
        }
        else
        {   //Done
            break;
        }
    }
    *buf = 0;   //root domain
    *buf_len = cur_buf_len + 1;
    free(dname);
    return 0;
}


/**
mdns_encode_question - Encode question section
@param[in]      qname           a domain name string, e.g. _z-wave._udp.local
@param[in]      qname_offset    domain name offset relative to dns header of this message. If non-zero then qname will
                                be encoded using compression; else if zero, qname will be encoded as per normal.
@param[in]      qtype       the type of the query, DNS_TYPE_XXX
@param[out]     buf         Buffer to store the encoded DNS header
@param[in,out]  buf_len     Buffer length as input; number of bytes stored in the buffer as output
@return     Return 0 on success, negative error number on failure.
*/
static int mdns_encode_question(const char *qname, uint16_t qname_offset, uint16_t qtype, uint8_t *buf, int *buf_len)
{
    int initial_buf_len;
    int res;

    initial_buf_len = *buf_len;

    if (qname_offset)
    {
        *buf = (qname_offset >> 8) | 0xC0;
        *(buf + 1) = qname_offset & 0xFF;
        res = 0;
        *buf_len = 2;
    }
    else
    {
        res = mdns_encode_dname(qname, buf, buf_len);
    }

    if (res == 0)
    {
        if (initial_buf_len < (*buf_len + 4))
        {
            return -100;
        }

        buf += *buf_len;

        //Assign qtype
        *buf++ = (uint8_t)(qtype >> 8);
        *buf++ = qtype & 0xFF;

        //Assign qclass
        *buf++ = 0;
        *buf = DNS_CLS_IN;

        //Return number of bytes stored
        *buf_len += 4;

        return 0;
    }

    return res;
}


/**
mdns_parse_resp_hdr - Parse DNS response header
@param[in]       buf         Buffer where the DNS header is stored
@param[in]       buf_len     Buffer length
@param[out]      ancnt       number of resource records (RR) in the answer section
@param[out]      nscnt       number of name server resource records (RR) in authority records section
@param[out]      arcnt       number of resource records (RR) in the additional records section
@return     Return 0 on success, negative error number on failure.
*/
static int mdns_parse_resp_hdr(uint8_t *buf, int buf_len, uint16_t *ancnt, uint16_t *nscnt, uint16_t *arcnt)
{
    uint16_t *u16_ptr = (uint16_t *)buf;
    uint16_t flags;

    if (buf_len < DNS_PACKET_HEADER_SIZE)
    {
        return -1;
    }

    //Skip ID
    u16_ptr++;

    //Check for response message. QR must be 1, Opcode & RCode must be 0
    flags = htons(*u16_ptr);
    if ((flags & 0xF80F) != 0x8000)
    {
        return -2;
    }

    //qcount must be 0
    u16_ptr++;
    if (*u16_ptr != 0)
    {
        return -3;
    }

    //Copy other counts
    u16_ptr++;
    *ancnt = htons(*u16_ptr);

    u16_ptr++;
    *nscnt = htons(*u16_ptr);

    u16_ptr++;
    *arcnt = htons(*u16_ptr);

    return 0;
}


/**
mdns_parse_name - Parse name
@param[in]      dns_pkt     DNS packet buffer
@param[in]      buf         Buffer where the domain name is stored
@param[in,out]  buf_len     Buffer length as input; number of bytes process as output
@param[out]     dname       null terminate utf-8 domain name
@return     Return 0 on success, negative error number on failure.
@post   Caller needs to free the memory allocated to dname if this call succeeds.
*/
static int mdns_parse_name(uint8_t *dns_pkt, uint8_t *buf, int *buf_len, char **dname)
{
    int     initial_buf_len;
    int     bytes_processed;
    int     res;
    int     i;
    uint8_t *initial_buf;
    char    tmp_dname[512];
    uint8_t label_len;

    initial_buf = buf;
    initial_buf_len = *buf_len;

    //Check for invalid length
    if (initial_buf_len <= 0)
    	return -7;

    bytes_processed = i = 0;

    while (1)
    {
        label_len = *buf++;

        if (label_len == 0)
        {   //Done
            if (i == 0)
            {   //Empty domain name
                return -1;
            }

            //Replace the last dot with null character
            tmp_dname[--i] = '\0';

            bytes_processed++;

            //Allocate memory
            *dname = (char *)malloc(i + 1);
            if (*dname)
            {
                memcpy(*dname, tmp_dname, i+1);
                *buf_len = bytes_processed;
                return 0;
            }
            return -2;
        }

        if (label_len <= DNS_MAX_LABEL_SIZE)
        {   //No compression
            if (initial_buf_len < (bytes_processed + label_len))
            {
                return -3;
            }
            //Copy and add a dot
            memcpy(tmp_dname + i, buf, label_len);
            i += label_len;
            buf += label_len;
            tmp_dname[i++] = '.';
            bytes_processed += (label_len + 1);
        }
        else if ((label_len & 0xC0) == 0xC0)
        {   //Compression
            uint16_t offset;
            char     *comp_label;
            int      comp_label_len;
            int      comp_label_bytes;

/*
            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
            | 1 1 |                 OFFSET                  |
            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/

            offset = label_len & ~0xC0;
            offset = (offset << 8) | *buf++;
            //Check offset to point to a prior occurance of the same name
            if (offset >= (initial_buf - dns_pkt))
            {
                return -8;
            }

            comp_label_bytes = (initial_buf - dns_pkt) - offset;

            res = mdns_parse_name(dns_pkt, dns_pkt + offset, &comp_label_bytes, &comp_label);
            if (res != 0)
            {
                return -4;
            }
            //Copy
            comp_label_len = strlen((const char *)comp_label);
            memcpy(tmp_dname + i, comp_label, comp_label_len);
            free(comp_label);
            i += comp_label_len;
            tmp_dname[i] = '\0';
            bytes_processed += 2;

            //Allocate memory
            *dname = (char *)malloc(i + 1);
            if (*dname)
            {
                memcpy(*dname, tmp_dname, i+1);
                *buf_len = bytes_processed;
                return 0;
            }
            return -5;

        }
        else
        {
            return -6;
        }
    }
}


/**
mdns_parse_rdata - Parse resource data
@param[in]      type        Resource record (RR) type
@param[in]      dns_pkt     DNS packet buffer
@param[in]      buf         Buffer where the resource data is stored
@param[in,out]  buf_len     Buffer length as input; number of bytes process as output
@param[out]     rdata       Resource data structure specific to the type given
@return     Return 0 on success, negative error number on failure.
@post   Caller needs to free the memory allocated to rdata if this call succeeds.
*/
static int mdns_parse_rdata(uint16_t type, uint8_t *dns_pkt, uint8_t *buf, int *buf_len, void **rdata)
{
    int     res;

    //Check for invalid length
    if (*buf_len <= 0)
    	return -110;


    switch (type)
    {
        case DNS_TYPE_PTR:
            {   //resource data is domain name
                res = mdns_parse_name(dns_pkt, buf, buf_len, (char **)rdata);
            }
            break;

        case DNS_TYPE_SRV:
            {   //resource data consists of priority, weight, port and target host name
                mdns_srv_rr_t   srv_rr;
                int             bytes_processed = 0;
                uint16_t        data;

                if (*buf_len < 6)
                {
                    return -1;
                }
                //Get priority
                data = buf[bytes_processed++];
                data = (data << 8) | buf[bytes_processed++];
                srv_rr.priority = data;

                //Get weight
                data = buf[bytes_processed++];
                data = (data << 8) | buf[bytes_processed++];
                srv_rr.weight = data;

                //Get port
                data = buf[bytes_processed++];
                data = (data << 8) | buf[bytes_processed++];
                srv_rr.port = data;

                *buf_len -= 6;

                res = mdns_parse_name(dns_pkt, buf + 6, buf_len, &srv_rr.target_host);

                if (res == 0)
                {
                    *rdata = malloc(sizeof(mdns_srv_rr_t));
                    if (*rdata == NULL)
                    {
                        free(srv_rr.target_host);
                        srv_rr.target_host = NULL;
                        return -2;
                    }

                    memcpy(*rdata, &srv_rr, sizeof(mdns_srv_rr_t));

                    *buf_len += 6;
                }
            }
            break;

        case DNS_TYPE_A:
        case DNS_TYPE_AAAA:
            {   //resource data is IP address
                int ip_addr_len;

                ip_addr_len = (type == DNS_TYPE_A)? IPV4_ADDR_LEN : IPV6_ADDR_LEN;

                if (*buf_len < ip_addr_len)
                {
                    return -1;
                }

                *rdata = malloc(ip_addr_len);
                if (*rdata == NULL)
                {
                    return -2;
                }

                memcpy(*rdata, buf, ip_addr_len);

                *buf_len = ip_addr_len;

                res = 0;
            }
            break;

        default:
            //Unhandled
            res = -100;
    }

    return res;
}


/**
mdns_rr_free - Free resource record
@param[in]      mdns_rr     Resource record
@return
*/
static void mdns_rr_free(mdns_rr_t *mdns_rr)
{

    if (mdns_rr->rr_data)
    {
        //Free type specific data structure
        switch (mdns_rr->type)
        {
            case DNS_TYPE_SRV:
                {
                    mdns_srv_rr_t   *srv_rr = (mdns_srv_rr_t *)mdns_rr->rr_data;
                    free(srv_rr->target_host);
                }
                break;
        }
        free(mdns_rr->rr_data);
    }

    if (mdns_rr->name)
        free(mdns_rr->name);
}


/**
mdns_parse_answer - Parse answer section of a DNS response
@param[in]      dns_pkt     DNS packet buffer
@param[in]      buf         Buffer where the DNS answer section is stored
@param[in,out]  buf_len     Input: Buffer length; output: number of bytes process
@param[in]      ancnt       Number of resource records (RR) in the answer section
@param[out]     resrc_rec   An array of [ancnt] resource records
@return     Return 0 on success, negative error number on failure.
@post   Caller needs to free the memory allocated to resrc_rec if this call succeeds.
*/
static int mdns_parse_answer(uint8_t *dns_pkt, uint8_t *buf, int *buf_len, uint16_t ancnt, mdns_rr_t **resrc_rec)
{
    int initial_buf_len;
    int bytes_processed;
    int res;
    int i;
    int remaining_bytes;
    mdns_rr_t   *mdns_rr;
    uint16_t    type;
    uint16_t    cls;
    uint16_t    rd_len;
    uint32_t    ttl;


    if (ancnt == 0)
    {
        return -1;
    }

    mdns_rr = (mdns_rr_t *)calloc(ancnt, sizeof(mdns_rr_t));
    if (!mdns_rr)
    {
        return -2;
    }

    initial_buf_len = *buf_len;

    bytes_processed = i = 0;
    for (i=0; i<ancnt; i++)
    {
        //Get domain name
        remaining_bytes = initial_buf_len - bytes_processed;
        res = mdns_parse_name(dns_pkt, buf + bytes_processed, &remaining_bytes, &mdns_rr[i].name);
        if (res != 0)
        {
            debug_gw_discvr("mdns_parse_name failed, err=%d\n", res);
            res = -3;
            goto l_PARSE_ANS_ERR;
        }
        bytes_processed += remaining_bytes;

        if (initial_buf_len < (bytes_processed + 10))
        {   //Not enough bytes for reading type, class, ttl and rd_len
            res = -4;
            goto l_PARSE_ANS_ERR;
        }

        type = buf[bytes_processed++];
        type = (type << 8) | buf[bytes_processed++];
        mdns_rr[i].type = type;

        cls = buf[bytes_processed++];
        cls = (cls << 8) | buf[bytes_processed++];
        //For mdns, the most significant bit is used to indicate that the record is a member of a unique RRSet
        if ((cls & ~0x8000) != DNS_CLS_IN)
        {
            res = -5;
            goto l_PARSE_ANS_ERR;
        }

        ttl = buf[bytes_processed++];
        ttl = (ttl << 8) | buf[bytes_processed++];
        ttl = (ttl << 8) | buf[bytes_processed++];
        ttl = (ttl << 8) | buf[bytes_processed++];
        mdns_rr[i].ttl = ttl;

        rd_len = buf[bytes_processed++];
        rd_len = (rd_len << 8) | buf[bytes_processed++];

        if (initial_buf_len < (bytes_processed + rd_len))
        {   //Not enough bytes for reading rdata
            res = -6;
            goto l_PARSE_ANS_ERR;
        }

        remaining_bytes = rd_len;

        res = mdns_parse_rdata(type, dns_pkt, buf + bytes_processed, &remaining_bytes, &mdns_rr[i].rr_data);
        if (res != 0)
        {
            if (res != -100)
            {   //Exclude unhandle type
                debug_gw_discvr("mdns_parse_rdata failed, type=%u, err=%d\n", type, res);
            }
        }

        bytes_processed += remaining_bytes;
    }

    *resrc_rec = mdns_rr;
    return 0;

l_PARSE_ANS_ERR:
    for (i=0; i<ancnt; i++)
        mdns_rr_free(&mdns_rr[i]);
    free(mdns_rr);
    return res;
}


/**
mdns_srv_find - find service name (with host name and IP adress are empty) from resource records
@param[in]      srv_name        Service name
@param[in]      zip_host_rec    Z/IP resource records
@param[in]      zip_host_rec_cnt    Number of entries in zip_host_rec
@return pointer to the entry of Z/IP resource records if found; else return NULL
*/
static mdns_host_rec_t *mdns_srv_find(char *srv_name, mdns_host_rec_t *zip_host_rec, int zip_host_rec_cnt)
{
    int i;

    for (i=0; i<zip_host_rec_cnt; i++)
    {
        if ((strcasecmp(srv_name, zip_host_rec[i].srv_name) == 0)
            && !(zip_host_rec[i].host_name) && (zip_host_rec[i].host_ip[0] == 0))
        {
            return &zip_host_rec[i];
        }
    }
    return NULL;
}


/**
mdns_host_find - find host name (with valid service and IP adress is empty) from resource records
@param[in]      host_name       Host name
@param[in]      zip_host_rec    Z/IP resource records
@param[in]      zip_host_rec_cnt    Number of entries in zip_host_rec
@return pointer to the entry of Z/IP resource records if found; else return NULL
*/
static mdns_host_rec_t *mdns_host_find(char *host_name, mdns_host_rec_t *zip_host_rec, int zip_host_rec_cnt)
{
    int i;

    for (i=0; i<zip_host_rec_cnt; i++)
    {
        if (zip_host_rec[i].srv_name
            && zip_host_rec[i].host_name && (strcasecmp(host_name, zip_host_rec[i].host_name) == 0)
            && (zip_host_rec[i].host_ip[0] == 0))
        {
            return &zip_host_rec[i];
        }
    }
    return NULL;
}


/**
mdns_record_host_find - find host name (with valid service and IP adress) from resource records
@param[in]      host_name       Host name
@param[in]      zip_host_rec    Z/IP resource records
@param[in]      zip_host_rec_cnt    Number of entries in zip_host_rec
@return pointer to the entry of Z/IP resource records if found; else return NULL
*/
static mdns_host_rec_t *mdns_record_host_find(char *host_name, mdns_host_rec_t *zip_host_rec, int zip_host_rec_cnt)
{
    int i;

    for (i=0; i<zip_host_rec_cnt; i++)
    {
        if (zip_host_rec[i].srv_name
            && zip_host_rec[i].host_name && (strcasecmp(host_name, zip_host_rec[i].host_name) == 0)
            && (zip_host_rec[i].host_ip[0] != 0))
        {
            return &zip_host_rec[i];
        }
    }
    return NULL;
}


/**
mdns_srv_host_find - find service and host name (with IP adress is empty) from resource records
@param[in]      srv_name        Service name
@param[in]      host_name       Host name
@param[in]      zip_host_rec    Z/IP resource records
@param[in]      zip_host_rec_cnt    Number of entries in zip_host_rec
@return pointer to the entry of Z/IP resource records if found; else return NULL
*/
static mdns_host_rec_t *mdns_srv_host_find(char *srv_name, char *host_name, mdns_host_rec_t *zip_host_rec, int zip_host_rec_cnt)
{
    int i;

    for (i=0; i<zip_host_rec_cnt; i++)
    {
        if ((strcasecmp(srv_name, zip_host_rec[i].srv_name) == 0)
            && zip_host_rec[i].host_name && (strcasecmp(host_name, zip_host_rec[i].host_name) == 0)
            && (zip_host_rec[i].host_ip[0] == 0))
        {
            return &zip_host_rec[i];
        }
    }
    return NULL;
}


/**
mdns_srv_host_ip_find - find service, host name and IP adress from resource records
@param[in]      srv_name        Service name
@param[in]      host_name       Host name
@param[in]      host_ip         Host IP
@param[in]      host_ip_len     Host IP length
@param[in]      zip_host_rec    Z/IP resource records
@param[in]      zip_host_rec_cnt    Number of entries in zip_host_rec
@return pointer to the entry of Z/IP resource records if found; else return NULL
*/
static mdns_host_rec_t *mdns_srv_host_ip_find(char *srv_name, char *host_name, uint8_t *host_ip, int host_ip_len,
                                              mdns_host_rec_t *zip_host_rec, int zip_host_rec_cnt)
{
    int i;

    for (i=0; i<zip_host_rec_cnt; i++)
    {
        if ((strcasecmp(srv_name, zip_host_rec[i].srv_name) == 0)
            && zip_host_rec[i].host_name && (strcasecmp(host_name, zip_host_rec[i].host_name) == 0)
            && (memcmp(zip_host_rec[i].host_ip, host_ip, host_ip_len) == 0))
        {
            return &zip_host_rec[i];
        }
    }
    return NULL;
}


/**
mdns_srv_save - save service name into resource records
@param[in]      srv_name        Service name
@param[out]     zip_host_rec    Buffer to store the service name selected for query
@param[in,out]  zip_host_rec_cnt    Input: number of entries in zip_host_rec; output: new number of entries
@param[in]      check_dup       Flag to determine whether duplicate checking is required
@return Pointer to the save record on success; else return NULL
*/
static mdns_host_rec_t *mdns_srv_save(char *srv_name, mdns_host_rec_t **zip_host_rec, int *zip_host_rec_cnt, int check_dup)
{
    mdns_host_rec_t     *tmp_host_rec;
    mdns_host_rec_t     *host_rec;
    int                 host_rec_cnt;

    host_rec = *zip_host_rec;
    host_rec_cnt = *zip_host_rec_cnt;

    if (check_dup && host_rec && (host_rec_cnt > 0))
    {
        tmp_host_rec = mdns_srv_find(srv_name, host_rec, host_rec_cnt);

        if (tmp_host_rec)
        {   //Found existing record, do not save
            return tmp_host_rec;
        }
    }

    //Create a new record
    if (host_rec)
    {
        tmp_host_rec = (mdns_host_rec_t *)realloc(host_rec, (host_rec_cnt + 1)*sizeof(mdns_host_rec_t));
    }
    else
    {
        tmp_host_rec = (mdns_host_rec_t *)malloc(sizeof(mdns_host_rec_t));
    }

    if (tmp_host_rec)
    {
        //Zero the newly allocated entry
        memset(&tmp_host_rec[host_rec_cnt], 0, sizeof(mdns_host_rec_t));
        tmp_host_rec[host_rec_cnt].srv_name = strdup(srv_name);

        //Return output
        *zip_host_rec = tmp_host_rec;
        *zip_host_rec_cnt = host_rec_cnt + 1;

        return &tmp_host_rec[host_rec_cnt];
    }

    return NULL;
}


/**
mdns_srv_host_save - save service and host name into resource records
@param[in]      srv_name        Service name
@param[in]      host_name       Host name
@param[out]     zip_host_rec    Buffer to store the service name selected for query
@param[in,out]  zip_host_rec_cnt    Input: number of entries in zip_host_rec; output: new number of entries
@param[in]      check_dup       Flag to determine whether duplicate checking is required
@return Pointer to the save record on success; else return NULL
*/
static mdns_host_rec_t *mdns_srv_host_save(char *srv_name, char *host_name, mdns_host_rec_t **zip_host_rec, int *zip_host_rec_cnt, int check_dup)
{
    mdns_host_rec_t     *tmp_host_rec;
    mdns_host_rec_t     *host_rec;
    int                 host_rec_cnt;

    host_rec = *zip_host_rec;
    host_rec_cnt = *zip_host_rec_cnt;

    if (check_dup && host_rec && (host_rec_cnt > 0))
    {
        tmp_host_rec = mdns_srv_host_find(srv_name, host_name, host_rec, host_rec_cnt);

        if (tmp_host_rec)
        {   //Found existing record, do not save
            return tmp_host_rec;
        }
    }

    tmp_host_rec = mdns_srv_save(srv_name, zip_host_rec, zip_host_rec_cnt, check_dup);

    if (tmp_host_rec)
    {
        tmp_host_rec->host_name = strdup(host_name);

        if (tmp_host_rec->host_name)
        {
            return tmp_host_rec;
        }
    }

    return NULL;
}


/**
mdns_srv_host_ip_save - save service, host name and IP address into resource records
@param[in]      srv_name        Service name
@param[in]      host_name       Host name
@param[in]	    use_ipv4	    Flag to indicate whether to use IPv4 as transport IP protocol. 1= use IPv4; 0= use IPv6
@param[out]     host_ip_buf     Buffer to store the output IP address
@param[out]     zip_host_rec    Buffer to store the service name selected for query
@param[in,out]  zip_host_rec_cnt    Input: number of entries in zip_host_rec; output: new number of entries
@return Pointer to the save record on success; else return NULL
*/
static mdns_host_rec_t *mdns_srv_host_ip_save(char *srv_name, char *host_name, int use_ipv4, uint8_t *host_ip_buf, mdns_host_rec_t **zip_host_rec, int *zip_host_rec_cnt)
{
    mdns_host_rec_t     *tmp_host_rec;
    mdns_host_rec_t     *host_rec;
    int                 host_rec_cnt;
    int                 ip_addr_len;

    ip_addr_len = (use_ipv4)? IPV4_ADDR_LEN : IPV6_ADDR_LEN;

    host_rec = *zip_host_rec;
    host_rec_cnt = *zip_host_rec_cnt;

    //Check for duplicated existing record
    if (host_rec && (host_rec_cnt > 0))
    {
        tmp_host_rec = mdns_srv_host_ip_find(srv_name, host_name, host_ip_buf, ip_addr_len, host_rec, host_rec_cnt);

        if (tmp_host_rec)
        {   //Found
            return tmp_host_rec;
        }
    }

    tmp_host_rec = mdns_srv_host_save(srv_name, host_name, zip_host_rec, zip_host_rec_cnt, 1);

    if (tmp_host_rec)
    {
        memcpy(tmp_host_rec->host_ip, host_ip_buf, ip_addr_len);
        return tmp_host_rec;
    }

    return NULL;
}


/**
mdns_tgt_host_get - get target host name from DNS_TYPE_SRV
@param[in]      sock_fd         Socket file descriptor for sending DNS query
@param[in]      dname           Domain name to search
@param[in]      ancnt           Number of resource records in rsrc_rec
@param[in]      rsrc_rec        Resource records
@param[in]	    use_ipv4	    Flag to indicate whether to use IPv4 as transport IP protocol. 1= use IPv4; 0= use IPv6
@param[out]     tgt_host        Target host name if success
@return   Return 0 on success; 1 on MDNS question has been sent;  negative error number on failure.
@post     Caller must free the memory allocated to tgt_host if this call is successful
*/
static int mdns_tgt_host_get(int sock_fd, const char *dname, uint16_t ancnt, mdns_rr_t *rsrc_rec,
                            int use_ipv4, char **tgt_host)
{
    ssize_t     bytes_sent;
    int         res;
    int         i;
    int         free_buf_sz;
    int         bytes_processed;
    int         dname_offset;   //Point to the first occurance of domain name. It is used for name compression.
    uint8_t     mdns_buf[512];
    uint16_t    qdcnt;

    bytes_processed = qdcnt = 0;

    for (i=0; i<ancnt; i++)
    {
        if ((rsrc_rec[i].type == DNS_TYPE_SRV)
            && (strcasecmp((const char *)rsrc_rec[i].name, dname) == 0)
            && rsrc_rec[i].rr_data)
        {
            mdns_srv_rr_t   *srv_rr;

            srv_rr = (mdns_srv_rr_t *)rsrc_rec[i].rr_data;

            if (srv_rr->target_host)
            {
                *tgt_host = strdup(srv_rr->target_host);

                if (*tgt_host)
                {
                    return 0;
                }
                break;
            }
        }
    }

    //
    //Get service info through MDNS
    //
    dname_offset = DNS_PACKET_HEADER_SIZE + bytes_processed;
    free_buf_sz = 512 - dname_offset;

    res = mdns_encode_question(dname, 0, DNS_TYPE_SRV,
                               mdns_buf + dname_offset, &free_buf_sz);
    if (res != 0)
    {
        return res;
    }

    //Adjustment
    bytes_processed += free_buf_sz;
    qdcnt++;

#ifdef  MDNS_GET_ZWAVE_RESOURCE_INFO
    //
    //Get Z-wave resource info from DNS_TYPE_TXT
    //
    free_buf_sz = 512 - DNS_PACKET_HEADER_SIZE - bytes_processed;

    res = mdns_encode_question(dname, dname_offset, DNS_TYPE_TXT,
                               mdns_buf + DNS_PACKET_HEADER_SIZE + bytes_processed, &free_buf_sz);

    if (res != 0)
    {
        return res;
    }

    //Adjustment
    bytes_processed += free_buf_sz;
    qdcnt++;
#endif


    if (qdcnt > 0)
    {
        //Prepare the mdns query
        mdns_encode_hdr(0, 0, qdcnt, 0, 0, 0, mdns_buf);

        if (use_ipv4)
        {
            bytes_sent = mdns_mcast4(sock_fd, mdns_buf, DNS_PACKET_HEADER_SIZE + bytes_processed);
        }
        else    //IPv6
        {
            bytes_sent = mdns_mcast6(sock_fd, mdns_buf, DNS_PACKET_HEADER_SIZE + bytes_processed);
        }

        if (bytes_sent == (DNS_PACKET_HEADER_SIZE + bytes_processed))
        {
            return 1;
        }
        return -20;
    }

    return -30;
}


/**
mdns_host_ip_get - get host IP address from DNS_TYPE_A or DNS_TYPE_AAAA
@param[in]      sock_fd         Socket file descriptor for sending DNS query
@param[in]      host_name       Host name, e.g. zwDFCEDBEF01.local
@param[in]      ancnt           Number of answers in resource records
@param[in]      rsrc_rec        Resource records
@param[in]	    use_ipv4	    Flag to indicate whether to use IPv4 as transport IP protocol. 1= use IPv4; 0= use IPv6
@param[out]     host_ip_buf     Buffer to store the output IP address
@return   Return 0 on success; 1 on MDNS question has been sent;  negative error number on failure.
*/
static int mdns_host_ip_get(int sock_fd, const char *host_name, uint16_t ancnt, mdns_rr_t *rsrc_rec,
                            int use_ipv4, uint8_t *host_ip_buf)
{
    ssize_t             bytes_sent;
    int                 res;
    int                 i;
    int                 free_buf_sz;
    int                 bytes_processed;
    int                 free_buf_offset;
    int                 ip_addr_len;
    uint8_t             mdns_buf[512];
    uint16_t            qdcnt;
    uint16_t            dns_type;

    if (use_ipv4)
    {
        ip_addr_len = IPV4_ADDR_LEN;
        dns_type = DNS_TYPE_A;
    }
    else
    {
        ip_addr_len = IPV6_ADDR_LEN;
        dns_type = DNS_TYPE_AAAA;
    }

    bytes_processed = qdcnt = 0;

    for (i=0; i<ancnt; i++)
    {
        if ((rsrc_rec[i].type == dns_type)
            && (strcasecmp((const char *)rsrc_rec[i].name, host_name) == 0)
            && rsrc_rec[i].rr_data)
        {
            memcpy(host_ip_buf, rsrc_rec[i].rr_data, ip_addr_len);
            return 0;
        }
    }

    //Get host IP through MDNS
    free_buf_offset = DNS_PACKET_HEADER_SIZE + bytes_processed;
    free_buf_sz = 512 - free_buf_offset;

    res = mdns_encode_question(host_name, 0, dns_type,
                               mdns_buf + free_buf_offset, &free_buf_sz);
    if (res != 0)
    {
        return res;
    }

    //Adjustment
    bytes_processed += free_buf_sz;
    qdcnt++;

    if (qdcnt > 0)
    {
        //Prepare the mdns query
        mdns_encode_hdr(0, 0, qdcnt, 0, 0, 0, mdns_buf);

        if (use_ipv4)
        {
            bytes_sent = mdns_mcast4(sock_fd, mdns_buf, DNS_PACKET_HEADER_SIZE + bytes_processed);
        }
        else    //IPv6
        {
            bytes_sent = mdns_mcast6(sock_fd, mdns_buf, DNS_PACKET_HEADER_SIZE + bytes_processed);
        }

        if (bytes_sent == (DNS_PACKET_HEADER_SIZE + bytes_processed))
        {
            return 1;
        }
        return -20;
    }

    return -30;
}


/**
mdns_mcast4_setup - create a network socket and set it up to join mdns link-local multicast address 224.0.0.251
@param[in]      host_ip     Host IP from which the network corresponding to the host IP message is broadcasted.
@param[out]     sfd         Socket handler which can be used to received response to the multicast message
@return                     Return 0 on success, negative error number on failure.
@post           Caller has to close the socket handler (sfd) if the return value is 0 (success)
*/
#if defined(_WINDOWS) || defined(WIN32)
static int mdns_mcast4_setup(struct sockaddr_in *host_ip, SOCKET *sfd)
#else
static int mdns_mcast4_setup(struct sockaddr_in *host_ip, int *sfd)
#endif
{
    #if defined(_WINDOWS) || defined(WIN32)
    SOCKET              sock_fd;
    BOOL                opt_yes = TRUE;
    BOOL                opt_no = FALSE;
    #else
    int                 sock_fd;
    int                 opt_yes = 1;
    int                 opt_no = 0;
    #endif
    struct ip_mreq      mreq;
    int                 res;

    //Open socket and bind it to the host ip address
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0)
    {
        return -1;
    }

    // Enable reuse of address and port
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt_yes, sizeof(opt_yes)) < 0)
    {
        res = -2;
        goto l_IPV4_MCAST_ERR;
    }
#ifdef OS_MAC_X
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, (const char *)&opt_yes, sizeof(opt_yes)) < 0)
    {
        res = -2;
        goto l_IPV4_MCAST_ERR;
    }
#endif

    // Disable multicast loop back
    if (setsockopt(sock_fd, IPPROTO_IP, IP_MULTICAST_LOOP, (const char *)&opt_no, sizeof(opt_no)) < 0)
    {
        res = -3;
        goto l_IPV4_MCAST_ERR;
    }

    //Bind to listening address
    host_ip->sin_family = AF_INET;
    host_ip->sin_port = htons(MDNS_PORT);

    // Set multicast source address
    if (setsockopt(sock_fd, IPPROTO_IP, IP_MULTICAST_IF, (const char *)&host_ip->sin_addr, sizeof(struct in_addr)) < 0)
    {
        res = -4;
        goto l_IPV4_MCAST_ERR;
    }

    // Join the multicast group
    memset(&mreq, 0, sizeof(mreq));
    mreq.imr_interface = host_ip->sin_addr; /* local IP address of interface */
    memcpy(&mreq.imr_multiaddr.s_addr, ipv4_mdns_mcast_addr, IPV4_ADDR_LEN);/* IP multicast address of group */

    /* Some network drivers have issues with dropping membership of
     * mcast groups when the iface is down, but don't allow rejoining
     * when it comes back up. This is an ugly workaround */
//  if (join)
//      setsockopt(fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));

    if (setsockopt(sock_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char *)&mreq, sizeof(mreq)) < 0)
    {
        res = -5;
        goto l_IPV4_MCAST_ERR;
    }
    debug_gw_discvr("Join ok. sending IP:%s \n", inet_ntoa(mreq.imr_interface));
    debug_gw_discvr("mcast IP:%s \n", inet_ntoa(mreq.imr_multiaddr));


    //For binding, specify the IP address as INADDR_ANY in order to receive datagrams that are addressed to a multicast group
    host_ip->sin_addr.s_addr = INADDR_ANY;

    res = bind(sock_fd, (const struct sockaddr *)host_ip, sizeof(struct sockaddr_in));

    if (res == 0)
    {
        *sfd = sock_fd;
        return 0;
    }
    else
    {
        debug_gw_discvr("Bind failed\n");
        res = -6;
    }

l_IPV4_MCAST_ERR:
#if defined(_WINDOWS) || defined(WIN32)
    closesocket(sock_fd);
#else
    close(sock_fd);
#endif
    return res;
}


/**
mdns_mcast6_setup - create a network socket and set it up to join mdns link-local multicast address FF02::FB
@param[in]      host_if     Host IP interface index
@param[out]     sfd         Socket handler which can be used to received response to the multicast message
@return                     Return 0 on success, negative error number on failure.
@post           Caller has to close the socket handler (sfd) if the return value is 0 (success)
*/
#if defined(_WINDOWS) || defined(WIN32)
static int mdns_mcast6_setup(uint32_t host_if, SOCKET *sfd)
#else
static int mdns_mcast6_setup(uint32_t host_if, int *sfd)
#endif
{

    #if defined(_WINDOWS) || defined(WIN32)
    SOCKET              sock_fd;
    BOOL                opt_yes = TRUE;
    BOOL                opt_no = FALSE;
    DWORD               addr_str_len;
    #else
    int                 sock_fd;
    int                 opt_yes = 1;
    int                 opt_no = 0;
    #endif
    int                 res;
    struct ipv6_mreq    mreq6;
    struct sockaddr_in6 sa_multicast;

    //Open socket and bind it to the host ip address
    sock_fd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sock_fd < 0)
    {
        return -1;
    }

//  ttl = 255;
//  if (setsockopt(fd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &ttl, sizeof(ttl)) < 0) {
//      avahi_log_warn("IPV6_MULTICAST_HOPS failed: %s", strerror(errno));
//      goto fail;
//  }
//
//  ttl = 255;
//  if (setsockopt(fd, IPPROTO_IPV6, IPV6_UNICAST_HOPS, &ttl, sizeof(ttl)) < 0) {
//      avahi_log_warn("IPV6_UNICAST_HOPS failed: %s", strerror(errno));
//      goto fail;
//  }

    // Enable reuse of address and port
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt_yes, sizeof(opt_yes)) < 0)
    {
        res = -2;
        goto l_IPV6_MCAST_ERR;
    }

#ifdef OS_MAC_X
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, (const char *)&opt_yes, sizeof(opt_yes)) < 0)
    {
        res = -2;
        goto l_IPV6_MCAST_ERR;
    }
#endif

    // Disable multicast loop back
    if (setsockopt(sock_fd, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, (const char *)&opt_no, sizeof(opt_no)) < 0)
    {
        res = -3;
        goto l_IPV6_MCAST_ERR;
    }

    // Disable IPv4 mapped addresses for security reason
    if (setsockopt(sock_fd, IPPROTO_IPV6, IPV6_V6ONLY, (const char *)&opt_yes, sizeof(opt_yes)) < 0)
    {
        res = -4;
        goto l_IPV6_MCAST_ERR;
    }

    // Set multicast source address interface
    if (setsockopt(sock_fd, IPPROTO_IPV6, IPV6_MULTICAST_IF, (const char *)&host_if, sizeof(uint32_t)) < 0)
    {
        res = -5;
        goto l_IPV6_MCAST_ERR;
    }

    // Join the multicast group
    memset(&mreq6, 0, sizeof(mreq6));
    memcpy(mreq6.ipv6mr_multiaddr.s6_addr, ipv6_mdns_mcast_addr, IPV6_ADDR_LEN);
    mreq6.ipv6mr_interface = host_if;

//  if (join)
//      setsockopt(fd, IPPROTO_IPV6, IPV6_DROP_MEMBERSHIP, &mreq6, sizeof(mreq6));

    if (setsockopt(sock_fd, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, &mreq6, sizeof(mreq6)) < 0)
    {
        res = -6;
        goto l_IPV6_MCAST_ERR;
    }

    //Bind to listening address
    sa_multicast.sin6_family = AF_INET6;
    sa_multicast.sin6_port = htons(MDNS_PORT);
    sa_multicast.sin6_flowinfo = 0;
    sa_multicast.sin6_scope_id = 0;

    debug_gw_discvr("Join ok. Multicast interface index: %u\n", host_if);

    //For binding, specify the IP address as in6addr_any in order to receive datagrams that are addressed to a multicast group
    sa_multicast.sin6_addr = in6addr_any;

    res = bind(sock_fd, (const struct sockaddr *)&sa_multicast, sizeof(struct sockaddr_in6));

    if (res == 0)
    {
        *sfd = sock_fd;
        return 0;
    }
    else
    {
        debug_gw_discvr("Bind failed\n");
        res = -7;
    }

l_IPV6_MCAST_ERR:
#if defined(_WINDOWS) || defined(WIN32)
    closesocket(sock_fd);
#else
    close(sock_fd);
#endif
    return res;
}


/**
mdns_gw_discvr_wait_resp - Wait for gateway discovery response
@param[in]	sfd		        Socket handler
@param[in]	gw_ip_buf_sz	The number of gateway addresses could be stored in the gw_ip
@param[in,out]	gw_ip	    (Input) The sin_family member indicates which IP family to use. (Output) Gateway IP addresses
@param[out]	gw_name	        Gateway names corresponding to the gw_ip
@return The number of gateway IP addresses found
*/
//#if defined(_WINDOWS) || defined(WIN32)
//static int mdns_gw_discvr_wait_resp(SOCKET sfd, uint8_t gw_ip_buf_sz, struct sockaddr *gw_ip)
//#else
static int mdns_gw_discvr_wait_resp(int sfd, uint8_t gw_ip_buf_sz, struct sockaddr *gw_ip, char **gw_name)
//#endif
{
    int                 rcx_size;
    int                 n;
    int                 rv;
    int                 use_ipv4;
    int                 res;
    fd_set              readfds;
    struct timeval      tv;
    struct sockaddr     *sock_addr;
    struct sockaddr_in6 *gw_ip6 = NULL;
    struct sockaddr_in  *gw_ip4 = NULL;
    struct sockaddr_in6 sa6;
    struct sockaddr_in  sa4;
    socklen_t           sock_len;
    char                *tgt_host_name;
    mdns_rr_t           *rsrc_rec;
    mdns_host_rec_t     *tmp_host_rec;
    mdns_host_rec_t     *zip_host_rec = NULL;
    util_tmr_tick_t     *tmr_tick_ctx;
    int                 zip_host_rec_cnt = 0;
    int                 resp_body_len;
    int                 i;
    int                 j;
    uint32_t            timer_tick_tmout;
    uint16_t            dns_type;
    uint16_t            ancnt, nscnt, arcnt;
    uint8_t             buf[MAX_MULTICAST_RCX_SZ];
    uint8_t             host_ip_buf[IPV6_ADDR_LEN];
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

    //Start timer to set upper limit for waiting of mdns responses
    //i.e. all responses after this timeout will be discarded
    if (util_tmr_tick_init(100, &tmr_tick_ctx) < 0)
    {
        return 0;
    }

    timer_tick_tmout = (MDNS_DISCVR_TMOUT / 100) + 1;

    while ((idx < gw_ip_buf_sz) && (tmr_tick_ctx->tick <= timer_tick_tmout))
    {
        // clear the set ahead of time
        FD_ZERO(&readfds);

        // add our descriptors to the set
        FD_SET(sfd, &readfds);

        // the n param in select()
        n = sfd + 1;

        // wait until either socket has data ready to be received or timeout
        tv.tv_sec = MDNS_DISCVR_TMOUT / 1000;
        tv.tv_usec = (MDNS_DISCVR_TMOUT % 1000) * 1000;

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
        else if (FD_ISSET(sfd, &readfds))
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

            sock_addr->sa_family = 0;   //Just to make compiler happy!
            rcx_size = recvfrom(sfd, buf, MAX_MULTICAST_RCX_SZ, 0, sock_addr, &sock_len);

            if (rcx_size > 0)
            {
                //Check for IP version mismatch
                if (use_ipv4)
                {
                    if (sock_addr->sa_family != AF_INET)
                    {
                        continue;
                    }
                    debug_gw_discvr("Rcv from host IP:%s\n", inet_ntoa(sa4.sin_addr));
                }
                else
                {
                    if (sock_addr->sa_family != AF_INET6)
                    {
                        continue;
                    }
                }

                res = mdns_parse_resp_hdr(buf, rcx_size, &ancnt, &nscnt, &arcnt);

                if (res != 0)
                {
                    continue;
                }

                //debug_gw_discvr("received mdns response with %u answers\n", ancnt);
                resp_body_len = rcx_size - DNS_PACKET_HEADER_SIZE;
                res = mdns_parse_answer(buf, buf + DNS_PACKET_HEADER_SIZE, &resp_body_len, ancnt, &rsrc_rec);

                if (res != 0)
                {
                    debug_gw_discvr("mdns_parse_answer with err: %d\n", res);
                    continue;
                }

                //debug_gw_discvr("mdns response %d bytes were processed\n", resp_body_len);
//                              for (i=0; i<ancnt; i++)
//                              {
//                                  debug_gw_discvr("----------------- %d ------------------\n", i);
//                                  debug_gw_discvr("Response name:%s, type:%d\n", rsrc_rec[i].name, rsrc_rec[i].type);
//                                  if (rsrc_rec[i].type == DNS_TYPE_PTR)
//                                  {
//                                      debug_gw_discvr("PTR answer:%s\n", (char *)rsrc_rec[i].rr_data);
//                                  }
//                              }
                //Find domain name that contains "Static Controller"
                for (i=0; i<ancnt; i++)
                {
                    if ((rsrc_rec[i].type == DNS_TYPE_PTR)
                        && (strcasecmp((const char *)rsrc_rec[i].name, ZIP_RESOURCE_DOMAIN) == 0)
                        && rsrc_rec[i].rr_data
                        && strcasestr(rsrc_rec[i].rr_data, "Static Controller"))
                    {
                        res = mdns_tgt_host_get(sfd, rsrc_rec[i].rr_data, ancnt, rsrc_rec, use_ipv4, &tgt_host_name);

                        if (res == 0)
                        {
                            int result;

                            result = mdns_host_ip_get(sfd, tgt_host_name, ancnt, rsrc_rec, use_ipv4, host_ip_buf);

                            if (result == 0)
                            {   //Everything is in the answer records.  Save the full record.
                                mdns_srv_host_ip_save(rsrc_rec[i].rr_data, tgt_host_name, use_ipv4, host_ip_buf,
                                                      &zip_host_rec, &zip_host_rec_cnt);

                            }
                            else if (result == 1)
                            {   //Save the domain (service) and host name for later MDNS answer
                                mdns_srv_host_save(rsrc_rec[i].rr_data, tgt_host_name, &zip_host_rec, &zip_host_rec_cnt, 0);
                            }
                            else
                            {
                                debug_gw_discvr("mdns_host_ip_get with err:%d\n", result);
                            }

                            free(tgt_host_name);
                        }
                        else if (res == 1)
                        {   //Save the domain (service) name for later MDNS answer
                            mdns_srv_save(rsrc_rec[i].rr_data, &zip_host_rec, &zip_host_rec_cnt, 0);
                        }
                        else
                        {
                            debug_gw_discvr("mdns_tgt_host_get with err:%d\n", res);
                        }
                    }
                }

                //Find target host name from service record
                for (i=0; i<ancnt; i++)
                {
                    if ((rsrc_rec[i].type == DNS_TYPE_SRV)
                        && rsrc_rec[i].name
                        && rsrc_rec[i].rr_data
                        && zip_host_rec
                        && (zip_host_rec_cnt > 0))
                    {
                        //Find record with only service name is valid but target host and IP address are invalid.
                        //Only this type of record is useful because the MDNS answer received is able to fill in the
                        //missing target host name.
                        tmp_host_rec = mdns_srv_find(rsrc_rec[i].name, zip_host_rec, zip_host_rec_cnt);

                        if (tmp_host_rec)
                        {   //Found
                            int             result;
                            mdns_srv_rr_t   *srv_rr = (mdns_srv_rr_t *)rsrc_rec[i].rr_data;


                            result = mdns_host_ip_get(sfd, srv_rr->target_host, ancnt, rsrc_rec, use_ipv4, host_ip_buf);

                            if (result == 0)
                            {   //Everything is in the answer records.  Save the full record.
                                mdns_srv_host_ip_save(rsrc_rec[i].name, srv_rr->target_host, use_ipv4, host_ip_buf,
                                                      &zip_host_rec, &zip_host_rec_cnt);

                            }
                            else if (result == 1)
                            {   //Save the domain (service) and host name for later MDNS answer
                                mdns_srv_host_save(rsrc_rec[i].name, srv_rr->target_host, &zip_host_rec, &zip_host_rec_cnt, 0);
                            }
                            else
                            {
                                debug_gw_discvr("mdns_host_ip_get with err:%d\n", result);
                            }
                        }
                    }
                }

                //Find IP address

                dns_type = (use_ipv4)? DNS_TYPE_A : DNS_TYPE_AAAA;

                for (i=0; i<ancnt; i++)
                {
                    if ((rsrc_rec[i].type == dns_type)
                        && rsrc_rec[i].name
                        && rsrc_rec[i].rr_data
                        && zip_host_rec
                        && (zip_host_rec_cnt > 0))
                    {
                        //Find record with only service name and host name are valid but IP address is invalid.
                        //Only this type of record is useful because the MDNS answer received is able to fill in the
                        //missing IP address.
                        tmp_host_rec = mdns_host_find(rsrc_rec[i].name, zip_host_rec, zip_host_rec_cnt);

                        if (!tmp_host_rec)
                        {   //Try to save a unique service, host name and IP combination
                            tmp_host_rec = mdns_record_host_find(rsrc_rec[i].name, zip_host_rec, zip_host_rec_cnt);
                        }

                        if (tmp_host_rec)
                        {   //Found
                            //Everything is in the answer records.  Save the full record.
                            mdns_srv_host_ip_save(tmp_host_rec->srv_name, rsrc_rec[i].name, use_ipv4, rsrc_rec[i].rr_data,
                                                  &zip_host_rec, &zip_host_rec_cnt);

                        }
                    }
                }

                //Free resource records
                for (i=0; i<ancnt; i++)
                    mdns_rr_free(rsrc_rec + i);
                free(rsrc_rec);
            }
            else if (rcx_size == TPT_SOCKET_ERR)
            {
                //debug_gw_discvr("udp rcx error:%d", WSAGetLastError());
                debug_gw_discvr("udp rcx error\n");
                break;
            }
        }
    }

    util_tmr_tick_exit(tmr_tick_ctx);

    if (zip_host_rec_cnt > 0)
    {
        for (j=i=0; i<zip_host_rec_cnt; i++)
        {
            if ((zip_host_rec[i].host_ip[0] != 0) && (j < gw_ip_buf_sz))
            {
                if (use_ipv4)
                {
                    memcpy(&sa4.sin_addr.s_addr, zip_host_rec[i].host_ip, IPV4_ADDR_LEN);
                    gw_ip4[j] = sa4;
                }
                else
                {
                    memcpy(sa6.sin6_addr.s6_addr, zip_host_rec[i].host_ip, IPV6_ADDR_LEN);
                    gw_ip6[j] = sa6;
                }
                gw_name[j++] = strdup(zip_host_rec[i].srv_name);

            }
            //Free host resource record
            free(zip_host_rec[i].srv_name);
            free(zip_host_rec[i].host_name);
        }

        free(zip_host_rec);
        //return value
        idx = j;
    }

    return idx;
}


/**
mdns_nw_intf_get - Get host network interfaces
@param[in]	host_ipv6_addr	Buffer that stores host IPv6 addresses
@param[in]	ip_cnt	        The number of host IPv6 addresses stored in host_ipv6_addr
@param[in]	max_intf	    The maximum number of network interfaces that could be stored in intf_idx
@param[out]	intf_idx	    Buffer to store the network interfaces
@return The number of host network interfaces
*/
int mdns_nw_intf_get(util_ipv6_addr_t *host_ipv6_addr, int ip_cnt, int max_intf, uint32_t *intf_idx)
{
    int i;
    int j;
    int found;
    int nw_intf = 0;

    for (i=0; i<ip_cnt; i++)
    {
        found = 0;
        for (j=0; j<nw_intf; j++)
        {
            if (intf_idx[j] == host_ipv6_addr[i].if_idx)
            {
                found = 1;
                break;
            }
        }

        if (!found && (nw_intf < max_intf))
        {
            intf_idx[nw_intf++] = host_ipv6_addr[i].if_idx;
        }
    }

    return nw_intf;
}


/**
mdns_gw_discvr_thrd - thread to perform gateway discovery
@param[in]	data    Context
@return
*/
static void mdns_gw_discvr_thrd(void *data)
{
    mdns_discvr_ctx_t *ctx = (mdns_discvr_ctx_t *)data;

#if defined(_WINDOWS) || defined(WIN32)
    WSADATA             wsa_dat;
    SOCKET              sock_fd;
#else
    int                 sock_fd;
#endif
    int                 result;
    int                 i;
    uint32_t            host_intf_idx[MAX_NW_INTF];
    int                 host_intf_cnt;       //Total number of host network interfaces
    uint8_t             gw_ip_addr[MAX_DISCOVERY_GW * IPV6_ADDR_LEN];   //IP address buffer for callback
    uint8_t             host_ipv4_addr[MAX_DISCOVERY_GW * IPV4_ADDR_LEN];
    util_ipv6_addr_t    host_ipv6_addr[MAX_DISCOVERY_GW];
    char                *gw_name[MAX_DISCOVERY_GW]; //Gateway name buffer for callback
    int                 host_ip_cnt;        //Total number of host IPv4 addresses
    int                 host_ip_idx = 0;    //Read index to the host IPv4 addresses
    struct sockaddr_in  sa4;
    struct sockaddr_in  gw_ip4[MAX_DISCOVERY_GW];
//    struct sockaddr_in6 sa6;
    struct sockaddr_in6 gw_ip6[MAX_DISCOVERY_GW];
    struct sockaddr     *gw_ip;
    uint8_t             gw_ip_addr_cnt;
    uint8_t             mdns_buf[200];
    int                 mdns_packet_sz;


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


    //Prepare the mdns query
    mdns_encode_hdr(0, 0, 1, 0, 0, 0, mdns_buf);
    mdns_packet_sz = 200 - DNS_PACKET_HEADER_SIZE;

    result = mdns_encode_question(ZIP_RESOURCE_DOMAIN, 0, DNS_TYPE_PTR, mdns_buf + DNS_PACKET_HEADER_SIZE, &mdns_packet_sz);
    if (result != 0)
    {
        debug_gw_discvr("Create mdns query failed with error %d", result);
    #if defined(_WINDOWS) || defined(WIN32)
        WSACleanup(); //Clean up Winsock
    #endif
        ctx->gw_thrd_sts = 0;
        return;
    }

    //Adjust mdns packet size
    mdns_packet_sz += DNS_PACKET_HEADER_SIZE;


    if (ctx->use_ipv4)
    {
        host_ip_cnt = util_ip_addr_get(MAX_DISCOVERY_GW, host_ipv4_addr, 1);
    }
    else
    {
        host_ip_cnt = util_ip_addr_get(MAX_DISCOVERY_GW, host_ipv6_addr, 0);
        host_intf_cnt = mdns_nw_intf_get(host_ipv6_addr, host_ip_cnt, MAX_NW_INTF, host_intf_idx);
        host_ip_cnt = host_intf_cnt;    //due to the loop using host_ip_cnt variable
        debug_gw_discvr("Found %d network interfaces\n", host_intf_cnt);
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

                result = mdns_mcast4_setup(&sa4, &sock_fd);
                if (result == 0)
                {
                    if (mdns_mcast4(sock_fd, mdns_buf, mdns_packet_sz) < 0)
                    {
                        result = -1;
                    }
                }
            }
            else
            {
                gw_ip6[0].sin6_family = AF_INET6;
                gw_ip = (struct sockaddr *)gw_ip6;
//              memcpy(sa6.sin6_addr.s6_addr, host_ipv6_addr[host_ip_idx].addr, IPV6_ADDR_LEN);
//              result = mdns_mcast6_setup(&sa6, host_ipv6_addr[host_ip_idx].if_idx, mdns_buf, mdns_packet_sz, &sock_fd);
                result = mdns_mcast6_setup(host_intf_idx[host_ip_idx], &sock_fd);
                if (result == 0)
                {
                    if (mdns_mcast6(sock_fd, mdns_buf, mdns_packet_sz) < 0)
                    {
                        result = -1;
                    }
                }
            }

            host_ip_idx++;
            gw_ip_addr_cnt = 0;
            memset(gw_name, 0, sizeof(gw_name));

            if (result == 0)
            {
                //Get responses
                result = mdns_gw_discvr_wait_resp(sock_fd, MAX_DISCOVERY_GW, gw_ip, gw_name);

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
                ctx->cb(gw_ip_addr, gw_ip_addr_cnt, ctx->use_ipv4, ctx->usr_param, host_ip_idx, host_ip_cnt, gw_name);
            }

            //Free gateway names
            for (i=0; i<MAX_DISCOVERY_GW; i++)
            {
                free(gw_name[i]);
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
mdns_gw_discvr_start - Start Z/IP gateway discovery using mdns
@param[in]	    cb		    Callback function when the gateway discovery has completed
@param[in]	    usr_param	User defined parameter used in callback function
@param[in]	    use_ipv4	Flag to indicate whether to use IPv4 as transport IP protocol. 1= use IPv4; 0= use IPv6
@return Context on success, null on failure.
@post   Caller is required to call mdns_gw_discvr_stop() with the returned context if it is not null.
*/
void *mdns_gw_discvr_start(mdns_gw_discvr_cb_t cb, void *usr_param, int use_ipv4)
{
    int               wait_cnt;
    mdns_discvr_ctx_t *ctx;

    ctx = (mdns_discvr_ctx_t *)calloc(1, sizeof(mdns_discvr_ctx_t));

    if (!ctx)
    {
        return NULL;
    }

    //Insert context id
    ctx->ctx_id = MDNS_CTX_ID;

    //Save the callback function
    ctx->cb = cb;
    ctx->usr_param = usr_param;

    ctx->use_ipv4 = use_ipv4;

    //Start Z/IP gateway discovery thread
    ctx->gw_thrd_run = 1;
    if (plt_thrd_create(mdns_gw_discvr_thrd, ctx) < 0)
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
mdns_gw_discvr_stop - Stop Z/IP gateway discovery
@param[in]	    ctx		    The context returned from the call to mdns_gw_discvr_start()
@return Zero on success, non-zero on failure.
@post After the call, the ctx is invalid and should not be used
*/
int mdns_gw_discvr_stop(void *ctx)
{
    mdns_discvr_ctx_t *gw_discvr_ctx = (mdns_discvr_ctx_t *)ctx;

    if (!ctx)
    {
        return -1;
    }

    if (gw_discvr_ctx->ctx_id != MDNS_CTX_ID)
    {
        return -2;
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
@}
*/



