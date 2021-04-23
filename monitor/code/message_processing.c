#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdarg.h>
#include <sys/socket.h>
#include "karn.h"
#include "game_parameters.h"
#include "commands.h"
#include "message_processing.h"
#include "player_database.h"
#include "constants.h"
#include "util.h"

void sendRequireDirective(ConnectionInfo* info, char* commad)
{
    addMessage(info, REQUIRE_MESSAGE, commad); 
    addMessage(info, WAITING_MESSAGE);
    sendMessageBuffer(info);
}

void sendGreetingtoActiveClient(ConnectionInfo* info)
{
    addMessage(info, GREETING_MESSAGE, MONITOR_VERSION);
    sendMessageBuffer(info);
}

void sendGreetingtoPassiveServer(ConnectionInfo* info)
{
    char password_checksum[PASSWORD_CHECKSUM_LEN];
    addMessage(info, GREETING_MESSAGE, MONITOR_VERSION);
    copyPlayerChecksum(info->player, password_checksum);
    addMessage(info, PASSWORD_CHECKSUM_MESSAGE, password_checksum);
    sendMessageBuffer(info);
}

void sendWaiting(ConnectionInfo* info)
{
    addMessage(info, WAITING_MESSAGE);
    sendMessageBuffer(info);
}

void getRandomCookie(char* output_buffer)
{
    int cookie_size = getCookieSize();
    for(int i = 0; i < cookie_size; i++)
    {
        output_buffer[i] = intToCharBase32(rand() % BASE_RADIX);
    }
    output_buffer[cookie_size] = 0x00;
}


void sendCommandError(ConnectionInfo* info, char* error_message, ...)
{
    String* message = initString();
    va_list argptr;

    va_start(argptr, error_message);
    stringVPrintf(message, error_message, argptr);
    va_end(argptr);

    addMessage(info, COMMAND_ERROR_MESSAGE, stringGetBuffer(message));
    deinitString(message);
}

void sendComment(ConnectionInfo* info, char* comment_message, ...)
{
    String* message = initString();
    va_list argptr;

    va_start(argptr, comment_message);
    stringVPrintf(message, comment_message, argptr);
    va_end(argptr);

    addMessage(info, COMMENT_FORMAT_STRING, stringGetBuffer(message));
    deinitString(message);
}

void logMessages(char* message, int encryption, int sending, char* option, Player* player)
{
    char time_stamp[TIME_STAMP_SIZE];
    memset(time_stamp, 0, sizeof(time_stamp));
    time_t timeStamp;
    time(&timeStamp);
    struct tm myTime;
    localtime_r(&timeStamp, &myTime);
    strftime(time_stamp, TIME_STAMP_SIZE, TIME_STAMP_FORMAT, &myTime);

    FILE *logger;
    logger = fopen (getLoggingFile(), option);
    if(logger)
    {
        if(player && getLogOptionPlayerName())
        {
            fprintf(logger, "%s [%s] ", time_stamp, player->name);
        }
        else
        {
            fprintf(logger, "%s ", time_stamp);
        }
        if(sending)
        {
            if(encryption)
            {
                fprintf(logger, "outgoing >e> %s\n", message);
            }
            else
            {
                fprintf(logger, "outgoing >>> %s\n", message);
            }
        }
        else
        {
            if(encryption)
            {
                fprintf(logger, "incoming <e< %s\n", message);
            }
            else
            {
                fprintf(logger, "incoming <<< %s\n", message);
            }
        }
        fclose(logger);
    }
}
