#ifndef PLAYER_PASSWORD_CRACK_COMMAND_H
#define PLAYER_PASSWORD_CRACK_COMMAND_H

#include "../commands.h"

#define MIN_PASSWORD_CRACK_CHANCE           0
#define MAX_PASSWORD_CRACK_CHANCE           100
//#define GROWTH_RATE_PASSWORD_CRACK        1.5

#define NO_PASSWORD_REGISTERED              " does not have a password set"

typedef struct
{
    char* player_name;
    char* computers_used;

} PlayerPasswordCrack_args;

Command* createPlayerPasswordCrackCommand();

#endif
