/* 20111007

   queue
   
   search engine prototype
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

void queue_item_init(queue_item_t *item)
{
  assert(item != NULL);
  if (item != NULL)
    {
      item->prev = NULL;
      item->next = NULL;
      item->data = NULL;
    }
  return item;
}

void queue_item_uninit(queue_item_t *item)
{
  assert(item != NULL);
  if (item != NULL)
    {
      free(item);
    }
}

void queue_init(queue_t *queue)
{
  assert(queue != NULL);
  if (queue == NULL) return;

  queue->head = NULL;
  queue->tail = NULL;
  queue->size = 0;
}

void queue_uninit(queue_t *queue)
{
  assert(queue != NULL);
}

void queue_clear(queue_t *queue)
{
  assert(queue != NULL);
  if (queue == NULL) return;

  if (queue->size != 0)
    {
      qitem_t *item;
      while (queue->head != NULL)
	{
	  item = queue->head;
	  queue->head = item->next;
	  queue->head->prev = NULL;
	  queue_item_uninit(item);
	}
    }
  queue->tail = NULL;
  queue->size = 0;
}

void queue_push(queue_t *queue, queue_item_t *item)
{
  assert(queue != NULL);
  assert(item != NULL);
  if (queue != NULL && item != NULL)
    {
      item->next = NULL;
      if (queue->tail == NULL)
	{
	  item->prev = NULL;
	  queue->head = item;
	}
      else
	{
	  item->prev = queue->tail;
	  queue->tail->next = item;
	}
      queue->tail = item;
      queue->size++;
    }
}

queue_item_t * queue_pop(queue_t *queue)
{
  assert(queue != NULL);
  if (queue != NULL && queue->head != NULL)
    {
      qitem_t *item = queue->head;
      if (queue->size == 1)
	{
	  queue->head = NULL;
	  queue->tail = NULL;
	}
      else
	{
	  queue->head = queue->head->next;
	  queue->head->prev = NULL;
	  item->next = NULL;
	}
      queue->size--;
      return item;
    }
  return NULL;
}

int queue_size(queue_t *queue)
{
  assert(queue != NULL);
  if (queue != NULL)
    return queue->size;
  return -1;
}    


