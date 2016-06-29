#include "lwk_redis.h"

int lwk_redis_llen(char *server, int port, char *key)
{
  int rst = 0;
  redisContext *c = redisConnect(server, port);
  if (c == NULL || c->err) {
    if (c) {
      printf("Connection error: %s\n", c->errstr);
      redisFree(c);
    }
    else {
      printf("Connection error: can't allocate redis context\n");
    }
    return -1;
  }

  redisReply *r = redisCommand(c, "LLEN %s", key);
  if (r != NULL) {
    rst = r->integer;
  }
  freeReplyObject(r);
  redisFree(c);

  return rst;
}
