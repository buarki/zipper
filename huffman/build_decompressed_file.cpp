#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "build_decompressed_file.h"
#include "huffman_tree.h"

unsigned char *buildDecompressedFile(
  HuffmanNode *root, 
  unsigned char *compressedSymbols,
  size_t compressedSymbolsSize,
  uint8_t paddingBits,
  size_t decompressedFileSize
) {
  unsigned char *decompressedContent = (unsigned char*) calloc(decompressedFileSize, sizeof(unsigned char));
  if (decompressedContent == NULL) {
    fprintf(stderr, "failed to allocate space for decompressed file content at file %s:%d\n", __FILE__, __LINE__);
    return NULL;
  }
  size_t decompressedContentIndex = 0;
  HuffmanNode *currentNode = root;
  size_t requiredBytes = 0;
  size_t compressedSymbolsIndex = 0;
  unsigned char currentByte = compressedSymbols[compressedSymbolsIndex++];
  while (compressedSymbolsIndex < compressedSymbolsSize) {
    for (int8_t byteIndex = 7; byteIndex >= 0; byteIndex--) {
      bool reachedLeaf = currentNode->left == NULL && currentNode->right == NULL;
      if (reachedLeaf) {
        decompressedContent[decompressedContentIndex++] = currentNode->byte;;
        currentNode = root;
      }
      bool currentByteBitIsSet = (currentByte >> byteIndex) & 1;
      currentNode = currentByteBitIsSet ? currentNode->right : currentNode->left;
    }
    currentByte = compressedSymbols[compressedSymbolsIndex++];
  }
  for (int8_t byteIndex = 7; byteIndex >= (paddingBits == 8 ? -1 : paddingBits - 1); --byteIndex)  {
    bool reachedLeaf = currentNode->left == NULL && currentNode->right == NULL;
    if (reachedLeaf) {
      decompressedContent[decompressedContentIndex++] = currentNode->byte;;
      currentNode = root;
    }
    bool currentByteBitIsSet = (currentByte >> byteIndex) & 1;
    currentNode = currentByteBitIsSet ? currentNode->right : currentNode->left;
  }
  return decompressedContent;
}
