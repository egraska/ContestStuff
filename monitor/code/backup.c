#include "player_database.h"
#include "master_database.h"
#include "resources.h"
#include "backup.h"
#include <stdio.h>

void copyDatabaseToFile()
{
    MasterDatabase* copy = CopyMasterDatabase();
    FILE *db_file;

    //Copy Market Values to file
    db_file = fopen (MARKET_DB, "w");
    if(db_file)
    {
        for(int i = 0; i < getTotalResources()-1; i++)
        {
            fprintf(db_file, "%f ", copy->marketValues->values[i]);
            fprintf(db_file, "%f ", copy->marketValues->demand_bonus[i]);
        }
        fprintf(db_file, "%f ", copy->marketValues->values[getTotalResources()-1]);
        fprintf(db_file, "%f", copy->marketValues->demand_bonus[getTotalResources()-1]);
        fclose(db_file);
    }

    //Copy Player DB to file (only if no. of players > 0)
    if(copy->players->numberOfPlayers)
    {
        db_file = fopen (PLAYER_DB, "w");
        if(db_file)
        {
            int total_players = copy->players->numberOfPlayers;
            for(int i = 0; i < total_players-1; i++)
            {
                Player* player = copy->players->players[i];
                fprintf(db_file, "%d %s %s %s ", player->serialNo, player->name, player->password, player->cookie);
                fprintf(db_file, "%s %s ", player->hostPort.host, player->hostPort.port);
                fprintf(db_file, "%s %s %s ", player->rsa_key.exp, player->rsa_key.mod, player->rsa_key.certificate);
                fprintf(db_file, "%d ", player->favoredResource);
                fprintf(db_file, "%d %d %d %d %d %d ", player->trades_done, player->wars_fought, player->wars_initialized, player->wars_won, player->truce_offered, player->truce_accepted);
                for(int j = 0; j < getTotalResources()-1; j++)
                {
                    fprintf(db_file, "%ld %ld ", player->wallet->holdings[j].amount_available, player->wallet->holdings[j].amount_commited);
                }
                fprintf(db_file, "%ld %ld", player->wallet->holdings[getTotalResources()-1].amount_available, player->wallet->holdings[getTotalResources()-1].amount_commited);
                fprintf(db_file, "\n");
            }
            Player* player = copy->players->players[total_players-1];
            fprintf(db_file, "%d %s %s %s ", player->serialNo, player->name, player->password, player->cookie);
            fprintf(db_file, "%s %s ", player->hostPort.host, player->hostPort.port);
            fprintf(db_file, "%s %s %s ", player->rsa_key.exp, player->rsa_key.mod, player->rsa_key.certificate);
            fprintf(db_file, "%d ", player->favoredResource);
            fprintf(db_file, "%d %d %d %d %d %d ", player->trades_done, player->wars_fought, player->wars_initialized, player->wars_won, player->truce_offered, player->truce_accepted);
            for(int j = 0; j < getTotalResources()-1; j++)
            {
                fprintf(db_file, "%ld %ld ", player->wallet->holdings[j].amount_available, player->wallet->holdings[j].amount_commited);
            }
            fprintf(db_file, "%ld %ld", player->wallet->holdings[getTotalResources()-1].amount_available, player->wallet->holdings[getTotalResources()-1].amount_commited);
            fclose(db_file);
        }
    }

    //Copy Monitor Wallet to file
    db_file = fopen (MONITOR_DB, "w");
    if(db_file)
    {
        for(int i = 0; i < getTotalResources()-1; i++)
        {
            fprintf(db_file, "%ld %ld ", copy->monitorWallet->holdings[i].amount_available, copy->monitorWallet->holdings[i].amount_commited);
        }
        fprintf(db_file, "%ld %ld", copy->monitorWallet->holdings[getTotalResources()-1].amount_available, copy->monitorWallet->holdings[getTotalResources()-1].amount_commited);
        fclose(db_file);
    }

    //Copy War DB to file (only if no. of wars > 0)
    if(copy->wars->numWars)
    {
        db_file = fopen (WAR_DB, "w");
        if(db_file)
        {
            int total_wars = copy->wars->numWars;
            for(int i = 0; i < total_wars-1; i++)
            {
                War* war = copy->wars->wars[i];
                fprintf(db_file, "%d %d %d ", war->winner, war->status, war->numBattles);
                fprintf(db_file, "%d %ld %ld ", war->armies[0].player_id, war->armies[0].weapons, war->armies[0].vehicles);
                fprintf(db_file, "%d %ld %ld", war->armies[1].player_id, war->armies[1].weapons, war->armies[1].vehicles);
                fprintf(db_file, "\n");
            }
            War* war = copy->wars->wars[total_wars-1];
            fprintf(db_file, "%d %d %d ", war->winner, war->status, war->numBattles);
            fprintf(db_file, "%d %ld %ld ", war->armies[0].player_id, war->armies[0].weapons, war->armies[0].vehicles);
            fprintf(db_file, "%d %ld %ld", war->armies[1].player_id, war->armies[1].weapons, war->armies[1].vehicles);
            fclose(db_file);
        }
    }

    //Free Master DB copy
    freeMasterDatabase(copy);
}
