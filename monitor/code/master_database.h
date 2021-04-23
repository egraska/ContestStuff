#ifndef _MASTER_DATABASE_H
#define _MASTER_DATABASE_H

#include "player_database.h"
#include "resources.h"
#include "war_database.h"

typedef struct{
	MarketValues* marketValues;
	ResourceWallet* monitorWallet;
	PlayerDatabase* players;
	WarDatabase* wars;
} MasterDatabase;

void initilizeGame();
void restoreGameFromFile(char* filePath);
MasterDatabase* CopyMasterDatabase();
void freeMasterDatabase(MasterDatabase* database);

#endif
