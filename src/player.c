#include "player.h"
#include <stdio.h>
#include <stdlib.h>

void init_player(Player *player) {
    initHand(&player->hand);
    player->bankroll = 0;
}

