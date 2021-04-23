#include <stdarg.h>
#include <unistd.h>
#include <sys/socket.h>
#include "constants.h"
#include "connection_info.h"
#include "util.h"
#include "stdio.h"
#include "string.h"
#include "karn.h"
#include "message_processing.h"
#include "backup.h"
#include "error_messages.h"
#include <errno.h>

#define KARN_GAURD_STRING           "1A"

#define ENCRYPTION_ON               1
#define ENCRYPTION_OFF              0
#define OUTGOING                    1
#define INCOMING                    0

void *calculateTimeout(void *connection_info_args);

ConnectionInfo* initConnectionInfo(int socket_file_desc, int connection_duration)
{
    ConnectionInfo* ret = malloc(sizeof(ConnectionInfo));
    ret->player = NULL;
    ret->socket_fd = socket_file_desc;
    ret->outputBuffer = initString();
    ret->receiveBuffer = initString();
    ret->currentCommand = initString();
    ret->receiveDataLength = 0;
    ret->lastMesageLength = 0;
    ret->sharedSecret = NULL;
    ret->session_cookie = (char*)malloc(sizeof(char) * (getCookieSize() +1));
    ret->connection_period = connection_duration;
    ret->mutex = malloc(sizeof(pthread_mutex_t));
    if(!ret->mutex)
    {
        fprintf(stderr, "ERROR: Could not create connection mutex\n");
    }
    if(pthread_mutex_init(ret->mutex, NULL))
    {
        fprintf(stderr, "ERROR: Mutex for player connection failed init\n");
    }
    ret->timeout_thread = malloc(sizeof(pthread_t));
    ret->wait_condition = malloc(sizeof(pthread_cond_t));
    if(!ret->wait_condition)
    {
        fprintf(stderr, "ERROR: Could not create condition variable\n");
    }
    if(pthread_cond_init(ret->wait_condition, NULL))
    {
        fprintf(stderr, "ERROR: Condition variable failed init\n");
    }
    pthread_create(ret->timeout_thread, NULL, calculateTimeout, ret);
    ret->synthesize_counter = 0;
    return ret;
}

void deinitConnectionInfo(ConnectionInfo* info)
{
    pthread_mutex_lock(info->mutex);
    pthread_cond_signal(info->wait_condition);
    pthread_mutex_unlock(info->mutex);
    pthread_join(*info->timeout_thread, NULL);
    free(info->timeout_thread);
    close(info->socket_fd);
    deinitString(info->receiveBuffer);
    deinitString(info->outputBuffer);
    deinitString(info->currentCommand);
    free(info->session_cookie);
    if(info->sharedSecret)
    {
        free(info->sharedSecret);
    }
    pthread_cond_destroy(info->wait_condition);
    free(info->wait_condition);
    free(info->mutex);
    free(info);
}

void *calculateTimeout(void *connection_info_args)
{
    ConnectionInfo* args = (ConnectionInfo*)connection_info_args;
    struct timespec ts;
    ts.tv_sec = time(NULL) + args->connection_period;
    ts.tv_nsec = 0;
    pthread_mutex_lock(args->mutex);
    int exit_reason = pthread_cond_timedwait(args->wait_condition, args->mutex, &ts);
    if(exit_reason == ETIMEDOUT)
    {
        sendComment(args, DISCONNECT_COMMENT, args->connection_period);
        sendMessageBuffer(args);
        shutdown(args->socket_fd, SHUT_RDWR);
    }
    pthread_mutex_unlock(args->mutex);
    return NULL;
}

void addMessage(ConnectionInfo* info, char* format_message, ...)
{
    va_list arglist;
    char* message = malloc(sizeof(char) * BASE_STRING_SIZE);
    if(!message)
    {
        fprintf(stderr, FATAL_ERROR);
        copyDatabaseToFile();
        exit(1);
    }

    va_start(arglist, format_message);
    size_t needed_size = vsnprintf(message, BASE_STRING_SIZE,
                                  format_message, arglist) + 1;
    va_end(arglist);
    if(info->sharedSecret)
    {
        //We do not have to encode the null byte in the cipher text
        size_t karnLength = karnGetPaddedLength(needed_size -1);
        if(karnLength > needed_size)
        {
            needed_size = karnLength;
        }
        //Don't forget a null byte
        needed_size = getRadixLenForByteArray(needed_size) + 1;
    }

    if(needed_size > BASE_STRING_SIZE)
    {
        message = realloc(message, needed_size * sizeof(char));
        if(message)
        {
            va_start(arglist, format_message);
            vsnprintf(message, needed_size, format_message, arglist);
            va_end(arglist);
        }
        else
        {
            fprintf(stderr, FATAL_ERROR);
            copyDatabaseToFile();
            exit(1);
        }
    }

    if(info->sharedSecret)
    {
        if(getLogOption() == SHOW_DECRYPTED_TEXT || getLogOption() == ONE_WAY_OUTGOING)
        {
            logMessages(message, ENCRYPTION_ON, OUTGOING, "a", info->player);
        }
        needed_size = encryptKarn((unsigned char*) message, info->sharedSecret);
        byteArrayToString((unsigned char*)message, needed_size, BASE_RADIX);
        stringAppend(info->outputBuffer, message);
        if(getLogOption() == SHOW_ENCRYPTED_TEXT || getLogOption() == ONE_WAY_INCOMING)
        {
            logMessages(message, ENCRYPTION_ON, OUTGOING, "a", info->player);
        }
    }
    else
    {
        stringAppend(info->outputBuffer, message);
        logMessages(message, ENCRYPTION_OFF, OUTGOING, "a", info->player);
    }
    stringAppend(info->outputBuffer, NEWLINE_STR);
    free(message);
}

void sendMessageBuffer(ConnectionInfo* info)
{
    send(info->socket_fd, stringGetBuffer(info->outputBuffer),
        stringLength(info->outputBuffer), MSG_NOSIGNAL);
    info->outputBuffer->buffer[0] = 0x00;
}

int hasNewMessage(ConnectionInfo* info, int starting_point)
{
    char* buffer = stringGetBuffer(info->receiveBuffer);
    int ret = 0;
    for(int i = starting_point; i < info->receiveDataLength; i++)
    {
        if(buffer[i] == NEWLINE_CHAR)
        {
            info->lastMesageLength = i;
            ret = 1;
            buffer[i] = 0x00;
            // Some utilites like telnet send \r\n as a new line
            // We are choosing to support this
            if(i-- && buffer[i] == CR_CHAR)
            {
                buffer[i] = 0x00;
            }
            break;
        }
    }
    return ret;
}

void handleErrorOnReceive(ConnectionInfo* info)
{
    stringPrintf(info->currentCommand, QUIT_COMMAND);
    //"flush the buffer"
    info->lastMesageLength = 0;
    logMessages(stringGetBuffer(info->currentCommand), ENCRYPTION_OFF, INCOMING, "a", info->player);
}

char* readLine(ConnectionInfo* info)
{
    //add one for the null char
    size_t existing_str_len = info->lastMesageLength;
    char* ret = stringGetBuffer(info->currentCommand);
    // advance past any nulls
    // then go one further to get to the first point of real data
    while( existing_str_len + 1 <= info->receiveDataLength &&
           !stringGetBuffer(info->receiveBuffer) [existing_str_len + 1] )
    {
        existing_str_len++;
    }
    if(existing_str_len + 1 <= info->receiveDataLength)
    {
       existing_str_len++;
    }
    stringRealign(info->receiveBuffer, existing_str_len,
                  info->receiveDataLength - existing_str_len);
    info->receiveDataLength -= existing_str_len;
    int bytes_scanned = 0;

    while(!hasNewMessage(info, bytes_scanned))
    {
        if(info->receiveBuffer->current_capacity - info->receiveDataLength <= 0)
        {
            //We need to increase the size of the buffer.
            if(!stringExpand(info->receiveBuffer))
            {
                // We can no longer expand the buffer.
                // For now lets terminate with the QUIT_COMMAND
                // This is less clean than a socket error as the
                // player may still be listening
                handleErrorOnReceive(info);
                return ret;
            }
        }
        bytes_scanned = info->receiveDataLength;
        ssize_t bytes_received = recv(info->socket_fd,
                stringGetBuffer(info->receiveBuffer) + info->receiveDataLength,
                info->receiveBuffer->current_capacity - info->receiveDataLength,
                0);
        if(bytes_received > 0)
        {
            info->receiveDataLength += bytes_received;
        }
        else if(bytes_received <= 0)
        {
            //EOF or error
            //return quit command
            handleErrorOnReceive(info);
            return ret;
        }
    }
    strUpper(stringGetBuffer(info->receiveBuffer));
    if(info->sharedSecret)
    {
        if(getLogOption() == SHOW_ENCRYPTED_TEXT || getLogOption() == ONE_WAY_OUTGOING)
        {
            logMessages(stringGetBuffer(info->receiveBuffer), ENCRYPTION_ON, INCOMING, "a", info->player);
        }
        size_t numberOfBytes = stringToByteArray((unsigned char*)stringGetBuffer(info->receiveBuffer), BASE_RADIX);
        //skip the gaurd bytes. We do not care about them
        if(numberOfBytes)
        {
            decryptKarn((unsigned char*)stringGetBuffer(info->receiveBuffer) + 1, info->sharedSecret, numberOfBytes -1);
            strUpper(stringGetBuffer(info->receiveBuffer));
            stringCopy(info->currentCommand, stringGetBuffer(info->receiveBuffer) + 1);
            if(getLogOption() == SHOW_DECRYPTED_TEXT || getLogOption() == ONE_WAY_INCOMING)
            {
                logMessages(stringGetBuffer(info->currentCommand), ENCRYPTION_ON, INCOMING, "a", info->player);
            }
        }
        else
        {
            //they arent playing nice we wont either
            stringPrintf(info->currentCommand, QUIT_COMMAND);
            //"flush the buffer"
            info->lastMesageLength = 0;
            logMessages(stringGetBuffer(info->currentCommand), ENCRYPTION_ON, INCOMING, "a", info->player);
        }
    }
    else
    {
        stringCopy(info->currentCommand, stringGetBuffer(info->receiveBuffer));
        logMessages(stringGetBuffer(info->currentCommand), ENCRYPTION_OFF, INCOMING, "a", info->player);
    }
    ret = stringGetBuffer(info->currentCommand);
    return ret;
}
