#ifndef BUILD_DECOMPRESSED_FILE_H
#define BUILD_DECOMPRESSED_FILE_H

#include <stdint.h>

#include "huffman_tree.h"

unsigned char *buildDecompressedFile(
  HuffmanNode *root, 
  unsigned char *compressedSymbols,
  size_t compressedSymbolsSize,
  uint8_t paddingBits,
  size_t decompressedFileSize
);

#endif
