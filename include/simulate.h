#ifndef SIMULATE_H
#define SIMULATE_H

#include "hand.h"
#include "deck.h"
#include "player.h"

#define STRAT_COLS 10

// Simulate functions
void play_shoe(Player *player, Player *dealer, char (*strategy)[STRAT_COLS], int num_decks, int penetration, int h17, int ls, int enhc);
int check_for_naturals(Hand *player_hand, Hand *dealer_hand);
int play_player_turn(Player *player, Deck *deck, char (*strategy)[STRAT_COLS], int dealer_upcard, int ls);
void simulate(int num_simulations, char (*strategy)[STRAT_COLS], int num_decks, int pen, int h17, int ls, int enhc);

#endif

