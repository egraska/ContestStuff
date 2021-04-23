#include <string.h>
#include <stdio.h>
#include "subset_k_command.h"
#include "subset_j_command.h"
#include "../connection_info.h"
#include "../constants.h"
#include "../util.h"
#include "../error_messages.h"
#include "../custom_string.h"
#include "../message_processing.h"

void* parseSubsetJCommand(ConnectionInfo* info, char** parameters)
{
    Subset_J_args* ret = malloc(sizeof(Subset_J_args));
    ret->subsets = *parameters;
    return ret;
}

int executeSubsetJCommand(ConnectionInfo* info, void* in_args)
{
    int ret = COMMAND_FAILURE;
    StateBaseArg* state_args = (StateBaseArg*)in_args;
    Subset_J_args* args = (Subset_J_args*)state_args->userArgs;
    Subset_J_State* state = (Subset_J_State*)state_args->state;

    if(validateSets(args->subsets, state->n))
    {
        addMessage(state->receivers_connection, SUBSET_J_FORMAT_STRING, info->player->name, args->subsets);

        StateBaseArg nextState;
        nextState.state = state->response_state;
        Command* next_command;
        if(state->response_state->response_command == TRADE_RESPONSE)
        {
            next_command = getTradeResponseCommand();
        }
        else if(state->response_state->response_command == TRUCE_RESPONSE)
        {
            next_command = getWarTruceResponseCommand();
        }
        ret = performRequireWithState(state->receivers_connection,
                                next_command,
                                &nextState);
    }
    else
    {
        sendCommandError(info, INVALID_SUBSET_J_FORMAT, state->n);
    }
    return ret;
}

void freeSubsetJArgs(void* args)
{
    free(args);
}

Command* createSubsetJCommand()
{
    Command* ret = malloc(sizeof(Command));
    ret->command_name = malloc(sizeof(char) * (strlen(SUBSET_J_COMMAND) + 1));
    ret->checkFormat = parseSubsetJCommand;
    ret->cleanUp = freeSubsetJArgs;
    ret->execute = executeSubsetJCommand;
    sprintf(ret->command_name, SUBSET_J_COMMAND);
    return ret;
}
