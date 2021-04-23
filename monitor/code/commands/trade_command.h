#ifndef TRADE_COMMAND_H
#define TRADE_COMMAND_H

#include "../commands.h"
#include "../resource_transfer.h"

typedef struct
{
    char* identity_offered;
    ResourceTransfer* offered;
    char* identity_requested;
    ResourceTransfer* requested;
} Trade_args;

Command* createTradeCommand();

#endif