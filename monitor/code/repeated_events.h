#ifndef __REPEATED_EVENTS_H
#define __REPEATED_EVENTS_H

#define CLEAR_COMMANDS_NAME                    "CLEAR_COMMAND_COUNTER"
#define RESOURCE_DISTRIBUTION_NAME             "RESOURCE_DISTRIBUTION"
#define MARKET_VALUE_CALCULATION_NAME          "MARKET_VALUE_CALCULATION"
#define SERIALIZE_DATABASE_NAME                "SERIALIZING_DATABASE"
#define CHECK_FOR_LIVING                       "CHECK_FOR_LIVING"
#define WAR_NAME                               "WAR"
#define SCORECARD_UPDATION_NAME                "UPDATING_SCORECARD"
#define BACKUP_NAME                            "BACKUP"
#define MARKET_VALUE_REFRESH_NAME              "MARKET_VALUE_REFRESH"


void createRepeatedEvent(void (*event)(), int period, char* name);
void createRandomRepeatedEvent(void (*event)(), int minPeriod,
                                int maxPeriod, char* name);
void resetCommandUsage();

#endif
