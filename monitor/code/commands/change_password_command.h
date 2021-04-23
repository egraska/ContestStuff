#ifndef CHANGE_PASSWORD_COMMAND_H
#define CHANGE_PASSWORD_COMMAND_H

#include "../commands.h"

typedef struct
{
    char* old_password;
    char* new_password;

} ChangePassword_args;

Command* createChangePasswordCommand();

#endif
