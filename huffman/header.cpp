#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "header.h"
#include "consts.h"

unsigned char *getHeaderContent(uint16_t treeContentSize, uint16_t paddingBitsRequiredForEncodedSymbols, HeaderInfo *info);

Header *createHeader(uint16_t treeContentSize, uint16_t paddingBitsRequiredForEncodedSymbols) {
  Header *header = (Header*) malloc(sizeof(Header));
  if (header == NULL) {
    return NULL;
  }
  HeaderInfo *info = (HeaderInfo*) malloc(sizeof(HeaderInfo));
  if (info == NULL) {
    free(header);
    return NULL;
  }
  header->info = info;
  unsigned char *content = getHeaderContent(treeContentSize, paddingBitsRequiredForEncodedSymbols, info);
  if (content == NULL) {
    free(info);
    free(header);
    return NULL;
  }
  header->bytes = content;
  return header;
}

void destroyHeaderInfo(HeaderInfo *info) {
  free(info);
}

void destroyHeader(Header *header) {
  free(header->bytes);
  destroyHeaderInfo(header->info);
  free(header);
}

HeaderInfo *getHeaderInfo(unsigned char *compressedFileContent) {
  HeaderInfo *info = (HeaderInfo*) malloc(sizeof(HeaderInfo));
  if (info == NULL) {
    return NULL;
  }
  info->paddingBitsForEncodedSymbols = compressedFileContent[0] >> 5;
  uint16_t treeContentSizeMostSignificantBits = (compressedFileContent[0] & 0b00011111) << 8;
  uint16_t treeContentSizeLeastSignificantBits = compressedFileContent[1];
  info->treeContentSize = treeContentSizeMostSignificantBits | treeContentSizeLeastSignificantBits;
  return info;
}

unsigned char *getHeaderContent(uint16_t treeContentSize, uint16_t paddingBitsRequiredForEncodedSymbols, HeaderInfo *info) {
  unsigned char paddingBitsAs3MostSignificantBits = paddingBitsRequiredForEncodedSymbols << 5;
  uint16_t mostSignificantBitsOfTreeSize = treeContentSize >> 8;
  unsigned char leastSignificantBitsOfTreeSize = treeContentSize & 0b11111111;
  unsigned char *bytes = (unsigned char*) calloc(HEADER_REQUIRED_BYTES, sizeof(unsigned char));
  if (bytes == NULL) {
    return NULL;
  }
  bytes[0] = paddingBitsAs3MostSignificantBits | mostSignificantBitsOfTreeSize;
  bytes[1] = leastSignificantBitsOfTreeSize;
  info->treeContentSize = treeContentSize;
  info->paddingBitsForEncodedSymbols = paddingBitsRequiredForEncodedSymbols;
  return bytes;
}
