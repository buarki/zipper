#include <stdlib.h>

#include "build_huffman_tree_from_compressed_file.h"
#include "huffman_tree.h"
#include "consts.h"

HuffmanNode *buildTreeFromCompressedFileUtil(unsigned char *preOrderTraversedTreeContent, size_t *index, size_t size);

HuffmanNode *buildTreeFromCompressedFile(unsigned char *preOrderTraversedTreeContent, size_t treeContentSize) {
  size_t index = 0;
  return buildTreeFromCompressedFileUtil(preOrderTraversedTreeContent, &index, treeContentSize);
}

HuffmanNode *buildTreeFromCompressedFileUtil(unsigned char *preOrderTraversedTreeContent, size_t *index, size_t size) {
  if (*index >= size) {
    return NULL;
  }
  unsigned char currentSymbol = preOrderTraversedTreeContent[(*index)++];
  HuffmanNode *node = newHuffmanNode('-', 0);
  bool shouldSkipCurrentSymbolAsItIsAnEscapingChar = currentSymbol == ESCAPING_SYMBOL;
  if (shouldSkipCurrentSymbolAsItIsAnEscapingChar) {
    node->byte = preOrderTraversedTreeContent[(*index)++];
  } else if (currentSymbol != JOINING_SYMBOL) {
    node->byte = currentSymbol;
  } else {
    node->byte = currentSymbol;
    node->left = buildTreeFromCompressedFileUtil(preOrderTraversedTreeContent, index, size);
    node->right = buildTreeFromCompressedFileUtil(preOrderTraversedTreeContent, index, size);
  }
  return node;
}
