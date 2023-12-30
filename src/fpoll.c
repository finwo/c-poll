#include <poll.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>

#include "fpoll.h"

/* struct fpoll { */
/*   struct pollfd *fds; */
/*   int size; */
/*   int limit; */
/* }; */

struct fpoll * fpoll_create() {
  struct fpoll *output = calloc(1, sizeof(struct fpoll));
  return output;
}

FPOLL_STATUS fpoll_close(struct fpoll *descriptor) {
  if (descriptor->fds) free(descriptor->fds);
  free(descriptor);
  return FPOLL_STATUS_OK;
}

int fpoll_wait(struct fpoll *descriptor, struct fpoll_ev *evs, int max_evs, int timeout) {
  int i, c;

  // Fetch new events
  if (!descriptor->remaining) {
    descriptor->remaining = poll(descriptor->fds, descriptor->size, timeout);
  }

  // Handle remaining events, possibly from previous run
  if (descriptor->remaining) {
    c = 0;
    for( i = 0 ; (i < descriptor->size) && (c < max_evs) ; i++ ) {
      if (!(descriptor->fds[i].revents)) continue;

      // Convert event to our standard
      evs[c].fd = descriptor->fds[i].fd;
      evs[c].ev = descriptor->fds[i].revents;

      // Reset the origin
      descriptor->fds[i].revents = 0;

      // Iterate to the next
      descriptor->remaining -= 1;
      c++;
    }

    // Prevent deadlocks
    if (i == descriptor->size) {
      descriptor->remaining = 0;
    }

    return c;
  }

  return 0;
}

FPOLL_STATUS fpoll_add(struct fpoll *descriptor, FPOLL_EVENT events, FPOLL_FD filedescriptor, void *udata) {

  // Initial alloc
  if (!descriptor->fds) {
    descriptor->limit = 4;
    descriptor->fds   = calloc(descriptor->limit, sizeof(struct pollfd));
  }

  // Grow alloc
  if (descriptor->limit < (descriptor->size + 1)) {
    descriptor->limit = descriptor->limit * 2;
    descriptor->fds   = realloc(descriptor->fds, descriptor->limit);
  }

  // Find the filedescriptor in the list
  struct pollfd *pfd = NULL;
  int i;
  for(i = 0 ; i < descriptor->size; i++) {
    if (filedescriptor != descriptor->fds[i].fd) continue;
    pfd = &(descriptor->fds[i]);
    break;
  }

  // Assign a new pollfd if needed
  if (!pfd) {
    pfd              = &(descriptor->fds[descriptor->size]);
    descriptor->size = descriptor->size + 1;
    pfd->fd          = filedescriptor;
    pfd->events      = 0;
  }

  // Assign the correct events
  pfd->events = events;

  return FPOLL_STATUS_OK;
}

FPOLL_STATUS fpoll_del(struct fpoll *descriptor, FPOLL_EVENT events, FPOLL_FD filedescriptor) {

  // Initial alloc
  if (!descriptor->fds) {
    descriptor->limit = 4;
    descriptor->fds   = calloc(descriptor->limit, sizeof(struct pollfd));
  }

  // Find the filedescriptor in the list
  struct pollfd *pfd = NULL;
  int i;
  for(i = 0 ; i < descriptor->size; i++) {
    if (filedescriptor != descriptor->fds[i].fd) continue;
    pfd = &(descriptor->fds[i]);
    break;
  }

  // Remove marked events
  if (pfd) {
    if (events & FPOLL_IN ) pfd->events &= ~POLLIN;
    if (events & FPOLL_OUT) pfd->events &= ~POLLOUT;

    // Remove fd from list if it has no events
    if (!pfd->events) {
      if ((i + 1) < descriptor->size) {
        memmove(&(descriptor->fds[i]), &(descriptor->fds[i+1]), (descriptor->size - i) * sizeof(struct pollfd));
      }
      descriptor->size -= 1;
    }
  }

  return FPOLL_STATUS_OK;
}
