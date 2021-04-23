#ifndef TRUCE_RESPONSE_COMMAND_H
#define TRUCE_RESPONSE_COMMAND_H

#include "../commands.h"
#include "../resource_transfer.h"

#define TRUCE_ACCEPTED              1
#define TRUCE_DECLINED             -1
#define TRUCE_NOT_ALIVE             0
#define TRUCE_SELF_NOT_ALIVE       -2

typedef struct
{
    int truce_proccessed;
} Truce_Response_Args;

Command* createWarTruceResponseCommand();

#endif
