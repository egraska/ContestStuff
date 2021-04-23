#ifndef WAR_H
#define WAR_H

#include <pthread.h>
#include "player_database.h"
#include "resources.h"
#include "custom_string.h"

#define WAR_STARTING               -1
#define WAR_ACTIVE                  0
#define WAR_TRUCE                   1
#define WAR_COMPLETE                2

typedef struct ResourceWallet ResourceWallet;
typedef struct Player Player;

typedef struct Army{
    int player_id;
    long weapons;
    long vehicles;
    ResourceWallet* wallet;
} Army;

typedef struct War{
    //army[0] is the attacker and army[1] is the defender
    Army armies[2];
    int winner;
    int status;
    int numBattles;
    pthread_rwlock_t* lock;
} War;

War* initWar(Player* attacker, Player* defender);
War* deepCopyWar(War* war);
void deinintWar(War* war);

int prepareForWar(War* war);
void setDefaultVictor(War* war, Player* attacker, Player* defender, Player* victor, double pillage_factor);
void endWarForTruce(War* war, int truce_sender, int truce_receiver);
void startWar(War* war, Player* attacker, Player* defender,
            long attacker_weapons, long attacker_vehicles,
            long defender_weapons, long defender_vehicles);

int isWarBetweenPlayers(War* war, Player* firstPlayer, Player* secondPlayer);
int isPlayerInWar(War* war, Player* player);
void forfeitWar(War* war, Player* player);

void getWarStatus(War* war, Player* playerRequesting, String* str);
void triggerBattle(War* war);

//Recovery functions
void populateArmies(War* war, Player* attacker, long attacker_weapons,
                              long attacker_vehicles, Player* defender,
                              long defender_weapons, long defender_vehicles);
void setWarStatus(War* war, int status);
void setWarWinner(War* war, int winner);
void setWarNumBattles(War* war, int numBattles);

#endif
