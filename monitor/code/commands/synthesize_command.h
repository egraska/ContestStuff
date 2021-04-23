#ifndef SYNTHESIZE_COMMAND_H
#define SYNTHESIZE_COMMAND_H

#define INVALID_SYNTHESIZE_FORMAT_FLEX_LESS    "Command SYNTHESIZE requires at least one argument, <FINISHED_PRODUCT_NAME> (<AMOUNT> and <PROOF_OF_WORK>)"
#define INVALID_SYNTHESIZE_FORMAT_FLEX_MORE    "Command SYNTHESIZE was provided unexpected arguments, at most three arguments are expected, <FINISHED_PRODUCT_NAME>, <AMOUNT> and <PROOF_OF_WORK>"

#define BAD_AMOUNT_YES_POW                     "Asset amount cannot be less than or equal to 0"
#define BAD_AMOUNT_NO_POW                      "Asset amount cannot be less than or equal to 0 or greater than %d"

#define POW_NO_MANY_UNLIMIT                    2
#define POW_NO_MANY_LIMIT                      3

#include "../commands.h"

typedef struct
{
    char* amount;
    char* proof_of_work;
    int finished_product_index;
    int command_format;
} Synthesize_args;

Command* createSynthesizeCommand();

#endif
