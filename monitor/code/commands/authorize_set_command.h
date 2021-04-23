#ifndef AUTHORIZE_SET_COMMAND_H
#define AUTHORIZE_SET_COMMAND_H

#include "../commands.h"
#include "../connection_info.h"

typedef struct
{
    char* set;
} Authorize_Set_args;

typedef struct
{
    //this needs to get passed all the way to the end
    Response_State* response_state;
    ConnectionInfo* receivers_connection;
    ConnectionInfo* requestors_passive_server;
    int n;
} Authorize_Set_State;

Command* createAuthorizeSetCommand();

#endif
