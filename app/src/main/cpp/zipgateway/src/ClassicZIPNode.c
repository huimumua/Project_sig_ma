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
#include"contiki.h"
#include "crc16.h"
#include"contiki-net.h"
#include"ClassicZIPNode.h"
#include"ZIP_Router.h"
#include"Serialapi.h"
#include"ZW_udp_server.h"
#include "ZW_SendDataAppl.h"
#include "security_layer.h"
#include <ZW_zip_classcmd.h>
#include <ZW_classcmd_ex.h>
#include "ResourceDirectory.h"
#include "DTLS_server.h"
#include "sys/ctimer.h"
#include "NodeCache.h"
#include "stdint.h"
#include "CC_InstalltionAndMaintenance.h"
#include "node_queue.h"
#include "CC_NetworkManagement.h"
#include "Mailbox.h"
#include "CommandAnalyzer.h"

#define MANGLE_MAGIC 0x55AA

#define MAX_CLASSIC_SESSIONS ZW_MAX_NODES /* Only used for non-bridge libraries */
#define FAIL_TIMEOUT_TEMP_ASSOC 2000 /* Time to wait for ZW_SendData() callback on unsolicited,
                                      * incoming frames before we give up. */
/* 10 sec delay, from SDS11402 */
#define CLASSIC_SESSION_TIMEOUT 65000UL

#define UIP_IP_BUF                          ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
#define UIP_ICMP_BUF                      ((struct uip_icmp_hdr *)&uip_buf[uip_l2_l3_hdr_len])
#define UIP_UDP_BUF                        ((struct uip_udp_hdr *)&uip_buf[uip_l2_l3_hdr_len])
#define ZIP_PKT_BUF                      ((ZW_COMMAND_ZIP_PACKET*)&uip_buf[UIP_LLH_LEN + UIP_IPUDPH_LEN])
#define IP_ASSOC_PKT_BUF                 ((ZW_COMMAND_IP_ASSOCIATION_SET*)&uip_buf[UIP_LLH_LEN + UIP_IPUDPH_LEN + ZIP_HEADER_SIZE])
#define BACKUP_UIP_IP_BUF                ((struct uip_ip_hdr *)backup_buf)
#define BACKUP_IP_ASSOC_PKT_BUF          ((ZW_COMMAND_IP_ASSOCIATION_SET*)&backup_buf[UIP_IPUDPH_LEN + ZIP_HEADER_SIZE])
#define BACKUP_PKT_BUF                      ((ZW_COMMAND_ZIP_PACKET*)&backup_buf[UIP_IPUDPH_LEN])

/* Return codes from handle_ip_association_set() */
typedef enum
{
  HIP_DONE, HIP_WAITING
} return_codes_ip_assoc_t;
/* 10 sec delay, from SDS11402 */
const BYTE ZW_NOP[] =
  { 0 };

static const ZW_APPLICATION_BUSY_FRAME application_busy =
  {
      COMMAND_CLASS_APPLICATION_STATUS,
      APPLICATION_BUSY,
      APPLICATION_BUSY_TRY_AGAIN_IN_WAIT_TIME_SECONDS,
      0xff, };

uint8_t backup_buf[UIP_BUFSIZE];
uint16_t backup_len;
uint8_t is_device_reset_locally = 0;

static u8_t txOpts;
static BOOL bSendNAck;
static BOOL __fromMailbox; //TODO clean this up

#if 0
struct ack_session
{
  struct ctimer timer;
  u8_t seq;
  u8_t snode;
  u16_t port;
  uip_ip6addr_t dest;
};

static struct ack_session ack;
#endif

static struct IP_association *current_ip_association;

static struct ctimer nak_wait_timer;

/* Place holders for parameters needed to send ACK, in the ZW_SendData callback */

zwave_connection_t zw;

static BYTE cur_flags0;
static BYTE cur_flags1;

static BYTE cur_send_ima;
static uint8_t cur_handle; //Current send data handle

/* TODO: Do we need this? Why not hardcode to call zip_data_sent() directly? */
static VOID_CALLBACKFUNC(cbCompletedFunc)
(BYTE,BYTE* data,uint16_t len);

static uint8_t
proxy_command_handler(zwave_connection_t* c, const u8_t* payload, u8_t len, BOOL was_dtls, BOOL ack_req);

static void
encap_and_send_ip_assoc_remove(struct IP_association* ipa);

static int
LogicalRewriteAndSend();

extern command_handler_codes_t
ZIPNamingHandler(zwave_connection_t *c, uint8_t* pData, uint16_t bDatalen);


uint16_t zip_payload_len;

uint8_t is_bridge_library;

/* Return address to use for NAK if virtual node add fails before we create an
 * association table entry */
static struct
{
  uip_ip6addr_t ipaddr;
  uint16_t port; /* port is stored in network byte order */
  nodeid_t dstNodeId;
  uint8_t has_failed_async;
} ip_assoc_reply_addr;

void
clear_ip_association_state()
{
  current_ip_association = NULL;
  memset(&ip_assoc_reply_addr, 0, sizeof(ip_assoc_reply_addr));
}

/*
 * Called when we are done sending.
 */
static void
send_complete(BYTE bStatus, void *usr, TX_STATUS_TYPE *t)
{
  uint16_t l;
  VOID_CALLBACKFUNC(tmp_cbCompletedFunc)
  (BYTE,BYTE* data,uint16_t len);

  l = backup_len;
  backup_len = 0;
  cur_handle = 0;
  tmp_cbCompletedFunc = cbCompletedFunc;
  cbCompletedFunc = NULL;
  if (tmp_cbCompletedFunc)
    tmp_cbCompletedFunc(bStatus, backup_buf, l);
}


void
reset_ip_association_state(uint8_t bStatus)
{
  //DBG_PRINTF("IP Ass reset\n");
  clear_ip_association_state();
  send_complete(bStatus, NULL, NULL);
  ctimer_stop(&nak_wait_timer);
}

void
reset_ip_association_state_ex(uint8_t bStatus, void* user, TX_STATUS_TYPE *t) {
  reset_ip_association_state(bStatus);
}

void
bridge_callback_func(struct IP_association *a)
{
  if (a)
  {
    send_zipack_ipassoc(TRANSMIT_COMPLETE_OK);
    send_complete(TRANSMIT_COMPLETE_OK, NULL, NULL);
  }
  else
  {
    /* Send a ZIP NACK back to the originator in ip_assoc_reply address */
    send_zipack_ipassoc(TRANSMIT_COMPLETE_FAIL);
    send_complete(TRANSMIT_COMPLETE_FAIL, NULL, NULL);
  }
}

/**
 * Add an extended header to a frame
 * @param pkt pointer to ZIP_PACKET buffer
 * @prarm bufsize size of buffer
 * @param data header data
 * @param len length of data
 *
 * @return number of bytes which the packet has increased
 */
u8_t add_ext_header(ZW_COMMAND_ZIP_PACKET* pkt, int bufsize,u8_t hdr_type,u8_t* data,u8_t data_len) {
  u8_t* p;
  int len;
  int k;
  if( (pkt->flags1 & ZIP_PACKET_FLAGS1_HDR_EXT_INCL) == 0 ) {
    pkt->flags1 |=ZIP_PACKET_FLAGS1_HDR_EXT_INCL;
    pkt->payload[0] = 1;
    len = 1;
  } else {
    if(pkt->payload[0] + sizeof(ZW_COMMAND_ZIP_PACKET)+ 2 + data_len > bufsize ) {
      //ERR_PRINTF("packet buffer too small!!! \n");
      return 0;
    }
    len = 0;
  }

  p = (u8_t*) pkt->payload +  pkt->payload[0];
  k=pkt->payload[0];
  k+= 2+data_len;

  *p++ = hdr_type;
  *p++ = data_len;
  memcpy(p,data,data_len);

  pkt->payload[0] = k;
  return len + 2 + data_len;
}

static void
UDP_SendBuffer(int udp_payload_len)
{

  uip_len = UIP_IPUDPH_LEN + udp_payload_len;

  UIP_UDP_BUF ->udplen = UIP_HTONS(UIP_UDPH_LEN + udp_payload_len);
  UIP_UDP_BUF ->udpchksum = 0;

  /* For IPv6, the IP length field does not include the IPv6 IP header
   length. */UIP_IP_BUF ->len[0] = ((uip_len - UIP_IPH_LEN) >> 8);
  UIP_IP_BUF ->len[1] = ((uip_len - UIP_IPH_LEN) & 0xff);
  UIP_IP_BUF ->ttl = 0xff;
  UIP_IP_BUF ->proto = UIP_PROTO_UDP;
  UIP_IP_BUF ->vtc = 0x60;
  UIP_IP_BUF ->tcflow = 0x00;
  UIP_IP_BUF ->flow = 0x00;

  UIP_UDP_BUF ->udpchksum = ~(uip_udpchksum());
  if (UIP_UDP_BUF ->udpchksum == 0)
  {
    UIP_UDP_BUF ->udpchksum = 0xffff;
  }

  memset(uip_buf, 0, UIP_LLH_LEN);
  /*Send */
  tcpip_ipv6_output();
}

static void
NOP_Callback(BYTE bStatus, void* user, TX_STATUS_TYPE *t)
{
  uint8_t node;

  memcpy(&uip_buf[UIP_LLH_LEN], backup_buf, backup_len);
  node = nodeOfIP(&UIP_IP_BUF ->destipaddr);
  uip_len = backup_len;

  if (bStatus == TRANSMIT_COMPLETE_OK)
  {
    uip_icmp6_echo_request_input();
    tcpip_ipv6_output();
  }
  else if (rd_get_node_mode(node) != MODE_MAILBOX)
  {
    uip_icmp6_error_output(ICMP6_DST_UNREACH, ICMP6_DST_UNREACH_ADDR, 0);
    tcpip_ipv6_output();
  }
  send_complete(bStatus, NULL, NULL);
}

#define DEBUG DEBUG_FULL
#include "uip-debug.h"
#include "crc16.h"

/*
 * Send Ack or nack to from srcNode to dst_ip(NOTE this write directly to uip_buf)
 * If ack is true send a ack otherwise send a nak , if wait is true also mark wait.
 * This is an extended version of SendUDPStatus which also sends RSSI info.
 */
void SendUDPStatus_rssi(int flags0, int flags1, zwave_connection_t *c, TX_STATUS_TYPE *txStat)
{
  u32_t delta;
  rd_node_database_entry_t *n;
  u16_t len;

  ZIP_PKT_BUF ->cmdClass = COMMAND_CLASS_ZIP;
  ZIP_PKT_BUF ->cmd = COMMAND_ZIP_PACKET;

  ZIP_PKT_BUF ->flags0 = flags0;
  ZIP_PKT_BUF ->flags1 = flags1;
  ZIP_PKT_BUF ->seqNo = c->seq;
  ZIP_PKT_BUF ->sEndpoint = c->lendpoint;
  ZIP_PKT_BUF ->dEndpoint = c->rendpoint;

  len = ZIP_HEADER_SIZE;
  if (flags0&ZIP_PACKET_FLAGS0_NACK_WAIT )
  {
    delta = 0;
    n = rd_get_node_dbe(nodeOfIP(&c->conn.sipaddr));
    if (n)
    {
      if (n->mode == MODE_MAILBOX)
      {
        delta = n->wakeUp_interval - (clock_seconds() - n->lastAwake);
      }
      else if (n->mode == MODE_FREQUENTLYLISTENING)
      {
        delta = 1;
      }
      rd_free_node_dbe(n);
    }
    else
    {
      ASSERT(0);
    }

    /*Convert to 24bit Big endian */
    u8_t data[3];
    data[0] =  (delta >> 16) & 0xff;
    data[1] =  (delta >>  8) & 0xff;
    data[2] =  (delta >>  0) & 0xff;
    delta =UIP_HTONL(delta) << 8;
    len +=add_ext_header(ZIP_PKT_BUF, sizeof(uip_buf) -(UIP_LLH_LEN + UIP_IPUDPH_LEN),
        ZIP_PACKET_EXT_EXPECTED_DELAY,data ,sizeof(data));
  }


/*
 * Add IMA, if this is a ACK or NACK response
 */
  if(cur_send_ima && (flags0 & (ZIP_PACKET_FLAGS0_ACK_RES | ZIP_PACKET_FLAGS0_NACK_RES) ))
  {
    const struct node_ima* ima =  ima_get_entry(nodeOfIP(&c->conn.sipaddr));
    u8_t ima_buffer[14+7+3+3];  // + IMA_OPTION_RSSI + IMA_OPTION_ACK_CHANNEL + IMA_OPTION_TRANSMIT_CHANNEL
    u8_t *p = ima_buffer;
    *p++ = IMA_OPTION_RC;
    *p++ = 1;
    *p++ = ima_was_route_changed;

    *p++ = IMA_OPTION_TT;
    *p++ = 2;
    *p++ = (ima_last_tt >> 8) & 0xff;
    *p++ = (ima_last_tt >> 0) & 0xff;

    *p++ = IMA_OPTION_LWR;
    *p++ = 5;
    memcpy(p,&ima->lwr, 5);
    p+=5;
    if (NULL != txStat) {
      *p++ = IMA_OPTION_RSSI;
      *p++ = 5;
      memcpy(p,&txStat->rssi_values, 5);
      p+= 5;
      *p++ = IMA_OPTION_ACK_CHANNEL;
      *p++ = 1;
      *p++ = txStat->ackChannel;
      *p++ = IMA_OPTION_TRANSMIT_CHANNEL;
      *p++ = 1;
      *p++ = txStat->lastTxChannel;
      assert((p - ima_buffer) == (14+7+3+3));
    } else {
      assert((p - ima_buffer) == 14);
    }

    len += add_ext_header(ZIP_PKT_BUF,sizeof(uip_buf), INSTALLATION_MAINTENANCE_REPORT,ima_buffer, p - ima_buffer );

  }
  udp_send_wrap(&c->conn,ZIP_PKT_BUF,len);
}

/*
 * Send Ack or nack to from srcNode to dst_ip(NOTE this write directly to uip_buf)
 * If ack is true send a ack otherwise send a nak , if wait is true also mark wait
 */
void SendUDPStatus(int flags0, int flags1, zwave_connection_t *c)
{
  SendUDPStatus_rssi(flags0, flags1, c, NULL);
}

void
abort_ip_assoc_session(struct IP_association *a)
{

  remove_bridge_association(a);
  reset_ip_association_state(TRANSMIT_COMPLETE_FAIL);
}

/*void nak_fail_timeout(void* user) {
 abort_ip_assoc_session((struct IP_association*)user);
 }*/

static void
nak_wait_timeout(void* user)
{
  SendUDPStatus(ZIP_PACKET_FLAGS0_NACK_WAIT | ZIP_PACKET_FLAGS0_NACK_RES ,cur_flags1,&zw);
}

/* Callback from senddata on an incoming, unsolicited frame.
 * Send a ZIP Ack/Nack and reset ip association state.
 *
 * Preconditions:
 *   - current_ip_association must be setup
 */
static void
unsolicited_senddata_callback(BYTE bStatus, void* user, TX_STATUS_TYPE *t)
{
  ctimer_stop(&nak_wait_timer);
  DBG_PRINTF("unsolicited_senddata_callback status %u\n", bStatus);
  //ASSERT(current_ip_association != NULL);
  if (current_ip_association == NULL)
  {
    /* Timed out before callback arrived  or callback double-fired */
    DBG_PRINTF("Timed out before callback arrived  or callback double-fired\n");
    ASSERT(0);
    reset_ip_association_state(bStatus);
    return;
  }

  /*Don't send NACK to mailbox nodes here. The mailbox module will do it.*/
  if (!(  ( get_queue_state() == QS_SENDING_FIRST )
      && (bStatus != TRANSMIT_COMPLETE_OK && bStatus != TRANSMIT_COMPLETE_ERROR)))
  {

    if ( (cur_flags0 & ZIP_PACKET_FLAGS0_ACK_REQ) )
    {
      SendUDPStatus_rssi( bStatus == TRANSMIT_COMPLETE_OK ? ZIP_PACKET_FLAGS0_ACK_RES :ZIP_PACKET_FLAGS0_NACK_RES , cur_flags1,&zw, t);
    }
  }

  if (bStatus == TRANSMIT_COMPLETE_OK)
  {
    rd_node_is_alive(current_ip_association->han_nodeid);
  }

  reset_ip_association_state(bStatus);
}

/* Callback from senddata after forwarding a frame
 * through a proxy IP Association.
 *
 */
static void
proxy_forward_callback(BYTE bStatus, void* user, TX_STATUS_TYPE *t)
{
  struct IP_association *a = (struct IP_association*)user;
  DBG_PRINTF("proxy_forward_callback status %u, node %u\n", bStatus, a->han_nodeid);

  if (bStatus == TRANSMIT_COMPLETE_OK)
  {
    rd_node_is_alive(a->han_nodeid);
  }

  reset_ip_association_state(bStatus);
}

static uint8_t classic_txBuf[UIP_BUFSIZE - UIP_IPUDPH_LEN];/*NOTE: the local txBuf is ok here, because the frame is sent via the SerialAPI*/

uint8_t
is_local_address(uip_ipaddr_t *ip)
{
  return nodeOfIP(ip) != 0;
}

/**
 * Send a ZIP Packet with ACK/NACK status on IP Association processing.
 *
 * \note This function uses TRANSMIT_COMPLETE_OK (==0) to indicate success.
 *
 * \param bStatus Status. TRANSMIT_COMPLETE_OK if succesful, TRANSMIT_COMPLETE_FAIL if failed.
 */
void
send_zipack_ipassoc(uint8_t bStatus)
{
  if(bStatus == TRANSMIT_COMPLETE_OK) {
    SendUDPStatus(ZIP_PACKET_FLAGS0_ACK_RES,cur_flags1,&zw);
  } else if(bSendNAck) {
    SendUDPStatus(ZIP_PACKET_FLAGS0_NACK_RES,cur_flags1,&zw);
  }
}

void
abort_current_ip_assoc(uint8_t bStatus)
{
  remove_bridge_association(current_ip_association);
  reset_ip_association_state(bStatus);
}

void
assign_return_route_callback(unsigned char bStatus)
{
  send_zipack_ipassoc(bStatus);
  if (bStatus != TRANSMIT_COMPLETE_OK)
  {
    abort_current_ip_assoc(TRANSMIT_COMPLETE_FAIL);
  }
  else
  {
    reset_ip_association_state(TRANSMIT_COMPLETE_OK);
  }
}

void
classic_assoc_set_callback(unsigned char bStatus, void* user, TX_STATUS_TYPE *t)
{
  struct IP_association *a = current_ip_association;
  ASSERT(a != NULL);
  //UNUSED(t);
  if (bStatus == TRANSMIT_COMPLETE_OK)
  {
    /* Setup return routes, then send back ZIP_ACK*/
    /* If this is a local assoc, setup return routes directly btw. assoc
     * source and dest, otherwise setup return routes to virtual node */
    if (!ZW_AssignReturnRoute(a->han_nodeid,
        a->type == local_assoc ? nodeOfIP(&a->resource_ip) : a->virtual_id,
        assign_return_route_callback))
    {
      send_zipack_ipassoc(TRANSMIT_COMPLETE_FAIL);
      abort_current_ip_assoc(TRANSMIT_COMPLETE_FAIL);
    }
  }
  else
  {
    send_zipack_ipassoc(TRANSMIT_COMPLETE_FAIL);
    abort_current_ip_assoc(TRANSMIT_COMPLETE_FAIL);
  }
}

void
setup_ip_assoc_reply_addr(struct uip_udp_conn* c)
{
  uip_ipaddr_copy(&ip_assoc_reply_addr.ipaddr, &c->ripaddr);
  ip_assoc_reply_addr.port = c->rport;
  ip_assoc_reply_addr.dstNodeId = nodeOfIP(&c->sipaddr);
}

/* Set up a classic Association Set in buffer p  */
int
setup_classic_assoc_set( /* RET number of bytes written */
char* p, /* IN Pointer to tx buffer */
ZW_COMMAND_IP_ASSOCIATION_SET *ip_assoc_cmd)
{
  /* FIXME: Is *p and ip_assoc_cmd pointing to the same thing?
   * If so, remove one of them and derive from the other */

  ZW_MULTI_CHANNEL_ASSOCIATION_SET_1BYTE_V2_FRAME* b =
      (ZW_MULTI_CHANNEL_ASSOCIATION_SET_1BYTE_V2_FRAME*) p;
  b->cmdClass = COMMAND_CLASS_ASSOCIATION_V2;
  b->cmd = ASSOCIATION_SET_V2;
  b->groupingIdentifier = ip_assoc_cmd->groupingIdentifier;
  b->nodeId1 = nodeOfIP(&ip_assoc_cmd->resourceIP);
  return 4;
}

/* Set up a classic Association Remove in buffer p  */
int
setup_classic_assoc_remove( /* RET number of bytes written */
char* p, /* IN  Pointer to tx buffer */
struct IP_association* ipa) /* IN  IP association to remove */
{
  ZW_MULTI_CHANNEL_ASSOCIATION_SET_1BYTE_V2_FRAME* b =
      (ZW_MULTI_CHANNEL_ASSOCIATION_SET_1BYTE_V2_FRAME*) p;
  ASSERT(ipa->type == permanent_assoc || ipa->type == local_assoc);
  b->cmdClass = COMMAND_CLASS_ASSOCIATION_V2;
  b->cmd = ASSOCIATION_REMOVE_V2;
  b->groupingIdentifier = ipa->grouping;
  if (ipa->type == local_assoc)
  {
    b->nodeId1 = nodeOfIP(&ipa->resource_ip);
  }
  else
  {
    b->nodeId1 = ipa->virtual_id;
  }
  return 4;
}

void
send_ip_assoc(nodeid_t dest_nodeid, uint16_t len)
{
  /* Send the association frame in classic_txBuf on the radio.
   * When callback comes back, setup return routes.
   * When that callback comes, send off ZIP_ACK. */
  ASSERT(current_ip_association != NULL);

  ts_param_t p;
  ts_set_std(&p, dest_nodeid);

  p.scheme = SupportsCmdClassSecure(dest_nodeid, classic_txBuf[0]) ? AUTO_SCHEME : NO_SCHEME ;
  cur_handle = ZW_SendDataAppl(&p, (BYTE*) &classic_txBuf, len,
      classic_assoc_set_callback, 0);
  if (cur_handle == FALSE)
  {
    send_zipack_ipassoc(TRANSMIT_COMPLETE_ERROR);
    ASSERT(0);

    abort_current_ip_assoc(TRANSMIT_COMPLETE_ERROR);
    /* drop packet */
  }
}

void
handle_ip_assoc_case2_callback(struct IP_association *a)
{
  uint16_t i = 0;
  uint8_t *p;
  ASSERT(a->grouping);
  p = classic_txBuf;
  p[i++] = COMMAND_CLASS_ASSOCIATION_V2;
  p[i++] = ASSOCIATION_SET_V2;
  p[i++] = a->grouping;
  p[i++] = a->virtual_id;
  current_ip_association = a;
  ASSERT(a != NULL);
  if (a)
  {
    send_ip_assoc(a->han_nodeid, i);
  }
  else
  {
    DBG_PRINTF("out of memory");
    send_zipack_ipassoc(TRANSMIT_COMPLETE_ERROR);
    abort_current_ip_assoc(TRANSMIT_COMPLETE_ERROR);
  }
}

void remove_from_bulk_list()
{
    struct IP_association *ipa = get_ipa_marked_for_removal();

//    ERR_PRINTF("------remove_from_bulk_list\n");
    if (ipa == NULL) {
        ASSERT(0); //This should not happen
        send_zipack_ipassoc(TRANSMIT_COMPLETE_OK);
        abort_current_ip_assoc(TRANSMIT_COMPLETE_OK);
        return;
    }

//    DBG_PRINTF("--------- Removed ip association ipa->resource_endpoint:%d\n ip:", ipa->resource_endpoint);
//    uip_debug_ipaddr_print(&ipa->resource_ip);
#if 0
    DBG_PRINTF("Virt-%d:%d \t HAN-%d:%d \t %c\n",
        ipa->virtual_id, ipa->virtual_endpoint,
        ipa->han_nodeid, ipa->han_endpoint ,
        assoc_types[ipa->type]);
#endif
    encap_and_send_ip_assoc_remove(ipa);
    return;
}
void
association_remove_callback(uint8_t bStatus, void* user, TX_STATUS_TYPE *t)
{
  /* We dont call ZW_DeleteReturnRoutes because that
   * would flush all return routes, and require us
   * to set the others up again.
   * Instead we rely on explorer searches to find routes. */
  if ((bStatus == TRANSMIT_COMPLETE_OK) || (bStatus == TRANSMIT_COMPLETE_NO_ACK))
  {
    remove_bridge_association_and_persist(current_ip_association);
  }
  else
  {
    send_zipack_ipassoc(TRANSMIT_COMPLETE_FAIL); /* Indicate delivery failure to source */
    reset_ip_association_state(TRANSMIT_COMPLETE_OK); /* _OK because we dont want to see this frame again */
  }

}

void
handle_ip_assoc_external_callback(struct IP_association *a)
{
  /* create association from .dest_endpoint to .assoc_endpoint to
   * and send ZIP_ACK to .src_endpoint.
   * All endpoints in struct IP_Association
   */
  int i = 0;
  if (!a)
  {
    /* malloc or create virtual failed, abort */
    DBG_PRINTF("out of memory");
    send_zipack_ipassoc(TRANSMIT_COMPLETE_ERROR);
    abort_current_ip_assoc(TRANSMIT_COMPLETE_ERROR);
    return;
  }

  if (a->han_endpoint != 0)
  {
    ZW_MULTI_CHANNEL_CMD_ENCAP_V2_FRAME *b =
        (ZW_MULTI_CHANNEL_CMD_ENCAP_V2_FRAME*) classic_txBuf;
    b->cmdClass = COMMAND_CLASS_MULTI_CHANNEL_V2;
    b->cmd = MULTI_CHANNEL_CMD_ENCAP_V2;
    b->properties1 = 0;
    b->properties2 = a->han_endpoint;
    i = 4;
  }

  if (a->virtual_endpoint != 0)
  {
    char *p;
    /* Use Multi Channel Association Set */
    p = ((char*) &classic_txBuf) + i;
    *(p++) = COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V2;
    *(p++) = MULTI_CHANNEL_ASSOCIATION_SET_V2;
    *(p++) = a->grouping;
    *(p++) = a->virtual_id;
    *(p++) = MULTI_CHANNEL_ASSOCIATION_SET_MARKER_V2;
    *(p++) = a->virtual_id;
    *(p++) = a->virtual_endpoint;
    i += 7;
  }
  else
  {
    /* Use normal Association Set */
    ZW_ASSOCIATION_SET_1BYTE_V2_FRAME *assoc_frame = (void*) &classic_txBuf + i;
    assoc_frame->cmdClass = COMMAND_CLASS_ASSOCIATION;
    assoc_frame->cmd = ASSOCIATION_SET_V2;
    assoc_frame->groupingIdentifier = a->grouping;
    assoc_frame->nodeId1 = a->virtual_id;
    i += 4;
  }
  /* Send the frame. When callback comes back, setup return routes.
   * When that callback comes, send off ZIP_ACK. */
  current_ip_association = a;
  send_ip_assoc(a->han_nodeid, i);
}

uint8_t
setup_multi_channel_assoc_remove(char *p, struct IP_association *ipa)
{
  *(p++) = COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V2;
  *(p++) = MULTI_CHANNEL_ASSOCIATION_REMOVE_V2;
  *(p++) = ipa->grouping;
  *(p++) = MULTI_CHANNEL_ASSOCIATION_REMOVE_MARKER_V2;
  *(p++) = ipa->type == local_assoc ? nodeOfIP(&ipa->resource_ip) :ipa->virtual_id ;
  *(p++) = ipa->type == local_assoc ? ipa->resource_endpoint : ipa->virtual_endpoint;
  return 6;
}

static void
encap_and_send_ip_assoc_remove(struct IP_association* ipa)
{
  uint8_t i = 0;
  ts_param_t p;


  ts_set_std(&p, ipa->han_nodeid);
  p.scheme = zw.scheme;
  p.dendpoint = ipa->han_endpoint;
  p.sendpoint = ipa->resource_endpoint;

  ASSERT(ipa);
  ASSERT(ipa->type == local_assoc || ipa->type == permanent_assoc);

  current_ip_association = ipa;


  if(!rd_node_exists(ipa->han_nodeid)) {
      association_remove_callback(TRANSMIT_COMPLETE_OK,0, NULL);
      return;
  }

  if (SupportsCmdClass(ipa->han_nodeid,
      COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V2)
      || SupportsCmdClassSecure(ipa->han_nodeid,
          COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V2)
     )
  {
    /* Case 5 */
    i = setup_multi_channel_assoc_remove((char*) &classic_txBuf, ipa);
  }
  else
  {
    /* Case 2 */
    i = setup_classic_assoc_remove((char*) &classic_txBuf, ipa);
  }

  /* Send Association Remove command to ASN */
  cur_handle = ZW_SendDataAppl(&p, (BYTE*) &classic_txBuf, i,
      association_remove_callback, 0);
  if (cur_handle == FALSE)
  {
    send_zipack_ipassoc(TRANSMIT_COMPLETE_ERROR);
    ASSERT(0);
    abort_current_ip_assoc(TRANSMIT_COMPLETE_ERROR);
    /* drop packet */
  }
}

void remove_ip_association_by_nodeid(BYTE node_id)
{
//  ERR_PRINTF("------remove_ip_association_by_nodeid%d\n", node_id);
  mark_bulk_removals(node_id, 0, 0, 0, 0);

  if (bulk_ipa_remove_val()) 
  {
//       ERR_PRINTF("------ bulk_ipa_remove\n");
       remove_from_bulk_list();
  }
  else {
//      ERR_PRINTF("------No bulk_ipa_remove\n");
      /* Trigger the resource directory update */
      process_post(&zip_process,ZIP_EVENT_NM_VIRT_NODE_REMOVE_DONE,0);
  }
}

/*
 * Handle an incoming IP Association Remove packet.
 *
 * Note: We dont remove the return routes, because we cannot
 * remove them individually. Instead we just rely on explorer
 * routing to sort this out.
 */
void
handle_ip_association_remove(zwave_connection_t* c, const u8_t* payload, const u8_t len)
{
  ZW_COMMAND_IP_ASSOCIATION_REMOVE *cmd = (ZW_COMMAND_IP_ASSOCIATION_REMOVE*)payload;
  //struct IP_association *ipa = NULL;
  uip_ip6addr_t zero_ip;
  /* TODO: Check lenght of frame */

  memset(&zero_ip, 0, sizeof(uip_ip6addr_t));
  ASSERT(cmd->cmd == IP_ASSOCIATION_REMOVE);
  ASSERT(cmd->cmd_class == COMMAND_CLASS_IP_ASSOCIATION);
  if ((cmd->resource_name_length & MASK_IP_ASSOCIATION_RES_NAME_LENGTH) == 0)
  {
    if (bulk_ipa_remove_val()) { /*Just locking further IP association removal till existing ones are finished */
      ERR_PRINTF("Still in the process of removing IP associations in response to previous IP_ASSOCIATION_REMOVE command.\n");
      send_zipack_ipassoc(TRANSMIT_COMPLETE_ERROR);
      reset_ip_association_state(TRANSMIT_COMPLETE_ERROR);
      return;
    }
    mark_bulk_removals(nodeOfIP(&c->lipaddr), c->lendpoint, cmd->grouping, &cmd->ip_addr, cmd->endpoint);
    if (bulk_ipa_remove_val()) {
        remove_from_bulk_list();
        return;
    } else {
        LOG_PRINTF("Association not found\n");
    }
  }
  else
  {
    /* TODO: Support symbolic IP Association cmds*/
    /* TODO: Find IP Association from symbolic name */
    LOG_PRINTF(
        "Symbolic names in IP Association commands not supported, dropping packet");
  }
  /* The Remove command was delivered successfully, but could not be processed due to invalid
   * contents. We still return _OK because we do not signal application layer errors in the ZIP
   * header. We treat it as strictly a transport layer mechanism. And delivery was successful. */
  send_zipack_ipassoc(TRANSMIT_COMPLETE_OK);
  reset_ip_association_state(TRANSMIT_COMPLETE_OK); /* _OK because we dont want to see this frame again */
  return;
}

uint8_t
setup_multi_channel_assoc_set(char* p, nodeid_t assoc_dest_node, ZW_COMMAND_IP_ASSOCIATION_SET *cmd)
{
  *(p++) = COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V2;
  *(p++) = MULTI_CHANNEL_ASSOCIATION_SET_V2;
  *(p++) = cmd->groupingIdentifier;
  *(p++) = assoc_dest_node;
  *(p++) = MULTI_CHANNEL_ASSOCIATION_SET_MARKER_V2;
  *(p++) = assoc_dest_node;
  *(p++) = cmd->endpoint;
  return 7;
}

/*
 * Handle an incoming IP Association Set packet.
 * Ground rule: You MUST call reset_ip_association_state() to clean up after passing through here.
 */
void
handle_ip_association_set(zwave_connection_t* c, const u8_t* payload, const u8_t len,BOOL was_dtls)
{
  uip_ip6addr_t *assoc_dest_ip;
  u8_t assoc_dest_ep;
  int i = 0;
  nodeid_t dest_nodeid;

  ZW_COMMAND_IP_ASSOCIATION_SET *ip_assoc_set_cmd = (ZW_COMMAND_IP_ASSOCIATION_SET*)payload;

  assoc_dest_ip = &ip_assoc_set_cmd->resourceIP;
  assoc_dest_ep = ip_assoc_set_cmd->endpoint;

  if (is_local_address(assoc_dest_ip))
  {
    /* Send either a multi-channel association set or an association set */
    /* ASN = Association Source Node
     * ADN = Association Destination Node
     *
     * There are 5 sub-cases here:
     *  Case 1: Single ASN, Single ADN -> Classic Assoc Set
     *  Case 2: Single ASN no multi support, Multi ADN -> Classic Assoc Set
     *          (ZIP Roter must proxy and provide Multichannel encap)
     *  Case 3: Multi ASN, Single ADN -> Multi-channel encapsulated Classic Assoc Set
     *  Case 4: Multi ASN, Multi ADN -> Multi-channel encapsulated Multi Channel Assoc Set
     *  Case 5: Like case 2, but ASN supports Multi Channel Assoc Set
     *          command class -> Multi Channel Assoc Set
     */
    dest_nodeid = nodeOfIP(&c->lipaddr);
    if ((c->lendpoint == 0 && assoc_dest_ep != 0)
        && !SupportsCmdClass(dest_nodeid,
            COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V2)
        && !SupportsCmdClassSecure(dest_nodeid,
            COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V2)
       )
    {
      /* Case 2 */
      /* we must proxy this connection */
      if (create_bridge_association(proxy_assoc,
          handle_ip_assoc_case2_callback, ip_assoc_set_cmd,was_dtls) != BRIDGE_OK)
      {
        send_zipack_ipassoc(TRANSMIT_COMPLETE_ERROR);
        ASSERT(0);
        reset_ip_association_state(TRANSMIT_COMPLETE_ERROR);
      }
      return;
    }
    current_ip_association = create_local_association(ip_assoc_set_cmd);
    if (!current_ip_association)
    {
      send_zipack_ipassoc(TRANSMIT_COMPLETE_ERROR);
      ASSERT(0);
      reset_ip_association_state(TRANSMIT_COMPLETE_ERROR);
      return;
    }
    if (c->lendpoint == 0 && assoc_dest_ep == 0)
    {
      /* Case 1 */
      i += setup_classic_assoc_set((char*) &classic_txBuf, ip_assoc_set_cmd);
      send_ip_assoc(current_ip_association->han_nodeid, i);
      return;
    }
    else if (c->lendpoint == 0 && assoc_dest_ep != 0)
    {
      ASSERT(SupportsCmdClass(dest_nodeid, COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V2)
          || SupportsCmdClassSecure(dest_nodeid, COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V2));
      /* Case 5, send Multi Channel assoc set, no encap */
      i = setup_multi_channel_assoc_set((char*) &classic_txBuf,
          nodeOfIP(assoc_dest_ip), ip_assoc_set_cmd);
      send_ip_assoc(current_ip_association->han_nodeid, i);
      return;
    }
    else if (c->lendpoint != 0  && assoc_dest_ep == 0)
    {
      ZW_MULTI_CHANNEL_CMD_ENCAP_V2_FRAME *b =
          (ZW_MULTI_CHANNEL_CMD_ENCAP_V2_FRAME*) classic_txBuf;
      /* Case 3 */
      b->cmdClass = COMMAND_CLASS_MULTI_CHANNEL_V2;
      b->cmd = MULTI_CHANNEL_CMD_ENCAP_V2;
      b->properties1 = 0;
      b->properties2 = c->lendpoint;
      i = 4;
      i += setup_classic_assoc_set((char*) &classic_txBuf + 4, ip_assoc_set_cmd);
      send_ip_assoc(current_ip_association->han_nodeid, i);
      return;
    }
    else
    {
      /* Case 4*/
      ZW_MULTI_CHANNEL_CMD_ENCAP_V2_FRAME *b =
          (ZW_MULTI_CHANNEL_CMD_ENCAP_V2_FRAME*) classic_txBuf;
      b->cmdClass = COMMAND_CLASS_MULTI_CHANNEL_V2;
      b->cmd = MULTI_CHANNEL_CMD_ENCAP_V2;
      b->properties1 = 0;
      b->properties2 = c->lendpoint;
      i = 4;
      /* TODO: Proxy if ASN doesn't support COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION. */
      i += setup_multi_channel_assoc_set((char*) &classic_txBuf + i,
          nodeOfIP(assoc_dest_ip), ip_assoc_set_cmd);
      send_ip_assoc(current_ip_association->han_nodeid, i);
      return;
    }
  }
  else /* if (is_local_address(assoc_dest_ip)) */
  {
    if (create_bridge_association(permanent_assoc,
        handle_ip_assoc_external_callback, ip_assoc_set_cmd,was_dtls) != BRIDGE_OK)
    {
      send_zipack_ipassoc(TRANSMIT_COMPLETE_ERROR);
      ASSERT(0);
      reset_ip_association_state(TRANSMIT_COMPLETE_ERROR);
      return;
    }
  }
}

void
temp_assoc_callback(struct IP_association *a)
{
  if (!a)
  {
    /* malloc or create virtual failed, abort */
    ERR_PRINTF("out of memory or virtual nid creation failed");
    send_zipack_ipassoc(TRANSMIT_COMPLETE_ERROR);
    abort_current_ip_assoc(TRANSMIT_COMPLETE_ERROR);
    return;
  }
  //ASSERT(!current_ip_association); /* Is this really invariant? */
  current_ip_association = a; /* store this for SendCommand_Callback_ip_assoc() */

  if (current_ip_association)
  {
    ts_param_t p;
    p.tx_flags = txOpts;
    p.rx_flags = 0;
    p.dendpoint = current_ip_association->han_endpoint;
    p.sendpoint = zw.rendpoint;
    p.dnode = current_ip_association->han_nodeid;
    if (is_device_reset_locally) {
        p.snode = MyNodeID;
    } else {
        p.snode = current_ip_association->virtual_id;
    }
    p.scheme = zw.scheme;

    cur_handle = ZW_SendDataAppl(&p, (BYTE*) &classic_txBuf, zip_payload_len,
        unsolicited_senddata_callback, 0);
    if (cur_handle)
    {
      ctimer_set(&nak_wait_timer, 150, nak_wait_timeout,
          current_ip_association);
      return;
    }
  }

  ERR_PRINTF("ZW_SendData_Bridge failed\n");
  unsolicited_senddata_callback(TRANSMIT_COMPLETE_FAIL, 0, NULL);
}

static void
handle_ip_association_get(zwave_connection_t* c, const u8_t* payload, const u8_t len)
{


  struct IP_association *a;
  ZW_IP_ASSOCIATION_GET_FRAME *req =(ZW_IP_ASSOCIATION_GET_FRAME *) payload;
  ZW_IP_ASSOCIATION_REPORT_1BYTE_FRAME f;
  u8_t nodes_in_grouping;

  send_zipack_ipassoc(TRANSMIT_COMPLETE_OK);
  if(len < sizeof(ZW_IP_ASSOCIATION_GET_FRAME)) {
    reset_ip_association_state(TRANSMIT_COMPLETE_OK);
  }

  nodes_in_grouping = find_ip_assoc_for_report(nodeOfIP(&c->lipaddr), c->lendpoint, req->groupingIdentifier,req->index, &a);

  memset(&f,0,sizeof(f));
  f.cmdClass = COMMAND_CLASS_IP_ASSOCIATION;
  f.cmd = ASSOCIATION_REPORT;
  f.groupingIdentifier = req->groupingIdentifier;
  f.index = req->index;
  f.actualNodes = nodes_in_grouping;
  if (a)
  {

    uip_ipaddr_copy((uip_ip6addr_t* )&f.associationDestinationIpv6Address1, &a->resource_ip);
    f.associationDestEndpoint = a->resource_endpoint;
    f.resourceName1 = 0; /* TODO: Support Symbolic naming*/
  }
  else
  {
    /* empty grouping or index out of bounds - report error */
  }

  ZW_SendDataZIP(c,&f,sizeof(ZW_IP_ASSOCIATION_REPORT_1BYTE_FRAME)-2,reset_ip_association_state_ex);
}

/**
 * Intercept version command class get for the ZIP naming class and the ip association class.
 */
static uint8_t
handle_version(const uint8_t* payload,u8_t len, BOOL ack_req)
{
  int the_version;
  ZW_VERSION_COMMAND_CLASS_REPORT_FRAME f;

  if(len < sizeof(ZW_VERSION_COMMAND_CLASS_GET_FRAME)) {
    return FALSE;
  }

  if(payload[1] == VERSION_COMMAND_CLASS_GET) {
    the_version = 0;

    if(payload[2] == COMMAND_CLASS_ZIP_NAMING) the_version = ZIP_NAMING_VERSION;
    if(payload[2] == COMMAND_CLASS_IP_ASSOCIATION) the_version = IP_ASSOCIATION_VERSION;
    if(payload[2] == COMMAND_CLASS_ZIP) the_version = ZIP_VERSION_V3;

    if(the_version)
    {
      /*Send ACK first*/
      if(ack_req) {
        SendUDPStatus(  ZIP_PACKET_FLAGS0_ACK_RES  , cur_flags1,&zw);
      }

      f.cmdClass = COMMAND_CLASS_VERSION;
      f.cmd = VERSION_COMMAND_CLASS_REPORT;
      f.requestedCommandClass = payload[2];
      f.commandClassVersion = the_version;

      ZW_SendDataZIP(&zw,&f,sizeof(f), send_complete);
      return TRUE;
    }
  }
  return FALSE;
}



static uint8_t
handle_ip_association(zwave_connection_t* c,const u8_t* payload,u8_t len,BOOL was_dtls)
{
  /* Reset uip_len since periodic timers expect it to be zero
   * TODO: This should perhaps happen earlier when we copy into backup_buf
   * and backup_len
   * I'm doing it only for IP Assoc to test the waters first */
  uip_len = 0;

  if((get_queue_state() == QS_SENDING_FIRST) && (payload[1] != ASSOCIATION_GET))
  {
    /* Slow operation, must go in the long queue.
     * When rejected, it will return from the long queue later*/
    send_complete(TRANSMIT_COMPLETE_NO_ACK, NULL, NULL);
    return TRUE;
  }

  /* TODO: Check if node/endpoint suppoerts association cc */
  switch (payload[1])
  {
    case ASSOCIATION_SET:
      handle_ip_association_set(c,payload,len,was_dtls);
      return TRUE;
      break;

    case ASSOCIATION_REMOVE:
      handle_ip_association_remove(c,payload,len);
      return TRUE;
      break;

    case ASSOCIATION_GET:
      handle_ip_association_get(c,payload,len);
      return TRUE;
      break;

    default:
      /* Unexpected cmd */
      reset_ip_association_state(TRANSMIT_COMPLETE_OK);
      return TRUE;
      break;
  }
  return FALSE;
}

static uint8_t
proxy_supervision_command_handler(zwave_connection_t* c, const uint8_t* payload, u8_t len, BOOL was_dtls, BOOL ack_req)
{
  ZW_SUPERVISION_GET_FRAME *f =(ZW_SUPERVISION_GET_FRAME *)payload;

  if(f->cmd == SUPERVISION_GET) {
    if(proxy_command_handler(c,payload+4,f->encapsulatedCommandLength,was_dtls,ack_req)) {
      ZW_SUPERVISION_REPORT_FRAME rep;
      rep.cmdClass = COMMAND_CLASS_SUPERVISION;
      rep.cmd = SUPERVISION_REPORT;
      rep.sessionid = f->sessionid & 0x3f;
      rep.status = SUPERVISION_REPORT_SUCCESS;
      rep.duration = 0;

      ZW_SendDataZIP(c,&rep,sizeof(rep), send_complete);
      return TRUE;
    }
  }

  return FALSE;

}


static uint8_t
proxy_command_handler(zwave_connection_t* c, const u8_t* payload, u8_t len, BOOL was_dtls, BOOL ack_req)
{
  zwave_connection_t tc = *c;
  tc.scheme = SECURITY_SCHEME_UDP;
  switch (payload[0])
  {
  case COMMAND_CLASS_IP_ASSOCIATION:
    if (ack_req)
    {
      ctimer_set(&nak_wait_timer, 150, nak_wait_timeout, NULL);
    }
    return handle_ip_association(&tc, payload, zip_payload_len, was_dtls);
  case COMMAND_CLASS_ZIP_NAMING:
    if (ack_req)
    {
      SendUDPStatus( ZIP_PACKET_FLAGS0_ACK_RES, cur_flags1, &tc);
    }
    return ZW_command_handler_run(&tc, (uint8_t*) payload, len) == COMMAND_HANDLED;
  case COMMAND_CLASS_VERSION:
    return handle_version(payload, zip_payload_len, ack_req);
  case COMMAND_CLASS_SUPERVISION:
    return proxy_supervision_command_handler(&tc, payload, len, was_dtls, ack_req);
  default:
    return FALSE;
  }
}

/**
 * Parse an incoming UDP packet adressed for a classic ZWave node. The packet must be already
 * decrypted and located in the backup_buf buffer. uip_buf is unsafe because it will be
 * overwritten if we use async requests as a part of the parsing.
 *
 * Input argument data must point to ZIP hdr and length must count only zip hdr + zip payload
 * Furthermore, callers must ensure that data points to somewhere in the backup_buf buffer.
 */
int ClassicZIPUDP_input(struct uip_udp_conn* c, u8_t* pktdata, u16_t len, BOOL secure)
{
  ZW_COMMAND_ZIP_PACKET* zip_ptk = (ZW_COMMAND_ZIP_PACKET*)pktdata;
  BYTE* payload;
  /*
   * Lots of nested if statements in this function.
   * Overview:
   *    if (destined to zwave port)
   *      if not idle: return
   *      if bridge library
   *        if IP_Association_Set
   *        else if unsolicited ZIP_Command
   *      else if not bridge library
   *        fall back to unsolicited session handling without virtual nodeids
   *    else
   *      throw Port Unreachable
   *    acknowledge packet has been transmitted
   */
  /*if (check_fingerprint_uip_buf())
   {
   WRN_PRINTF("Dropping duplicate UDP frame.\n");
   return TRUE;
   }*/

  if ((bridge_state != initialized)  && ((NetworkManagement_getState() != NM_WAIT_FOR_PROBE_BY_SIS)))
  {
    DBG_PRINTF("Bridge not initialized\n");
    resume_bridge_init();
    return FALSE;
  }

  /* Save return address in case we need to send back a NAK
   * TODO: Why dont we just use the udp_connection_object
   * if/when the NAK is sent? */
  setup_ip_assoc_reply_addr(c);


  if (zip_ptk->cmdClass == COMMAND_CLASS_ZIP
      && zip_ptk->cmd == COMMAND_ZIP_KEEP_ALIVE && secure)
  {
    const u8_t keep_alive_ack[] = {COMMAND_CLASS_ZIP,COMMAND_ZIP_KEEP_ALIVE,ZIP_KEEP_ALIVE_ACK_RESPONSE};
    if( pktdata[2] & ZIP_KEEP_ALIVE_ACK_REQUEST) {
      udp_send_wrap(c, &keep_alive_ack, sizeof(keep_alive_ack));
    }
    send_complete(TRANSMIT_COMPLETE_OK, NULL, NULL);
    return TRUE;
  }

  if (zip_ptk->cmdClass == COMMAND_CLASS_ZIP
      && zip_ptk->cmd == COMMAND_ZIP_PACKET)
  {
    /* Sanity check
     */
    zip_payload_len = len - ZIP_HEADER_SIZE;
    payload = &zip_ptk->payload[0];

    cur_flags0 = zip_ptk->flags0;
    cur_flags1 = secure ? ZIP_PACKET_FLAGS1_SECURE_ORIGIN : 0;
    zw.conn = *c;
//  ERR_PRINTF("Setting seq[%d]:  to %x\n", nodeOfIP(&c->sipaddr), zip_ptk->seqNo);
    zw.seq = zip_ptk->seqNo;
    zw.lendpoint = zip_ptk->dEndpoint;
    zw.rendpoint = zip_ptk->sEndpoint;
    zw.scheme = (zip_ptk->flags1 & ZIP_PACKET_FLAGS1_SECURE_ORIGIN) ? AUTO_SCHEME  : NO_SCHEME;

#ifndef DEBUG_ALLOW_NONSECURE
    if((zip_ptk->flags1 & ZIP_PACKET_FLAGS1_SECURE_ORIGIN) && !secure ) {
      ERR_PRINTF("Frame marked with secure origin but was not received secure\n");
      goto drop_frame;
    }
#endif

    /*
     * This is an ACK for a previously sent command
     */
    if ((zip_ptk->flags0 & ZIP_PACKET_FLAGS0_ACK_RES))
    {
      UDPCommandHandler(c,pktdata, len, secure);
    }

    /*Parse header extensions FIXME this is copy paste from ZW_udp_server.c*/
    if (zip_ptk->flags1 & ZIP_PACKET_FLAGS1_HDR_EXT_INCL)
    {
      tlv_t* opt = (tlv_t*) &zip_ptk->payload[1];

      if( *payload == 0 || *payload > zip_payload_len) {
        ERR_PRINTF("BAD extended header\n");
        goto drop_frame;
      }

      zip_payload_len-= *payload;
      payload += *payload;
      cur_send_ima = 0;

      while ( (BYTE*)opt < payload)
      {
        switch(opt->type & 0x7f) {
          case INSTALLATION_MAINTENANCE_GET:
            cur_send_ima = TRUE;
            break;
          case ENCAPSULATION_FORMAT_INFO:
            if(opt->length < 2) {
              goto opt_error;
            }
            zw.scheme = efi_to_shceme(opt->value[0],opt->value[1]);
            break;
          default:
            if (opt->type & 0x80) //Check for critical options
            {
              ERR_PRINTF("Unsupported critical option %i\n", (opt->type & 0x7f));
              /*Send option error*/
              goto opt_error;
            }
            break;
        }
        opt = (tlv_t*)((BYTE*) opt + (opt->length + 2));
      }

      DBG_PRINTF("zw.scheme: %d\n", zw.scheme);
      if((BYTE*)opt !=payload) {
        ERR_PRINTF("Invalid extended header\n");
        goto drop_frame;
      }
    }

    /*
     * If no ZW Command included just drop
     */
    if( ((zip_ptk->flags1 & ZIP_PACKET_FLAGS1_ZW_CMD_INCL)==0) || zip_payload_len==0 ) {
      goto drop_frame;
    }


    if (zip_payload_len > sizeof(classic_txBuf))
    {
      ERR_PRINTF("Frame too large\n");
      goto drop_frame;
    }

    if(payload[0] == COMMAND_CLASS_DEVICE_RESET_LOCALLY) { /* Do not set source node as virtual node if its COMMAND_CLASS_DEVICE_RESET_LOCALLY */
        is_device_reset_locally = 1;
    } else {
        is_device_reset_locally = 0;
    }


    /*
     * TO#03860 Check that we are done probing the node. Re queue the frame
     */



    if ( (!__fromMailbox && mb_is_busy()) ||
        ((NetworkManagement_getState() != NM_IDLE) && (NetworkManagement_getState() != NM_WAIT_FOR_PROBE_BY_SIS)) ||
        (rd_get_node_state(nodeOfIP(&zw.lipaddr)) < STATUS_DONE))
    {
      reset_ip_association_state(TRANSMIT_COMPLETE_REQUEUE);
      DBG_PRINTF("Requeueing frame \n");
      return TRUE;
    }


    struct IP_association *a;
    /* When proxying a case2 IP Association, keep the virtual node id when forwarding.
     * Without this exception, the outgoing sender field would be changed to one of
     * the temporary association virtual nodeids.
     *
     * Warning: zw.conn struct notion of local and remote have been swapped. The virtual
     * destination node is now stored as remote IP address.
     * The swapping happens in get_udp_conn() as a preparation for the common case
     * of _replying_. Forwarding, as we are doing here, is a special case.  */
    uip_debug_ipaddr_print(&zw.ripaddr);
    if (nodeOfIP(&zw.ripaddr)
        && (a = get_ip_assoc_by_virtual_node(nodeOfIP(&zw.ripaddr))))
    {
      ASSERT(a->type == proxy_assoc);
      ts_param_t p;
      p.tx_flags = txOpts;
      p.rx_flags = 0;
      p.dendpoint = a->resource_endpoint;
      p.sendpoint = 0;
      p.dnode = nodeOfIP(&zw.lipaddr);
      ASSERT(uip_ipaddr_prefixcmp(&zw.lipaddr, &a->resource_ip, 128));
      ASSERT(p.dnode);
      if(is_device_reset_locally) {
          p.snode = MyNodeID;
      } else {
          p.snode = a->virtual_id;
      }
      p.scheme = zw.scheme;

      cur_handle = ZW_SendDataAppl(&p, (BYTE*) &zip_ptk->payload, zip_payload_len,
          proxy_forward_callback, a);
      if (!cur_handle)
      {
        /* Give up if transmission fails. TODO: Should we report/retry this error? */
        WRN_PRINTF("ZW_SenddataAppl failed on case2 IP Assoc proxying\n");
      }
      return TRUE;
    }


    if(proxy_command_handler(&zw,payload,zip_payload_len,secure, zip_ptk->flags0 & ZIP_PACKET_FLAGS0_ACK_REQ)) {
      return TRUE;
    }

    if (zip_payload_len > sizeof(classic_txBuf))
    {
      goto drop_frame;
    }
    memcpy((BYTE*) &classic_txBuf, payload, zip_payload_len);



    if (create_bridge_association(temporary_assoc, temp_assoc_callback, NULL,secure)
        != BRIDGE_OK)
    {
      ASSERT(0);
      reset_ip_association_state(TRANSMIT_COMPLETE_ERROR);

      /* In some cases the SendCommand_Callback_ip_assoc will have fired here and reset the current_ip_association*/
    }
    return TRUE;
  } /* if COMMAND_CLASS_ZIP */
  else
  {
    DBG_PRINTF("Dropping packet received on Z-Wave port: Not ZIP encapped\n");
    /* unknown packet, drop and continue */
    reset_ip_association_state(TRANSMIT_COMPLETE_ERROR);
  }

  return TRUE;

drop_frame:
/*TODO why send a NACK at all? */
  if (ZIP_PKT_BUF->flags0 & ZIP_PACKET_FLAGS0_ACK_REQ) {
    SendUDPStatus(ZIP_PACKET_FLAGS0_NACK_RES,cur_flags1,&zw);
  }
  reset_ip_association_state(TRANSMIT_COMPLETE_ERROR);
  return TRUE;

opt_error:
  ERR_PRINTF("Option error\n");
  if (ZIP_PKT_BUF->flags0 & ZIP_PACKET_FLAGS0_ACK_REQ) {
    SendUDPStatus(ZIP_PACKET_FLAGS0_NACK_RES | ZIP_PACKET_FLAGS0_NACK_OERR , cur_flags1,&zw);
  }
  reset_ip_association_state(TRANSMIT_COMPLETE_ERROR);
  return TRUE;
}


int ClassicZIPNode_dec_input(void * pktdata, int len) {
  /* Copy decrypted payload into uip_buf, leaving the ip header intact */
  //memcpy(uip_appdata,pktdata,len); //djnakata
  backup_len = UIP_IPUDPH_LEN + len;
  memcpy(backup_buf,uip_buf+UIP_LLH_LEN,backup_len);

  struct uip_udp_conn* c = get_udp_conn();

  if(ClassicZIPUDP_input(c, backup_buf + UIP_IPUDPH_LEN, backup_len - UIP_IPUDPH_LEN, TRUE)) {
      return TRUE;
  } else {
      send_complete(TRANSMIT_COMPLETE_FAIL, NULL, NULL);
      return FALSE;
  }
}



/* This function is used in both directions.
Normally from LAN to PAN
And used from PAN to LAN after LogicalRewriteAndSend()
*/
int
ClassicZIPNode_input(int node, VOID_CALLBACKFUNC(completedFunc)
(BYTE,BYTE*,uint16_t), int bFromMailbox)
{

  if ((nodeOfIP(&(UIP_IP_BUF->destipaddr)) < 1) ||
      (nodeOfIP(&(UIP_IP_BUF->destipaddr)) > ZW_MAX_NODES))
  {
    ASSERT(0);
    goto drop;
  }

  if(backup_len > 0)
  {
    if (bFromMailbox)
    {
        DBG_PRINTF("Reque from Mailbox\n");
    }
    else
    {
        DBG_PRINTF("Reque from node_queue\n");
    }

    completedFunc(TRANSMIT_COMPLETE_REQUEUE, uip_buf+UIP_LLH_LEN,uip_len);
    return TRUE;
  }

  cbCompletedFunc = completedFunc;
  __fromMailbox = bFromMailbox;

  /*Create a backup of package, in order to make async requests. */
  backup_len = uip_len;
  memcpy(backup_buf,uip_buf+UIP_LLH_LEN,backup_len);
  ASSERT(nodeOfIP(&(BACKUP_UIP_IP_BUF->destipaddr)));

  if(LogicalRewriteAndSend())
  {
    send_complete(TRANSMIT_COMPLETE_OK, NULL, NULL);
    return TRUE;
  }

  switch(UIP_IP_BUF->proto)
  {
    case UIP_PROTO_UDP:
    {
      uip_appdata = &uip_buf[UIP_LLH_LEN + UIP_IPUDPH_LEN];
      uip_len -= UIP_IPUDPH_LEN;

      if( UIP_UDP_BUF ->destport == UIP_HTONS(ZWAVE_PORT) )
      {
        struct uip_udp_conn* c = get_udp_conn();
        return ClassicZIPUDP_input(c, &backup_buf[UIP_IPUDPH_LEN],
            backup_len - UIP_IPUDPH_LEN, uip_buf[UIP_LLH_LEN+3] ==FLOW_FROM_TUNNEL);
      }
#ifndef DISABLE_DTLS
      else if( UIP_UDP_BUF ->destport == UIP_HTONS(DTLS_PORT) )
      {
        /*If this frame is posted from mailbox it has already been de-crypted. ,just call
         *
         */
        if(bFromMailbox) {
          struct uip_udp_conn* c = get_udp_conn();
          return ClassicZIPUDP_input(c, &backup_buf[UIP_IPUDPH_LEN],
              backup_len - UIP_IPUDPH_LEN, TRUE);
        }

        /*This should call classic UDP input at some point */
        process_post_synch(&dtls_server_process,DTLS_SERVER_INPUT_EVENT,0);

        if(!session_done_for_uipbuf() ) {
          DBG_PRINTF("DTLS Classic node session package\n");
          send_complete(TRANSMIT_COMPLETE_OK, NULL, NULL); //If session is not yet established we are done processing the package.
        } else if (dtls_ssl_read_failed){ //If the SSL_read call failed then there will be no callback.
          send_complete(TRANSMIT_COMPLETE_OK, NULL, NULL);
        } else {
          DBG_PRINTF("DTLS for Classic node\n");
        }
        return TRUE;
      }
#endif
      else /* Unknown port */
      {
        /*Throw a port unreachable */
        uip_icmp6_error_output(ICMP6_DST_UNREACH, ICMP6_DST_UNREACH_NOPORT, 0);
        tcpip_ipv6_output();
        send_complete(TRANSMIT_COMPLETE_OK, NULL, NULL);
        return TRUE;
      }
    }
    break;
    case UIP_PROTO_ICMP6:
    switch(UIP_ICMP_BUF->type)
    {
      case ICMP6_ECHO_REQUEST:
      /*Create a backup of package, in order to make async requests. */
      DBG_PRINTF("Echo request for classic node %i\n",node);
      /*We send NOP as a non secure package*/
      ts_param_t p;
      ts_set_std(&p,node);
      p.scheme = NO_SCHEME;
      p.tx_flags = txOpts;
      cur_handle = ZW_SendDataAppl(&p,(u8_t*)ZW_NOP,sizeof(ZW_NOP),NOP_Callback,0);
      if( cur_handle )
      {
        return TRUE;
      } // else drop
      break; //ICMP6_ECHO_REQUEST
    default:
      DBG_PRINTF("ICMP type 0x%02x not supported by classic node\n",UIP_ICMP_BUF->type);
      send_complete(TRANSMIT_COMPLETE_OK, NULL, NULL);
      return TRUE;
    }
    break; //UIP_PROTO_ICMP6
  default:
    DBG_PRINTF("Unknown protocol %i\n",UIP_IP_BUF->proto);
    send_complete(TRANSMIT_COMPLETE_OK, NULL, NULL);
    return TRUE;
  }
drop:
  /* We have processed this packet but it is not delivered. */
  completedFunc(TRANSMIT_COMPLETE_ERROR,(BYTE*)UIP_IP_BUF,uip_len);
  return TRUE;
}

static BYTE ack_rnode = 0;
static security_scheme_t busy_scheme;

static void send_unsolicited_callback(BYTE status, void* d, TX_STATUS_TYPE *t) {
  ts_param_t p;
  //d=d;

  if(status != TRANSMIT_COMPLETE_OK) {
    ts_set_std(&p, ack_rnode);
    p.scheme = busy_scheme;
    p.tx_flags = txOpts;

    if(!ZW_SendDataAppl(&p, (BYTE*) &application_busy, sizeof(application_busy), 0,
        0)) {
      ERR_PRINTF("Unable to send application busy\n");
    }
  }
}

void
ClassicZIPNode_SendUnsolicited(
    zwave_connection_t* __c, ZW_APPLICATION_TX_BUFFER *pCmd,
    BYTE cmdLength, uip_ip6addr_t* destIP, u16_t port, BOOL bAck)
{
  zwave_connection_t c;
  uint8_t rnode;
  memcpy(&c, __c, sizeof(zwave_connection_t));

  /* rpiaddr is here ip address of source from where this unsol
     package is originated it could be one following two
     - PAN address of node
     - LAN address of ZIP Client or unsolicited destination itself
   If the unsol package originated from PAN side 
        Swap addresses if we are forwaridng packet from PAN side.

   Else if we are replying to unsol destination
        packets we use the LAN or PAN address of gateway where the unsol destination sent the packet*/

  if( uip_ipaddr_prefixcmp(&cfg.pan_prefix, &c.ripaddr, 64) ){
    c.lipaddr = c.ripaddr;
  }
  c.ripaddr = *destIP;

  /* Swap endpoints */
  uint8_t tmp;
  tmp = c.lendpoint;
  c.lendpoint = c.rendpoint;
  c.rendpoint = tmp;

  c.rport = port;
  c.lport = (port == UIP_HTONS(ZWAVE_PORT)) ? UIP_HTONS(ZWAVE_PORT): UIP_HTONS(DTLS_PORT);
  /* NOte we are just forwardig the packet to unsol destination. Do not flip the end points */

  rnode = nodeOfIP( &c.lipaddr );

  DBG_PRINTF("Sending Unsolicited to IP app...\n");


  if(bAck &&( CommandAnalyzerIsGet(pCmd->ZW_Common.cmdClass,pCmd->ZW_Common.cmd) || CommandAnalyzerIsSet(pCmd->ZW_Common.cmdClass,pCmd->ZW_Common.cmd) ) ) {
    ack_rnode = rnode;
    busy_scheme = c.scheme;

    ZW_SendDataZIP_ack(&c,pCmd,cmdLength,send_unsolicited_callback);
  } else {
    ZW_SendDataZIP(&c,pCmd,cmdLength,0);
  }
}

void
ClassicZIPNode_init()
{
#if NON_BRIDGE
  int n;

  for (n = 0; n < MAX_CLASSIC_SESSIONS; n++)
  {
    classic_sessions[n].dstNode = 0;
    timer_set(&(classic_sessions[n].timeout),CLASSIC_SESSION_TIMEOUT);
  }
#endif
  clear_ip_association_state();
  backup_len = 0;
  txOpts = TRANSMIT_OPTION_ACK | TRANSMIT_OPTION_AUTO_ROUTE
      | TRANSMIT_OPTION_EXPLORE;

}

/**
 * Form a UDP package from the the Z-Wave package. Destination address will
 * be a logical HAN address which contains the source and destination node
 * IDs and endpoints.
 *
 * The resulting destination address is on the form
 * han_pefix::ffaa:0000:[dendpoint][sendpoint]:[dnode][snode]
 */
void
CreateLogicalUDP(ts_param_t* p, unsigned char *pCmd, uint8_t cmdLength)
{
  BYTE l;

  zwave_connection_t c;
   
  memset(&c,0,sizeof(c));

  c.tx_flags = p->tx_flags;
  c.rx_flags = p->rx_flags;
  c.scheme = p->scheme;
  c.rendpoint = p->dendpoint;
  c.lendpoint = p->sendpoint;
  /* UDP Header */
  c.lport = UIP_HTONS(ZWAVE_PORT);
  c.rport = UIP_HTONS(ZWAVE_PORT);

  /* IP header */
  uip_ipaddr_copy(&c.ripaddr, &cfg.pan_prefix);
  uip_ipaddr_copy(&c.lipaddr, &cfg.pan_prefix);
  c.ripaddr.u8[15] = p->snode;
  c.ripaddr.u8[14] = p->dnode;
  c.ripaddr.u8[13] = p->sendpoint;
  c.ripaddr.u8[12] = p->dendpoint;
  c.ripaddr.u16[5] = MANGLE_MAGIC;

  /* Lookup destination node in assoc table*/
  if (p->dendpoint & 0x80)
  {
    WRN_PRINTF(
        "bit-addressed multichannel encaps should never be sent to virtual nodes\n");
    return;
  }
  
 /*
  uip_debug_ipaddr_print(& UIP_IP_BUF ->srcipaddr);
  uip_debug_ipaddr_print(& UIP_IP_BUF ->destipaddr);
 */

  ZW_SendData_UDP(&c, pCmd, cmdLength, NULL, FALSE);
}

/**
 * Rewrite source and destination addresses of IP package based on
 * logical HAN destination address and temporary association tables.
 */
static int
LogicalRewriteAndSend()
{
  struct IP_association* a;
  if (UIP_IP_BUF ->destipaddr.u16[5] == MANGLE_MAGIC)
  {
    u8_t snode = UIP_IP_BUF ->destipaddr.u8[15];
    u8_t dnode = UIP_IP_BUF ->destipaddr.u8[14];
    u8_t endpoint = UIP_IP_BUF ->destipaddr.u8[12];
    DBG_PRINTF("DeMangled src %i dst %i:%i\n", snode, dnode, endpoint);
    a = get_ip_assoc_by_nodes(snode, dnode, endpoint);
    if (a)
    {
      struct uip_udp_conn c;

      ZIP_PKT_BUF ->dEndpoint = a->resource_endpoint;
      ZIP_PKT_BUF ->sEndpoint =  UIP_IP_BUF ->destipaddr.u8[13];
      c.rport = a->resource_port ;
      c.lport = a->was_dtls ? UIP_HTONS(DTLS_PORT) : UIP_HTONS(ZWAVE_PORT);
      if (a->type != proxy_assoc)
      {
        /* IP Association to LAN - use DTLS and association source as source ip */
        ipOfNode(&c.sipaddr,a->han_nodeid);
      }
      else
      {
        /* proxy frame back to PAN with virtual ID as source */
        ipOfNode(&c.sipaddr,a->virtual_id);
      }
      uip_ipaddr_copy(&c.ripaddr, &a->resource_ip);
      DBG_PRINTF("Packet from Z-wave side (nodeid: %d) to port:%d IP addr: ", snode, c.rport);
      uip_debug_ipaddr_print(&c.ripaddr);
      udp_send_wrap(&c,ZIP_PKT_BUF,uip_len - UIP_IPUDPH_LEN);
    }
    else
    {
      WRN_PRINTF("No IP assc found for package\n");
    }
    return TRUE;
  }
  return FALSE;
}

/* Multichannel encapsulate and forward data being forwarded from
 * single-channel to HAN-local multi-channel node */
void
forward_proxy_data(struct IP_association* a, unsigned char *data, char datalen)
{
  int i = 0;
  ASSERT(a->resource_endpoint); /* Always multi-channel target in this case*/
  ASSERT(nodeOfIP(&a->resource_ip));

  //TODO we should wait for the callback or ??.....
  ts_param_t p;
  ts_set_std(&p, 0);
  p.dnode = nodeOfIP(&a->resource_ip);
  p.sendpoint = a->virtual_endpoint;
  p.dendpoint = a->resource_endpoint;
  p.snode = a->virtual_id;

  if(!ZW_SendDataAppl(&p, (BYTE*) &classic_txBuf, i, 0, 0)) {
    //pass
  }
}

void
ClassicZIPNode_setTXOptions(u8_t opt)
{
  txOpts = opt;
}

void
ClassicZIPNode_sendNACK(BOOL __sendNAck)
{
  bSendNAck = __sendNAck;
}

void
ClassicZIPNode_AbortSending()
{
  if (cur_handle)
  {
    ZW_SendDataApplAbort(cur_handle);
  }
}

