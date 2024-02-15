#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdlib.h>

#include "config.h"

#include "../huffman/compute_padding_bits_for_compressed_codes.h"

void testComputePadingBitsForCompressedCode(
  size_t lengthOfBytesWithItsFrequencies,
  size_t expectedPaddingBits
) {
  size_t received = computePadingBitsForCompressedCode(lengthOfBytesWithItsFrequencies);

  assert(received == expectedPaddingBits);
}

int main() {
  size_t lengthOfBytesWithItsFrequencies1 = 8;
  size_t expectedPaddingBits1 = 8;
  testComputePadingBitsForCompressedCode(lengthOfBytesWithItsFrequencies1, expectedPaddingBits1);

  TEST_VERBOSE printf("---------\n");

  size_t lengthOfBytesWithItsFrequencies2 = 9;
  size_t expectedPaddingBits2 = 7;
  testComputePadingBitsForCompressedCode(lengthOfBytesWithItsFrequencies2, expectedPaddingBits2);

  size_t lengthOfBytesWithItsFrequencies3 = 17;
  size_t expectedPaddingBits3 = 7;
  testComputePadingBitsForCompressedCode(lengthOfBytesWithItsFrequencies3, expectedPaddingBits3);
  return 0;
}
