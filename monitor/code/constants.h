#ifndef CONSTANTS_H
#define CONSTANTS_H

#define CR_CHAR                                 '\r'
#define NEWLINE_CHAR                            '\n'
#define NEWLINE_STR                             "\n"
#define COMMENT_STR                             "//"
#define SEPERATOR_STR                           " "
#define CONNECTOR_STR                           " and "
#define TRANSFER_SEPERATOR_STR                  " FOR "
#define TRUCE_PLAYER_SEPERATOR_STR              " TO "
#define COMMAND_OR_SEPERATOR                    " OR %s"
#define VEHICLES_SEPERATOR                      " VEHICLES "
#define WEAPONS_SEPERATOR                       " WEAPONS "

#define REQUIRE_FROMAT_STRING                   "REQUIRE: %s"
#define RESULTS_FORMAT_STRING                   "RESULT: %s %s"
#define RESULT_FORMAT_STRING                    "RESULT: %s"
#define RESULTS_LIST_FORMAT_STRING              "RESULT: %s%s"
#define COMMENT_FORMAT_STRING                   "COMMENT: %s"
#define TRADE_FORMAT_STRING                     "TRADE_REQUEST %s"
#define TRUCE_FORMAT_STRING                     "WAR_TRUCE_OFFERED %s"
#define WAR_DECLARATION_FORMAT                  "WAR_DECLARATION %s"
#define PUBLIC_KEY_FORMAT_STRING                "PUBLIC_KEY %s"
#define ROUNDS_FORMAT_STRING                    "ROUNDS %s"
#define CHALLENGE_FORMAT_STRING                 "CHALLENGE %s"
#define AUTHORIZE_SET_FORMAT_STRING             "AUTHORIZE_SET %s %s"
#define CHALLENGE_RESPONSE_FORMAT_STRING        "CHALLENGE_RESPONSE %s %s"
#define SUBSET_A_FORMAT_STRING                  "SUBSET_A %s"
#define SUBSET_K_FORMAT_STRING                  "SUBSET_K %s %s"
#define SUBSET_J_FORMAT_STRING                  "SUBSET_J %s %s"

#define IDENT_COMMAND                           "IDENT"
#define PASSWORD_COMMAND                        "PASSWORD"
#define ALIVE_COMMAND                           "ALIVE"
#define CHANGE_PASSWORD_COMMAND                 "CHANGE_PASSWORD"
#define SIGN_OFF_COMMAND                        "SIGN_OFF"
#define QUIT_COMMAND                            "QUIT"
#define SYNTHESIZE_COMMANMD                     "SYNTHESIZE"
#define HOST_PORT_COMMAND                       "HOST_PORT"
#define PLAYER_STATUS_COMMAND                   "PLAYER_STATUS"
#define TRADE_REQUEST_COMMAND                   "TRADE_REQUEST"
#define TRADE_COMMAND                           "TRADE"
#define TRADE_RESPONSE_COMMAND                  "TRADE_RESPONSE"
#define WAR_DEFEND_COMMAND                      "WAR_DEFEND"
#define WAR_DECLARE_COMMAND                     "WAR_DECLARE"
#define WAR_TRUCE_OFFER_COMMAND                 "WAR_TRUCE_OFFER"
#define WAR_TRUCE_RESPONSE_COMMAND              "WAR_TRUCE_RESPONSE"
#define WAR_STATUS_COMMAND                      "WAR_STATUS"
#define GET_GAME_IDENTS_COMMAND                 "GET_GAME_IDENTS"
#define RANDOM_PLAYER_HOST_PORT_COMMAND         "RANDOM_PLAYER_HOST_PORT"
#define PLAYER_HOST_PORT_COMMAND                "PLAYER_HOST_PORT"
#define PLAYER_STATUS_CRACK_COMMAND             "PLAYER_STATUS_CRACK"
#define PLAYER_PASSWORD_CRACK_COMMAND           "PLAYER_PASSWORD_CRACK"
#define PLAYER_MONITOR_PASSWORD_CRACK_COMMAND   "PLAYER_MONITOR_PASSWORD_CRACK"
#define SYNTHESIZE_COMMAND                      "SYNTHESIZE"
#define HELP_COMMAND                            "HELP"
#define MAKE_CERTIFICATE_COMMAND                "MAKE_CERTIFICATE"
#define GET_CERTIFICATE_COMMAND                 "GET_CERTIFICATE"
#define GET_MONITOR_KEY_COMMAND                 "GET_MONITOR_KEY"
#define PUBLIC_KEY_COMMAND                      "PUBLIC_KEY"
#define ROUNDS_COMMAND                          "ROUNDS"
#define AUTHORIZE_SET_COMMAND                   "AUTHORIZE_SET"
#define CHALLENGE_COMMAND                       "CHALLENGE"
#define CHALLENGE_RESPONSE_COMMAND              "CHALLENGE_RESPONSE"
#define SUBSET_A_COMMAND                        "SUBSET_A"
#define SUBSET_K_COMMAND                        "SUBSET_K"
#define SUBSET_J_COMMAND                        "SUBSET_J"

#define ACCEPT_STRING                           "ACCEPT"
#define DECLINE_STRING                          "DECLINE"

#define ACCEPTED_STRING                         "ACCEPTED"
#define DECLINED_STRING                         "DECLINED"
#define NOT_ALIVE_STRING                        "NOT_ALIVE"
#define SELF_NOT_ALIVE_STRING                   "Passive server unavailable for authentication"

#define HOST_PORT_SUCCESS                       "Command succeeded"

#define BASE_DECIMAL                            10
#define BASE_HEX                                16
#define BASE_RADIX                              32

#define BASE_HEX_EXPONENT                       4
#define BASE_RADIX_EXPONENT                     5

#define SECONDS_IN_MINUTES                      60
#define SECONDS_IN_HOUR                         (SECONDS_IN_MINUTES * 60)

#define RESOURCE_NAME_VALUE_FORMAT_STRING       " %s %lu"

#define IDENT_MONITOR                           "MONITOR"

#define DISCONNECT_COMMENT                      "Timeout occurred at %d seconds. Disconnecting..."

#define TIME_STAMP_SIZE                         20
#define TIME_STAMP_FORMAT                       "[%D %H:%M:%S]"
#define SCORECARD_TIME_STAMP_FORMAT             "%D %H:%M:%S"

#define HASH_LENGTH_HEX                         (((SHA_DIGEST_LENGTH * 2) + 1) * sizeof(char))

//Command Line args
#define COMMAND_LINE_OPTION_STRING              "p:d:r:b:g:"
#define COMMAND_LINE_OPTION_PORT                'p'
#define COMMAND_LINE_OPTION_DB_STATE            'd'
#define COMMAND_LINE_OPTION_RECOVERY            'r'
#define COMMAND_LINE_OPTION_BACKUP              'b'
#define COMMAND_LINE_OPTION_PARAMETERS_FILE     'g'

//Contest times
#define START_SLEEP_TIME                        getStartTime()-(int)time(NULL)
#define CURRENT_TIME                            (int)time(NULL)
#define END_SLEEP_TIME                          getEndTime()-(int)time(NULL)

#endif
