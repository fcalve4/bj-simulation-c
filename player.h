#ifndef PLAYER_H
#define PLAYER_H

#include "hand.h"

typedef struct {
    Hand hand;

} Player;

// Player functions
void initPlayer(Player *player);

#endif
