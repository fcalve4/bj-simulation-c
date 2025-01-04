#include "player.h"
#include <stdio.h>
#include <stdlib.h>

void initPlayer(Player *player) {
    initHand(&player->hand);
    player->bankroll = 0;
}

