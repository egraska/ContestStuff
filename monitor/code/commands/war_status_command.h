#ifndef WAR_STATUS_COMMAND_H
#define WAR_STATUS_COMMAND_H

#include "../commands.h"

typedef struct WarStatusArgs
{
    char* opponentIdent;
} WarStatusArgs;

Command* createWarStatusCommand();

#endif
