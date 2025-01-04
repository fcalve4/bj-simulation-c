#include <stdio.h>
#include "hand.h"
#include "deck.h"
#include "player.h"
#include "strategy.h"
#include "simulate.h"


void play_shoe(Player *player, Player *dealer, char **strategy, int num_decks, int penetration, int h17, int ls, int enhc)
{
    Deck deck;
    initDeck(&deck, num_decks);
    shuffleDeck(&deck);

    while (deck.top <= deck.capacity * penetration)
    {   
        // Init hands
        initHand(&player->hand);
        initHand(&dealer->hand);
        
        // Deal initial cards to hands
        addCardToHand(&player->hand, dealCard(&deck));
        addCardToHand(&player->hand, dealCard(&deck));

        addCardToHand(&dealer->hand, dealCard(&deck));
        // Grab the dealer upcard value now and store it separately (this is to check for naturals)
        int dealerUpcard = getHandValue(&dealer->hand);

        // Deal the dealer another hand
        addCardToHand(&dealer->hand, dealCard(&deck));

        // If enhc is false (usually it is), check for naturals immediately
        if (enhc == 0)
        {
            check_for_naturals(&player->hand, &dealer->hand);
        }

        // -=-=-PLAYER TURN TO ACT-=-=-
        while (1) {
            // Determine player action based on strategy sheet
            char action = determineAction(&player->hand, dealerUpcard, strategy);
            // Player hit - add 1 card and check for bust
            if (action == 'H') {
                addCardToHand(&player->hand, dealCard(&deck));
                if (isBust(&player->hand)) {
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
                initHand(&player->hand);
                addCardToHand(&player->hand, card);
                continue;
            }
            // Player Double otherwise hit
            else if (action == 'D') {
                if (canDouble(&player->hand))
                {
                    // Double the wager
                }
                addCardToHand(&player->hand, dealCard(&deck));
                if (isBust(&player->hand)) { // Is this statement necesarry? will the player ever bust when action is 'D'?
                    printf("Player busts!\n");
                    break;
                }
            }
            // Player Double otherwise stand
            else if (action == 'T') {
                if (canDouble(&player->hand))
                {
                    // Double the wager
                    addCardToHand(&player->hand, dealCard(&deck));
                }
                if (isBust(&player->hand)) {
                    printf("Player busts!\n");
                    break;
                }
                break;
            }
            // Player Surrender otherwise stand
            else if (action == 'X') {
                if (canSurrender(&player->hand) && ls == 1)
                {
                    break;
                }
                break;
            }
            // Player Surrender otherwise hit
            else if (action == 'Y') {
                if (canSurrender(&player->hand) && ls == 1)
                {
                    break;
                }
                // Else hit
                addCardToHand(&player->hand, dealCard(&deck));

                if (isBust(&player->hand)) {
                    printf("Player busts!\n");
                    break;
                }
            }
            // Player Surrender otherwise split
            else if (action == 'Z') {
                if (canSurrender(&player->hand) && ls == 1)
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
            while (getHandValue(&dealer->hand) < 17 || ((hasSoftAce(&dealer->hand) == 1) && getHandValue(&dealer->hand) == 17)) {
            addCardToHand(&dealer->hand, dealCard(&deck));
            }
        }
        // case 2: S17, dealer stands on all 17s
        else
        {
            while (getHandValue(&dealer->hand) < 17) {
            addCardToHand(&dealer->hand, dealCard(&deck));
            }
        }

        // Determine winner, Player wins if dealer busts or has a higher hand
        if (isBust(&dealer->hand) || getHandValue(&player->hand) > getHandValue(&dealer->hand)) {
            printf("Player wins!\n");
        }
        else if (getHandValue(&player->hand) < getHandValue(&dealer->hand)) {
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
    if (getHandValue(playerhand) == 21 && getHandValue(dealerhand) == 21) {
        return 0;
    }
    // If player has blackjack
    else if (getHandValue(playerhand) == 21) {
        return 0;
    }
    // If dealer has blackjack
    else if (getHandValue(dealerhand) == 21) {
        return 0;
    }
    // If neither player has blackjack
    else {
        return 0;
    }
}

void simulate(int num_simulations, char* strategy)
{
    Player player;
    Player dealer;
    for (int i=0; i < num_simulations; i++)
    {   
        play_shoe(&player, &dealer, strategy, 6, 0.75, 1, 1, 0);
    }
}