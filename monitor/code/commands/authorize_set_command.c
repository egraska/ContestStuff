#include <string.h>
#include <stdio.h>
#include "authorize_set_command.h"
#include "subset_a_command.h"
#include "../connection_info.h"
#include "../constants.h"
#include "../util.h"
#include "../error_messages.h"
#include "../custom_string.h"
#include "../message_processing.h"

void* parseAuthorizeSetCommand(ConnectionInfo* info, char** parameters)
{
    Authorize_Set_args* ret = malloc(sizeof(Authorize_Set_args));
    ret->set = *parameters;
    // normally we would do more validation here
    // however it makes more sense to validate when we know how many we are expecting.
    // also the user should never "execute" this it will only be required.
    return ret;
}

int executeAuthorizeSetCommand(ConnectionInfo* info, void* in_args)
{
    int ret = COMMAND_FAILURE;
    StateBaseArg* state_args = (StateBaseArg*)in_args;
    Authorize_Set_args* args = (Authorize_Set_args*)state_args->userArgs;
    Authorize_Set_State* state = (Authorize_Set_State*)state_args->state;

    if(validateSets(args->set, state->n))
    {
        addMessage(state->receivers_connection, AUTHORIZE_SET_FORMAT_STRING, info->player->name, args->set);
        StateBaseArg** nextStates = malloc(sizeof(StateBaseArg*) * 2);
        nextStates[0] = malloc(sizeof(StateBaseArg));
        nextStates[1] = malloc(sizeof(StateBaseArg));

        Subset_A_State SubsetAState;
        SubsetAState.requestors_connection = info;
        SubsetAState.response_state = state->response_state;
        SubsetAState.requestors_passive_server = state->requestors_passive_server;
        SubsetAState.n = state->n;

        nextStates[0]->state = state->response_state;
        nextStates[1]->state = &SubsetAState;

        Command* commands[2];
        commands[0] = getTradeResponseCommand();
        commands[1] = getSubsetACommand();

        ret = performRequireWithMultiple(state->receivers_connection, commands,
                        nextStates, 2);

        free(nextStates[0]);
        free(nextStates[1]);
        free(nextStates);
    }
    else
    {
        sendCommandError(info, INVALID_AUTHORIZE_SET_FORMAT, state->n);
    }
    return ret;
}

void freeAuthorizeSetArgs(void* args)
{
    free(args);
}

Command* createAuthorizeSetCommand()
{
    Command* ret = malloc(sizeof(Command));
    ret->command_name = malloc(sizeof(char) * (strlen(AUTHORIZE_SET_COMMAND) + 1));
    ret->checkFormat = parseAuthorizeSetCommand;
    ret->cleanUp = freeAuthorizeSetArgs;
    ret->execute = executeAuthorizeSetCommand;
    sprintf(ret->command_name, AUTHORIZE_SET_COMMAND);
    return ret;
}
