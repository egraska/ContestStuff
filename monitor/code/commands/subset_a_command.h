#ifndef SUBSET_A_COMMAND_H
#define SUBSET_A_COMMAND_H

#include "../commands.h"
#include "../connection_info.h"

typedef struct
{
    char* chosen_subsets;
} Subset_A_args;

typedef struct
{
    Response_State* response_state;
    ConnectionInfo* requestors_connection;
    ConnectionInfo* requestors_passive_server;
    int n;
} Subset_A_State;

Command* createSubsetACommand();

#endif
