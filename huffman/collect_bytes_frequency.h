#ifndef COLLECT_BYTES_FREQUENCY_H
#define COLLECT_BYTES_FREQUENCY_H

/*
It iterates through the file content and counts
the frequency of each symbol it has. To do so, it
crates an array having 256 elements in which each
index of such array represents a file's byte, and
the value store on each index is the byte frequency.

In case it fails to allocate the 256 array it'll
return NULL.
*/
size_t *collectBytesFrequency(unsigned char *fileContent, size_t fileSize);

#endif
