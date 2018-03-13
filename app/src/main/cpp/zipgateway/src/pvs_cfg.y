/* -*- C -*- */
%{
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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "provisioning_list.h"
#include "pvs_internal.h"
#include "pvs_parse_help.h"
#include <ZIP_Router_logging.h>

    extern FILE *yyin;
    extern int yyleng;
    /* size_t pvs_parse_buf_len; */

  extern int yylex(void);
  extern int yylex_destroy (void );

  void yyerror (char const *);

  static uint16_t device_count = 0;
  static uint16_t devices_added = 0;
  static uint16_t errors_found = 0;

#ifndef DOXYGEN_SHOULD_SKIP_THIS

%}

%locations
%defines
%define parse.error verbose

%define api.value.type union
/* %define api.value.type {int} */
 /* The terminals */
%token <int> NUM
%token <uint16_t> QRBLOCK
%token <int> HEADER
%token <int> DEVICE
%token <int> QR
%token <int> TYPE
%token <struct pvs_parse_buf*> DSK
%token <int> S2
%token <int> SS
%token <int> DSK-BOOTMODE
%token <int> VALUE
%token <int> TLVS

 //%token <struct pvs_parse_buf*> DSKVAL
%token <struct pvs_parse_buf*> ASCIIHEX
%token <struct provision*> DEVICEPTR

%token <int> TLVTYPE
%token <struct pvs_parse_buf*> STR
%token <struct pvs_tlv*> TLV

 /* The non-terminals */
%type <int> dsk-bootmode-spec
%type <struct provision*> pvscfg
%type <struct provision*> devspec
%type <struct pvs_parse_buf*> dsk-spec
%type <struct pvs_tlv*> tlvspec
%type <struct pvs_tlv*> tlv-list
%type <struct pvs_tlv*> tlv-list-spec
%type <struct pvs_parse_buf*> tlv-value-spec

%destructor {
    /* printf("Cleaning up strings automagically\n"); */
    pvs_parse_buf_free($$);
} dsk-spec ASCIIHEX STR tlv-value-spec

%destructor {
    /* printf("Cleaning up tlvs automagically\n"); */
    pvs_tlv_clear($$);
  } tlv-list tlv-list-spec tlvspec

%% /* Grammar rules and actions follow.  */

config:
HEADER pvscfg {
    printf("Created %u devices\n", devices_added);
}

/* A provisioning list specification is a collection of device specifications */
pvscfg:
%empty {
    printf("Starting import.\n");
}
| pvscfg devspec {
    /* printf("Reduce on device %d of %d\n", devices_added, device_count); */
}
| pvscfg error {
    printf("Error on %d, %d\n", devices_added, device_count);
}


/* A device specification gives the DSK, the bootmode and the list of tlvs */
devspec:
  DEVICE '=' '[' dsk-spec ',' dsk-bootmode-spec ',' tlv-list-spec ']'  {
      device_count++;
      if ($6 >= 0 && $4 != NULL) {
          struct provision *dev;
          size_t ii;
          dev = provisioning_list_dev_add($4->len, $4->buf, $6);
          if (dev != NULL) {
              devices_added++;
              dev->tlv_list = $8;
              printf("Created device %3d (0x",
                     device_count);
              for (ii=0; ii<$4->len; ii++) {
                  printf("%02x", $4->buf[ii]);
              }
          printf(").\n");
          } else {
              pvs_tlv_clear($8);
              errors_found++;
              if (device_count > PROVISIONING_LIST_SIZE) {
                  printf("Failed to add device %d. Max %d devices allowed.\n",
                         device_count, PROVISIONING_LIST_SIZE);
              } else {
                  printf("Failed to add device %d.  DSK overlap or memory shortage.\n", device_count);
              }
          }
      } else {
          pvs_tlv_clear($8);
          errors_found++;
          printf("Errors in specification.  Skipping device %d.\n", device_count);
      }
      if ($4) {
          if ($4->buf) {
              free($4->buf);
              $4->buf = NULL;
          }
          free($4);
      }
/* | '\n' { printf("> "); } */
  }
|  DEVICE '=' '[' dsk-spec ',' dsk-bootmode-spec ',' error ']'  {
    pvs_parse_buf_free($4);
    device_count++;
    errors_found++;
    printf("Invalid TLV specification in device %u.\n",
           device_count);
   }
|  DEVICE '=' '[' dsk-spec ',' error ',' tlv-list-spec ']'  {
    pvs_parse_buf_free($4);
    pvs_tlv_clear($8);
    device_count++;
    errors_found++;
    printf("Invalid device boot mode specification in device %u, line %d.\n",
           device_count, yylloc.last_line);
   }
|  DEVICE '=' '[' error ',' dsk-bootmode-spec ',' tlv-list-spec ']'  {
    device_count++;
    errors_found++;
    pvs_tlv_clear($8);
    printf("Invalid DSK specification in device %u.\n",
        device_count);
   }
|  DEVICE '=' '[' error ',' error ',' error ']'  {
    device_count++;
    errors_found++;
    printf("Invalid device specification in device %u.\n",
        device_count);
   }


dsk-spec:
DSK '=' ASCIIHEX {
    $$ = $3;
    if ($3 != NULL) {
         /* printf("Read ascii-hex dsk of length %d, converted to ascii buffer (size %u).\n", */
         /*        yyleng, $3->len); */
        if ($3->len != 16) {
            printf("DSK length incorrect in device %u, line %d (%d), got %u, expected %u.\n",
                   device_count+1, yylloc.last_line, yylloc.first_column, $3->len, 16);
            free($3->buf);
            free($3);
            $$ = NULL;
            YYERROR;
        }
    }
}
| DSK '=' QR ':' QRBLOCK '-' QRBLOCK '-' QRBLOCK '-' QRBLOCK '-' QRBLOCK '-' QRBLOCK '-' QRBLOCK '-' QRBLOCK {
    /* printf("Read QR block\n"); */
    $$ = pvs_qr_dsk_add($5, $7, $9, $11, $13, $15, $17, $19);
}
| DSK '=' QR ':' error {
    printf("Incorrect DSK QR format in device %d, line %d (%d).\n",
           device_count+1, yylloc.last_line, yylloc.first_column);
    $$ = NULL;
}

dsk-bootmode-spec:
  TYPE '=' S2                { $$ = PVS_BOOTMODE_S2; }
| TYPE '=' SS              { $$ = PVS_BOOTMODE_SMART_START; }

tlv-list-spec:
  TLVS '=' '(' tlv-list ')' { $$ = $4; }
| TLVS '=' '(' error ')' {
    printf("Invalid tlv list for device %u\n", device_count+1);
    $$ = NULL;
    YYERROR;
  }

tlv-list:
%empty { $$ = NULL;}
| tlv-list tlvspec {
    /*  The list is reversed here.  That is ok, since the order is not important. */
    if ($2) {
        if (pvs_tlv_get($1, $2->type) != NULL) { 
            printf("Duplicate TVL definition (type %u) in line %d\n", $2->type, yylloc.last_line);
            errors_found++;
            free($2->value);
            free($2);
            $$ = $1; /* Should be cleaned up later */
        } else {
            $2->next = $1;
            $$ = $2;
        }
    } else {
        $$ = $1;
    }
 }

tlvspec:
'[' TYPE '=' NUM ',' VALUE '=' tlv-value-spec ']' {
    if ($8 != NULL) {
        $$ = malloc(sizeof(struct pvs_tlv));
        memset($$, 0, sizeof(struct pvs_tlv));
        if ($$ != NULL) {
            $$->type = $4;
            $$->length = $8->len;
            $$->value = $8->buf;
            $8->buf = NULL;
        } else {
            if ($8->buf) {
                free($8->buf);
            }
        }
        free($8);
    } else {
        $$ = NULL;
    }
}
| '[' TYPE '=' NUM ',' VALUE '=' error ']' {
    printf("Illegal tlv value specification on line %d.\n", yylloc.last_line);
    $$ = NULL;
    YYERROR;
  }
| '[' error ']' {
    printf("Illegal tlv specification on line %d.\n", yylloc.last_line);
    $$ = NULL;
  }

tlv-value-spec:
  ASCIIHEX
| STR

%%

  int pvs_parse_config_file(FILE *strm)
  {
      int res = 0;

      device_count = 0;
      devices_added = 0;
      errors_found = 0;
      yylloc.last_line = 1;

      yyin = strm;
      res = yyparse();
      yylex_destroy();

      if (errors_found > 0) {
          ERR_PRINTF("Ignoring config file and starting with empty provisioning list.  Found %d errors during import.\n",
                     errors_found);
          provisioning_list_clear();
          return -1; /* Parsing succeeded, but we found semantic errors */
      } else {
          return res;
      }
  }

/* Called by yyparse on error.  */
void
yyerror (char const *s)
{
    errors_found++;
    fprintf (stderr, "Cannot import the PVL configuration file: %s\n", s);
}

#endif /* DOXYGEN_SHOULD_SKIP_THIS */
