#include "envelope.h"

float envelope_next(Envelope *env)
{
    if (env->attack_seconds <= 0.0f)
    {
        env->level = 1.0f;
    }
    else
    {
        env->level += 1.0f / (env->attack_seconds * env->sample_rate);
        env->level = env->level > 1.0f ? 1.0f : env->level;
    }

    return env->level;
}
