#ifndef STRATEGY_H
#define STRATEGY_H

#define STRAT_ROWS 40 // Number of player hand conditions
#define STRAT_COLS 10 // Dealer's upcards (2 to Ace)

#include "hand.h"

int determineAction(const Hand *playerHand, const Hand *dealerHand, char (*strategyArray)[STRAT_COLS]);
void readStrategySheet(const char *filename, char (*strategyArray)[STRAT_COLS]);
#endif