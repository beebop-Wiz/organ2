#ifndef RANK_H
#define RANK_H

#define N_NOTE 61
#define N_HARM 64
#define SAMPLERATE 48000

struct rank_val {
  int bdef;
  float v[11];
};

struct harm_val {
  struct rank_val v[N_HARM];
};

struct rank {
  float h_phase[N_NOTE][N_HARM];
  float v_env[N_NOTE];
  int fn, fd;

  char name[32];
  char mnem[8];

  struct rank_val vol, off, ran;
  struct rank_val ins, att, atd, dct, dcd;
  struct harm_val lev, h_ran, h_att, h_atp;

  float volcache[N_NOTE];
  float hlcache[N_NOTE][N_HARM];
};

#include <stdio.h>
void r_read(struct rank *r, FILE *f);
float get_rv(struct rank_val r, int note);
float r_advance(struct rank *r, int i);

#endif
