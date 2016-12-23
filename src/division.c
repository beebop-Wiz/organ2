#include "division.h"
#include <stdlib.h>

struct division *create_division() {
  struct division *d = calloc(1, sizeof(struct division));
  d->ranks = calloc(1, sizeof(struct rank_list));
  return d;
}

void free_division(struct division *d) {
  free(d);
}

void add_rank(struct division *d, struct rank *nr) {
  struct rank_list *r = d->ranks;
  while(r->next) r = r->next;
  r->r = nr;
  r->next = calloc(1, sizeof(struct rank_list));
}

void d_proc_midi(struct division *d) {
  
}
