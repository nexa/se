/* 20111008
  
   page

   search engine prototype
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "page.h"

void page_init(page_t *page)
{
  assert(page != NULL);
  if (page != NULL)
    {
      page->url = NULL;
      page->content = (char*)malloc(CONTENTMAX);
      page->content_length = 0;
      page->state = 0;
      page->signature = 0;
    }
}

void page_free(page_t *page)
{
  assert(page != NULL);
  if (page != NULL)
    {
      free(page->url);
      free(page->host);
      if (page->path != NULL)
	free(page->path);
      free(page->content);
      free(page);
    }
}

void page_set_url(page_t *page, char *url, char *host, char *path)
{
  assert(page != NULL && url != NULL && host != NULL);
  int len = strlen(url) + 1;
  assert(len > 1 && len <= URLMAX);
  if (page != NULL && url != NULL && len > 1 && len <= URLMAX)
    {
      int len;
      char cmd[256];
      bzero(cmd, 256);
      page->url = (char*)malloc(strlen(url) + 1);
      strcpy(page->url, url);
      page->host = (char*)malloc(strlen(host) + 1);
      strcpy(page->host, host);
      if (path != NULL && ((len = strlen(path)) > 0))
	{
	  page->path = (char*)malloc(len + 1);
	  strcpy(page->path, path);
	  sprintf(cmd, "GET /%s HTTP/1.1\r\n\r\n", path);
	}
      else
	{
	  sprintf(cmd, "GET /%s HTTP/1.1\r\nHost:\r\nConnection:Keep-Alive\n\n", "index.htm");
	}
      len = strlen(cmd) + 1;
      page->request = (char*)malloc(len);
      bzero(page->request, len);
      strcpy(page->request, cmd);
    }
}
