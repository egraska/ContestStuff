#ifndef MAKE_CERTIFICATE_COMMAND_H
#define MAKE_CERTIFICATE_COMMAND_H

#include "../commands.h"

typedef struct
{
    char* exp;
    char* mod;
} Make_cert_args;

Command* createMakeCertCommand();

#endif
