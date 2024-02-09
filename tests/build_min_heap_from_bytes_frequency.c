#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "config.h"

#include "../huffman/build_min_heap_from_bytes_frequency.h"
#include "../huffman/huffman_tree.h"
#include "../huffman/min_heap.h"
#include "../huffman/collect_bytes_frequency.h"

void testBuildMinHeapFromBytesFrequency() {
  unsigned char fileContent[] = "@@@@\\//---";
  size_t fileSize = 10;
  size_t expectedHeapSize = 4;

  size_t *bytesFrequency = collectBytesFrequency(fileContent, fileSize);
  assert(bytesFrequency != NULL);

  MinHeap *heap = buildMinHeapFromBytesFrequency(bytesFrequency);
  assert(heap != NULL);
  TEST_VERBOSE printMinHeap(heap);

  assert(sizeOf(heap) == expectedHeapSize);

  HuffmanNode *minNode = dequeue(heap);
  assert(minNode != NULL);
  assert(minNode->byte == '\\');
  assert(minNode->frequency == 1);
  assert(sizeOf(heap) == 3);

  minNode = dequeue(heap);
  assert(minNode != NULL);
  assert(minNode->byte == '/');
  assert(minNode->frequency == 2);
  assert(sizeOf(heap) == 2);

  minNode = dequeue(heap);
  assert(minNode != NULL);
  assert(minNode->byte == '-');
  assert(minNode->frequency == 3);
  assert(sizeOf(heap) == 1);

  minNode = dequeue(heap);
  assert(minNode != NULL);
  assert(minNode->byte == '@');
  assert(minNode->frequency == 4);
  assert(sizeOf(heap) == 0);

  free(bytesFrequency);
  destroyMinHeap(heap);
}

int main() {
  testBuildMinHeapFromBytesFrequency();
  
  return 0;
}