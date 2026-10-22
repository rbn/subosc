#ifndef SYNTH_H
#define SYNTH_H

#include "oscillator.h"
#include "envelope.h"

typedef struct
{
    Oscillator oscillator;
    Envelope envelope;
    float gain;
} Synth;

void synth_render(Synth *synth, float *outut, int num_frames);

#endif
