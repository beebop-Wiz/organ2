#include "division.h"
#include "rank.h"
#include <stdio.h>

int main(void) {
  struct division *d = create_division();
  struct rank *r = calloc(1, sizeof(struct rank));
  r_read(r, fopen("aeolus_stops/default/I_trumpet.ae0", "r"));
  r->fn = r->fd = 1;
  add_rank(d, r);
  d->kbd[0] = 1;
  int i;
  for(i = 0; i < 480000; i++)
    d_advance(d);
  return 0;
}
