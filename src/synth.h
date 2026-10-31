#ifndef SYNTH_H
#define SYNTH_H

#include "oscillator.h"
#include "envelope.h"
#include "filter.h"

typedef struct
{
    Oscillator oscillator;
    Envelope envelope;
    float gain;
    int held_notes[128];
    int held_note_count;
    float target_frequency;
    float glide_seconds;
    LowPassFilter filter;
} Synth;

void synth_render(Synth *synth, float *outut, int num_frames);
void synth_note_on(Synth *synth, int midi_note);
void synth_note_off(Synth *synth, int midi_note);
void synth_all_notes_off(Synth *synth);

#endif
