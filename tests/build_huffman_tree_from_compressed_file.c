#include <stdlib.h>
#include <assert.h>

#include "../huffman/consts.h"
#include "../huffman/huffman_tree.h"
#include "../huffman/build_huffman_tree_from_compressed_file.h"

void buildTreeFromPreOrderContent() {
  unsigned char preOrderTraversedTreeContent[] = {JOINING_SYMBOL, 'a', 'b'};
  size_t treeContentSize = sizeof(preOrderTraversedTreeContent) / sizeof(preOrderTraversedTreeContent[0]);

  HuffmanNode *tree = buildTreeFromCompressedFile(preOrderTraversedTreeContent, treeContentSize);

  assert(tree->byte == JOINING_SYMBOL);
  assert(tree->left->byte == 'a');
  assert(tree->right->byte == 'b');

  destroyHuffmanNode(tree);
}

int main() {
  buildTreeFromPreOrderContent();
  return 0;
}