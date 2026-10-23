#ifndef ENVELOPE_H
#define ENVELOPE_H

typedef enum
{
    ENV_IDLE,
    ENV_ATTACK,
    ENV_SUSTAIN,
    ENV_RELEASE,
} EnvelopeState;

typedef struct
{
    float level;
    float attack_seconds;
    float sample_rate;
    EnvelopeState state;
    float release_seconds;
} Envelope;

float envelope_next(Envelope *env);
void envelope_note_on(Envelope *env);
void envelope_note_off(Envelope *env);

#endif
