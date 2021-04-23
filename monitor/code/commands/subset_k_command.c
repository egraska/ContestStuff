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

void* parseSubsetKCommand(ConnectionInfo* info, char** parameters)
{
    Subset_K_args* ret = malloc(sizeof(Subset_K_args));
    ret->subsets = *parameters;
    return ret;
}

int executeSubsetKCommand(ConnectionInfo* info, void* in_args)
{
    int ret = COMMAND_FAILURE;
    StateBaseArg* state_args = (StateBaseArg*)in_args;
    Subset_K_args* args = (Subset_K_args*)state_args->userArgs;
    Subset_K_State* state = (Subset_K_State*)state_args->state;

    if(validateSets(args->subsets, state->n))
    {
        addMessage(state->receivers_connection, SUBSET_K_FORMAT_STRING, info->player->name, args->subsets);

        StateBaseArg next_state;
        Subset_J_State subset_j_state;
        subset_j_state.n = state->n;
        subset_j_state.receivers_connection = state->receivers_connection;
        subset_j_state.response_state = state->response_state;

        next_state.state = &subset_j_state;
        ret = performRequireWithState(state->requestors_passive_server, getSubsetJCommand(), &next_state);
    }
    else
    {
        sendCommandError(info, INVALID_SUBSET_K_FORMAT, state->n);
    }
    return ret;
}

void freeSubsetKArgs(void* args)
{
    free(args);
}

Command* createSubsetKCommand()
{
    Command* ret = malloc(sizeof(Command));
    ret->command_name = malloc(sizeof(char) * (strlen(SUBSET_K_COMMAND) + 1));
    ret->checkFormat = parseSubsetKCommand;
    ret->cleanUp = freeSubsetKArgs;
    ret->execute = executeSubsetKCommand;
    sprintf(ret->command_name, SUBSET_K_COMMAND);
    return ret;
}
