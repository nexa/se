/* http header parser

   history:
   20111108: create
 */
#ifndef _HTTPHDR_H_
#define _HTTPHDR_H_

typedef enum _state
  {
    DEFAULT = 0,
    MATCHING,
    PENDING,
    MATCHED,
  }STATE;

typedef enum _type
  {
    ROOT = 0,
    BRANCH,
    LEAF,
  }TYPE;

typedef struct _node_t
{
  int id;
  char *body;
  int body_len;
  STATE state;
  TYPE type;
  char *subnode_keys;
  struct _node_t *subnode_ptrs;
  int subnode_count;
}node_t;

node_t * tree_build(int *ids, char **hdrs);

#endif
