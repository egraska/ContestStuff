#ifndef _RESOURCE_TRANSFER_H
#define _RESOURCE_TRANSFER_H

#include "resources.h"
#include "custom_string.h"

typedef struct ResourceTransfer
{
    int* resourceIndexes;
    long* resourceAmount;
    int numberResources;
} ResourceTransfer;

ResourceTransfer* initResouceTransferFromString(char* string);
void deinitResourceTransfer(ResourceTransfer* transfer);
void appendTransferToString(ResourceTransfer* transfer, String* string);

#endif