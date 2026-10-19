#include <stdint.h>

#ifndef FILE_IO_H
#define FILE_IO_H

int write_wav(const char *filename, 
              float *samples, 
              uint32_t num_frames, 
              uint32_t sample_rate);
  
#endif
