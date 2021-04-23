#ifndef _PARAMETERS
#define _PARAMETERS

#include "constants.h"

// v 0.1.2  Ident Command does not respond with result if encryption is not used
// v 0.1.3  Added random player host port command
// v 0.1.4  Added Check For living check
// v 0.1.5  Added Timeout for Connections
// v 0.1.6  Added Timeout for Connections patch to set timeout time in nanoseconds and shutdown is now called only on timeout
// v 0.1.7  Added Synthesize command
// v 0.1.8  Added Help command
// v 0.1.9  Added Serialization of database
// v 0.1.10 Serialization now writes the whole state
// v 0.1.11 Added unexpected argument check and empty string check
// v 0.1.12 Added the make certificate and get certificate commands
// v 0.1.13 Updated the commands to free memory allocated when parsing the command
// v 0.1.14 Added support for command line arguments
// v 0.1.15 Added Monitor as a reserved ident which cannot be used by players
// v 0.1.16 Updated the perform require function to handle empty strings
// v 0.1.17 Readline now converts input to uppercase after decryption
// v 0.1.18 Added trades Command
// v 0.1.19 Added Public_Key Command
// v 0.1.20 Added Rounds Command
// v 0.1.21 Guard byte is now appended to the byte array instead of the string
// v 0.1.22 Added support to store the RSA key of a player. Get certificate command returns public key along with the certificate.
// v 0.1.23 Added the RSA authentication to trades
// v 0.1.24 Updated the way resources are distributed
// v 0.1.25 Modified the select random resource function
// v 0.1.26 Game parameters can now be set from a file
// v 0.1.27 Added subset_a, subset_k and subset_j commands. ZKP authentication is now fully implemented.
// v 0.1.28 Added Wars
// v 0.1.29 Added War truce
// Shifted to new repo
// v 0.1.30 Base for RSA and DH keys can now be configured
// v 0.1.31 Trade and truce messages are now created before performing the next require.
// v 0.1.32 Certificates are made without converting key to byte array.
// v 0.1.33 Parsing and rallying the key type in the public key command.
// v 0.1.34 Changed the way war declare command is parsed.
// v 0.1.35 Changed the way trade command is parsed.
// v 0.1.36 Added resource distribution parameters as configurable parameters.
// v 0.1.37 Added log file. Logs are now printed to file.
// v 0.1.38 Added scorecard.
// v 0.1.39 Authorize set command, subset j and k commands now send sending player name along with the required info.
// v 0.1.40 The Require command sent in response to trade and truce requests now depends on the type of public key sent.
// v 0.1.41 Added one way encrypted/decrypted logging options.
// v 0.1.42 Added challenge_response command.
// v 0.1.43 Server is made "NOT_ALIVE" only if alive check fails. Not otherwise.
// v 0.1.44 The scorecard is now a webpage and not a text file.
// v 0.1.45 Changed the format of the trade command that is sent by the monitor to the receiver of the trade request.
// v 0.1.46 Public key command also sends the signed certificate along with the key.
// v 0.1.47 Commands relayed between players do not contain colons.
// Major Rework
// v 0.1.48 Removed Require: Quit from all passive server commands. Public key, and other authentication commands for the initiator are now asked from the passive server of the initiator.
// v 0.1.49 In the earlier version, only the requires were asked from the passive server, the messages were still sent to the active client. This is now fixed.
// v 0.1.50 Added support for public key none. In the authentication command validation, if messages less than no of rounds are sent, it is accepted. Validation still needs more fixes.
// v 0.1.51 If public key is none, it is notified as such to the subject.
// v 0.1.52 War declare command accepts weapons and vehicles in any order.
// v 0.1.53 War defend command accepts weapons and vehicles in any order.
// v 0.1.54 Added option to disable authentication in truce and trade.
// v 0.1.55 Removed the command co error.
// v 0.1.56 Added more parameters into Parameters.txt file
// v 0.1.57 Updated Scorecard
// v 0.1.58 Added a trades done attribute in the player db. The number of trades done by a player is also shown in the scorecard webpage.
// v 0.1.59 Made some more modifications to the scorecard webpage.
// v 0.1.60 Added wars fought, initialized and won attributes to player db. These attributes are also reflected in the scorecard webpage.
// v 0.1.61 Added truce offered and truce accepted as additional scoreboard attributes. Also updated truce command to reflect war winner as the person who accepts the truce.
// v 0.1.62 Modified an else if to if in war_truce_command.c which did not send command error in case of no active wars. Now it does.
// v 0.1.63 Modified the final auth commands (challenge response and subset j) to require trade response or truce response as per the command which initiated authentication. Earlier, it only asked for trade response.
// v 0.1.64 Updated scoreboard to show more spacing between coloumns.
// v 0.1.65 Modified truce command to transmit correct message in case of declined truce.
// v 0.1.66 Number of rounds is expected as a base 10 number instead of a base 32 number.
// v 0.1.67 Updated get game idents command to only show alive players.
// v 0.1.68 Market value is now displayed on a webpage. Monopoly bonus is only calculated when number of players is > 0.
// v 0.1.69 Added Start time as a configurable parameter. The monitor only accepts commands (other than handshake commands) after time crosses start time.
// v 0.1.70 Added end time as a configurable parameter. Once end time is passed, no commands are accepted and events are stopped. Scoreboard is also updated with the winner.
// v 0.1.71 Added a check in make standings. A winner will only be displayed on the webpage if the number of players is >= 1.
// v 0.1.72 Added backup function. The player, market, monitor and war databases are periodically backed up.
// v 0.1.73 Added recovery function. If recovery flag is 1, data is backed up from all the databases when the monitor starts up.
// v 0.1.74 Polished the functions that used start and end times. Converted printf statements into defines. The monitor does not cancel the socket and event threads. It just updates the scoreboard and quits.
// v 0.1.75 Changed the initializeScoreboard function to call updateScorecard, so that the scoreboard shows the scores as soon as it is initialized.
// v 0.1.76 Added checks to the recovery function. Host, port, password and cookie are only recovered if they are not null.
// v 0.1.77 Updated the synthesize command to expect proof of work(pow) only when the pow flag is set. If not set, players need not provide pow, but then, they can only synthesize 1 resource unit per command.
// v 0.1.78 Converted monitor messages to defines. In case of a static database, the recovery function makes the db dynamic, loads all the player data and makes it static again.
// v 0.1.79 A RESULT statement is sent to the issuer of TRADE_RESPONSE and WAR_TRUCE_RESPONSE command.
// v 0.1.80 Updated Recovery State such that when set to 1, all 4 dbs are recovered. Whereas if set to 0, only Player db is recovered.
// v 0.1.81 Updated isPlayerInWar function to only check active wars.
// v 0.1.82 Updated resource distribution algorithm to distribute rupyulars if favoured resource cannot be distributed.
// v 0.1.83 Updated the scoreboard to show multiple winners in case of tied wealth.
// v 0.1.84 Implemented GET_MONITOR_KEY command.
// v 0.1.85 Removed cbor backup. Added parameter to control backup epoch. Made the locations of db files relative to the monitor.
// v 0.1.86 Added a small recovery fix. Earlier password was not recovered if cookie was (null). But it should be, since a new cookie is generated everytime a correct or new password is provided.
// v 0.1.87 Updated the naming convention for resources in the messages sent out to clients.
// v 0.1.88 Added growth rate for crack as a configurable parameter.
// v 0.1.89 Added a max player check to the player db.
// v 0.1.90 Updated war declare command to check the host port provided before trying to make connection.
// v 0.1.91 Updated the normalization factors for rarity bonus and monopoly bonus. Left out monitor resources from monopoly calculation.
// v 0.1.92 Added demand bonus.
// v 0.1.93 Signoff command forgets the host port of players. i.e host port of a player is only given if alive time is non zero.
// v 0.1.94 Pillage factor reduced if incorrect host port details are provided.
// v 0.1.95 Demand bonus is now refreshed once monopoly bonus and rarity bonus are recalculated.
// v 0.1.96 Monitor certificate is now generated when the monitor is started. This certificate can be fetched using the get_certificate command.
// v 0.1.97 Corrected a typo in resources.c
// v 0.1.98 Configurable parameter USE_PROOF_OF_WORK now takes 3 values - Never require POW, always require POW and either format is allowed
// v 0.1.99 Added web directory as a configurable parameter. Market and Scoreboard web pages are refreshed at this directory.
// v 0.1.100 Updated battle losses. Loser's losses are rounded up and winner's losses are rounded down.
// v 0.1.101 Updated few comments and error messages.
// v 0.1.102 A player is now signed off if truce response is not given.
// v 0.1.103 Added a configurable parameter for authentication for trades.
// v 0.1.104 Added a check in dh_engine.c which creates a karn key equal to the size specified in the parameters file.
// v 0.1.105 Market value is no longer printed to terminal.
// v 0.1.106 Trade request command first checks if sender's passive server is available to provide authentication.
// v 0.1.107 War truce command first checks if sender's passive server is available to provide authentication.
// v 0.1.108 Handled error for failed malloc.
// v 0.1.109 Updated the pow command to allow synthesis of upto 10 units at a time without proof of work.
// v 0.1.110 Updated the synthesize command such that the monitor is more forgiving with regard to the arguments.
// v 0.1.111 Updated the monitor code to create web files in a specific directory instead of creating them in the default directory.
// v 0.1.112 Updated the CalculateMarketValue function. Locks are acquired in the following order -> Market, Monitor, Player.
// v 0.1.113 Fixed the war db extention function.
// v 0.1.114 Fixed the logging of war initiator and victor.
// v 0.1.115 Updated logging feature. Player name can now be added to the logs if desired.

//Version 0.1.115 seems to be stable and is a part of the first release on the cyber range. Version 0.2.0 onwards, we will be bringing new functionalities into the monitor.

// v 0.2.0 Adding the player password crack command.
// v 0.2.1 Fixed the bug that was causing the market and points to go hay-wire. And, added size overflow check in synthesize command.

#define MONITOR_VERSION                                 "0.2.1"
#define DEFAULT_MONITOR_PORT                            8180
#define DEFAULT_BACKLOG                                 8

//Player Parameters
#define DEFAULT_MAX_PLAYERS                             100
#define DEFAULT_COOKIE_SIZE                             20
#define MIN_PLAYER_PORT                                 2048
#define MAX_PLAYER_PORT                                 65000

//DH Encryption Parameters
#define DEFAULT_KARN_KEY_SIZE                           (64 * 8)
#define DEFAULT_MONITOR_DH_PRIVATE_KEY_LEN_BITS         1024
#define DEFAULT_MONITOR_DH_P_LEN_BITS                   1024

//Game Parameters
#define DEFAULT_COMPUTERS_REQUIRED_TO_GET_HP            1
#define DEFAULT_RES_REQUIRED_TO_SYN                     2
#define DEFAULT_TIME_FOR_RANDOM_HOST_PORT_SECONDS       (1 * SECONDS_IN_MINUTES)
#define DEFAULT_MONITOR_TAX_VALUE                       1.1
#define DEFAULT_COMMAND_ERRORS_ALLOWED                  5
#define DEFAULT_COMMANDS_PER_HOUR                       100
#define DEFAULT_START_TIME                              0
#define DEFAULT_END_TIME                                0
#define DEFAULT_RECOVERY_STATE                          1
#define POW_NO                                          0
#define POW_YES                                         1
#define DEFAULT_USE_PROOF_OF_WORK                       POW_NO
#define DEFAULT_MAX_SYNTHESIZABLE_AMOUNT                10
#define DEFAULT_MAX_CRACK_CHANCE                        100
#define DEFAULT_CRACK_GROWTH_RATE                       1.5
#define DEFAULT_PWD_CRACK_GROWTH_RATE                   1.5
#define DEFAULT_PASSWORD_CRACK_CHARS                    (DEFAULT_COOKIE_SIZE / 2)
#define MARKET_VALUE_REFRESH_TIME                       15

//Resource Parameters
#define DEFAULT_ALIVE_TIME_REQUIRED_FOR_RESOUCES        (15 * SECONDS_IN_MINUTES)
#define DEFAULT_DECAY_RATE_INVERSE                      1

//Database Parameters
#define DYNAMIC_PLAYER_DATABASE                         1
#define STATIC_PLAYER_DATABASE                          0
#define DEFAULT_PLAYER_DATABASE                         DYNAMIC_PLAYER_DATABASE

//Repeating event parameters
#define DEFAULT_DISTRIBUTE_RESOURCE_AFTER               SECONDS_IN_HOUR
#define DEFAULT_CALCULATE_MARKET_VALUE_AFTER            SECONDS_IN_HOUR
#define DEFAULT_SERIALIZE_DATABASE_PERIOD               30
#define DEFAULT_CHECK_FOR_ALIVE_MIN_TIME_SEC            (15 * SECONDS_IN_MINUTES)
#define DEFAULT_CHECK_FOR_ALIVE_MAX_TIME_SEC            (20 * SECONDS_IN_MINUTES)

//Connection timeout parameters
#define DEFAULT_ACTIVE_CONNECTION_PERIOD_SECONDS        (3 * SECONDS_IN_MINUTES)
#define DEFAULT_TRADE_RESPONSE_TIME_OUT                 (5 * SECONDS_IN_MINUTES)
#define DEFAULT_ALIVE_CHECK_PERIOD_SECONDS              25
#define DEFAULT_TRUCE_RESPONSE_TIME_OUT                 (5 * SECONDS_IN_MINUTES)
#define DEFAULT_WAR_DEFEND_TIME_OUT                     45
#define DEFAULT_PASSIVE_SERVER_AUTHENTICATION_TIMEOUT   (3 * SECONDS_IN_MINUTES)

//War parameters
#define DEFAULT_PILLAGE_AMOUNT                          0.5
#define DEFAULT_WAR_DEFENDER_BIAS                       1.2
#define DEFAULT_WINNER_BATTLE_LOSS                      0.25
#define DEFAULT_LOSER_BATTLE_LOSS                       0.50
#define DEFAULT_SECONDS_BETWEEN_BATTLES                 (5 * SECONDS_IN_MINUTES)

//Files
#define DEFAULT_DATABASE_FILE                           "Backup.cbor"
#define DEFAULT_PARAMETERS_FILE                         "Parameters.txt"
#define DEFAULT_SCORECARD_FILE                          "Scorecard.html"
#define DEFAULT_SCORECARD_URL                           "file:///home/cryptpc/Projects/C_Projects/iWars/Git/iWars/Monitor/Scorecard.html"
#define DEFAULT_MARKET_VALUE_FILE                       "Market_values.html"
#define DEFAULT_MARKET_VALUE_URL                        "file:///home/cryptpc/Projects/C_Projects/iWars/Git/iWars/Monitor/Market_values.html"

//Web parameters
#define SHOW_MARKET_VALUE                               1
#define NO_SHOW_MARKET_VALUE                            0
#define DEFAULT_SHOW_MARKET                             SHOW_MARKET_VALUE
#define DEFAULT_WEB_DIRECTORY                           "/home/cryptpc/Projects/C_Projects/iWars/Git/iWars/Monitor/"
#define DEFAULT_WEB_URL                                 "file:///home/cryptpc/Projects/C_Projects/iWars/Git/iWars/Monitor/"

//Scoreboard Params
#define DEFAULT_SCORECARD_TITLE                         "iWars"

//Logging Params
#define DEFAULT_LOG_FILE                                "Competition_log.txt"
#define SHOW_ENCRYPTED_TEXT                             0
#define SHOW_DECRYPTED_TEXT                             1
#define ONE_WAY_OUTGOING                                2
#define ONE_WAY_INCOMING                                3
#define DEFAULT_LOGGING_OPTION                          SHOW_ENCRYPTED_TEXT
#define DEFAULT_SHOW_PLAYER_NAME                        0

//RSA parameters
#define DEFAULT_RSA_PUBLIC_KEY_EXP                      "2001"
#define DEFAULT_RSA_PUBLIC_KEY_MOD                      "1Q1929537OTMQVOD5GBBK6U89GBH9RMVIHLK0EH1KHH9OORG2HLUBRIN1GDD51EHLNCLHOOTCRJ7JFQ1HNQVV3NLEJNOC8RGMJ0JGTT"
#define DEFAULT_RSA_PRIVATE_KEY                         "DM8K9CK4FR504R03TE4J54EO9MTNSDMDR398H33ENO16AAVSHGRN5EIU4CBVOQF9TGT6LBJ76V3NOM4DHRNL34QJ366J33GBA7O53T"
#define DEFAULT_RSA_KEY_BASE                            32
#define DEFAULT_SIGNED_CERT_BASE                        32

//DH parameters
#define DEFAULT_DH_KEY_EXCHANGE_P                       "96C99B60C4F823707B47A848472345230C5B25103DC37412A701833E8FF5C567A53A41D0B37B10F0060D50F4131C57CF1FD11B6A6CB958F36B1E7D878A4C4BC7"
#define DEFAULT_DH_KEY_EXCHANGE_G                       "2C900DF142E2B839E521725585A92DC0C45D6702A48004A917F74B73DB26391F20AEAE4C6797DD5ABFF0BFCAECB29554248233B5E6682CE1C73DD2148DED76C3"
#define DEFAULT_DH_KEY_EXCHANGE_BASE                    16

//Resource Parameters
#define DEFAULT_INITIAL_RESOURCES_WITH_MONITOR          10
#define DEFAULT_INITIAL_RESOURCE_MARKET_VALUE           1
#define DEFAULT_INITIAL_RESOURCES_WITH_PLAYER           10
#define DEFAULT_DISTRIBUTION_AMOUNT_FOR_PLAYER          100
#define DEFAULT_RESOURCE_MULTIPLIER_FOR_MONITOR         2
#define DEFAULT_DISTRIBUTION_AMOUNT_FOR_MONITOR         (DEFAULT_DISTRIBUTION_AMOUNT_FOR_PLAYER * DEFAULT_RESOURCE_MULTIPLIER_FOR_MONITOR)
#define DEFAULT_BASE_CURRENCY_RESOURCE_MARKET_VALUE     1
#define DEFAULT_BASE_BASIC_RESOURCE_MARKET_VALUE        5
#define DEFAULT_BASE_ASSEMBLED_RESOURCE_MARKET_VALUE    50

//Authentication Parameters
#define USE_AUTH_FOR_TRUCE                              0
#define DEFAULT_USE_AUTH_FOR_TRADE                      1

//Config file Tokens
#define MONITOR_PORT_TOKEN                              "MONITOR_PORT"
#define COOKIE_SIZE_TOKEN                               "COOKIE_SIZE"
#define CONNECTION_BACKLOG_TOKEN                        "CONNECTION_BACKLOG"
#define MAX_PLAYERS_TOKEN                               "MAX_PLAYERS"
#define KARN_KEY_SIZE_TOKEN                             "KARN_KEY_SIZE"
#define MONITOR_DH_PRIVATE_KEY_LEN_BITS_TOKEN           "MONITOR_DH_PRIVATE_KEY_LEN_BITS"
#define MONITOR_DH_P_LEN_BITS_TOKEN                     "MONITOR_DH_P_LEN_BITS"
#define COMPUTERS_REQUIRED_TO_GET_HP_TOKEN              "COMPUTERS_REQUIRED_TO_GET_HP"
#define RES_REQUIRED_TO_SYN_TOKEN                       "RES_REQUIRED_TO_SYN"
#define COMMAND_ERRORS_ALLOWED_TOKEN                    "COMMAND_ERRORS_ALLOWED"
#define PLAYER_DATABASE_STATE_TOKEN                     "PLAYER_DATABASE_STATE"
#define COMMANDS_PER_HOUR_TOKEN                         "COMMANDS_PER_HOUR"
#define DISTRIBUTE_RESOURCE_TOKEN                       "RESOURCE_DISTRIBUTION_TIME_IN_MINS"
#define CALCULATE_MARKET_VALUE_TOKEN                    "MARKET_VALUE_CALCULATION_TIME_IN_MINS"
#define SERIALIZE_DATABASE_TOKEN                        "DATABASE_SERIALIZATION_TIME_IN_SECONDS"
#define ALIVE_CHECK_MIN_TOKEN                           "MIN_ALIVE_CHECK_TIME_IN_MINS"
#define ALIVE_CHECK_MAX_TOKEN                           "MAX_ALIVE_CHECK_TIME_IN_MINS"
#define RANDOM_HP_TIME_TOKEN                            "TIME_FOR_RANDOM_HOST_PORT_MINS"
#define ACTIVE_CONNECTION_PERIOD_TOKEN                  "ACTIVE_CONNECTION_TIMEOUT_MINS"
#define TRADE_RESPONSE_TIMEOUT_PERIOD_TOKEN             "TRADE_RESPONSE_TIMEOUT_MINS"
#define TRUCE_RESPONSE_TIMEOUT_PERIOD_TOKEN             "TRUCE_RESPONSE_TIMEOUT_MINS"
#define ALIVE_CHECK_PERIOD_TOKEN                        "ALIVE_CHECK_TIMEOUT_SECONDS"
#define MONITOR_TAX_VALUE_TOKEN                         "MONITOR_TAX_VALUE"
#define MONITOR_RSA_EXP_TOKEN                           "MONITOR_RSA_PUBLIC_KEY_EXP"
#define MONITOR_RSA_MOD_TOKEN                           "MONITOR_RSA_PUBLIC_KEY_MOD"
#define MONITOR_RSA_PRIVATE_KEY_TOKEN                   "MONITOR_RSA_PRIVATE_KEY"
#define MONITOR_RSA_BASE_TOKEN                          "MONITOR_RSA_KEY_BASE"
#define MONITOR_SIGNED_CERT_BASE_TOKEN                  "MONITOR_RSA_SIGNED_CERT_BASE"
#define DH_KEY_EXCHANGE_P_TOKEN                         "DH_KEY_EXCHANGE_P"
#define DH_KEY_EXCHANGE_G_TOKEN                         "DH_KEY_EXCHANGE_G"
#define DH_KEY_EXCHANGE_BASE_TOKEN                      "DH_KEY_EXCHANGE_BASE"
#define PILLAGE_AMOUNT_TOKEN                            "PILLAGE_AMOUNT"
#define WAR_DEFENDER_BIAS_TOKEN                         "WAR_DEFENDER_BIAS"
#define WINNER_BATTLE_LOSS_TOKEN                        "WINNER_BATTLE_LOSS"
#define LOSER_BATTLE_LOSS_TOKEN                         "LOSER_BATTLE_LOSS"
#define SECONDS_BETWEEN_BATTLES_TOKEN                   "SECONDS_BETWEEN_BATTLES"
#define WAR_DEFEND_TIME_OUT_TOKEN                       "WAR_DEFEND_TIME_OUT"
#define ALIVE_TIME_FOR_RESOURCE_TOKEN                   "ALIVE_TIME_FOR_RESOURCE"
#define DECAY_RATE_INVERSE_TOKEN                        "DECAY_RATE_INVERSE"
#define INITIAL_RESOURCES_WITH_MON_TOKEN                "INITIAL_RESOURCES_WITH_MONITOR"
#define INITIAL_RESOURCES_MARKET_VALUE_TOKEN            "INITIAL_RESOURCES_MARKET_VALUE"
#define INITIAL_RESOURCES_WITH_PLAYER_TOKEN             "INITIAL_RESOURCES_WITH_PLAYER"
#define DISTRIBUTION_AMOUNT_PLAYERS_TOKEN               "DISTRIBUTION_AMOUNT_PLAYERS"
#define RESOURCE_MULTIPLIER_MON_TOKEN                   "RESOURCE_MULTIPLIER_MON"
#define CURRENCY_MARKET_VALUE_TOKEN                     "CURRENCY_MARKET_VALUE"
#define BASIC_MARKET_VALUE_TOKEN                        "BASIC_MARKET_VALUE"
#define ASSEMBLED_MARKET_VALUE_TOKEN                    "ASSEMBLED_MARKET_VALUE"
#define LOG_FILE_TOKEN                                  "LOG_FILE"
#define LOGGING_OPTION_TOKEN                            "LOGGING_OPTION"
#define PASSIVE_SERVER_AUTH_TIME_OUT_TOKEN              "PASSIVE_SERVER_AUTHENTICATION_TIMEOUT_MINS"
#define SCORECARD_FILE_TOKEN                            "SCORECARD_FILE"
#define SCORECARD_URL_TOKEN                             "SCORECARD_URL"
#define DATABASE_FILE_TOKEN                             "DATABASE_FILE"
#define SCORECARD_TITLE_TOKEN                           "SCORECARD_TITLE"
#define MARKET_VALUE_FILE_TOKEN                         "MARKET_VALUE_FILE"
#define MARKET_VALUE_URL_TOKEN                          "MARKET_VALUE_URL"
#define SHOW_MARKET_TOKEN                               "SHOW_MARKET_VALUE"
#define START_TIME_TOKEN                                "START_TIME"
#define END_TIME_TOKEN                                  "END_TIME"
#define RECOVERY_STATE_TOKEN                            "RECOVERY_STATE"
#define PROOF_OF_WORK_TOKEN                             "USE_PROOF_OF_WORK"
#define MAX_CRACK_CHANCE_TOKEN                          "MAX_CRACK_CHANCE"
#define CRACK_GROWTH_RATE_TOKEN                         "CRACK_GROWTH_RATE"
#define PWD_CRACK_GROWTH_RATE_TOKEN                     "PASSWORD_CRACK_GROWTH_RATE"
#define WEB_DIRECTORY_TOKEN                             "WEB_DIRECTORY"
#define WEB_URL_TOKEN                                   "WEB_URL"
#define TRADE_AUTH_TOKEN                                "USE_AUTH_FOR_TRADE"
#define MAX_SYNTHESIZABLE_AMOUNT_TOKEN                  "MAX_SYNTHESIZABLE_AMOUNT"
#define PASSWORD_CRACK_CHARS_TOKEN                      "PASSWORD_CRACK_CHARS"

void setMonitorPort(int port);
int getMonitorPort();
void setPlayerDatabaseState(int state);
int getPlayerDatabaseState();
void setDatabaseFile(char* db_file);
char* getDatabaseFile();
void setParametersFile(char* param_file);
char* getParametersFile();
void setScorecardFile(char* scorecard_file);
char* getScorecardFile();
void setScorecardURL(char* scorecard_url);
char* getScorecardURL();
void setCookieSize(int cookie_size);
int getCookieSize();
void setConnectionBacklog(int number);
int getConnectionBacklog();
void setMaxPlayers(int max_players);
int getMaxPlayers();
void setKarnKeySize(int size);
int getKarnKeySize();
int getKarnKeySizeBytes();
void setMonDHKeyPrivateKeyLen(int size);
int getMonDHKeyPrivateKeyLen();
void setMonDHKeyPublicKeyLen(int size);
int getMonDHKeyPublicKeyLen();
void setMonRequiredForHP(int number);
int getMonRequiredForHP();
void setResourcesReqdToSyn(int number);
int getResourcesReqdToSyn();
void setCommandErrorsAllowed(int number);
int getCommandErrorsAllowed();
void setCommandsPerHour(int number);
int getCommandsPerHour();
void setResourceDistributionTimeMins(int number);
int getResourceDistributionTimeMins();
void setMarketValueCalcTimeMins(int number);
int getMarketValueCalcTimeMins();
void setSerializeDBTimeMins(int number);
int getSerializeDBTimeMins();
void setAliveCheckMinsMin(int number);
int getAliveCheckMinsMin();
void setAliveCheckMinsMax(int number);
int getAliveCheckMinsMax();
void setTimeForRandomHP(int number);
int getTimeForRandomHP();
void setActiveConnectionTimeout(int number);
int getActiveConnectionTimeout();
void setTradeResponseTimeout(int number);
int getTradeResponseTimeout();
void setTruceResponseTimeout(int number);
int getTruceResponseTimeout();
void setAliveCheckTimeout(int number);
int getAliveCheckTimeout();
void setMonitorTaxValue(double value);
int getMonitorTaxValue();
void setMonRSAExp(char* exp);
char* getMonRSAExp();
void setMonRSAMod(char* mod);
char* getMonRSAMod();
void setMonRSACert(char* cert);
char* getMonRSACert();
void setMonRSAPrivateKey(char* key);
char* getMonRSAPrivateKey();
void setMonRSAKeyBase(int number);
int getMonRSAKeyBase();
void setMonSignedCertBase(int number);
int getMonSignedCertBase();
void setDHKeyP(char* p);
char* getDHKeyP();
void setDHKeyG(char* g);
char* getDHKeyG();
void setDHKeyBase(int number);
int getDHKeyBase();
float getPillageAmount();
void setPillageAmount(float newValue);
float getWarDefenderBias();
void setWarDefenderBias(float newValue);
float getWinnerBattleLoss();
void setWinnderBattleLoss(float newValue);
float getLoserBattleLoss();
void setLoserBattleLoss(float newValue);
int getSecondsBetweenBattles();
void setSecondsBetweenBattles(int newValue);
int getWarDefendTimeOut();
void setWarDefendTimeOut(int newValue);
int getPassiveServerAuthTimeOut();
void setPassiveServerAuthTimeOut(int newValue);
void setAliveTimeForResources(int number);
int getAliveTimeForResources();
void setInverseDecayRate(int number);
int getInverseDecayRate();
void setInitialResourcesWithMonitor(int number);
int getInitialResourcesWithMonitor();
void setInitialResourceMarketValue(int number);
int getInitialResourceMarketValue();
void setInitialResourcesWithPlayer(int number);
int getInitialResourcesWithPlayer();
void setDistributionAmountForPlayers(int number);
int getDistributionAmountForPlayers();
void setDistributionAmountForMonitor(int number);
int getDistributionAmountForMonitor();
void setBaseCurrencyMarketValue(int number);
int getBaseCurrencyMarketValue();
void setBaseBasicResourceMarketValue(int number);
int getBaseBasicResourceMarketValue();
void setBaseAssembledResourceMarketValue(int number);
int getBaseAssembledResourceMarketValue();
void setLoggingFile(char* logging_file);
char* getLoggingFile();
void setLogOption(int number);
int getLogOption();
int getLogOptionPlayerName();
void setScorecardTitle(char* value);
char* getScorecardTitle();
void setMarketValueFile(char* market_value_file);
char* getMarketValueFile();
void setMarketValueURL(char* market_value_url);
char* getMarketValueURL();
void setShowMarket(int number);
int getShowMarket();
void setStartTime(int number);
int getStartTime();
void setEndTime(int number);
int getEndTime();
void setRecoveryState(int state);
int getRecoveryState();
void setProofOfWork(int number);
int getProofOfWork();
void setMaxCrackChance(int number);
int getMaxCrackChance();
void setCrackGrowthRate(float number);
float getCrackGrowthRate();
void setPasswordCrackGrowthRate(float number);
float getPasswordCrackGrowthRate();
void setWebDirectory(char* url);
char* getWebDirectory();
char* getMarketPath();
char* getScorecardPath();
void setWebURL(char* url);
char* getWebURL();
void setTradeAuthState(int state);
int getTradeAuthState();
void setMaxSynthesisAmount(int number);
int getMaxSynthesisAmount();
void setPasswordCrackChars(int number);
int getPasswordCrackChars();

void loadParameters();
void checkCommandLineArgs(int argc, char *argv[]);

#endif
