#include <string.h>
#include <stdio.h>
#include "war_database.h"

#define INITAL_WAR_DATABASE_SIZE                5

WarDatabase* warDatabase;

WarDatabase* initWarDatabase()
{
    warDatabase = malloc(sizeof(WarDatabase));
    warDatabase->wars = malloc(sizeof(War*) *
                        INITAL_WAR_DATABASE_SIZE);

    memset(warDatabase->wars, 0,
        sizeof(War*) * INITAL_WAR_DATABASE_SIZE);
    warDatabase->numWars = 0;
    warDatabase->currentCapacity = INITAL_WAR_DATABASE_SIZE;
    warDatabase->lock = malloc(sizeof(pthread_rwlock_t));
    pthread_rwlock_init(warDatabase->lock, NULL);
    return warDatabase;
}

WarDatabase* deepCopyWarDatabase()
{
    WarDatabase* ret = malloc(sizeof(WarDatabase));

    ret->wars = malloc(sizeof(War*) *
                        warDatabase->numWars);

    ret->numWars = warDatabase->numWars;
    ret->currentCapacity = warDatabase->numWars;

    int warsLeft = ret->numWars;
    for(int i = 0; i < warDatabase->currentCapacity; i++)
    {
        if(warDatabase->wars[i])
        {
            warsLeft--;
            ret->wars[warsLeft] =
                deepCopyWar(warDatabase->wars[i]);
            if(!warsLeft)
            {
                break;
            }
        }
    }

    ret->lock = malloc(sizeof(pthread_rwlock_t));
    pthread_rwlock_init(ret->lock, NULL);


    return ret;
}

void deinitWarDatabase(WarDatabase* database)
{
    for(int i = 0; i < database->currentCapacity; i++)
    {
        if(database->wars[i])
        {
            deinintWar(database->wars[i]);
        }
    }
    pthread_rwlock_destroy(database->lock);
    free(database->lock);
    free(database->wars);
    free(database);
}

void _addWar(War* war)
{
    if(warDatabase->numWars == warDatabase->currentCapacity)
    {
        War** temp = realloc(warDatabase->wars,
                        (sizeof(War*)*warDatabase->currentCapacity * 2));
        if(temp)
        {
            warDatabase->wars = temp;
            warDatabase->currentCapacity *= 2;
            warDatabase->wars[warDatabase->numWars] = war;
            for(int i = warDatabase->numWars+1; i < warDatabase->currentCapacity; i++)
            {
                warDatabase->wars[i] = NULL;
            }
        }
        else
        {
            fprintf(stderr, "Fatal error. War Db could not be extended\n");
        }
    }
    else
    {
        for(int i = 0; i < warDatabase->currentCapacity; i++)
        {
            if(!warDatabase->wars[i])
            {
                warDatabase->wars[i] = war;
                break;
            }
        }
    }
    warDatabase->numWars++;
}

War* _findExistingWar(Player* player_a, Player* player_b)
{
    War* ret = NULL;
    for(int i = 0; i < warDatabase->currentCapacity; i++)
    {
        if(warDatabase->wars[i])
        {
            if(isWarBetweenPlayers(warDatabase->wars[i],
                player_a, player_b))
            {
                ret = warDatabase->wars[i];
                break;
            }
        }
    }
    return ret;
}

void forfeitAllWars(Player* player)
{
    pthread_rwlock_rdlock(warDatabase->lock);
    for(int i = 0; i < warDatabase->currentCapacity; i++)
    {
        if(warDatabase->wars[i] &&
           isPlayerInWar(warDatabase->wars[i], player))
        {
            forfeitWar(warDatabase->wars[i], player);
        }
    }
    pthread_rwlock_unlock(warDatabase->lock);
}

War* getPlayersWar(Player* player_a, Player* player_b, int shouldCreate)
{
    War* ret = NULL;
    pthread_rwlock_rdlock(warDatabase->lock);
    ret = _findExistingWar(player_a, player_b);
    pthread_rwlock_unlock(warDatabase->lock);
    if(!ret && shouldCreate)
    {
        pthread_rwlock_wrlock(warDatabase->lock);
        ret = _findExistingWar(player_a, player_b);
        if(!ret)
        {
            ret = initWar(player_a, player_b);
            _addWar(ret);
        }
        pthread_rwlock_unlock(warDatabase->lock);
    }
    return ret;
}

void runWars()
{
    pthread_rwlock_rdlock(warDatabase->lock);
    for(int i = 0; i < warDatabase->currentCapacity; i++)
    {
        if(warDatabase->wars[i])
        {
            triggerBattle(warDatabase->wars[i]);
        }
    }
    pthread_rwlock_unlock(warDatabase->lock);
}

//Recovery functions
void addWar(War* war)
{
    _addWar(war);
}
