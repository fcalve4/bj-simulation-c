#include <stdio.h>
#include <string.h>
#include "strategy.h"

#define STRAT_ROWS 37 // Number of player hand conditions
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

char determineAction(const Hand *playerHand, int dealerUpcard, char (*strategyArray)[STRAT_COLS])
{
    int playerTotal = getHandValue(playerHand);
    int rowIndex;

    if (canSplit(playerHand))
    {
        int value = playerHand->cards[0].rank; // Store the card that we have two of
        // For splitable hands, decisions start at row #28 and index 27 (zero index for array)
        // The row index will stay at 27 for a deuce, and increase by one for every value we increase, so we subtract 2 and add the value of the single card
        rowIndex = 27 + value - 2;

        return strategyArray[rowIndex][dealerUpcard - 2];
    }
    else if (hasSoftAce(playerHand))
    {
         // For soft hands, decisions start at row #19 and index 18 (zero index for array)
         // Totals start at soft 13 so we subtract 13
        rowIndex = 18 + playerTotal - 13;
        return strategyArray[rowIndex][dealerUpcard - 2];
    }
    else
    {
        // Player totals start at 4 (index 0) and end at 21 (index 17), so if total is 4, the index is 0, so we subtract 4
        rowIndex = playerTotal - 4;
        return strategyArray[rowIndex][dealerUpcard - 2];

    }
    
    return '?'; // Return '?' if the strategy sheet does not contain a decision for the given hand
    
}
