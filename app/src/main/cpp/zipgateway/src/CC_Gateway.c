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
#include "CC_Gateway.h"
#include "ZW_classcmd.h"
#include "ZW_classcmd_ex.h"
#include "ZIP_Router.h"
#include "uip-debug.h"
#include "parse_config.h"
#include "eeprom_layout.h"
#include "eeprom.h"
#include "ZW_tcp_client.h"
#include "ZW_udp_server.h"
#include "ipv46_nat.h"
#include "Serialapi.h"
#include "ZW_ZIPApplication.h"
#include "command_handler.h"
#include <stdlib.h>

static u8_t tunnelTimer = 0xFF;

#define UPGRADE_START_TIMEOUT  300
static int cmd_cls_parse(uint8_t *cmd_buf, uint8_t cmd_buf_len, uint8_t *cmd_cls_buf, uint8_t *cmd_cnt,
    uint8_t *cmd_cls_sec_buf, uint8_t *cmd_cnt_sec);

extern void reset_the_nif();


void parse_gw_profile(Gw_PeerProfile_St_t *pGwPeerProfile)
{

  printf("parse_gw_profile..\r\n");

  memset(cfg.portal_url, 0, sizeof(cfg.portal_url));
  cfg.portal_portno = 0;

  if(!pGwPeerProfile)
  {
    printf("parse_gw_profile NULL Pointer.\r\n");
    return;
  }


  if( (!uip_is_addr_unspecified(&pGwPeerProfile->peer_ipv6_addr)) &&
    (!uip_is_addr_linklocal(&pGwPeerProfile->peer_ipv6_addr)) &&
    (!uip_is_addr_mcast(&pGwPeerProfile->peer_ipv6_addr)) &&
    (!uip_is_addr_loopback(&pGwPeerProfile->peer_ipv6_addr))
    )
  {
    if(is_4to6_addr((ip6addr_t*)&pGwPeerProfile->peer_ipv6_addr))
    {
      sprintf(cfg.portal_url, "%d.%d.%d.%d", pGwPeerProfile->peer_ipv6_addr.u8[12],  pGwPeerProfile->peer_ipv6_addr.u8[13], pGwPeerProfile->peer_ipv6_addr.u8[14],
          pGwPeerProfile->peer_ipv6_addr.u8[15] );
      cfg.portal_portno = (pGwPeerProfile->port1 << 8) | pGwPeerProfile->port2;
    }

    else
    {
      printf("Not a valid 4 to 6 Portal address.\r\n");
      #if 0
      sprintf(cfg.portal_url,"%x:%x:%x:%x:%x:%x:%x:%x",  pGwPeerProfile->peer_ipv6_addr.u16[0], pGwPeerProfile->peer_ipv6_addr.u16[1], pGwPeerProfile->peer_ipv6_addr.u16[2], pGwPeerProfile->peer_ipv6_addr.u16[3],
          pGwPeerProfile->peer_ipv6_addr.u16[4], GwPeerProfile.peer_ipv6_addr.u16[5], pGwPeerProfile->peer_ipv6_addr.u16[6], pGwPeerProfile->peer_ipv6_addr.u16[7]);
      #endif
    }


  }
  else
  {
    if(pGwPeerProfile->peerNameLength < sizeof(cfg.portal_url))
    {
      memcpy(cfg.portal_url,&pGwPeerProfile->peerName, pGwPeerProfile->peerNameLength);
      cfg.portal_url[pGwPeerProfile->peerNameLength]=0;
      cfg.portal_portno = (pGwPeerProfile->port1 << 8) | pGwPeerProfile->port2;
    }
  }

  printf("Portal URL: %s Port no = %u \r\n", cfg.portal_url, cfg.portal_portno);

  return;
}


static command_handler_codes_t Gateway_CommandHandler(zwave_connection_t* c,uint8_t* pData, uint16_t bDatalen)
{
  ZW_APPLICATION_TX_BUFFER_EX* pCmd = (ZW_APPLICATION_TX_BUFFER_EX*) pData;

  //printf("Gateway_CommandHandler: Received command = 0x%bx\r\n", pCmd->ZW_Common.cmd);

  switch (pCmd->ZW_Common.cmd)
  {
  case GATEWAY_MODE_SET:
  {
    Gw_Config_St_t GwConfig;

    //Read the GW configuration;
    eeprom_read(EXT_GW_SETTINGS_START_ADDR, (u8_t*) &GwConfig, sizeof(Gw_Config_St_t));

    if ((GwConfig.showlock & ZIP_GW_LOCK_ENABLE_BIT) != ZIP_GW_LOCK_ENABLE_BIT) //check whether lock is enabled or disabled
    {
      if (pCmd->ZW_GatewayModeSet.mode == ZIP_GW_MODE_STDALONE)
      {
        GwConfig.mode = ZIP_GW_MODE_STDALONE;
        GwConfig.peerProfile = 0; //Make peer profile invalid
        //Write back GW configuration;
        eeprom_write(EXT_GW_SETTINGS_START_ADDR, (u8_t*) &GwConfig, sizeof(Gw_Config_St_t));

        memset(cfg.portal_url, 0, sizeof(cfg.portal_url));

        TcpTunnel_ReStart();

      } else if (pCmd->ZW_GatewayModeSet.mode == ZIP_GW_MODE_PORTAL)
      {
        //if((GwConfig.peerProfile > 0) & (GwConfig.peerProfile <= MAX_PEER_PROFILES) )
        {
          GwConfig.mode = ZIP_GW_MODE_PORTAL;
          //Write back GW configuration;
          eeprom_write(EXT_GW_SETTINGS_START_ADDR, (u8_t*) &GwConfig, sizeof(Gw_Config_St_t));
        }
      }
    }
  }
    break;
  case GATEWAY_MODE_GET:
  {
    ZW_GATEWAY_MODE_REPORT_FRAME GatewayModeReport;
    Gw_Config_St_t GwConfig;

    //Read the GW configuration;
    eeprom_read(EXT_GW_SETTINGS_START_ADDR, (u8_t*) &GwConfig, sizeof(Gw_Config_St_t));

    if ((GwConfig.showlock & ZIP_GW_LOCK_SHOW_BIT_MASK) != ZIP_GW_LOCK_ENABLE_SHOW_DISABLE)
    {
      GatewayModeReport.cmdClass = COMMAND_CLASS_ZIP_GATEWAY;
      GatewayModeReport.cmd = GATEWAY_MODE_REPORT;
      GatewayModeReport.mode = GwConfig.mode;
      ZW_SendDataZIP(c, (BYTE*) &GatewayModeReport, sizeof(ZW_GATEWAY_MODE_REPORT_FRAME), NULL);
    }
  }
    break;

  case GATEWAY_PEER_SET:
  {
    Gw_Config_St_t GwConfig;
    u16_t len = 0;
    eeprom_read(EXT_GW_SETTINGS_START_ADDR, (u8_t*) &GwConfig, sizeof(Gw_Config_St_t));

    if ((GwConfig.showlock & ZIP_GW_LOCK_ENABLE_BIT) != ZIP_GW_LOCK_ENABLE_BIT) //check whether lock is enabled or disabled
    {
      if (GwConfig.mode == ZIP_GW_MODE_PORTAL)
      {
        if (pCmd->ZW_GatewayPeerSet.peerProfile == 1)
        {
          //check for the valid IPv6 address
          if ((!uip_is_addr_linklocal((uip_ip6addr_t *)&pCmd->ZW_GatewayPeerSet.ipv6Address1)) &&
          (!uip_is_addr_mcast((uip_ip6addr_t *)&pCmd->ZW_GatewayPeerSet.ipv6Address1)) &&
          (!uip_is_addr_loopback((uip_ip6addr_t *)&pCmd->ZW_GatewayPeerSet.ipv6Address1))
          ){
          if (uip_is_addr_unspecified((uip_ip6addr_t *)&pCmd->ZW_GatewayPeerSet.ipv6Address1) && (pCmd->ZW_GatewayPeerSet.peerNameLength == 0))
          {
            printf("Invalid profile settings.\r\n");
            break;
          }
          else if(!uip_is_addr_unspecified((uip_ip6addr_t *)&pCmd->ZW_GatewayPeerSet.ipv6Address1) &&
              !is_4to6_addr((ip6addr_t *)&pCmd->ZW_GatewayPeerSet.ipv6Address1)
          )
          {
            //Allow only 4 to 6 addresses for now.
            printf("Profile IPv6 addr is not 4 to 6 Ipv6 address.\r\n");
            break;
          }
          else
          {
            GwConfig.peerProfile = pCmd->ZW_GatewayPeerSet.peerProfile;
            GwConfig.actualPeers = 1;

            //write peer profile setting info
            eeprom_write(EXT_GW_SETTINGS_START_ADDR,(u8_t*) &GwConfig, sizeof(Gw_Config_St_t));

            printf("Received Profile Name len = %d\r\n", pCmd->ZW_GatewayPeerSet.peerNameLength);

            pCmd->ZW_GatewayPeerSet.peerNameLength &= 0x3F;//last two bits are reserved

            len = sizeof(ZW_GATEWAY_PEER_SET_FRAME)+ pCmd->ZW_GatewayPeerSet.peerNameLength - 4;

            printf("EEPROM Write Profile len = %u\r\n", len);

            //write the profile to eeprom..for get about the actual peer length
            eeprom_write(EXT_GW_PROFILE_START_ADDR, (u8_t*)&pCmd->ZW_GatewayPeerSet.ipv6Address1, len);

            parse_gw_profile((Gw_PeerProfile_St_t *)&pCmd->ZW_GatewayPeerSet.ipv6Address1);

            //start the gateway to connect to new portal server.
            tunnelTimer = ZW_LTimerStart(TcpTunnel_ReStart,UPGRADE_START_TIMEOUT,TIMER_ONE_TIME);
          }
        }
      }
    }
  }
}
break;
case GATEWAY_PEER_GET:
{
  Gw_PeerProfile_Report_St_t GwPeerProfileReport;
  Gw_Config_St_t GwConfig;
  u16_t len = 0;

  eeprom_read(EXT_GW_SETTINGS_START_ADDR,(u8_t*) &GwConfig, sizeof(Gw_Config_St_t));

  if((GwConfig.showlock & ZIP_GW_LOCK_SHOW_BIT_MASK) != ZIP_GW_LOCK_ENABLE_SHOW_DISABLE)
  {
    memset(&GwPeerProfileReport, 0, sizeof(Gw_PeerProfile_Report_St_t));
    GwPeerProfileReport.cmdClass = COMMAND_CLASS_ZIP_GATEWAY;
    GwPeerProfileReport.cmd = GATEWAY_PEER_REPORT;

    if ((pCmd->ZW_GatewayPeerGet.peerProfile == 1) && (GwConfig.actualPeers == 1))
    {
      GwPeerProfileReport.peerProfile = pCmd->ZW_GatewayPeerGet.peerProfile;
      GwPeerProfileReport.actualPeers = GwConfig.actualPeers;

      eeprom_read(EXT_GW_PROFILE_START_ADDR, (u8_t*)&GwPeerProfileReport.peer_ipv6_addr, sizeof(Gw_PeerProfile_St_t));

      len = sizeof(ZW_GATEWAY_PEER_REPORT_FRAME) -1 + GwPeerProfileReport.peerNameLength;

      //printf("GwPeerProfileReport.peerNameLength = %bu len = %u\r\n", GwPeerProfileReport.peerNameLength, len);
    }
    else
    {
      GwPeerProfileReport.peerProfile = 0;
      GwPeerProfileReport.actualPeers = GwConfig.actualPeers;
      GwPeerProfileReport.peerNameLength = 0;
      len = sizeof(ZW_GATEWAY_PEER_REPORT_FRAME) - 1;
      printf("Invalid Peer Get received.\r\n");
    }

    ZW_SendDataZIP(c, (u8_t*)&GwPeerProfileReport, len, NULL);
  }
}

break;

case GATEWAY_LOCK_SET:
{
  Gw_Config_St_t GwConfig;

  eeprom_read(EXT_GW_SETTINGS_START_ADDR,(u8_t*) &GwConfig, sizeof(Gw_Config_St_t));

  if(!(pCmd->ZW_GatewayLockSet.showEnable & ZIP_GW_LOCK_ENABLE_BIT))
  {
    //Accept unlocking only from the secured source ....
    if(gisTnlPacket == TRUE)
    {
      GwConfig.showlock = pCmd->ZW_GatewayLockSet.showEnable & ZIP_GW_LOCK_SHOW_BIT;
      //write the GW configuration back
      eeprom_write(EXT_GW_SETTINGS_START_ADDR,(u8_t*) &GwConfig, sizeof(Gw_Config_St_t));

      gGwLockEnable = FALSE;

      printf("Gateway Unlocked...\r\n");

    }
    else if((GwConfig.showlock & ZIP_GW_LOCK_ENABLE_BIT) != ZIP_GW_LOCK_ENABLE_BIT)
    {
      printf("Enable/disable show enable\r\n");
      GwConfig.showlock = (pCmd->ZW_GatewayLockSet.showEnable & ZIP_GW_LOCK_SHOW_BIT);
      //write the GW configuration back
      eeprom_write(EXT_GW_SETTINGS_START_ADDR,(u8_t*) &GwConfig, sizeof(Gw_Config_St_t));
    }
  }
  else
  {
    if(((GwConfig.showlock & ZIP_GW_LOCK_ENABLE_BIT) != ZIP_GW_LOCK_ENABLE_BIT))
    {
      printf("Locking the Gateway...\r\n");
      //Read the GW configuration;
      GwConfig.showlock = pCmd->ZW_GatewayLockSet.showEnable & 0x3;
      //write the GW configuration back
      eeprom_write(EXT_GW_SETTINGS_START_ADDR,(u8_t*) &GwConfig, sizeof(Gw_Config_St_t));

      gGwLockEnable = TRUE;
#ifdef __ASIX_C51__
              //Stop the lockset timer if it is running
              StopGwLockTimer();
#endif
            }
          }
        }

        break;

        case UNSOLICITED_DESTINATION_SET:
        {
          if( (!uip_is_addr_linklocal((uip_ip6addr_t *)&pCmd->ZW_GatewayUnsolicitDstSet.unsolicit_ipv6_addr[0])) &&
              (!uip_is_addr_mcast((uip_ip6addr_t *)&pCmd->ZW_GatewayUnsolicitDstSet.unsolicit_ipv6_addr[0])) &&
              (!uip_is_addr_loopback((uip_ip6addr_t *)&pCmd->ZW_GatewayUnsolicitDstSet.unsolicit_ipv6_addr[0]))
          )
          {
            char str[128];
            memcpy(&cfg.unsolicited_dest, &pCmd->ZW_GatewayUnsolicitDstSet.unsolicit_ipv6_addr[0], 16);
            cfg.unsolicited_port= ((unsigned int)pCmd->ZW_GatewayUnsolicitDstSet.unsolicitPort1 << 8)| pCmd->ZW_GatewayUnsolicitDstSet.unsolicitPort2;

            uip_ipaddr_sprint(str,&cfg.unsolicited_dest);
            config_update("ZipUnsolicitedDestinationIp6",str);
            sprintf(str,"%i", cfg.unsolicited_port);
            config_update("ZipUnsolicitedDestinationPort",str);
          }
        }
          break;

          case UNSOLICITED_DESTINATION_GET:
          {
            ZW_GATEWAY_UNSOLICITED_DESTINATION_REPORT_FRAME UnsolicitDstReport_st;

            UnsolicitDstReport_st.cmdClass = COMMAND_CLASS_ZIP_GATEWAY;
            UnsolicitDstReport_st.cmd = UNSOLICITED_DESTINATION_REPORT;

            memcpy(&UnsolicitDstReport_st.unsolicit_ipv6_addr[0], &cfg.unsolicited_dest, 16);
            UnsolicitDstReport_st.unsolicitPort1 = (cfg.unsolicited_port >> 8) & 0xFF;
            UnsolicitDstReport_st.unsolicitPort2 = cfg.unsolicited_port & 0xFF;
            ZW_SendDataZIP(c, (u8_t*)&UnsolicitDstReport_st, sizeof(ZW_GATEWAY_UNSOLICITED_DESTINATION_REPORT_FRAME), NULL);
          }

          break;

          case COMMAND_APPLICATION_NODE_INFO_SET:
          {
            application_nodeinfo_st_t appNodeInfo_st;
            ZW_GATEWAY_APP_NODE_INFO_SET_FRAME *f = (ZW_GATEWAY_APP_NODE_INFO_SET_FRAME *)pData;
            memset(&appNodeInfo_st, 0, sizeof(application_nodeinfo_st_t));

            if(cmd_cls_parse(&f->payload[0], bDatalen-2, &appNodeInfo_st.nonSecureCmdCls[0], &appNodeInfo_st.nonSecureLen,
                    &appNodeInfo_st.secureCmdCls[0], &appNodeInfo_st.secureLen) != 0)
            {
              char output[64*5];
              char *t = output;
              char *end = output + sizeof(output);
              cfg.extra_classes_len = appNodeInfo_st.nonSecureLen;
              int i;
              for (i = 0; i < appNodeInfo_st.nonSecureLen; i++)
              {
                t+=snprintf(t, end-t, "0x%02X ", appNodeInfo_st.nonSecureCmdCls[i]);
                cfg.extra_classes[i] = appNodeInfo_st.nonSecureCmdCls[i];
              }

              t+=snprintf(t, end-t, "0xF100 ");

              cfg.sec_extra_classes_len = appNodeInfo_st.secureLen;
              for (i = 0; i < appNodeInfo_st.secureLen; i++)
              {
                t+=snprintf(t,end-t, "0x%02X ", appNodeInfo_st.secureCmdCls[i]);
                cfg.sec_extra_classes[i] = appNodeInfo_st.secureCmdCls[i];
              }
              *t = 0;
              printf("output: %s\n", output);
              config_update("ExtraClasses", output);

              appNodeInfo_CC_Add();
            }

            }
            break;
            case COMMAND_APPLICATION_NODE_INFO_GET:
            {
              uint16_t len = 0;
              uint8_t tmplen = 0;
              uint8_t buf[128+2+2] =
              { 0}; //cmd class + cmd + commands and Security marker

              ZW_GATEWAY_APP_NODE_INFO_REPORT_FRAME *pNodeAppInfo_Report = (ZW_GATEWAY_APP_NODE_INFO_REPORT_FRAME *)buf;

              pNodeAppInfo_Report->cmdClass = COMMAND_CLASS_ZIP_GATEWAY;
              pNodeAppInfo_Report->cmd = COMMAND_APPLICATION_NODE_INFO_REPORT;

              len += 2;//cmd class and cmd

              if (cfg.extra_classes_len != 0)
              {
                memcpy(&pNodeAppInfo_Report->payload[0], cfg.extra_classes, cfg.extra_classes_len);
                len += cfg.extra_classes_len;
                tmplen = cfg.extra_classes_len;
              }

              if (cfg.sec_extra_classes_len!= 0)
              {
                //Fill Up the security Mark
                pNodeAppInfo_Report->payload[tmplen] = 0xF1;
                pNodeAppInfo_Report->payload[tmplen+1] = 0x00;
                len += 2;

                memcpy(&pNodeAppInfo_Report->payload[tmplen+2], cfg.sec_extra_classes, cfg.sec_extra_classes_len);
                len += cfg.sec_extra_classes_len;
              }

              ZW_SendDataZIP(c, (u8_t*)pNodeAppInfo_Report, len, NULL);
            }

            break;

            default:
            printf("Gateway_CommandHandler: Unsupported command received.\r\n");
            return COMMAND_NOT_SUPPORTED;
          }

  return COMMAND_HANDLED;
}

#define CMD_CLS_PARSE_MODE_INSECURE     0
#define CMD_CLS_PARSE_MODE_CONTROL      1
#define CMD_CLS_PARSE_MODE_SECURE       2

//Re-Used Logic from HC API code.
static int cmd_cls_parse(uint8_t *cmd_buf, uint8_t cmd_buf_len, uint8_t *cmd_cls_buf, uint8_t *cmd_cnt,
    uint8_t *cmd_cls_sec_buf, uint8_t *cmd_cnt_sec)
{
  unsigned i;
  int mode; // 0=insecure supported command classes; 1=insecure controlled command classes;
            // 2=secure supported command classes
  uint8_t cnt;
  uint8_t cnt_sec;

  if ((cmd_buf_len == 0) || !cmd_buf || !cmd_cls_buf || !cmd_cls_sec_buf)
  {
    return 0;
  }

  //Initialize the parser
  cnt_sec = cnt = i = 0;
  mode = CMD_CLS_PARSE_MODE_INSECURE;

  while (i < cmd_buf_len)
  {
    if (*cmd_buf >= 0xF1)
    { //Check whether this is the security scheme 0 marker
      if (*(cmd_buf + 1) == 0)
      { //Change to secure mode
        mode = CMD_CLS_PARSE_MODE_SECURE;

        cmd_buf += 2;
        i += 2;
        continue;
      }

      //Extended command class
      if (mode == CMD_CLS_PARSE_MODE_INSECURE)
      {
        cmd_cls_buf[cnt] = *cmd_buf++;
        cmd_cls_buf[cnt + 1] = (cmd_cls_buf[cnt] << 8) | (*cmd_buf++);
        cnt++;
      } else if (mode == CMD_CLS_PARSE_MODE_SECURE)
      {
        cmd_cls_sec_buf[cnt_sec] = *cmd_buf++;
        cmd_cls_sec_buf[cnt_sec + 1] = (cmd_cls_sec_buf[cnt_sec] << 8) | (*cmd_buf++);
        cnt_sec++;
      } else
      {
        cmd_buf += 2;
      }

      i += 2;
    } else
    {
      //Check whether this is the controlled command class marker
      if (*cmd_buf == 0xEF)
      { //Change mode
        mode = CMD_CLS_PARSE_MODE_CONTROL;

        cmd_buf++;
        i++;
        continue;
      }

      //Normal command class
      if (mode == CMD_CLS_PARSE_MODE_INSECURE)
      {
        cmd_cls_buf[cnt++] = *cmd_buf++;
      } else if (mode == CMD_CLS_PARSE_MODE_SECURE)
      {
        cmd_cls_sec_buf[cnt_sec++] = *cmd_buf++;
      } else
      {
        cmd_buf++;
      }
      i++;
    }
  }
  //Parsing done
  *cmd_cnt = cnt;
  *cmd_cnt_sec = cnt_sec;
  return 1;
}

#ifdef __ASIX_C51__
void GwDefaultConfigVersion_Get(uint8_t *pMajor, uint8_t *pMinor) CC_REENTRANT_ARG
{
  Gw_Config_St_t GwConfig;

  //Read the first block; Assuming that size of header is always less than 256 bytes
  eeprom_read(EXT_DEFAULT_CONFIG_START_ADDR, (u8_t*)&GwConfig, sizeof(Gw_Config_St_t));

  *pMajor = GwConfig.major;
  *pMinor = GwConfig.minor;

  return;
}
#endif

uint8_t validate_gw_profile(uint32_t addr, uint8_t isInt)
{
  Gw_Config_St_t GwConfig;
  Gw_PeerProfile_St_t GwPeerProfile;
  u8_t i = 0;

  if (isInt == TRUE)
  {
#ifdef __ASIX_C51__
    gconfig_ReadDataFromIntFlash(addr, &GwConfig, sizeof(Gw_Config_St_t));
#endif
  } else
  {
    eeprom_read(addr, (u8_t *) &GwConfig, sizeof(Gw_Config_St_t));
  }

  if (GwConfig.magic != GW_CONFIG_MAGIC)
  {
    printf("Gw profile: Invalid magic = 0x%x\r\n", GwConfig.magic);
    return FALSE;
  } else if (GwConfig.mode == ZIP_GW_MODE_PORTAL)
  {
    if (GwConfig.peerProfile == 0)
    {
      printf("No peer profiles defined..\r\n");
      return FALSE;
    } else if ((sizeof(Gw_Config_St_t) + (GwConfig.actualPeers * sizeof(Gw_PeerProfile_St_t)))
        > GW_SETTINGS_PORFILE_CONFIG_LEN)
    {
      printf("Number of peer profiles exceeded.\r\n");
      return FALSE;
    }

    addr += sizeof(Gw_Config_St_t);

    for (i = 1; i <= GwConfig.actualPeers; i++)
    {
      if (isInt == TRUE)
      {
#ifdef __ASIX_C51__
        gconfig_ReadDataFromIntFlash(addr, (uint8_t *)&GwPeerProfile, sizeof(Gw_PeerProfile_St_t));
#endif
      } else
      {
        eeprom_read(addr, (uint8_t *) &GwPeerProfile, sizeof(Gw_PeerProfile_St_t));
      }

      //check for the valid IPv6 address
      if ((!uip_is_addr_linklocal(&GwPeerProfile.peer_ipv6_addr)) &&
      (!uip_is_addr_mcast(&GwPeerProfile.peer_ipv6_addr)) &&
      (!uip_is_addr_loopback(&GwPeerProfile.peer_ipv6_addr))
      ){
      if (uip_is_addr_unspecified(&GwPeerProfile.peer_ipv6_addr) && (GwPeerProfile.peerNameLength == 0))
      {
        printf("Invalid profile settings.\r\n");
        return FALSE;
      }
      else if(!uip_is_addr_unspecified(&GwPeerProfile.peer_ipv6_addr) &&
          !is_4to6_addr((ip6addr_t*)&GwPeerProfile.peer_ipv6_addr)
      )
      {
        //Allow only 4 to 6 addresses for now.
        printf("Profile IPv6 addr is not 4 to 6 Ipv6 address.\r\n");
        return FALSE;
      }

    }

      addr += sizeof(Gw_PeerProfile_St_t);
    }

    return TRUE;
  } else if (GwConfig.mode == ZIP_GW_MODE_STDALONE)
  {
    //printf("GW is in stand alone mode.\r\n");
    return TRUE;
  }

  return FALSE;
}

#ifdef __ASIX_C51__
//Decfault config format: GW Settings + GW Profiles + Certificates
uint8_t validate_defaultconfig(uint32_t addr, uint8_t isInt, uint16_t *pOutGwSettingsLen) CC_REENTRANT_ARG
{
  Gw_Config_St_t GwConfig;
  uint16_t gwConfigLen = 0;

  if (validate_gw_profile(addr, isInt) == TRUE)
  {
    if(isInt == TRUE)
    {
      gconfig_ReadDataFromIntFlash(addr, (uint8_t *)&GwConfig, sizeof(Gw_Config_St_t));
    }
    else
    {
      eeprom_read(addr, (uint8_t *)&GwConfig, sizeof(Gw_Config_St_t));
    }

    gwConfigLen = sizeof(Gw_Config_St_t) + (GwConfig.actualPeers * sizeof(Gw_PeerProfile_St_t));

    if (gconfig_ValidateCertImg((addr + gwConfigLen), CERT_SCRATCH_PAD_LEN) != TRUE)
    {
      printf("Validation of Default config--Certs failed.\r\n");
      return FALSE;
    }
  }
  else
  {
    printf("Validation of Default config--GW settings failed.\r\n");
    return FALSE;
  }

  if(pOutGwSettingsLen)
  {
    *pOutGwSettingsLen = gwConfigLen;
  }
  return TRUE;

}
#endif







#if 0
/*---------------------------------------------------------------------------*/
/* Test adding of CCs to gateway. Intended for use when receiving
 *  COMMAND_APPLICATION_NODE_INFO_SET from portal. And when reloading
 *  those portal portal settings from NVM during startup.
 * */
void test_cc_add()
{
  BYTE xCC[64];
  BYTE i;
  xCC[0] = COMMAND_CLASS_GEOGRAPHIC_LOCATION;
  xCC[1] = COMMAND_CLASS_GROUPING_NAME;
  AddUnsocDestCCsToGW(xCC, 2);
  xCC[0] = COMMAND_CLASS_ENERGY_PRODUCTION;
  xCC[1] = COMMAND_CLASS_DCP_MONITOR;
  AddSecureUnsocDestCCsToGW(xCC, 2);
  for (i=2; i < 64; i++)
    xCC[i]=i;
  AddSecureUnsocDestCCsToGW(&xCC[2], 14);
  AddUnsocDestCCsToGW(xCC, 64);
}
#endif

void appNodeInfo_CC_Add(void) CC_REENTRANT_ARG
{
  BYTE class_buf[64] = {0};
  BYTE class_len = 0;

  reset_the_nif();

  if (cfg.extra_classes_len != 0)
  {
    AddUnsocDestCCsToGW(cfg.extra_classes, cfg.extra_classes_len);
  }

  if (cfg.sec_extra_classes_len!= 0)
  {
    AddSecureUnsocDestCCsToGW(cfg.sec_extra_classes, cfg.sec_extra_classes_len);
  }

  CommandClassesUpdated();

  return;
}


/*
  Check whether input command class exist in the support node info set list
*/
BYTE IsCCInNodeInfoSetList(BYTE cmdclass,BOOL secure) CC_REENTRANT_ARG
{
  BYTE class_buf[64] = {0};
  BYTE class_len = 0;
  BYTE i = 0;

  if (cfg.extra_classes_len != 0)
  {
    for(i=0; i<cfg.extra_classes_len; i++)
    {
      if (cfg.extra_classes[i] ==  cmdclass)
      {
        return TRUE;
      }
    }
  }


  if(secure) {
    if (cfg.sec_extra_classes_len != 0)
    {
      for(i=0; i < cfg.sec_extra_classes_len; i++)
      {
        if (cfg.sec_extra_classes[i] ==  cmdclass)
        {
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}


#ifdef __ASIX_C51__

static void gw_lockset_handler(void *d) CC_REENTRANT_ARG
{
  Gw_Config_St_t   GwConfig;

  eeprom_read(EXT_GW_SETTINGS_START_ADDR,(u8_t*) &GwConfig, sizeof(Gw_Config_St_t));

  if((GwConfig.showlock & ZIP_GW_LOCK_ENABLE_BIT) != ZIP_GW_LOCK_ENABLE_BIT)  //GW is unlocked
  {
    GwConfig.showlock = GwConfig.showlock | ZIP_GW_LOCK_ENABLE_BIT;
    //write the GW configuration back
    eeprom_write(EXT_GW_SETTINGS_START_ADDR,(u8_t*) &GwConfig, sizeof(Gw_Config_St_t));

    gGwLockEnable = TRUE;

    printf("GW lock handler and called and GW locked now.\r\n");
  }

  ctimer_stop(&gw_lockset_timer);
}

void StopGwLockTimer(void)
{
  if (!ctimer_expired(&gw_lockset_timer))
  {
    printf("Stopping gw lock timer.\r\n");
    ctimer_stop(&gw_lockset_timer);
  }

}

static void factory_reset_handler(void *d)
{
  if(gisPortalClose == TRUE)
  {
    printf("Portal unregister handler called.\r\n");
    process_exit(&zip_tcp_client_process);
    process_exit(&ntpclient_process);
    gisPortalClose = FALSE;
  }

  if(GPIO_READ(FACTORY_RESET_GPIO) ==  STATUS_LOW)
    {
    printf("Reset detected.\r\n");
    //call the reset functions and restart the device
    Reset_Gateway();

    StopGwLockTimer();

      ctimer_set(&gw_lockset_timer, GW_LOCK_SET_INTERVEL, gw_lockset_handler, 0);
  }

  ctimer_restart(&factory_reset_timer);

  return;
}


/*---------------------------------------------------------------------------*/
/*
  Checks the previously unfinished downloads(if any)  and  retry each time when device starts up.
  It starts factory reset timer to poll factory reset events periodically.
  It also starts GW lock timer if the gateways has not been locked explicitly.
  GW automatically locks itself after 24 hrs if it has not been locked explicitly.
  Reads the GW settings from ext NVM and load it to RAM.

*/
static void GwConfigCheck(void) CC_REENTRANT_ARG
{
    Gw_Config_St_t   GwConfig_St;
    Gw_PeerProfile_St_t GwPeerProfile;
    application_nodeinfo_with_magic_st_t  appNodeInfo_St;
    u8_t status = 0;
    u32_t dat_len = 0;
    u16_t config_len = 0;
    zw_fw_header_st_t zwfw_header;


    //check for the default GW configuration;
      eeprom_read(EXT_DEFAULT_CONFIG_START_ADDR, (u8_t *)&GwConfig_St, sizeof(GwConfig_St));

       //Flash Erased or new flash?
      if(GwConfig_St.magic != GW_CONFIG_MAGIC)
      {
        Set_DefaultConfig();
      }

     //Start factory reset timer
     ctimer_set(&factory_reset_timer, FACTORY_RESET_INTERVEL, factory_reset_handler, 0);

      //Check for ZW download status...
   eeprom_read(EXT_ZW_FW_UPGRADE_STATUS_ADDR,(u8_t*) &status, 1);

  if(status == ZW_FW_UPDATE_BACKUP_DONE)
  {
    eeprom_read(EXT_ZW_FW_DOWNLOAD_DATA_LEN_ADDR, (u8_t *)&dat_len, ZW_FW_DATA_LEN);
    eeprom_read(EXT_FW_START_ADDR, &zwfw_header, sizeof(zwfw_header));

     if(dat_len < zwfw_header.FileLen)
     {
      printf("Download len and Fw header len not matched.\r\n");
      status = ZW_FW_DOWNLOAD_IDLE;
      eeprom_write(EXT_ZW_FW_UPGRADE_STATUS_ADDR,(u8_t*) &status, 1);

     }
     else if(!((zwfw_header.Signature[0] == 'Z') && (zwfw_header.Signature[1] == 'W') && (zwfw_header.Signature[2] == 'F') && (zwfw_header.Signature[3] == 'W')))
     {
      printf("ZW Firmware signature doesn't match.\r\n");
      status = ZW_FW_DOWNLOAD_IDLE;
      eeprom_write(EXT_ZW_FW_UPGRADE_STATUS_ADDR,(u8_t*) &status, 1);
     }
     else
     {
       printf("Downloading ZW firmware..\r\n");
       SerialInit(cfg.serial_port);
       ZWFirmwareUpdate(FALSE);
     }
  }
  else if(status == ZW_FW_UPDATE_AP_SET_DONE)
  {
    eeprom_read(EXT_ZW_FW_DOWNLOAD_DATA_LEN_ADDR, (u8_t *)&dat_len, ZW_FW_DATA_LEN);

    if (gconfig_ValidatZwFwImg(EXT_FW_START_ADDR, dat_len, MAX_ZW_FWLEN) == TRUE)
    {
      printf("ZWF FW download retrying with status = 0x%bx\r\n", status);
      SerialInit(cfg.serial_port);
      ZWFirmwareUpdate(FALSE);
    }
    else
    {
      printf("No Valid ZW FW exists on external EEPROM.\r\n");
    }
  }


   //check for the default config downaload status
   eeprom_read(EXT_DEFAULT_CONFIG_UPGRADE_STATUS_ADDR, &status, DEFAULT_CONFIG_UPGRADE_STATUS_LEN);

   if(status == DEFAULT_CONFIG_UPDATE_IN_PROGRESS)
   {

    printf("Retrying default firmware download..\r\n");

    eeprom_read(EXT_DEFAULT_CONFIG_DATA_LEN_ADDR, &config_len, DEFAULT_CONFIG_DATA_LEN);

    if(MD5_ValidatFwImg(EXT_DEFAULT_CONFIG_SCRATCH_PAD_SART, config_len-16) != TRUE)
    {
      printf("Invalid md5 checksum: default config download failed.\r\n");

    }      //Validate the certificate and gw profile
    else if (validate_defaultconfig(EXT_DEFAULT_CONFIG_SCRATCH_PAD_SART, FALSE, NULL) != TRUE)
    {
      printf("Invalid checksum: default config  download failed.\r\n");
    }
    else
    {
      //copy the cert from the scratch pad to  default config partition
      gaconfig_WriteConfigToDefault(EXT_DEFAULT_CONFIG_SCRATCH_PAD_SART, EXT_DEFAULT_CONFIG_START_ADDR, config_len-16);

      if(validate_defaultconfig(EXT_DEFAULT_CONFIG_START_ADDR, FALSE, NULL) != TRUE)
      {
        printf("Error FATAL: default config update failed.\r\n");
      }
      else
      {
        status = DEFAULT_CONFIG_DOWNLOAD_IDLE;
        eeprom_write(EXT_DEFAULT_CONFIG_UPGRADE_STATUS_ADDR, &status, DEFAULT_CONFIG_UPGRADE_STATUS_LEN);
        printf("Default config download successful.\r\n");
      }
    }
   }


   memset(&appNodeInfo_St, 0, sizeof(application_nodeinfo_with_magic_st_t));

   eeprom_read(EXT_EXTRA_CLASS_CONFIG_PARTITION_START,(u8_t*) &appNodeInfo_St, sizeof(application_nodeinfo_with_magic_st_t));

   if(appNodeInfo_St.magic != ZIPMAGIC)
   {
     memset(&appNodeInfo_St, 0, sizeof(application_nodeinfo_with_magic_st_t));
     appNodeInfo_St.magic = ZIPMAGIC;
     eeprom_write(EXT_EXTRA_CLASS_CONFIG_PARTITION_START,(u8_t*) &appNodeInfo_St, sizeof(application_nodeinfo_with_magic_st_t));
   }

  //Parse the Gateway Profile
  eeprom_read(EXT_GW_SETTINGS_START_ADDR,(u8_t*) &GwConfig_St, sizeof(GwConfig_St));

  if((GwConfig_St.magic == GW_CONFIG_MAGIC) && (GwConfig_St.mode == ZIP_GW_MODE_PORTAL) && (GwConfig_St.peerProfile == 1))
  {
    eeprom_read(EXT_GW_PROFILE_START_ADDR,(u8_t*) &GwPeerProfile, sizeof(Gw_PeerProfile_St_t));
    parse_gw_profile(&GwPeerProfile);
  }
  else
  {
  #if 0
    GwConfig_St.magic =  GW_CONFIG_MAGIC;
    GwConfig_St.mode = 0x1;
    GwConfig_St.showlock = 0x0;
    eeprom_write(EXT_GW_SETTINGS_START_ADDR,(u8_t*) &GwConfig_St, sizeof(GwConfig_St));
  #endif

    memset(cfg.portal_url, 0, sizeof(cfg.portal_url));
  }

    eeprom_read(EXT_GW_SETTINGS_START_ADDR,(u8_t*) &GwConfig_St, sizeof(Gw_Config_St_t));
  if((GwConfig_St.showlock & ZIP_GW_LOCK_ENABLE_BIT) != ZIP_GW_LOCK_ENABLE_BIT)  //GW is unlocked
  {
    printf("start Gw lock timer\r\n");
    gGwLockEnable = FALSE;
    ctimer_set(&gw_lockset_timer, GW_LOCK_SET_INTERVEL, gw_lockset_handler, 0);
  }

}
#endif



REGISTER_HANDLER(
    Gateway_CommandHandler,
    0,
    COMMAND_CLASS_ZIP_GATEWAY, ZIP_GATEWAY_VERSION, SECURITY_SCHEME_UDP);

