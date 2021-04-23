#ifndef ALIVE_COMMAND_H
#define ALIVE_COMMAND_H

#include "../commands.h"

typedef struct
{
    char* alive;

} Alive_args;

Command* createAliveCommand();

#endif
