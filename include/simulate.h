#ifndef SIMULATE_H
#define SIMULATE_H

#include "hand.h"
#include "deck.h"
#include "player.h"
#include "metadata.h"

#define STRAT_COLS 10

// Simulate functions
void play_shoe(Player *player, Player *dealer, char (*strategy)[STRAT_COLS], Metadata *metadata);
int check_for_naturals(Hand *player_hand, Hand *dealer_hand);
int play_player_turn(Player *player, Player *dealer, Deck *deck, char (*strategy)[STRAT_COLS], int dealer_upcard, Metadata *metadata);
void play_dealer_turn(Hand *dealer_hand, Deck *deck, int h17);
void determine_winner(Hand *player_hand, Hand *dealer_hand);
void simulate(int num_simulations, char (*strategy)[STRAT_COLS], Metadata *metadata);

#endif

