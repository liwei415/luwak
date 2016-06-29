#include "lwk_string.h"

size_t ox_strlcpy(char *dst, const char *src, size_t size) {
  size_t srclen;
  // Figure out how much room is needed...
  size --;
  srclen = strlen(src);

  // Copy the appropriate amount...
  if (srclen > size) {
    srclen = size;
  }

  memcpy(dst, src, srclen);
  dst[srclen] = '\0';

  return (srclen);
}
