#include "envelope.h"

void envelope_note_on(Envelope *env) { env->state = ENV_ATTACK; }
void envelope_note_off(Envelope *env) { env->state = ENV_RELEASE; }

float envelope_next(Envelope *env)
{
    switch (env->state)
    {
    case ENV_IDLE:
        env->level = 0.0f;
        break;
    case ENV_ATTACK:
        if (env->attack_seconds <= 0.0f)
        {
            env->level = 1.0f;
            env->state = ENV_SUSTAIN;
        }
        else
        {
            env->level += 1.0f / (env->attack_seconds * env->sample_rate);

            if (env->level >= 1.0f)
            {
                env->level = 1.0f;
                env->state = ENV_SUSTAIN;
            }
        }
        break;
    case ENV_SUSTAIN:
        env->level = 1.0f;
        break;
    case ENV_RELEASE:
        if (env->release_seconds <= 0.0f)
        {
            env->level = 0.0f;
            env->state = ENV_IDLE;
        }
        else
        {
            env->level -= 1.0f / (env->release_seconds * env->sample_rate);

            if (env->level <= 0.0f)
            {
                env->level = 0.0f;
                env->state = ENV_IDLE;
            }
        }
        break;
    }
    return env->level;
}
