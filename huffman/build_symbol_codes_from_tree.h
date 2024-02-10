#ifndef BUILD_SYMBOL_CODES_FROM_TREE_H
#define BUILD_SYMBOL_CODES_FROM_TREE_H

#include "huffman_tree.h"

unsigned char **buildSymbolCodesFromTree(HuffmanNode *tree);

void destroyHuffmanTable(unsigned char **table);

void printHuffmanTable(unsigned char **table);

#endif 
