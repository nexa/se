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
  unsigned char *child_keys;
  struct node_s **child_ptrs;
  int child_count;
  int child_count_max;
  MODE mode;
  STATE state;
  TYPE type;
  struct node_s *parent_ptr;
}node_t, *NPTR;

typedef struct msme_s
{
  char **strings;
  unsigned char *indexs;
  node_t **nodes;
  unsigned char capility;
  node_t *root;
  MODE mode;
}msme_t;

int node_init(node_t *node, int child_count_max);
int node_stabilize(node_t *node);
void node_uninit(node_t *node);
node_t * node_fork(node_t *node);

int msme_init(msme_t *msme, char **strings, unsigned char string_count);
void msme_uninit(msme_t *msme);
int msme_compile(msme_t *msme);

#endif
