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

typedef struct node_s
{
  int id;
  int beg;
  int end;
  int mi;
  maplite_t childs;
  STATE state;
  TYPE type;
  struct node_s *parent;
}node_t;

typedef struct hdrme_s
{
  char **keys;
  int key_count;
  struct node_s *root;
}hdrme_t;

int node_init(node_t *node);
void node_uninit(node_t *node);

int hdrme_init(hdrme_t *me, char **keys, int count);
void hdrme_uninit(hdrme_t *me);
int hdrme_compile(hdrme_t *me);
void hdrme_dismiss(hdrme_t *me);
int hdrme_parse(hdrme_t *me, char *txt);
node_t *hdrme_division(hdrme_t *me, node_t *node, int break_index);
node_t *hdrme_reproduce(hdrme_t *me, node_t *node, int new_id, int new_end);

#endif
