#ifndef UTIL_H
#define UTIL_H
#include <stdbool.h>
#define MAX_LINE_SIZE 100

int utilAddTwoNumbers(int a, int b);
int utilNoOfChars(const char *word);
void utilCaptureConfig(char *filePath, void (*configAdd) (char *, int, int));
int utilEscapeSpace(char *buffer, int index);
bool utilExtractKeyVal(char *buf, int *k, int *v, int index);

#endif
