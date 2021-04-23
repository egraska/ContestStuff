#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include "game_parameters.h"
#include "player_functions.h"
#include "commands.h"
#include "repeated_events.h"
#include "constants.h"
#include "active_connection.h"
#include "serializer.h"
#include "master_database.h"
#include "util.h"
#include "make_standings.h"
#include "display_market_values.h"
#include "backup.h"

//Monitor Messages
#define VERSION_STR           "Monitor version %s\n"
#define CURRENT_TIME_STR      "Current time is %d\n"
#define START_TIME_STR        "Contest start time is %d\n"
#define END_TIME_STR          "Contest end time is %d\n"
#define SOCKET_READY_STR      "Monitor running on port %d\nWaiting for connections.\n"

#define CONTEST_START_MESSAGE "Contest started. Enabling services."
#define CONTEST_END_MESSAGE   "Contest ended. Disabling services."

void *readySocket()
{
    int sockfd;
    int connfd;
    pthread_t thread_id;

    socklen_t player_address_len;

    struct sockaddr_in player_address;
    struct sockaddr_in monitor_address;

    //Create socket
    if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
        perror("Problem in creating the socket");
        exit(2);
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0)
    {
       perror("setsockopt(SO_REUSEADDR) failed");
    }

    //Binding created socket to monitor address
    monitor_address.sin_family = AF_INET;
    monitor_address.sin_addr.s_addr = htonl(INADDR_ANY);
    monitor_address.sin_port = htons(getMonitorPort());

    if(bind (sockfd, (struct sockaddr *) &monitor_address, sizeof(monitor_address)))
    {
        perror("ERROR on binding");
        exit(1);
    }

    //Listen
    listen (sockfd, getConnectionBacklog());
    FILE *logger;
    logger = fopen (getLoggingFile(), "w");
    if(logger)
    {
        fprintf(logger, VERSION_STR, MONITOR_VERSION);
        fclose(logger);
    }
    fprintf(stdout, VERSION_STR, MONITOR_VERSION);
    fprintf(stdout, CURRENT_TIME_STR, CURRENT_TIME);
    if(getStartTime())
    {
        fprintf(stdout, START_TIME_STR, getStartTime());
    }
    if(getEndTime())
    {
        fprintf(stdout, END_TIME_STR, getEndTime());
    }
    fprintf(stdout, SOCKET_READY_STR, getMonitorPort());

    player_address_len = sizeof(player_address);

    while((connfd = accept (sockfd, (struct sockaddr *) &player_address, &player_address_len)))
    {
        printf("\n%s\n","Connection request received");
        if(pthread_create(&thread_id , NULL,  handleIncommingConnection, (void*)&connfd) < 0)
        {
            printf("Problem in creating thread");
            exit(1);
        }
        pthread_detach(thread_id);
    }
    return NULL;
}

void *repeatedEventCreation()
{
    //Create repeated events
    createRepeatedEvent(resetCommandUsage,
                        SECONDS_IN_HOUR,
                        CLEAR_COMMANDS_NAME);

    createRepeatedEvent(runWars,
                        getSecondsBetweenBattles(),
                        WAR_NAME);

    createRepeatedEvent(distributeResource,
                        getResourceDistributionTimeMins(),
                        RESOURCE_DISTRIBUTION_NAME);

    createRepeatedEvent(calculateMarketValue,
                        getMarketValueCalcTimeMins(),
                        MARKET_VALUE_CALCULATION_NAME);

    createRepeatedEvent(updateScorecard,
                        SECONDS_IN_MINUTES,
                        SCORECARD_UPDATION_NAME);

    createRepeatedEvent(refreshMarketValues,
                        MARKET_VALUE_REFRESH_TIME,
                        MARKET_VALUE_REFRESH_NAME);

    createRepeatedEvent(copyDatabaseToFile,
                        getSerializeDBTimeMins(),
                        BACKUP_NAME);

    createRandomRepeatedEvent(checkPlayersForLiving,
                        getAliveCheckMinsMin(),
                        getAliveCheckMinsMax(),
                        CHECK_FOR_LIVING);

    return NULL;
}

int main (int argc, char *argv[])
{
    checkCommandLineArgs(argc, argv);
    loadParameters();
    initilizeGame();
    initializeScoreboard();
    initializeMarketWebpage();
    if(!createCommands())
    {
        perror("Could not create commands table");
        exit(2);
    }

    pthread_t socket_thread;
    pthread_create(&socket_thread , NULL, readySocket, NULL);

    if(START_SLEEP_TIME > 0)
    {
        sleep(START_SLEEP_TIME);
    }
    fprintf(stdout, "%s\n", CONTEST_START_MESSAGE);

    pthread_t event_thread;
    pthread_create(&event_thread , NULL, repeatedEventCreation, NULL);

    if(END_SLEEP_TIME > 0)
    {
        sleep(END_SLEEP_TIME);
    }
    else
    {
        sleep(CURRENT_TIME);
    }
    fprintf(stdout, "%s\n", CONTEST_END_MESSAGE);

    updateScorecard();
    return 0;
}
