#include <stdlib.h>
#include <stdio.h>

#include "huffman_tree.h"

void traverseAndPrintHelper(HuffmanNode *root, TraversalOrder order);
void collectNodes(HuffmanNode *root, ByteFrequencyPair *pairArray, size_t *index, TraversalOrder order);
size_t countNodes(HuffmanNode *root);

HuffmanNode *newHuffmanNode(unsigned char byte, size_t frequency) {
  HuffmanNode *newNode = (HuffmanNode*) malloc(sizeof(HuffmanNode));
  if (newNode == NULL) {
    fprintf(stderr, "newHuffmanNode(): could not allocate space for new huffman tree node\n");
    return NULL;
  }
  newNode->byte = byte;
  newNode->frequency = frequency;
  newNode->left = newNode->right = NULL;
  return newNode;
}

void destroyHuffmanNode(HuffmanNode *node) {
  if (node == NULL) {
    return;
  }
  destroyHuffmanNode(node->left);
  destroyHuffmanNode(node->right);
  free(node);
}

void traverseAndPrint(HuffmanNode *root, TraversalOrder order) {
  printf("Huffman Tree contents (Traversal Order: ");
  switch (order) {
    case PRE_ORDER:
      printf("Pre-order");
      break;
    case IN_ORDER:
      printf("In-order");
      break;
    case POST_ORDER:
      printf("Post-order");
      break;
  }
  printf("):\n");

  traverseAndPrintHelper(root, order);

  printf("\n");
}

ByteFrequencyPair *traverseAndCollectByteFrequencyPair(HuffmanNode *root, TraversalOrder order, size_t *amountOfNodes) {
  size_t totalNodes = countNodes(root);
  ByteFrequencyPair *pairArray = (ByteFrequencyPair*) malloc(totalNodes * sizeof(ByteFrequencyPair));
  if (pairArray == NULL) {
    fprintf(stderr, "traverseAndCollectByteFrequencyPair(): could not allocate space for new list of byte-frequency pair\n");
    return NULL;
  }

  size_t index = 0;
  collectNodes(root, pairArray, &index, order);

  *amountOfNodes = totalNodes;
  return pairArray;
}

void destroyBytesWithItsFrequencies(ByteFrequencyPair *collectedNodes) {
  free(collectedNodes);
}

// Implementation of helper functions
void traverseAndPrintHelper(HuffmanNode *root, TraversalOrder order) {
  if (root == NULL) {
    return;
  }
  switch (order) {
    case PRE_ORDER:
      printf("(%c, %ld) ", root->byte, root->frequency);
      traverseAndPrintHelper(root->left, order);
      traverseAndPrintHelper(root->right, order);
      break;
    case IN_ORDER:
      traverseAndPrintHelper(root->left, order);
      printf("(%c, %ld) ", root->byte, root->frequency);
      traverseAndPrintHelper(root->right, order);
      break;
    case POST_ORDER:
      traverseAndPrintHelper(root->left, order);
      traverseAndPrintHelper(root->right, order);
      printf("(%c, %ld) ", root->byte, root->frequency);
      break;
  }
}

void collectNodes(
  HuffmanNode *root,
  ByteFrequencyPair *pairArray,
  size_t *index,
  TraversalOrder order
) {
  if (root == NULL) {
    return;
  }
  if (order == PRE_ORDER) {
    pairArray[*index].byte = root->byte;
    pairArray[*index].frequency = root->frequency;
    (*index)++;
  }
  collectNodes(root->left, pairArray, index, order);
  if (order == IN_ORDER) {
    pairArray[*index].byte = root->byte;
    pairArray[*index].frequency = root->frequency;
    (*index)++;
  }
  collectNodes(root->right, pairArray, index, order);
  if (order == POST_ORDER) {
    pairArray[*index].byte = root->byte;
    pairArray[*index].frequency = root->frequency;
    (*index)++;
  }
}

size_t countNodes(HuffmanNode *root) {
  if (root == NULL) {
    return 0;
  }
  return 1 + countNodes(root->left) + countNodes(root->right);
}
