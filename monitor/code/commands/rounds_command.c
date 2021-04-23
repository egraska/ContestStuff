#include <string.h>
#include <stdio.h>
#include "rounds_command.h"
#include "authorize_set_command.h"
#include "../connection_info.h"
#include "../constants.h"
#include "../util.h"
#include "../error_messages.h"
#include "../custom_string.h"
#include "../message_processing.h"

#define MAX_ROUNDS                                      30

void* parseRoundsCommand(ConnectionInfo* info, char** parameters)
{
    Rounds_args* ret = NULL;
    char *rounds_str = getNextToken(parameters, SEPERATOR_STR);

    if(!rounds_str || !isBase32(rounds_str))
    {
        sendCommandError(info, INVALID_ROUNDS_FORMAT);
    }
    else if(getNextToken(parameters, SEPERATOR_STR))
    {
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, ROUNDS_COMMAND);
    }
    else
    {
        ret = malloc(sizeof(Rounds_args));
        ret->rounds = rounds_str;
        //Changed to atoi from charToIntBase32 since we are using base 10 instead of base 32
        int value = atoi(rounds_str);
        //Removed length check of rounds_str, since 30 >= rounds >=10 will have len > 1 in base 10, unlike being = 1 in base 32  
        if(value == 0 || value > MAX_ROUNDS)
        {
            free(ret);
            ret = NULL;
            sendCommandError(info, INVALID_ROUNDS_FORMAT);
        }
    }
    return ret;
}

int executeRoundsCommand(ConnectionInfo* info, void* in_args)
{
    StateBaseArg* state_args = (StateBaseArg*)in_args;
    Rounds_args* args = (Rounds_args*)state_args->userArgs;
    Authentication_State* state = (Authentication_State*)state_args->state;
    addMessage(state->requestors_passive_server, ROUNDS_FORMAT_STRING, args->rounds);

    StateBaseArg next_state;
    Authorize_Set_State authorize_state;
    //Storing rounds as a base 10 number instead of base 32
    authorize_state.n = atoi(args->rounds);
    authorize_state.receivers_connection = info;
    authorize_state.response_state = state->response_state;
    authorize_state.requestors_passive_server = state->requestors_passive_server;

    next_state.state = &authorize_state;

    return performRequireWithState(state->requestors_passive_server, getAuthorizeSetCommand(), &next_state);
}

void freeRoundsArgs(void* args)
{
    free(args);
}

Command* createRoundsCommand()
{
    Command* ret = malloc(sizeof(Command));
    ret->command_name = malloc(sizeof(char) * (strlen(ROUNDS_COMMAND) + 1));
    ret->checkFormat = parseRoundsCommand;
    ret->cleanUp = freeRoundsArgs;
    ret->execute = executeRoundsCommand;
    sprintf(ret->command_name, ROUNDS_COMMAND);
    return ret;
}
