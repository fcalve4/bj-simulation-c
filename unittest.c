#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "deck.h"
#include "player.h"
#include "strategy.h"

#define STRAT_ROWS 37 // Player hand possibilities
#define STRAT_COLS 10 // Dealer upcards

int main() {

    Deck deck;

    initDeck(&deck, 1);

    for (int i = 0; i < 52; i++) {
        printf("%d\n", deck.cards[i]);
    }
    return 0;
}