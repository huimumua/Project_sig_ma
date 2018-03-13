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
#include <transport_service2.h>
ts_param_t p;

//#define DEBUG
#ifdef DEBUG
#define v_printf(format, args...)  printf(format, ## args);
#else
#define v_printf(format, args...)
#endif
#define fail_if_nonzero(ret) if (ret) \
                            goto fail;
unsigned char test_first_frag1[] = 
{
 0x55,  0xc0,  0x6a,  0x00,  0x20,  0x01,  0x00,  0x03,
 0x04,  0x05,  0x06,  0x07,  0x08,  0x09,  0x0a,  0x0b,
 0x0c,  0x0d,  0x0e,  0x0f,  0x10,  0x11,  0x12,  0x13,
 0x14,  0x15,  0x16,  0x17,  0x18,  0x19,  0x1a,  0x1b,
 0x1c,  0x1d,  0x1e,  0x1f,  0x20,  0x21,  0x22,  0x23,
 0x24,  0x25,  0x26,  0x27,  0x28,  0x29,  0x2a,  0x2b,
 0x2c,  0x2d,  0x2e,  0x04,  0xa5
};
unsigned char test_subseq_frag2[] =
{
 0x55, 0xe0, 0x6a, 0x00, 0x2f, 0x2f, 0x30, 0x31, 0x32,
 0x33, 0x34, 0x20, 0x01, 0x00, 0x03, 0x04, 0x05, 0x06, 0x07,
 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11,
 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a,
 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23,
 0x24, 0x25, 0x26, 0x27, 0x28, 0x1d, 0x69
};
unsigned char test_subseq_frag3[] =
{
 0x55, 0xe0, 0x6a, 0x00, 0x5e, 0x29, 0x2a, 0x2b, 0x2c,
 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x05, 0xd6
};
unsigned char test_first_frag[] = {0x55 ,0xC0 ,0x35 ,0x00 ,0x20 ,0x01 ,0x00 ,0x03 ,0x04 ,0x05 ,0x06 ,0x07 ,0x08 ,0x09 ,0x0A ,0x0B ,0x0C ,0x0D ,0x0E ,0x0F ,0x10 ,0x11 ,0x12 ,0x13 ,0x14 ,0x15 ,0x16 ,0x17 ,0x18 ,0x19 ,0x1A ,0x1B ,0x1C ,0x1D ,0x1E ,0x1F ,0x20 ,0x21 ,0x22 ,0x23 ,0x24 ,0x25 ,0x26 ,0x27 ,0x28 ,0x29 ,0x2A ,0x2B ,0x2C ,0x2D ,0x2E ,0x56, 0xd9};
unsigned char test_subseq_frag[] = {0x55 ,0xE0 ,0x35 ,0x00 ,0x2F ,0x2F ,0x30 ,0x31 ,0x32 ,0x33 ,0x34 ,0x53, 0x74};
unsigned char test_frag_compl[] = {0x55, 0xE8, 0x00};
unsigned char test_frag_req[] = {0x55, 0xC8, 0x00, 0x2f};
unsigned char test_frag_req2[] = {0x55, 0xC8, 0x00, 0x5e};
unsigned char test_frag_wait_zero_pending[] = {0x55, 0xF0, 0x00};
unsigned char test_frag_wait[] = {0x55, 0xF0, 0x01};
unsigned char test_frag_wait_two_pending[] = {0x55, 0xF0, 0x02};
unsigned char test_frag_wait_three_pending[] = {0x55, 0xF0, 0x03};
unsigned char output[1024];
extern void fire_rx_timer();
extern void fire_timer_btwn_2_frags(int test_type);
extern int fc_timer_counter;
extern uint8_t MyNodeID;
extern int check_flag_tie_broken();
extern void fire_fc_timer();
extern int check_flag_fc_timer_expired_once();
extern int check_scb_current_dnode();
extern int get_current_scb_cmn_session_id();
extern WORD ZW_CheckCrc16(WORD crc, BYTE *pDataAddr, WORD bDataLen);
extern int call_with_large_value;

unsigned char global_status = 0xff;
void status_callback(uint8_t txStatus, void *t)
{
    global_status = txStatus;
}

unsigned char raw_data [] = {0x20, 0x01, 0x00 ,0x03 ,0x04 ,0x05 ,0x06 ,0x07 ,0x08 ,0x09 ,0x0A ,0x0B ,0x0C ,0x0D ,0x0E ,0x0F ,0x10 ,0x11 ,0x12 ,0x13 ,0x14 ,0x15 ,0x16 ,0x17 ,0x18 ,0x19 ,0x1A ,0x1B ,0x1C ,0x1D ,0x1E ,0x1F ,0x20 ,0x21 ,0x22 ,0x23 ,0x24 ,0x25 ,0x26 ,0x27 ,0x28 ,0x29 ,0x2A ,0x2B ,0x2C ,0x2D ,0x2E, 0x2F ,0x30 ,0x31 ,0x32 ,0x33 ,0x34};
unsigned char raw_data2 [] = {0x20, 0x01, 0x00 ,0x03 ,0x04 ,0x05 ,0x06 ,0x07 ,0x08 ,0x09 ,0x0A ,0x0B ,0x0C ,0x0D ,0x0E ,0x0F ,0x10 ,0x11 ,0x12 ,0x13 ,0x14 ,0x15 ,0x16 ,0x17 ,0x18 ,0x19 ,0x1A ,0x1B ,0x1C ,0x1D ,0x1E ,0x1F ,0x20 ,0x21 ,0x22 ,0x23 ,0x24 ,0x25 ,0x26 ,0x27 ,0x28 ,0x29 ,0x2A ,0x2B ,0x2C ,0x2D ,0x2E, 0x2F ,0x30 ,0x31 ,0x32 ,0x33 ,0x34, 0x20, 0x01, 0x00 ,0x03 ,0x04 ,0x05 ,0x06 ,0x07 ,0x08 ,0x09 ,0x0A ,0x0B ,0x0C ,0x0D ,0x0E ,0x0F ,0x10 ,0x11 ,0x12 ,0x13 ,0x14 ,0x15 ,0x16 ,0x17 ,0x18 ,0x19 ,0x1A ,0x1B ,0x1C ,0x1D ,0x1E ,0x1F ,0x20 ,0x21 ,0x22 ,0x23 ,0x24 ,0x25 ,0x26 ,0x27 ,0x28 ,0x29 ,0x2A ,0x2B ,0x2C ,0x2D ,0x2E, 0x2F ,0x30 ,0x31 ,0x32 ,0x33 ,0x34};
void ask_TS_to_send()
{
    ZW_TransportService_SendData(&p, raw_data2, sizeof(raw_data2), status_callback);
}

void ask_TS_to_receive(unsigned char *cmd, unsigned int len)
{
    TransportService_ApplicationCommandHandler(&p, cmd, len);
}
int print_failed_if_nonzero(int ret, const char *test_name)
{   
   if( ret != 0) {
        v_printf("\t%s failed <-------------\n", test_name);
        return 1;
    } else {
        v_printf("\t%s passed\n", test_name);
        return 0;
    }
}
/* -------------- Test transport service's Receiving functionality ---------------------------------------*/
int simple_test()
{
    int ret = 0;
    printf("simple_test\n");
    ask_TS_to_receive(test_first_frag1, sizeof(test_first_frag1));
    ask_TS_to_receive(test_subseq_frag2, sizeof(test_subseq_frag2));
    ask_TS_to_receive(test_subseq_frag3, sizeof(test_subseq_frag3));
//    fire_rx_timer();
    ret = memcmp(output, test_frag_compl, sizeof(test_frag_compl));
    ret = print_failed_if_nonzero(ret, "simple test");
    fail_if_nonzero(ret);
    ret = print_failed_if_nonzero(!(current_state == ST_IDLE), "current state has to be ST_IDLE...");
    fail_if_nonzero(ret);
    printf("passed\n");
    return 0;
fail:
    printf("failed\n");
    return 1;

}

int dont_send_one_frag()
{
    int ret = 0;
    printf("dont_send_one_frag\n");

    ask_TS_to_receive(test_first_frag1, sizeof(test_first_frag1));
    fire_rx_timer();

    ret = memcmp(output, test_frag_req, sizeof(test_frag_req));
    ret = print_failed_if_nonzero(ret, "dont_send_one_frag frag_req check");
    fail_if_nonzero(ret);

    ask_TS_to_receive(test_subseq_frag2, sizeof(test_subseq_frag2));
    ask_TS_to_receive(test_subseq_frag3, sizeof(test_subseq_frag3));
    ret = memcmp(output, test_frag_compl, sizeof(test_frag_compl));
    ret = print_failed_if_nonzero(ret, "dont_send_one_frag frag_compl check");
    fail_if_nonzero(ret);
    ret = print_failed_if_nonzero(!(current_state == ST_IDLE), "current state has to be ST_IDLE...");
    fail_if_nonzero(ret);
    printf("passed\n");
    return 0;
fail:
    printf("failed\n");
    return 1;
}

int test_dont_send_first_frag()
{
    int ret = 0;
    printf("test_dont_send_first_frag\n");
    ask_TS_to_receive(test_subseq_frag2, sizeof(test_subseq_frag2)); /* Skip sending first frag and send subseq frag instead */

    ret = memcmp(output, test_frag_wait_zero_pending, sizeof(test_frag_wait_zero_pending)); /* Check if we get fragment wait */
    ret = print_failed_if_nonzero(ret, "test_dont_send_first_frag frag_wait");
    fail_if_nonzero(ret);

    ret = print_failed_if_nonzero(!(current_state == ST_IDLE), "current state has to be ST_IDLE...");
    fail_if_nonzero(ret);

    printf("passed\n");
    return 0;
fail:
    printf("failed\n");
    return 1;
}

int test_frag_wait_fn()
{
    int ret = 0;

    printf("test_frag_wait_fn\n");
    ask_TS_to_receive(test_first_frag1, sizeof(test_first_frag1)); /* Send first frag */

    p.snode = 0xf1;
    ask_TS_to_receive(test_subseq_frag2, sizeof(test_subseq_frag2)); /* Send subseq frag from different source node */
    p.snode = 0xff;

    ret = memcmp(output, test_frag_wait_two_pending, sizeof(test_frag_wait_two_pending));
    ret = print_failed_if_nonzero(ret, "test_frag_wait receive check ");
    fail_if_nonzero(ret);

    memset(output, 0, sizeof(test_frag_compl));
    ask_TS_to_receive(test_subseq_frag2, sizeof(test_subseq_frag2)); /* Send correct second subseq frag */
    ask_TS_to_receive(test_subseq_frag3, sizeof(test_subseq_frag3)); /* Send correct third subseq frag */

    ret = memcmp(output, test_frag_compl, sizeof(test_frag_compl)); /* Check if we recive fragment complete */
    ret = print_failed_if_nonzero(ret, "frag_complete receive check");

    ret = print_failed_if_nonzero(!(current_state == ST_IDLE), "current state has to be ST_IDLE...");
    fail_if_nonzero(ret);
    printf("passed\n");
    return 0;
fail:
    printf("failed\n");
    return 1;

}
/* -------------- Test transport service's Sending functionality ---------------------------------------*/

extern TRANSPORT2_ST_T current_state;
/* Purpose of this test is to test the sending functionality of TS

Steps: 
1. Ask TS to send first frag and check
2. Ask TS to send second frag and check
3. Ask TS to receive Fragment_complete and see if it has processed the fragment complete
    by checking if the scb.current_dnode == 0
*/
int test_send_whole_datagram()
{
    int ret = 0;
    printf("test_send_whole_datagram\n");
    ask_TS_to_send();

    ret = memcmp(output, test_first_frag1, sizeof(test_first_frag1));
    ret = print_failed_if_nonzero(ret, "tets_send_whole_data_gram first fragment");
    fail_if_nonzero(ret);

    fire_timer_btwn_2_frags(0);
    ret = memcmp(output, test_subseq_frag2, sizeof(test_subseq_frag2));
    ret = print_failed_if_nonzero(ret, "tets_send_whole_data_gram first subseq fragment");
    fail_if_nonzero(ret);

    fire_timer_btwn_2_frags(0);
    ret = memcmp(output, test_subseq_frag3, sizeof(test_subseq_frag3));
    ret = print_failed_if_nonzero(ret, "tets_send_whole_data_gram second subseq fragment");
    fail_if_nonzero(ret);

    p.snode = 0xfe;
    p.dnode = 0xff;
    ask_TS_to_receive(test_frag_compl, sizeof(test_frag_compl));
    p.snode = 0xff;
    p.dnode = 0xfe;

    ret = print_failed_if_nonzero(!(global_status == S2_TRANSMIT_COMPLETE_OK), "Did transmissino OK for sending session...");
    fail_if_nonzero(ret);
    ret = print_failed_if_nonzero(check_scb_current_dnode(), "tets_send_whole_data_gram frag_compl processing");
    fail_if_nonzero(ret);
    ret = print_failed_if_nonzero(!(current_state == ST_IDLE), "current state has to be ST_IDLE...");
    fail_if_nonzero(ret);
    printf("passed\n");
    return 0;
fail:
    printf("failed\n");
    return 1;
}

/* Purpose of this test is to send fragment request frame with different session ID,
    while transport service is in the middle of sending different session ID. This test checks if
    the transport service sending side fires the fragment complete timer. Which
    it should not do as this fragment has to be ignored. 

Steps:
1. Ask the TS to send first frag and check
2. ask the TS to receive frament request with different session id
3. Check if fc_timer is fired
4. ask the TS to send second frag and check
*/

int test_jakob()
{
    int ret = 0;
    unsigned backup_byte;

    printf("test_jakob\n");
    ask_TS_to_send();

    ret = memcmp(output, test_first_frag1, sizeof(test_first_frag1));
    ret = print_failed_if_nonzero(ret, "test_jakob first fragment");
    fail_if_nonzero(ret);

    backup_byte = test_frag_req[2]; /*Change the session ID of frag_req cmd */
    test_frag_req[2] = 0xA0;
    fc_timer_counter = 0;
    p.snode = 0xfe;
    p.dnode = 0xff;
    ask_TS_to_receive(test_frag_req, sizeof(test_frag_req)); /* <-- Send fragment request with different session id */
    ret = print_failed_if_nonzero(fc_timer_counter, "test_send_frag_req_with_diff_session_id(jakob)");
    fail_if_nonzero(ret);
    test_frag_req[2] = backup_byte;

    fire_timer_btwn_2_frags(0);
    ret = memcmp(output, test_subseq_frag2, sizeof(test_subseq_frag2));
    ret = print_failed_if_nonzero(ret, "test_jakob first subseq fragment");
    fail_if_nonzero(ret);

    fire_timer_btwn_2_frags(0);
    ret = memcmp(output, test_subseq_frag3, sizeof(test_subseq_frag3));
    ret = print_failed_if_nonzero(ret, "test_jakob second subseq fragment");
    fail_if_nonzero(ret);

    p.snode = 0xfe;
    p.dnode = 0xff;
    ask_TS_to_receive(test_frag_compl, sizeof(test_frag_compl)); /* <-- Send fragment request with different session id */

    ret = print_failed_if_nonzero(!(global_status == S2_TRANSMIT_COMPLETE_OK), "Did transmissino OK for sending session...");
    fail_if_nonzero(ret);

    ret = print_failed_if_nonzero(!(current_state == ST_IDLE), "current state has to be ST_IDLE...");
    fail_if_nonzero(ret);

    printf("passed\n");
    return 0;
fail:
    printf("failed\n");
    return 1;
    

}

/* Purpose of this test is to see if the sending side tie breaks on receival of segment
steps:
1. set source node as more than destination node
2. ask transport service to send 
3. set source node as less than destination node
4. ask TS to receive first frag
        At this point tie must be broken on receiving side as
        - TS is in Sending mode (step 1)
        - We, the sender of the new fragment(step 4) is receiver of current datagram transmission(step 1) from TS in progress
        - Node id of TS is less than ours
5. check the flag_tie_broken
6. ask TS to receive second frag
7. check if we recive frag_compl
*/
int test_tie_break()
{
    int ret = 0;

    printf("test_tie_break\n");
    p.snode = 0xfe;
    p.dnode = 0xff;
    ask_TS_to_send();
    p.snode = 0xff;
    p.dnode = 0xfe;
    MyNodeID = 0xfe;

    ask_TS_to_receive(test_first_frag1, sizeof(test_first_frag1)); /* <-- ask to receive*/
    ret = print_failed_if_nonzero(!check_flag_tie_broken(), "Flag tie broken");
    fail_if_nonzero(ret);
    ask_TS_to_receive(test_subseq_frag2, sizeof(test_subseq_frag2));/* <-- ask to receive*/
    ask_TS_to_receive(test_subseq_frag3, sizeof(test_subseq_frag3));/* <-- ask to receive*/
//    fire_rx_timer();
    ret = memcmp(output, test_frag_compl, sizeof(test_frag_compl));

    ret = print_failed_if_nonzero(!(global_status == S2_TRANSMIT_COMPLETE_OK), "Did transmissino OK for sending session...");
    fail_if_nonzero(ret);

    ret = print_failed_if_nonzero(ret, "test_tie_break");
    fail_if_nonzero(ret);

    ret = print_failed_if_nonzero(!(current_state == ST_IDLE), "current state has to be ST_IDLE...");
    fail_if_nonzero(ret);

    printf("passed\n");
    return 0;
fail:
    printf("failed\n");
    return 1;
}

/* Purpose of this test is to check if the on timeout of fc_timer does transport service
   send the last fragment again
steps:
1. ask TS to send first frag and check if we receive it
2. ask TS to send second frag and check if we receive it
3. ask TS to send second frag again by firing the fc_timer and check if we receive it
4. check if the flag_fc_timer_expired_once is 1
*/
int test_two_last_fragments()
{
    int ret = 0;
    printf("test_two_last_fragments\n");
    ask_TS_to_send();

    ret = memcmp(output, test_first_frag1, sizeof(test_first_frag1));
    print_failed_if_nonzero(ret, "test_two_last_fragments first fragment");

    fire_timer_btwn_2_frags(0);/* <-- ask to send second frag */
    ret = memcmp(output, test_subseq_frag2, sizeof(test_subseq_frag2));
    ret = print_failed_if_nonzero(ret, "test_two_last_fragments second fragment");
    fail_if_nonzero(ret);

    fire_timer_btwn_2_frags(0);/* <-- ask to send second frag */
    ret = memcmp(output, test_subseq_frag3, sizeof(test_subseq_frag3));
    ret = print_failed_if_nonzero(ret, "test_two_last_fragments second fragment");
    fail_if_nonzero(ret);

    memset(output, 0, sizeof(test_subseq_frag3));
    fire_fc_timer(); /*Tell transport service that we did not receive the last fragment */
    ret = memcmp(output, test_subseq_frag3, sizeof(test_subseq_frag3));
    ret = print_failed_if_nonzero(ret, "test_two_last_fragments second fragment again");
    fail_if_nonzero(ret);

    ret = print_failed_if_nonzero(!check_flag_fc_timer_expired_once(), "fc_timer_expired_flag_check");
    fail_if_nonzero(ret);
    p.snode = 0xfe;
    p.dnode = 0xff;
    ask_TS_to_receive(test_frag_compl, sizeof(test_frag_compl));
    ret = print_failed_if_nonzero(!(current_state == ST_IDLE), "current state has to be ST_IDLE...");
    p.snode = 0xff;
    p.dnode = 0xfe;
    fail_if_nonzero(ret);
    ret = print_failed_if_nonzero(!(global_status == S2_TRANSMIT_COMPLETE_OK), "Did transmissino OK for sending session...");
    fail_if_nonzero(ret);

    printf("passed\n");
    return 0;
fail:
    printf("failed\n");
    return 1;
}

int test_send_frag_compl_from_diff_session()
{
    int ret = 0;
    unsigned backup_byte;

    printf("test_send_frag_compl_from_diff_session\n");
    ask_TS_to_send(); /* <-- Start sending session */

    ret = memcmp(output, test_first_frag1, sizeof(test_first_frag1));
    ret = print_failed_if_nonzero(ret, "test_send_frag_compl_from_diff_session first fragment");
    fail_if_nonzero(ret);

    fire_timer_btwn_2_frags(0);
    ret = memcmp(output, test_subseq_frag2, sizeof(test_subseq_frag2));
    ret = print_failed_if_nonzero(ret, "test_send_frag_compl_from_diff_session second fragment");
    fail_if_nonzero(ret);

    fire_timer_btwn_2_frags(0);
    ret = memcmp(output, test_subseq_frag3, sizeof(test_subseq_frag3));
    ret = print_failed_if_nonzero(ret, "test_send_frag_compl_from_diff_session second fragment");
    fail_if_nonzero(ret);

    backup_byte = test_frag_compl[2];
    test_frag_compl[2] = 0xA0;
    fc_timer_counter = 0;
    p.snode = 0xfe;
    p.dnode = 0xff;
    ask_TS_to_receive(test_frag_compl, sizeof(test_frag_compl)); /* <-- Send fragment request with different session id */
    ret = print_failed_if_nonzero(fc_timer_counter, "test_send_frag_compl_from_diff_session fc_timer");
    fail_if_nonzero(ret);
    test_frag_compl[2] = backup_byte;
    ask_TS_to_receive(test_frag_compl, sizeof(test_frag_compl));
    p.snode = 0xff;
    p.dnode = 0xfe;
    ret = print_failed_if_nonzero(!(global_status == S2_TRANSMIT_COMPLETE_OK), "Did transmissino OK for sending session...");
    fail_if_nonzero(ret);
    ret = print_failed_if_nonzero(check_scb_current_dnode(), "test_send_frag_compl_from_diff_session frag_compl processing");
    fail_if_nonzero(ret); 
    ret = print_failed_if_nonzero(!(current_state == ST_IDLE), "current state has to be ST_IDLE...");
    fail_if_nonzero(ret);
    printf("passed\n");
    return 0;
fail:
    printf("failed\n");
    return 1;
}
static void test_add_crc(uint8_t *buf, uint8_t len)
{
        uint8_t *tmp_buf = buf;
        WORD crc = ZW_CheckCrc16(0x1D0F, tmp_buf, len);
        tmp_buf+=len;
        *tmp_buf++ =  (crc>>8)&0xff;
        *tmp_buf=(crc)&0xff;
}

void regenerate_crc(unsigned char *array, unsigned int len, unsigned char *crc)
{
        crc[0] = array[len-2];
        crc[1] = array[len-1];
        test_add_crc(array, len - 2);
}

void restore_crc(unsigned char *array, unsigned int len, unsigned char *crc)
{
        array[len-2] = crc[0];
        array[len-1] = crc[1];
}

int test_abort_transmission()
{
        int ret = 0;
        unsigned char backup_byte;
        unsigned char crc[2];

        printf("test_abort_transmission\n");
        p.snode = 0xfe;
        p.dnode = 0xff;
        ask_TS_to_send(); /* <-- Start sending session */
        p.snode = 0xff;
        p.dnode = 0xfe;
        MyNodeID = 0xfe;
        backup_byte = test_first_frag1[3];
        test_first_frag1[3] = 0xA0;

        regenerate_crc(test_first_frag1, sizeof(test_first_frag1), crc);
        ask_TS_to_receive(test_first_frag1, sizeof(test_first_frag1)); /* <-- ask to receive*/
        restore_crc(test_first_frag1, sizeof(test_first_frag1), crc);
        test_first_frag1[3] = backup_byte;

        ret = print_failed_if_nonzero(!check_flag_tie_broken(), "Flag tie broken");
        fail_if_nonzero(ret);

        fc_timer_counter = 0;
        ask_TS_to_receive(test_frag_req, sizeof(test_frag_req)); /* <-- Send fragment request with different session id */
        ret = print_failed_if_nonzero(fc_timer_counter, "test_abort_transmission frag_req should be ignored");

        test_subseq_frag2[3] = 0xA0;
        regenerate_crc(test_subseq_frag2, sizeof(test_subseq_frag2), crc);
        ask_TS_to_receive(test_subseq_frag2, sizeof(test_subseq_frag2));/* <-- ask to receive*/
        restore_crc(test_subseq_frag2, sizeof(test_subseq_frag2), crc);
        test_subseq_frag2[3] = backup_byte;

        test_subseq_frag3[3] = 0xA0;
        regenerate_crc(test_subseq_frag3, sizeof(test_subseq_frag3), crc);
        ask_TS_to_receive(test_subseq_frag3, sizeof(test_subseq_frag3));/* <-- ask to receive*/
        restore_crc(test_subseq_frag3, sizeof(test_subseq_frag3), crc);
        test_subseq_frag3[3] = backup_byte;

        backup_byte = test_frag_compl[2];
        test_frag_compl[2] = 0xA0;
        ret = memcmp(output, test_frag_compl, sizeof(test_frag_compl));
        ret = print_failed_if_nonzero(ret, "frag_complete receive check");
        test_frag_compl[2] = backup_byte; 
        fail_if_nonzero(ret);
        ret = print_failed_if_nonzero(!(current_state == ST_IDLE), "current state has to be ST_IDLE...");
        fail_if_nonzero(ret);
        printf("passed\n");
        return 0;
fail:
        printf("failed\n");
        return 1;
}

int test_fc_timer_after_frag_req()
{   
        int ret = 0;
        printf("test_fc_timer_after_frag_req\n");
        ask_TS_to_send(); /* <-- ask to send first frag */

        ret = memcmp(output, test_first_frag1, sizeof(test_first_frag1));
        print_failed_if_nonzero(ret, "test_fc_timer_after_frag_req first fragment");

        fire_timer_btwn_2_frags(0);/* <-- ask to send second frag */
        ret = memcmp(output, test_subseq_frag2, sizeof(test_subseq_frag2));
        ret = print_failed_if_nonzero(ret, "test_fc_timer_after_frag_req second fragment");
        fail_if_nonzero(ret);

        fire_timer_btwn_2_frags(0);/* <-- ask to send second frag */
        ret = memcmp(output, test_subseq_frag3, sizeof(test_subseq_frag3));
        ret = print_failed_if_nonzero(ret, "test_fc_timer_after_frag_req second fragment");
        fail_if_nonzero(ret);

        memset(output, 0, sizeof(test_subseq_frag3));

        p.snode = 0xfe;
        p.dnode = 0xff;
        ask_TS_to_receive(test_frag_req, sizeof(test_frag_req));
        fire_fc_timer();
        ret = print_failed_if_nonzero(!(global_status == S2_TRANSMIT_COMPLETE_FAIL), "Did transmissino fail...");
        fail_if_nonzero(ret);
        ret = print_failed_if_nonzero(!(current_state == ST_IDLE), "current state has to be ST_IDLE...");
        fail_if_nonzero(ret);

        printf("passed\n");
        return 0;
fail:
        printf("failed\n");
        return 1;


}

int test_fc_timer_after_frag_compl_of_aborted_transmission()
{
        int ret = 0;
        unsigned char backup_byte;
        unsigned char crc[2];

        printf("test_fc_timer_after_frag_compl_of_aborted_transmission\n");
        p.snode = 0xfe;
        p.dnode = 0xff;
        ask_TS_to_send(); /* <-- ask to send */

        ret = memcmp(output, test_first_frag1, sizeof(test_first_frag1));
        ret = print_failed_if_nonzero(ret, "test_fc_timer_after_frag_compl_of_aborted_transmission first fragment");
        fail_if_nonzero(ret); 

        fire_timer_btwn_2_frags(0); /* ask to send subseq frag */
        ret = memcmp(output, test_subseq_frag2, sizeof(test_subseq_frag2));
        ret = print_failed_if_nonzero(ret, "test_fc_timer_after_frag_compl_of_aborted_transmission second fragment");
        fail_if_nonzero(ret); 

        fire_timer_btwn_2_frags(0); /* ask to send subseq frag */
        ret = memcmp(output, test_subseq_frag3, sizeof(test_subseq_frag3));
        ret = print_failed_if_nonzero(ret, "test_fc_timer_after_frag_compl_of_aborted_transmission second fragment");
        fail_if_nonzero(ret); 

        p.snode = 0xff;  /* setup to do tie break */
        p.dnode = 0xfe;
        MyNodeID = 0xfe;

        backup_byte = test_first_frag1[3];
        test_first_frag1[3] = 0xA0;
        regenerate_crc(test_first_frag1, sizeof(test_first_frag1), crc);
        ask_TS_to_receive(test_first_frag1, sizeof(test_first_frag1)); /* <-- ask to receive with session id:10 */
        restore_crc(test_first_frag1, sizeof(test_first_frag1), crc);
        test_first_frag1[3] = backup_byte;

        ret = print_failed_if_nonzero(!check_flag_tie_broken(), "Flag tie broken");
        fail_if_nonzero(ret);

        fire_fc_timer();
        ret = print_failed_if_nonzero(!(global_status == S2_TRANSMIT_COMPLETE_FAIL), "Did transmissino fail for sending session...");
        fail_if_nonzero(ret);

        test_subseq_frag2[3] = 0xA0;
        regenerate_crc(test_subseq_frag2, sizeof(test_subseq_frag2), crc);
        ask_TS_to_receive(test_subseq_frag2, sizeof(test_subseq_frag2));/* <-- ask to receive subseq frag with session id:10 */
        restore_crc(test_subseq_frag2, sizeof(test_subseq_frag2), crc);
        test_subseq_frag2[3] = backup_byte;

        test_subseq_frag3[3] = 0xA0;
        regenerate_crc(test_subseq_frag3, sizeof(test_subseq_frag3), crc);
        ask_TS_to_receive(test_subseq_frag3, sizeof(test_subseq_frag3));/* <-- ask to receive subseq frag with session id:10 */
        restore_crc(test_subseq_frag3, sizeof(test_subseq_frag3), crc);
        test_subseq_frag3[3] = backup_byte;

        backup_byte = test_frag_compl[2]; /* Check if frag compl received for session id:10 */
        test_frag_compl[2] = 0xA0;
        ret = memcmp(output, test_frag_compl, sizeof(test_frag_compl));
        ret = print_failed_if_nonzero(ret, "test_fc_timer_after_frag_compl_of_aborted_transmission frag_compl received check");
        test_frag_compl[2] = backup_byte;
        fail_if_nonzero(ret);

        ret = print_failed_if_nonzero(!(current_state == ST_IDLE), "current state has to be ST_IDLE...");
        fail_if_nonzero(ret);

        p.snode = 0xff;
        p.dnode = 0xfe;
        printf("passed\n");
        return 0;
fail:
        printf("failed\n");
        return 1;
}

int test_fc_timer_after_last_frag_twice()
{
    int ret = 0;
    printf("test_fc_timer_after_last_frag_twice\n");
    ask_TS_to_send(); /* <-- ask to send first frag */

    ret = memcmp(output, test_first_frag1, sizeof(test_first_frag1));
    print_failed_if_nonzero(ret, "test_fc_timer_after_last_frag_twice first fragment");

    fire_timer_btwn_2_frags(0);/* <-- ask to send second frag */
    ret = memcmp(output, test_subseq_frag2, sizeof(test_subseq_frag2));
    ret = print_failed_if_nonzero(ret, "test_fc_timer_after_last_frag_twice second fragment");
    fail_if_nonzero(ret);

    fire_timer_btwn_2_frags(0);/* <-- ask to send second frag */
    ret = memcmp(output, test_subseq_frag3, sizeof(test_subseq_frag3));
    ret = print_failed_if_nonzero(ret, "test_fc_timer_after_last_frag_twice second fragment");
    fail_if_nonzero(ret);

    memset(output, 0, sizeof(test_subseq_frag3));

    fire_fc_timer(); /*Tell transport service that we did not receive the last fragment */
    ret = memcmp(output, test_subseq_frag3, sizeof(test_subseq_frag3));
    ret = print_failed_if_nonzero(ret, "test_fc_timer_after_last_frag_twice second fragment again");
    fail_if_nonzero(ret);

    ret = print_failed_if_nonzero(!check_flag_fc_timer_expired_once(), "fc_timer_expired_flag_check");
    fail_if_nonzero(ret);

    fire_fc_timer(); /*Tell transport service that we did not receive the last fragment */
    ret = print_failed_if_nonzero(!(global_status == S2_TRANSMIT_COMPLETE_FAIL), "Did transmissino fail for sending session...");
    fail_if_nonzero(ret);

    ret = print_failed_if_nonzero(check_scb_current_dnode(), "current_dnode is set to 0 check");
    fail_if_nonzero(ret); 
    ret = print_failed_if_nonzero(!(current_state == ST_IDLE), "current state has to be ST_IDLE...");
    fail_if_nonzero(ret);
    printf("passed\n");
    return 0;
fail:
    printf("failed\n");
    return 1;
}

int test_frag_wait_for_completed_session()
{
    int ret = 0;
    printf("test_frag_wait_for_completed_session\n");
    ask_TS_to_send();

    ret = memcmp(output, test_first_frag1, sizeof(test_first_frag1));
    ret = print_failed_if_nonzero(ret, "test_frag_wait_for_completed_session first fragment");
    fail_if_nonzero(ret);

    fire_timer_btwn_2_frags(0);
    ret = memcmp(output, test_subseq_frag2, sizeof(test_subseq_frag2));
    ret = print_failed_if_nonzero(ret, "test_frag_wait_for_completed_session second fragment");
    fail_if_nonzero(ret);

    fire_timer_btwn_2_frags(0);
    ret = memcmp(output, test_subseq_frag3, sizeof(test_subseq_frag3));
    ret = print_failed_if_nonzero(ret, "test_frag_wait_for_completed_session second fragment");
    fail_if_nonzero(ret);

    p.snode = 0xfe;
    p.dnode = 0xff;
    ask_TS_to_receive(test_frag_compl, sizeof(test_frag_compl));
    ask_TS_to_receive(test_frag_wait, sizeof(test_frag_wait));
    p.snode = 0xff;
    p.dnode = 0xfe;
    ret = print_failed_if_nonzero(!(global_status == S2_TRANSMIT_COMPLETE_OK), "Did transmissino OK for sending session...");
    fail_if_nonzero(ret);
    ret = print_failed_if_nonzero(check_scb_current_dnode(), "tets_send_whole_data_gram frag_compl processing");
    fail_if_nonzero(ret);
    ret = print_failed_if_nonzero(!(current_state == ST_IDLE), "current state has to be ST_IDLE...");
    fail_if_nonzero(ret);
    printf("passed\n");
    return 0;
fail:
    printf("failed\n");
    return 1;
   
}

int three_node_test()
{
    int ret = 0;
    global_status = 0xff;
    printf("three_node_stress_test\n");
    ask_TS_to_send();

    ret = memcmp(output, test_first_frag1, sizeof(test_first_frag1));
    ret = print_failed_if_nonzero(ret, "first fragment");
    fail_if_nonzero(ret);

    p.snode = 0xf1;
    ask_TS_to_receive(test_first_frag1, sizeof(test_first_frag1));

    fire_timer_btwn_2_frags(0);
    ret = memcmp(output, test_frag_wait_two_pending, sizeof(test_frag_wait_two_pending)); /* Check if we get fragment wait */
    ret = print_failed_if_nonzero(ret, "received fragment_wait check");
    fail_if_nonzero(ret);

    fire_timer_btwn_2_frags(0);
    ret = memcmp(output, test_subseq_frag2, sizeof(test_subseq_frag2));
    ret = print_failed_if_nonzero(ret, "first subseq fragment");
    fail_if_nonzero(ret);

    ask_TS_to_receive(test_first_frag1, sizeof(test_first_frag1));

    fire_timer_btwn_2_frags(0);
    ret = memcmp(output, test_frag_wait, sizeof(test_frag_wait)); /* Check if we get fragment wait */
    ret = print_failed_if_nonzero(ret, "received fragment_wait again check");
    fail_if_nonzero(ret);

    fire_timer_btwn_2_frags(0);
    ret = memcmp(output, test_subseq_frag3, sizeof(test_subseq_frag3));
    ret = print_failed_if_nonzero(ret, "second subseq fragment");
    fail_if_nonzero(ret);

    p.snode = 0xfe;
    p.dnode = 0xff;
    ask_TS_to_receive(test_frag_compl, sizeof(test_frag_compl));
    ret = print_failed_if_nonzero(!(global_status == S2_TRANSMIT_COMPLETE_OK), "Did transmissino OK for sending session...");
    fail_if_nonzero(ret);
    p.snode = 0xf1;

    ask_TS_to_receive(test_first_frag1, sizeof(test_first_frag1)); /* <-- ask to receive*/
    ask_TS_to_receive(test_subseq_frag2, sizeof(test_subseq_frag2));/* <-- ask to receive*/
    ask_TS_to_receive(test_subseq_frag3, sizeof(test_subseq_frag3));/* <-- ask to receive*/
//    fire_rx_timer();
    ret = memcmp(output, test_frag_compl, sizeof(test_frag_compl));
    ret = print_failed_if_nonzero(ret, "fragment complete");
    fail_if_nonzero(ret);

    return 0;
fail:
    return 1;
}

int send_big_datagram()
{
    int ret = 0;
    unsigned char backup[2];
    unsigned char crc[2];

    printf("send_big_datagram\n");
    ask_TS_to_receive(test_first_frag1, sizeof(test_first_frag1));
    ask_TS_to_receive(test_subseq_frag2, sizeof(test_subseq_frag2));
    backup[0] = test_subseq_frag3[3];
    backup[1] = test_subseq_frag3[4];

    test_subseq_frag3[3] = ((DATAGRAM_SIZE_MAX+1) & 0x700) >> 8;
    test_subseq_frag3[4] = ((DATAGRAM_SIZE_MAX+1) & 0xff);

    regenerate_crc(test_subseq_frag3, sizeof(test_subseq_frag3), crc);
    ask_TS_to_receive(test_subseq_frag3, sizeof(test_subseq_frag3));
    restore_crc(test_subseq_frag3, sizeof(test_subseq_frag3), crc);
    test_subseq_frag3[3] = backup[0];
    test_subseq_frag3[4] = backup[1];
    fire_rx_timer();

    ret = memcmp(output, test_frag_req2, sizeof(test_frag_req2));
    ret = print_failed_if_nonzero(ret, "send_big_datagram frag_req received");
    fail_if_nonzero(ret);

    ask_TS_to_receive(test_subseq_frag3, sizeof(test_subseq_frag3));
    ret = memcmp(output, test_frag_compl, sizeof(test_frag_compl));
    ret = print_failed_if_nonzero(ret, "send_big_datagram frag_compl");
    fail_if_nonzero(ret);
    ret = print_failed_if_nonzero(!(current_state == ST_IDLE), "current state has to be ST_IDLE...");
    fail_if_nonzero(ret);
    printf("passed\n");
    return 0;
fail:
    printf("failed\n");
    return 1;

}

int send_big_datagram2()
{
    int ret = 0;
    unsigned char backup[2];
    unsigned char crc[2];

    printf("send_big_datagram2\n");
    backup[0] = test_first_frag1[1];
    backup[1] = test_first_frag1[2];
    test_first_frag1[1] = 0xC0 + (((DATAGRAM_SIZE_MAX+1) & 0x700) >> 8);
    test_first_frag1[2] = ((DATAGRAM_SIZE_MAX+1) & 0xff);
    regenerate_crc(test_first_frag1, sizeof(test_first_frag1), crc);
    ask_TS_to_receive(test_first_frag1, sizeof(test_first_frag1));
    restore_crc(test_first_frag1, sizeof(test_first_frag1), crc);
    test_first_frag1[1] = backup[0];
    test_first_frag1[2] = backup[1];

    backup[0] = test_subseq_frag2[1];
    backup[1] = test_subseq_frag2[2];
    test_subseq_frag2[1] = 0xE0 + (((DATAGRAM_SIZE_MAX+1) & 0x700) >> 8);
    test_subseq_frag2[2] = ((DATAGRAM_SIZE_MAX+1) & 0xff);
    regenerate_crc(test_subseq_frag2, sizeof(test_subseq_frag2), crc);
    ask_TS_to_receive(test_subseq_frag2, sizeof(test_subseq_frag2));
    restore_crc(test_subseq_frag2, sizeof(test_subseq_frag2), crc);
    test_subseq_frag2[1] = backup[0];
    test_subseq_frag2[2] = backup[1];

#if 0
    backup[0] = test_subseq_frag3[3];
    backup[1] = test_subseq_frag3[4];

    test_subseq_frag3[3] = ((DATAGRAM_SIZE_MAX+1) & 0x700) >> 8;
    test_subseq_frag3[4] = ((DATAGRAM_SIZE_MAX+1) & 0xff);

    regenerate_crc(test_subseq_frag3, sizeof(test_subseq_frag3), crc);
    ask_TS_to_receive(test_subseq_frag3, sizeof(test_subseq_frag3));
    restore_crc(test_subseq_frag3, sizeof(test_subseq_frag3), crc);
    test_subseq_frag3[3] = backup[0];
    test_subseq_frag3[4] = backup[1];
    fire_rx_timer();

    ret = memcmp(output, test_frag_req2, sizeof(test_frag_req2));
    ret = print_failed_if_nonzero(ret, "send_big_datagram frag_req received");
    fail_if_nonzero(ret);

    ask_TS_to_receive(test_subseq_frag3, sizeof(test_subseq_frag3));
    ret = memcmp(output, test_frag_wait_zero_pending, sizeof(test_frag_wait_zero_pending)); /* Check if we get fragment wait */
    ret = print_failed_if_nonzero(ret, "test_frag_wait_zero_pending");
    fail_if_nonzero(ret);
#endif
    ret = print_failed_if_nonzero(!(current_state == ST_IDLE), "current state has to be ST_IDLE...");
    fail_if_nonzero(ret);
    printf("passed\n");
    return 0;
fail:
    printf("failed\n");
    return 1;

}
int check_output_generated(void)
/* Verify no output has been generated.
 * Assumes that output buffer has been zeroed before with command
 * Return FALSE if no output generated since output buffer was cleared.
 * Return TRUE  if output was generated.
 *
 * Usage:
 *   memset(output, 0, sizeof(output));
 *   ... Do something that should not generate output...
 *   fail_if_nonzero(!check_no_output());
 *
 *   */
{
  int i;
  for (i=0; i<sizeof(output); i++)
  {
    if(output[i])
    {
      return TRUE;
    }
  }
  return FALSE;
}

int send_first_frag_with_big_size()
/* Steps:
 *  1. Send a first fragment with too long total frame size
 *  2. Check that we ignore the segment and remain in state idle
 *  3. Fire rx_timer
 *  4. Check that no segment request is sent
 */
{
    int ret = 0;
    unsigned char backup[2];
    unsigned char crc[2];

    printf("send_first_frag_with_big_size\n");
    backup[0] = test_first_frag1[1];
    backup[1] = test_first_frag1[2];
    test_first_frag1[1] = COMMAND_FIRST_FRAGMENT | ((DATAGRAM_SIZE_MAX+1) & 0x700) >> 8;
    test_first_frag1[2] = ((DATAGRAM_SIZE_MAX+1) & 0xff);
    regenerate_crc(test_first_frag1, sizeof(test_first_frag1), crc);

    ask_TS_to_receive(test_first_frag1, sizeof(test_first_frag1));

    test_first_frag1[1] = backup[0];
    test_first_frag1[2] = backup[1];
    regenerate_crc(test_first_frag1, sizeof(test_first_frag1), crc);

    ret = print_failed_if_nonzero(!(current_state == ST_IDLE), "current state has to be ST_IDLE...");
    fail_if_nonzero(ret);

    memset(output, 0, sizeof(output));
    fire_rx_timer();

    ret = check_output_generated();
    ret = print_failed_if_nonzero(ret, "sending segment request after too long first segment");
    fail_if_nonzero(ret);

    memset(output, 0, sizeof(output));
    ask_TS_to_receive(test_subseq_frag2, sizeof(test_subseq_frag2));
    ret = memcmp(output, test_frag_wait_zero_pending, sizeof(test_frag_wait_zero_pending)); /* Check if we get fragment wait */
    ret = print_failed_if_nonzero(ret, "correct subsequent without first triggers data");
    fail_if_nonzero(ret);

    ret = print_failed_if_nonzero(!(current_state == ST_IDLE), "current state has to be ST_IDLE...");
    fail_if_nonzero(ret);

    printf("passed\n");
    return 0;
fail:
    printf("failed\n");
    return 1;


}

int call_ask_TS_to_receive_with_large_size()
{
    int ret = 0;
    
    printf("call_ask_TS_to_receive_with_large_size\n");
    ask_TS_to_receive(test_first_frag1, DATAGRAM_SIZE_MAX+1);
    ret = print_failed_if_nonzero(!call_with_large_value, "call_ask_TS_to_receive_with_large_size");
    fail_if_nonzero(ret);
    ret = print_failed_if_nonzero(!(current_state == ST_IDLE), "current state has to be ST_IDLE...");
    fail_if_nonzero(ret);
    printf("passed\n");
    return 0;
fail:
    printf("failed\n");
    return 1;
}

int main()
{
    memset(&p, 0, sizeof(ts_param_t));
    p.snode = 0xff;
    p.dnode = 0xfe;
    fail_if_nonzero(simple_test());
    fail_if_nonzero(dont_send_one_frag());
    fail_if_nonzero(test_dont_send_first_frag());

    fail_if_nonzero(test_frag_wait_fn());
    fail_if_nonzero(test_send_whole_datagram());
    fail_if_nonzero(test_jakob());

    fail_if_nonzero(test_tie_break());
    fail_if_nonzero(test_two_last_fragments());
    fail_if_nonzero(test_send_frag_compl_from_diff_session());
    fail_if_nonzero(test_abort_transmission());
    fail_if_nonzero(test_fc_timer_after_frag_req());
    fail_if_nonzero(test_fc_timer_after_frag_compl_of_aborted_transmission());
    fail_if_nonzero(test_fc_timer_after_last_frag_twice());
    fail_if_nonzero(test_frag_wait_for_completed_session());

    fail_if_nonzero(three_node_test());
    fail_if_nonzero(send_big_datagram());
    fail_if_nonzero(send_first_frag_with_big_size());
    fail_if_nonzero(call_ask_TS_to_receive_with_large_size());
    fail_if_nonzero(send_big_datagram2());

    printf("All test passed\n");
    return 0;
fail:
    printf("Above test failed\n");
    return 1;
}
