#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "war_status_command.h"
#include "../util.h"
#include "../war_database.h"
#include "../custom_string.h"
#include "../constants.h"
#include "../commands.h"
#include "../error_messages.h"
#include "../message_processing.h"

void* parseWarStatusCommand(ConnectionInfo* info, char** parameters)
{
    WarStatusArgs* ret = malloc(sizeof(WarStatusArgs));
    ret->opponentIdent = getNextToken(parameters, SEPERATOR_STR);
    if(!ret->opponentIdent)
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_WAR_STATUS_FORMAT);
    }
    else if(getNextToken(parameters, SEPERATOR_STR))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, WAR_STATUS_COMMAND);
    }
    return ret;
}

int executeWarStatusCommand(ConnectionInfo* info, void* in_args)
{
    int ret = COMMAND_FAILURE;
    WarStatusArgs* args = (WarStatusArgs*)in_args;
    Player* opponent = checkIfPlayerExists(args->opponentIdent);
    if(opponent)
    {

        War* war = getPlayersWar(info->player, opponent, 0);
        if (war)
        {
            String* war_status = initString();
            getWarStatus(war, info->player, war_status);
            addMessage(info, RESULTS_FORMAT_STRING, WAR_STATUS_COMMAND, stringGetBuffer(war_status));
            deinitString(war_status);
            ret = COMMAND_SUCCESS;
        }
        else
        {
            sendCommandError(info, WAR_DOES_NOT_EXIST, info->player->name, opponent->name);
        }
    }
    else
    {
        sendCommandError(info, OTHER_PLAYER_DOES_NOT_EXIST, args->opponentIdent);
    }
    return ret;
}

void freeWarStatusArgs(void* args)
{
    free(args);
}

Command* createWarStatusCommand()
{
    Command* ret = malloc(sizeof(Command));
    int commandSize = strlen(WAR_STATUS_COMMAND) + 1;
    ret->command_name = malloc(sizeof(char) * commandSize);
    ret->checkFormat = parseWarStatusCommand;
    ret->cleanUp = freeWarStatusArgs;
    ret->execute = executeWarStatusCommand;
    sprintf(ret->command_name, WAR_STATUS_COMMAND);
    return ret;
}
