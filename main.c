#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "deck.h"
#include "player.h"
#include "strategy.h"

// DECLARE CONSTANTS (WILL BE USER INPUT FROM THE UI [IMPLEMENTED LATER])
#define NUM_DECKS 6
#define H17 1 // 1 for true - TOGGLE FUNCTIONALITY NOT ADDED YET
#define DAS 1 // 1 for true - TOGGLE FUNCTIONALITY NOT ADDED YET
#define LS 1 // 1 for true - TOGGLE FUNCTIONALITY NOT ADDED YET
#define RSA 0 // 0 for false - TOGGLE FUNCTIONALITY NOT ADDED YET
#define ENHC 0 // 0 for false - TOGGLE FUNCTIONALITY NOT ADDED YET
#define MAX_SPLITS 4
#define BJ_PAY 1.5
#define PEN 0.8

#define NUM_SIMULATIONS 1000 // Number of shoes to play
#define BANKROLL 1000
#define WAGER 1

// Number of rows & cols for strategy sheet array creation. Not really sure what to do with these.
// They are constants but might change depending on how the strategy sheet is configured
#define STRAT_ROWS 37 // Player hand possibilities
#define STRAT_COLS 10 // Dealer upcards


int main() {
    // Initialize variables & start clock
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    // Initialize output file
    FILE *out = fopen("output.txt", "w");

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
    int total_num_hands = 0;
    int num_credits_won = 0;
    int num_credits_wagered = 1;
    float rtp_percent = 0.0; // RTP Formula = NUM WON / NUM WAGERED * 100

    // Initialize players
    initPlayer(&player, 100); // Start with $100
    initPlayer(&dealer, 0);   // Dealer has no bankroll

    for (int i=0; i < NUM_SIMULATIONS; i++) { // i = number of shoes  
        // Initialize deck and shuffle
        initDeck(&deck, NUM_DECKS);
        shuffleDeck(&deck);

        // MAIN HAND LOOP
            // While loop goes until the 'cut card' is reached
        while (deck.top <= deck.capacity * PEN) { 
            total_num_hands++; // Increment counter (temporary)
            // Clear previous hand data & reinit
            initHand(&player.hand);
            initHand(&dealer.hand);

            // Deal initial hands
            addCardToHand(&player.hand, dealCard(&deck));
            addCardToHand(&player.hand, dealCard(&deck));
            addCardToHand(&dealer.hand, dealCard(&deck));
            
            // Grab the dealer upcard value and store it separately
            int dealerUpcard = getHandValue(&dealer.hand);

            // Deal the dealer another hand (this is to check for naturals)
            addCardToHand(&dealer.hand, dealCard(&deck));

            // --CHEK FOR NATURALS
            // BOTH PLAYERS HAVE NATURALS. PUSH
            if (getHandValue(&dealer.hand) == 21 && getHandValue(&player.hand) == 21) {
                fprintf(out, "push blackjacks!\n)");
                num_credits_won += WAGER;
                num_credits_wagered += WAGER;
                freeHand(&dealer.hand);
                freeHand(&player.hand);
                continue;
            }
            // Dealer natural
            if (getHandValue(&dealer.hand) == 21) {
                fprintf(out, "blackjack for dealer\n)");
                num_credits_wagered += WAGER;
                net_credits -= WAGER;
                freeHand(&dealer.hand);
                freeHand(&player.hand);
                continue;
            }
            // Player natural - PLAYER WINS
            if (getHandValue(&player.hand) == 21) {
                fprintf(out, "BLACKJACK FOR PLAYER!\n");
                num_credits_wagered += WAGER;
                num_credits_won += (WAGER * BJ_PAY) + WAGER;
                
                num_hands_won += 1;
                net_credits += WAGER * BJ_PAY;
                freeHand(&dealer.hand);
                freeHand(&player.hand);
                continue;
            }

            fprintf(out, "Player's hand value: %d\n", getHandValue(&player.hand));
            fprintf(out, "Dealer's hand value: %d\n", getHandValue(&dealer.hand));

            // While loop to allow the player to act multiple times
            while (1) {
                // THIS IS WHERE THE STRATEGY IMPLEMENTATION GOES
                    // Function determinAction() comes from  strategy.c 
                char action = determineAction(&player.hand, dealerUpcard, strategy);

                // Player hits - add 1 card and check for bust
                if (action == 'H')
                {
                    fprintf(out, "hit\n");
                    addCardToHand(&player.hand, dealCard(&deck));
                    if (isBust(&player.hand)) {
                        net_credits -= WAGER;
                        num_credits_wagered += WAGER;
                        fprintf(out, "Player busts with value: %d\n", getHandValue(&player.hand));
                        break;
                    }
                }
                // Player stands - No action needed
                else if (action == 'S') {
                    fprintf(out, "stand\n");
                    num_credits_wagered += WAGER;
                    break;
                }
                else if (action == 'P') {
                    fprintf(out, "split\n");
                    num_credits_wagered += WAGER;
                    break;
                }
                else if (action == 'D') {
                    fprintf(out, "double/hit\n");
                    addCardToHand(&player.hand, dealCard(&deck));
                    if (isBust(&player.hand)) {
                        fprintf(out, "Player busts with value: %d\n", getHandValue(&player.hand));
                        num_credits_wagered += 2 * WAGER;
                        break;
                    }
                }
                else if (action == 'T') {
                    fprintf(out, "double/stand\n");
                    addCardToHand(&player.hand, dealCard(&deck));
                    if (isBust(&player.hand)) {
                        fprintf(out, "Player busts with value: %d\n", getHandValue(&player.hand));
                        num_credits_wagered += WAGER;
                        break;
                    }
                }
                else if (action == 'X') {
                    fprintf(out, "surredner/stand\n");
                    num_credits_wagered += WAGER;
                    break;
                }
                else if (action == 'Y') {
                    fprintf(out, "surrender/hit\n");
                    addCardToHand(&player.hand, dealCard(&deck));
                    if (isBust(&player.hand)) {
                        fprintf(out, "Player busts with value: %d\n", getHandValue(&player.hand));
                        num_credits_wagered += WAGER;
                        break;
                    }
                }
                else if (action == 'Z') {
                    fprintf(out, "Surrender/split\n");
                    num_credits_wagered += WAGER;
                    break;
                }
                else {
                    fprintf(out, "ERROR: INVALID DECISION SELECTED");
                    break;
                }
            }
            // Check for bust again after player's turn, return to start of the loop if bust
            if (isBust(&player.hand)) {
                freeHand(&dealer.hand);
                freeHand(&player.hand);
                continue;
            }
            // Dealer's turn (simple rule: hit until 17+)
            while (getHandValue(&dealer.hand) < 17) {
                addCardToHand(&dealer.hand, dealCard(&deck));
                fprintf(out, "DEALER HITTING - Dealer's NEW hand value: %d\n", getHandValue(&dealer.hand));
            }
            // Determine winner, Player wins if dealer busts or has a higher hand
            if (isBust(&dealer.hand) || getHandValue(&player.hand) > getHandValue(&dealer.hand)) {
                fprintf(out, "Player wins!\n");
                net_credits += WAGER;
                num_credits_won += 2 * WAGER;
                num_hands_won += 1;
            }
            else if (getHandValue(&player.hand) < getHandValue(&dealer.hand)) {
                fprintf(out, "Dealer wins!\n");
                net_credits -= WAGER;
            }
            else {
                fprintf(out, "It's a tie!\n");
            }
            freeHand(&dealer.hand);
            freeHand(&player.hand);
            
            rtp_percent = ((float) num_credits_won / num_credits_wagered) * 100;
            fprintf(out, "\tShoe #: %d |Hand #: %d | Net Credits: %d | # Hands Won: %d\n", i + 1, total_num_hands, net_credits, num_hands_won);
            fprintf(out, "\tRTP: %.10f | Num Credits Won: %d | Num Creds Wagered: %d\n", rtp_percent, num_credits_won, num_credits_wagered);
        }
        freeDeck(&deck);
    }

    // Stop the clock, calculate the elapsed time, print the result
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    fprintf(out, "Execution Time: %f seconds\n", cpu_time_used);

    return 0;

}

