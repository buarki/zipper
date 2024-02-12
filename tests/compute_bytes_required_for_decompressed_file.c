#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include "config.h"

#include "../huffman/compute_padding_bits_for_compressed_codes.h"
#include "../huffman/collect_symbol_codes_to_export.h"
#include "../huffman/compute_required_bytes_for_encoded_symbols.h"
#include "../huffman/compute_compressed_file_symbols_length.h"
#include "../huffman/build_symbol_codes_from_tree.h"
#include "../huffman/build_huffman_tree_from_min_heap.h"
#include "../huffman/huffman_tree.h"
#include "../huffman/build_min_heap_from_bytes_frequency.h"
#include "../huffman/collect_bytes_frequency.h"
#include "../huffman/compute_bytes_required_for_decompressed_file.h"

void testComputeBytesRequiredForDecompressedFile(
  unsigned char *fileContent,
  size_t fileContentSize,
  size_t expectedBytes
) {
  size_t *bytesFrequency = collectBytesFrequency(fileContent, fileContentSize);
  assert(bytesFrequency != NULL);

  MinHeap *minHeap = buildMinHeapFromBytesFrequency(bytesFrequency);
  assert(bytesFrequency != NULL);

  HuffmanNode *tree = buildHuffmanTreeFromMinHeap(minHeap);
  assert(tree != NULL);
  traverseAndPrint(tree, PRE_ORDER);

  unsigned char **tableCodes = buildSymbolCodesFromTree(tree);
  assert(tableCodes != NULL);
  printHuffmanTable(tableCodes);

  uint16_t treeContentSize = computeRequiredBytesForTreeContent(tree);
  size_t compressedFileSymbolsLength = computeCompressedFileSymbolsLength(tableCodes, bytesFrequency);
  uint16_t encodedSymbolCodesSize = computeRequiredBytesForEncodedSymbols(compressedFileSymbolsLength);

  uint16_t paddingBitsRequiredForEncodedSymbols = computePadingBitsForCompressedCode(compressedFileSymbolsLength);

  unsigned char *encodedSymbolsContent = collectSymbolCodesToExport(fileContent, fileContentSize, tableCodes, encodedSymbolCodesSize);
  assert(encodedSymbolsContent != NULL);

  size_t requiredBytes = computeBytesRequiredForDecompressedFile(tree, encodedSymbolsContent, encodedSymbolCodesSize, paddingBitsRequiredForEncodedSymbols);

  assert(requiredBytes == expectedBytes);

  free(encodedSymbolsContent);
  destroyHuffmanTable(tableCodes);
  destroyHuffmanNode(tree);
  destroyMinHeap(minHeap);
  free(bytesFrequency);
}

int main() {
  size_t fileContentSize1 = 3;
  unsigned char fileContent1[fileContentSize1] = "abb";
  size_t expectedSize1 = fileContentSize1;
  testComputeBytesRequiredForDecompressedFile(fileContent1, fileContentSize1, expectedSize1);  
  
  size_t fileContentSize2 = 8;
  unsigned char fileContent2[fileContentSize2] = "aabbccc@";
  size_t expectedSize2 = fileContentSize2;
  testComputeBytesRequiredForDecompressedFile(fileContent2, fileContentSize2, expectedSize2);

  size_t fileContentSize3 = 31;
  unsigned char fileContent3[fileContentSize3] = "\\a======@@ssssssssssssss;;;;;;;";
  size_t expectedSize3 = fileContentSize3;
  testComputeBytesRequiredForDecompressedFile(fileContent3, fileContentSize3, expectedSize3);

  return 0;
}
