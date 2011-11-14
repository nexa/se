#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "msme.h"

int node_init(node_t *node, int child_count_max)
{
  unsigned char *childkeys = NULL;
  NPTR *childptrs = NULL;
  assert(node != NULL && child_count_max > 1);
  if (node == NULL || child_count_max <= 1) return -1;
  
  childkeys = (unsigned char*)malloc(child_count_max * sizeof(unsigned char));
  if (childkeys == NULL) return -1;
  childptrs = (NPTR*)malloc(child_count_max * sizeof(NPTR));
  if (childptrs == NULL) return  -1;

  node->id = -1;
  node->beg = 0;
  node->end = 0;
  node->child_keys = childkeys;
  node->child_ptrs = childptrs;
  node->child_count = 0;
  node->child_count_max = child_count_max;
  node->mode = BUILDING;
  node->state = DEFAULT;
  node->type = LEAF;
  node->parent_ptr = NULL;

  return 0;
}

int node_stabilize(node_t *node)
{
  int child_keys_size;
  int child_ptrs_size;
  unsigned char *child_keys = NULL;
  NPTR *child_ptrs = NULL;
  assert(node != NULL && node->mode == BUILDING);
  if (node == NULL || node->mode != BUILDING) return -1;
  if (node->child_count == 0) return -1;

  child_keys_size = node->child_count * sizeof(unsigned char);
  child_ptrs_size = node->child_count * sizeof(NPTR);
  child_keys = (unsigned char*)malloc(child_keys_size);
  if (child_keys == NULL) return -1;
  child_ptrs = (NPTR*)malloc(child_ptrs_size); 
  if (child_ptrs == NULL) return  -1;

  memcpy(child_keys, node->child_keys, child_keys_size);
  memcpy(child_ptrs, node->child_ptrs, child_ptrs_size);
  
  free (node->child_keys);
  free (node->child_ptrs);

  node->child_keys = child_keys;
  node->child_ptrs = child_ptrs;

  return 0;
}

void node_uninit(node_t *node)
{
  assert(node != NULL);
  if (node == NULL) return;
  
  free (node->child_keys);
  free (node->child_ptrs);
}

node_t * node_fork(node_t *node)
{
  unsigned char *childkeys = NULL;
  NPTR *childptrs = NULL;
  assert(node != NULL);
  if (node == NULL) return;

  NPTR node_new = (NPTR)malloc(sizeof(node_t));
  if (node_new == NULL) return NULL;

  childkeys = (unsigned char*)malloc(node->child_count_max * sizeof(unsigned char));
  if (childkeys == NULL) return NULL;
  childptrs = (NPTR*)malloc(node->child_count_max * sizeof(NPTR));
  if (childptrs == NULL) return NULL;

  node_new->child_keys = childkeys;
  node_new->child_ptrs = childptrs;

  node_new->id = node->id;
  node_new->beg = node->beg;
  node_new->end = node->end - 1;
  node_new->child_keys[0] = node->end;
  node_new->child_ptrs[0] = node;
  node_new->child_count = 1;
  node_new->child_count_max = node->child_count_max;
  node_new->mode = BUILDING;
  node_new->state = DEFAULT;
  node_new->type = BRANCH;
  node_new->parent_ptr = node->parent_ptr;
  
  node->beg = node->end;
  node->parent_ptr = node_new;
}

/*
node_t *tree_build(char **ss, int count)
{
  NPTR nptrs;
  assert(ss != NULL && count > 0);
  if (ss == NULL || count <= 0) return -1;

  
  
}
*/

int main(int argc, char **argv)
{

  exit(0);
}
