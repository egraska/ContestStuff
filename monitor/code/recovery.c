#include "backup.h"
#include "resources.h"
#include "util.h"
#include "player_database.h"
#include "war.h"
#include "war_database.h"
#include <stdio.h>
#include <string.h>

#define NULL_STR "(null)"

void loadMarket()
{
    char* line = NULL;
    size_t line_len = 0;
    ssize_t bytes_read;

    FILE* backup;
    backup = fopen(MARKET_DB, "r");

    if(backup)
    {
        if((bytes_read = getline(&line, &line_len, backup)) != -1)
        {
            for(int i = 0; i < getTotalResources()-1; i++)
            {
                char* value = getNextToken(&line, SEPERATOR_STR);
                setResourceValue(i, atof(value));
                char* demand_value = getNextToken(&line, SEPERATOR_STR);
                setResourceDemand(i, atof(demand_value));
            }
            char* value = getNextToken(&line, SEPERATOR_STR);
            setResourceValue(getTotalResources()-1, atof(value));
            char* demand_value = getNextToken(&line, NEWLINE_STR);
            setResourceDemand(getTotalResources()-1, atof(demand_value));
        }
        else
        {
            //fprintf(stderr, "DB Empty\n");
        }
        fclose(backup);
    }
}

void loadMonitorResources()
{
    char* line = NULL;
    size_t line_len = 0;
    ssize_t bytes_read;

    FILE* backup;
    backup = fopen(MONITOR_DB, "r");

    if(backup)
    {
        if((bytes_read = getline(&line, &line_len, backup)) != -1)
        {
            for(int i = 0; i < getTotalResources()-1; i++)
            {
                char* value_available = getNextToken(&line, SEPERATOR_STR);
                char* value_committed = getNextToken(&line, SEPERATOR_STR);
                setMonitorResource(i, atol(value_available), atol(value_committed));
            }
            char* value_available = getNextToken(&line, SEPERATOR_STR);
            char* value_committed = getNextToken(&line, NEWLINE_STR);
            setMonitorResource(getTotalResources()-1, atol(value_available), atol(value_committed));
        }
        else
        {
            //fprintf(stderr, "DB Empty\n");
        }
        fclose(backup);
    }
}

void loadPlayerDatabase()
{
    int change_db = 0;

    int serialNo;
    char* name;
    char* password;
    char* cookie;
    char* host;
    char* port;
    char* exp;
    char* mod;
    char* certificate;
    int favoredResource;
    int trades_done;
    int wars_fought;
    int wars_initialized;
    int wars_won;
    int truce_offered;
    int truce_accepted;

    char* line = NULL;
    size_t line_len = 0;
    ssize_t bytes_read;

    FILE* backup;
    backup = fopen(PLAYER_DB, "r");

    if(backup)
    {
        //If db is static, change it to dynamic to load players from db to game.
        if(getPlayerDatabaseState() == STATIC_PLAYER_DATABASE)
        {
            setPlayerDatabaseState(DYNAMIC_PLAYER_DATABASE);
            change_db = 1;
        }
        while((bytes_read = getline(&line, &line_len, backup)) != -1)
        {
            char* pointer_to_line = line;

            serialNo = atoi(getNextToken(&line, SEPERATOR_STR));
            name = getNextToken(&line, SEPERATOR_STR);

            Player* player = loadPlayerFromFile(name);
            if(!player)
            {
                fprintf(stderr, "Player db cannot be loaded. Please check MAX_PLAYER parameter.");
                exit(2);
            }

            password = getNextToken(&line, SEPERATOR_STR);
            cookie = getNextToken(&line, SEPERATOR_STR);

            host = getNextToken(&line, SEPERATOR_STR);
            port = getNextToken(&line, SEPERATOR_STR);

            exp = getNextToken(&line, SEPERATOR_STR);
            mod = getNextToken(&line, SEPERATOR_STR);
            certificate = getNextToken(&line, SEPERATOR_STR);

            favoredResource = atoi(getNextToken(&line, SEPERATOR_STR));

            trades_done = atoi(getNextToken(&line, SEPERATOR_STR));
            wars_fought = atoi(getNextToken(&line, SEPERATOR_STR));
            wars_initialized = atoi(getNextToken(&line, SEPERATOR_STR));
            wars_won = atoi(getNextToken(&line, SEPERATOR_STR));
            truce_offered = atoi(getNextToken(&line, SEPERATOR_STR));
            truce_accepted = atoi(getNextToken(&line, SEPERATOR_STR));

            setPlayerSerialNo(player, serialNo);
            if(strcmp(password, NULL_STR))
            {
                handleReceivedPassword(player, password, cookie);
            }
            if(strcmp(host, NULL_STR) && strcmp(port, NULL_STR))
            {
                setPlayerHostPort(player, host, port);
                setPlayerAliveTime(player);
            }
            if(strcmp(exp, NULL_STR) && strcmp(mod, NULL_STR) && strcmp(certificate, NULL_STR))
            {
                setPlayerRSAKey(player, certificate, exp, mod);
            }
            setFavouredResource(player, favoredResource);
            setPlayerTradesDone(player, trades_done);
            setPlayerWarsFought(player, wars_fought);
            setPlayerWarsInitialized(player, wars_initialized);
            setPlayerWarsWon(player, wars_won);
            setPlayerTruceOffered(player, truce_offered);
            setPlayerTruceAccepted(player, truce_accepted);

            for(int i = 0; i < getTotalResources()-1; i++)
            {
                char* value_available = getNextToken(&line, SEPERATOR_STR);
                char* value_committed = getNextToken(&line, SEPERATOR_STR);
                setPlayerResource(player, i, atol(value_available), atol(value_committed));
            }
            char* value_available = getNextToken(&line, SEPERATOR_STR);
            char* value_committed = getNextToken(&line, NEWLINE_STR);
            setPlayerResource(player, getTotalResources()-1, atol(value_available), atol(value_committed));

            line = pointer_to_line;
        }
        //If db was changed from static to dynamic, change it back to static
        if(change_db)
        {
            setPlayerDatabaseState(STATIC_PLAYER_DATABASE);
            change_db = 0;
        }
        fclose(backup);
    }
}

void loadWarDatabase()
{
    int winner;
    int status;
    int numBattles;
    int attacker_no;
    long attacker_weapons;
    long attacker_vehicles;
    int defender_no;
    long defender_weapons;
    long defender_vehicles;

    char* line = NULL;
    size_t line_len = 0;
    ssize_t bytes_read;

    FILE* backup;
    backup = fopen(WAR_DB, "r");

    if(backup)
    {
        while((bytes_read = getline(&line, &line_len, backup)) != -1)
        {
            char* pointer_to_line = line;

            winner = atoi(getNextToken(&line, SEPERATOR_STR));
            status = atoi(getNextToken(&line, SEPERATOR_STR));
            numBattles = atoi(getNextToken(&line, SEPERATOR_STR));
            attacker_no = atoi(getNextToken(&line, SEPERATOR_STR));
            attacker_weapons = atol(getNextToken(&line, SEPERATOR_STR));
            attacker_vehicles = atol(getNextToken(&line, SEPERATOR_STR));
            defender_no = atoi(getNextToken(&line, SEPERATOR_STR));
            defender_weapons = atol(getNextToken(&line, SEPERATOR_STR));
            defender_vehicles = atol(getNextToken(&line, NEWLINE_STR));

            Player* attacker = getPlayerFromSerialNo(attacker_no);
            Player* defender = getPlayerFromSerialNo(defender_no);

            War* war = initWar(attacker, defender);
            addWar(war);
            populateArmies(war, attacker, attacker_weapons, attacker_vehicles, defender, defender_weapons, defender_vehicles);

            setWarStatus(war, status);
            setWarWinner(war, winner);
            setWarNumBattles(war, numBattles);

            line = pointer_to_line;
        }
        fclose(backup);
    }
}
