#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <stdatomic.h>
#include "midi.h"

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 201112L
#error "This project requires C11 or later."
#endif

#if defined(__STDC_NO_ATOMICS__) && __STDC_NO_ATOMICS__ == 1
#error "This project requires C atomic support."
#endif

#define EVENT_QUEUE_CAPACITY 256

typedef struct
{
    MidiEvent events[EVENT_QUEUE_CAPACITY];
    atomic_int read_index;
    atomic_int write_index;
} EventQueue;

void event_queue_init(EventQueue *queue);
int event_queue_push(EventQueue *queue, MidiEvent event);
int event_queue_pop(EventQueue *queue, MidiEvent *event);

#endif
