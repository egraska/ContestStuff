#ifndef SUBSET_J_COMMAND_H
#define SUBSET_J_COMMAND_H

#include "../commands.h"
#include "../connection_info.h"

typedef struct
{
    char* subsets;
} Subset_J_args;

typedef struct
{
    Response_State* response_state;
    ConnectionInfo* receivers_connection;
    int n;
} Subset_J_State;

Command* createSubsetJCommand();

#endif
