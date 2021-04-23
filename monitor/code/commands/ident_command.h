#ifndef IDENT_COMMAND_H
#define IDENT_COMMAND_H

#include "../commands.h"

typedef struct
{
    char* idenity;
    char* client_dh_pk;
} Ident_args;

Command* createIdentCommand();

#endif