/* 
   a simple map implement

   history:
   20111121: create

 */
#ifndef _MAPLITE_H_
#define _MAPLITE_H_

typedef enum _TYPE
  {
    DEC = 0,
    HEX,
    LOWER,
    UPPER,
    ASCII,
  }TYPE;

typedef struct maplite_s
{
  TYPE type;
  void **values;
  int capility;
  int size;
  int keybase;
}maplite_t;

int maplite_init(maplite_t *map, TYPE type);
void maplite_uninit(maplite_t *map);
void maplite_set(maplite_t *map, int key, void *value);
void *maplite_get(maplite_t *map, int key);


#endif
