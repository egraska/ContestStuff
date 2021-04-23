#include "game_parameters.h"
#include "util.h"
#include "constants.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "custom_string.h"

int MONITOR_PORT = DEFAULT_MONITOR_PORT;
int PLAYER_DATABASE_STATE = DEFAULT_PLAYER_DATABASE;
int RECOVERY_STATE = DEFAULT_RECOVERY_STATE;
char* DATABASE_FILE = DEFAULT_DATABASE_FILE;
char* PARAMETERS_FILE = DEFAULT_PARAMETERS_FILE;
char* SCORECARD_FILE = DEFAULT_SCORECARD_FILE;
char* SCORECARD_PATH = DEFAULT_SCORECARD_FILE;
char* SCORECARD_URL = DEFAULT_SCORECARD_URL;
char* MARKET_VALUE_FILE = DEFAULT_MARKET_VALUE_FILE;
char* MARKET_VALUE_PATH = DEFAULT_MARKET_VALUE_FILE;
char* MARKET_VALUE_URL = DEFAULT_MARKET_VALUE_URL;
int COOKIE_SIZE = DEFAULT_COOKIE_SIZE;
int PASSWORD_CRACK_CHARS = DEFAULT_PASSWORD_CRACK_CHARS;
int CONNECTION_BACKLOG = DEFAULT_BACKLOG;
int MAX_PLAYERS = DEFAULT_MAX_PLAYERS;
int KARN_KEY_SIZE = DEFAULT_KARN_KEY_SIZE;
int MONITOR_DH_PRIVATE_KEY_LEN_BITS = DEFAULT_MONITOR_DH_PRIVATE_KEY_LEN_BITS;
int MONITOR_DH_P_LEN_BITS = DEFAULT_MONITOR_DH_P_LEN_BITS;
int COMPUTERS_REQUIRED_TO_GET_HP = DEFAULT_COMPUTERS_REQUIRED_TO_GET_HP;
int RES_REQUIRED_TO_SYN = DEFAULT_RES_REQUIRED_TO_SYN;
int COMMAND_ERRORS_ALLOWED = DEFAULT_COMMAND_ERRORS_ALLOWED;
int COMMANDS_PER_HOUR = DEFAULT_COMMANDS_PER_HOUR;
int DISTRIBUTE_RESOURCE_AFTER = DEFAULT_DISTRIBUTE_RESOURCE_AFTER;
int CALCULATE_MARKET_VALUE_AFTER = DEFAULT_CALCULATE_MARKET_VALUE_AFTER;
int SERIALIZE_DATABASE_PERIOD = DEFAULT_SERIALIZE_DATABASE_PERIOD;
int CHECK_FOR_ALIVE_MIN_TIME_SEC = DEFAULT_CHECK_FOR_ALIVE_MIN_TIME_SEC;
int CHECK_FOR_ALIVE_MAX_TIME_SEC = DEFAULT_CHECK_FOR_ALIVE_MAX_TIME_SEC;
int TIME_FOR_RANDOM_HOST_PORT_SEC = DEFAULT_TIME_FOR_RANDOM_HOST_PORT_SECONDS;
int ACTIVE_CONNECTION_PERIOD_SEC = DEFAULT_ACTIVE_CONNECTION_PERIOD_SECONDS;
int TRADE_RESPONSE_TIME_OUT = DEFAULT_TRADE_RESPONSE_TIME_OUT;
int TRUCE_RESPONSE_TIME_OUT = DEFAULT_TRUCE_RESPONSE_TIME_OUT;
int ALIVE_CHECK_PERIOD_SEC = DEFAULT_ALIVE_CHECK_PERIOD_SECONDS;
float MONITOR_TAX_VALUE = DEFAULT_MONITOR_TAX_VALUE;
char* RSA_PUBLIC_KEY_EXP = DEFAULT_RSA_PUBLIC_KEY_EXP;
char* RSA_PUBLIC_KEY_MOD = DEFAULT_RSA_PUBLIC_KEY_MOD;
char* RSA_PRIVATE_KEY = DEFAULT_RSA_PRIVATE_KEY;
char* RSA_PUBLIC_KEY_CERT;
int RSA_KEY_BASE = DEFAULT_RSA_KEY_BASE;
int SIGNED_CERT_BASE = DEFAULT_SIGNED_CERT_BASE;
char* DH_KEY_EXCHANGE_P = DEFAULT_DH_KEY_EXCHANGE_P;
char* DH_KEY_EXCHANGE_G = DEFAULT_DH_KEY_EXCHANGE_G;
int DH_KEY_EXCHANGE_BASE = DEFAULT_DH_KEY_EXCHANGE_BASE;
float pillageAmount = DEFAULT_PILLAGE_AMOUNT;
float warDefenderBias = DEFAULT_WAR_DEFENDER_BIAS;
float winnerBattleLoss = DEFAULT_WINNER_BATTLE_LOSS;
float loserBattleLoss = DEFAULT_LOSER_BATTLE_LOSS;
int secondsBetweenBattle = DEFAULT_SECONDS_BETWEEN_BATTLES;
int warDefendTimeOut = DEFAULT_WAR_DEFEND_TIME_OUT;
int PASSIVE_SERVER_AUTHENTICATION_TIMEOUT = DEFAULT_PASSIVE_SERVER_AUTHENTICATION_TIMEOUT;
int ALIVE_TIME_REQUIRED_FOR_RESOUCES = DEFAULT_ALIVE_TIME_REQUIRED_FOR_RESOUCES;
int DECAY_RATE_INVERSE = DEFAULT_DECAY_RATE_INVERSE;
int INITIAL_RESOURCES_WITH_MONITOR = DEFAULT_INITIAL_RESOURCES_WITH_MONITOR;
int INITIAL_RESOURCE_MARKET_VALUE = DEFAULT_INITIAL_RESOURCE_MARKET_VALUE;
int INITIAL_RESOURCES_WITH_PLAYER = DEFAULT_INITIAL_RESOURCES_WITH_PLAYER;
int DISTRIBUTION_AMOUNT_FOR_PLAYER = DEFAULT_DISTRIBUTION_AMOUNT_FOR_PLAYER;
int RESOURCE_MULTIPLIER_FOR_MONITOR = DEFAULT_RESOURCE_MULTIPLIER_FOR_MONITOR;
int DISTRIBUTION_AMOUNT_FOR_MONITOR = DEFAULT_DISTRIBUTION_AMOUNT_FOR_MONITOR;
int BASE_CURRENCY_RESOURCE_MARKET_VALUE = DEFAULT_BASE_CURRENCY_RESOURCE_MARKET_VALUE;
int BASE_BASIC_RESOURCE_MARKET_VALUE = DEFAULT_BASE_BASIC_RESOURCE_MARKET_VALUE;
int BASE_ASSEMBLED_RESOURCE_MARKET_VALUE = DEFAULT_BASE_ASSEMBLED_RESOURCE_MARKET_VALUE;
char* LOG_FILE = DEFAULT_LOG_FILE;
int LOGGING_OPTION = DEFAULT_LOGGING_OPTION;
char* SCORECARD_TITLE = DEFAULT_SCORECARD_TITLE;
int SHOW_MARKET = DEFAULT_SHOW_MARKET;
int START_TIME = DEFAULT_START_TIME;
int END_TIME = DEFAULT_END_TIME;
int USE_PROOF_OF_WORK = DEFAULT_USE_PROOF_OF_WORK;
int MAX_CRACK_CHANCE = DEFAULT_MAX_CRACK_CHANCE;
float CRACK_GROWTH_RATE = DEFAULT_CRACK_GROWTH_RATE;
float PWD_CRACK_GROWTH_RATE = DEFAULT_PWD_CRACK_GROWTH_RATE;
char* WEB_URL = DEFAULT_WEB_URL;
char* WEB_DIRECTORY = DEFAULT_WEB_DIRECTORY;
int USE_AUTH_FOR_TRADE = DEFAULT_USE_AUTH_FOR_TRADE;
int MAX_SYNTHESIZABLE_AMOUNT = DEFAULT_MAX_SYNTHESIZABLE_AMOUNT;
int SHOW_PLAYER_NAME = DEFAULT_SHOW_PLAYER_NAME;

void setMonitorPort(int port)
{
    MONITOR_PORT = port;
}

int getMonitorPort()
{
    return MONITOR_PORT;
}

void setPlayerDatabaseState(int state)
{
    PLAYER_DATABASE_STATE = state;
}

int getPlayerDatabaseState()
{
    return PLAYER_DATABASE_STATE;
}

void setRecoveryState(int state)
{
    RECOVERY_STATE = state;
}

int getRecoveryState()
{
    return RECOVERY_STATE;
}

void setTradeAuthState(int state)
{
    USE_AUTH_FOR_TRADE = state;
}

int getTradeAuthState()
{
    return USE_AUTH_FOR_TRADE;
}

void setDatabaseFile(char* db_file)
{
    char* file = malloc(sizeof(char) * (strlen(db_file) + 1));
    strcpy(file, db_file);
    DATABASE_FILE = file;
}

char* getDatabaseFile()
{
    return DATABASE_FILE;
}

void setParametersFile(char* param_file)
{
    char* file = malloc(sizeof(char) * (strlen(param_file) + 1));
    strcpy(file, param_file);
    PARAMETERS_FILE = file;
}

char* getParametersFile()
{
    return PARAMETERS_FILE;
}

void setScorecardFile(char* scorecard_file)
{
    char* file = malloc(sizeof(char) * (strlen(scorecard_file) + 1));
    strcpy(file, scorecard_file);
    SCORECARD_FILE = file;
}

char* getScorecardFile()
{
    return SCORECARD_FILE;
}

void setScorecardURL(char* scorecard_url)
{
    char* url = malloc(sizeof(char) * (strlen(scorecard_url) + 1));
    strcpy(url, scorecard_url);
    SCORECARD_URL = url;
}

char* getScorecardURL()
{
    return SCORECARD_URL;
}

void setMarketValueFile(char* market_value_file)
{
    char* file = malloc(sizeof(char) * (strlen(market_value_file) + 1));
    strcpy(file, market_value_file);
    MARKET_VALUE_FILE = file;
}

char* getMarketValueFile()
{
    return MARKET_VALUE_FILE;
}

void setMarketValueURL(char* market_value_url)
{
    char* url = malloc(sizeof(char) * (strlen(market_value_url) + 1));
    strcpy(url, market_value_url);
    MARKET_VALUE_URL = url;
}

char* getMarketValueURL()
{
    return MARKET_VALUE_URL;
}

void setCookieSize(int cookie_size)
{
    COOKIE_SIZE = cookie_size;
}

int getCookieSize()
{
    return COOKIE_SIZE;
}

void setPasswordCrackChars(int number)
{
    PASSWORD_CRACK_CHARS = number;
}

int getPasswordCrackChars()
{
    return PASSWORD_CRACK_CHARS;
}

void setConnectionBacklog(int number)
{
    CONNECTION_BACKLOG = number;
}

int getConnectionBacklog()
{
    return CONNECTION_BACKLOG;
}

void setMaxCrackChance(int number)
{
    MAX_CRACK_CHANCE = number;
}

int getMaxCrackChance()
{
    return MAX_CRACK_CHANCE;
}

void setCrackGrowthRate(float number)
{
    CRACK_GROWTH_RATE = number;
}

float getCrackGrowthRate()
{
    return CRACK_GROWTH_RATE;
}

void setPasswordCrackGrowthRate(float number)
{
    PWD_CRACK_GROWTH_RATE = number;
}

float getPasswordCrackGrowthRate()
{
    return PWD_CRACK_GROWTH_RATE;
}

void setProofOfWork(int number)
{
    USE_PROOF_OF_WORK = number;
}

int getProofOfWork()
{
    return USE_PROOF_OF_WORK;
}

void setMaxPlayers(int max_players)
{
    MAX_PLAYERS = max_players;
}

int getMaxPlayers()
{
    return MAX_PLAYERS;
}

void setKarnKeySize(int size)
{
    KARN_KEY_SIZE = size * 8;
}

int getKarnKeySize()
{
    return KARN_KEY_SIZE;
}

int getKarnKeySizeBytes()
{
    return (getKarnKeySize() / 8);
}

void setMonDHKeyPrivateKeyLen(int size)
{
    MONITOR_DH_PRIVATE_KEY_LEN_BITS = size;
}

int getMonDHKeyPrivateKeyLen()
{
    return MONITOR_DH_PRIVATE_KEY_LEN_BITS;
}

void setMonDHKeyPublicKeyLen(int size)
{
    MONITOR_DH_P_LEN_BITS = size;
}

int getMonDHKeyPublicKeyLen()
{
    return MONITOR_DH_P_LEN_BITS;
}

void setMonRequiredForHP(int number)
{
    COMPUTERS_REQUIRED_TO_GET_HP = number;
}

int getMonRequiredForHP()
{
    return COMPUTERS_REQUIRED_TO_GET_HP;
}

void setResourcesReqdToSyn(int number)
{
    RES_REQUIRED_TO_SYN = number;
}

int getResourcesReqdToSyn()
{
    return RES_REQUIRED_TO_SYN;
}

void setCommandErrorsAllowed(int number)
{
    COMMAND_ERRORS_ALLOWED = number;
}

int getCommandErrorsAllowed()
{
    return COMMAND_ERRORS_ALLOWED;
}

void setCommandsPerHour(int number)
{
    COMMANDS_PER_HOUR = number;
}

int getCommandsPerHour()
{
    return COMMANDS_PER_HOUR;
}

void setResourceDistributionTimeMins(int number)
{
    DISTRIBUTE_RESOURCE_AFTER = number * SECONDS_IN_MINUTES;
}

int getResourceDistributionTimeMins()
{
    return DISTRIBUTE_RESOURCE_AFTER;
}

void setMarketValueCalcTimeMins(int number)
{
    CALCULATE_MARKET_VALUE_AFTER = number * SECONDS_IN_MINUTES;
}

int getMarketValueCalcTimeMins()
{
    return CALCULATE_MARKET_VALUE_AFTER;
}

void setSerializeDBTimeMins(int number)
{
    SERIALIZE_DATABASE_PERIOD = number;
}

int getSerializeDBTimeMins()
{
    return SERIALIZE_DATABASE_PERIOD;
}

void setAliveCheckMinsMin(int number)
{
    CHECK_FOR_ALIVE_MIN_TIME_SEC = number * SECONDS_IN_MINUTES;
}

int getAliveCheckMinsMin()
{
    return CHECK_FOR_ALIVE_MIN_TIME_SEC;
}

void setAliveCheckMinsMax(int number)
{
    CHECK_FOR_ALIVE_MAX_TIME_SEC = number * SECONDS_IN_MINUTES;
}

int getAliveCheckMinsMax()
{
    return CHECK_FOR_ALIVE_MAX_TIME_SEC;
}

void setTimeForRandomHP(int number)
{
    TIME_FOR_RANDOM_HOST_PORT_SEC = number * SECONDS_IN_MINUTES;
}

int getTimeForRandomHP()
{
    return TIME_FOR_RANDOM_HOST_PORT_SEC;
}

void setActiveConnectionTimeout(int number)
{
    ACTIVE_CONNECTION_PERIOD_SEC = number * SECONDS_IN_MINUTES;
}

int getActiveConnectionTimeout()
{
    return ACTIVE_CONNECTION_PERIOD_SEC;
}

void setTradeResponseTimeout(int number)
{
    TRADE_RESPONSE_TIME_OUT = number * SECONDS_IN_MINUTES;
}

int getTradeResponseTimeout()
{
    return TRADE_RESPONSE_TIME_OUT;
}

void setTruceResponseTimeout(int number)
{
    TRUCE_RESPONSE_TIME_OUT = number * SECONDS_IN_MINUTES;
}

int getTruceResponseTimeout()
{
    return TRUCE_RESPONSE_TIME_OUT;
}


void setAliveCheckTimeout(int number)
{
    ALIVE_CHECK_PERIOD_SEC = number * SECONDS_IN_MINUTES;
}

int getAliveCheckTimeout()
{
    return ALIVE_CHECK_PERIOD_SEC;
}

void setMonitorTaxValue(double value)
{
    MONITOR_TAX_VALUE = value;
}

int getMonitorTaxValue()
{
    return MONITOR_TAX_VALUE;
}

void setMonRSAExp(char* exp)
{
    char* rsa_exp = malloc(sizeof(char) * (strlen(exp) + 1));
    strcpy(rsa_exp, exp);
    RSA_PUBLIC_KEY_EXP = rsa_exp;
}

char* getMonRSAExp()
{
    return RSA_PUBLIC_KEY_EXP;
}

void setMonRSAMod(char* mod)
{
    char* rsa_mod = malloc(sizeof(char) * (strlen(mod) + 1));
    strcpy(rsa_mod, mod);
    RSA_PUBLIC_KEY_MOD = rsa_mod;
}

char* getMonRSAMod()
{
    return RSA_PUBLIC_KEY_MOD;
}

void setMonRSACert(char* cert)
{
    RSA_PUBLIC_KEY_CERT = cert;
}

char* getMonRSACert()
{
    return RSA_PUBLIC_KEY_CERT;
}

void setMonRSAPrivateKey(char* key)
{
    char* private_key = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(private_key, key);
    RSA_PRIVATE_KEY = private_key;
}

char* getMonRSAPrivateKey()
{
    return RSA_PRIVATE_KEY;
}

void setMonRSAKeyBase(int number)
{
    RSA_KEY_BASE = number;
}

int getMonRSAKeyBase()
{
    return RSA_KEY_BASE;
}

void setMonSignedCertBase(int number)
{
    SIGNED_CERT_BASE = number;
}

int getMonSignedCertBase()
{
    return SIGNED_CERT_BASE;
}

void setDHKeyP(char* p)
{
    char* dh_p = malloc(sizeof(char) * (strlen(p) + 1));
    strcpy(dh_p, p);
    DH_KEY_EXCHANGE_P = dh_p;
}

char* getDHKeyP()
{
    return DH_KEY_EXCHANGE_P;
}

void setDHKeyG(char* g)
{
    char* dh_g = malloc(sizeof(char) * (strlen(g) + 1));
    strcpy(dh_g, g);
    DH_KEY_EXCHANGE_G = dh_g;
}

char* getDHKeyG()
{
    return DH_KEY_EXCHANGE_G;
}

void setDHKeyBase(int number)
{
    DH_KEY_EXCHANGE_BASE = number;
}

int getDHKeyBase()
{
    return DH_KEY_EXCHANGE_BASE;
}

float getPillageAmount()
{
    return pillageAmount;
}

void setPillageAmount(float newValue)
{
    pillageAmount = newValue;
}

float getWarDefenderBias()
{
    return warDefenderBias;
}

void setWarDefenderBias(float newValue)
{
    warDefenderBias = newValue;
}

float getWinnerBattleLoss()
{
    return winnerBattleLoss;
}

void setWinnderBattleLoss(float newValue)
{
    winnerBattleLoss = newValue;
}

float getLoserBattleLoss()
{
    return loserBattleLoss;
}

void setLoserBattleLoss(float newValue)
{
    loserBattleLoss = newValue;
}

int getSecondsBetweenBattles()
{
    return secondsBetweenBattle;
}

void setSecondsBetweenBattles(int newValue)
{
    secondsBetweenBattle = newValue;
}

int getWarDefendTimeOut()
{
    return warDefendTimeOut;
}

void setWarDefendTimeOut(int newValue)
{
    warDefendTimeOut = newValue;
}

int getPassiveServerAuthTimeOut()
{
    return PASSIVE_SERVER_AUTHENTICATION_TIMEOUT;
}

void setPassiveServerAuthTimeOut(int newValue)
{
    PASSIVE_SERVER_AUTHENTICATION_TIMEOUT = newValue * SECONDS_IN_MINUTES;
}

void setAliveTimeForResources(int number)
{
    ALIVE_TIME_REQUIRED_FOR_RESOUCES = number * SECONDS_IN_MINUTES;
}

int getAliveTimeForResources()
{
    return ALIVE_TIME_REQUIRED_FOR_RESOUCES;
}

void setInverseDecayRate(int number)
{
    DECAY_RATE_INVERSE = number;
}

int getInverseDecayRate()
{
    return DECAY_RATE_INVERSE;
}

void setInitialResourcesWithMonitor(int number)
{
    INITIAL_RESOURCES_WITH_MONITOR = number;
}

int getInitialResourcesWithMonitor()
{
    return INITIAL_RESOURCES_WITH_MONITOR;
}

void setInitialResourceMarketValue(int number)
{
    INITIAL_RESOURCE_MARKET_VALUE = number;
}

int getInitialResourceMarketValue()
{
    return INITIAL_RESOURCE_MARKET_VALUE;
}

void setInitialResourcesWithPlayer(int number)
{
    INITIAL_RESOURCES_WITH_PLAYER = number;
}

int getInitialResourcesWithPlayer()
{
    return INITIAL_RESOURCES_WITH_PLAYER;
}

void setDistributionAmountForPlayers(int number)
{
    DISTRIBUTION_AMOUNT_FOR_PLAYER = number;
}

int getDistributionAmountForPlayers()
{
    return DISTRIBUTION_AMOUNT_FOR_PLAYER;
}

void setDistributionAmountForMonitor(int number)
{
    RESOURCE_MULTIPLIER_FOR_MONITOR = number;
    DISTRIBUTION_AMOUNT_FOR_MONITOR = (DISTRIBUTION_AMOUNT_FOR_PLAYER * RESOURCE_MULTIPLIER_FOR_MONITOR);
}

int getDistributionAmountForMonitor()
{
    return DISTRIBUTION_AMOUNT_FOR_MONITOR;
}

void setBaseCurrencyMarketValue(int number)
{
    BASE_CURRENCY_RESOURCE_MARKET_VALUE = number;
}

int getBaseCurrencyMarketValue()
{
    return BASE_CURRENCY_RESOURCE_MARKET_VALUE;
}

void setBaseBasicResourceMarketValue(int number)
{
    BASE_BASIC_RESOURCE_MARKET_VALUE = number;
}

int getBaseBasicResourceMarketValue()
{
    return BASE_BASIC_RESOURCE_MARKET_VALUE;
}

void setBaseAssembledResourceMarketValue(int number)
{
    BASE_ASSEMBLED_RESOURCE_MARKET_VALUE = number;
}

int getBaseAssembledResourceMarketValue()
{
    return BASE_ASSEMBLED_RESOURCE_MARKET_VALUE;
}

void setLoggingFile(char* logging_file)
{
    char* log_file = malloc(sizeof(char) * (strlen(logging_file) + 1));
    strcpy(log_file, logging_file);
    LOG_FILE = log_file;
}

char* getLoggingFile()
{
    return LOG_FILE;
}

void setScorecardTitle(char* value)
{
    char* title = malloc(sizeof(char) * (strlen(value) + 1));
    strcpy(title, value);
    SCORECARD_TITLE = title;
}

char* getScorecardTitle()
{
    return SCORECARD_TITLE;
}

void setLogOption(int number)
{
    if(number > 3)
    {
        LOGGING_OPTION = number - 4;
        SHOW_PLAYER_NAME = 1;
    }
    else
    {
        LOGGING_OPTION = number;
        SHOW_PLAYER_NAME = 0;
    }
}

int getLogOption()
{
    return LOGGING_OPTION;
}

int getLogOptionPlayerName()
{
    return SHOW_PLAYER_NAME;
}

void setShowMarket(int number)
{
    SHOW_MARKET = number;
}

int getShowMarket()
{
    return SHOW_MARKET;
}

void setMarketPath(char* dir_url)
{
    char* url = malloc(sizeof(char) * ((strlen(dir_url) + strlen(getMarketValueFile())) + 1));
    strcpy(url, dir_url);
    strcat(url, getMarketValueFile());
    MARKET_VALUE_PATH = url;
}

void setScorecardPath(char* dir_url)
{
    char* url = malloc(sizeof(char) * ((strlen(dir_url) + strlen(getScorecardFile())) + 1));
    strcpy(url, dir_url);
    strcat(url, getScorecardFile());
    SCORECARD_PATH = url;
}

char* getMarketPath()
{
    return MARKET_VALUE_PATH;
}

char* getScorecardPath()
{
    return SCORECARD_PATH;
}

void setWebDirectory(char* url)
{
    char* dir_url = malloc(sizeof(char) * (strlen(url) + 1));
    strcpy(dir_url, url);
    WEB_DIRECTORY = dir_url;

    setMarketPath(url);
    setScorecardPath(url);
}

char* getWebDirectory()
{
    return WEB_DIRECTORY;
}

void setWebURL(char* url)
{
    char* dir_url = malloc(sizeof(char) * (strlen(url) + 1));
    strcpy(dir_url, url);
    WEB_URL = dir_url;
}

char* getWebURL()
{
    return WEB_URL;
}

void setStartTime(int number)
{
    START_TIME = number;
}

int getStartTime()
{
    return START_TIME;
}

void setEndTime(int number)
{
    END_TIME = number;
}

int getEndTime()
{
    return END_TIME;
}

void setMaxSynthesisAmount(int number)
{
    MAX_SYNTHESIZABLE_AMOUNT = number;
}

int getMaxSynthesisAmount()
{
    return MAX_SYNTHESIZABLE_AMOUNT;
}

void loadParameters()
{
    FILE* pParamFile;
    pParamFile = fopen(getParametersFile(), "r");
    char* line = NULL;
    size_t line_len = 0;
    ssize_t bytes_read;
    if(pParamFile)
    {
        while((bytes_read = getline(&line, &line_len, pParamFile)) != -1)
        {
            char* pointer_to_line = line;
            char* token = getNextToken(&line, SEPERATOR_STR);
            char* value;
            if(!(strcmp(token, COOKIE_SIZE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setCookieSize(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", COOKIE_SIZE_TOKEN);
                }
            }
            else if(!(strcmp(token, PASSWORD_CRACK_CHARS_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setPasswordCrackChars(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", PASSWORD_CRACK_CHARS_TOKEN);
                }
            }
            else if(!(strcmp(token, CONNECTION_BACKLOG_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setConnectionBacklog(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", CONNECTION_BACKLOG_TOKEN);
                }
            }
            else if(!(strcmp(token, MAX_PLAYERS_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setMaxPlayers(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", MAX_PLAYERS_TOKEN);
                }
            }
            else if(!(strcmp(token, KARN_KEY_SIZE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    int key = atoi(value);
                    if(key % 2 != 0 || key > 64)
                    {
                        fprintf(stderr, "Value provided for %s should be a multiple of 2 and less than 64. Using default value %d\n", KARN_KEY_SIZE_TOKEN, (getKarnKeySize()/8));
                    }
                    else
                    {
                        setKarnKeySize(atoi(value));
                    }
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", KARN_KEY_SIZE_TOKEN);
                }
            }
            else if(!(strcmp(token, MONITOR_DH_PRIVATE_KEY_LEN_BITS_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setMonDHKeyPrivateKeyLen(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", MONITOR_DH_PRIVATE_KEY_LEN_BITS_TOKEN);
                }
            }
            else if(!(strcmp(token, MONITOR_DH_P_LEN_BITS_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setMonDHKeyPublicKeyLen(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", MONITOR_DH_P_LEN_BITS_TOKEN);
                }
            }
            else if(!(strcmp(token, MAX_SYNTHESIZABLE_AMOUNT_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value) && atoi(value) > 0)
                {
                    setMaxSynthesisAmount(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", MAX_SYNTHESIZABLE_AMOUNT_TOKEN);
                }
            }
            else if(!(strcmp(token, COMPUTERS_REQUIRED_TO_GET_HP_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setMonRequiredForHP(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", COMPUTERS_REQUIRED_TO_GET_HP_TOKEN);
                }
            }
            else if(!(strcmp(token, RES_REQUIRED_TO_SYN_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setResourcesReqdToSyn(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", RES_REQUIRED_TO_SYN_TOKEN);
                }
            }
            else if(!(strcmp(token, COMMAND_ERRORS_ALLOWED_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setCommandErrorsAllowed(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", COMMAND_ERRORS_ALLOWED_TOKEN);
                }
            }
            else if(!(strcmp(token, COMMANDS_PER_HOUR_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setCommandsPerHour(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", COMMANDS_PER_HOUR_TOKEN);
                }
            }
            else if(!(strcmp(token, MONITOR_PORT_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    int key = atoi(value);
                    if(key > MIN_PLAYER_PORT && key < MAX_PLAYER_PORT)
                    {
                        setMonitorPort(key);
                    }
                    else
                    {
                        fprintf(stderr, "Invalid port provided, using default port - %d\n", getMonitorPort());
                    }
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", MONITOR_PORT_TOKEN);
                }
            }
            else if(!(strcmp(token, PLAYER_DATABASE_STATE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    int key = atoi(value);
                    if(key == DYNAMIC_PLAYER_DATABASE || key == STATIC_PLAYER_DATABASE)
                    {
                        setPlayerDatabaseState(key);
                    }
                    else
                    {
                        fprintf(stderr, "Invalid database state, using default database state - %d\n", getPlayerDatabaseState());
                    }
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", PLAYER_DATABASE_STATE_TOKEN);
                }
            }
            else if(!(strcmp(token, RECOVERY_STATE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    int key = atoi(value);
                    if(key == 0 || key == 1)
                    {
                        setRecoveryState(key);
                    }
                    else
                    {
                        fprintf(stderr, "Invalid recovery state, using default recovery state - %d\n", getRecoveryState());
                    }
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", RECOVERY_STATE_TOKEN);
                }
            }
            else if(!(strcmp(token, TRADE_AUTH_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    int key = atoi(value);
                    if(key == 0 || key == 1)
                    {
                        setTradeAuthState(key);
                    }
                    else
                    {
                        fprintf(stderr, "Invalid trade auth state, using default trade auth state - %d\n", getTradeAuthState());
                    }
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", TRADE_AUTH_TOKEN);
                }
            }
            else if(!(strcmp(token, PROOF_OF_WORK_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    int key = atoi(value);
                    if(key == POW_NO || key == POW_YES)
                    {
                        setProofOfWork(key);
                    }
                    else
                    {
                        fprintf(stderr, "Invalid proof of work state, using default proof of work state - %d\n", getProofOfWork());
                    }
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", PROOF_OF_WORK_TOKEN);
                }
            }

            else if(!(strcmp(token, DISTRIBUTE_RESOURCE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setResourceDistributionTimeMins(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", DISTRIBUTE_RESOURCE_TOKEN);
                }
            }
            else if(!(strcmp(token, MAX_CRACK_CHANCE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setMaxCrackChance(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", MAX_CRACK_CHANCE_TOKEN);
                }
            }
            else if(!(strcmp(token, CRACK_GROWTH_RATE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isFloat(value))
                {
                    setCrackGrowthRate(atof(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", CRACK_GROWTH_RATE_TOKEN);
                }
            }
            else if(!(strcmp(token, PWD_CRACK_GROWTH_RATE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isFloat(value))
                {
                    setPasswordCrackGrowthRate(atof(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", PWD_CRACK_GROWTH_RATE_TOKEN);
                }
            }
            else if(!(strcmp(token, CALCULATE_MARKET_VALUE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setMarketValueCalcTimeMins(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", CALCULATE_MARKET_VALUE_TOKEN);
                }
            }
            else if(!(strcmp(token, SERIALIZE_DATABASE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setSerializeDBTimeMins(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", SERIALIZE_DATABASE_TOKEN);
                }
            }
            else if(!(strcmp(token, ALIVE_CHECK_MIN_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setAliveCheckMinsMin(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", ALIVE_CHECK_MIN_TOKEN);
                }
            }
            else if(!(strcmp(token, ALIVE_CHECK_MAX_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setAliveCheckMinsMax(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", ALIVE_CHECK_MAX_TOKEN);
                }
            }
            else if(!(strcmp(token, RANDOM_HP_TIME_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setTimeForRandomHP(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", RANDOM_HP_TIME_TOKEN);
                }
            }
            else if(!(strcmp(token, SHOW_MARKET_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    int key = atoi(value);
                    if(key == SHOW_MARKET_VALUE || key == NO_SHOW_MARKET_VALUE)
                    {
                        setShowMarket(key);
                    }
                    else
                    {
                        fprintf(stderr, "Invalid show market value, using default value - %d\n", getShowMarket());
                    }
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", SHOW_MARKET_TOKEN);
                }
            }
            else if(!(strcmp(token, ACTIVE_CONNECTION_PERIOD_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setActiveConnectionTimeout(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", ACTIVE_CONNECTION_PERIOD_TOKEN);
                }
            }
            else if(!(strcmp(token, TRADE_RESPONSE_TIMEOUT_PERIOD_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setTradeResponseTimeout(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", TRADE_RESPONSE_TIMEOUT_PERIOD_TOKEN);
                }
            }
            else if(!(strcmp(token, TRUCE_RESPONSE_TIMEOUT_PERIOD_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setTruceResponseTimeout(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", TRUCE_RESPONSE_TIMEOUT_PERIOD_TOKEN);
                }
            }
            else if(!(strcmp(token, ALIVE_CHECK_PERIOD_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setAliveCheckTimeout(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", ALIVE_CHECK_PERIOD_TOKEN);
                }
            }
            else if(!(strcmp(token, MONITOR_TAX_VALUE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isFloat(value))
                {
                    setMonitorTaxValue(atof(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", MONITOR_TAX_VALUE_TOKEN);
                }
            }
            else if(!(strcmp(token, MONITOR_RSA_EXP_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value)
                {
                    setMonRSAExp(value);
                }
            }
            else if(!(strcmp(token, MONITOR_RSA_MOD_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value)
                {
                    setMonRSAMod(value);
                }
            }
            else if(!(strcmp(token, MONITOR_RSA_PRIVATE_KEY_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value)
                {
                    setMonRSAPrivateKey(value);
                }
            }
            else if(!(strcmp(token, MONITOR_RSA_BASE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setMonRSAKeyBase(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", MONITOR_RSA_BASE_TOKEN);
                }
            }
            else if(!(strcmp(token, MONITOR_SIGNED_CERT_BASE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setMonSignedCertBase(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", MONITOR_SIGNED_CERT_BASE_TOKEN);
                }
            }
            else if(!(strcmp(token, DH_KEY_EXCHANGE_P_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value)
                {
                    setDHKeyP(value);
                }
            }
            else if(!(strcmp(token, DH_KEY_EXCHANGE_G_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value)
                {
                    setDHKeyG(value);
                }
            }
            else if(!(strcmp(token, DH_KEY_EXCHANGE_BASE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setDHKeyBase(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", DH_KEY_EXCHANGE_BASE_TOKEN);
                }
            }
            else if(!(strcmp(token, PILLAGE_AMOUNT_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isFloat(value))
                {
                    setPillageAmount(atof(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", PILLAGE_AMOUNT_TOKEN);
                }
            }
            else if(!(strcmp(token, WAR_DEFENDER_BIAS_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isFloat(value))
                {
                    setWarDefenderBias(atof(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", WAR_DEFENDER_BIAS_TOKEN);
                }
            }
            else if(!(strcmp(token, WINNER_BATTLE_LOSS_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isFloat(value))
                {
                    setWinnderBattleLoss(atof(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", WINNER_BATTLE_LOSS_TOKEN);
                }
            }
            else if(!(strcmp(token, LOSER_BATTLE_LOSS_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isFloat(value))
                {
                    setLoserBattleLoss(atof(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", LOSER_BATTLE_LOSS_TOKEN);
                }
            }
            else if(!(strcmp(token, SECONDS_BETWEEN_BATTLES_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setSecondsBetweenBattles(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", SECONDS_BETWEEN_BATTLES_TOKEN);
                }
            }
            else if(!(strcmp(token, WAR_DEFEND_TIME_OUT_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setWarDefendTimeOut(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", WAR_DEFEND_TIME_OUT_TOKEN);
                }
            }
            else if(!(strcmp(token, PASSIVE_SERVER_AUTH_TIME_OUT_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setPassiveServerAuthTimeOut(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", PASSIVE_SERVER_AUTH_TIME_OUT_TOKEN);
                }
            }
            else if(!(strcmp(token, ALIVE_TIME_FOR_RESOURCE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setAliveTimeForResources(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", ALIVE_TIME_FOR_RESOURCE_TOKEN);
                }
            }
            else if(!(strcmp(token, DECAY_RATE_INVERSE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setInverseDecayRate(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", DECAY_RATE_INVERSE_TOKEN);
                }
            }
            else if(!(strcmp(token, INITIAL_RESOURCES_WITH_MON_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setInitialResourcesWithMonitor(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", INITIAL_RESOURCES_WITH_MON_TOKEN);
                }
            }
            else if(!(strcmp(token, INITIAL_RESOURCES_MARKET_VALUE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setInitialResourceMarketValue(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", INITIAL_RESOURCES_MARKET_VALUE_TOKEN);
                }
            }
            else if(!(strcmp(token, INITIAL_RESOURCES_WITH_PLAYER_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setInitialResourcesWithPlayer(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", INITIAL_RESOURCES_WITH_PLAYER_TOKEN);
                }
            }
            else if(!(strcmp(token, DISTRIBUTION_AMOUNT_PLAYERS_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setDistributionAmountForPlayers(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", DISTRIBUTION_AMOUNT_PLAYERS_TOKEN);
                }
            }
            else if(!(strcmp(token, RESOURCE_MULTIPLIER_MON_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setDistributionAmountForMonitor(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", RESOURCE_MULTIPLIER_MON_TOKEN);
                }
            }
            else if(!(strcmp(token, CURRENCY_MARKET_VALUE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setBaseCurrencyMarketValue(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", CURRENCY_MARKET_VALUE_TOKEN);
                }
            }
            else if(!(strcmp(token, BASIC_MARKET_VALUE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setBaseBasicResourceMarketValue(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", BASIC_MARKET_VALUE_TOKEN);
                }
            }
            else if(!(strcmp(token, ASSEMBLED_MARKET_VALUE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setBaseAssembledResourceMarketValue(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", ASSEMBLED_MARKET_VALUE_TOKEN);
                }
            }
            else if(!(strcmp(token, START_TIME_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setStartTime(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", START_TIME_TOKEN);
                }
            }
            else if(!(strcmp(token, END_TIME_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    setEndTime(atoi(value));
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", END_TIME_TOKEN);
                }
            }
            else if(!(strcmp(token, LOG_FILE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value)
                {
                    setLoggingFile(value);
                }
            }
            else if(!(strcmp(token, SCORECARD_TITLE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value)
                {
                    setScorecardTitle(value);
                }
            }
            else if(!(strcmp(token, SCORECARD_FILE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value)
                {
                    setScorecardFile(value);
                }
            }
            else if(!(strcmp(token, MARKET_VALUE_FILE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value)
                {
                    setMarketValueFile(value);
                }
            }
            else if(!(strcmp(token, DATABASE_FILE_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value)
                {
                    setDatabaseFile(value);
                }
            }
            else if(!(strcmp(token, SCORECARD_URL_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value)
                {
                    setScorecardURL(value);
                }
            }
            else if(!(strcmp(token, MARKET_VALUE_URL_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value)
                {
                    setMarketValueURL(value);
                }
            }
            else if(!(strcmp(token, WEB_DIRECTORY_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value)
                {
                    setWebDirectory(value);
                }
            }
            else if(!(strcmp(token, WEB_URL_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value)
                {
                    setWebURL(value);
                }
            }
            else if(!(strcmp(token, LOGGING_OPTION_TOKEN)))
            {
                value = getNextToken(&line, NEWLINE_STR);
                if(value && isBase10(value))
                {
                    int key = atoi(value);
                    if(key >= 0 && key < 8)
                    {
                        setLogOption(key);
                    }
                    else
                    {
                        fprintf(stderr, "Invalid logging option, using default logging option - %d\n", getLogOption());
                    }
                }
                else
                {
                    fprintf(stderr, "Value provided for %s not a valid number\n", LOGGING_OPTION_TOKEN);
                }
            }
            else
            {
                if(strcmp(token, NEWLINE_STR) && strncmp(token, COMMENT_STR, strlen(COMMENT_STR)))
                {
                    fprintf(stderr, "%s is not a valid token\n", token);
                }
            }
            line = pointer_to_line;
        }
        fclose(pParamFile);
    }
    else
    {
        fprintf(stderr, "Could not open the specified file, using default parameters file - %s\n", getParametersFile());
    }
    if(line){
        free(line);
    }
}

void checkCommandLineArgs(int argc, char *argv[])
{
    int port = getMonitorPort();
    int database_state;

    int option;
    while((option = getopt(argc, argv, COMMAND_LINE_OPTION_STRING)) != -1)
    {
        if(option == COMMAND_LINE_OPTION_PORT)
        {
            if(isBase10(optarg))
            {
                port = atoi(optarg);
                if(port > 0 && port < MAX_PLAYER_PORT)
                {
                    setMonitorPort(port);
                }
                else
                {
                    port = getMonitorPort();
                    fprintf(stderr, "Invalid port number, using default port number - %d\n", port);
                }
            }
        }
        else if(option == COMMAND_LINE_OPTION_DB_STATE)
        {
            if(isBase10(optarg))
            {
                database_state = atoi(optarg);
                if(database_state == DYNAMIC_PLAYER_DATABASE || database_state == STATIC_PLAYER_DATABASE)
                {
                    setPlayerDatabaseState(database_state);
                }
                else
                {
                    fprintf(stderr, "Invalid database state, using default database state - %d\n", getPlayerDatabaseState());
                }
            }
        }
        else if(option == COMMAND_LINE_OPTION_BACKUP)
        {
            FILE* pDBFile;
            pDBFile = fopen(optarg, "wb");
            if(pDBFile)
            {
                fclose(pDBFile);
                setDatabaseFile(optarg);
            }
            else
            {
                fprintf(stderr, "Could not create the specified file, using default backup file - %s\n", getDatabaseFile());
            }
        }
        else if(option == COMMAND_LINE_OPTION_PARAMETERS_FILE)
        {
            setParametersFile(optarg);
        }
    }
}
