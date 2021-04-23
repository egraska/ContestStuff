#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "war_truce_response_command.h"
#include "../util.h"
#include "../constants.h"
#include "../error_messages.h"
#include "../message_processing.h"
#include "../commands.h"

void* parseWarTruceResponseCommand(ConnectionInfo* info, char** parameters)
{
    Truce_Response_Args* ret = malloc(sizeof(Truce_Response_Args));
    char* answer = getNextToken(parameters, SEPERATOR_STR);
    if(!strcmp(answer, ACCEPT_STRING))
    {
        ret->truce_proccessed = TRUCE_ACCEPTED;
    }
    else if(!strcmp(answer, DECLINE_STRING))
    {
        ret->truce_proccessed = TRUCE_DECLINED;
    }
    else
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_TRUCE_RESPONSE);
    }

    if(ret && getNextToken(parameters, SEPERATOR_STR))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT,
            WAR_TRUCE_RESPONSE_COMMAND);
    }
    return ret;
}

int executeWarTruceResponseCommand(ConnectionInfo* info, void* args)
{
    StateBaseArg* master_args = (StateBaseArg*) args;

    Truce_Response_Args* command_args =
        (Truce_Response_Args*)master_args->userArgs;

    Response_State* state_args =
        (Response_State*)master_args->state;

    state_args->accepted = command_args->truce_proccessed;
    addMessage(info, RESULT_FORMAT_STRING, WAR_TRUCE_RESPONSE_COMMAND);
    return COMMAND_SUCCESS;
}

void freeWarTruce_Response_Args(void* args)
{
    free(args);
}

Command* createWarTruceResponseCommand()
{
    Command* ret = malloc(sizeof(Command));
    ret->command_name = malloc(sizeof(char) * (strlen(WAR_TRUCE_RESPONSE_COMMAND) + 1));
    ret->checkFormat = parseWarTruceResponseCommand;
    ret->cleanUp = freeWarTruce_Response_Args;
    ret->execute = executeWarTruceResponseCommand;
    sprintf(ret->command_name, WAR_TRUCE_RESPONSE_COMMAND);
    return ret;
}
