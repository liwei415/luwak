#include "lwk_xml.h"

lwk_consumers_t *lwk_conf_load_consumers(char *conf)
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
  lwk_consumers_t *consumers = calloc(1, sizeof(lwk_consumers_t));
  consumers->size = sum;
  consumers->consumer = calloc(consumers->size, sizeof(lwk_consumer_t));

  int i;
  cur_root = xmlDocGetRootElement(doc);
  cur_consumer = cur_root->xmlChildrenNode;
  for (i = 0; i < consumers->size; i++) {
    if ((!xmlStrcmp(cur_consumer->name, (const xmlChar *)"Consumer"))){
      cur_consumer_chd = cur_consumer->xmlChildrenNode;
      while (cur_consumer_chd != NULL) {
        key = xmlNodeListGetString(doc, cur_consumer_chd->xmlChildrenNode, 1);
        if ((!xmlStrcmp(cur_consumer_chd->name, (const xmlChar *)"Server"))) {
          //开内存
          (consumers->consumer+i)->server = calloc(strlen((char *)key), sizeof(char));
          strcpy((consumers->consumer+i)->server, (char *)key);
        }
        else if ((!xmlStrcmp(cur_consumer_chd->name, (const xmlChar *)"Port"))) {
          (consumers->consumer+i)->port = atoi((char *)key);
        }
        else if ((!xmlStrcmp(cur_consumer_chd->name, (const xmlChar *)"Key"))) {
          (consumers->consumer+i)->key = calloc(strlen((char *)key), sizeof(char));
          strcpy((consumers->consumer+i)->key, (char *)key);
        }
        else if ((!xmlStrcmp(cur_consumer_chd->name, (const xmlChar *)"Command"))) {
          (consumers->consumer+i)->command = calloc(strlen((char *)key), sizeof(char));
          strcpy((consumers->consumer+i)->command, (char *)key);
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
