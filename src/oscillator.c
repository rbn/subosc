#include <math.h>
#include "oscillator.h"

#define PI 3.14159265358979323846

// s[n] = sin ( 2*pi*frequency * (sample index/sample rate))
void generate_sine_wave(float frequency, int num_samples, float sample_rate, float *samples){

  for(int n = 0; n < num_samples; n++){
    samples[n] = sin(2.0 * PI * frequency * ((float)n / sample_rate));
  }
}

// 2.0f * phase - 1.0f
void generate_saw_wave(float frequency, int num_samples, float sample_rate, float *samples){

  for(int n = 0; n < num_samples; n++){
    float elapsed_cycles = frequency * (n / sample_rate); 
    float phase = elapsed_cycles - floorf(elapsed_cycles);

    samples[n] = 2.0f * phase - 1.0f;
  }
}
