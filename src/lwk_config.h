/*
 * Copyright (C) Liwei
 * Copyright (C) Roadoor, Inc.
 */


#ifndef _LWK_CONFIG_
#define _LWK_CONFIG_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <hiredis/hiredis.h>


/* last_do用于判断是否应该进行心跳 */
typedef struct lwk_consumer_s lwk_consumer_t;
typedef struct lwk_consumers_s lwk_consumers_t;

struct lwk_consumer_s {
  char *server;
  int port;
  char *key;
  char *command;
  int enabled;
};

struct lwk_consumers_s {
  int size;
  lwk_consumer_t *consumer;
};

#endif
