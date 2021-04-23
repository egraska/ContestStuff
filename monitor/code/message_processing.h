#ifndef MESSAGE_PROCESSING_H
#define MESSAGE_PROCESSING_H

#define GREETING_MESSAGE                        "COMMENT: Monitor Version %s"
#define REQUIRE_MESSAGE                         "REQUIRE: %s"
#define PASSWORD_CHECKSUM_MESSAGE               "PLAYER_PASSWORD_CHECKSUM: %s"
#define WAITING_MESSAGE                         "WAITING:"
#define COMMAND_ERROR_MESSAGE                   "COMMAND_ERROR: %s"
#define COMMENT_MESSAGE                         "COMMENT: %s"
#define RESULT_MESSAGE                          "RESULT: %s %s"
#define WAR_DECLARATION_MESSAGE                 "WAR_DECLARATION: %s"

#include "player_functions.h"
#include "connection_info.h"

void sendRequireDirective(ConnectionInfo* info, char* commad);
void sendGreetingtoActiveClient(ConnectionInfo* info);
void sendGreetingtoPassiveServer(ConnectionInfo* info);
void sendCommandError(ConnectionInfo* info, char* error_message, ...);
void sendComment(ConnectionInfo* info, char* comment_message, ...);
void sendWaiting(ConnectionInfo* info);
void getRandomCookie(char* output_buffer);
void logMessages(char* message, int encryption, int sending, char* option, Player* player);

#endif
