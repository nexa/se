#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

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
  me->key_count = count;
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
  node_t *node, *tmp;
  int i, j, key_len;
  char *key_str, *node_str;

  tmp = (node_t*)malloc(sizeof(node_t));
  node_init(tmp);
  tmp->beg = -1;
  tmp->end = -1;
  tmp->id = -1;
  me->root = tmp;

  node = me->root;
  for (i = 0;i < me->key_count;i++)
    {
      j = 0;
      key_str = me->keys[i];
      key_len = strlen(key_str);
      
      while (1)
	{
	  if (j < key_len)
	    {
	      if (j > node->end)
		{
		  tmp = (node_t*)maplite_get(&node->childs, (int)key_str[j]);
		  if (tmp == NULL)
		    {
		      hdrme_reproduce(me, node, i, key_len - 1);
		      node = me->root;
		      break;
		    }
		  else
		    {
		      node = tmp;
		      node->mi = node->beg;
		    }
		}	   
	      else if (j >= node->beg && j <= node->end)
		{
		  node_str = me->keys[node->id];
		  if (key_str[j] == node_str[node->mi + 1])
		    {
		      node->mi += 1;
		    }
		  else
		    {
		      tmp = hdrme_division(me, node, node->mi);
		      hdrme_reproduce(me, tmp, i, key_len - 1);
		      node = me->root;
		      break;
		    }
		}
	      else {/*error*/}
	    }
	  else
	    {
	      if (node->mi < node->end)
		{
		  tmp = hdrme_division(me, node, node->mi);
		}
	      else if (node->mi == node->end){}
	      else {/*error*/}
	      node = me->root;
	      break;
	    }
	  j++;
	}
    }

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
  int index, mark, len;
  node_t *node;
  char *node_str;

  assert(me != NULL && txt != NULL);
  if (me == NULL || txt == NULL) return -1;

  me->root->state = PENDING;
  
  index = 0;
  mark = 0;
  len = strlen(txt);
  node = me->root;
  while (index < len)
    {
      if (node->state == PENDING)
	{
	  if (node == me->root) mark = index;
	  node = maplite_get(&node->childs, (int)(txt[index]));
	  if (node == NULL) node = me->root;
	  if (node != me->root)
	    {
	      node->mi = node->beg;
	      node->state = MATCHING;
	      node_str = me->keys[node->id];
	    }
	}
      if (node->state == MATCHING)
	{
	  if (node->mi <= node->end)
	    {
	      if (txt[index] == node_str[node->mi])
		{
		  if (node->mi == node->end)
		    {
		      if ((node->type & LEAF) == LEAF)
			{
			  /*got it!*/
			  printf("Find key:%4d at %4d to %4d.\n", node->id, mark, index);
			  node->state = MATCHING;
			  node = me->root;
			  node->state = PENDING;
			}
		      else
			{
			  node->state = PENDING;
			}
		    }
		  else
		    {
		      node->mi++;
		    }
		}
	      else
		{
		  node = me->root;
		  node->state = PENDING;
		}
	    }
	}
      index++;
    }

  return 0;
}

node_t *hdrme_division(hdrme_t *me, node_t *node, int break_index)
{
  node_t *new;
  char *key_str;
  assert(me != NULL && node != NULL && break_index <= (node->end - 1));
  if (me == NULL || node == NULL || break_index > (node->end -1)) return NULL;

  new = (node_t*)malloc(sizeof(node_t));
  node_init(new);
  new->id = node->id;
  new->beg = node->beg;
  new->end = break_index;
  new->type = BRANCH;
  new->parent = node->parent;

  node->beg = break_index + 1;
  node->parent = new;

  key_str = me->keys[node->id];
  maplite_set(&new->childs, (int)key_str[node->beg], (void*)node);
  maplite_set(&new->parent->childs, (int)key_str[new->beg], (void*)new);

  return new;
}

node_t *hdrme_reproduce(hdrme_t *me, node_t *node, int new_id, int new_end)
{
  node_t *new;
  char *key_str;
  assert(me != NULL && node != NULL);
  assert(new_id > node->id && new_id < me->key_count);
  if (me == NULL || node == NULL) return NULL;
  if (new_id <= node->id || new_id >= me->key_count) return NULL;

  key_str = me->keys[new_id];
  if (new_end < strlen(key_str))
    {
      new = (node_t*)malloc(sizeof(node_t));
      node_init(new);
      new->id = new_id;
      new->beg = node->end + 1;
      new->end = new_end;
      new->parent = node;

      node->type = BRANCH;

      maplite_set(&node->childs, (int)key_str[new->beg], (void*)new);
      
      return new;
    }
  return NULL;
}
