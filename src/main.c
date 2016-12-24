#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "thread.h"
#include "thread_audio.h"
#include "thread_iface.h"

thread_t t_audio;
thread_t t_iface;

int main(void) {
  t_audio = thread_create(ta_evcb);
  t_iface = thread_create(ti_evcb);
  thread_signal(t_audio, ecreate(E_INIT, 0));
  sleep(-1);
  return 0;
}
