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

#include "zpg.h"
#include <string.h>
#include "crc32.h"
#include <unistd.h>
/* Include the USB loader as a binary blob.
   Generated by Makefile.usb_loader */
#include "usb_loader_part1.h"
#include "usb_loader_part2.h"
#include "ZIP_Router_logging.h"

#define USB_WRITE_BURST 3
#define USB_READ_BURST 1   /* Setting to 3 may speed up flash readback. But needs testing. */

/**
 * Perform a build read of SRAM or FLASH
 *
 * return -1 on error
 */
static int
bulk_read(zw_pgmr_t* p, u8_t*buf, u16_t len)
{
  /* Number of commands to burst read */
  int burst = p->usb ? USB_READ_BURST : 1;;
  u8_t t[3 * burst];

  /*Number of triplets to read */
  int left = len/3 +1;

  /*number of commands to use in this cycle.*/
  int n;
  int s =0;

  while(left > 0) {
    n = left < burst ? left : burst;
    if (zpgp_continue_read(p, t, n))
    {
      return -1;
    }
    left -= n;

    memcpy(&buf[s],t, left ? 3*n : len-s);
    s+= 3*n;
  }

  return len;
}

/**
 * Read a single sector from flash.
 */
int
zpg_read_flash_sector(zw_pgmr_t* p, u8_t sector, u8_t*buf)
{
  buf[0] = zpgp_read_flash(p, sector);
  return bulk_read(p, &buf[1], p->blk_sz - 1);
}

/**
 * Read a region of SRAM
 */
int
zpg_bulk_read_SRAM(zw_pgmr_t* p, u16_t addr, u16_t len, u8_t* buf)
{
  buf[0] = zpgp_read_SRAM(p, addr);
  return bulk_read(p, &buf[1], len - 1);
}

/**
 * Write SRAM
 */
int
zpg_bulk_write_SRAM(zw_pgmr_t* p, u16_t addr, u16_t len, const u8_t*buf)
{
  int i, c;

  int burst = p->usb ? USB_WRITE_BURST : 1;
  u8_t t[burst * 3];

  if (zpgp_write_SRAM(p, addr, buf[0]))
  {
    return -1;
  }

  for (i = 1; i < len; i += 3 * burst)
  {
    /* Burst reduce bursts to something smaller*/
    while( (i+3*burst) > len) burst--;

    memcpy(t, &buf[i], 3 * burst);
    if (zpgp_continue_write(p, t, burst))
    {
      return -1;
    }
  }

  c = i - len;

  if (c)
  {
    for (i = len - c; i < len; i++)
    {
      if (zpgp_write_SRAM(p, addr + i, buf[i]))
      {
        return -1;
      }
    }
  }
  return 0;
}

/**
 * The "Read Signature Byte" command reads a signature byte from the chip
 * jedec_id = 7f7f7f71f
 * chip_type = 0x4
 * rev = (x01)
 */
int
read_signature_bytes(zw_pgmr_t* p, struct signature* sig)
{
  u8_t *q = (u8_t*) sig;
  int i, v;
  memset(sig,0,sizeof(struct signature));
  for (i = 0; i < 7; i++)
  {
    int v = zpgp_read_signature_byte(p, i);
    if(v<0) {
      return 0 ;
    }
    *q++ = (v) & 0xFF;
  }

#ifdef ANDROID_PLATFORM
  DBG_PRINTF("Chip signature:\n");
  DBG_PRINTF("  JDEC ID %.2x%.2x%.2x%.2x%.2x\n", sig->jdec_id[0], sig->jdec_id[1],
      sig->jdec_id[2], sig->jdec_id[3], sig->jdec_id[4]);
  DBG_PRINTF("  Type %i revision %i\n", sig->chip_type, sig->chip_revision);
#else
  p->con("Chip signature:\n");
  p->con("  JDEC ID %.2x%.2x%.2x%.2x%.2x\n", sig->jdec_id[0], sig->jdec_id[1],
      sig->jdec_id[2], sig->jdec_id[3], sig->jdec_id[4]);
  p->con("  Type %i revision %i\n", sig->chip_type, sig->chip_revision);
#endif

  v = (sig->chip_type == 4 && sig->jdec_id[0] == 0x7F && sig->jdec_id[1] == 0x7F
      && sig->jdec_id[2] == 0x7F && sig->jdec_id[3] == 0x7F
      && sig->jdec_id[4] == 0x1F);
  if (!v)
  {
#ifdef ANDROID_PLATFORM
    p->err("Bad chip signature\n");
#else
    DBG_PRINTF("Bad chip signature\n");
#endif
  }
  else
  {
    p->blk_sz = 2048; //ZW05xx
    p->blk_num = 64;
  }

  return v;
}

/**
 * Try to enable and sync programming interface.
 *
 * Return true on success
 *
 * Figure 25
 */
int
enable_and_sync_interface(zw_pgmr_t* p)
{
  u8_t cmd[8];
  u8_t rxed, i;

  for (i = 0; i < 3; i++)
  {
    cmd[0] = 0xAC;
    cmd[1] = 0x53;
    cmd[2] = 0xAA;
    cmd[3] = 0x55;

    rxed = p->xfer(cmd, 4, 4);
    if ((rxed >= 2) && cmd[2] == 0xAA && cmd[3] == 0x55)
    {
      return 1; //Sync!
    }
    else if (rxed == 0)
    {
#ifdef ANDROID_PLATFORM
      DBG_PRINTF("No data received during sync\n");
#else
      p->err("No data received during sync\n");
#endif
      break; //No data
    }
    else
    {
      cmd[0] = 0; //Send dummy
      p->xfer(cmd, 1, 1);
    }
  }
#ifndef ANDROID_PLATFORM
  DBG_PRINTF("Unable to sync programming interface\n");
#else
  p->err("Unable to sync programming interface\n");
#endif
  return 0;
}

/**
 * Figure 27, SPI/UART Program Flash Code Space
 *
 * Program a code sector.
 * @param sector the flash sector to be programmed
 * @param start the start offset into data/flash area where we need to program
 * @param pointer to a 2k byte array of data
 * @param end the last address to program.
 *
 */
int
program_flash_code_space(zw_pgmr_t* p, int sector, int start, const u8_t* data, int end)
{
  int len;
  int burst = p->usb ? USB_WRITE_BURST : 1;
  int ntriplets;
  /*Skip leading 0xFF*/
  while (data[start] == 0xFF && start < end)
    start++;

  /*Skip trailing 0xFF */
  while (data[end - 1] == 0xFF && end > start)
    end--;

  /*Nothing to program*/
  if (end <= start)
  {
    //p->con("skipping section\n");
    return 0;
  }

  len = end - start;
#ifdef ANDROID_PLATFORM
  DBG_PRINTF("Programming sector %i bytes %i\r\n", sector, len);
#else
  p->con("Programming sector %i bytes %i\r\n", sector, len);
#endif
  while(1) {
    /*Write the first byte*/
    if(zpgp_write_SRAM(p, start, data[start])) {
      return -1;
    }
    start++;
    ntriplets = (end-start) / 3; //The number of triplets to program

    /*The last triplet must not end with 0xFF, if so single-byte-write the first
     * byte and go another round */
    if(ntriplets>0 && data[start+ntriplets*3-1] == 0xFF) {
      if (zpgp_write_flash_sector(p, sector))
      {
        return -1;
      }
    } else {
      break;
    }
  };


  /* Write triplets */
  while(ntriplets) {
    if( burst > ntriplets) {
      burst = ntriplets;
    }
    if(zpgp_continue_write(p, &data[start], burst)) {
      return -1;
    }
    ntriplets -= burst;
    start+= burst*3;
  }
  /* Write the data just stored in SRAM */
  if (zpgp_write_flash_sector(p, sector))
  {
    return -1;
  }

  /*Flash the last bytes with single commands */
  for ( ; start < end; start++)
  {
    if(data[start] !=0xFF) {
      zpgp_write_SRAM(p, start, data[start]);
      if (zpgp_write_flash_sector(p, sector))
      {
        return -1;
      }
    }
  }
  return 0;
}

/**
 * Initalize the programmer
 */
int
zpg_init(zw_pgmr_t* p)
{
  int i;

  for (i = 0; i < 3; i++)
  {

    if (!enable_and_sync_interface(p))
    {
      return -1;
    }

    if (read_signature_bytes(p, &p->sig))
    {
      break;
    }
    p->xfer(0, 0,1);
  }

  return i == 3 ? -1 : 0;
}

/* Program and boot the USB loader
 * The chip MUST not be in auto programming mode
 * in this state.
 */
static int program_and_boot_usb_loader(zw_pgmr_t* p) {
  int i=0;
  u8_t sector[0x800];

  for(i=0; i < 10; i++) {
    /* Write first part as fast as possible */
    program_flash_code_space(p,0,0,usb_loader_part1,0x500);
    program_flash_code_space(p,0,0x508,usb_loader_part1,0x800);

    zpg_read_flash_sector(p,0,sector);
    if(memcmp(sector,usb_loader_part1,0x800) ==0) {
      break;
    }
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("verify of usb_loader1 failed\n");
#else
    p->err("verify of usb_loader1 failed\n");
#endif
    zpgp_erase_sector(p,0);
  }
  if(i==10) {
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("Fatal error. Failed to load the usb loader. I hope this was not a USB stick :-(");
#else
    p->err("Fatal error. Failed to load the usb loader. I hope this was not a USB stick :-(");
#endif
    zpgp_erase_chip(p);
    return -1;
  }
  /* Program the rest of sector 0 */
  program_flash_code_space(p,0,0x0,usb_loader_part2,0x800);

  /* Clear lock bit 1 to leave APM mode and use the usb loader we just flashed */
  zpgp_set_lock_bits(p, RBAP, ~RBAP_AUTOPROG1);

  zpgp_reset_chip(p);
  return 1;
}

/**
 * Program the CHIP
 * \param data An memory buffer that containt the binray data. It must be atleast the same size at the flash
 * \param size Size of the FLASH
 */
int
zpg_program_chip(zw_pgmr_t* p, u8_t* data, int size)
{
  u8_t nvr[0xFF];
  u8_t lockbits[9];
  int i, r;

  if (size < p->blk_num * p->blk_sz)
  {
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("Data buffer not big enough\n");
#else
    p->err("Data buffer not big enough\n");
#endif
    return -1;
  }

  /* Read NVR and store data */
  for (i = 0x9; i < 0xFF; i++)
  {
    if ((r = zpgp_read_nvr(p, i)) < 0)
    {
      return -1;
    }
    nvr[i] = (u8_t) r;
  }

  /*TODO NVR CRC check*/
  /* Read LockBits and store data */
  for (i = 0; i < 0x9; i++)
  {
    if ((r = zpgp_read_lock_bits(p, i)) < 0)
    {
      return -1;
    }
    lockbits[i] = (u8_t) r;
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("%x: lockbits 0x%x\n",i,lockbits[i]);
#else
    p->con("%x: lockbits 0x%x\n",i,lockbits[i]);
#endif
  }

  /*
   * Erase code space, NVR and Lock bits
   */
  if (zpgp_erase_chip(p) < -1)
  {
    return -1;
  }

  /* Program NVR */
  for (i = 0x9; i < 0xFF; i++)
  {
    if ((r = zpgp_set_nvr(p, i, nvr[i])) < 0)
    {
      return -1;
    }
    if ((r = zpgp_read_nvr(p, i)) != nvr[i])
    {
#ifdef ANDROID_PLATFORM
      DBG_PRINTF("%s nvr verify fail, %x != %x at idx %x \n",__FUNCTION__, r, nvr[i], i);
#else
      p->err("%s nvr verify fail, %x != %x at idx %x \n",__FUNCTION__, r, nvr[i], i);
#endif
      return -2;
    }
  }

  if (p->apm && ! (p->usb == 2))
  {
    /*Clear AutoProg0 Lockbit*/
    zpgp_set_lock_bits(p, RBAP, ~RBAP_AUTOPROG0);
  }

  /* start the usb loader, if this is the "native" usb programmer interface  */
  if(p->usb == 1) {
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("Programming the USB loader\n");
#else
    p->con("Programming the USB loader\n");
#endif
    program_and_boot_usb_loader(p);

    /*Re-init the programming interface*/
    p->close();

    //usleep(2*1000*1000);

    /*if(!p->open(p,"USB")) {
#ifdef ANDROID_PLATFORM
      DBG_PRINTF("Unable to re-connect to usb loader #1\n");
#else
      p->err("Unable to re-connect to usb loader #1\n");
#endif
      return -1;
    }
    if(p->usb!=2 ) {
#ifdef ANDROID_PLATFORM
      DBG_PRINTF("Unable to re-connect to usb loader #2\n");
#else
      p->err("Unable to re-connect to usb loader #2\n");
#endif
      return -1;
    }*/
    while(!p->open(p,"USB") || p->usb!=2 ) {
      p->err("Unable to re-connect to usb loader\n");
      sleep(2);
    }
  }

  if(p->usb) {
    /*In the usb targets we cannot use data erea*/
    memset(&data[0x500],0xff,8);
  }
  /*
   * Calculate Flash Code Space CRC32 value
   */
  insertCRC(data, size);

  /*
   *  Program Flash Code Space, skip sector 0 if this is usb
   */
  for (i = p->usb ? 1 : 0 ; i < size / p->blk_sz; i++)
  {
    program_flash_code_space(p, i,0, &data[ 0x800 * i ], 0x800);
  }

  /*In usb mode we need to go into autoprogramming mode now */
  if(p->usb == 2) {
    u8_t ZW_FLASH_auto_prog_set[] = {0x01, 0x03, 0x00, 0x27, 0xDB};
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("Exiting the USB loader\n");
#else
    p->con("Exiting the USB loader\n");
#endif

    /* Clear AutoProg0 Lockbit */
    zpgp_set_lock_bits(p, RBAP, ~RBAP_AUTOPROG0);
    p->xfer(ZW_FLASH_auto_prog_set,sizeof(ZW_FLASH_auto_prog_set),1);
    zpgp_reset_chip(p);
    p->close();

    /* Wait for chip to reappear in APM mode. Watchdog restart causes one second delay. */
    usleep(1000*1000);

    // p->open(p,"USB"); TINY
    while (!p->open(p,"USB"))
      sleep(1);

    if(p->usb != 1) {
#ifdef ANDROID_PLATFORM
      DBG_PRINTF("device did no start in native programming mode\n");
#else
      p->err("device did no start in native programming mode\n");
#endif
      return -1;
    }
    enable_and_sync_interface(p);

    /*Program the real sector 0*/
    zpgp_erase_sector(p,0);

    /*Program sector 0, leaving out 0x500 - 0x508 */
    program_flash_code_space(p,0,0x0, data,0x500);
    program_flash_code_space(p,0,0x508,data,0x800);
  }


  /**
   * Check CRC32 Checksum
   */
  if (zpgp_run_CRC_check(p) < 0)
  {
    return -1;
  }
  /*CRC Check Done?*/
  while ((r = zpgp_check_state(p)) & CRC_BUSY)
  {
    if (r < 0)
    {
      return -1;
    }
  }
  /* CRC Check Passed? */
  if (r & CRC_FAILED)
  {
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("CRC32 failed!\n");
#else
    p->con("CRC32 failed!\n");
#endif
    return -1;
  } else {
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("CRC32 PASSED\n");
#else
    p->con("CRC32 PASSED\n");
#endif
  }

  //if (p->apm)
  {
    /*Clear AutoProg0-1 Lockbit*/
    lockbits[RBAP] &= ~(RBAP_AUTOPROG1 | RBAP_AUTOPROG0);
  }

  /*  Program Lock Bits */
  for (i = 0; i < 0x9; i++)
  {
    if ((r = zpgp_set_lock_bits(p, i, lockbits[i])) < 0)
    {
      return -1;
    }
  }

  zpgp_reset_chip(p);
  return 0;
}

/**
 * Programmer read chip
 * \param data An memory buffer that containt the binray data. It must be atleast the same size at the flash
 * \param size Size of the FLASH
 * \return the number of bytes actually read
 */
int
zpg_read_chip(zw_pgmr_t* p, u8_t* data, int offset, int length)
{
  int s;
  int n = 0;
  for (s = offset / p->blk_sz; s < (length + offset) / p->blk_sz ; s++)
  {
#ifdef ANDROID_PLATFORM
    DBG_PRINTF("Reading sector %i bytes %i \r\n", s, p->blk_sz);
#else
    p->con("Reading sector %i bytes %i \r\n", s, p->blk_sz);
#endif
    zpg_read_flash_sector(p, s, data + p->blk_sz * n);
    n++;
  }
  return n * p->blk_sz;
}

