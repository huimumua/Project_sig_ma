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
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "ZIP_Router.h"
#include "provisioning_list.h"
#include "contiki.h"
#include "contiki-net.h"
#include "node_queue.h"
#include "mDNSService.h"

#include "ipv4_interface.h"
#include "Serialapi.h"
#include "dev/serial-line.h"

#include "ZW_SendRequest.h"
#include "ZW_udp_server.h"
#include "ZW_zip_classcmd.h"
#include "ZW_SendDataAppl.h"
#ifdef SECURITY_SUPPORT
#include "security_layer.h"
#endif
#include "transport_service2.h"
#include "ZW_ZIPApplication.h"
#include "NodeCache.h"

#include "CC_NetworkManagement.h"
#include "Mailbox.h"

#include "ipv46_nat.h"
#include "dhcpc2.h"

#include "serial_api_process.h"
#define DEBUG DEBUG_FULL
#include "net/uip.h"
#include "net/uip-ds6.h"
#include "net/uip-icmp6.h"
#include "net/uip-debug.h"
#include "net/uiplib.h"

#include "CC_Gateway.h"
#include "lib/rand.h"

#include "ClassicZIPNode.h"
#include "NodeCache.h"
#include "ResourceDirectory.h"
#include "security_layer.h"
#include "s2_keystore.h"
#include "S2_wrap.h"

#include "eeprom_layout.h"
#include "ZW_tcp_client.h"
#include "ZW_udp_server.h"
#include "CC_FirmwareUpdate.h"
#include "DTLS_server.h"

//#include "ZW_queue.h"
#include "Bridge.h"
#include "DataStore.h"
#include "ZW_tcp_client.h"
#include "S2.h"
//Asix mac header file to enable MAC filtering.
#ifdef __ASIX_C51__
#include "reg80390.h"
#include "asix_mac.h"
#include "axled.h"
#include "gconfig.h"

int ZWFirmwareUpdate(unsigned char isAPM) REENTRANT;
int SerialInit(const char* port);
void TcpTunnel_ReStart(void);
uint8_t validate_defaultconfig(uint32_t  addr, uint8_t isInt, uint16_t *pOutGwSettingsLen) CC_REENTRANT_ARG;
uint8_t MD5_ValidatFwImg(uint32_t baseaddr, uint32_t len) REENTRANT;
void gaconfig_WriteConfigToDefault(U32_T srcaddr, U32_T dstaddr, U16_T defconflen) REENTRANT;
#endif

#include "dev/eeprom.h"

#include "ZW_ZIPApplication.h"
#include "uip-packetqueue.h"

#include "command_handler.h"

#define TX_QUEUE_LIFETIME 10000

#ifdef DEBUG_WEBSERVER
#include "webserver-nogui.h"
#endif

extern BYTE serial_ok;

extern void
AddSecureUnsocDestCCsToGW(BYTE *ccList, BYTE ccCount) CC_REENTRANT_ARG;
extern const char* linux_conf_provisioning_cfg_file;
extern const char* linux_conf_provisioning_list_storage_file;
extern const char* linux_conf_fin_script;
/*---------------------------------------------------------------------------*/PROCESS(
    zip_process, "ZIP process");
PROCESS_NAME(udp_server_process);
PROCESS_NAME(coap_server_process);
PROCESS_NAME(mDNS_server_process);
PROCESS_NAME(dtls_server_process);
PROCESS_NAME(zip_tcp_client_process);
PROCESS_NAME(resolv_process);
#ifdef __ASIX_C51__
PROCESS_NAME(tcpip_ipv4_process);
PROCESS_NAME(dhcp_client_process);
PROCESS_NAME(ntpclient_process);
#endif

#ifndef __ASIX_C51__
AUTOSTART_PROCESSES(&zip_process);
#endif


VOID_CALLBACKFUNC(uip_completedFunc)(BYTE,void*);

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])
#define IPBUF ((struct uip_tcpip_hdr *)&uip_buf[UIP_LLH_LEN])
uint16_t dag_id[] =
{ 0x1111, 0x1100, 0, 0, 0, 0, 0, 0x0011 };

#define MAGIC 0xee




/*global configuration*/
struct router_config cfg;
BYTE MyNodeID; // ID of this node
DWORD homeID;
controller_role_t controller_role;

extern void
tcpip_ipv4_force_start_periodic_tcp_timer(void);

#ifdef TEST_MULTICAST_TX
uint8_t sec2_send_multicast_nofollowup(uint8_t src_node, const uint8_t* node_list, uint8_t node_list_len,const uint8_t * data, uint8_t data_len,ZW_SendDataAppl_Callback_t callback);
#endif


/* Dummy linklayer address based on HOME ID used to track where packages come from. */
uip_lladdr_t pan_lladdr;
const uip_lladdr_t tun_lladdr =
  {
    { 0xFF, 0xFF, 0xEE, 0xEE, 0xEE, 0xEE } };

static u8_t
(*landev_send)(uip_lladdr_t *a);
static u8_t ipv4_init_once = 0;


void
Ext_Eeprom_Config_Read(u32_t baddr, u32_t start, void* dst, u16_t size)
{
    eeprom_read(baddr+start ,dst, size);
}

void
Ext_Eeprom_Config_Write(u32_t baddr, u32_t start, void* dst, u16_t size)
{
   eeprom_write(baddr+start ,dst, size);
}

u8_t
is_4to6_addr_check(uip_ip6addr_t* ip)
{
  return (ip->u16[0] == 0 && ip->u16[1] == 0 && ip->u16[2] == 0
      && ip->u16[3] == 0 && ip->u16[4] == 0 && ip->u16[5] == 0xFFFF);
}

void
set_landev_outputfunc(u8_t
(*f)(uip_lladdr_t *a))
{
  landev_send = f;
}

/*static void debug_print_time() {
    time_t     now;
    struct tm  ts;
    char       buf[80];
    // Get the current time
    time(&now);
    // Format and print the time, "hh:mm:ss "
    ts = *localtime(&now);
    strftime(buf, sizeof(buf), "%H:%M:%S", &ts);
    printf("%s", buf);
}*/

/**
 * Create a ULA prefix according to rfc4193
 */
void
create_ula_prefix(uip_ip6addr_t* a, u16_t subnet_id)
{
  a->u16[0] = UIP_HTONS((0xFD << 8) | (random_rand() & 0xFF));
  a->u16[1] = random_rand();
  a->u16[2] = random_rand();
  a->u16[3] = UIP_HTONS(subnet_id);
  a->u16[4] = 0;
  a->u16[5] = 0;
  a->u16[6] = 0;
  a->u16[7] = 0;
}

/*
 * Get the ipaddress of a given node
 */
void
ipOfNode(uip_ip6addr_t* dst, u8_t nodeID)
{
  if (nodeID == MyNodeID)
  {
    uip_ipaddr_copy(dst,&cfg.lan_addr);
  }
  else
  {
    uip_ipaddr_copy(dst,&cfg.pan_prefix);
    dst->u8[15] = nodeID;
  }
}


/**
 * Get the mac address of a node. For MyNodeID this returns
 * the device mac address which should be globally unique.
 * For the Z-Wave nodes we use locally administered MAC addresses.
 *
 * The locally administered address MUST be created in a way so
 * multiple Z/IP gateways can coincide in the same network. If the
 * gateway are member of the same Z-Wave network(HomeID) they will use the
 * same MAC range.
 */
void macOfNode(uip_lladdr_t* dst, u8_t nodeID) {
  if(nodeID == MyNodeID) {
    memcpy(dst,&uip_lladdr, sizeof(uip_lladdr_t));
  } else {
    dst->addr[0] = 2; //Use the upper 6 bits and set the local bit
    dst->addr[1] = ((homeID>>24) & 0xff);
    dst->addr[2] = ((homeID>>16) & 0xff);
    dst->addr[3] = (homeID>>8) & 0xFF;
    dst->addr[4] = (homeID & 0xFF);
    dst->addr[5] = nodeID;
  }
}

/**
 * Return the node is corresponding to an ip address, returns 0 if the
 * ip address is not in the pan
 */BYTE
nodeOfIP(uip_ip6addr_t* ip)
{
  if (uip_ipaddr_prefixcmp(ip,&cfg.pan_prefix,64))
  {
		return ip->u8[15];
  }

  if (uip_ds6_is_my_addr(ip))
  {
    return MyNodeID;
  }

  else if (is_4to6_addr((ip6addr_t*) ip))
  {
    //return ipv46_get_nat_addr((uip_ipv4addr_t*) &ip->u8[12]); //djnakata
    if(ip->u8[15] == 1)
    {
      return MyNodeID;
    }

    return ip->u8[15];
  }
		return 0;
	}

/**
 *   Search through a received multi cmd encap and notify mailbox if a Wake Up Notification is found.
 *   Separated into own function to ease unit test.
 */
static void find_WUN_in_multi(
    ts_param_t *p,
    ZW_APPLICATION_TX_BUFFER *pCmd,
    WORD cmdLength) CC_REENTRANT_ARG
{
  int i;
  int no_cmd = pCmd->ZW_MultiCommandEncapFrame.numberOfCommands; /*Get number of commands to iterate */
  int len = 0;
  int off = 3; /* COMMAND_CLASS_MULTI_CMD + MULTI_CMD_ENCAP + number of cmds */
  uint8_t *data = (uint8_t *)pCmd;
  uint8_t *data_t;
  security_scheme_t snode_scheme = highest_scheme(GetCacheEntryFlag(p->snode));

  DBG_PRINTF("Received multi cmd encap\n");
  for ( i = 0; i < no_cmd; i++) {
      //uint8_t* pCmd_m = &data[3+1+len]; /*Beginning of the cmd inside multi encap*/
      off += 1 + len;
      if (off >= cmdLength) {
        /* no_of_cmd exceeded command length, bail out */
        return;
      }
      ZW_APPLICATION_TX_BUFFER* pCmd_m = (ZW_APPLICATION_TX_BUFFER *)&data[off]; /*Beginning of the cmd inside multi encap*/
      len = (uint8_t)data[off-1]; /* cmd length of the cmd inside multi cmd*/
      if ((pCmd_m->ZW_Common.cmdClass == COMMAND_CLASS_WAKE_UP) && (pCmd_m->ZW_Common.cmd == WAKE_UP_NOTIFICATION))
      {
          DBG_PRINTF("wake up notification inside multi cmd encap\n");
          if ((snode_scheme!=SECURITY_SCHEME_0) && (p->scheme != snode_scheme)) {
              ERR_PRINTF("Wake up notification from node:% is received on lower security level. Ignoring\n", p->snode);
              break;
          }
          if (
              (pCmd_m->ZW_WakeUpNotificationFrame.cmd == WAKE_UP_NOTIFICATION) &&
              (cfg.mb_conf_mode != DISABLE_MAILBOX))
             {
               mb_wakeup_event(p->snode, (p->rx_flags & RECEIVE_STATUS_TYPE_BROAD) != 0);
             }
      }
  }
}

/*========================   ApplicationCommandHandler   ====================
 **    Handling of a received application commands and requests
 **
 ** This is an handler for all incomming non-IP frames. This is not only RAW z-wave frames
 ** but also decrypted and multicannel stripped frames.
 **
 **--------------------------------------------------------------------------*/
void
ApplicationCommandHandlerZIP(ts_param_t *p, ZW_APPLICATION_TX_BUFFER *pCmd,
    WORD cmdLength) CC_REENTRANT_ARG
{
  if (cmdLength == 0)
    return;

  /*
   * Check if this was a reply to a request we have made
   */
  if (SendRequest_ApplicationCommandHandler(p, pCmd, cmdLength))
  {
	  return;
  }
  security_scheme_t snode_scheme = highest_scheme(GetCacheEntryFlag(p->snode));

  DBG_PRINTF("ApplicationCommandHandler %d->%d class 0x%02x cmd 0x%02x size %d\n",
      (int )p->snode,(int)p->dnode, (int )pCmd->ZW_Common.cmdClass,(int )pCmd->ZW_Common.cmd, (int )cmdLength);

  switch (pCmd->ZW_Common.cmdClass)
  {
    case COMMAND_CLASS_CONTROLLER_REPLICATION:
      ZW_ReplicationReceiveComplete();
    break;
    case COMMAND_CLASS_MULTI_CMD:
      if (pCmd->ZW_Common.cmd == MULTI_CMD_ENCAP) {
        find_WUN_in_multi(p, pCmd, cmdLength);
      }
    break;
    case COMMAND_CLASS_WAKE_UP:
      if (
          (pCmd->ZW_WakeUpNotificationFrame.cmd == WAKE_UP_NOTIFICATION) &&
          (cfg.mb_conf_mode != DISABLE_MAILBOX))
      {
        /* S2 nodes MUST send WUN on highest level, and we filter them out otherwise. We don't filter S0 nodes
         * because they are allowed to use either level. */
        if ((snode_scheme!=SECURITY_SCHEME_0) && (p->scheme != snode_scheme)) {
            ERR_PRINTF("Wake up notification from node:% is received on lower security level. Ignoring\n", p->snode);
            break;
        }
        mb_wakeup_event(p->snode, (p->rx_flags & RECEIVE_STATUS_TYPE_BROAD) != 0);
        return;
      }
      break;
    case COMMAND_CLASS_CRC_16_ENCAP:
      if (pCmd->ZW_Common.cmd != CRC_16_ENCAP)
      {
        return;
      }
      if (p->scheme != NO_SCHEME)
      {
        WRN_PRINTF("Security encapsulated CRC16 frame received. Ignoring.\n");
        return;
      }

      if (ZW_CheckCrc16(CRC_INIT_VALUE, &((BYTE*) pCmd)[0], cmdLength) == 0)
      {
        p->scheme = USE_CRC16;
        ApplicationCommandHandlerZIP(p,
            (ZW_APPLICATION_TX_BUFFER*) ((BYTE*) pCmd + 2), cmdLength - 4);
      }
      else
      {
        ERR_PRINTF("CRC16 Checksum failed\n");
      }
      return;
    case COMMAND_CLASS_SECURITY:
      if (pCmd->ZW_Common.cmd != SECURITY_COMMANDS_SUPPORTED_REPORT)
      {
        security_CommandHandler(p,pCmd, cmdLength); /* IN Number of command bytes including the command */
        return;
      }
      break;
    case COMMAND_CLASS_SECURITY_2:
      security2_CommandHandler(p,pCmd,cmdLength);
      if(pCmd->ZW_Common.cmd == 0x0E)
      {
        // Security 2 commands supported report
        // Need report to application layer
        goto END_CASE;
      }
      return;
      break;
    case COMMAND_CLASS_TRANSPORT_SERVICE:
      if (p->scheme != NO_SCHEME)
      {
        WRN_PRINTF("Security encapsulated transport service frame received. Ignoring.\n");
        return;
      }
      TransportService_ApplicationCommandHandler(p, (BYTE *)pCmd,
          cmdLength);
      return;
  case COMMAND_CLASS_MULTI_CHANNEL_V2:
      if (pCmd->ZW_MultiChannelCmdEncapV2Frame.cmd == MULTI_CHANNEL_CMD_ENCAP_V2
          && cmdLength > 4)
    {
      /*Strip off multi cannel encap */
      p->sendpoint = pCmd->ZW_MultiChannelCmdEncapV2Frame.properties1;
      p->dendpoint = pCmd->ZW_MultiChannelCmdEncapV2Frame.properties2;
        ApplicationCommandHandlerZIP(p,
            (ZW_APPLICATION_TX_BUFFER*) &pCmd->ZW_MultiChannelCmdEncapV2Frame.encapFrame,
            cmdLength - 4);
      return;
    }
  } // end case

  /* In general, frames to other nodeIDs than ours are for virtual nodes, and should be directed to bridge_virtual_node_commandhandler.
   * But there is an exception: Frames received via S2 multicast have their destination nodes altered to the multicast group ID by libs2.
   * They also have the RECEIVE_STATUS_TYPE_BROAD set. Such frames never go to the bridge_..._commandhandler().
   * Note: This check does not allow S2 multicast to virtual nodes. So far, this is not a feature the ZIPGW supports.
   * All S2 multicast will go to the unsolicited destination, regardless of IP associations. */
  /* TODO: Support S2 multicast to ZIP clients via IP Association (filed under ZGW-935). */
  END_CASE:
  if( !(p->rx_flags & RECEIVE_STATUS_TYPE_BROAD) &&
      (p->dnode != MyNodeID) && bridge_virtual_node_commandhandler(p, (BYTE*) pCmd, cmdLength)
    ) {
      //Frames targeted virtual nodes should not go to the unsolicited destination.
    DBG_PRINTF("bridge_virtual_node_commandhandler Handled\n");
    node_input_queued(p->dnode); //djnakata
      return;
  }

  /*
   * FIXME:
   * The address 0::nodeID is interpreted as a Z-Wave address. This is sort of broken
   * We should instead derive the source address with ipOfNode()
   * Likewise SendData_ZIP should use nodeOfIP or general routing to determine if the package
   * be sent via the Z-wave or not
   */
  zwave_connection_t c;
  memset(&c, 0, sizeof(c));

  /*
    Note: the lipaddr address is set back to ripaddr in ClassicZIPNode_SendUnsolicited()
    for sending unsol packets. Because ZIP Gateway is forwarding the packet */

  ipOfNode(&c.ripaddr,p->snode);
  ipOfNode(&c.lipaddr,p->dnode);
  /* Check if command should be handled by the Z/IP router itself */
  c.scheme = p->scheme;
  c.rendpoint = p->sendpoint;
  c.lendpoint = p->dendpoint;
  c.rx_flags = p->rx_flags;

  ApplicationIpCommandHandler(&c, (BYTE*) pCmd,  cmdLength);
}

/**
 * Input for RAW Z-Wave frames
 */
void
ApplicationCommandHandlerSerial(BYTE rxStatus, BYTE destNode, BYTE sourceNode,
    ZW_APPLICATION_TX_BUFFER *pCmd, BYTE cmdLength)CC_REENTRANT_ARG
{

  ts_param_t p;
  p.dendpoint = 0;
  p.sendpoint = 0;

  p.rx_flags = rxStatus;
  p.tx_flags = ((rxStatus & RECEIVE_STATUS_LOW_POWER) ?
      TRANSMIT_OPTION_LOW_POWER : 0) | TRANSMIT_OPTION_ACK
      | TRANSMIT_OPTION_EXPLORE | TRANSMIT_OPTION_AUTO_ROUTE;


  p.scheme = NO_SCHEME;
  p.dnode = destNode ? destNode : MyNodeID;
  p.snode = sourceNode;

  ApplicationCommandHandlerZIP(&p,pCmd, cmdLength);
  rd_node_is_alive(p.snode);
}

int zip_is_sending = 0;

/*TODO consider a double buffer scheme */
#define UIP_ICMP_BUF ((struct uip_icmp_hdr *)&uip_buf[UIP_LLIPH_LEN + uip_ext_len])
#define UIP_IP_BUF ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
/*
 * This is the output function of uip.
 * \parm addr is the linklayer(L2) destination address.
 * We use this address to check which physical network the package should
 * go out on.
 * If adder is 0, then package should be multicasted to all interfaces.
 */
static u8_t
zwave_send(uip_lladdr_t *addr)
{

  /* Is Unicast? */
  if (addr)
  {
    /*    DBG_PRINTF("  %02x:%02x:%02x:%02x:%02x:%02x length %i",
     pan_lladdr.addr[0],
     pan_lladdr.addr[1],
     pan_lladdr.addr[1],
     pan_lladdr.addr[2],
     pan_lladdr.addr[3],
     pan_lladdr.addr[5],uip_len);*/

    if(memcmp(addr->addr, tun_lladdr.addr, 6) == 0)  /* Is L2 address in portal */
    {
      send_ipv6_to_tcp_ssl_client();
    }
    else if (memcmp(addr->addr, pan_lladdr.addr, 5) == 0) /* Is L2 address on PAN? */
    {
      node_input_queued(addr->addr[5]);
      uip_len =0;
    }
    else
    {
      if (landev_send)
      {
         //DBG_PRINTF("Send to lan\n");
         landev_send(addr);
      }
    }
  }
  else
  {
    /*Do not send RPL messages to LAN. It is not very pretty
     * to do package IP package inspection in a L2 function,
     * but I think this is the only way right now. */
    if (!(UIP_IP_BUF ->proto == UIP_PROTO_ICMP6
        && UIP_ICMP_BUF ->type == ICMP6_RPL))
    {
      /* Multicast, send to all interfaces */
      if(landev_send) { //djnakata
          landev_send(addr);
      }
    }
  }
  if (uip_completedFunc)
  {
   uip_completedFunc(TRANSMIT_COMPLETE_OK,0);
  }
  uip_completedFunc = 0;
  return 0;
}

/* Generate Ethernet MAC address*/
static void
permute_l2_addr(uip_lladdr_t* a)
{
  uint32_t seed;

  seed = (MyNodeID << 24) | (MyNodeID << 16) | (MyNodeID << 8)
      | (MyNodeID << 0);
  seed ^= homeID;
  seed ^= (a->addr[3] << 16) | (a->addr[4] << 8) | (a->addr[5] << 0);
  srand(seed);

  a->addr[0] = 0x00; //IEEE OUI Assignment for Zensys
  a->addr[1] = 0x1E; //
  a->addr[2] = 0x32; //
  a->addr[3] = 0x10 | (random_rand() & 0x0F);
  a->addr[4] = random_rand() & 0xFF;
  a->addr[5] = random_rand() & 0xFF;
}

#ifdef NO_ZW_NVM
extern const char* linux_conf_nvm_file;
static int nvm_file = 0;

void nvm_init() {
  if(nvm_file==0) {
    nvm_file = open(linux_conf_nvm_file, O_RDWR | O_CREAT , 0644);
  }

  if(nvm_file<0) {
    fprintf(stderr, "Error opening NVM file %s\n",linux_conf_nvm_file);
    perror("");
    exit(1);
  }
}
void
NVM_Read(u16_t start, void* dst, u8_t size)
{
  lseek(nvm_file, start, SEEK_SET);
  if(read(nvm_file,dst,size) != size) {
    perror(__FUNCTION__);
  }
}


void
NVM_Write(u16_t start, void* dst, u8_t size)
{
  lseek(nvm_file, start, SEEK_SET);
  if(write(nvm_file,dst,size) != size) {
    perror(__FUNCTION__);
  }
}

#else
/**
 * Read NVM of Z-Wave module
 */
void
NVM_Read(u16_t start, void* dst, u8_t size)
{
  MemoryGetBuffer( EEOFFSET_MAGIC_START+start ,dst, size);
}

/**
 * Write NVM of Z-Wave module
 */
void
NVM_Write(u16_t start,const void* dst, u8_t size)
{
  MemoryPutBuffer(EEOFFSET_MAGIC_START+start,(BYTE*)dst,size,0);
}
#endif

/**
 * Write a new pan  NWM to Z-Wave module
 */
static void
new_pan_ula() REENTRANT
{

  uip_ip6addr_t pan_prefix;
  LOG_PRINTF("Entering new network\n");
  create_ula_prefix(&pan_prefix,2);
  NVM_Write(offsetof(zip_nvm_config_t,pan_prefix), &pan_prefix,
      sizeof(pan_prefix));
}



/**
 * Read appropriate settings from Z-Wave nvm. This is stuff like generated ULA addresses
 */
static void
ZIP_eeprom_init() CC_REENTRANT_ARG
{
  uip_ip6addr_t ula;
  u32_t magic=0;
  u16_t version = NVM_CONFIG_VERSION;
  Gw_Config_St_t   GwConfig_St;

  nvm_config_get(magic,&magic);

  DBG_PRINTF("Magic check %x  == %x\n",magic, ZIPMAGIC );
  if (magic != ZIPMAGIC || cfg.clear_eeprom)
  {
	  //In case of ZIPR, we will get mac address from I2C EEPROM/Asix registers
#ifndef __ASIX_C51__
    WRN_PRINTF("Writing new MAC addr\n");
    memset(&uip_lladdr,0,sizeof(uip_lladdr));
    permute_l2_addr(&uip_lladdr);
#endif
    magic = ZIPMAGIC;
    nvm_config_set(magic,&magic);
    nvm_config_set(config_version,&version);

    nvm_config_set(mac_addr,&uip_lladdr);

    /*Create a ULA LAN address*/
    create_ula_prefix(&ula,1);
    nvm_config_set(lan_prefix,&ula);

    cfg.mb_conf_mode = DISABLE_MAILBOX; //djnakata //ENABLE_MAILBOX_SERVICE;
    cfg.mb_port=ZWAVE_PORT;
    ApplicationDefaultSet();

    /* Create a new PAN ULA address */
    new_pan_ula();
    sec2_create_new_static_ecdh_key();
    sec2_create_new_network_keys();
  }
  /*Migrate nvm for old version*/
  nvm_config_get(config_version,&version);
  DBG_PRINTF("NVM version is %i\n",version);
  if(version <= 1) {
    version = NVM_CONFIG_VERSION;
    sec2_create_new_static_ecdh_key();
    sec2_create_new_network_keys();
    nvm_config_set(config_version,&version);
    /* This is to adapt new network scheme */
    ApplicationInitSW();

  }

  //check for the default GW configuration;
  eeprom_read(EXT_GW_SETTINGS_START_ADDR, (u8_t *)&GwConfig_St, sizeof(GwConfig_St));
   //Flash Erased or new flash?
  if(GwConfig_St.magic != GW_CONFIG_MAGIC)
  {
    /*Initialize the GW config TODO this should be a part of nvm_config */
    GwConfig_St.magic =  GW_CONFIG_MAGIC;
    GwConfig_St.mode = ZIP_GW_MODE_STDALONE;
    GwConfig_St.showlock = 0x0;
    eeprom_write(EXT_GW_SETTINGS_START_ADDR,(u8_t*) &GwConfig_St, sizeof(GwConfig_St));
  }


  //Sasidhar: in ZIPR, we have L2 address from ethernet controller vendor
  #ifndef __ASIX_C51__
    /* TODO Store the L2 address in EEPROM and read it from the EEPROM,
     * Use IPv6 DAD to ensure there are no devices on the network which has the same L2 address
     * */
    nvm_config_get(mac_addr,&uip_lladdr);

    LOG_PRINTF("L2 HW addr ");
    PRINTLLADDR(&uip_lladdr);
    LOG_PRINTF("\n");
  #endif

  /*If lan and pan prefixes are not given in config file, use ULA addresses*/
  //uip_debug_ipaddr_print(&uip_all_zeroes_addr);

  if (uip_is_addr_unspecified(&cfg.cfg_lan_addr))
  {
    nvm_config_get(lan_prefix,&cfg.lan_addr);

    if (uip_is_addr_unspecified(&cfg.lan_addr))
    {
      /*Create a ULA LAN address*/
      create_ula_prefix(&cfg.lan_addr,1);
      nvm_config_set(lan_prefix,&cfg.lan_addr);
    }

    /*Create autoconf address */
    uip_ds6_set_addr_iid(&cfg.lan_addr, &uip_lladdr);
    cfg.lan_prefix_length = 64;
    LOG_PRINTF("Using ULA address for LAN \n");
  }
  else
  {
    cfg.lan_prefix_length = cfg.cfg_lan_prefix_length;
    uip_ipaddr_copy(&cfg.lan_addr,&cfg.cfg_lan_addr);
  }

  if (uip_is_addr_unspecified(&cfg.cfg_pan_prefix))
  {
    nvm_config_get(pan_prefix,&cfg.pan_prefix);
    if (uip_is_addr_unspecified(&cfg.pan_prefix))
    {
      /*Create a ULA LAN address*/
      create_ula_prefix(&cfg.pan_prefix,2);
      nvm_config_set(pan_prefix,&cfg.pan_prefix);
    }
    LOG_PRINTF("Using ULA address for HAN\n");
  }
  else
  {
    uip_ipaddr_copy(&cfg.pan_prefix,&cfg.cfg_pan_prefix);
  }
}

/**
 * Reread all IPv6 addresses from cfg structure
 */
void
refresh_ipv6_addresses() CC_REENTRANT_ARG
{
  uip_ipaddr_t ipaddr;
  const uip_ipaddr_t ipv4prefix =  { .u16 = {0,0,0,0,0,0xFFFF,0,0} };

  /*Clear all old addresses*/
  uip_ds6_init();

  uip_ipaddr_copy(&ipaddr,&(cfg.pan_prefix));
  ipaddr.u8[15] = MyNodeID;
  uip_ds6_addr_add(&ipaddr, 0, ADDR_MANUAL);

  /*Add Lan prefix */
  uip_ds6_prefix_add(&cfg.lan_addr, cfg.lan_prefix_length, 1,
      UIP_ND6_RA_FLAG_ONLINK | UIP_ND6_RA_FLAG_AUTONOMOUS, 2 * 60 * 60,
      2 * 60 * 60);

  /* Add lan IP address */
  uip_ds6_addr_add(&cfg.lan_addr, 0, ADDR_MANUAL);

  /* Add default route */
  if (!uip_is_addr_unspecified(&cfg.gw_addr))
  {
        uip_ds6_defrt_add(&cfg.gw_addr, 0);
    }

  /*Add pan prefix */
  uip_ds6_prefix_add(&(cfg.pan_prefix), 64, 0, 0, 0, 0);

  if (!uip_is_addr_unspecified(&(cfg.tun_prefix)))
  {
    /*Add tun prefix */
    uip_ds6_prefix_add(&(cfg.tun_prefix), cfg.tun_prefix_length, 1, 0, 0, 0);
  }

  /*Add IPv4 prefix*/
  uip_ds6_prefix_add((uip_ipaddr_t*)&ipv4prefix,128-32,0,0,0,0);

  LOG_PRINTF("Tunnel prefix ");
  uip_debug_ipaddr_print(&cfg.tun_prefix);
  LOG_PRINTF("\n");
  LOG_PRINTF("Lan address ");
  uip_debug_ipaddr_print(&cfg.lan_addr);
  LOG_PRINTF("\n");
  LOG_PRINTF("Han address ");
  uip_debug_ipaddr_print(&cfg.pan_prefix);
  LOG_PRINTF("\n");
  LOG_PRINTF("Gateway address ");
  uip_debug_ipaddr_print(&cfg.gw_addr);
  LOG_PRINTF("\n");
  LOG_PRINTF("Unsolicited address ");
  uip_debug_ipaddr_print(&cfg.unsolicited_dest);
  LOG_PRINTF("\n");

  {
    uint8_t buf[32];
    keystore_public_key_read(buf);
  }
}
/**
 * Initialize the IPv6 stack
 * This will at some point trigger a TCP_EVENT,TCP_DONE
 * Which means that the DAD has completed and the IPv6 stack is ready to
 * send an receive messages
 */
static void
ipv6_init()
{
  //tcpip_set_outputfunc(zwave_send); //djnakata

  /* Use homeid to set pan link local address, this address is only used in this file
   * to keep track of whare packages are comming from.  */
  pan_lladdr.addr[0] = (homeID & 0xFF000000) >> 24;
  pan_lladdr.addr[1] = (homeID & 0xFF0000) >> 16;
  pan_lladdr.addr[2] = (homeID & 0xFF00) >> 8;
  pan_lladdr.addr[3] = (homeID & 0xFF);
  pan_lladdr.addr[4] = 0x0;
  pan_lladdr.addr[5] = MyNodeID;

  DBG_PRINTF(
      "ZIP_Router_Reset: pan_lladdr: %02x:%02x:%02x:%02x:%02x:%02x  Home ID = 0x%x \r\n",
      (unsigned int ) pan_lladdr.addr[0], (unsigned int )pan_lladdr.addr[1],
      (unsigned int )pan_lladdr.addr[2], (unsigned int )pan_lladdr.addr[3],
      (unsigned int ) pan_lladdr.addr[4], (unsigned int ) pan_lladdr.addr[5],
      UIP_HTONL(homeID));

  //process_exit(&tcpip_process); //djnakata
  //process_start(&tcpip_process,0);

  refresh_ipv6_addresses();
  /* Init RPL as dag root using our PAN IP as DODAGID */
#ifdef UIP_CONF_IPV6_RPL
  {
    rpl_dag_t * dag;

    rpl_init();
    dag = rpl_set_root(&ipaddr);
    if(dag != NULL)
    {
      rpl_set_prefix(dag, &(cfg.pan_prefix), 64);
      DBG_PRINTF("created a new RPL dag\n");
    }
  }
#endif

  //djnakata
  /*Init UDP listners */
  //process_exit(&udp_server_process);
  //process_start(&udp_server_process,0);

  process_exit(&dtls_server_process);
  process_start(&dtls_server_process,0);
#ifdef SUPPORTS_MDNS
  /* Make sure that mDNS is started, but do not stop mDNS if it is
   * already running as that is slow, complicated, and not needed.
   * process_start() is nop when process is running. */
  process_start(&mDNS_server_process,0);
#endif

#ifdef SUPPORTS_COAP
  process_exit(&coap_server_process);
  process_start(&coap_server_process,0);
#endif

#ifdef DEBUG_WEBSERVER
  process_exit(&webserver_nogui_process);
  process_start(&webserver_nogui_process,0);
#endif

}

/*---------------------------------------------------------------------------*/
static BOOL
ZIP_Router_Reset() CC_REENTRANT_ARG
{
  BYTE rnd[8];
  LOG_PRINTF("Resetting ZIP Gateway\n");

  /*Fire up the serial API process */
  process_exit(&serial_api_process);
  process_start(&serial_api_process,cfg.serial_port);

  if (!serial_ok)
  {
    return FALSE;
  }

  MemoryGetID((BYTE*)&homeID,&MyNodeID);
  ZW_GetRandomWord(rnd,0);
  /*Seed the contiki random number generator*/
  random_init(rnd[0] <<24 | rnd[1] <<16 | rnd[2] <<8 | rnd[3]);

  DBG_PRINTF("................. the  version %i ...............",ZW_comamnd_handler_version_get(TRUE, COMMAND_CLASS_FIRMWARE_UPDATE_MD ) );

  ZIP_eeprom_init();

  ipv6_init();

  //The IPv4 tunnel is only initalized once. This means that it is not restarted when we are entering or leaving networks.
#ifdef IP_4_6_NAT
  if(!cfg.ipv4disable && !ipv4_init_once)
  {
    ipv4_interface_init();
    DBG_PRINTF("Starting zip tcp client\n");
    process_exit(&zip_tcp_client_process);
    process_start(&zip_tcp_client_process, cfg.portal_url);
    ipv4_init_once = 1;
    process_exit(&resolv_process);
    process_start(&resolv_process, NULL);

#ifdef __ASIX_C51__
  if(process_is_running(&zip_tcp_client_process))
  {
	  process_exit(&ntpclient_process);
	  process_start(&ntpclient_process, NULL);
  }
#endif

  }
  else if(ipv4_init_once)
  {
    uip_ds6_prefix_add(&(cfg.tun_prefix), cfg.tun_prefix_length, 1, 0, 0, 0);
  }
#endif

//  TransportService_SetCommandHandler(ApplicationCommandHandlerZIP);
  node_queue_init();

//  ClassicZIPNode_init();
  ZW_TransportService_Init(ApplicationCommandHandlerZIP);
  ZW_SendRequest_init();

  /* init: at this point ZW_Send_* functions can be used */
  data_store_init();
  mb_init();

  process_exit(&dhcp_client_process);
  process_start(&dhcp_client_process, 0);

  provisioning_list_init(linux_conf_provisioning_list_storage_file, 
                         linux_conf_provisioning_cfg_file);

  /*Network management is initialized async*/
  network_management_init_done = 0;

  return TRUE;
}


/***************************************************************************/
/* The backup interface is used when a destination is not found in ND cache */

static void
backup_init(void)
{
}

static void
backup_output(void)
{
  //ASSERT(0);
  /*FIXME: What is the right thing to do? */
  /* Send at Ethernet multicast */
  ERR_PRINTF("Using backup interface!\n");
  landev_send(0);
}

const struct uip_fallback_interface backup_interface =
{ backup_init, backup_output };

/*TODO this is debug stuff */
extern uip_ds6_nbr_t uip_ds6_nbr_cache[];
extern uip_ds6_defrt_t uip_ds6_defrt_list[];
extern uip_ds6_route_t uip_ds6_routing_table[];
extern void start_periodic_tcp_timer(void);
extern struct etimer periodic;

void
rd_set_failing(rd_node_database_entry_t* n, uint8_t failing);
#if ASIX_MULTICAST_FILTER_ENABLE
void Preppare_Multicast_Maclist(void);
extern uint8_t MacList_len;
extern uint8_t Multicast_MacFilter[];
#endif

#ifndef ROUTER_REVISION
#define ROUTER_REVISION "Unknown"
#endif

/*---------------------------------------------------------------------------*/
/**
 * Main Z/IP process this process starts all other required processes
 */
/*

 ZIP Router init procedure
 --------------------------
 Outline of the init procedure used on startup and setdefault of zip router family.
 Full inits are triggered by either receiving a Network Management DEFAULT SET command
 or by starting the ZIP Router. In the latter case, ZIP_Router_Reset() is called directly.

 <NetworkManagement Received DEFAULT_SET CMD>
 ZW_SetDefault(...)
 SetDefaultStatus()
 ApplicationDefaultSet()
 Security_OnApplicationInitSW
 process_post(&zip_process,zip_router_reset_evnet,0);

 <zip_router_reset_event_received>
 ZIP_Router_Reset()

 process_start(serialapi_process)
 process_start(tcpip_process) ---> will emit tcpip_event when done
 TransportService_SetCommandHandler(ApplicationCommandHandlerZIP);
 ZW_Queue_init(&txQueue);
 ZW_SendRequest_init();
 ZW_SendDataAppl_init();
 TransportService_Init();
 ZW_SendDataEx_init();
 mb_init();
 process_start(udp_server_process)
 process_start(mDNS_server_process)

 <tcpip_event received>
 ApplicationInitProtocols
 ClassicZIPNode_init()
 bridge_init()

 (async call to temporary_virtual_add_callback)

 rd_init(FALSE)
 Security_OnApplicationInitSW(...)



 A minor initialization is performed when the ZIP Router is excluded:

 <LEARN_MODE_DONE received and we are excluded>
 LearnModeStatus
 ApplicationDefaultSet
 Security_OnApplicationInitSW(...)
 SecureLearnDone(...)

 */
BYTE zip_process_ok = TRUE; //djnakata
PROCESS_THREAD(zip_process, ev, data)
{
  int i;
  PROCESS_BEGIN()
    ;
    /*Setup uip */

#ifdef __ASIX_C51__
  LOG_PRINTF("Starting ZIP Gateway version %bu build %bu\n", zipr_fw_version_st.major_version
         ,zipr_fw_version_st.minor_version);
#else
  LOG_PRINTF("Starting " PACKAGE_STRING " build " PACKAGE_SVN_REVISION "\n");
#endif

#ifdef NO_ZW_NVM
  nvm_init();
#endif

#ifdef __ASIX_C51__
    GwConfigCheck();
#endif

    while(1)
    {
      //DBG_PRINTF("Event ***************** %x ********************\n",ev);
#ifndef __ASIX_C51__
      if(ev == serial_line_event_message)
      {
        /* Keyboard input */
        switch (((char*) data)[0])
        {
#ifdef TEST_MULTICAST_TX
          case 'm':
          {
            static const uint8_t node_list[] = {1,7};
            static uint8_t basic_set[] =  {COMMAND_CLASS_BASIC, BASIC_SET,0x42,0};
            basic_set[2] = ~basic_set[2];

            sec2_send_multicast(MyNodeID, node_list,sizeof(node_list),basic_set,sizeof(basic_set),0);
          }
          break;
          case 'n':
          {
            static const uint8_t node_list[] = {1,7};
            static uint8_t basic_set[] =  {COMMAND_CLASS_BASIC, BASIC_SET,0x42,0};
            basic_set[2] = ~basic_set[2];

            sec2_send_multicast_nofollowup(MyNodeID, node_list,sizeof(node_list),basic_set,sizeof(basic_set),0);
          }
          break;
#endif
          case 'x':
            LOG_PRINTF("Z/IP router is exiting\n");

            rd_exit();
            process_post(PROCESS_BROADCAST, PROCESS_EVENT_EXIT, 0);
            break;
          case 'd':
		    rd_full_network_discovery();
            break;
          case 'f':
          {
            rd_node_database_entry_t* n = rd_get_node_dbe(atoi( &((char*) data)[2] ) );
            if (!n)
            {
                ERR_PRINTF("Usage: f <node id>\n");
                break;
            }
            rd_set_failing(n, TRUE);

            mb_failing_notify(n->nodeid);
            rd_free_node_dbe(n);
		  }
            break;
          case 'r':
          process_post(&zip_process,ZIP_EVENT_RESET,0);
            break;
          case 'b':
            print_bridge_status();
            break;
          case 'w':
            provisioning_list_print();
            break;
          case 'p':
          {
            uint8_t test_prekit_homeid[] = {0xDE, 0xAD, 0xBE, 0xEF};
            NetworkManagement_smart_start_inclusion(0xC0, test_prekit_homeid);
          }

          case 's':
          {
            NetworkManagement_smart_start_init_if_pending();
          }
          break;
          default:

        LOG_PRINTF("IP addresses:\n");
        for(i = 0; i < UIP_DS6_ADDR_NB; i++)
        {
          if(uip_ds6_if.addr_list[i].isused)
          {
            PRINT6ADDR(&uip_ds6_if.addr_list[i].ipaddr);
          }
        }

        LOG_PRINTF("IPv4 address %d.%d.%d.%d:\n",uip_hostaddr.u8[0],uip_hostaddr.u8[1],uip_hostaddr.u8[2],uip_hostaddr.u8[3]);


        LOG_PRINTF("Neighbor cache:\n");
        for(i = 0; i < UIP_DS6_NBR_NB; i++)
        {
          if(uip_ds6_nbr_cache[i].isused)
          {
            PRINT6ADDR(&uip_ds6_nbr_cache[i].ipaddr);
            LOG_PRINTF("  ----  %02x:%02x:%02x:%02x:%02x:%02x",
                uip_ds6_nbr_cache[i].lladdr.addr[0],
                uip_ds6_nbr_cache[i].lladdr.addr[1],
                uip_ds6_nbr_cache[i].lladdr.addr[1],
                uip_ds6_nbr_cache[i].lladdr.addr[2],
                uip_ds6_nbr_cache[i].lladdr.addr[3],
                uip_ds6_nbr_cache[i].lladdr.addr[5]);
            LOG_PRINTF("\n");
          }
        }
        LOG_PRINTF("Routes:\n");
        for(i = 0; i < UIP_DS6_ROUTE_NB; i++)
        {
          if(uip_ds6_routing_table[i].isused)
          {
            PRINT6ADDR(&uip_ds6_routing_table[i].ipaddr);
            LOG_PRINTF(" via ");
            PRINT6ADDR(&uip_ds6_routing_table[i].nexthop);
            LOG_PRINTF("\n");
          }
        }

        LOG_PRINTF("Default Routes:\n");
        for(i = 0; i < UIP_DS6_DEFRT_NB; i++)
        {
          if(uip_ds6_defrt_list[i].isused)
          {
            PRINT6ADDR(&uip_ds6_defrt_list[i].ipaddr);
            LOG_PRINTF("\n");
          }
        }
        break;
      }
#endif
      }
      else if (ev == PROCESS_EVENT_INIT)
      {
        //djnakata
        if (!ZIP_Router_Reset())
        {
          ERR_PRINTF("Fatal error\n");
          process_exit(&zip_process);
          zip_process_ok = FALSE;
          return 1;
        }
        else
        {
          /* Now we are able to send IPv6 packages */
          LOG_PRINTF("Comming up\n");

          ClassicZIPNode_init();
          bridge_init();
        }
      }
      else if (ev == ZIP_EVENT_NODE_IPV4_ASSIGNED)
      {
        u8_t node = ((uintptr_t)data) & 0xFF;
        LOG_PRINTF("New IPv4 assigned for node %d\n", node);
        if (node == MyNodeID)
        {
          /*Now the that the Z/IP gateway has an IPv4 address we may route IPv4 to IPv6 mapped addresses to the gateway itself.*/

          /* With this new route it might be possible to for the NM module to deliver the reply package after a Set default
           * or learn mode. */
          NetworkManagement_Init();
        }
        else
        {
          /* Notify network management module that we have a new IPv4 address. */
          NetworkManagement_IPv4_assigned(node);
        }
      }
      else if (ev == ZIP_EVENT_RESET)
      {
        if (data == (void*) 0)
        {
    		  LOG_PRINTF("Resetting....\n");

          if (!ZIP_Router_Reset())
          {
            ERR_PRINTF("Fatal error\n");
            process_exit(&zip_process);
            zip_process_ok = FALSE;
            return 1;
          }
          else
          {
            /* Now we are able to send IPv6 packages */
            LOG_PRINTF("Comming up\n");

            ClassicZIPNode_init();
            bridge_init();
          }

        }
      }
      else if (ev == ZIP_EVENT_TUNNEL_READY)
      {
        printf("TUNNEL READY!\n");

        /*Refresh used lan and pan addresses*/
        ZIP_eeprom_init();

        /*Reinit the IPv6 addresses*/
        refresh_ipv6_addresses();



        /*Reinsert the IPv6 mapped route.*/
#if 0
        uip_ip6addr_t addr_map;
        addr_map.u16[0] = 0;
        addr_map.u16[1] = 0;
        addr_map.u16[2] = 0;
        addr_map.u16[3] = 0;
        addr_map.u16[4] = 0;
        addr_map.u16[5] = 0xFFFF;
        addr_map.u16[6] = 0;
        addr_map.u16[7] = 0;
        uip_ds6_route_add(&addr_map, 64,&cfg.gw_addr,1);
#endif
        /*The Portal tunnel is ready. Now we might be able to send the network management reply. */
        NetworkManagement_Init();
      }
      else if (ev == ZIP_EVENT_NEW_NETWORK)
      {
        /*We have entered a new Z-Wave network */
        new_pan_ula();
      }
      else if (ev == ZIP_EVENT_QUEUE_UPDATED)
      {
        process_node_queues();
      }
      else if (ev == ZIP_EVENT_SEND_DONE)
      {
        if (uip_completedFunc)
        {
          uip_completedFunc(TRANSMIT_COMPLETE_OK,0);
        }
        uip_completedFunc = 0;
      }
      else if (ev == PROCESS_EVENT_EXITED)
      {
        LOG_PRINTF("A process exited %p\n",data);
        if (data == &dtls_server_process)
        {
          LOG_PRINTF("dtls process exited \n");
          dtls_exited();
        } else
        if (data == &mDNS_server_process)
        {
          LOG_PRINTF("mDNS process exited \n");
          rd_destroy();
          NetworkManagement_mdns_exited();
        }
      } else if(ev == PROCESS_EVENT_EXIT) {
        LOG_PRINTF("Bye bye\n");

#ifdef NO_ZW_NVM
        close(nvm_file);
#endif
      }
      else if (ev == ZIP_EVENT_ALL_NODES_PROBED)
      {
        LOG_PRINTF("All nodes have been probed\n");
        NetworkManagement_all_nodes_probed();
        NetworkManagement_NetworkUpdateStatusUpdate(NETWORK_UPDATE_FLAG_PROBE);

        /* We need to reenable this in case we disabled it during virtual node creation */
        NetworkManagement_smart_start_init_if_pending();

        /* Send node list if needed and all nodes at this point has an ipaddress, if they do not
         * ZIP_EVENT_NODE_DHCP_TIMEOUT will be called at a later stage */
        if( ipv46nat_all_nodes_has_ip() ) {
          send_nodelist();
        }
        /*Kickstart the node queue again*/
        process_post(&zip_process,ZIP_EVENT_QUEUE_UPDATED,0);
      }
      else if (ev == ZIP_EVENT_ALL_IPV4_ASSIGNED
          || ev == ZIP_EVENT_NODE_DHCP_TIMEOUT)
      {
        /* FIXME: We trigger the script here. But what if few nodes do not get DHCP lease. THen
           we dont get this event. Find a correct place where GW gets the IP address or?
         */
#if 0 //djnakata
        if (ev == ZIP_EVENT_ALL_IPV4_ASSIGNED)
        {
            char buf[128];
            int i = 0;
            int rc;

            i+= snprintf(buf +i, sizeof(buf)-i, "IP=");
            i+= snprintf(buf +i, sizeof(buf)-i, "%d.%d.%d.%d",uip_hostaddr.u8[0],uip_hostaddr.u8[1],uip_hostaddr.u8[2],uip_hostaddr.u8[3]);

            i+= snprintf(buf +i, sizeof(buf)-i, " %s &",linux_conf_fin_script);
//            ERR_PRINTF("%s\n", buf);
            rc = system(buf);
            if( rc ) {
              ERR_PRINTF("Error executing: %s\n",buf);
            }
        }
#endif
        udp_server_check_ipv4_queue();
        /*Send node list if we have no unprobed nodes */
        if(!rd_probe_in_progress()) {
          send_nodelist();
        }

        LOG_PRINTF("DHCP pass completed\n");

        NetworkManagement_NetworkUpdateStatusUpdate(NETWORK_UPDATE_FLAG_DHCPv4);
      }
      else if (ev == ZIP_EVENT_BRIDGE_INITIALIZED
          || ev == ZIP_EVENT_NODE_DHCP_TIMEOUT)
      {
        LOG_PRINTF("Bridge init done\n");
        if(ev == ZIP_EVENT_BRIDGE_INITIALIZED) {
          ApplicationInitProtocols();

          /* With this IPv6 ready it might be possible to for the NM module to deliver the reply package after a Set default
           * or learn mode. */
          NetworkManagement_Init();
          send_nodelist();
        }
        tcpip_ipv4_force_start_periodic_tcp_timer();

        NetworkManagement_NetworkUpdateStatusUpdate(NETWORK_UPDATE_FLAG_VIRTUAL);
      } else if(ev == ZIP_EVENT_NETWORK_MANAGEMENT_DONE) {
        send_nodelist();
        process_node_queues();
      } else if(ev == ZIP_EVENT_NM_VIRT_NODE_REMOVE_DONE) {
        DBG_PRINTF(" ZIP_EVENT_NM_VIRT_NODE_REMOVE_DONE triggered\n");
        NetworkManagement_VirtualNodes_removed();
      }

      PROCESS_WAIT_EVENT()
      ;
    }

  PROCESS_END()
  }

  /*---------------------------------------------------------------------------*/
