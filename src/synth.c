#include "synth.h"

void synth_render(Synth *synth, float *output, int num_frames)
{
    oscillator_render(&synth->oscillator, output, num_frames);

    for (int n = 0; n < num_frames; n++)
    {
        output[n] *= synth->gain;
    }
}
