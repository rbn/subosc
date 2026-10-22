#ifndef SYNTH_H
#define SYNTH_H

#include "oscillator.h"

typedef struct
{
    Oscillator oscillator;
    float gain;
} Synth;

void synth_render(Synth *synth, float *outut, int num_frames);

#endif
