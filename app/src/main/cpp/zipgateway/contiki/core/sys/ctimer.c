/**
 * \addtogroup ctimer
 * @{
 */

/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 * $Id: ctimer.c,v 1.1 2010/06/14 07:34:36 adamdunkels Exp $
 */

/**
 * \file
 *         Callback timer implementation
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "sys/ctimer.h"
#include "contiki.h"
#include "lib/list.h"
#ifdef ZW_DEBUG_CTIMER
#include <ZW_typedefs.h>
#include <ZW_uart_api.h>
#include <ZW_timer_api.h>
#endif

#define data _data

LIST(ctimer_list);

static char initialized;

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#if !CC_NO_VA_ARGS
#define PRINTF(...)
#endif
/* TODO: Find some replacement for printf() here */
#endif
#ifdef ZW_DEBUG_CTIMER
#define ZW_DEBUG_CTIMER_INIT(baud)       ZW_UART1_init(baud, TRUE, FALSE)
#define ZW_DEBUG_CTIMER_SEND_BYTE(bData) ZW_UART1_tx_send_byte(bData)
#define ZW_DEBUG_CTIMER_SEND_NUM(bData)  ZW_UART1_tx_send_num(bData)
#ifdef ZWAVE_IP
#define ZW_DEBUG_CTIMER_SEND_NUMW(bData) ZW_UART1_tx_send_numW(bData)
#endif
#define ZW_DEBUG_CTIMER_SEND_NL()        ZW_UART1_tx_send_nl()
#define ZW_DEBUG_CTIMER_SEND_STR(STR)    ZW_UART1_tx_send_str(STR)
#define ZW_DEBUG_CTIMER_TX_STATUS()      ZW_UART1_tx_active_get()
#else
#define ZW_DEBUG_CTIMER_INIT(baud)
#define ZW_DEBUG_CTIMER_SEND_BYTE(bData)
#define ZW_DEBUG_CTIMER_SEND_NUM(bData)
#ifdef ZWAVE_IP
#define ZW_DEBUG_CTIMER_SEND_NUMW(bData)
#endif
#define ZW_DEBUG_CTIMER_SEND_NL()
#define ZW_DEBUG_CTIMER_SEND_STR(STR)
#define ZW_DEBUG_CTIMER_TX_STATUS()
#endif /* ZW_DEBUG */
/*---------------------------------------------------------------------------*/
PROCESS(ctimer_process, "Ctimer process");
PROCESS_THREAD(ctimer_process, ev, data)
{
  struct ctimer *c;
  PROCESS_BEGIN();

  for(c = list_head(ctimer_list); c != NULL; c = c->next) {
    etimer_set(&c->etimer, c->etimer.timer.interval);
  }
  initialized = 1;

  while(1) {
    PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_TIMER);
    for(c = list_head(ctimer_list); c != NULL; c = c->next) {
      if(&c->etimer == data) {
	list_remove(ctimer_list, c);
	PROCESS_CONTEXT_BEGIN(c->p);
	if(c->f != NULL) {
	  c->f(c->ptr);
	}
	PROCESS_CONTEXT_END(c->p);
	break;
      }
    }
  }
  PROCESS_END();
}

#ifdef ZW_DEBUG_CTIMER
extern struct ctimer periodic_timer;

void
dump_ctimer_list()
{
  struct ctimer *c;

  ZW_DEBUG_SEND_BYTE('c');
  ZW_DEBUG_SEND_BYTE('l');
  ZW_DEBUG_SEND_NUMW(getTickTime());
  ZW_DEBUG_SEND_NL();
  for(c = list_head(ctimer_list); c != NULL; c = c->next) {
    ZW_DEBUG_SEND_NUMW(c->etimer.timer.start);
    ZW_DEBUG_SEND_NUMW(c->etimer.timer.interval);
    if(c == &periodic_timer) {
      ZW_DEBUG_SEND_BYTE('*');
    }
    ZW_DEBUG_SEND_NL();
  }
}
#endif

/*---------------------------------------------------------------------------*/
void
ctimer_init(void)
{
  initialized = 0;
  list_init(ctimer_list);
  process_start(&ctimer_process, NULL);
}
/*---------------------------------------------------------------------------*/
void
ctimer_set(struct ctimer *c, clock_time_t t,
	   void (*f)(void *), void *ptr)
{
#if ! CC_NO_VA_ARGS
  PRINTF("ctimer_set %p %u\n", c, (unsigned)t);
#endif
  c->p = PROCESS_CURRENT();
  c->f = f;
  c->ptr = ptr;
  if(initialized) {
    PROCESS_CONTEXT_BEGIN(&ctimer_process);
    etimer_set(&c->etimer, t);
    PROCESS_CONTEXT_END(&ctimer_process);
  } else {
    c->etimer.timer.interval = t;
  }

  list_remove(ctimer_list, c);
  list_add(ctimer_list, c);
}
/*---------------------------------------------------------------------------*/
void
ctimer_reset(struct ctimer *c)
{
  if(initialized) {
    PROCESS_CONTEXT_BEGIN(&ctimer_process);
    etimer_reset(&c->etimer);
    PROCESS_CONTEXT_END(&ctimer_process);
  }

  list_remove(ctimer_list, c);
  list_add(ctimer_list, c);
}
/*---------------------------------------------------------------------------*/
void
ctimer_restart(struct ctimer *c)
{
  if(initialized) {
    PROCESS_CONTEXT_BEGIN(&ctimer_process);
    etimer_restart(&c->etimer);
    PROCESS_CONTEXT_END(&ctimer_process);
  }

  list_remove(ctimer_list, c);
  list_add(ctimer_list, c);
}
/*---------------------------------------------------------------------------*/
void
ctimer_stop(struct ctimer *c)
{
  if(initialized) {
    etimer_stop(&c->etimer);
  } else {
    c->etimer.next = NULL;
    c->etimer.p = PROCESS_NONE;
  }
  list_remove(ctimer_list, c);
}
/*---------------------------------------------------------------------------*/
int
ctimer_expired(struct ctimer *c)
{
  struct ctimer *t;
  if(initialized) {
    return etimer_expired(&c->etimer);
  }
  for(t = list_head(ctimer_list); t != NULL; t = t->next) {
    if(t == c) {
      return 0;
    }
  }
  return 1;
}
/*---------------------------------------------------------------------------*/
/** @} */
