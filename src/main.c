#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "deck.h"
#include "strategy.h"
#include "simulate.h"
#include "metadata.h"

// DECLARE CONSTANTS (WILL BE USER INPUT FROM THE COMMAND LINE via flags [IMPLEMENTED LATER]
#define NUM_DECKS 6
#define H17 1 // 1 for true
#define DAS 1 // 1 for true - TOGGLE FUNCTIONALITY NOT ADDED YET
#define LS 1 // 1 for true 
#define RSA 0 // 0 for false - TOGGLE FUNCTIONALITY NOT ADDED YET
#define ENHC 0 // 0 for false 
#define MAX_SPLITS 4 // not factored, currently can split infinitely 
#define BJ_PAY 1.5
#define PEN 0.8 // Specifically, the percentage of the shoe from 0-1 that is dealt, 0.8 means 80% of the cards are dealt

#define NUM_SIMULATIONS 200 // Number of shoes to play
#define BANKROLL 10000
#define WAGER 10

// Number of rows & cols for strategy sheet array creation. Not really sure what to do with these.
// They are constants but might change depending on how the strategy sheet is configured - EDIT: strategy sheet vals can probably be hardcoded
#define STRAT_ROWS 37 // Player hand possibilities
#define STRAT_COLS 10 // Dealer upcards


int main(int argc, char *argv[]) {
    // Initialize clock variables & start clock
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    if (argc != 3) {
        fprintf(stderr, "Format: %s <strategy.csv> <output.txt>\n", argv[0]);
        return -1;
    }

    // Initialize in/out files
    FILE *strategy_file = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");

    // Seed random numbers
    srand(time(NULL));

    // Allocate memory for the strategy sheet 
    // 40 rows (soft, hard, splits, surrenders AND 10 columns [2-A]
    char strategy[STRAT_ROWS][STRAT_COLS];
    memset(strategy, 0, sizeof(strategy));
    read_strategy_sheet(strategy_file, strategy);

    // Read the metadata into a struct
    Metadata metadata = {NUM_DECKS, H17, LS, ENHC, BJ_PAY, PEN, NUM_SIMULATIONS, BANKROLL, WAGER, WAGER, 0, 0, 0, 0};


    // Call the simulate function to run the main game loop
    simulate(out, NUM_SIMULATIONS, strategy, &metadata);

    printf("Total Shoes Played: %d\n", metadata.total_shoes_played);
    printf("Total Hands Played: %d\n", metadata.total_hands_played);
    printf("Total Wagered: %d\n", metadata.total_wagered);
    printf("Total Won: %d\n", metadata.total_won);
    printf("Bankroll: %d\n", metadata.bankroll);
    printf("RTP : %f\n", (metadata.total_won )/ (float)metadata.total_wagered);

    // Stop the clock, calculate the elapsed time, print the result to terminal
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Execution Time: %f seconds\n", cpu_time_used);

    return 0;
}

