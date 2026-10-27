#include "event_queue.h"

void event_queue_init(EventQueue *queue)
{
    atomic_init(&queue->read_index, 0);
    atomic_init(&queue->write_index, 0);
}

int event_queue_push(EventQueue *queue, MidiEvent event)
{
    // TODO: implement
}
