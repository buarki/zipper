#include <stdlib.h>
#include <stdio.h>

#include "min_heap.h"
#include "huffman_tree.h"

void swapNodes(HuffmanNode*, HuffmanNode*);
void minHeapify(MinHeap*, size_t);

MinHeap *newMinHeap(size_t capacity) {
  MinHeap *minHeap = (MinHeap*) malloc(sizeof(MinHeap));
  if (minHeap == NULL) {
    fprintf(stderr, "newMinHeap(): could not allocate space for min heap\n");
    return NULL;
  }
  minHeap->nodes = (HuffmanNode*) calloc(capacity, sizeof(HuffmanNode));
  if (minHeap->nodes == NULL) {
    fprintf(stderr, "newMinHeap(): could not allocate space for min heap nodes elements\n");
    return NULL;
  }
  minHeap->size = 0;
  minHeap->capacity = capacity;
  return minHeap;
}

void addNode(MinHeap *minHeap, HuffmanNode *newNode) {
  if (minHeap->size == minHeap->capacity) {
    minHeap->capacity *= 2;
    minHeap->nodes = (HuffmanNode*) realloc(minHeap->nodes, minHeap->capacity * sizeof(HuffmanNode));
  }

  size_t newIndex = minHeap->size++;
  minHeap->nodes[newIndex] = *newNode;

  while (newIndex > 0 && minHeap->nodes[newIndex].frequency < minHeap->nodes[(newIndex - 1) / 2].frequency) {
    swapNodes(&minHeap->nodes[newIndex], &minHeap->nodes[(newIndex - 1) / 2]);
    newIndex = (newIndex - 1) / 2;
  }
}

HuffmanNode *dequeue(MinHeap *minHeap) {
  if (minHeap->size == 0) {
    return NULL;
  }
  HuffmanNode* minNode = (HuffmanNode*) malloc(sizeof(HuffmanNode));
  if (minNode == NULL) {
    fprintf(stderr, "dequeue(): could not allocate space for huffman node\n");
    return NULL;
  }
  *minNode = minHeap->nodes[0];
  minHeap->nodes[0] = minHeap->nodes[--minHeap->size];
  minHeapify(minHeap, 0);
  return minNode;
}

size_t sizeOf(MinHeap *minHeap) {
  return minHeap->size;
}

void destroyMinHeap(MinHeap *minHeap) {
  free(minHeap->nodes);
  free(minHeap);
}

void swapNodes(HuffmanNode *a, HuffmanNode *b) {
  HuffmanNode temp = *a;
  *a = *b;
  *b = temp;
}

void minHeapify(MinHeap *minHeap, size_t index) {
  size_t smallest = index;
  size_t leftChild = 2 * index + 1;
  size_t rightChild = 2 * index + 2;
  if (leftChild < minHeap->size && minHeap->nodes[leftChild].frequency < minHeap->nodes[smallest].frequency) {
    smallest = leftChild;
  }
  if (rightChild < minHeap->size && minHeap->nodes[rightChild].frequency < minHeap->nodes[smallest].frequency) {
    smallest = rightChild;
  }
  if (smallest != index) {
    swapNodes(&minHeap->nodes[index], &minHeap->nodes[smallest]);
    minHeapify(minHeap, smallest);
  }
}

void printMinHeap(MinHeap* minHeap) {
  printf("MinHeap contents:\n");
  for (size_t i = 0; i < minHeap->size; ++i) {
    printf("(%c, %ld) ", minHeap->nodes[i].byte, minHeap->nodes[i].frequency);
  }
  printf("\n");
}
