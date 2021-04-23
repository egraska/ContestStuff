#ifndef SUBSET_K_COMMAND_H
#define SUBSET_K_COMMAND_H

#include "../commands.h"
#include "../connection_info.h"

typedef struct
{
    char* subsets;
} Subset_K_args;

typedef struct
{
    Response_State* response_state;
    ConnectionInfo* receivers_connection;
    ConnectionInfo* requestors_passive_server;
    int n;
} Subset_K_State;

Command* createSubsetKCommand();

#endif
