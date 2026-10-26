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

void synth_note_on(Synth *synth, int midi_note)
{
    synth->oscillator.frequency = note_to_frequency(midi_note);
    envelope_note_on(&synth->envelope);
}

void synth_note_off(Synth *synth) { envelope_note_off(&synth->envelope); }
