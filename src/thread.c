#include "thread.h"
#include "event.h"
#include <stdlib.h>

void *_entry(void *arg) {
  struct thread_args *ta = (struct thread_args *) arg;
  thread_t self = ta->self;
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  struct event evt;
  while(1) {
    evt = eq_waitevent(self->queue);
    ta->evcb(evt);
    pthread_mutex_lock(&self->queue->lock);
    if(self->queue->nevent == 0)
      pthread_testcancel();
    pthread_mutex_unlock(&self->queue->lock);
  }
}

thread_t thread_create(void evcb(struct event)) {
  thread_t r = malloc(sizeof(struct thread));
  r->queue = eq_create();
  struct thread_args *ta = malloc(sizeof(struct thread_args));
  ta->self = r;
  ta->evcb = evcb;
  pthread_create(&r->tdesc, NULL, _entry, ta);
  pthread_detach(r->tdesc);
  return r;
}

void thread_destroy(thread_t th) {
  pthread_cancel(th->tdesc);
  pthread_join(th->tdesc, NULL);
  eq_free(th->queue);
  free(th);
}

void thread_exit() {
  pthread_exit(NULL);
}

void thread_signal(thread_t th, struct event evt) {
  eq_event(th->queue, evt);
}
