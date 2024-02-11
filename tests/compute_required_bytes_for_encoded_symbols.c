#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdlib.h>

#include "config.h"

#include "../huffman/compute_required_bytes_for_encoded_symbols.h"
#include "../huffman/build_symbol_codes_from_tree.h"
#include "../huffman/build_min_heap_from_bytes_frequency.h"
#include "../huffman/build_min_heap_from_bytes_frequency.h"
#include "../huffman/collect_bytes_frequency.h"
#include "../huffman/build_huffman_tree_from_min_heap.h"
#include "../huffman/huffman_tree.h"
#include "../huffman/consts.h"

void testcomputeRequiredBytesForEncodedSymbols(
  size_t lengthOfBytesWithItsFrequencies,
  size_t expectedBytes
) {
  size_t requiredBytesForEncodedSymbols = computeRequiredBytesForEncodedSymbols(lengthOfBytesWithItsFrequencies);
  assert(requiredBytesForEncodedSymbols == expectedBytes);
}

int main() {
  size_t lengthOfBytesWithItsFrequencies1 = 8;
  size_t expectedBytes1 = 1;
  testcomputeRequiredBytesForEncodedSymbols(lengthOfBytesWithItsFrequencies1, expectedBytes1);

  printf("---------\n");

  size_t lengthOfBytesWithItsFrequencies2 = 3;
  size_t expectedBytes2 = 1;
  testcomputeRequiredBytesForEncodedSymbols(lengthOfBytesWithItsFrequencies2, expectedBytes2);

  printf("---------\n");

  size_t lengthOfBytesWithItsFrequencies3 = 9;
  size_t expectedBytes3 = 2;
  testcomputeRequiredBytesForEncodedSymbols(lengthOfBytesWithItsFrequencies3, expectedBytes3);
  return 0;
}
