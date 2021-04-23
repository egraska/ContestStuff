#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "random_player_host_port_command.h"
#include "../util.h"
#include "../player_database.h"
#include "../resources.h"
#include "../custom_string.h"
#include "../constants.h"
#include "../commands.h"
#include "../error_messages.h"
#include "../message_processing.h"
#include "../util.h"

void* parseRandomPlayerHostPortCommand(ConnectionInfo* info, char** parameters)
{
    void* ret;
    if(!getNextToken(parameters, SEPERATOR_STR))
    {
        ret = (int*)1;
    }
    else
    {
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, RANDOM_PLAYER_HOST_PORT_COMMAND);
    }
    return ret;
}

int executeRandomPlayerHostPortCommand(ConnectionInfo* info, void* args)
{
    String* random_player_host_port = initString();
    int ret = getRandomPlayerHostPort(random_player_host_port, info->player);
    if(ret == RANDOM_HOST_PORT_GIVEN)
    {
        addMessage(info, RESULTS_FORMAT_STRING, RANDOM_PLAYER_HOST_PORT_COMMAND, stringGetBuffer(random_player_host_port));
        ret = COMMAND_SUCCESS;
    }
    else if(ret == RANDOM_HOST_PORT_NO_CONNECTED_PLAYERS)
    {
        addMessage(info, RESULTS_FORMAT_STRING, RANDOM_PLAYER_HOST_PORT_COMMAND, PLAYERS_NOT_CONNECTED);
        ret = COMMAND_SUCCESS;
    }
    else if(ret == RANDOM_HOST_PORT_TOO_SOON_TO_RECEIVE)
    {
        sendCommandError(info, TOO_SOON_TO_RECEIVE_RANDOM_HOST_PORT);
        ret = COMMAND_FAILURE;
    }
    deinitString(random_player_host_port);
    return ret;
}

void freeRandomPlayerHostPortArgs(void* args)
{
}

Command* createRandomPlayerHostPortCommand()
{
    Command* ret = malloc(sizeof(Command));
    int commandSize = strlen(RANDOM_PLAYER_HOST_PORT_COMMAND) + 1;
    ret->command_name = malloc(sizeof(char) * commandSize);
    ret->checkFormat = parseRandomPlayerHostPortCommand;
    ret->cleanUp = freeRandomPlayerHostPortArgs;
    ret->execute = executeRandomPlayerHostPortCommand;
    sprintf(ret->command_name, RANDOM_PLAYER_HOST_PORT_COMMAND);
    return ret;
}

