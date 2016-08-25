/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _LWK_CORE_
#define _LWK_CORE_

#include <pthread.h>

#include "lwk_common.h"
#include "lwk_xml.h"
#include "lwk_str.h"
#include "lwk_rabbit.h"
#include "lwk_redis.h"

#define U_SLEEP_TIME 1000000
#define SLEEP_TIME 1

typedef struct t_redis_s t_redis_t;
struct t_redis_s
{
  int num;
  char server[128];
  int port;
  char key[128];
  char command[512];
  int enabled;
};

typedef struct t_rabbit_s t_rabbit_t;
struct t_rabbit_s
{
  int num;
  int tnum;
  char server[128];
  int port;
  char username[128];
  char password[128];
  char vhost[128];
  char queue[128];
  char command[512];
  int passive;
  int durable;
  int exclusive;
  int auto_delete;
  int threads;
  int enabled;
};

#endif
