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
 * CC_Version.c
 *
 *  Created on: Nov 9, 2016
 *      Author: aes
 */

#include "Serialapi.h"
#include "command_handler.h"
#include "ZIP_Router.h"
#include <stdlib.h>
#include "security_layer.h"

extern ZW_APPLICATION_TX_BUFFER txBuf;

/*Macro to compress this case statement*/
#define VERSION_CASE(a) \
    case COMMAND_CLASS_##a: \
      txBuf.ZW_VersionCommandClassReportFrame.commandClassVersion = a##_VERSION ;\
      break;

#define VERSION_CASE2(a,b) \
    case COMMAND_CLASS_##a: \
      txBuf.ZW_VersionCommandClassReportFrame.commandClassVersion = a##_VERSION_##b ;\
      break;

/* If the request is from PAN side return the version as 0 in case of LAN only CCs
 * for e.g. (ZIP and ZIP_ND) */
#define VERSION_CASE_LAN_ONLY(a) \
    case COMMAND_CLASS_##a: \
      if (c->scheme == SECURITY_SCHEME_UDP) { \
          txBuf.ZW_VersionCommandClassReportFrame.commandClassVersion = a##_VERSION ;\
      } else { \
          txBuf.ZW_VersionCommandClassReportFrame.commandClassVersion = 0 ;\
      }\
      break;

#define VERSION_CASE2_LAN_ONLY(a,b) \
    case COMMAND_CLASS_##a: \
      if (c->scheme == SECURITY_SCHEME_UDP) { \
          txBuf.ZW_VersionCommandClassReportFrame.commandClassVersion = a##_VERSION_##b ;\
      } else {\
          txBuf.ZW_VersionCommandClassReportFrame.commandClassVersion = 0; \
      } \
      break;

static command_handler_codes_t
VersionHandler(zwave_connection_t *c, uint8_t* frame, uint16_t length)
{
  char buf[64];
  ZW_APPLICATION_TX_BUFFER* pCmd = (ZW_APPLICATION_TX_BUFFER*) frame;

  switch (pCmd->ZW_Common.cmd)
  {
  case VERSION_GET:
    {
      ZW_VERSION_REPORT_2ID_V2_FRAME* f = (ZW_VERSION_REPORT_2ID_V2_FRAME*) &txBuf;
      f->cmdClass = COMMAND_CLASS_VERSION;
      f->cmd = VERSION_REPORT;
      f->zWaveLibraryType = ZW_Version((BYTE*) buf);

      f->zWaveProtocolVersion = atoi(buf + 7);
      f->zWaveProtocolSubVersion = atoi(buf + 9);

      Get_SerialAPI_AppVersion(&f->firmware0Version, &f->firmware0SubVersion);

#ifdef __ASIX_C51__
      f->hardwareVersion = 0x1;
      f->noOfFirmwareTargets = 0x3;
      f->firmware1Version = zipr_fw_version_st.major_version;
      f->firmware1SubVersion = zipr_fw_version_st.minor_version;
      gconfig_GetCertVersion(&f->firmware2Version, &f->firmware2SubVersion);
      GwDefaultConfigVersion_Get(&f->firmware3Version, &f->firmware3SubVersion);
#else
      f->noOfFirmwareTargets = 0x3;
      f->hardwareVersion = cfg.hardware_version;
      f->firmware1Version = PACKAGE_VERSION_MAJOR;
      f->firmware1SubVersion = PACKAGE_VERSION_MINOR;
      f->firmware2Version = 1;
      f->firmware2SubVersion = 0;
      f->firmware3Version = 1;
      f->firmware3SubVersion = 0;
#endif
      ZW_SendDataZIP(c, (BYTE*) &txBuf, sizeof(ZW_VERSION_REPORT_2ID_V2_FRAME), NULL);
      break; //VERSION_GET
    }
  case VERSION_COMMAND_CLASS_GET:
    /*If asked non-secre only answer on what we support secure*/
    if (IsCCInNodeInfoSetList(pCmd->ZW_VersionCommandClassGetFrame.requestedCommandClass, scheme_compare(c->scheme ,net_scheme)))
    {
      DBG_PRINTF("Version GET handled by backend 2\n");
      return CLASS_NOT_SUPPORTED;
    }

    txBuf.ZW_VersionCommandClassReportFrame.cmdClass =
    COMMAND_CLASS_VERSION;
    txBuf.ZW_VersionCommandClassReportFrame.cmd =
    VERSION_COMMAND_CLASS_REPORT;
    txBuf.ZW_VersionCommandClassReportFrame.requestedCommandClass =
        pCmd->ZW_VersionCommandClassGetFrame.requestedCommandClass;

    switch (pCmd->ZW_VersionCommandClassGetFrame.requestedCommandClass)
    {
    case COMMAND_CLASS_SECURITY:
      txBuf.ZW_VersionCommandClassReportFrame.commandClassVersion = get_net_scheme() ? SECURITY_VERSION : 0x00;
      break;
    VERSION_CASE_LAN_ONLY(ZIP_ND)
    VERSION_CASE2_LAN_ONLY(ZIP, V3)
    VERSION_CASE2(TRANSPORT_SERVICE, V2)
    VERSION_CASE(SECURITY_2)
    VERSION_CASE(CRC_16_ENCAP)
    VERSION_CASE(APPLICATION_STATUS)
    VERSION_CASE(APPLICATION_CAPABILITY)
    default:
      txBuf.ZW_VersionCommandClassReportFrame.commandClassVersion = ZW_comamnd_handler_version_get(c->scheme,
          pCmd->ZW_VersionCommandClassGetFrame.requestedCommandClass);
      break;
    }
    ZW_SendDataZIP(c, (BYTE*) &txBuf, sizeof(txBuf.ZW_VersionCommandClassReportFrame), NULL);
    break; //VERSION_COMMAND_CLASS_GET

  default:
    return COMMAND_NOT_SUPPORTED;
  }
  return COMMAND_HANDLED;
}

REGISTER_HANDLER(VersionHandler, 0, COMMAND_CLASS_VERSION, VERSION_VERSION_V2, NET_SCHEME);
