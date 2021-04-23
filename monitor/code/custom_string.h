#ifndef CUSTOM_STRING_H
#define CUSTOM_STRING_H

#include <stdlib.h>
#include <stdarg.h>

#define BASE_STRING_SIZE				256

typedef struct{
	char* buffer;
	int max_size;
	int current_capacity;
} String;

String* initString();
void deinitString(String* str);
void stringCopy(String* destination, char* source);
size_t stringLength(String* str);
size_t stringPrintf(String* str, char* format, ...);
size_t stringVPrintf(String* str, char* format, va_list argsrc);
size_t stringExpand(String* str);
int stringRealign(String* str, size_t start_index, size_t length);
char* stringGetBuffer(String* str);
size_t stringAppend(String* str, char* format, ...);
size_t stringVAppend(String* str, char* format, va_list argsrc);

#endif
