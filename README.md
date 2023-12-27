finwo/poll
==========

Basic cross-platform io multiplexing library, focussing on polling

## Why

Yes, there's already modern replacements for [poll(2)][poll2] and
[select(2)][select2], and things like libevent, that handle io multiplexing
properly, but none that I could find that were easily embeddable without having
the resulting binary depend on those libraries.

While initially an easy-to-use wrapper around poll(2), platform-specific
optimizations (like using epoll/kqueue) will be implemented here without the
external api changing to allow all my projects to benefit from the work here.

## Installation

This library is intended to be installed through
[dep](https://github.com/finwo/dep), but can be embedded by dropping in the
[fpoll.c](src/fpoll.c) and [fpoll.h](src/fpoll.h) files into your project and
including them during compilation.

```
dep add finwo/poll
```

## API

[poll2]: https://man7.org/linux/man-pages/man2/poll.2.html
[select2]: https://man7.org/linux/man-pages/man2/select.2.html
