#include "filter.h"
#include <math.h>

#define PI 3.14159265358979323846f

void filter_init(LowPassFilter *filter, float sample_rate, float cutoff_hz)
{
    filter->sample_rate = sample_rate;
    filter->cutoff_hz = cutoff_hz;
    filter->state = 0.0f;

    filter->coefficient = 1.0f - expf(-2.0f * PI * cutoff_hz / sample_rate);
}

float filter_next(LowPassFilter *filter, float input)
{
    filter->state += filter->coefficient * (input - filter->state);

    return filter->state;
}
