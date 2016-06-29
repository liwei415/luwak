#include "lwk_log.h"

struct log_level_desc{
  enum LOG_LEVEL level;
  char *endesc;
};

static struct log_level_desc log_level_descs[] = {
  { LOG_LEVEL_FATAL, "FATAL" },
  { LOG_LEVEL_ALERT, "ALERT" },
  { LOG_LEVEL_CRIT, "CRITICAL" },
  { LOG_LEVEL_ERROR, "ERROR" },
  { LOG_LEVEL_WARNING, "WARNING" },
  { LOG_LEVEL_NOTICE, "NOTICE" },
  { LOG_LEVEL_INFO, "INFO" },
  { LOG_LEVEL_DEBUG, "DEBUG" },
};

static FILE* log_files[MAX_LOGS+1];
static ox_slock_t log_locks[MAX_LOGS+1];

static int ox_log_valid(int log_id);

/**
 * @brief log_valid check log is valid
 *
 * @param log_id the log id
 *
 * @return 1 for OK and 0 for fail
 */
static int ox_log_valid(int log_id)
{
  if (log_id < 0 || log_id > MAX_LOGS) {
    return 0;
  }
  return 1;
}

/**
 * @brief log_init init log
 */
void ox_log_init(void)
{
  int i;
  for (i = 0; i < MAX_LOGS+1; i++) {
    log_files[i] = NULL;
    ox_slock_init(&log_locks[i], NULL);
  }
}

/**
 * @brief log_open open a log file
 *
 * @param path the path of log
 * @param mode the mode of open
 *
 * @return log open id for OK and -1 for fail
 */
int ox_log_open(const char *path, const char* mode)
{
  int i;
  for (i = LOG_USER; i < MAX_LOGS+1; i++) {
    ox_slock_lock(&log_locks[i]);
    if (log_files[i] == NULL) {
      log_files[i] = fopen(path, mode);
      if (log_files[i]) {
        ox_slock_unlock(&log_locks[i]);
        return i;
      }
    }
    ox_slock_unlock(&log_locks[i]);
  }
  return LOG_INVALID;
}

/* Log a fixed message without printf-alike capabilities, in a way that is
 * safe to call from a signal handler.
 *
 * We actually use this only for signals that are not fatal from the point
 * of view of Redis. Signals that are going to kill the server anyway and
 * where we need printf-alike features are served by redisLog(). */
void ox_log_handler(const char *msg)
{
  int fd;
  int log_to_stdout = vars.log_level == -1;

  fd = log_to_stdout ? STDOUT_FILENO : open(vars.log_path, O_APPEND|O_CREAT|O_WRONLY, 0644);
  if (fd == -1) {
    return;
  }

  if (write(fd, msg, strlen(msg)) == -1) {
    goto err;
  }

  if (write(fd, "\n", 1) == -1) {
    goto err;
  }

 err:
  if (!log_to_stdout) {
    close(fd);
  }
}

/**
 * @brief log_printf0 print log to file function
 *
 * @param log_id log id
 * @param log_level log level
 * @param fmt format of string
 * @param ... other args
 */
void ox_log_printf0(int log_id, int log_level, const char *fmt, ...)
{
  FILE *fp;
  time_t t;
  char tmbuf[30];
  const char *p;
  va_list args;
  int level;

  if (!ox_log_valid(log_id)) {
    fp = stdout;
  }
  else {
    ox_slock_lock(&log_locks[log_id]);
    if (!(fp = log_files[log_id])) {
      ox_slock_unlock(&log_locks[log_id]);
      return;
    }
  }

  if (log_level > LOG_LEVEL_DEBUG) {
    level = LOG_LEVEL_DEBUG;
  }
  else if (log_level < LOG_LEVEL_FATAL) {
    level = LOG_LEVEL_FATAL;
  }
  else {
    level = log_level;
  }

  t = time(NULL);
  struct timeval tv;
  gettimeofday(&tv , NULL);
  memset(tmbuf, 0, sizeof(tmbuf));
  strftime(tmbuf, sizeof(tmbuf), "%Y/%m/%d %H:%M:%S", localtime(&t));
  fprintf (fp, "%s:%.6d ", tmbuf, (int)tv.tv_usec);

#ifdef DEBUG
  fprintf(fp, "Thread ID: %lu ", (unsigned long)pthread_self());
#endif

  fprintf(fp, "[%s] ", log_level_descs[level].endesc);

  va_start(args, fmt);
  vfprintf(fp, fmt, args);
  va_end(args);

  p = fmt + strlen(fmt) - 1;
  if (*p != '\n') {
    fputc('\n', fp);
  }


  if (ox_log_valid(log_id)) {
    fflush(fp);
    ox_slock_unlock(&log_locks[log_id]);
  }
}

/**
 * @brief log_flush flush log string to file
 *
 * @param log_id log id
 */
void ox_log_flush(int log_id)
{
  if (!ox_log_valid(log_id)) {
    return;
  }

  ox_slock_lock(&log_locks[log_id]);

  if (log_files[log_id]) {
    fflush(log_files[log_id]);
  }

  ox_slock_unlock(&log_locks[log_id]);
}

/**
 * @brief log_close close the log
 *
 * @param log_id the log id
 */
void ox_log_close(int log_id)
{
  if (!ox_log_valid(log_id)) {
    return;
  }

  ox_slock_lock(&log_locks[log_id]);

  if (log_files[log_id]) {
    fclose(log_files[log_id]);
    log_files[log_id] = NULL;
  }

  ox_slock_unlock(&log_locks[log_id]);
}
