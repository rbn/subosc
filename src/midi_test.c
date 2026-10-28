#include <alsa/asoundlib.h>
#include <stdio.h>

int main(void)
{
    snd_rawmidi_t *input;
    int result = snd_rawmidi_open(&input, NULL, "hw:0,0,0", 0);

    if (result < 0)
    {
        fprintf(stderr, "Cannot open MIDI input: %s\n", snd_strerror(result));
        return 1;
    }

    printf("Press some keys. Ctrl+C to stop.\n");

    unsigned char byte;

    unsigned char status = 0;
    unsigned char note = 0;
    int data_count = 0;

    while ((result = snd_rawmidi_read(input, &byte, 1)) == 1)
    {
        // midi clock guard
        if (byte >= 0xF8)
        {
            continue;
        }

        // status byte guard
        if (byte & 0x80)
        {
            status = byte;
            data_count = 0;
            continue;
        }
        unsigned char type = status & 0xF0;

        if (type != 0x90 && type != 0x80)
        {
            continue;
        }

        if (data_count == 0)
        {
            note = byte;
            data_count = 1;
        }
        else
        {
            unsigned char velocity = byte;

            if (type == 0x90 && velocity > 0)
            {
                printf(
                    "Note ON:  note=%u velocity=%u\n", (unsigned int)note, (unsigned int)velocity);
            }
            else
            {
                printf(
                    "Note OFF: note=%u velocity=%u\n", (unsigned int)note, (unsigned int)velocity);
            }

            data_count = 0;
        }
    }

    if (result < 0)
    {
        fprintf(stderr, "MIDI read failed: %s\n", snd_strerror(result));
    }

    snd_rawmidi_close(input);
    return result < 0 ? 1 : 0;
}
