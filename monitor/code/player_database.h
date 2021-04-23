#ifndef PLAYER_DATABASE_H
#define PLAYER_DATABASE_H

#include <pthread.h>
#include "game_parameters.h"
#include "custom_string.h"
#include "resources.h"
#include "sys/types.h"
#include <openssl/sha.h>

#define INIT_PLAYER_CAPACITY                    0
#define INC_PLAYER_CAPACITY                     5
#define ALIVE                                   1
#define PASSWORD_CHECKSUM_LEN                   (((SHA_DIGEST_LENGTH * 2) + 1) * sizeof(char))

#define RESULT_INSUFFICIENT_COMPUTERS           0
#define RESULT_CRACK_SUCCESSFUL                 1
#define RESULT_CRACK_UNSUCCESSFUL              -1

#define RANDOM_HOST_PORT_GIVEN                  1
#define RANDOM_HOST_PORT_NO_CONNECTED_PLAYERS   -1
#define RANDOM_HOST_PORT_TOO_SOON_TO_RECEIVE    0

#define PLAYER_NEVER_CONNECTED                  "has never connected"

typedef struct ResourceWallet ResourceWallet;

typedef struct HostPort
{
    char* host;
    char* port;
} HostPort;

typedef struct RSAKey
{
    char* exp;
    char* mod;
    char* certificate;
} RSAKey;

typedef struct Player
{
    int serialNo;
    char* name;
    char* password;
    char password_checksum[PASSWORD_CHECKSUM_LEN];
    HostPort hostPort;
    char* cookie;
    RSAKey rsa_key;
    time_t alive_start_time;
    time_t random_host_port_requested;
    int favoredResource;
    ResourceWallet* wallet;
    int commands_used;
    int trades_done;
    int wars_fought;
    int wars_initialized;
    int wars_won;
    int truce_offered;
    int truce_accepted;
    pthread_rwlock_t* lock;
} Player;

typedef struct PlayerDatabase{
    int numberOfPlayers;
    int currentCapacity;
    Player ** players;
    pthread_rwlock_t* lock;
} PlayerDatabase;

PlayerDatabase* createNewPlayerDatabase();
Player* getPlayerByName(char* name);
void freeDatabase();

int getNumberOfPlayers();
int getPlayerList(Player** list, int maxPlayers);
void copyPlayerChecksum(Player* player, char* dest);
int getPlayerWallets(ResourceWallet** list, int maxPlayers);
ResourceWallet* getPlayerWallet(Player* player);


int getPlayerCommandUsage(Player* player);
void incPlayerCommandUsage(Player* player);
void resetPlayerCommandUsage(Player* player);

time_t getPlayerAliveTime(Player* player);
HostPort* getPlayerHostPortCopy(Player* player);
void freeHostPortCopy(HostPort* copy);
void setPlayerHostPort(Player* player, char* host, char* port);

void signOffPlayer(Player* player);
void addRSAKey(Player* player, char *certificate, char *exp, char *mod);
int getRSAKey(Player* player, String* output_buffer);
int handleReceivedPassword(Player* player, char *new_password, char *cookie);
int changePassword(Player* player, char *old_password, char *new_password, char *cookie);
void setPlayerCookie(Player* player, char *new_cookie);
void timeout(int seconds);
void *countDown();
void calculateVictor();
void getGameIdents(String* idents);
int doesCookieMatch(Player* player, char* cookie);
void getPlayerStatus(Player* player, String* player_status);
int getPlayerPassword(Player* player, String* player_password);
int performCrack(Player* player, long computers_used, int min_crack_chance, int max_crack_chance, double growth_rate);
Player* checkIfPlayerExists(char* name);
Player* getPlayerFromSerialNo(int player_serialNo);
int getPlayerSerialNo(Player* player);
int getPlayerHostPort(Player* player, String* player_host_port);
int getRandomPlayerHostPort(String* output_buffer, Player* callingPlayer);
char* getPlayerNamefromId(int serialNo);

PlayerDatabase* createPlayerDatabaseDeepCopy();
void freePlayerDatabaseCopy(PlayerDatabase* copy);

void logPlayerTrade(Player* identity_offered, Player* identity_requested);
void logMonitorTrade(Player* identity_offered);
int getPlayerTradesDone(Player* player);
void logPlayerWar(Player* aggressor, Player* defender);
int getPlayerWarsFought(Player* player);
int getPlayerWarsInitialized(Player* player);
void logPlayerWarVictory(Player* winner);
int getPlayerWarsWon(Player* player);
void logPlayerTruceOffered(Player* player);
void logPlayerTruceAccepted(Player* player);
int getPlayerTruceOffered(Player* player);
int getPlayerTruceAccepted(Player* player);

//Recovery functions
Player* loadPlayerFromFile(char* name);
void setPlayerSerialNo(Player* player, int number);
void setFavouredResource(Player* player, int resource_index);
void setPlayerTradesDone(Player* player, int number);
void setPlayerWarsFought(Player* player, int number);
void setPlayerWarsInitialized(Player* player, int number);
void setPlayerWarsWon(Player* player, int number);
void setPlayerTruceOffered(Player* player, int number);
void setPlayerTruceAccepted(Player* player, int number);
void setPlayerResource(Player* player, int resourceIndex, long amount_available, long amount_committed);
void setPlayerRSAKey(Player* player, char* certificate, char* exp, char* mod);
void setPlayerAliveTime(Player* player);

#endif
