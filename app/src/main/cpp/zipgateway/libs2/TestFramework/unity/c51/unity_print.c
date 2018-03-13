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

#include "../unity_print.h"
#include <stdio.h>
#include "reg51.h"

sfr SFRPAGE   = 0xFF;
sfr UART0BRL  = 0xAF;
sfr UARTCON   = 0xEC;
sfr UARTSTAT  = 0xED;
sfr UART0BUF  = 0xEF;

sfr WDTCON    = 0xE4;

#define WDTCON_PAGE SFRPAGE=0x02
#define WATCHDOG_DISABLE {WDTCON_PAGE; WDTCON = WDTCON_NOTIFY_BIT; WDTCON = 0;}
#define WDTCON_NOTIFY_BIT              0x04

#define UARTCON_PAGE
#define UARTSTAT_PAGE
#define UART0BUF_PAGE /* ANY */
#define UART0BRL_PAGE      SFRPAGE=0x02
#define UART0BRL_BRL0_BITS 0xFF

#define UARTCON_CTX1_BIT               0x80
#define UARTCON_REN1_BIT               0x40
#define UARTCON_CTX0_BIT               0x08
#define UARTCON_REN0_BIT               0x04
#define UARTCON_BRH0_BITS              0x03

#define UARTSTAT_TA0_BIT               0x01

sfr RSETVEC   = 0xE6;
#define RSETVEC_PORTKEEP_BIT           0x40
#define RSETVEC_PAGE SFRPAGE=0x02

#define SFR_AND(r,v) {r##_PAGE;r=(r&v);}
#define SFR_SET(r,v) {r##_PAGE;r=v;}
#define SFR_OR(r,v) {r##_PAGE;r=(r|v);}

#define OPEN_IOS {SFR_AND(RSETVEC,~RSETVEC_PORTKEEP_BIT);}

#define UART0_SET_BAUD_RATE(BAUD_TIMER) {SFR_AND(UARTCON,  ~UARTCON_BRH0_BITS);\
                                         SFR_OR(UARTCON,((BAUD_TIMER & 0x0300) >> 8));\
                                         SFR_SET(UART0BRL, (BAUD_TIMER & (0x00FF)));\
                                        }
#define UART0_TX_WRITE_BUFFER(TX_DATA) {SFR_SET(UART0BUF,  TX_DATA);}
#define UART0_TX_ENABLE      {SFR_OR(UARTCON, UARTCON_CTX0_BIT);}
#define UART0_TX_ACTIVE_GET  (UARTSTAT & UARTSTAT_TA0_BIT)


void unity_print_init(void)
{
#if 1
  SCON  = 0x50;                   /* SCON: mode 1, 8-bit UART, enable rcvr    */
  TMOD |= 0x20;                   /* TMOD: timer 1, mode 2, 8-bit reload      */
  TH1   = 0xf3;                   /* TH1:  reload value for 2400 baud         */
  TR1   = 1;                      /* TR1:  timer 1 run                        */
  TI    = 1;                      /* TI:   set TI to send first char of UART  */
#else
  int bBaudRate;
  WATCHDOG_DISABLE;
  bBaudRate = 1152;
  bBaudRate = (80000/bBaudRate ) + (((80000 %bBaudRate ) >= (bBaudRate >> 1))  ? 1:0);
  UART0_SET_BAUD_RATE(68);
  UART0_TX_ENABLE;
  
  OPEN_IOS
  UART0BUF = '*';
#endif

}

#if 0
char
putchar(
  char c)  /* IN a byte to write to the UART transmit register.*/
{
  while(UART0_TX_ACTIVE_GET);
  UART0_TX_WRITE_BUFFER(c);
  return c;
}
#endif

void unity_print_close(void)
{
  // Do nothing.
}

int ZW_putchar(int c)
{
  while(UART0_TX_ACTIVE_GET);
  UART0_TX_WRITE_BUFFER(c);
  return c;
}

