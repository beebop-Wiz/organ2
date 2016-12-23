#include "event.h"
#include <stdlib.h>
#include <string.h>

void el_push(struct elist **list, int type, void *data) {
  if(*list == NULL) {
    *list = malloc(sizeof(struct elist));
    (*list)->next = NULL;
    (*list)->evt.type = type;
    (*list)->evt.data = data;
    return;
  }
  struct elist *q = *list;
  while(q->next) q = q->next;
  q->next = malloc(sizeof(struct elist));
  q->next->next = NULL;
  q->next->evt.type = type;
  q->next->evt.data = data;
}

struct event el_pop(struct elist **list) {
  struct event r;
  if(*list == NULL) {
    r.type = 0;
    r.data = NULL;
  } else {
    struct elist *q = *list;
    memcpy(&r, &q->evt, sizeof(struct event));
    struct elist *n = q->next;
    free(q);
    *list = n;
  }
  return r;
}

equeue_t eq_create() {
  equeue_t r = malloc(sizeof(struct equeue));
  pthread_cond_init(&r->wait, NULL);
  pthread_mutex_init(&r->lock, NULL);
  r->nevent = 0;
  return r;
}

void eq_event(equeue_t queue, struct event evt) {
  pthread_mutex_lock(&queue->lock);
  el_push(&queue->list, evt.type, evt.data);
  queue->nevent++;
  pthread_cond_signal(&queue->wait);
  pthread_mutex_unlock(&queue->lock);
}

struct event eq_waitevent(equeue_t queue) {
  struct event r;
  pthread_mutex_lock(&queue->lock);
  if(queue->nevent == 0) {
    pthread_cond_wait(&queue->wait, &queue->lock);
  }
  r = el_pop(&queue->list);    
  queue->nevent--;
  pthread_mutex_unlock(&queue->lock);
  return r;
}

void eq_free(equeue_t queue) {
  pthread_cond_destroy(&queue->wait);
  pthread_mutex_destroy(&queue->lock);
  free(queue);
}

struct event ecreate(int type, void *data) {
  struct event r = {type, data};
  return r;
}
