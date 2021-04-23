#include <string.h>
#include <stdio.h>
#include "challenge_response_command.h"
#include "../connection_info.h"
#include "../constants.h"
#include "../util.h"
#include "../error_messages.h"
#include "../custom_string.h"
#include "../message_processing.h"

void* parseChallengeResponseCommand(ConnectionInfo* info, char** parameters)
{
    Challenge_Response_args* ret = malloc(sizeof(Challenge_Response_args));
    ret->set = *parameters;
    // normally we would do more validation here
    // however it makes more sense to validate when we know how many we are expecting.
    // also the user should never "execute" this it will only be required.
    return ret;
}

int executeChallengeResponseCommand(ConnectionInfo* info, void* in_args)
{
    int ret = COMMAND_FAILURE;
    StateBaseArg* state_args = (StateBaseArg*)in_args;
    Challenge_Response_args* args = (Challenge_Response_args*)state_args->userArgs;
    Challenge_Response_State* state = (Challenge_Response_State*)state_args->state;

    if(validateSets(args->set, state->n))
    {
        addMessage(state->receivers_connection, CHALLENGE_RESPONSE_FORMAT_STRING, info->player->name, args->set);
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
        sendCommandError(info, INVALID_CHALLENGE_RESPONSE_FORMAT, state->n);
    }
    return ret;
}

void freeChallengeResponseArgs(void* args)
{
    free(args);
}

Command* createChallengeResponseCommand()
{
    Command* ret = malloc(sizeof(Command));
    ret->command_name = malloc(sizeof(char) * (strlen(CHALLENGE_RESPONSE_COMMAND) + 1));
    ret->checkFormat = parseChallengeResponseCommand;
    ret->cleanUp = freeChallengeResponseArgs;
    ret->execute = executeChallengeResponseCommand;
    sprintf(ret->command_name, CHALLENGE_RESPONSE_COMMAND);
    return ret;
}
