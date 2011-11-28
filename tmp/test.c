#include <stdio.h>
#include <stdlib.h>
#include "http.h"

char sz1[] = "ab";
char sz2[] = "ac";
char sz3[] = "acbe";

char txt[] = "ab:12;ac:21;";

int main(int argc, char **argv)
{
  http_t me;
  char *keys[] = {sz1, sz2};

  http_init(&me, keys, 2);
  hdr_parser_compile(&me);

  http_parse(&me, txt);

  hdr_parser_dismiss(&me);
  http_uninit(&me);

  exit(0);
}
