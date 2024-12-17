#include "player.h"
#include <stdio.h>
#include <stdlib.h>

void initPlayer(Player *player, int bankroll, int wager_size) {
    initHand(&player->hand);
    player->wager_size = wager_size;
    player->bankroll = bankroll;
}

