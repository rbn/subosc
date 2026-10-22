#ifndef ENVELOPE_H
#define ENVELOPE_H

typedef struct
{
    float level;
    float attack_seconds;
    float sample_rate;
} Envelope;

float envelope_next(Envelope *env);

#endif
