/**
@file   bin_sensor.c - Sample application to control a binary sensor.

@author David Chow

@version    1.0 24-4-17  Initial release

@copyright © 2017 SIGMA DESIGNS, INC. THIS IS AN UNPUBLISHED WORK PROTECTED BY SIGMA DESIGNS, INC.
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
#include <time.h>
#include "../../include/zip_api.h"

#define  MAX_DTLS_PSK       64                 //Maximum DTLS pre-shared key hex string length


const char *bsensor_type_str[] =
{
    "unknown",
    "General purpose sensor",
    "Smoke sensor",
    "CO sensor",
    "CO2 sensor",
    "Heat sensor",
    "Water sensor",
    "Freeze sensor",
    "Tamper sensor",
    "AUX sensor",
    "Door/Window sensor",
    "Tilt sensor",
    "Motion sensor",
    "Glass break sensor"
};


/** user application context*/
typedef struct
{
    volatile int    init_status;   ///< Network initialization status. 0=initializing; 1=setting up unsolicited address; 2=done
    int             use_ipv4;      ///< Flag to indicate whether to use IPv4 or IPv6. 1=IPv4; 0=IPv6
    zwnet_p         zwnet;         ///< Network handle
    uint8_t         zip_gw_ip[16]; ///< Z/IP gateway address in IPv4 or IPv6
} hl_appl_ctx_t;


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
prompt_str - prompt for a string from user
@param[in] disp_str   The prompt string to display
@param[in] out_buf_sz The size of out_str buffer
@param[out] out_str   The buffer where the user input string to be stored
@return          The out_str if successful; else NULL.
*/
static char  *prompt_str(const char *disp_str, int out_buf_sz, char *out_str)
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
prompt_uint - prompt for an unsigned integer input from user
@param[in] str   The prompt string to display
@return          The unsigned integer that user has input
*/
static unsigned prompt_uint(char *str)
{
    char        user_input_str[36];
    unsigned    ret;

    if (prompt_str(str, 36, user_input_str))
    {
        if (sscanf(user_input_str, "%u", &ret) == 1)
        {
            return ret;
        }
    }
    return 0;
}


/**
show_menu - Display menu for user action
@return
*/
static void show_menu(void)
{
    printf("\n(1) Setup binary sensor report\n");
    printf("(2) Get supported binary sensor types\n");
    printf("(3) Get live binary sensor report\n");
    printf("(4) Get cache binary sensor report\n");
    printf("(x) Exit\n");
    printf("Select your choice:\n");

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
        //printf("Send command completed\n");
    }
    else
    {
        printf("Send command completed with error:%s\n",
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
        case ZWNET_NODE_STATUS_ALIVE:
            {
                printf("node:%u is alive\n", (unsigned)noded->nodeid);
            }
            break;

        case ZWNET_NODE_STATUS_DOWN:
            {
                printf("node:%u is down\n", (unsigned)noded->nodeid);
            }
            break;

        case ZWNET_NODE_STATUS_SLEEP:
            {
                printf("node:%u is sleeping\n", (unsigned)noded->nodeid);
            }
            break;
    }
}


/**
gw_intf_get - Search for the Z/IP gateway interface
@param[in]  net     network handle
@param[out] gw_if   Z/IP gateway interface
@return  0 on success; otherwise negative number
*/
static int gw_intf_get(zwnet_p net, zwifd_t *gw_if)
{
    int         result;
    zwnoded_t   node;
    zwepd_t     ep;
    zwifd_t     intf;

    //Get first node (controller node)
    result = zwnet_get_node(net, &node);
    if (result != 0)
    {
        return result;
    }

    if (!zwnode_get_ep(&node, &ep)) //get first endpoint of the node
    {
        if (!zwep_get_if(&ep, &intf)) //get first interface of the endpoint
        {
            do
            {
                if (intf.cls == COMMAND_CLASS_ZIP_GATEWAY)
                {   //Found
                    *gw_if = intf;
                    return 0;
                }

            }while (!zwif_get_next(&intf, &intf)); //get next interface
        }
    }

    return  ZW_ERR_INTF_NOT_FOUND;
}


/**
hl_unsolicited_addr_setup - Setup unsolicited address to receive unsolicited report
@param[in]	hl_appl	    The high-level api context
@return  0 on success, negative error number on failure
*/
static int hl_unsolicited_addr_setup(hl_appl_ctx_t *hl_appl)
{
    int         result;
    uint8_t     local_ip[16];
    uint16_t    local_port;
    zwifd_t     gw_ifd;

    //Get local Z/IP client listening address and port
    result = zwnet_local_addr_get(hl_appl->zwnet, hl_appl->zip_gw_ip, local_ip, hl_appl->use_ipv4);

    if (result != 0)
    {
        printf("Error: couldn't get local Z/IP client listening address: %d\n", result);
        return result;
    }

    local_port = zwnet_listen_port_get(hl_appl->zwnet);

    if (hl_appl->use_ipv4)
    {   //Convert to IPv4-mapped IPv6 address
        uint8_t unsolicit_ipv4[4];

        //Save the IPv4 address
        memcpy(unsolicit_ipv4, local_ip, 4);

        //Convert the IPv4 address to IPv4-mapped IPv6 address
        memset(local_ip, 0, 10);
        local_ip[10] = 0xFF;
        local_ip[11] = 0xFF;
        memcpy(&local_ip[12], unsolicit_ipv4, 4);
    }

    result = gw_intf_get(hl_appl->zwnet, &gw_ifd);
    if (result != 0)
    {
        printf("Error: couldn't find Z/IP gateway interface: %d\n", result);
        return result;
    }

    result = zwif_gw_unsolicit_set(&gw_ifd, local_ip, local_port);

    if (result != 0)
    {
        printf("Error: couldn't set unsolicited address: %d\n", result);
    }

    return result;

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
                zwnetd_p nw_desp;

                //Get network descriptor
                nw_desp = zwnet_get_desc(hl_appl->zwnet);
                printf("network id:%08X, Z/IP controller id:%u\n", nw_desp->id, nw_desp->ctl_id);

                hl_appl->init_status = 1;

                printf("Network initialized!  Setting up unsolicited address, please wait ...\n");
                printf("Press 'x' to exit ...\n");

                if (hl_unsolicited_addr_setup(hl_appl) == 0)
                {
                    hl_appl->init_status = 2;
                    show_menu();
                }
            }
            else
            {
                printf("Press 'x' to exit ...\n");
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
    ret = zwnet_ip_aton(zip_gw_addr_str, hl_appl->zip_gw_ip, &hl_appl->use_ipv4);

    if (ret != 0)
    {
        printf("Invalid Z/IP router IP address:%s\n", zip_gw_addr_str);
        return ZW_ERR_IP_ADDR;
    }

    //Initialize library
    ret = lib_init(hl_appl, host_port, hl_appl->zip_gw_ip, hl_appl->use_ipv4, "zwave_device_rec.txt" /*device info database file*/,
                  dtls_psk, psk_len/2, NULL, "cmd_class.cfg");

    if (ret < 0)
    {
        printf("lib_init with error: %d\n", ret);
    }
    return ret;
}


/**
bin_sensor_intf_get - Search for the first binary sensor interface
@param[in]  net             network handle
@param[out] bin_sensor_if   first binary sensor interface found
@return  0 on success; otherwise negative number
*/
static int bin_sensor_intf_get(zwnet_p net, zwifd_t *bin_sensor_if)
{
    int         result;
    zwnoded_t   node;
    zwepd_t     ep;
    zwifd_t     intf;

    //Get first node (controller node)
    result = zwnet_get_node(net, &node);
    if (result != 0)
    {
        return result;
    }

    while (!zwnode_get_next(&node, &node))//get next node
    {
        if (!zwnode_get_ep(&node, &ep)) //get first endpoint of the node
        {
            do
            {
                if (!zwep_get_if(&ep, &intf)) //get first interface of the endpoint
                {
                    do
                    {
                        if (intf.cls == COMMAND_CLASS_SENSOR_BINARY)
                        {   //Found
                            *bin_sensor_if = intf;
                            return 0;
                        }

                    }while (!zwif_get_next(&intf, &intf)); //get next interface
                }
            }while (!zwep_get_next(&ep, &ep)); //get next endpoint
        }
    }

    return  ZW_ERR_INTF_NOT_FOUND;

}


/**
hl_bin_snsr_rep_cb - binary sensor report callback
@param[in]	ifd	    interface that received the report
@param[in]	state	state of the sensor: 0=idle, else event detected
@param[in]	type	sensor type, ZW_BSENSOR_TYPE_XXX. If type equals to zero, sensor type is unknown.
@param[in]	ts      time stamp.  If this is zero, the callback has no data and hence other parameter
                    values should be ignored.
@return
*/
void hl_bin_snsr_rep_cb(zwifd_p ifd, uint8_t state, uint8_t type, time_t ts)
{
    if (ts == 0)
    {
        printf("\nBinary sensor report: no cached data. Report get: type=%u\n", type);
        return;
    }

    if (type > ZW_BSENSOR_TYPE_GLASS_BRK)
    {
        type = 0;
    }

    printf("\nBinary sensor (%s) state :%s", bsensor_type_str[type], (state == 0)? "idle" : "event detected");
    printf("\nTime stamp:%s\n", ctime(&ts));

}


/**
hl_bsnsr_sup_report_cb - Report callback for supported sensor types
@param[in]	ifd	        interface
@param[in]	type_len    size of sensor type buffer
@param[in]	type        buffer to store supported sensor types (ZW_SENSOR_TYPE_XXX)
@param[in]	valid       validity of the report. If 1 the report is valid; else this report contains no data and should be ignored.
*/
static void hl_bsnsr_sup_report_cb(zwifd_p ifd, uint8_t type_len, uint8_t *type, int valid)
{
    if (!valid)
    {
        printf("\nNo cache for supported binary sensor types\n");
        return;
    }

    if (type_len > 0)
    {
        int i;
        printf("\nZ-wave binary sensor types: ");
        for (i=0; i<type_len; i++)
        {
            if (type[i] > ZW_BSENSOR_TYPE_GLASS_BRK)
            {
                type[i] = 0;
            }
            if (i == 0)
            {
                printf("%s", bsensor_type_str[type[i]]);
            }
            else
            {
                printf(", %s", bsensor_type_str[type[i]]);
            }
        }
        printf("\n");
    }
}


int main(int argc, char **argv)
{
    hl_appl_ctx_t   appl_ctx = {0};
    int             choice;
    int             result;
    int             i;
    unsigned        sensor_type;
    zwifd_t         bin_sensor_if = {0};

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

        if (appl_ctx.init_status < 2)
        {
            if (choice == 'x')
            {
                //Exit and clean up
                zwnet_exit(appl_ctx.zwnet);
                return 0;
            }
        }
        else
        {
            if ((choice >= '1') && (choice <= '4'))
            {
                if (!bin_sensor_if.net)
                {
                    result = bin_sensor_intf_get(appl_ctx.zwnet, &bin_sensor_if);
                    if (result != 0)
                    {
                        printf("\nSearching of binary sensor interface with error:%d\n", result);
                        show_menu();
                        continue;
                    }
                }
            }

            switch (choice)
            {
                case '1':
                    result = zwif_bsensor_rpt_set(&bin_sensor_if, hl_bin_snsr_rep_cb);

                    if (result != 0)
                    {
                        printf("\nBinary sensor report setup with error:%d\n", result);
                    }
                    break;

                case '2':
                    result = zwif_bsensor_sup_get(&bin_sensor_if, hl_bsnsr_sup_report_cb, 0);

                    if (result < 0)
                    {
                        printf("\nGet supported binary sensor with error:%d\n", result);
                    }
                    break;

                case '3':
                case '4':
                    printf("\nBinary Sensor types:");
                    for (i=1; i<=ZW_BSENSOR_TYPE_GLASS_BRK; i++)
                    {
                        printf("\n<%d> %s", i, bsensor_type_str[i]);
                    }
                    printf("\n<255> First supported sensor");
                    sensor_type = prompt_uint("Enter sensor type:");

                    result = zwif_bsensor_get(&bin_sensor_if, sensor_type, (choice == '3')? ZWIF_GET_BMSK_LIVE : ZWIF_GET_BMSK_CACHE);
                    if (result != 0)
                    {
                        printf("\nGet binary sensor reading with error:%d\n", result);
                    }
                    break;

                case 'x':
                    //Exit and clean up
                    zwnet_exit(appl_ctx.zwnet);
                    return 0;
                    break;

                default:
                    printf("Invalid choice:%02xh\n", choice);
            }

            show_menu();
        }
    }

    return 0;
}


