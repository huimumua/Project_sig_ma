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
#include "ZW_SendRequest.h"
#include "ZIP_Router.h"
#include "ZW_SendDataAppl.h"
#include "sys/ctimer.h"
#include "lib/list.h"
#include "lib/memb.h"


#define NUM_REQS 4
#define NOT_SENDING 0xFF

typedef enum
{
  REQ_SENDING, REQ_WAITING, REQ_DONE,
} req_state_t;

typedef struct send_request_state
{
  list_t *list;
  ts_param_t param;
  req_state_t state;
  BYTE class;
  BYTE cmd;
  WORD timeout;
  void* user;
  ZW_SendRequst_Callback_t callback;
  struct ctimer timer;
} send_request_state_t;

MEMB(reqs,struct send_request_state,NUM_REQS);
LIST(reqs_list);

static void
request_timeout(void* d)
{
  send_request_state_t* s = (send_request_state_t*) d;
  s->param.dnode = 0;

  s->state = REQ_DONE;
  ctimer_stop(&s->timer);
  list_remove(reqs_list,s);
  memb_free(&reqs,s);

  WRN_PRINTF("SendRequest timeout waiting for 0x%2x 0x%2x\n", s->class, s->cmd);
  if (s->callback)
  {
    s->callback(TRANSMIT_COMPLETE_FAIL, 0, 0, 0, s->user);
  }
}

static void
ZW_SendRequest_Callback(BYTE status, void* user, TX_STATUS_TYPE *t)
{
  send_request_state_t* s = (send_request_state_t*)user;



  if (status == TRANSMIT_COMPLETE_OK && s->state == REQ_SENDING)
  {
    s->state = REQ_WAITING;
    ctimer_set(&s->timer, s->timeout * 10, request_timeout, s);
  }
  else
  {
    request_timeout(s);
  }
}

/**
 * Send a request to a node, and trigger the callback once the
 * response is received
 *
 * NOTE! If there is a destination endpoint, then there must be room for 4 bytes before pData
 *
 */
BYTE
ZW_SendRequest(ts_param_t* p, const BYTE *pData,
BYTE dataLength,
BYTE responseCmd, WORD timeout, void* user, ZW_SendRequst_Callback_t callback)
{
  send_request_state_t* s;

  if (!callback)
    goto fail;

  s = memb_alloc(&reqs);

  if (!s)
    goto fail;

  list_add(reqs_list,s);

  ts_param_make_reply(&s->param, p); //Save the node/endpoint which is supposed to reply

  s->state = REQ_SENDING;
  s->class = pData[0];
  s->cmd = responseCmd;
  s->user = user;
  s->callback = callback;
  s->timeout = timeout;

  if (ZW_SendDataAppl(p, pData, dataLength, ZW_SendRequest_Callback, s))
  {
    return TRUE;
  }
  else
  {
    list_remove(reqs_list,s);
    memb_free(&reqs,s);
  }

fail:
  ERR_PRINTF("ZW_SendRequest fail\n");
  return FALSE;
}

void ZW_Abort_SendRequest(uint8_t n)
{
//  DBG_PRINTF("--------------- Inside ZW_Abort_SendRequest()\n");
  send_request_state_t* s;
  for (s = list_head(reqs_list); s; s=list_item_next(s)) {
//    DBG_PRINTF("--------------- Comparing n=%bu with dnode=%bu, snode=%bu\n", n, s->param.dnode, s->param.snode);
    if (s->state == REQ_WAITING && (s->param.snode == n))
      {
        s->state = REQ_DONE;
        ctimer_stop(&s->timer);
        list_remove(reqs_list,s);
        memb_free(&reqs,s);
        s->callback(TRANSMIT_COMPLETE_FAIL, 0, 0, 0, s->user);
      }
  }
}

BOOL
SendRequest_ApplicationCommandHandler(ts_param_t* p,
    ZW_APPLICATION_TX_BUFFER *pCmd, WORD cmdLength)
{
  send_request_state_t* s;
  for (s = list_head(reqs_list); s; s=list_item_next(s))
  {
    if (s->state == REQ_WAITING && ts_param_cmp(&s->param, p)
        && s->class == pCmd->ZW_Common.cmdClass
        && s->cmd == pCmd->ZW_Common.cmd
         )
    {
      /*Only accept the command if it was received with the same security scheme which was used
       * durring transmission */
      if(s->param.scheme != p->scheme) {
        WRN_PRINTF("Message with wrong scheme received was %i expected %i\n",p->scheme,s->param.scheme);
        continue;
      }

      s->state = REQ_DONE;
      ctimer_stop(&s->timer);

      list_remove(reqs_list,s);
      memb_free(&reqs,s);
      s->callback(TRANSMIT_COMPLETE_OK, p->rx_flags, pCmd, cmdLength, s->user);
      return TRUE;
    }
  }
  return FALSE;
}

void
ZW_SendRequest_init()
{
  memb_init(&reqs);
  list_init(reqs_list);
}
