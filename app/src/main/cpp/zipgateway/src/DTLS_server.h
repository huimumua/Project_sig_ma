#ifndef DTLS_SERVER_H
#define DTLS_SERVER_H

#include "contiki.h"
#include <netinet/in.h>

#define DTLS_PORT 41230
PROCESS_NAME(dtls_server_process);

int dtls_send(struct sockaddr_in *addr, const void* data, u16_t len, u8_t create_new);

int session_done_for_uipbuf();

extern int dtls_ssl_read_failed;
extern int dtls_server_conn;

enum {DTLS_SERVER_INPUT_EVENT,
    DTLS_CONNECTION_CLOSE_EVENT,
    DLTS_CONNECTION_INIT_DONE_EVENT,
    DLTS_SESSION_UPDATE_EVENT,
};

void dtls_exited();

#endif