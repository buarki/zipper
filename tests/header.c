#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include "config.h"

#include "../huffman/consts.h"
#include "../huffman/header.h"

void testCreateHeader() {
  uint16_t treeSize = 3;
  uint16_t encodedSymbolCodesSize = 3;
  uint8_t expectedPaddingBits = 5;
  uint16_t expectedGeneratedFirstByte = 0b10100000;
  unsigned char expectedGeneratedSecondByte = 0b00000011;

  Header *header = createHeader(treeSize, encodedSymbolCodesSize);
  assert(header != NULL);

  assert(header->info->paddingBitsForEncodedSymbols == expectedPaddingBits);
  assert(header->info->treeContentSize == treeSize);
  assert(header->bytes[0] == expectedGeneratedFirstByte);
  assert(header->bytes[1] == expectedGeneratedSecondByte);

  destroyHeader(header);
}

void testGetHeaderInfo() {
  unsigned char compressedFileContent[2];
  compressedFileContent[0] = 0b10100000;
  compressedFileContent[1] = 0b00000011;
  uint8_t expectedPaddingBits = 5;
  uint16_t expectedTreeContentSize = 3;

  HeaderInfo *info = getHeaderInfo(compressedFileContent);
  assert(info != NULL);

  assert(info->paddingBitsForEncodedSymbols == expectedPaddingBits);
  assert(info->treeContentSize == expectedTreeContentSize);

  free(info);
}

int main() {
  testCreateHeader();
  testGetHeaderInfo();
  return 0;
}
