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

 /* Author: mdumbare */

#ifdef __C51__
/* Put XDATA in XDATA_LOWER because upper XDATA is full on some targets */
#pragma userclass (xdata = LOWER)
#endif

#include <stddef.h>
#include "transport2_fsm.h"
#ifdef GRAPH /* This is set by Makefile.transport2_fsm while generating graphs */
#define CODE
#define WORD unsigned short
#define BYTE unsigned char

#define VOID_CALLBACKFUNC(completedFunc)  void (CODE *completedFunc)
#else
#include "transport_service2.h"
//#include <TYPES.H>
#endif

#ifdef __C51__
#define slash /
#define printf  slash/

#define ST_DEFINE(a, b, c, d) {a, b, c}

#else

const char *T2_STATES_STRING[]  = {
        "ST_IDLE",

        /* Receive state machine states */
        "ST_RECEIVING",
        "ST_SEND_FRAG_COMPLETE",
        "ST_SEND_FRAG_REQ",
        "ST_SEND_FRAG_WAIT",
        "ST_FIND_MISS_FRAG",

        /* Send state machine states */
        "ST_SEND_FRAG",
        "ST_SEND_LAST_FRAG",
        "ST_WAIT_ACK",

};
const char *T2_EVENTS_STRING[] = {
        /* Receive state machine events */
        "EV_START_RECV",
        "EV_RECV_NEW_FRAG",
        "EV_NO_SESSION_ID",
        "EV_SEND_FRAG_COMPLETE",
        "EV_NO_SEND_FRAG_COMPLETE",
        "EV_SUCCESS",
        "EV_SUCCESS2",
        "EV_SUCCESS3",
        "EV_FRAG_RX_TIMER",
        "EV_MISSING_FRAG",
        "EV_MISSING_FRAG_BCAST",
        "EV_SCAST_DIFF_NODE",
        "EV_BCAST_DIFF_NODE",
        "EV_DIFF_SESSION",
        "EV_SUBSEQ_DIFF_SESSION",
        "EV_FRAG_REQ_COMPL_DIFF_SESSION",
        "EV_FRAG_REQ_COMPL_WAIT_DIFF_NODE",
        "EV_RECV_FRAG_REQ",
        "EV_RECV_LAST_FRAG",
        "EV_RECV_FRAG_COMPLETE",
        "EV_TIE_BREAK",
        "EV_DUPL_FRAME",
        "EV_FRAG_REQ_OR_COMPL",

        /* Send state machine events */
        "EV_START_SEND",
        "EV_SEND_NEW_FRAG",
        "EV_SEND_NXT_MISS_FRAG",
        "EV_SENT_MISS_FRAG",
        "EV_RECV_FRAG_WAIT",
        "EV_SEND_LAST_FRAG",
        "EV_SEND_LAST_MISS_FRAG",
        "EV_FRAG_COMPL_TIMER",
        "EV_FRAG_COMPL_TIMER2",
        "EV_FRAG_COMPL_TIMER_REQ",
        "EV_FAILURE_LAST_FRAG2",
        "EV_RECV_FRAG_COMPL",
        "EV_REPLY_FRAG_REQ_DONE",
        "EV_FAIL",
};

#define ST_DEFINE(a, b, c, d) {a, b, c, d}

#endif

#define STATE enum TRANSPORT2_STATES
#define EVENT enum TRANSPORT2_EVENTS

typedef struct {
        STATE st;             /* Current state */
        EVENT ev;             /* Incoming event */
        STATE next_st;
        char *fun_name;    /* Action function returning next state */
} transition_t;

/* State transition table
 * This is where the state machine is defined.
 *
 * Each transition can optionally trigger a call to an action function.
 * An action function defines the actions that must occur when a particular
 * transition occurs.
 *
 * Format: {Current state, incoming event, next state, action_function} */
static code const transition_t trans[] = {
        /* ========================*/
        /* Receiving State diagram */
        /* ========================*/

        /* First event when on receiving end. */
         ST_DEFINE(ST_IDLE, EV_START_RECV, ST_RECEIVING, "start receive"),
         ST_DEFINE(ST_IDLE, EV_START_SEND, ST_SEND_FRAG, "send_first_frag"),

        /* This happens when there is a fragment received from different session or node, which
            will be ignored. Then the state machine lands up in ST_IDLE */
         ST_DEFINE(ST_IDLE, EV_SEND_NEW_FRAG, ST_SEND_FRAG, "keep sending"),

        /* Send new frame. Call the same funciton i.e. send(); */
         ST_DEFINE(ST_SEND_FRAG, EV_SEND_NEW_FRAG, ST_SEND_FRAG, "send_subseq_frag"),

         ST_DEFINE(ST_RECEIVING, EV_SEND_NEW_FRAG, ST_SEND_FRAG, "send"),
         ST_DEFINE(ST_WAIT_ACK, EV_SEND_NEW_FRAG, ST_SEND_FRAG, "send"),

        /* If the current state is ST_RECEIVING and new fragment arrives */
        /* If the fragment length is wrong */
        /* If the crc is bad */
         ST_DEFINE(ST_RECEIVING, EV_RECV_NEW_FRAG, ST_RECEIVING, "process new fram"),

        /* If duplicate frame arrives after frag completion move back to idle state */
         ST_DEFINE(ST_RECEIVING, EV_DUPL_FRAME, ST_IDLE, "discard the fragment"),

        /* If the fragment received is corrupt and there is no session ID. Receiving node
         * has to send Fragment wait command to sending node */
         ST_DEFINE(ST_RECEIVING, EV_NO_SESSION_ID, ST_SEND_FRAG_WAIT, "send_frag_wait_cmd"),

        /* If the fragment received is singlecast fragment while receiving another fragmented
         * datagram. Receiving node has to send Fragment wait command to sending node */
         ST_DEFINE(ST_RECEIVING, EV_SCAST_DIFF_NODE, ST_SEND_FRAG_WAIT, "send_frag_wait_cmd"),

        /* If the fragment received is singlecast fragment while sending another fragmented
         * datagram. */
         ST_DEFINE(ST_SEND_FRAG, EV_SCAST_DIFF_NODE, ST_SEND_FRAG_WAIT, "send_frag_wait_cmd"),

        /* If the fragment received is singlecast fragment while waiting for ack (frag req or frag compl)
        */
        ST_DEFINE(ST_WAIT_ACK, EV_SCAST_DIFF_NODE, ST_SEND_FRAG_WAIT, "send_frag_wait_cmd"),


        /* If the fragment received is singlecast fragment while receiving fragmented
         * datagram from different session. Receiving node has to discard the fragment */
         ST_DEFINE(ST_RECEIVING, EV_DIFF_SESSION, ST_IDLE, "discard the fragment"),

         ST_DEFINE(ST_RECEIVING, EV_SUBSEQ_DIFF_SESSION, ST_SEND_FRAG_WAIT, "send frag_wait"),

        /* Fragment request/complete/wait from different node id*/
         ST_DEFINE(ST_RECEIVING, EV_FRAG_REQ_COMPL_WAIT_DIFF_NODE, ST_IDLE, "discard the fragment"),

        /* Fragment request/complete/wait from different session */
         ST_DEFINE(ST_RECEIVING, EV_FRAG_REQ_COMPL_WAIT_DIFF_SESSION, ST_IDLE, "discard the fragment"),

        /* Go back to receiving state if Fragment wait command is sent successfully */
         ST_DEFINE(ST_SEND_FRAG_WAIT, EV_SUCCESS, ST_RECEIVING, NULL),
        /* Go back to sending state as the fragment wait was sent in sending session */
         ST_DEFINE(ST_SEND_FRAG_WAIT, EV_SUCCESS2, ST_SEND_FRAG, NULL),
        /* Go back to ST_WAIT_ACK as the fragment wait was sent in sending session */
         ST_DEFINE(ST_SEND_FRAG_WAIT, EV_SUCCESS3, ST_WAIT_ACK, NULL),

        /* TODO. What if sending fragment wait command fails ?*/

        /* If Fragment complete has been sent to sending node successfully, Go back to receiving
         * state */
         ST_DEFINE(ST_SEND_FRAG_COMPLETE, EV_SUCCESS, ST_IDLE, NULL),
        /*After sending one fragment request go back to receiving state, either we will receive
        another fragment request of fragment complete */
         ST_DEFINE(ST_SEND_FRAG_REQ, EV_SUCCESS, ST_RECEIVING, NULL),

        /* If RX timer event happens while ST_RECEIVING */
         ST_DEFINE(ST_RECEIVING, EV_FRAG_RX_TIMER, ST_FIND_MISS_FRAG, "find_missing"),
        /* If there is Fragment Rx timer event whiel sending Fragment Req command discard all the previous
         * Discard received fragmets in the datagram and go to Idle state */
         ST_DEFINE(ST_IDLE, EV_FRAG_RX_TIMER, ST_IDLE, "discard_all_received_fragments"),

        /* If the fragment received is broadcast fragment while receiving another fragmented datagram,
         * Receiving node needs to drop that fragment */
         ST_DEFINE(ST_RECEIVING, EV_BCAST_DIFF_NODE, ST_RECEIVING, "drop_fragment"),

         ST_DEFINE(ST_RECEIVING, EV_RECV_LAST_FRAG, ST_FIND_MISS_FRAG, "find_missing"),
         ST_DEFINE(ST_WAIT_ACK, EV_SEND_FRAG_COMPLETE, ST_SEND_FRAG_COMPLETE, NULL),
         ST_DEFINE(ST_RECEIVING, EV_RECV_FRAG_COMPL, ST_IDLE, NULL),

        /* Stay in the same state ST_WAIT_ACK until we know if its REQUEST OR COMPLETE cmd */
         ST_DEFINE(ST_WAIT_ACK, EV_FRAG_REQ_OR_COMPL, ST_WAIT_ACK, NULL),

        /* If the rx timer expires after sending Fragment Request, discard all fragments and go to IDLE state */
         ST_DEFINE(ST_WAIT_ACK, EV_FRAG_RX_TIMER, ST_IDLE, "discard all fragments"),

        /* Need to move to ST_RECEIVING state to process the FRAGMENT_REQUEST or FRAGMENT_COMPLETE received while sending */
         ST_DEFINE(ST_RECEIVING, EV_FRAG_REQ_OR_COMPL, ST_RECEIVING, "process received"),
         ST_DEFINE(ST_SEND_FRAG, EV_FRAG_REQ_OR_COMPL, ST_RECEIVING, "process received"),

        /* If there are missing frames, receiving node needs to send Fragment request command to
         * sending node */
         ST_DEFINE(ST_FIND_MISS_FRAG, EV_MISSING_FRAG, ST_SEND_FRAG_REQ, "send_frag_req_cmd"),
        /* If the broacast fragment is being received and rx timer times out of there are some missing fragments*/
         ST_DEFINE(ST_FIND_MISS_FRAG, EV_MISSING_FRAG_BCAST, ST_IDLE, NULL),
        /* If all the fragments of the datagram are receivied receiving node needs to send
         * Fragment complete command to sending node. */
         ST_DEFINE(ST_FIND_MISS_FRAG, EV_SEND_FRAG_COMPLETE, ST_SEND_FRAG_COMPLETE, "send_frag_complete_cmd"),

        /* NO need to send frag complete if it was bcast datagram which was being recived */
         ST_DEFINE(ST_FIND_MISS_FRAG, EV_NO_SEND_FRAG_COMPLETE, ST_IDLE, NULL),

        /* ======================*/
        /* Sending State diagram */
        /* ======================*/


         ST_DEFINE(ST_SEND_FRAG, EV_RECV_FRAG_WAIT, ST_SEND_FRAG, "wait_restart_from_first"),

        /* ST_SEND_FRAG ->(EV_TIE_BREAK) -> ST_RECEIVING ->(EV_RECV_NEW_FRAG) ->( EV_RECV_FRAG_WAIT )*/
         ST_DEFINE(ST_RECEIVING, EV_RECV_FRAG_WAIT, ST_SEND_FRAG, "wait_restart_from_first"),

         ST_DEFINE(ST_SEND_FRAG, EV_SEND_LAST_FRAG, ST_SEND_LAST_FRAG, "send_last_frag"),
         ST_DEFINE(ST_SEND_FRAG, EV_SEND_LAST_MISS_FRAG, ST_SEND_LAST_FRAG, "send_last_frag"),

         ST_DEFINE(ST_SEND_LAST_FRAG, EV_SUCCESS, ST_WAIT_ACK, NULL),
        /* If the Fragment completion timer even happens send the last fragment again */
         ST_DEFINE(ST_WAIT_ACK, EV_FRAG_COMPL_TIMER, ST_SEND_LAST_FRAG, "send_last_frag"),

        /*  fc_timer_expired in reply_frag_req*/
         ST_DEFINE(ST_WAIT_ACK, EV_FRAG_COMPL_TIMER_REQ, ST_IDLE, "send failure to application"),

        /* If the Fragment completion timer event happens again send error to application */
         ST_DEFINE(ST_WAIT_ACK, EV_FRAG_COMPL_TIMER2, ST_IDLE, NULL),

        /* Failure in sending last fragment second time after one Fragment completion timer event */
        /*P.S. This is not failure in sending the last fragment at first time */
         ST_DEFINE(ST_WAIT_ACK, EV_FAILURE_LAST_FRAG2, ST_IDLE, NULL),

        /* If received Fragment request command */
         ST_DEFINE(ST_WAIT_ACK, EV_RECV_FRAG_REQ, ST_SEND_FRAG, "reply_frag_req"),
         ST_DEFINE(ST_RECEIVING, EV_RECV_FRAG_REQ, ST_SEND_FRAG, "reply_frag_req"),

        /* If received Fragment complete command, disable the fragment complete timer*/
         ST_DEFINE(ST_WAIT_ACK, EV_RECV_FRAG_COMPL, ST_IDLE, NULL),

         ST_DEFINE(ST_SEND_FRAG, EV_SEND_NXT_MISS_FRAG, ST_SEND_FRAG, "reply_frag_req"),
         ST_DEFINE(ST_SEND_FRAG, EV_FRAG_COMPL_TIMER, ST_IDLE, "wait"),

        /* This happens when the FC_complete timer happens before  reply_frag_req() gets a callback
           state machine is still in ST_SEND_FRAG as it goes in ST_WAIT_ACK in the callback*/
         ST_DEFINE(ST_SEND_FRAG, EV_FRAG_COMPL_TIMER_REQ, ST_IDLE, "wait"),

         ST_DEFINE(ST_SEND_FRAG, EV_SENT_MISS_FRAG, ST_WAIT_ACK, NULL),
         ST_DEFINE(ST_SEND_FRAG, EV_TIE_BREAK, ST_RECEIVING, "receive"),
         ST_DEFINE(ST_SEND_LAST_FRAG, EV_TIE_BREAK, ST_RECEIVING, "receive"),
         ST_DEFINE(ST_WAIT_ACK, EV_RECV_FRAG_WAIT, ST_SEND_FRAG, NULL),

        /* If the ZW_SendData() function itself fails we go back to IDLE state
           The other side will send fragment request command. But we need to be in IDLE
            state to atleast receive and process it */
         ST_DEFINE(ST_SEND_LAST_FRAG, EV_FAIL, ST_IDLE, NULL),
         ST_DEFINE(ST_WAIT_ACK, EV_TIE_BREAK, ST_RECEIVING, "receive"),
};

TRANSPORT2_ST_T current_state = ST_IDLE;
uint8_t find_transition(TRANSPORT2_ST_T cstate, TRANSPORT2_EV_T event)
{
        uint8_t num_trans;

        num_trans = sizeof(trans) / sizeof(trans[0]); /* FIXME better to move it inside some init function */
//        printf("Transprot2: Current State: %s(%d), Event: %s(%d)\n", T2_STATES_STRING[current_state], current_state, T2_EVENTS_STRING[event], event);
        while(num_trans--) {        
                if ((cstate == trans[num_trans].st) && (event == trans[num_trans].ev))
                        return num_trans+1;
        }

//        printf("Transport2: Transition not found.");
        return 0;
}

void t2_sm_post_event(TRANSPORT2_EV_T ev)
{
/*
        function fn;
*/
        uint8_t i;

        i = find_transition(current_state, ev);
        if (i) {
                current_state = trans[i-1].next_st;
        } else {
        /* TODO FIXME: can not find the transtion. probably stay in the same state */
        /* print error for debugging */
        }
}
