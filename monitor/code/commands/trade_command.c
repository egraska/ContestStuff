#include <stdio.h>
#include <string.h>
#include "trade_command.h"
#include "trade_response_command.h"
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
#include "../socket_util.h"

#define NUMBER_REQUIRED_OPTIONS             2

void freeTradeArgs(void* args)
{
    Trade_args* trade_args = (Trade_args*)args;
    if(trade_args->offered)
    {
        deinitResourceTransfer(trade_args->offered);
    }
    if (trade_args->requested)
    {
        deinitResourceTransfer(trade_args->requested);
    }
    free(args);
}

void* parseTradeCommand(ConnectionInfo* info, char** parameters)
{
    Trade_args* args = malloc(sizeof(Trade_args));

    args->identity_offered = getNextToken(parameters, SEPERATOR_STR);

    args->offered = initResouceTransferFromString(
        getNextToken(parameters, TRANSFER_SEPERATOR_STR));

    args->identity_requested = getNextToken(parameters, SEPERATOR_STR);

    args->requested = initResouceTransferFromString(
        getNextToken(parameters, NEWLINE_STR));

    if(!args->identity_offered || !args->identity_requested)
    {
        free(args);
        args = NULL;
        sendCommandError(info, INVALID_TRADE_FORMAT);
    }
    else if(!args->offered)
    {
        freeTradeArgs(args);
        args = NULL;
        sendCommandError(info, INVALID_TRADE_FORMAT);
    }
    else if(!args->requested)
    {
        freeTradeArgs(args);
        args = NULL;
        sendCommandError(info, INVALID_TRADE_FORMAT);
    }

    return args;
}

int sendTradeRequest(ConnectionInfo* info,
                     Trade_args* args,
                     Player* other_player)
{
    int ret = TRADE_NOT_ALIVE;
    String* public_key = initString();
    StateBaseArg state;
    state.state = malloc(sizeof(Public_Key_State));
    ((Public_Key_State*)state.state)->public_key = public_key;

    String* messages[2];
    messages[0] = initString();
    messages[1] = initString();
    stringAppend(messages[0], TRADE_FORMAT_STRING, args->identity_offered);
    appendTransferToString(args->offered, messages[0]);
    stringAppend(messages[0], TRANSFER_SEPERATOR_STR);
    stringAppend(messages[0], args->identity_requested);
    appendTransferToString(args->requested, messages[0]);

    int public_key_success = 0;
    int use_auth = getTradeAuthState();
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
            return TRADE_SELF_NOT_ALIVE;
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

        responseState.response_command = TRADE_RESPONSE;

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
        commands[0] = getTradeResponseCommand();

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
        request.connection_duration = getTradeResponseTimeout();
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

int executeTradeCommand(ConnectionInfo* info, void* in_args)
{
    Trade_args* args = (Trade_args*)in_args;
    int ret = COMMAND_FAILURE;
    int accepted = 0;
    //confirm player gave themselves as ident
    if(strcmp(args->identity_offered, info->player->name))
    {
        sendCommandError(info, TRADE_NOT_SELF_IDENTITY);
    }
    else
    {
        //confirm and lock the players resources
        ResourceWallet* players_wallet = getPlayerWallet(info->player);
        if(commitResourcesForTransfer(players_wallet, args->offered))
        {
            //confirm player gave real player
            Player* other_player = checkIfPlayerExists(args->identity_requested);
            if((other_player && other_player != info->player))
            {
                ResourceWallet* other_wallet = getPlayerWallet(other_player);
                ret = COMMAND_SUCCESS;
                //make sure the player currently has resources (do not lock)
                if(hasResources(other_wallet, args->requested))
                {
                    //we are good to make our request
                    accepted = sendTradeRequest(info, args, other_player);
                    if(accepted == TRADE_ACCEPTED && !attemptAtmoicTrade(other_wallet, args->offered, args->requested))
                    {
                        //we attempted the trade but it failed
                        accepted = TRADE_DECLINED;
                    }
                }
                else
                {
                    //We are not telling the player why the trade was declined
                    accepted = TRADE_DECLINED;
                }
            }
            //check if it is a trade with the monitor
            else if(!strcmp(args->identity_requested, IDENT_MONITOR))
            {
                ret = COMMAND_SUCCESS;
                accepted = TRADE_DECLINED;
                ResourceWallet* monitor_wallet = getMonitorWallet();
                long offered_value = getValueOfTransfer(args->offered);
                long requested_value = getValueOfTransfer(args->requested);
                if(offered_value >= requested_value * getMonitorTaxValue() &&
                    attemptAtmoicTrade(monitor_wallet, args->offered, args->requested))
                {
                    accepted = TRADE_ACCEPTED;
                }
            }
            else
            {
                sendCommandError(info, OTHER_PLAYER_DOES_NOT_EXIST,
                    args->identity_requested);
            }
            finalizeTrade(players_wallet,
                accepted == TRADE_ACCEPTED ? args->requested : args->offered,
                args->offered);
            if(accepted == TRADE_ACCEPTED)
            {
                updateMarketValue(args->requested, args->offered);
            }
        }
        else
        {
            sendCommandError(info, OFFER_INSUFFICIENT_RESOURCES);
        }
    }
    if(ret == COMMAND_SUCCESS)
    {
        if(accepted == TRADE_ACCEPTED)
        {
            if(!strcmp(args->identity_requested, IDENT_MONITOR))
            {
                logMonitorTrade(info->player);
            }
            else
            {
                Player* other_player = checkIfPlayerExists(args->identity_requested);
                logPlayerTrade(info->player, other_player);
            }
            addMessage(info, RESULTS_FORMAT_STRING, TRADE_REQUEST_COMMAND, ACCEPTED_STRING);
        }
        else if(accepted == TRADE_DECLINED)
        {
            addMessage(info, RESULTS_FORMAT_STRING, TRADE_REQUEST_COMMAND, DECLINED_STRING);
        }
        else if(accepted == TRADE_NOT_ALIVE)
        {
            addMessage(info, RESULTS_FORMAT_STRING, TRADE_REQUEST_COMMAND, NOT_ALIVE_STRING);
        }
        else
        {
            addMessage(info, RESULTS_FORMAT_STRING, TRADE_REQUEST_COMMAND, SELF_NOT_ALIVE_STRING);
        }
    }
    return ret;
}

Command* createTradeCommand()
{
    Command* ret = malloc(sizeof(Command));
    ret->command_name = malloc(sizeof(char) * (strlen(TRADE_REQUEST_COMMAND) + 1));
    ret->checkFormat = parseTradeCommand;
    ret->cleanUp = freeTradeArgs;
    ret->execute = executeTradeCommand;
    sprintf(ret->command_name, TRADE_REQUEST_COMMAND);
    return ret;
}
