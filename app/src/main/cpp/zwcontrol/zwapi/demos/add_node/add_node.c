/**
@file   add_node.c - Sample application to add a node into Z-wave network.

@author David Chow

@version    1.0 26-1-15  Initial release

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

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../../include/zip_api.h"

#define  MAX_DTLS_PSK       64                 //Maximum DTLS pre-shared key hex string length

#define ADD_NODE_STS_UNKNOWN    0   ///<Add node status: unknown
#define ADD_NODE_STS_PROGRESS   1   ///<Add node status: in progress
#define ADD_NODE_STS_DONE       2   ///<Add node status: done

#define     SEC2_ENTER_KEY_REQ  1   ///< Bit-mask for allowing S2 key request callback
#define     SEC2_ENTER_DSK      2   ///< Bit-mask for allowing S2 DSK callback

/** user application context*/
typedef struct
{
    volatile int    init_status;   ///< Network initialization status. 0=unknown; 1=done
    volatile int    add_status;    ///< Add node status.  0=unknown; 1=in progress; 2=done
    volatile unsigned   sec2_cb_enter;///< Control security 2 callback entry bitmask, see SEC2_ENTER_XXX. bit set = allowed callback, 0 = not allowed
    volatile int    sec2_cb_exit;  ///< Security 2 callback status. 1 = exited callback, 0 = waiting or still in the callback
    uint8_t         sec2_add_node; ///< Flag to determine whether to use security 2 when adding node
    sec2_add_prm_t  sec2_add_prm;  ///< Add node with security 2 parameters

    int             use_ipv4;      ///< Flag to indicate whether to use IPv4 or IPv6. 1=IPv4; 0=IPv6
    zwnet_p         zwnet;         ///< Network handle
} hl_appl_ctx_t;


/**
prompt_str - prompt for a string from user
@param[in] disp_str   The prompt string to display
@param[in] out_buf_sz The size of out_str buffer
@param[out] out_str   The buffer where the user input string to be stored
@return          The out_str if successful; else NULL.
*/
static char  *prompt_str(hl_appl_ctx_t *hl_appl, const char *disp_str, int out_buf_sz, char *out_str)
{
    int retry;
    puts(disp_str);
    retry = 3;
    while (retry-- > 0)
    {
        if (fgets(out_str, out_buf_sz, stdin) && (*out_str) && ((*out_str) != '\n'))
        {
            char *newline;
            //Remove newline character

            newline = strchr(out_str, '\n');
            if (newline)
            {
                *newline = '\0';
            }
            return out_str;
        }
    }
    return NULL;
}


/**
prompt_hex - prompt for an hexadecimal unsigned integer input from user
@param[in] str   The prompt string to display
@return          The unsigned integer that user has input
*/
static unsigned prompt_hex(hl_appl_ctx_t *hl_appl, char *str)
{
    char user_input_str[36];
    unsigned  ret;

    if (prompt_str(hl_appl, str, 36, user_input_str))
    {
#ifdef USE_SAFE_VERSION
        if (sscanf_s(user_input_str, "%x", &ret) == 1)
        {
            return ret;
        }
#else
        if (sscanf(user_input_str, "%x", &ret) == 1)
        {
            return ret;
        }
#endif
    }
    return 0;
}


/**
prompt_char - prompt for a character input from user
@param[in] str   The prompt string to display
@return          The character that user has input. Null character on error.
*/
static char prompt_char(hl_appl_ctx_t *hl_appl, char *str)
{
    char ret[80];

    if (prompt_str(hl_appl, str, 80, ret))
    {
        return ret[0];
    }
    return 0;
}


/**
prompt_yes - prompt for yes or no from user
@param[in] str   The prompt string to display
@return          1 = user has input yes, 0 =  user has input no
*/
static int prompt_yes(hl_appl_ctx_t *hl_appl, char *str)
{
    char c;

    c = prompt_char(hl_appl, str);
    if (c == 'y' || c == 'Y')
    {
        return 1;
    }
    return 0;
}


/**
hex2bin - Convert hex character to binary
@param[in] c        hex character
@return  Value of hex character on success, negative value on failure
*/
static int hex2bin(char c)
{
    if (c >= '0' && c <= '9')
    {
        return c-'0';
    }
    else if (c >= 'a' && c <= 'f')
    {
        return c-'a' + 10;
    }
    else if (c >= 'A' && c <= 'F')
    {
        return c-'A' + 10;
    }
    else
    {
        return -1;
    }
}


/**
hexstring_to_bin - Convert ASCII hexstring to binary string
@param[in] psk_str   ASCII hexstring
@param[in] psk_len   ASCII hexstring length (must be even number)
@param[out] psk_bin  Binary string
@return  Zero on success, non-zero on failure
*/
static int hexstring_to_bin(char *psk_str, int psk_len, uint8_t *psk_bin)
{
    int i = 0;
    int val;

    while(psk_len > 0)
    {
      val = hex2bin(*psk_str++);
      if(val < 0)
          return -1;
      psk_bin[i]  = (val & 0x0F) << 4;

      val = hex2bin(*psk_str++);
      if(val < 0)
          return -1;
      psk_bin[i] |= (val & 0x0F);

      i++;
      psk_len -= 2;
    }

    return 0;
}


/**
config_param_get - get configuration parameters
@param[in] cfg_file     Configuration file name
@param[out] host_port   Host port
@param[out] router      Z/IP router IP address
@param[out] psk         DTLS pre-shared key (PSK)
@return         ZW_ERR_XXX
*/
static int config_param_get(char *cfg_file, uint16_t *host_port, char *router, char *psk)
{
    FILE        *file;
    const char  delimiters[] = " =\r\n";
    char        line[384];
    char        *prm_name;
    char        *prm_val;

    //Initialize output
    *router = '\0';
    *psk = '\0';
    *host_port = 0;

    //Open config file
    if (!cfg_file)
    {
        return ZW_ERR_FILE_OPEN;
    }

    file = fopen(cfg_file, "rt");
    if (!file)
    {
        return ZW_ERR_FILE_OPEN;
    }

    while (fgets(line, 384, file))
    {
        if (*line == '#')
        {   //Skip comment line
            continue;
        }

        //Check if '=' exists
        if (strchr(line, '='))
        {
            //Get the parameter name and value
            prm_name = strtok(line, delimiters);

            if (prm_name)
            {
                prm_val = strtok(NULL, delimiters);

                if (!prm_val)
                {
                    continue;
                }

                //Compare the parameter name
                if (strcmp(prm_name, "ZipLanPort") == 0)
                {
                    unsigned port;
                    if (sscanf(prm_val, "%u", &port) == 1)
                    {
                        *host_port = (uint16_t)port;
                    }
                }
                else if (strcmp(prm_name, "ZipRouterIP") == 0)
                {
                    strcpy(router, prm_val);
                }
                else if (strcmp(prm_name, "DTLSPSK") == 0)
                {
                    strcpy(psk, prm_val);
                }
            }
        }
    }

    fclose(file);

    return 0;
}


/**
hl_nw_tx_cb - Callback function to notify application transmit data status
@param[in]	user	    The high-level api context
param[in]	tx_sts	    Transmit status ZWNET_TX_xx
@return
*/
static void hl_nw_tx_cb(void *user, uint8_t tx_sts)
{
    static const char    *tx_cmplt_sts[] = {"ok",
        "timeout: no ACK received",
        "system error",
        "destination host needs long response time",
        "frame failed to reach destination host"
    };

    if (tx_sts == TRANSMIT_COMPLETE_OK)
    {
        //printf("Higher level appl send data completed successfully\n");
    }
    else
    {
        printf("Higher level appl send data completed with error:%s\n",
               (tx_sts < sizeof(tx_cmplt_sts)/sizeof(char *))? tx_cmplt_sts[tx_sts]  : "unknown");
    }
}


/**
hl_nw_node_cb - Callback function to notify node is added, deleted, or updated
@param[in]	user	    The high-level api context
@param[in]	noded	Node
@param[in]	mode	    The node status
@return
*/
static void hl_nw_node_cb(void *user, zwnoded_p noded, int mode)
{
    switch (mode)
    {
        case ZWNET_NODE_ADDED:
            {
                printf("\nNode:%u added\n", (unsigned)noded->nodeid);
            }
            break;

        case ZWNET_NODE_REMOVED:
            {
                printf("\nNode:%u removed\n", (unsigned)noded->nodeid);
            }
            break;
    }
}


/**
hl_nw_notify_cb - Callback function to notify the status of current operation
@param[in]	user	    user context
@param[in]	op		    network operation ZWNET_OP_XXX
@param[in]	sts		    status of current operation
@param[in]	info	    additional information for the specified op and sts; NULL if there is no additional info
@return
*/
static void hl_nw_notify_cb(void *user, uint8_t op, uint16_t sts, zwnet_sts_t *info)
{
    hl_appl_ctx_t *hl_appl = (hl_appl_ctx_t *)user;

    //Check whether the status is progress status of discovering each detailed node information
    if (sts & OP_GET_NI_TOTAL_NODE_MASK)
    {
        uint16_t    total_nodes;
        uint16_t    cmplt_nodes;

        total_nodes = (sts & OP_GET_NI_TOTAL_NODE_MASK) >> 8;
        cmplt_nodes = sts & OP_GET_NI_NODE_CMPLT_MASK;
        printf("Get node info %u/%u completed\n", cmplt_nodes, total_nodes);
        return;
    }

    switch (op)
    {
        case ZWNET_OP_INITIALIZE:
            printf("\nInitialization status:%u\n",(unsigned)sts);
            if (sts == OP_DONE)
            {
                hl_appl->init_status = 1;
                printf("\n(1) Add node\n(x) Exit\n");
                printf("Select your choice:\n");
            }
            else
            {
                printf("Press 'x' to exit ...\n");
            }
            break;

        case ZWNET_OP_ADD_NODE:
            printf("Add node status:%u\n",(unsigned)sts);
            if (sts == OP_DONE)
            {   //Clear add node DSK callback control & status
                hl_appl->sec2_cb_enter = 0;
                hl_appl->sec2_cb_exit = 1;

                hl_appl->add_status = ADD_NODE_STS_DONE;
            }
            else if (sts == OP_FAILED)
            {   //Clear add node DSK callback control & status
                hl_appl->sec2_cb_enter = 0;
                hl_appl->sec2_cb_exit = 1;

                hl_appl->add_status = ADD_NODE_STS_UNKNOWN;
            }


            if (hl_appl->add_status != ADD_NODE_STS_PROGRESS)
            {
                printf("\n(1) Add node\n(x) Exit\n");
                printf("Select your choice:\n");
            }
            break;

        default:
            printf("hl_nw_notify_cb op:%u, status:%u\n", (unsigned)op, (unsigned)sts);
    }
}


/**
lib_init - Initialize library
@param[in]	hl_appl		        The high-level api context
@param[in]	host_port		    Host listening port
@param[in]	zip_router_ip		Z/IP router IP address in numerical form
@param[in]	use_ipv4		    Flag to indicate zip_router_ip is in IPv4 or IPv6 format. 1= IPv4; 0= IPv6
@param[in]	dev_cfg_file_name	Device specific configuration database file name
@param[in]	dtls_psk		    DTLS pre-shared key
@param[in]	dtls_psk_len		DTLS pre-shared key length (in bytes)
@param[in]	pref_dir		    Full path of directory for storing network/user preference files
@param[in]	cmd_cls_cfg_file    Full path to the command class configuration file
@return  0 on success, negative error number on failure
*/
int lib_init(hl_appl_ctx_t *hl_appl, uint16_t host_port, uint8_t *zip_router_ip, int use_ipv4,
             char *dev_cfg_file_name, uint8_t *dtls_psk, uint8_t dtls_psk_len, char *pref_dir, char *cmd_cls_cfg_file)
{
    int                 result;
    zwnet_init_t        zw_init = {0};

    zw_init.user = hl_appl; //high-level application context
    zw_init.node = hl_nw_node_cb;
    zw_init.notify = hl_nw_notify_cb;
    zw_init.appl_tx = hl_nw_tx_cb;
    zw_init.pref_dir = pref_dir;
    zw_init.cmd_cls_cfg_file = cmd_cls_cfg_file;
    zw_init.print_txt_fn = NULL;
    zw_init.net_info_dir = NULL;
    zw_init.host_port = host_port;
    zw_init.use_ipv4 = use_ipv4;
    memcpy(zw_init.zip_router, zip_router_ip, (use_ipv4)? IPV4_ADDR_LEN : IPV6_ADDR_LEN);
    zw_init.dev_cfg_file = dev_cfg_file_name;
    zw_init.dev_cfg_usr = NULL;
    zw_init.dtls_psk_len = dtls_psk_len;
    if (dtls_psk_len)
    {
        memcpy(zw_init.dtls_psk, dtls_psk, dtls_psk_len);
    }
    //Unhandled command handler
    zw_init.unhandled_cmd = NULL;

    //Init ZW network
    result = zwnet_init(&zw_init, &hl_appl->zwnet);

    if (result != 0)
    {
        printf("zwnet_init with error:%d\n", result);

        //Display device configuration file error
        if (zw_init.err_loc.dev_ent)
        {
            printf("Parsing device configuration file error loc:\n");
            printf("Device entry number:%u\n", zw_init.err_loc.dev_ent);
            if (zw_init.err_loc.ep_ent)
            {
                printf("Endpoint entry number:%u\n", zw_init.err_loc.ep_ent);
            }

            if (zw_init.err_loc.if_ent)
            {
                printf("Interface entry number:%u\n", zw_init.err_loc.if_ent);
            }
        }
        return result;
    }

    return 0;
}


/**
nw_init - initialization network
@param[in] hl_appl   user application context
@return  0 on success; otherwise negative number
*/
int nw_init(hl_appl_ctx_t *hl_appl)
{
    int             ret;
    char            zip_gw_addr_str[100];
    uint16_t        host_port;                  ///< Host listening port
    char            psk_str[384];
    int             psk_len;
    uint8_t         dtls_psk[MAX_DTLS_PSK/2];   ///< DTLS pre-shared key
    uint8_t         zip_gw_ip[16];              ///< Z/IP gateway address in IPv4 or IPv6

    //Read config file to get configuration parameters
    ret = config_param_get("app.cfg", &host_port, zip_gw_addr_str, psk_str);
    if (ret != 0)
    {
        printf("Error: couldn't get config param from file: app.cfg\n");
        return ret;
    }

    //Check DTLS pre-shared key validity
    psk_len = strlen(psk_str);

    if (psk_len > 0)
    {
        if (psk_len > MAX_DTLS_PSK)
        {
            printf("PSK string length is too long\n");
            return ZW_ERR_VALUE;
        }
        if (psk_len % 2)
        {
            printf("PSK string length should be even\n");
            return ZW_ERR_VALUE;
        }
        //Convert ASCII hexstring to binary string
        ret = hexstring_to_bin(psk_str, psk_len, dtls_psk);
        if (ret != 0)
        {
            printf("PSK string is not hex string\n");
            return ZW_ERR_VALUE;
        }
    }

    //Convert IPv4 / IPv6 address string to numeric equivalent
    ret = zwnet_ip_aton(zip_gw_addr_str, zip_gw_ip, &hl_appl->use_ipv4);

    if (ret != 0)
    {
        printf("Invalid Z/IP router IP address:%s\n", zip_gw_addr_str);
        return ZW_ERR_IP_ADDR;
    }

    //Initialize library
    ret = lib_init(hl_appl, host_port, zip_gw_ip, hl_appl->use_ipv4, "zwave_device_rec.txt" /*device info database file*/,
                  dtls_psk, psk_len/2, NULL, "cmd_class.cfg");

    if (ret < 0)
    {
        printf("lib_init with error: %d\n", ret);
    }
    return ret;
}


/**
hl_add_node_s2_cb - callback for add node with security 2 operation to report Device Specific Key (DSK)
@param[in]	usr_param  user supplied parameter when calling zwnet_add
@param[in]	cb_param   DSK related callback parameters
*/
static void hl_add_node_s2_cb(void *usr_param, sec2_add_cb_prm_t *cb_param)
{
    hl_appl_ctx_t *hl_appl = (hl_appl_ctx_t *)usr_param;
    int           res;

    if (cb_param->cb_type == S2_CB_TYPE_REQ_KEY)
    {
        uint8_t granted_key;
        uint8_t grant_csa;

        if (hl_appl->sec2_cb_enter & SEC2_ENTER_KEY_REQ)
        {   //Requested keys callback is allowed
            hl_appl->sec2_cb_enter &= ~SEC2_ENTER_KEY_REQ;
        }
        else
        {
            printf("\nNot allowed to processed Security 2 requested keys callback!\n");
            return;
        }

        printf("\nDevice requested keys bit-mask: %02Xh\n", cb_param->cb_prm.req_key.req_keys);

        printf("Key (bit-mask in hex) :\n");
        printf("                      Security 2 key 0 (01)\n");
        printf("                      Security 2 key 1 (02)\n");
        printf("                      Security 2 key 2 (04)\n");
        printf("                      Security 0       (80)\n");

        granted_key = prompt_hex(hl_appl, "Grant keys bit-mask (hex):");

        grant_csa = 0;
        if (cb_param->cb_prm.req_key.req_csa)
        {
            printf("Device requested for client-side authentication (CSA)\n");
            if (prompt_yes(hl_appl, "Grant CSA (y/n)?:"))
            {
                grant_csa = 1;
                printf("Please enter this 10-digit CSA Pin into the joining device:%s\n", cb_param->cb_prm.req_key.csa_pin);
            }
            //No DSK callback when in CSA mode
            hl_appl->sec2_cb_enter &= ~SEC2_ENTER_DSK;

        }

        res = zwnet_add_sec2_grant_key(hl_appl->zwnet, granted_key, grant_csa);

        if (res != 0)
        {
            printf("zwnet_add_sec2_grant_key with error: %d\n", res);
        }

        //Check whether if there is DSK callback pending
        if (!(hl_appl->sec2_cb_enter))
        {   //No callback pending
            hl_appl->sec2_cb_exit = 1;
        }
    }
    else
    {
        sec2_dsk_cb_prm_t   *dsk_prm;
        int                 accept;
        char                dsk_str[200];

        if (hl_appl->sec2_cb_enter & SEC2_ENTER_DSK)
        {   //DSK callback is allowed
            hl_appl->sec2_cb_enter &= ~SEC2_ENTER_DSK;
        }
        else
        {
            printf("\nNot allowed to processed Security 2 DSK callback!\n");
            return;
        }

        dsk_prm = &cb_param->cb_prm.dsk;

        if (dsk_prm->pin_required)
        {
            printf("\nReceived DSK: XXXXX%s\n", dsk_prm->dsk);
        }
        else
        {
            printf("\nReceived DSK: %s\n", dsk_prm->dsk);
        }

        accept = prompt_yes(hl_appl, "Do you accept this device to be added securely (y/n)?:");

        printf("You %s the device.\n", (accept)? "accepted" : "rejected");

        if (accept && dsk_prm->pin_required)
        {
            if (prompt_str(hl_appl, "Enter 5-digit PIN that matches the received DSK:", 200, dsk_str))
            {

#ifdef USE_SAFE_VERSION
                strcat_s(dsk_str, 200, dsk_prm->dsk);
#else
                strcat(dsk_str, dsk_prm->dsk);
#endif
            }
        }

        res = zwnet_add_sec2_accept(hl_appl->zwnet, accept, (dsk_prm->pin_required)? dsk_str : dsk_prm->dsk);

        if (res != 0)
        {
            printf("zwnet_add_sec2_accept with error: %d\n", res);
        }

        hl_appl->sec2_cb_exit = 1;
    }
}


/**
hl_add_node - Add node
@param[in]	hl_appl  Application context
@return zero if successful; else negative error number
*/
static int hl_add_node(hl_appl_ctx_t *hl_appl)
{
    int     res;
    char    dsk_str[200];
    zwnetd_p netdesc;

    netdesc = zwnet_get_desc(hl_appl->zwnet);

    if (netdesc->ctl_cap & ZWNET_CTLR_CAP_S2)
    {
        printf("Controller supports security 2.\n");
        hl_appl->sec2_add_node = 1;
    }
    else
    {
        hl_appl->sec2_add_node = 0;
    }

    if (hl_appl->sec2_add_node)
    {
        hl_appl->sec2_add_prm.dsk = NULL;

        if (prompt_yes(hl_appl, "Pre-enter Device Specific Key (DSK) (y/n)?:"))
        {
            if (prompt_str(hl_appl, "DSK:", 200, dsk_str))
            {
                hl_appl->sec2_add_prm.dsk = dsk_str;
            }
        }

        hl_appl->sec2_add_prm.usr_param = hl_appl;

        hl_appl->sec2_add_prm.cb = hl_add_node_s2_cb;

    }

    res = zwnet_add(hl_appl->zwnet, 1, (hl_appl->sec2_add_node)? &hl_appl->sec2_add_prm : NULL, 0);

    if (res == 0)
    {
        if (hl_appl->sec2_add_node)
        {
            int wait_count;

            hl_appl->sec2_cb_enter = SEC2_ENTER_KEY_REQ;

            if (!hl_appl->sec2_add_prm.dsk)
            {   //No pre-entered DSK, requires DSK callback
                hl_appl->sec2_cb_enter |= SEC2_ENTER_DSK;
            }

            hl_appl->sec2_cb_exit = 0;

            printf("Waiting for Requested keys and/or DSK callback ...\n");

            //Wait for S2 callback to exit
            wait_count = 600;    //Wait for 60 seconds
            while (wait_count-- > 0)
            {
                if (hl_appl->sec2_cb_exit == 1)
                    break;
                plt_sleep(100);
            }
        }
    }

    return res;

}


int main(int argc, char **argv)
{
    hl_appl_ctx_t   appl_ctx = {0};
    int             choice;
    int             result;

    //Init user-application
    appl_ctx.use_ipv4 = 1; //Use IPv4

    //Initialize network
    if (nw_init(&appl_ctx) != 0)
        return -1;

    printf("Initialize network in progress, please wait for status ...\n");
    printf("Press 'x' to exit ...\n");

    while (1)
    {
        do
        {
            choice = getchar();
        } while (choice == 0x0A);

        if (appl_ctx.init_status == 0)
        {
            if (choice == 'x')
            {
                 //Exit and clean up
                zwnet_exit(appl_ctx.zwnet);
                return 0;
           }
        }
        else if ((appl_ctx.add_status == ADD_NODE_STS_UNKNOWN) || (appl_ctx.add_status == ADD_NODE_STS_DONE))
        {
            switch (choice)
            {
                case '1':
                    result = hl_add_node(&appl_ctx);
                    //result = zwnet_add(appl_ctx.zwnet, 1, (appl_ctx.sec2_add_node)? &appl_ctx.sec2_add_prm : NULL);
                    if (!appl_ctx.sec2_add_node)
                    {
                        if (result == 0)
                        {
                            printf("Add node in progress, please wait for status ...\n");
                            appl_ctx.add_status = ADD_NODE_STS_PROGRESS;
                        }
                        else
                        {
                            printf("Add node with error:%d\n", result);
                        }
                    }
                    break;

                case 'x':
                    //Exit and clean up
                    zwnet_exit(appl_ctx.zwnet);
                    return 0;
                    break;

                default:
                    printf("Invalid choice:%c\n", choice);
            }

            printf("\n(1) %s\n(x) Exit\n", (appl_ctx.add_status == ADD_NODE_STS_PROGRESS)?
                   "Abort add node operation" : "Add node");
            printf("Select your choice:\n");
        }
        else    //ADD_NODE_STS_PROGRESS
        {
            switch (choice)
            {
                case '1':
                    result = zwnet_abort(appl_ctx.zwnet);
                    if (result == 0)
                    {
                        printf("Add node operation aborted.\n");
                        appl_ctx.add_status = ADD_NODE_STS_UNKNOWN;
                    }
                    else
                    {
                        printf("Add node operation can't be aborted, error:%d\n", result);
                    }
                    break;

                case 'x':
                    //Exit and clean up
                    zwnet_exit(appl_ctx.zwnet);
                    return 0;
                    break;

                default:
                    printf("Invalid choice:%c\n", choice);
            }

            printf("\n(1) %s\n(x) Exit\n", (appl_ctx.add_status == ADD_NODE_STS_PROGRESS)?
                   "Abort add node operation" : "Add node");
            printf("Select your choice:\n");

        }
    }

    return 0;
}

