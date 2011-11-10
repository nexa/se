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
  char *subkeys;
  struct node_s *subptrs;
  int subcount;
  int subcount_max;
  MODE mode;
  STATE state;
  TYPE type;
}node_t, *NPTR;

int node_init(node_t *node, int subcount_max);
int node_stabilize(node_t *node);
void node_uninit(node_t *node);

node_t *tree_build(char **ss, int count);

#endif
