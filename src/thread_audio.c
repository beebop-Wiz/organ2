#include <pthread.h>
#include <stdio.h>
#include <jack/jack.h>
#include <jack/midiport.h>
#include <sys/time.h>

#include "thread_audio.h"
#include "thread_iface.h"
#include "thread.h"
#include "event.h"
#include "model.h"
#include "division.h"
#include "rank.h"

extern thread_t t_iface;

jack_port_t *output_l, *output_r, *imidi;
jack_client_t *client;

#define MAX_FRAME_MIDI 32

struct midiframe {
  jack_midi_event_t e[MAX_FRAME_MIDI];
  int nevents;
};

struct model *mod;

int process(jack_nframes_t nframes, void *arg) {
  jack_midi_event_t midievent;
  void *midibuf = jack_port_get_buffer(imidi, nframes);
  jack_default_audio_sample_t *out_l, *out_r;
  out_l = jack_port_get_buffer(output_l, nframes);
  out_r = jack_port_get_buffer(output_r, nframes);
  jack_nframes_t i, count = jack_midi_get_event_count(midibuf);
  struct midiframe *evts = calloc(nframes, sizeof(struct midiframe));
  for(i = 0; i < count; i++) {
    jack_midi_event_get(&midievent, midibuf, i);
    evts[midievent.time].e[evts[midievent.time].nevents++] = midievent;
  }
  int j;
  for(i = 0; i < nframes; i++) {
    for(j = 0; j < evts[i].nevents; j++) {
      midievent = evts[i].e[j];
      m_proc_midi(mod, midievent);
    }
    float r = m_advance(mod);
    out_r[i] = r;
    out_l[i] = r;
  }
  free(evts);
  return 0;
}

void audio_init() {
  mod = create_model();

  struct division *d0 = create_division();
  d0->midich = 0;
  struct rank *r0 = calloc(1, sizeof(struct rank));
  r_read(r0, fopen("aeolus_stops/default/I_principal_8.ae0", "r"));
  add_rank(d0, r0);
  add_division(mod, d0);

  struct division *d1 = create_division();
  d1->midich = 1;
  struct rank *r1 = calloc(1, sizeof(struct rank));
  r_read(r1, fopen("aeolus_stops/default/I_trumpet.ae0", "r"));
  add_rank(d1, r1);
  add_division(mod, d1);
  
  //  thread_signal(t_iface, ecreate(E_INIT, 0));

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
