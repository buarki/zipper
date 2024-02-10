#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "config.h"

#include "../huffman/build_symbol_codes_from_tree.h"
#include "../huffman/build_min_heap_from_bytes_frequency.h"
#include "../huffman/collect_bytes_frequency.h"
#include "../huffman/build_huffman_tree_from_min_heap.h"
#include "../huffman/huffman_tree.h"
#include "../huffman/consts.h"

int main() {
  unsigned char fileContent[] = "aabbccc@\\";
  size_t fileSize = 9;

  size_t *bytesFrequency = collectBytesFrequency(fileContent, fileSize);
  assert(bytesFrequency != NULL);

  MinHeap *heap = buildMinHeapFromBytesFrequency(bytesFrequency);
  assert(heap != NULL);

  HuffmanNode *tree = buildHuffmanTreeFromMinHeap(heap);
  assert(tree != NULL);

  unsigned char **table = buildSymbolCodesFromTree(tree);
  assert(table != NULL);

  TEST_VERBOSE printHuffmanTable(table);


  assert(strcmp((const char*) table['a'], "10") == 0);
  assert(strcmp((const char*) table['b'], "00") == 0);
  assert(strcmp((const char*) table['c'], "11") == 0);
  assert(strcmp((const char*) table['@'], "010") == 0);
  assert(strcmp((const char*) table['\\'], "011") == 0);

  destroyHuffmanTable(table);
  destroyHuffmanNode(tree);
  free(bytesFrequency);
  destroyMinHeap(heap);

  return 0;
}
