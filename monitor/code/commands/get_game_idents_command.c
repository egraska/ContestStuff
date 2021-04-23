#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "get_game_idents_command.h"
#include "../util.h"
#include "../player_database.h"
#include "../custom_string.h"
#include "../constants.h"
#include "../commands.h"
#include "../error_messages.h"
#include "../message_processing.h"

void* parseGetGameIdentsCommand(ConnectionInfo* info, char** parameters){
    void* ret;
    if(!getNextToken(parameters, SEPERATOR_STR))
    {
        ret = (int*)1;
    }
    else
    {
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, GET_GAME_IDENTS_COMMAND);
    }
    return ret;
}

int executeGetGameIdentsCommand(ConnectionInfo* info, void* args)
{
    String* idents = initString();
    getGameIdents(idents);
    addMessage(info, RESULTS_LIST_FORMAT_STRING, GET_GAME_IDENTS_COMMAND, stringGetBuffer(idents));
    deinitString(idents);
    return COMMAND_SUCCESS;
}

void freeGetGameIdentsArgs(void* args)
{
}

Command* createGetGameIdentsCommand()
{
    Command* ret = malloc(sizeof(Command));
    int commandSize = strlen(GET_GAME_IDENTS_COMMAND) + 1;
    ret->command_name = malloc(sizeof(char) * commandSize);
    ret->checkFormat = parseGetGameIdentsCommand;
    ret->cleanUp = freeGetGameIdentsArgs;
    ret->execute = executeGetGameIdentsCommand;
    sprintf(ret->command_name, GET_GAME_IDENTS_COMMAND);
    return ret;
}
