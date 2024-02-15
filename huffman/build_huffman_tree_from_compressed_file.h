#ifndef BUILD_HUFFMAN_TREE_FROM_COMPRESSED_FILE_H
#define BUILD_HUFFMAN_TREE_FROM_COMPRESSED_FILE_H

#include "huffman_tree.h"

HuffmanNode *buildTreeFromCompressedFile(unsigned char *preOrderTraversedTreeContent, size_t treeContentSize);

#endif
