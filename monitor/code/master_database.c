#include <stdlib.h>
#include "master_database.h"
#include "recovery.h"
#include "rsa_engine.h"

MasterDatabase* masterDatabase;

void initilizeGame()
{
    masterDatabase = malloc(sizeof(MasterDatabase));
    masterDatabase->players = createNewPlayerDatabase();
    masterDatabase->marketValues = initMarket();
    masterDatabase->monitorWallet = initMonitorWallet();
    masterDatabase->wars = initWarDatabase();
    if(getRecoveryState())
    {
        loadMarket();
        loadMonitorResources();
        loadPlayerDatabase();
        loadWarDatabase();
    }
    else
    {
        loadPlayerDatabase();
    }
    createMonitorCertificate();
}

void restoreGameFromFile(char* filePath);

MasterDatabase* CopyMasterDatabase()
{
    pthread_rwlock_rdlock(masterDatabase->wars->lock);
    for(int i = 0; i < masterDatabase->wars->currentCapacity; i++)
    {
        if(masterDatabase->wars->wars[i])
        {
            pthread_rwlock_rdlock(masterDatabase->wars->wars[i]->lock);
        }
    }
    pthread_rwlock_rdlock(masterDatabase->players->lock);
    pthread_rwlock_rdlock(masterDatabase->monitorWallet->lock);
    for(int i = 0; i < masterDatabase->players->numberOfPlayers; i++)
    {
        pthread_rwlock_rdlock(masterDatabase->players->players[i]->lock);
        pthread_rwlock_rdlock(masterDatabase->players->players[i]->wallet->lock);
    }
    pthread_rwlock_rdlock(masterDatabase->marketValues->lock);

    MasterDatabase* copy = malloc(sizeof(MasterDatabase));
    copy->players = createPlayerDatabaseDeepCopy();
    copy->monitorWallet = deepCopyWallet(masterDatabase->monitorWallet);
    copy->marketValues = deepCopyMarketValues();
    copy->wars = deepCopyWarDatabase();

    pthread_rwlock_unlock(masterDatabase->marketValues->lock);
    for(int i = 0; i < masterDatabase->players->numberOfPlayers; i++)
    {
        pthread_rwlock_unlock(masterDatabase->players->players[i]->lock);
        pthread_rwlock_unlock(masterDatabase->players->players[i]->wallet->lock);
    }
    pthread_rwlock_unlock(masterDatabase->players->lock);
    pthread_rwlock_unlock(masterDatabase->monitorWallet->lock);
    for(int i = 0; i < masterDatabase->wars->currentCapacity; i++)
    {
        if(masterDatabase->wars->wars[i])
        {
            pthread_rwlock_unlock(masterDatabase->wars->wars[i]->lock);
        }
    }
    pthread_rwlock_unlock(masterDatabase->wars->lock);
    return copy;
}

void freeMasterDatabase(MasterDatabase* database)
{
    deinitMarketValues(database->marketValues);
    freePlayerDatabaseCopy(database->players);
    deinitWallet(database->monitorWallet);
    deinitWarDatabase(database->wars);
    free(database);
}
