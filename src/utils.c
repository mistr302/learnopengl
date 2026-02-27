#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>

char *read_file(const char *path, size_t *out_size) {
  FILE *file = fopen(path, "rb");
  if (!file)
    return NULL;

  // Go to end of file
  if (fseek(file, 0, SEEK_END) != 0) {
    fclose(file);
    return NULL;
  }

  long size = ftell(file);
  if (size < 0) {
    fclose(file);
    return NULL;
  }

  rewind(file);

  // Allocate buffer (+1 for null terminator)
  char *buffer = malloc((size_t)size + 1);
  if (!buffer) {
    fclose(file);
    return NULL;
  }

  size_t read_size = fread(buffer, 1, (size_t)size, file);
  fclose(file);

  if (read_size != (size_t)size) {
    free(buffer);
    return NULL;
  }

  buffer[size] = '\0'; // Null terminate
  if (out_size)
    *out_size = (size_t)size;

  return buffer;
}
