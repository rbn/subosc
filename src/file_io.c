#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>

int write_silent_wav(const char *filename){
  FILE *file = fopen(filename, "wb");

  if (file == NULL) {
    fprintf(stderr, "Error opening file %s\n", filename);
    return 1;
  }

  // 44 byte WAV header
  fwrite("RIFF", 1, 4, file);

  uint32_t chunk_size = 96036;
  fwrite(&chunk_size, sizeof(chunk_size), 1, file);

  fwrite("WAVE", 1, 4, file);
  fwrite("fmt ", 1, 4, file);

  uint32_t subchunk1_size = 16;
  fwrite(&subchunk1_size, sizeof(subchunk1_size), 1, file);
  
  uint16_t audio_format = 1;
  fwrite(&audio_format, sizeof(audio_format), 1, file);

  uint16_t num_channels = 1;
  fwrite(&num_channels, sizeof(num_channels), 1, file);

  uint32_t sample_rate = 48000;
  fwrite(&sample_rate, sizeof(sample_rate), 1, file);

  uint32_t byte_rate = 96000;
  fwrite(&byte_rate, sizeof(byte_rate), 1, file);

  uint16_t block_align = 1;
  fwrite(&block_align, sizeof(block_align), 1, file);
  
  uint16_t bits_per_sample = 16;
  fwrite(&bits_per_sample, sizeof(bits_per_sample), 1, file);

  fwrite("data", 1, 4, file);

  uint32_t subchunk2_size = 96000;
  fwrite(&subchunk2_size, sizeof(subchunk2_size), 1, file);

  // fill sounds with silence

  // - print file size
  // Ensure all buffered data is written to the file descriptor
  fflush(file); 

  // Move the file pointer to the end of the file
  fseek(file, 0L, SEEK_END);

  // Get the current position (which equals the file size in bytes)
  long file_size = ftell(file);

  // OPTIONAL: Move the pointer back to where it was if you plan to keep writing
  fseek(file, 0L, SEEK_SET); 

  printf("file is %zu bytes prior to write\n", file_size);

  fclose(file);

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
