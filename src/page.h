/* 20111008

   page:
   
   search engine prototype
*/
#ifndef _PAGE_H_
#define _PAGE_H_

#define URLMAX 256
#define CONTENTMAX 20480

typedef struct _page_t
{
  char *url;
  char *host;
  char *path;
  char *content;
  char *request;
  int content_length;
  int state;
  int signature;
}page_t;

void page_init(page_t *);
void page_uninit(page_t *);

void page_set_url(page_t *, char *, char *, char*);

#endif
