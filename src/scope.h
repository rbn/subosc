#ifndef SCOPE_H
#define SCOPE_H

#include <stdatomic.h>

#define SCOPE_SAMPLES 960
#define SCOPE_CAPTURE_SAMPLES (SCOPE_SAMPLES * 2)

typedef struct
{
    float samples[SCOPE_CAPTURE_SAMPLES];
    int count;
    atomic_int ready;
} ScopeCapture;

typedef struct
{
    float samples[SCOPE_SAMPLES];
} ScopeDisplay;

void scope_display_update(ScopeDisplay *display, ScopeCapture *capture);
void scope_display_draw(const ScopeDisplay *display);
void scope_capture_push(ScopeCapture *capture, const float *samples, int num_frames);
void scope_capture_init(ScopeCapture *capture);
#endif
