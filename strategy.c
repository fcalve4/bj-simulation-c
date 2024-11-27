#include <stdio.h>
#include <string.h>
#include "strategy.h"

#define STRAT_ROWS 40 // Number of player hand conditions
#define STRAT_COLS 10 // Dealer's upcards (2 to Ace)

void readStrategySheet(const char *filename, char (*strategyArray)[STRAT_COLS])
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file '%s'.\n", filename);
        return;
    }

    char line[256];
    int row = 0;

    while (fgets(line, sizeof(line), file) && row < STRAT_ROWS) {
        // Tokenize the line to extract decisions
        char *token = strtok(line, ",");
        int col = 0;
        while (token != NULL && col < STRAT_COLS) {
            strategyArray[row][col] = token[0]; // Store the first character of the token
            token = strtok(NULL, ",");
            col++;
        }
        row++;
    }

    fclose(file);
}

int determineAction(const Hand *playerHand, const Hand *dealerHand, char (*strategyArray)[STRAT_COLS])
{
    
    int playerTotal = getHandValue(playerHand);
    int dealerUpcard = getHandValue(dealerHand);


    if (playerTotal < STRAT_ROWS && dealerUpcard < STRAT_COLS) {
        printf("Action for player hand %d and dealer upcard %d: %c\n",
               playerTotal, dealerUpcard, strategyArray[playerTotal][dealerUpcard]);
        return strategyArray[playerTotal][dealerUpcard]; // Return action as a character
    } else {
        fprintf(stderr, "Error: Invalid hand values.\n");
        return -1; // Return error code
    }
    return 0;
}