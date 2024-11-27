#include "player.h"
#include <stdio.h>
#include <stdlib.h>

void initPlayer(Player *player, int bankroll) {
    initHand(&player->hand);
    player->bankroll = bankroll;
}

void placeBet(Player *player, int bet) {
    if (bet > player->bankroll) {
        fprintf(stderr, "Error: Insufficient funds.\n");
        exit(1);
    }
    player->bankroll -= bet;
}

void winBet(Player *player, int bet) {
    player->bankroll += 2 * bet;
}

void loseBet(Player *player, int bet) {
    // No action needed; bet is already subtracted
}
