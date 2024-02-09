#include <stdio.h>
#include <assert.h>

#include "config.h"

#include "../huffman/min_heap.h"
#include "../huffman/huffman_tree.h"

void testCreatingAMinHeap() {
  size_t expectedCapacity = 10;

  MinHeap* minHeap = newMinHeap(expectedCapacity);

  assert(minHeap != NULL);
  assert(minHeap->capacity == expectedCapacity);

  destroyMinHeap(minHeap);
}

void testAddingNodesToMinHeap() {
  MinHeap* minHeap = newMinHeap(10);
  assert(minHeap != NULL);

  HuffmanNode *node1 = newHuffmanNode('A', 5);
  HuffmanNode *node2 = newHuffmanNode('B', 9);
  HuffmanNode *node3 = newHuffmanNode('C', 12);
  addNode(minHeap, node1);
  assert(sizeOf(minHeap) == 1);
  addNode(minHeap, node2);
  assert(sizeOf(minHeap) == 2);
  addNode(minHeap, node3);
  assert(sizeOf(minHeap) == 3);

  destroyMinHeap(minHeap);
}

void testPrint() {
  MinHeap* minHeap = newMinHeap(10);
  assert(minHeap != NULL);

  HuffmanNode *node1 = newHuffmanNode('A', 5);
  HuffmanNode *node2 = newHuffmanNode('B', 9);
  HuffmanNode *node3 = newHuffmanNode('C', 12);
  HuffmanNode *node4 = newHuffmanNode('D', -10);

  addNode(minHeap, node3);
  assert(sizeOf(minHeap) == 1);
  TEST_VERBOSE printMinHeap(minHeap);

  addNode(minHeap, node2);
  assert(sizeOf(minHeap) == 2);
  TEST_VERBOSE printMinHeap(minHeap);

  addNode(minHeap, node1);
  assert(sizeOf(minHeap) == 3);
  TEST_VERBOSE printMinHeap(minHeap);

  addNode(minHeap, node4);
  assert(sizeOf(minHeap) == 4);
  TEST_VERBOSE printMinHeap(minHeap);

  destroyMinHeap(minHeap);
}

void testEqueueWithAFrequencyAlreadyPresent() {
  MinHeap* minHeap = newMinHeap(5);
  assert(minHeap != NULL);

  HuffmanNode *node1 = newHuffmanNode('A', 5);
  addNode(minHeap, node1);
  assert(sizeOf(minHeap) == 1);
  TEST_VERBOSE printMinHeap(minHeap);

  HuffmanNode *node2 = newHuffmanNode('B', 3);
  addNode(minHeap, node2);
  assert(sizeOf(minHeap) == 2);
  TEST_VERBOSE printMinHeap(minHeap);

  HuffmanNode *node3 = newHuffmanNode('@', 5);
  addNode(minHeap, node3);
  assert(sizeOf(minHeap) == 3);
  TEST_VERBOSE printMinHeap(minHeap);

  HuffmanNode *node4 = newHuffmanNode('-', 3);
  addNode(minHeap, node4);
  assert(sizeOf(minHeap) == 4);
  TEST_VERBOSE printMinHeap(minHeap);

  char expectedChars[] = {'B', '-', '@', 'A'};

  int expectedCharsIndex = 0;
  while (sizeOf(minHeap) > 0) {
    HuffmanNode *current = dequeue(minHeap);
    TEST_VERBOSE printf("%c = %c\n", current->byte, expectedChars[expectedCharsIndex]);
    assert(current->byte == expectedChars[expectedCharsIndex]);
    expectedCharsIndex++;    
  }

  destroyHuffmanNode(node1);
  destroyHuffmanNode(node2);
  destroyHuffmanNode(node3);
  destroyHuffmanNode(node4);
}

int main() {
  testCreatingAMinHeap();
  testAddingNodesToMinHeap();
  testPrint();
  testEqueueWithAFrequencyAlreadyPresent();

  return 0;
}
