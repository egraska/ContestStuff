#include <string.h>
#include <stdio.h>
#include "war.h"
#include "war_database.h"
#include "repeated_events.h"

#define AGGRESOR_FORMAT_STRING              "AGGRESSOR %s "
#define DEFENDER_FORMAT_STRING              "DEFENDER %s "
#define WINNER_FORMAT_STRING                "WINNER %s "
#define BATTLES_FORMAT_STRING               "BATTLES %d "
#define WAR_STATUS_FORMAT_STRING            "STATUS %s "
#define WEAPONS_FORMAT_STRING               "WEAPONS %d "
#define VEHICLES_FORMAT_STRING              "VEHICLES %d"
#define TRUCE_WINNER_FORMAT_STRING          "TRUCE-%s"

#define NONE_STRING                         "NONE"
#define COMPLETE_STRING                     "COMPLETED"
#define ACTIVE_STRING                       "ACTIVE"

#define BATTLES_WINNER_FORMAT_STRING        "%s: Player %s won a battle against %s\n"
#define WAR_WINNER_FORMAT_STRING            "%s: Player %s won the war against %s\n"

#define ATTACKER_INDEX                      0
#define DEFENDER_INDEX                      1


void* doWar(void *war);

void populateArmy(Army* army, Player* player,
    long weapons, long vehicles)
{
    army->player_id = player->serialNo;
    army->vehicles = vehicles;
    army->weapons = weapons;
    army->wallet = player->wallet;
}

War* initWar(Player* attacker, Player* defender)
{
    War* ret = malloc(sizeof(War));
    populateArmy(ret->armies + ATTACKER_INDEX, attacker, 0, 0);
    populateArmy(ret->armies + DEFENDER_INDEX, defender, 0, 0);
    ret->status = WAR_TRUCE;
    ret->winner = 0;
    ret->numBattles = 0;
    ret->lock = malloc(sizeof(pthread_rwlock_t));
    pthread_rwlock_init(ret->lock, NULL);
    return ret;
}

War* deepCopyWar(War* war)
{
    War* ret = malloc(sizeof(War));
    memcpy(ret->armies, war->armies, sizeof(Army) * 2);

    ret->lock = malloc(sizeof(pthread_rwlock_t));
    pthread_rwlock_init(ret->lock, NULL);

    ret->winner = war->winner;
    ret->status = war->status;
    ret->numBattles = war->numBattles;
    return ret;
}

void deinintWar(War* war)
{
    pthread_rwlock_destroy(war->lock);
    free(war->lock);
    free(war);
}

int prepareForWar(War* war)
{
    int ret = 0;
    pthread_rwlock_wrlock(war->lock);
    if(war->status == WAR_COMPLETE || war->status == WAR_TRUCE)
    {
        ret = 1;
        war->status = WAR_STARTING;
    }
    pthread_rwlock_unlock(war->lock);
    return ret;
}

void startWar(War* war, Player* attacker, Player* defender,
            long attacker_weapons, long attacker_vehicles,
            long defender_weapons, long defender_vehicles)
{
    pthread_rwlock_wrlock(war->lock);

    populateArmy(war->armies + ATTACKER_INDEX, attacker,
        attacker_weapons, attacker_vehicles);
    populateArmy(war->armies + DEFENDER_INDEX, defender,
        defender_weapons, defender_vehicles);

    war->winner = 0;
    war->status = WAR_ACTIVE;
    war->numBattles = 0;

    pthread_rwlock_unlock(war->lock);
}

void endWar(War* war)
{
    Player* winner = getPlayerFromSerialNo(war->armies[war->winner].player_id);
    if(winner)
    {
        logPlayerWarVictory(winner);
    }

    returnTroopsFromWar(war->armies[ATTACKER_INDEX].wallet,
                    war->armies[ATTACKER_INDEX].weapons,
                    war->armies[ATTACKER_INDEX].vehicles);
    returnTroopsFromWar(war->armies[DEFENDER_INDEX].wallet,
                    war->armies[DEFENDER_INDEX].weapons,
                    war->armies[DEFENDER_INDEX].vehicles);
    war->armies[ATTACKER_INDEX].weapons = 0;
    war->armies[ATTACKER_INDEX].vehicles = 0;
    war->armies[DEFENDER_INDEX].weapons = 0;
    war->armies[DEFENDER_INDEX].vehicles = 0;
}

void setDefaultVictor(War* war, Player* attacker, Player* defender, Player* victor, double pillage_factor)
{
    pthread_rwlock_wrlock(war->lock);
    if(war->status != WAR_COMPLETE && war->status != WAR_TRUCE)
    {
        war->status = WAR_COMPLETE;
        war->numBattles = 0;

        war->winner = victor == attacker ? ATTACKER_INDEX : DEFENDER_INDEX;
        int loser_index = victor == attacker ? DEFENDER_INDEX : ATTACKER_INDEX;

        populateArmy(war->armies + ATTACKER_INDEX, attacker, 0, 0);
        populateArmy(war->armies + DEFENDER_INDEX, defender, 0, 0);

        pillage(war->armies + war->winner, war->armies + loser_index, pillage_factor);
    }
    pthread_rwlock_unlock(war->lock);
    logPlayerWarVictory(victor);
}

void endWarForTruce(War* war, int truce_sender, int truce_receiver)
{
    if(war->armies[ATTACKER_INDEX].player_id == truce_receiver)
    {
        war->winner = ATTACKER_INDEX;
    }
    else
    {
        war->winner = DEFENDER_INDEX;
    }
    war->status = WAR_TRUCE;
    war->numBattles = 0;
    endWar(war);
}

int isWarBetweenPlayers(War* war, Player* firstPlayer, Player* secondPlayer)
{
    int ret;
    pthread_rwlock_rdlock(war->lock);

    ret = ((war->armies[ATTACKER_INDEX].player_id == firstPlayer->serialNo &&
            war->armies[DEFENDER_INDEX].player_id == secondPlayer->serialNo)) ||
          ((war->armies[DEFENDER_INDEX].player_id == firstPlayer->serialNo &&
            war->armies[ATTACKER_INDEX].player_id == secondPlayer->serialNo));

    pthread_rwlock_unlock(war->lock);

    return ret;
}

int isPlayerInWar(War* war, Player* player)
{
    int ret = 0;
    pthread_rwlock_rdlock(war->lock);

    if(war->status !=  WAR_COMPLETE && war->status !=  WAR_TRUCE)
    {
        if(war->armies[ATTACKER_INDEX].player_id == player->serialNo ||
               war->armies[DEFENDER_INDEX].player_id == player->serialNo)
        {
            ret = 1;
        }
    }

    pthread_rwlock_unlock(war->lock);
    return ret;
}

void forfeitWar(War* war, Player* player)
{
    pthread_rwlock_wrlock(war->lock);
    int playerFound = 0;
    if(war->armies[ATTACKER_INDEX].player_id == player->serialNo)
    {
        war->status = WAR_COMPLETE;
        war->winner = DEFENDER_INDEX;
        playerFound = 1;
    }
    else if(war->armies[DEFENDER_INDEX].player_id == player->serialNo)
    {
        war->status = WAR_COMPLETE;
        war->winner = ATTACKER_INDEX;
        playerFound = 1;
    }

    if(playerFound)
    {
        endWar(war);
        int pillager = war->winner;
        int loser = ATTACKER_INDEX == pillager ? DEFENDER_INDEX : ATTACKER_INDEX;
        pillage(war->armies + pillager, war->armies + loser, getPillageAmount());
    }
    pthread_rwlock_unlock(war->lock);
}

void getWarStatus(War* war, Player* playerRequesting, String* str)
{
    pthread_rwlock_rdlock(war->lock);

    stringAppend(str, AGGRESOR_FORMAT_STRING,
        getPlayerNamefromId(war->armies[ATTACKER_INDEX].player_id));

    stringAppend(str, DEFENDER_FORMAT_STRING,
        getPlayerNamefromId(war->armies[DEFENDER_INDEX].player_id));

    if(war->status == WAR_COMPLETE)
    {
       stringAppend(str, WINNER_FORMAT_STRING,
        getPlayerNamefromId(war->armies[war->winner].player_id));
    }
    else if(war->status == WAR_TRUCE)
    {
        String* truce_winner = initString();
        stringAppend(truce_winner, TRUCE_WINNER_FORMAT_STRING, getPlayerNamefromId(war->armies[war->winner].player_id));
        stringAppend(str, WINNER_FORMAT_STRING, stringGetBuffer(truce_winner));
        deinitString(truce_winner);
    }
    else
    {
        stringAppend(str, WINNER_FORMAT_STRING, NONE_STRING);
    }
    stringAppend(str, BATTLES_FORMAT_STRING, war->numBattles);

    if(war->status == WAR_COMPLETE ||
       war->status == WAR_TRUCE    ||
       war->status == WAR_STARTING)
    {
       stringAppend(str, WAR_STATUS_FORMAT_STRING, COMPLETE_STRING);
    }
    else
    {
        stringAppend(str, WAR_STATUS_FORMAT_STRING, ACTIVE_STRING);
    }

    if(playerRequesting->serialNo == war->armies[ATTACKER_INDEX].player_id)
    {
        stringAppend(str,
            WEAPONS_FORMAT_STRING,
            war->armies[ATTACKER_INDEX].weapons);

        stringAppend(str,
            VEHICLES_FORMAT_STRING,
            war->armies[ATTACKER_INDEX].vehicles);
    }
    else
    {
        stringAppend(str, WEAPONS_FORMAT_STRING,
            war->armies[DEFENDER_INDEX].weapons);

        stringAppend(str, VEHICLES_FORMAT_STRING,
            war->armies[DEFENDER_INDEX].vehicles);
    }

    pthread_rwlock_unlock(war->lock);
}

int determineBattlewinner(War* war)
{
    float attackerTotal = war->armies[ATTACKER_INDEX].weapons +
                          war->armies[ATTACKER_INDEX].vehicles;
    float defenderTotal = war->armies[DEFENDER_INDEX].weapons +
                          war->armies[DEFENDER_INDEX].vehicles;
    //from documenation
    float odds = attackerTotal / ((attackerTotal + defenderTotal)
                 * getWarDefenderBias());
    return (rand() % 100 > (odds*100));
}

void checkifWarOver(War* war)
{
    if(!war->armies[ATTACKER_INDEX].weapons &&
       !war->armies[ATTACKER_INDEX].vehicles)
    {
        war->status = WAR_COMPLETE;
        war->winner = DEFENDER_INDEX;
        fprintf(stdout, WAR_WINNER_FORMAT_STRING, WAR_NAME,
        getPlayerNamefromId(war->armies[DEFENDER_INDEX].player_id),
        getPlayerNamefromId(war->armies[ATTACKER_INDEX].player_id));
        endWar(war);
        pillage(war->armies + DEFENDER_INDEX, war->armies + ATTACKER_INDEX, getPillageAmount());
    }
    else if (!war->armies[DEFENDER_INDEX].weapons &&
             !war->armies[DEFENDER_INDEX].vehicles)
    {
        war->status = WAR_COMPLETE;
        war->winner = ATTACKER_INDEX;
        fprintf(stdout, WAR_WINNER_FORMAT_STRING, WAR_NAME,
        getPlayerNamefromId(war->armies[ATTACKER_INDEX].player_id),
        getPlayerNamefromId(war->armies[DEFENDER_INDEX].player_id));
        endWar(war);
        pillage(war->armies + ATTACKER_INDEX, war->armies + DEFENDER_INDEX, getPillageAmount());
    }
}

void triggerBattle(War* war)
{
    pthread_rwlock_wrlock(war->lock);
    if(war->status == WAR_ACTIVE)
    {
        war->numBattles++;
        int winner = determineBattlewinner(war);
        if(winner)
        {
            fprintf(stdout, BATTLES_WINNER_FORMAT_STRING, WAR_NAME,
                getPlayerNamefromId(war->armies[DEFENDER_INDEX].player_id),
                getPlayerNamefromId(war->armies[ATTACKER_INDEX].player_id));
        }
        else
        {
            fprintf(stdout, BATTLES_WINNER_FORMAT_STRING, WAR_NAME,
                getPlayerNamefromId(war->armies[ATTACKER_INDEX].player_id),
                getPlayerNamefromId(war->armies[DEFENDER_INDEX].player_id));
        }

        killTroops(war-> armies + ATTACKER_INDEX,
                   war->armies + DEFENDER_INDEX,
                   winner);

        checkifWarOver(war);
    }
    pthread_rwlock_unlock(war->lock);
}

//Recovery functions
void populateArmies(War* war, Player* attacker, long attacker_weapons,
                              long attacker_vehicles, Player* defender,
                              long defender_weapons, long defender_vehicles)
{
    populateArmy(war->armies + ATTACKER_INDEX, attacker, attacker_weapons, attacker_vehicles);
    populateArmy(war->armies + DEFENDER_INDEX, defender, defender_weapons, defender_vehicles);
}

void setWarStatus(War* war, int status)
{
    war->status = status;
}

void setWarWinner(War* war, int winner)
{
    war->winner = winner;
}

void setWarNumBattles(War* war, int numBattles)
{
    war->numBattles = numBattles;
}
