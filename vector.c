/* 20111007
   
   vector

   search engine prototype
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

void vector_init(vector_t *vec, int cap)
{
  assert(vec != NULL && cap > 0);
  if (vec != NULL && cap > 0)
    {
      int i;
      vitem_t *item;
      
      vector_t *vec = (vector_t*)malloc(sizeof(vector_t));
      vec->items = (vitem_t*)malloc(sizeof(vitem_t) * cap);
      vec->capability = cap;
      vec->size = 0;

      for (i = 0;i < cap;i++)
	{
	  item = vec->items + i;
	  item->index = i;
	  item->enable = 0;
	}
    }
}

void vector_uninit(vector_t *vec)
{
  assert(vec != NULL);
  if (vec != NULL)
    {
      free(vec->items);
      free(vec);
    }
}

void vector_clear(vector_t *vec)
{
  assert(vec != NULL);
  if (vec != NULL)
    {
      int i;
      vitem_t *item;
      
      for (i = 0;i < vec->capability;i++)
	{
	  item->enable = 0;
	  item->data = NULL;
	}
      vec->size = 0;
    }
}

int vector_fit(vector_t *vec, void *data)
{
  assert(vec != NULL);
  if (vec != NULL)
    {
      int i;
      vitem_t *item;
      
      for (i = 0;i < vec->capability;i++)
	{
	  item = vec->items + i;
	  if (item->enable == 0)
	    {
	      item->data = data;
	      item->enable = 1;
	      vec->size++;
	      return item->index;
	    }
	}
    }
  return -1;
}

void vector_enable(vector_t *vec, int index)
{
  assert(vec != NULL && index >= 0 && index < vec->capability);
  if (vec != NULL && index >= 0 && index < vec->capability)
    {
      vitem_t *item = vec->items + index;
      item->enable = 0;
      item->data = NULL;
      vec->size--;
    }
}

void * vector_get_data(vector_t *vec, int index)
{
  assert(vec != NULL && index >= 0 && index < vec->capability);
  if (vec != NULL && index >= 0 && index < vec->capability)
    {
      vitem_t *item = vec->items + index;
      return item->data;
    }
  return NULL;
}

int vector_next(vector_t *vec, int index)
{
  assert(vec != NULL && index >= 0 && index < (vec->capability - 1));
  if (vec != NULL && index >= 0 && index < (vec->capability - 1))
    {
      int i = index + 1;
      vitem_t *item;
      
      while (i < vec->capability)
	{
	  item = vec->items + i;
	  if (item->enable)
	    return i;
	  i++;
	}
    }
  return -1;
}
