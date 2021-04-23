#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "quit_command.h"
#include "../util.h"
#include "../constants.h"
#include "../commands.h"
#include "../error_messages.h"
#include "../message_processing.h"

void* parseQuitCommand(ConnectionInfo* info, char** parameters)
{
    void* ret;
    if(!getNextToken(parameters, SEPERATOR_STR))
    {
        ret = (int*)1;
    }
    else
    {
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, QUIT_COMMAND);
    }
    return ret;
}

int executeQuitCommand(ConnectionInfo* info, void* args)
{
    addMessage(info, RESULT_FORMAT_STRING, QUIT_COMMAND);
    return COMMAND_CLOSE_CONNECTION;
}

void freeQuitArgs(void* args)
{
}

Command* createQuitCommand()
{
    Command* ret = malloc(sizeof(Command));
    int commandSize = strlen(QUIT_COMMAND) + 1;
    ret->command_name = malloc(sizeof(char) * commandSize);
    ret->checkFormat = parseQuitCommand;
    ret->cleanUp = freeQuitArgs;
    ret->execute = executeQuitCommand;
    sprintf(ret->command_name, QUIT_COMMAND);
    return ret;
}
