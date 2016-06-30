#ifndef _LWK_LOG_
#define _LWK_LOG_

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdarg.h>
#include <pthread.h>

#include "lwk_common.h"
#include "lwk_slock.h"

#define MAX_LOGS        100

#define LOG_INVALID     -1
#define LOG_SYSTEM      0
#define LOG_USER        1

enum LOG_LEVEL{
  LOG_LEVEL_FATAL = 0,                    /* System is unusable */
  LOG_LEVEL_ALERT,                        /* Action must be taken immediately */
  LOG_LEVEL_CRIT,                         /* Critical conditions */
  LOG_LEVEL_ERROR,                        /* Error conditions */
  LOG_LEVEL_WARNING,                      /* Warning conditions */
  LOG_LEVEL_NOTICE,                       /* Normal, but significant */
  LOG_LEVEL_INFO,                         /* Information */
  LOG_LEVEL_DEBUG,                        /* DEBUG message */
};

void lwk_log_init(void);
int lwk_log_open(const char *path, const char* mode);
void lwk_log_handler(const char *msg);
void lwk_log_printf0(int log_id, int level, const char *fmt, ...);
void lwk_log_flush(int log_id);
void lwk_log_close(int log_id);

#endif
