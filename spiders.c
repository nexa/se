/* 20111010
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "spiders.h"

void spider_init(spider_t *spider)
{
  assert(spider != NULL);
  if (spider != NULL)
    {
      spider->send_event = (struct event*)malloc(sizeof(struct event));
      spider->recv_event = (struct event*)malloc(sizeof(struct event));
      spider->sock = 0;
      spider->state = 0;
    }
}

void spider_uninit(spider_t *spider)
{
  assert(spider != NULL);
  if (spider != NULL)
    {
      event_del(spider->recv_event);
      free(spider->send_event);
      free(spider->recv_event);
      free(spider);
    }
}

void spiders_init(spiders_t *spiders, int num)
{
  assert(spiders != NULL && num > 0);
  if (spiders != NULL && num > 0)
    {
      vector_t *vec = (vector_t*)malloc(sizeof(vector_t));
      vector_init(vec, num);
      spiders->spiders = vec;
      spiders->listen_event = (struct event*)malloc(sizeof(struct event));
      spiders->state = 0;
    }
}

void spiders_clear(spiders_t *spiders)
{
  assert(spiders != NULL);
  if (spiders != NULL)
    {
      event_del(spiders->listen_event);
      free(spiders->spiders);
    }
}
