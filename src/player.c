#include "player.h"
#include <stdio.h>
#include <stdlib.h>

void init_player(Player *player) {
    init_hand(&player->hand);
    player->bankroll = 0;
}

