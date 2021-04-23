#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include "string.h"
#include "player_database.h"
#include "game_parameters.h"
#include "message_processing.h"
#include "resources.h"
#include "commands.h"
#include "custom_string.h"
#include "constants.h"

#include "commands/ident_command.h"
#include "commands/password_command.h"
#include "commands/get_game_idents_command.h"
#include "commands/alive_command.h"
#include "commands/sign_off_command.h"
#include "commands/quit_command.h"
#include "commands/host_port_command.h"
#include "commands/change_password_command.h"
#include "commands/player_status_command.h"
#include "commands/player_status_crack_command.h"
#include "commands/player_password_crack_command.h"
#include "commands/player_host_port_command.h"
#include "commands/random_player_host_port_command.h"
#include "commands/synthesize_command.h"
#include "commands/help_command.h"
#include "commands/trade_command.h"
#include "commands/trade_response_command.h"
#include "commands/make_certificate_command.h"
#include "commands/get_certificate_command.h"
#include "commands/public_key_command.h"
#include "commands/rounds_command.h"
#include "commands/challenge_command.h"
#include "commands/authorize_set_command.h"
#include "commands/challenge_response_command.h"
#include "commands/war_declare_command.h"
#include "commands/war_defend_command.h"
#include "commands/war_status_command.h"
#include "commands/subset_a_command.h"
#include "commands/subset_k_command.h"
#include "commands/subset_j_command.h"
#include "commands/war_truce_command.h"
#include "commands/war_truce_response_command.h"
#include "commands/get_monitor_key_command.h"

//This value should be updated as commands are implemented
#define TOTAL_NUMBER_OF_COMMANDS                33

//This should be updated to be beyond
//the commands only accessible by the monitor
#define PLAYER_COMMANDS_START                   14

//Add defines here per command
#define IDENT_COMMAND_INDEX                     0
#define PASSWORD_COMMAND_INDEX                  1
#define ALIVE_COMMAND_INDEX                     2
#define TRADE_RESPONSE_COMMAND_INDEX            3
#define PUBLIC_KEY_COMMAND_INDEX                4
#define ROUNDS_COMMAND_INDEX                    5
#define AUTHORIZE_SET_INDEX                     6
#define CHALLENGE_INDEX                         7
#define SUBSET_A_INDEX                          8
#define SUBSET_K_INDEX                          9
#define SUBSET_J_INDEX                          10
#define WAR_DEFEND_COMMAND_INDEX                11
#define WAR_TRUCE_RESPONSE_COMMAND_INDEX        12
#define CHALLENGE_RESPONSE_INDEX                13

//All commands after this the player can execute
#define PLAYER_STATUS_CRACK_COMMAND_INDEX       14
#define PLAYER_HOST_PORT_COMMAND_INDEX          15
#define RANDOM_PLAYER_HOST_PORT_COMMAND_INDEX   16
#define SYNTHESIZE_COMMAND_INDEX                17
#define HELP_COMMAND_INDEX                      18
#define MAKE_CERTIFICATE_COMMAND_INDEX          19
#define GET_CERTIFICATE_COMMAND_INDEX           20
#define TRADE_COMMAND_INDEX                     21
#define HOST_PORT_COMMAND_INDEX                 22
#define QUIT_COMMAND_INDEX                      23
#define GET_GAME_IDENTS_COMMAND_INDEX           24
#define SIGN_OFF_COMMAND_INDEX                  25
#define CHANGE_PASSWORD_COMMAND_INDEX           26
#define PLAYER_STATUS_COMMAND_INDEX             27
#define WAR_STATUS_COMMAND_INDEX                28
#define WAR_DECLARE_COMMAND_INDEX               29
#define WAR_TRUCE_OFFER_COMMAND_INDEX           30
#define GET_MONITOR_KEY_INDEX                   31
#define PLAYER_PASSWORD_CRACK_COMMAND_INDEX     32

Command* commands[TOTAL_NUMBER_OF_COMMANDS];

void deinitCommand(Command* command)
{
    free(command->command_name);
    free(command);
}

int createCommands()
{
    commands[TRADE_RESPONSE_COMMAND_INDEX] = createTradeResponseCommand();
    commands[IDENT_COMMAND_INDEX] = createIdentCommand();
    commands[PASSWORD_COMMAND_INDEX] = createPasswordCommand();
    commands[ALIVE_COMMAND_INDEX] = createAliveCommand();
    commands[GET_GAME_IDENTS_COMMAND_INDEX] = createGetGameIdentsCommand();
    commands[SIGN_OFF_COMMAND_INDEX] = createSignOffCommand();
    commands[QUIT_COMMAND_INDEX] = createQuitCommand();
    commands[HOST_PORT_COMMAND_INDEX] = createHostPortCommand();
    commands[CHANGE_PASSWORD_COMMAND_INDEX] = createChangePasswordCommand();
    commands[PLAYER_STATUS_COMMAND_INDEX] = createPlayerStatusCommand();
    commands[PLAYER_STATUS_CRACK_COMMAND_INDEX] = createPlayerStatusCrackCommand();
    commands[PLAYER_HOST_PORT_COMMAND_INDEX] = createPlayerHostPortCommand();
    commands[RANDOM_PLAYER_HOST_PORT_COMMAND_INDEX] = createRandomPlayerHostPortCommand();
    commands[SYNTHESIZE_COMMAND_INDEX] = createSynthesizeCommand();
    commands[HELP_COMMAND_INDEX] = createHelpCommand();
    commands[TRADE_COMMAND_INDEX] = createTradeCommand();
    commands[MAKE_CERTIFICATE_COMMAND_INDEX] = createMakeCertCommand();
    commands[GET_CERTIFICATE_COMMAND_INDEX] = createGetCertCommand();
    commands[PUBLIC_KEY_COMMAND_INDEX] = createPublicKeyCommand();
    commands[ROUNDS_COMMAND_INDEX] = createRoundsCommand();
    commands[AUTHORIZE_SET_INDEX] = createAuthorizeSetCommand();
    commands[CHALLENGE_INDEX] = createChallengeCommand();
    commands[WAR_DECLARE_COMMAND_INDEX] = createWarDeclareCommand();
    commands[WAR_DEFEND_COMMAND_INDEX] = createWarDefendCommand();
    commands[WAR_STATUS_COMMAND_INDEX] = createWarStatusCommand();
    commands[SUBSET_A_INDEX] = createSubsetACommand();
    commands[SUBSET_K_INDEX] = createSubsetKCommand();
    commands[SUBSET_J_INDEX] = createSubsetJCommand();
    commands[WAR_TRUCE_OFFER_COMMAND_INDEX] = createWarTruceCommand();
    commands[WAR_TRUCE_RESPONSE_COMMAND_INDEX] = createWarTruceResponseCommand();
    commands[CHALLENGE_RESPONSE_INDEX] = createChallengeResponseCommand();
    commands[GET_MONITOR_KEY_INDEX] = createGetMonitorKeyCommand();
    commands[PLAYER_PASSWORD_CRACK_COMMAND_INDEX] = createPlayerPasswordCrackCommand();
    //Add new commands here
    // make sure commands that can only be required are at the start of the list

    int ret = 1;
    for (int i = 0; i < TOTAL_NUMBER_OF_COMMANDS; i++)
    {
        if(!commands[i])
        {
            //command failed creation
            ret = 0;
            break;
        }
    }
    return ret;
}

void freeCommands()
{
    for (int i = 0; i < TOTAL_NUMBER_OF_COMMANDS; i++)
    {
        if(commands[i])
        {
            deinitCommand(commands[i]);
        }
    }
}

Command* getIdentCommand()
{
    return commands[IDENT_COMMAND_INDEX];
}

Command* getPasswordCommand()
{
    return commands[PASSWORD_COMMAND_INDEX];
}

Command* getHostPortCommand()
{
    return commands[HOST_PORT_COMMAND_INDEX];
}

Command* getAliveCommand()
{
    return commands[ALIVE_COMMAND_INDEX];
}

Command* getQuitCommand()
{
    return commands[QUIT_COMMAND_INDEX];
}

Command* getWarDefendCommand()
{
    return commands[WAR_DEFEND_COMMAND_INDEX];
}

Command* getWarTruceResponseCommand()
{
    return commands[WAR_TRUCE_RESPONSE_COMMAND_INDEX];
}

Command* getTradeResponseCommand()
{
    return commands[TRADE_RESPONSE_COMMAND_INDEX];
}

Command* getPublicKeyCommand()
{
    return commands[PUBLIC_KEY_COMMAND_INDEX];
}

Command* getRoundsCommand()
{
    return commands[ROUNDS_COMMAND_INDEX];
}

Command* getAuthorizeSetCommand()
{
    return commands[AUTHORIZE_SET_INDEX];
}

Command* getChallengeCommand()
{
    return commands[CHALLENGE_INDEX];
}

Command* getSubsetACommand()
{
    return commands[SUBSET_A_INDEX];
}

Command* getSubsetKCommand()
{
    return commands[SUBSET_K_INDEX];
}

Command* getSubsetJCommand()
{
    return commands[SUBSET_J_INDEX];
}

Command* getChallengeResponseCommand()
{
    return commands[CHALLENGE_RESPONSE_INDEX];
}

Command* getPlayerCommandByName(char* name)
{
    Command* ret = NULL;
    for(int i = PLAYER_COMMANDS_START; i < TOTAL_NUMBER_OF_COMMANDS; i++)
    {
        if(!strcmp(name, commands[i]->command_name))
        {
            ret = commands[i];
            break;
        }
    }
    return ret;
}

void getExecutableCommandList(String* output_buffer)
{
    for(int i = PLAYER_COMMANDS_START; i < TOTAL_NUMBER_OF_COMMANDS; i++)
    {
        stringAppend(output_buffer, SEPERATOR_STR);
        stringAppend(output_buffer, commands[i]->command_name);
    }
}
