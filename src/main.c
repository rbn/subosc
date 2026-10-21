#include "file_io.h"
#include "oscillator.h"
#include <stdio.h>

#define SAMPLE_RATE 48000
#define NUM_SAMPLES 48000
#define FREQUENCY 440.0

void print_samples(float *samples, int count)
{
    for (int n = 0; n < count; n++)
    {
        printf("%d. samples[%d] is %f\n", n + 1, n, samples[n]);
    }
}

void print_samples_range(float *samples, int start, int count)
{
    for (int n = 0; n < count; n++)
    {
        printf("%d. samples[%d] is %f\n", n + 1, start + n, samples[start + n]);
    }
}

void diagnostics(float *samples)
{
    print_samples_range(samples, 0, 1);
    print_samples_range(samples, 120, 1);
    print_samples_range(samples, 240, 1);
    print_samples_range(samples, 360, 1);
    print_samples_range(samples, 480, 1);
}

int main()
{
    Oscillator osc = {.frequency = 100.0f,
                      .sample_rate = SAMPLE_RATE,
                      .phase = 0.0f,
                      .waveform = WAVE_SINE};

    float samples[NUM_SAMPLES];

    for (int n = 0; n < NUM_SAMPLES; n++)
    {
        samples[n] = oscillator_next(&osc);
    }

    if (write_wav("sin.wav", samples, NUM_SAMPLES, SAMPLE_RATE) != 0)
    {
        fprintf(stderr, "Error writing wav file\n");
        return 1;
    }

    osc.waveform = WAVE_SAW;
    osc.phase = 0;

    for (int n = 0; n < NUM_SAMPLES; n++)
    {
        samples[n] = oscillator_next(&osc);
    }

    if (write_wav("saw.wav", samples, NUM_SAMPLES, SAMPLE_RATE) != 0)
    {
        fprintf(stderr, "Error writing wav file\n");
        return 1;
    }

    osc.waveform = WAVE_SQUARE;
    osc.phase = 0;

    for (int n = 0; n < NUM_SAMPLES; n++)
    {
        samples[n] = oscillator_next(&osc);
    }

    if (write_wav("square.wav", samples, NUM_SAMPLES, SAMPLE_RATE) != 0)
    {
        fprintf(stderr, "Error writing wav file\n");
        return 1;
    }

    return 0;
}
