/* http parser

   histroy:
   20111103: create
*/
#ifndef _HTTP_H_
#define _HTTP_H_

#include "maplite.h"

typedef enum _STATE
  {
    DEFAULT = 0,
    MATCHING,
    PENDING,
    MATCHED,
  }STATE;

typedef enum _TYPE
  {
    ROOT = 0x1,
    BRANCH = 0x2,
    LEAF = 0x4,
  }TYPE;

typedef struct hdr_parser_s
{
  int id;
  int beg;
  int end;
  int mi;
  maplite_t childs;
  STATE state;
  TYPE type;
  struct hdr_parser_s *parent;
}hdr_parser_t;

typedef struct http_s
{
  char *version;
  char *code;
  char *state;
  char **hdr_names;
  int hdr_count;
  struct hdr_parser_s *hdr_parser_root;
  char *hypertext;
  int hypertext_length;
  int mi;
}http_t;

int hdr_parser_init(hdr_parser_t *hdr);
void hdr_parser_uninit(hdr_parser_t *hdr);
int hdr_parser_compile(http_t *http);
void hdr_parser_dismiss(http_t *http);
hdr_parser_t *hdr_parser_division(http_t *http, hdr_parser_t *hdr, int break_index);
hdr_parser_t *hdr_parser_reproduce(http_t *http, hdr_parser_t *hdr, int new_id, int new_end);

int http_init(http_t *http, char **hdr_names, int hdr_count);
void http_uninit(http_t *http);
int http_parse_result(http_t *http);
int http_parse_hdr(http_t *http, char *txt);
void http_hypertext_attach(http_t *http, char* hypertext);
void http_hypertext_detach(http_t *http);
int http_hypertext_receving(http_t *http, int hypertext_len);

#endif
