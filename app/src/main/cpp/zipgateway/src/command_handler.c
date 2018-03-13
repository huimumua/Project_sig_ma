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
 * command_handler.c
 *
 *  Created on: Apr 7, 2016
 *      Author: aes
 */
#include "ZIP_Router.h"

#include "command_handler.h"
#ifdef __APPLE__
extern command_handler_t __start__handlers[] __asm("section$start$__TEXT$__handlers");
extern command_handler_t __stop__handlers[] __asm("section$end$__TEXT$__handlers");
#else
extern command_handler_t __start__handlers[];
extern command_handler_t __stop__handlers[];
#endif

static uint16_t* disable_command_list;
static uint32_t  disable_command_list_len;

int is_in_disabled_list(command_handler_t* fn) {
  uint32_t i;

  for(i= 0; i<disable_command_list_len; i++ ) {
    if( fn->cmdClass == disable_command_list[i] ) {
      return 1;
    }
  };
  return 0;
}

/*
 * If a handler is supported with NO_SCHEME we allow the package no matter what
 * scheme it was received on.
 *
 * If a handler has a minimal security scheme it has to be sent with the net_scheme
 * ie the highest scheme supported by the node.
 */
static int
supports_frame_at_security_level(command_handler_t *fn, security_scheme_t scheme)
{
  if(scheme==USE_CRC16 ) {
    scheme = NO_SCHEME;
  }

  if(fn->minimal_scheme == NET_SCHEME) {
    return (scheme == net_scheme) || ( scheme == SECURITY_SCHEME_UDP );
  } else if ((fn->minimal_scheme == NO_SCHEME) || ( scheme_compare(scheme , net_scheme) && scheme_compare(scheme, fn->minimal_scheme)))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

command_handler_codes_t
ZW_command_handler_run(zwave_connection_t *connection, uint8_t* payload, uint16_t len)
{
  command_handler_t *fn;

  for (fn = __start__handlers; fn < __stop__handlers; fn++)
  {
    if (fn->cmdClass == payload[0])
    {
      if (supports_frame_at_security_level(fn, connection->scheme))
      {
        if(is_in_disabled_list(fn) ) {
          return COMMAND_CLASS_DISABLED;
        } else {
          return fn->handler(connection, payload, len);
        }
      } else {
        WRN_PRINTF("Command rejected because of wrong security class %i\n", connection->scheme);
      }
    }
  }
  return CLASS_NOT_SUPPORTED;
}

uint8_t
ZW_comamnd_handler_version_get(security_scheme_t scheme, uint16_t cmdClass)
{
  command_handler_t *fn;

  for (fn = __start__handlers; fn < __stop__handlers; fn++)
  {
    if( is_in_disabled_list(fn) ) continue;

    if (fn->cmdClass == cmdClass)
    {
      if (supports_frame_at_security_level(fn, scheme))
      {
        return fn->version;
      }
    }
  }
  return 0x00; //UNKNOWN_VERSION;;
}

uint8_t
ZW_command_handler_get_nif(security_scheme_t scheme, uint8_t* nif, uint8_t max_len)
{

  command_handler_t *fn;
  uint8_t n = 0;

  for (fn = __start__handlers; fn < __stop__handlers; fn++)
  {
    if (is_in_disabled_list(fn))
      continue;

    if (supports_frame_at_security_level(fn, scheme))
    {
      /*If we are building a secure nif then don't add stuff which is supported non-secure*/
      if(scheme !=NO_SCHEME && supports_frame_at_security_level(fn, NO_SCHEME)) {
        continue;
      }

      /* Z-Wave Plus info is inserted elsewhere because it MUST be listed first in NIF */
      if (fn->cmdClass == COMMAND_CLASS_ZWAVEPLUS_INFO_V2)
      {
        continue;
      }
      if ((fn->cmdClass & 0xF0) == 0xF0)
      {
        if (max_len - n < 2)
        {
          WRN_PRINTF("Max classes exceeded.\n");
          assert(0);
          return n;
        }
        nif[n++] = (fn->cmdClass >> 8) & 0xFF;
        nif[n++] = fn->cmdClass & 0xFF;
      }
      else
      {
        if (max_len - n < 1)
        {
          WRN_PRINTF("Max classes exceeded.\n");
          assert(0);
          return n;
        }
        nif[n++] = fn->cmdClass & 0xFF;
      }
    }
  }
  return n;
}

void
ZW_command_handler_init()
{
  command_handler_t *fn;

  for (fn = __start__handlers; fn < __stop__handlers; fn++)
  {
    if (fn->init)
    {
      fn->init();
    }
  }
}


void ZW_command_handler_disable_list(uint16_t *cmdList, uint8_t cmdListLen) {
  disable_command_list = cmdList;
  disable_command_list_len = cmdListLen;
}

