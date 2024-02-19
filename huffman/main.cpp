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
    fprintf(stderr, "failed to decompress at file %s:%d\n", __FILE__, __LINE__);
    return -1;
  }
  decompressedContent = (unsigned char*) calloc(decompressionResult->size, sizeof(unsigned char));
  if (decompressedContent == NULL) {
    fprintf(stderr, "failed to allocate space for decompressed file buffer at file %s:%d\n", __FILE__, __LINE__);
    destroyDecompressionResult(decompressionResult);
    return -1;
  }
  memcpy(decompressedContent, decompressionResult->bytes, decompressionResult->size);
  decompressedSize = decompressionResult->size;
  destroyDecompressionResult(decompressionResult);
  return decompressedSize;
}

void collectDecompressedContent(unsigned char *buffer, size_t bufferSize) {
  memcpy(buffer, decompressedContent, bufferSize);
  free(decompressedContent);
  decompressedContent = NULL;
}

uint32_t c_compress(unsigned char *fileContent, size_t fileContentSize) {
  if (content != NULL) {
    free(content);
    content = NULL;
  }
  CompressionResult *result = compress(fileContent, fileContentSize);
  if (result == NULL) {
    fprintf(stderr, "failed to compress at file %s:%d\n", __FILE__, __LINE__);
    return -1;
  }
  content = (unsigned char*) calloc(result->size, sizeof(unsigned char));
  if (content == NULL) {
    fprintf(stderr, "failed to allocate space for compressed file buffer at file %s:%d\n", __FILE__, __LINE__);
    destroyCompressionResult(result);
    return -1;
  }
  memcpy(content, result->bytes, result->size);
  size = result->size;
  destroyCompressionResult(result);
  return size;
}

void receiveContent(unsigned char *buffer, size_t bufferSize) {
  memcpy(buffer, content, bufferSize);
  free(content);
  content = NULL;
}
