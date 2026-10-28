#include "event_queue.h"

void event_queue_init(EventQueue *queue)
{
    atomic_init(&queue->read_index, 0);
    atomic_init(&queue->write_index, 0);
}

int event_queue_push(EventQueue *queue, MidiEvent event)
{
    int write = atomic_load(&queue->write_index);
    int next = (write + 1) % EVENT_QUEUE_CAPACITY;

    if (next == atomic_load(&queue->read_index))
    {
        return 0;
    }

    queue->events[write] = event;
    atomic_store(&queue->write_index, next);

    return 1;
}

int event_queue_pop(EventQueue *queue, MidiEvent *event)
{
    int read = atomic_load(&queue->read_index);

    if (read == atomic_load(&queue->write_index))
    {
        return 0;
    }

    *event = queue->events[read];

    int next = (read + 1) % EVENT_QUEUE_CAPACITY;
    atomic_store(&queue->read_index, next);

    return 1;
}
