#ifndef _DHEngine_H
#define _DHEngine_H

void generateRandomKey(char *random_number_buffer, int len);
void calculateDHPublicKey(char *public_key, char *private_key);
void calculateDHSharedSecret(unsigned char *dh_key_buffer, char *own_private_key, char *other_public_key);

#endif
