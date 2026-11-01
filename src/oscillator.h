#ifndef OSCILLATOR_H
#define OSCILLATOR_H

typedef enum
{
    WAVE_SINE,
    WAVE_SAW,
    WAVE_SQUARE,
    WAVE_COUNT
} Waveform;

typedef struct
{
    float frequency;
    float sample_rate;
    float phase;
    Waveform waveform;
} Oscillator;

float oscillator_next(Oscillator *osc);
void oscillator_render(Oscillator *osc, float *output, int num_frames);
Waveform oscillator_cycle_waveform(Waveform wf);
const char *oscillator_get_wave_desc(Waveform wf);

#endif
