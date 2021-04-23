#ifndef UTIL_H
#define UTIL_H

#include <pthread.h>

int isAlpha(char* str);
int isAlphaNumeric(char* str);
int isFloat(char* str);
int isBase10(char* str);
int isBase16(char* str);
int isBase32(char* str);

char* safeStrCpy(char* str);
void safeFree(void** pointer);

char intToCharBase32(int input);
int  charToIntBase32(char input);

void strUpper(char *);
void strLower(char *);

void str_compare_case_insenitive(char* a, char* b, int max_size);

void lock_mutex(pthread_mutex_t* mutex);
void unlock_mutex(pthread_mutex_t* mutex);

size_t getRadixLenForByteArray(size_t byte_size);
void byteArrayToString(unsigned char* byteArray, size_t input_size, int base);
size_t stringToByteArray(unsigned char* byteArray, int base);

void shuffleArrayPointers(void** array, int numElements);
char* getNextToken(char **stringp, const char *delim);
int validateSets(char* parameter, int n);
#endif
