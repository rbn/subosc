#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include "file_io.h"

#define BIT_PCM_16 32767

// mono only
int write_wav(const char *filename, const float *samples, uint32_t num_frames){
  FILE *file = fopen(filename, "wb");

  // fundamentals 
  uint32_t subchunk1_size = 16;
  uint16_t audio_format = 1;
  uint16_t num_channels = 1;
  uint16_t bits_per_sample = 16;
  uint32_t sample_rate =  48000;

  // derived values
  uint16_t block_align = num_channels * (bits_per_sample / 8);
  uint32_t byte_rate = sample_rate * block_align;
  uint32_t subchunk2_size = num_frames * block_align;
  uint32_t chunk_size = 36 + subchunk2_size;

  if (file == NULL) {
    fprintf(stderr, "Error opening file %s\n", filename);
    return 1;
  }

  // write 44 byte WAV header
  if (fwrite("RIFF", 1, 4, file) != 4){
    fprintf(stderr, "Failed to write ChunkId\n");
    fclose(file);
    return 2;
  }

  if ( fwrite(&chunk_size, sizeof(chunk_size), 1, file) != 1){
    fprintf(stderr, "Failed to write to chunk_size\n");
    fclose(file);
    return 2;
  }

  if ( fwrite("WAVE", 1, 4, file) != 4){
    fprintf(stderr, "Failed to write to WAVE\n");
    fclose(file);
    return 2;
  }

  if ( fwrite("fmt ", 1, 4, file) != 4){
    fprintf(stderr, "Failed to write to fmt \n");
    fclose(file);
    return 2;
  }

  if ( fwrite(&subchunk1_size, sizeof(subchunk1_size), 1, file) != 1){
    fprintf(stderr, "Failed to write to subchunk1_size\n");
    fclose(file);
    return 2;
  }
  
  if ( fwrite(&audio_format, sizeof(audio_format), 1, file) != 1){
    fprintf(stderr, "Failed to write to audio_format\n");
    fclose(file);
    return 2;
  }

  if ( fwrite(&num_channels, sizeof(num_channels), 1, file) != 1){
    fprintf(stderr, "Failed to write to num_channels\n");
    fclose(file);
    return 2;
  }

  if ( fwrite(&sample_rate, sizeof(sample_rate), 1, file) != 1){
    fprintf(stderr, "Failed to write to sample_rate\n");
    fclose(file);
    return 2;
  }

  if ( fwrite(&byte_rate, sizeof(byte_rate), 1, file) != 1){
    fprintf(stderr, "Failed to write to byte_rate\n");
    fclose(file);
    return 2;
  }

  if ( fwrite(&block_align, sizeof(block_align), 1, file) != 1){
    fprintf(stderr, "Failed to write to block_align\n");
    fclose(file);
    return 2;
  }
  
  if ( fwrite(&bits_per_sample, sizeof(bits_per_sample), 1, file) != 1){
    fprintf(stderr, "Failed to write to bits_per_sample\n");
    fclose(file);
    return 2;
  }

  if ( fwrite("data", 1, 4, file) != 4){
    fprintf(stderr, "Failed to write to data\n");
    fclose(file);
    return 2;
  }

  if ( fwrite(&subchunk2_size, sizeof(subchunk2_size), 1, file) != 1){
    fprintf(stderr, "Failed to write to subchunk2_size\n");
    fclose(file);
    return 2;
  }

  // -- write sounds
  for(uint32_t frame = 0; frame < num_frames; frame++){
    int16_t sound = samples[frame] * BIT_PCM_16;

    if ( fwrite(&sound, sizeof(sound), 1, file) != 1) {
      fclose(file);
      return 2;
    }
  }

  if (fclose(file) == EOF) {
    fprintf(stderr, "Error closing file\n");
    return 2;
  }

  return 0;
}

int write_test_file(const char *filename) {
  printf("Hello world: %s\n", filename);

  FILE *file = fopen(filename, "wb");

  if (file == NULL) {
    fprintf(stderr, "Error opening file %s\n", filename);
    return 1;
  }

  uint8_t data[] = {0x41, 0x42, 0x43, 0x44};
  size_t count = sizeof(data) / sizeof(data[0]);
  size_t written = fwrite(data, sizeof(uint8_t), count, file);

  if (written < count) {
    fprintf(stderr, "Error writing to %s\n", filename);
    fclose(file);
    return 2;
  }

  printf("Wrote %zu bytes to %s\n", written, filename);

  if (fclose(file) == EOF){
    fprintf(stderr, "Error closing file\n");
    return 3;
  }

  return 0;
}
