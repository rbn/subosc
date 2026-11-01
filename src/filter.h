#ifndef FILTER_H
#define FILTER_H

typedef struct
{
    float cutoff_hz;
    float sample_rate;
    float resonance_q;

    float b0;
    float b1;
    float b2;
    float a1;
    float a2;

    float z1;
    float z2;
} LowPassFilter;

void filter_init(LowPassFilter *filter, float sample_rate, float cutoff_hz);
float filter_next(LowPassFilter *filter, float input);
void filter_set_cutoff(LowPassFilter *filter, float cutoff_hz);
void filter_set_resonance(LowPassFilter *filter, float resonance_q);
#endif
