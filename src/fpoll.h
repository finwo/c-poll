#ifndef __FINWO_POLL_H__
#define __FINWO_POLL_H__

#include <stdbool.h>

#include <poll.h>

#ifndef FPOLL_STATUS
#define FPOLL_STATUS        int
#endif
#define FPOLL_STATUS_OK     0
#define FPOLL_STATUS_ERROR  1

#ifndef FPOLL_EVENT
#define FPOLL_EVENT         int
#endif
#define FPOLL_IN            POLLIN
#define FPOLL_OUT           POLLOUT
#define FPOLL_HUP           POLLHUP

#ifndef FPOLL_FD
#define FPOLL_FD            int
#endif

struct fpoll {
  struct pollfd *fds;
  void          **udata;
  int size;
  int limit;
  int remaining;
};

struct fpoll_ev {
  FPOLL_FD    fd;
  FPOLL_EVENT ev;
  void        *udata;
};

struct fpoll * fpoll_create();
FPOLL_STATUS   fpoll_close(struct fpoll *);
int            fpoll_wait(struct fpoll *, struct fpoll_ev *, int max_evs, int timeout);
FPOLL_STATUS   fpoll_add(struct fpoll *, FPOLL_EVENT, FPOLL_FD, void *udata);
FPOLL_STATUS   fpoll_del(struct fpoll *, FPOLL_EVENT, FPOLL_FD);

#endif // __FINWO_POLL_H__
