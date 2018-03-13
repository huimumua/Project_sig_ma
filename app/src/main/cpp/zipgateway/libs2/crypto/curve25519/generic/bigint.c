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
 * File:    avrnacl_8bitc/shared/bigint.c
 * Author:  Michael Hutter, Peter Schwabe
 * Version: Tue Aug 12 08:23:16 2014 +0200
 * Public Domain
 */


#include <stdint.h>
#include "bigint.h"
//#include "wc_util.h"

uint8_t bigint_add(uint8_t *r, const uint8_t *a, const uint8_t *b, uint16_t len)
{
  uint16_t i;
  uint16_t tmp = 0;
  for (i=0; i<len; i++) 
  {
    tmp = ((uint16_t)a[i]) + ((uint16_t)b[i]) + tmp;
    r[i] = tmp & 0xff;
    tmp >>= 8;
  }
  return (uint8_t)tmp;
}

uint8_t bigint_sub(uint8_t *r, const uint8_t *a, const uint8_t *b, uint16_t len)
{
  uint16_t i;
  uint16_t tmp = 0;
  for (i=0; i<len; i++) 
  {
    tmp = ((uint16_t)a[i]) - ((uint16_t)b[i]) - tmp;
    r[i] = tmp & 0xff;
    tmp >>= 15;
  }
  return (uint8_t)tmp;
}

void bigint_mul(unsigned char *r, const unsigned char *a, const unsigned char *b, uint8_t len)
{
  //unsigned int i,j;
  uint8_t i;
  uint8_t j;
  //uint16_t t;
  uint16_t t;
  for(i=0;i<2*len;i++)
    r[i] = 0;

  for (i=0; i<len; i++) {
    t = 0;
    for (j=0; j<len; j++) {
      t=r[i+j]+a[i]*b[j] + (t>>8);
      r[i+j]=(t & 0xFF);
    }
    r[i+len]=(t>>8);
  }
}

static void bigint_mul16c(unsigned char r[34], const unsigned char a[17], const unsigned char b[17]) 
{
  unsigned char t0[9], t1[9],t[18], i;
  int u;
  bigint_mul(r, a, b, 8);
  bigint_mul(r+16, a+8, b+8, 9);

  t0[8] = a[16] + bigint_add(t0,a,a+8,8);
  t1[8] = b[16] + bigint_add(t1,b,b+8,8);
  bigint_mul(t,t0,t1,9);
  t[17] -= bigint_sub(t,t,r+16,17);
  u = t[16];
  //t[16] -= bigint_sub(t,t,r,16);
  u -= bigint_sub(t,t,r,16);
  t[16] = u & 0xff;
  u>>=15;
  t[17] -= u;
  u = bigint_add(r+8,r+8,t,17);
  for(i=25;i<34;i++)
  {
    u += r[i];
    r[i] = u & 0xff;
    u >>= 8;
  }
}

static void bigint_mul16(unsigned char r[32], const unsigned char a[16], const unsigned char b[16]) 
{
  unsigned char t0[9], t1[9],t[18], i;
  //int u;
  int16_t u;
  bigint_mul(r, a, b, 8);
//  PRINT_TXT("first mul");
//print16(r); // First mul equals
  bigint_mul(r+16, a+8, b+8, 8);
//  PRINT_TXT("second mul");
//print16(r+16); // First mul equals
  t0[8] = bigint_add(t0,a,a+8,8);
  t1[8] = bigint_add(t1,b,b+8,8);
  bigint_mul(t,t0,t1,9);
  t[16] -= bigint_sub(t,t,r,16);
  t[16] -= bigint_sub(t,t,r+16,16);
  u = bigint_add(r+8,r+8,t,17);
  for(i=25;i<32;i++)
  {
    u += (int16_t)r[i];
    r[i] = u & 0xff;
    u >>= 8;
  }
}

void bigint_mul32(uint8_t *r, const uint8_t *a, const uint8_t *b)
{
  uint8_t t0[17], t1[17],t[34], i;
  int16_t u;
  bigint_mul16(r, a, b);
//  PRINT_TXT("first mul16");
//print32(r);
  bigint_mul16(r+32, a+16, b+16);
//print64(r);
  t0[16] = bigint_add(t0,a,a+16,16);
  t1[16] = bigint_add(t1,b,b+16,16);
  bigint_mul16c(t,t0,t1);
  t[32] -= bigint_sub(t,t,r,32);
  t[32] -= bigint_sub(t,t,r+32,32);
  u = bigint_add(r+16,r+16,t,33);
  for(i=49;i<64;i++)
  {
    u += r[i];
    r[i] = u & 0xff;
    u >>= 8;
  }
}


void bigint_cmov(uint8_t *r, const uint8_t *x, uint8_t b, uint16_t len)
{
  uint16_t i;
  uint8_t mask = b;
  mask = -mask;
  for(i=0;i<len;i++) 
    r[i] ^= mask & (x[i] ^ r[i]);
}

