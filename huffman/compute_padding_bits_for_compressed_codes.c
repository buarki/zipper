#include <stdint.h>
#include <stdlib.h>

#include "compute_padding_bits_for_compressed_codes.h"

uint8_t computePadingBitsForCompressedCode(size_t lengthOfBytesWithItsFrequencies) {
  return 8 - (lengthOfBytesWithItsFrequencies % 8);
}
