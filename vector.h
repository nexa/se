/* 20111007

   vector

   search engine prototype
*/
#ifndef _VECTOR_H_
#define _VECTOR_H_

typedef struct _vector_item_t
{
  void* data;
  int index;
  int enable;
}vector_item_t, vitem_t;

typedef struct _vector_t
{
  struct _vector_item_t *items;
  int capability;
  int size;
}vector_t, v_t;

void vector_init(vector_t *, int);
void vector_uninit(vector_t *);
void vector_clear(vector_t *);

int vector_fit(vector_t *, void *);
void vector_enable(vector_t *, int);
void * vector_get_date(vector_t *, int);

int vector_next(vector_t *, int);

#endif
