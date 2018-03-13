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

/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/


#define NO_MEM_FUNCTIONS
#include<string.h>


#include "ZIP_Router.h"
#include "Serialapi.h"
#include "ZW_PRNG.h"

void GetRNGData(BYTE *pRNDData, BYTE noRNDDataBytes)REENTRANT;
static void PRNGUpdate(void)REENTRANT;

/**
 * Pseudo-random number generator seed - is only reset on reset, */
/* on wakeup by either WUT or BEAM the previous seed is maintained
 * via usage of the NON_ZERO_START block */
static BYTE prngState[16];  /* PRNG state */

/* Use SerialAPI */
#define ZW_AES_ECB(key, inputDat, outputDat) SerialAPI_AES128_Encrypt(inputDat, outputDat, key);

/**
 * XOR 16 bytes
 */
static void xor16(BYTE* c, BYTE* a, BYTE* b)
{
  int i;
  for (i = 0; i < 16; i++)
  {
    c[i] = (BYTE) (a[i] ^ b[i]);
  }
}

/*================================   AESRaw   ===============================
 **    AES Raw
 **
 **    Side effects :
 **
 **--------------------------------------------------------------------------*/
/*
 Declaration: void AESRaw(BYTE *pKey, BYTE *pSrc, BYTE *pDest)
 Called: When individual 128-bit blocks of data have to be encrypted
 Arguments: pKey Pointer to key (input; fixed size 16 bytes)
 pSrc Pointer to source data (input; fixed size 16 bytes)
 pDest Pointer to destination buffer (output; fixed size
 16 bytes)
 Return value: None
 Global vars: None affected
 Task: Encrypts 16 bytes of data at pSrc, using Raw AES and the key at pKey. The
 16-byte result is written to pDest.*/
void AESRaw(BYTE *pKey, BYTE *pSrc, BYTE *pDest)
{
  memcpy(pDest, pSrc, 16);
  ZW_AES_ECB(pKey, pSrc, pDest);
}


void InitPRNG(void)
{
  /* Reset PRNG State */
  memset(prngState, 0, 16);
  /* Update PRNG State */
  PRNGUpdate();
}

/*===============================   GetRNGData   =============================
 **    GetRNGData
 **
 **    Side effects :
 **
 **--------------------------------------------------------------------------*/
void GetRNGData(BYTE *pRNDData, BYTE noRNDDataBytes)
{
  BYTE i,j;
  BYTE rnd[8];
  ZW_SetRFReceiveMode(FALSE);

  j=0;
  for(i=0; i <noRNDDataBytes; i++) {
    if(j==0) ZW_GetRandomWord(rnd, FALSE);
    pRNDData[i] = rnd[j];
    j = (j+1) & 0x7;
  }
}

/**
 Incorporate new data from hardware RNG into the PRNG State
 Called, When fresh input from hardware RNG is needed
 */
static void PRNGUpdate(void) REENTRANT
{

  BYTE k[16], h[16], ltemp[16], btemp[16],j;

  /* H = 0xA5 (repeated x16) */
  memset(h, 0xA5, sizeof(h));
  /* The two iterations of the hardware generator */
  for (j = 0; j <2; j++)
  {
    /* Random data to K */
    GetRNGData(k, 16);
    /* ltemp = AES(K, H) */
    AESRaw(k, h, ltemp);
    /* H = AES(K, H) ^ H */
    xor16(h,ltemp,h);
  }
  /* Update inner state */
  /* S = S ^ H */
  xor16(prngState,prngState,h);

  /* ltemp = 0x36 (repeated x16) */
  memset(ltemp, 0x36, 16);
  /* S = AES(S, ltemp) */
  AESRaw(prngState, ltemp, btemp);
  memcpy(prngState, btemp, 16);
}

void PRNGOutput(BYTE *pDest) REENTRANT
{
  BYTE ltemp[16], btemp[16];

  /* Generate output */
  /* ltemp = 0x5C (repeated x16) */
  memset((BYTE *) ltemp, 0x5C/*0xA5*/, 16);
  /* ltemp = AES(PRNGState, ltemp) */
  AESRaw(prngState, ltemp, btemp);
  /* pDest[0..7] = ltemp[0..7] */
  memcpy(pDest, btemp, 8);
  /* Generate next internal state */
  /* ltemp = 0x36 (repeated x16) */
  memset((BYTE *) ltemp, 0x36, 16);
  /* PRNGState = AES(PRNGState, ltemp) */
  AESRaw(prngState, ltemp, btemp);
  memcpy(prngState, btemp, 16);
}
