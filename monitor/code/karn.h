#ifndef _KARN_H
#define _KARN_H

#define BLOCK_SIZE 40

//VERY IMPORTANT, THESE METHODS DO NOT HANDLE THE GAURD BYTE
int karnGetPaddedLength(int originalLength);
size_t encryptKarn(unsigned char* plaintext, unsigned char* key);
void decryptKarn(unsigned char* ciphertext, unsigned char* key, size_t size);

#endif
