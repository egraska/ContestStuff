#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sign_off_command.h"
#include "../util.h"
#include "../constants.h"
#include "../commands.h"
#include "../error_messages.h"
#include "../message_processing.h"

void* ParseSignOffCommand(ConnectionInfo* info, char** parameters)
{
    void* ret;
    if(!getNextToken(parameters, SEPERATOR_STR))
    {
        ret = (int*)1;
    }
    else
    {
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, SIGN_OFF_COMMAND);
    }
    return ret;
}

int executeSignOffCommand(ConnectionInfo* info, void* args)
{
    signOffPlayer(info->player);
    addMessage(info, RESULT_FORMAT_STRING, SIGN_OFF_COMMAND);
    return COMMAND_CLOSE_CONNECTION;
}

void freeSignOffArgs(void* args)
{
}

Command* createSignOffCommand()
{
    Command* ret = malloc(sizeof(Command));
    int commandSize = strlen(SIGN_OFF_COMMAND) + 1;
    ret->command_name = malloc(sizeof(char) * commandSize);
    ret->checkFormat = ParseSignOffCommand;
    ret->cleanUp = freeSignOffArgs;
    ret->execute = executeSignOffCommand;
    sprintf(ret->command_name, SIGN_OFF_COMMAND);
    return ret;
}
