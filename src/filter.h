#ifndef FILTER_H
#define FILTER_H

typedef struct
{
    float cutoff_hz;
    float sample_rate;
    float state;
    float coefficient;
} LowPassFilter;

void filter_init(LowPassFilter *filter, float sample_rate, float cutoff_hz);
float filter_next(LowPassFilter *filter, float input);

#endif
