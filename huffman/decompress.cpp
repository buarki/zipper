#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "decompress.h"
#include "huffman_tree.h"
#include "header.h"
#include "consts.h"
#include "compute_bytes_required_for_decompressed_file.h"
#include "build_huffman_tree_from_compressed_file.h"
#include "build_decompressed_file.h"

DecompressionResult *decompress(unsigned char *compressedContent, size_t compressedContentSize) {
  DecompressionResult *result = (DecompressionResult*) malloc(sizeof(DecompressionResult));
  if (result == NULL) {
    fprintf(stderr, "failed to allocate space decompression result at file %s:%d\n", __FILE__, __LINE__);
    return NULL;
  }
  HeaderInfo *headerInfo = getHeaderInfo(compressedContent);
  if (headerInfo == NULL) {
    destroyDecompressionResult(result);
    return NULL;
  }
  unsigned char *treeContent = &compressedContent[HEADER_REQUIRED_BYTES];
  unsigned char *compressedSymbols = &compressedContent[HEADER_REQUIRED_BYTES + headerInfo->treeContentSize];
  HuffmanNode *tree = buildTreeFromCompressedFile(treeContent, headerInfo->treeContentSize);
  if (tree == NULL) {
    destroyDecompressionResult(result);
    destroyHeaderInfo(headerInfo);
    treeContent = NULL;
    compressedSymbols = NULL;
    return NULL;
  }
  size_t compressedSymbolsSize = compressedContentSize - HEADER_REQUIRED_BYTES - headerInfo->treeContentSize;
  result->size = computeBytesRequiredForDecompressedFile(tree, compressedSymbols, compressedSymbolsSize, headerInfo->paddingBitsForEncodedSymbols);
  result->bytes = buildDecompressedFile(tree, compressedSymbols, compressedSymbolsSize, headerInfo->paddingBitsForEncodedSymbols, result->size);
  if (result->bytes == NULL) {
    destroyDecompressionResult(result);
    destroyHeaderInfo(headerInfo);
    destroyHuffmanNode(tree);
    treeContent = NULL;
    compressedSymbols = NULL;
    return NULL;
  }
  destroyHuffmanNode(tree);
  destroyHeaderInfo(headerInfo);
  treeContent = NULL;
  compressedSymbols = NULL;
  return result;
}

void destroyDecompressionResult(DecompressionResult *decompressionResult) {
  free(decompressionResult->bytes);
  free(decompressionResult);
}
