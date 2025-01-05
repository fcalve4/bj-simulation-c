#include <stdio.h>
#include "hand.h"
#include "deck.h"
#include "player.h"
#include "strategy.h"
#include "simulate.h"
#include "metadata.h"

#define STRAT_COLS 10


void play_shoe(FILE *out, Player *player, Player *dealer, char (*strategy)[STRAT_COLS], Metadata *metadata) {
    // Init and shuffle the deck
    Deck deck;
    init_deck(&deck, metadata->num_decks);
    shuffle_deck(&deck);

    // Play the shoe one hand at a time
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
        fprintf(out, "--Dealer upcard: %d\n", dealer_upcard);

        // Deal the dealer another hand
        add_card_to_hand(&dealer->hand, deal_card(&deck));

        // If enhc is false (usually it is), check for naturals immediately
        if (metadata->enhc == 0) {
            check_for_naturals(out, &player->hand, &dealer->hand);
        }

        // -=-=-PLAYER TURN TO ACT-=-=-
        int player_turn_loop_bool = 1; // LOOP BOOLEAN

        // While the player is still acting, call the player turn function
        while (player_turn_loop_bool) {
            fprintf(out, "--Player hand total: %d\n", get_hand_value(&player->hand));
            player_turn_loop_bool = play_player_turn(out, player, dealer, &deck, strategy, dealer_upcard, metadata);
        }

        // If enhc is true (rarely is), check for naturals after player acts
        if (metadata->enhc == 1) {
            check_for_naturals(out, &player->hand, &dealer->hand);
        }

        // -=-=-DEALER TURN TO ACT-=-=-
        play_dealer_turn(out, &dealer->hand, &deck, metadata->h17);

        // Determine the winner
        determine_winner(out, &player->hand, &dealer->hand);

        free_hand(&player->hand);
        free_hand(&dealer->hand);
    }
    free_deck(&deck);

}





// Player "make decision" function
// This function will determine the player's action based on the strategy sheet
    // and returns 0 or 1 depending on if loop continues or breaks.
int play_player_turn(FILE *out, Player *player, Player *dealer, Deck *deck, char (*strategy)[STRAT_COLS], int dealer_upcard, Metadata *metadata) {
    // Determine player action based on strategy sheet
    char action = determine_action(&player->hand, dealer_upcard, strategy);
    fprintf(out, "PLAYER ACTION: %c\n", action);
    // Player hit - add 1 card and check for bust
    if (action == 'H') {
        add_card_to_hand(&player->hand, deal_card(deck));
        if (is_bust(&player->hand)) {
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
        Card split_card = player->hand.cards[0];
        fprintf(out, "-------PLAYER SPLITTING!------[%d]----\n", split_card.rank);
        
        // Create a new loop for the split off hand.
        int split_loop_bool = 1;
        // Reinitialize the player's hand and add the split card + additional card
        init_hand(&player->hand);
        add_card_to_hand(&player->hand, split_card);
        add_card_to_hand(&player->hand, deal_card(deck));
        fprintf(out, "-Beginning first split off hand logic\n");
        while (split_loop_bool) {
            // Call the player turn function for the split off hand and play normally
            // This should just play another hand and then once this loop breaks, the next hand will play via the main loop
            fprintf(out, "--Player hand total: %d\n", get_hand_value(&player->hand));
            split_loop_bool = play_player_turn(out, player, dealer, deck, strategy, dealer_upcard, metadata);
        }
        fprintf(out, "-Finished first split off hand logic\n");
        // Play dealer turn
        fprintf(out, "-Beginning dealer logic for 1st hand\n");
        play_dealer_turn(out, &player->hand, deck, metadata->h17);
        fprintf(out, "-Finished dealer logic for 1st hand\n");
        // Determine winner of the hand
        fprintf(out, "determining winner of first hand\n");
        determine_winner(out, &player->hand, &dealer->hand);
        
        
        // Reinitialize the player's hand and add the split card
        init_hand(&player->hand);
        add_card_to_hand(&player->hand, split_card);
        add_card_to_hand(&player->hand, deal_card(deck));
        // Return to start of loop to continue playing the hand
        fprintf(out, "reiniting hands and starting second hand\n");
    }


    // Player Double otherwise hit
    else if (action == 'D') {
        if (can_double(&player->hand))
        {
            // Double the wager
        }
        add_card_to_hand(&player->hand, deal_card(deck));
        if (is_bust(&player->hand)) { // Is this statement necesarry? will the player ever bust when action is 'D'?
            fprintf(out, "Player busts!\n");
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
            fprintf(out, "Player busts!\n");
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
            fprintf(out, "Player busts!\n");
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

void determine_winner(FILE *out, Hand *player_hand, Hand *dealer_hand) {
    // Determine winner, Player wins if dealer busts or has a higher hand
    if (is_bust(player_hand) || get_hand_value(player_hand) > get_hand_value(dealer_hand)) {
        fprintf(out, "---Player wins!\n");
    }
    else if (get_hand_value(player_hand) < get_hand_value(dealer_hand)) {
        fprintf(out, "---Dealer wins!\n");
    }
    else {
        fprintf(out, "---It's a push!\n");
    }
}


void check_for_naturals(FILE *out, Hand *playerhand, Hand *dealerhand) {
    // If both players have blackjack
    if (get_hand_value(playerhand) == 21 && get_hand_value(dealerhand) == 21) {
        fprintf(out, "---NATURAL PUSH\n");
        return;
    }
    // If player has blackjack
    else if (get_hand_value(playerhand) == 21) {
        fprintf(out, "---PLAYER BLACKJACK\n");
        return;
    }
    // If dealer has blackjack
    else if (get_hand_value(dealerhand) == 21) {
        fprintf(out, "---DEALER BLACKJACK\n");
        return;
    }
    // If neither player has blackjack
    return;
}

void simulate(FILE *out, int num_simulations, char (*strategy)[STRAT_COLS], Metadata *metadata) {
    Player player;
    Player dealer;
    for (int i=0; i < num_simulations; i++)
    {   
        play_shoe(out, &player, &dealer, strategy, metadata);
    }
}