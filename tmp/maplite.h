/* 
   a simple map implement

   history:
   20111121: create

 */
#ifndef _MAPLITE_H_
#define _MAPLITE_H_

typedef enum _KEYRANGE
  {
    DEC = 0,
    HEX,
    LOWER,
    UPPER,
    ASCII,
  }KEYRANGE;

typedef struct maplite_s
{
  KEYRANGE range;
  void **values;
  unsigned int capility;
  unsigned int size;
  unsigned int keybase;
}maplite_t;

int maplite_init(maplite_t *map, KEYRANGE range);
void maplite_uninit(maplite_t *map);
void maplite_set(maplite_t *map, unsigned int key, void *value);
void *maplite_get(maplite_t *map, unsigned int key);


#endif
