#include "lwk_xml.h"

lwk_xml_t *lwk_xml_load(char *conf)
{
  xmlDocPtr doc;
  xmlNodePtr cur_root, cur_chd;
  xmlChar *key;

  xmlKeepBlanksDefault(0);
  doc = xmlParseFile(conf);
  if (doc == NULL ) {
    fprintf(stderr,"Document not parsed successfully.\n");
    exit(-1);
  }

  cur_root = xmlDocGetRootElement(doc);
  if (cur_root == NULL) {
    fprintf(stderr,"Empty document.\n");
    xmlFreeDoc(doc);
    exit(-1);
  }

  lwk_xml_t *luwak = calloc(1, sizeof(lwk_xml_t));
  cur_chd = cur_root->xmlChildrenNode;
  while (cur_chd != NULL) {
    key = xmlNodeListGetString(doc, cur_chd->xmlChildrenNode, 1);
    if ((!xmlStrcmp(cur_chd->name, (const xmlChar *)"IsDaemon"))) {
      luwak->is_daemon = atoi((char *)key);
    }
    else if ((!xmlStrcmp(cur_chd->name, (const xmlChar *)"LogLevel"))) {
      luwak->log_level = atoi((char *)key);
    }
    else if ((!xmlStrcmp(cur_chd->name, (const xmlChar *)"LogPath"))) {
      lwk_strlcpy(luwak->log_path, (char *)key, 512);
    }
    xmlFree(key);
    cur_chd = cur_chd->next;
  }

  /* 释放内存 */
  xmlFreeDoc(doc);

  return luwak;
}

lwk_redis_consumers_t *lwk_xml_load_redis(char *conf)
{
  xmlDocPtr doc;
  xmlNodePtr cur_root, cur_consumer, cur_consumer_chd;
  xmlChar *key;
  int sum = 0;

  xmlKeepBlanksDefault(0);
  doc = xmlParseFile(conf);
  if (doc == NULL ) {
    fprintf(stderr,"Document not parsed successfully.\n");
    exit(-1);
  }

  cur_root = xmlDocGetRootElement(doc);
  if (cur_root == NULL) {
    fprintf(stderr,"Empty document.\n");
    xmlFreeDoc(doc);
    exit(-1);
  }

  /* 计算XML内consumer个数 */
  cur_consumer = cur_root->xmlChildrenNode;
  while (cur_consumer != NULL) {
    if ((!xmlStrcmp(cur_consumer->name, (const xmlChar *)"Consumer"))){
      ++sum;
    }
    cur_consumer = cur_consumer->next;
  }

  /* 重新开始解析XML文件 */
  lwk_redis_consumers_t *consumers = calloc(1, sizeof(lwk_redis_consumers_t));
  consumers->size = sum;
  consumers->consumer = calloc(consumers->size, sizeof(lwk_redis_consumer_t));

  int i;
  cur_root = xmlDocGetRootElement(doc);
  cur_consumer = cur_root->xmlChildrenNode;
  for (i = 0; i < consumers->size; i++) {
    if ((!xmlStrcmp(cur_consumer->name, (const xmlChar *)"Consumer"))){
      cur_consumer_chd = cur_consumer->xmlChildrenNode;
      while (cur_consumer_chd != NULL) {
        key = xmlNodeListGetString(doc, cur_consumer_chd->xmlChildrenNode, 1);
        if ((!xmlStrcmp(cur_consumer_chd->name, (const xmlChar *)"Server"))) {
          lwk_strlcpy((consumers->consumer+i)->server, (char *)key, 128);
        }
        else if ((!xmlStrcmp(cur_consumer_chd->name, (const xmlChar *)"Port"))) {
          (consumers->consumer+i)->port = atoi((char *)key);
        }
        else if ((!xmlStrcmp(cur_consumer_chd->name, (const xmlChar *)"Key"))) {
          lwk_strlcpy((consumers->consumer+i)->key, (char *)key, 128);
        }
        else if ((!xmlStrcmp(cur_consumer_chd->name, (const xmlChar *)"Command"))) {
          lwk_strlcpy((consumers->consumer+i)->command, (char *)key, 512);
        }
        else if ((!xmlStrcmp(cur_consumer_chd->name, (const xmlChar *)"Enabled"))) {
          (consumers->consumer+i)->enabled = atoi((char *)key);
        }
        xmlFree(key);
        cur_consumer_chd = cur_consumer_chd->next;
      }
    }
    cur_consumer = cur_consumer->next;
  }

  /* 释放内存 */
  xmlFreeDoc(doc);

  return consumers;
}

lwk_rabbit_consumers_t *lwk_xml_load_rabbit(char *conf)
{
  xmlDocPtr doc;
  xmlNodePtr cur_root, cur_consumer, cur_consumer_chd;
  xmlChar *key;
  int sum = 0;

  xmlKeepBlanksDefault(0);
  doc = xmlParseFile(conf);
  if (doc == NULL ) {
    fprintf(stderr,"Document not parsed successfully.\n");
    exit(-1);
  }

  cur_root = xmlDocGetRootElement(doc);
  if (cur_root == NULL) {
    fprintf(stderr,"Empty document.\n");
    xmlFreeDoc(doc);
    exit(-1);
  }

  /* 计算XML内consumer个数 */
  cur_consumer = cur_root->xmlChildrenNode;
  while (cur_consumer != NULL) {
    if ((!xmlStrcmp(cur_consumer->name, (const xmlChar *)"Consumer"))){
      ++sum;
    }
    cur_consumer = cur_consumer->next;
  }

  /* 重新开始解析XML文件 */
  lwk_rabbit_consumers_t *consumers = calloc(1, sizeof(lwk_rabbit_consumers_t));
  consumers->size = sum;
  consumers->consumer = calloc(consumers->size, sizeof(lwk_rabbit_consumer_t));

  int i;
  cur_root = xmlDocGetRootElement(doc);
  cur_consumer = cur_root->xmlChildrenNode;
  for (i = 0; i < consumers->size; i++) {
    if ((!xmlStrcmp(cur_consumer->name, (const xmlChar *)"Consumer"))){
      cur_consumer_chd = cur_consumer->xmlChildrenNode;
      while (cur_consumer_chd != NULL) {
        key = xmlNodeListGetString(doc, cur_consumer_chd->xmlChildrenNode, 1);
        if ((!xmlStrcmp(cur_consumer_chd->name, (const xmlChar *)"Server"))) {
          lwk_strlcpy((consumers->consumer+i)->server, (char *)key, 128);
        }
        else if ((!xmlStrcmp(cur_consumer_chd->name, (const xmlChar *)"Port"))) {
          (consumers->consumer+i)->port = atoi((char *)key);
        }
        else if ((!xmlStrcmp(cur_consumer_chd->name, (const xmlChar *)"Username"))) {
          lwk_strlcpy((consumers->consumer+i)->username, (char *)key, 128);
        }
        else if ((!xmlStrcmp(cur_consumer_chd->name, (const xmlChar *)"Password"))) {
          lwk_strlcpy((consumers->consumer+i)->password, (char *)key, 128);
        }
        else if ((!xmlStrcmp(cur_consumer_chd->name, (const xmlChar *)"Vhost"))) {
          lwk_strlcpy((consumers->consumer+i)->vhost, (char *)key, 128);
        }
        else if ((!xmlStrcmp(cur_consumer_chd->name, (const xmlChar *)"Queue"))) {
          lwk_strlcpy((consumers->consumer+i)->queue, (char *)key, 128);
        }
        else if ((!xmlStrcmp(cur_consumer_chd->name, (const xmlChar *)"Command"))) {
          lwk_strlcpy((consumers->consumer+i)->command, (char *)key, 512);
        }
        else if ((!xmlStrcmp(cur_consumer_chd->name, (const xmlChar *)"Passive"))) {
          (consumers->consumer+i)->passive = atoi((char *)key);
        }
        else if ((!xmlStrcmp(cur_consumer_chd->name, (const xmlChar *)"Durable"))) {
          (consumers->consumer+i)->durable = atoi((char *)key);
        }
        else if ((!xmlStrcmp(cur_consumer_chd->name, (const xmlChar *)"Exclusive"))) {
          (consumers->consumer+i)->exclusive = atoi((char *)key);
        }
        else if ((!xmlStrcmp(cur_consumer_chd->name, (const xmlChar *)"AutoDelete"))) {
          (consumers->consumer+i)->auto_delete = atoi((char *)key);
        }
        else if ((!xmlStrcmp(cur_consumer_chd->name, (const xmlChar *)"Threads"))) {
          (consumers->consumer+i)->threads = atoi((char *)key);
        }
        else if ((!xmlStrcmp(cur_consumer_chd->name, (const xmlChar *)"Enabled"))) {
          (consumers->consumer+i)->enabled = atoi((char *)key);
        }
        xmlFree(key);
        cur_consumer_chd = cur_consumer_chd->next;
      }
    }
    cur_consumer = cur_consumer->next;
  }

  /* 释放内存 */
  xmlFreeDoc(doc);

  return consumers;
}
