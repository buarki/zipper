#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "config.h"

#include "../huffman/huffman_tree.h"

HuffmanNode* createSampleHuffmanTree() {
  HuffmanNode* leafA = newHuffmanNode('a', 5);
  HuffmanNode* leafB = newHuffmanNode('b', 3);
  HuffmanNode* leafC = newHuffmanNode('c', 2);

  HuffmanNode* internalNode1 = newHuffmanNode('@', 0);
  HuffmanNode* internalNode2 = newHuffmanNode('@', 0);

  internalNode1->left = leafB;
  internalNode1->right = leafC;

  internalNode2->left = leafA;
  internalNode2->right = internalNode1;

  HuffmanNode* root = internalNode2;

  return root;
}

void testCreatingANode() {
  HuffmanNode *node = newHuffmanNode('A', 10);
  
  assert(node != NULL);
  assert(node->byte == 'A');
  assert(node->frequency == 10);

  destroyHuffmanNode(node);
}

void testtraverseAndCollectByteFrequencyPair() {
  HuffmanNode* root = createSampleHuffmanTree();
  ByteFrequencyPair expectedNodes[] = {
    { '@', 0 },
    { 'a', 5 },
    { '@', 0 },
    { 'b', 3 },
    { 'c', 2 },
  };
  size_t expectedAmountOfNodes = 5;
  
  size_t actualAmountOfNodes;
  ByteFrequencyPair* actualNodes = traverseAndCollectByteFrequencyPair(root, PRE_ORDER, &actualAmountOfNodes);

  assert(expectedAmountOfNodes == actualAmountOfNodes);
  for (size_t i = 0; i < expectedAmountOfNodes; ++i) {
    assert(expectedNodes[i].byte == actualNodes[i].byte);
    assert(expectedNodes[i].frequency == actualNodes[i].frequency);
  }

  destroyHuffmanNode(root);
  free(actualNodes);
}

int main() {
  testCreatingANode();
  testtraverseAndCollectByteFrequencyPair();

  return 0;
}
