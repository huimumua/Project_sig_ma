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
//djnakata
#ifdef ANDROID_PLATFORM
#include <stdio.h>
#include <unistd.h>
#include <ZIP_Router_logging.h>

#include "zpg.h"
#include "linux_serial_interface.h"

extern int  UsbSerial_Open();
extern void UsbSerial_Close();
extern void UsbSerial_WriteData(uint8_t *data, int size);
extern int  UsbSerial_ReadData(uint8_t *data, int len);
extern int  UsbSerial_Check();

static int OpenSerialPort(const char *bsdPath)
{
    if(UsbSerial_Open() != 0)
    {
        return 0;
    }

    return 1;
}

int xfer(u8_t* buf,u8_t len,u8_t rlen)
{
    u8_t null =0 ;
    u8_t r;

    if(buf)
    {
        UsbSerial_WriteData(buf, len);
        //tcdrain(fd);
        //printf("TX %2.2x %2.2x %2.2x %2.2x\n",buf[0],buf[1],buf[2],buf[3]);

        r=0;

        while(r < rlen)
        {
            r+= UsbSerial_ReadData(&buf[r], rlen-r );
            usleep(100);
        }

        if(rlen ==  r )  {
            //printf("RX %2.2x %2.2x %2.2x %2.2x\n",buf[0],buf[1],buf[2],buf[3]);
            return r;
        } else {
            LOG_PRINTF("Read too short len = %i\n",r);
            return r;
        }
    } else {
        UsbSerial_WriteData(&null,1);
        return 1;
    }
}

void close_port(void)
{
    UsbSerial_Close();
}

int open_port(zw_pgmr_t* p, const char* dev_string)
{
    if(OpenSerialPort(dev_string) == 0)
    {
        return 0;
    }

    p->xfer = xfer;
    p->close = close_port;
    p->open = open_port;
    p->usb = 0;

    return 1;
}


const struct zpg_interface linux_serial_interface  = {
        "Linux Serial programming interface\n",
        open_port,
        "/dev/ttyXXX"
};
#else
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include<string.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "zpg.h"
#include "linux_serial_interface.h"

#define error_message printf
int fd;
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/file.h>

// Given the path to a serial device, open the device and configure it.
// Return the file descriptor associated with the device.
static int OpenSerialPort(const char *bsdPath)
{
    int                         fileDescriptor = -1;
    struct termios      options;

    // Open the serial port read/write, with no controlling terminal, and don't wait for a connection.
    // The O_NONBLOCK flag also causes subsequent I/O on the device to be non-blocking.
    // See open(2) ("man 2 open") for details.

    fileDescriptor = open(bsdPath, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fileDescriptor == -1 || flock(fileDescriptor, LOCK_EX) == -1 )
    {
        printf("Error opening serial port %s - %s(%d).\n",
               bsdPath, strerror(errno), errno);
        goto error;
    }
    // Now that the device is open, clear the O_NONBLOCK flag so subsequent I/O will block.
    // See fcntl(2) ("man 2 fcntl") for details.
    if (fcntl(fileDescriptor, F_SETFL, 0) == -1)
    {
        printf("Error clearing O_NONBLOCK %s - %s(%d).\n",
            bsdPath, strerror(errno), errno);
        goto error;
    }

    if (ioctl(fileDescriptor, TIOCEXCL, (char *) 0) < 0) {
      printf("Error setting TIOCEXCL %s - %s(%d).\n",
          bsdPath, strerror(errno), errno);
      goto error;
    }
    memset(&options,0,sizeof(options));
    // The baud rate, word length, and handshake options can be set as follows:
    options.c_iflag=0;
    options.c_oflag=0;


    options.c_cflag=CS8|CREAD|CLOCAL|CSTOPB;           // 8n2, see termios.h for more information
    options.c_lflag=0;
    options.c_cc[VMIN]=1;
    options.c_cc[VTIME]=100;
    cfsetospeed(&options, B115200);            // Set 115200 baud
    cfsetispeed(&options, B115200);

    tcflush(fileDescriptor, TCIFLUSH);
    //cfmakeraw(&options);

    // Cause the new options to take effect immediately.
    if (tcsetattr(fileDescriptor, TCSANOW, &options) == -1)
    {
        printf("Error setting tty attributes %s - %s(%d).\n",
            bsdPath, strerror(errno), errno);
        goto error;
    }

      // Success
    return fileDescriptor;

    // Failure path
error:
    if (fileDescriptor != -1)
    {
        close(fileDescriptor);
    }

    return -1;
}


void
set_blocking(int fd, int should_block)
{
  struct termios tty;
  memset(&tty, 0, sizeof tty);
  if (tcgetattr(fd, &tty) != 0)
  {
    error_message("error %d from tggetattr", errno);
    return;
  }

  tty.c_cc[VMIN] = should_block ? 1 : 0;
  tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

  if (tcsetattr(fd, TCSANOW, &tty) != 0)
    error_message("error %d setting term attributes", errno);
}


int xfer(u8_t* buf,u8_t len,u8_t rlen) {
  u8_t null =0 ;
  u8_t r;
  int i;
  if(buf) {
    i=write(fd, buf, len);
    //tcdrain(fd);
    //printf("TX %2.2x %2.2x %2.2x %2.2x\n",buf[0],buf[1],buf[2],buf[3]);

    r=0;
    i=0;
    while((r < rlen) && (i < 3)) {
      r+= read(fd, &buf[r], rlen-r );
      i++;
    }

    if(rlen ==  r )  {
      //printf("RX %2.2x %2.2x %2.2x %2.2x\n",buf[0],buf[1],buf[2],buf[3]);
      return r;
    } else {
      printf("Read too short len = %i\n",r);
      return r;
    }
  } else {
    i=write(fd,&null,1);
    tcdrain(fd);
    return 1;
  }
}

void close_port(void) {
  flock(fd, LOCK_UN);
  close(fd);
}

int open_port(zw_pgmr_t* p, const char* dev_string) {
  struct stat s;

  if(stat(dev_string,&s) == -1) {
    return 0;
  }

  if(!S_ISCHR(s.st_mode)) {
    return 0;
  }

  fd = OpenSerialPort(dev_string);
  set_blocking(fd, 0); // set no blocking

  p->xfer = xfer;
  p->close = close_port;
  p->open = open_port;
  p->usb = 0;
  return 1;
}


const struct zpg_interface linux_serial_interface  = {
    "Linux Serial programming interface\n",
    open_port,
    "/dev/ttyXXX"
};
#endif

#if 0
void hexdump(u8_t *buf, int len) {
  int i=0;

  for(i=0; i < len; i++) {
    if((i & 0xF) == 0x0) printf("\n %4.4x: ",i);
    printf("%2.2x",buf[i]);
  }
  printf("\n");
}


void sram_read_write_test(zw_pgmr_t* p) {
  int i=0;
  unsigned char buf1[2048];	
  unsigned char buf2[2048];	
  
  for(i=0; i < sizeof(buf1); i++) {
    buf1[i] = i & 0xFF;
  }
  bulk_write_SRAM(p,0, sizeof(buf1), buf1 );
  printf("Buffer written\n");
  bulk_read_SRAM(p,0, sizeof(buf1), buf2 );
  printf("Buffer read\n");
  if(memcmp(buf1,buf2,sizeof(buf1))==0 ) {
    printf("SRAM test passed\n");
  } else {
    printf("SRAM test FAILED\n");
    hexdump(buf2,sizeof(buf2));
  }
}
#endif

#if 0
void flash_read_write_test(zw_pgmr_t* p) {
  int i=0;
  unsigned char buf1[2048];
  unsigned char buf2[2048];

  for(i=0; i < sizeof(buf1); i++) {
    buf1[i] = (i & 0xFF);
  }
  erase_sector(p,0);

  program_flash_code_space(p,0,buf1);

  read_flash_sector(p,0,buf2);
  printf("Buffer read\n");
  if(memcmp(buf1,buf2,sizeof(buf1))==0 ) {
    printf("FLASH test passed\n");
  } else {
    printf("FLASH test FAILED\n");
    hexdump(buf2,sizeof(buf2));
  }
}
#endif

#if 0
int
main(int argc, char** argv)
{
  int i;
  u8_t flash[2048*64];
  FILE* f;

  char *portname = argv[1];
  char *filename = argv[2];

  memset(flash,0xFF,sizeof(flash));



  zw_pgmr_t pgmr = {xfer,printf,printf};
  /**
   * Enable serial programming after reset has been low t_PE,
   */
  if(enable_interface(&pgmr)) {
    printf("Programming interface enabled\n");
  }



  programmer_init(&pgmr);

  /*
  for(i=0; i < 64; i++) {
    printf("Read sector %i\r",i);
    read_flash_sector(&pgmr,i,&flash[i*2048]);
  }
  printf("\n");
  f = fopen(filename,"wb");
  fwrite(flash,sizeof(flash),1,f);
  fclose(f);*/

  f = fopen(filename,"r");
  fread(flash,sizeof(flash),1,f);
  fclose(f);

  programmer_program_chip(&pgmr,flash,sizeof(flash));


 // sram_read_write_test(&pgmr);
  //flash_read_write_test(&pgmr);;
  //read_flash_sector(&pgmr,0,sector);
  //hexdump(sector,sizeof(sector));

}
#endif
