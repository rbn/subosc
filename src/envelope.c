#include "envelope.h"

void envelope_note_on(Envelope *env) { env->state = ENV_ATTACK; }
void envelope_note_off(Envelope *env)
{
    if (env->state == ENV_RELEASE || env->state == ENV_IDLE)
    {
        return;
    }

    if (env->release_seconds <= 0.0f)
    {
        env->level = 0.0f;
        env->state = ENV_IDLE;
        return;
    }

    env->release_step = env->level / (env->release_seconds * env->sample_rate);

    env->state = ENV_RELEASE;
}

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
            env->state = ENV_DECAY;
        }
        else
        {
            env->level += 1.0f / (env->attack_seconds * env->sample_rate);

            if (env->level >= 1.0f)
            {
                env->level = 1.0f;
                env->state = ENV_DECAY;
            }
        }
        break;
    case ENV_DECAY:
        if (env->decay_seconds <= 0.0f)
        {
            env->level = env->sustain_level;
            env->state = ENV_SUSTAIN;
        }
        else
        {
            env->level -= (1.0f - env->sustain_level) / (env->decay_seconds * env->sample_rate);

            if (env->level <= env->sustain_level)
            {
                env->level = env->sustain_level;
                env->state = ENV_SUSTAIN;
            }
        }
        break;
    case ENV_SUSTAIN:
        env->level = env->sustain_level;
        break;
    case ENV_RELEASE:
        if (env->release_seconds <= 0.0f)
        {
            env->level = 0.0f;
            env->state = ENV_IDLE;
        }
        else
        {
            env->level -= env->release_step;

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
