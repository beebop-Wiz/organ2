#ifndef EVENT_H
#define EVENT_H

#include <pthread.h>

enum evtype {
  E_INIT,
  E_EXIT,
  E_STOP_TOGGLE,
  E_UPDATE_IFELM
};

struct event {
  int type;
  void *data;
};

struct elist {
  struct elist *next;
  struct event evt;
};

struct equeue {
  struct elist *list;
  int nevent;
  pthread_cond_t wait;
  pthread_mutex_t lock;
};

typedef struct equeue *equeue_t;

void el_push(struct elist **list, int type, void *data);
struct event el_pop(struct elist **list);

equeue_t eq_create();
void eq_event(equeue_t queue, struct event evt);
struct event eq_waitevent(equeue_t queue);
void eq_free(equeue_t queue);

struct event ecreate(int type, void *data);

#endif
