#include <stdio.h>

#include "metadata.h"
#include "shoe.h"
#include "simulate.h"
#include "strategy.h"

#define STRAT_COLS 10

void play_hand(Shoe *shoe, Hand *player_hand, Hand *dealer_hand,
               char (*strategy)[STRAT_COLS], Metadata *metadata) {
  // Increment hand counter
  metadata->total_hands_played++;
  //  RESET WAGER TO DEFAULT
  metadata->wager = metadata->wager_static;
  // Init hands
  init_hand(player_hand);
  init_hand(dealer_hand);

  // Deal initial cards to player
  add_card_to_hand(player_hand, deal_card(shoe));
  add_card_to_hand(player_hand, deal_card(shoe));

  // Deal initial cards to dealer
  Card dealer_upcard = deal_card(shoe);
  add_card_to_hand(dealer_hand, dealer_upcard);
  // Grab dealer upcard value, store it separately (to check for naturals)

  int dealer_upcard_value = get_hand_value(dealer_hand);
  //  Deal the dealer another hand
  add_card_to_hand(dealer_hand, deal_card(shoe));
  // If enhc is false (usually it is), check for naturals immediately
  if (metadata->enhc == 0) {
    // If a natural is returned, return to the start of the loop, else continue
    // play as normal
    if (check_for_naturals(player_hand, dealer_hand, metadata) == 1) {
      free_hands(player_hand, dealer_hand);
      return;
    }
  }
  // Player turn to act
  int player_turn_loop_bool = 1; // LOOP ACTIVE BOOLEAN
  // While the player is still acting, call the player turn function
  while (player_turn_loop_bool == 1 && get_hand_value(player_hand) < 21) {
    player_turn_loop_bool =
        play_player_turn(player_hand, dealer_hand, shoe, strategy,
                         dealer_upcard, dealer_upcard_value, metadata);
  }
  if (player_turn_loop_bool == 2) // IF THEY PLAYER HAS SURRENDERD,
  {
    // continue to the start of the loop and don't evaluate hands
    return;
  }
  // If enhc is true (rarely is), check for naturals after player acts
  // Maybe we can do some work on this so we dont have 2 separate checks for if
  // ehnc == 1. not what i would change tho.
  if (metadata->enhc == 1) {
    // If a natural is returned, return to the start of the loop, else continue
    // play as normal
    if (check_for_naturals(player_hand, dealer_hand, metadata) == 1) {
      free_hands(player_hand, dealer_hand);
      return;
    }
  }
  // -=-=-DEALER TURN TO ACT-=-=-
  play_dealer_turn(dealer_hand, shoe, metadata->h17);
  // Determine the winner
  determine_winner(player_hand, dealer_hand, metadata);

  free_hands(player_hand, dealer_hand);
}

void play_shoe(Hand *player_hand, Hand *dealer_hand,
               char (*strategy)[STRAT_COLS], Metadata *metadata) {
  // Init and shuffle the shoe
  Shoe shoe;
  init_shoe(&shoe, metadata->num_decks);
  shuffle_shoe(&shoe);

  // Play hands until cut card is reached
  while (shoe.top <= shoe.capacity * metadata->pen) {
    play_hand(&shoe, player_hand, dealer_hand, strategy, metadata);
  }
  free_shoe(&shoe);
}

void split(Hand *player_hand, Hand *dealer_hand, Shoe *shoe,
           char (*strategy)[STRAT_COLS], Card dealer_upcard,
           int dealer_upcard_value, Metadata *metadata) {
  // Grab the split card
  Card split_card = player_hand->cards[0];
  //  Create a new loop for the split off hand.
  int split_loop_bool = 1;
  // Reinitialize the player's hand and add the split card + additional card
  init_hand(player_hand);
  add_card_to_hand(player_hand, split_card);
  add_card_to_hand(player_hand, deal_card(shoe));
  while (split_loop_bool) {
    // Call the player turn function for the split off hand and play normally
    // This should just play another hand and then once this loop breaks, the
    // next hand will play via the main loop
    split_loop_bool =
        play_player_turn(player_hand, dealer_hand, shoe, strategy,
                         dealer_upcard, dealer_upcard_value, metadata);
  }
  // Play dealer turn
  play_dealer_turn(player_hand, shoe, metadata->h17);
  // Determine winner of the hand
  determine_winner(player_hand, dealer_hand, metadata);

  // Free the hands
  free_hands(player_hand, dealer_hand);
  // Reinitialize the hands
  init_hand(player_hand);
  init_hand(dealer_hand);

  // Add the split card to the player's hand & dealer upcard to dealer's hand
  add_card_to_hand(player_hand, split_card);
  add_card_to_hand(player_hand, deal_card(shoe));
  add_card_to_hand(dealer_hand, dealer_upcard);
  // Return to start of loop to continue playing the hand
}

// Player "make decision" function
// This function will determine the player's action based on the strategy sheet
// and returns 0 or 1 depending on if loop continues or breaks.
// returns 2 for surrender, as this is a special case
int play_player_turn(Hand *player_hand, Hand *dealer_hand,
                     Shoe *shoe, char (*strategy)[STRAT_COLS],
                     Card dealer_upcard, int dealer_upcard_value,
                     Metadata *metadata) {

  // Determine player action based on strategy sheet
  char action;
  action =
      determine_action(player_hand, dealer_upcard_value, strategy, metadata);


  // ORDER OF OPERATIONS
  // 1. Can/Should the player surrender?
  // 2. Can/Should the player split?
  // 2. Can/Should the player double?
  // 2. Should the player hit/stand?

  // Player Surrender otherwise stand
  if (action == 'X') {
    // Check if surrender is possible
    if (metadata->ls == 1 && can_surrender(player_hand)) {
      metadata->total_wagered += metadata->wager;
      metadata->total_won += metadata->wager / 2;
      metadata->bankroll -= metadata->wager / 2;
      return 2;
    }
    return 0;
  }
  // Player Surrender otherwise hit
  else if (action == 'Y') {
    // Check if surrender is possible
    if (metadata->ls == 1 && can_surrender(player_hand)) {
      metadata->total_wagered += metadata->wager;
      metadata->total_won += metadata->wager / 2;
      metadata->bankroll -= metadata->wager / 2;
      return 2;
    }
    // Else hit
    add_card_to_hand(player_hand, deal_card(shoe));
  }
  // Player Surrender otherwise split
  else if (action == 'Z') {
    // Check if surrender is possible
    if (metadata->ls == 1 && can_surrender(player_hand)) {
      metadata->total_wagered += metadata->wager;
      metadata->total_won += metadata->wager / 2;
      metadata->bankroll -= metadata->wager / 2;
      return 2;
    }
    // else split
    else {
      if (can_split(player_hand, metadata)) {
        // Split the hand
        metadata->num_times_split++;
        split(player_hand, dealer_hand, shoe, strategy, dealer_upcard,
              dealer_upcard_value, metadata);
      }
    }
  }

  // Player split
  else if (action == 'P') {
    if (can_split(player_hand, metadata)) {
      // Split the hand
      metadata->num_times_split++;
      split(player_hand, dealer_hand, shoe, strategy, dealer_upcard,
            dealer_upcard_value, metadata);
    }
  }

  // Player Double otherwise hit
  else if (action == 'D') {
    if (can_double(player_hand)) {
      // Double the wager
      metadata->wager *= 2;
      add_card_to_hand(player_hand, deal_card(shoe));
      return 0;
    } else {
      add_card_to_hand(player_hand, deal_card(shoe));
    }
  }

  // Player Double otherwise stand
  else if (action == 'T') {
    if (can_double(player_hand)) {
      // Double the wager
      metadata->wager *= 2;
      add_card_to_hand(player_hand, deal_card(shoe));
      return 0;
    }
    return 0;
  }
  // Player hit - add 1 card and check for bust
  else if (action == 'H') {
    add_card_to_hand(player_hand, deal_card(shoe));
  }
  // Player stand - no action needed
  else if (action == 'S') {
    return 0;
  } else {
    return 0;
  }
  return 1; // Default is to hit
}


void play_dealer_turn(Hand *dealer_hand, Shoe *shoe, int h17) {
  //  Dealer turn to act
  //  case 1: H17, dealer hits on soft 17s
  if (h17 == 1) {
    while (get_hand_value(dealer_hand) < 17 ||
           ((has_soft_ace(dealer_hand) == 1) &&
            get_hand_value(dealer_hand) == 17)) {
      add_card_to_hand(dealer_hand, deal_card(shoe));
    }
  } else { // case 2: S17, dealer stands on all 17s 
    while (get_hand_value(dealer_hand) < 17) {
      add_card_to_hand(dealer_hand, deal_card(shoe));
    }
  }
}

void determine_winner(Hand *player_hand, Hand *dealer_hand,
                      Metadata *metadata) {
  // Determine winner, Player wins if dealer busts or has a higher hand

  if (is_bust(player_hand)) {
    //  Update metadata counters
    metadata->total_wagered += metadata->wager;
    metadata->bankroll -= metadata->wager;
    return;
  } else if (is_bust(dealer_hand)) {
    //  Update metadata counters
    metadata->total_won += metadata->wager * 2;
    metadata->total_wagered += metadata->wager;
    metadata->bankroll += metadata->wager;
    return;
  }

  if (get_hand_value(player_hand) > get_hand_value(dealer_hand)) {
    // Update metadata counters
    metadata->total_won += metadata->wager * 2;
    metadata->total_wagered += metadata->wager;
    metadata->bankroll += metadata->wager;
    return;
  } else if (get_hand_value(player_hand) < get_hand_value(dealer_hand)) {
    //  Update metadata counters
    metadata->total_wagered += metadata->wager;
    metadata->bankroll -= metadata->wager;
    return;
  } else {
    //  Update metadata counters
    metadata->total_won += metadata->wager;
    metadata->total_wagered += metadata->wager;
    return;
  }
}

// CONTINUES TO NEXT HAND IF A NATURAL OCCURS, NEED TO FREE HANDS IF THIS
// HAPPENS TOO
int check_for_naturals(Hand *player_hand, Hand *dealer_hand,
                       Metadata *metadata) {
  // If both players have blackjack
  if (get_hand_value(player_hand) == 21 && get_hand_value(dealer_hand) == 21) {
    // Update metadata counters
    metadata->total_won += metadata->wager * metadata->bj_pay;
    metadata->total_wagered +=
        metadata->wager * metadata->bj_pay + metadata->wager;
    return 1;
  }
  // If player has blackjack
  else if (get_hand_value(player_hand) == 21) {
    // Update metadata counters
    metadata->total_won += metadata->wager * metadata->bj_pay + metadata->wager;
    metadata->total_wagered += metadata->wager;
    metadata->bankroll += metadata->wager * metadata->bj_pay;
    return 1;
  }
  // If dealer has blackjack
  else if (get_hand_value(dealer_hand) == 21) {
    // Update metadata counters
    metadata->total_wagered += metadata->wager;
    metadata->bankroll -= metadata->wager;
    return 1;
  }
  // If neither player has blackjack nothing happens
  return 0;
}

void simulate(char (*strategy)[STRAT_COLS],
              Metadata *metadata) {
  Hand player_hand;
  Hand dealer_hand;
  for (int i = 0; i < metadata->num_simulations; i++) {
    play_shoe(&player_hand, &dealer_hand, strategy, metadata);
    metadata->total_shoes_played++;
  }
}