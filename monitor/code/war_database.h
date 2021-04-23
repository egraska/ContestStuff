#ifndef WAR_DATABASE_H
#define WAR_DATABASE_H

#include <pthread.h>
#include "war.h"

typedef struct WarDatabase{
    War** wars;
    int numWars;
    int currentCapacity;
    pthread_rwlock_t* lock;
} WarDatabase;

WarDatabase* initWarDatabase();
WarDatabase* deepCopyWarDatabase();
void deinitWarDatabase(WarDatabase* database);

void forfeitAllWars(Player* player);
War* getPlayersWar(Player* player_a, Player* player_b, int shouldCreate);
void runWars();

//Recovery functions
void addWar(War* war);

#endif
