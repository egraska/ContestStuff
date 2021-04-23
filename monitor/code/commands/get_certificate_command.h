#ifndef GET_CERTIFICATE_COMMAND_H
#define GET_CERTIFICATE_COMMAND_H

#include "../commands.h"

typedef struct
{
    char* player_name;

} Get_Cert_args;

Command* createGetCertCommand();

#endif
