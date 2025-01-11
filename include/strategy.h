
#ifndef STRATEGY_H
#define STRATEGY_H

#define STRAT_COLS 10 // Dealer's upcards (2 to Ace)

#include <stdio.h>
#include "hand.h"
#include "metadata.h"

// Strategy functions
char determine_action(const Hand *player_hand, int dealer_upcard, char (*strategy_array)[STRAT_COLS], Metadata *metadata);
void read_strategy_sheet(FILE *file, char (*strategy_array)[STRAT_COLS]);

#endif