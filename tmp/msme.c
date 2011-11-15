#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "msme.h"

int node_init(node_t *node, int child_count_max)
{
  unsigned char *childids = NULL;
  NPTR *childptrs = NULL;
  assert(node != NULL && child_count_max > 1);
  if (node == NULL || child_count_max <= 1) return -1;
  
  childids = (unsigned char*)malloc(child_count_max * sizeof(unsigned char));
  if (childids == NULL) return -1;
  childptrs = (NPTR*)malloc(child_count_max * sizeof(NPTR));
  if (childptrs == NULL) return  -1;

  node->id = -1;
  node->beg = 0;
  node->end = 0;
  node->child_ids = childids;
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
  int child_ids_size;
  int child_ptrs_size;
  unsigned char *child_ids = NULL;
  NPTR *child_ptrs = NULL;
  assert(node != NULL && node->mode == BUILDING);
  if (node == NULL || node->mode != BUILDING) return -1;
  if (node->child_count == 0) return -1;

  child_ids_size = node->child_count * sizeof(unsigned char);
  child_ptrs_size = node->child_count * sizeof(NPTR);
  child_ids = (unsigned char*)malloc(child_ids_size);
  if (child_ids == NULL) return -1;
  child_ptrs = (NPTR*)malloc(child_ptrs_size); 
  if (child_ptrs == NULL) return  -1;

  memcpy(child_ids, node->child_ids, child_ids_size);
  memcpy(child_ptrs, node->child_ptrs, child_ptrs_size);
  
  free (node->child_ids);
  free (node->child_ptrs);

  node->child_ids = child_ids;
  node->child_ptrs = child_ptrs;

  return 0;
}

void node_uninit(node_t *node)
{
  assert(node != NULL);
  if (node == NULL) return;
  
  free (node->child_ids);
  free (node->child_ptrs);
}

node_t * node_fork(node_t *node)
{
  unsigned char *childids = NULL;
  NPTR *childptrs = NULL;
  assert(node != NULL);
  if (node == NULL) return;

  NPTR node_new = (NPTR)malloc(sizeof(node_t));
  if (node_new == NULL) return NULL;

  childids = (unsigned char*)malloc(node->child_count_max * sizeof(unsigned char));
  if (childids == NULL) return NULL;
  childptrs = (NPTR*)malloc(node->child_count_max * sizeof(NPTR));
  if (childptrs == NULL) return NULL;

  node_new->child_ids = childids;
  node_new->child_ptrs = childptrs;

  node_new->id = node->id;
  node_new->beg = node->beg;
  node_new->end = node->end - 1;
  node_new->child_ids[0] = node->id;
  node_new->child_ptrs[0] = node;
  node_new->child_count = 1;
  node_new->child_count_max = node->child_count_max;
  node_new->mode = BUILDING;
  node_new->state = DEFAULT;
  node_new->type = BRANCH;
  node_new->parent_ptr = node->parent_ptr;
  
  node->beg = node->end;
  node->end = node->beg;
  node->parent_ptr = node_new;
}

int msme_init(msme_t *msme, char **strings, unsigned char string_count)
{
  char **_strings;
  int _strings_size;
  unsigned char *_indexs;
  int _indexs_size;
  node_t **_nodes;
  int _nodes_size;
  
  int i;

  assert(msme != NULL && strings != NULL && string_count > 0);
  if (msme == NULL || strings == NULL || string_count <= 0) return -1;

  _strings_size = string_count *sizeof(char*);
  _strings = (char**)malloc(_strings_size);
  if (_strings == NULL) return -1;
  
  _indexs_size = string_count * sizeof(unsigned char);
  _indexs = (unsigned char*)malloc(_indexs_size);
  if (_indexs == NULL) return -1;
  
  _nodes_size = string_count * sizeof(node_t*);
  _nodes = (node_t**)malloc(_nodes_size);
  if (_nodes == NULL) return -1;

  memcpy(_strings, strings, _strings_size);
  
  for (i = 0;i < string_count;i++)
    {
      _indexs[i] = i;
      _nodes[i] = NULL;
    }

  msme->strings = _strings;
  msme->indexs = _indexs;
  msme->nodes = _nodes;
  msme->capility = string_count;
  msme->root = NULL;
  msme->mode = BUILDING;
  msme->state = INITIALIZED;

  return 0;
}

void msme_uninit(msme_t *msme)
{  
  assert(msme != NULL);
  if (msme == NULL) return;
  
  free (msme->strings);
  free (msme->indexs);
  free (msme->nodes);
}

int msme_compile(msme_t *msme)
{
  node_t *_root, *_first;

  int i, res1, res2;

  assert(msme != NULL && msme->state == INITIALIZED);
  if (msme == NULL || msme->state != INITIALIZED) return -1;

  _root = (NPTR)malloc(sizeof(node_t));
  _first = (NPTR)malloc(sizeof(node_t));
  if (_root == NULL || _first == NULL) return -1;
  
  res1 = node_init(_root, msme->capility);
  res2 = node_init(_first, msme->capility);
  if (res1 != 0 || res2 != 0) return -1;

  for (i = 0;i < msme->capility;i++)
    {
      msme->nodes[i] = _root;
    }

  _root->child_ids[0] = 0;
  

  return 0;
}

void msme_dismiss(msme_t *msme)
{

}

int main(int argc, char **argv)
{

  exit(0);
}
