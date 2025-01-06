#include <stdio.h>

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
    int hand_counter = 0;


    // Play hands until cut card is reached
    while (deck.top <= deck.capacity * metadata->pen) {
        fprintf(out, "Hand #%d\n", hand_counter++);
        // Increment hand counter
        metadata->total_hands_played++;
        fprintf(out, "Total Hands Played: %d\n", metadata->total_hands_played);

        // RESET WAGER TO DEFAULT
        metadata->wager = metadata->wager_static;

        // Init hands
        init_hand(player_hand);
        init_hand(dealer_hand);
        
        // Deal initial cards to player
        add_card_to_hand(player_hand, deal_card(&deck));
        add_card_to_hand(player_hand, deal_card(&deck));
        

        // Deal initial cards to dealer
        Card dealer_upcard = deal_card(&deck);
        add_card_to_hand(dealer_hand, dealer_upcard);
        // Grab the dealer upcard value now and store it separately (this is to check for naturals)
        int dealer_upcard_value = get_hand_value(dealer_hand);
        fprintf(out, "--Dealer upcard value: %d\n", dealer_upcard_value);

        // Deal the dealer another hand
        add_card_to_hand(dealer_hand, deal_card(&deck));

        // If enhc is false (usually it is), check for naturals immediately
        if (metadata->enhc == 0) {
            // If a natural is returned, return to the start of the loop, else continue play as normal
            if (check_for_naturals(out, player_hand, dealer_hand, metadata) == 1) {
                free_hands(player_hand, dealer_hand);
                continue;
            }
        }

        // -=-=-PLAYER TURN TO ACT-=-=-
        int player_turn_loop_bool = 1; // LOOP BOOLEAN

        // While the player is still acting, call the player turn function
        while (player_turn_loop_bool && get_hand_value(player_hand) < 21) {
            fprintf(out, "--Player hand total: %d\n", get_hand_value(player_hand));
            player_turn_loop_bool = play_player_turn(out, player_hand, dealer_hand, &deck, strategy,dealer_upcard, dealer_upcard_value, metadata);
        }

        // If enhc is true (rarely is), check for naturals after player acts
        // Maybe we can do some work on this so we dont have 2 separate checks for if ehnc == 1. not what i would change tho.
        if (metadata->enhc == 1) {
            // If a natural is returned, return to the start of the loop, else continue play as normal
            if (check_for_naturals(out, player_hand, dealer_hand, metadata) == 1)
            {
                free_hands(player_hand, dealer_hand);
                continue;
            }
        }

        // -=-=-DEALER TURN TO ACT-=-=-
        play_dealer_turn(out, dealer_hand, &deck, metadata->h17);

        // Determine the winner
        determine_winner(out, player_hand, dealer_hand, metadata);
        
        free_hands(player_hand, dealer_hand);
    }
    free_deck(&deck);
}





// Player "make decision" function
// This function will determine the player's action based on the strategy sheet
    // and returns 0 or 1 depending on if loop continues or breaks.
int play_player_turn(FILE *out, Hand *player_hand, Hand *dealer_hand, Deck *deck, char (*strategy)[STRAT_COLS], Card dealer_upcard, int dealer_upcard_value, Metadata *metadata) {
    // Determine player action based on strategy sheet
    char action;
    action = determine_action(player_hand, dealer_upcard_value, strategy);
    fprintf(out, "PLAYER ACTION: %c\n", action);

    // Player hit - add 1 card and check for bust
    if (action == 'H') {
        add_card_to_hand(player_hand, deal_card(deck));
    }
    // Player stand - no action needed
    else if (action == 'S') {
        return 0;
    }
    // Player split
    // It looks like theres a bug in the split logic
        // When player splits for the first hand the dealer logic isnt played out, dealer stands on 14, stuff like this etc
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
            split_loop_bool = play_player_turn(out, player_hand, dealer_hand, deck, strategy, dealer_upcard, dealer_upcard_value, metadata);
        }
        // Play dealer turn
        play_dealer_turn(out, player_hand, deck, metadata->h17);
        // Determine winner of the hand
        determine_winner(out, player_hand, dealer_hand, metadata);

        // Free the hands
        free_hands(player_hand, dealer_hand);
        // Reinitialize the hands
        init_hand(player_hand);
        init_hand(dealer_hand);

        // Add the split card to the player's hand & dealer upcard to dealer's hand
        add_card_to_hand(player_hand, split_card);
        add_card_to_hand(player_hand, deal_card(deck));
        add_card_to_hand(dealer_hand, dealer_upcard);
        // Return to start of loop to continue playing the hand
    }


    // Player Double otherwise hit
    else if (action == 'D') {
        if (can_double(player_hand)) {
            // Double the wager
            metadata->wager *= 2;
        }
        add_card_to_hand(player_hand, deal_card(deck));
    }
    
    // Player Double otherwise stand
    else if (action == 'T') {
        if (can_double(player_hand)) {
            // Double the wager
            add_card_to_hand(player_hand, deal_card(deck));
        }
        return 0;
    }
    // Player Surrender otherwise stand
    else if (action == 'X') {
        if (can_surrender(player_hand) && metadata->ls == 1) {
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

// Fix this somewhere else but dealer logic plays out even when player busts, this is unnecesarry and a waste
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
    fprintf(out, "Dealer Total: %d | Player Total: %d\n", get_hand_value(dealer_hand), get_hand_value(player_hand));

    if (is_bust(player_hand))
    {
        fprintf(out, "---Player busts!\n");
        // Update metadata counters
        metadata->total_wagered += metadata->wager;
        metadata->bankroll -= metadata->wager;
        return;
    }
    else if (is_bust(dealer_hand))
    {
        fprintf(out, "---Dealer busts!\n");
        // Update metadata counters
        metadata->total_won += metadata->wager * 2;
        metadata->total_wagered += metadata->wager;
        metadata->bankroll += metadata->wager;
        return;
    }

    if (get_hand_value(player_hand) > get_hand_value(dealer_hand)) {
        fprintf(out, "---Player wins!\n");
        // Update metadata counters
        metadata->total_won += metadata->wager * 2;
        metadata->total_wagered += metadata->wager;
        metadata->bankroll += metadata->wager;
    }
    else if (get_hand_value(player_hand) < get_hand_value(dealer_hand)) {
        fprintf(out, "---Dealer wins!\n");
        // Update metadata counters
        metadata->total_wagered += metadata->wager;
        metadata->bankroll -= metadata->wager;
    }
    else {
        fprintf(out, "---It's a push!\n");
        // Update metadata counters
        metadata->total_won += metadata->wager;
        metadata->total_wagered += metadata->wager;
    }
}

// CONTINUES TO NEXT HAND IF A NATURAL OCCURS, NEED TO FREE HANDS IF THIS HAPPENS TOO
int check_for_naturals(FILE *out, Hand *player_hand, Hand *dealer_hand, Metadata *metadata) {
    // If both players have blackjack
    if (get_hand_value(player_hand) == 21 && get_hand_value(dealer_hand) == 21) {
        fprintf(out, "---NATURAL PUSH\n");
        fprintf(out, "Dealer Hand: ");
        for (int i=0; i < dealer_hand->num_cards; i++) {
            fprintf(out, "%d ", dealer_hand->cards[i].rank);
        }
        fprintf(out, "\n");
        fprintf(out, "Player Hand: ");
        for (int i=0; i < player_hand->num_cards; i++) {
            fprintf(out, "%d", player_hand->cards[i].rank);
        }
        fprintf(out, "\n");

        // Update metadata counters 
        metadata->total_won += metadata->wager * metadata->bj_pay + metadata->wager;
        metadata->total_wagered += metadata->wager * metadata->bj_pay + metadata->wager;
        return 1;
    }
    // If player has blackjack
    else if (get_hand_value(player_hand) == 21) {
        fprintf(out, "---PLAYER BLACKJACK\n");
        fprintf(out, "Player Hand: ");
        for (int i=0; i < player_hand->num_cards; i++) {
            fprintf(out, "%d ", player_hand->cards[i].rank);
        }
        fprintf(out, "\n");

        // Update metadata counters
        metadata->total_won += metadata->wager * metadata->bj_pay + metadata->wager;
        metadata->total_wagered += metadata->wager;
        metadata->bankroll += metadata->wager * metadata->bj_pay;
        return 1;
    }
    // If dealer has blackjack
    else if (get_hand_value(dealer_hand) == 21) {
        fprintf(out, "---DEALER BLACKJACK\n");
        fprintf(out, "Dealer Hand: ");
        for (int i=0; i < dealer_hand->num_cards; i++) {
            fprintf(out, "%d", dealer_hand->cards[i].rank);
        }
        fprintf(out, "\n");

        // Update metadata counters 
        metadata->total_wagered += metadata->wager;
        metadata->bankroll -= metadata->wager;
        return 1;
    }
    // If neither player has blackjack nothing happens
    return 0;
}

void simulate(FILE *out, int num_simulations, char (*strategy)[STRAT_COLS], Metadata *metadata) {
    Hand player_hand;
    Hand dealer_hand;
    int shoe_counter = 0;
    for (int i=0; i < num_simulations; i++)
    {   
        fprintf(out, "Shoe #%d\n", shoe_counter++);
        metadata->total_shoes_played++;
        play_shoe(out, &player_hand, &dealer_hand, strategy, metadata);
    }
}