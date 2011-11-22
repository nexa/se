#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "maplite.h"

int maplite_init(maplite_t *map, TYPE type)
{
  int memsize;
  assert(map != NULL);
  if (map == NULL) return -1;

  if (type == DEC)
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

  memsize = map->capility * sizeof(void*);
  map->values = (void**)malloc(memsize);
  bzero(map->values, memsize);
  map->size = 0;
}

void maplite_uninit(maplite_t *map)
{
  assert(map != NULL);
  if (map == NULL) return;

  free(map->values);
}

void maplite_set(maplite_t *map, int key, void *value)
{
  int index;
  assert(map != NULL);
  assert(key >= map->keybase && key <= (map->keybase + map->capility));
  if (map == NULL) return;
  if (key < map->keybase || key > (map->keybase + map->capility)) return;
  
  index = key - map->keybase;
  map->values[index] = value; 
}

void *maplite_get(maplite_t *map, int key)
{
  int index;
  assert(map != NULL);
  assert(key >= map->keybase && key <= (map->keybase + map->capility));
  if (map == NULL) return;
  if (key < map->keybase || key > (map->keybase + map->capility)) return;

  index = key - map->keybase;
  return map->values[index];
}
