#ifndef _EV_H_
#define _EV_H_

#define EV_SIZE (1024*5)

#define EV_OK 0
#define EV_ERR -1

#define EV_NONE 0
#define EV_READABLE 1
#define EV_WRITABLE 2

#define EV_FILE_EVENTS 1
#define EV_DONT_WAIT 4

#define EV_NOMORE -1

#define EV_NOTUSED(v) ((void)v)

struct _event_mgr;

typedef void EVPROC(struct _event_mgr *mgr, int fd, void* data, int mask);
typedef void EVEVENTFINALIZERPROC(struct _event_mgr *mgr, void *data);
typedef void EVBEFORESLEEPPROC(struct _event_mgr *mgr);

typedef struct _event {
  int mask;
  EVPROC *rproc;
  EVPROC *wproc;
  void *data;
}event_t;

typedef struct _fired_event {
  int fd;
  int mask;
}fired_event_t;

typedef struct _event_mgr {
  int max_fd;
  int stop;
  void *tag;
  event_t events[EV_SIZE];
  fired_event_t fired[EV_SIZE];
  EVBEFORESLEEPPROC *bproc;
}event_mgr_t;

event_mgr_t *ev_create_event_mgr(void);
void ev_delete_event_mgr(event_mgr_t *mgr);
void ev_stop(event_mgr_t *mgr);
int ev_create_event(event_mgr_t *mgr, int fd, int mask,
			 EVPROC *proc, void *data);
void ev_delete_event(event_mgr_t *mgr, int fd, int mask);
int ev_get_mask(event_mgr_t *mgr, int fd);
int ev_process_events(event_mgr_t *mgr);
int ev_wait(int fd, int mask, long long ms);
void ev_main(event_mgr_t *mgr);
static char *ev_get_api_name(void);
void ev_set_before_sleep_proc(event_mgr_t *mgr, EVBEFORESLEEPPROC *before_proc);

#endif
