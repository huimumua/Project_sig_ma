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
 * $Id: uip-udp-packet.c,v 1.7 2009/10/18 22:02:01 adamdunkels Exp $
 */

/**
 * \file
 *         Module for sending UDP packets through uIP.
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki-conf.h"

extern u16_t uip_ipv4_slen;

#include "net/uip-ipv4-udp-packet.h"

#ifdef ZIP_NATIVE
#include <ZW_typedefs.h>
#include <ZW_mem_api.h>
#else
#include <string.h>
#endif

#ifdef __C51__
#define data _data
#endif

/*---------------------------------------------------------------------------*/
void
uip_ipv4_udp_packet_send(struct uip_udp_conn *c, const void *data, int len)
{
#if UIP_UDP
  uip_ipv4_udp_conn = c;
  uip_ipv4_slen = len;
  memmove(&uip_ipv4_buf[UIP_LLH_LEN + UIP_IPUDPH_LEN], data, len > UIP_BUFSIZE? UIP_BUFSIZE: len);
  uip_ipv4_process(UIP_UDP_SEND_CONN);
#if UIP_CONF_IPV6 //math
  tcpip_ipv6_output();
#else
  if(uip_ipv4_len > 0) {
     tcpip_ipv4_output();
  }
#endif
  uip_ipv4_slen = 0;
#endif /* UIP_UDP */
}
/*---------------------------------------------------------------------------*/
void
uip_ipv4_udp_packet_sendto(struct uip_udp_conn *c, const void *data, int len,
		      const uip_ipaddr_t *toaddr, uint16_t toport)
{
  uip_ipaddr_t curaddr;
  uint16_t curport;

  /* Save current IP addr/port. */
  uip_ipv4_ipaddr_copy(&curaddr, &c->ripaddr);
  curport = c->rport;

  /* Load new IP addr/port */
  uip_ipv4_ipaddr_copy(&c->ripaddr, toaddr);
  c->rport = toport;

  uip_ipv4_udp_packet_send(c, data, len);

  /* Restore old IP addr/port */
  uip_ipv4_ipaddr_copy(&c->ripaddr, &curaddr);
  c->rport = curport;
}
/*---------------------------------------------------------------------------*/
