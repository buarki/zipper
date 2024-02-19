#include <string.h>
#include <stdint.h>

#include "compute_required_bytes_for_encoded_symbols.h"
#include "consts.h"

size_t computeRequiredBytesForEncodedSymbols(size_t lengthOfBytesWithItsFrequencies) {
  uint8_t byte = 8;
  size_t requiredBytes = lengthOfBytesWithItsFrequencies / byte;
  uint8_t fullContentRequiresOneExtraByte = lengthOfBytesWithItsFrequencies % byte != 0;
  return fullContentRequiresOneExtraByte ? requiredBytes + 1 : requiredBytes;
}
