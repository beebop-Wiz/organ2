#include <stdio.h>
#include <math.h>
#include <limits.h>

int main(void) {
  int i;
  for(i = 0; i < 1024; i++) {
    printf("%d, ", (int) (sin(2. * 3.14145926 * i / 1024.) * (double) INT_MAX));
    if(i % 8 == 7) printf("\n");
  }
}
