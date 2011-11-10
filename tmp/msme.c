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

  node->id = -1;
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
  if (node->subcount == 0) return -1;

  subkeys_size = node->subcount * sizeof(char);
  subptrs_size = node->subcount * sizeof(NPTR);
  subkeys = (char*)malloc(subkeys_size);
  if (subkeys == NULL) return -1;
  subptrs = (NPTR)malloc(subptrs_size); 
  if (subptrs == NULL) return  -1;

  memcpy(subkeys, node->subkeys, subkeys_size);
  memcpy(subptrs, node->subptrs, subptrs_size);
  
  free (node->subkeys);
  free (node->subptrs);

  node->subkeys = subkeys;
  node->subptrs = subptrs;

  return 0;
}

void node_uninit(node_t *node)
{
  assert(node != NULL);
  if (node == NULL) return;
  
  free (node->subkeys);
  free (node->subptrs);
}

node_t *tree_build(char **ss, int count)
{
  NPTR nptrs;
  assert(ss != NULL && count > 0);
  if (ss == NULL || count <= 0) return -1;


  
}
