/* http parser

   histroy:
   20111103: create
*/
#ifndef _HTTP_H_
#define _HTTP_H_

#include "maplite.h"

typedef enum _STATE
  {
    MATCHING = 0,
    PENDING,
    MATCHED,
  }STATE;

typedef enum _TYPE
  {
    ROOT = 0x1,
    BRANCH = 0x2,
    LEAF = 0x4,
  }TYPE;

typedef struct node_s
{
  int id;
  int beg;
  int end;
  int mi;
  maplite_t childs;
  STATE state;
  TYPE type;
  
}node_t;

#endif
