#include <pthread.h>
#include "thread_audio.h"
#include "thread_iface.h"
#include "thread.h"
#include "event.h"

#include <stdio.h>
#include <jack/jack.h>
#include <jack/midiport.h>

extern thread_t t_iface;

jack_port_t *output_l, *output_r, *imidi;
jack_client_t *client;

int process(jack_nframes_t nframes, void *arg) {
  return 0;
}

void audio_init() {
  thread_signal(t_iface, ecreate(E_INIT, 0));

  jack_status_t status;
  client = jack_client_open("organ", JackNullOption, &status, NULL);
  jack_set_process_callback(client, process, 0);
  output_l = jack_port_register(client, "L", "32 bit float mono audio", JackPortIsOutput, 0);
  output_r = jack_port_register(client, "R", "32 bit float mono audio", JackPortIsOutput, 0);
  imidi = jack_port_register(client, "midi", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 1);
  jack_activate(client);
}

void ta_evcb(struct event evt) {
  switch(evt.type) {
  case E_INIT:
    printf("[Audio] Initializing...\n");
    audio_init();
    printf("[Audio] Initialized.\n");
    break;
  }
}
