#ifndef METADATA_H
#define METADATA_H

#include <stdint.h>

// Metadata struct for storing user input / game constants

typedef struct {
  // Rules
  int num_decks;
  int h17;
  int ls;
  int das;
  int rsa;
  int enhc;
  int max_splits;
  float bj_pay;
  float pen;

  // Simulation parameters
  int bankroll;
  int wager;
  int wager_static; // temporary variable for splits

  // Tracked metrics
  int total_shoes_played;
  int total_hands_played;
  int total_wagered;
  int total_won;

  // REQUIRED TRACKING VARIABLES
  int num_times_split;

} Metadata;

#endif
