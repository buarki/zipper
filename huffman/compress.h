#ifndef COMPRESS_H
#define COMPRESS_H

typedef struct {
  size_t size;
  unsigned char *bytes;
} CompressionResult;

CompressionResult *compress(unsigned char *fileContent, size_t fileContentSize);

void destroyCompressionResult(CompressionResult *compressionResult);

#endif
