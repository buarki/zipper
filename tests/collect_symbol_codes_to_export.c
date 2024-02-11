#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "config.h"

#include "../huffman/compute_required_bytes_for_encoded_symbols.h"
#include "../huffman/collect_symbol_codes_to_export.h"
#include "../huffman/build_symbol_codes_from_tree.h"
#include "../huffman/build_min_heap_from_bytes_frequency.h"
#include "../huffman/collect_bytes_frequency.h"
#include "../huffman/build_huffman_tree_from_min_heap.h"
#include "../huffman/huffman_tree.h"
#include "../huffman/consts.h"
#include "../huffman/compute_compressed_file_symbols_length.h"

void testCollectSymbolCodesToExport(
  unsigned char *fileContent,
  size_t fileSize,
  unsigned char *expectedSymbols
) {
  size_t *bytesFrequency = collectBytesFrequency(fileContent, fileSize);
  assert(bytesFrequency != NULL);

  MinHeap *heap = buildMinHeapFromBytesFrequency(bytesFrequency);
  assert(heap != NULL);

  HuffmanNode *tree = buildHuffmanTreeFromMinHeap(heap);
  assert(tree != NULL);
  TEST_VERBOSE traverseAndPrint(tree, PRE_ORDER);

  unsigned char **table = buildSymbolCodesFromTree(tree);
  assert(table != NULL);
  TEST_VERBOSE printHuffmanTable(table);

  size_t compressedFileSymbolsLength = computeCompressedFileSymbolsLength(table, bytesFrequency);

  size_t encodedSymbolCodesSize = computeRequiredBytesForEncodedSymbols(compressedFileSymbolsLength);
  TEST_VERBOSE printf("encodedSymbolCodesSize: [%ld]\n", encodedSymbolCodesSize);

  unsigned char *symbolCodesToExport = collectSymbolCodesToExport(fileContent, fileSize, table, encodedSymbolCodesSize);
  assert(symbolCodesToExport != NULL);

  for (size_t i = 0; i < encodedSymbolCodesSize; i++) {
    TEST_VERBOSE printf("[%d]===[%d]\n", symbolCodesToExport[i], expectedSymbols[i]);
    assert(symbolCodesToExport[i] == expectedSymbols[i]);
  }

  free(symbolCodesToExport);
  destroyHuffmanTable(table);
  destroyHuffmanNode(tree);
  free(bytesFrequency);
  destroyMinHeap(heap);
}

int main() {
  unsigned char fileContent1[] = "abb";
  size_t fileSize1 = 3;
  unsigned char expected1[1] = {0b01100000};
  testCollectSymbolCodesToExport(fileContent1, fileSize1, expected1);

  unsigned char fileContent2[] = "aabbccc@";
  size_t fileSize2 = 8;
  unsigned char expected2[2] = {0b11111110, 0b10000110};
  testCollectSymbolCodesToExport(fileContent2, fileSize2, expected2);

  unsigned char fileContent3[] = "dabbcdddeceaefdfc";
  size_t fileSize3 = 17;
  unsigned char expected3[6] = {
    0b10010011,
    0b01111110,
    0b10100011,
    0b10001000,
    0b11010110,
    0b11100000,
  };
  testCollectSymbolCodesToExport(fileContent3, fileSize3, expected3);

  return 0;
}
