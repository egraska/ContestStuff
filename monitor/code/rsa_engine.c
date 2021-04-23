#include <gmp.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "rsa_engine.h"
#include "constants.h"
#include "game_parameters.h"
#include "util.h"
#include "constants.h"
#include <openssl/sha.h>

void createMonitorCertificate()
{
    char* exp = malloc(sizeof(char) * (strlen(getMonRSAExp()) + 1));
    char* mod = malloc(sizeof(char) * (strlen(getMonRSAMod()) + 1));

    strcpy(exp, getMonRSAExp());
    strcpy(mod, getMonRSAMod());

    unsigned char hashed_cert[HASH_LENGTH_HEX];
    SHA_CTX sha;
    SHA1_Init(&sha);
    SHA1_Update(&sha, exp, strlen(exp));
    SHA1_Update(&sha, mod, strlen(mod));
    SHA1_Final(hashed_cert, &sha);
    byteArrayToString(hashed_cert, SHA_DIGEST_LENGTH, BASE_HEX);

    char* signed_cert = getMonitorSignedHash(hashed_cert);
    setMonRSACert(signed_cert);
}

char* getMonitorSignedHash(unsigned char* hash)
{
    mpz_t private_key;
    mpz_t mod;
    mpz_t input;
    mpz_t sign;

    mpz_init (sign);
    mpz_init_set_str (input, (char*)hash, BASE_HEX);
    mpz_init_set_str (mod, getMonRSAMod(), getMonRSAKeyBase());
    mpz_init_set_str (private_key, getMonRSAPrivateKey(), getMonRSAKeyBase());

    mpz_powm (sign, input, private_key, mod);

    char *signed_hash = malloc((mpz_sizeinbase (sign, getMonSignedCertBase()) + 2)* sizeof(char));
    mpz_get_str (signed_hash, getMonSignedCertBase(), sign);

    mpz_clear (private_key);
    mpz_clear (mod);
    mpz_clear (input);
    mpz_clear (sign);

    return signed_hash;
}
