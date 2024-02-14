#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "compress.h"
#include "decompress.h"

unsigned char *content;
uint32_t size;

extern "C" {
  uint32_t c_compress(unsigned char *fileContent, size_t fileContentSize);
  unsigned char *c_getContent();

  void receiveContent(unsigned char *buffer, size_t bufferSize) {
    printf("buffer size: [%d]\n", bufferSize);
    printf("first: [%d]\n", content[0]);
    printf("second: [%d]\n", content[1]);
    memcpy(buffer, content, bufferSize);
    free(content);
  }

  uint32_t simple(uint32_t a, uint32_t b) {
    return a + b;
  }
}

uint32_t c_compress(unsigned char *fileContent, size_t fileContentSize) {
  printf("1\n");
  if (content != NULL) {
    free(content);
    content = NULL;
    printf("content destroyed\n");
  }
  printf("2\n");
  CompressionResult *result = compress(fileContent, fileContentSize);
  if (result == NULL) {
    printf("failed to compress file, size: [%d]\n", fileContentSize);
    return -1;
  }
  printf("3\n");
  printf("size: [%ld]\n", result->size);
  for (int i = 0; i < 3; i++) {
    printf("%d ", result->bytes[i]);
  }
  printf("\n");
  memcpy(content, result->bytes, result->size);
  size = result->size;
  destroyCompressionResult(result);
  for (int i = 0; i < 3; i++) {
    printf("%d ", content[i]);
  }
  printf("\n");
  return size;
}

unsigned char *c_getContent() {
  printf("first: [%d]\n", content[0]);
  return content;
}
