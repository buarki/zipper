#include <stdlib.h>

#include "huffman_tree.h"
#include "min_heap.h"
#include "consts.h"

HuffmanNode *buildHuffmanTreeFromMinHeap(MinHeap *minHeap) {
  while (sizeOf(minHeap) > 1) {
    HuffmanNode *left = dequeue(minHeap);
    HuffmanNode *right = dequeue(minHeap);
    HuffmanNode *tree = newHuffmanNode(JOINNING_SYMBOL, left->frequency + right->frequency);
    tree->left = left;
    tree->right = right;
    addNode(minHeap, tree);
  }
  return dequeue(minHeap);
}
