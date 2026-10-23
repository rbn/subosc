#include "envelope.h"
#include "file_io.h"
#include "oscillator.h"
#include <stdio.h>
#include "../third_party/miniaudio.h"
#include "synth.h"
#include <stdatomic.h>
#include <raylib.h>

#define SAMPLE_RATE 48000
#define NUM_SAMPLES 48000
#define FREQUENCY 440.0
#define OUTPUT_GAIN 0.2f
#define SCOPE_SAMPLES 960
#define SCOPE_CAPTURE_SAMPLES (SCOPE_SAMPLES * 2)

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

typedef enum
{
    COMMAND_NONE,
    COMMAND_NOTE_ON,
    COMMAND_NOTE_OFF,
} Command;

typedef struct
{
    Synth synth;
    atomic_int pending_command;
    float scope_samples[SCOPE_CAPTURE_SAMPLES];
    int scope_count;
    atomic_int scope_ready;
} AppState;

static void audio_callback(ma_device *device,
                           void *output,
                           const void *input,
                           ma_uint32 frame_count)
{
    AppState *app = device->pUserData;

    (void)input;

    int command = atomic_exchange(&app->pending_command, COMMAND_NONE);

    switch (command)
    {
    case COMMAND_NOTE_ON:
        envelope_note_on(&app->synth.envelope);
        break;
    case COMMAND_NOTE_OFF:
        envelope_note_off(&app->synth.envelope);
        break;
    }

    synth_render(&app->synth, output, (int)frame_count);

    if (atomic_load(&app->scope_ready) == 0)
    {
        const float *samples = output;

        for (ma_uint32 n = 0; n < frame_count; n++)
        {
            app->scope_samples[app->scope_count++] = samples[n];

            if (app->scope_count == SCOPE_CAPTURE_SAMPLES)
            {
                atomic_store(&app->scope_ready, 1);
                break;
            }
        }
    }
}

int main()
{
    Oscillator osc = {
        .frequency = 440.0f,
        .sample_rate = SAMPLE_RATE,
        .phase = 0.0f,
        .waveform = WAVE_SINE,
    };

    AppState app = {.synth = {
                        .oscillator = osc,
                        .envelope =
                            {
                                .level = 0.0f,
                                .attack_seconds = 2.0f,
                                .release_seconds = 1.0f,
                                .sample_rate = SAMPLE_RATE,
                                .state = ENV_IDLE,
                            },
                        .gain = OUTPUT_GAIN,
                    }};

    atomic_init(&app.pending_command, COMMAND_NONE);
    atomic_init(&app.scope_ready, 0);

    InitWindow(800, 400, "Subosc Scope");
    SetTargetFPS(60);

    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = ma_format_f32;
    config.playback.channels = 1;
    config.sampleRate = SAMPLE_RATE;
    config.dataCallback = audio_callback;
    config.pUserData = &app;

    ma_device device;

    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS)
    {
        fprintf(stderr, "Failed to initialize audio device\n");
        CloseWindow();
        return 1;
    }

    if (ma_device_start(&device) != MA_SUCCESS)
    {
        fprintf(stderr, "Failied to start audio device\n");
        ma_device_uninit(&device);
        CloseWindow();
        return 1;
    }

    float display_samples[SCOPE_SAMPLES] = {0};
    float captured_samples[SCOPE_CAPTURE_SAMPLES];

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_N))
            atomic_store(&app.pending_command, COMMAND_NOTE_ON);

        if (IsKeyPressed(KEY_R))
            atomic_store(&app.pending_command, COMMAND_NOTE_OFF);

        if (atomic_load(&app.scope_ready) == 1)
        {
            for (int n = 0; n < SCOPE_CAPTURE_SAMPLES; n++)
            {
                captured_samples[n] = app.scope_samples[n];
            }

            app.scope_count = 0;
            atomic_store(&app.scope_ready, 0);

            int start = 0;

            for (int n = 1; n <= SCOPE_SAMPLES; n++)
            {
                if (captured_samples[n - 1] <= 0.0f && captured_samples[n] > 0.0f)
                {
                    start = n;
                    break;
                }
            }

            for (int n = 0; n < SCOPE_SAMPLES; n++)
            {
                display_samples[n] = captured_samples[start + n];
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        const int left = 70;
        const int top = 40;
        const int width = 700;
        const int height = 300;
        const float amplitude_range = 0.25f;

        /* Horizontal grid: amplitude from +0.25 to -0.25. */
        for (int row = 0; row <= 10; row++)
        {
            int y = top + row * height / 10;
            float amplitude = amplitude_range - row * 0.05f;

            DrawLine(left, y, left + width, y, row == 5 ? GRAY : (Color){35, 35, 35, 255});

            if (row % 2 == 0 || row == 5)
            {
                DrawText(TextFormat("%+.2f", amplitude), 10, y - 5, 10, LIGHTGRAY);
            }
        }

        /* Vertical grid: one division every 2 ms. */
        for (int column = 0; column <= 10; column++)
        {
            int x = left + column * width / 10;

            DrawLine(x, top, x, top + height, (Color){35, 35, 35, 255});

            DrawText(TextFormat("%d", column * 2), x - 5, top + height + 10, 10, LIGHTGRAY);
        }

        DrawText("Amplitude", 10, 15, 14, LIGHTGRAY);
        DrawText("Time from trigger (ms)", 300, 375, 14, LIGHTGRAY);

        /* Draw the captured output samples. */
        for (int n = 0; n < SCOPE_SAMPLES - 1; n++)
        {
            float x1 = left + n * (float)width / SCOPE_SAMPLES;
            float x2 = left + (n + 1) * (float)width / SCOPE_SAMPLES;

            float center_y = top + height / 2.0f;
            float scale_y = height / (2.0f * amplitude_range);

            float y1 = center_y - display_samples[n] * scale_y;
            float y2 = center_y - display_samples[n + 1] * scale_y;

            DrawLineV((Vector2){x1, y1}, (Vector2){x2, y2}, GREEN);
        }

        EndDrawing();
    }

    ma_device_uninit(&device);
    CloseWindow();
    return 0;
}
