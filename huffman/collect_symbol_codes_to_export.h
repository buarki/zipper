#ifndef COLLECT_SYMBOL_CODES_TO_EXPORT_H
#define COLLECT_SYMBOL_CODES_TO_EXPORT_H

unsigned char *collectSymbolCodesToExport(
  unsigned char *fileContent,
  size_t fileContentSize,
  unsigned char **tableCodes,
  size_t encodedSymbolCodesSize
);

#endif
