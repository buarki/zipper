#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "config.h"

#include "../huffman/build_min_heap_from_bytes_frequency.h"
#include "../huffman/collect_bytes_frequency.h"
#include "../huffman/build_huffman_tree_from_min_heap.h"
#include "../huffman/huffman_tree.h"
#include "../huffman/consts.h"

int main() {
  unsigned char fileContent[] = "aabbccc@";
  size_t fileSize = 8;

  size_t *bytesFrequency = collectBytesFrequency(fileContent, fileSize);
  assert(bytesFrequency != NULL);

  MinHeap *heap = buildMinHeapFromBytesFrequency(bytesFrequency);
  assert(heap != NULL);

  HuffmanNode *tree = buildHuffmanTreeFromMinHeap(heap);
  assert(tree != NULL);

  size_t expectedAmountOfNodes = 7;
  ByteFrequencyPair expectedNodesInPreOrder[] = {
    { '@', 8 },
    { 'c', 3 },
    { '@', 5 },
    { 'b', 2 },
    { '@', 3 },
    { '@', 1 },
    { 'a', 2 },
  };
  size_t amountOfNodes = 0;
  ByteFrequencyPair *pairs = traverseAndCollectByteFrequencyPair(tree, PRE_ORDER, &amountOfNodes);
  assert(pairs != NULL);
  assert(amountOfNodes == expectedAmountOfNodes);

  for (size_t i = 0; i < amountOfNodes; i++) {
    TEST_VERBOSE printf("[%c] [%d]\n", pairs[i].byte, pairs[i].frequency);
    assert(pairs[i].byte == expectedNodesInPreOrder[i].byte);
    assert(pairs[i].frequency == expectedNodesInPreOrder[i].frequency);
  }

  destroyBytesWithItsFrequencies(pairs);
  destroyHuffmanNode(tree);
  free(bytesFrequency);
  destroyMinHeap(heap);

  return 0;
}
