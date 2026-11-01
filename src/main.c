#include "envelope.h"
#include "file_io.h"
#include "filter.h"
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

typedef enum
{
    COMMAND_NONE,
    COMMAND_NOTE_OFF,
} Command;

typedef struct
{
    Synth synth;
    atomic_int pending_command;
    ScopeCapture scope;
    EventQueue events;
    atomic_int requested_cutoff_hz;
    atomic_int requested_resonance_q100;
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
    case COMMAND_NOTE_OFF:
        synth_all_notes_off(&app->synth);
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
            synth_note_off(&app->synth, event.note);
            break;
        }
    }

    int requested_cutoff = atomic_load(&app->requested_cutoff_hz);

    if ((float)requested_cutoff != app->synth.filter.cutoff_hz)
    {
        filter_set_cutoff(&app->synth.filter, (float)requested_cutoff);
    }

    int requested_q100 = atomic_load(&app->requested_resonance_q100);

    float requested_q = requested_q100 / 100.0f;

    if (requested_q != app->synth.filter.resonance_q)
    {
        filter_set_resonance(&app->synth.filter, requested_q);
    }

    synth_render(&app->synth, output, (int)frame_count);
    scope_capture_push(&app->scope, output, (int)frame_count);
}

int main()
{
    AppState app = {.synth = {
                        .oscillator =
                            {
                                .frequency = 440.0f,
                                .sample_rate = SAMPLE_RATE,
                                .phase = 0.0f,
                                .waveform = WAVE_SAW,
                            },
                        .envelope =
                            {
                                .level = 0.0f,
                                .attack_seconds = 0.0f,
                                .decay_seconds = 0.5f,
                                .sustain_level = 0.4f,
                                .release_seconds = 1.0f,
                                .sample_rate = SAMPLE_RATE,
                                .state = ENV_IDLE,
                            },
                        .gain = OUTPUT_GAIN,
                        .target_frequency = 440.0f,
                        .glide_seconds = 0.2f,
                    }};

    // initializations
    filter_init(&app.synth.filter, SAMPLE_RATE, 1000.0f);
    filter_set_resonance(&app.synth.filter, 2.0f);
    atomic_init(&app.requested_cutoff_hz, 1000);

    // Q value -- 200 means Q = 2.0
    atomic_init(&app.requested_resonance_q100, 200);
    event_queue_init(&app.events);

    atomic_init(&app.pending_command, COMMAND_NONE);
    scope_capture_init(&app.scope);

    // midi controller setup (Arturia KeyStep)
    MidiInput midi;
    int midi_result = midi_input_open(&midi, "hw:0,0,0");

    if (midi_result < 0)
    {
        fprintf(
            stderr, "MIDI unavailable; using computer keyboard: %s\n", snd_strerror(midi_result));
    }

    int midi_available = midi_result >= 0;

    // oscilliscope visual
    InitWindow(800, 400, "Subosc Scope");
    SetTargetFPS(60);

    // miniaudio device and config
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

    const int piano_keys[] = {
        KEY_A, KEY_W, KEY_S, KEY_E, KEY_D, KEY_F, KEY_T, KEY_G, KEY_Y, KEY_H, KEY_U, KEY_J, KEY_K};

    const int piano_key_count = sizeof(piano_keys) / sizeof(piano_keys[0]);
    int octave_offset = 0;

    int active_notes[13];

    for (int n = 0; n < piano_key_count; n++)
    {
        active_notes[n] = -1;
    }

    int cutoff_hz = 1000;
    int resonance_q100 = 200;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_BACKSPACE))
            atomic_store(&app.pending_command, COMMAND_NOTE_OFF);

        if (IsKeyPressed(KEY_F3))
        {
            octave_offset = octave_offset <= -12 ? octave_offset : octave_offset - 12;
        }

        if (IsKeyPressed(KEY_F4))
        {
            octave_offset = octave_offset >= 12 ? octave_offset : octave_offset + 12;
        }

        // filter controls
        if (IsKeyPressed(KEY_LEFT))
        {
            cutoff_hz = cutoff_hz * 4 / 5;
        }

        if (IsKeyPressed(KEY_RIGHT))
        {
            cutoff_hz = cutoff_hz * 5 / 4;
        }

        if (cutoff_hz < 20)
        {
            cutoff_hz = 20;
        }

        if (cutoff_hz > 20000)
        {
            cutoff_hz = 20000;
        }

        if (IsKeyPressed(KEY_DOWN))
        {
            resonance_q100 -= 10;
        }

        if (IsKeyPressed(KEY_UP))
        {
            resonance_q100 += 10;
        }

        if (resonance_q100 < 50)
        {
            resonance_q100 = 50;
        }

        if (resonance_q100 > 500)
        {
            resonance_q100 = 500;
        }

        atomic_store(&app.requested_resonance_q100, resonance_q100);
        atomic_store(&app.requested_cutoff_hz, cutoff_hz);

        for (int n = 0; n < piano_key_count; n++)
        {
            MidiEvent event = {
                .note = 60 + n + octave_offset,
                .velocity = 100,
            };

            if (IsKeyPressed(piano_keys[n]))
            {
                event.type = MIDI_NOTE_ON;
                active_notes[n] = 60 + n + octave_offset;
                event.note = active_notes[n];

                if (!event_queue_push(&app.events, event))
                {
                    fprintf(stderr, "Keyboard event queue is full\n");
                }
            }

            if (IsKeyReleased(piano_keys[n]) && active_notes[n] != -1)
            {
                event.type = MIDI_NOTE_OFF;
                event.velocity = 0;
                event.note = active_notes[n];
                active_notes[n] = -1;

                if (!event_queue_push(&app.events, event))
                {
                    fprintf(stderr, "Keyboard event queue is full\n");
                }
            }
        }

        if (midi_available)
        {
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

                midi_available = 0;
            }
        }

        scope_display_update(&display, &app.scope);

        // drawing
        BeginDrawing();
        ClearBackground(BLACK);
        scope_display_draw(&display);
        DrawText(TextFormat("Cutoff: %d Hz  Left/Right", cutoff_hz), 390, 10, 14, WHITE);
        DrawText(
            TextFormat("Resonance Q: %.2f  Up/Down", resonance_q100 / 100.0f), 390, 28, 14, WHITE);
        EndDrawing();
    }
    ma_device_uninit(&device);
    midi_input_close(&midi);
    CloseWindow();
    return 0;
}
