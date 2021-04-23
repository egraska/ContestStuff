#include <string.h>
#include <stdio.h>
#include "subset_a_command.h"
#include "subset_k_command.h"
#include "../connection_info.h"
#include "../constants.h"
#include "../util.h"
#include "../error_messages.h"
#include "../custom_string.h"
#include "../message_processing.h"

void* parseSubsetACommand(ConnectionInfo* info, char** parameters)
{
    Subset_A_args* ret = malloc(sizeof(Subset_A_args));
    ret->chosen_subsets = *parameters;
    return ret;
}

int executeSubsetACommand(ConnectionInfo* info, void* in_args)
{
    int ret = COMMAND_FAILURE;
    StateBaseArg* state_args = (StateBaseArg*)in_args;
    Subset_A_args* args = (Subset_A_args*)state_args->userArgs;
    Subset_A_State* state = (Subset_A_State*)state_args->state;

    if(validateSets(args->chosen_subsets, state->n))
    {
        addMessage(state->requestors_passive_server, SUBSET_A_FORMAT_STRING, args->chosen_subsets);

        StateBaseArg next_state;
        Subset_K_State subset_k_state;
        subset_k_state.n = state->n;
        subset_k_state.receivers_connection = info;
        subset_k_state.response_state = state->response_state;
        subset_k_state.requestors_passive_server = state->requestors_passive_server;

        next_state.state = &subset_k_state;
        ret = performRequireWithState(state->requestors_passive_server, getSubsetKCommand(), &next_state);
    }
    else
    {
        sendCommandError(info, INVALID_SUBSET_A_FORMAT, state->n);
    }
    return ret;
}

void freeSubsetAArgs(void* args)
{
    free(args);
}

Command* createSubsetACommand()
{
    Command* ret = malloc(sizeof(Command));
    ret->command_name = malloc(sizeof(char) * (strlen(SUBSET_A_COMMAND) + 1));
    ret->checkFormat = parseSubsetACommand;
    ret->cleanUp = freeSubsetAArgs;
    ret->execute = executeSubsetACommand;
    sprintf(ret->command_name, SUBSET_A_COMMAND);
    return ret;
}
