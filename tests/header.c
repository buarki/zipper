#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include "config.h"

#include "../huffman/consts.h"
#include "../huffman/header.h"

void testCreateHeader() {
  uint16_t treeSize = 3;
  uint16_t paddingBitsRequiredForEncodedSymbols = 5;
  uint8_t expectedPaddingBits = 5;
  uint16_t expectedGeneratedFirstByte = 0b10100000;
  unsigned char expectedGeneratedSecondByte = 0b00000011;

  Header *header = createHeader(treeSize, paddingBitsRequiredForEncodedSymbols);
  assert(header != NULL);

  assert(header->info->paddingBitsForEncodedSymbols == expectedPaddingBits);
  assert(header->info->treeContentSize == treeSize);
  assert(header->bytes[0] == expectedGeneratedFirstByte);
  assert(header->bytes[1] == expectedGeneratedSecondByte);

  destroyHeader(header);
}

//0b10 00000001
void testCreateHeader1() {
  uint16_t treeSize = 513;
  uint16_t paddingBitsRequiredForEncodedSymbols = 4;
  uint8_t expectedPaddingBits = 4;
  uint16_t expectedGeneratedFirstByte = 0b10000010;
  unsigned char expectedGeneratedSecondByte = 0b00000001;

  Header *header = createHeader(treeSize, paddingBitsRequiredForEncodedSymbols);
  assert(header != NULL);

  printf("padding: [%d], tree size: [%d]\n", header->info->paddingBitsForEncodedSymbols, header->info->treeContentSize);

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

void testGetHeaderInfo1() {
  unsigned char compressedFileContent[2];
  compressedFileContent[0] = 0b10000010;
  compressedFileContent[1] = 0b00000001;
  uint8_t expectedPaddingBits = 4;
  uint16_t expectedTreeContentSize = 513;

  HeaderInfo *info = getHeaderInfo(compressedFileContent);
  assert(info != NULL);

  assert(info->paddingBitsForEncodedSymbols == expectedPaddingBits);
  assert(info->treeContentSize == expectedTreeContentSize);

  free(info);
}

int main() {
  testCreateHeader();
  testGetHeaderInfo();
  testCreateHeader1();
  testGetHeaderInfo1();
  return 0;
}
