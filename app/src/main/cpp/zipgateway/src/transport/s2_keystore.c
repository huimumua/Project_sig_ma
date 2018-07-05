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
 * s2_keystore.c

 *
 *  Created on: Nov 2, 2015
 *      Author: aes
 */
#include "ZIP_Router.h"
#include "s2_keystore.h"
#include "Security_Scheme0.h"

#define DllExport
#include "curve25519.h"
#include "ctr_drbg.h"
#include "CC_NetworkManagement.h"

extern uint8_t ecdh_dynamic_key[32];

void
print_hex(uint8_t* buf, int len)
{
  int i;
  char hex[len], str[len*3];
  memset(hex, 0, sizeof(hex));
  memset(str, 0, sizeof(str));

  for (i = 0; i < len; i++)
  {
    sprintf(hex, "%02X ", buf[i]);
    strcat(str, hex);
  }
  DBG_PRINTF("%s", str);
}


bool keystore_network_key_clear(uint8_t keyclass) {
  uint8_t random_bytes[64];
  uint8_t assigned_keys;
  uint8_t i;

  AES_CTR_DRBG_Generate(&s2_ctr_drbg, random_bytes);

  if(keyclass==0xFF) {
    keystore_network_key_clear(KEY_CLASS_S0);
    keystore_network_key_clear(KEY_CLASS_S2_UNAUTHENTICATED);
    keystore_network_key_clear(KEY_CLASS_S2_AUTHENTICATED);
    keystore_network_key_clear(KEY_CLASS_S2_ACCESS);
    return 1;
  }

  if( keystore_network_key_write(keyclass,random_bytes) ) {
    nvm_config_get(assigned_keys,&assigned_keys);
    assigned_keys &= ~keyclass;
    nvm_config_set(assigned_keys,&assigned_keys);
    memset(random_bytes,0,sizeof(random_bytes));
    return 1;
  }
  return 0;
}

void keystore_private_key_read(uint8_t *buf) {
  int i;

  if((NetworkManagement_getState() == NM_WAIT_FOR_SECURE_LEARN)
      || (NetworkManagement_getState() == NM_LEARN_MODE_STARTED)
      || (NetworkManagement_getState() == NM_LEARN_MODE)) {
    nvm_config_get(ecdh_priv_key,buf);
    printf(" static \n");
  } else {
    memcpy(buf,ecdh_dynamic_key,32);
    printf(" dynamic \n");
  }


#if 0
  printf("ECDH Private key is \n");
  print_hex(buf,32);
#endif
}


void
keystore_public_key_read(uint8_t *buf)
{
  int i;
  uint8_t priv_key[32];

  keystore_private_key_read(priv_key);
  crypto_scalarmult_curve25519_base(buf,priv_key);

  printf("ECDH Public key is \n");
  for(i=0; i < 16; i++) {
    uint16_t d = (buf[2*i]<<8) | buf[2*i +1];
    printf("%05hu-", d);
    if( (i&3)==3 )printf("\n");
  }
  memset(priv_key,0,sizeof(priv_key));
}


bool keystore_network_key_read(uint8_t keyclass, uint8_t *buf)
{
  uint8_t assigned_keys;

  nvm_config_get(assigned_keys,&assigned_keys);
  if(0==(keyclass & assigned_keys)) {
    return 0;
  }

  switch(keyclass)
  {
  case KEY_CLASS_S0:
    nvm_config_get(security_netkey,buf);
  break;
  case KEY_CLASS_S2_UNAUTHENTICATED:
    nvm_config_get(security2_key[0],buf);
    break;
  case KEY_CLASS_S2_AUTHENTICATED:
    nvm_config_get(security2_key[1],buf);
    break;
  case KEY_CLASS_S2_ACCESS:
    nvm_config_get(security2_key[2],buf);
    break;
  default:
    assert(0);
    return 0;
  }

  DBG_PRINTF("Key  class  %x \n",keyclass);
  print_hex(buf,16);

  return 1;
}


bool keystore_network_key_write(uint8_t keyclass, const uint8_t *buf)
{
  uint8_t assigned_keys;

  switch(keyclass)
  {
  case KEY_CLASS_S0:
    nvm_config_set(security_netkey,buf);
    sec0_set_key(buf);
  break;
  case KEY_CLASS_S2_UNAUTHENTICATED:
    nvm_config_set(security2_key[0],buf);
    break;
  case KEY_CLASS_S2_AUTHENTICATED:
    nvm_config_set(security2_key[1],buf);
    break;
  case KEY_CLASS_S2_ACCESS:
    nvm_config_set(security2_key[2],buf);
    break;
  default:
    assert(0);
    return 0;
  }

  nvm_config_get(assigned_keys,&assigned_keys);
  assigned_keys |= keyclass;
  nvm_config_set(assigned_keys,&assigned_keys);
  return 1;
}
