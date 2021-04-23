#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "player_status_crack_command.h"
#include "../util.h"
#include "../player_database.h"
#include "../custom_string.h"
#include "../constants.h"
#include "../commands.h"
#include "../error_messages.h"
#include "../message_processing.h"
#include "../util.h"

void* parsePlayerStatusCrackCommand(ConnectionInfo* info, char** parameters)
{
    PlayerStatusCrack_args* ret = malloc(sizeof(PlayerStatusCrack_args));
    ret->player_name = getNextToken(parameters, SEPERATOR_STR);
    ret->computers_used = getNextToken(parameters, SEPERATOR_STR);
    if(!ret->player_name || !ret->computers_used)
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_PLAYER_STATUS_CRACK_FORMAT);
    }
    else if(getNextToken(parameters, SEPERATOR_STR))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, PLAYER_STATUS_CRACK_COMMAND);
    }
    else if(!isBase10(ret->computers_used))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, RESOURCE_AMOUNT_NOT_A_NUMBER);
    }
    return ret;
}

int executePlayerStatusCrackCommand(ConnectionInfo* info, void* args)
{
    PlayerStatusCrack_args* playerStatusCrack_args = (PlayerStatusCrack_args*)args;
    Player* victim;
    victim = checkIfPlayerExists(playerStatusCrack_args->player_name);
    int ret = 1;
    int crack_result = 0;
    if(!victim)
    {
        sendCommandError(info, INVALID_PLAYER_NAME, playerStatusCrack_args->player_name);
        ret = COMMAND_FAILURE;
    }
    if(ret)
    {
        crack_result = performCrack(info->player, atol(playerStatusCrack_args->computers_used), MIN_STATUS_CRACK_CHANCE, getMaxCrackChance(), getCrackGrowthRate());
        if(crack_result == RESULT_CRACK_SUCCESSFUL)
        {
            String* player_status_crack = initString();
            stringAppend(player_status_crack, playerStatusCrack_args->player_name);
            getPlayerStatus(victim, player_status_crack);
            addMessage(info, RESULTS_FORMAT_STRING, PLAYER_STATUS_CRACK_COMMAND, stringGetBuffer(player_status_crack));
            deinitString(player_status_crack);
            ret = COMMAND_SUCCESS;
        }
        else if(crack_result == RESULT_CRACK_UNSUCCESSFUL)
        {
            addMessage(info, RESULTS_FORMAT_STRING, PLAYER_STATUS_CRACK_COMMAND, CRACK_UNSUCCESSFUL);
            ret = COMMAND_SUCCESS;
        }
        else if(crack_result == RESULT_INSUFFICIENT_COMPUTERS)
        {
            sendCommandError(info, INSUFFICIENT_COMPUTERS);
            ret = COMMAND_FAILURE;
        }
    }
    return ret;
}

void freePlayerStatusCrackArgs(void* args)
{
    free(args);
}

Command* createPlayerStatusCrackCommand()
{
    Command* ret = malloc(sizeof(Command));
    int commandSize = strlen(PLAYER_STATUS_CRACK_COMMAND) + 1;
    ret->command_name = malloc(sizeof(char) * commandSize);
    ret->checkFormat = parsePlayerStatusCrackCommand;
    ret->cleanUp = freePlayerStatusCrackArgs;
    ret->execute = executePlayerStatusCrackCommand;
    sprintf(ret->command_name, PLAYER_STATUS_CRACK_COMMAND);
    return ret;
}

