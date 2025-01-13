#include <stdio.h>
#include <string.h>

#include "metadata.h"
#include "strategy.h"

#define STRAT_ROWS 37 // Number of player hand conditions
#define STRAT_COLS 10 // Dealer's upcards (2 to Ace)

void read_strategy_sheet(FILE *file, char (*strategy_array)[STRAT_COLS]) {
  char line[256];
  int row = 0;

  while (fgets(line, sizeof(line), file) && row < STRAT_ROWS) {
    // Tokenize the line to extract decisions
    char *token = strtok(line, ",");
    int col = 0;
    while (token != NULL && col < STRAT_COLS) {
      strategy_array[row][col] =
          token[0]; // Store the first character of the token
      token = strtok(NULL, ",");
      col++;
    }
    row++;
  }
  fclose(file);
}

char determine_action(const Hand *player_hand, int dealer_upcard,
                      char (*strategy_array)[STRAT_COLS], Metadata *metadata) {
  int player_total = get_hand_value(player_hand);
  int row_index;

  if (can_split(player_hand, metadata)) {
    int value =
        player_hand->cards[0].rank; // Store the card that we have two of
    // For splitable hands, decisions start at row #28 and index 27 (zero index
    // for array) The row index will stay at 27 for a deuce, and increase by one
    // for every value we increase, so we subtract 2 and add the value of the
    // single card
    row_index = 27 + value - 2;

    return strategy_array[row_index][dealer_upcard - 2];
  } else if (has_soft_ace(player_hand)) {
    // For soft hands, decisions start at row #19 and index 18 (zero index for
    // array) Totals start at soft 13 so we subtract 13
    row_index = 18 + player_total - 13;
    return strategy_array[row_index][dealer_upcard - 2];
  } else {
    // Player totals start at 4 (index 0) and end at 21 (index 17), so if total
    // is 4, the index is 0, so we subtract 4
    row_index = player_total - 4;
    return strategy_array[row_index][dealer_upcard - 2];
  }

  return '?'; // Return '?' if the strategy sheet does not contain a decision
              // for the given hand
}
