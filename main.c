#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "deck.h"
#include "player.h"
#include "strategy.h"

// DECLARE CONSTANTS (WILL BE USER INPUT FROM THE UI [IMPLEMENTED LATER])
#define NUM_DECKS 6
#define H17 True
#define DAS True
#define LS True
#define RSA False
#define MAX_SPLITS 4
#define BJ_PAY 1.5
#define PEN 0.8

#define NUM_SIMULATIONS 50000
#define BANKROLL 1000
#define WAGER 1

#define STRAT_ROWS 40 // Player hand possibilities
#define STRAT_COLS 10 // Dealer upcards


int main() {
    // Seed random numbers
    srand(time(NULL));

    // Allocate memory for the strategy sheet 
    // 40 rows (soft, hard, splits, surrenders AND 10 columns [2-A]
    char strategy[STRAT_ROWS][STRAT_COLS];

    readStrategySheet("strategy.csv", strategy);

    

    // Initialize some variables
    Deck deck;
    Player player, dealer;

    int net_credits = 0;
    int num_hands_won = 0;

    // Initialize players
    initPlayer(&player, 100); // Start with $100
    initPlayer(&dealer, 0);   // Dealer has no bankroll

    for (int i=0; i < NUM_SIMULATIONS; i++)
    { 
        // Initialize deck and shuffle
        initDeck(&deck, NUM_DECKS);
        shuffleDeck(&deck);

        // Clear previous hand data
        initHand(&player.hand);
        initHand(&dealer.hand);

        // Deal initial hands
        addCardToHand(&player.hand, dealCard(&deck));
        addCardToHand(&player.hand, dealCard(&deck));
        addCardToHand(&dealer.hand, dealCard(&deck));

        // --CHEK FOR NATURALS
        // Player natural - PLAYER WINS
        if (getHandValue(&player.hand) == 21)
        {
            num_hands_won += 1;
            net_credits += WAGER * BJ_PAY;
            freeHand(&dealer.hand);
            freeHand(&player.hand);
            freeDeck(&deck);
            continue;
        }
        
        while (1) { // Player's turn to act
            printf("Player's hand value: %d\n", getHandValue(&player.hand));
            printf("Dealer's hand value: %d\n", getHandValue(&dealer.hand));
            
            // THIS IS WHERE THE STRATEGY IMPLEMENTATION GOES
                // Function determinAction() comes from  strategy.c 
            int choice = determineAction(&player.hand, &dealer.hand, strategy);

            if (choice == 1) {
                addCardToHand(&player.hand, dealCard(&deck));
                if (isBust(&player.hand)) {
                    printf("Player busts with value: %d\n", getHandValue(&player.hand));
                    break;
                }
            } else {
                break;
            }
        }

        // Dealer's turn (simple rule: hit until 17+)
        while (getHandValue(&dealer.hand) < 17) {
            addCardToHand(&dealer.hand, dealCard(&deck));
            printf("DEALER HITTING - Dealer's NEW hand value: %d\n", getHandValue(&dealer.hand));
        }

        // Determine winner
        if (isBust(&dealer.hand) || getHandValue(&player.hand) > getHandValue(&dealer.hand)) {
            printf("Player wins!\n");
            net_credits += WAGER;
            num_hands_won += 1;
        }
        else if (getHandValue(&player.hand) < getHandValue(&dealer.hand)) {
            printf("Dealer wins!\n");
            net_credits -= WAGER;
        }
        else {
            printf("It's a tie!\n");
        }
        freeHand(&dealer.hand);
        freeHand(&player.hand);
        freeDeck(&deck);
        printf("Iteration: %d | Net Credits: %d | Num Hands Won: %d\n", i + 1, net_credits, num_hands_won);
    }

    
    return 0;
}
