#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "get_monitor_key_command.h"
#include "../util.h"
#include "../constants.h"
#include "../commands.h"
#include "../error_messages.h"
#include "../message_processing.h"

void* parseGetMonitorKeyCommand(ConnectionInfo* info, char** parameters){
    void* ret;
    if(!getNextToken(parameters, SEPERATOR_STR))
    {
        ret = (int*)1;
    }
    else
    {
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, GET_MONITOR_KEY_COMMAND);
    }
    return ret;
}

int executeGetMonitorKeyCommand(ConnectionInfo* info, void* args)
{
    addMessage(info, RESULTS_FORMAT_STRING, GET_MONITOR_KEY_COMMAND, getMonRSAMod());
    return COMMAND_SUCCESS;
}

void freeGetMonitorKeyArgs(void* args)
{
}

Command* createGetMonitorKeyCommand()
{
    Command* ret = malloc(sizeof(Command));
    int commandSize = strlen(GET_MONITOR_KEY_COMMAND) + 1;
    ret->command_name = malloc(sizeof(char) * commandSize);
    ret->checkFormat = parseGetMonitorKeyCommand;
    ret->cleanUp = freeGetMonitorKeyArgs;
    ret->execute = executeGetMonitorKeyCommand;
    sprintf(ret->command_name, GET_MONITOR_KEY_COMMAND);
    return ret;
}
