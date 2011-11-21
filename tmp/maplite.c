#include <assert.h>
#include <stdlib.h>

#include "maplite.h"

int maplite_init(maplite_t *map, TYPE type)
{
  assert(map != NULL);
  if (map == NULL) return -1;

  if (type == DEV)
    {
      map->capility = 10;
      map->keybase = 0;
    }
  else if (type == HEX)
    {
      map->capility = 16;
      map->keybase = 0;
    }
  else if (type == LOWER)
    {
      map->capility = 26;
      map->keybase = 'a';
    }
  else if (type == UPPER)
    {
      map->capility = 26;
      map->keybase = 'A';
    }
  else
    {
      map->capility = 256;
      map->keybase = 0;
    }

  map->keys = (int*)malloc(map->capility * sizeof(int));
  map->values = (void**)malloc(map->capility * sizeof(void*));
  map->size = 0;
}

void maplite_uninit(maplite_t *map)
{
  assert(map != NULL);
  if (map == NULL) return;

  free(map->keys);
  free(map->values);
}

void maplite_set(maplite_t *map, int key, void *value)
{
  
}

void maplite_get(maplite_t *map, int key)
{

}
