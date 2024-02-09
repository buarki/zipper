#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

typedef struct huffmanNode {
  unsigned char byte;
  size_t frequency;
  struct huffmanNode *left;
  struct huffmanNode *right;
} HuffmanNode;

HuffmanNode *newHuffmanNode(unsigned char byte, size_t frequency);

void destroyHuffmanNode(HuffmanNode *node);

// Bellow elements are useful for unit testing and debugging.
enum TraversalOrder {
  PRE_ORDER,
  IN_ORDER,
  POST_ORDER,
};

void traverseAndPrint(HuffmanNode *node, TraversalOrder order);

typedef struct {
  unsigned char byte;
  int frequency;
} ByteFrequencyPair;

ByteFrequencyPair *traverseAndCollectByteFrequencyPair(HuffmanNode *root, TraversalOrder order, size_t *amountOfNodes);

void destroyBytesWithItsFrequencies(ByteFrequencyPair *collectedNodes);

#endif
