#include <stdio.h>
#include <stdlib.h>
#include "http.h"

char sz1[] = "abcd";
char sz2[] = "abce";
char sz3[] = "acbe";

char txt[] = "abcd1abce3acbeabcd";

int main(int argc, char **argv)
{
  hdrme_t me;
  char *keys[] = {sz1, sz2, sz3};

  hdrme_init(&me, keys, 3);
  hdrme_compile(&me);

  hdrme_parse(&me, txt);

  hdrme_dismiss(&me);
  hdrme_uninit(&me);

  exit(0);
}
