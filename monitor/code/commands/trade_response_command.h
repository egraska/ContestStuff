#ifndef TRADE_RESPONSE_COMMAND_H
#define TRADE_RESPONSE_COMMAND_H

#include "../commands.h"
#include "../resource_transfer.h"

#define TRADE_ACCEPTED              1
#define TRADE_DECLINED             -1
#define TRADE_NOT_ALIVE             0
#define TRADE_SELF_NOT_ALIVE       -2

typedef struct
{
    int trade_proccessed;
} Trade_Response_Args;

Command* createTradeResponseCommand();

#endif
