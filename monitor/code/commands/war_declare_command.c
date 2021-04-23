#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "war_declare_command.h"
#include "war_defend_command.h"
#include "../util.h"
#include "../constants.h"
#include "../commands.h"
#include "../error_messages.h"
#include "../message_processing.h"
#include "../war_database.h"
#include "../war.h"
#include "../active_connection.h"


int sendWarDecleration(Player* defender, WarDefendState* state,
                       char* attacker_name)
{
    String* message = initString();
    Command* command = getWarDefendCommand();
    StateBaseArg* stateBase = malloc(sizeof(StateBaseArg));
    stateBase->state = state;
    state->attacker_name = attacker_name;

    stringAppend(message, WAR_DECLARATION_FORMAT, attacker_name);

    ActiveRequest request;
    request.player = defender;
    request.commandsRequired = &command;
    request.states = &stateBase;
    request.message = &message;
    request.connection_duration = getWarDefendTimeOut();
    request.number_messages = 1;
    request.number_commands = 1;


    int ret =  sendActiveRequest(&request, NULL);
    deinitString(message);
    free(stateBase);
    return ret;
}

void* parseWarDeclareCommand(ConnectionInfo* info, char** parameters)
{
    WarDeclareArgs* ret = malloc(sizeof(WarDeclareArgs));
    char* weapons;
    char* vehicles;
    ret->targetIdent = getNextToken(parameters, SEPERATOR_STR);
    ret->targetHost = getNextToken(parameters, SEPERATOR_STR);
    ret->targetPort = getNextToken(parameters, SEPERATOR_STR);
    char* firstUnit = getNextToken(parameters, SEPERATOR_STR);
    if(firstUnit && !strcmp(firstUnit, VEHICLES_STRING))
    {
        vehicles = getNextToken(parameters, WEAPONS_SEPERATOR);
        weapons = getNextToken(parameters, SEPERATOR_STR);
    }
    else if(firstUnit && !strcmp(firstUnit, WEAPONS_STRING))
    {
        weapons = getNextToken(parameters, VEHICLES_SEPERATOR);
        vehicles = getNextToken(parameters, SEPERATOR_STR);
    }
    if(!vehicles || !weapons)
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_WAR_DECLARE_FORMAT);
    }
    else if(getNextToken(parameters, SEPERATOR_STR))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, UNEXPECTED_ARGUMENTS_ERROR_FORMAT, WAR_DECLARE_COMMAND);
    }
    else if(!isBase10(weapons))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_NUMBER_OF_WEAPONS);
    }
    else if(!isBase10(vehicles))
    {
        free(ret);
        ret = NULL;
        sendCommandError(info, INVALID_NUMBER_OF_VEHICLES);
    }
    else
    {
        ret->weapons = atol(weapons);
        ret->vehicles = atol(vehicles);
        if(ret->weapons <= 0)
        {
            free(ret);
            ret = NULL;
            sendCommandError(info, INVALID_NUMBER_OF_WEAPONS);
        }
        else if(ret->vehicles <= 0)
        {
            free(ret);
            ret = NULL;
            sendCommandError(info, INVALID_NUMBER_OF_VEHICLES);
        }
    }
    return ret;
}

int executeWarDeclareCommand(ConnectionInfo* info, void* in_args)
{
    int ret = COMMAND_FAILURE;
    WarDeclareArgs* args = (WarDeclareArgs*)in_args;
    Player* defender = checkIfPlayerExists(args->targetIdent);
    if(!defender)
    {
        sendCommandError(info, OTHER_PLAYER_DOES_NOT_EXIST, args->targetIdent);
    }
    else if(defender == info->player)
    {
        sendCommandError(info, CANNOT_FIGHT_SELF);
    }
    else if(!defender->alive_start_time)
    {
        sendCommandError(info, WAR_OPPONENT_NOT_ONLINE, defender->name);
    }
    else if(!commitResourcesForWar(info->player->wallet, args->weapons, args->vehicles))
    {
        sendCommandError(info, INSUFFICIENT_RESOURCES_WAR);
    }
    else
    {
        //should create if not present
        War* players_war = getPlayersWar(info->player, defender, 1);
        if(!prepareForWar(players_war))
        {
            returnTroopsFromWar(info->player->wallet, args->weapons, args->vehicles);
            sendCommandError(info, WAR_ALREADY_EXISTS);
        }
        else
        {
            //From this point on, whatever happens, war will be won by someone.
            //Hence log the war.
            logPlayerWar(info->player, defender);

            //Check if correct location was provided
            HostPort* actual =  getPlayerHostPortCopy(defender);
            if(strcmp(args->targetHost, actual->host) ||
               strcmp(args->targetPort, actual->port))
            {
                //Incorrect info was provided.
                sendCommandError(info, WAR_COULD_NOT_FIND_OPPONENT,
                       defender->name, args->targetHost, args->targetPort);
                returnTroopsFromWar(info->player->wallet, args->weapons, args->vehicles);
                setDefaultVictor(players_war, info->player, defender, defender, getPillageAmount()/2);
            }
            else
            {
                //Alright we are going to war
                ret = COMMAND_SUCCESS;
                WarDefendState defenderState;
                int response = sendWarDecleration(defender, &defenderState, info->player->name);
                String* war_response = initString();
                stringAppend(war_response, WAR_DECLARE_COMMAND);
                stringAppend(war_response, SEPERATOR_STR);
                if(response != ACTIVE_SUCCESS)
                {
                    returnTroopsFromWar(info->player->wallet, args->weapons, args->vehicles);
                    setDefaultVictor(players_war, info->player, defender, info->player, getPillageAmount());
                    stringAppend(war_response, WAR_WON_STRING, info->player->name);
                    addMessage(info, RESULT_FORMAT_STRING, stringGetBuffer(war_response));
                }
                else
                {
                    startWar(players_war, info->player, defender, args->weapons, args->vehicles,
                            defenderState.weapons, defenderState.vehicles);
                    stringAppend(war_response, WAR_BEGUN_STRING, defender->name);
                    addMessage(info, RESULT_FORMAT_STRING, stringGetBuffer(war_response));
                }
                deinitString(war_response);

            }
            freeHostPortCopy(actual);
        }
    }
    return ret;
}

void freeWarDeclareArgs(void* args)
{
    free(args);
}

Command* createWarDeclareCommand()
{
    Command* ret = malloc(sizeof(Command));
    int commandSize = strlen(WAR_DECLARE_COMMAND) + 1;
    ret->command_name = malloc(sizeof(char) * commandSize);
    ret->checkFormat = parseWarDeclareCommand;
    ret->cleanUp = freeWarDeclareArgs;
    ret->execute = executeWarDeclareCommand;
    sprintf(ret->command_name, WAR_DECLARE_COMMAND);
    return ret;
}
