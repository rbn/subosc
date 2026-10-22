#ifndef ENVELOPE_H
#define ENVELOPE_H

typedef enum
{
    ENV_IDLE,
    ENV_ATTACK,
    ENV_SUSTAIN,
    ENV_RELEASE,
} EnvelopeState; // TODO: left off here

typedef struct
{
    float level;
    float attack_seconds;
    float sample_rate;
} Envelope;

float envelope_next(Envelope *env);

#endif
