#ifndef WAR_DEFEND_COMMAND_H
#define WAR_DEFEND_COMMAND_H

#include "../commands.h"

#define VEHICLES_STRING "VEHICLES"
#define WEAPONS_STRING  "WEAPONS"

typedef struct WarDefendArgs{
    int weapons;
    int vehicles;
} WarDefendArgs;

typedef struct WarDefendState{
    int weapons;
    int vehicles;
    char* attacker_name;
} WarDefendState;


Command* createWarDefendCommand();

#endif
