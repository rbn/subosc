#include "envelope.h"
#include "file_io.h"
#include "oscillator.h"
#include <stdio.h>
#include "../third_party/miniaudio.h"
#include "synth.h"

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

static void audio_callback(ma_device *device,
                           void *output,
                           const void *input,
                           ma_uint32 frame_count)
{
    Synth *synth = device->pUserData;
    static ma_uint64 frames_rendered = 0;
    static int released = 0;

    (void)input;

    if (!released && frames_rendered >= SAMPLE_RATE * 4)
    {
        envelope_note_off(&synth->envelope);
        released = 1;
    }

    synth_render(synth, output, (int)frame_count);
    frames_rendered += frame_count;
}

int main()
{
    Oscillator osc = {
        .frequency = 220.0f,
        .sample_rate = SAMPLE_RATE,
        .phase = 0.0f,
        .waveform = WAVE_SINE,
    };

    Synth synth = {
        .oscillator = osc,
        .envelope =
            {
                .level = 0.0f,
                .attack_seconds = 2.0f,
                .sample_rate = SAMPLE_RATE,
                .state = ENV_IDLE,
                .release_seconds = 1.0f,
            },
        .gain = OUTPUT_GAIN,
    };

    envelope_note_on(&synth.envelope);

    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = ma_format_f32;
    config.playback.channels = 1;
    config.sampleRate = SAMPLE_RATE;
    config.dataCallback = audio_callback;
    config.pUserData = &synth;

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
