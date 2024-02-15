#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../huffman/compress.h"
#include "../huffman/decompress.h"

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

bool filesAreEqual(const char *file1Path, const char *file2Path) {
  FILE *file1 = fopen(file1Path, "rb");
  if (file1 == NULL) {
    perror("error opening file1");
    return false;
  }
  FILE *file2 = fopen(file2Path, "rb");
  if (file2 == NULL) {
    perror("error opening file2");
    return false;
  }
  int byte1, byte2;
  int position = 0;
  while (true) {
    byte1 = fgetc(file1);
    byte2 = fgetc(file2);
    if (byte1 != byte2) {
      printf("files differ at position %d\n", position);
      fclose(file1);
      fclose(file2);
      return position;
    }
    if (byte1 == EOF || byte2 == EOF) {
      break;
    }
    position++;
  }
  fclose(file1);
  fclose(file2);
  return true;
}


void testCompressionAndDecompression(
  const char *pathOfFileToCompress,
  const char *pathToPlaceCompressedContent,
  const char *pathToPlaceDecompressedContent
) {
  SampleFile fileToCompress = loadSampleFile(pathOfFileToCompress);
  CompressionResult *compressionResult = compress(fileToCompress.content, fileToCompress.size);
  if (compressionResult == NULL) {
    perror("failed to compress file");
    free(fileToCompress.content);
    exit(1);
  }

  FILE *compressedOutputFile = fopen(pathToPlaceCompressedContent, "wb");
  if (compressedOutputFile == NULL) {
    perror("error opening file to place compressed content");
    destroyCompressionResult(compressionResult);
    free(fileToCompress.content);
    exit(1);
  }
  fwrite(compressionResult->bytes, sizeof(unsigned char), compressionResult->size, compressedOutputFile);
  fclose(compressedOutputFile);
  destroyCompressionResult(compressionResult);
  free(fileToCompress.content);

  SampleFile fileToDecompress = loadSampleFile(pathToPlaceCompressedContent);

  DecompressionResult *decompressionResult = decompress(fileToDecompress.content, fileToDecompress.size);
  if (decompressionResult == NULL) {
    perror("error to decompress compressed file");
    free(fileToDecompress.content);
    exit(1);
  }

  FILE *decompressedOutputFile = fopen(pathToPlaceDecompressedContent, "wb");
  if (decompressedOutputFile == NULL) {
    perror("error opening file with compressed content");
    destroyDecompressionResult(decompressionResult);
    free(fileToDecompress.content);
    exit(1);
  }
  fwrite(decompressionResult->bytes, sizeof(unsigned char), decompressionResult->size, decompressedOutputFile);
  fclose(decompressedOutputFile);

  destroyDecompressionResult(decompressionResult);
  free(fileToDecompress.content);

  assert(filesAreEqual(pathOfFileToCompress, pathToPlaceDecompressedContent));

  assert(remove(pathToPlaceCompressedContent) == 0);
  assert(remove(pathToPlaceDecompressedContent) == 0);
}

void testDecompression(const char *originalFile, const char *compressedFile, const char *tempFile) {
  SampleFile fileToDecompress = loadSampleFile(compressedFile);
  DecompressionResult *decompressionResult = decompress(fileToDecompress.content, fileToDecompress.size);
  if (decompressionResult == NULL) {
    free(fileToDecompress.content);
    exit(1);
  }
  FILE *decompressedOutputFile = fopen(tempFile, "wb");
  if (decompressedOutputFile == NULL) {
    free(fileToDecompress.content);
    destroyDecompressionResult(decompressionResult);
    exit(1);
  }
  fwrite(decompressionResult->bytes, sizeof(unsigned char), decompressionResult->size, decompressedOutputFile);
  fclose(decompressedOutputFile);
  destroyDecompressionResult(decompressionResult);
  free(fileToDecompress.content);

  assert(filesAreEqual(originalFile, tempFile));
}

int main() {
  const char sampleFilePath1[] = "./tests/samples/wave.txt";
  const char compressedFilePath1[] = "./tests/samples/wave.bin";
  const char decompressedFilePath1[] = "./tests/samples/wave1.txt";
  testCompressionAndDecompression(sampleFilePath1, compressedFilePath1, decompressedFilePath1);

  const char sampleFilePath2[] = "./tests/samples/japa.png";
  const char compressedFilePath2[] = "./tests/samples/japa.bin";
  const char decompressedFilePath2[] = "./tests/samples/japa-1.png";
  testCompressionAndDecompression(sampleFilePath2, compressedFilePath2, decompressedFilePath2);

  return 0;
}
