#include "repeated_events.h"
#include "player_database.h"
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define BEGINNING                              "Beginning..."
#define ENDING                                 "Ending..."

typedef struct 
{
    void (*event)();
    int min_period;
    int max_period;
    char* event_name;
}EventArgs;

void *repeatEvent(void* arg)
{
    EventArgs* args = (EventArgs*)arg;
    int max_delta = args->max_period - args->min_period;
    int delta = 0;
    while(1)
    {
        if(max_delta)
        {
            delta = rand() % (max_delta + 1);
        }
        sleep(args->min_period + delta);
        fprintf(stdout, "%s: %s\n", args->event_name, BEGINNING);
        args->event();
        fprintf(stdout, "%s: %s\n", args->event_name, ENDING);
    }
}

void startEvent(EventArgs* args)
{
    pthread_t event_thread;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&event_thread, &attr, repeatEvent, args);
    pthread_attr_destroy(&attr);
}

void createRepeatedEvent(void (*event)(), int period, char* name)
{
    EventArgs* args = malloc(sizeof(EventArgs));
    args->min_period = period;
    args->max_period = period;
    args->event = event;
    args->event_name = name;
    startEvent(args);
}

void createRandomRepeatedEvent(void (*event)(), int minPeriod,
                                int maxPeriod, char* name)
{
    EventArgs* args = malloc(sizeof(EventArgs));
    args->min_period = minPeriod;
    args->max_period = maxPeriod;
    args->event = event;
    args->event_name = name;
    startEvent(args);
}

void resetCommandUsage()
{
    int playerCount = getNumberOfPlayers();
    Player** players = malloc(sizeof(Player*) * playerCount);
    playerCount = getPlayerList(players, playerCount);
    for(int i = 0; i < playerCount; i++)
    {
        resetPlayerCommandUsage(players[i]);
    }
    free(players);
}
