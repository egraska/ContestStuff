#ifndef CHALLENGE_COMMAND_H
#define CHALLENGE_COMMAND_H

#include "../commands.h"
#include "../connection_info.h"

typedef struct
{
    char* challenge;
} Challenge_args;

Command* createChallengeCommand();

#endif
