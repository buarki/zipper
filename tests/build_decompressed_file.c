#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

#include "../huffman/huffman_tree.h"
#include "../huffman/huffman_tree.h"
#include "../huffman/consts.h"
#include "../huffman/build_decompressed_file.h"

HuffmanNode *createSimpleTree() {
  HuffmanNode *left = newHuffmanNode('a', 1);
  HuffmanNode *right = newHuffmanNode('b', 2);
  HuffmanNode *root = newHuffmanNode(JOINING_SYMBOL, left->frequency + right->frequency);
  root->left = left;
  root->right = right;
  return root;
}

void testBuildDecompressedFile() {
  HuffmanNode *root = createSimpleTree();
  unsigned char compressedSymbols[1] = {0b01100000};
  size_t compressedSymbolsSize = 1;
  uint8_t paddingBits = 5;
  size_t decompressedFileSize = 3;
  unsigned char expectedDecompressedFileContent[3] = {'a', 'b', 'b'};

  unsigned char *decompressedFile = buildDecompressedFile(
                                                        root,
                                                        compressedSymbols,
                                                        compressedSymbolsSize,
                                                        paddingBits,
                                                        decompressedFileSize);

  for (size_t i = 0; i < decompressedFileSize; i++) {
    assert(decompressedFile[i] == expectedDecompressedFileContent[i]);
  }

  free(decompressedFile);
  destroyHuffmanNode(root);
}

int main() {
  testBuildDecompressedFile();
  return 0;
}
