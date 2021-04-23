#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "host_port_command.h"
#include "../util.h"
#include "../constants.h"
#include "../commands.h"
#include "../error_messages.h"
#include "../message_processing.h"
#include "../util.h"
#include "../active_connection.h"

void* parseHostPortCommand(ConnectionInfo* info, char** parameters)
{
    Host_port_args* ret = malloc(sizeof(Host_port_args));
    ret->host = getNextToken(parameters, SEPERATOR_STR);
    ret->port = getNextToken(parameters, SEPERATOR_STR);
    if(!ret->host || !ret->port)
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_HOST_PORT_FORMAT);
    }
    else if(getNextToken(parameters, SEPERATOR_STR))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, HOST_PORT_COMMAND);
    }
    else if(isBase10(ret->port))
    {
        int port = atoi(ret->port);
        if(port < MIN_PLAYER_PORT || port > MAX_PLAYER_PORT)
        {
            free(ret);
            ret = NULL;
            sendCommandError(info, INVALID_PORT_RANGE, MIN_PLAYER_PORT, MAX_PLAYER_PORT);
        }
    }
    else
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_PORT_FORMAT);
    }

    return ret;
}

int executeHostPortCommand(ConnectionInfo* info, void* args)
{
    int ret = 0;
    int active_result = 0;
    Host_port_args* host_port_args = (Host_port_args*)args;
    HostPort override;
    override.host = host_port_args->host;
    override.port = host_port_args->port;
    active_result = checkForAlive(info->player, &override);
    if(active_result == ACTIVE_SUCCESS)
    {
        ret = 1;
        addMessage(info, RESULTS_FORMAT_STRING, HOST_PORT_COMMAND, HOST_PORT_SUCCESS);
        setPlayerHostPort(info->player, host_port_args->host, host_port_args->port);
    }
    else if(active_result == ACTIVE_COULD_NOT_CONNECT)
    {
        sendCommandError(info, COULD_NOT_CONNECT_TO_SERVER,
            host_port_args->host, host_port_args->port);
    }
    else if(active_result == ACTIVE_COULD_NOT_VALIDATE ||
            active_result == ACTIVE_CONNECTION_ERROR)
    {
        sendCommandError(info, COULD_NOT_VALIDATE_SERVER,
            host_port_args->host, host_port_args->port);
    }
    return ret;
}

void freeHostPortArgs(void* args)
{
    free(args);
}

Command* createHostPortCommand()
{
    Command* ret = malloc(sizeof(Command));
    int commandSize = strlen(HOST_PORT_COMMAND) + 1;
    ret->command_name = malloc(sizeof(char) * commandSize);
    ret->checkFormat = parseHostPortCommand;
    ret->cleanUp = freeHostPortArgs;
    ret->execute = executeHostPortCommand;
    sprintf(ret->command_name, HOST_PORT_COMMAND);
    return ret;
}
