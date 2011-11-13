/* msme

   multi strings match engine
   
   histroy:
   20111110: create
 */
#ifndef _MSME_H_
#define _MSME_H_

typedef enum _STATE
  {
    DEFAULT = 0,
    MATCHING,
    PENDING,
    MATCHED,
  }STATE;

typedef enum _TYPE
  {
    ROOT = 0,
    BRANCH,
    LEAF,
  }TYPE;

typedef enum _MODE
  {
    BUILDING = 0,
    STABLE,
  }MODE;

typedef struct node_s
{
  int id;
  int beg;
  int end;
  unsigned char *child_eys;
  struct node_s *child_ptrs;
  int child_count;
  int child_count_max;
  MODE mode;
  STATE state;
  TYPE type;
  struct node_t *parent_ptr;
}node_t, *NPTR;

int node_init(node_t *node, int child_count_max);
int node_stabilize(node_t *node);
void node_uninit(node_t *node);
node_t * node_fork(node_t *node);

node_t * tree_build(char **ss, int count);

#endif
