#ifndef COMPUTE_BYTES_REQUIRED_FOR_DECOMPRESSED_FILE_H
#define COMPUTE_BYTES_REQUIRED_FOR_DECOMPRESSED_FILE_H

#include <stdint.h>
#include <stdlib.h>

#include "huffman_tree.h"

size_t computeBytesRequiredForDecompressedFile(
  HuffmanNode *root, 
  unsigned char *compressedSymbols,
  size_t compressedSymbolsSize,
  uint8_t paddingBits
);

#endif

