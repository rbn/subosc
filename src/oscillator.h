#ifndef OSCILLATOR_H
#define OSCILLATOR_H

typedef enum {
  WAVE_SINE,
  WAVE_SAW,
  WAVE_SQUARE
} Waveform;

typedef struct {
    float frequency;
    float sample_rate;
    float phase;
    Waveform waveform;
} Oscillator;

void generate_sine_wave(float frequency, int num_samples, float sample_rate, float *samples);
void generate_saw_wave(float frequency, int num_samples, float sample_rate, float *samples);
void generate_square_wave(float frequency, int num_samples, float sample_rate, float *samples);
float oscillator_next(Oscillator *osc);
float oscillator_next_sine(Oscillator *osc);
float oscillator_next_saw(Oscillator *osc); 
float oscillator_next_square(Oscillator *osc); 

#endif
