#ifndef ROUNDS_COMMAND_H
#define ROUNDS_COMMAND_H

#include "../commands.h"
#include "../connection_info.h"

typedef struct
{
    char* rounds;
} Rounds_args;

Command* createRoundsCommand();

#endif
