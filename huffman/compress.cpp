#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "build_huffman_tree_from_min_heap.h"
#include "compress.h"
#include "collect_bytes_frequency.h"
#include "build_min_heap_from_bytes_frequency.h"
#include "build_symbol_codes_from_tree.h"
#include "huffman_tree.h"
#include "header.h"
#include "compute_required_bytes_for_encoded_symbols.h"
#include "collect_symbol_codes_to_export.h"
#include "compute_compressed_file_symbols_length.h"
#include "compute_padding_bits_for_compressed_codes.h"
  
CompressionResult *compress(unsigned char *fileContent, size_t fileContentSize) {
  CompressionResult *compressionResult = (CompressionResult*) malloc(sizeof(CompressionResult));
  if (compressionResult == NULL) {
    return NULL;
  }
  size_t *bytesFrequency = collectBytesFrequency(fileContent, fileContentSize);
  if (bytesFrequency == NULL) {
    destroyCompressionResult(compressionResult);
    return NULL;
  }
  MinHeap *minHeap = buildMinHeapFromBytesFrequency(bytesFrequency);
  if (minHeap == NULL) {
    free(bytesFrequency);
    destroyCompressionResult(compressionResult);
    return NULL;
  }
  HuffmanNode *tree = buildHuffmanTreeFromMinHeap(minHeap);
  if (tree == NULL) {
    destroyMinHeap(minHeap);
    free(bytesFrequency);
    destroyCompressionResult(compressionResult);
    return NULL;
  }
  unsigned char **tableCodes = buildSymbolCodesFromTree(tree);
  if (tableCodes == NULL) {
    destroyHuffmanNode(tree);
    destroyMinHeap(minHeap);
    free(bytesFrequency);
    destroyCompressionResult(compressionResult);
    return NULL;
  }
  uint16_t treeContentSize = computeRequiredBytesForTreeContent(tree);
  size_t compressedFileSymbolsLength = computeCompressedFileSymbolsLength(tableCodes, bytesFrequency);
  uint16_t encodedSymbolCodesSize = computeRequiredBytesForEncodedSymbols(compressedFileSymbolsLength);
  uint16_t paddingBitsRequiredForEncodedSymbols = computePadingBitsForCompressedCode(compressedFileSymbolsLength);
  Header *header = createHeader(treeContentSize, paddingBitsRequiredForEncodedSymbols);
  if (header == NULL) {
    destroyHuffmanTable(tableCodes);
    destroyHuffmanNode(tree);
    destroyMinHeap(minHeap);
    free(bytesFrequency);
    destroyCompressionResult(compressionResult);
    return NULL;
  }
  compressionResult->size = HEADER_REQUIRED_BYTES + treeContentSize + encodedSymbolCodesSize;
  compressionResult->bytes = (unsigned char*) calloc(compressionResult->size, sizeof(unsigned char));
  if (compressionResult->bytes == NULL) {
    destroyHeader(header);
    destroyHuffmanTable(tableCodes);
    destroyHuffmanNode(tree);
    destroyMinHeap(minHeap);
    free(bytesFrequency);
    destroyCompressionResult(compressionResult);
    return NULL;
  }
  unsigned char *treeContent = collectTreeSymbolsToExport(tree);
  if (treeContent == NULL) {
    destroyHeader(header);
    destroyHuffmanTable(tableCodes);
    destroyHuffmanNode(tree);
    destroyMinHeap(minHeap);
    free(bytesFrequency);
    destroyCompressionResult(compressionResult);
    return NULL;
  }
  unsigned char *encodedSymbolsContent = collectSymbolCodesToExport(fileContent, fileContentSize, tableCodes, encodedSymbolCodesSize);
  if (encodedSymbolsContent == NULL) {
    free(treeContent);
    destroyHeader(header);
    destroyHuffmanTable(tableCodes);
    destroyHuffmanNode(tree);
    destroyMinHeap(minHeap);
    free(bytesFrequency);
    destroyCompressionResult(compressionResult);
    return NULL;
  }
  memcpy(compressionResult->bytes, header->bytes, HEADER_REQUIRED_BYTES);
  memcpy(&compressionResult->bytes[HEADER_REQUIRED_BYTES], treeContent, treeContentSize);
  memcpy(&compressionResult->bytes[HEADER_REQUIRED_BYTES + treeContentSize], encodedSymbolsContent, encodedSymbolCodesSize);

  free(encodedSymbolsContent);
  free(treeContent);
  destroyHeader(header);
  destroyHuffmanTable(tableCodes);
  destroyHuffmanNode(tree);
  destroyMinHeap(minHeap);
  free(bytesFrequency);
  return compressionResult;
}

void destroyCompressionResult(CompressionResult *compressionResult) {
  free(compressionResult->bytes);
  free(compressionResult);
}
