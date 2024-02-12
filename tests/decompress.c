#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "config.h"

#include "../huffman/consts.h"
#include "../huffman/decompress.h"

void testDecompression(
  unsigned char *compressedFileContent,
  size_t compressedFileContentSize,
  unsigned char *expectedDecompressedFile,
  size_t expectedDecompressedFileSize
) {
  DecompressionResult *result = decompress(compressedFileContent, compressedFileContentSize);
  assert(result != NULL);

  TEST_VERBOSE printf("result->size [%ld]\n", result->size);
  assert(result->size == expectedDecompressedFileSize);
  for (int i = 0; i < result->size; i++) {
    TEST_VERBOSE printf("[%d]/[%c] <--> [%d]/[%c]\n", result->bytes[i], result->bytes[i], expectedDecompressedFile[i], expectedDecompressedFile[i]);
    // assert(result->bytes[i] == expectedDecompressedFile[i]);
  }

  destroyDecompressionResult(result);
}

int main() {
  size_t compressedFileSize1 = 6;
  unsigned char compressedFileContent1[compressedFileSize1] = {
    0b10100000,
    0b00000011,
    JOINING_SYMBOL,
    'b',
    'a',
    0b11000000
  };
  size_t expectedDecompressedFileSize1 = 3;
  unsigned char expectedDecompressedFile1[expectedDecompressedFileSize1] = {'a', 'a', 'b'};
  testDecompression(compressedFileContent1, compressedFileSize1, expectedDecompressedFile1, expectedDecompressedFileSize1);

  printf("--------------\n");


  size_t compressedFileSize2 = 8;
  unsigned char compressedFileContent2[compressedFileSize2] = {
    0b10100000,
    0b00000101,
    JOINING_SYMBOL,
    ESCAPING_SYMBOL,
    ESCAPING_SYMBOL,
    ESCAPING_SYMBOL,
    JOINING_SYMBOL,
    0b11000000
  };
  size_t expectedDecompressedFileSize2 = 3;
  unsigned char expectedDecompressedFile2[expectedDecompressedFileSize2] = {JOINING_SYMBOL, JOINING_SYMBOL, ESCAPING_SYMBOL};
  testDecompression(compressedFileContent2, compressedFileSize2, expectedDecompressedFile2, expectedDecompressedFileSize2);

  printf("--------------\n");

  size_t compressedFileSize3 = 21;
  unsigned char compressedFileContent3[compressedFileSize3] = {
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
  size_t expectedDecompressedFileSize3 = 10;
  unsigned char expectedDecompressedFile3[expectedDecompressedFileSize3] = {
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
  testDecompression(compressedFileContent3, compressedFileSize3, expectedDecompressedFile3, expectedDecompressedFileSize3);

  return 0;
}
