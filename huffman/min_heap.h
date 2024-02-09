#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include "huffman_tree.h"

typedef struct {
  HuffmanNode *nodes;
  size_t size;
  size_t capacity;
} MinHeap;

MinHeap *newMinHeap(size_t capacity);

void addNode(MinHeap *minHeap, HuffmanNode *node);

HuffmanNode *dequeue(MinHeap *minHeap);

size_t sizeOf(MinHeap *minHeap);

void destroyMinHeap(MinHeap *minHeap);

// printMinHeap is useful for unit testing and debugging.
void printMinHeap(MinHeap *minHeap);

#endif
