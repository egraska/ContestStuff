#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <openssl/sha.h>
#include "game_parameters.h"
#include "karn.h"
#include "constants.h"

#define BLOCK_HALF_SIZE                                     (BLOCK_SIZE / 2)

int karnGetPaddedLength(int originalLength)
{
    return (((originalLength - 1) / BLOCK_SIZE ) +1) * BLOCK_SIZE + 1;
}

void encryptBlock(unsigned char* output_buffer, unsigned char* key, unsigned char* block)
{
    //This will allow us to support an in place encrypt
    int KEY_HALF = getKarnKeySizeBytes() / 2;
    unsigned char buffer[BLOCK_HALF_SIZE];
    unsigned char* key_left = key;
    unsigned char* key_right = key + KEY_HALF;
    SHA_CTX sha;

    SHA1_Init(&sha);

    SHA1_Update(&sha, block, BLOCK_HALF_SIZE);
    SHA1_Update(&sha, key_left, KEY_HALF);
    
    SHA1_Final(buffer, &sha);

    for(int i = 0; i < BLOCK_HALF_SIZE; i++)
    {
        output_buffer[i + BLOCK_HALF_SIZE] = buffer[i] ^ block[i + BLOCK_HALF_SIZE];
    }

    SHA1_Init(&sha);

    SHA1_Update(&sha, output_buffer + BLOCK_HALF_SIZE, BLOCK_HALF_SIZE);
    SHA1_Update(&sha, key_right, KEY_HALF);

    SHA1_Final(buffer, &sha);

    for(int i = 0; i < BLOCK_HALF_SIZE; i++)
    {
        output_buffer[i] = buffer[i] ^  block[i];
    }
}

void decryptBlock(unsigned char* output_buffer, unsigned char* key, unsigned char* block)
{
    int KEY_HALF = getKarnKeySizeBytes() / 2;
    unsigned char* key_left = key;
    unsigned char* key_right = key + KEY_HALF;
    //This will allow us to support an in place decrypt
    unsigned char buffer[BLOCK_SIZE];

    SHA_CTX sha;
    SHA1_Init(&sha);
    
    SHA1_Update(&sha, block + BLOCK_HALF_SIZE, BLOCK_HALF_SIZE);
    SHA1_Update(&sha, key_right, KEY_HALF);
    
    SHA1_Final(buffer, &sha);

    for(int i = 0; i < BLOCK_HALF_SIZE; i++)
    {
        output_buffer[i] = buffer[i] ^ block[i];
    }

    SHA1_Init(&sha);

    SHA1_Update(&sha, output_buffer, BLOCK_HALF_SIZE);
    SHA1_Update(&sha, key_left, KEY_HALF);

    SHA1_Final(buffer, &sha);

    for(int i = 0; i < BLOCK_HALF_SIZE; i++)
    {
        output_buffer[i + BLOCK_HALF_SIZE] = buffer[i] ^ block[i + BLOCK_HALF_SIZE];
    }
}

size_t encryptKarn(unsigned char* plaintext, unsigned char* key)
{
    unsigned char plainBuffer[BLOCK_SIZE];
    size_t plaintext_length = strlen((char*)plaintext);
    size_t needed_size = karnGetPaddedLength(plaintext_length);
    unsigned char* plaintext_pointer = plaintext;
    unsigned char* tempBuffer = malloc(sizeof(char) * (needed_size));
    memset(tempBuffer, 0, needed_size);
    size_t i;
    int in_message = 1;
    for(i = 0; in_message || i % BLOCK_SIZE; i++)
    {
        if(in_message)
        {
            plainBuffer[i % BLOCK_SIZE] = plaintext[i % BLOCK_SIZE];
            if(!plaintext[i % BLOCK_SIZE])
            {
                in_message = 0;
                //Edge case where this would be the first byte of a block
                if(!(i % BLOCK_SIZE))
                {
                    break;
                }
            }
        }
        else
        {
            plainBuffer[i % BLOCK_SIZE] = rand() % 0xFF;
        }

        if(!((i+1) % BLOCK_SIZE))
        {
            encryptBlock(plaintext, key, plainBuffer);
            plaintext += BLOCK_SIZE;
        }
    }

    tempBuffer[0] = 42;
    plaintext = plaintext_pointer;
    for(int j = 0; j<(needed_size - 1); j++)
    {
        tempBuffer[j+1] = tempBuffer[j+1] ^ plaintext[j];
    }
    for(int j = 0; j<needed_size; j++)
    {
        plaintext[j] = tempBuffer[j];
    }
    free(tempBuffer);

    return (i+1);
}

void decryptKarn(unsigned char* ciphertext, unsigned char* key, size_t size)
{
    for(int i = 0; i + BLOCK_SIZE - 1< size; i += BLOCK_SIZE)
    {
        decryptBlock(ciphertext + i, key, ciphertext + i);
    }
}
