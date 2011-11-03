/* 20111008

   spider engine

   search engine prototype
*/
#ifndef _SPIDERS_H_
#define _SPIDERS_H_

#include <assert.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "event.h"
#include "page.h"
#include "vector.h"

#define PAGEMAX 128
#define PORT 80
#define BACKLOG 5

typedef struct _spider_t
{
  struct event *send_event;
  struct event *recv_event;
  int sock;
  page_t *page;
  int state;
}spider_t;

typedef struct _spiders_t
{
  vector_t *spiders;
  struct event *listen_event;
  int state;
}spiders_t;


void spider_init(spider_t *);
void spider_uninit(spider_t *);

void spiders_init(spiders_t *, int);
void spiders_clear(spiders_t *);

#endif
