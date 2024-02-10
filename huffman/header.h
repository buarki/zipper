#ifndef HEADER_H
#define HEADER_H

#include <stdint.h>

#define HEADER_REQUIRED_BYTES 2

typedef struct {
  uint16_t treeContentSize;
  uint8_t paddingBitsForEncodedSymbols;
} HeaderInfo;

typedef struct {
  unsigned char *bytes;
  HeaderInfo *info;
} Header;

Header *createHeader(uint16_t treeContentSize, uint16_t encodedSymbolCodesSize);

HeaderInfo *getHeaderInfo(unsigned char *compressedFileContent);

void destroyHeader(Header *header);

void destroyHeaderInfo(HeaderInfo *info);

#endif
