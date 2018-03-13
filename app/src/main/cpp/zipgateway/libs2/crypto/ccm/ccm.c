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

/* author: mdumbare */
/* Refer http://csrc.nist.gov/publications/nistpubs/800-38C/SP800-38C_updated-July20_2007.pdf */
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <aes.h>
#include "ccm.h"

#ifndef __C51__
//#define VERBOSE_DEBUG
#endif

#ifdef VERBOSE_DEBUG
#define print_block(block, size)  debug_print_block(block, size)
#define ccm_print_str(str)        debug_ccm_print_str(str)
#define debug_ccm_print_str(str)  printf(str)
#else
#define print_block(block, size)
#define ccm_print_str(str)
#define debug_ccm_print_str(str)
#endif

/* ---------------------------- Encrypt part ---------------------------------*/
#define B0_AAD 0x40
#define BLOCK_SIZE 16
#define ENCRYPT 1
#define DECRYPT 0

#ifndef CCM_USE_PREDEFINED_VALUES
static uint8_t q = 0;
static uint8_t n = 0;
static uint8_t t = 0;
#else
//#define KYO 2
/* max payoad length will be 2 pow(8q) which is 2 pow(16)
 * q is the octet length of the binary representation of the octet length of the payload.
 * So if q is 3 and payload length is 4096, then Q is 00000000 00000010 00000000 */
//#define Q_DEF KYO
static const uint8_t q = Q_DEF;
/* n is the octet length of nonce. And (n = 15 - q) */
static const uint8_t n = N_DEF;
static const uint8_t t = T_DEF; /* Length of auth_tag in bytes*/
#endif

/* Converts @number to octet string @oct_str of octet length oct_len
 * e.g. if text_to_encrypt_len is 512 and q is 3 then Q is 00000000 00000010 00000000
 * P.S. octet strings are stored in array of unsinged 8 bit integer
 * See section A.1 and A.2.1 in the document mentioned in header */
static void convert_to_octet_string(uint16_t text_to_encrypt_len, uint8_t *Q, uint8_t q)
{
    //int i;
  uint8_t i;
  uint16_t bitmask = 0xff00;

    /* As the text_to_encrypt_len wont be more than 0xffff. There can be max 2 octet strings (Q)
     * so initialize the Q octet string to all zeroes except last two bytes. which is going to be p(text_to_encrypt_len)*/
    for (i = 0; i < (q - 2); i++)
        Q[i] = 0;

    /* Fill second last byte with 8 MSBs of text_to_encrypt_len.
        for e.g. text_to_encrypt_len is 512 (00000010 00000000) then Q[i] is 00000010 */
    Q[i] = (text_to_encrypt_len & bitmask) >> 8;
    bitmask = bitmask >> 8;
    /* Fill last byte with remaining 8 LSBs of text_to_encrypt_len.
        for e.g. text_to_encrypt_len is 512 (00000010 00000000) then Q[i+1] is 00000000 */
    Q[i+1] = (text_to_encrypt_len & bitmask);
}

#ifdef VERBOSE_DEBUG
static void debug_ccm_print(const char *format, ...)
{
    va_list args;
    va_start( args, format );
    vprintf(format, args);
    va_end( args );
}

/*
static void debug_ccm_print_str(const char * str)
{
  printf(str);
}
*/
#endif

/* See section A.2.1 in the document mentioned in header */
static int format_b0(const uint8_t t, const uint16_t text_to_encrypt_len,
                      uint8_t *first_block, const uint8_t *nonce)
{
  uint8_t i;

  // The value 8 is the maximum value of q according to the NIST document.
  uint8_t Q[8];


    first_block[0] = 0;

    /* do not set resreved bit */

    first_block[0] = first_block[0] | B0_AAD; /* set Adata bit. See A.2.1 */
    first_block[0] = first_block[0] | ((((t - 2) / 2) & 0x7) << 3);
    first_block[0] = first_block[0] | ((q - 1) & 0x7);

    /* Copy the nonce from 1st byte (not 0th). See Table 2 in A.2.1 */
    for (i = 1; i < n + 1 ; i++) {
        first_block[i] = nonce[i-1];
    }

    convert_to_octet_string(text_to_encrypt_len, Q, q);

    /* Copy the Q after nonce. See Table 2 in A.2.1 */
    memcpy(&first_block[i], Q, q);
    return 1;

}

static void ciph_block(uint8_t *blocks, const uint8_t *key)
{
    AES128_ECB_encrypt(blocks, key, blocks);
}

#ifdef VERBOSE_DEBUG
static void debug_print_block(uint8_t *block, int size)
{
    int i;

    for (i = 0; i < size; i++)
        debug_ccm_print("%x ", block[i]);

    debug_ccm_print_str("\n");
}
#endif

static void bit_xor(const uint8_t *block, uint8_t *next_block, int size)
{
    int i;
    for (i = 0; i < size; i++)
        next_block[i] = block[i] ^ next_block[i];
}


/*See section A.2.2 in NIST pdf */
static int format_aad(uint8_t blocks[2][BLOCK_SIZE], const uint8_t *aad, const uint32_t aad_len, const uint8_t *key)
{
    int i;
    int offset;
    int new_aad_len = 0;
    int no_of_blocks_for_aad = 0;
    int aad_bytes_in_first_block;
    int aad_start = 2;

    /* See more about this lenghth ranges in beginning of page 14 in the document mentioned in header */
    if (aad_len < 65280) { 
        blocks[1][0] = 0x0;
        blocks[1][1] = aad_len;
        aad_bytes_in_first_block = 14;
    }else if ((aad_len >= 65280) && (aad_len <= 0xffffffff)) { /*TODO Check the limits */
        blocks[1][0] = 0xff;
        blocks[1][1] = 0xfe;
        blocks[1][2] = 0;
        blocks[1][3] = 1;
        blocks[1][4] = 0;
        blocks[1][5] = 0;
        aad_bytes_in_first_block = 10;
        aad_start = 6;
    } else {
      ccm_print_str("AAD length more than 2pow(16) - 2pow(8) is not supported ");
      return 0;
    }

    memcpy(&blocks[1][aad_start], &aad[0], aad_bytes_in_first_block);/* Fill the first block's 14 octets with first 14 octets of aad*/
    /* Copy first 14 octets of AAD to blocks[1] */
    if (aad_len < aad_bytes_in_first_block) { /* need to pad with zeroes if aad_len is less than 14*/
        memset(&blocks[1][aad_start] + aad_len, 0, aad_bytes_in_first_block - aad_len);
        bit_xor(blocks[1], blocks[0], BLOCK_SIZE);
        ciph_block(blocks[0], key);
        print_block(blocks[1], BLOCK_SIZE);
    } else if (aad_len > aad_bytes_in_first_block) { /* If aad_len is more than 14 we need more blocks */
        bit_xor(blocks[1], blocks[0], BLOCK_SIZE);
        ciph_block(blocks[0], key);
        new_aad_len = aad_len - aad_bytes_in_first_block ;
        no_of_blocks_for_aad = new_aad_len / BLOCK_SIZE; /* Find the number of more blocks needed */
        if (new_aad_len % BLOCK_SIZE)
            no_of_blocks_for_aad++;

        offset = aad_bytes_in_first_block; /* Start copying the octets from aad_bytes_in_first_block'th offset*/
        for (i = 0; i < no_of_blocks_for_aad; i++) {

                    /* if its the last block and not aligned to BLOCK_SIZE bytes */
            if ((i == (no_of_blocks_for_aad - 1)) && (new_aad_len % BLOCK_SIZE)) {
                memcpy(&blocks[1][0], &aad[offset], (new_aad_len % BLOCK_SIZE));
                /* pad with zeroes */
                memset(&blocks[1][0] + (new_aad_len % BLOCK_SIZE), 0, BLOCK_SIZE - (new_aad_len % BLOCK_SIZE));
                print_block(blocks[1], BLOCK_SIZE);
                bit_xor(blocks[1], blocks[0], BLOCK_SIZE);
                ciph_block(blocks[0], key);
                break;
            }

            memcpy(&blocks[1][0], &aad[offset], BLOCK_SIZE);
            //print_block(blocks[1], BLOCK_SIZE);
            bit_xor(blocks[1], blocks[0], BLOCK_SIZE);
            ciph_block(blocks[0], key);
            offset += BLOCK_SIZE;
        //    print_block(blocks[i+2], BLOCK_SIZE);
        }
    } else {
        bit_xor(blocks[1], blocks[0], BLOCK_SIZE);
        ciph_block(blocks[0], key);
        print_block(blocks[1], BLOCK_SIZE);
    }
    return 1;
}

static void format_payload_block(uint8_t blocks[2][BLOCK_SIZE], const uint8_t *P,
                                 const uint16_t text_to_encrypt_len,
                                 const uint8_t *key)
{
    int i;
    int no_blocks_payload = (text_to_encrypt_len / BLOCK_SIZE);
    //int bytes_to_pad;
    int offset = 0;

//#ifdef __C51__
//    UNUSED(mac_len);
//#endif

    if (text_to_encrypt_len % BLOCK_SIZE)
        no_blocks_payload++;

    for (i = 0; i < no_blocks_payload; i++) {
        if ((i == (no_blocks_payload - 1)) && (text_to_encrypt_len % BLOCK_SIZE)) { /* If its a last block and not aligned to BLOCK_SIZE */
            memcpy(&blocks[1][0], &P[offset], (text_to_encrypt_len % BLOCK_SIZE));
            /* pad with zeroes */
            memset(&blocks[1][0] + (text_to_encrypt_len % BLOCK_SIZE), 0, BLOCK_SIZE - (text_to_encrypt_len % BLOCK_SIZE));
            print_block(blocks[1], BLOCK_SIZE);
            bit_xor(blocks[1], blocks[0], BLOCK_SIZE);
            ciph_block(blocks[0], key);
            break;
        }

        memcpy(&blocks[1][0], &P[offset], BLOCK_SIZE);
        print_block(blocks[1], BLOCK_SIZE);
        bit_xor(blocks[1], blocks[0], BLOCK_SIZE);
        ciph_block(blocks[0], key);
        offset += BLOCK_SIZE;

   }
}

/* FIXME see if one from "block" and "mac" can be used */
static void encrypt_or_decrypt(
        uint8_t *data_array,
        uint16_t data_len,
        uint16_t num_ctr_blks,
        uint8_t *block,
        const uint8_t *nonce,
        int mac_len,
        const uint8_t* key,
        uint8_t *mac,
        int mode)
                                  
{
    uint16_t i;
    uint8_t ctr[BLOCK_SIZE];
    uint16_t length = data_len;
    
    ccm_print_str("cntr_blks: \n");
    for (i = 0; i < num_ctr_blks; i++) {
        memset(ctr, 0, BLOCK_SIZE);
        ctr[0] = 0;
        ctr[0] |= ((q-1) & 0x7); /* Add flag see table 4 in A.3 */
        memcpy(&ctr[1], nonce, n); /* Add N see table 2 in A.3  */

        /* FIXME:as max payload length is 2 pow(16). Last two octets of counters is sufficient */
        ctr[14] = (i & 0xff00) >> 8; /*Add counter see table 2 in A.3  */
        ctr[15] = i & 0xff;

        print_block(ctr, BLOCK_SIZE);
        ciph_block(ctr, key);
        print_block(ctr, BLOCK_SIZE);
        /*TODO there was bit_xor() here. What was it?*/
        if ((i == 0) && (mode == ENCRYPT)) {
          ccm_print_str("2 rows for mac: \n");
            print_block(mac, mac_len);
            bit_xor(ctr, mac, mac_len);
            print_block(mac, mac_len);
            ccm_print_str("\n");
        } else if ((i == 0) && (mode == DECRYPT)) {
            memcpy(block, ctr, BLOCK_SIZE); /* We need this first block to decrypt MAC */
        } else {
            if (length >= BLOCK_SIZE) {
                bit_xor(ctr, &data_array[(i - 1) * BLOCK_SIZE], BLOCK_SIZE);/* step 7 in NIST pdf*/
            } else {
                bit_xor(ctr, &data_array[(i - 1)  * BLOCK_SIZE], length);
            }
            length = length - BLOCK_SIZE;
        }
    }
    
}

uint32_t CCM_encrypt_and_auth(
        const uint8_t *key,
        const uint8_t *nonce,
        const uint8_t *aad,
        const uint32_t aad_len,
        uint8_t *plain_ciphertext,
        const uint16_t text_to_encrypt_len)
{
    uint8_t blocks[2][BLOCK_SIZE];
    int mac_len = t;
    //uint8_t mac[BLOCK_SIZE]; /* see mac_len */
    //int i;
    uint16_t counter_blocks = (text_to_encrypt_len / BLOCK_SIZE) + 1;
    //unsigned int offset = 0;
    //int ret;

    if (text_to_encrypt_len % BLOCK_SIZE)
        counter_blocks++;

    format_b0(mac_len, text_to_encrypt_len, blocks[0], nonce);
    ccm_print_str("block[0]: ");
    print_block(blocks[0], BLOCK_SIZE);
    ciph_block(blocks[0], key);
    if(!format_aad(blocks, aad, aad_len, key)) {
        return 0;
    }

    format_payload_block(blocks, plain_ciphertext, text_to_encrypt_len, key);

    ccm_print_str("MAC/T/auth tag: ");
    print_block(blocks[0], mac_len);
    encrypt_or_decrypt(plain_ciphertext, text_to_encrypt_len, counter_blocks, blocks[0], nonce, mac_len, key, blocks[0], ENCRYPT);
    ccm_print_str("Encrypted text: ");
    print_block(plain_ciphertext, text_to_encrypt_len);
    memcpy(plain_ciphertext + text_to_encrypt_len, blocks[0], mac_len);

    return (uint32_t)(text_to_encrypt_len + mac_len);
}
/* ---------------------------- Decrypt part ---------------------------------*/


uint16_t CCM_decrypt_and_auth(
   const uint8_t *key,
   const uint8_t *nonce,
   const uint8_t *aad,
   const uint32_t aad_len,
   uint8_t *cipher_plaintext,
   const uint32_t ciphertext_len
)
{
    uint8_t blocks[2][BLOCK_SIZE];
    unsigned int num_counter_blocks = (ciphertext_len - t) / BLOCK_SIZE + 1;
    uint8_t mac[BLOCK_SIZE]; /* see mac_len */
    int mac_len = t;
    //int i, j;
    //uint8_t *first_block;
    //unsigned int offset = 0;
    //int ret;

    if ((ciphertext_len - t) % BLOCK_SIZE)
        num_counter_blocks++;

    if (ciphertext_len < t) {
      ccm_print_str("INVALID");
      return 0;
    }
    ccm_print_str("decryption ");
    encrypt_or_decrypt(cipher_plaintext, ciphertext_len - t, num_counter_blocks, blocks[0], nonce, mac_len, key, mac, DECRYPT);
    ccm_print_str("decrypted text: ");
    print_block(cipher_plaintext, ciphertext_len - t);

    /* Decrypt the mac which is at the end of cyphertex */
    bit_xor(&cipher_plaintext[ciphertext_len - t], blocks[0], t);
    memcpy(mac, blocks[0], t);
    ccm_print_str("mac from ciphertext: ");
    print_block(mac, mac_len);

    format_b0(mac_len, ciphertext_len - t, blocks[0], nonce);
    print_block(blocks[0], BLOCK_SIZE);
    ciph_block(blocks[0], key);
    if(!format_aad(blocks, aad, aad_len, key)) {
        return 0;
    }

    format_payload_block(blocks, cipher_plaintext, ciphertext_len - t, key);

    ccm_print_str("mac: ");
    print_block(blocks[0], mac_len);
    if (memcmp(blocks[0], mac, t) != 0) {
      ccm_print_str("INVALID See step 10 in NIST pdf\n"); /* TODO: Come up with nice error message here */
        //memset(cipher_plaintext, 0, ciphertext_len - t);
        return 0;
    }
    return ciphertext_len - t;
}

#ifndef CCM_USE_PREDEFINED_VALUES
void set_q_n_t(uint8_t q_in, uint8_t n_in, uint8_t t_in)
{
  q = q_in;
  n = n_in;
  t = t_in;
}
#endif

void get_q_n_t(uint8_t * q_out, uint8_t * n_out, uint8_t * t_out)
{
  *q_out = q;
  *n_out = n;
  *t_out = t;
}

void get_q(uint8_t * q_out)
{
  *q_out = q;
}
