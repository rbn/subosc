#ifndef MIDI_H
#define MIDI_H

#include <alsa/asoundlib.h>

typedef enum
{
    MIDI_NOTE_ON,
    MIDI_NOTE_OFF
} MidiEventType;

typedef struct
{
    MidiEventType type;
    int note;
    int velocity;
} MidiEvent;

typedef struct
{
    snd_rawmidi_t *input;
    unsigned char status;
    unsigned char note;
    int data_count;
} MidiInput;

int midi_input_open(MidiInput *midi, const char *device);
int midi_input_next(MidiInput *midi, MidiEvent *event);
void midi_input_close(MidiInput *midi);
#endif
