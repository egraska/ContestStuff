#ifndef _ACTIVE_CONNECITON_H
#define _ACTIVE_CONNECITON_H

#include "custom_string.h"
#include "commands.h"
#include "player_database.h"

#define ACTIVE_COULD_NOT_CONNECT       -1
#define ACTIVE_COULD_NOT_VALIDATE      -2
#define ACTIVE_CONNECTION_ERROR        -3
#define ACTIVE_SUCCESS                  0

typedef struct
{
    Player* player;
    Command** commandsRequired;
    String** message;
    StateBaseArg** states;
    int connection_duration;
    int number_messages;
    int number_commands;
} ActiveRequest;

void checkPlayersForLiving();
int checkForAlive(Player* player, HostPort* overrideLocation);
int sendActiveRequest(ActiveRequest* request, HostPort* location);

#endif
