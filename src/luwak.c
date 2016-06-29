#include "luwak.h"

int _run(lwk_consumer_t *consumer)
{
  while(1) {
    if (lwk_redis_llen(consumer->server, consumer->port, consumer->key) > 0) {
      system(consumer->command);
    }
    usleep(U_SLEEP_TIME);
  }
}

int main()
{
  lwk_consumers_t *consumers = lwk_conf_load_consumers("/webdata/luwak/conf/luwak.xml");

  pid_t pids[consumers->size];

  int i;
  for (i = 0; i < consumers->size; i++) {
    pids[i] = fork();
    if(pids[i] == 0) {
      _run(consumers->consumer+i);
    }
    usleep(U_SLEEP_TIME);
  }

  return 0;
}
