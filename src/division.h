#ifndef DIVISION_H
#define DIVISION_H

#include "rank.h"

struct division {
  struct rank_list *ranks;
  int midich;
};

struct rank_list {
  struct rank_list *next;
  struct rank *r;
};

struct division *create_division();
void free_division(struct division *);
void add_rank(struct division *, struct rank *);
void d_proc_midi(struct division *);

#endif
