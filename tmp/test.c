#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "http.h"

char sz1[] = "ab";
char sz2[] = "ac";
char sz3[] = "acbe";

char txt[] = "ab:12\nad\nac:213\n";
char ht[] = "HTTP/1.1 200 OK\nab:12\nadab:12\nad\nac:213\n";

int main(int argc, char **argv)
{
  http_t me;
  int res;
  char *keys[] = {sz1, sz2};



 
  http_init(&me, keys, 2);
 

  http_hypertext_attach(&me, ht);
  http_hypertext_receving(&me, strlen(ht));

  res = http_parse_result(&me);

  /*  http_hypertext_detach(&me);*/



  hdr_parser_compile(&me);

  me.hdr_parser_current = me.hdr_parser_root;

  http_parse_hdr(&me);

  hdr_parser_dismiss(&me);
  http_uninit(&me);

  exit(0);
}
