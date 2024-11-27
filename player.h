#ifndef PLAYER_H
#define PLAYER_H

#include "hand.h"

typedef struct {
    Hand hand;
    int bankroll; // Player's money
} Player;

// Player functions
void initPlayer(Player *player, int bankroll);
void placeBet(Player *player, int bet);
void winBet(Player *player, int bet);
void loseBet(Player *player, int bet);

#endif
