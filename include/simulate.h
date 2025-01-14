#ifndef SIMULATE_H
#define SIMULATE_H

#include "hand.h"
#include "metadata.h"
#include "shoe.h"

#define STRAT_COLS 10

// Simulate functions
void play_hand(Shoe *shoe, Hand *player_hand, Hand *dealer_hand,
               char (*strategy)[STRAT_COLS], Metadata *metadata);
void play_shoe(Hand *player_hand, Hand *dealer_hand,
               char (*strategy)[STRAT_COLS], Metadata *metadata);
int check_for_naturals(Hand *player_hand, Hand *dealer_hand,
                       Metadata *metadata);
int play_player_turn(Hand *player_hand, Hand *dealer_hand,
                     Shoe *shoe, char (*strategy)[STRAT_COLS],
                     Card dealer_upcard, int dealer_upcard_value,
                     Metadata *metadata);
void play_dealer_turn(Hand *dealer_hand, Shoe *shoe, int h17);
void determine_winner(Hand *player_hand, Hand *dealer_hand,
                      Metadata *metadata);
void split(Hand *player_hand, Hand *dealer_hand, Shoe *shoe,
           char (*strategy)[STRAT_COLS], Card dealer_upcard,
           int dealer_upcard_value, Metadata *metadata);
void simulate(char (*strategy)[STRAT_COLS], Metadata *metadata);

#endif
