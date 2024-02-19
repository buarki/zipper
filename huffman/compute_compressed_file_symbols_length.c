#include <string.h>
#include <stdint.h>

#include "compute_compressed_file_symbols_length.h"
#include "consts.h"

size_t computeCompressedFileSymbolsLength(
  unsigned char **codesTable,
  size_t *bytesFrequency
) {
  size_t lengthOfBytesWithItsFrequencies = 0;
  uint8_t byte = 8;
  for (size_t i = 0; i < ASCII_SYMBOLS; i++) {
    if (codesTable[i][0] != '\0') {
      size_t currentSymbolLength = strlen((const char*) codesTable[i]);
      size_t currentSymbolFrequency = bytesFrequency[i];
      lengthOfBytesWithItsFrequencies += currentSymbolLength * currentSymbolFrequency;
    }
  }
  return lengthOfBytesWithItsFrequencies;
}
