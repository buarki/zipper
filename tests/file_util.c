#include <stdio.h>
#include <stdlib.h>

typedef struct {
  unsigned char *content;
  size_t size;
} SampleFile;

SampleFile loadSampleFile(const char *filePath) {
  FILE *sampleFile = fopen(filePath, "rb");
  if (sampleFile == NULL) {
    perror("failed to open sample file");
    exit(1);
  }
  fseek(sampleFile, 0, SEEK_END);
  size_t fileContentSize = ftell(sampleFile);
  rewind(sampleFile);
  unsigned char *fileContent = (unsigned char*) calloc(fileContentSize, sizeof(unsigned char));
  if (fileContent == NULL) {
    perror("failed to allocate memory to load file content");
    fclose(sampleFile);
    exit(1);
  }
  size_t bytesRead = 0;
  int currentByte;
  while ((currentByte = fgetc(sampleFile)) != EOF) {
    fileContent[bytesRead++] = (unsigned char) currentByte;
  }
  fclose(sampleFile);
  SampleFile loadedFile = { fileContent, fileContentSize };
  return loadedFile;
}
