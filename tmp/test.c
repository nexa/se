#include <stdio.h>
#include <stdlib.h>
#include "http.h"

char sz1[] = "ab";
char sz2[] = "ac";
char sz3[] = "acbe";

char txt[] = "ab:12;ac:21;";

int main(int argc, char **argv)
{
  hdrme_t me;
  char *keys[] = {sz1, sz2};

  hdrme_init(&me, keys, 3);
  hdrme_compile(&me);

  hdrme_parse(&me, txt);

  hdrme_dismiss(&me);
  hdrme_uninit(&me);

  exit(0);
}
