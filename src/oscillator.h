#ifndef OSCILLATOR_H
#define OSCILLATOR_H

typedef struct {
    float frequency;
    float sample_rate;
    float phase;
} Oscillator;

void generate_sine_wave(float frequency, int num_samples, float sample_rate, float *samples);
void generate_saw_wave(float frequency, int num_samples, float sample_rate, float *samples);
void generate_square_wave(float frequency, int num_samples, float sample_rate, float *samples);
float oscillator_next_saw(Oscillator *osc); 

#endif
