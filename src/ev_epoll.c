#include <sys/epoll.h>

typedef struct _ev_api_state {
  int epfd;
  struct epoll_event events[EV_SIZE];
}ev_api_state_t;

static int ev_api_create(event_mgr_t *mgr) {
  ev_api_state_t *state = malloc(sizeof(ev_api_state_t));

  if (!state) return -1;
  state->epfd = epoll_create(1024);
  if (state->epfd == -1) return -1;
  mgr->tag = state;
  return 0;
}

static void ev_api_free(event_mgr_t *mgr) {
  ev_api_state_t *state = mgr->tag;
  
  close(state->epfd);
  free(state);
}

static int ev_api_add_event(event_mgr_t *mgr, int fd, int mask) {
  ev_api_state_t *state = mgr->tag;
  struct epoll_event ee;
  
  int op = mgr->events[fd].mask == EV_NONE ?
    EPOLL_CTL_ADD : EPOLL_CTL_MOD;
  ee.events = 0;
  mask |= mgr->events[fd].mask;
  if (mask & EV_READABLE) ee.events |= EPOLLIN;
  if (mask & EV_WRITABLE) ee.events |= EPOLLOUT;
  ee.data.u64 = 0;
  ee.data.fd = fd;
  if (epoll_ctl(state->epfd, op, fd, &ee) == -1) return -1;
  return 0;
}

static void ev_api_delete_event(event_mgr_t *mgr, int fd, int delmask) {
  ev_api_state_t *state = mgr->tag;
  struct epoll_event ee;
  int mask = mgr->events[fd].mask & (~delmask);
  
  ee.events = 0;
  if (mask & EV_READABLE) ee.events |= EPOLLIN;
  if (mask & EV_WRITABLE) ee.events |= EPOLLOUT;
  ee.data.u64 = 0;
  ee.data.fd = fd;
  if (mask != EV_NONE) {
    epoll_ctl(state->epfd, EPOLL_CTL_MOD, fd, &ee);
  } else {
    epoll_ctl(state->epfd, EPOLL_CTL_DEL, fd, &ee);
  }
}

static int ev_api_poll(event_mgr_t *mgr, struct timeval *tvp) {
  ev_api_state *state = mgr->tag;
  int retval, numevents = 0;

  retval = epoll_wait(state->epfd, state->events, AE_SIZE, 1000);
  if (retval > 0) {
    int j;
    
    numevents = retval;
    for (j = 0;j < numevents;j++) {
      int mask = 0;
      struct epoll_event *e = state->events + j;
      
      if (e->events & EPOLLIN) mask |= EV_READABLE;
      if (e->events & EPOLLOUT) mask |= EV_WRITABLE;
      mgr->fired[j].fd = e->data.fd;
      mgr->fired[j].mask = mask;
    }
  }
  return numevents;
}

static char *ev_get_api_name(void) {
  return "epoll";
}
