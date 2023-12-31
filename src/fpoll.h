#ifndef __FINWO_POLL_H__
#define __FINWO_POLL_H__

#include <stdbool.h>

#include "poll_compat.h"

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
FPOLL_STATUS   fpoll_close(struct fpoll *descriptor);
int            fpoll_wait(struct fpoll *descriptor, struct fpoll_ev *evs, int max_evs, int timeout);
FPOLL_STATUS   fpoll_add(struct fpoll *descriptor, FPOLL_EVENT events, FPOLL_FD filedescriptor, void *udata);
FPOLL_STATUS   fpoll_del(struct fpoll *descriptor, FPOLL_EVENT events, FPOLL_FD filedescriptor);

#endif // __FINWO_POLL_H__
