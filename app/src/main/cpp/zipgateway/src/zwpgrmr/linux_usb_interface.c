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



/*
 * linux_serial.c
 *
 *  Created on: Nov 28, 2010
 *      Author: esban
 */
//djnakata
#ifdef ANDROID_PLATFORM
#include <stdio.h>
#include <unistd.h>
#include <ZIP_Router_logging.h>

#include "zpg.h"
#include "linux_usb_interface.h"

// The Z-Wave 500-series chip uses a max packet size of 32 bytes
#define USB_MAX_PACKET_SIZE 32

static unsigned char rx_buf[USB_MAX_PACKET_SIZE];

extern int  UsbSerial_APMOpen();
extern void UsbSerial_APMClose();
extern void UsbSerial_APMWriteData(uint8_t *data, int size);
extern int  UsbSerial_APMReadData(uint8_t *data, int len);
extern int  UsbSerial_APMGetBcdDevice();

#define DUMP 0 /* Dump APM communications to debug log if enabled  */
#if DUMP

void hexdump(u8_t *buf, int len) {
  int i=0;
  int j=0;
  for(i=0; i < len; i++) {
    for(j=0; (j < 0xF) && (i+j < len); j++) {
      DBG_PRINTF("%2.2x",buf[i+j]);
    }
    i+=j;
  }
}
#endif

static int xfer(u8_t* buf,u8_t len,u8_t rlen)
{
    u8_t dummy[4] ;
    int actual=0;
    int i;
    int m;

    if(buf)
    {
        for(i=0; i < len ; )
        {
#if DUMP
            DBG_PRINTF("%lu TX:", clock_time());
            hexdump(&buf[i],len);
#endif
            m = len -i;

            UsbSerial_APMWriteData(&buf[i], m);
            i +=m;
        }

        memset(buf,0,rlen);
        memset(rx_buf, 0xaa, sizeof rx_buf);
        actual = 0;
        for(i=0; i < rlen ; ) {
            actual = UsbSerial_APMReadData(rx_buf, USB_MAX_PACKET_SIZE);
            if(actual == 0) {

                break;
            }
#if DUMP
            DBG_PRINTF("%lu RX:", clock_time());
            hexdump(&rx_buf[i],actual);
#endif
            if (actual < 0) {
                /* This probably cannot happen, but better safe than sorry */
                DBG_PRINTF("UsbSerial_APMReadData error, got %i bytes expcted %i\n",i,rlen);
                return i;
            }
            /* Make sure we don't overflow buf by writing more than rlen*/
            if (actual > rlen - i) {
                DBG_PRINTF("Warning: USB read overflow. Discarding %i bytes", actual - (rlen - i));
                actual = rlen - i;
            }
            memcpy(&buf[i], rx_buf, actual);
            i +=actual;
        } /* for (... ) */
        return i;
    } else {
        DBG_PRINTF("sync\n");
        dummy[0]=0;
        UsbSerial_APMWriteData(dummy, 1);
        return 1;
    }
}

static void close_port()
{
    UsbSerial_APMClose();
    sleep(1);
}

static int open_port(zw_pgmr_t* p, const char* port)
{
    if(strncmp("USB",port,3) !=0) {
        return 0;
    }

    if(UsbSerial_APMOpen() != 0)
    {
        return 0;
    }

    p->xfer = xfer;
    p->close = close_port;
    p->open = open_port;
    p->usb = (UsbSerial_APMGetBcdDevice() == 0) ? 1 : 2;

    return 1;
}

const struct zpg_interface linux_usb_interface  = {
    "Linux USB programming interface\n",
    open_port,
    "USB"
};
#else
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>
//#include <errno.h>
#include <sys/time.h>

#include <ctype.h>

#include <libusb.h>

#include "zpg.h"
#include "linux_usb_interface.h"
#include <ZIP_Router.h>

// The Z-Wave 500-series chip uses a max packet size of 32 bytes
#define USB_MAX_PACKET_SIZE 32

libusb_device_handle *dev_handle; //a device handle
libusb_context *ctx = 0; //a libusb session

static unsigned char rx_buf[USB_MAX_PACKET_SIZE];

#define DUMP 0 /* Dump APM communications to debug log if enabled  */
#if DUMP
static FILE *fp;


void hexdump(u8_t *buf, int len) {
  int i=0;
  int j=0;
  for(i=0; i < len; i++) {
    for(j=0; (j < 0xF) && (i+j < len); j++) {
      fprintf(fp, "%2.2x",buf[i+j]);
    }
    i+=j;
  }
  fprintf(fp,"\n");
}
#endif

static int xfer(u8_t* buf,u8_t len,u8_t rlen) {
  u8_t dummy[4] ;
  int actual=0;
  int i;
  int r;
  int m;
  if(buf) {

    for(i=0; i < len ; ) {
#if DUMP
    fprintf(fp, "%lu TX:", clock_time());
    hexdump(&buf[i],len);
#endif
      m = len -i;
/*      if(m > 12) {
        m = 12; //Set a 12 byte limit
      }*/

      r = libusb_bulk_transfer(dev_handle, (2 | LIBUSB_ENDPOINT_OUT), &buf[i], m, &actual, 200);
      i +=actual;
      if(r) {
        perror("out error");
        return 0;
      }
    }
    memset(buf,0,rlen);
    memset(rx_buf, 0xaa, sizeof rx_buf);
    actual = 0;
    for(i=0; i < rlen ; ) {
      r = libusb_bulk_transfer(dev_handle, (2 | LIBUSB_ENDPOINT_IN), rx_buf, USB_MAX_PACKET_SIZE, &actual, 2000);
      if(actual == 0) {
        if(r) {
          printf("libusb error %i\n", r);
        }
        break;
      }
#if DUMP
      fprintf(fp, "%lu RX:", clock_time());
      hexdump(&rx_buf[i],actual);
#endif
      if (actual < 0) {
        /* This probably cannot happen, but better safe than sorry */
        printf("actual usb read was negative - skipping!\n");
        continue;
      }
      /* Make sure we don't overflow buf by writing more than rlen*/
      if (actual > rlen - i) {
        printf("Warning: USB read overflow. Discarding %i bytes", actual - (rlen - i));
        actual = rlen - i;
      }
      memcpy(&buf[i], rx_buf, actual);
      i +=actual;
      if(r) {
        printf("libusb error %i\n", r);
#if DUMP
        fprintf(fp, "libusb error %i\n", r);
#endif
        //perror("in error:);
        printf("got %i bytes expcted %i\n",i,rlen);
        return i;
      }
    } /* for (... ) */
    return i;
  } else {
    printf("sync\n");
    dummy[0]=0;
    r = libusb_bulk_transfer(dev_handle, (2 | LIBUSB_ENDPOINT_OUT), dummy, 1, &actual, 0);
    /*r = libusb_bulk_transfer(dev_handle, (2 | LIBUSB_ENDPOINT_IN), dummy, 4, &actual, 2);*/
    return 1;
  }
}

static  void close_port() {

  int r;
  r = libusb_release_interface(dev_handle, 0); //release the claimed interface
  if (r != 0)
  {
    printf( "Cannot Release Interface\n");
  }
  else
  {
    printf( "Released Interface\n");
  }

  libusb_close(dev_handle); //close the device we opened
  libusb_exit(ctx); //needs to be called to end the
#if DUMP
  fclose(fp);
#endif

  sleep(1);
}

static int open_port(zw_pgmr_t* p, const char* port) {
  struct libusb_device_descriptor desc;
  libusb_device* dev;
  int r;
  if(strncmp("USB",port,3) !=0) {
    return 0;
  }

#if DUMP
  fp = fopen("./prog.log", "a");
  if (!fp)
    ERR_PRINTF("Error opening APM debug log file\n");
#endif
  r = libusb_init(&ctx); //initialize the library for the session we just declared
  if (r < 0)
  {
    printf( "Init Error %i\n" ,r); //there was an error
    return 0;
  }
  libusb_set_debug(ctx, 3); //set verbosity level to 3, as suggested in the documentation

  dev_handle = libusb_open_device_with_vid_pid(ctx, 0x0658, 0x0280); //these are vendorID and productID I found for my usb device
  if (dev_handle == 0) {
    printf( "Cannot open device\n");
    return 0;
  }

  if (libusb_kernel_driver_active(dev_handle, 0) == 1)
  { //find out if kernel driver is attached
    printf( "Kernel Driver Active\n");
    if (libusb_detach_kernel_driver(dev_handle, 0) == 0) //detach it
    {
      printf( "Kernel Driver Detached!\n");
    }
  }
  r = libusb_claim_interface(dev_handle, 0); //claim interface 0 (the first) of device (mine had jsut 1)
  if (r < 0)
  {
    printf( "Cannot Claim Interface\n");
    return 0;
  }

  dev = libusb_get_device(dev_handle);
  libusb_get_device_descriptor(dev,&desc);
  p->xfer = xfer;
  p->close = close_port;
  p->open = open_port;

  /*Check if this is the native programmer or the usb_loader workaround */
  p->usb = desc.bcdDevice == 0 ? 1 : 2;

  printf("Using usb serial driver with %s interface(%x)....\n",desc.bcdDevice == 0 ? "native" : "usb_loader",desc.bcdDevice);

  return 1;
}


const struct zpg_interface linux_usb_interface  = {
    "Linux USB programming interface\n",
    open_port,
    "USB"
};
#endif