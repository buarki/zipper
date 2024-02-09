#include <stdlib.h>

#include "build_min_heap_from_bytes_frequency.h"
#include "collect_bytes_frequency.h"
#include "min_heap.h"
#include "huffman_tree.h"
#include "consts.h"

MinHeap *buildMinHeapFromBytesFrequency(size_t *bytesFrequency) {
  size_t bytesUsed = 0;
  for (size_t i = 0; i < ASCII_SYMBOLS; i++) {
    if (bytesFrequency[i]) {
      bytesUsed++;
    }
  }
  MinHeap *queue = newMinHeap(bytesUsed);
  for (size_t i = 0; i < ASCII_SYMBOLS; i++) {
    if (bytesFrequency[i]) {
      HuffmanNode *node = newHuffmanNode(i, bytesFrequency[i]);
      addNode(queue, node);
    }
  }
  return queue;
}
