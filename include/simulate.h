#ifndef SIMULATE_H
#define SIMULATE_H

#include "hand.h"
#include "deck.h"
#include "player.h"
#include "metadata.h"

#define STRAT_COLS 10

// Simulate functions
void play_shoe(FILE* out, Player *player, Player *dealer, char (*strategy)[STRAT_COLS], Metadata *metadata);
void check_for_naturals(FILE* out, Hand *player_hand, Hand *dealer_hand);
int play_player_turn(FILE *out, Player *player, Player *dealer, Deck *deck, char (*strategy)[STRAT_COLS], int dealer_upcard, Metadata *metadata);
void play_dealer_turn(FILE *out, Hand *dealer_hand, Deck *deck, int h17);
void determine_winner(FILE* out, Hand *player_hand, Hand *dealer_hand);
void simulate(FILE *out, int num_simulations, char (*strategy)[STRAT_COLS], Metadata *metadata);

#endif

