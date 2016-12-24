#include <stdio.h>
#include "rank.h"
#include <math.h>

void read_rank_func(struct rank_val *r, FILE *f) {
  fread(&r->bdef, 1, sizeof(int), f);
  fread(&r->v, 11, sizeof(float), f);
}

void read_harm_func(struct harm_val *h, FILE *f) {
  int i;
  for(i = 0; i < N_HARM; i++)
    read_rank_func(&h->v[i], f);
}

void r_read(struct rank *r, FILE *f) {
  char buf[64];
  fread(buf, 1, 32, f);
  r->fn = buf[30];
  r->fd = buf[31];
  fread(r->name, 1, 32, f);
  fread(buf, 1, 56, f);
  fread(r->mnem, 1, 8, f);
  fread(buf, 1, 56, f);
  fread(buf, 1, 8, f);
  read_rank_func(&r->vol, f);
  read_rank_func(&r->off, f);
  read_rank_func(&r->ran, f);
  read_rank_func(&r->ins, f);
  read_rank_func(&r->att, f);
  read_rank_func(&r->atd, f);
  read_rank_func(&r->dct, f);
  read_rank_func(&r->dcd, f);

  read_harm_func(&r->lev, f);
  read_harm_func(&r->h_ran, f);
  read_harm_func(&r->h_att, f);
  read_harm_func(&r->h_atp, f);

  int i, j;
  for(i = 0; i < N_NOTE; i++) {
    r->volcache[i] = powf(10, get_rv(r->vol, i) / 10);
    for(j = 0; j < N_HARM; j++) {
      r->hlcache[i][j] = powf(10, get_rv(r->lev.v[j], i) / 10);
    }
  }
}

float get_rv(struct rank_val r, int note) {
  int i = note / 6;
  int k = note - 6 * i;
  float v = r.v[i];
  if(k) v += k * (r.v[i + 1] - v) / 6;
  return v;
}


float fundamental(float note) {
  return 440.0 * pow(2, (note - 33) / 12.0);
}

float r_advance(struct rank *r, int i) {
  float v = 0;
  float f0 = (fundamental(i) * r->fn / r->fd) / SAMPLERATE;
  int j;
  float vol = r->v_env[i] * r->volcache[i] * 5;
  for(j = 0; j < N_HARM; j++) {
    if(get_rv(r->lev.v[j], i) < -80) continue;
    float x = r->h_phase[i][j];
    x = cos(x);
    v += x * vol * r->hlcache[i][j];
    r->h_phase[i][j] += 2 * 3.14159 * f0 * (j + 1);
    while(r->h_phase[i][j] > 2 * 3.14159) r->h_phase[i][j] -= 2 * 3.14159;
  }
  return v;
}
