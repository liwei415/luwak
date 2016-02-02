/*
 * Copyright (C) Liwei
 * Copyright (C) Roadoor, Inc.
 */


#ifndef _LWK_REDIS_H_
#define _LWK_REDIS_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <hiredis/hiredis.h>

int lwk_redis_llen(char *server, int port, char *key);

#endif
