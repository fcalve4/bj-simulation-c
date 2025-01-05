#include <stdio.h>
#include "hand.h"
#include "deck.h"
#include "player.h"
#include "strategy.h"
#include "simulate.h"
#include "metadata.h"

#define STRAT_COLS 10


void play_shoe(Player *player, Player *dealer, char (*strategy)[STRAT_COLS], Metadata *metadata) {
    Deck deck;
    init_deck(&deck, metadata->num_decks);
    shuffle_deck(&deck);

    // Play the shoe
    while (deck.top <= deck.capacity * metadata->pen) {   
        // Init hands
        init_hand(&player->hand);
        init_hand(&dealer->hand);
        
        // Deal initial cards to player
        add_card_to_hand(&player->hand, deal_card(&deck));
        add_card_to_hand(&player->hand, deal_card(&deck));

        // Deal initial cards to dealer
        add_card_to_hand(&dealer->hand, deal_card(&deck));
        // Grab the dealer upcard value now and store it separately (this is to check for naturals)
        int dealer_upcard = get_hand_value(&dealer->hand);

        // Deal the dealer another hand
        add_card_to_hand(&dealer->hand, deal_card(&deck));

        // If enhc is false (usually it is), check for naturals immediately
        if (metadata->enhc == 0) {
            check_for_naturals(&player->hand, &dealer->hand);
        }

        // -=-=-PLAYER TURN TO ACT-=-=-
        int player_turn_loop_bool = 1; // LOOP BOOLEAN

        // While the player is still acting, call the player turn function
        while (player_turn_loop_bool) {
            player_turn_loop_bool = play_player_turn(player, dealer, &deck, strategy, dealer_upcard, metadata);
        }

        // -=-=-DEALER TURN TO ACT-=-=-
        play_dealer_turn(&dealer->hand, &deck, metadata->h17);

        // Determine the winner
        determine_winner(&player->hand, &dealer->hand);
        
        
        free_hand(&player->hand);
        free_hand(&dealer->hand);
    }
    free_deck(&deck);

}





// Player "make decision" function
// This function will determine the player's action based on the strategy sheet
    // and returns 0 or 1 depending on if loop continues or breaks.
int play_player_turn(Player *player, Player *dealer, Deck *deck, char (*strategy)[STRAT_COLS], int dealer_upcard, Metadata *metadata) {
    // Determine player action based on strategy sheet
    char action = determine_action(&player->hand, dealer_upcard, strategy);
    // Player hit - add 1 card and check for bust
    if (action == 'H') {
        add_card_to_hand(&player->hand, deal_card(deck));
        if (is_bust(&player->hand)) {
            printf("Player busts!\n");
            return 0;
        }
    }
    // Player stand - no action needed
    else if (action == 'S') {
        return 0;
    }


    // Player split
    else if (action == 'P') {
        printf("-------PLAYER SPLITTING!\n");
        // Grab the split card
        Card split_card = player->hand.cards[0];

        // Create a new loop for the split off hand.
        int split_loop_bool = 1;
        // Reinitialize the player's hand and add the split card
        init_hand(&player->hand);
        add_card_to_hand(&player->hand, split_card);
        while (split_loop_bool) {
            // Call the player turn function for the split off hand and play normally
            // This should just play another hand and then once this loop breaks, the next hand will play via the main loop
            split_loop_bool = play_player_turn(player, dealer, deck, strategy, dealer_upcard, metadata);
        }
        // Play dealer turn
        play_dealer_turn(&player->hand, deck, metadata->h17);
        // Determine winner of the hand
        determine_winner(&player->hand, &dealer->hand);

        // Reinitialize the player's hand and add the split card
        init_hand(&player->hand);
        add_card_to_hand(&player->hand, split_card);
        // Return to start of loop to continue playing the hand
    }


    // Player Double otherwise hit
    else if (action == 'D') {
        if (can_double(&player->hand))
        {
            // Double the wager
        }
        add_card_to_hand(&player->hand, deal_card(deck));
        if (is_bust(&player->hand)) { // Is this statement necesarry? will the player ever bust when action is 'D'?
            printf("Player busts!\n");
            return 0;
        }
    }
    // Player Double otherwise stand
    else if (action == 'T') {
        if (can_double(&player->hand))
        {
            // Double the wager
            add_card_to_hand(&player->hand, deal_card(deck));
        }
        if (is_bust(&player->hand)) {
            printf("Player busts!\n");
            return 0;
        }
        return 0;
    }
    // Player Surrender otherwise stand
    else if (action == 'X') {
        if (can_surrender(&player->hand) && metadata->ls == 1)
        {
            return 0;
        }
        return 0;
    }
    // Player Surrender otherwise hit
    else if (action == 'Y') {
        if (can_surrender(&player->hand) && metadata->ls == 1) {
            return 0;
        }
        // Else hit
        add_card_to_hand(&player->hand, deal_card(deck));
        if (is_bust(&player->hand)) {
            printf("Player busts!\n");
            return 0;
        }
    }
    // Player Surrender otherwise split
    else if (action == 'Z') {
        if (can_surrender(&player->hand) && metadata->ls == 1) {
            return 0;
        }
        // else split
        return 0;
    }
    else {
        fprintf(stderr, "ERROR: INVALID DECISION SELECTED\n");
        return 0;
    }
    return 1; // Default is to hit
}

void play_dealer_turn(Hand *dealer_hand, Deck *deck, int h17)
{
    // -=-=-DEALER TURN TO ACT-=-=-
    // case 1: H17, dealer hits on soft 17s
    if (h17 == 1) {
        while (get_hand_value(dealer_hand) < 17 || ((has_soft_ace(dealer_hand) == 1) && get_hand_value(dealer_hand) == 17)) {
        add_card_to_hand(dealer_hand, deal_card(deck));
        }
    }
    // case 2: S17, dealer stands on all 17s
    else {
        while (get_hand_value(dealer_hand) < 17) {
        add_card_to_hand(dealer_hand, deal_card(deck));
        }
    }
}

void determine_winner(Hand *player_hand, Hand *dealer_hand) {
    // Determine winner, Player wins if dealer busts or has a higher hand
    if (is_bust(player_hand) || get_hand_value(player_hand) > get_hand_value(dealer_hand)) {
        printf("Player wins!\n");
    }
    else if (get_hand_value(player_hand) < get_hand_value(dealer_hand)) {
        printf("Dealer wins!\n");
    }
    else {
        printf("It's a push!\n");
    }
}


int check_for_naturals(Hand *playerhand, Hand *dealerhand) {
    // If both players have blackjack
    if (get_hand_value(playerhand) == 21 && get_hand_value(dealerhand) == 21) {
        return 0;
    }
    // If player has blackjack
    else if (get_hand_value(playerhand) == 21) {
        return 0;
    }
    // If dealer has blackjack
    else if (get_hand_value(dealerhand) == 21) {
        return 0;
    }
    // If neither player has blackjack
    return 0;
}

void simulate(int num_simulations, char (*strategy)[STRAT_COLS], Metadata *metadata) {
    Player player;
    Player dealer;
    for (int i=0; i < num_simulations; i++)
    {   
        play_shoe(&player, &dealer, strategy, metadata);
    }
}