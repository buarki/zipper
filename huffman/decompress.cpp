#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "decompress.h"
#include "huffman_tree.h"
#include "header.h"
#include "consts.h"
#include "compute_bytes_required_for_decompressed_file.h"

HuffmanNode *buildTreeFromCompressedFileUtil(unsigned char *preOrderTraversedTree, size_t *index, size_t size) {
  if (*index >= size) {
    return NULL;
  }
  unsigned char currentSymbol = preOrderTraversedTree[(*index)++];
  HuffmanNode *node = newHuffmanNode('-', 0);
  bool shouldSkipCurrentSymbolAsItIsAScapingChar = currentSymbol == ESCAPING_SYMBOL;
  if (shouldSkipCurrentSymbolAsItIsAScapingChar) {
    node->byte = preOrderTraversedTree[(*index)++];
  } else if (currentSymbol != JOINING_SYMBOL) {
    node->byte = currentSymbol;
  } else {
    node->byte = currentSymbol;
    node->left = buildTreeFromCompressedFileUtil(preOrderTraversedTree, index, size);
    node->right = buildTreeFromCompressedFileUtil(preOrderTraversedTree, index, size);
  }
  return node;
}

HuffmanNode *buildTreeFromCompressedFile(unsigned char *treeCompressedContent, size_t treeContentSize) {
  size_t index = 0;
  return buildTreeFromCompressedFileUtil(treeCompressedContent, &index, treeContentSize);
}

unsigned char *buildDecompressedFile(
  HuffmanNode *root, 
  unsigned char *compressedSymbols,
  size_t compressedSymbolsSize,
  uint8_t paddingBits,
  size_t decompressedFileSize
) {
  unsigned char *decompressedContent = (unsigned char*) calloc(decompressedFileSize, sizeof(unsigned char));
  if (decompressedContent == NULL) {
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

DecompressionResult *decompress(unsigned char *compressedContent, size_t compressedContentSize) {
  DecompressionResult *result = (DecompressionResult*) malloc(sizeof(DecompressionResult));
  if (result == NULL) {
    return NULL;
  }
  HeaderInfo *headerInfo = getHeaderInfo(compressedContent);
  if (headerInfo == NULL) {
    return NULL;
  }
  unsigned char *treeContent = &compressedContent[HEADER_REQUIRED_BYTES];
  unsigned char *compressedSymbols = &compressedContent[HEADER_REQUIRED_BYTES + headerInfo->treeContentSize];
  HuffmanNode *tree = buildTreeFromCompressedFile(treeContent, headerInfo->treeContentSize);
  if (tree == NULL) {
    destroyDecompressionResult(result);
    destroyHeaderInfo(headerInfo);
    return NULL;
  }
  size_t compressedSymbolsSize = compressedContentSize - HEADER_REQUIRED_BYTES - headerInfo->treeContentSize;
  result->size = computeBytesRequiredForDecompressedFile(tree, compressedSymbols, compressedSymbolsSize, headerInfo->paddingBitsForEncodedSymbols);
  result->bytes = buildDecompressedFile(tree, compressedSymbols, compressedSymbolsSize, headerInfo->paddingBitsForEncodedSymbols, result->size);
  if (result->bytes == NULL) {
    destroyDecompressionResult(result);
    destroyHuffmanNode(tree);
    destroyHeaderInfo(headerInfo);
    return NULL;
  }
  destroyHuffmanNode(tree);
  destroyHeaderInfo(headerInfo);
  return result;
}

void destroyDecompressionResult(DecompressionResult *decompressionResult) {
  free(decompressionResult->bytes);
  free(decompressionResult);
}
