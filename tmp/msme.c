#include <assert.h>
#include <string.h>
#include "msme.h"

int node_init(node_t *node, int subcount_max)
{
  char *subkeys = NULL;
  NPTR subptrs = NULL;
  assert(node != NULL && subcount_max > 1);
  if (node == NULL || subcount_max <= 1) return -1;
  
  subkeys = (char*)malloc(subcount_max * sizeof(char));
  if (subkeys == NULL) return -1;
  subptrs = (NPTR)malloc(subcount_max * sizeof(NPTR));
  if (subptrs == NULL) return  -1;

  node->id = 0;
  node->beg = 0;
  node->end = 0;
  node->subkeys = subkeys;
  node->subptrs = subptrs;
  node->subcount = 0;
  node->subcount_max = subcount_max;
  node->mode = BUILDING;
  node->state = MATCHING;
  node->type = LEAF;

  return 0;
}

int node_stabilize(node_t *node)
{
  int subkeys_size;
  int subptrs_size;
  char *subkeys = NULL;
  NPTR subptrs = NULL;
  assert(node != NULL && node->mode == BUILDING);
  if (node == NULL || node->node != BUILDING) return -1;

  
  subkeys = (char*)malloc(node->subcount * sizeof(char));
  if (subkeys == NULL) return -1;
  subptrs = (NPTR)malloc(node->subcount * sizeof(NPTR)); 
  if (subptrs == NULL) return  -1;

  memcpy(subkeys, node->subkeys, node->subcount)
  
}

void node_uninit(node_t *node)
{}
