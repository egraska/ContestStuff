#ifndef CHALLENGE_RESPONSE_COMMAND_H
#define CHALLENGE_RESPONSE_COMMAND_H

#include "../commands.h"
#include "../connection_info.h"

typedef struct
{
    char* set;
} Challenge_Response_args;

typedef struct
{
    //this needs to get passed all the way to the end
    Response_State* response_state;
    ConnectionInfo* receivers_connection;
    int n;
} Challenge_Response_State;

Command* createChallengeResponseCommand();

#endif
