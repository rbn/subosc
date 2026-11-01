#include "filter.h"
#include <math.h>

#define PI 3.14159265358979323846f

static void filter_update_coefficients(LowPassFilter *filter)
{
    float omega = 2.0f * PI * filter->cutoff_hz / filter->sample_rate;

    float cosine = cosf(omega);
    float alpha = sinf(omega) / (2.0f * filter->resonance_q);
    float a0 = 1.0f + alpha;

    filter->b0 = ((1.0f - cosine) / 2.0f) / a0;
    filter->b1 = (1.0f - cosine) / a0;
    filter->b2 = filter->b0;
    filter->a1 = (-2.0f * cosine) / a0;
    filter->a2 = (1.0f - alpha) / a0;
}

void filter_set_cutoff(LowPassFilter *filter, float cutoff_hz)
{
    if (cutoff_hz < 20.0f)
    {
        cutoff_hz = 20.0f;
    }

    if (cutoff_hz > filter->sample_rate * 0.45f)
    {
        cutoff_hz = filter->sample_rate * 0.45f;
    }

    filter->cutoff_hz = cutoff_hz;
    filter_update_coefficients(filter);
}

void filter_set_resonance(LowPassFilter *filter, float resonance_q)
{
    if (resonance_q < 0.5f)
    {
        resonance_q = 0.5f;
    }

    if (resonance_q > 5.0f)
    {
        resonance_q = 5.0f;
    }

    filter->resonance_q = resonance_q;
    filter_update_coefficients(filter);
}

void filter_init(LowPassFilter *filter, float sample_rate, float cutoff_hz)
{
    filter->sample_rate = sample_rate;
    filter->resonance_q = 0.70710678f;
    filter->z1 = 0.0f;
    filter->z2 = 0.0f;

    filter_set_cutoff(filter, cutoff_hz);
}

float filter_next(LowPassFilter *filter, float input)
{
    float output = filter->b0 * input + filter->z1;

    filter->z1 = filter->b1 * input - filter->a1 * output + filter->z2;

    filter->z2 = filter->b2 * input - filter->a2 * output;

    return output;
}
