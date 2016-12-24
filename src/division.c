#include "division.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

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

void d_proc_midi(struct division *d, jack_midi_event_t e) {
  printf("[Audio] %x %d %d\n", e.buffer[0], e.buffer[1], e.buffer[2]);
  if((e.buffer[0] & 0x0f) == d->midich) {
    if(e.buffer[1] >= 36 && e.buffer[1] <= 96) {
      if(((e.buffer[0] & 0xf0) == 0x90)
	 && e.buffer[2]) {
	printf("[Audio] + %d\n", e.buffer[1]);
	d->kbd[e.buffer[1] - 36] = 1;
      } else if(((e.buffer[0] & 0xf0) == 0x80)
		 || ((e.buffer[0] & 0xf0) == 0x90)) {
	d->kbd[e.buffer[1] - 36] = 0;
	printf("[Audio] - %d\n", e.buffer[1]);
      }
    }
    if(((e.buffer[0] & 0xf0) == 0xb0)
       && (e.buffer[1] == 123)) {
      printf("[Audio] - *\n");
      memset(d->kbd, 0, sizeof(d->kbd));
    }
  }
}

double d_advance(struct division *d) {
  double v = 0;
  struct rank_list *r = d->ranks;
  int i;
  while(r->next) {
    for(i = 0; i < N_NOTE; i++) {
      if(r->r->v_env[i] > 0.001 || d->kbd[i]) {
	v += r_advance(r->r, i);
	if(d->kbd[i] && r->r->v_env[i] < 0.99) r->r->v_env[i] += 0.01;
	if(!d->kbd[i] && r->r->v_env[i] > 0.01) r->r->v_env[i] -= 0.01;
	if(r->r->v_env[i] < 0.03 && !d->kbd[i]) r->r->v_env[i] = 0;
      }
    }
    r = r->next;
  }
  return v;
}
