#include "file_io.h"
#include "oscillator.h"
#include <stdio.h>
#include "../third_party/miniaudio.h"

#define SAMPLE_RATE 48000
#define NUM_SAMPLES 48000
#define FREQUENCY 440.0
#define OUTPUT_GAIN 0.2f

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

static void
audio_callback(ma_device *device, void *output, const void *input, ma_uint32 frame_count)
{
    Oscillator *osc = device->pUserData;
    float *samples = output;

    (void)input;

    oscillator_render(osc, samples, (int)frame_count);

    for (ma_uint32 n = 0; n < frame_count; n++)
    {
        samples[n] *= OUTPUT_GAIN;
    }
}

int main()
{
    Oscillator osc = {
        .frequency = 100.0f, .sample_rate = SAMPLE_RATE, .phase = 0.0f, .waveform = WAVE_SINE};

    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = ma_format_f32;
    config.playback.channels = 1;
    config.sampleRate = SAMPLE_RATE;
    config.dataCallback = audio_callback;
    config.pUserData = &osc;

    ma_device device;

    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS)
    {
        fprintf(stderr, "Failed to initialize audio device\n");
        return 1;
    }

    if (ma_device_start(&device) != MA_SUCCESS)
    {
        fprintf(stderr, "Failied to start audio device\n");
        ma_device_uninit(&device);
        return 1;
    }

    printf("Playing. Press Enter to stop.\n");
    getchar();

    ma_device_uninit(&device);
    return 0;
}
