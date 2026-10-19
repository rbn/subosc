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
    // 100Hz
  printf("100 Hz\n");
  generate_sine_wave(100, NUM_SAMPLES, SAMPLE_RATE, samples);

  // 0, 120, 240, 360, and 480 f
  print_samples_range(samples, 0, 1);
  print_samples_range(samples, 120, 1);
  print_samples_range(samples, 240, 1);
  print_samples_range(samples, 360, 1);
  print_samples_range(samples, 480, 1);

  // 200Hz
  printf("200 Hz\n");
  generate_sine_wave(200, NUM_SAMPLES, SAMPLE_RATE, samples);


  // 0, 120, 240, 360, and 480 f
  print_samples_range(samples, 0, 1);
  print_samples_range(samples, 120, 1);
  print_samples_range(samples, 240, 1);
  print_samples_range(samples, 360, 1);
  print_samples_range(samples, 480, 1);
}

int main(){
  printf("hello from main\n");

  float samples[NUM_SAMPLES];
  generate_sine_wave(100, NUM_SAMPLES, SAMPLE_RATE, samples);

  if (write_wav("bass.wav", samples, NUM_SAMPLES, sample_rate) != 0){
    fprintf(stderr, "Error writing wav file\n");
    return 1;
  }

  return 0;
}
