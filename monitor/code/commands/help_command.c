#include <stdio.h>
#include <string.h>
#include "help_command.h"
#include "../util.h"
#include "../custom_string.h"
#include "../constants.h"
#include "../commands.h"
#include "../message_processing.h"
#include "../error_messages.h"

void* parseHelpCommand(ConnectionInfo* info, char** parameters){
    void* ret;
    if(!getNextToken(parameters, SEPERATOR_STR))
    {
        ret = (int*)1;
    }
    else
    {
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, HELP_COMMAND);
    }
    return ret;
}

int executeHelpCommand(ConnectionInfo* info, void* args)
{
    String* commands = initString();
    getExecutableCommandList(commands);
    addMessage(info, RESULTS_LIST_FORMAT_STRING, HELP_COMMAND, stringGetBuffer(commands));
    deinitString(commands);
    return COMMAND_SUCCESS;
}

void freeHelpArgs(void* args)
{
}

Command* createHelpCommand()
{
    Command* ret = malloc(sizeof(Command));
    int commandSize = strlen(HELP_COMMAND) + 1;
    ret->command_name = malloc(sizeof(char) * commandSize);
    ret->checkFormat = parseHelpCommand;
    ret->cleanUp = freeHelpArgs;
    ret->execute = executeHelpCommand;
    sprintf(ret->command_name, HELP_COMMAND);
    return ret;
}
