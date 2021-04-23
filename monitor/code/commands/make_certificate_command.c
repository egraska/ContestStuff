#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "make_certificate_command.h"
#include "../connection_info.h"
#include "../constants.h"
#include "../util.h"
#include "../error_messages.h"
#include "../custom_string.h"
#include "../rsa_engine.h"
#include "../message_processing.h"
#include "../player_database.h"

void* parseMakeCertCommand(ConnectionInfo* info, char** parameters)
{
    Make_cert_args* ret = malloc(sizeof(Make_cert_args));
    ret->exp = getNextToken(parameters, SEPERATOR_STR);
    ret->mod = getNextToken(parameters, SEPERATOR_STR);
    if(!ret->exp || !ret->mod)
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_MAKE_CERTIFICATE_FORMAT);

    }
    else if(!isBase32(ret->exp) && !isBase32(ret->mod))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_RSA_KEY_FORMAT);
    }
    else if(getNextToken(parameters, SEPERATOR_STR))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, MAKE_CERTIFICATE_COMMAND);
    }
    return ret;
}

int executeMakeCertCommand(ConnectionInfo* info, void* args)
{
    Make_cert_args* make_cert_args = (Make_cert_args*)args;

    char* exp = malloc(sizeof(char) * (strlen(make_cert_args->exp) + 1));
    char* mod = malloc(sizeof(char) * (strlen(make_cert_args->mod) + 1));

    strcpy(exp, make_cert_args->exp);
    strcpy(mod, make_cert_args->mod);

    unsigned char hashed_cert[HASH_LENGTH_HEX];
    SHA_CTX sha;
    SHA1_Init(&sha);
    SHA1_Update(&sha, exp, strlen(exp));
    SHA1_Update(&sha, mod, strlen(mod));
    SHA1_Final(hashed_cert, &sha);
    byteArrayToString(hashed_cert, SHA_DIGEST_LENGTH, BASE_HEX);

    char* signed_cert = getMonitorSignedHash(hashed_cert);
    addRSAKey(info->player, signed_cert, exp, mod);

    String* result = initString();
    stringAppend(result, "%s %s", info->player->name, signed_cert);

    addMessage(info, RESULTS_FORMAT_STRING, MAKE_CERTIFICATE_COMMAND, stringGetBuffer(result));
    deinitString(result);
    return COMMAND_SUCCESS;
}

void freeMakeCertArgs(void* args)
{
    free(args);
}

Command* createMakeCertCommand()
{
    Command* ret = malloc(sizeof(Command));
    int commandSize = strlen(MAKE_CERTIFICATE_COMMAND) + 1;
    ret->command_name = malloc(sizeof(char) * commandSize);
    ret->checkFormat = parseMakeCertCommand;
    ret->cleanUp = freeMakeCertArgs;
    ret->execute = executeMakeCertCommand;
    sprintf(ret->command_name, MAKE_CERTIFICATE_COMMAND);
    return ret;
}
