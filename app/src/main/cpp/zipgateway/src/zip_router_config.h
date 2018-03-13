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

#ifndef ZIP_ROUTER_CONFIG_H_
#define ZIP_ROUTER_CONFIG_H_

#include "Serialapi.h"
#include "ZW_classcmd_ex.h"

/* Yes this is actually right!! */
#if !UIP_CONF_IPV6
typedef union uip_ip6addr_t {
  u8_t  u8[16];     /* Initializer, must come first!!! */
  u16_t u16[8];
} uip_ip6addr_t ;
#endif

/**
 * This structure holds configurable but runtime static
 * variable used to setup the Z/IP router network.
 */
struct router_config {

  /**
   * Name of serial port to use. This string is parsed to \ref SerialInit upon intialization.
   */
  const char* serial_port;

  /**
   * Destination of unsolicitated ZW frames.
   */
  uip_ip6addr_t unsolicited_dest;
  u16_t unsolicited_port;

  uip_ip6addr_t unsolicited_dest2;
  u16_t unsolicited_port2;


/**
 * Prefix of Z-Wave HAN (we used to call it pan :-) )
 */
  uip_ip6addr_t pan_prefix;


  uip_ip6addr_t tun_prefix;
  u8_t tun_prefix_length;

  /**
   * IPv6 address of Z/IP router LAN
   */
  uip_ip6addr_t lan_addr;
  u8_t lan_prefix_length;

  uip_ip6addr_t cfg_lan_addr;   //Lan address from config file
  uip_ip6addr_t cfg_pan_prefix; //PAN address from config file

  u8_t cfg_lan_prefix_length;  //Not used on ZIP_Gateway..always 64

  /**
   * IPv6 default GW.
   */
  uip_ip6addr_t gw_addr;

  const char* serial_log;
  int log_level;

#ifndef __ASIX_C51__
  u8_t extra_classes_len;
  u8_t sec_extra_classes_len;
  u8_t extra_classes[32];
  u8_t sec_extra_classes[32];
#endif

  /**
   * Portal url
   */

  //const char* portal_url;
  char portal_url[64];
  u16_t portal_portno;

  u8_t ipv4disable;
  u8_t clear_eeprom;
  u16_t client_key_size;

  u16_t manufacturer_id;
  u16_t product_type;
  u16_t product_id;
  u16_t hardware_version;
#ifndef __ASIX_C51__
  //certs info
  const char *ca_cert;
  const char *cert;
  const char *priv_key;
  char psk[64];
  u8_t psk_len;
#endif


  mailbox_configuration_mode_t mb_conf_mode;

  /**
   * IP address of the network destination
   */
  uip_ip6addr_t mb_destination;

  /**
   * Port of the mailbox proxy destination in network byte order
   */
  uint16_t mb_port;

  const char* echd_key_file;

  /*True when smart start is enabled*/
  int enable_smart_start;

};

/**
 * When ever the struct zip_nvm_config, is changed the NVM config version should be incremented accordingly
 */
#define NVM_CONFIG_VERSION 2
typedef struct zip_nvm_config {
  u32_t magic;
  u16_t config_version;
  uip_lladdr_t mac_addr;
  uip_ip6addr_t pan_prefix;
  uip_ip6addr_t lan_prefix;
  int8_t security_scheme;
  /* Security 0 key */
  u8_t security_netkey[16];
  u8_t security_status;

  mailbox_configuration_mode_t mb_conf_mode;
  uip_ip6addr_t mb_destination;
  uint16_t mb_port;

  /*S2 Network keys */
  u8_t assigned_keys; //Bitmask of keys which are assigned
  u8_t security2_key[3][16];
  u8_t ecdh_priv_key[32];
} CC_ALIGN_PACK zip_nvm_config_t ;
#define MAX_PEER_PROFILES 	1
#define GW_CONFIG_MAGIC  	0x434F4E46
#define ZIP_GW_MODE_STDALONE 	0x1
#define ZIP_GW_MODE_PORTAL		0x2

#define LOCK_BIT	0x0
#define ZIP_GW_LOCK_ENABLE_BIT	0x1
#define ZIP_GW_LOCK_SHOW_BIT	0x2
#define ZIP_GW_UNLOCK_SHOW_ENABLE 0x2

#define ZIP_GW_LOCK_SHOW_BIT_MASK 			0x3
#define ZIP_GW_LOCK_ENABLE_SHOW_DISABLE		0x1

#define ZWAVE_TUNNEL_PORT	44123

#define ZIPR_READY_OK   0xFF
#define INVALID_CONFIG	0x01

#define IPV6_ADDR_LEN	  16


typedef struct  _Gw_Config_St_
{
	u32_t magic;
	u8_t major;
	u8_t minor;
	u8_t mode;
	u8_t showlock;
	u8_t peerProfile;
	u8_t actualPeers;
}Gw_Config_St_t;

typedef struct  _Gw_PeerProfile_St_
{
	uip_ip6addr_t 	peer_ipv6_addr;
	u8_t 			port1;
	u8_t 			port2;
	u8_t	  		peerNameLength;
	char	  		peerName[63];
}Gw_PeerProfile_St_t;


typedef struct  _Gw_PeerProfile_Report_St_
{
	u8_t  			cmdClass;
	u8_t  			cmd;
	u8_t  			peerProfile;
	u8_t  			actualPeers;
	uip_ip6addr_t 	peer_ipv6_addr;
	u8_t 			port1;
	u8_t 			port2;
	u8_t	  		peerNameLength;
	char	  		peerName[63];

}Gw_PeerProfile_Report_St_t;


typedef struct  _Gw_Portal_Config_St_
{
	uip_ip6addr_t 	lan_ipv6_addr;
	u8_t  			lanPrefixLen;
	uip_ip6addr_t 	portal_ipv6_addr;
	u8_t  			portalPrefixLen;
	uip_ip6addr_t 	gw_ipv6_addr;
	uip_ip6addr_t 	pan_ipv6_addr;
	uip_ip6addr_t 	unsolicit_ipv6_addr;
	u8_t 			unsolicitPort1;
	u8_t 			unsolicitPort2;

}Gw_Portal_Config_St_t;

typedef struct portal_ip_configuration_with_magic_st
{
  u32_t magic;
  uip_ip6addr_t lan_address; //may be 0
  u8_t lan_prefix_length;

  uip_ip6addr_t tun_prefix;
  u8_t tun_prefix_length;

  uip_ip6addr_t gw_address; //default gw address, may be 0.

  uip_ip6addr_t pan_prefix; //pan prefix may be 0... prefix length is always /64

  uip_ip6addr_t unsolicited_dest; //unsolicited destination
  u16_t unsolicited_destination_port;
} CC_ALIGN_PACK portal_ip_configuration_with_magic_st_t;


typedef struct portal_ip_configuration_st 
{
  uip_ip6addr_t lan_address; //may be 0
  u8_t lan_prefix_length;

  uip_ip6addr_t tun_prefix;
  u8_t tun_prefix_length;

  uip_ip6addr_t gw_address; //default gw address, may be 0.

  uip_ip6addr_t pan_prefix; //pan prefix may be 0... prefix length is always /64

  uip_ip6addr_t unsolicited_dest; //unsolicited destination
  u16_t unsolicited_destination_port;
} CC_ALIGN_PACK portal_ip_configuration_st_t;


typedef struct  application_nodeinfo_with_magic_st
{
  u32_t magic;
  u8_t nonSecureLen;
  u8_t secureLen;
  u8_t nonSecureCmdCls[64];
  u8_t secureCmdCls[64];
} CC_ALIGN_PACK application_nodeinfo_with_magic_st_t;

typedef struct  application_nodeinfo_st
{
  u8_t nonSecureLen;
  u8_t secureLen;
  u8_t nonSecureCmdCls[64];
  u8_t secureCmdCls[64];
} CC_ALIGN_PACK application_nodeinfo_st_t;

extern struct router_config cfg;

extern uint8_t gisTnlPacket;
extern uint8_t gGwLockEnable;

extern void NVM_Read(u16_t start,void* dst,u8_t size);
extern void NVM_Write(u16_t start,const void* dst,u8_t size);
#define nvm_config_get(par_name,dst) NVM_Read(offsetof(zip_nvm_config_t,par_name),dst,sizeof(((zip_nvm_config_t*)0)->par_name))
#define nvm_config_set(par_name,src) NVM_Write(offsetof(zip_nvm_config_t,par_name),src,sizeof(((zip_nvm_config_t*)0)->par_name))


typedef enum { FLOW_FROM_LAN, FLOW_FROM_TUNNEL} zip_flow_t;

#endif /* ZIP_ROUTER_CONFIG_H_ */
