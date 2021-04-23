#ifndef _MONITOR_RESOURCES_H
#define _MONITOR_RESOURCES_H

#define R_RUPYULARS_L                           "Rupyulars"
#define R_OIL_L                                 "Oil"
#define R_STEEL_L                               "Steel"
#define R_PLASTIC_L                             "Plastic"
#define R_COPPER_L                              "Copper"
#define R_GLASS_L                               "Glass"
#define R_RUBBER_L                              "Rubber"
#define R_WEAPONS_L                             "Weapons"
#define R_COMPUTERS_L                           "Computers"
#define R_VEHICLES_L                            "Vehicles"

#define R_RUPYULARS                             "RUPYULARS"
#define R_OIL                                   "OIL"
#define R_STEEL                                 "STEEL"
#define R_PLASTIC                               "PLASTIC"
#define R_COPPER                                "COPPER"
#define R_GLASS                                 "GLASS"
#define R_RUBBER                                "RUBBER"
#define R_WEAPONS                               "WEAPONS"
#define R_COMPUTERS                             "COMPUTERS"
#define R_VEHICLES                              "VEHICLES"

#define R_RUPYULARS_INDEX                       0
#define R_OIL_INDEX                             1
#define R_STEEL_INDEX                           2
#define R_PLASTIC_INDEX                         3
#define R_COPPER_INDEX                          4
#define R_GLASS_INDEX                           5
#define R_RUBBER_INDEX                          6
#define R_WEAPONS_INDEX                         7
#define R_COMPUTERS_INDEX                       8
#define R_VEHICLES_INDEX                        9

#define COMPUTER_PART_1_RESOURCE_INDEX          R_PLASTIC_INDEX
#define COMPUTER_PART_2_RESOURCE_INDEX          R_COPPER_INDEX
#define COMPUTER_PART_3_RESOURCE_INDEX          R_GLASS_INDEX

#define WEAPON_PART_1_RESOURCE_INDEX            R_OIL_INDEX
#define WEAPON_PART_2_RESOURCE_INDEX            R_STEEL_INDEX
#define WEAPON_PART_3_RESOURCE_INDEX            R_PLASTIC_INDEX

#define VEHICLE_PART_1_RESOURCE_INDEX           R_OIL_INDEX
#define VEHICLE_PART_2_RESOURCE_INDEX           R_STEEL_INDEX
#define VEHICLE_PART_3_RESOURCE_INDEX           R_GLASS_INDEX
#define VEHICLE_PART_4_RESOURCE_INDEX           R_RUBBER_INDEX

#define CURRENCY_RESOURCES_START                0
#define NUM_CURRENCY_RESOURCES                  1
#define BASIC_RESOURCES_START                   (CURRENCY_RESOURCES_START + NUM_CURRENCY_RESOURCES)
#define NUM_BASIC_RESOURCES                     6
#define ASSEMBLED_RESOURCES_START               (BASIC_RESOURCES_START + NUM_BASIC_RESOURCES)
#define NUM_ASSEMBLED_RESOURCES                 3

#define TOTAL_RESOURCES                         (NUM_CURRENCY_RESOURCES + NUM_BASIC_RESOURCES + NUM_ASSEMBLED_RESOURCES)
#define DISTRIBUTABLE_RESOURCES                 7

#include <pthread.h>
#include "custom_string.h"
#include "resource_transfer.h"
#include "war.h"

typedef struct ResourceTransfer ResourceTransfer;
typedef struct Army Army;
typedef struct War War;

typedef struct ResourceHeld
{
    long amount_available;
    long amount_commited;
} ResourceHeld;

typedef struct ResourceWallet
{
    ResourceHeld holdings[TOTAL_RESOURCES];
    pthread_rwlock_t* lock;
} ResourceWallet;

typedef struct Resource
{
    char* name;
    double baseValue;
} Resource;

typedef struct MarketValues
{
    double values[TOTAL_RESOURCES];
    double demand_bonus[TOTAL_RESOURCES];
    pthread_rwlock_t* lock;
} MarketValues;

MarketValues* initMarket();
MarketValues* deepCopyMarketValues();
void deinitMarketValues(MarketValues* values);

ResourceWallet* initMonitorWallet();
ResourceWallet* initPlayerWallet();
ResourceWallet* deepCopyWallet(ResourceWallet* wallet);
ResourceWallet* getMonitorWallet();
void deinitWallet(ResourceWallet* wallet);
int hasResources(ResourceWallet* wallet, ResourceTransfer* transfer);
int commitResourcesForTransfer(ResourceWallet* wallet, ResourceTransfer* transfer);
int attemptAtmoicTrade(ResourceWallet* wallet, ResourceTransfer* receiving,
        ResourceTransfer* trading);
int attemptTruce(ResourceWallet* wallet, ResourceTransfer* receiving, War* war, int truce_sender, int truce_receiver);
void finalizeTrade(ResourceWallet* wallet, ResourceTransfer* receiving,
                   ResourceTransfer* commited);
void finalizeResourceTransferForTruce(ResourceWallet* wallet,
                   ResourceTransfer* committed, int accepted);

int commitResourcesForWar(ResourceWallet* wallet, long weapons, long vehicles);
void returnTroopsFromWar(ResourceWallet* wallet, long weapons, long vehicles);
void killTroops(Army* attacker, Army* defender, int didDefenderWin);
void pillage(Army* pillager, Army* loser, double pillage_factor);

int getResourceIndex(char* resourceName);
char* getResourceName(int resourceIndex);
char* getResourceNameLowerCase(int resourceIndex);
double getResourceValue(int resourceIndex);
double getResourceDemand(int resourceIndex);
void getMarket(double market_values[], double market_demand[]);
int getTotalResources();

long getValueOfTransfer(ResourceTransfer* transfer);
long _getValueOfTransfer(ResourceTransfer* transfer);

void distributeResource();
void calculateMarketValue();
void updateMarketValue(ResourceTransfer* requested, ResourceTransfer* offered);
void updateScorecard();
int transferPlayerResourceToMonitor(ResourceWallet * playerWallet, int resource_index, long resource_amount);
int consumePlayerComputersForInformation(ResourceWallet * playerWallet, long computers_used);
int synthesizeResource(ResourceWallet * playerWallet, int resource_index, int amount);

//Recovery functions
void setResourceValue(int resourceIndex, double value);
void setResourceDemand(int resourceIndex, double value);
void setMonitorResource(int resourceIndex, long amount_available, long amount_committed);

#endif
