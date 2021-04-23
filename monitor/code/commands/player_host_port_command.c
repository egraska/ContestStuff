#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "player_host_port_command.h"
#include "../util.h"
#include "../player_database.h"
#include "../resources.h"
#include "../custom_string.h"
#include "../constants.h"
#include "../commands.h"
#include "../error_messages.h"
#include "../message_processing.h"
#include "../util.h"

void* ParsePlayerHostPortCommand(ConnectionInfo* info, char** parameters)
{
    PlayerHostPort_args* ret = malloc(sizeof(PlayerHostPort_args));
    ret->player_name = getNextToken(parameters, SEPERATOR_STR);
    if(!ret->player_name)
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_PLAYER_HOST_PORT_FORMAT);
    }
    else if(getNextToken(parameters, SEPERATOR_STR))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, PLAYER_HOST_PORT_COMMAND);
    }
    return ret;
}

int executePlayerHostPortCommand(ConnectionInfo* info, void* args)
{
    PlayerHostPort_args* playerHostPort_args = (PlayerHostPort_args*)args;
    Player* victim;
    victim = checkIfPlayerExists(playerHostPort_args->player_name);
    int ret = 1;
    int transfer_result;
    if(!victim)
    {
        sendCommandError(info, INVALID_PLAYER_NAME, playerHostPort_args->player_name);
        ret = COMMAND_FAILURE;
    }
    else
    {
        transfer_result = consumePlayerComputersForInformation(info->player->wallet, getMonRequiredForHP());
        if(transfer_result == 1)
        {
            String* player_host_port = initString();
            stringAppend(player_host_port, playerHostPort_args->player_name);
            stringAppend(player_host_port, SEPERATOR_STR);
            if(getPlayerHostPort(victim, player_host_port))
            {
                addMessage(info, RESULTS_FORMAT_STRING, PLAYER_HOST_PORT_COMMAND, stringGetBuffer(player_host_port));
            }
            else
            {
                addMessage(info, RESULTS_FORMAT_STRING, PLAYER_HOST_PORT_COMMAND, HOST_PORT_NOT_REGISTERED);
            }
            deinitString(player_host_port);
            ret = COMMAND_SUCCESS;
        }
        else
        {
            sendCommandError(info, INSUFFICIENT_COMPUTERS);
            ret = COMMAND_FAILURE;
        }
    }
    return ret;
}

void freePlayerHostPortArgs(void* args)
{
    free(args);
}

Command* createPlayerHostPortCommand()
{
    Command* ret = malloc(sizeof(Command));
    int commandSize = strlen(PLAYER_HOST_PORT_COMMAND) + 1;
    ret->command_name = malloc(sizeof(char) * commandSize);
    ret->checkFormat = ParsePlayerHostPortCommand;
    ret->cleanUp = freePlayerHostPortArgs;
    ret->execute = executePlayerHostPortCommand;
    sprintf(ret->command_name, PLAYER_HOST_PORT_COMMAND);
    return ret;
}

