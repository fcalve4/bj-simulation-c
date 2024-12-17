#ifndef PLAYER_H
#define PLAYER_H

#include "hand.h"

typedef struct {
    Hand hand;
    int bankroll; // Player's money
    int wager_size;
} Player;

// Player functions
void initPlayer(Player *player, int bankroll, int wager_size);

#endif
