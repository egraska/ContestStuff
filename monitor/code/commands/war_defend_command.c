#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "war_defend_command.h"
#include "../util.h"
#include "../constants.h"
#include "../commands.h"
#include "../error_messages.h"
#include "../message_processing.h"
#include "../war.h"
#include "../active_connection.h"

#define WAR_DEFEND_RESULT_FORMAT                      "War begun with %s"

void* parseWarDefendCommand(ConnectionInfo* info, char** parameters)
{
    WarDefendArgs* ret = malloc(sizeof(WarDefendArgs));
    char* weapons;
    char* vehicles;
    char* firstUnit = getNextToken(parameters, SEPERATOR_STR);
    if(firstUnit && !strcmp(firstUnit, VEHICLES_STRING))
    {
        vehicles = getNextToken(parameters, WEAPONS_SEPERATOR);
        weapons = getNextToken(parameters, SEPERATOR_STR);
    }
    else if(firstUnit && !strcmp(firstUnit, WEAPONS_STRING))
    {
        weapons = getNextToken(parameters, VEHICLES_SEPERATOR);
        vehicles = getNextToken(parameters, SEPERATOR_STR);
    }
    if(!vehicles || !weapons)
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_WAR_DEFEND_FORMAT);
    }
    else if(getNextToken(parameters, SEPERATOR_STR))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, WAR_DEFEND_COMMAND);
    }
    else if(!isBase10(weapons))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_NUMBER_OF_WEAPONS);
    }
    else if(!isBase10(vehicles))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_NUMBER_OF_VEHICLES);
    }
    else
    {
        ret->weapons = atol(weapons);
        ret->vehicles = atol(vehicles);
        if(ret->weapons <= 0)
        {
            free(ret);
            ret = NULL;
            sendCommandError(info, INVALID_NUMBER_OF_WEAPONS);
        }
        else if(ret->vehicles <= 0)
        {
            free(ret);
            ret = NULL;
            sendCommandError(info, INVALID_NUMBER_OF_VEHICLES);
        }
    }
    return ret;
}

int executeWarDefendCommand(ConnectionInfo* info, void* in_args)
{
    int ret = COMMAND_FAILURE;
    StateBaseArg* master_args = (StateBaseArg*)in_args;

    WarDefendArgs* args = (WarDefendArgs*)master_args->userArgs;
    WarDefendState* state_args = (WarDefendState*)master_args->state;

    if(!commitResourcesForWar(info->player->wallet, args->weapons, args->vehicles))
    {
        sendCommandError(info, INSUFFICIENT_RESOURCES_WAR);
    }
    else
    {
        state_args->vehicles = args->vehicles;
        state_args->weapons = args->weapons;
        ret = COMMAND_SUCCESS;
        String* str = initString();
        stringAppend(str, WAR_DEFEND_RESULT_FORMAT, state_args->attacker_name);
        addMessage(info, RESULTS_FORMAT_STRING, WAR_DEFEND_COMMAND, stringGetBuffer(str));
        deinitString(str);
    }
    return ret;
}

void freeWarDefendArgs(void* args)
{
    free(args);
}

Command* createWarDefendCommand()
{
    Command* ret = malloc(sizeof(Command));
    int commandSize = strlen(WAR_DEFEND_COMMAND) + 1;
    ret->command_name = malloc(sizeof(char) * commandSize);
    ret->checkFormat = parseWarDefendCommand;
    ret->cleanUp = freeWarDefendArgs;
    ret->execute = executeWarDefendCommand;
    sprintf(ret->command_name, WAR_DEFEND_COMMAND);
    return ret;
}
