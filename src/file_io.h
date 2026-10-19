#include <stdint.h>

#ifndef FILE_IO_H
#define FILE_IO_H

int write_wav(const char *filename, const float *samples, uint32_t num_frames);

#endif
