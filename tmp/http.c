#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "http.h"

int hdr_parser_init(hdr_parser_t *hdr)
{
  assert(hdr != NULL);
  if (hdr == NULL) return -1;

  hdr->id = -1;
  hdr->beg = -1;
  hdr->end = -1;
  hdr->mi = -1;
  hdr->state = DEFAULT;
  hdr->type = LEAF;
  hdr->parent = NULL;

  maplite_init(&hdr->childs, ASCII);

  return 0;
}

void hdr_parser_uninit(hdr_parser_t *hdr)
{
  int i;
  assert(hdr != NULL);
  if (hdr == NULL) return;

  for (i = 0;i < (hdr->childs).capility;i++)
    {
      if ((hdr->childs).values[i] != NULL)
	{
	  hdr_parser_uninit((hdr_parser_t*)(hdr->childs).values[i]);
	  free ((hdr->childs).values[i]);
	}
    }  

  maplite_uninit(&hdr->childs);
}

int http_init(http_t *http, char **hdr_names, int hdr_count)
{
  int memsize;
  assert(http != NULL && hdr_names != NULL && hdr_count >= 1);
  if (http == NULL || hdr_names == NULL || hdr_count < 1) return -1;

  memsize = hdr_count * sizeof(char*);
  http->hdr_names = (char**)malloc(memsize);
  if (http->hdr_names == NULL) return -1;
  memcpy(http->hdr_names, hdr_names, memsize);
  http->hdr_count = hdr_count;
  http->hdr_parser_root = NULL;
  http->hdr_parser_current = NULL;
  
  return 0;
}
  
void http_uninit(http_t *http)
{
  assert(http != NULL);
  if (http == NULL) return;

  free (http->hdr_names);
}
  
int hdr_parser_compile(http_t *http)
{
  hdr_parser_t *hdr, *tmp;
  int i, j, hdr_len;
  char *hdr_str, *hdr_parser_str;

  tmp = (hdr_parser_t*)malloc(sizeof(hdr_parser_t));
  hdr_parser_init(tmp);
  tmp->beg = -1;
  tmp->end = -1;
  tmp->id = -1;
  http->hdr_parser_root = tmp;

  hdr = http->hdr_parser_root;
  for (i = 0;i < http->hdr_count;i++)
    {
      j = 0;
      hdr_str = http->hdr_names[i];
      hdr_len = strlen(hdr_str);
      
      while (1)
	{
	  if (j < hdr_len)
	    {
	      if (j > hdr->end)
		{
		  tmp = (hdr_parser_t*)maplite_get(&hdr->childs, (unsigned int)hdr_str[j]);
		  if (tmp == NULL)
		    {
		      hdr_parser_reproduce(http, hdr, i, hdr_len - 1);
		      hdr = http->hdr_parser_root;
		      break;
		    }
		  else
		    {
		      hdr = tmp;
		      hdr->mi = hdr->beg;
		    }
		}	   
	      else if (j >= hdr->beg && j <= hdr->end)
		{
		  hdr_parser_str = http->hdr_names[hdr->id];
		  if (hdr_str[j] == hdr_parser_str[hdr->mi + 1])
		    {
		      hdr->mi += 1;
		    }
		  else
		    {
		      tmp = hdr_parser_division(http, hdr, hdr->mi);
		      hdr_parser_reproduce(http, tmp, i, hdr_len - 1);
		      hdr = http->hdr_parser_root;
		      break;
		    }
		}
	      else {/*error*/}
	    }
	  else
	    {
	      if (hdr->mi < hdr->end)
		{
		  tmp = hdr_parser_division(http, hdr, hdr->mi);
		}
	      else if (hdr->mi == hdr->end){}
	      else {/*error*/}
	      hdr = http->hdr_parser_root;
	      break;
	    }
	  j++;
	}
    }

  return 0;
}  

void hdr_parser_dismiss(http_t *http)
{
  assert(http != NULL);
  if (http == NULL) return;

  hdr_parser_uninit(http->hdr_parser_root);
  free (http->hdr_parser_root);
  http->hdr_parser_root = NULL;
}

int http_parse_result(http_t *http)
{
  int blanks[2];
  int index, flag, mark, cr;
  assert(http != NULL);
  if (http == NULL) return -1;
  
  cr = 0;
  mark = 0;
  flag = 0;
  index = 0;
  http->mi = 0;
  while (http->mi <= http->hypertext_length)
    {
      if (flag == 0)
	{
	  if (http->hypertext[http->mi] != ' ')
	    {
	      mark = http->mi;
	      flag = 1;
	    }
	}
      else
	{
	  if (index < 2)
	    {
	      if (http->hypertext[http->mi] == ' ')
		{
		  /*done!*/
		  int len = http->mi - mark;
		  char *buf = (char*)malloc(sizeof(char) * (len + 1));
		  memset(buf, 0, len + 1);
		  memcpy(buf, http->hypertext + mark, len);

		  printf("result: %8s\n", buf);
		  index++;
		  flag = 0;
		}
	    }
	  else
	    {
	      if (http->hypertext[http->mi] == '\r' )
		{
		  cr++;
		}
	      else if (http->hypertext[http->mi] == '\n')
		{
		  if (cr > 1) return -1;
		  /*done*/
		  int len = http->mi - mark - cr;
		  char *buf = (char*)malloc(sizeof(char) * (len + 1));
		  memset(buf, 0, len + 1);
		  memcpy(buf, http->hypertext + mark, len);
		  printf("result: %8s\n", buf);
		  http->mi++;
		  return http->mi;
		}
	    }
	}

      http->mi++;
    }
}

int http_parse_hdr(http_t *http)
{
  int mark, len, markv;
  char *hdr_parser_str;

  assert(http != NULL);
  if (http == NULL || http->hypertext_length <= 0) return -1;
    
  http->hdr_parser_root->state = PENDING;
  
  mark = 0;
  markv = 0;

  while (http->mi < http->hypertext_length)
    {
      if (http->mi > http->hdr_parser_current->end)
	{
	  if (http->hdr_parser_current->state == MATCHED)
	    {
	      if (http->hypertext[http->mi] == '\n')
		{
		  int len = (http->mi - 1) - (markv + 1) + 1;
		  char buf[32];
		  bzero(buf, 32);
		  memcpy(buf, http->hypertext + (markv + 1), len);
		  /*done*/
		  printf("key:%4d value: %8s\n", http->hdr_parser_current->id, buf);
		  http->hdr_parser_current = http->hdr_parser_root;
		}
	    }
	  else
	    {
	      if (http->hypertext[http->mi] == ' ') {}
	      else if (http->hypertext[http->mi] == ':')
		{
		  http->hdr_parser_current->state = MATCHED;
		  markv = http->mi;
		}
	      else if (http->hypertext[http->mi] == '\n')
		{
		  /*unrecgniziable*/
		  http->hdr_parser_current = http->hdr_parser_root;
		  http->hdr_parser_current->state = MATCHING;
		}
	      else
		{
		  if (http->hdr_parser_current == http->hdr_parser_root) mark = http->mi;
		  http->hdr_parser_current = maplite_get(&http->hdr_parser_current->childs, (unsigned int)http->hypertext[http->mi]);
		  if (http->hdr_parser_current == NULL) http->hdr_parser_current = http->hdr_parser_root;
		  if (http->hdr_parser_current != http->hdr_parser_root)
		    {
		      http->hdr_parser_current->mi = http->hdr_parser_current->beg;
		      http->hdr_parser_current->state = MATCHING;
		      hdr_parser_str = http->hdr_names[http->hdr_parser_current->id];
		    }
		}
	    }
	}
      else if (http->mi > http->hdr_parser_current->mi && http->mi <= http->hdr_parser_current->end)
	{
	  if (http->hypertext[http->mi] == hdr_parser_str[http->hdr_parser_current->mi + 1])
	    {
	      http->hdr_parser_current->mi++;
	    }
	  else
	    {
	      http->hdr_parser_current = http->hdr_parser_root;
	      http->hdr_parser_current->state = MATCHING;
	    }
	}
      else {/*error!*/}
      
      http->mi++;
    }
  return 0;
}

hdr_parser_t *hdr_parser_division(http_t *http, hdr_parser_t *hdr, int break_index)
{
  hdr_parser_t *new;
  char *hdr_str;
  assert(http != NULL && hdr != NULL && break_index <= (hdr->end - 1));
  if (http == NULL || hdr == NULL || break_index > (hdr->end -1)) return NULL;

  new = (hdr_parser_t*)malloc(sizeof(hdr_parser_t));
  hdr_parser_init(new);
  new->id = hdr->id;
  new->beg = hdr->beg;
  new->end = break_index;
  new->type = BRANCH;
  new->parent = hdr->parent;

  hdr->beg = break_index + 1;
  hdr->parent = new;

  hdr_str = http->hdr_names[hdr->id];
  maplite_set(&new->childs, (unsigned int)hdr_str[hdr->beg], (void*)hdr);
  maplite_set(&new->parent->childs, (unsigned int)hdr_str[new->beg], (void*)new);

  return new;
}

hdr_parser_t *hdr_parser_reproduce(http_t *http, hdr_parser_t *hdr, int new_id, int new_end)
{
  hdr_parser_t *new;
  char *hdr_str;
  assert(http != NULL && hdr != NULL);
  assert(new_id > hdr->id && new_id < http->hdr_count);
  if (http == NULL || hdr == NULL) return NULL;
  if (new_id <= hdr->id || new_id >= http->hdr_count) return NULL;

  hdr_str = http->hdr_names[new_id];
  if (new_end < strlen(hdr_str))
    {
      new = (hdr_parser_t*)malloc(sizeof(hdr_parser_t));
      hdr_parser_init(new);
      new->id = new_id;
      new->beg = hdr->end + 1;
      new->end = new_end;
      new->parent = hdr;

      hdr->type = BRANCH;

      maplite_set(&hdr->childs, (unsigned int)hdr_str[new->beg], (void*)new);
      
      return new;
    }
  return NULL;
}

void http_hypertext_attach(http_t *http, char* hypertext)
{
  assert(http != NULL || hypertext != NULL);
  if (http == NULL || hypertext == NULL) return;

  http->hypertext = hypertext;
  http->hypertext_length = 0;
  http->mi = 0;
  http->hdr_parser_current = http->hdr_parser_root;
}

void http_hypertext_detach(http_t *http)
{
  assert(http != NULL);
  if (http == NULL) return;

  http->hypertext = NULL;
  http->hypertext_length = -1;
  http->mi = -1;
}

int http_hypertext_receving(http_t *http, int hypertext_len)
{
  assert(http != NULL);
  if (http == NULL || hypertext_len <= http->hypertext_length) return -1;

  http->hypertext_length = hypertext_len;
  /*todo*/
}
