#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "build_symbol_codes_from_tree.h"
#include "consts.h"

void buildSymbolCodesFromTreeUtil(
  unsigned char **table,
  HuffmanNode *tree,
  unsigned char *leafPathCreatedSoFar,
  size_t leafPathCreatedSoFarIndex
);

unsigned char **buildSymbolCodesFromTree(HuffmanNode *tree) {
  unsigned char **table = (unsigned char**) calloc(ASCII_SYMBOLS, sizeof(unsigned char*));
  for (size_t i = 0; i < ASCII_SYMBOLS; i++) {
    table[i] = (unsigned char*) calloc(ASCII_SYMBOLS, sizeof(unsigned char));
  }
  unsigned char *symbolCodeBuilt = (unsigned char*) calloc(ASCII_SYMBOLS, sizeof(unsigned char));
  size_t symbolCodeBuiltIndex = 0;
  buildSymbolCodesFromTreeUtil(table, tree, symbolCodeBuilt, symbolCodeBuiltIndex);
  free(symbolCodeBuilt);
  return table;
}

void destroyHuffmanTable(unsigned char **table) {
  for (int i = 0; i < ASCII_SYMBOLS; i++) {
    free(table[i]);
  }
  free(table);
}

void printHuffmanTable(unsigned char **table) {
  for (int i = 0; i < ASCII_SYMBOLS; i++) {
    if (table[i][0] != '\0') {
      printf("[%c]->[%s]\n", i, table[i]);
    }
  }
}

void buildSymbolCodesFromTreeUtil(
  unsigned char **table,
  HuffmanNode *tree,
  unsigned char *leafPathCreatedSoFar,
  size_t leafPathCreatedSoFarIndex
) {
  bool currentNodeIsALeaf = tree->left == NULL && tree->right == NULL;
  if (currentNodeIsALeaf) {
    leafPathCreatedSoFar[leafPathCreatedSoFarIndex] = '\0';
    unsigned char currentLeafSymbol = tree->byte;
    size_t oneExtraByteForStringNullTerminator = 1;
    memcpy(table[currentLeafSymbol], leafPathCreatedSoFar, leafPathCreatedSoFarIndex + oneExtraByteForStringNullTerminator);
  }
  if (tree->left != NULL) {
    leafPathCreatedSoFar[leafPathCreatedSoFarIndex] = '0';
    buildSymbolCodesFromTreeUtil(table, tree->left, leafPathCreatedSoFar, leafPathCreatedSoFarIndex + 1);
  }
  if (tree->right != NULL) {
    leafPathCreatedSoFar[leafPathCreatedSoFarIndex] = '1';
    buildSymbolCodesFromTreeUtil(table, tree->right, leafPathCreatedSoFar, leafPathCreatedSoFarIndex + 1);
  }
}
