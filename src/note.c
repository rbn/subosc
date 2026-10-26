#include <math.h>
#include "note.h"

float note_to_frequency(int midi_note) { return 440 * powf(2.0f, (midi_note - 69) / 12.0f); }
