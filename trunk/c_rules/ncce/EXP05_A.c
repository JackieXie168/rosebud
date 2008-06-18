#include <stdlib.h>

void remove_spaces(char const *str, size_t slen) {
   char *p = (char *)str;
   size_t i;
   for (i = 0; i < slen && str[i]; i++) {
      if (str[i] != ' ') *p++ = str[i];
   }
   *p = '\0';
}
