#ifndef WAR_TRUCE_COMMAND_H
#define WAR_TRUCE_COMMAND_H

#include "../commands.h"
#include "../resource_transfer.h"

typedef struct
{
    char* identity_offered;
    ResourceTransfer* offered;
    char* identity_offered_to;
} Truce_args;

Command* createWarTruceCommand();

#endif
