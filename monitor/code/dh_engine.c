#include <time.h>
#include <stdlib.h>
#include <gmp.h>
#include "dh_engine.h"
#include "game_parameters.h"
#include "constants.h"
#include "stdio.h"
#include "util.h"

void generateRandomKey(char *random_number_buffer, int len)
{
    short int n;
    srand(time(0));
    for(int i=0; i<len; i++)
    {
        n = rand() % getDHKeyBase();
        random_number_buffer[i] = intToCharBase32(n);
    }
    random_number_buffer[len] = '\0';
}

void calculateDHPublicKey(char *public_key, char *private_key)
{
    mpz_t p;
    mpz_t g;
    mpz_t b;
    mpz_t B;

    mpz_init (B);
    mpz_init_set_str (p, getDHKeyP(), getDHKeyBase());
    mpz_init_set_str (g, getDHKeyG(), getDHKeyBase());
    mpz_init_set_str (b, private_key, getDHKeyBase());

    mpz_powm (B, g, b, p);

    mpz_get_str(public_key, getDHKeyBase(), B);

    mpz_clear (p);
    mpz_clear (g);
    mpz_clear (b);
    mpz_clear (B);
}

void calculateDHSharedSecret(unsigned char *dh_key_buffer, char *own_private_key, char *other_public_key)
{
    mpz_t p_value;
    mpz_t b;
    mpz_t A;
    mpz_t SS;

    mpz_init (SS);
    mpz_init_set_str (p_value, getDHKeyP(), getDHKeyBase());
    mpz_init_set_str (b, own_private_key, getDHKeyBase());
    mpz_init_set_str (A, other_public_key, getDHKeyBase());

    mpz_powm (SS, A, b, p_value);

    size_t size =  getKarnKeySizeBytes() - (((mpz_sizeinbase(SS, 2) - 1) / 8) + 1);

    if((int)size < 0)
    {
        size_t new_size = (((mpz_sizeinbase(SS, 2) - 1) / 8) + 1);
        unsigned char temp_buffer[new_size];
        mpz_export(temp_buffer, &new_size, 1, 1, -1, 0, SS);
        for(int i=0; i<getKarnKeySizeBytes(); i++)
        {
            dh_key_buffer[i] = temp_buffer[i];
        }
    }
    else
    {
        mpz_export(dh_key_buffer + size, &size, 1, 1, -1, 0, SS);
    }

    mpz_clear (p_value);
    mpz_clear (b);
    mpz_clear (A);
    mpz_clear (SS);
}     
