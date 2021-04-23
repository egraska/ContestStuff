#ifndef _COMMANDS_H
#define _COMMANDS_H
#include "connection_info.h"

#define COMMAND_SUCCESS             1
#define COMMAND_FAILURE             0
#define COMMAND_CLOSE_CONNECTION   -1

#define TRADE_RESPONSE              1
#define TRUCE_RESPONSE              2

struct Command;

typedef struct Command{
    char* command_name;
    void* (*checkFormat)(ConnectionInfo*, char**);
    int (*execute)(ConnectionInfo*, void*);
    void (*cleanUp)(void*);
} Command;

typedef struct {
    void* state;
    void* userArgs;
} StateBaseArg;

typedef struct
{
    int accepted;
    int response_command;
} Response_State;

typedef struct
{
    //this needs to get passed all the way to the end
    Response_State* response_state;
    ConnectionInfo* requestors_connection;
    ConnectionInfo* requestors_passive_server;
} Authentication_State;

void deinitCommand(Command* command);

int createCommands();
void freeCommands();

Command* getIdentCommand();
Command* getPasswordCommand();
Command* getAliveCommand();
Command* getHostPortCommand();
Command* getQuitCommand();
Command* getTradeResponseCommand();
Command* getWarDefendCommand();
Command* getWarTruceResponseCommand();
Command* getPublicKeyCommand();
Command* getRoundsCommand();
Command* getAuthorizeSetCommand();
Command* getChallengeCommand();
Command* getSubsetACommand();
Command* getSubsetKCommand();
Command* getSubsetJCommand();
Command* getChallengeResponseCommand();
Command* getPlayerCommandByName(char* name);
void getExecutableCommandList(String* output_buffer);

#endif
