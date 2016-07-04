#ifndef _LWK_XML_
#define _LWK_XML_

//#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "lwk_common.h"
#include "lwk_str.h"


lwk_xml_t *lwk_xml_load(char *conf);
lwk_redis_consumers_t *lwk_xml_load_redis(char *conf);
lwk_rabbit_consumers_t *lwk_xml_load_rabbit(char *conf);

#endif
