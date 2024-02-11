#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "collect_symbol_codes_to_export.h"

unsigned char *collectSymbolCodesToExport(
  unsigned char *fileContent,
  size_t fileContentSize,
  unsigned char **tableCodes,
  size_t encodedSymbolCodesSize
) {
  unsigned char *symbolsToExport = (unsigned char*) calloc(encodedSymbolCodesSize, sizeof(unsigned char));
  if (symbolsToExport == NULL) {
    return NULL;
  }
  size_t symbolsToExportIndex = 0;
  size_t collectedBits = 0;
  unsigned char formedByte = 0;
  uint8_t byte = 8;
  for (size_t i = 0; i < fileContentSize; i++) {
    size_t currentByteIndex = 0;
    while (tableCodes[fileContent[i]][currentByteIndex] != '\0') {
      if (collectedBits == byte) {
        memcpy(&symbolsToExport[symbolsToExportIndex], &formedByte, byte);
        symbolsToExportIndex++;
        collectedBits = formedByte = 0;
      }
      if (tableCodes[fileContent[i]][currentByteIndex] & 1) {
        formedByte |= (1 << (7 - collectedBits));
      }
      collectedBits++;
      currentByteIndex++;
    }
  }
  memcpy(&symbolsToExport[symbolsToExportIndex], &formedByte, byte);
  return symbolsToExport;
}
