#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trade_response_command.h"
#include "../util.h"
#include "../constants.h"
#include "../error_messages.h"
#include "../message_processing.h"
#include "../commands.h"

void* parseTradeResponseCommand(ConnectionInfo* info, char** parameters)
{
    Trade_Response_Args* ret = malloc(sizeof(Trade_Response_Args));
    char* answer = getNextToken(parameters, SEPERATOR_STR);
    if(!strcmp(answer, ACCEPT_STRING))
    {
        ret->trade_proccessed = TRADE_ACCEPTED;
    }
    else if(!strcmp(answer, DECLINE_STRING))
    {
        ret->trade_proccessed = TRADE_DECLINED;
    }
    else
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_TRADE_RESPONSE);
    }

    if(ret && getNextToken(parameters, SEPERATOR_STR))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT,
            TRADE_RESPONSE_COMMAND);
    }
    return ret;
}

int executeTradeResponseCommand(ConnectionInfo* info, void* args)
{
    StateBaseArg* master_args = (StateBaseArg*) args;

    Trade_Response_Args* command_args =
        (Trade_Response_Args*)master_args->userArgs;

    Response_State* state_args =
        (Response_State*)master_args->state;

    state_args->accepted = command_args->trade_proccessed;
    addMessage(info, RESULT_FORMAT_STRING, TRADE_RESPONSE_COMMAND);
    return COMMAND_SUCCESS;
}

void freeTrade_Response_Args(void* args)
{
    free(args);
}

Command* createTradeResponseCommand()
{
    Command* ret = malloc(sizeof(Command));
    ret->command_name = malloc(sizeof(char) * (strlen(TRADE_RESPONSE_COMMAND) + 1));
    ret->checkFormat = parseTradeResponseCommand;
    ret->cleanUp = freeTrade_Response_Args;
    ret->execute = executeTradeResponseCommand;
    sprintf(ret->command_name, TRADE_RESPONSE_COMMAND);
    return ret;
}
