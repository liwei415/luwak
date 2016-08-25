#include "luwak.h"

lwk_vars_t vars;

static void vars_init(void)
{
  lwk_xml_t *luwak = lwk_xml_load("luwak.xml");

  lwk_strlcpy(vars.version, LWK_VERSION, 512);
  vars.is_daemon = luwak->is_daemon;
  vars.log_level = luwak->log_level;
  lwk_strlcpy(vars.log_path, luwak->log_path, 512);
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

  free(t_redis);t_redis = NULL; //Deallocate memory for argument

  LOG_PRINT(LOG_DEBUG, "REDIS ready to work...num:%d", num);

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

  char username[128];
  lwk_strlcpy(username, ((t_rabbit_t *)t_rabbit)->username, 128);

  char password[128];
  lwk_strlcpy(password, ((t_rabbit_t *)t_rabbit)->password, 128);

  char vhost[128];
  lwk_strlcpy(vhost, ((t_rabbit_t *)t_rabbit)->vhost, 128);

  char queue[128];
  lwk_strlcpy(queue, ((t_rabbit_t *)t_rabbit)->queue, 128);

  char command[512];
  lwk_strlcpy(command, ((t_rabbit_t *)t_rabbit)->command, 512);

  int passive = ((t_rabbit_t *)t_rabbit)->passive;

  int exclusive = ((t_rabbit_t *)t_rabbit)->exclusive;

  int durable = ((t_rabbit_t *)t_rabbit)->durable;

  int auto_delete = ((t_rabbit_t *)t_rabbit)->auto_delete;

  free(t_rabbit);t_rabbit = NULL; //Deallocate memory for argument

  LOG_PRINT(LOG_DEBUG, "RABBIT ready to work...num:%d", num);

  while(1) {
    if (lwk_rabbit_llen(server, port, username, password, vhost, queue, passive, durable, exclusive, auto_delete) > 0) {
      //LOG_PRINT(LOG_DEBUG, "!!!!!!!!!!RABBIT:%s---->%d\n", queue, lwk_rabbit_llen(server, port, queue, passive, durable, exclusive, auto_delete));
      system(command);
    }
    usleep(U_SLEEP_TIME);
  }

  pthread_exit(0);
}

void _monitor() {
  while(1) {
    printf("I am the boss!\n");
    sleep(60);
  }
}

int main()
{
  vars_init();
  lwk_redis_consumers_t *redis_consumers = lwk_xml_load_redis("redis.xml");
  lwk_rabbit_consumers_t *rabbit_consumers = lwk_xml_load_rabbit("rabbit.xml");

  int i, j;

  // daemon
  if(vars.is_daemon == 1) {
    if(daemon(1, 1) < 0) {
      fprintf(stderr, "Create daemon failed!\n");
      return -1;
    }
    else {
      fprintf(stdout, "luwak %s\n", vars.version);
      fprintf(stdout, "Copyright (c) 2015-2017 l.inux.xyz\n");
      fprintf(stderr, "\n");
    }
  }



  // redis
  for (i = 0; i < redis_consumers->size; i++) {
    if ((redis_consumers->consumer+i)->enabled == 0) {
      continue;
    }

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
    if ((rabbit_consumers->consumer+i)->enabled == 0) {
      continue;
    }
    for (j = 0; j <= (rabbit_consumers->consumer+i)->threads; j++) {
      t_rabbit_t *t_rabbit = (t_rabbit_t *)calloc(1, sizeof(t_rabbit_t));
      if (t_rabbit == NULL) {
        LOG_PRINT(LOG_DEBUG, "create thread failed...");
      }
      else {
        t_rabbit->num = i;
        t_rabbit->tnum = j;
        lwk_strlcpy(t_rabbit->server, (rabbit_consumers->consumer+i)->server, 128);
        t_rabbit->port = (rabbit_consumers->consumer+i)->port;
        lwk_strlcpy(t_rabbit->username, (rabbit_consumers->consumer+i)->username, 128);
        lwk_strlcpy(t_rabbit->password, (rabbit_consumers->consumer+i)->password, 128);
        lwk_strlcpy(t_rabbit->vhost, (rabbit_consumers->consumer+i)->vhost, 128);
        lwk_strlcpy(t_rabbit->queue, (rabbit_consumers->consumer+i)->queue, 128);
        lwk_strlcpy(t_rabbit->command, (rabbit_consumers->consumer+i)->command, 512);
        t_rabbit->passive = (rabbit_consumers->consumer+i)->passive;
        t_rabbit->durable = (rabbit_consumers->consumer+i)->durable;
        t_rabbit->exclusive = (rabbit_consumers->consumer+i)->exclusive;
        t_rabbit->auto_delete = (rabbit_consumers->consumer+i)->auto_delete;
        t_rabbit->enabled = (rabbit_consumers->consumer+i)->enabled;

        pthread_t tid;
        int error = pthread_create(&tid, NULL, _t_rabbit, t_rabbit);
        if (error != 0) {
          LOG_PRINT(LOG_DEBUG, "Couldn't run thread");
        }
      }
    }
  }

  // 主线程定时检查，有问题重启线程
  _monitor();

  return 0;
}
