#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "deck.h"
#include "player.h"
#include "strategy.h"

#define STRAT_ROWS 37 // Player hand possibilities
#define STRAT_COLS 10 // Dealer upcards

int main() {

    char strategy[STRAT_ROWS][STRAT_COLS];

    readStrategySheet("strategy.csv", strategy);

    
    for (int i = 0; i < STRAT_ROWS; i++) {
        printf("row #%d ", i+ 1);
        for (int j=0; j < STRAT_COLS; j++) {
            printf("%c ", strategy[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}