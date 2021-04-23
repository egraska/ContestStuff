#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "player_password_crack_command.h"
#include "../util.h"
#include "../player_database.h"
#include "../custom_string.h"
#include "../constants.h"
#include "../commands.h"
#include "../error_messages.h"
#include "../message_processing.h"
#include "../util.h"

void* parsePlayerPasswordCrackCommand(ConnectionInfo* info, char** parameters)
{
    PlayerPasswordCrack_args* ret = malloc(sizeof(PlayerPasswordCrack_args));
    ret->player_name = getNextToken(parameters, SEPERATOR_STR);
    ret->computers_used = getNextToken(parameters, SEPERATOR_STR);
    if(!ret->player_name || !ret->computers_used)
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_PLAYER_PASSWORD_CRACK_FORMAT);
    }
    else if(getNextToken(parameters, SEPERATOR_STR))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, PLAYER_PASSWORD_CRACK_COMMAND);
    }
    else if(!isBase10(ret->computers_used))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, RESOURCE_AMOUNT_NOT_A_NUMBER);
    }
    return ret;
}

int executePlayerPasswordCrackCommand(ConnectionInfo* info, void* args)
{
    PlayerPasswordCrack_args* playerPasswordCrack_args = (PlayerPasswordCrack_args*)args;
    Player* victim;
    victim = checkIfPlayerExists(playerPasswordCrack_args->player_name);
    int ret = 1;
    int crack_result = 0;
    if(!victim)
    {
        sendCommandError(info, INVALID_PLAYER_NAME, playerPasswordCrack_args->player_name);
        ret = COMMAND_FAILURE;
    }
    if(ret)
    {
        crack_result = performCrack(info->player, atol(playerPasswordCrack_args->computers_used), MIN_PASSWORD_CRACK_CHANCE, MAX_PASSWORD_CRACK_CHANCE, getPasswordCrackGrowthRate());
        if(crack_result == RESULT_CRACK_SUCCESSFUL)
        {
            String* player_password_crack = initString();
            stringAppend(player_password_crack, playerPasswordCrack_args->player_name);
            if(getPlayerPassword(victim, player_password_crack))
            {
                addMessage(info, RESULTS_FORMAT_STRING, PLAYER_PASSWORD_CRACK_COMMAND, stringGetBuffer(player_password_crack));
            }
            else
            {
                stringAppend(player_password_crack, NO_PASSWORD_REGISTERED);
                addMessage(info, RESULTS_FORMAT_STRING, PLAYER_PASSWORD_CRACK_COMMAND, stringGetBuffer(player_password_crack));
            }
            deinitString(player_password_crack);
            ret = COMMAND_SUCCESS;
        }
        else if(crack_result == RESULT_CRACK_UNSUCCESSFUL)
        {
            addMessage(info, RESULTS_FORMAT_STRING, PLAYER_PASSWORD_CRACK_COMMAND, CRACK_UNSUCCESSFUL);
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

void freePlayerPasswordCrackArgs(void* args)
{
    free(args);
}

Command* createPlayerPasswordCrackCommand()
{
    Command* ret = malloc(sizeof(Command));
    int commandSize = strlen(PLAYER_PASSWORD_CRACK_COMMAND) + 1;
    ret->command_name = malloc(sizeof(char) * commandSize);
    ret->checkFormat = parsePlayerPasswordCrackCommand;
    ret->cleanUp = freePlayerPasswordCrackArgs;
    ret->execute = executePlayerPasswordCrackCommand;
    sprintf(ret->command_name, PLAYER_PASSWORD_CRACK_COMMAND);
    return ret;
}

