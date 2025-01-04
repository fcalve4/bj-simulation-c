#include <stdio.h>
#ifndef STRATEGY_H
#define STRATEGY_H


#define STRAT_COLS 10 // Dealer's upcards (2 to Ace)

#include "hand.h"

// Strategy functions
char determineAction(const Hand *playerHand, int dealerUpcard, char (*strategyArray)[STRAT_COLS]);
void readStrategySheet(FILE *file, char (*strategyArray)[STRAT_COLS]);

#endif