#ifndef COMPRESS_H
#define COMPRESS_H

typedef struct {
  unsigned char *bytes;
  size_t size;
} CompressionResult;

CompressionResult *compress(unsigned char *fileContent, size_t fileContentSize);

void destroyCompressionResult(CompressionResult *compressionResult);

#endif
