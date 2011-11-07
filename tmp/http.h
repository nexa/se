/* http parser

   histroy:
   20111103: create
*/
#ifndef _HTTP_H_
#define _HTTP_H_

typedef struct _hdr_entry_t hdr_entry_t;
typedef struct _http_t http_t;

/* entry process runtines
 */
typedef int (*FUNC)(http_t *, hdr_entry_t *);

/* entry state
 */
enum HDR_ENTRY_STATE
  {
    IGNORING = -1,
    MATCHING = 0,
    PENDING = 1,
    MATCHED = 2,
  };

/* entry type
 */
enum HDR_ENTRY_TYPE
  {
    ROOT = 0,
    PATH,
    ENTRY,
  };

/* hdr_entry_t
 */
struct _hdr_entry_t
{
  char *body;
  int blen;
  char *sub_entry_keys;
  hdr_entry_t *sub_entry_ptrs;
  int sub_entry_count;
  int in_beg;
  int in_end;
  FUNC func1;
  FUNC func2;
};

/* _http_t
 */
struct _http_t
{
  char *ptr_buf;
  int *ptr_len;
  int index;
  hdr_entry_t *entry_root;
  hdr_entry_t *entry_current;
  int entry_beg_tmp;
};



#endif
