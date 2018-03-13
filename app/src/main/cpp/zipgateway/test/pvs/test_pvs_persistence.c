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

#include <stdio.h>
#include <provisioning_list.h>
#include <provisioning_list_files.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <lib/zgw_log.h>
#include "test_helpers.h"
#include "pvs_cfg_test_help.h"

zgw_log_id_declare(pvs_test);
zgw_log_id_default_set(pvs_test);

/**
\ingroup pvs_test

* @{
*
*/


static char *default_filename = PROVISIONING_LIST_STORE_FILENAME_DEFAULT;

//static char *buf[8192];

static uint8_t dsk1[16] = {12, 34, 00, 17, 12, 34, 00, 17, 12, 34, 00, 17, 12, 34, 00, 17};
static uint8_t dsk2[16] = {56, 78, 21, 19, 12, 34, 00, 17, 12, 34, 00, 17, 12, 34, 00, 17};
static uint8_t dsk3[16] = {78, 21, 19, 77, 3, 0, 4, 0, 0, 78, 21, 19, 77, 3, 0, 4};
static uint8_t dsk4[16] = {56, 78, 21, 19, 0, 3, 0, 4, 0, 0, 0xff, 0xfe, 0x10, 0xa, -1, -1};
static uint8_t dsk5[16] = {56, 78, 21, 19, 0, 0xbe, 0, 4, 0, 0, 0xff, 0xfe, 0x10, 0xa, -1, -1};
static uint8_t dsk6[16] = {56, 78, 21, 19, 0, 3, 0xff, 4, 0, 0, 0xff, 0xfe, 0x10, 0xa, -1, -1};
//static uint8_t dsk11[] =        {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
//static uint8_t challenge11[] =  { 0,  0, 12, 13, 14, 15, 16, 17, 18, 19, 20};
//static uint8_t dsk15[] =        {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 51, 62, 73, 76};
//static uint8_t challenge15[]  =         {12,  0, 12, 13, 14, 15, 16, 17, 18, 19, 20};
//static uint8_t challenge155[] =         {12,  0, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22};
/* static uint8_t tmp_dsk[16] = {88, 90, 128, 00, */
/*                        17, 3, 10, 245, */
/*                        255, 0, 192, 168,  */
/*                        10, 10, 5, 9}; */

//static uint8_t *bulb = (uint8_t *)"Bulb";
//static const uint8_t *danish_bulb = (uint8_t*)"Pære";
//static uint8_t const * backyard = (uint8_t * const)"Backyard";
static uint8_t *sensor = (uint8_t *)"Sensor";
//static uint8_t *lvngroom = (uint8_t *)"Living Room\n";
//static uint8_t *bedroom = (uint8_t *)"soveværelse\n";
//static uint8_t *node3 = (uint8_t *)"Node3";
//static uint8_t *location3 = (uint8_t *)"Location3";
//static char *long_name = "I am a really long name (aka: more than 128 chars); that some weirdo has decided is a good name for his device, without considering the ensuing IT problems that he might trigger (or by a system tester)";
//static char *weird_name = "Something-else. with =-;'\"";
//static char *dummy_id = "String id";

//static uint8_t prod_type_len = 25;
/* static uint8_t prod_type[25] = {0x00, 0x45, 0x45, 0x45, */
/*                          0x45, 0x45, 0x45, 0x45, */
/*                          0x45, 0xAE, 0x45, 0xFF, */
/*                          0x45, 0x45, 0x45, 0x03, */
/*                          0x45, 0x45, 0x45, 0x45, */
/*                          0x45, 0x45, 0x45, 0x45, 0x00}; */

struct dsk_spec {
    uint8_t dsk_len;
    uint8_t * dsk;
};

static struct dsk_spec test_dsks[6] = {{16, dsk1}, {16, dsk2}, {16, dsk3}, {16, dsk4}, {16, dsk5}, {16, dsk6} };

static struct pvs_tlv tlv_sensor = {NULL, PVS_TLV_TYPE_NAME, 7, (uint8_t *)"Sensor"};
static struct pvs_tlv tlv_backyard =  {NULL, PVS_TLV_TYPE_LOCATION, 9, (uint8_t *)"Backyard"};
//static struct pvs_tlv * test_tlvs[2] = {&tlv_sensor, &tlv_backyard};

static void print_dsk(uint8_t lvl, uint8_t dsk_len, uint8_t *dsk);
static void test_filename(FILE *strm1);
static void add_tlv(void);
static void add_tlv2(void);

static void add_tlv(void)
{
    struct provision *pvs = provisioning_list_dev_get(test_dsks[3].dsk_len, test_dsks[3].dsk);

    provisioning_list_tlv_set(pvs, tlv_backyard.type, tlv_backyard.length, tlv_backyard.value);
}

static void add_tlv2(void)
{
    struct provision *pvs = provisioning_list_dev_get(test_dsks[3].dsk_len, test_dsks[3].dsk);

    provisioning_list_tlv_set(pvs, tlv_backyard.type, tlv_backyard.length, tlv_backyard.value);
    provisioning_list_tlv_set(pvs, tlv_sensor.type, tlv_sensor.length, tlv_sensor.value);
}

static void create_test_file(int numDevices, int numTLVs, int use_add) {
    struct provision *new_pvs1;
    int ii, jj;
    int res;

    for (ii = 0; ii < numDevices; ii++) {
        if (use_add) {
            new_pvs1 = provisioning_list_dev_add(test_dsks[ii].dsk_len, test_dsks[ii].dsk, PVS_BOOTMODE_S2);
        } else {
            new_pvs1 = provisioning_list_dev_set(test_dsks[ii].dsk_len, test_dsks[ii].dsk, PVS_BOOTMODE_S2);
        }
        test_print(3, "Added device %d, ", ii);
        print_dsk(3, test_dsks[ii].dsk_len, test_dsks[ii].dsk);
        test_print(3, "%p\n", new_pvs1);
        for (jj = 0; jj < numTLVs; jj++) {
            res = provisioning_list_tlv_set(new_pvs1, PVS_TLV_TYPE_NAME, 7, sensor);
            check_true(res==PVS_SUCCESS, "Create tlv");
        }
    }
}

static void print_dsk(uint8_t lvl, uint8_t dsk_len, uint8_t *dsk) {
    uint8_t ii;

    if (lvl > verbosity) {
        return;
    }
    printf("0x");
    for (ii = 0; ii < dsk_len; ii++) {
        printf("%02X", dsk[ii]);
    }
    printf(" ");
}

static void test_filename(FILE *strm1)
{
    start_case("Initialize with non-existing default file (create)", strm1);
    (void)remove(default_filename);
    provisioning_list_init(NULL, NULL);
    check_zero((int)provisioning_list_get_count(), "Importing empty list imports nothing");
    close_case("Initialize with non-existing default file (create)");

    start_case("Initialize with NULL pointer, use default", strm1);
    provisioning_list_init(NULL, NULL);
    check_zero((int)provisioning_list_get_count(), "Importing empty list imports nothing");
    // TODO: check that default file is used instead.
    pvs_list_print(strm1);

    test_print(2, "\n--- Initialize with non-existing filename (create) ---\n");
    fprintf(strm1, "\n--- Initialize with non-existing filename, use that file ---\n");
    (void)remove("newname.dat");
    provisioning_list_init("newname.dat", NULL);
    check_zero((int)provisioning_list_get_count(), "Importing empty list imports nothing");
    pvs_list_print(strm1);

    test_print(2, "\n--- Initialize with file with invalid header ---\n");
    fprintf(strm1, "\n--- Initialize with file with invalid header, use default ---\n");
    provisioning_list_init("provisioning_list_store.invalid", NULL);
    check_zero((int)provisioning_list_get_count(), "Importing empty list imports nothing");
    // TODO: validate which file is actually used.
    pvs_list_print(strm1);

    test_print(2, "\n--- Initialize with non-existing path ---\n");
    fprintf(strm1, "\n--- Initialize with non-existing path, use default ---\n");
    provisioning_list_init("dir/does/not/exist.dat", NULL);
    check_zero((int)provisioning_list_get_count(), "Importing incorrect filename imports nothing");
    close_case("Initialize with NULL pointer, use default");

    //pvs_list_print(strm1);

    start_case("Initialize from empty file", strm1);
    provisioning_list_init("empty.data", NULL);
    check_zero((int)provisioning_list_get_count(), "Importing empty file imports nothing");
    close_case("Initialize from empty file");

    start_case("Initialize from valid file with no contents (pvs list was empty)", strm1);
    provisioning_list_init("newname.dat", NULL);
    check_zero((int)provisioning_list_get_count(), "Importing empty list imports nothing");
    close_case("Initialize from valid file with no contents (pvs list was empty)");

}

int main()
{
    FILE *strm1;
    uint8_t cnt;
//    FILE *strm2;

    verbosity = test_case_start_stop;

    zgw_log_setup(NULL);

    zgw_log_enter();

    zgw_log(1, "ZGWlog test 1 \n");
    zgw_log(1, "ZGWlog test arg %d\n", 7);

    strm1 = fopen("pvs_print1.txt", "w");
    if (strm1 == NULL) {
        printf("Test error %s (%d)\n", strerror(errno), errno);
    }

    test_filename(strm1);
    provisioning_list_init("newname.dat", NULL);

    start_case("Initialize from valid file with one element\n    Store to file after add one device", strm1);
    create_test_file(1, 0, 1);
    pvs_list_print(strm1);
    steal_file("newname.dat", "one-dsk.dat");
    provisioning_list_clear();
    test_print(3, "Loading from stolen file\n");
    provisioning_list_init("one-dsk.dat", NULL);
    pvs_list_print(strm1);
    // TODO: compare before and after
    check_true(1 == provisioning_list_get_count(), "Import of one element");
    close_case("Initialize from valid file with one element\n    Store to file after add one device");

    start_case("Initialize from valid file with one element with one tlv\n    Initialize with valid existing default file (import and use)", strm1);
    provisioning_list_clear();
    provisioning_list_init(NULL, NULL);
    create_test_file(1, 1, 1);
    create_test_file(1, 1, 0);
    pvs_list_print(strm1);
    steal_file(default_filename, "tmp.dat");
    provisioning_list_clear();
    steal_file("tmp.dat", default_filename);
    provisioning_list_init(NULL, NULL);
    check_true(1 == provisioning_list_get_count(), "Import of one element");
    {
        struct provision *pvs = provisioning_list_dev_get(test_dsks[0].dsk_len, test_dsks[0].dsk);
        check_not_null(pvs, "Import dsk correctly");
        if (pvs) {
            struct pvs_tlv *tlv = provisioning_list_tlv_get(pvs, PVS_TLV_TYPE_NAME);
            check_not_null(tlv, "Import NAME tlv");
            if (tlv) {
                check_zero(strncmp((char*)tlv->value, (char*)sensor, tlv->length), "Import NAME tlv value correctly");
            }
        }
    }
    create_test_file(3,1, 0);
    check_true(provisioning_list_get_count() == 3, "Create 3 devices with one tlv");
    close_case("Initialize from valid file with one element with one tlv\n    Initialize with valid existing default file (import and use)");

    start_case("Initialize from valid file with more than one element\n    Store to file when array has holes, - Store to file after remove device", strm1);
    create_test_file(4,1, 0);
    cnt = provisioning_list_get_count();
    printf("cnt: %u\n", cnt);
    check_true(provisioning_list_get_count() == 4, "Create 4 test devices");
    // make a hole in it
    provisioning_list_dev_remove(test_dsks[2].dsk_len, test_dsks[2].dsk);
    pvs_list_print(strm1);

    steal_file(default_filename, "tmp.dat");
    provisioning_list_clear();
    provisioning_list_init("tmp.dat", NULL);

    check_true(provisioning_list_get_count() == 3, "Import 3 test devices");
    pvs_list_print(strm1);
//    steal_file("tmp.dat", "store.dat");
    close_case("Initialize from valid file with more than one element\n    Store to file when array has holes, - Store to file after remove device");

    start_case("Initialize from valid file with one element with several tlvs", strm1);
    add_tlv();
    add_tlv2();
    pvs_list_print(strm1);
    steal_file("tmp.dat", "two_tlv.dat");

    provisioning_list_clear();
    provisioning_list_init("two_tlv.dat", NULL);
    check_true(provisioning_list_get_count() == 3, "Import 3 test devices");
    pvs_list_print(strm1);
    close_case("Initialize from valid file with one element with several tlvs");

    start_case("Initialize from valid file with modified bootmode and status", strm1);
    {
       struct provision *pvs = provisioning_list_dev_get(test_dsks[2].dsk_len, test_dsks[3].dsk);
       check_true(pvs->status == PVS_STATUS_PENDING, "Status is pending");
       check_true(pvs->bootmode == PVS_BOOTMODE_S2, "Mode is S2");
       provisioning_list_status_set(pvs, PVS_STATUS_IGNORED);
       provisioning_list_bootmode_set(pvs, PVS_BOOTMODE_SMART_START);
       check_true(pvs->status == PVS_STATUS_IGNORED, "Status is 'ignored' after set");
       check_true(pvs->bootmode == PVS_BOOTMODE_SMART_START, "Mode is Smart Start after set");
       pvs_list_print(strm1);
       steal_file("two_tlv.dat", "bootmode_status.dat");

       provisioning_list_clear();
       provisioning_list_init("bootmode_status.dat", NULL);
       check_true(provisioning_list_get_count() == 3, "Import 3 test devices");
       pvs_list_print(strm1);
       pvs = provisioning_list_dev_get(test_dsks[3].dsk_len, test_dsks[3].dsk);
       check_not_null(pvs, "Provision imported");
       if (pvs != NULL) {
          check_true(pvs->status == PVS_STATUS_IGNORED, "Status is 'ignored' after re-boot");
          check_true(pvs->bootmode == PVS_BOOTMODE_SMART_START, "Mode is Smart Start after re-boot");
       }
    }
    close_case("Initialize from valid file with modified bootmode and status");

    provisioning_list_clear();

    close_run();

    fclose(strm1);

    zgw_log_exit();
    return numErrs;
}

/**
 * @}
 */
