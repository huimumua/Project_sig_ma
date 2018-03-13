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

#include "contiki-net.h"
#include "ZIP_Router.h"
#include "ClassicZIPNode.h"
#include "crc16.h"
#include "Security_Scheme0.h"
#include "ResourceDirectory.h"
#include "Mailbox.h"
#include "CC_NetworkManagement.h"

#include "uip-debug.h"
#include "node_queue.h"
#include "crc32alg.h"
#define UIP_IP_BUF                          ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
struct uip_packetqueue_handle first_attempt_queue;
struct uip_packetqueue_handle long_queue;
static struct ctimer queue_timer;

#define BLACKLIST_LENGTH 2
static uint32_t black_list_crc32[BLACKLIST_LENGTH];
static enum en_queue_state queue_state;
enum en_queue_state
get_queue_state()
{
  return queue_state;
}
/**
 * Add fingerprint of uipbuf to blacklist
 */
static void
add_fingerprint_uip_buf()
{
  static uint8_t n = 0;

  black_list_crc32[n] = crc32(&uip_buf[UIP_LLH_LEN], uip_len, 0xFFFF);
  n++;
  if (n >= BLACKLIST_LENGTH)
    n = 0;
}

/**
 * Check if fingerprint of uip_buf is in blacklist
 */
static uint8_t
check_fingerprint_uip_buf()
{
  uint16_t crc;
  uint8_t i;

  crc = crc32(&uip_buf[UIP_LLH_LEN], uip_len, 0xFFFF);
  for (i = 0; i < BLACKLIST_LENGTH; i++)
  {
    if (crc == black_list_crc32[i])
      return TRUE;
  }
  return FALSE;
}

//#define TO4085_FIX
#ifndef TO4085_FIX
/**
 * Check long queue to see if there already is frame queued for this node.
 * @return true if a queued frame exists.
 */
static struct uip_packetqueue_packet*
queued_elements_existes(u8_t node)
{
  struct uip_packetqueue_packet* p;
  struct uip_ip_hdr* ip;
  for (p = (struct uip_packetqueue_packet*) list_head((list_t) &(long_queue.list)); p; p =
      (struct uip_packetqueue_packet*) list_item_next(p))
  {
    ip = (struct uip_ip_hdr*) p->queue_buf;
    if (nodeOfIP(&(ip->destipaddr)) == node)
    {
      return p;
    }
  }
  return 0;
}
#endif /* TO4085_FIX */

/**
 * Called when a new frame is to be transmitted to the PAN
 */
int
node_input_queued(int node)
{
  u8_t rc;

  if (check_fingerprint_uip_buf())
  {
#ifndef __ASIX_C51__
    WRN_PRINTF("Dropping duplicate UDP frame.\n");
#endif
    return TRUE;
  }
  else
  {
    /* We don't want to queue this frame again so generate a blacklist finger print. */
    add_fingerprint_uip_buf();
  }

  /* Disable first_attempt_queue due to TO4085.
   * Long term fix is TBD, perhaps go directly to long_queue
   * when destination is FLiRS node. */
#ifndef TO4085_FIX
  /**
   * If there are already packages for the node in the long queue go directly to the long queue
   */
  ASSERT(uip_len > UIP_IPH_LEN);
  if (queued_elements_existes(node) || (rd_get_node_mode(node) == MODE_FREQUENTLYLISTENING))
  {
    rc = uip_packetqueue_alloc(&long_queue, &uip_buf[UIP_LLH_LEN], uip_len, 60000) != 0;
  }
  else
#endif
  {
    rc = uip_packetqueue_alloc(&first_attempt_queue, &uip_buf[UIP_LLH_LEN], uip_len, 60000) != 0;
  }

  process_post(&zip_process, ZIP_EVENT_QUEUE_UPDATED, 0);
  return rc;
}

/**
 * Timeout for first attempt
 */
static void
queue_send_timeout(void* user)
{
  ClassicZIPNode_AbortSending();
}

/**
 * Send done event
 */
int is_wu_no_more_info_frame(uint8_t* pkt, uint8_t node)
{
  ZW_APPLICATION_TX_BUFFER *pCmd;
  ZW_COMMAND_ZIP_PACKET* zip_pkt;
  uint8_t ext_hdr_len = 0;

  if (is_zip_pkt(pkt)) {
      zip_pkt = (ZW_COMMAND_ZIP_PACKET*)(pkt + UIP_IPUDPH_LEN); 
      /* if there is extension header */
      if (zip_pkt->flags1 & ZIP_PACKET_FLAGS1_HDR_EXT_INCL) {
          ext_hdr_len = (uint8_t)zip_pkt->payload[0]; 
      }
      pCmd = (ZW_APPLICATION_TX_BUFFER *)(&zip_pkt->payload[ext_hdr_len]);

      if (((int )pCmd->ZW_Common.cmdClass == COMMAND_CLASS_WAKE_UP) &&
          ((int )pCmd->ZW_Common.cmd == WAKE_UP_NO_MORE_INFORMATION)) {
          DBG_PRINTF("Wakeup no more info frame sent from ZIP Client to node: %d is NOT put in the Mailbox\n", node);
          return TRUE;
      }
  }
  return FALSE;
}
static void
queue_send_done(BYTE status, BYTE* sent_buffer, uint16_t send_len)
{
  u8_t* data;
  int len;
  struct uip_ip_hdr* iph;
  u8_t node;

  ctimer_stop(&queue_timer);

  if (queue_state == QS_SENDING_FIRST) {
      data = uip_packetqueue_buf(&first_attempt_queue);
      len = uip_packetqueue_buflen(&first_attempt_queue);
      iph = (struct uip_ip_hdr*) data;
  } else if  (queue_state == QS_SENDING_LONG) {
      data = uip_packetqueue_buf(&long_queue);
      len = uip_packetqueue_buflen(&long_queue);
      iph = (struct uip_ip_hdr*) data;
  } else {
      return;
  }

  if(data == 0) {
    assert(0);
    queue_state = QS_IDLE;
    return;
  }

  node = nodeOfIP(&iph->destipaddr);
  DBG_PRINTF("queue_send_done to node %i queue %i\n",node,queue_state);
  if(status == TRANSMIT_COMPLETE_OK) {
    mb_node_transmission_ok_event(node);
  }

  if (queue_state == QS_SENDING_FIRST)
  {
    if ( ((status == TRANSMIT_COMPLETE_NO_ACK)|| (status==TRANSMIT_COMPLETE_REQUEUE)) && send_len && sent_buffer)
    {
      /*Frames for sleeping nodes must be queued in mailbox */
      if ((cfg.mb_conf_mode != DISABLE_MAILBOX) && rd_get_node_mode(node) == MODE_MAILBOX && !is_wu_no_more_info_frame(uip_buf + UIP_LLH_LEN, node))
      {
        memcpy(uip_buf + UIP_LLH_LEN, sent_buffer, send_len);
        uip_len = send_len;
        mb_post_uipbuf(0, 0);
        uip_len = 0;
      }
      else if (rd_get_node_mode(node) == MODE_FREQUENTLYLISTENING)
      { //Packages to FLIRS nodes are not DTLS decrypted
        uip_packetqueue_alloc(&long_queue, data, len, 60000);
      }
      else
      {
        uip_packetqueue_alloc(&long_queue, sent_buffer, send_len, 60000);
      }
    }
    uip_packetqueue_pop(&first_attempt_queue);
  }
  else if ((queue_state == QS_SENDING_LONG) && (status !=TRANSMIT_COMPLETE_REQUEUE) )
  {
    uip_packetqueue_pop(&long_queue);
  }
  queue_state = QS_IDLE;

  if(status != TRANSMIT_COMPLETE_REQUEUE) {
    process_post(&zip_process, ZIP_EVENT_QUEUE_UPDATED, 0);
  }
}

/**
 * Must be called when when queues has been updated
 */
void
process_node_queues()
{
  u8_t node;
  BOOL already_decrypted;
  struct uip_packetqueue_handle *q = 0;

  if (queue_state == QS_IDLE)
  {
    if (uip_packetqueue_len(&first_attempt_queue))
    {
      q = &first_attempt_queue;
      queue_state = QS_SENDING_FIRST;
      ClassicZIPNode_setTXOptions(
      TRANSMIT_OPTION_ACK);
      ClassicZIPNode_sendNACK(FALSE);
      ctimer_set(&queue_timer, 800, queue_send_timeout, q);
      DBG_PRINTF("Sending first attempt\n");
    }
    else if (uip_packetqueue_len(&long_queue))
    {
      q = &long_queue;
      queue_state = QS_SENDING_LONG;
      ClassicZIPNode_setTXOptions(
      TRANSMIT_OPTION_ACK | TRANSMIT_OPTION_AUTO_ROUTE | TRANSMIT_OPTION_EXPLORE);
      ClassicZIPNode_sendNACK(TRUE);
      DBG_PRINTF("Sending long attempt\n");
    }
  }

  if (q && queue_state != QS_IDLE)
  {
    uip_len = uip_packetqueue_buflen(q);
    if (uip_len)
    {
      memcpy((void*) &uip_buf[UIP_LLH_LEN], (void*) uip_packetqueue_buf(q), uip_len);
      node = nodeOfIP(&(UIP_IP_BUF->destipaddr));
      ASSERT(node);

      /* Extend probe timer if we are doing smart start inclusion
       * and are forwarded a frame to the newly included node
       * No frames sent to node for 10 seconds means that middleware has finished probing
       */
      if (node == NM_get_newly_included_nodeid())
      {
        extend_middleware_probe_timeout();
      }

      already_decrypted = (queue_state == QS_SENDING_LONG) && (rd_get_node_mode(node) != MODE_FREQUENTLYLISTENING);

      if (!ClassicZIPNode_input(node, queue_send_done, already_decrypted))
      {
        queue_send_done(TRANSMIT_COMPLETE_FAIL, 0, 0);
      }
      uip_len = 0;
    }
    else
    {
      ASSERT(0);
      queue_state = QS_IDLE;
      uip_packetqueue_pop(q);
    }
  }
}

void
node_queue_init()
{
  /* Free up already initialized queues */
  if (queue_state != QS_IDLE)
  {
    while (uip_packetqueue_len(&first_attempt_queue))
    {
      uip_packetqueue_pop(&first_attempt_queue);
    }
    while (uip_packetqueue_len(&long_queue))
    {
      uip_packetqueue_pop(&long_queue);
    }
  }
  uip_packetqueue_new(&first_attempt_queue);
  uip_packetqueue_new(&long_queue);
  queue_state = QS_IDLE;
}
