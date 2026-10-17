// gcc -Wall -Wextra -std=c11 -o subosc src/main.c -lm
#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846
#define SAMPLE_RATE 48000
#define FREQUENCY 440.0
#define NUM_SAMPLES 48000

int main(void) {
    float s[NUM_SAMPLES];

    for (int n = 0; n < NUM_SAMPLES; n++) {
        s[n] = (float)sin(2.0 * PI * FREQUENCY * ((double)n / SAMPLE_RATE));
    }

    printf("First 20 samples:\n");
    for (int n = 0; n < 20; n++) {
        printf("s[%d] = %f\n", n, s[n]);
    }

    return 0;
}
