#include <stdio.h>
#include "file_io.h"
#include "oscillator.h"

#define SAMPLE_RATE 48000
#define NUM_SAMPLES 48000
#define FREQUENCY 440.0

void print_samples(float *samples, int count){

  for (int n = 0; n < count; n++){
    printf("%d. samples[%d] is %f\n", n + 1, n, samples[n]);
  }

}

void print_samples_range(float *samples, int start, int count){
  for (int n = 0; n < count; n++){
    printf("%d. samples[%d] is %f\n", n + 1, start + n, samples[start + n]);
  }
}

void diagnostics(float *samples){

  print_samples_range(samples, 0, 1);
  print_samples_range(samples, 120, 1);
  print_samples_range(samples, 240, 1);
  print_samples_range(samples, 360, 1);
  print_samples_range(samples, 480, 1);

}

int main(){

  Oscillator osc = {
    .frequency = 110.5f,
    .sample_rate = SAMPLE_RATE,
    .phase = 0.0f,
    .waveform = WAVE_SINE
  };

  float samples[NUM_SAMPLES];

  for(int n = 0; n < NUM_SAMPLES / 2; n++){
    samples[n] = oscillator_next_saw(&osc);
  }

  for(int n = NUM_SAMPLES / 2; n < NUM_SAMPLES; n++){
    samples[n] = oscillator_next_saw(&osc);
  }

  if (write_wav("saw.wav", samples, NUM_SAMPLES, SAMPLE_RATE) != 0){
    fprintf(stderr, "Error writing wav file\n");
    return 1;
  }

  for(int n = 0; n < NUM_SAMPLES; n++){
    samples[n] = oscillator_next_saw(&osc);
  }

  if (write_wav("sin.wav", samples, NUM_SAMPLES, SAMPLE_RATE) != 0){
    fprintf(stderr, "Error writing wav file\n");
    return 1;
  }

  for(int n = 0; n < NUM_SAMPLES; n++){
    samples[n] = oscillator_next_square(&osc);
  }

  if (write_wav("square.wav", samples, NUM_SAMPLES, SAMPLE_RATE) != 0){
    fprintf(stderr, "Error writing wav file\n");
    return 1;
  }
  // generate_sine_wave(100, NUM_SAMPLES, SAMPLE_RATE, samples);
  // // diagnostics(samples);

  // if (write_wav("bass.wav", samples, NUM_SAMPLES, SAMPLE_RATE) != 0){
  //   fprintf(stderr, "Error writing wav file\n");
  //   return 1;
  // }

  // SAW WAVE

  // generate_saw_wave(100, NUM_SAMPLES, SAMPLE_RATE, samples);

  // if (write_wav("saw.wav", samples, NUM_SAMPLES, SAMPLE_RATE) != 0){
  //   fprintf(stderr, "Error writing wav file\n");
  //   return 1;
  // }

  // SQUARE WAVE
  // generate_square_wave(100, NUM_SAMPLES, SAMPLE_RATE, samples);
  // //diagnostics(samples);

  // if (write_wav("square.wav", samples, NUM_SAMPLES, SAMPLE_RATE) != 0){
  //   fprintf(stderr, "Error writing wav file\n");
  //   return 1;
  // }

  return 0;
}
