/* © 2014 Sigma Designs, Inc. This is an unpublished work protected by Sigma
 * Designs, Inc. as a trade secret, and is not to be used or disclosed except as
 * provided Z-Wave Controller Development Kit Limited License Agreement. All
 * rights reserved.
 *
 * Notice: All information contained herein is confidential and/or proprietary to
 * Sigma Designs and may be covered by U.S. and Foreign Patents, patents in
 * process, and are protected by trade secret or copyright law. Dissemination or
 * reproduction of the source code contained herein is expressly forbidden to
 * anyone except Licensees of Sigma Designs  who have executed a Sigma Designs’
 * Z-WAVE CONTROLLER DEVELOPMENT KIT LIMITED LICENSE AGREEMENT. The copyright
 * notice above is not evidence of any actual or intended publication of the
 * source code. THE RECEIPT OR POSSESSION OF  THIS SOURCE CODE AND/OR RELATED
 * INFORMATION DOES NOT CONVEY OR IMPLY ANY RIGHTS  TO REPRODUCE, DISCLOSE OR
 * DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE, USE, OR SELL A PRODUCT THAT IT  MAY
 * DESCRIBE.
 *
 * THE SIGMA PROGRAM AND ANY RELATED DOCUMENTATION OR TOOLS IS PROVIDED TO COMPANY
 * "AS IS" AND "WITH ALL FAULTS", WITHOUT WARRANTY OF ANY KIND FROM SIGMA. COMPANY
 * ASSUMES ALL RISKS THAT LICENSED MATERIALS ARE SUITABLE OR ACCURATE FOR
 * COMPANY’S NEEDS AND COMPANY’S USE OF THE SIGMA PROGRAM IS AT COMPANY’S
 * OWN DISCRETION AND RISK. SIGMA DOES NOT GUARANTEE THAT THE USE OF THE SIGMA
 * PROGRAM IN A THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICES ENVIRONMENT WILL
 * BE: (A) PERFORMED ERROR-FREE OR UNINTERRUPTED; (B) THAT SIGMA WILL CORRECT ANY
 * THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICE ENVIRONMENT ERRORS; (C) THE
 * THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICE ENVIRONMENT WILL OPERATE IN
 * COMBINATION WITH COMPANY’S CONTENT OR COMPANY APPLICATIONS THAT UTILIZE THE
 * SIGMA PROGRAM; (D) OR WITH ANY OTHER HARDWARE, SOFTWARE, SYSTEMS, SERVICES OR
 * DATA NOT PROVIDED BY SIGMA. COMPANY ACKNOWLEDGES THAT SIGMA DOES NOT CONTROL
 * THE TRANSFER OF DATA OVER COMMUNICATIONS FACILITIES, INCLUDING THE INTERNET,
 * AND THAT THE SERVICES MAY BE SUBJECT TO LIMITATIONS, DELAYS, AND OTHER PROBLEMS
 * INHERENT IN THE USE OF SUCH COMMUNICATIONS FACILITIES. SIGMA IS NOT RESPONSIBLE
 * FOR ANY DELAYS, DELIVERY FAILURES, OR OTHER DAMAGE RESULTING FROM SUCH ISSUES.
 * SIGMA IS NOT RESPONSIBLE FOR ANY ISSUES RELATED TO THE PERFORMANCE, OPERATION
 * OR SECURITY OF THE THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICES
 * ENVIRONMENT THAT ARISE FROM COMPANY CONTENT, COMPANY APPLICATIONS OR THIRD
 * PARTY CONTENT. SIGMA DOES NOT MAKE ANY REPRESENTATION OR WARRANTY REGARDING THE
 * RELIABILITY, ACCURACY, COMPLETENESS, CORRECTNESS, OR USEFULNESS OF THIRD PARTY
 * CONTENT OR SERVICE OR THE SIGMA PROGRAM, AND DISCLAIMS ALL LIABILITIES ARISING
 * FROM OR RELATED TO THE SIGMA PROGRAM OR THIRD PARTY CONTENT OR SERVICES. TO THE
 * EXTENT NOT PROHIBITED BY LAW, THESE WARRANTIES ARE EXCLUSIVE. SIGMA OFFERS NO
 * WARRANTY OF NON-INFRINGEMENT, TITLE, OR QUIET ENJOYMENT. NEITHER SIGMA NOR ITS
 * SUPPLIERS OR LICENSORS SHALL BE LIABLE FOR ANY INDIRECT, SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES OR LOSS (INCLUDING DAMAGES FOR LOSS OF BUSINESS, LOSS OF
 * PROFITS, OR THE LIKE), ARISING OUT OF THIS AGREEMENT WHETHER BASED ON BREACH OF
 * CONTRACT, INTELLECTUAL PROPERTY INFRINGEMENT, TORT (INCLUDING NEGLIGENCE),
 * STRICT LIABILITY, PRODUCT LIABILITY OR OTHERWISE, EVEN IF SIGMA OR ITS
 * REPRESENTATIVES HAVE BEEN ADVISED OF OR OTHERWISE SHOULD KNOW ABOUT THE
 * POSSIBILITY OF SUCH DAMAGES. THERE ARE NO OTHER EXPRESS OR IMPLIED WARRANTIES
 * OR CONDITIONS INCLUDING FOR SOFTWARE, HARDWARE, SYSTEMS, NETWORKS OR
 * ENVIRONMENTS OR FOR MERCHANTABILITY, NONINFRINGEMENT, SATISFACTORY QUALITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * The Sigma Program  is not fault-tolerant and is not designed, manufactured or
 * intended for use or resale as on-line control equipment in hazardous
 * environments requiring fail-safe performance, such as in the operation of
 * nuclear facilities, aircraft navigation or communication systems, air traffic
 * control, direct life support machines, or weapons systems, in which the failure
 * of the Sigma Program, or Company Applications created using the Sigma Program,
 * could lead directly to death, personal injury, or severe physical or
 * environmental damage ("High Risk Activities").  Sigma and its suppliers
 * specifically disclaim any express or implied warranty of fitness for High Risk
 * Activities.Without limiting Sigma’s obligation of confidentiality as further
 * described in the Z-Wave Controller Development Kit Limited License Agreement,
 * Sigma has no obligation to establish and maintain a data privacy and
 * information security program with regard to Company’s use of any Third Party
 * Service Environment or Cloud Service Environment. For the avoidance of doubt,
 * Sigma shall not be responsible for physical, technical, security,
 * administrative, and/or organizational safeguards that are designed to ensure
 * the security and confidentiality of the Company Content or Company Application
 * in any Third Party Service Environment or Cloud Service Environment that
 * Company chooses to utilize.
 */
#include "CC_Portal.h"
#include "ZW_classcmd.h"
#include "ZW_classcmd_ex.h"
#include "ZIP_Router.h"
#include "eeprom_layout.h"
#include "eeprom.h"
#include "ZW_tcp_client.h"
#include "ZW_udp_server.h"
#include "ipv46_nat.h"
#include "Serialapi.h"
#include "ZW_ZIPApplication.h"
#include "command_handler.h"

uint8_t gGwLockEnable = TRUE;


command_handler_codes_t ZIP_Portal_CommandHandler(zwave_connection_t *c, uint8_t* pData, uint16_t bDatalen)
{
  ZW_APPLICATION_TX_BUFFER* pCmd = (ZW_APPLICATION_TX_BUFFER*) pData;

  switch (pCmd->ZW_Common.cmd)
  {
  case GATEWAY_CONFIGURATION_SET:
  {
    ZW_GATEWAY_CONFIGURATION_STATUS GwConfigStatus;
    ZW_GATEWAY_CONFIGURATION_SET_FRAME *f = (ZW_GATEWAY_CONFIGURATION_SET_FRAME*) pData;

    GwConfigStatus.cmdClass = COMMAND_CLASS_ZIP_PORTAL;
    GwConfigStatus.cmd = GATEWAY_CONFIGURATION_STATUS;

    if (parse_portal_config(&f->lanIpv6Address1, (bDatalen - sizeof(ZW_GATEWAY_CONFIGURATION_SET) + 1)))
    {
      GwConfigStatus.status = ZIPR_READY_OK;
      process_post(&zip_process, ZIP_EVENT_TUNNEL_READY, 0);
    } else
    {
      GwConfigStatus.status = INVALID_CONFIG;
    }

    ZW_SendDataZIP(c, (BYTE*) &GwConfigStatus, sizeof(ZW_GATEWAY_CONFIGURATION_STATUS), NULL);
  }
    break;

  case GATEWAY_CONFIGURATION_GET:
  {
    uint8_t buf[128] =
    { 0 }; //buffer should be more than size of portal config + 2
    ZW_GATEWAY_CONFIGURATION_REPORT *pGwConfigReport = (ZW_GATEWAY_CONFIGURATION_REPORT *) buf;
    portal_ip_configuration_st_t *portal_config = (portal_ip_configuration_st_t *) (&pGwConfigReport->payload[0]);

    pGwConfigReport->cmdClass = COMMAND_CLASS_ZIP_PORTAL;
    pGwConfigReport->cmd = GATEWAY_CONFIGURATION_REPORT;

    memcpy(&portal_config->lan_address, &cfg.lan_addr, IPV6_ADDR_LEN);
    portal_config->lan_prefix_length = cfg.lan_prefix_length;

    memcpy(&portal_config->tun_prefix, &cfg.tun_prefix, IPV6_ADDR_LEN);
    portal_config->tun_prefix_length = cfg.tun_prefix_length;

    memcpy(&portal_config->gw_address, &cfg.gw_addr, IPV6_ADDR_LEN);

    memcpy(&portal_config->pan_prefix, &cfg.pan_prefix, IPV6_ADDR_LEN);
#if 0
    memcpy(&portal_config->unsolicited_dest, &cfg.unsolicited_dest, IPV6_ADDR_LEN);
    portal_config->unsolicited_destination_port = UIP_HTONS(cfg.unsolicited_port);
#endif

    ZW_SendDataZIP(c, buf,
        (sizeof(ZW_GATEWAY_CONFIGURATION_REPORT) - 1 + sizeof(portal_ip_configuration_st_t)), NULL);

  }
    break;

  default:
    return COMMAND_NOT_SUPPORTED;
    break;
  }
  return COMMAND_HANDLED;
}

#ifdef __ASIX_C51__
void Reset_Gateway(void) CC_REENTRANT_ARG
{
  Gw_Config_St_t GwConfig;
  uint16_t gwSettingsLen = 0;

  printf("Resetting Gateway start..\n");

  //Validate default configuration
  if (validate_defaultconfig(EXT_DEFAULT_CONFIG_START_ADDR, FALSE, &gwSettingsLen) != TRUE)
  {
    printf("Reset_Gateway FAILED: Invalid GW Config.\r\n");
#ifndef ZIP_GW_SP_MODE
    //Unlock the Gateway
    eeprom_read(EXT_GW_SETTINGS_START_ADDR, &GwConfig, sizeof(Gw_Config_St_t));
    //clear the lock bit
    GwConfig.showlock &= ~(1 << LOCK_BIT);
    eeprom_write(EXT_GW_SETTINGS_START_ADDR, &GwConfig, sizeof(Gw_Config_St_t));

    gGwLockEnable = FALSE;
#endif
    return;
  }

  //Write the default GW settings to active settings partition...
  gaconfig_WriteConfigToDefault(EXT_DEFAULT_CONFIG_START_ADDR, EXT_GW_SETTINGS_START_ADDR, gwSettingsLen);

#ifndef ZIP_GW_SP_MODE
  eeprom_read(EXT_GW_SETTINGS_START_ADDR, &GwConfig, sizeof(Gw_Config_St_t));
  //clear the lock bit
  GwConfig.showlock &= ~(1 << LOCK_BIT);
  eeprom_write(EXT_GW_SETTINGS_START_ADDR, &GwConfig, sizeof(Gw_Config_St_t));
  gGwLockEnable = FALSE;
#endif

  //copy the certs from the default partition
  gaconfig_WriteCertToFlash(EXT_DEFAULT_CONFIG_START_ADDR+gwSettingsLen);

  if(gconfig_ValidateCertPartition() != TRUE)
  {
    printf("Reset_Gateway FAILED: Cert default set failed.\r\n");
  }

  if (validate_gw_profile(EXT_GW_SETTINGS_START_ADDR, FALSE) != TRUE)
  {
    printf("Reset_Gateway FAILED: Gw settings default set failed.\r\n");
  }

  ResetGW_Callback();

  return;
}
#endif



REGISTER_HANDLER(
    ZIP_Portal_CommandHandler,
    0,
    COMMAND_CLASS_ZIP_PORTAL, ZIP_PORTAL_VERSION, SECURITY_SCHEME_UDP);
