#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "compute_bytes_required_for_decompressed_file.h"
#include "huffman_tree.h"

size_t computeBytesRequiredForDecompressedFile(
  HuffmanNode *root,
  unsigned char *compressedSymbols,
  size_t compressedSymbolsSize,
  uint8_t paddingBits
) {
  HuffmanNode *currentNode = root;
  size_t requiredBytes = 0;
  size_t compressedSymbolsIndex = 0;
  unsigned char currentByte = compressedSymbols[compressedSymbolsIndex++];
  while (compressedSymbolsIndex < compressedSymbolsSize) {
    for (int8_t byteIndex = 7; byteIndex >= 0; byteIndex--) {
      uint8_t reachedLeaf = currentNode->left == NULL && currentNode->right == NULL;
      if (reachedLeaf) {
        requiredBytes++;
        currentNode = root;
      }
      uint8_t currentByteBitIsSet = (currentByte >> byteIndex) & 1;
      currentNode = currentByteBitIsSet ? currentNode->right : currentNode->left;
    }
    currentByte = compressedSymbols[compressedSymbolsIndex++];
  }
  for (int8_t byteIndex = 7; byteIndex >= (paddingBits == 8 ? -1 : paddingBits - 1); --byteIndex)  {
    uint8_t reachedLeaf = currentNode->left == NULL && currentNode->right == NULL;
    if (reachedLeaf) {
      requiredBytes++;
      currentNode = root;
    }
    uint8_t currentByteBitIsSet = (currentByte >> byteIndex) & 1;
    currentNode = currentByteBitIsSet ? currentNode->right : currentNode->left;
  }
  return requiredBytes;
}
