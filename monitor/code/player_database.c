#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "game_parameters.h"
#include "player_database.h"
#include "war_database.h"
#include "constants.h"
#include "util.h"
#include "backup.h"
#include "error_messages.h"

PlayerDatabase* player_database;

int ensureSpaceForNewPlayer()
{
    if(player_database->currentCapacity - player_database->numberOfPlayers == 0)
    {
        int new_capacity = player_database->currentCapacity
            + INC_PLAYER_CAPACITY;
        if(new_capacity > getMaxPlayers())
        {
            new_capacity = getMaxPlayers();
            if(new_capacity == player_database->currentCapacity)
            {
                return 0;
            }
        }
        Player** new_val = (Player**)realloc(player_database->players,
            sizeof(Player*) *  new_capacity);

        if(new_val)
        {
            player_database->players = new_val;
        }
        else
        {
            fprintf(stderr, FATAL_ERROR);
            copyDatabaseToFile();
            exit(1);
        }
        player_database->currentCapacity = new_capacity;
    }
    return 1;
}

Player* privateFindPlayerByName(char * name){
    //This should only be called from within a rwlock.
    Player* ret = NULL;
    for (Player** player = player_database->players;
         player < player_database->players + player_database->numberOfPlayers;
         player++)
    {
        if(strcmp(name, (*player)->name) == 0)
        {
            ret = *player;
            break;
        }
    }
    return ret;
}

Player* addPlayer(char *name){
    //This must be called from within a write
    Player* ret = NULL;
    if(getPlayerDatabaseState())
    {
        int space_available = ensureSpaceForNewPlayer();
        if(!space_available)
        {
            return NULL;
        }
        ret = malloc(sizeof(Player));
        if(!ret)
        {
            fprintf(stderr, FATAL_ERROR);
            copyDatabaseToFile();
            exit(1);
        }
        ret->name = malloc(sizeof(char) * strlen(name) + 1);
        if(!ret->name)
        {
            free(ret);
            fprintf(stderr, FATAL_ERROR);
            copyDatabaseToFile();
            exit(1);
        }
        strcpy(ret->name, name);
        ret->serialNo = getNumberOfPlayers();
        ret->password = NULL;
        ret->password_checksum[0] = 0;
        ret->hostPort.host = NULL;
        ret->hostPort.port = NULL;
        ret->cookie = NULL;
        ret->rsa_key.certificate = NULL;
        ret->rsa_key.exp = NULL;
        ret->rsa_key.mod = NULL;
        ret->alive_start_time = 0;
        ret->random_host_port_requested = 0;
        ret->commands_used = 0;
        ret->trades_done = 0;
        ret->wars_fought = 0;
        ret->wars_initialized = 0;
        ret->wars_won = 0;
        ret->truce_offered = 0;
        ret->truce_accepted = 0;
        ret->favoredResource = (rand() % NUM_BASIC_RESOURCES) + BASIC_RESOURCES_START;
        ret->wallet = initPlayerWallet();
        ret->lock = malloc(sizeof(pthread_rwlock_t));
        if(!ret->lock){
            fprintf(stderr, "FATAL ERROR: Could not create player lock\n");
        }
        if(pthread_rwlock_init(ret->lock, NULL)){
            fprintf(stderr,
                "FATAL ERROR: Read Write lock for player failed init\n");
            fprintf(stderr, FATAL_ERROR);
            copyDatabaseToFile();
            exit(1);
        }
        player_database->players[player_database->numberOfPlayers] = ret;
        player_database->numberOfPlayers++;
    }
    return ret;
}

PlayerDatabase* createNewPlayerDatabase(){
    player_database = malloc(sizeof(PlayerDatabase));
    player_database->numberOfPlayers = 0;
    player_database->currentCapacity = INIT_PLAYER_CAPACITY;
    player_database->players = malloc(sizeof(Player*)*INIT_PLAYER_CAPACITY);
    player_database->lock = malloc(sizeof(pthread_rwlock_t));
    if(!player_database->players || !player_database->lock)
    {
        fprintf(stderr, "FATAL ERROR: Could not create database\n");
    }

    if(pthread_rwlock_init(player_database->lock, NULL))
    {
        fprintf(stderr,
            "FATAL ERROR: Read Write lock for player database failed init\n");
        exit(-1);
    }
    return player_database;
}

Player* getPlayerByName(char* name)
{
    Player* ret = NULL;
    pthread_rwlock_rdlock(player_database->lock);
    ret = privateFindPlayerByName(name);
    pthread_rwlock_unlock(player_database->lock);
    if(!ret)
    {
        pthread_rwlock_wrlock(player_database->lock);
        //Recheck now that we have the write lock
        ret = privateFindPlayerByName(name);
        if(!ret)
        {
            // Player still does not exist
            // Create them
            ret = addPlayer(name);
        }
        pthread_rwlock_unlock(player_database->lock);
    }
    return ret;
}

void freeDatabase()
{
    freePlayerDatabaseCopy(player_database);
}

int getNumberOfPlayers()
{
    return player_database->numberOfPlayers;
}

int getPlayerList(Player** list, int maxPlayers)
{
    pthread_rwlock_rdlock(player_database->lock);
    int ret = player_database->numberOfPlayers;
    if(maxPlayers < ret)
    {
        ret = maxPlayers;
    }
    for(int i = 0; i < ret; i++)
    {
        list[i] = player_database->players[i];
    }
    pthread_rwlock_unlock(player_database->lock);
    return ret;
}

int getPlayerWallets(ResourceWallet** list, int maxPlayers)
{
    pthread_rwlock_rdlock(player_database->lock);
    int ret = player_database->numberOfPlayers;
    if(maxPlayers < ret)
    {
        ret = maxPlayers;
    }
    for(int i = 0; i < ret; i++)
    {
        list[i] = player_database->players[i]->wallet;
    }
    pthread_rwlock_unlock(player_database->lock);
    return ret;
}

ResourceWallet* getPlayerWallet(Player* player)
{
    //I do not think a lock is needed to return the pointer
    return player->wallet;
}

int getPlayerCommandUsage(Player* player)
{
    int ret;
    pthread_rwlock_rdlock(player->lock);
    ret = player->commands_used;
    pthread_rwlock_unlock(player->lock);
    return ret;
}

void copyPlayerChecksum(Player* player, char* dest)
{
    pthread_rwlock_rdlock(player_database->lock);
    strcpy(dest, player->password_checksum);
    pthread_rwlock_unlock(player_database->lock);
}

void incPlayerCommandUsage(Player* player)
{
    pthread_rwlock_wrlock(player->lock);
    player->commands_used++;
    pthread_rwlock_unlock(player->lock);
}

void resetPlayerCommandUsage(Player* player)
{
    pthread_rwlock_wrlock(player->lock);
    player->commands_used = 0;
    pthread_rwlock_unlock(player->lock);
}


void signOffPlayer(Player* player)
{
    pthread_rwlock_wrlock(player->lock);
    player->alive_start_time = 0;
    pthread_rwlock_unlock(player->lock);
    forfeitAllWars(player);
}

void addRSAKey(Player* player, char *certificate, char *exp, char *mod)
{
    pthread_rwlock_wrlock(player->lock);

    safeFree((void**)&player->rsa_key.certificate);
    safeFree((void**)&player->rsa_key.exp);
    safeFree((void**)&player->rsa_key.mod);

    player->rsa_key.certificate = certificate;
    player->rsa_key.exp = exp;
    player->rsa_key.mod = mod;

    pthread_rwlock_unlock(player->lock);
}

int getRSAKey(Player* player, String* output_buffer)
{
    int ret = 0;
    pthread_rwlock_rdlock(player->lock);
    if(player->rsa_key.certificate)
    {
        stringAppend(output_buffer, "%s %s %s", player->rsa_key.exp,
                   player->rsa_key.mod, player->rsa_key.certificate);
        ret = 1;
    }
    pthread_rwlock_unlock(player->lock);
    return ret;
}

int setPassword(Player* player, char *new_password, char *cookie){
    int ret;
    safeFree((void**)&player->password);

    size_t length = strlen(new_password);
    player->password = (char*)malloc(sizeof(char) * (length +1));
    if(!player->password)
    {
        fprintf(stderr, FATAL_ERROR);
        copyDatabaseToFile();
        exit(1);
    }
    strcpy(player->password, new_password);
    setPlayerCookie(player, cookie);
    SHA1((unsigned char*)player->password,
        length, (unsigned char*)player->password_checksum);

    byteArrayToString((unsigned char*)player->password_checksum,
        SHA_DIGEST_LENGTH, BASE_HEX);

    ret = 1;
    return ret;
}

int handleReceivedPassword(Player* player, char *new_password, char *cookie){
    int ret = 1;
    pthread_rwlock_rdlock(player->lock);
    if(player->password)
    {
        ret = (!strcmp(player->password, new_password));
    }
    pthread_rwlock_unlock(player->lock);
    if(ret){
    pthread_rwlock_wrlock(player->lock);
        if(player->password)
        {
            ret = (!strcmp(player->password, new_password));
        }
        if(ret){
            ret = setPassword(player, new_password, cookie);
        }
    pthread_rwlock_unlock(player->lock);
    }
    return ret;
}

int changePassword(Player* player, char *old_password,
                   char *new_password, char *cookie)
{
    int ret;
    pthread_rwlock_rdlock(player->lock);
    ret = (!strcmp(player->password, old_password));
    pthread_rwlock_unlock(player->lock);
    if(ret)
    {
        pthread_rwlock_wrlock(player->lock);
        ret = (!strcmp(player->password, old_password));
        if(ret)
        {
            ret = setPassword(player, new_password, cookie);
        }
        pthread_rwlock_unlock(player->lock);
    }
    return ret;
}

void setPlayerCookie(Player* player, char *new_cookie)
{
    safeFree((void**)&player->cookie);
    player->cookie = (char*)malloc(sizeof(char) * (strlen(new_cookie) +1));
    if(!player->cookie)
    {
        fprintf(stderr, FATAL_ERROR);
        copyDatabaseToFile();
        exit(1);
    }
    strcpy(player->cookie, new_cookie);
}

time_t getPlayerAliveTime(Player* player)
{
    time_t ret = 0;
    pthread_rwlock_rdlock(player->lock);
    ret = player->alive_start_time;
    pthread_rwlock_unlock(player->lock);
    return ret;
}

HostPort* getPlayerHostPortCopy(Player* player)
{
    pthread_rwlock_rdlock(player->lock);
    HostPort* ret = malloc(sizeof(HostPort));
    ret->host = malloc(sizeof(char) * strlen(player->hostPort.host) + 1);
    strcpy(ret->host, player->hostPort.host);
    ret->port = malloc(sizeof(char) * strlen(player->hostPort.port) + 1);
    strcpy(ret->port, player->hostPort.port);
    pthread_rwlock_unlock(player->lock);
    return ret;
}

void freeHostPortCopy(HostPort* copy)
{
    if(copy)
    {
        safeFree((void**)&copy->host);
        safeFree((void**)&copy->port);
        free(copy);
    }
}

void setPlayerHostPort(Player* player, char* host, char* port)
{
    pthread_rwlock_wrlock(player->lock);

    safeFree((void**)&player->hostPort.host);
    safeFree((void**)&player->hostPort.port);

    player->hostPort.host = malloc(sizeof(char) * (strlen(host) + 1));
    player->hostPort.port = malloc(sizeof(char) * (strlen(port) + 1));

    strcpy(player->hostPort.host, host);
    strcpy(player->hostPort.port, port);

    if(!player->alive_start_time)
    {
        player->alive_start_time = time(NULL);
    }

    pthread_rwlock_unlock(player->lock);
}

void calculateVictor(){
    /*printf("\nCalculating Victor\n");
    double highestWealth = 0;
    double wealth = 0;
    int rank;
    for (int i = 0; i<getNumberOfPlayers(); i++){
        for (int j = 0; j<10; j++) {
            wealth = getPlayerResource(i, j) * getMarketValue(j) + wealth;
        }
        printf("Player %s has wealth %f\n", getPlayerNameFromID(i), wealth);
        if(highestWealth < wealth){
            rank = i;
            highestWealth = wealth;
        }
        wealth = 0;
    }
    printf("Player %s wins\n", getPlayerNameFromID(rank));*/
}

void getGameIdents(String* idents)
{
    pthread_rwlock_rdlock(player_database->lock);
    Player** player;
    for(player = player_database->players;
        player <
            (player_database->players + player_database->numberOfPlayers);
        player++)
    {
        if((*player)->alive_start_time)
        {
            stringAppend(idents, SEPERATOR_STR);
            stringAppend(idents, (*player)->name);
        }
    }
    pthread_rwlock_unlock(player_database->lock);
}

int doesCookieMatch(Player* player, char* cookie){
    return (!strcmp(player->cookie, cookie));
}

void getPlayerStatus(Player* player, String* player_status)
{
    pthread_rwlock_rdlock(player->wallet->lock);
    for(int i=0; i<TOTAL_RESOURCES; i++)
    {
        stringAppend(player_status, RESOURCE_NAME_VALUE_FORMAT_STRING,
            getResourceName(i),
            player->wallet->holdings[i].amount_available);
    }
    pthread_rwlock_unlock(player->wallet->lock);
}

int getPlayerPassword(Player* player, String* player_password)
{
    int ret = 0;
    pthread_rwlock_rdlock(player->lock);
    if(player->cookie)
    {
        char* temp = malloc(sizeof(char) * (getPasswordCrackChars() + 1));
        memset(temp, 0, (sizeof(char) * (getPasswordCrackChars() + 1)));
        strncpy(temp, player->cookie, getPasswordCrackChars());
        stringAppend(player_password, SEPERATOR_STR);
        stringAppend(player_password, temp);
        free(temp);
        ret = 1;
    }
    pthread_rwlock_unlock(player->lock);
    return ret;
}

int performCrack(Player* player, long computers_used,
                int min_crack_chance, int max_crack_chance, double growth_rate)
{
    int ret = 1;
    if(computers_used)
    {
        ret = consumePlayerComputersForInformation(player->wallet, computers_used);
    }
    if(ret)
    {
        double success_chance;
        if(computers_used==0)
        {
            success_chance = min_crack_chance;
        }
        else
        {
            success_chance = (max_crack_chance)*
            (pow(growth_rate, computers_used)-1) /
            (pow(growth_rate, computers_used));
        }
        double range = max_crack_chance - min_crack_chance;
        double chance = ((double)rand()/(double)RAND_MAX) * range;
        if(chance < success_chance)
        {
            ret = RESULT_CRACK_SUCCESSFUL;
        }
        else
        {
            ret = RESULT_CRACK_UNSUCCESSFUL;
        }
    }
    return ret;
}

Player* checkIfPlayerExists(char* name)
{
    Player* ret = NULL;
    pthread_rwlock_rdlock(player_database->lock);
    ret = privateFindPlayerByName(name);
    pthread_rwlock_unlock(player_database->lock);
    return ret;
}

int getPlayerHostPort(Player* player, String* player_host_port)
{
    int ret = 0;
    pthread_rwlock_rdlock(player->lock);
    if(player->alive_start_time && player->hostPort.host && player->hostPort.port)
    {
        stringAppend(player_host_port, "%s %s",
            player->hostPort.host, player->hostPort.port);
        ret = 1;
    }
    pthread_rwlock_unlock(player->lock);
    return ret;
}

Player* getPlayerFromSerialNo(int player_serialNo)
{
    Player* ret = NULL;
    pthread_rwlock_rdlock(player_database->lock);
    ret = player_database->players[player_serialNo];
    pthread_rwlock_unlock(player_database->lock);
    return ret;
}

int getPlayerSerialNo(Player* player)
{
    int ret = -1;
    pthread_rwlock_rdlock(player->lock);
    ret = player->serialNo;
    pthread_rwlock_unlock(player->lock);
    return ret;
}

int getConnectedPlayerList(Player** list, int maxPlayers, Player* callingPlayer)
{
    int connectedPlayerIndex = 0;
    for(int i = 0; i < maxPlayers; i++)
    {
        if(player_database->players[i] == callingPlayer)
        {
            continue;
        }
        list[connectedPlayerIndex] = player_database->players[i];
        connectedPlayerIndex++;
    }
    return connectedPlayerIndex;
}

int checkPlayerEligibilityForRandomHostPort(Player* player)
{
    int ret = 0;
    pthread_rwlock_wrlock(player->lock);
    if((time(NULL)-(player->random_host_port_requested)) >
        getTimeForRandomHP())
    {
        player->random_host_port_requested = time(NULL);
        ret = 1;
    }
    pthread_rwlock_unlock(player->lock);
    return ret;
}

int getRandomPlayerHostPort(String* output_buffer, Player* callingPlayer)
{
    int numberOfConnectedPlayers = 0;
    int timeCheck = checkPlayerEligibilityForRandomHostPort(callingPlayer);
    int ret;
    if(timeCheck)
    {
        pthread_rwlock_rdlock(player_database->lock);
        int numberOfPlayers = player_database->numberOfPlayers;
        Player** connectedPlayerList = malloc(sizeof(Player*)*numberOfPlayers);
        numberOfConnectedPlayers = getConnectedPlayerList(connectedPlayerList,
            numberOfPlayers, callingPlayer);

        pthread_rwlock_unlock(player_database->lock);
        if(numberOfConnectedPlayers)
        {
            int random_number = rand()%numberOfConnectedPlayers;
            pthread_rwlock_rdlock(connectedPlayerList[random_number]->lock);
            stringAppend(output_buffer,
                connectedPlayerList[random_number]->name);
            stringAppend(output_buffer, SEPERATOR_STR);
            if(connectedPlayerList[random_number]->hostPort.host &&
                connectedPlayerList[random_number]->hostPort.port)
            {
                stringAppend(output_buffer, "%s %s",
                    connectedPlayerList[random_number]->hostPort.host,
                    connectedPlayerList[random_number]->hostPort.port);
            }
            else
            {
                stringAppend(output_buffer, "%s", PLAYER_NEVER_CONNECTED);
            }
            ret = RANDOM_HOST_PORT_GIVEN;
            pthread_rwlock_unlock(connectedPlayerList[random_number]->lock);
        }
        else
        {
            ret = RANDOM_HOST_PORT_NO_CONNECTED_PLAYERS;
        }
        free(connectedPlayerList);
    }
    else
    {
        ret = RANDOM_HOST_PORT_TOO_SOON_TO_RECEIVE;
    }
    return ret;
}

char* getPlayerNamefromId(int serialNo)
{
    char* ret = NULL;
    pthread_rwlock_rdlock(player_database->lock);
    if(serialNo < player_database->numberOfPlayers)
    {
        ret = player_database->players[serialNo]->name;
    }
    pthread_rwlock_unlock(player_database->lock);
    return ret;
}

Player* playerDeepCopy(Player* player)
{
    //this method assumes the player is aready locked.
    Player* ret = malloc(sizeof(Player));
    ret->serialNo = player->serialNo;
    ret->name = safeStrCpy(player->name);
    ret->password = safeStrCpy(player->password);
    memcpy(ret->password_checksum, player->password_checksum,
        PASSWORD_CHECKSUM_LEN);

    ret->hostPort.host = safeStrCpy(player->hostPort.host);
    ret->hostPort.port = safeStrCpy(player->hostPort.port);
    ret->cookie = safeStrCpy(player->cookie);
    ret->rsa_key.certificate = safeStrCpy(player->rsa_key.certificate);
    ret->rsa_key.exp = safeStrCpy(player->rsa_key.exp);
    ret->rsa_key.mod = safeStrCpy(player->rsa_key.mod);
    ret->commands_used = player->commands_used;
    ret->lock = malloc(sizeof(pthread_rwlock_t));
    ret->wallet = deepCopyWallet(player->wallet);
    ret->alive_start_time = player->alive_start_time;
    ret->random_host_port_requested = player->random_host_port_requested;
    ret->favoredResource = player->favoredResource;
    ret->trades_done = player->trades_done;
    ret->wars_fought = player->wars_fought;
    ret->wars_initialized = player->wars_initialized;
    ret->wars_won = player->wars_won;
    ret->truce_offered = player->truce_offered;
    ret->truce_accepted = player->truce_accepted;
    pthread_rwlock_init(ret->lock, NULL);
    return ret;
}

PlayerDatabase* createPlayerDatabaseDeepCopy()
{
    PlayerDatabase* ret = malloc(sizeof(PlayerDatabase));
    ret->numberOfPlayers = player_database->numberOfPlayers;
    ret->currentCapacity = player_database->numberOfPlayers;
    ret->players = malloc(sizeof(Player**)*ret->numberOfPlayers);
    ret->lock = malloc(sizeof(pthread_rwlock_t));
    pthread_rwlock_init(ret->lock, NULL);

    for(int i = 0; i < player_database->numberOfPlayers; i++)
    {
        ret->players[i] = playerDeepCopy(player_database->players[i]);
    }

    return ret;
}

void deinitPlayer(Player* player)
{
    safeFree((void**)&player->name);
    safeFree((void**)&player->password);
    safeFree((void**)&player->hostPort.port);
    safeFree((void**)&player->hostPort.host);
    safeFree((void**)&player->cookie);
    safeFree((void**)&player->rsa_key.certificate);
    safeFree((void**)&player->rsa_key.exp);
    safeFree((void**)&player->rsa_key.mod);
    deinitWallet(player->wallet);
    pthread_rwlock_destroy(player->lock);
    safeFree((void**)&player->lock);
    safeFree((void**)&player);
}

void freePlayerDatabaseCopy(PlayerDatabase* copy)
{
    for(int i=0; i<copy->numberOfPlayers; i++)
    {
        deinitPlayer(copy->players[i]);
    }
    free(copy->players);
    pthread_rwlock_destroy(copy->lock);
    free(copy->lock);
    free(copy);
}

void logPlayerTrade(Player* identity_offered, Player* identity_requested)
{
    pthread_rwlock_wrlock(identity_offered->lock);
    identity_offered->trades_done++;
    pthread_rwlock_unlock(identity_offered->lock);

    pthread_rwlock_wrlock(identity_requested->lock);
    identity_requested->trades_done++;
    pthread_rwlock_unlock(identity_requested->lock);
}

void logMonitorTrade(Player* identity_offered)
{
    pthread_rwlock_wrlock(identity_offered->lock);
    identity_offered->trades_done++;
    pthread_rwlock_unlock(identity_offered->lock);
}

int getPlayerTradesDone(Player* player)
{
    int ret;
    pthread_rwlock_rdlock(player->lock);
    ret = player->trades_done;
    pthread_rwlock_unlock(player->lock);
    return ret;
}

void logPlayerWar(Player* aggressor, Player* defender)
{
    pthread_rwlock_wrlock(aggressor->lock);
    aggressor->wars_fought++;
    aggressor->wars_initialized++;
    pthread_rwlock_unlock(aggressor->lock);

    pthread_rwlock_wrlock(defender->lock);
    defender->wars_fought++;
    pthread_rwlock_unlock(defender->lock);
}

int getPlayerWarsFought(Player* player)
{
    int ret;
    pthread_rwlock_rdlock(player->lock);
    ret = player->wars_fought;
    pthread_rwlock_unlock(player->lock);
    return ret;
}

int getPlayerWarsInitialized(Player* player)
{
    int ret;
    pthread_rwlock_rdlock(player->lock);
    ret = player->wars_initialized;
    pthread_rwlock_unlock(player->lock);
    return ret;
}

void logPlayerWarVictory(Player* winner)
{
    pthread_rwlock_wrlock(winner->lock);
    winner->wars_won++;
    pthread_rwlock_unlock(winner->lock);
}

int getPlayerWarsWon(Player* player)
{
    int ret;
    pthread_rwlock_rdlock(player->lock);
    ret = player->wars_won;
    pthread_rwlock_unlock(player->lock);
    return ret;
}

void logPlayerTruceOffered(Player* player)
{
    pthread_rwlock_wrlock(player->lock);
    player->truce_offered++;
    pthread_rwlock_unlock(player->lock);
}

void logPlayerTruceAccepted(Player* player)
{
    pthread_rwlock_wrlock(player->lock);
    player->truce_accepted++;
    pthread_rwlock_unlock(player->lock);
}

int getPlayerTruceOffered(Player* player)
{
    int ret;
    pthread_rwlock_rdlock(player->lock);
    ret = player->truce_offered;
    pthread_rwlock_unlock(player->lock);
    return ret;
}

int getPlayerTruceAccepted(Player* player)
{
    int ret;
    pthread_rwlock_rdlock(player->lock);
    ret = player->truce_accepted;
    pthread_rwlock_unlock(player->lock);
    return ret;
}

//Recovery functions
Player* loadPlayerFromFile(char* name)
{
    Player* ret = addPlayer(name);
    return ret;
}

void setPlayerSerialNo(Player* player, int number)
{
    player->serialNo = number;
}

void setFavouredResource(Player* player, int resource_index)
{
    //Favoured resource is only set from out of add player when player is being recovered.
    //Hence, does not require lock.
    player->favoredResource = resource_index;
}

void setPlayerTradesDone(Player* player, int number)
{
    player->trades_done = number;
}

void setPlayerWarsFought(Player* player, int number)
{
    player->wars_fought = number;
}

void setPlayerWarsInitialized(Player* player, int number)
{
    player->wars_initialized = number;
}

void setPlayerWarsWon(Player* player, int number)
{
    player->wars_won = number;
}

void setPlayerTruceOffered(Player* player, int number)
{
    player->truce_offered = number;
}

void setPlayerTruceAccepted(Player* player, int number)
{
    player->truce_accepted = number;
}

void setPlayerResource(Player* player, int resourceIndex, long amount_available, long amount_committed)
{
    //This method should only be called from the recovery function.
    //It does not use a lock, since only the main thread uses this function, before the game even begins.
    player->wallet->holdings[resourceIndex].amount_available = amount_available;
    player->wallet->holdings[resourceIndex].amount_commited = amount_committed;
}

void setPlayerRSAKey(Player* player, char* certificate, char* exp, char* mod)
{
    char* player_exp = malloc(sizeof(char) * (strlen(exp) + 1));
    char* player_mod = malloc(sizeof(char) * (strlen(mod) + 1));
    char* player_certificate = malloc(sizeof(char) * (strlen(certificate) + 1));

    strcpy(player_exp, exp);
    strcpy(player_mod, mod);
    strcpy(player_certificate, certificate);

    addRSAKey(player, player_certificate, player_exp, player_mod);
}

void setPlayerAliveTime(Player* player)
{
    //This method is only called from the recovery function.
    //Setting the alive time to 0, because when host port is recovered, player alive time begins,
    //which we do not want.
    //Hence resetting to 0.
    player->alive_start_time = 0;
}
