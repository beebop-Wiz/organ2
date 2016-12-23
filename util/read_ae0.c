#include <stdio.h>

#define N_NOTE 11
#define N_HARM 64
#define NOTE_MIN 36
#define NOTE_MAX 96

struct N_func {
  float v[N_NOTE];
  int b;
};

void read_N_func(struct N_func *n) {
  fread(&n->b, 1, sizeof(int), stdin);
  fread(&n->v, N_NOTE, sizeof(float), stdin);
}

void print_N_func(struct N_func *n) {
  int i;
  printf("b = 0x%x, v = [", n->b);
  for(i = 0; i < N_NOTE; i++) {
    if(n->b & (1 << i)) printf("\e[47;30m");
    printf("%f\e[0m%s", n->v[i], i < (N_NOTE - 1) ? ", " : "]\n");
  }
}

struct HN_func {
  struct N_func h[N_HARM];
};

void read_HN_func(struct HN_func *h) {
  int i;
  for(i = 0; i < N_HARM; i++) {
    read_N_func(&h->h[i]);
  }
}

void print_HN_func(struct HN_func *h) {
  int i;
  for(i = 0; i < N_HARM; i++) {
    printf("%02d: ", i);
    print_N_func(&h->h[i]);
  }
}

int main(void) {
  char header[32];
  char stopname[32];
  char copyrite[56];
  char mnemonic[8];
  char comments[56];
  char reserved[8];

  struct N_func n_vol, n_off, n_ran;
  struct N_func n_ins, n_att, n_atd, n_dct, n_dcd;
  struct HN_func h_lev, h_ran, h_att, h_atp;
  
  int v, k;
  int n0, n1, fn, fd;
  fread(header, 1, 32, stdin);
  if(strcmp(header, "AEOLUS")) {
    fprintf(stderr, "Incorrect magic number (got '%6s')\n", header);
    return 1;
  }
  v = header[7];
  k = header[26];
  if(!k) k = 48;
  n0 = header[28];
  n1 = header[29];
  if(n1 == 0x2E) n1 = 96;
  fn = header[30];
  fd = header[31];

  fread(stopname, 1, 32, stdin);
  fread(copyrite, 1, 56, stdin);
  fread(mnemonic, 1, 8, stdin);
  fread(comments, 1, 56, stdin);
  fread(reserved, 1, 8, stdin);

  read_N_func(&n_vol);
  read_N_func(&n_off);
  read_N_func(&n_ran);
  read_N_func(&n_ins);
  read_N_func(&n_att);
  read_N_func(&n_atd);
  read_N_func(&n_dct);
  read_N_func(&n_dcd);

  read_HN_func(&h_lev);
  read_HN_func(&h_ran);
  read_HN_func(&h_att);
  read_HN_func(&h_atp);
  
  printf("`%s' (%8s): %s\n", stopname, mnemonic, comments);
  printf(" (c) %s\n", copyrite);
  printf("version %d, %d harmonics, notes %d-%d, octave %d/%d\n", v, k, n0, n1, fn, fd);
  printf("volume ");
  print_N_func(&n_vol);
  printf("offset ");
  print_N_func(&n_off);
  printf("random ");
  print_N_func(&n_ran);
  printf("instab ");
  print_N_func(&n_ins);
  printf("atime  ");
  print_N_func(&n_att);
  printf("aofft  ");
  print_N_func(&n_atd);
  printf("dtime  ");
  print_N_func(&n_dct);
  printf("dofft  ");
  print_N_func(&n_dcd);

  printf("Levels:\n");
  print_HN_func(&h_lev);

  printf("\nRandom levels:\n");
  print_HN_func(&h_ran);

  printf("\nAttack time:\n");
  print_HN_func(&h_att);

  printf("\nAttack levels:\n");
  print_HN_func(&h_atp);
}
