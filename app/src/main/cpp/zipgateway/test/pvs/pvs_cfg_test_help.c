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
#include <string.h>
#include "test_helpers.h"
#include "provisioning_list.h"
#include "provisioning_list_files.h"
#include "pvs_cfg.h"

/* Misc pvs test helpers */

/* Test data */

uint8_t happy_bootmode[14] = {PVS_BOOTMODE_S2, PVS_BOOTMODE_SMART_START, PVS_BOOTMODE_SMART_START, PVS_BOOTMODE_SMART_START,
                            PVS_BOOTMODE_S2, PVS_BOOTMODE_S2, PVS_BOOTMODE_S2, PVS_BOOTMODE_S2,
                            PVS_BOOTMODE_SMART_START, PVS_BOOTMODE_S2, PVS_BOOTMODE_S2, PVS_BOOTMODE_SMART_START,
                            PVS_BOOTMODE_S2, PVS_BOOTMODE_S2
};

static uint8_t val_ni_ch[] = {0xe4, 0xbd, 0xa0, 0x00};

struct pvs_tlv tlv_name_lamp = {NULL, PVS_TLV_TYPE_NAME, 11, (uint8_t*)"loftslampe"};
struct pvs_tlv tlv_name_sensor = {NULL, PVS_TLV_TYPE_NAME, 7, (uint8_t*)"sensor"};
struct pvs_tlv tlv_name_lamp2 = {NULL, PVS_TLV_TYPE_NAME, 10, (uint8_t*)"bordlampe"};
struct pvs_tlv tlv_name_child = {NULL, PVS_TLV_TYPE_NAME, 28, (uint8_t*)"This is *mine*!! Hands off."};
struct pvs_tlv tlv_name_ni_ch = {NULL, PVS_TLV_TYPE_NAME, 4, val_ni_ch};

uint8_t val_name_long[] = "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123";
struct pvs_tlv tlv_name_long = {NULL, PVS_TLV_TYPE_NAME, 255, val_name_long};

struct pvs_tlv tlv_254_empty = {NULL, 254, 1, (uint8_t*)""};
struct pvs_tlv tlv_255_empty = {NULL, 254, 1, (uint8_t*)""};

struct pvs_tlv tlv_loc_ni_ch = {NULL, PVS_TLV_TYPE_LOCATION, 4, val_ni_ch};
struct pvs_tlv tlv_loc_bedroom_dk = {NULL, PVS_TLV_TYPE_LOCATION, 13, (uint8_t*)"soveværelse"};
struct pvs_tlv tlv_loc_thebedroom_dk = {NULL, PVS_TLV_TYPE_LOCATION, 14, (uint8_t*)"soveværelset"};
struct pvs_tlv tlv_loc_bedroom_fr = {NULL, PVS_TLV_TYPE_LOCATION, 17, (uint8_t*)"Chambre d'Irène"};
struct pvs_tlv tlv_loc_childs_room_dk = {NULL, PVS_TLV_TYPE_LOCATION, 15, (uint8_t*)"Børneværelse"};

struct pvs_tlv tlv_loc_oakchest_fr = {NULL, PVS_TLV_TYPE_LOCATION, 18, (uint8_t*)"Commode en chêne"};
struct pvs_tlv tlv_loc_cottage_q_dk = {NULL, PVS_TLV_TYPE_LOCATION, 22, (uint8_t*)"Sommerhus, 'Hornbæk'"};
struct pvs_tlv tlv_loc_cottage_dk = {NULL, PVS_TLV_TYPE_LOCATION, 19, (uint8_t*)"Sommerhus Hornbæk"};
struct pvs_tlv tlv_loc_cottage_nl_dk = {NULL, PVS_TLV_TYPE_LOCATION, 20, (uint8_t*)"Sommerhus\n Hornbæk"};
struct pvs_tlv tlv_loc_cottage_backslash_dk = {NULL, PVS_TLV_TYPE_LOCATION, 20, (uint8_t*)"Sommerhus\\nHornbæk"};
struct pvs_tlv tlv_loc_cottage_tab_dk = {NULL, PVS_TLV_TYPE_LOCATION, 19, (uint8_t*)"Sommerhus	Hornbæk"};
uint8_t val_fuwa[] = {0xe5, 0xaf, 0x8c, 0xe5, 0x8d, 0x8e, 0x20, 0xe9, 0xa5, 0xad, 0xe5, 0xba, 0x97, 0x00};
static struct pvs_tlv tlv_loc_restaurant_ch =  {NULL, PVS_TLV_TYPE_LOCATION, 14, val_fuwa};

uint8_t val_sigma_C[] = {0xc2, 0xa9, 0x20, 0x53, 0x69, 0x67, 0x6d, 0x61, 0x20, 0x44, 0x65, 0x73, 0x69, 0x67, 0x6e, 0x73, 0x00};
struct pvs_tlv tlv_loc_sigma = {NULL, PVS_TLV_TYPE_LOCATION, 17, val_sigma_C};

uint8_t val_hex_54[] = {0x54, 0xff, 0x37, 0x81, 0x00};
uint8_t val_hex_5401[] = {0x54, 0xFF, 0x37, 0x81, 0x01};
struct pvs_tlv tlv_prodid_54 = {NULL, PVS_TLV_TYPE_PRODUCT_ID, 5, val_hex_54};
struct pvs_tlv tlv_prodid_5401 = {NULL, PVS_TLV_TYPE_PRODUCT_ID, 5, val_hex_5401};
struct pvs_tlv tlv_0_54 = {NULL, 0, 5, val_hex_54};
struct pvs_tlv tlv_255_54 = {NULL, 255, 5, val_hex_54};

struct pvs_tlv *happy_tlvs0[] = {NULL};
struct pvs_tlv *happy_tlvs1[] = {&tlv_prodid_54, &tlv_loc_thebedroom_dk, &tlv_name_lamp, NULL};
struct pvs_tlv *happy_tlvs2[] = {&tlv_prodid_54, &tlv_name_lamp2, &tlv_loc_oakchest_fr,
                                 &tlv_254_empty, &tlv_255_54, &tlv_0_54, NULL};
struct pvs_tlv *happy_tlvs3[] = {&tlv_prodid_5401, &tlv_loc_ni_ch, NULL};
struct pvs_tlv *happy_tlvs4[] = {&tlv_loc_cottage_q_dk, &tlv_name_ni_ch, NULL};
struct pvs_tlv *happy_tlvs5[] = {&tlv_loc_sigma, &tlv_name_sensor, NULL};
struct pvs_tlv *happy_tlvs6[] = {&tlv_loc_childs_room_dk, &tlv_name_child, NULL};
struct pvs_tlv *happy_tlvs7[] = {&tlv_loc_cottage_tab_dk, &tlv_name_long, NULL};
struct pvs_tlv *happy_tlvs8[] = {&tlv_prodid_54, NULL};
struct pvs_tlv *happy_tlvs9[] = {&tlv_loc_cottage_nl_dk, &tlv_name_sensor, NULL};
struct pvs_tlv *happy_tlvs10[] = {&tlv_loc_cottage_backslash_dk, &tlv_name_sensor, NULL};
struct pvs_tlv *happy_tlvs11[] = {&tlv_0_54, &tlv_loc_bedroom_fr, &tlv_name_lamp, NULL};
struct pvs_tlv *happy_tlvs12[] = {&tlv_loc_childs_room_dk, &tlv_name_child, NULL};
struct pvs_tlv *happy_tlvs13[] = {&tlv_loc_restaurant_ch, &tlv_name_long, NULL};
struct pvs_tlv *happy_tlvs14[] = {NULL};

struct pvs_tlv **tlvs[] = {happy_tlvs0, happy_tlvs1, happy_tlvs2, happy_tlvs3,
                           happy_tlvs4, happy_tlvs5, happy_tlvs6, happy_tlvs7,
                           happy_tlvs8, happy_tlvs9, happy_tlvs10, happy_tlvs11,
                           happy_tlvs12, happy_tlvs13, happy_tlvs14};




/* Validators */

void steal_file(const char *oldname, const char *newname){
    test_print(3, "Steal file %s\n", oldname);
    (void)rename(oldname, newname);
}


void check_file_exists(const char *filename, const char *msg)
{
    FILE *strm;

    strm = fopen(filename, "r");
    if (strm) {
        test_print(2, "PASS - found %s. %s\n", filename, msg);
        (void)fclose(strm);
    } else {
        test_print(0, "FAIL - did not find %s. %s\n", filename, msg);
        numErrs++;
    }
}

void check_tlv(struct pvs_tlv *template, struct pvs_tlv *tlv)
{
    check_not_null(tlv, "TLV exists");
    if (tlv == NULL) {
        return;
    }
    test_print(3, "Compare lengths, got %u, expected %u\n", tlv->length, template->length);
    check_true(tlv->length == template->length, "Length is correct");
    if (tlv->type == 100 || tlv->type == 101) {
        int res = strcmp((char*)(tlv->value), (char*)(template->value));
        test_print(3, "Compare %s and %s\n", tlv->value, template->value);
        check_true(res==0, "String is imported correctly");
    } else {
        int res = strncmp((char*)(tlv->value), (char*)(template->value), tlv->length);
        check_true(res==0, "Value is imported correctly");
    }
}

void check_device(struct provision *pvs, uint8_t ii)
{
    struct pvs_tlv **templates;
    uint8_t jj = 0;

    test_print(3, "Checking device %d\n", ii);
    check_not_null(pvs, "DSK imported");

    if (pvs == NULL) {
        return;
    }
    check_true(pvs->bootmode == happy_bootmode[ii], "Boot mode imported");
    templates = tlvs[ii];
    while (templates[jj] != NULL) {
        test_print(3, "Check tlv type %u\n", templates[jj]->type);
        check_tlv(templates[jj], provisioning_list_tlv_get(pvs, templates[jj]->type));
        jj++;
        /* Negative test, no examples have type 17*/
        check_null(provisioning_list_tlv_get(pvs, 17), "Incorrect tlv not imported.");
        if (11 == ii) {
            check_null(provisioning_list_tlv_get(pvs, 1), "Incorrect tlv 0 not imported.");
        }
    }
}
