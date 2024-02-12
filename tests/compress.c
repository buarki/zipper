#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "config.h"

#include "../huffman/consts.h"
#include "../huffman/compress.h"

void testCompression(
  unsigned char *fileContent,
  size_t fileSize,
  unsigned char *expectedSymbols,
  size_t expectedSize
) {
  CompressionResult *result = compress(fileContent, fileSize);
  assert(result != NULL);

  printf("result->size [%ld]\n", result->size);
  assert(result->size == expectedSize);
  for (int i = 0; i < result->size; i++) {
    TEST_VERBOSE printf("[%d] <--> [%d]\n", result->bytes[i], expectedSymbols[i]);
    assert(result->bytes[i] == expectedSymbols[i]);
  }

  destroyCompressionResult(result);
}

int main() {
  size_t fileSize1 = 3;
  unsigned char fileContent1[fileSize1] = {'a', 'a', 'b'};
  size_t expectedSize1 = 6;
  unsigned char compressedFile1[expectedSize1] = {
    0b10100000,
    0b00000011,
    JOINING_SYMBOL,
    'b',
    'a',
    0b11000000
  };
  testCompression(fileContent1, fileSize1, compressedFile1, expectedSize1);

  printf("--------------\n");


  size_t fileSize2 = 3;
  unsigned char fileContent2[fileSize2] = {JOINING_SYMBOL, JOINING_SYMBOL, ESCAPING_SYMBOL};
  size_t expectedSize2 = 8;
  unsigned char compressedFile2[expectedSize2] = {
    0b10100000,
    0b00000101,
    JOINING_SYMBOL,
    ESCAPING_SYMBOL,
    ESCAPING_SYMBOL,
    ESCAPING_SYMBOL,
    JOINING_SYMBOL,
    0b11000000
  };
  testCompression(fileContent2, fileSize2, compressedFile2, expectedSize2);

  size_t fileSize3 = 10;
  unsigned char fileContent3[fileSize3] = {
    JOINING_SYMBOL,
    JOINING_SYMBOL,
    ESCAPING_SYMBOL,
    JOINING_SYMBOL,
    'a',
    'a',
    'b',
    'c',
    '3',
    '2'
  };
  printf("content: [%s]\n", fileContent3);
  size_t expectedSize3 = 21;
  unsigned char compressedFile3[expectedSize3] = {
    // header content
    0b10100000,
    0b00001111,
    // tree content
    JOINING_SYMBOL,
    JOINING_SYMBOL,
    JOINING_SYMBOL,
    '2',
    'c',
    'a',
    JOINING_SYMBOL,
    JOINING_SYMBOL,
    'b',
    JOINING_SYMBOL,
    '3',
    ESCAPING_SYMBOL,
    ESCAPING_SYMBOL,
    ESCAPING_SYMBOL,
    JOINING_SYMBOL,
    // encoded content
    0b11111011,
    0b11010110,
    0b00011010,
    0b00000000
  };
  testCompression(fileContent3, fileSize3, compressedFile3, expectedSize3);

  return 0;
}
