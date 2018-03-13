/**
@file   gw_discovery.c - Sample application to discover a Z/IP gateway.

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


typedef struct
{
    int     use_ipv4;       ///< Flag to indicate whether to use IPv4 or IPv6. 1=IPv4; 0=IPv6
    uint8_t *gw_addr_buf;   ///< Gateway addresses buffer
    char    **gw_name;      ///< Gateway names corresponding to entries in gw_addr_buf
    uint8_t gw_addr_cnt;    ///< Number of gateway addresses in gw_addr_buf

} hl_appl_ctx_t;


/**
gw_discvr_cb - gateway discovery callback
@param[in]	gw_addr	    Gateway addresses
@param[in]	gw_addr_cnt Number of gateway addresses returned in gw_addr
@param[in]	ipv4        Flag to indicate the addr parameter is IPv4 or IPv6. 1=IPv4; 0=IPv6
@param[in]	usr_param   User defined parameter used in callback function
@param[in]	rpt_num     Report number that this callback is delivering the gateway addresses report; start from 1
@param[in]	total_rpt   Total reports that will be delivered by callbacks. Each callback delivers one report.
@param[in]	gw_name	    Gateway names corresponding to the gw_ip.  If NULL, it means gateway name information is unavailable.
*/
void gw_discvr_cb(uint8_t *gw_addr, uint8_t gw_addr_cnt, int ipv4, void *usr_param,
                  int rpt_num, int total_rpt, char **gw_name)
{
    hl_appl_ctx_t   *hl_appl = (hl_appl_ctx_t *)usr_param;
    uint8_t         *tmp_buf;
    char            **tmp_gw_name;
    int             gw_addr_buf_sz;
    int             i;
    int             result;
    int             one_ip_addr_len;    //number of bytes required for storing one IP address. IPv4=4, IPv6=16
    char            addr_str[80];

    printf("\nReceived report:%d/%d with gw count:%u\n", rpt_num, total_rpt, gw_addr_cnt);

    if (total_rpt == 0)
    {
        printf("The system has no valid IP, please configure it.\n");
        return;
    }

    one_ip_addr_len = (ipv4)? 4 : 16;

    if (gw_addr_cnt > 0)
    {
        //Calculate IP address buffer size for storing new found gateways from this report
        gw_addr_buf_sz = (gw_addr_cnt * one_ip_addr_len);

        if (hl_appl->gw_addr_buf)
        {   //Expand buffer to store new found gateways from this report
            tmp_buf = realloc(hl_appl->gw_addr_buf, (hl_appl->gw_addr_cnt * one_ip_addr_len) + gw_addr_buf_sz);
        }
        else
        {   //Allocate buffer to store new found gateways
            tmp_buf = malloc(gw_addr_buf_sz);
        }

        if (gw_name)
        {   //Gateway names are available
            if (hl_appl->gw_name)
            {   //Expand buffer to store new found gateways from this report
                tmp_gw_name = (char **)realloc(hl_appl->gw_name, (hl_appl->gw_addr_cnt + gw_addr_cnt)*sizeof(char *));
            }
            else
            {   //Allocate buffer to store new found gateways
                tmp_gw_name = (char **)malloc(gw_addr_cnt * sizeof(char *));
            }
            if (tmp_gw_name)
            {   //Save gateway names
                hl_appl->gw_name = tmp_gw_name;
                for (i=0; i<gw_addr_cnt; i++)
                {
                    tmp_gw_name[hl_appl->gw_addr_cnt + i] = strdup(gw_name[i]);
                }
            }
        }

        if (tmp_buf)
        {   //Save gateway IP addresses
            hl_appl->gw_addr_buf = tmp_buf;
            memcpy(hl_appl->gw_addr_buf + (hl_appl->gw_addr_cnt * one_ip_addr_len), gw_addr, gw_addr_buf_sz);
            hl_appl->gw_addr_cnt += gw_addr_cnt;
        }
    }

    if (hl_appl->gw_addr_cnt > 0)
    {
        //Display gateway ip addresses
        printf("\n---Gateways found---\n");
        for (i=0; i<hl_appl->gw_addr_cnt; i++)
        {
            result = zwnet_ip_ntoa(hl_appl->gw_addr_buf + (i * one_ip_addr_len), addr_str, 80, ipv4);
            if (result == 0)
            {
                printf("(%d) %s [%s]\n", i, addr_str, (hl_appl->gw_name)? hl_appl->gw_name[i] : "unknown");
            }
        }
        printf("Press 'x' to exit ...\n");
    }
}


/**
show_usage - show program usage
*/
void show_usage(void)
{
    printf("usage: gw_discovery <ip option>\n");
    printf("ip option: -4 for using IPv4; -6 for using IPv6; default to IPv6 without option given\n");
}

int main(int argc, char **argv)
{
    hl_appl_ctx_t   appl_ctx = {0};
    int             choice;
    void            *gw_discvr_ctx;

    //Parsing program arguments
    if (argc == 1)
    {   //Default to IPv6
        appl_ctx.use_ipv4 = 0;
    }
    else if (argc == 2)
    {
        if ((strcmp(argv[1], "-4") == 0) || (strcmp(argv[1], "-6") == 0))
        {
            appl_ctx.use_ipv4 = (strcmp(argv[1], "-4") == 0)? 1 : 0;
        }
        else
        {
            show_usage();
            return -1;
        }
    }
    else
    {
        show_usage();
        return -1;
    }

    printf("Scanning for Z/IP gateway ...\n");
    printf("Press 'x' to exit ...\n");

    //Start gateway discovery
    gw_discvr_ctx = zwnet_gw_discvr_start(gw_discvr_cb, &appl_ctx, appl_ctx.use_ipv4, 1 /*use mDNS for discovery*/);

    if (!gw_discvr_ctx)
    {
        return -1;
    }

    do
    {
        choice = getchar();
    } while (choice != 'x');

    //Stop gateway discovery
    zwnet_gw_discvr_stop(gw_discvr_ctx);

    //Free allocated memory
    if (appl_ctx.gw_addr_cnt > 0)
    {
        //Free gateway names buffer
        if (appl_ctx.gw_name)
        {
            int i;
            for (i=0; i<appl_ctx.gw_addr_cnt; i++)
            {
                free(appl_ctx.gw_name[i]);
            }
            free(appl_ctx.gw_name);
            appl_ctx.gw_name = NULL;
        }
        //Free gateway IP address buffer
        free(appl_ctx.gw_addr_buf);
        appl_ctx.gw_addr_buf = NULL;
        appl_ctx.gw_addr_cnt = 0;
    }
    return 0;
}

