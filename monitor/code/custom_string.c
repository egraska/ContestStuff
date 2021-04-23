#include "custom_string.h"
#include "error_messages.h"
#include "backup.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#define BASE_MAX_SIZE                       2048

String* initString()
{
    String* ret = malloc(sizeof(String));
    ret->buffer = malloc(sizeof(char) * BASE_STRING_SIZE);
    ret->max_size = BASE_MAX_SIZE;
    ret->current_capacity = BASE_STRING_SIZE;
    if(BASE_STRING_SIZE)
    {
        //set a null character to the first byte
        ret->buffer[0] = 0;
    }
    return ret;
}

void deinitString(String* str)
{
    free(str->buffer);
    free(str);
}

void stringCopy(String* destination, char* source)
{
    size_t len = strlen(source);
    if(len >= destination->current_capacity)
    {
        char* str = realloc(destination->buffer, len + 1);
        if(str != NULL)
        {
            destination->buffer = str;
        }
        else
        {
            fprintf(stderr, FATAL_ERROR);
            copyDatabaseToFile();
            exit(1);
        }
    }
    strcpy(destination->buffer, source);
}

size_t stringLength(String* str)
{
    return strlen(str->buffer);
}

size_t stringPrintf(String* str, char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    int needed_size = vsnprintf(str->buffer, str->current_capacity, format, argptr) + 1;
    va_end(argptr);
    if(needed_size > str->current_capacity)
    {
        while(needed_size > str->current_capacity)
        {
            if(!stringExpand(str))
            {
                break;
            }
        }
        va_start(argptr, format);
        vsnprintf(str->buffer, str->current_capacity, format, argptr);
        va_end(argptr);
    }
    return needed_size;
}

size_t stringVPrintf(String* str, char* format, va_list argsrc)
{
    va_list argptr;
    va_copy(argptr, argsrc);
    int needed_size = vsnprintf(str->buffer, str->current_capacity, format, argptr) + 1;
    va_end(argptr);
    if(needed_size > str->current_capacity)
    {
        while(needed_size > str->current_capacity)
        {
            if(!stringExpand(str))
            {
                break;
            }
        }
        va_copy(argptr, argsrc);
        vsnprintf(str->buffer, str->current_capacity, format, argptr);
        va_end(argptr);
    }
    return needed_size;
}

//Extends the capacity of the string
//Returns the new size on success
//Returns 0 if the string can not be expanded
size_t stringExpand(String* str)
{
    size_t ret = 0;
    if(str->current_capacity < str->max_size)
    {
        size_t new_size = str->current_capacity * 2;
        new_size = new_size < str->max_size ? new_size : str->max_size;
        char *tmp = realloc(str->buffer, sizeof(char) * new_size);
        if(tmp)
        {
            str->buffer = tmp;
            ret = new_size;
            str->current_capacity = new_size;
        }
    }
    return ret;
}

int stringRealign(String* str, size_t start_index, size_t length)
{
    if(start_index + length <= str->current_capacity)
    {
        memmove(str->buffer, str->buffer + start_index, length);
        return length;
    }
    return 0;
}

char* stringGetBuffer(String* str)
{
    return str->buffer;
}

size_t stringAppend(String* str, char* format, ...)
{
    size_t starting_index = strlen(str->buffer);

    va_list argptr;
    va_start(argptr, format);
    int needed_size = vsnprintf(str->buffer + starting_index,
                               str->current_capacity - starting_index,
                               format, argptr) + 1;
    va_end(argptr);

    if(needed_size + starting_index > str->current_capacity)
    {
        while(needed_size + starting_index >  str->current_capacity)
        {
            if(!stringExpand(str))
            {
                break;
            }
        }
        va_start(argptr, format);
        vsnprintf(str->buffer + starting_index,
                 str->current_capacity - starting_index,
                 format, argptr);
        va_end(argptr);
    }
    return needed_size + starting_index;
}

size_t stringVAppend(String* str, char* format, va_list argsrc)
{
    size_t starting_index = strlen(str->buffer);

    va_list argptr;
    va_copy(argptr, argsrc);
    int needed_size = vsnprintf(str->buffer + starting_index,
                               str->current_capacity - starting_index,
                               format, argptr) + 1;
    va_end(argptr);

    if(needed_size + starting_index > str->current_capacity)
    {
        while(needed_size + starting_index >  str->current_capacity)
        {
            if(!stringExpand(str))
            {
                break;
            }
        }
        va_copy(argptr, argsrc);
        vsnprintf(str->buffer + starting_index,
                 str->current_capacity - starting_index,
                 format, argptr);
        va_end(argptr);
    }
    return needed_size + starting_index;
}
