/* http parser

   histroy:
   20111103: create
*/
#ifndef _HTTP_H_
#define _HTTP_H_

typedef struct _tree_node_t tree_node_t, node_t;
typedef struct _http_t http_t;

/* header process runtines
 */
typedef int (*FUNC)(http_t *, node_t *);

/* header process tree node state
 */
typedef enum _NODE_STATE
  {
    IGNORING = -1,
    MATCHING = 0,
    PENDING = 1,
    MATCHED = 2,
  }NODE_STATE;

/* tree node type
 */
typedef enum _NODE_TYPE
  {
    ROOT = 0,
    PATH,
    ENTRY,
  }NODE_TYPE;

/* tree node
 */
struct _tree_node_t
{
  char *body;
  int body_len;
  char *subnode_keys;
  node_t *subnode_ptrs;
  int subnode_count;
  
  NODE_TYPE type;
  NODE_STATE state;
  
  int in_beg;
  int in_end;
  FUNC func1;
  FUNC func2;
};

/* _http_t
 */
struct _http_t
{
  char *ptr_buf;
  int *ptr_len;
  int index;
  node_t *entry_root;
  node_t *entry_current;
  int entry_beg_tmp;
};



#endif
