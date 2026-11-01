#include "filter.h"
#include <math.h>

#define PI 3.14159265358979323846f

void filter_init(LowPassFilter *filter, float sample_rate, float cutoff_hz)
{
    filter->sample_rate = sample_rate;
    filter->state = 0.0f;
    filter_set_cutoff(filter, cutoff_hz);

    filter->coefficient = 1.0f - expf(-2.0f * PI * cutoff_hz / sample_rate);
}

float filter_next(LowPassFilter *filter, float input)
{
    filter->state += filter->coefficient * (input - filter->state);

    return filter->state;
}

void filter_set_cutoff(LowPassFilter *filter, float cutoff_hz)
{
    if (cutoff_hz < 20.f)
    {
        cutoff_hz = 20.0f;
    }

    if (cutoff_hz > filter->sample_rate * 0.45f)
    {
        cutoff_hz = filter->sample_rate * 0.45f;
    }

    filter->cutoff_hz = cutoff_hz;
    filter->coefficient = 1.0f - expf(-2.0f * PI * cutoff_hz / filter->sample_rate);
}
