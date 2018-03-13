/*
 * linux_serial.c
 *
 *  Created on: Nov 28, 2010
 *      Author: esban
 */
//djnakata
#if ANDROID_PLATFORM
#include <stdio.h>
#include <unistd.h>

extern int  UsbSerial_Destroy();
extern int  UsbSerial_Open();
extern void UsbSerial_Close();
extern void UsbSerial_WriteData(uint8_t *data, int size);
extern int  UsbSerial_ReadData(uint8_t *data, int len);
extern int  UsbSerial_Check();

int SerialGetBuffer(unsigned char* c, int len);

int SerialInit(const char* port)
{
    if(UsbSerial_Open() != 0)
    {
        return 0;
    }

    return 1;
}

void SerialClose()
{
    UsbSerial_Close();
}

void SerialDestroy()
{
    UsbSerial_Close();

    UsbSerial_Destroy();
}

int SerialGetByte()
{
    unsigned char c;
    SerialGetBuffer(&c,1);
    return c;
}

void SerialPutBuffer(unsigned char* c, int len)
{
    UsbSerial_WriteData(c, len);
}

void SerialPutByte(unsigned char c)
{
    SerialPutBuffer(&c, 1);
}

int SerialGetBuffer(unsigned char* c, int len)
{
    int n, k;
    k = 0;

    if(c == NULL)
    {
        return 0;
    }

    while(k < len)
    {
        n = UsbSerial_ReadData(c+k, len-k);

        if(n == 0)
        {
            continue;
        }

        k = k+n;
    }

    return k;
}

int SerialCheck()
{
    usleep(50);

    if(UsbSerial_Check() <= 0)
        return 0;

    return 1;
}

void SerialFlush()
{
}
#else
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/clock.h>
#include "port.h"
#include "ZIP_Router.h"
#include <sys/file.h>

int serial_fd;
//#define SERIAL_LOG

#ifdef SERIAL_LOG
FILE* log_fd=0;
static int log_dir = 0;;
#endif

clock_time_t last_time;
// Given the path to a serial device, open the device and configure it.
// Return the file descriptor associated with the device.
static int OpenSerialPort(const char *bsdPath)
{
    int				fileDescriptor = -1;
    struct termios	options;

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
    options.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
    options.c_lflag=0;
    options.c_cc[VMIN]=1;
    options.c_cc[VTIME]=5;
    cfsetospeed(&options, B115200);            // Set 115200 baud
    cfsetispeed(&options, B115200);

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
    exit(1);
    return -1;
}


int SerialInit(const char* port) {
    serial_fd = OpenSerialPort(port);
    tcflush(serial_fd,TCIOFLUSH);
#ifdef SERIAL_LOG
    if(log_fd==0 && cfg.serial_log) {
      log_fd = fopen(cfg.serial_log,"w");
    }
#endif
    return serial_fd>0;
}

void SerialClose() {
        flock(serial_fd, LOCK_UN);
	close(serial_fd);

}

void SerialDestroy() {
  flock(serial_fd, LOCK_UN);
  close(serial_fd);
#ifdef SERIAL_LOG
  if(log_fd) fclose(log_fd);
#endif
}

int SerialGetByte() {
	unsigned char c;
	SerialGetBuffer(&c,1);
	return c;
}

void SerialPutByte(unsigned char c) {
  SerialPutBuffer(&c,1);
}



int SerialGetBuffer(unsigned char* c, int len) {

  int n,i,k;
  k=0;

  while(k < len) {
    n = read(serial_fd,c+k,len-k);
    if(n <= 0) {
        perror("Serial Read Error\n");
        exit(1);
        return n;
    }
    k = k+n;
  }

#ifdef SERIAL_LOG
  if(log_fd){
    if(log_dir==1 || ((clock_time()-last_time) > 200)) {
      fprintf(log_fd,"\n%lu R ", clock_time());
      fflush(log_fd);
      log_dir=0;
    }
    for(i=0; i < k; i++) {
      fprintf(log_fd,"%02x ",(unsigned int) (c[i] & 0xFF));
    }
    last_time = clock_time();
  }
#endif

  return k;
}

void SerialPutBuffer(unsigned char* c, int len) {
  int n,i;
  n=-1;

  do {
    n = write(serial_fd,c,len);
    if(n==len) {
      break;
    }
    ASSERT(0);
  } while(errno == EAGAIN);

#ifdef SERIAL_LOG
    if(log_fd)
      {
      if(log_dir==0 || ((clock_time()-last_time) > 200)) {
        fprintf(log_fd,"\n%lu W ", clock_time());
        fflush(log_fd);
        log_dir=1;
      }
      for(i=0; i <n; i++) {
        fprintf(log_fd,"%02x ",(unsigned int)(c[i] & 0xFF));
      }
      last_time = clock_time();

      fflush(log_fd);
    }
#endif
}


int SerialCheck() {
    fd_set rfds;
    struct timeval tv;
    int retval;

    /* Watch stdin (fd 0) to see when it has input. */
    FD_ZERO(&rfds);
    FD_SET(serial_fd, &rfds);

    tv.tv_sec = 0;
    tv.tv_usec = 1000;

    retval = select(serial_fd+1, &rfds, NULL, NULL, &tv);
    /* Don't rely on the value of tv now! */
    if (retval == -1) {
        perror("select()");
		return 0;
    } else if (retval>0) {
        return 1;
    } else {
    	return 0;
    }
}

void SerialFlush() {
  #ifdef __BIONIC__
     //ioctl(fd, TCSBRK, 1);
     ioctl(serial_fd, TCSBRK, 1);
  #else
     if(tcdrain(serial_fd)) {
    ERR_PRINTF("Unable to drain serial buffer. Target might be dead....\n");
  }

  #endif
}
#endif