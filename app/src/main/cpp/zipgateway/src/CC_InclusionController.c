/* © 2017 Sigma Designs, Inc. This is an unpublished work protected by Sigma
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
/*
 * CC_ControllerInclusion.c
 *
 *  Created on: May 30, 2016
 *      Author: aes
 */

#include "ZIP_Router.h"
#include "NodeCache.h"
#include "CC_InclusionController.h"
#include "CC_NetworkManagement.h"
#include "Serialapi.h"
#include "ZW_classcmd_ex.h"
#include "ZW_SendDataAppl.h"
#include "command_handler.h"
#include "security_layer.h"
#include "sys/ctimer.h"
#include <stdint.h>
#include "ZW_ZIPApplication.h"
typedef enum  {EV_REQUEST_SUC_INCLUSION,EV_SUC_STARTED,EV_INITIATE, EV_COMPLETE,EV_TX_DONE_OK,
  EV_TX_DONE_FAIL, EV_TIMEOUT, EV_ADD_NODE_DONE, EV_S0_INCLUSION,EV_S0_INCLUSION_DONE} handler_events_t;

const char* handler_event_name(handler_events_t ev)
{
  static char str[25];

  switch(ev)
  {
  case EV_REQUEST_SUC_INCLUSION:    return "EV_REQUEST_SUC_INCLUSION";
  case EV_SUC_STARTED          :    return "EV_SUC_STARTED"; 
  case EV_INITIATE             :    return "EV_INITIATE"; 
  case EV_COMPLETE             :    return "EV_COMPLETE"; 
  case EV_TX_DONE_OK           :    return "EV_TX_DONE_OK"; 
  case EV_TX_DONE_FAIL         :    return "EV_TX_DONE_FAIL"; 
  case EV_TIMEOUT              :    return "EV_TIMEOUT"; 
  case EV_ADD_NODE_DONE        :    return "EV_ADD_NODE_DONE"; 
  case EV_S0_INCLUSION         :    return "EV_S0_INCLUSION"; 
  case EV_S0_INCLUSION_DONE    :    return "EV_S0_INCLUSION_DONE"; 
  default:
    sprintf(str, "%d", ev);
    return str;
  }
};



static struct {
  enum {STATE_IDLE, STATE_WAIT_FOR_SUC_ACCEPT, STATE_WAIT_FOR_ADD, STATE_WAIT_S0_INCLUSION,STATE_PERFORM_S0_INCLUSION } state;
  struct ctimer timer;
  inclusion_controller_cb_t complete_func;

  uint8_t node_id;
  union {
    uint8_t inclusion_conrtoller;
    uint8_t suc_node;
  };
  uint8_t is_replace;
  zwave_connection_t connection;
} handler_state;

const char* handler_state_name(int state)
{
  static char str[25];

  switch(state)
  {
  case STATE_IDLE                 : return "STATE_IDLE"; 
  case STATE_WAIT_FOR_SUC_ACCEPT  : return "STATE_WAIT_FOR_SUC_ACCEPT"; 
  case STATE_WAIT_FOR_ADD         : return "STATE_WAIT_FOR_ADD"; 
  case STATE_WAIT_S0_INCLUSION    : return "STATE_WAIT_S0_INCLUSION"; 
  case STATE_PERFORM_S0_INCLUSION : return "STATE_PERFORM_S0_INCLUSION"; 
  default:
    sprintf(str, "%d", state);
    return str;
  }
};
static void handler_fsm(handler_events_t ev, void* data) ;


static void timeout(void* user ) {
  handler_fsm(EV_TIMEOUT,0);
}

static void send_done(uint8_t status, void* user, TX_STATUS_TYPE *t) {
  UNUSED(t);
  handler_fsm(status == TRANSMIT_COMPLETE_OK ? EV_TX_DONE_OK : EV_TX_DONE_FAIL ,0 );
}

static void S0InclusionDone(int status) {
  handler_fsm(EV_S0_INCLUSION_DONE,&status);
}

static void handler_fsm(handler_events_t ev, void* event_data) {
  DBG_PRINTF("Inclusion Controller handler_fsm: state %s event %s\n",handler_state_name(handler_state.state), handler_event_name(ev));
  switch (handler_state.state) {
  case STATE_IDLE:
    if(ev == EV_REQUEST_SUC_INCLUSION) {
      handler_state.state = STATE_WAIT_FOR_SUC_ACCEPT;
      handler_state.complete_func = (inclusion_controller_cb_t)event_data;
      handler_state.suc_node = ZW_GetSUCNodeID();

      ZW_INCLUSION_CONTROLLER_INITIATE_FRAME frame;
      ts_param_t ts;
      ts_set_std(&ts,handler_state.suc_node);

      frame.cmdClass = COMMAND_CLASS_INCLUSION_CONTROLLER;
      frame.cmd = INCLUSION_CONTROLLER_INITIATE;
      frame.node_id = handler_state.node_id;
      frame.step_id = handler_state.is_replace ? STEP_ID_PROXY_REPLACE : STEP_ID_PROXY_INCLUSION ;

      if(!ZW_SendDataAppl(&ts,&frame,sizeof(frame),send_done,0) ) {
        send_done(TRANSMIT_COMPLETE_FAIL,0, NULL);
      }
    } else if(ev == EV_INITIATE) {
      ZW_INCLUSION_CONTROLLER_INITIATE_FRAME* frame = (ZW_INCLUSION_CONTROLLER_INITIATE_FRAME*) event_data;
      handler_state.node_id = frame->node_id;

      handler_state.state = STATE_WAIT_FOR_ADD;
      StopNewNodeProbeTimer();

      if(frame->step_id == STEP_ID_PROXY_INCLUSION) {
        NetworkManagement_start_proxy_inclusion(frame->node_id);
      } else if(frame->step_id == STEP_ID_PROXY_REPLACE) {
        NetworkManagement_start_proxy_replace(frame->node_id);
      }  else {

        handler_state.state = STATE_IDLE;
        ERR_PRINTF("invalid inclusion step.");
        return;
      }

    }
    break;
  case STATE_WAIT_FOR_SUC_ACCEPT:
    if(ev  == EV_TX_DONE_OK) {
      //ctimer_set(&handler_state.timer, CLOCK_SECOND *3, timeout, 0);
      ctimer_set(&handler_state.timer, CLOCK_SECOND *( 60* 4 +2), timeout, 0);
    } else if(ev == EV_COMPLETE) {
      ZW_INCLUSION_CONTROLLER_COMPLETE_FRAME* frame =(ZW_INCLUSION_CONTROLLER_COMPLETE_FRAME*) event_data;
      if(frame->step_id == STEP_ID_PROXY_INCLUSION) {
        handler_state.state = STATE_IDLE;
        handler_state.complete_func(1);
      }
    } else if ((ev == EV_TX_DONE_FAIL) || (ev == EV_TIMEOUT)) {
      handler_state.state = STATE_IDLE;
      handler_state.complete_func(0);
    } else if(ev == EV_INITIATE) {
      ZW_INCLUSION_CONTROLLER_INITIATE_FRAME* frame =(ZW_INCLUSION_CONTROLLER_INITIATE_FRAME*) event_data;

      if(frame->step_id == STEP_ID_S0_INCLUSION) {
        handler_state.state = STATE_PERFORM_S0_INCLUSION;
        security_add_begin(handler_state.node_id,
            TRANSMIT_OPTION_ACK | TRANSMIT_OPTION_EXPLORE | TRANSMIT_OPTION_AUTO_ROUTE,
            isNodeController(handler_state.node_id), S0InclusionDone);
      }
    }
    break;
  case STATE_PERFORM_S0_INCLUSION:
    if(EV_S0_INCLUSION_DONE) {
      ZW_INCLUSION_CONTROLLER_COMPLETE_FRAME frame;
      ts_param_t ts;
      ts_set_std(&ts,handler_state.suc_node);

      frame.cmdClass = COMMAND_CLASS_INCLUSION_CONTROLLER;
      frame.cmd = INCLUSION_CONTROLLER_COMPLETE;
      frame.step_id = STEP_ID_S0_INCLUSION;
      frame.status = *((int*)event_data);

      handler_state.state = STATE_WAIT_FOR_SUC_ACCEPT;

      if(!ZW_SendDataAppl(&ts,&frame,sizeof(frame),send_done,0) ) {
        send_done(TRANSMIT_COMPLETE_FAIL,0, NULL);
      }
    }
    break;
  case STATE_WAIT_FOR_ADD:
    if(ev == EV_ADD_NODE_DONE) {

      ZW_INCLUSION_CONTROLLER_COMPLETE_FRAME frame;
      ts_param_t ts;
      ts_set_std(&ts,handler_state.inclusion_conrtoller);

      frame.cmdClass = COMMAND_CLASS_INCLUSION_CONTROLLER;
      frame.cmd = INCLUSION_CONTROLLER_COMPLETE;
      frame.step_id = STEP_ID_PROXY_INCLUSION;
      frame.status = *((uint8_t*)event_data);

      if(!ZW_SendDataAppl(&ts,&frame,sizeof(frame),0,0)) {
      /* Dummy check. Code intentionally kept blank */
      }
      handler_state.state = STATE_IDLE;
    }
    else if(ev == EV_S0_INCLUSION) {
      ZW_INCLUSION_CONTROLLER_INITIATE_FRAME frame;
      ts_param_t ts;

      handler_state.complete_func = (inclusion_controller_cb_t)event_data;
      ts_set_std(&ts,handler_state.inclusion_conrtoller);

      frame.cmdClass = COMMAND_CLASS_INCLUSION_CONTROLLER;
      frame.cmd = INCLUSION_CONTROLLER_INITIATE;
      frame.node_id = handler_state.node_id;
      frame.step_id = STEP_ID_S0_INCLUSION;

      if(!ZW_SendDataAppl(&ts,&frame,sizeof(frame),send_done,0) ) {
        send_done(TRANSMIT_COMPLETE_FAIL,0, NULL);
      }

      handler_state.state = STATE_WAIT_S0_INCLUSION;
      ctimer_set(&handler_state.timer, CLOCK_SECOND *30, timeout, 0);
    }
    break;
  case STATE_WAIT_S0_INCLUSION:
    if( ev == EV_TIMEOUT) {
      handler_state.state = STATE_WAIT_FOR_ADD;
      handler_state.complete_func(0);
    } else if(ev == EV_COMPLETE) {
      ZW_INCLUSION_CONTROLLER_COMPLETE_FRAME* frame =(ZW_INCLUSION_CONTROLLER_COMPLETE_FRAME*) event_data;
      if(frame->step_id == STEP_ID_S0_INCLUSION) {
        handler_state.state = STATE_WAIT_FOR_ADD;
        handler_state.complete_func(frame->status);
      }
    }
    break;
  }
}

static command_handler_codes_t inclusuion_controller_handler(zwave_connection_t *connection, uint8_t* frame, uint16_t length)
{
  switch(frame[1]) {
  case INCLUSION_CONTROLLER_INITIATE:
    if(handler_state.state== STATE_IDLE) {
      handler_state.inclusion_conrtoller = nodeOfIP(&connection->ripaddr);
    }
    handler_fsm(EV_INITIATE,frame);
    break;
  case INCLUSION_CONTROLLER_COMPLETE:
    handler_fsm(EV_COMPLETE,frame);
    break;
  default:
    return COMMAND_NOT_SUPPORTED;
  }
  return COMMAND_HANDLED;
}

void inclusion_controller_you_do_it(inclusion_controller_cb_t complete_func) {
  handler_fsm(EV_S0_INCLUSION,complete_func);

}
void inclusion_controller_send_report(inclusion_cotroller_status_t status) {
  handler_fsm(EV_ADD_NODE_DONE,&status);
}

void inclusion_controller_started(void) {
  handler_fsm(EV_SUC_STARTED,0);
}



void request_inclusion_controller_handover( uint8_t node_id,uint8_t is_replace,inclusion_controller_cb_t complete_func  )
{
  if(handler_state.state != STATE_IDLE && complete_func) {
    complete_func(0);
  } else {
    handler_state.node_id = node_id;
    handler_state.is_replace = is_replace;
    handler_fsm(EV_REQUEST_SUC_INCLUSION,complete_func);
  }
}

void controller_inclusuion_init() {
  handler_state.state = STATE_IDLE;
}

REGISTER_HANDLER(
    inclusuion_controller_handler,
    controller_inclusuion_init,
    COMMAND_CLASS_INCLUSION_CONTROLLER, 1, NO_SCHEME);

