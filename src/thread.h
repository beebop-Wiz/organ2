#ifndef THREAD_H
#define THREAD_H

#include "event.h"

struct thread_args {
  struct thread *self;
  void (*evcb)(struct event);
};

struct thread {
  pthread_t tdesc;
  equeue_t queue;
};

typedef struct thread *thread_t;

thread_t thread_create(void (*evcb)(struct event));
void thread_destroy(thread_t th);
void thread_exit();
void thread_signal(thread_t th, struct event evt);

#endif
