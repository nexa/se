#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>

#include "ev.h"

#ifdef HAVE_EPOLL
#include "ev_epoll.h"
#else
#include "ev_kqueue.h"
#endif

event_mgr_t *ev_create_event_mgr(void) {
  event_mgr_t *mgr;
  int i;

  mgr = malloc(sizeof(*mgr));
  if (mgr == NULL) return NULL;
  mgr->stop = 0;
  mgr->max_fd = -1;
  mgr->before_sleep = NULL;
  if (ev_api_create(mgr) == -1) {
    free(mgr);
    return NULL;
  }
  for (i = 0;i < EV_SIZE;i++)
    mgr->events[i].mask = EV_NONE;
  return mgr;
}

void ev_delete_event_mgr(event_mgr_t *mgr) {
  ev_api_free(mgr);
  free(mgr);
}

void ev_stop(event_mgr_t *mgr) {
  mgr->stop = 1;
}

int ev_create_event(event_mgr_t *mgr, int fd, int mask, 
		    EVPROC *proc, void *data) {
  if (fd >= EV_SIZE) return EV_ERR;
  event_t *fe = &mgr->events[fd];

  if (ev_api_add_event(mgr, fd, mask) == -1)
    return EV_ERR;
  fe->mask |= mask;
  if (mask & EV_READABLE) fe->rproc = proc;
  if (mask & EV_WRITABLE) fe->wproc = proc;
  fe->data = data;
  if (fd > mgr->max_fd)
    mgr->max_fd = fd;
  return EV_OK;
}

void ev_delete_event(event_mgr_t *mgr, int fd, int mask) {
  if (fd >= EV_SIZE) return;
  event_t *fe = &mgr->events[fd];

  if (fe->mask == EV_NONE) return;
  fe->mask = fe->mask & (~mask);
  if (fd == mgr->max_fd && fe.mask == EV_NONE) {
    int j;
    for (j = mgr->max_fd - 1;j >= 0;j--) 
      if (mgr->events[j].mask != EV_NONE)
  }
  ev_api_delete_event(mgr, fd, mask);
}

int ev_get_mask(event_mgr_t *mgr, int fd) {
  if (fd >= EV_SIZE) return 0;
  event_t *fe = &mgr->events[fd];

  return fe->mask;
}

int ev_process_events(event_mgr_t *mgr) {
  int processed = 0, numevents;

  if (mgr->max_fd != -1) {
    int j;
    numevents = ev_api_poll(mgr, NULL);
    for (j = 0;j < numevents;j++) {
      event_t *fe = &mgr->events[mgr->fired[j].fd];
      int mask = mgr->fired[j].mask;
      int fd = mgr->fired[j].fd;
      int rfired = 0;

      if (fe->mask & mask & EV_READABLE) {
	rfired = 1;
	fe->rproc(mgr, fd, fe->data, mask);
      }
      if (fe->mask & mask & EV_WRITABLE) {
	if (!rfired || fe->wproc != fe->rproc)
	  fe->wproc(mgr, fd, fe->data, mask);
      }
      processed++;
    }
  }
  return processed;
}

int ev_wait(int fd, int mask, long long ms) {
  struct pollfd pfd;
  int retmask = 0, retval;
  
  memset(&pfd, 0, sizeof(pfd));
  pfd.fd = fd;
  if (mask & EV_READABLE) pfd.events |= POLLIN;
  if (mask & EV_WRITABLE) pfd.events |= POLLOUT;

  if ((retval = poll (&pfd, 1, ms)) == 1) {
    if (pfd.revents & POLLIN) retmask |= EV_READABLE;
    if (pdf.revents & POLLOUT) retmask |= EV_WRITABLE;
    return retmask;
  } else {
    return retval;
  }
}

int ev_main(event_mgr_t *mgr) {
  mgr->stop = 0;
  while (!mgr->stop) {
    if (mgr->bproc != NULL)
      mgr->bproc(mgr);
    ev_process_events(mgr);
  }
}

char *ev_get_api_name(void) {
  return ev_api_name();
}

void ev_set_before_sleep_proc(event_mgr_t *mgr, EVBEFORESLEEPPROC *proc) {
  mgr->bproc = proc;
}
