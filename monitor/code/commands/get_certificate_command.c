#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "get_certificate_command.h"
#include "../player_database.h"
#include "../resources.h"
#include "../custom_string.h"
#include "../constants.h"
#include "../commands.h"
#include "../error_messages.h"
#include "../message_processing.h"
#include "../util.h"

void* parseGetCertCommand(ConnectionInfo* info, char** parameters)
{
    Get_Cert_args* ret = malloc(sizeof(Get_Cert_args));
    ret->player_name = getNextToken(parameters, SEPERATOR_STR);
    if(!ret->player_name)
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_GET_CERTIFICATE_FORMAT);
    }
    else if(getNextToken(parameters, SEPERATOR_STR))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, GET_CERTIFICATE_COMMAND);
    }
    return ret;
}

int executeGetCertCommand(ConnectionInfo* info, void* args)
{
    Get_Cert_args* get_cert_args = (Get_Cert_args*)args;
    Player* other_player;
    other_player = checkIfPlayerExists(get_cert_args->player_name);
    int ret = 1;
    if(!other_player)
    {
        if(!strcmp(get_cert_args->player_name, IDENT_MONITOR))
        {
            String* result = initString();
            stringAppend(result, "%s %s %s %s", get_cert_args->player_name, getMonRSAExp(), getMonRSAMod(), getMonRSACert());
            addMessage(info, RESULTS_FORMAT_STRING, GET_CERTIFICATE_COMMAND, stringGetBuffer(result));
            deinitString(result);
        }
        else
        {
            sendCommandError(info, INVALID_PLAYER_NAME, get_cert_args->player_name);
            ret = COMMAND_FAILURE;
        }
    }
    else
    {
        String* player_certificate = initString();
        if(getRSAKey(other_player, player_certificate))
        {
            String* result = initString();
            stringAppend(result, "%s %s", other_player->name, stringGetBuffer(player_certificate));
            addMessage(info, RESULTS_FORMAT_STRING, GET_CERTIFICATE_COMMAND, stringGetBuffer(result));
            deinitString(result);
        }
        else
        {
            addMessage(info, RESULTS_FORMAT_STRING, GET_CERTIFICATE_COMMAND, NO_CERTIFICATE_REGISTERED);
        }
        deinitString(player_certificate);
        ret = COMMAND_SUCCESS;
    }
    return ret;
}

void freeGetCertArgs(void* args)
{
    free(args);
}

Command* createGetCertCommand()
{
    Command* ret = malloc(sizeof(Command));
    int commandSize = strlen(GET_CERTIFICATE_COMMAND) + 1;
    ret->command_name = malloc(sizeof(char) * commandSize);
    ret->checkFormat = parseGetCertCommand;
    ret->cleanUp = freeGetCertArgs;
    ret->execute = executeGetCertCommand;
    sprintf(ret->command_name, GET_CERTIFICATE_COMMAND);
    return ret;
}
