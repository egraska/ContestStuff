#ifndef PLAYER_STATUS_CRACK_COMMAND_H
#define PLAYER_STATUS_CRACK_COMMAND_H

#include "../commands.h"

#define MIN_STATUS_CRACK_CHANCE           0
//#define MAX_STATUS_CRACK_CHANCE         100
//#define GROWTH_RATE_STATUS_CRACK        1.5

typedef struct
{
    char* player_name;
    char* computers_used;

} PlayerStatusCrack_args;

Command* createPlayerStatusCrackCommand();

#endif
