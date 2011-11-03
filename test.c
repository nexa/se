/* 20111007

   test case

   search engine prototype
*/

#include <stdio.h>
#include <stdlib.h>

#include "queue.h"
#include "vector.h"

void pqi(queue_t *queue)
{
  printf("Queue:%16X %16d\n", (int)queue, queue_size(queue));
}

void pqii(qitem_t *item)
{
  printf("Item:%16X %16d\n", (int)item, (int)item->data);
}

void  queue()
{
  queue_t queue, *q;
  queue_item_t *item;

  q = &queue;
  queue_init(q);

  item = (queue_item_t*)malloc(sizeof(queue_item_t));
  queue_item_init(item);
  item->data = (void*)0;
  pqii(item);
  queue_push(q, item);
  pqi(q);
  item = queue_pop(q);
  pqii(item);
  pqi(q);
  queue_item_uninit(item);

  queue_clear(&queue);
}

void pvi(vector_t *vec)
{
  printf("Vector: %16X %16d\n", (int)vec, vec->size);
}

void pvii(vector_t *vec, int index)
{
  vitem_t *item;
  item = vec->items + index;
  printf("VItem: %16d %16d %16d\n", (int)item->data, item->index, item->enable);
}

void vector()
{
  vector_t vec, *v;
  v = &vec;

  vector_init(v, 10);
  vector_fit(v, (void*)1);
  vector_fit(v, (void*)2);
  vector_fit(v, (void*)3);
  vector_fit(v, (void*)4);

 

  pvi(v);
  pvii(v, 2);

  vector_enable(v, 2);

  pvi(v);
  pvii(v, 2);
  vector_fit(v, (void*)5);
  pvi(v);
  pvii(v, 2);

  vector_clear(v);
}

int main(int argc, char** argv)
{
  queue();
  vector();

  exit(0);
}
