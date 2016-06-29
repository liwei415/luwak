#ifndef _LWK_CONF_
#define _LWK_CONF_

//#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "lwk_common.h"


lwk_consumers_t *lwk_conf_load_consumers(char *conf);

#endif
