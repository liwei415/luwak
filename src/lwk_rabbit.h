#ifndef _LWK_RABBIT_
#define _LWK_RABBIT_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdint.h>
#include <amqp_tcp_socket.h>
#include <amqp.h>
#include <amqp_framing.h>

#include "lwk_common.h"

int lwk_rabbit_llen(char *server, int port, char *queue, int passive, int durable, int auto_delete);

#endif
