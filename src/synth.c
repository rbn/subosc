#include "synth.h"
#include "note.h"

void synth_render(Synth *synth, float *output, int num_frames)
{
    oscillator_render(&synth->oscillator, output, num_frames);

    for (int n = 0; n < num_frames; n++)
    {
        output[n] *= envelope_next(&synth->envelope) * synth->gain;
    }
}

static int synth_remove_held_note(Synth *synth, int midi_note)
{
    for (int n = 0; n < synth->held_note_count; n++)
    {
        if (synth->held_notes[n] == midi_note)
        {
            for (int j = n; j < synth->held_note_count - 1; j++)
            {
                synth->held_notes[j] = synth->held_notes[j + 1];
            }

            synth->held_note_count--;
            return 1;
        }
    }
    return 0;
}

void synth_note_on(Synth *synth, int midi_note)
{
    if (midi_note < 0 || midi_note > 127)
    {
        return;
    }

    synth_remove_held_note(synth, midi_note);
    synth->held_notes[synth->held_note_count++] = midi_note;

    synth->oscillator.frequency = note_to_frequency(midi_note);
    envelope_note_on(&synth->envelope);
}

void synth_note_off(Synth *synth, int midi_note)
{
    if (synth->held_note_count == 0)
    {
        return;
    }

    int current_note = synth->held_notes[synth->held_note_count - 1];

    if (!synth_remove_held_note(synth, midi_note))
    {
        return;
    }

    if (synth->held_note_count == 0)
    {
        envelope_note_off(&synth->envelope);
    }
    else if (midi_note == current_note)
    {
        int previous_note = synth->held_notes[synth->held_note_count - 1];

        synth->oscillator.frequency = note_to_frequency(previous_note);
    }
}

void synth_all_notes_off(Synth *synth)
{
    synth->held_note_count = 0;
    envelope_note_off(&synth->envelope);
}
