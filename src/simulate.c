#include <stdio.h>
#include "hand.h"
#include "deck.h"
#include "player.h"
#include "strategy.h"
#include "simulate.h"

#define STRAT_COLS 10


void play_shoe(Player *player, Player *dealer, char (*strategy)[STRAT_COLS], int num_decks, int pen, int h17, int ls, int enhc)
{
    Deck deck;
    init_deck(&deck, num_decks);
    shuffle_deck(&deck);

    while (deck.top <= deck.capacity * pen)
    {   
        // Init hands
        init_hand(&player->hand);
        init_hand(&dealer->hand);
        
        // Deal initial cards to hands
        add_card_to_hand(&player->hand, deal_card(&deck));
        add_card_to_hand(&player->hand, deal_card(&deck));

        add_card_to_hand(&dealer->hand, deal_card(&deck));
        // Grab the dealer upcard value now and store it separately (this is to check for naturals)
        int dealer_upcard = get_hand_value(&dealer->hand);

        // Deal the dealer another hand
        add_card_to_hand(&dealer->hand, deal_card(&deck));

        // If enhc is false (usually it is), check for naturals immediately
        if (enhc == 0)
        {
            check_for_naturals(&player->hand, &dealer->hand);
        }

        // -=-=-PLAYER TURN TO ACT-=-=-
        while (1) {
            // Determine player action based on strategy sheet
            char action = determine_action(&player->hand, dealer_upcard, strategy);
            // Player hit - add 1 card and check for bust
            if (action == 'H') {
                add_card_to_hand(&player->hand, deal_card(&deck));
                if (is_bust(&player->hand)) {
                    printf("Player busts!\n");
                    break;
                }
            }
            // Player stand - no action needed
            else if (action == 'S') {
                break;
            }
            // Player split
            // Fake split functionality for now:
            // Reinits the hand and plays it with 1 card from the initial deal
            else if (action == 'P') {
                Card card = player->hand.cards[0];
                init_hand(&player->hand);
                add_card_to_hand(&player->hand, card);
                continue;
            }
            // Player Double otherwise hit
            else if (action == 'D') {
                if (can_double(&player->hand))
                {
                    // Double the wager
                }
                add_card_to_hand(&player->hand, deal_card(&deck));
                if (is_bust(&player->hand)) { // Is this statement necesarry? will the player ever bust when action is 'D'?
                    printf("Player busts!\n");
                    break;
                }
            }
            // Player Double otherwise stand
            else if (action == 'T') {
                if (can_double(&player->hand))
                {
                    // Double the wager
                    add_card_to_hand(&player->hand, deal_card(&deck));
                }
                if (is_bust(&player->hand)) {
                    printf("Player busts!\n");
                    break;
                }
                break;
            }
            // Player Surrender otherwise stand
            else if (action == 'X') {
                if (can_surrender(&player->hand) && ls == 1)
                {
                    break;
                }
                break;
            }
            // Player Surrender otherwise hit
            else if (action == 'Y') {
                if (can_surrender(&player->hand) && ls == 1)
                {
                    break;
                }
                // Else hit
                add_card_to_hand(&player->hand, deal_card(&deck));

                if (is_bust(&player->hand)) {
                    printf("Player busts!\n");
                    break;
                }
            }
            // Player Surrender otherwise split
            else if (action == 'Z') {
                if (can_surrender(&player->hand) && ls == 1)
                {
                    break;
                }
                // else split
                break;
            }
            else {
                fprintf(stderr, "ERROR: INVALID DECISION SELECTED\n");
                break;
            }
        }

        // -=-=-DEALER TURN TO ACT-=-=-
        // case 1: H17, dealer hits on soft 17s
        if (h17 == 1)
        {
            while (get_hand_value(&dealer->hand) < 17 || ((hasSoftAce(&dealer->hand) == 1) && get_hand_value(&dealer->hand) == 17)) {
            add_card_to_hand(&dealer->hand, deal_card(&deck));
            }
        }
        // case 2: S17, dealer stands on all 17s
        else
        {
            while (get_hand_value(&dealer->hand) < 17) {
            add_card_to_hand(&dealer->hand, deal_card(&deck));
            }
        }

        // Determine winner, Player wins if dealer busts or has a higher hand
        if (is_bust(&dealer->hand) || get_hand_value(&player->hand) > get_hand_value(&dealer->hand)) {
            printf("Player wins!\n");
        }
        else if (get_hand_value(&player->hand) < get_hand_value(&dealer->hand)) {
            printf("Dealer wins!\n");
        }
        else {
            printf("It's a push!\n");
        }

        freeHand(&player->hand);
        freeHand(&dealer->hand);
    }
    freeDeck(&deck);

}

int check_for_naturals(Hand *playerhand, Hand *dealerhand)
{
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
    else {
        return 0;
    }
}

void simulate(int num_simulations, char (*strategy)[STRAT_COLS], int num_decks, int pen, int h17, int ls, int enhc)
{
    Player player;
    Player dealer;
    for (int i=0; i < num_simulations; i++)
    {   
        play_shoe(&player, &dealer, strategy, num_decks, pen, h17, ls, enhc);
    }
}