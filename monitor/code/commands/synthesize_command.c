#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "synthesize_command.h"
#include "../util.h"
#include "../constants.h"
#include "../commands.h"
#include "../error_messages.h"
#include "../message_processing.h"
#include "../util.h"
#include "../active_connection.h"

void* parseSynthesizeCommand(ConnectionInfo* info, char** parameters)
{
    Synthesize_args* ret = malloc(sizeof(Synthesize_args));
    char* finished_product = getNextToken(parameters, SEPERATOR_STR);
    char* amount = getNextToken(parameters, SEPERATOR_STR);
    char* proof_of_work = getNextToken(parameters, SEPERATOR_STR);

    int required_format = getProofOfWork();

    if(required_format == POW_NO)
    {
        if(!finished_product)
        {
            free(ret);
            ret = NULL;
            sendCommandError(info, INVALID_SYNTHESIZE_FORMAT_FLEX_LESS);
        }
        else if(!amount)
        {
            ret->command_format = POW_NO;
        }
        else if(!proof_of_work)
        {
            if(!isBase10(amount))
            {
                free(ret);
                ret = NULL;
                sendCommandError(info, RESOURCE_AMOUNT_NOT_A_NUMBER);
            }
            else
            {
                ret->amount = amount;
                ret->command_format = POW_NO_MANY_UNLIMIT;
            }
        }
        else if(getNextToken(parameters, SEPERATOR_STR))
        {
            free(ret);
            ret = NULL;
            sendCommandError(info, INVALID_SYNTHESIZE_FORMAT_FLEX_MORE);
        }
        else
        {
            if(!isBase10(amount))
            {
                free(ret);
                ret = NULL;
                sendCommandError(info, RESOURCE_AMOUNT_NOT_A_NUMBER);
            }
            else
            {
                ret->amount = amount;
                ret->command_format = POW_NO_MANY_UNLIMIT;
            }
        }
    }
    else if(required_format == POW_YES)
    {
        if(!finished_product)
        {
            free(ret);
            ret = NULL;
            sendCommandError(info, INVALID_SYNTHESIZE_FORMAT_FLEX_LESS);
        }
        else if(!amount)
        {
            ret->command_format = POW_NO;
        }
        else if(!proof_of_work)
        {
            if(!isBase10(amount))
            {
                free(ret);
                ret = NULL;
                sendCommandError(info, RESOURCE_AMOUNT_NOT_A_NUMBER);
            }
            else
            {
                ret->amount = amount;
                ret->command_format = POW_NO_MANY_LIMIT;
            }
        }
        else if(getNextToken(parameters, SEPERATOR_STR))
        {
            free(ret);
            ret = NULL;
            sendCommandError(info, INVALID_SYNTHESIZE_FORMAT_FLEX_MORE);
        }
        else
        {
            if(!isBase10(amount))
            {
                free(ret);
                ret = NULL;
                sendCommandError(info, RESOURCE_AMOUNT_NOT_A_NUMBER);
            }
            else
            {
                ret->amount = amount;
                ret->command_format = POW_YES;
                ret->proof_of_work = proof_of_work;
            }
        }
    }
    if(ret)
    {
        ret->finished_product_index = getResourceIndex(finished_product);
        if(ret->finished_product_index == -1)
        {
            free(ret);
            ret = NULL;
            sendCommandError(info, INVALID_RESOURCE_NAME, finished_product);
        }
        else if(ret->finished_product_index < ASSEMBLED_RESOURCES_START)
        {
            free(ret);
            ret = NULL;
            sendCommandError(info, RESOURCE_CANNOT_BE_SYNTHESIZED, finished_product);
        }
    }
    return ret;
}

int executeSynthesizeCommand(ConnectionInfo* info, void* args)
{
    int ret = 1;
    Synthesize_args* synthesize_args = (Synthesize_args*)args;
    int resource_index = synthesize_args->finished_product_index;
    info->synthesize_counter++;
    if(synthesize_args->command_format == POW_YES)
    {
        unsigned char buffer[SHA_DIGEST_LENGTH];
        SHA_CTX sha;
        SHA1_Init(&sha);
        SHA1_Update(&sha, info->session_cookie, strlen(info->session_cookie));
        SHA1_Update(&sha, &info->synthesize_counter, sizeof(info->synthesize_counter));
        SHA1_Update(&sha, synthesize_args->proof_of_work, strlen(synthesize_args->proof_of_work));
        SHA1_Final(buffer, &sha);
        uint8_t* length = buffer + SHA_DIGEST_LENGTH;
        int leading_zero_counter = 0;
        uint8_t* buffer_bits = buffer;
        unsigned char mask = 0x80;
        while ((leading_zero_counter < atoi(synthesize_args->amount)) && (buffer_bits < length))
        {
            if(!(mask & *buffer))
            {
                leading_zero_counter++;
                mask>>=1;
                if(!mask)
                {
                    mask = 0x80;
                    buffer_bits++;
                }
            }
            else
            {
                break;
            }
        }
        if(atoi(synthesize_args->amount) <= 0)
        {
            sendCommandError(info, BAD_AMOUNT_YES_POW);
            ret = COMMAND_FAILURE;
        }
        if(ret != COMMAND_FAILURE)
        {
            if(leading_zero_counter >= atoi(synthesize_args->amount))
            {
                if(synthesizeResource(info->player->wallet, resource_index, atoi(synthesize_args->amount)))
                {
                    String* synthesize = initString();
                    stringAppend(synthesize, "%s %d", getResourceName(resource_index), atoi(synthesize_args->amount));
                    addMessage(info, RESULTS_FORMAT_STRING, SYNTHESIZE_COMMAND, stringGetBuffer(synthesize));
                    deinitString(synthesize);
                }
                else
                {
                    sendCommandError(info, INSUFFICIENT_RESOURCES, getResourceName(resource_index));
                    ret = COMMAND_FAILURE;
                }
            }
            else
            {
                sendCommandError(info, PROOF_OF_WORK_VERIFICATION_FAILED);
                ret = COMMAND_FAILURE;
            }
        }
    }
    else
    {
        int amount_to_synthesize = 1;
        if(synthesize_args->command_format != POW_NO)
        {
            amount_to_synthesize = atoi(synthesize_args->amount);
            if(amount_to_synthesize <= 0 || (synthesize_args->command_format == POW_NO_MANY_LIMIT && amount_to_synthesize > getMaxSynthesisAmount()))
            {
                sendCommandError(info, BAD_AMOUNT_NO_POW, getMaxSynthesisAmount());
                ret = COMMAND_FAILURE;
            }
        }
        if(ret != COMMAND_FAILURE)
        {
            if(synthesizeResource(info->player->wallet, resource_index, amount_to_synthesize))
            {
                String* synthesize = initString();
                stringAppend(synthesize, "%s %d", getResourceName(resource_index), amount_to_synthesize);
                addMessage(info, RESULTS_FORMAT_STRING, SYNTHESIZE_COMMAND, stringGetBuffer(synthesize));
                deinitString(synthesize);
            }
            else
            {
                sendCommandError(info, INSUFFICIENT_RESOURCES, getResourceName(resource_index));
                ret = COMMAND_FAILURE;
            }
        }
    }
    return ret;
}

void freeSynthesizeArgs(void* args)
{
    free(args);
}

Command* createSynthesizeCommand()
{
    Command* ret = malloc(sizeof(Command));
    int commandSize = strlen(SYNTHESIZE_COMMAND) + 1;
    ret->command_name = malloc(sizeof(char) * commandSize);
    ret->checkFormat = parseSynthesizeCommand;
    ret->cleanUp = freeSynthesizeArgs;
    ret->execute = executeSynthesizeCommand;
    sprintf(ret->command_name, SYNTHESIZE_COMMAND);
    return ret;
}
