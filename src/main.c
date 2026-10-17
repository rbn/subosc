#include <stdio.h>
#include <math.h>
#include "file_io.h"

#define PI 3.14159265358979323846
#define SAMPLE_RATE 48000
#define FREQUENCY 440.0
#define NUM_SAMPLES 48000


void reset_samples(float *samples, int num_samples){

  for (int n = 0; n < num_samples; n++){
    samples[n] = 0.0f;
  }

}

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
// s[n] = sin ( 2*pi*frequency * (sample index/sample rate))
void generate_sine_wave(float frequency, int num_samples, float sample_rate, float *samples){

  for(int n = 0; n < num_samples; n++){
    samples[n] = sin(2.0 * PI * frequency * ((float)n / sample_rate));
  }
}


int main(){

  write_test_file("test.bin");

  return 0;


  // setup
  float samples[NUM_SAMPLES];

  // 100Hz
  printf("100 Hz\n");
  reset_samples(samples, NUM_SAMPLES);
  generate_sine_wave(100, NUM_SAMPLES, SAMPLE_RATE, samples);

  // 0, 120, 240, 360, and 480 f
  print_samples_range(samples, 0, 1);
  print_samples_range(samples, 120, 1);
  print_samples_range(samples, 240, 1);
  print_samples_range(samples, 360, 1);
  print_samples_range(samples, 480, 1);

  reset_samples(samples, NUM_SAMPLES);

  // 200Hz
  printf("200 Hz\n");
  reset_samples(samples, NUM_SAMPLES);
  generate_sine_wave(200, NUM_SAMPLES, SAMPLE_RATE, samples);


  // 0, 120, 240, 360, and 480 f
  print_samples_range(samples, 0, 1);
  print_samples_range(samples, 120, 1);
  print_samples_range(samples, 240, 1);
  print_samples_range(samples, 360, 1);
  print_samples_range(samples, 480, 1);


  return 0;
}
