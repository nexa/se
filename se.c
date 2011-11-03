/* 20111008

   spider engine

   search engine prototype
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "event.h"
#include "page.h"
#include "vector.h"
#include "spiders.h"

#define SPIDERS 20

typedef struct sockaddr SA;

struct event_base *base;
spiders_t *spiders;
int flags;

#define msg(msg) printf("%s\n", msg);
#define err_msg(msg) printf("Error: %s\n%s\n%s %d\n", msg, strerror(errno), __FILE__, __LINE__)
#define err_exit(msg) printf("Error: %s\n%s\n%s %d\n", msg, strerror(errno), __FILE__, __LINE__); \
  exit(0)

void on_send_request(int, short, void *);
void on_recv_response(int, short, void *);

void on_send_request(int sock, short event, void *arg)
{
  int error, len;
  spider_t *spider = (spider_t*)arg;
  len = sizeof(error);
  if (getsockopt(sock, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
    {
      err_msg("getsockopt");
      return;
    }
  len = strlen(spider->page->request);
  send(sock, spider->page->request, len, 0);
  bzero(spider->page->content, CONTENTMAX);
  event_set(spider->recv_event, sock, EV_READ, on_recv_response, arg);
  event_base_set(base, spider->recv_event);
  event_add(spider->recv_event, NULL);
}

void on_recv_response(int sock, short event, void *arg)
{
  int n;

  spider_t *spider = (spider_t*)arg;
  page_t *page = spider->page;
  n = recv(sock, page->content + page->content_length, CONTENTMAX - page->content_length, 0);
  page->content_length += n;
  if (n > 0)
    {
      event_set(spider->recv_event, sock, EV_READ, on_recv_response, arg);
      event_base_set(base, spider->recv_event);
      event_add(spider->recv_event, NULL);
      msg(page->content);
    }
}

void on_accept(int sock, short event, void *arg)
{

}

void on_connect(int sock, short event, void *arg)
{
  int error, len;
  spider_t *spider = (spider_t*)arg;
  len = sizeof(error);
  if (getsockopt(sock, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
    {
      err_msg("getsockopt");
      return;
    }
  else
    {
      if (error == 0)
	{
	  fcntl(sock, F_SETFL, flags);
	  event_set(spider->send_event, sock, EV_WRITE, on_send_request, arg);
	  event_base_set(base, spider->send_event);
	  event_add(spider->send_event, NULL);
	}
      else
	{
	  msg("connect failed");
	}
    }
}

void crawl_page(page_t *page)
{
  assert(page != NULL);
  int sock, n;
  spider_t *spider;
  struct sockaddr_in sa;
  struct hostent *hp;

  spider = (spider_t*)malloc(sizeof(spider_t));
  if (spider == NULL) return;
  spider_init(spider);
  spider->page = page;
  spider->send_event = (struct event*)malloc(sizeof(struct event));
  spider->recv_event = (struct event*)malloc(sizeof(struct event));
  
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      err_msg("socket");
      return;
    }
  bzero(&sa, sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons(80);
  
  if ((hp = gethostbyname(page->host)))
    {
      memcpy(&sa.sin_addr.s_addr, hp->h_addr, hp->h_length);
    }
  else
    {
      err_msg("gethostbyname");
      return;
    }
  
  /*
  if (inet_pton(AF_INET, page->host, &sa.sin_addr) <= 0)
    {
      err_msg("inet_pton");
      return;
    }
  */
  flags = fcntl(sock, F_GETFL, 0);
  fcntl(sock, F_SETFL, flags | O_NONBLOCK);

  if ((n = connect(sock, (SA*)&sa, sizeof(sa))) < 0)
    {
      if (errno != EINPROGRESS)
	{
	  err_msg("connect");
	  return;
	}

      spider->sock = sock;
      
      event_set(spider->send_event, sock, EV_WRITE, on_connect, spider);
      event_base_set(base, spider->send_event);
      event_add(spider->send_event, NULL);
    }
  else
    {
      fcntl(sock, F_SETFL, flags);
     
      event_set(spider->send_event, sock, EV_WRITE, on_send_request, spider);
      event_base_set(base, spider->send_event);
      event_add(spider->send_event, NULL);
    }
}

int main(int argc, char **argv)
{
  page_t *page;

  base = event_base_new();
 
  page = (page_t*)malloc(sizeof(page_t));
  page_init(page);

  /*page_set_url(page, "ubuntu.unix-center.net", "ubuntu.unix-center.net", NULL);*/
  page_set_url(page, "192.168.1.5", "192.168.1.5", NULL);
  
  crawl_page(page);

  event_base_dispatch(base);
 
  exit(0);
}
