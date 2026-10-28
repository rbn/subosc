#include "envelope.h"
#include "file_io.h"
#include "midi.h"
#include "oscillator.h"
#include <stdio.h>
#include "../third_party/miniaudio.h"
#include "synth.h"
#include <stdatomic.h>
#include <raylib.h>
#include "scope.h"
#include "event_queue.h"

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

typedef enum
{
    COMMAND_NONE,
    COMMAND_NOTE_C,
    COMMAND_NOTE_E,
    COMMAND_NOTE_G,
    COMMAND_NOTE_OFF,
} Command;

typedef struct
{
    Synth synth;
    atomic_int pending_command;
    ScopeCapture scope;
    EventQueue events;
} AppState;

static void audio_callback(ma_device *device,
                           void *output,
                           const void *input,
                           ma_uint32 frame_count)
{
    AppState *app = device->pUserData;

    (void)input; // ignore this parameter - MA says we need it, we dont use it right now

    int command = atomic_exchange(&app->pending_command, COMMAND_NONE);

    switch (command)
    {
    case COMMAND_NOTE_C:
        synth_note_on(&app->synth, 60);
        break;
    case COMMAND_NOTE_E:
        synth_note_on(&app->synth, 64);
        break;
    case COMMAND_NOTE_G:
        synth_note_on(&app->synth, 67);
        break;
    case COMMAND_NOTE_OFF:
        synth_note_off(&app->synth);
        break;
    }

    MidiEvent event;
    while (event_queue_pop(&app->events, &event))
    {
        switch (event.type)
        {
        case MIDI_NOTE_ON:
            synth_note_on(&app->synth, event.note);
            break;
        case MIDI_NOTE_OFF:
            synth_note_off(&app->synth);
            break;
        }
    }

    synth_render(&app->synth, output, (int)frame_count);
    scope_capture_push(&app->scope, output, (int)frame_count);
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

    event_queue_init(&app.events);

    atomic_init(&app.pending_command, COMMAND_NONE);
    scope_capture_init(&app.scope);

    MidiInput midi;
    int midi_result = midi_input_open(&midi, "hw:0,0,0");

    if (midi_result < 0)
    {
        fprintf(stderr, "Failed to open MIDI input: %s\n", snd_strerror(midi_result));
        return 1;
    }

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
        midi_input_close(&midi);
        return 1;
    }

    if (ma_device_start(&device) != MA_SUCCESS)
    {
        fprintf(stderr, "Failied to start audio device\n");
        ma_device_uninit(&device);
        CloseWindow();
        midi_input_close(&midi);
        return 1;
    }

    ScopeDisplay display = {0};

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_A))
            atomic_store(&app.pending_command, COMMAND_NOTE_C);

        if (IsKeyPressed(KEY_S))
            atomic_store(&app.pending_command, COMMAND_NOTE_E);

        if (IsKeyPressed(KEY_D))
            atomic_store(&app.pending_command, COMMAND_NOTE_G);

        if (IsKeyPressed(KEY_R))
            atomic_store(&app.pending_command, COMMAND_NOTE_OFF);

        MidiEvent event;

        while ((midi_result = midi_input_next(&midi, &event)) == 1)
        {
            if (!event_queue_push(&app.events, event))
            {
                fprintf(stderr, "MIDI event queue is full\n");
            }
        }

        if (midi_result < 0)
        {
            fprintf(stderr, "MIDI read failed: %s\n", snd_strerror(midi_result));
            break;
        }

        scope_display_update(&display, &app.scope);

        BeginDrawing();
        ClearBackground(BLACK);
        scope_display_draw(&display);
        EndDrawing();
    }
    ma_device_uninit(&device);
    midi_input_close(&midi);
    CloseWindow();
    return 0;
}
