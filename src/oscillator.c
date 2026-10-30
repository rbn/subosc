#include "oscillator.h"
#include <math.h>

#define PI 3.14159265358979323846f

// -- amplitude formulae

// sinf(2.0f * PI * phase)          // sine
// 2.0f * phase - 1.0f              // saw
// phase < 0.5f ? 1.0f : -1.0f     // square

static void oscillator_advance(Oscillator *osc)
{
    float phase_increment = osc->frequency / osc->sample_rate;
    osc->phase += phase_increment;
    osc->phase -= floorf(osc->phase);
}

static float oscillator_next_sine(Oscillator *osc)
{
    float amplitude = sinf(2.0f * PI * osc->phase);
    oscillator_advance(osc);
    return amplitude;
}

static float oscillator_next_saw(Oscillator *osc)
{
    float amplitude = 2.0f * osc->phase - 1.0f;
    oscillator_advance(osc);
    return amplitude;
}

static float oscillator_next_square(Oscillator *osc)
{
    float amplitude = osc->phase < 0.5f ? 1.0f : -1.0f;
    oscillator_advance(osc);
    return amplitude;
}

float oscillator_next(Oscillator *osc)
{
    switch (osc->waveform)
    {
    case WAVE_SINE:
        return oscillator_next_sine(osc);
    case WAVE_SAW:
        return oscillator_next_saw(osc);
    case WAVE_SQUARE:
        return oscillator_next_square(osc);
    default:
        return 0.0f;
    }
}

void oscillator_render(Oscillator *osc, float *output, int num_frames)
{
    for (int n = 0; n < num_frames; n++)
    {
        output[n] = oscillator_next(osc);
    }
}
