#ifndef SIMULATE_H
#define SIMULATE_H

#include "hand.h"
#include "deck.h"
#include "metadata.h"

#define STRAT_COLS 10

// Simulate functions
void play_shoe(FILE* out, Hand *player_hand, Hand *dealer_hand, char (*strategy)[STRAT_COLS], Metadata *metadata);
int check_for_naturals(FILE* out, Hand *player_hand, Hand *dealer_hand, Metadata *metadata);
int play_player_turn(FILE *out, Hand *player_hand, Hand *dealer_hand, Deck *deck, char (*strategy)[STRAT_COLS], Card dealer_upcard, int dealer_upcard_value, Metadata *metadata);
void play_dealer_turn(FILE *out, Hand *dealer_hand, Deck *deck, int h17);
void determine_winner(FILE* out, Hand *player_hand, Hand *dealer_hand, Metadata *metadata);
void split(FILE *out, Hand *player_hand, Hand *dealer_hand, Deck *deck, char (*strategy)[STRAT_COLS], Card dealer_upcard, int dealer_upcard_value, Metadata *metadata);
void simulate(FILE *out, int num_simulations, char (*strategy)[STRAT_COLS], Metadata *metadata);

#endif

