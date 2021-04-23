#include <string.h>
#include <stdio.h>
#include "challenge_command.h"
#include "challenge_response_command.h"
#include "../connection_info.h"
#include "../constants.h"
#include "../util.h"
#include "../error_messages.h"
#include "../custom_string.h"
#include "../message_processing.h"

void* parseChallengeCommand(ConnectionInfo* info, char** parameters)
{
    Challenge_args* ret = NULL;
    char *challenge_str = getNextToken(parameters, SEPERATOR_STR);

    if(!challenge_str || !isBase32(challenge_str))
    {
        sendCommandError(info, INVALID_CHALLENGE_FORMAT);
    }
    else if(getNextToken(parameters, SEPERATOR_STR))
    {
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, CHALLENGE_COMMAND);
    }
    else
    {
        ret = malloc(sizeof(Challenge_args));
        ret->challenge = challenge_str;
    }
    return ret;
}

int executeChallengeCommand(ConnectionInfo* info, void* in_args)
{
    StateBaseArg* state_args = (StateBaseArg*)in_args;
    Challenge_args* args = (Challenge_args*)state_args->userArgs;
    Authentication_State* state = (Authentication_State*)state_args->state;
    addMessage(state->requestors_passive_server, CHALLENGE_FORMAT_STRING, args->challenge);

    StateBaseArg next_state;
    Challenge_Response_State challenge_response_state;
    challenge_response_state.n = 1;
    challenge_response_state.receivers_connection = info;
    challenge_response_state.response_state = state->response_state;

    next_state.state = &challenge_response_state;

    return performRequireWithState(state->requestors_passive_server, getChallengeResponseCommand(), &next_state);
}

void freeChallengeArgs(void* args)
{
    free(args);
}

Command* createChallengeCommand()
{
    Command* ret = malloc(sizeof(Command));
    ret->command_name = malloc(sizeof(char) * (strlen(CHALLENGE_COMMAND) + 1));
    ret->checkFormat = parseChallengeCommand;
    ret->cleanUp = freeChallengeArgs;
    ret->execute = executeChallengeCommand;
    sprintf(ret->command_name, CHALLENGE_COMMAND);
    return ret;
}
