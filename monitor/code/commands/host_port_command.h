#ifndef HOST_PORT_COMMAND_H
#define HOST_PORT_COMMAND_H

#include "../commands.h"

typedef struct
{
    char* host;
    char* port;
} Host_port_args;

Command* createHostPortCommand();

#endif
