#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <stdint.h>

typedef struct huffmanNode {
  unsigned char byte;
  size_t frequency;
  struct huffmanNode *left;
  struct huffmanNode *right;
} HuffmanNode;

HuffmanNode *newHuffmanNode(unsigned char byte, size_t frequency);

void destroyHuffmanNode(HuffmanNode *node);

/*
It computes the required bytes for tree content already
including escaping symbol.

We use uint16_t for this because:
- (1) we know that it is enough to store the required bytes
for tree content size. We can say it because the huffman
tree is a binary tree, thus the number of nodes can be
found by doing (2 * number_of_leaves) - 1. It means that
if all possible symbols of extended ASCII table gets used,
including the used JOINING_SYMBOL and ESCAPING_SYMBOLS, the
total number of nodes will be ((2 * number_of_leaves) - 1) + 2.
- (2) we'll need to be sure it really fits into 16 bits as
the allocated space of header on the compressed file is
2 bytes only.
*/
uint16_t computeRequiredBytesForTreeContent(HuffmanNode *root);

/**
It traverses the tree in pre order collecting the nodes
already taking care of the leaves that has the symbols
also used for representing joining nodes and the escaping
one.
*/
unsigned char *collectTreeSymbolsToExport(HuffmanNode *root);

// Bellow elements are useful for unit testing and debugging.
enum TraversalOrder {
  PRE_ORDER,
  IN_ORDER,
  POST_ORDER,
};

void traverseAndPrint(HuffmanNode *node, enum TraversalOrder order);

typedef struct {
  unsigned char byte;
  int frequency;
} ByteFrequencyPair;

ByteFrequencyPair *traverseAndCollectByteFrequencyPair(HuffmanNode *root, enum TraversalOrder order, size_t *amountOfNodes);

void destroyBytesWithItsFrequencies(ByteFrequencyPair *collectedNodes);

#endif
