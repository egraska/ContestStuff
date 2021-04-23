#include <stdio.h>
#include "cbor/cbor.h"
#include "player_database.h"
#include "master_database.h"

#define DEFAULT_SPACE               100
#define DEFAULT_SPACE_PER_PLAYER    400

#define CBOR_MAP_PLAYER_TAGS        13
#define CBOR_MAP_MASTER_TAGS        4
#define CBOR_MAP_ARMY_TAGS          3

#define CBOR_MAP_WAR_TAGS           4
#define CBOR_MAP_WAR_DATABASE_TAGS  1
#define CBOR_ARRAY_ARMIES_TAGS      2

#define CBOR_MONITOR_TAG            "Monitor"
#define CBOR_VALUES_TAG             "Market"

#define CBOR_PLAYERS_TAG            "Players"
#define CBOR_NAME_TAG               "Name"
#define CBOR_PASSWORD_TAG           "Password"
#define CBOR_HOST_TAG               "Host"
#define CBOR_PORT_TAG               "Port"
#define CBOR_COOKIE_TAG             "Cookie"
#define CBOR_CERTIFICATE_TAG        "Certificate"
#define CBOR_EXP_TAG                "Exp"
#define CBOR_MOD_TAG                "Mod"
#define CBOR_ALIVE_TIME             "Alive Start"
#define CBOR_RANDOM_TIME            "Last Random"
#define CBOR_COMMANDS_USED_TAG      "Commands Used"
#define CBOR_WALLET_TAG             "Wallet"
#define CBOR_FAVORED_TAG            "Favored Resource"

#define CBOR_WAR_DATABASE_TAG       "War Database"
#define CBOR_WARS_TAG               "Wars"
#define CBOR_ARMIES_TAG             "Armies"
#define CBOR_WAR_STATUS_TAG         "Status"
#define CBOR_WAR_NUM_BATTLES_TAG    "Battles"
#define CBOR_WAR_WINNER_TAG         "Winner"


void safeEncodeString(CborEncoder* parentEncoder, char* encoding)
{
    if(encoding)
    {
        cbor_encode_text_stringz(parentEncoder, encoding);
    }
    else
    {
        cbor_encode_null(parentEncoder);
    }
}

void writeBufferToFile(char* filepath, uint8_t* buffer, size_t size)
{
    FILE* pFile;
    pFile = fopen(filepath, "wb");
    if(pFile)
    {
        fwrite(buffer, size, 1, pFile);
    }
    fclose(pFile);
}

void serializeWallet(CborEncoder* parentEncoder, ResourceWallet* wallet)
{
    CborEncoder mapEncoder;

    cbor_encoder_create_map(parentEncoder, &mapEncoder, TOTAL_RESOURCES);

    for(int i = 0; i < TOTAL_RESOURCES; i++)
    {
        cbor_encode_text_stringz(&mapEncoder, getResourceName(i));
        cbor_encode_int(&mapEncoder, wallet->holdings[i].amount_available +
                                       wallet->holdings[i].amount_commited );
    }

    cbor_encoder_close_container(parentEncoder, &mapEncoder);
}

void serializePlayer(CborEncoder* parentEncoder, Player* player)
{
    CborEncoder mapEncoder;
    cbor_encoder_create_map(parentEncoder, &mapEncoder, CBOR_MAP_PLAYER_TAGS);

    cbor_encode_text_stringz(&mapEncoder, CBOR_NAME_TAG);
    cbor_encode_text_stringz(&mapEncoder, player->name);

    cbor_encode_text_stringz(&mapEncoder, CBOR_PASSWORD_TAG);
    safeEncodeString(&mapEncoder, player->password);

    cbor_encode_text_stringz(&mapEncoder, CBOR_HOST_TAG);
    safeEncodeString(&mapEncoder, player->hostPort.host);

    cbor_encode_text_stringz(&mapEncoder, CBOR_PORT_TAG);
    safeEncodeString(&mapEncoder, player->hostPort.port);

    cbor_encode_text_stringz(&mapEncoder, CBOR_COOKIE_TAG);
    safeEncodeString(&mapEncoder, player->cookie);

    cbor_encode_text_stringz(&mapEncoder, CBOR_CERTIFICATE_TAG);
    safeEncodeString(&mapEncoder, player->rsa_key.certificate);

    cbor_encode_text_stringz(&mapEncoder, CBOR_EXP_TAG);
    safeEncodeString(&mapEncoder, player->rsa_key.exp);

    cbor_encode_text_stringz(&mapEncoder, CBOR_MOD_TAG);
    safeEncodeString(&mapEncoder, player->rsa_key.mod);

    cbor_encode_text_stringz(&mapEncoder, CBOR_ALIVE_TIME);
    cbor_encode_int(&mapEncoder, player->alive_start_time);

    cbor_encode_text_stringz(&mapEncoder, CBOR_RANDOM_TIME);
    cbor_encode_int(&mapEncoder, player->random_host_port_requested);

    cbor_encode_text_stringz(&mapEncoder, CBOR_COMMANDS_USED_TAG);
    cbor_encode_int(&mapEncoder, player->commands_used);

    cbor_encode_text_stringz(&mapEncoder, CBOR_WALLET_TAG);
    serializeWallet(&mapEncoder, player->wallet);

    cbor_encode_text_stringz(&mapEncoder, CBOR_FAVORED_TAG);
    cbor_encode_int(&mapEncoder, player->favoredResource);

    cbor_encoder_close_container(parentEncoder, &mapEncoder);
}

void serializeMarket(CborEncoder* parentEncoder, MarketValues* values)
{
    CborEncoder mapEncoder;
    cbor_encoder_create_map(parentEncoder, &mapEncoder, TOTAL_RESOURCES);

    for(int i = 0; i < TOTAL_RESOURCES; i++)
    {
        cbor_encode_text_stringz(&mapEncoder, getResourceName(i));
        cbor_encode_int(&mapEncoder, values->values[i]);
    }

    cbor_encoder_close_container(parentEncoder, &mapEncoder);
}

void serializePlayerDatabase(CborEncoder* parentEncoder,
                            PlayerDatabase* database)
{
    CborEncoder listEncoder;
    cbor_encoder_create_array(parentEncoder, &listEncoder,
        database->numberOfPlayers);

    for(int i = 0; i < database->numberOfPlayers; i++)
    {
        serializePlayer(&listEncoder, database->players[i]);
    }
    cbor_encoder_close_container(parentEncoder, &listEncoder);
}

void serializeArmy(CborEncoder* parentEncoder, Army* army)
{
    CborEncoder mapEncoder;
    cbor_encoder_create_map(parentEncoder, &mapEncoder, CBOR_MAP_ARMY_TAGS);

    cbor_encode_text_stringz(&mapEncoder, CBOR_NAME_TAG);
    cbor_encode_text_stringz(&mapEncoder, getPlayerNamefromId(army->player_id));

    cbor_encode_text_stringz(&mapEncoder, R_WEAPONS);
    cbor_encode_int(&mapEncoder, army->weapons);

    cbor_encode_text_stringz(&mapEncoder, R_VEHICLES);
    cbor_encode_int(&mapEncoder, army->vehicles);

    cbor_encoder_close_container(parentEncoder, &mapEncoder);
}

void serializeWar(CborEncoder* parentEncoder, War* war)
{
    CborEncoder mapEncoder, listEncoder;
    cbor_encoder_create_map(parentEncoder, &mapEncoder, CBOR_MAP_WAR_TAGS);


    cbor_encode_text_stringz(&mapEncoder, CBOR_WAR_STATUS_TAG);
    cbor_encode_int(&mapEncoder, war->status);


    cbor_encode_text_stringz(&mapEncoder, CBOR_WAR_WINNER_TAG);
    cbor_encode_int(&mapEncoder, war->winner);


    cbor_encode_text_stringz(&mapEncoder, CBOR_WAR_NUM_BATTLES_TAG);
    cbor_encode_int(&mapEncoder, war->numBattles);

    cbor_encode_text_stringz(&mapEncoder, CBOR_ARMIES_TAG);
    cbor_encoder_create_array(&mapEncoder, &listEncoder, CBOR_ARRAY_ARMIES_TAGS);

    serializeArmy(&listEncoder, war->armies);
    serializeArmy(&listEncoder, war->armies + 1);

    cbor_encoder_close_container(&mapEncoder, &listEncoder);
    cbor_encoder_close_container(parentEncoder, &listEncoder);
}

void serializeWarDatabase(CborEncoder* parentEncoder, WarDatabase* database)
{
    CborEncoder mapEncoder, listEncoder;
    cbor_encoder_create_map(parentEncoder, &mapEncoder, CBOR_MAP_WAR_DATABASE_TAGS);

    cbor_encode_text_stringz(&mapEncoder, CBOR_WARS_TAG);
    cbor_encoder_create_array(&mapEncoder, &listEncoder, database->numWars);

    for(int i = 0; i <  database->currentCapacity; i++)
    {
        if(database->wars[i])
        {
            serializeWar(&listEncoder,database->wars[i]);
        }
    }

    cbor_encoder_close_container(&mapEncoder, &listEncoder);
    cbor_encoder_close_container(parentEncoder, &listEncoder);
}

void serializeMasterDatabase(CborEncoder* parentEncoder,
    MasterDatabase* database)
{
    CborEncoder mapEncoder;
    cbor_encoder_create_map(parentEncoder, &mapEncoder, CBOR_MAP_MASTER_TAGS);

    cbor_encode_text_stringz(&mapEncoder, CBOR_PLAYERS_TAG);
    serializePlayerDatabase(&mapEncoder, database->players);

    cbor_encode_text_stringz(&mapEncoder, CBOR_MONITOR_TAG);
    serializeWallet(&mapEncoder, database->monitorWallet);

    cbor_encode_text_stringz(&mapEncoder, CBOR_VALUES_TAG);
    serializeMarket(&mapEncoder, database->marketValues);

    cbor_encode_text_stringz(&mapEncoder, CBOR_WAR_DATABASE_TAG);
    serializeWarDatabase(&mapEncoder, database->wars);

    cbor_encoder_close_container(parentEncoder, &mapEncoder);
}

void serializeTask()
{
    MasterDatabase* copy = CopyMasterDatabase();
    CborEncoder encoder;

    int allocatedSize = DEFAULT_SPACE +
            copy->players->numberOfPlayers * DEFAULT_SPACE_PER_PLAYER;

    uint8_t* buffer = malloc(allocatedSize);

    cbor_encoder_init(&encoder, buffer, allocatedSize, 0);
    serializeMasterDatabase(&encoder, copy);

    size_t needed = cbor_encoder_get_extra_bytes_needed(&encoder);
    if(needed)
    {
        buffer = realloc(buffer, allocatedSize + needed);
        cbor_encoder_init(&encoder, buffer, allocatedSize + needed, 0);
        serializeMasterDatabase(&encoder, copy);
    }

    size_t finalSize = cbor_encoder_get_buffer_size(&encoder, buffer);
    writeBufferToFile(getDatabaseFile(), buffer, finalSize);

    freeMasterDatabase(copy);
    free(buffer);
}
