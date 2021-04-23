#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>//for socket
#include <sys/socket.h>//for socket
#include <netinet/in.h>//for htonl and htons
#include <string.h>
#include <pthread.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <openssl/sha.h>
#include <gmp.h>
#include "game_parameters.h"
#include "karn.h"
#include "player_database.h"
#include "resources.h"
#include "message_processing.h"
#include "connection_info.h"
#include "constants.h"
#include "commands.h"
#include "active_connection.h"
#include "error_messages.h"
#include "util.h"

void sendRequireMessage(ConnectionInfo* info,
                        Command** requiredCommands, int numberCommands)
{
    String* str = initString();
    stringAppend(str, REQUIRE_FROMAT_STRING, requiredCommands[0]->command_name);
    for(int i = 1; i < numberCommands; i++)
    {
        stringAppend(str, COMMAND_OR_SEPERATOR,
            requiredCommands[i]->command_name);
    }
    addMessage(info, stringGetBuffer(str));
    deinitString(str);
}

void sendRequireError(ConnectionInfo* info, char* command_received,
                    Command** requiredCommands, int numberCommands)
{
    String* str = initString();
    stringAppend(str, REQUIRE_COMMAND_NOT_FOUND,
                command_received);
    for(int i = 1; i < numberCommands; i++)
    {
        stringAppend(str, COMMAND_OR_SEPERATOR,
            requiredCommands[i]->command_name);
    }
    sendCommandError(info, stringGetBuffer(str));
    deinitString(str);
}

int performRequire(ConnectionInfo* info, Command* requiredCommand)
{
    return performRequireWithState(info, requiredCommand, NULL);
}

int performRequireWithState(ConnectionInfo* info, Command* requiredCommand,
                            StateBaseArg* state)
{
    return performRequireWithMultiple(info, &requiredCommand, state ? (&state) : NULL, 1);
}

int performRequireWithMultiple(ConnectionInfo* info, Command** requiredCommands,
                            StateBaseArg** states, int numberCommands)
{
    int attempts = 0;
    char* received = NULL;
    void* args = NULL;
    char** stepPtr;
    while(attempts < getCommandErrorsAllowed())
    {
        attempts++;
        sendRequireMessage(info, requiredCommands, numberCommands);
        sendWaiting(info);
        received = readLine(info);
        stepPtr = &received;
        char* command_name = getNextToken(stepPtr, SEPERATOR_STR);
        int command_found = 0;
        if(command_name)
        {
            for(int i = 0; i < numberCommands; i++)
            {
                if(!strcmp(command_name, requiredCommands[i]->command_name))
                {
                    command_found = 1;
                    args = requiredCommands[i]->checkFormat(info, stepPtr);
                    if(args)
                    {
                        int success;
                        if(states && states[i])
                        {
                            states[i]->userArgs = args;
                            success = requiredCommands[i]->execute(info, states[i]);
                        }
                        else
                        {
                            success = requiredCommands[i]->execute(info, args);
                        }
                        requiredCommands[i]->cleanUp(args);
                        if(success)
                        {
                            return success;
                        }
                    }
                    break;
                }
            }
        }
        if(!command_name || !command_found)
        {
            sendRequireError(info, command_name,
                requiredCommands, numberCommands);
        }
    }
    sendCommandError(info, EXCESSIVE_FAILURES);
    sendMessageBuffer(info);
    return COMMAND_FAILURE;
}

int acceptUserCommand(ConnectionInfo* info)
{
    int attempts = 0;
    char* received = NULL;
    void* args = NULL;
    char** stepPtr;
    while(attempts < getCommandErrorsAllowed())
    {
        attempts++;
        if(START_SLEEP_TIME > 0)
        {
            sleep(START_SLEEP_TIME);
        }
        sendWaiting(info);
        received = readLine(info);
        stepPtr = &received;
        char* command_name = getNextToken(stepPtr, SEPERATOR_STR);
        Command* command = command_name ? getPlayerCommandByName(command_name) : NULL;
        if(command)
        {
            args = command->checkFormat(info, stepPtr);
            if(args)
            {
                //This needs to be atomic
                incPlayerCommandUsage(info->player);
                int success = command->execute(info, args);
                command->cleanUp(args);
                if(success)
                {
                    return success;
                }
            }
        }
        else
        {
            sendCommandError(info, COMMAND_NOT_FOUND, command_name);
        }
    }
    sendCommandError(info, EXCESSIVE_FAILURES);
    sendMessageBuffer(info);
    return COMMAND_FAILURE;
}

int getConnectedPlayer(ConnectionInfo* info)
{
    return performRequire(info, getIdentCommand()) > 0;
}

int getCredentials(ConnectionInfo* info)
{
    Command* secondCommand = NULL;
    //No need to lock it here. Just checking for existance
    if(!info->player->alive_start_time)
    {
        secondCommand = getPasswordCommand();
    }
    else
    {
        secondCommand = getAliveCommand();
    }
    return performRequire(info, secondCommand) > 0;
}

int validatePassiveServer(ConnectionInfo* info)
{
    int ret = 1;
    if(checkForAlive(info->player, NULL) != ACTIVE_SUCCESS)
    {
        ret = performRequire(info, getHostPortCommand()) > 0;
        sendMessageBuffer(info);
    }
    else
    {
        sendMessageBuffer(info);
    }
    return ret;
}

void *handleIncommingConnection(void *socket_desc)
{
    ConnectionInfo* info = initConnectionInfo(*(int*)socket_desc, getActiveConnectionTimeout());
    sendGreetingtoActiveClient(info);
    if(getConnectedPlayer(info) &&
       getCredentials(info) &&
       validatePassiveServer(info))
    {
        while(getPlayerCommandUsage(info->player) < getCommandsPerHour())
        {
            if(acceptUserCommand(info) <= 0)
            {
                sendMessageBuffer(info);
                break;
            }
        }
    }
    deinitConnectionInfo(info);
    return NULL;
}
