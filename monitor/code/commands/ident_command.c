#include <string.h>
#include <stdio.h>
#include "ident_command.h"
#include "../connection_info.h"
#include "../constants.h"
#include "../util.h"
#include "../error_messages.h"
#include "../custom_string.h"
#include "../dh_engine.h"
#include "../message_processing.h"

void* parseIdentCommand(ConnectionInfo* info, char** parameters)
{
    Ident_args* ret = malloc(sizeof(Ident_args));
    ret->idenity = getNextToken(parameters, SEPERATOR_STR);
    ret->client_dh_pk = getNextToken(parameters, SEPERATOR_STR);
    if(!ret->idenity)
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_IDENT_FORMAT);

    }
    else if(ret->client_dh_pk && !isBase32(ret->client_dh_pk))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_DH_KEY_FORMAT);
    }
    else if(getNextToken(parameters, SEPERATOR_STR))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, IDENT_COMMAND);
    }
    return ret;
}

int executeIdentCommand(ConnectionInfo* info, void* args)
{
    Ident_args* ident_args = (Ident_args*)args;
    if(!strcmp(IDENT_MONITOR, ident_args->idenity))
    {
        sendCommandError(info, RESERVED_IDENT, ident_args->idenity);
        return COMMAND_FAILURE;
    }
    info->player = getPlayerByName(ident_args->idenity);
    if(!info->player)
    {
        sendCommandError(info, PLAYER_NOT_FOUND, ident_args->idenity);
        return COMMAND_FAILURE;
    }

    if(ident_args->client_dh_pk)
    {
        int monitor_dh_private_key_len_bits = getMonDHKeyPrivateKeyLen();
        int monitor_dh_p_len_bits = getMonDHKeyPublicKeyLen();
        int karn_key_size_byte = getKarnKeySizeBytes();
        //step 1: generate Private Key
        char* private_key = malloc(sizeof(char) * (monitor_dh_private_key_len_bits / BASE_RADIX_EXPONENT + 1));
        generateRandomKey(private_key, monitor_dh_private_key_len_bits / BASE_RADIX_EXPONENT);
        //step 2: generate public key
        char* public_key = malloc(sizeof(char) * (monitor_dh_p_len_bits / BASE_RADIX_EXPONENT + 1));
        calculateDHPublicKey(public_key, private_key);
        //step 3: share the public key
        addMessage(info, RESULTS_FORMAT_STRING, IDENT_COMMAND, public_key);
        //step 4: Calculate shared secret
        info->sharedSecret = malloc(sizeof(unsigned char) * karn_key_size_byte);

        memset(info->sharedSecret, 0, karn_key_size_byte);
        calculateDHSharedSecret(info->sharedSecret, private_key, ident_args->client_dh_pk);
        //step 5: release resources
        free(private_key);
        free(public_key);
    }
    return COMMAND_SUCCESS;
}

void freeIdentArgs(void* args)
{
    free(args);
}

Command* createIdentCommand()
{
    Command* ret = malloc(sizeof(Command));
    ret->command_name = malloc(sizeof(char) * (strlen(IDENT_COMMAND) + 1));
    ret->checkFormat = parseIdentCommand;
    ret->cleanUp = freeIdentArgs;
    ret->execute = executeIdentCommand;
    sprintf(ret->command_name, IDENT_COMMAND);
    return ret;
}
