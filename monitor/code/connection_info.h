#ifndef CONNECTION_INFO_H
#define CONNECTION_INFO_H

#include "custom_string.h"
#include "game_parameters.h"
#include "player_database.h"

typedef struct connectionInfo{
    int socket_fd;
    Player* player;
    unsigned char* sharedSecret;
    String* receiveBuffer;
    String* outputBuffer;
    String* currentCommand;
    size_t receiveDataLength;
    size_t lastMesageLength;
    int connection_period;
    pthread_mutex_t* mutex;
    pthread_t* timeout_thread;
    pthread_cond_t* wait_condition;
    char* session_cookie;
    int synthesize_counter;
} ConnectionInfo;

ConnectionInfo* initConnectionInfo(int socket_file_desc, int connection_duration);
void deinitConnectionInfo(ConnectionInfo* info);

void addMessage(ConnectionInfo* info, char* format_message, ...);
void sendMessageBuffer(ConnectionInfo* info);
char* readLine(ConnectionInfo* info);

#endif
