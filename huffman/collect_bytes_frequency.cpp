#include <stdlib.h>
#include <stdio.h>

#include "collect_bytes_frequency.h"
#include "consts.h"

size_t *collectBytesFrequency(unsigned char *fileContent, size_t fileSize) {
  size_t *bytesFrequency = (size_t*) calloc(ASCII_SYMBOLS, sizeof(size_t));
  if (bytesFrequency == NULL) {
    fprintf(stderr, "collectBytesFrequency(): failed to allocate memory for bytes frequency\n");
    return NULL;
  }
  for (size_t i = 0; i < fileSize; i++) {
    bytesFrequency[fileContent[i]]++;
  }
  return bytesFrequency;
}
