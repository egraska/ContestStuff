#include <stdlib.h>
#include <stdio.h>
#include "active_connection.h"
#include "repeated_events.h"
#include "message_processing.h"
#include "connection_info.h"
#include "player_functions.h"
#include "socket_util.h"
#include "commands.h"
#include "util.h"

void checkPlayersForLiving()
{
    int num_players = getNumberOfPlayers();
    Player** players = malloc(sizeof(Player*) * num_players);
    num_players = getPlayerList(players, num_players);
    shuffleArrayPointers((void**)players, num_players);
    for(int i = 0; i < num_players; i++)
    {
        if(checkForAlive(players[i], NULL) == ACTIVE_SUCCESS)
        {
            fprintf(stdout, "%s: %s is currently alive\n",
                CHECK_FOR_LIVING, players[i]->name);
        }
        else
        {
            fprintf(stdout, "%s: %s is currently not alive\n",
                CHECK_FOR_LIVING, players[i]->name);
        }
    }
    free(players);
}

int checkForAlive(Player* player, HostPort* overrideLocation)
{
    int ret = ACTIVE_COULD_NOT_CONNECT;
    if(overrideLocation || getPlayerAliveTime(player))
    {
        ActiveRequest* request = malloc(sizeof(ActiveRequest));
        request->player = player;
        request->commandsRequired = NULL;
        request->states = NULL;
        request->message = NULL;
        request->connection_duration = getAliveCheckTimeout();
        request->number_messages = 0;
        request->number_commands = 0;
        ret = sendActiveRequest(request, overrideLocation);
        free(request);
    }
    return ret;
}

int sendActiveRequest(ActiveRequest* request, HostPort* location)
{
    int s = 0;
    int ret = ACTIVE_COULD_NOT_CONNECT;
    if(location)
    {
        s = openConnection(location);
    }
    else
    {

        HostPort* hostPort = getPlayerHostPortCopy(request->player);
        if(hostPort)
        {
            s = openConnection(hostPort);
            freeHostPortCopy(hostPort);
        }
    }
    if(s > 0)
    {
        //We have connected
        ConnectionInfo* info = initConnectionInfo(s,
            request->connection_duration);
        info->player = request->player;
        sendGreetingtoPassiveServer(info);
        if(performRequire(info, getIdentCommand()) > 0 &&
           info->player == request->player &&
           performRequire(info, getAliveCommand()) > 0)
        {
            ret = ACTIVE_SUCCESS;
            for(int i = 0; i < request->number_messages; i++)
            {
                addMessage(info, stringGetBuffer(request->message[i]));
            }
            if(request->commandsRequired)
            {
                if(performRequireWithMultiple(info,
                                           request->commandsRequired,
                                           request->states,
                                           request->number_commands) != COMMAND_SUCCESS)
                {
                    ret = ACTIVE_CONNECTION_ERROR;
                }
            }
        }
        else
        {
            ret = ACTIVE_COULD_NOT_VALIDATE;
        }
        sendMessageBuffer(info);
        deinitConnectionInfo(info);
    }

    // If we are trying to connect to them and we fail.
    // They are no longer alive (only if we cannot connect to them, or if the alive check fails)
    // They are still alive even if they do not give the required commands (except ident and alive)
    if(ret == ACTIVE_COULD_NOT_CONNECT || ret == ACTIVE_COULD_NOT_VALIDATE)
    {
        signOffPlayer(request->player);
    }

    return ret;
}
