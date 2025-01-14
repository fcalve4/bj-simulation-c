#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "metadata.h"
#include "shoe.h"
#include "simulate.h"
#include "strategy.h"

// DEFAULT RULESET -> 6D H17 DAS 3:2 80%Pen
// DECLARE CONSTANTS FOR DEFAULT VALUES
#define NUM_DECKS 6
#define H17 1  // 1 for true
#define LS 0   // 1 for true
#define DAS 1  // 1 for true - TOGGLE FUNCTIONALITY NOT ADDED YET
#define RSA 0  // 0 for false - TOGGLE FUNCTIONALITY NOT ADDED YET
#define ENHC 0 // 0 for false
#define MAX_SPLITS 4
#define BJ_PAY 1.5
#define PEN 0.8 // % of shoe from 0-1 that is dealt out, 0.8 = 80%
#define NUM_SIMULATIONS 5000 // Number of shoes to play
#define BANKROLL 10000
#define WAGER 10

// Note - insurance is not implemented yet, but insurance is never taken in
// basic strategy, so this shouldn't affect basic RTP calculations

// Number of rows & cols for strategy sheet array creation. Not really sure what
// to do with these. They are constants but might change depending on how the
// strategy sheet is configured - EDIT: strategy sheet vals can probably be
// hardcoded
#define STRAT_ROWS 37 // Player hand possibilities
#define STRAT_COLS 10 // Dealer upcards

int main(int argc, char *argv[]) {

  // Initialize clock variables & start clock
  clock_t start, end;
  double cpu_time_used;
  start = clock();

  // Set configurables to their default values
  int num_decks = NUM_DECKS;
  int h17 = H17;
  int ls = LS;
  int das = DAS;
  int rsa = RSA;
  int enhc = ENHC;
  int max_splits = MAX_SPLITS;
  float bj_pay = BJ_PAY;
  float pen = PEN;
  int num_simulations = NUM_SIMULATIONS;
  int bankroll = BANKROLL;
  int wager = WAGER;

  // Check if user has provided minimum required arguments
  if (argc < 2) {
    fprintf(stderr, "Format: %s <strategy.csv> {Optional Flags}\n", argv[0]);
    return -1;
  }
  
  // Check for provided flags & set values accordingly
  for (int i=1; i < argc; i++) {
    if (strcmp(argv[i], "--num-decks") == 0) {
      num_decks = strtod(argv[i], NULL);
    }
    else if (strcmp(argv[i], "-h17") == 0) {
        h17 = strtod(argv[i], NULL);
    }
    else if (strcmp(argv[i], "-ls") == 0) {
        ls = strtod(argv[i], NULL);
    }
    else if (strcmp(argv[i], "-das") == 0) {
        das = strtod(argv[i], NULL);
    }
    else if (strcmp(argv[i], "-rsa") == 0) {
        rsa = strtod(argv[i], NULL);
    }
    else if (strcmp(argv[i], "-enhc") == 0) {
        enhc = strtod(argv[i], NULL);
    }
    else if (strcmp(argv[i], "--max-splits") == 0) {
        max_splits = strtod(argv[i], NULL);
    }
    else if (strcmp(argv[i], "--bj-pay") == 0) {
        bj_pay = strtof(argv[i], NULL);
    }
    else if (strcmp(argv[i], "-pen") == 0) {
        pen = strtof(argv[i], NULL);
    }
    else if (strcmp(argv[i], "--num-simulations") == 0) {
        num_simulations = strtod(argv[i], NULL);
    }
    else if (strcmp(argv[i], "-bankroll") == 0) {
        bankroll = strtod(argv[i], NULL);
    }
    else if (strcmp(argv[i], "-wager") == 0) {
        wager = strtod(argv[i], NULL);
    }
  }

  // Open strategy file
  FILE *strategy_file = fopen(argv[1], "r");

  // Seed random numbers
  srand(time(NULL));

  // Allocate memory for the strategy sheet
  // 40 rows (soft, hard, splits, surrenders AND 10 columns [2-A]
  char strategy[STRAT_ROWS][STRAT_COLS];
  memset(strategy, 0, sizeof(strategy));
  read_strategy_sheet(strategy_file, strategy);

  // Read the metadata into a struct
  Metadata metadata = { num_decks, h17, ls, das, rsa, enhc, 
                        max_splits, bj_pay, pen,
                        num_simulations, bankroll, wager, wager,  
                        0, 0, 0, 0, 0};

  // Call the simulate function to run the main game loop
  simulate(strategy, &metadata);

  // Dump metadata to terminal 
  printf("Total Shoes Played: %d\n", metadata.total_shoes_played);
  printf("Total Hands Played: %d\n", metadata.total_hands_played);
  printf("Total Wagered: %d\n", metadata.total_wagered);
  printf("Total Won: %d\n", metadata.total_won);
  printf("Bankroll: %d\n", metadata.bankroll);
  double rtp = 100 * (double)(metadata.total_won) / (metadata.total_wagered);
  printf("RTP: %f %%\n", rtp);
  
  // Stop the clock, calculate the elapsed time, print the result to terminal
  end = clock();
  cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("Execution Time: %f seconds\n", cpu_time_used);

  return 0;
}
