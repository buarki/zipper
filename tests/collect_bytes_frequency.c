#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "config.h"
#include "../huffman/collect_bytes_frequency.h"
#include "../huffman/consts.h"


int main() {
  unsigned char content[] = "@\\2//@a-2=";
  size_t contentSize = 10;

  size_t *collectedFrequencies = collectBytesFrequency(content, contentSize);
  
  assert(collectedFrequencies != NULL);
  assert(collectedFrequencies['@'] == 2);
  assert(collectedFrequencies['\\'] == 1);
  assert(collectedFrequencies['/'] == 2);
  assert(collectedFrequencies['a'] == 1);
  assert(collectedFrequencies['-'] == 1);
  assert(collectedFrequencies['2'] == 2);
  assert(collectedFrequencies['='] == 1);

  free(collectedFrequencies);
  return 0;
}
