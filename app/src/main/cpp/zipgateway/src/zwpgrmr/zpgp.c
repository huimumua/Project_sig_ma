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


#include "zpgp.h"
#include <string.h>
#include <unistd.h>
#include "ZIP_Router_logging.h"

/**
 * Wait for the FLASH FSM to report non busy
 */
static int wait_for_FSM(zw_pgmr_t* p) {
  int i;

  for(i=0; i< 64; i++) {
    if((zpgp_check_state(p) & 0x8) == 0) {
      return 0;
    }
    usleep(200*1000); /* wait 200 ms before checking again */
  }
#ifdef ANDROID_PLATFORM
  DBG_PRINTF("FLASH FSM stuck!\n");
#else
  p->err("FLASH FSM stuck!\n");
#endif
  return -1;
}



int zpgp_enable_interface(zw_pgmr_t* p) {
  u8_t cmd[5];
  int i;
  cmd[0] =0xAC;
  cmd[1] =0x53;
  cmd[2] =0xAA;
  cmd[3] =0x55;

  p->xfer(cmd,4,4);

  for(i=0; i < 4; i++) {
    if(cmd[i] == 0xAC) {
      break;
    }
  }
  return i;
  /*return (cmd[2] ==0xAA && cmd[3] ==0x55 );*/
}


int zpgp_read_flash(zw_pgmr_t* p,u8_t sector) {
  u8_t cmd[4];

  cmd[0] =0x10;
  cmd[1] =sector;
  cmd[2] =0x0;
  cmd[3] =0x0;

  if( (p->xfer(cmd,4,4)!=4) ||
      cmd[0]!=0x10 || cmd[1] != sector || cmd[2]!=0x00 ){
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("%s fail\n",__FUNCTION__);
#else
    p->err("%s fail\n",__FUNCTION__);
#endif
    return -1;
  } else {
    return cmd[3];
  }
}

int zpgp_read_SRAM(zw_pgmr_t* p, u16_t addr) {
  u8_t cmd[4];
  cmd[0] =0x06;
  cmd[1] =(addr >> 8) & 0xff;
  cmd[2] =(addr >> 0) & 0xff;
  cmd[3] =0x0;

  if( (p->xfer(cmd,4,4)!=4) ||
      cmd[0]!=0x06 || cmd[1] != ((addr >> 8) & 0xFF) || cmd[2]!=((addr >> 0) & 0xFF)){
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("%s fail\n",__FUNCTION__);
#else
    p->err("%s fail\n",__FUNCTION__);
#endif
    return -1;
  }

  return cmd[3];
}


int zpgp_continue_read(zw_pgmr_t* p,u8_t* data,u8_t burst) {
  int i;
  u8_t cmd[4*burst];

  for(i=0; i < burst; i++) {
    cmd[4*i+0] =0xA0;
    cmd[4*i+1] =0x0;
    cmd[4*i+2] =0x0;
    cmd[4*i+3] =0x0;
  }

  if( p->xfer(cmd,4*burst,4*burst)!=4*burst )
  {
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("%s fail\n",__FUNCTION__);
#else
    p->err("%s fail\n",__FUNCTION__);
#endif
    return -1;
  } else {
    for(i=0; i < burst; i++) {
      if(cmd[4*i+0] != 0xA0) {
#ifdef ANDROID_PLATFORM
        DBG_PRINTF("%s fail\n",__FUNCTION__);
#else
        p->err("%s fail\n",__FUNCTION__);
#endif
        return -1;
      }
      data[3*i+0] = cmd[4*i+1];
      data[3*i+1] = cmd[4*i+2];
      data[3*i+2] = cmd[4*i+3];
    }
  }
  return 0;
}

int zpgp_write_SRAM(zw_pgmr_t* p, u16_t addr, u8_t data) {
  u8_t cmd[4];

  cmd[0] =0x04;
  cmd[1] =(addr >> 8) & 0xFF;
  cmd[2] =(addr >> 0) & 0xFF;
  cmd[3] =data;

  if( (p->xfer(cmd,4,4)!=4) ||
      cmd[0]!=0x04 || cmd[1] != ((addr >> 8) & 0xFF) || cmd[2]!=((addr >> 0) & 0xFF) || cmd[3] != data){
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("%s fail %.2x %.2x %.2x %.2x\n",__FUNCTION__,cmd[0],cmd[1],cmd[2],cmd[3]);
#else
    p->err("%s fail %.2x %.2x %.2x %.2x\n",__FUNCTION__,cmd[0],cmd[1],cmd[2],cmd[3]);
#endif
    return -1;
  } else {
    return 0;
  }
}


int zpgp_continue_write(zw_pgmr_t* p, const u8_t* data,u8_t burst) {
  u8_t cmd[4*burst];
  int i;

  for(i=0; i < burst; i++) {
    cmd[4*i+0] =0x80;
    cmd[4*i+1] =data[3*i+0];
    cmd[4*i+2] =data[3*i+1];
    cmd[4*i+3] =data[3*i+2];
  }

  if( p->xfer(cmd,4*burst,4*burst)!=4*burst ){
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("%s fail %x %x %x %x\n",__FUNCTION__,cmd[0],cmd[1],cmd[2],cmd[3]);
#else
    p->err("%s fail %x %x %x %x\n",__FUNCTION__,cmd[0],cmd[1],cmd[2],cmd[3]);
#endif
    return -1;
  } else {

    for(i=0; i < burst; i++) {
      if(cmd[4*i+0] !=0x80 ||
          cmd[4*i+1] !=data[3*i+0]||
          cmd[4*i+2] !=data[3*i+1]||
          cmd[4*i+3] !=data[3*i+2]) {
        return -1;
      }
    }

    return 0;
  }
}

int zpgp_erase_chip(zw_pgmr_t* p) {
  u8_t cmd[4];

  cmd[0] =0x0A;
  cmd[1] =0x0;
  cmd[2] =0x0;
  cmd[3] =0x0;

  if( (p->xfer(cmd,4,4) !=4)||
      cmd[0]!=0x0A || cmd[1] != 0 || cmd[2]!=0 || cmd[3] != 0){
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("%s fail\n",__FUNCTION__);
#else
    p->err("%s fail\n",__FUNCTION__);
#endif
    return -1;
  } else {
    wait_for_FSM(p);
    return 0;
  }
}


int zpgp_erase_sector(zw_pgmr_t* p,u8_t sector) {
  u8_t cmd[4];

  cmd[0] =0x0B;
  cmd[1] =sector;
  cmd[2] =0x0;
  cmd[3] =0x0;

  if((p->xfer(cmd,4,4) !=4) ||
      cmd[0]!=0x0B || cmd[1] != sector || cmd[2]!=0 || cmd[3] != 0){
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("%s fail\n",__FUNCTION__);
#else
    p->err("%s fail\n",__FUNCTION__);
#endif
    return -1;
  } else {
    wait_for_FSM(p);
    return 0;
  }
}



int zpgp_write_flash_sector(zw_pgmr_t* p,u8_t sector) {
  u8_t cmd[4];

  cmd[0] =0x20;
  cmd[1] =sector;
  cmd[2] =0x0;
  cmd[3] =0x0;

  if( (p->xfer(cmd,4,4) !=4) ||
      cmd[0]!=0x20 || cmd[1] != sector || cmd[2]!=0 || cmd[3] != 0){
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("%s fail\n",__FUNCTION__);
#else
    p->err("%s fail\n",__FUNCTION__);
#endif
    return -1;
  } else {
    wait_for_FSM(p);
    return 0;
  }
}

int zpgp_check_state(zw_pgmr_t* p) {
  u8_t cmd[4];

  cmd[0] =0x7F;
  cmd[1] =0xFE;
  cmd[2] =0x0;
  cmd[3] =0x0;

  if( (p->xfer(cmd,4,4) !=4) ||
      cmd[0]!=0x7F || cmd[1] != 0xFE || cmd[2]!=0){
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("%s fail\n",__FUNCTION__);
#else
    p->err("%s fail\n",__FUNCTION__);
#endif
    return -1;
  } else {
    return cmd[3];
  }
}


int zpgp_read_signature_byte(zw_pgmr_t* p,u8_t num) {
  u8_t cmd[4];

  cmd[0] =0x30;
  cmd[1] = num;
  cmd[2] =0x0;
  cmd[3] =0x0;

  if( (p->xfer(cmd,4,4) !=4) ||
      cmd[0]!=0x30 || cmd[1] != num || cmd[2]!=0){
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("%s fail\n",__FUNCTION__);
#else
    p->err("%s fail\n",__FUNCTION__);
#endif
    return -1;
  } else {
    return cmd[3];
  }
}


int zpgp_zpgp_disable_EooS_mode(zw_pgmr_t* p) {
  u8_t cmd[4];

  cmd[0] =0xD0;
  cmd[1] =0x0;
  cmd[2] =0x0;
  cmd[3] =0x0;

  if( (p->xfer(cmd,4,4) !=4) ||
      cmd[0]!=0xD0 || cmd[1] != 0 || cmd[2]!=0 || cmd[3]!=0){
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("%s fail\n",__FUNCTION__);
#else
    p->err("%s fail\n",__FUNCTION__);
#endif
    return -1;
  } else {
    return 0;
  }
}


int zpgp_enable_EooS_mode(zw_pgmr_t* p) {
  u8_t cmd[4];

  cmd[0] =0xC0;
  cmd[1] =0x0;
  cmd[2] =0x0;
  cmd[3] =0x0;

  if((p->xfer(cmd,4,4) !=4) ||
      cmd[0]!=0xC0 || cmd[1] != 0 || cmd[2]!=0 || cmd[3]!=0){
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("%s fail\n",__FUNCTION__);
#else
    p->err("%s fail\n",__FUNCTION__);
#endif
    return -1;
  } else {
    return 0;
  }
}


int zpgp_set_lock_bits(zw_pgmr_t* p,lock_byte_t num, u8_t lock_data) {
  u8_t cmd[4];

  cmd[0] =0xF0;
  cmd[1] =num;
  cmd[2] =0x0;
  cmd[3] =lock_data;

  if( (p->xfer(cmd,4,4) !=4) ||
      cmd[0]!=0xF0 || cmd[1] != num || cmd[2]!=0 || cmd[3]!=lock_data){
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("%s fail\n",__FUNCTION__);
#else
    p->err("%s fail\n",__FUNCTION__);
#endif
    return -1;
  } else {
    wait_for_FSM(p);
    return 0;
  }
}

int zpgp_read_lock_bits(zw_pgmr_t* p,lock_byte_t num) {
  u8_t cmd[4];

  cmd[0] =0xF1;
  cmd[1] =num;
  cmd[2] =0x0;
  cmd[3] =0x0;

  if( (p->xfer(cmd,4,4) !=4) ||
      cmd[0]!=0xF1 || cmd[1] != num || cmd[2]!=0){
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("%s fail\n",__FUNCTION__);
#else
    p->err("%s fail\n",__FUNCTION__);
#endif
    return -1;
  } else {
    return cmd[3];
  }
}

int zpgp_set_nvr(zw_pgmr_t* p,u8_t addr, u8_t value) {
  u8_t cmd[4];

  cmd[0] =0xFE;
  cmd[1] =0x0;
  cmd[2] =addr;
  cmd[3] =value;

  if( (p->xfer(cmd,4,4) !=4) ||
      cmd[0]!=0xFE || cmd[1] != 0 || cmd[2]!=addr || cmd[3]!= value){
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("%s fail\n",__FUNCTION__);
#else
    p->err("%s fail\n",__FUNCTION__);
#endif
    return -1;
  } else {
    if (wait_for_FSM(p) >= 0) {
      return cmd[3];
    }
    else {
#ifdef ANDROID_PLATFORM
      DBG_PRINTF("%s non-fatal fail waiting for flash write, continuing...\n",__FUNCTION__);
#else
      p->err("%s non-fatal fail waiting for flash write, continuing...\n",__FUNCTION__);
#endif
      return cmd[3]; /* It is nonfatal for the flash FSM to get stuck, I have been told,
                        so we pretend everything is okay and continue. We have waited long enough
                        for the FLASH write to finish.  */
    }
  }
}

int zpgp_read_nvr(zw_pgmr_t* p,u8_t addr) {
  u8_t cmd[4];

  cmd[0] =0xF2;
  cmd[1] =0x0;
  cmd[2] =addr;
  cmd[3] =0x0;

  if( (p->xfer(cmd,4,4) !=4) ||
      cmd[0]!=0xF2 || cmd[1] != 0 || cmd[2]!=addr){
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("%s fail\n",__FUNCTION__);
#else
    p->err("%s fail\n",__FUNCTION__);
#endif
    return -1;
  } else {
    return cmd[3];
  }
}

/**
 * Run the CRC check procedure. Used to verify
 * that the correct data has been written to the
 * Flash.
 */
int zpgp_run_CRC_check(zw_pgmr_t* p) {
  u8_t cmd[4];

  cmd[0] =0xC3;
  cmd[1] =0x0;
  cmd[2] =0x0;
  cmd[3] =0x0;

  if( (p->xfer(cmd,4,4) !=4) ||
      cmd[0]!=0xC3 || cmd[1] != 0 || cmd[2]!=0x0 || cmd[3]!=0x0){
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("%s fail\n",__FUNCTION__);
#else
    p->err("%s fail\n",__FUNCTION__);
#endif
    return -1;
  } else {
    return cmd[3];
  }
}


/**
 * If the Auto Prog mode register bit is set then
 * the command clears the Auto Prog mode
 * register bit and resets the chip
 */
void zpgp_reset_chip(zw_pgmr_t* p) {
  u8_t cmd[4];

  cmd[0] =0xFF;
  cmd[1] =0xFF;
  cmd[2] =0xFF;
  cmd[3] =0xFF;

  p->xfer(cmd,4,0);
}

