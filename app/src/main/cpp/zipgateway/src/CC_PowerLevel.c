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
 * CC_PowerLevel.c
 *
 *  Created on: Nov 9, 2016
 *      Author: aes
 */

#include "Serialapi.h"
#include "command_handler.h"

clock_time_t  powerLevelTimerEnd;
static struct ctimer powerLevelTimer;

/*State of the powerlevel test*/
static struct
{
  BYTE node;
  BYTE powerLevel;
  WORD count;
  WORD ack;
  BYTE status;
} powerlevel_test;

static void
powerLevelTest(BYTE txStatus)
{
  powerlevel_test.count--;

  if (txStatus == TRANSMIT_COMPLETE_OK)
  {
    powerlevel_test.ack++;
  }

  if (powerlevel_test.count > 0 && ZW_SendTestFrame(powerlevel_test.node, powerlevel_test.powerLevel, powerLevelTest))
  {
    return;
  }

  powerlevel_test.status =
      ((powerlevel_test.count) > 0 || (powerlevel_test.ack == 0)) ?
          POWERLEVEL_TEST_NODE_REPORT_ZW_TEST_FAILED : POWERLEVEL_TEST_NODE_REPORT_ZW_TEST_SUCCES;

  powerlevel_test.count = 0;
  ZW_RFPowerLevelSet(normalPower);
}

static void
resetPowerLevel(void* data)
{
  ZW_RFPowerLevelSet(normalPower);
  powerLevelTimerEnd = 0;
}

extern   ZW_APPLICATION_TX_BUFFER txBuf;

static command_handler_codes_t
PowerLevelHandler(zwave_connection_t *c, uint8_t* frame, uint16_t length)
{
  ZW_APPLICATION_TX_BUFFER* pCmd = (ZW_APPLICATION_TX_BUFFER*) frame;

  switch (pCmd->ZW_Common.cmd)
  {
  case POWERLEVEL_SET:
    if (pCmd->ZW_PowerlevelSetFrame.powerLevel > miniumPower)
    {
      break;
    }

    if (pCmd->ZW_PowerlevelSetFrame.powerLevel == normalPower)
    {
      ctimer_stop(&powerLevelTimer);
      resetPowerLevel(0);
    }
    else
    {
      if (pCmd->ZW_PowerlevelSetFrame.timeout == 0)
      {
        break;
      }
      ZW_RFPowerLevelSet(pCmd->ZW_PowerlevelSetFrame.powerLevel);
      ctimer_set(&powerLevelTimer, pCmd->ZW_PowerlevelSetFrame.timeout * 1000UL, resetPowerLevel, 0);

      powerLevelTimerEnd = clock_time() + pCmd->ZW_PowerlevelSetFrame.timeout * 1000UL;
    }
    break;
  case POWERLEVEL_GET:
    txBuf.ZW_PowerlevelReportFrame.cmdClass = COMMAND_CLASS_POWERLEVEL;
    txBuf.ZW_PowerlevelReportFrame.cmd = POWERLEVEL_REPORT;
    txBuf.ZW_PowerlevelReportFrame.powerLevel = ZW_RFPowerLevelGet();
    txBuf.ZW_PowerlevelReportFrame.timeout = powerLevelTimerEnd > 0 ? ((powerLevelTimerEnd - clock_time()) / 1000) : 0;

    ZW_SendDataZIP(c, (BYTE*) &txBuf, sizeof(txBuf.ZW_PowerlevelReportFrame), NULL);
    break;
  case POWERLEVEL_TEST_NODE_SET:

    if (powerlevel_test.count == 0)
    {
      if (pCmd->ZW_PowerlevelTestNodeSetFrame.powerLevel > miniumPower)
      {
        break;
      }
      powerlevel_test.node = pCmd->ZW_PowerlevelTestNodeSetFrame.testNodeid;
      powerlevel_test.powerLevel = pCmd->ZW_PowerlevelTestNodeSetFrame.powerLevel;
      powerlevel_test.count = pCmd->ZW_PowerlevelTestNodeSetFrame.testFrameCount1 << 8
          | pCmd->ZW_PowerlevelTestNodeSetFrame.testFrameCount2;
      powerlevel_test.ack = 0;
      if (powerlevel_test.count == 0)
      {
        break;
      }
      powerlevel_test.status = POWERLEVEL_TEST_NODE_REPORT_ZW_TEST_INPROGRESS;
      ZW_RFPowerLevelSet(powerlevel_test.powerLevel);
      if (ZW_SendTestFrame(powerlevel_test.node, powerlevel_test.powerLevel, powerLevelTest))
      {
        //
      }
      else
      {
        powerlevel_test.count = 0;
        powerlevel_test.status = POWERLEVEL_TEST_NODE_REPORT_ZW_TEST_FAILED;
        ZW_RFPowerLevelSet(normalPower);
      }
    }
    break;
  case POWERLEVEL_TEST_NODE_GET:
    txBuf.ZW_PowerlevelTestNodeReportFrame.cmdClass = COMMAND_CLASS_POWERLEVEL;
    txBuf.ZW_PowerlevelTestNodeReportFrame.cmd = POWERLEVEL_TEST_NODE_REPORT;
    txBuf.ZW_PowerlevelTestNodeReportFrame.testNodeid = powerlevel_test.node;
    txBuf.ZW_PowerlevelTestNodeReportFrame.statusOfOperation = powerlevel_test.status;
    txBuf.ZW_PowerlevelTestNodeReportFrame.testFrameCount1 = (powerlevel_test.ack >> 8) & 0xFF;
    txBuf.ZW_PowerlevelTestNodeReportFrame.testFrameCount2 = (powerlevel_test.ack >> 0) & 0xFF;

    ZW_SendDataZIP(c, (BYTE*) &txBuf, sizeof(txBuf.ZW_PowerlevelTestNodeReportFrame), NULL);
    break;
  default:
    return COMMAND_NOT_SUPPORTED;;
  }
  return COMMAND_HANDLED;

}

static void PowerLevelInit()
{
  memset(&powerlevel_test,0,sizeof(powerlevel_test));
  powerLevelTimerEnd = 0;
  ctimer_stop(&powerLevelTimer);
}


REGISTER_HANDLER(
    PowerLevelHandler,
    PowerLevelInit,
    COMMAND_CLASS_POWERLEVEL, POWERLEVEL_VERSION, NET_SCHEME);
