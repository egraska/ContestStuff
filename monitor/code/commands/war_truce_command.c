#include <stdio.h>
#include <string.h>
#include "war_truce_command.h"
#include "war_truce_response_command.h"
#include "rounds_command.h"
#include "public_key_command.h"
#include "challenge_command.h"
#include "../player_database.h"
#include "../constants.h"
#include "../util.h"
#include "../constants.h"
#include "../commands.h"
#include "../error_messages.h"
#include "../message_processing.h"
#include "../resources.h"
#include "../active_connection.h"
#include "../war_database.h"
#include "../socket_util.h"

#define NUMBER_REQUIRED_OPTIONS             2

void freeWarTruceArgs(void* args)
{
    Truce_args* truce_args = (Truce_args*)args;
    if(truce_args->offered)
    {
        deinitResourceTransfer(truce_args->offered);
    }
    free(args);
}

void* parseWarTruceCommand(ConnectionInfo* info, char** parameters)
{
    Truce_args* args = malloc(sizeof(Truce_args));

    args->identity_offered = getNextToken(parameters, TRUCE_PLAYER_SEPERATOR_STR);

    args->identity_offered_to = getNextToken(parameters, SEPERATOR_STR);

    args->offered = initResouceTransferFromString(
        getNextToken(parameters, NEWLINE_STR));

    if(!args->identity_offered || !args->identity_offered_to || !args->offered)
    {
        freeWarTruceArgs(args);
        args = NULL;
        sendCommandError(info, INVALID_WAR_TRUCE_FORMAT);
    }
    else if(getNextToken(parameters, SEPERATOR_STR))
    {
        freeWarTruceArgs(args);
        args = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, WAR_TRUCE_OFFER_COMMAND);
    }
    return args;
}

int sendWarTruceRequest(ConnectionInfo* info,
                     Truce_args* args,
                     Player* other_player)
{
    int ret = TRUCE_NOT_ALIVE;
    String* public_key = initString();
    StateBaseArg state;
    state.state = malloc(sizeof(Public_Key_State));
    ((Public_Key_State*)state.state)->public_key = public_key;

    String* messages[2];
    messages[0] = initString();
    messages[1] = initString();
    stringAppend(messages[0], TRUCE_FORMAT_STRING, args->identity_offered);
    stringAppend(messages[0], TRUCE_PLAYER_SEPERATOR_STR);
    stringAppend(messages[0], args->identity_offered_to);
    appendTransferToString(args->offered, messages[0]);

    int public_key_success = 0;
    int use_auth = USE_AUTH_FOR_TRUCE;
    int public_key_provided = 1;
    ConnectionInfo* info_passive;
    if(use_auth)
    {
        HostPort* hostPort = getPlayerHostPortCopy(info->player);

        int s = 0;
        if(hostPort)
        {
            s = openConnection(hostPort);
            freeHostPortCopy(hostPort);
        }
        if(s > 0)
        {
            //We have connected
            info_passive = initConnectionInfo(s, getPassiveServerAuthTimeOut());
            info_passive->player = info->player;
        }
        else
        {
            //Player's own passive server is not alive for authentication.
            free(state.state);
            return TRUCE_SELF_NOT_ALIVE;
        }
        if(performRequireWithState(info_passive, getPublicKeyCommand(), &state) ==
        COMMAND_SUCCESS)
        {
            public_key_success = 1;
        }
    }
    if(public_key_success || !use_auth)
    {
        if(use_auth)
        {
            stringAppend(messages[1],
                PUBLIC_KEY_FORMAT_STRING, stringGetBuffer(public_key));
        }

        Response_State responseState;
        Authentication_State authenticationState;

        responseState.response_command = TRUCE_RESPONSE;

        authenticationState.requestors_connection = info;
        authenticationState.requestors_passive_server = info_passive;
        authenticationState.response_state = &responseState;

        StateBaseArg** states = malloc(sizeof(StateBaseArg*) *
                                       NUMBER_REQUIRED_OPTIONS);
        states[0] = malloc(sizeof(StateBaseArg));
        states[1] = malloc(sizeof(StateBaseArg));
        states[0]->state = &responseState;
        states[1]->state = &authenticationState;

        Command* commands[NUMBER_REQUIRED_OPTIONS];
        commands[0] = getWarTruceResponseCommand();

        if(use_auth)
        {
            if(!strncmp(stringGetBuffer(public_key), KEY_TYPE_RSA, strlen(KEY_TYPE_RSA)))
            {
                commands[1] = getChallengeCommand();
            }
            else if(!strncmp(stringGetBuffer(public_key), KEY_TYPE_ZKP, strlen(KEY_TYPE_ZKP)))
            {
                commands[1] = getRoundsCommand();
            }
            else
            {
                public_key_provided = 0;
            }
        }

        ActiveRequest request;
        request.player = other_player;
        request.commandsRequired = commands;
        request.states = states;
        request.connection_duration = getTruceResponseTimeout();
        request.message = messages;
        if(use_auth)
        {
            request.number_messages = 2;
            if(public_key_provided)
            {
                request.number_commands = 2;
            }
            else
            {
                request.number_commands = 1;
            }
        }
        else
        {
            request.number_messages = 1;
            request.number_commands = 1;
        }

        if(sendActiveRequest(&request, NULL) == ACTIVE_SUCCESS)
        {
            ret = responseState.accepted;
        }

        free(states[0]);
        free(states[1]);
        free(states);
    }
    if(use_auth)
    {
        deinitConnectionInfo(info_passive);
    }
    deinitString(messages[0]);
    deinitString(messages[1]);
    deinitString(public_key);
    free(state.state);
    return ret;
}

int executeWarTruceCommand(ConnectionInfo* info, void* in_args)
{
    Truce_args* args = (Truce_args*)in_args;
    int ret = COMMAND_FAILURE;
    int active_war = 0;
    int war_status = 1;
    int accepted = 0;
    int truce_result = 0;
    War* war = NULL;
    //confirm player gave themselves as ident
    if(strcmp(args->identity_offered, info->player->name))
    {
        sendCommandError(info, TRUCE_NOT_SELF_IDENTITY);
    }
    else
    {
        //confirm and lock the players resources
        ResourceWallet* players_wallet = getPlayerWallet(info->player);
        if(commitResourcesForTransfer(players_wallet, args->offered))
        {
            //confirm player gave real player
            Player* other_player = checkIfPlayerExists(args->identity_offered_to);
            if((other_player && other_player != info->player))
            {
                //confirm if war exists between players
                war = getPlayersWar(info->player, other_player, 0);
                if(war)
                {
                    ResourceWallet* other_wallet = getPlayerWallet(other_player);
                    pthread_rwlock_rdlock(war->lock);
                    war_status = war->status;
                    if(war_status == WAR_ACTIVE)
                    {
                        ret = COMMAND_SUCCESS;
                        active_war = 1;
                    }
                    pthread_rwlock_unlock(war->lock);
                    if(ret)
                    {
                        accepted = sendWarTruceRequest(info, args, other_player);
                        if(accepted != TRUCE_NOT_ALIVE)
                        {
                            logPlayerTruceOffered(info->player);
                        }
                        if(accepted == TRUCE_ACCEPTED)
                        {
                            logPlayerTruceAccepted(other_player);
                            int truce_sender = getPlayerSerialNo(info->player);
                            int truce_receiver = getPlayerSerialNo(other_player);
                            truce_result = attemptTruce(other_wallet, args->offered, war, truce_sender, truce_receiver);
                            if(!truce_result)
                            {
                                active_war = 0;
                            }
                        }
                        else if(accepted == TRUCE_NOT_ALIVE)
                        {
                            signOffPlayer(other_player);
                        }
                    }
                }
                //If war gets over before truce is made, or if no active war, then send no active war error.
                if(!active_war)
                {
                    accepted = TRUCE_DECLINED;
                    sendCommandError(info, NO_ACTIVE_WARS_WITH_PLAYER,
                        args->identity_offered_to);
                }
            }
            else
            {
                sendCommandError(info, OTHER_PLAYER_DOES_NOT_EXIST,
                    args->identity_offered_to);
            }
            finalizeResourceTransferForTruce(players_wallet, args->offered, accepted);
        }
        else
        {
            sendCommandError(info, OFFER_INSUFFICIENT_RESOURCES);
        }
    }
    if(ret == COMMAND_SUCCESS && active_war)
    {
        if(accepted == TRUCE_ACCEPTED)
        {
            addMessage(info, RESULTS_FORMAT_STRING, WAR_TRUCE_OFFER_COMMAND, ACCEPTED_STRING);
        }
        else if(accepted == TRUCE_DECLINED)
        {
            addMessage(info, RESULTS_FORMAT_STRING, WAR_TRUCE_OFFER_COMMAND, DECLINED_STRING);
        }
        else if(accepted == TRUCE_NOT_ALIVE)
        {
            addMessage(info, RESULTS_FORMAT_STRING, WAR_TRUCE_OFFER_COMMAND, NOT_ALIVE_STRING);
        }
        else
        {
            addMessage(info, RESULTS_FORMAT_STRING, WAR_TRUCE_OFFER_COMMAND, SELF_NOT_ALIVE_STRING);
        }
    }
    return ret;
}

Command* createWarTruceCommand()
{
    Command* ret = malloc(sizeof(Command));
    ret->command_name = malloc(sizeof(char) * (strlen(WAR_TRUCE_OFFER_COMMAND) + 1));
    ret->checkFormat = parseWarTruceCommand;
    ret->cleanUp = freeWarTruceArgs;
    ret->execute = executeWarTruceCommand;
    sprintf(ret->command_name, WAR_TRUCE_OFFER_COMMAND);
    return ret;
}
