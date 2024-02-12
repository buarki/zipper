#ifndef DECOMPRESS_H
#define DECOMPRESS_H

typedef struct {
  unsigned char *bytes;
  size_t size;
} DecompressionResult;

DecompressionResult *decompress(unsigned char *compressedContent, size_t compressedContentSize);

void destroyDecompressionResult(DecompressionResult *decompressionResult);

#endif
