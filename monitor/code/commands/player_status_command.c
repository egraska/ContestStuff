#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "player_status_command.h"
#include "../util.h"
#include "../player_database.h"
#include "../custom_string.h"
#include "../constants.h"
#include "../commands.h"
#include "../error_messages.h"
#include "../message_processing.h"

void* parsePlayerStatusCommand(ConnectionInfo* info, char** parameters)
{
    void* ret;
    if(!getNextToken(parameters, SEPERATOR_STR))
    {
        ret = (int*)1;
    }
    else
    {
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, PLAYER_STATUS_COMMAND);
    }
    return ret;
}

int executePlayerStatusCommand(ConnectionInfo* info, void* args)
{
    String* player_status = initString();
    getPlayerStatus(info->player, player_status);
    addMessage(info, RESULTS_LIST_FORMAT_STRING, PLAYER_STATUS_COMMAND, stringGetBuffer(player_status));
    deinitString(player_status);
    return COMMAND_SUCCESS;
}

void freePlayerStatusArgs(void* args)
{
}

Command* createPlayerStatusCommand()
{
    Command* ret = malloc(sizeof(Command));
    int commandSize = strlen(PLAYER_STATUS_COMMAND) + 1;
    ret->command_name = malloc(sizeof(char) * commandSize);
    ret->checkFormat = parsePlayerStatusCommand;
    ret->cleanUp = freePlayerStatusArgs;
    ret->execute = executePlayerStatusCommand;
    sprintf(ret->command_name, PLAYER_STATUS_COMMAND);
    return ret;
}
