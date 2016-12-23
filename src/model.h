#ifndef MODEL_H
#define MODEL_H

#include "division.h"

struct model {
  struct division_list *divisions;
};

struct division_list {
  struct division_list *next;
  struct division *d;
};

struct model *create_model();
void free_model(struct model *);
void add_division(struct model *, struct division *);
void m_proc_midi(struct model *);

#endif
