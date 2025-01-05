#include <stdio.h>

#include "hand.h"
#include "deck.h"
#include "strategy.h"
#include "simulate.h"
#include "metadata.h"

#define STRAT_COLS 10


void play_shoe(FILE *out, Hand *player_hand, Hand *dealer_hand, char (*strategy)[STRAT_COLS], Metadata *metadata) {
    // Init and shuffle the deck
    Deck deck;
    init_deck(&deck, metadata->num_decks);
    shuffle_deck(&deck);

    // Play hands until cut card is reached
    while (deck.top <= deck.capacity * metadata->pen) {
        // Increment hand counter
        metadata->total_hands_played++;
        fprintf(out, "Total Hands Played: %d\n", metadata->total_hands_played);

        // Init hands
        init_hand(player_hand);
        init_hand(dealer_hand);
        
        // Deal initial cards to player
        add_card_to_hand(player_hand, deal_card(&deck));
        add_card_to_hand(player_hand, deal_card(&deck));
        

        // Deal initial cards to dealer
        add_card_to_hand(dealer_hand, deal_card(&deck));
        // Grab the dealer upcard value now and store it separately (this is to check for naturals)
        int dealer_upcard = get_hand_value(dealer_hand);
        fprintf(out, "--Dealer upcard: %d\n", dealer_upcard);

        // Deal the dealer another hand
        add_card_to_hand(dealer_hand, deal_card(&deck));

        // If enhc is false (usually it is), check for naturals immediately
        if (metadata->enhc == 0) {
            check_for_naturals(out, player_hand, dealer_hand, metadata);
        }

        // -=-=-PLAYER TURN TO ACT-=-=-
        int player_turn_loop_bool = 1; // LOOP BOOLEAN

        // While the player is still acting, call the player turn function
        while (player_turn_loop_bool) {
            fprintf(out, "--Player hand total: %d\n", get_hand_value(player_hand));
            player_turn_loop_bool = play_player_turn(out, player_hand, dealer_hand, &deck, strategy, dealer_upcard, metadata);
        }

        // If enhc is true (rarely is), check for naturals after player acts
        if (metadata->enhc == 1) {
            check_for_naturals(out, player_hand, dealer_hand, metadata);
        }

        // -=-=-DEALER TURN TO ACT-=-=-
        play_dealer_turn(out, dealer_hand, &deck, metadata->h17);

        // Determine the winner
        determine_winner(out, player_hand, dealer_hand, metadata);
    }
}





// Player "make decision" function
// This function will determine the player's action based on the strategy sheet
    // and returns 0 or 1 depending on if loop continues or breaks.
int play_player_turn(FILE *out, Hand *player_hand, Hand *dealer_hand, Deck *deck, char (*strategy)[STRAT_COLS], int dealer_upcard, Metadata *metadata) {
    // Determine player action based on strategy sheet
    char action = determine_action(player_hand, dealer_upcard, strategy);
    fprintf(out, "PLAYER ACTION: %c\n", action);
    // Player hit - add 1 card and check for bust
    if (action == 'H') {
        add_card_to_hand(player_hand, deal_card(deck));
        if (is_bust(player_hand)) {
            fprintf(out, "Player busts!\n");
            return 0;
        }
    }
    // Player stand - no action needed
    else if (action == 'S') {
        return 0;
    }
    // Player split
    else if (action == 'P') {
        // Grab the split card
        Card split_card = player_hand->cards[0];
        fprintf(out, "-------PLAYER SPLITTING!------[%d]----\n", split_card.rank);
        // Create a new loop for the split off hand.
        int split_loop_bool = 1;
        // Reinitialize the player's hand and add the split card + additional card
        init_hand(player_hand);
        add_card_to_hand(player_hand, split_card);
        add_card_to_hand(player_hand, deal_card(deck));
        while (split_loop_bool) {
            // Call the player turn function for the split off hand and play normally
            // This should just play another hand and then once this loop breaks, the next hand will play via the main loop
            fprintf(out, "--Player hand total: %d\n", get_hand_value(player_hand));
            split_loop_bool = play_player_turn(out, player_hand, dealer_hand, deck, strategy, dealer_upcard, metadata);
        }
        // Play dealer turn
        play_dealer_turn(out, player_hand, deck, metadata->h17);
        // Determine winner of the hand
        determine_winner(out, player_hand, dealer_hand, metadata);
        
        
        // Reinitialize the player's hand and add the split card
        init_hand(player_hand);
        add_card_to_hand(player_hand, split_card);
        add_card_to_hand(player_hand, deal_card(deck));
        // Return to start of loop to continue playing the hand
    }


    // Player Double otherwise hit
    else if (action == 'D') {
        if (can_double(player_hand))
        {
            // Double the wager
        }
        add_card_to_hand(player_hand, deal_card(deck));
        if (is_bust(player_hand)) { // Is this statement necesarry? will the player ever bust when action is 'D'?
            fprintf(out, "Player busts!\n");
            return 0;
        }
    }
    // Player Double otherwise stand
    else if (action == 'T') {
        if (can_double(player_hand))
        {
            // Double the wager
            add_card_to_hand(player_hand, deal_card(deck));
        }
        if (is_bust(player_hand)) {
            fprintf(out, "Player busts!\n");
            return 0;
        }
        return 0;
    }
    // Player Surrender otherwise stand
    else if (action == 'X') {
        if (can_surrender(player_hand) && metadata->ls == 1)
        {
            return 0;
        }
        return 0;
    }
    // Player Surrender otherwise hit
    else if (action == 'Y') {
        if (can_surrender(player_hand) && metadata->ls == 1) {
            return 0;
        }
        // Else hit
        add_card_to_hand(player_hand, deal_card(deck));
        if (is_bust(player_hand)) {
            fprintf(out, "Player busts!\n");
            return 0;
        }
    }
    // Player Surrender otherwise split
    else if (action == 'Z') {
        if (can_surrender(player_hand) && metadata->ls == 1) {
            return 0;
        }
        // else split
        return 0;
    }
    else {
        fprintf(out, "ERROR: INVALID DECISION SELECTED\n");
        return 0;
    }
    return 1; // Default is to hit
}

void play_dealer_turn(FILE *out, Hand *dealer_hand, Deck *deck, int h17)
{
    // -=-=-DEALER TURN TO ACT-=-=-
    // case 1: H17, dealer hits on soft 17s
    if (h17 == 1) {
        while (get_hand_value(dealer_hand) < 17 || ((has_soft_ace(dealer_hand) == 1) && get_hand_value(dealer_hand) == 17)) {
        add_card_to_hand(dealer_hand, deal_card(deck));
        fprintf(out, "Dealer hand total: %d\n", get_hand_value(dealer_hand));
        }
    }
    // case 2: S17, dealer stands on all 17s
    else {
        while (get_hand_value(dealer_hand) < 17) {
        add_card_to_hand(dealer_hand, deal_card(deck));
        fprintf(out, "Dealer hand total: %d\n", get_hand_value(dealer_hand));
        }
    }
}

void determine_winner(FILE *out, Hand *player_hand, Hand *dealer_hand, Metadata *metadata) {
    // Determine winner, Player wins if dealer busts or has a higher hand
    if (is_bust(player_hand) || get_hand_value(player_hand) > get_hand_value(dealer_hand)) {
        fprintf(out, "---Player wins!\n");
        metadata->bankroll += metadata->wager;
    }
    else if (get_hand_value(player_hand) < get_hand_value(dealer_hand)) {
        fprintf(out, "---Dealer wins!\n");
        metadata->bankroll -= metadata->wager;
    }
    else {
        fprintf(out, "---It's a push!\n");
    }
}


void check_for_naturals(FILE *out, Hand *playerhand, Hand *dealerhand, Metadata *metadata) {
    // If both players have blackjack
    if (get_hand_value(playerhand) == 21 && get_hand_value(dealerhand) == 21) {
        fprintf(out, "---NATURAL PUSH\n");
        return;
    }
    // If player has blackjack
    else if (get_hand_value(playerhand) == 21) {
        fprintf(out, "---PLAYER BLACKJACK\n");
        metadata->bankroll += metadata->wager * metadata->bj_pay;
        return;
    }
    // If dealer has blackjack
    else if (get_hand_value(dealerhand) == 21) {
        fprintf(out, "---DEALER BLACKJACK\n");
        metadata->bankroll -= metadata->wager;
        return;
    }
    // If neither player has blackjack nothing happens
    return;
}

void simulate(FILE *out, int num_simulations, char (*strategy)[STRAT_COLS], Metadata *metadata) {
    Hand player_hand;
    Hand dealer_hand;
    for (int i=0; i < num_simulations; i++)
    {   
        metadata->total_shoes_played++;
        play_shoe(out, &player_hand, &dealer_hand, strategy, metadata);
    }
}