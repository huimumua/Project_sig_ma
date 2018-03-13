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
/**
 * @file
 */
/******************************************************************************
 * Copyright AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <ctr_drbg.h>
#include <aes.h>

#ifdef VERBOSE
#include <stdio.h>
#endif

#ifndef __C51__
void AJ_AES_ECB_128_ENCRYPT(uint8_t* key, uint8_t* in, uint8_t* out)
{
    AES128_ECB_encrypt(in, key, out);
}
#endif


static void AES_CTR_DRBG_Increment(uint8_t* __data, size_t size)
{
    while (size--) {
        __data[size]++;
        if (__data[size]) {
            break;
        }
    }
}
/*
CTR_DRBG_Update (provided_data, Key, V):
1.provided_data: The data to be used. This must be exactly seedlen
bits in length; this length is guaranteed by the construction of the
provided_data in the instantiate, reseed and generate functions.
2.Key: The current value of Key.
3.V: The current value of V.

Output:
1.K: The new value for Key.
2.V: The new value for V.
*/

static void AES_CTR_DRBG_Update(CTR_DRBG_CTX* ctx, uint8_t __data[SEEDLEN])
{
    size_t i = 0;
    uint8_t tmp[SEEDLEN];
    uint8_t* t = tmp;
#ifdef VERBOSE
    int j = 0;
#endif

    //AJ_AES_Enable(ctx->k);
    for (i = 0; i < SEEDLEN; i += OUTLEN) {
        AES_CTR_DRBG_Increment(ctx->v, OUTLEN); /*V= (V+ 1) mod 2 pow(outlen) */
#ifdef __C51__
        AES128_ECB_encrypt(ctx->v, ctx->k, t);
#else
        AJ_AES_ECB_128_ENCRYPT(ctx->k, ctx->v, t); /* output_block =  Block_Encrypt(Key, V). */
#endif
        t += OUTLEN; /*temp = temp || ouput_block */
    }

    for (i = 0; i < SEEDLEN; i++) {
        /* temp = Leftmost seedlen bits of temp.
           temp = temp || provided_data;
        */
        tmp[i] ^= __data[i];
    }

    memcpy(ctx->k, tmp, KEYLEN);
#ifdef VERBOSE
    printf("\n");
    printf("key: ");
    for(j = 0; j < KEYLEN; j++)
        printf("%x ", ctx->k[j]);
    printf("\n");
#endif
    memcpy(ctx->v, tmp + KEYLEN, OUTLEN);
#ifdef VERBOSE
    printf("v: ");
    for(j = 0; j < OUTLEN; j++)
        printf("%x ", ctx->v[j]);
    printf("\n");
#endif
}


void AES_CTR_DRBG_Reseed(CTR_DRBG_CTX* ctx, uint8_t* seed)
{
#ifdef VERBOSE
    printf("Reseed: ");
#endif
    AES_CTR_DRBG_Update(ctx, seed);
}


void AES_CTR_DRBG_Instantiate(CTR_DRBG_CTX* ctx, uint8_t* entropy, const uint8_t* personal)
{
#if 1
    uint8_t i;
#else
    int i;
#endif

    for (i = 0; i < SEEDLEN; i++) {
        /* temp = Leftmost seedlen bits of temp.
           temp = temp || provided_data;
        */
        entropy[i] ^= personal[i];
    }

#ifdef VERBOSE
    printf("Instantiate: ");
#endif
    memset(ctx->k, 0, KEYLEN);
    memset(ctx->v, 0, OUTLEN);
    ctx->df = 0;
    AES_CTR_DRBG_Reseed(ctx, entropy);
}

void AES_CTR_DRBG_Generate(CTR_DRBG_CTX* ctx, uint8_t* rand)
{
    uint8_t __data[SEEDLEN];
    size_t copy;
    size_t size = RANDLEN;

//    /* Needed?? Is uint8_t enough ??? */
//    uint16_t count = 0;

#ifdef VERBOSE
    printf("Generate: ");
#endif
    // Reseed interval 2^32 (counter wraps to zero)
    // See section 10.2.1.5.1. Step 1 in "CTR_DRBG Generate Proces"
    //AJ_AES_Enable(ctx->k);
    while (size) {
        AES_CTR_DRBG_Increment(ctx->v, OUTLEN);
#ifdef __C51__
        AES128_ECB_encrypt(ctx->v, ctx->k, __data);
#else
        AJ_AES_ECB_128_ENCRYPT(ctx->k, ctx->v, __data);
#endif
        copy = (size < OUTLEN) ? size : OUTLEN;
        memcpy(rand, __data, copy);
        rand += copy;
        size -= copy;
//        count+= copy;
    }

    memset(__data, 0, SEEDLEN);
    AES_CTR_DRBG_Update(ctx, __data);
}
