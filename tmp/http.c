#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "http.h"

int node_init(node_t *node)
{
  assert(node != NULL);
  if (node == NULL) return -1;

  node->id = -1;
  node->beg = -1;
  node->end = -1;
  node->mi = -1;
  node->state = DEFAULT;
  node->type = LEAF;
  node->parent = NULL;

  maplite_init(&node->childs, ASCII);

  return 0;
}

void node_uninit(node_t *node)
{
  int i;
  assert(node != NULL);
  if (node == NULL) return;

  for (i = 0;i < (node->childs).capility;i++)
    {
      if ((node->childs).values[i] != NULL)
	{
	  node_uninit((node_t*)(node->childs).values[i]);
	  free ((node->childs).values[i]);
	}
    }  

  maplite_uninit(&node->childs);
}

int hdrme_init(hdrme_t *me, char **keys, int count)
{
  int memsize;
  assert(me != NULL && keys != NULL && count >= 1);
  if (me == NULL || keys == NULL || count < 1) return -1;

  memsize = count * sizeof(char*);
  me->keys = (char**)malloc(memsize);
  if (me->keys == NULL) return -1;
  memcpy(me->keys, keys, memsize);
  me->keycount = count;
  me->root = NULL;

  return 0;
}
  
void hdrme_uninit(hdrme_t *me)
{
  assert(me != NULL);
  if (me == NULL) return;

  free (me->keys);
}
  
int hdrme_compile(hdrme_t *me)
{
  return 0;
}  

void hdrme_dismiss(hdrme_t *me)
{
  assert(me != NULL);
  if (me == NULL) return;

  node_uninit(me->root);
  free (me->root);
  me->root = NULL;
}

int hdrme_parse(hdrme_t *me, char *txt)
{
  return 0;
}
