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
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <contiki-net.h>
#include "ZIP_Router.h"
#include "ZW_ZIPApplication.h"
#include "Serialapi.h"
#include "ZW_classcmd_ex.h"
#include "ZW_classcmd.h"

#include "ZIP_Router.h"
#include "NodeCache.h"
#include "uip-ds6.h"
#include "sys/rtimer.h"
#include "ipv46_nat.h"
#include "dev/eeprom.h"
#include "ZW_udp_server.h"
#include "ZW_tcp_client.h"
#include "CC_FirmwareUpdate.h"
#include "eeprom_layout.h"
#include "ZW_SendDataAppl.h"
#include "command_handler.h"

#include "Bridge.h"
#include "DataStore.h"

#define TEMP_EEPROM "/tmp/eeprom.dat123"
#define UPGRADE_ZWAVE_TIMEOUT  500   //5 seconds
#define UPGRADE_UDP_TIMEOUT    300   //3 seconds
#define UPGRADE_START_TIMEOUT  300
#define UPGRADE_TIMER_REPEATS  3 

#define NON_BLOCK_MODE	0x0;
#define BLOCK_MODE	0x1
extern uint8_t
nvm_open();
extern uint8_t
nvm_close();
extern uint8_t
nvm_restore(uint32_t offset, uint8_t *buf, uint8_t length, uint8_t *);
extern uint8_t
nvm_backup(uint32_t offset, uint8_t *buf, uint8_t length, uint8_t *);
extern uint8_t
SupportsCommand_func(uint8_t);
extern const char* linux_conf_eeprom_file;
static u8_t updatemode = BLOCK_MODE;
static u16_t upgradeTimeout = UPGRADE_UDP_TIMEOUT;

void
uip_debug_ipaddr_print(const uip_ipaddr_t *addr);

PROCESS_NAME(serial_api_process);

/*16 bit value*/
enum
{
  ZW_FW_ID = 0, ZIPR_FW_ID = 1, CERT_FW_ID = 2, DEFAULT_CONFIG_ID = 3, ZW_FW_EEPROM_ID = 4
};

/*8 bit value*/
enum
{
  ZW_FW_TARGET_ID = 0,
  ZIPR_FW_TARGET_ID = 1,
  CERTS_TARGET_ID = 2,
  DEFAULT_CONFIG_TARGET_ID = 3,
  ZW_FW_EEPROM_TARGET_ID = 4,
};

#define ZIPR_NO_OF_FWIMGS 4
#define CERT_SCRATCH_PAD_LEN          0x2000  //8KB
static u8_t tmp_cert[CERT_SCRATCH_PAD_LEN];

struct image_descriptor
{
  uint32_t firmware_len;
  uint16_t firmware_id;
  uint16_t crc;
  uint8_t target;
};

typedef struct _cert_header_st
{
  u32_t signature;
  u16_t version;
  u16_t len;
  u16_t zipr_ssl_cert_offset;
  u16_t zipr_ssl_cert_len;
  u16_t zipr_priv_key_offset;
  u16_t zipr_priv_key_len;
  u16_t portal_ca_cert_offset;
  u16_t portal_ca_cert_len;
  u8_t reserved[6];
  u16_t checksum;
} cert_header_st_t;

typedef struct _zw_fw_header_st
{
  u8_t Signature[4];
  u32_t FileLen;
} zw_fw_header_st_t;

#define WAIT_TIME_ZERO				0x0
#define WAIT_TIME_ZIPR_FW_UPDATE    30     //Seconds
#define WAIT_TIME_ZW_FW_UPDATE	    60     //Seconds

#define WAIT_TIME_FW_SSL_UPDATE    240    //Seconds

#define ERROR_INVALID_CHECKSUM_FATAL				0x0
#define ERROR_TIMED_OUT								0x1
#define ERROR_LASTUSED								ERROR_TIMED_OUT
#define UPGRADE_SUCCESSFUL_NO_POWERCYCLE			0xFE
#define UPGRADE_SUCCESSFUL_POWERCYCLE				0xFF

// ZIP Header Len = 7;
// FW meta data report header len  6 (cmd+cmd_class+report0+report1+chksum1+chksum2)

#define FW_UPDATE_MAX_SEGMENT_SIZE     	    500
#define FW_UPDATE_MIN_SEGMENT_SIZE      	40

enum
{
  INVALID_COMBINATION = 0x00,
  OUT_OF_BAND_AUTH_REQUIRED = 0x01,
  EXCEEDS_MAX_FRAGMENT_SIZE = 0x02,
  FIRMWARE_NOT_UPGRADABALE = 0x03,
  INVALID_HARDWARE_VERSION = 0x4,
  //FIRMWARE_UPDATE_IN_PROGESS = 0x05, // This status code is not in the spec

  INITIATE_FWUPDATE = 0xFF,
};

#define MAX_RETRY   3   //(1st time + 2 retries)

/**
 * State machine struct for the EEPROM backup feature
 */
struct
{
  BYTE verison; //The veriosn of the command class which we are currently using
  enum
  {
    FW_FSM_IDLE,
    FW_FSM_PREPARE_IMAGE,
    FW_FSM_SEND_FRAGEMNTS,
    FW_FSM_SEND_LAST_FRAGEMNT,
    FW_FSM_META_DATA_SPACING,
    FW_FSM_WRITE_EEPROM,
  } state;

  zwave_connection_t c;
  uint16_t first_report;
  uint16_t last_report;
  BYTE retry;
  unsigned long image_size;         /* whole eeprom.dat file size */
  int nvm_eeprom_size;  /* size of NVM eeprom excluding the struct zw_eeprom_img_hdr*/
  uint16_t fragment_size; /* Fragment size for DOWNLOAD, upload uses fwmss */

  struct ctimer timer;
} fw_fsm;

static void
FwUpdate_Reset_Var(void);
static void
FwUpdateMdReqReport_SendTo(zwave_connection_t* c, BYTE status);
static void
FwUpdate_StatusReport_Send(BYTE status, WORD waitTimeSec);
static void
FwUpdateDataGet_Send(void);

static void
activate_image(uint16_t firmware_id, uint8_t firmware_target, uint16_t crc, uint8_t activation);

struct zw_eeprom_img_hdr
{
  char zwee[4];
  BYTE zw_lib_type;
  BYTE zw_app_major;
  BYTE zw_app_minor;
  BYTE zw_ver[25];
  unsigned long nvm_eeprom_len;
};

static void
send_fragment();
static void
burn_eeprom(void);
static void
get_eeprom_img_hdr(struct zw_eeprom_img_hdr* hdr);
static void
FwUpdate_Handler(BYTE* pData, WORD bDatalen);
static void
FwUpdate_Timeout(void);
static void
FwStatus_Timeout(void);
static void
FwUpdateActivationReport_SendTo(zwave_connection_t *c, BYTE status);
static void backup_eeprom(void *user);
static void prepare_image(void* user);
void fw_fsm_timeout(void* fsm);
static void send_firmware_prepare_report(zwave_connection_t *c, u8_t status);


#define BLOCK_REQ_NO  255

static u16_t blockno = 0;
static u16_t reqno = 0;

static u8_t upgradeTimer = 0xFF;
static u8_t statusTimer = 0xFF;
static u8_t retrycnt = 0;
static u8_t status_retrycnt = 0;
static struct image_descriptor fw_desc;
uint16_t build_crc;

static uint8_t bActivationRequest;
static u8_t bVersion; //the version which we are currently using

static u16_t fwmss = 0x0;
static zwave_connection_t ups;

extern uint8_t gisZIPRReady;

extern u16_t
chksum(u16_t sum, const u8_t *data, u16_t len);

ZW_FIRMWARE_UPDATE_MD_REQUEST_REPORT_V3_FRAME gfwUpdateMdReqReportV3;
ZW_FIRMWARE_MD_REPORT_2ID_V3_FRAME gfwMdReport2idV3;
ZW_FIRMWARE_UPDATE_MD_GET_V3_FRAME gupdateMdGet;
ZW_FIRMWARE_UPDATE_MD_STATUS_REPORT_V3_FRAME gstatusReport;
#ifndef AXTLS
#include <openssl/md5.h>
uint8_t
MD5_ValidatFwImg(uint32_t baseaddr, uint32_t len) REENTRANT
{
  uint32_t noOf256blks = 0, residue = 0, index = 0, i = 0;
  uint8_t buf[450] =
    { 0 };
  uint8_t digest[16] =
    { 0 };
  MD5_CTX context;

  noOf256blks = len / sizeof(buf);

  residue = len % sizeof(buf);

  MD5_Init(&context);

  for (i = 0; i < noOf256blks; i++)
  {
    eeprom_read(baseaddr + index, buf, sizeof(buf));
    MD5_Update(&context, buf, sizeof(buf));
    index += sizeof(buf);
  }

  if (residue)
  {
    eeprom_read(baseaddr + index, buf, residue);
    MD5_Update(&context, buf, residue);
    index += residue;
#if 0
    printf("residue:  index = 0x%lx residue=0x%lx : \r\n", index, residue);
    for(i=0; i < residue; i++)
    {
      printf("0x%bx ",buf[i]);
    }
    printf("\r\n");
#endif

  }

  MD5_Final(digest, &context);

  //read md5 checksum
  eeprom_read(baseaddr + index, buf, 16);

  //printf("baseaddr = 0x%lx,  len = 0x%lx index = 0x%lx\r\n", baseaddr, len, index);

  printf("calculated checksum : \r\n");
  for (i = 0; i < 16; i++)
  {
    printf("0x%xc ", digest[i]);
  }
  printf("\r\n");

#if 0
  printf("Read index: \r\n");
  for(i=0; i < 16; i++)
  {
    printf("0x%bx ",buf[i]);
  }
  printf("\r\n");
#endif

  if (!memcmp(digest, buf, 16))
  {
    printf("MD5 checksum passed.\r\n");
    return TRUE;
  }
  else
  {
    printf("MD5 checksum failed.\r\n");
    return FALSE;
  }
}
#endif

u8_t
gconfig_ValidatZwFwImg(u32_t baseaddr, u32_t downloadlen, u32_t maxlen)
{
  zw_fw_header_st_t zwfw_header;

  eeprom_read(baseaddr, (u8_t*) &zwfw_header, sizeof(zwfw_header));

  /*Convert to network byte order*/
  zwfw_header.FileLen = uip_htonl(zwfw_header.FileLen);

  printf("gconfig_ValidatZwFwImg: downloadlen = 0x%x headerlen = 0x%x \r\n", downloadlen, zwfw_header.FileLen);

  //printf("MAX_FW_LEN = 0x%lx\r\n", (unsigned long)maxlen);

  if (downloadlen < zwfw_header.FileLen)
  {
    printf("Download len and Fw header len not matched.\r\n");
    return FALSE;
  }
  else if (!((zwfw_header.Signature[0] == 'Z') && (zwfw_header.Signature[1] == 'W') && (zwfw_header.Signature[2] == 'F')
      && (zwfw_header.Signature[3] == 'W')))
  {
    printf("Firmware signature doesn't match.\r\n");
    return FALSE;
  }
  else if ((zwfw_header.FileLen == 0) || (zwfw_header.FileLen > maxlen))
  {
    printf("Invalid firmware len in the header.\r\n");
    return FALSE;
  }
  else
  {
    return MD5_ValidatFwImg(baseaddr, zwfw_header.FileLen);
  }
}

/*
 * Call back function for the firmware meta data get request send
 */
static void
fwupdate_get_callback(BYTE txstatus, void* user, TX_STATUS_TYPE *t)
{

  DBG_PRINTF("fwupdate_get_callback: txstatus = 0x%02x\r\n", txstatus);

  if (nodeOfIP(&ups.ripaddr) || ZW_IsZWAddr(&ups.ripaddr))
  {
    if (txstatus == TRANSMIT_COMPLETE_OK)
    {
      if (upgradeTimer != 0xFF)
      {
        ZW_LTimerCancel(upgradeTimer);
        upgradeTimer = 0xFF;
      }

      upgradeTimer = ZW_LTimerStart(FwUpdate_Timeout, upgradeTimeout,
      UPGRADE_TIMER_REPEATS);
    }
    else
    {
      if (upgradeTimer != 0xFF)
      {
        ZW_LTimerCancel(upgradeTimer);
        upgradeTimer = 0xFF;
      }

      if (retrycnt >= MAX_RETRY)
      {
        FwUpdate_StatusReport_Send(ERROR_TIMED_OUT, WAIT_TIME_ZERO);
        ERR_PRINTF("fwupdate_get_callback: Error timed out\r\n");
      }
      else
      {
        DBG_PRINTF("fwupdate_get_callback: retrycnt = %u\r\n", retrycnt);
        FwUpdateDataGet_Send();
        retrycnt++;
      }
    }
  }
  else
  {
    if (upgradeTimer == 0xFF)
    {
      upgradeTimer = ZW_LTimerStart(FwUpdate_Timeout, upgradeTimeout,
      UPGRADE_TIMER_REPEATS);
      DBG_PRINTF("Starting the upgrade timer.. upgradeTimer = 0x%02x\r\n", upgradeTimer);
    }

  }
}

/*
 * Call back function for the firmware meta data status send
 */

static void
fwupdate_status_callback(BYTE txstatus, void* user, TX_STATUS_TYPE *t)
{

  DBG_PRINTF("fwupdate_status_callback: txstatus = 0x%02x\r\n", txstatus);

  if (nodeOfIP(&ups.ripaddr) || ZW_IsZWAddr(&ups.ripaddr))
  {
    if (txstatus == TRANSMIT_COMPLETE_OK)
    {
      ZW_LTimerCancel(upgradeTimer);
      ZW_LTimerCancel(statusTimer);
      if ((fw_desc.firmware_id == CERT_FW_ID) && (gstatusReport.status >= UPGRADE_SUCCESSFUL_NO_POWERCYCLE))
      {
        FwUpdate_Reset_Var();
        TcpTunnel_ReStart();
      }
      else
      {
        FwUpdate_Reset_Var();
      }
    }
    else
    {
      if (status_retrycnt >= MAX_RETRY)
      {
        FwUpdate_Reset_Var();
        ERR_PRINTF("FwStatus_Timeout: Error timed out\r\n");
      }
      else
      {
        status_retrycnt++;
        //gstatusReport is global status structure..report still be stored here
        ZW_SendDataZIP(&ups, (BYTE*) &gstatusReport, sizeof(ZW_FIRMWARE_UPDATE_MD_STATUS_REPORT_V3_FRAME),
            fwupdate_status_callback);
      }
    }
  }
  else
  {
    if (txstatus == TRANSMIT_COMPLETE_OK)
    {

      ZW_LTimerCancel(upgradeTimer);
      ZW_LTimerCancel(statusTimer);
      if ((fw_desc.firmware_id == CERT_FW_ID) && (gstatusReport.status >= UPGRADE_SUCCESSFUL_NO_POWERCYCLE))
      {
        upgradeTimer = ZW_LTimerStart(TcpTunnel_ReStart, UPGRADE_START_TIMEOUT,
        TIMER_ONE_TIME);
        FwUpdate_Reset_Var();
      }
      else
      {
        FwUpdate_Reset_Var();
      }
    }
    else
    {
      if (statusTimer == 0xFF)
      {
        statusTimer = ZW_LTimerStart(FwStatus_Timeout, upgradeTimeout,
        UPGRADE_TIMER_REPEATS);
      }
    }
  }
}

/*
 *  Firmware update command class handler
 *  Returns TRUE if received command is supported command.
 *  Returns FALSE if received command is non-supported command(e.g. meta data get and firmware update status)
 */
command_handler_codes_t
Fwupdate_Md_CommandHandler(zwave_connection_t *c,uint8_t* pData, uint16_t bDatalen)
{

  ZW_APPLICATION_TX_BUFFER * pCmd = (ZW_APPLICATION_TX_BUFFER*) pData;

  DBG_PRINTF("Fwupdate_Md_CommandHandler: pCmd->ZW_Common.cmd = %02x fw_desc.desc = %u\n", pCmd->ZW_Common.cmd,
      fw_desc.firmware_id);

  switch (pCmd->ZW_Common.cmd)
  {
  case FIRMWARE_MD_GET_V3:
    {
      ZW_FIRMWARE_MD_REPORT_2ID_V3_FRAME *fwMdReport2idV3 = &gfwMdReport2idV3;

      memset(fwMdReport2idV3, 0, sizeof(ZW_FIRMWARE_MD_REPORT_2ID_V3_FRAME));

      fwMdReport2idV3->cmdClass = COMMAND_CLASS_FIRMWARE_UPDATE_MD_V3;
      fwMdReport2idV3->cmd = FIRMWARE_MD_REPORT_V3;

      fwMdReport2idV3->manufacturerId1 = (cfg.manufacturer_id >> 8) & 0xff;
      fwMdReport2idV3->manufacturerId2 = (cfg.manufacturer_id >> 0) & 0xff;

      fwMdReport2idV3->firmwareId1 = 0x00;
      fwMdReport2idV3->firmwareId2 = 0x00;
      fwMdReport2idV3->checksum1 = 0x00;
      fwMdReport2idV3->checksum2 = 0x00;
      fwMdReport2idV3->fwUpgradable = 0xFF;

      fwMdReport2idV3->numberOfFwTargets = ZIPR_NO_OF_FWIMGS;
      if (nodeOfIP(&c->ripaddr) || ZW_IsZWAddr(&c->ripaddr))
      {
        fwMdReport2idV3->maxFragmentSize1 = (FW_UPDATE_MIN_SEGMENT_SIZE >> 8) & 0xFF;
        fwMdReport2idV3->maxFragmentSize2 = FW_UPDATE_MIN_SEGMENT_SIZE & 0xFF;
      }
      else
      {
        fwMdReport2idV3->maxFragmentSize1 = (FW_UPDATE_MAX_SEGMENT_SIZE >> 8) & 0xFF;
        fwMdReport2idV3->maxFragmentSize2 = FW_UPDATE_MAX_SEGMENT_SIZE & 0xFF;
      }
      WORD* fid = (WORD*) &fwMdReport2idV3->firmwareId11;
      *fid++ = UIP_HTONS(ZIPR_FW_ID);
      *fid++ = UIP_HTONS(CERT_FW_ID);
      *fid++ = UIP_HTONS(DEFAULT_CONFIG_ID);
      *fid++ = UIP_HTONS(ZW_FW_EEPROM_ID);
      *((uint8_t*) fid) = cfg.hardware_version;
      ZW_SendDataZIP(c, (BYTE*) fwMdReport2idV3, 12 + 2 * ZIPR_NO_OF_FWIMGS + 1, NULL);

    }
    break;

  case FIRMWARE_UPDATE_MD_REPORT_V3:
    {
      if (fw_desc.firmware_id == ZW_FW_ID || fw_desc.firmware_id == ZW_FW_EEPROM_ID
          || fw_desc.firmware_id == CERT_FW_ID)
      {
        FwUpdate_Handler(pData, bDatalen);
      }
      else
      {
        DBG_PRINTF("FIRMWARE_UPDATE_MD_REPORT_V3: Device is not ready\r\n");
      }
    }
    break;

  case FIRMWARE_UPDATE_ACTIVATION_SET_V4:
    {
      uint8_t hw_version;
      uint16_t manuId;

      WORD manufacturerId = (pCmd->ZW_FirmwareUpdateActivationSetV4Frame.manufacturerId1 << 8)
          | pCmd->ZW_FirmwareUpdateActivationSetV4Frame.manufacturerId2;
      WORD firmwareID = (pCmd->ZW_FirmwareUpdateActivationSetV4Frame.firmwareId1 << 8)
          | pCmd->ZW_FirmwareUpdateActivationSetV4Frame.firmwareId2;
      WORD crc = (pCmd->ZW_FirmwareUpdateActivationSetV4Frame.checksum1 << 8)
          | pCmd->ZW_FirmwareUpdateActivationSetV4Frame.checksum1;

      BYTE fwTarget = pCmd->ZW_FirmwareUpdateActivationSetV4Frame.firmwareTarget;

      hw_version = ((uint8_t*) pCmd)[9];
      if ((bDatalen > 9) && hw_version != cfg.hardware_version)
      {

        FwUpdateActivationReport_SendTo(c, INVALID_COMBINATION);
        return COMMAND_HANDLED;
      }

      if (fw_desc.firmware_id != 0xFFFF)
      {
        FwUpdateActivationReport_SendTo(c, 0x01); //Generic error
        return COMMAND_HANDLED;
      }

      ups = *c;
      activate_image(firmwareID, fwTarget, crc, 1);
      return COMMAND_HANDLED;
    }
    break;

  case FIRMWARE_UPDATE_MD_REQUEST_GET_V4:
    {
      WORD manufacturerId = (pCmd->ZW_FirmwareUpdateMdRequestGetV3Frame.manufacturerId1 << 8)
          | pCmd->ZW_FirmwareUpdateMdRequestGetV3Frame.manufacturerId2;
      WORD firmwareID = (pCmd->ZW_FirmwareUpdateMdRequestGetV3Frame.firmwareId1 << 8)
          | pCmd->ZW_FirmwareUpdateMdRequestGetV3Frame.firmwareId2;
      BYTE fwTarget = pCmd->ZW_FirmwareUpdateMdRequestGetV3Frame.firmwareTarget;
      uint16_t fragmentSize = (pCmd->ZW_FirmwareUpdateMdRequestGetV3Frame.fragmentSize1 << 8)
          | pCmd->ZW_FirmwareUpdateMdRequestGetV3Frame.fragmentSize2;
      uint16_t crc = (pCmd->ZW_FirmwareUpdateMdRequestGetV3Frame.checksum1 << 8)
          | pCmd->ZW_FirmwareUpdateMdRequestGetV3Frame.checksum2;

      uint16_t max_allowed_fragSize;

      if (bDatalen < 8)
      {
        ERR_PRINTF("Parse error FW Upd Req Get\n");
        return COMMAND_PARSE_ERROR;
      }

      //Validate the md request get command
      if (fw_desc.firmware_id != 0xFFFF)
      {
        //firmware upgrade in progress return error from here
        /* We should really be returning FIRMWARE_UPDATE_IN_PROGRESS,
         * but unfortunately it did not make it into the spec */
        FwUpdateMdReqReport_SendTo(c, FIRMWARE_NOT_UPGRADABALE);
        DBG_PRINTF("Firmware/Cert Upgrade in Progress. !!!\r\n");
        return COMMAND_HANDLED;
      }

      bActivationRequest = 0;
      if (bDatalen == 8)
      { //Verison 1 and 2
        bVersion = 2;
        fwTarget = ZW_FW_TARGET_ID;
        firmwareID = ZW_FW_ID;
        fragmentSize = 40;

      }
      if (bDatalen >= 12)
      { //Version 4
        bVersion = 4;

        bActivationRequest = pData[11] & 1;
      }
      if (bDatalen >= 13)
      {
        bVersion = 5;
      }

      if (manufacturerId != cfg.manufacturer_id)
      {
        FwUpdateMdReqReport_SendTo(c, INVALID_COMBINATION);
        return COMMAND_HANDLED;
      }

      if (bVersion >= 5)
      { //Version 5
        if (pData[12] != cfg.hardware_version)
        {
          ERR_PRINTF("Wrong hardware version\n");
          FwUpdateMdReqReport_SendTo(c, INVALID_HARDWARE_VERSION);
          return COMMAND_HANDLED;
        }
      }

      if ((0 != nodeOfIP(&c->ripaddr))
           || ZW_IsZWAddr(&c->ripaddr)) /* TODO: Is ZW_IsZWAddr() even relevant any more? */
                                        /* Or should it be replaced by odeOfIP everywhere? */
       {
         max_allowed_fragSize = FW_UPDATE_MIN_SEGMENT_SIZE;
       }
       else
       {
         max_allowed_fragSize = FW_UPDATE_MAX_SEGMENT_SIZE;
       }
      if ((fragmentSize > max_allowed_fragSize)
          || (fragmentSize == 0))
      {
        //Invalid max segment size
        FwUpdateMdReqReport_SendTo(c, EXCEEDS_MAX_FRAGMENT_SIZE);
        return COMMAND_HANDLED;
      }

      if ((fwTarget == ZIPR_FW_TARGET_ID && firmwareID == ZIPR_FW_ID) || // Check the  ZIPR target ID first
          (fwTarget == DEFAULT_CONFIG_TARGET_ID && firmwareID == DEFAULT_CONFIG_ID))
      {
        FwUpdateMdReqReport_SendTo(c, FIRMWARE_NOT_UPGRADABALE);
        return COMMAND_HANDLED;
      }

      if ((fwTarget == ZW_FW_TARGET_ID && firmwareID == ZW_FW_ID) || // Check the  ZW target ID first
          (fwTarget == ZW_FW_EEPROM_TARGET_ID && firmwareID == ZW_FW_EEPROM_ID) || // Check the target ID first
          (fwTarget == CERTS_TARGET_ID && firmwareID == CERT_FW_ID) // Check the target ID first
          )
      {

        FwUpdate_Reset_Var();
        ups = *c;

        if (nodeOfIP(&c->ripaddr) || ZW_IsZWAddr(&c->ripaddr))
        {
          upgradeTimeout = UPGRADE_ZWAVE_TIMEOUT;
          updatemode = NON_BLOCK_MODE;
        }
        else
        {
          upgradeTimeout = UPGRADE_UDP_TIMEOUT;
          updatemode = BLOCK_MODE;
        }

        fwmss = fragmentSize;

        //Send the request report
        FwUpdateMdReqReport_SendTo(c, INITIATE_FWUPDATE);
        blockno = 1;
        retrycnt = 1;

        fw_desc.firmware_id = firmwareID;
        fw_desc.target = fwTarget;
        fw_desc.crc = crc;
        fw_desc.firmware_len = 0;

        build_crc = CRC_INIT_VALUE;

        //send the firmware update data get command
        FwUpdateDataGet_Send();
        return COMMAND_HANDLED;
      }
      else
      {
        DBG_PRINTF("Invalid combination-1.\r\n");
        FwUpdateMdReqReport_SendTo(c, INVALID_COMBINATION);
      }

      break;
    }

    /* EEPROM BACKUP specific frames below here */

  case FIRMWARE_UPDATE_MD_PREPARE_GET:
    {
      ZW_FIRMWARE_UPDATE_MD_PREPARE_GET_V5_FRAME* f = (ZW_FIRMWARE_UPDATE_MD_PREPARE_GET_V5_FRAME*) pData;
      uint16_t manufactureID;
      uint16_t firmwareID;
      uint16_t fragSize;
      uint16_t max_allowed_fragSize;

      if (bDatalen < sizeof(ZW_FIRMWARE_UPDATE_MD_PREPARE_GET_V5_FRAME))
      {
        return COMMAND_PARSE_ERROR;
      }
      manufactureID = (f->manufacturerId1 << 8) + f->manufacturerId2;
      firmwareID = (f->firmwareId1 << 8) + f->firmwareId2;
      fragSize = (f->fragmentSize1 << 8) + f->fragmentSize2;

      if (fw_fsm.state != FW_FSM_IDLE)
      {
        /* We should really be returning FIRMWARE_UPDATE_IN_PROGRESS,
         * but unfortunately it did not make it into the spec */
        send_firmware_prepare_report(c, FIRMWARE_NOT_UPGRADABALE);
        return COMMAND_HANDLED;
      }

      if ((firmwareID != ZW_FW_EEPROM_ID) || (f->firmwareTarget != ZW_FW_EEPROM_TARGET_ID))
      {
        /* Reject anything except ZW EEPROM target */
        uint8_t returnStatus = INVALID_COMBINATION;
        /* If the firmwareID and target match, but are not ZW_EEPROM,
         * another return code is appropriate */
        if (firmwareID == f->firmwareTarget)
        {
          returnStatus = FIRMWARE_NOT_UPGRADABALE;
        }
        send_firmware_prepare_report(c, returnStatus);
        return COMMAND_HANDLED;
      }

      if (manufactureID != cfg.manufacturer_id)
      {
        send_firmware_prepare_report(c, INVALID_COMBINATION);
        return COMMAND_HANDLED;
      }

      if (f->hardwareVersion != cfg.hardware_version)
      {
        send_firmware_prepare_report(c, INVALID_HARDWARE_VERSION);
        return COMMAND_HANDLED;
      }
      if ((0 != nodeOfIP(&c->ripaddr))
          || ZW_IsZWAddr(&c->ripaddr)) /* TODO: Is ZW_IsZWAddr() even relevant any more? */
                                       /* Or should it be replaced by odeOfIP everywhere? */
      {
        max_allowed_fragSize = FW_UPDATE_MIN_SEGMENT_SIZE;
      }
      else
      {
        max_allowed_fragSize = FW_UPDATE_MAX_SEGMENT_SIZE;
      }
      if ((fragSize) == 0 || (fragSize > max_allowed_fragSize))
      {
        send_firmware_prepare_report(c, EXCEEDS_MAX_FRAGMENT_SIZE);
        return COMMAND_HANDLED;
      }
      fw_fsm.fragment_size = fragSize;

      DBG_PRINTF("Preparing to send the FW EEPROM\n");
      /* Make a copy of the module eeprom into a
       * section of our eeprom file. */

      ZW_SetRFReceiveMode(FALSE);
      fw_fsm.state = FW_FSM_PREPARE_IMAGE;
      fw_fsm.c = *c;
      fw_fsm.nvm_eeprom_size = 0;

      if (SupportsCommand_func(FUNC_ID_NVM_BACKUP_RESTORE))
      {
        backup_eeprom(NULL);
      }
      else
      {
        ctimer_set(&fw_fsm.timer, 1, prepare_image, 0);
      }
      return COMMAND_HANDLED;
    }
    break;

  case FIRMWARE_UPDATE_MD_GET_V3:
    {
      ZW_FIRMWARE_UPDATE_MD_GET_V3_FRAME *f = (ZW_FIRMWARE_UPDATE_MD_GET_V3_FRAME *) pData;

      if (fw_fsm.state != FW_FSM_SEND_FRAGEMNTS || !zwave_connection_compare(c, &fw_fsm.c))
      {
        WRN_PRINTF("Got FIRMWARE_UPDATE_MD_GET for no reason fsm state %x \n", fw_fsm.state);
        return COMMAND_HANDLED;
      }

      fw_fsm.retry = 3;
      /* -1 because first_report is zero based, frame field is one based */
      fw_fsm.first_report = ((f->properties1 << 8) | f->reportNumber2) - 1;
      /* The first firmware fragment MUST be identified by the Report Number value 1 */
      ERR_PRINTF("fs_fsm.first_report : %d \n", (uint16_t)fw_fsm.first_report+1);
      if(((uint16_t)(fw_fsm.first_report + 1)) == 0)
      {
         ERR_PRINTF("Firmware MD update get command parse error\n");
         return COMMAND_PARSE_ERROR;
      }
      fw_fsm.last_report = fw_fsm.first_report + f->numberOfReports;

      ctimer_set(&fw_fsm.timer, 3000, fw_fsm_timeout, &fw_fsm);
      send_fragment();
    }
    break;

  default:
    DBG_PRINTF("This FW update command is not for the GW. cmd = 0x%02x bDatalen = %u \r\n", pCmd->ZW_Common.cmd,
        bDatalen);
    return COMMAND_NOT_SUPPORTED;
    break;
  }

  return COMMAND_HANDLED;
}

/*---------------------------------------------------------------------*/

static void
fw_send_done(BYTE status, void* user, TX_STATUS_TYPE *t);

void
fw_fsm_timeout(void* fsm)
{
  switch (fw_fsm.state)
  {
  case FW_FSM_SEND_FRAGEMNTS:
    ERR_PRINTF("Firmware request Timeout\n");
    fw_fsm.state = FW_FSM_IDLE;
    break;
  case FW_FSM_META_DATA_SPACING:
    fw_fsm.state = FW_FSM_SEND_FRAGEMNTS;
    send_fragment();
    break;
  default:
    break;
  }
}

static void
fw_send_done(BYTE status, void* user, TX_STATUS_TYPE *t)
{
  if (status == TRANSMIT_COMPLETE_OK)
  {
    fw_fsm.first_report++;
    fw_fsm.retry = 3;
  }
  else
  {
    fw_fsm.retry--;
  }

  ctimer_stop(&fw_fsm.timer);
  if (fw_fsm.retry)
  {
    if (fw_fsm.first_report * fw_fsm.fragment_size >= fw_fsm.image_size)
    {
      LOG_PRINTF("Firmware transfer complete\n");
      fw_fsm.state = FW_FSM_IDLE;
    }
    else if (fw_fsm.first_report < fw_fsm.last_report)
    {
      fw_fsm.state = FW_FSM_META_DATA_SPACING;
      fw_fsm.retry = 3;
      ctimer_set(&fw_fsm.timer, 50, fw_fsm_timeout, &fw_fsm);
    }
    else
    {
      fw_fsm.state = FW_FSM_SEND_FRAGEMNTS;
      ctimer_set(&fw_fsm.timer, 3000, fw_fsm_timeout, &fw_fsm);
    }
  }
  else
  {
    ERR_PRINTF("No more retrys! Aboting!\n");
    fw_fsm.state = FW_FSM_IDLE;
  }
}
/**
 * Send a fragment of the EEPROM target contents.
 */
static void
send_fragment()
{
  WORD crc;
  BYTE buffer[sizeof(ZW_FIRMWARE_UPDATE_MD_REPORT_1BYTE_FRAME) - 1 + FW_UPDATE_MAX_SEGMENT_SIZE + 2];
  ZW_FIRMWARE_UPDATE_MD_REPORT_1BYTE_V2_FRAME* f = (ZW_FIRMWARE_UPDATE_MD_REPORT_1BYTE_V2_FRAME*) buffer;

  f->cmdClass = COMMAND_CLASS_FIRMWARE_UPDATE_MD_V3;
  f->cmd = FIRMWARE_UPDATE_MD_REPORT;
  f->properties1 = ((fw_fsm.first_report + 1) >> 8) & 0x7F;
  f->reportNumber2 = (fw_fsm.first_report + 1) & 0xFF;

  int offset = fw_fsm.first_report * fw_fsm.fragment_size;
  unsigned long len = fw_fsm.image_size - offset;
  len = len > fw_fsm.fragment_size ? fw_fsm.fragment_size : len;

  if (offset + fw_fsm.fragment_size >= fw_fsm.image_size)
  {
    f->properties1 |= 0x80; //Mark this as the last fragment
  }

  /* Now reading the whole eeprom file and not only the nvm backup */
  eeprom_read(offset, &f->data1, len);

  /*Calculate checksum and insert it in the end of the frame*/
  crc = ZW_CheckCrc16(CRC_INIT_VALUE, (BYTE*) f, sizeof(ZW_FIRMWARE_UPDATE_MD_REPORT_1BYTE_FRAME) - 1 + len);
  *(&f->data1 + len) = (crc >> 8) & 0xff;
  *(&f->data1 + len + 1) = (crc >> 0) & 0xff;

  DBG_PRINTF("Send fragment offset %x size %lu\n", offset, len);
  ZW_SendDataZIP(&fw_fsm.c, f, len + sizeof(ZW_FIRMWARE_UPDATE_MD_REPORT_1BYTE_FRAME) - 1 + 2, fw_send_done);
}

static void
send_firmware_prepare_report(zwave_connection_t *c, u8_t status)
{
  ZW_FIRMWARE_UPDATE_MD_PREPARE_REPORT_V5_FRAME f;

  f.cmdClass = COMMAND_CLASS_FIRMWARE_UPDATE_MD_V3;
  f.cmd = FIRMWARE_UPDATE_MD_PREPARE_REPORT;

  if(INITIATE_FWUPDATE == status) {
    uint8_t* buf;
    uint16_t crc;
    unsigned long size = 0;
    size = eeprom_size();

    buf = (uint8_t*) malloc(size);
    eeprom_read(0, buf, size);
    crc = ZW_CheckCrc16(CRC_INIT_VALUE, buf, size);
    free(buf);

    f.firmwareChecksum1 = (crc>>8) & 0xFF;
    f.firmwareChecksum2 = (crc>>0) & 0xFF;
  } else {
    f.firmwareChecksum1 = 0;
    f.firmwareChecksum2 = 0;
  }
  f.status = status;
  ZW_SendDataZIP(c, &f, sizeof(f), NULL);
}

static void
post_zip_event_reset(void *user)
{
  process_post(&zip_process, ZIP_EVENT_RESET, 0);
}

/**
 * Restore the EEPROM image to Z-Wave module from a backup using
 * the SerialAPI command FUNC_ID_NVM_BACKUP_RESTORE.
 * This is the preferred method to use if the module supports it.
 * Deprecated method for older modules in burn_eeprom().
 */
/** 
 * New design:
 * Receive full eeprom.dat file to a temp location
 * Replace eeprom.dat with just received version
 * Restore module NVM from eeprom.dat[EXT_FW_START_ADDR]
 */
static void
restore_eeprom()
{
  u8_t buf[64];
  uint8_t ret = 0;
  uint8_t length_written;
  uint8_t len_to_write;
  struct zw_eeprom_img_hdr hdr;
  uint32_t download_len;

  if (nvm_open())
  {
    ERR_PRINTF("nvm_open failed");
    goto error;
  }


  eeprom_read(EXT_FW_START_ADDR, (BYTE*) &hdr, sizeof(struct zw_eeprom_img_hdr));
  download_len = hdr.nvm_eeprom_len; 
  DBG_PRINTF("Size of nvm backup is: %d(0x%x)\n", download_len, download_len);
  /* fw_fsm.nvm_eeprom_size is 0 in the beginning set by the parent function. */
  while (fw_fsm.nvm_eeprom_size < download_len)
  {
    if ((download_len - fw_fsm.nvm_eeprom_size) > sizeof(buf))
    {
      len_to_write = sizeof(buf);
    }
    else
    {
      len_to_write = (download_len - fw_fsm.nvm_eeprom_size);
    }

    DBG_PRINTF("Burning EEPROM address %04x\n", fw_fsm.nvm_eeprom_size);
    eeprom_read(EXT_FW_START_ADDR + sizeof(struct zw_eeprom_img_hdr) + fw_fsm.nvm_eeprom_size, buf, len_to_write);
    ret = nvm_restore(fw_fsm.nvm_eeprom_size, buf, len_to_write, &length_written);
    fw_fsm.nvm_eeprom_size += length_written;
    if (ret)
    {
      break;
    }
  }

  if (ret == 2)
  {
    DBG_PRINTF("EOF reached, size restored: %d (0x%x)\n", fw_fsm.nvm_eeprom_size, fw_fsm.nvm_eeprom_size);
    
  }
  if (nvm_close())
  {
    ERR_PRINTF("nvm_close failed");
    goto error;
  }

  if (ret == 1)
  {
    ERR_PRINTF("Error");
    goto error;
  }

  ZW_WatchDogEnable();
  DBG_PRINTF("ZW FW Download to External EEPROM successful\r\n");
  FwUpdate_StatusReport_Send(UPGRADE_SUCCESSFUL_POWERCYCLE,
      gisZIPRReady == ZIPR_READY ? WAIT_TIME_FW_SSL_UPDATE : WAIT_TIME_ZW_FW_UPDATE);
  fw_fsm.state = FW_FSM_IDLE;
  /* Shutdown the serial API process */
  process_exit(&serial_api_process);

  ctimer_set(&fw_fsm.timer, 5000, post_zip_event_reset, 0);

  return;
  error: FwUpdate_StatusReport_Send(ERROR_TIMED_OUT,
      gisZIPRReady == ZIPR_READY ? WAIT_TIME_FW_SSL_UPDATE : WAIT_TIME_ZW_FW_UPDATE);
  fw_fsm.state = FW_FSM_IDLE;
}

/**
 * Restore the EEPROM image to Z-Wave module from a backup using
 * an index overflow in the SerialAPI command FUNC_ID_MEMORY_PUT_BUFFER.
 * The preferred method to use is restore_eeprom().
 * This is a deprecated method for older modules which don't support restore_eeprom().
 */
static void
burn_eeprom(void)
{
  u8_t buf[64];
  if (fw_fsm.state != FW_FSM_WRITE_EEPROM)
  {
    return;
  }

  if (fw_fsm.nvm_eeprom_size < 0xFFFF)
  {
    DBG_PRINTF("Burning EEPROM address %04x\n", fw_fsm.nvm_eeprom_size);

    eeprom_read(EXT_FW_START_ADDR + sizeof(struct zw_eeprom_img_hdr) + fw_fsm.nvm_eeprom_size, buf, sizeof(buf));
    MemoryPutBuffer(fw_fsm.nvm_eeprom_size, buf, sizeof(buf), burn_eeprom);
    fw_fsm.nvm_eeprom_size += sizeof(buf);
  }
  else
  {
    ZW_WatchDogEnable();
    DBG_PRINTF("ZW FW Download to External EEPROM successful\r\n");
    FwUpdate_StatusReport_Send(UPGRADE_SUCCESSFUL_POWERCYCLE,
        gisZIPRReady == ZIPR_READY ? WAIT_TIME_FW_SSL_UPDATE : WAIT_TIME_ZW_FW_UPDATE);
    fw_fsm.state = FW_FSM_IDLE;
    process_post(&zip_process, ZIP_EVENT_RESET, 0);
  }
}

/**
 * Get information about the firmware of the module
 */
static void
get_eeprom_img_hdr(struct zw_eeprom_img_hdr* hdr)
{

  memset(hdr, 0, sizeof(struct zw_eeprom_img_hdr));
  memcpy(hdr->zwee, "ZWEE", 4);
  hdr->zw_lib_type = ZW_Version(hdr->zw_ver);
  Get_SerialAPI_AppVersion(&hdr->zw_app_major, &hdr->zw_app_minor);
}

/**
 * Read the EEPROM from the module using the EEPROM backup API
 * of newer SerialAPIs. This is the preferred method to use if
 * the module supports it. Deprecated method for old modules in
 * prepare_image().
 */

/**
 * New design:
 * Copy module NVM to eeprom.dat[EXT_FW_START_ADDR]
 * Transfer eeprom.dat containing fw_img_hdr and module NVM using C.C. Firmware 
 */
static void
backup_eeprom(void *user)
{
  const int blk_sz = 64;
  u8_t buf[blk_sz];
  uint8_t ret = 0;
  uint8_t length_read = 0;
  struct zw_eeprom_img_hdr hdr;

  if (fw_fsm.state != FW_FSM_PREPARE_IMAGE)
  {
    return;
  }

  if (fw_fsm.nvm_eeprom_size == 0)
  { /* Do open() only once in the beginning*/
    if (nvm_open())
    {
      ERR_PRINTF("nvm_open failed");
      goto error;
    }
  }
  DBG_PRINTF("Backing up EEPROM address %04x\n", fw_fsm.nvm_eeprom_size);
  ret = nvm_backup(fw_fsm.nvm_eeprom_size, buf, sizeof(buf), &length_read);

  /* This writes to eeprom.dat file*/
  eeprom_write(EXT_FW_START_ADDR + sizeof(struct zw_eeprom_img_hdr) + fw_fsm.nvm_eeprom_size, buf, length_read);
  fw_fsm.nvm_eeprom_size += length_read;

  if (ret)
  {
    goto skip;
  }

  if ((fw_fsm.nvm_eeprom_size < 0xFFFF))
  {
    ctimer_set(&fw_fsm.timer, 1, backup_eeprom, 0);
  }
  else
  {

    if (fw_fsm.nvm_eeprom_size > 0xffffffff) {
        assert(0);
    }
    skip: if (ret == 2)
    {
      ERR_PRINTF("EOF reached, backup size: %d (0x%x)\n", fw_fsm.nvm_eeprom_size, fw_fsm.nvm_eeprom_size);
    }
    get_eeprom_img_hdr(&hdr);
    /* We store the actual nvm eeprom backup in header. So that restore function knows it 
     * in the whole eeprom.dat file we backup */
    hdr.nvm_eeprom_len = fw_fsm.nvm_eeprom_size;
    /* This writes to eeprom.dat file*/
    eeprom_write(EXT_FW_START_ADDR, (BYTE*) &hdr, sizeof(struct zw_eeprom_img_hdr));

    if (nvm_close())
    {
      ERR_PRINTF("nvm_close failed");
      goto error;
    }

    if (ret == 1)
    {
      goto error;
    }

    /* Sending the whole eeprom.dat file, because we just backed up the nvm eeprom on it*/
    fw_fsm.image_size = eeprom_size();

    ERR_PRINTF("eeprom.dat size: %lu\n", fw_fsm.image_size);
    /*Re-enable the radio */
    ZW_SetRFReceiveMode(TRUE);

    DBG_PRINTF("Preparing prepare done\n");

    fw_fsm.state = FW_FSM_SEND_FRAGEMNTS;
    ctimer_set(&fw_fsm.timer, 3000, fw_fsm_timeout, &fw_fsm);
    send_firmware_prepare_report(&fw_fsm.c, INITIATE_FWUPDATE);
  }
  return;
  error:
  /*Re-enable the radio */
  ZW_SetRFReceiveMode(TRUE);
  DBG_PRINTF("Preparing prepare failed\n");
  fw_fsm.state = FW_FSM_IDLE;
  send_firmware_prepare_report(&fw_fsm.c, FIRMWARE_NOT_UPGRADABALE);
}

/*
 *
 * We do an async read of the image so the gateway will keep responding.
 *
 * The preferred way of doing this is with backup_eeprom().
 *
 * Only use this approach if the module serialapi version is too
 * old to support EEPROM backup.
 */
static void
prepare_image(void* user)
{
  const int blk_sz = 64;
  u8_t buf[blk_sz];
  struct zw_eeprom_img_hdr hdr;

  if (fw_fsm.state != FW_FSM_PREPARE_IMAGE)
  {
    return;
  }

  //Write the payload to the ext flash
  MemoryGetBuffer(fw_fsm.nvm_eeprom_size, buf, blk_sz);
  eeprom_write(EXT_FW_START_ADDR + sizeof(struct zw_eeprom_img_hdr) + fw_fsm.nvm_eeprom_size, buf, blk_sz);
  fw_fsm.nvm_eeprom_size += blk_sz;

  if (fw_fsm.nvm_eeprom_size < 0xFFFF)
  {
    ctimer_set(&fw_fsm.timer, 1, prepare_image, 0);
  }
  else
  {
    get_eeprom_img_hdr(&hdr);
    /* We store the actual nvm eeprom backup in header. So that restore function knows it 
     * in the whole eeprom.dat file we backup */
    hdr.nvm_eeprom_len = fw_fsm.nvm_eeprom_size;
    /* This writes to eeprom.dat file*/
    eeprom_write(EXT_FW_START_ADDR, (BYTE*) &hdr, sizeof(struct zw_eeprom_img_hdr));

    fw_fsm.image_size = eeprom_size();
    /*Re-enable the radio */
    ZW_SetRFReceiveMode(TRUE);

    DBG_PRINTF("Preparing prepare done\n");

    fw_fsm.state = FW_FSM_SEND_FRAGEMNTS;
    ctimer_set(&fw_fsm.timer, 3000, fw_fsm_timeout, &fw_fsm);
    send_firmware_prepare_report(&fw_fsm.c, INITIATE_FWUPDATE);
  }
}

/*
 * CRC-16 verification
 */

WORD
ZW_CheckCrc16(WORD crc, BYTE *pDataAddr, unsigned long bDataLen)
{
  BYTE WorkData;
  BYTE bitMask;
  BYTE NewBit;
  //printf("ZW_CheckCrc16: bDataLen = %u\r\n", bDataLen);
  while (bDataLen--)
  {
    WorkData = *pDataAddr++;
    for (bitMask = 0x80; bitMask != 0; bitMask >>= 1)
    {
      /* Align test bit with next bit of the message byte, starting with msb. */
      NewBit = ((WorkData & bitMask) != 0) ^ ((crc & 0x8000) != 0);
      crc <<= 1;
      if (NewBit)
      {
        crc ^= CRC_POLY;
      }
    } /* for (bitMask = 0x80; bitMask != 0; bitMask >>= 1) */
  }
  return crc;
}

/*
 * Firmware update status send timeout handler
 */
static void
FwStatus_Timeout(void)
{

  if (status_retrycnt >= MAX_RETRY)
  {
    FwUpdate_Reset_Var();
    ERR_PRINTF("FwStatus_Timeout: Error timed out\r\n");
  }
  else
  {
    status_retrycnt++;
    ZW_SendDataZIP(&ups, (BYTE*) &gstatusReport, sizeof(ZW_FIRMWARE_UPDATE_MD_STATUS_REPORT_V3_FRAME),
        fwupdate_status_callback);
  }
  return;
}

/*
 * Firmware meta data get timeout handler
 */

static void
FwUpdate_Timeout(void)
{
#ifdef __ASIX_C51__
  DBG_PRINTF("FwUpdate_Timeout: retrycnt = %02x cuurtime = 0x%lu\r\n", retrycnt, (unsigned long)getCurrTime());
#endif
  if (retrycnt >= MAX_RETRY)
  {
    FwUpdate_StatusReport_Send(ERROR_TIMED_OUT, WAIT_TIME_ZERO);
    ERR_PRINTF("FwUpdate_Timeout: Error timed out\r\n");
  }
  else
  {
    DBG_PRINTF("FwUpdate_Timeout: calling re-request blockno = %u\r\n", blockno);
    FwUpdateDataGet_Send();
    retrycnt++;
  }

  return;
}

#ifdef __ASIX_C51__
/*
 * ZIPR Firmware Update handler function
 */
static void FwUpdate_Start(void)
{
  DBG_PRINTF("Going to update the fw using boot loader. Please wait !!!\r\n");
  FwUpdate_Reset_Var();

  GCONFIG_EnableFirmwareUpgrade();
  gudpuc_RebootDevice();

  return;
}
#endif

/*
 * Send Firmware update meta data get request to peer/requested node
 */
static void
FwUpdateDataGet_Send(void)
{
  ZW_FIRMWARE_UPDATE_MD_GET_V3_FRAME *updateMdGet = &gupdateMdGet;

  DBG_PRINTF("FwUpdateDataGet_Send  blockno = %u\r\n", blockno);

  updateMdGet->cmdClass = COMMAND_CLASS_FIRMWARE_UPDATE_MD_V3;
  updateMdGet->cmd = FIRMWARE_UPDATE_MD_GET_V3;

  if (updatemode == BLOCK_MODE)
  {
    updateMdGet->numberOfReports = BLOCK_REQ_NO; //Block request
    reqno = blockno;
  }
  else
  {
    updateMdGet->numberOfReports = 1; //we will send ACK for each report
  }

  updateMdGet->properties1 = (blockno >> 8) & 0x7F;
  updateMdGet->reportNumber2 = blockno & 0xFF;

  ZW_SendDataZIP(&ups, (BYTE*) updateMdGet, sizeof(ZW_FIRMWARE_UPDATE_MD_GET_V3_FRAME), fwupdate_get_callback);

  return;
}

/*
 * Send Firmware update meta data req report to specified destination and port number
 */
void
FwUpdateMdReqReport_SendTo(zwave_connection_t *c, BYTE status)
{
  ZW_FIRMWARE_UPDATE_MD_REQUEST_REPORT_V3_FRAME * fwUpdateMdReqReportV3 = &gfwUpdateMdReqReportV3;

  DBG_PRINTF("FwUpdateMdReqReport_Send with status = %02x\r\n", status);

  fwUpdateMdReqReportV3->cmdClass = COMMAND_CLASS_FIRMWARE_UPDATE_MD_V3;
  fwUpdateMdReqReportV3->cmd = FIRMWARE_UPDATE_MD_REQUEST_REPORT_V3;

  fwUpdateMdReqReportV3->status = status;

  ZW_SendDataZIP(c, (BYTE*) fwUpdateMdReqReportV3, sizeof(ZW_FIRMWARE_UPDATE_MD_REQUEST_REPORT_V3_FRAME), NULL);

  return;
}

/*
 * Send Firmware update activation report
 */
static void
FwUpdateActivationReport_SendTo(zwave_connection_t *c, BYTE status)
{
  ZW_FIRMWARE_UPDATE_ACTIVATION_STATUS_REPORT_V5_FRAME f;

  f.cmdClass = COMMAND_CLASS_FIRMWARE_UPDATE_MD_V4;
  f.cmd = FIRMWARE_UPDATE_ACTIVATION_STATUS_REPORT_V4;
  f.manufacturerId1 = (cfg.manufacturer_id >> 8) & 0xFF;
  f.manufacturerId2 = (cfg.manufacturer_id >> 0) & 0xFF;
  f.firmwareId1 = (fw_desc.firmware_id >> 8) & 0xFF;
  f.firmwareId2 = (fw_desc.firmware_id >> 0) & 0xFF;
  f.checksum1 = (fw_desc.crc >> 8) & 0xff;
  f.checksum2 = (fw_desc.crc >> 0) & 0xff;
  f.firmwareTarget = fw_desc.target;
  f.firmwareUpdateStatus = status;
  f.hardwareVersion = cfg.hardware_version;

  ZW_SendDataZIP(c, (BYTE*) &f, sizeof(ZW_FIRMWARE_UPDATE_ACTIVATION_STATUS_REPORT_V5_FRAME), NULL);

  return;
}

/*
 * Send firmware status report to peer/requested node
 */
void
FwUpdate_StatusReport_Send(BYTE status, WORD waitTimeSec)
{
  ZW_FIRMWARE_UPDATE_MD_STATUS_REPORT_V3_FRAME *statusReport = &gstatusReport;

  statusReport->cmdClass = COMMAND_CLASS_FIRMWARE_UPDATE_MD_V3;
  statusReport->cmd = FIRMWARE_UPDATE_MD_STATUS_REPORT_V3;
  statusReport->status = status;
  statusReport->waittime1 = (waitTimeSec >> 8) & 0xFF;
  statusReport->waittime2 = waitTimeSec & 0xFF;

  status_retrycnt++;

  DBG_PRINTF("FwUpdate_StatusReport_Send with satus = %02x\r\n", status);

  ZW_SendDataZIP(&ups, (BYTE*) statusReport, sizeof(ZW_FIRMWARE_UPDATE_MD_STATUS_REPORT_V3_FRAME),
      fwupdate_status_callback);

  return;
}

static void
FwUpdate_Reset_Var(void)
{
  retrycnt = 0;
  status_retrycnt = 0;
  upgradeTimer = 0xFF;
  statusTimer = 0xFF;
  blockno = 0;
  fw_desc.firmware_id = 0xFFFF;
  fwmss = 0;
  reqno = 0;
}

/**
 * Write data to TEMP_EEPROM file
 *
 * \param index index in to the file
 * \param buf buf to read from 
 * \param dat_len size of data to read
 * 
 * \return 1 on error, 0 on success
 */
int temp_eeprom_write(unsigned long index, uint8_t *buf, unsigned long dat_len)
{
    static int f;
    f = open(TEMP_EEPROM, O_RDWR | O_CREAT, 0644);
    if(f<0) {
      fprintf(stderr, "Error opening temp eeprom file %s\n",TEMP_EEPROM);
      perror("");
      return 1;
    }
    
    lseek(f, index, SEEK_SET);
    if(write(f, buf, dat_len) != dat_len) {
      perror("Write error");
      close(f);
      remove(TEMP_EEPROM);
      return 1;
    }
    close(f);
    return 0;
}
/**
 * Read data from TEMP_EEPROM file
 *
 * \param index index in to the file
 * \param buf buf to read data to 
 * \param dat_len size of data to read
 * 
 * \return 1 on error, 0 on success
 */
int temp_eeprom_read(unsigned long index, uint8_t* buf , unsigned long size)
{
    static int f;
    f = open(TEMP_EEPROM, O_RDWR | O_CREAT, 0644);
    if(f<0) {
      fprintf(stderr, "Error opening temp eeprom file %s\n",TEMP_EEPROM);
      perror("");
      return 1;
    }
 
    lseek(f, index, SEEK_SET);
    if(read(f, buf, size) != size) {
      perror("Read error");
      close(f);
      return 1;
    }
    close(f);
    return 0;
}
unsigned long temp_eeprom_size()
{
    static int f;
    unsigned long length;
    f = open(TEMP_EEPROM, O_RDWR | O_CREAT, 0644);
    if(f<0) {
      fprintf(stderr, "Error opening temp eeprom file %s\n",TEMP_EEPROM);
      perror("");
      return 0;
    }
 
    lseek(f, 0, SEEK_SET);
    length = lseek(f, 0, SEEK_END);
    close(f);
    return length;
}
/*
 * ZIPR Firmware and Z-wave FW meta data reports handler
 */
void
FwUpdate_Handler(BYTE* pData, WORD bDatalen)
{
  ZW_APPLICATION_TX_BUFFER* pCmd = (ZW_APPLICATION_TX_BUFFER*) pData;
#ifdef __ASIX__
  uint16_t payload_len = 0;
#endif
  uint32_t index = 0, dat_len = 0, downloadlen = 0;
  uint16_t calcrc = CRC_INIT_VALUE;
  int status = 0;
  //uint8_t buf[410] = {0};

  if (blockno
      != (((pCmd->ZW_FirmwareUpdateMdReport1byteV3Frame.properties1 & 0x7F) << 8)
          | (pCmd->ZW_FirmwareUpdateMdReport1byteV3Frame.reportNumber2)))
  {
    DBG_PRINTF("Invalid/Duplicate block no = %u\r\n",
        (unsigned )(((pCmd->ZW_FirmwareUpdateMdReport1byteV3Frame.properties1 & 0x7F) << 8)
            | (pCmd->ZW_FirmwareUpdateMdReport1byteV3Frame.reportNumber2)));
    return;
  }

  if ((bDatalen - (sizeof(ZW_FIRMWARE_UPDATE_MD_REPORT_1BYTE_V3_FRAME) - 1)) == 0)
  {
    DBG_PRINTF("Firmware packet with zero len received.\r\n");
    return;
  }

  //Stop the timer
  ZW_LTimerCancel(upgradeTimer);
  upgradeTimer = 0xFF;

  //it is the last block
  if (pCmd->ZW_FirmwareUpdateMdReport1byteV3Frame.properties1 & 0x80)
  {
    DBG_PRINTF("Last byte received. \n");
    dat_len = bDatalen - (sizeof(ZW_FIRMWARE_UPDATE_MD_REPORT_1BYTE_V3_FRAME) - 1);
    if (dat_len > fwmss)
    {
      dat_len = fwmss;
    }
  }
  else
  {
    dat_len = fwmss;
  }

  if (blockno == 1)
  {
    uint8_t block_ok = 0;
    BYTE* p = &(pCmd->ZW_FirmwareUpdateMdReport1byteV3Frame.data1);
    DBG_PRINTF("First byte received.\n");

    if (fw_desc.firmware_id == ZW_FW_ID)
    {
      block_ok = (strncmp((char*) p, "ZWFW", 4) == 0);
    }
    else if (fw_desc.firmware_id == ZW_FW_EEPROM_ID)
    {
        /* As we are backig up 
           the whole eeprom.dat (along with nvm eeprom backup on it) we can do this check
           only after the whole file is received. And this is done after Last Frame received
           in the code below */
        block_ok = 1; //lets just pass the check for now
    }
    else if (fw_desc.firmware_id == CERT_FW_ID)
    {
      block_ok = (strncmp((char*) p, "CERT", 4) == 0);
    }

    if (!block_ok)
    {
      DBG_PRINTF("Wrong FW file received.\r\n");
      FwUpdate_StatusReport_Send(ERROR_INVALID_CHECKSUM_FATAL, WAIT_TIME_ZERO);
      return;
    }
  }

  //Verify the received block
  //includes the command header data and 2 byte crc
  if (ZW_CheckCrc16(calcrc, (BYTE *) pData, dat_len + sizeof(ZW_FIRMWARE_UPDATE_MD_REPORT_1BYTE_V3_FRAME) - 1))
  {
    DBG_PRINTF("Checksum verify failed for the blockno = %u\r\n", blockno);
    if (retrycnt >= MAX_RETRY)
    {
      FwUpdate_StatusReport_Send(ERROR_INVALID_CHECKSUM_FATAL,
      WAIT_TIME_ZERO);
    }
    else
    {
      retrycnt++;
      DBG_PRINTF("Retrying block no = %u\r\n", blockno);
      FwUpdateDataGet_Send();
    }
    return;
  }

  index = (unsigned long) fwmss * (blockno - 1);

  if (fw_desc.firmware_id != ZW_FW_EEPROM_ID) {
    //check the max allowed fw size
    if ((index + dat_len) > MAX_ZW_WITH_MD5_FWLEN)
    {
      DBG_PRINTF("Bigger File received. received len = 0x%lx maxlen = 0x%lx\r\n", (unsigned long )(index + dat_len),
          MAX_ZW_FWLEN);
      FwUpdate_StatusReport_Send(ERROR_INVALID_CHECKSUM_FATAL,
      WAIT_TIME_ZERO);
      return;
    }
  }

  if (fw_desc.firmware_id == ZW_FW_EEPROM_ID) {
    if(temp_eeprom_write(index, &pCmd->ZW_FirmwareUpdateMdReport1byteV3Frame.data1, dat_len)) {
      FwUpdate_StatusReport_Send(ERROR_INVALID_CHECKSUM_FATAL, WAIT_TIME_ZERO);
      return;
    }
  } else {
    //Write the payload to the ext flash
    eeprom_write(EXT_FW_START_ADDR + index, &pCmd->ZW_FirmwareUpdateMdReport1byteV3Frame.data1, dat_len);
  }

  build_crc = ZW_CheckCrc16(build_crc, &pCmd->ZW_FirmwareUpdateMdReport1byteV3Frame.data1, dat_len);

  blockno++;
  retrycnt = 1;

  /* Last frame */
  if (pCmd->ZW_FirmwareUpdateMdReport1byteV3Frame.properties1 & 0x80)
  {
    if (fw_desc.firmware_id == ZW_FW_EEPROM_ID) {
        struct zw_eeprom_img_hdr hdr;
        struct zw_eeprom_img_hdr hdr2;
        uint8_t block_ok = 0;
        unsigned long length_eeprom_dat = 0;

        /* The eeprom.dat received is in temp eeprom file. */
        /* Find the nvm eeprom backup header on that file */
        if(temp_eeprom_read(EXT_FW_START_ADDR, (BYTE*) &hdr2, sizeof(struct zw_eeprom_img_hdr))) {
            goto error;
        }
        get_eeprom_img_hdr(&hdr);
        /* Compare the header structure except the nvm_eeprom_len filed */ 
        block_ok = memcmp(&hdr, &hdr2, sizeof(struct zw_eeprom_img_hdr) - sizeof(unsigned long)) == 0;
        if (!block_ok)
        {
          DBG_PRINTF("Wrong FW file received.\r\n");
          goto error;
        }
        /* Find length of the file */
        length_eeprom_dat = temp_eeprom_size();
        if (!length_eeprom_dat) {
            assert(0);
            goto error;
        }

        /* Write the temporary eeprom file back to main eeprom.dat file */
        unsigned char buf[64];
        unsigned long offs = 0;
        uint8_t data_to_read;
        while (offs < length_eeprom_dat) {
            if ((length_eeprom_dat - offs) < sizeof(buf)) {
                data_to_read = length_eeprom_dat - offs;
            } else {
                data_to_read = sizeof(buf);
            }
            if(temp_eeprom_read(offs, (BYTE*) &buf, data_to_read)) {
                perror("Read error");
                goto error;
            }
            eeprom_write(offs, (uint8_t *)&buf, data_to_read);

            offs += data_to_read;
        }
    }
    //Write the downloaded len to EEPROM
    downloadlen = (index + dat_len);
    fw_desc.firmware_len = downloadlen;

    if (build_crc != fw_desc.crc)
    {
      DBG_PRINTF("Invalid checksum: Download failed.\r\n");
      FwUpdate_StatusReport_Send(ERROR_INVALID_CHECKSUM_FATAL,
      WAIT_TIME_ZERO);
      return;
    }

    eeprom_write(EXT_ZW_FW_DOWNLOAD_DATA_LEN_ADDR, (u8_t*) &fw_desc, sizeof(struct image_descriptor));

    if (bActivationRequest)
    {
      FwUpdate_StatusReport_Send(
      FIRMWARE_UPDATE_MD_STATUS_REPORT_SUCCESSFULLY_WAITING_FOR_ACTIVATION_V4, WAIT_TIME_ZERO);
    }
    else
    {
      activate_image(fw_desc.firmware_id, fw_desc.target, fw_desc.crc, 0);
    }
  }
  else
  {
    if (updatemode == BLOCK_MODE)
    {
      if (((reqno + blockno - 1) % BLOCK_REQ_NO) == 1)
      {
        //DBG_PRINTF("FwUpdateDataGet_Send: called  blockno = %u\r\n", blockno);
        FwUpdateDataGet_Send();
      }
      else if (upgradeTimer == 0xFF)
      {
        upgradeTimer = ZW_LTimerStart(FwUpdate_Timeout, upgradeTimeout,
        UPGRADE_TIMER_REPEATS);
      }
    }
    else
    {
      FwUpdateDataGet_Send();
    }

  }

  return;
error:
  FwUpdate_StatusReport_Send(ERROR_INVALID_CHECKSUM_FATAL, WAIT_TIME_ZERO);
  return;
}

static void
activate_image(uint16_t firmware_id, uint8_t firmware_target, uint16_t crc, uint8_t activation)
{
  struct image_descriptor desc;
  uint8_t status;

  eeprom_read(EXT_ZW_FW_DOWNLOAD_DATA_LEN_ADDR, (u8_t*) &desc, sizeof(struct image_descriptor));

  status = FALSE;
  if (activation)
  {
    if (desc.firmware_id != firmware_id || desc.target != firmware_target || desc.crc != crc)
    {
      FwUpdateActivationReport_SendTo(&ups, INVALID_COMBINATION);
      return;
    }
  }

  if (desc.firmware_id == ZW_FW_EEPROM_ID && fw_fsm.state == FW_FSM_IDLE)
  {
    ZW_SetRFReceiveMode(FALSE);
    fw_fsm.state = FW_FSM_WRITE_EEPROM;
    fw_fsm.nvm_eeprom_size = 0;
    if (SupportsCommand_func(FUNC_ID_NVM_BACKUP_RESTORE))
    {
      restore_eeprom();
    }
    else
    {
      burn_eeprom();
    }
    status = TRUE;
  }
  else if (desc.firmware_id == ZW_FW_ID) //Z-wave firmware
  {
    /* Validate the received image */
    if (gconfig_ValidatZwFwImg(EXT_FW_START_ADDR, desc.firmware_len,
    MAX_ZW_FWLEN))
    {
      status = ZWFirmwareUpdate(TRUE);
    }
    else
    {
      status = FALSE;
    }
  }
  else if (desc.firmware_id == CERT_FW_ID)
  {
    FILE* f;
    u16_t crc;

    eeprom_read(EXT_FW_START_ADDR, tmp_cert, desc.firmware_len);

    cert_header_st_t* h = (cert_header_st_t*) tmp_cert;
    crc = chksum(0, tmp_cert, uip_ntohs(h->len));
    if (crc == 0xFFFF)
    {
      status = FALSE;
      if ((f = fopen(cfg.ca_cert, "w")))
      {
        fwrite(&tmp_cert[uip_ntohs(h->portal_ca_cert_offset)],
        uip_ntohs(h->portal_ca_cert_len), 1, f);
        fclose(f);

        if ((f = fopen(cfg.cert, "w")))
        {
          fwrite(&tmp_cert[uip_ntohs(h->zipr_ssl_cert_offset)],
          uip_ntohs(h->zipr_ssl_cert_len), 1, f);
          fclose(f);

          if ((f = fopen(cfg.priv_key, "w")))
          {
            fwrite(&tmp_cert[uip_ntohs(h->zipr_priv_key_offset)],
            uip_ntohs(h->zipr_priv_key_len), 1, f);
            fclose(f);
            status = TRUE;
          }
        }
      }
    }

  }

  if (activation)
  {
    FwUpdateActivationReport_SendTo(&ups, status ? UPGRADE_SUCCESSFUL_POWERCYCLE : 0x1);
  }
  else
  {
    if (status)
    {
      FwUpdate_StatusReport_Send(UPGRADE_SUCCESSFUL_POWERCYCLE,
          gisZIPRReady == ZIPR_READY ? WAIT_TIME_FW_SSL_UPDATE : WAIT_TIME_ZW_FW_UPDATE);
    }
    else
    {
      FwUpdate_StatusReport_Send((status + ERROR_LASTUSED), WAIT_TIME_ZERO);
    }
  }

}

void
Fwupdate_MD_init()
{
  FwUpdate_Reset_Var();
  fw_fsm.state = FW_FSM_IDLE;
}


REGISTER_HANDLER(
    Fwupdate_Md_CommandHandler,
    Fwupdate_MD_init,
    COMMAND_CLASS_FIRMWARE_UPDATE_MD, FIRMWARE_UPDATE_MD_VERSION_V5, SECURITY_SCHEME_0);

