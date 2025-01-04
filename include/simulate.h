#ifndef SIMULATE_H
#define SIMULATE_H

#include "hand.h"
#include "deck.h"
#include "player.h"

void play_shoe(Player *player, Player *dealer, char **strategy, int num_decks, int penetration, int h17, int ls, int enhc);

int check_for_naturals(Hand *playerhand, Hand *dealerhand);

void simulate();

#endif

