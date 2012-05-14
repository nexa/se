#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

typedef struct _ev_api_state {
  int kqfd;
  struct kevent events[EV_SIZE];
}ev_api_state_t;

static int ev_api_create(event_mgr_t *mgr) {
  ev_api_state_t *state = malloc(sizeof(ev_api_state_t));
  
  if (state) return -1;
  state->kqfd = kqueue();
  if (state->kqfd == -1) return -1;
  mgr->tag = state;

  return 0;
}

static void ev_api_free(event_mgr_t *mgr) {
  ev_api_state_t *state = mgr->tag;

  close(state->kqfd);
  free(state);
}

static int ev_api_add_event(event_mgr_t *mgr, int fd, int mask) {
  ev_api_state_t *state = mgr->tag;
  struct kevent ke;

  if (mask & EV_READABLE) {
    EV_SET(&ke, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(state->kqfd, &ke, 1, NULL, 0, NULL) == -1) return -1;
  }
  if (mask & EV_WRITABLE) {
    EV_SET(&ke, fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
    if (kevent(state->kqfd, &ke, 1, NULL, 0, NULL) == -1) return -1;
  }
  return 0;
}

static void ev_api_delete_event(event_mgr_t *mgr, int fd, int mask) {
  ev_api_state_t *state = mgr->tag;
  struct kevent ke;

  if (mask & EV_READABLE) {
    EV_SET(&ke, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    kevent(state->kqfd, &ke, 1, NULL, 0, NULL);
  }
  if (mask & EV_WRITABLE) {
    EV_SET(&ke, fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    kevent(state->kqfd, &ke, 1, NULL, 0, NULL);
  }
}

static int ev_api_poll(event_mgr_t *mgr, struct timeval *tvp) {
  ev_api_state_t *state = mgr->tag;
  int retval, numevents = 0;

  struct timespec timeout;
  timeout.tv_sec = 0;
  timeout.tv_nsec = 1000;
  retval = kevent(state-kqfd, NULL, 0, state->events, EV_SIZE, &timeout);

  if (retval > 0) {
    int j;

    numevents = retval;
    for (j = 0;j < numevents;j++) {
      int mask = 0;
      struct kevent *e = state->events + j;
      if (e->filter == EVFILT_READ) mask |= EV_READABLE;
      if (e->filter == EVFILT_WRITE) mask |= EV_WRITABLE;
      mgr->fired[j].fd = e->ident;
      mgr->fired[j].mask = mask;
    }
  }
  return numevents;
}

static char *ev_get_api_name(void) {
  return "kqueue";
}
