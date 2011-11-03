/* 20111007

   queue
   
   search engine prototype
*/
#ifndef _QUEUE_H_
#define _QUEUE_H_

typedef struct _queue_item_t
{
  struct _queue_item_t *prev;
  struct _queue_item_t *next;
  void *data;
}queue_item_t, qitem_t;

typedef struct _queue_t
{
  struct _queue_item_t *head;
  struct _queue_item_t *tail;
  int size;
}queue_t, q_t;

void queue_item_init(queue_item_t *);
void queue_item_uninit(queue_item_t *);

void queue_init(queue_t *);
void queue_uninit(queue_t *);
void queue_clear(queue_t *);

void queue_push(queue_t *, queue_item_t *);
queue_item_t* queue_pop(queue_t *);

int queue_size(queue_t *);

#endif
