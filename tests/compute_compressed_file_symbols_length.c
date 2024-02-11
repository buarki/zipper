#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdlib.h>

#include "config.h"

#include "../huffman/compute_compressed_file_symbols_length.h"
#include "../huffman/build_symbol_codes_from_tree.h"
#include "../huffman/build_min_heap_from_bytes_frequency.h"
#include "../huffman/build_min_heap_from_bytes_frequency.h"
#include "../huffman/collect_bytes_frequency.h"
#include "../huffman/build_huffman_tree_from_min_heap.h"
#include "../huffman/huffman_tree.h"
#include "../huffman/consts.h"

void testComputeCompressedFileSymbolsLength(
  unsigned char *fileContent,
  size_t fileContentSize,
  size_t expectedSymbolsLength
) {
  size_t *bytesFrequency = collectBytesFrequency(fileContent, fileContentSize);
  assert(bytesFrequency != NULL);

  MinHeap *heap = buildMinHeapFromBytesFrequency(bytesFrequency);
  assert(heap != NULL);

  HuffmanNode *tree = buildHuffmanTreeFromMinHeap(heap);
  assert(tree != NULL);

  unsigned char **table = buildSymbolCodesFromTree(tree);
  printHuffmanTable(table);
  assert(table != NULL);

  size_t compressedFileSymbolsLength = computeCompressedFileSymbolsLength(table, bytesFrequency);
  assert(compressedFileSymbolsLength == expectedSymbolsLength);

  destroyHuffmanTable(table);
  destroyHuffmanNode(tree);
  free(bytesFrequency);
  destroyMinHeap(heap);
}

int main() {
  unsigned char fileContent1[] = "aabbccc@";
  size_t fileSize1 = 8;
  size_t expectedBytes1 = 16;
  testComputeCompressedFileSymbolsLength(fileContent1, fileSize1, expectedBytes1);

  printf("---------\n");

  unsigned char fileContent2[] = "aabbc";
  size_t fileSize2 = 5;
  size_t expectedBytes2 = 8;
  testComputeCompressedFileSymbolsLength(fileContent2, fileSize2, expectedBytes2);
  return 0;
}
