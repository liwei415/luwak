#include "luwak.h"

lwk_vars_t vars;

static void vars_init(void)
{
  vars.is_daemon = 0;
  vars.log_level = 7;
  lwk_strlcpy(vars.log_path, "lwk.log", sizeof(vars.log_path));
}

void *_t_redis(void *t_redis)
{
  //Detach thread
  pthread_detach(pthread_self());

  //Extract t_redis
  int num = ((t_redis_t *)t_redis)->num;

  char server[128];
  lwk_strlcpy(server, ((t_redis_t *)t_redis)->server, 128);

  int port = ((t_redis_t *)t_redis)->port;

  char key[128];
  lwk_strlcpy(key, ((t_redis_t *)t_redis)->key, 128);

  char command[512];
  lwk_strlcpy(command, ((t_redis_t *)t_redis)->command, 512);

  int enabled = ((t_redis_t *)t_redis)->enabled;

  free(t_redis);t_redis = NULL; //Deallocate memory for argument

  LOG_PRINT(LOG_DEBUG, "REDIS ready to work...num:%d, server:%s, port:%d, key:%s, command:%s, enabled:%d\n",
            num,
            server,
            port,
            key,
            command,
            enabled);

  while(1) {
    if (lwk_redis_llen(server, port, key) > 0) {
      system(command);
    }
    usleep(U_SLEEP_TIME);
  }

  pthread_exit(0);
}

void *_t_rabbit(void *t_rabbit)
{
  //Detach thread
  pthread_detach(pthread_self());

  //Extract t_rabbit
  int num = ((t_rabbit_t *)t_rabbit)->num;

  char server[128];
  lwk_strlcpy(server, ((t_rabbit_t *)t_rabbit)->server, 128);

  int port = ((t_rabbit_t *)t_rabbit)->port;

  char key[128];
  lwk_strlcpy(key, ((t_rabbit_t *)t_rabbit)->key, 128);

  char command[512];
  lwk_strlcpy(command, ((t_rabbit_t *)t_rabbit)->command, 512);

  int enabled = ((t_rabbit_t *)t_rabbit)->enabled;

  free(t_rabbit);t_rabbit = NULL; //Deallocate memory for argument

  LOG_PRINT(LOG_DEBUG, "RABBIT ready to work...num:%d, server:%s, port:%d, key:%s, command:%s, enabled:%d\n",
            num,
            server,
            port,
            key,
            command,
            enabled);

  while (1) {
    printf("num:%d, server:%s, port:%d, key:%s, command:%s, enabled:%d\n", num, server, port, key, command, enabled);
    sleep(1);
  }

  pthread_exit(0);
}

void _monitor() {
  while(1) {
    printf("I am the boss!\n");
    sleep(5);
  }
}

int main()
{
  vars_init();
  lwk_redis_consumers_t *redis_consumers = lwk_xml_load_redis("redis.xml");
  lwk_rabbit_consumers_t *rabbit_consumers = lwk_xml_load_rabbit("rabbit.xml");

  int i;

  // redis
  for (i = 0; i < redis_consumers->size; i++) {
    t_redis_t *t_redis = (t_redis_t *)calloc(1, sizeof(t_redis_t));
    if (t_redis == NULL) {
      LOG_PRINT(LOG_DEBUG, "create thread failed...");
    }
    else {
      t_redis->num = i;
      lwk_strlcpy(t_redis->server, (redis_consumers->consumer+i)->server, 128);
      t_redis->port = (redis_consumers->consumer+i)->port;
      lwk_strlcpy(t_redis->key, (redis_consumers->consumer+i)->key, 128);
      lwk_strlcpy(t_redis->command, (redis_consumers->consumer+i)->command, 512);
      t_redis->enabled = (redis_consumers->consumer+i)->enabled;

      pthread_t tid;
      int error = pthread_create(&tid, NULL, _t_redis, t_redis);
      if (error != 0) {
        LOG_PRINT(LOG_DEBUG, "Couldn't run thread");
      }
    }
  }

  // rabbit
  for (i = 0; i < rabbit_consumers->size; i++) {
    t_rabbit_t *t_rabbit = (t_rabbit_t *)calloc(1, sizeof(t_rabbit_t));
    if (t_rabbit == NULL) {
      LOG_PRINT(LOG_DEBUG, "create thread failed...");
    }
    else {
      t_rabbit->num = i;
      lwk_strlcpy(t_rabbit->server, (rabbit_consumers->consumer+i)->server, 128);
      t_rabbit->port = (rabbit_consumers->consumer+i)->port;
      lwk_strlcpy(t_rabbit->key, (rabbit_consumers->consumer+i)->key, 128);
      lwk_strlcpy(t_rabbit->command, (rabbit_consumers->consumer+i)->command, 512);
      t_rabbit->enabled = (rabbit_consumers->consumer+i)->enabled;

      pthread_t tid;
      int error = pthread_create(&tid, NULL, _t_rabbit, t_rabbit);
      if (error != 0) {
        LOG_PRINT(LOG_DEBUG, "Couldn't run thread");
      }
    }
  }

  // 主线程定时检查，有问题重启线程
  _monitor();

  return 0;
}
