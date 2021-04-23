#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "change_password_command.h"
#include "../util.h"
#include "../constants.h"
#include "../commands.h"
#include "../error_messages.h"
#include "../message_processing.h"

void* parseChangePasswordCommand(ConnectionInfo* info, char** parameters)
{
    ChangePassword_args* ret = malloc(sizeof(ChangePassword_args));
    ret->old_password = getNextToken(parameters, SEPERATOR_STR);
    ret->new_password = getNextToken(parameters, SEPERATOR_STR);
    if(!ret->old_password || !ret->new_password)
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_CHANGE_PASSWORD_FORMAT);
    }
    else if(getNextToken(parameters, SEPERATOR_STR))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, CHANGE_PASSWORD_COMMAND);
    }
    return ret;
}

int executeChangePasswordCommand(ConnectionInfo* info, void* args)
{
    ChangePassword_args* change_password_args = (ChangePassword_args*)args;
    int ret;
    char* cookie = malloc((sizeof(char) * (getCookieSize() + 1)));
    getRandomCookie(cookie);
    if(changePassword(info->player, change_password_args->old_password, change_password_args->new_password, cookie)){
        addMessage(info, RESULTS_FORMAT_STRING, CHANGE_PASSWORD_COMMAND, cookie);
        ret = COMMAND_SUCCESS;
    }
    else{
        sendCommandError(info, INVALID_PASSWORD);
        ret = COMMAND_FAILURE;
    }
    free(cookie);
    return ret;
}

void freeChangePasswordArgs(void* args)
{
    free(args);
}


Command* createChangePasswordCommand()
{
    Command* ret = malloc(sizeof(Command));
    int commandSize = strlen(CHANGE_PASSWORD_COMMAND) + 1;
    ret->command_name = malloc(sizeof(char) * commandSize);
    ret->checkFormat = parseChangePasswordCommand;
    ret->cleanUp = freeChangePasswordArgs;
    ret->execute = executeChangePasswordCommand;
    sprintf(ret->command_name, CHANGE_PASSWORD_COMMAND);
    return ret;
}
