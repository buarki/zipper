#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "compress.h"
#include "decompress.h"

unsigned char *content;
uint32_t size;

unsigned char *decompressedContent;
uint32_t decompressedSize;

extern "C" {
  uint32_t c_compress(unsigned char *fileContent, size_t fileContentSize);
  void receiveContent(unsigned char *buffer, size_t bufferSize);

  uint32_t c_decompress(unsigned char *compressedFileContent, size_t compressedFileContentSize);
  void collectDecompressedContent(unsigned char *buffer, size_t bufferSize);
}

uint32_t c_decompress(unsigned char *compressedFileContent, size_t compressedFileContentSize) {
  if (decompressedContent == NULL) {
    free(decompressedContent);
    decompressedContent = NULL;
  }
  DecompressionResult *decompressionResult = decompress(compressedFileContent, compressedFileContentSize);
  if (decompressionResult == NULL) {
    printf("failed to decompressed\n");
    return -1;
  }
  memcpy(decompressedContent, decompressionResult->bytes, decompressionResult->size);
  decompressedSize = decompressionResult->size;
  destroyDecompressionResult(decompressionResult);
  return decompressedSize;
}

void collectDecompressedContent(unsigned char *buffer, size_t bufferSize) {
  printf("buffer size: [%d]\n", bufferSize);
  printf("first: [%d]\n", decompressedContent[0]);
  printf("second: [%d]\n", decompressedContent[1]);
  memcpy(buffer, decompressedContent, bufferSize);
  free(decompressedContent);
  decompressedContent = NULL;
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

void receiveContent(unsigned char *buffer, size_t bufferSize) {
  printf("buffer size: [%d]\n", bufferSize);
  printf("first: [%d]\n", content[0]);
  printf("second: [%d]\n", content[1]);
  memcpy(buffer, content, bufferSize);
  free(content);
  content = NULL;
}
