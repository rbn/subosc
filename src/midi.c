#include "midi.h"
#include <alsa/asoundlib.h>
#include <errno.h>

int midi_input_open(MidiInput *midi, const char *device)
{
    midi->input = NULL;
    midi->status = 0;
    midi->note = 0;
    midi->data_count = 0;

    return snd_rawmidi_open(&midi->input, NULL, device, SND_RAWMIDI_NONBLOCK);
}

void midi_input_close(MidiInput *midi)
{
    if (midi->input != NULL)
    {
        snd_rawmidi_close(midi->input);
        midi->input = NULL;
    }
}

int midi_input_next(MidiInput *midi, MidiEvent *event)
{
    unsigned char byte;

    for (;;)
    {
        int result = snd_rawmidi_read(midi->input, &byte, 1);

        if (result == -EAGAIN || result == 0)
        {
            return 0;
        }

        if (result < 0)
        {
            return result;
        }

        if (byte >= 0xF8)
        {
            continue;
        }

        if (byte & 0x80)
        {
            midi->status = byte;
            midi->data_count = 0;
            continue;
        }

        unsigned char type = midi->status & 0xF0;

        if (type != 0x90 && type != 0x80)
        {
            continue;
        }

        if (midi->data_count == 0)
        {
            midi->note = byte;
            midi->data_count = 1;
            continue;
        }

        event->note = midi->note;
        event->velocity = byte;
        event->type = (type == 0x90 && byte > 0) ? MIDI_NOTE_ON : MIDI_NOTE_OFF;

        midi->data_count = 0;
        return 1;
    }
}
