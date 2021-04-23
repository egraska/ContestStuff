#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "string.h"
#include "resources.h"
#include "game_parameters.h"
#include "player_database.h"
#include "repeated_events.h"
#include "make_standings.h"
#include "display_market_values.h"


ResourceWallet* monitor_wallet;
MarketValues* marketValue;

const Resource resources[TOTAL_RESOURCES] =
        {{R_RUPYULARS, DEFAULT_BASE_CURRENCY_RESOURCE_MARKET_VALUE},
        {R_OIL, DEFAULT_BASE_BASIC_RESOURCE_MARKET_VALUE},
        {R_STEEL, DEFAULT_BASE_BASIC_RESOURCE_MARKET_VALUE},
        {R_PLASTIC, DEFAULT_BASE_BASIC_RESOURCE_MARKET_VALUE},
        {R_COPPER, DEFAULT_BASE_BASIC_RESOURCE_MARKET_VALUE},
        {R_GLASS, DEFAULT_BASE_BASIC_RESOURCE_MARKET_VALUE},
        {R_RUBBER, DEFAULT_BASE_BASIC_RESOURCE_MARKET_VALUE},
        {R_WEAPONS, DEFAULT_BASE_ASSEMBLED_RESOURCE_MARKET_VALUE},
        {R_COMPUTERS, DEFAULT_BASE_ASSEMBLED_RESOURCE_MARKET_VALUE},
        {R_VEHICLES, DEFAULT_BASE_ASSEMBLED_RESOURCE_MARKET_VALUE}};

char* resources_l[TOTAL_RESOURCES] =
        {R_RUPYULARS_L, R_OIL_L, R_STEEL_L, R_PLASTIC_L, 
         R_COPPER_L, R_GLASS_L, R_RUBBER_L, R_WEAPONS_L, R_COMPUTERS_L,
         R_VEHICLES_L};

MarketValues* initMarket()
{
    marketValue = malloc(sizeof(MarketValues));
    marketValue->lock = malloc(sizeof(pthread_rwlock_t));
    pthread_rwlock_init(marketValue->lock, NULL);
    for(int i = 0; i < TOTAL_RESOURCES; i++)
    {
        marketValue->values[i] = resources[i].baseValue;
        marketValue->demand_bonus[i] = 0;
    }
    return marketValue;
}

ResourceWallet* initMonitorWallet()
{
    monitor_wallet = malloc(sizeof(ResourceWallet));
    monitor_wallet->lock = malloc(sizeof(pthread_rwlock_t));
    if(pthread_rwlock_init(monitor_wallet->lock, NULL))
    {
        fprintf(stderr, "FATAL ERROR: Read Write lock for player failed init\n");
    }
    for(int j=0; j<TOTAL_RESOURCES; j++)
    {
        monitor_wallet->holdings[j].amount_available = getInitialResourcesWithMonitor();
        monitor_wallet->holdings[j].amount_commited = 0;
    }
    return monitor_wallet;
}

ResourceWallet* initPlayerWallet()
{
    ResourceWallet* ret = malloc(sizeof(ResourceWallet));
    ret->lock = malloc(sizeof(pthread_rwlock_t));
    if(pthread_rwlock_init(ret->lock, NULL))
    {
        fprintf(stderr, "FATAL ERROR: Read Write lock for player failed init\n");
    }
    for(int j=0; j<TOTAL_RESOURCES; j++)
    {
       ret->holdings[j].amount_available = getInitialResourcesWithPlayer();
       ret->holdings[j].amount_commited = 0;
    }
    return ret;
}

void deinitWallet(ResourceWallet* wallet)
{
    pthread_rwlock_destroy(wallet->lock);
    free(wallet->lock);
    free(wallet);
}

void _changeAmountAvailable(ResourceWallet* wallet, int index, long amount)
{

    wallet->holdings[index].amount_available += amount;
    if(wallet->holdings[index].amount_available < 0)
    {
        //this shouldnt happen, but protection is good.
        wallet->holdings[index].amount_available = 0;
    }
}

void _changeAmountCommited(ResourceWallet* wallet, int index, long amount)
{
    wallet->holdings[index].amount_commited += amount;
    if(wallet->holdings[index].amount_commited < 0)
    {
        //this shouldnt happen, but protection is good.
        wallet->holdings[index].amount_commited = 0;
    }
}


void _commitResources(ResourceWallet* wallet, int index, long amount)
{
    _changeAmountAvailable(wallet, index, -amount);
    _changeAmountCommited(wallet, index, amount);
}

int _hasResourcesbyIndex(ResourceWallet* wallet,
                        int index, long amount)
{
    return wallet->holdings[index].amount_available >= amount;
}

int _hasResources(ResourceWallet* wallet, ResourceTransfer* transfer)
{
    int ret = 1;
    for(int i =0; i < transfer->numberResources; i++)
    {
        if(!_hasResourcesbyIndex(wallet, transfer->resourceIndexes[i],
                                        transfer->resourceAmount[i]))
        {
            ret = 0;
            break;
        }
    }
    return ret;
}

int hasResources(ResourceWallet* wallet, ResourceTransfer* transfer)
{
    int ret = 0;
    pthread_rwlock_rdlock(wallet->lock);
    ret = _hasResources(wallet, transfer);
    pthread_rwlock_unlock(wallet->lock);
    return ret;
}

int commitResourcesForWar(ResourceWallet* wallet, long weapons, long vehicles)
{
    int ret = 0;
    pthread_rwlock_wrlock(wallet->lock);
    ret = _hasResourcesbyIndex(wallet, R_WEAPONS_INDEX, weapons) &&
          _hasResourcesbyIndex(wallet, R_VEHICLES_INDEX, vehicles);

    if(ret)
    {
        _commitResources(wallet, R_WEAPONS_INDEX, weapons);
        _commitResources(wallet, R_VEHICLES_INDEX, vehicles);
    }

    pthread_rwlock_unlock(wallet->lock);
    return ret;
}

void returnTroopsFromWar(ResourceWallet* wallet, long weapons, long vehicles)
{
    pthread_rwlock_wrlock(wallet->lock);
    _commitResources(wallet, R_WEAPONS_INDEX, -1 * weapons);
    _commitResources(wallet, R_VEHICLES_INDEX, -1 * vehicles);
    pthread_rwlock_unlock(wallet->lock);
}

void _killPercentageTroops(Army* army, float percentage, int isLoser)
{
    long deadTroops;
    if(isLoser)
    {
        deadTroops = ceil(army->weapons * percentage);
    }
    else
    {
        deadTroops = army->weapons * percentage;
    }
    //ensure at least one died
    if(!deadTroops) { deadTroops = 1; }
    _changeAmountCommited(army->wallet, R_WEAPONS_INDEX, -deadTroops);
    army->weapons -= deadTroops;
    if(army->weapons < 0) { army->weapons = 0; }

    if(isLoser)
    {
        deadTroops = ceil(army->vehicles * percentage);
    }
    else
    {
        deadTroops = army->vehicles * percentage;
    }
    //ensure at least one died
    if(!deadTroops) { deadTroops = 1; }
    _changeAmountCommited(army->wallet, R_VEHICLES_INDEX, -deadTroops);
    army->vehicles -= deadTroops;
    if(army->vehicles < 0) { army->vehicles = 0; }
}

void killTroops(Army* attacker, Army* defender, int didDefenderWin)
{
    if(attacker->player_id < defender->player_id)
    {
        pthread_rwlock_wrlock(attacker->wallet->lock);
        pthread_rwlock_wrlock(defender->wallet->lock);
    }
    else
    {
        pthread_rwlock_wrlock(defender->wallet->lock);
        pthread_rwlock_wrlock(attacker->wallet->lock);
    }

    if(didDefenderWin)
    {
        _killPercentageTroops(attacker, getLoserBattleLoss(), 1);
        _killPercentageTroops(defender, getWinnerBattleLoss(), 0);
    }
    else
    {
        _killPercentageTroops(defender, getLoserBattleLoss(), 1);
        _killPercentageTroops(attacker, getWinnerBattleLoss(), 0);
    }

    pthread_rwlock_unlock(attacker->wallet->lock);
    pthread_rwlock_unlock(defender->wallet->lock);
}

void pillage(Army* pillager, Army* loser, double pillage_factor)
{
    if(pillager->player_id < loser->player_id)
    {
        pthread_rwlock_wrlock(pillager->wallet->lock);
        pthread_rwlock_wrlock(loser->wallet->lock);
    }
    else
    {
        pthread_rwlock_wrlock(loser->wallet->lock);
        pthread_rwlock_wrlock(pillager->wallet->lock);
    }

    for(int i = 0; i < TOTAL_RESOURCES; i++)
    {
        long amount = loser->wallet->holdings[i].amount_available;
        amount *= pillage_factor;
        if(!amount && loser->wallet->holdings[i].amount_available)
        {
            amount = 1;
        }
        _changeAmountAvailable(loser->wallet, i, -amount);
        _changeAmountAvailable(pillager->wallet, i, amount);
    }

    pthread_rwlock_unlock(loser->wallet->lock);
    pthread_rwlock_unlock(pillager->wallet->lock);
}


int commitResourcesForTransfer(ResourceWallet* wallet, ResourceTransfer* transfer)
{
    int ret = 0;
    pthread_rwlock_wrlock(wallet->lock);
    ret = _hasResources(wallet, transfer);
    if(ret)
    {
        for(int i =0; i < transfer->numberResources; i++)
        {
            _commitResources(wallet, transfer->resourceIndexes[i],
                                     transfer->resourceAmount[i]);
        }
    }
    pthread_rwlock_unlock(wallet->lock);
    return ret;
}

int attemptAtmoicTrade(ResourceWallet* wallet, ResourceTransfer* receiving,
        ResourceTransfer* trading)
{
    int ret = 0;
    pthread_rwlock_wrlock(wallet->lock);
    ret = _hasResources(wallet, trading);
    if(ret == 1)
    {
        for(int i =0; i < receiving->numberResources; i++)
        {
            wallet->holdings[receiving->resourceIndexes[i]].amount_available +=
                receiving->resourceAmount[i];
        }
        for(int i =0; i < trading->numberResources; i++)
        {
            wallet->holdings[trading->resourceIndexes[i]].amount_available -=
                trading->resourceAmount[i];
        }
    }
    pthread_rwlock_unlock(wallet->lock);
    return ret;
}

int attemptTruce(ResourceWallet* wallet, ResourceTransfer* receiving, War* war, int truce_sender, int truce_receiver)
{
    int ret = 0;
    pthread_rwlock_wrlock(war->lock);
    if(war->status == WAR_ACTIVE)
    {
        pthread_rwlock_wrlock(wallet->lock);
        for(int i =0; i < receiving->numberResources; i++)
        {
            _changeAmountAvailable(wallet, receiving->resourceIndexes[i], receiving->resourceAmount[i]);
        }
        pthread_rwlock_unlock(wallet->lock);
        endWarForTruce(war, truce_sender, truce_receiver);
        ret = 1;
    }
    pthread_rwlock_unlock(war->lock);
    return ret;
}

void finalizeTrade(ResourceWallet* wallet, ResourceTransfer* receiving,
                   ResourceTransfer* committed)
{
    pthread_rwlock_wrlock(wallet->lock);
    for(int i =0; i < committed->numberResources; i++)
    {
        wallet->holdings[committed->resourceIndexes[i]].amount_commited -=
            committed->resourceAmount[i];
    }
    for(int i =0; i < receiving->numberResources; i++)
    {
        wallet->holdings[receiving->resourceIndexes[i]].amount_available +=
            receiving->resourceAmount[i];
    }
    pthread_rwlock_unlock(wallet->lock);
}

void finalizeResourceTransferForTruce(ResourceWallet* wallet, ResourceTransfer* committed, int accepted)
{
    pthread_rwlock_wrlock(wallet->lock);
    for(int i =0; i < committed->numberResources; i++)
    {
            _changeAmountCommited(wallet, committed->resourceIndexes[i], -(committed->resourceAmount[i]));
    }
    if(accepted != 1)
    {
        for(int i =0; i < committed->numberResources; i++)
        {
            _changeAmountAvailable(wallet, committed->resourceIndexes[i], committed->resourceAmount[i]);
        }
    }
    pthread_rwlock_unlock(wallet->lock);
}

ResourceWallet* deepCopyWallet(ResourceWallet* wallet)
{
    //We are assuuming the lock has already been locked
    ResourceWallet* ret = malloc(sizeof(ResourceWallet));
    memcpy(ret->holdings, wallet->holdings, sizeof(wallet->holdings));
    ret->lock = malloc(sizeof(pthread_rwlock_t));
    pthread_rwlock_init(ret->lock, NULL);
    return ret;
}

MarketValues* deepCopyMarketValues()
{
    MarketValues* values = malloc(sizeof(MarketValues));
    values->lock = malloc(sizeof(pthread_rwlock_t));
    pthread_rwlock_init(values->lock, NULL);
    memcpy(values->values, marketValue->values, sizeof(marketValue->values));
    memcpy(values->demand_bonus, marketValue->demand_bonus, sizeof(marketValue->demand_bonus));
    return values;
}

ResourceWallet* getMonitorWallet()
{
    return monitor_wallet;
}

void deinitMarketValues(MarketValues* values)
{
    pthread_rwlock_destroy(values->lock);
    free(values->lock);
    free(values);
}

long _getResourceTotal(int index, ResourceWallet* wallet)
{
    ResourceHeld* holding = &wallet->holdings[index];
    return holding->amount_commited +
        holding->amount_available;
}

int getResourceIndex(char* resourceName)
{
    int index = -1;
    for(int i=0; i<TOTAL_RESOURCES; i++)
    {
        if(!strcmp(resourceName, resources[i].name))
        {
            index = i;
            break;
        }
    }
    return index;
}

char* getResourceName(int resourceIndex)
{
    return resources[resourceIndex].name;
}

char* getResourceNameLowerCase(int resourceIndex)
{
    return resources_l[resourceIndex];
}

double getResourceValue(int resourceIndex)
{
    double ret;
    pthread_rwlock_rdlock(marketValue->lock);
    ret = marketValue->values[resourceIndex];
    pthread_rwlock_unlock(marketValue->lock);
    return ret;
}

double getResourceDemand(int resourceIndex)
{
    double ret;
    pthread_rwlock_rdlock(marketValue->lock);
    ret = marketValue->demand_bonus[resourceIndex];
    pthread_rwlock_unlock(marketValue->lock);
    return ret;
}

void getMarket(double market_values[], double market_demand[])
{
    pthread_rwlock_rdlock(marketValue->lock);
    for(int i=0; i<TOTAL_RESOURCES; i++)
    {
        market_values[i] = marketValue->values[i];
        market_demand[i] = marketValue->demand_bonus[i];
    }
    pthread_rwlock_unlock(marketValue->lock);
}

int getTotalResources()
{
    return TOTAL_RESOURCES;
}

int selectRandomResource(int maxValue)
{
    int eligibleResources = 0;
    int chosen_resource = rand();
    for(int i = 0; i < DISTRIBUTABLE_RESOURCES;i++)
    {
        if(marketValue->values[i] <= maxValue)
        {
            eligibleResources++;
        }
    }
    chosen_resource = (chosen_resource % eligibleResources) + 1;

    for(int i = 0; i < DISTRIBUTABLE_RESOURCES; i++)
    {
        if(marketValue->values[i] <= maxValue)
        {
            chosen_resource--;
            if(!chosen_resource)
            {
                chosen_resource = i;
                break;
            }
        }
    }
    return chosen_resource;
}

void distrubuteToWallet(ResourceWallet* wallet, int distribution_amount_left, int favored)
{
    int resource;
    if(favored)
    {
        int favoured_amount = distribution_amount_left/2;
        distribution_amount_left -= distribution_amount_left/2;
        int amount =  (favoured_amount) / marketValue->values[favored];
        favoured_amount -= (amount * marketValue->values[favored]);
        wallet->holdings[favored].amount_available += amount;
        wallet->holdings[R_RUPYULARS_INDEX].amount_available += favoured_amount;
    }

    while(distribution_amount_left > 0)
    {
        resource = selectRandomResource(distribution_amount_left);
        wallet->holdings[resource].amount_available += 1;
        distribution_amount_left -= marketValue->values[resource];
    }
}

void decayWallet(ResourceWallet* wallet)
{
    for(int i = 0; i < TOTAL_RESOURCES; i++)
    {
        long amount = _getResourceTotal(i, wallet);
        long decay = (amount * getInverseDecayRate()) / 100;

        if(amount && !decay)
        {
            //always decay one if possible
            decay = 1;
        }

        if( amount - decay < 0)
        {
            wallet->holdings[i].amount_available = 0;
            monitor_wallet->holdings[i].amount_available += amount;
        }
        else
        {
            wallet->holdings[i].amount_available = amount - decay;
            monitor_wallet->holdings[i].amount_available += decay;
        }
    }
}

void distributeResource()
{
    int total_players = getNumberOfPlayers();
    Player** players = malloc(sizeof(Player*) * total_players);
    total_players = getPlayerList(players, total_players);
    pthread_rwlock_rdlock(marketValue->lock);
    for(int i=0; i<total_players; i++)
    {
        pthread_rwlock_wrlock(players[i]->wallet->lock);
        time_t alive_time = getPlayerAliveTime(players[i]);
        if(alive_time && time(NULL) - alive_time >=
            getAliveTimeForResources())
        {
            fprintf(stdout, "%s: Player %s was awarded resources\n",
                RESOURCE_DISTRIBUTION_NAME, players[i]->name);

            distrubuteToWallet(players[i]->wallet,
                getDistributionAmountForPlayers(),
                players[i]->favoredResource);
        }
        else
        {
            fprintf(stdout, "%s: Player %s's resources were given to the monitor\n",
                RESOURCE_DISTRIBUTION_NAME, players[i]->name);

            decayWallet(players[i]->wallet);
        }

        pthread_rwlock_unlock(players[i]->wallet->lock);
    }
    pthread_rwlock_wrlock(monitor_wallet->lock);
    distrubuteToWallet(monitor_wallet, getDistributionAmountForMonitor(), 0);
    pthread_rwlock_unlock(monitor_wallet->lock);

    pthread_rwlock_unlock(marketValue->lock);
    free(players);
}

void printMarketValues()
{
    pthread_rwlock_rdlock(marketValue->lock);
    for(int i=0; i<TOTAL_RESOURCES; i++)
    {
        if(i == R_RUPYULARS_INDEX || i == R_COMPUTERS_INDEX || i == R_VEHICLES_INDEX)
        {
            printf("%s\t%f\n", getResourceName(i), marketValue->values[i]);
        }
        else
        {
            printf("%s\t\t%f\n", getResourceName(i), marketValue->values[i]);
        }
    }
    pthread_rwlock_unlock(marketValue->lock);
}

double calculateRatityBonus(double amount, double tierTotal, double tierLength)
{
    double rarity_bonus = 1;
    if(tierTotal)
    {
        rarity_bonus = ((tierLength + 1) - (tierLength * amount / tierTotal))/(tierLength);
    }
    return rarity_bonus;
}

double calculateMonopolyBonus(double squaredDev,
                              double numPlayers,
                              double totalResource,
                              double mean)
{
    int monopoly_factor = 1;
    double monopoly_bonus = 1;
    if(totalResource)
    {
        monopoly_bonus = (monopoly_factor * (squaredDev/totalResource)) + 1;
    }
    return monopoly_bonus;
}

void calculateMarketValue()
{
    long basic_resources = 0;
    long assembled_resources = 0;
    long resourcesTotals[TOTAL_RESOURCES];
    double rarityBonus[TOTAL_RESOURCES];
    double resource_means[TOTAL_RESOURCES];
    double monopolyBonus[TOTAL_RESOURCES];
    double total_squared_difference;
    double difference;

    for(int i = 0; i < TOTAL_RESOURCES; i++)
    {
        resourcesTotals[i] = 0;
    }

    int total_players = getNumberOfPlayers();
    ResourceWallet ** wallets = malloc(sizeof(ResourceWallet*) * total_players);
    total_players = getPlayerWallets(wallets, total_players);

    pthread_rwlock_wrlock(marketValue->lock);
    pthread_rwlock_rdlock(monitor_wallet->lock);
    for(int i=0; i<total_players; i++)
    {
        pthread_rwlock_rdlock(wallets[i]->lock);
    }

    //step 1: find the sum of each resource
    for(int j=0; j<TOTAL_RESOURCES; j++)
    {

        resourcesTotals[j] = 0;
        for(int i=0; i<total_players; i++)
        {
            resourcesTotals[j] += _getResourceTotal(j, wallets[i]);
        }
        resourcesTotals[j] += _getResourceTotal(j, monitor_wallet);
    }

    for(int i = 0; i < NUM_BASIC_RESOURCES; i++)
    {
        basic_resources += resourcesTotals[i + BASIC_RESOURCES_START];
    }

    for(int i = 0; i < NUM_ASSEMBLED_RESOURCES; i++)
    {
        assembled_resources += resourcesTotals[i + ASSEMBLED_RESOURCES_START];
    }

    for(int i = BASIC_RESOURCES_START; i < ASSEMBLED_RESOURCES_START; i++)
    {
        rarityBonus[i] = calculateRatityBonus(resourcesTotals[i],
                                              basic_resources,
                                              NUM_BASIC_RESOURCES);
    }

    for(int i = ASSEMBLED_RESOURCES_START; i < TOTAL_RESOURCES; i++)
    {
        rarityBonus[i] = calculateRatityBonus(resourcesTotals[i],
                                              assembled_resources,
                                              NUM_ASSEMBLED_RESOURCES);
    }

    for(int i = BASIC_RESOURCES_START; i < TOTAL_RESOURCES; i++)
    {
        resourcesTotals[i] -= _getResourceTotal(i, monitor_wallet);
        resource_means[i] = ((double)resourcesTotals[i]) / ((double)total_players);
    }

    for(int i = BASIC_RESOURCES_START; i < TOTAL_RESOURCES; i++)
    {
        total_squared_difference = 0;
        //difference = _getResourceTotal(i, monitor_wallet) - resource_means[i];

        //total_squared_difference += difference * difference;
        for(int j = 0; j < total_players; j++)
        {
            difference = _getResourceTotal(i,wallets[j]) - resource_means[i];

            total_squared_difference += difference * difference;
        }
        if(total_players)
        {
            monopolyBonus[i] = calculateMonopolyBonus(sqrt(total_squared_difference),
                                                      total_players,
                                                      resourcesTotals[i],
                                                      resource_means[i]);
        }
        else
        {
            monopolyBonus[i] = 1;
        }
    }

    for(int i = BASIC_RESOURCES_START; i < TOTAL_RESOURCES; i++)
    {
        marketValue->values[i] = resources[i].baseValue *
                                 monopolyBonus[i] *
                                 rarityBonus[i] +
                                 marketValue->demand_bonus[i];
        marketValue->demand_bonus[i] = 0;
        if(marketValue->values[i] < 0)
        {
            marketValue->values[i] = 0;
        }
    }

    for(int i = total_players - 1; i >= 0; i--)
    {
        pthread_rwlock_unlock(wallets[i]->lock);
    }
    pthread_rwlock_unlock(monitor_wallet->lock);
    pthread_rwlock_unlock(marketValue->lock);
    free(wallets);

    //Copying market values into an array and passing it to be displayed on a web page.
    double market_values[TOTAL_RESOURCES];
    double market_demand[TOTAL_RESOURCES];
    for(int i=0; i<TOTAL_RESOURCES; i++)
    {
        market_values[i] = getResourceValue(i);
        market_demand[i] = getResourceDemand(i);
    }
    displayMarketValues(market_values, market_demand, TOTAL_RESOURCES);
}

void updateMarketValue(ResourceTransfer* requested, ResourceTransfer* offered)
{
    long offered_value = getValueOfTransfer(offered);
    long requested_value = getValueOfTransfer(requested);
    long difference = 0;
    int greater = 0;
    if(offered_value != requested_value)
    {
        pthread_rwlock_wrlock(marketValue->lock);
        offered_value = _getValueOfTransfer(offered);
        requested_value = _getValueOfTransfer(requested);
        difference = (offered_value > requested_value) ? (offered_value - requested_value) : (requested_value - offered_value);
        greater = (offered_value > requested_value) ? 1 : 0;
        if(difference)
        {
            for(int i=0; i<offered->numberResources; i++)
            {
                int resource_index = offered->resourceIndexes[i];
                if(resource_index != R_RUPYULARS_INDEX)
                {
                    double change = (double)difference/(double)offered->resourceAmount[i];
                    if(greater)
                    {
                        marketValue->values[resource_index]-=change;
                        marketValue->demand_bonus[resource_index]-=change;
                    }
                    else
                    {
                        marketValue->values[resource_index]+=change;
                        marketValue->demand_bonus[resource_index]+=change;
                    }
                    if(marketValue->values[resource_index] < 1)
                    {
                        marketValue->values[resource_index] = 1;
                    }
                }
            }
            for(int i=0; i<requested->numberResources; i++)
            {
                int resource_index = requested->resourceIndexes[i];
                if(resource_index != R_RUPYULARS_INDEX)
                {
                    double change = (double)difference/(double)requested->resourceAmount[i];
                    if(greater)
                    {
                        marketValue->values[resource_index]+=change;
                        marketValue->demand_bonus[resource_index]+=change;
                    }
                    else
                    {
                        marketValue->values[resource_index]-=change;
                        marketValue->demand_bonus[resource_index]-=change;
                    }
                    if(marketValue->values[resource_index] < 1)
                    {
                        marketValue->values[resource_index] = 1;
                    }
                }
            }
        }
        pthread_rwlock_unlock(marketValue->lock);
    }
    //printMarketValues();
}

void updateScorecard()
{
    int total_players = getNumberOfPlayers();
    ResourceWallet ** wallets = malloc(sizeof(ResourceWallet*) * total_players);
    total_players = getPlayerWallets(wallets, total_players);
    int rankings[total_players];
    for(int i=0; i<total_players; i++)
    {
        rankings[i] = i;
    }
    int temp_rank;
    double temp_wealth;
    double wealth[total_players];
    for(int i=0; i<total_players; i++)
    {
        pthread_rwlock_rdlock(wallets[i]->lock);
    }
    pthread_rwlock_rdlock(marketValue->lock);
    for(int i=0; i<total_players; i++)
    {
        wealth[i] = 0;
        for(int j=0; j<TOTAL_RESOURCES; j++)
        {
            wealth[i] += (wallets[i]->holdings[j].amount_available * marketValue->values[j]);
        }
    }
    for(int i=0; i<total_players-1; i++)
    {
        for(int j=0; j<total_players-i-1; j++)
        {
            if(wealth[j] > wealth[j+1])
            {
                temp_rank = rankings[j];
                rankings[j] = rankings[j+1];
                rankings[j+1] = temp_rank;
                temp_wealth = wealth[j];
                wealth[j] = wealth[j+1];
                wealth[j+1] = temp_wealth;
            }
        }
    }
    pthread_rwlock_unlock(marketValue->lock);
    for(int i = total_players - 1; i >= 0; i--)
    {
        pthread_rwlock_unlock(wallets[i]->lock);
    }
    free(wallets);
    if(CURRENT_TIME >= getEndTime())
    {
        makeStandings(rankings, wealth, total_players, 1);
    }
    else
    {
        makeStandings(rankings, wealth, total_players, 0);
    }
}

long getValueOfTransfer(ResourceTransfer* transfer)
{
    long value = 0;
    pthread_rwlock_rdlock(marketValue->lock);
    for(int i = 0; i < transfer->numberResources;i++)
    {
        int index = transfer->resourceIndexes[i];
        value += marketValue->values[index] *
                 transfer->resourceAmount[i];

    }
    pthread_rwlock_unlock(marketValue->lock);
    return value;
}

long _getValueOfTransfer(ResourceTransfer* transfer)
{
    long value = 0;
    for(int i = 0; i < transfer->numberResources;i++)
    {
        int index = transfer->resourceIndexes[i];
        value += marketValue->values[index] *
                 transfer->resourceAmount[i];

    }
    return value;
}

int transferPlayerResourceToMonitor(ResourceWallet * playerWallet, int resource_index, long resource_amount)
{
    int ret = 0;
    pthread_rwlock_wrlock(monitor_wallet->lock);
    pthread_rwlock_wrlock(playerWallet->lock);
    if(playerWallet->holdings[resource_index].amount_available >= resource_amount)
    {
        monitor_wallet->holdings[resource_index].amount_available += resource_amount;
        playerWallet->holdings[resource_index].amount_available -= resource_amount;
        ret = 1;
    }
    pthread_rwlock_unlock(playerWallet->lock);
    pthread_rwlock_unlock(monitor_wallet->lock);
    return ret;
}

int consumePlayerComputersForInformation(ResourceWallet * playerWallet, long computers_used)
{
    int ret = 0;
    pthread_rwlock_wrlock(playerWallet->lock);
    if(playerWallet->holdings[R_COMPUTERS_INDEX].amount_available >= computers_used)
    {
        playerWallet->holdings[R_COMPUTERS_INDEX].amount_available -= computers_used;
        ret = 1;
    }
    pthread_rwlock_unlock(playerWallet->lock);
    return ret;
}

int synthesizeResource(ResourceWallet * playerWallet, int resource_index, int amount)
{
    int ret = 0;

    int res_1;
    int res_2;
    int res_3;
    int res_4 = 0;

    if(resource_index == R_WEAPONS_INDEX)
    {
        res_1 = WEAPON_PART_1_RESOURCE_INDEX;
        res_2 = WEAPON_PART_2_RESOURCE_INDEX;
        res_3 = WEAPON_PART_3_RESOURCE_INDEX;
    }
    else if(resource_index == R_COMPUTERS_INDEX)
    {
        res_1 = COMPUTER_PART_1_RESOURCE_INDEX;
        res_2 = COMPUTER_PART_2_RESOURCE_INDEX;
        res_3 = COMPUTER_PART_3_RESOURCE_INDEX;
    }
    else if(resource_index == R_VEHICLES_INDEX)
    {
        res_1 = VEHICLE_PART_1_RESOURCE_INDEX;
        res_2 = VEHICLE_PART_2_RESOURCE_INDEX;
        res_3 = VEHICLE_PART_3_RESOURCE_INDEX;
        res_4 = VEHICLE_PART_4_RESOURCE_INDEX;
    }

    int amount_required = getResourcesReqdToSyn() * amount;
    if(amount_required < amount)
    {
        return 0;
    }

    pthread_rwlock_wrlock(playerWallet->lock);
    if(playerWallet->holdings[res_1].amount_available >= amount_required
       && playerWallet->holdings[res_2].amount_available >= amount_required
       && playerWallet->holdings[res_3].amount_available >= amount_required)
    {
        ret = 1;
        if(res_4)
        {
            if(playerWallet->holdings[res_4].amount_available >= amount_required)
            {
                playerWallet->holdings[res_4].amount_available -= amount_required;
            }
            else
            {
                ret = 0;
            }
        }
        if(ret)
        {
            playerWallet->holdings[res_1].amount_available -= amount_required;
            playerWallet->holdings[res_2].amount_available -= amount_required;
            playerWallet->holdings[res_3].amount_available -= amount_required;

            playerWallet->holdings[resource_index].amount_available += amount;
        }
    }
    pthread_rwlock_unlock(playerWallet->lock);
    return ret;
}

//Recovery functions
void setResourceValue(int resourceIndex, double value)
{
    //This method should only be called from the recovery function.
    //It does not use a lock, since only the main thread uses this function, before the game even begins.
    marketValue->values[resourceIndex] = value;
}

void setResourceDemand(int resourceIndex, double value)
{
    //This method should only be called from the recovery function.
    //It does not use a lock, since only the main thread uses this function, before the game even begins.
    marketValue->demand_bonus[resourceIndex] = value;
}

void setMonitorResource(int resourceIndex, long amount_available, long amount_committed)
{
    //This method should only be called from the recovery function.
    //It does not use a lock, since only the main thread uses this function, before the game even begins.
    monitor_wallet->holdings[resourceIndex].amount_available = amount_available;
    monitor_wallet->holdings[resourceIndex].amount_commited = amount_committed;
}
