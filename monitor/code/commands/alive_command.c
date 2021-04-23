#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "alive_command.h"
#include "../util.h"
#include "../constants.h"
#include "../commands.h"
#include "../error_messages.h"
#include "../message_processing.h"

#define ALIVE_SUCCESS                           "Identity has been verified"

void* parseAliveCommand(ConnectionInfo* info, char** parameters)
{
    Alive_args* ret = malloc(sizeof(Alive_args));
    ret->alive = getNextToken(parameters, SEPERATOR_STR);
    if(!ret->alive)
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_ALIVE_FORMAT);

    }
    else if(getNextToken(parameters, SEPERATOR_STR))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, ALIVE_COMMAND);
    }
    return ret;
}

int executeAliveCommand(ConnectionInfo* info, void* args)
{   Alive_args* alive_args = (Alive_args*)args;
    if(!doesCookieMatch(info->player, alive_args->alive))
    {
        sendCommandError(info, ALIVE_FAULT);
        return COMMAND_FAILURE;
    }
    else
    {
        addMessage(info, RESULTS_FORMAT_STRING, ALIVE_COMMAND, ALIVE_SUCCESS);
        strcpy(info->session_cookie, alive_args->alive);
        return COMMAND_SUCCESS;
    }
}

void freeAliveArgs(void* args)
{
    free(args);
}

Command* createAliveCommand()
{
    Command* ret = malloc(sizeof(Command));
    int commandSize = strlen(ALIVE_COMMAND) + 1;
    ret->command_name = malloc(sizeof(char) * commandSize);
    ret->checkFormat = parseAliveCommand;
    ret->cleanUp = freeAliveArgs;
    ret->execute = executeAliveCommand;
    sprintf(ret->command_name, ALIVE_COMMAND);
    return ret;
}
