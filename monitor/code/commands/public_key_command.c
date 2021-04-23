#include <string.h>
#include <stdio.h>
#include "public_key_command.h"
#include "../connection_info.h"
#include "../constants.h"
#include "../util.h"
#include "../error_messages.h"
#include "../custom_string.h"
#include "../message_processing.h"
#include "../rsa_engine.h"

void* parsePublicKeyCommand(ConnectionInfo* info, char** parameters)
{
    Public_Key_args* ret = malloc(sizeof(Public_Key_args));
    ret->auth_type = getNextToken(parameters, SEPERATOR_STR);
    ret->exp = getNextToken(parameters, SEPERATOR_STR);
    ret->mod = getNextToken(parameters, SEPERATOR_STR);
    if(!ret->auth_type)
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_PUBLIC_KEY_FORMAT);
    }
    else if(!ret->exp || !ret->mod)
    {
        if(strcmp(ret->auth_type, NO_KEY))
        {
            free(ret);
            ret = NULL;
            sendCommandError(info, INVALID_PUBLIC_KEY_FORMAT);
        }
    }
    else if(strcmp(ret->auth_type, KEY_TYPE_RSA) && strcmp(ret->auth_type, KEY_TYPE_ZKP) && strcmp(ret->auth_type, NO_KEY))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_PUBLIC_KEY_FORMAT);
    }
    else if(strcmp(ret->auth_type, NO_KEY))
    {
        if(!isBase32(ret->exp) || !isBase32(ret->mod))
        {
            free(ret);
            ret = NULL;
            sendCommandError(info, INVALID_RSA_KEY_FORMAT);
        }
    }
    else if(getNextToken(parameters, SEPERATOR_STR))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, PUBLIC_KEY_COMMAND);
    }
    return ret;
}

int executePublicKeyCommand(ConnectionInfo* info, void* in_args)
{
    StateBaseArg* state_args = (StateBaseArg*)in_args;
    Public_Key_args* args = (Public_Key_args*)state_args->userArgs;
    Public_Key_State* state = (Public_Key_State*)state_args->state;

    if(strcmp(args->auth_type, NO_KEY))
    {
        char* exp = malloc(sizeof(char) * (strlen(args->exp) + 1));
        char* mod = malloc(sizeof(char) * (strlen(args->mod) + 1));

        strcpy(exp, args->exp);
        strcpy(mod, args->mod);

        unsigned char hashed_cert[HASH_LENGTH_HEX];
        SHA_CTX sha;
        SHA1_Init(&sha);
        SHA1_Update(&sha, exp, strlen(exp));
        SHA1_Update(&sha, mod, strlen(mod));
        SHA1_Final(hashed_cert, &sha);
        byteArrayToString(hashed_cert, SHA_DIGEST_LENGTH, BASE_HEX);

        char* signed_cert = getMonitorSignedHash(hashed_cert);

        stringAppend(state->public_key,"%s %s %s %s", args->auth_type, args->exp, args->mod, signed_cert);

    free(exp);
    free(mod);
    }
    else
    {
        stringAppend(state->public_key,"%s", args->auth_type);
    }

    return COMMAND_SUCCESS;
}

void freePublicKeyArgs(void* args)
{
    free(args);
}

Command* createPublicKeyCommand()
{
    Command* ret = malloc(sizeof(Command));
    ret->command_name = malloc(sizeof(char) * (strlen(PUBLIC_KEY_COMMAND) + 1));
    ret->checkFormat = parsePublicKeyCommand;
    ret->cleanUp = freePublicKeyArgs;
    ret->execute = executePublicKeyCommand;
    sprintf(ret->command_name, PUBLIC_KEY_COMMAND);
    return ret;
}
