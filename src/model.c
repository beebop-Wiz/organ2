#include "model.h"
#include <stdlib.h>

struct model *create_model() {
  struct model *r = calloc(1, sizeof(struct model));
  r->divisions = calloc(1, sizeof(struct division_list));
  return r;
}

void free_model(struct model *m) {
  free(m);
}

void add_division(struct model *m, struct division *nd) {
  struct division_list *d = m->divisions;
  while(d->next) d = d->next;
  d->d = nd;
  d->next = calloc(1, sizeof(struct division_list));
}

void m_proc_midi(struct model *m, jack_midi_event_t e) {
  struct division_list *d = m->divisions;
  while(d->next) {
    d_proc_midi(d->d, e);
    d = d->next;
  }
}

double m_advance(struct model *m) {
  struct division_list *d = m->divisions;
  double r = 0;
  while(d->next) {
    r += d_advance(d->d);
    d = d->next;
  }
  return r;
}

