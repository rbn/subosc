#include <assert.h>
#include "event_queue.h"

int main(void)
{
    EventQueue queue;
    event_queue_init(&queue);

    MidiEvent output;

    assert(event_queue_pop(&queue, &output) == 0);

    MidiEvent on = {MIDI_NOTE_ON, 60, 85};
    MidiEvent off = {MIDI_NOTE_OFF, 60, 64};

    assert(event_queue_push(&queue, on) == 1);
    assert(event_queue_push(&queue, off) == 1);

    assert(event_queue_pop(&queue, &output) == 1);
    assert(output.type == MIDI_NOTE_ON && output.note == 60);
    assert(output.velocity == 85);

    assert(event_queue_pop(&queue, &output) == 1);
    assert(output.type == MIDI_NOTE_OFF && output.note == 60);
    assert(output.velocity == 64);

    assert(event_queue_pop(&queue, &output) == 0);

    return 0;
}
