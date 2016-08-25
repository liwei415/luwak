#ifndef _LWK_COMMON_
#define _LWK_COMMON_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <hiredis/hiredis.h>

#include "lwk_log.h"

#ifndef LWK_VERSION
#define LWK_VERSION "2.0.1"
#endif

#define U_SLEEP_TIME 1000000
#define SLEEP_TIME 1

typedef struct lwk_xml_s lwk_xml_t;
struct lwk_xml_s {
  int is_daemon;
  int log_level;
  char log_path[512];
};

typedef struct lwk_redis_consumer_s lwk_redis_consumer_t;
typedef struct lwk_redis_consumers_s lwk_redis_consumers_t;

struct lwk_redis_consumer_s {
  char server[128];
  int port;
  char key[128];
  char command[512];
  int enabled;
};

struct lwk_redis_consumers_s {
  int size;
  lwk_redis_consumer_t *consumer;
};

typedef struct lwk_rabbit_consumer_s lwk_rabbit_consumer_t;
typedef struct lwk_rabbit_consumers_s lwk_rabbit_consumers_t;

struct lwk_rabbit_consumer_s {
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

struct lwk_rabbit_consumers_s {
  int size;
  lwk_rabbit_consumer_t *consumer;
};

typedef struct lwk_vars_s lwk_vars_t;
struct lwk_vars_s {
  char version[32];
  int is_daemon;
  int log_level;
  char log_path[512];

};

extern lwk_vars_t vars;

#define LOG_FATAL       0           /* System is unusable */
#define LOG_ALERT       1           /* Action must be taken immediately */
#define LOG_CRIT        2           /* Critical conditions */
#define LOG_ERROR       3           /* Error conditions */
#define LOG_WARNING     4           /* Warning conditions */
#define LOG_NOTICE      5           /* Normal, but significant */
#define LOG_INFO        6           /* Information */
#define LOG_DEBUG       7           /* DEBUG message */

#ifdef DEBUG
#define LOG_PRINT(level, fmt, ...)                      \
  do {                                                  \
    int log_id = lwk_log_open(vars.log_path, "a");      \
    lwk_log_printf0(log_id, level, "%s:%d %s() "fmt,    \
                   __FILE__, __LINE__, __FUNCTION__,    \
                   ##__VA_ARGS__);                      \
    lwk_log_close(log_id);                              \
  } while (0)
#else
#define LOG_PRINT(level, fmt, ...)                          \
  do {                                                      \
    if (level <= vars.log_level) {                          \
      int log_id = lwk_log_open(vars.log_path, "a");        \
      lwk_log_printf0(log_id, level, fmt, ##__VA_ARGS__) ;  \
      lwk_log_close(log_id);                                \
    }                                                       \
  } while (0)
#endif

#endif
