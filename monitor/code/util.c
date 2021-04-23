#include <gmp.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "constants.h"

#define SHUFFLES_PER_ELEMENT        10

int isAlpha(char* str)
{
    int ret = 1;
    for(char* byte = str; *byte; byte++)
    {
        if(ret < 'A' || ret > 'Z')
        {
            ret = 0;
            break;
        }
    }
    return ret;
}

int isAlphaNumeric(char* str)
{
    int ret = 1;
    for(char* byte = str; *byte; byte++)
    {
        if((ret < 'A' || ret > 'Z') && (ret < '0' || ret > '9'))
        {
            ret = 0;
            break;
        }
    }
    return ret;
}

int isFloat(char* str)
{
    int ret = 1;
    int decimal_point = 0;
    for(char* byte = str; *byte; byte++)
    {
        if(*byte < '0' || *byte > '9')
        {
            if(*byte == '.' && !decimal_point)
            {
                decimal_point = 1;
            }
            else
            {
                ret = 0;
                break;
            }
        }
    }
    return ret;
}

int isBase10(char* str)
{
    int ret = 1;
    for(char* byte = str; *byte; byte++)
    {
        if(*byte < '0' || *byte > '9')
        {
            ret = 0;
            break;
        }
    }
    return ret;
}

int isBase16(char* str)
{
    int ret = 1;
    for(char* byte = str; *byte; byte++)
    {
        if((*byte < 'A' || *byte > 'F') &&
           (*byte < '0' || *byte > '9'))
        {
            ret = 0;
            break;
        }
    }
    return ret;
}

int isBase32(char* str)
{
    int ret = 1;
    for(char* byte = str; *byte; byte++)
    {
        if((*byte < 'A' || *byte > 'V') &&
           (*byte < '0' || *byte > '9'))
        {
            ret = 0;
            break;
        }
    }
    return ret;
}

char* safeStrCpy(char* str)
{
    char* ret = NULL;
    if(str)
    {
        int length = strlen(str);
        ret = malloc(sizeof(char) * length + 1);
        strcpy(ret, str);
    }
    return ret;
}

void safeFree(void** pointer)
{
    if(*pointer)
    {
        free(*pointer);
        *pointer = NULL;
    }
}

void strUpper(char *str)
{
    int i = 0;
    while(str[i])
    {
        if(str[i] >= 'a' && str[i] <= 'z')
        {
            str[i] = str[i] - 'a' + 'A';
        }
        i++;
    }
}

void strLower(char *str)
{
    int i = 0;
    while(str[i])
    {
        if(str[i] >= 'A' && str[i] <= 'Z')
        {
            str[i] = str[i] - 'A' + 'a';
        }
        i++;
    }
}

char intToCharBase32(int input)
{
    char ret = 0;
    if(input <= 9)
    {
        ret = '0' + input;
    }
    else if(input < 32)
    {
        ret = (input - 10) + 'A';
    }
    return ret;
}


int charToIntBase32(char input)
{
    int ret = -1;
    if(input >= 'A' && input <= 'V')
    {
        ret = input - 'A';
    }
    else if (input >= '0' && input <= '9')
    {
        ret = input - '0';
    }
    else if(input >= 'a' && input <= 'v')
    {
        ret = input - 'a';
    }
    return ret;
}

size_t getRadixLenForByteArray(size_t byte_size)
{
    return (byte_size  * 8) / 5 + 1;
}


void byteArrayToString(unsigned char* byteArray, size_t input_size, int base)
{
    mpz_t val;
    mpz_init(val);
    mpz_import(val, input_size, 1, 1, 0, 0, byteArray);
    mpz_get_str((char*) byteArray, base, val);
    mpz_clear(val);
}

size_t stringToByteArray(unsigned char* str, int base)
{
    mpz_t val;
    size_t size = 0;
    mpz_init_set_str(val, (char*) str, base);
    mpz_export(str, &size, 1, 1, 0, 0, val);
    mpz_clear(val);
    return size;
}

void shuffleArrayPointers(void** array, int numElements)
{
    // Sure this isnt gaunteed to be random, and sure its ugly.
    // But doing this "right" just isnt worth it.  psudeo random is fine.
    if(numElements > 1)
    {
        int x = 0;
        int y = 0;
        for(int i = 0; i < SHUFFLES_PER_ELEMENT * numElements; i++)
        {
            do
            {
                x = rand() % numElements;
                y = rand() % numElements;
            }while( x == y );

            void* swap = array[x];
            array[x] = array[y];
            array[y] = swap;
        }
    }
}

char* strsep_str(char** stringp, const char* delim)
{
    char* token = *stringp;
    int found = 0;
    if(*stringp)
    {
        int delim_len = strlen(delim);
        while(**stringp && !found)
        {
            found = 1;
            for(int i = 0; i < delim_len; i++)
            {
                if((*stringp)[i] != delim[i])
                {
                    found = 0;
                    break;
                }
            }

            if(found)
            {
                for(int i = 0; i < delim_len; i++)
                {
                    **stringp = 0x00;
                    (*stringp)++;
                }
            }
            else
            {
                (*stringp)++;
            }
        }
        if(!found)
        {
            *stringp = NULL;
        }
    }
    return token;
}

char* getNextToken(char **stringp, const char* delim)
{
    char *token;
    while((token = strsep_str(stringp, delim)) != NULL)
    {
        if(*token != 0x00)
        {
            break;
        }
    }
    return token;
}

int validateSets(char* parameter, int n)
{
    int ret = 1;
    int inData = 0;
    int numSet = 0;
    while(*parameter)
    {
        if((*parameter >= 'A' && *parameter <= 'V') ||
           (*parameter >= '0' && *parameter <= '9'))
        {
            if(!inData)
            {
                inData = 1;
                numSet++;
            }
        }
        else if(*parameter == ' ')
        {
            if(inData)
            {
                if(numSet >= n)
                {
                    ret = 0;
                    break;
                }
                inData = 0;
            }
        }
        else
        {
            ret = 0;
            break;
        }
        parameter++;
    }
    return ret;
}
