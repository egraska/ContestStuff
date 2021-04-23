#ifndef PUBLIC_KEY_COMMAND_H
#define PUBLIC_KEY_COMMAND_H

#include "../commands.h"
#include "../custom_string.h"

#define KEY_TYPE_RSA   "RSA"
#define KEY_TYPE_ZKP   "ZKP"
#define NO_KEY         "NONE"

typedef struct
{
    char* auth_type;
    char* exp;
    char* mod;
} Public_Key_args;

typedef struct
{
    String* public_key;
} Public_Key_State;

Command* createPublicKeyCommand();

#endif
