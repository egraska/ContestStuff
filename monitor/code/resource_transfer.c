#include "resource_transfer.h"
#include "util.h"
#include "constants.h"
#include <string.h>

#define TRASNFER_APPEND_FROMAT_STRING           " %s %d"

ResourceTransfer* initResouceTransferFromString(char* string)
{
    char** currentPos = &string;
    char* resources[TOTAL_RESOURCES];
    char* amounts[TOTAL_RESOURCES];
    int included[TOTAL_RESOURCES];
    memset(included,0, sizeof(int) * TOTAL_RESOURCES);
    ResourceTransfer* ret = NULL;
    int i = 0;
    if(string)
    {
        while(*currentPos && i < TOTAL_RESOURCES)
        {
            resources[i] = getNextToken(currentPos, SEPERATOR_STR);
            amounts[i] = getNextToken(currentPos, SEPERATOR_STR);
            i++;
        }

        if(i < TOTAL_RESOURCES || (i == TOTAL_RESOURCES &&
            getNextToken(currentPos, SEPERATOR_STR) == NULL))
        {
            ret = malloc(sizeof(ResourceTransfer));
            ret->resourceIndexes = malloc(sizeof(int) * i);
            ret->resourceAmount = malloc(sizeof(long) * i);
            ret->numberResources = i;
            for(int j = 0; j < i; j++)
            {
                ret->resourceIndexes[j] = getResourceIndex(resources[j]);
                if(amounts[j] && isBase10(amounts[j]))
                {
                    ret->resourceAmount[j] = atol(amounts[j]);
                }
                else
                {
                    ret->resourceAmount[j] = 0;
                }
            }
            for(int j = 0; j < i; j++)
            {
                if(ret->resourceAmount[j] <= 0 ||
                   ret->resourceIndexes[j] == -1)
                {
                    deinitResourceTransfer(ret);
                    ret = NULL;
                    break;
                }
                //check to ensure they do not have duplicate resources
                if(included[ret->resourceIndexes[j]])
                {
                    deinitResourceTransfer(ret);
                    ret = NULL;
                    break;
                }
                else
                {
                    included[ret->resourceIndexes[j]]++;
                }
            }
        }
    }
    return ret;
}

void deinitResourceTransfer(ResourceTransfer* transfer)
{
    free(transfer->resourceIndexes);
    free(transfer->resourceAmount);
    free(transfer);
}

void appendTransferToString(ResourceTransfer* transfer, String* string)
{
    for(int i = 0; i < transfer->numberResources; i++)
    {
        stringAppend(string, TRASNFER_APPEND_FROMAT_STRING,
            getResourceName(transfer->resourceIndexes[i]),
            transfer->resourceAmount[i]);
    }
}