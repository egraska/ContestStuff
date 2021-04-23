#ifndef PLAYER_HOST_PORT_COMMAND_H
#define PLAYER_HOST_PORT_COMMAND_H

#include "../commands.h"

typedef struct
{
    char* player_name;

} PlayerHostPort_args;

Command* createPlayerHostPortCommand();

#endif
