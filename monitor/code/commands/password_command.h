#ifndef PASSWORD_COMMAND_H
#define PASSWORD_COMMAND_H

#include "../commands.h"

typedef struct
{
    char* password;

} Password_args;

Command* createPasswordCommand();

#endif
