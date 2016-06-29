#ifndef _LWK_REDIS_
#define _LWK_REDIS_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <hiredis/hiredis.h>

int lwk_redis_llen(char *server, int port, char *key);

#endif
