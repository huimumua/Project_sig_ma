/*
 * Copyright (c) 2002, Adam Dunkels.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the Contiki OS
 *
 * $Id: contiki-main.c,v 1.13 2010/06/14 18:58:45 adamdunkels Exp $
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>
#include <pthread.h>

#include "contiki.h"

#include "dev/serial-line.h"
#include "tapdev-drv.h"
#include "tapdev6.h"
#include "net/uip.h"

#include "dev/eeprom.h"
#include "ZIP_Router.h"
#include "parse_config.h"
#include <stdlib.h>
#include "serial_api_process.h"
#include "DTLS_server.h"

PROCINIT(&etimer_process);

extern void set_landev_outputfunc(u8_t (* f)(uip_lladdr_t *a));
extern int net_fd;
static pthread_t threadId = 0;

extern BYTE zip_process_ok;

#ifndef ANDROID_PLATFORM
extern int serial_fd;
#else
extern int UsbSerial_Check();
#endif

void* thread_func(void *param)
{
    while(1)
    {
        fd_set fds;
        int n;
        struct timeval tv;

        if(!zip_process_ok || !process_is_running(&zip_process)){
            break;
        }

        while(process_run()) {
        }

        tv.tv_sec  = 0;
        tv.tv_usec = 50;

        FD_ZERO(&fds);

        if(dtls_server_conn > 0)
        {
            FD_SET(dtls_server_conn, &fds);
        }

#ifndef ANDROID_PLATFORM
        FD_SET(serial_fd, &fds);

        n = serial_fd > dtls_server_conn ? serial_fd : dtls_server_conn;
#else
        n = dtls_server_conn;

        if(UsbSerial_Check() > 0)
        {
            process_poll(&serial_api_process);
        }
#endif

        if( select(n+1, &fds, NULL, NULL, &tv) > 0)
        {
#ifndef ANDROID_PLATFORM
            if (FD_ISSET(serial_fd, &fds))
            {
                process_poll(&serial_api_process);
                //printf("serial input\n");
            }
#endif
            if(FD_ISSET(dtls_server_conn, &fds))
            {
                process_post(&dtls_server_process, DTLS_SERVER_INPUT_EVENT, 0);
            }
        }
        else
        {
            //printf("timeout %i\n",delay);
        }

        etimer_request_poll();
    }

    return NULL;
}

int StartZipGateWay(const char *resPath)
{
    ConfigInit(resPath);

    zip_process_ok = TRUE;

    LOG_PRINTF("Starting Contiki\n");
    set_landev_outputfunc(NULL);

    eeprom_init();
    process_init();
    ctimer_init();

    procinit_init();

    serial_line_init();

    autostart_start(autostart_processes);

    if(!zip_process_ok || !process_is_running(&zip_process))
    {
        return -1;
    }

    int res = pthread_create(&threadId, NULL, thread_func, NULL);

    if(res != 0)
    {
        return -1;
    }

    return 0;
}

void StopZipGateWay()
{
    process_post(PROCESS_BROADCAST,PROCESS_EVENT_EXIT,0);
    pthread_join(threadId, NULL);
}
