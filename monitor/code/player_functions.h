#ifndef PLAYER_FUNCTIONS_H
#define PLAYER_FUNCTIONS_H

#include "connection_info.h"
#include "commands.h"

int performRequire(ConnectionInfo* info, Command* requiredCommad);
int performRequireWithState(ConnectionInfo* info, Command* requiredCommad,
                            StateBaseArg* states);
int performRequireWithMultiple(ConnectionInfo* info, Command** requiredCommads,
                                StateBaseArg** states, int numberCommands);
void *handleIncommingConnection(void *socket_desc);

#endif
