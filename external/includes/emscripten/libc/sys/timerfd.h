#ifndef _SYS_TIMERFD_H
#define _SYS_TIMERFD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include <fcntl.h>

#define TFD_NONBLOCK O_NONBLOCK
#define TFD_CLOEXEC O_CLOEXEC

#define TFD_TIMER_ABSTIME 1

int timerfd_create(int, int);
int timerfd_settime(int, int, const struct itimerspec *, struct itimerspec *);
int timerfd_gettime(int, struct itimerspec *);

#ifdef __cplusplus
}
#endif

#endif
