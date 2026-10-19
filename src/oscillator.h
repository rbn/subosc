#ifndef OSCILLATOR_H
#define OSCILLATOR_H

void generate_sine_wave(float frequency, int num_samples, float sample_rate, float *samples);
void generate_saw_wave(float frequency, int num_samples, float sample_rate, float *samples);

#endif
