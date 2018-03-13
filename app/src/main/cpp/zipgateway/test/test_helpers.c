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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "test_helpers.h"
#include <errno.h>

/* For now... We probaby want to move these to a separate helper file when we want to use them in cases. */
uint8_t MyNodeID = 1;
uint32_t homeID = 0x7357F0F0;

int numErrs = 0;
int numCases = 0;
int numCasesFail = 0;

int verbosity = test_number_of_levels;

const char* currCase = NULL;
int numErrsStartCase = 0;
FILE *caseLogFile = NULL;

void test_print_int(int lvl, char *fmt, ...);
static const char *path_trim(const char *name);

/**
 * This version is for Linux with '/' path delimiter.
 *
 */
static const char *path_trim(const char *name) {
  const char *res = strrchr(name, '/');

  /* Remove the path */
  if (res) {
    /* Skip the '/' */
    res++;
  } else {
    /* There was no path in the name, so just use as is */
    res = name;
  }
  return res;
}

/* dummy for testing, should return constant */
/* Used by DBG_PRINTF and friends.  Should go in a separate file that stubs contiki/core/sys/clock.h */
unsigned long clock_time(void) {
    return 0;
}

// TODO add paths
FILE *test_create_log(const char *logname) {
    FILE *strm;

    if (logname == NULL) {
        printf("Test error: please provide a log file name.\n");
        numErrs++;
    }
    logname = path_trim(logname);
    printf("Logging to: %s\n", logname);
    strm = fopen(logname, "w");
    if (strm == NULL) {
        numErrs++;
        printf("Test error %s (%d)\n", strerror(errno), errno);
    }
    return strm;
}

void start_case(const char *str, FILE *logfile)
{
    test_print_int(2, "\n--- CASE %s ---\n", str);
    numCases++;
    currCase = str;
    numErrsStartCase = numErrs;
    caseLogFile = logfile;
    if (logfile) {
        fprintf(logfile, "\n--- CASE %s ---\n", str);
    }
}

void close_case(const char *str)
{
    int caseErrs = 0;
    if (strcmp(str, currCase) == 0) {
        caseErrs = numErrs - numErrsStartCase;
    } else {
        test_print_int(2, "\n--- CASE %s ---\n", str);
        if (caseLogFile) {
            fprintf(caseLogFile, "\n--- CASE %s ---\n", str);
        }
        caseErrs = numErrs;
    }
    if (caseErrs == 0) {
        test_print_int(2, "    CASE PASSED\n--- END %s ---\n", str);
        if (caseLogFile) {
            fprintf(caseLogFile,  "CASE PASSED\n--- END %s ---\n", str);
        }
    } else {
        numCasesFail++;
        test_print_int(2, "    CASE FAILED with %d errors\n--- END %s ---\n",
                   caseErrs, str);
        if (caseLogFile) {
            fprintf(caseLogFile, "CASE FAILED with %d errors\n--- END %s ---\n",
                    caseErrs, str);
        }
    }
}

void close_run()
{
    if (numCases) {
        test_print_int(0, "\n\nExecuted %d cases.\n", numCases);
        if (numCasesFail) {
            test_print_int(0, "%d of the cases had at least one error.\n", numCasesFail);
        }
    }
    if (numErrs) {
        test_print_int(0, "\nFAILED, %d errors\n", numErrs);
    } else {
        test_print_int(0, "\nPASSED\n");
    }
}

void test_print_int(int lvl, char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    if (lvl <= verbosity) {
        vprintf(fmt, args);
    }
    va_end(args);
}

void test_print(int lvl, char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    if (lvl <= verbosity) {
        printf("    TC: ");
        vprintf(fmt, args);
        if (caseLogFile) {
            fprintf(caseLogFile, "    ");
            vfprintf(caseLogFile, fmt, args);
        }
    }
    va_end(args);
}

void test_print_suite_title(int lvl, char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    if (lvl <= verbosity) {
        printf("\n===== ");
        vprintf(fmt, args);
        printf(" =====\n");
        if (caseLogFile) {
            fprintf(caseLogFile, "\n===== ");
            vfprintf(caseLogFile, fmt, args);
            fprintf(caseLogFile, " =====\n");
        }
    }
    va_end(args);
}

void check_zero(int ii, char *msg)
{
    if (ii) {
        test_print(0, "FAIL - value is %d. %s\n", ii, msg);
        numErrs++;
    } else {
        test_print(2, "PASS. %s\n", msg);
    }
}

void check_true(int8_t ii, char *msg)
{
    if (ii) {
        test_print(2, "PASS. %s\n", msg);
    } else {
        test_print(0, "FAIL, check is false. %s\n", msg);
        numErrs++;
    }
}

void check_null(void *pvs, char *msg)
{
    if (!pvs) {
        test_print(2, "PASS. %s\n", msg);
    } else {
        test_print(0, "FAIL - pointer %p found. %s\n", pvs, msg);
        numErrs++;
    }
}

void check_not_null(void *pvs, char *msg)
{
    if (pvs) {
        test_print(2, "PASS - Pointer %p found. %s\n", pvs, msg);
    } else {
        test_print(0, "FAIL - %s\n", msg);
        numErrs++;
    }
}
