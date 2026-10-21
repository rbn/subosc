#include "oscillator.h"
#include <math.h>

#define PI 3.14159265358979323846f

// -- amplitude formulae

// sinf(2.0f * PI * phase)          // sine
// 2.0f * phase - 1.0f              // saw
// phase < 0.5f ? 1.0f : -1.0f     // square

// -- SINE WAVE
// s[n] = sin ( 2*pi*frequency * (sample index/sample rate))

static void oscillator_advance(Oscillator *osc) {

  float phase_increment = osc->frequency / osc->sample_rate;
  osc->phase += phase_increment;
  osc->phase -= floorf(osc->phase);
}

float oscillator_next(Oscillator *osc) {
  switch (osc->waveform) {
  case WAVE_SINE:
    oscillator_next_sine(osc);
    return sinf(2.0f * PI * osc->phase);
  case WAVE_SAW:
    oscillator_next_saw(osc);
    return 2.0f * osc->phase - 1.0f;
  case WAVE_SQUARE:
    oscillator_next_square(osc);
    return osc->phase < 0.5f ? 1.0f : -1.0f;
  default:
    return 0.0f;
  }
}

void generate_sine_wave(float frequency, int num_samples, float sample_rate,
                        float *samples) {

  for (int n = 0; n < num_samples; n++) {
    samples[n] = sinf(2.0f * PI * frequency * ((float)n / sample_rate));
  }
}

float oscillator_next_sine(Oscillator *osc) {

  float amplitude = sinf(2.0f * PI * osc->phase);
  oscillator_advance(osc);
  return amplitude;
}

// -- SAW WAVE
// 2.0f * phase - 1.0f
void generate_saw_wave(float frequency, int num_samples, float sample_rate,
                       float *samples) {

  for (int n = 0; n < num_samples; n++) {
    float elapsed_cycles = frequency * (n / sample_rate);
    float phase = elapsed_cycles - floorf(elapsed_cycles);

    samples[n] = 2.0f * phase - 1.0f;
  }
}

float oscillator_next_saw(Oscillator *osc) {

  float amplitude = 2.0f * osc->phase - 1.0f;
  oscillator_advance(osc);
  return amplitude;
}

// -- SQUARE WAVE
void generate_square_wave(float frequency, int num_samples, float sample_rate,
                          float *samples) {

  for (int n = 0; n < num_samples; n++) {
    float elapsed_cycles = frequency * (n / sample_rate);
    float phase = elapsed_cycles - floorf(elapsed_cycles);

    samples[n] = phase < 0.5f ? 1.0f : -1.0f;
  }
}

float oscillator_next_square(Oscillator *osc) {

  float amplitude = osc->phase < 0.5f ? 1.0f : -1.0f;
  oscillator_advance(osc);
  return amplitude;
}
