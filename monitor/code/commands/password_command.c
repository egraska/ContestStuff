#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "password_command.h"
#include "../util.h"
#include "../constants.h"
#include "../commands.h"
#include "../error_messages.h"
#include "../message_processing.h"

void* ParsePasswordCommand(ConnectionInfo* info, char** parameters)
{
    Password_args* ret = malloc(sizeof(Password_args));
    ret->password = getNextToken(parameters, SEPERATOR_STR);
    if(!ret->password)
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_PASSWORD_FORMAT);
    }
    else if(getNextToken(parameters, SEPERATOR_STR))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, PASSWORD_COMMAND);
    }
    return ret;
}

int executePasswordCommand(ConnectionInfo* info, void* args)
{
    Password_args* password_args = (Password_args*)args;
    int ret;
    char* cookie = malloc((sizeof(char) * (getCookieSize() + 1)));
    getRandomCookie(cookie);
    if(handleReceivedPassword(info->player, password_args->password, cookie))
    {
        addMessage(info, RESULTS_FORMAT_STRING, PASSWORD_COMMAND, cookie);
        ret = COMMAND_SUCCESS;
        strcpy(info->session_cookie, cookie);
    }
    else
    {
        sendCommandError(info, INVALID_PASSWORD);
        ret = COMMAND_FAILURE;
    }
    free(cookie);
    return ret;
}

void freePasswordArgs(void* args)
{
    free(args);
}

Command* createPasswordCommand()
{
    Command* ret = malloc(sizeof(Command));//freed from deinitCommand
    int commandSize = strlen(PASSWORD_COMMAND) + 1;
    ret->command_name = malloc(sizeof(char) * commandSize);//freed from deinitPasswordCommand
    ret->checkFormat = ParsePasswordCommand;
    ret->cleanUp = freePasswordArgs;
    ret->execute = executePasswordCommand;
    sprintf(ret->command_name, PASSWORD_COMMAND);
    return ret;
}
