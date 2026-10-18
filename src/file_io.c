#include <stdio.h>
#include <stdint.h>

int write_silent_wav(const char *filename){
  FILE *file = fopen(filename, "wb");

  if (file == NULL) {
    fprintf(stderr, "Error opening file %s\n", filename);
    return 1;
  }

  size_t written = 0;

  written = fwrite("RIFF", 1, 4, file);

  uint32_t chunk_size = 96036;
  written += fwrite(&chunk_size, sizeof(chunk_size), 1, file);

  printf("Wrote %zu bytes to %s\n", written, filename);

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
