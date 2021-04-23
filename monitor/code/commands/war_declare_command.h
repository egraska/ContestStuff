#ifndef WAR_DECLARE_COMMAND_H
#define WAR_DECLARE_COMMAND_H

#include "../commands.h"

#define VEHICLES_STRING  "VEHICLES"
#define WEAPONS_STRING   "WEAPONS"

#define WAR_BEGUN_STRING "War begun with %s"
#define WAR_WON_STRING   "%s wins"

typedef struct WarDeclareArgs{
    char* targetIdent;
    char* targetHost;
    char* targetPort;
    int weapons;
    int vehicles;
} WarDeclareArgs;

Command* createWarDeclareCommand();

#endif
