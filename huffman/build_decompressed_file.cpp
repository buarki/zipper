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
  HuffmanNode *currentNode = root;
  size_t decompressedContentIndex = 0;
  size_t compressedSymbolsIndex = 0;
  unsigned char currentByte = compressedSymbols[compressedSymbolsIndex++];
  while (decompressedContentIndex < decompressedFileSize) {
    for (int8_t byteIndex = 7; byteIndex >= 0; --byteIndex) {
      if (currentNode->left == NULL && currentNode->right == NULL) {
        decompressedContent[decompressedContentIndex++] = currentNode->byte;
        if (decompressedContentIndex == decompressedFileSize) {
          break;
        }
        currentNode = root;
      }
      currentNode = (currentByte >> byteIndex) & 1 ? currentNode->right : currentNode->left;
    }
    currentByte = compressedSymbols[compressedSymbolsIndex++];
  }
  for (int8_t byteIndex = 7; paddingBits > 0; --paddingBits, --byteIndex) {
    if (currentNode->left == NULL && currentNode->right == NULL) {
      decompressedContent[decompressedContentIndex++] = currentNode->byte;
      currentNode = root;
    }
    currentNode = (currentByte >> byteIndex) & 1 ? currentNode->right : currentNode->left;
  }
  return decompressedContent;
}
