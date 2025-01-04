#ifndef SIMULATE_H
#define SIMULATE_H

#include "hand.h"
#include "deck.h"
#include "player.h"

#define STRAT_COLS 10

// Simulate functions
void play_shoe(Player *player, Player *dealer, char (*strategy)[STRAT_COLS], int num_decks, int penetration, int h17, int ls, int enhc);
int check_for_naturals(Hand *playerhand, Hand *dealerhand);
void simulate();

#endif

