#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "deck.h"
#include "player.h"

int main() {
    // Seed random numbers
    srand(time(NULL));

    Deck deck;
    Deck *pDeck = &deck;

    Player player;


    pDeck->capacity = 52;
    pDeck->numCards = pDeck->capacity;
    pDeck->cards = (Card *)malloc(pDeck->capacity * sizeof(Card));
    pDeck->top = 0;
    

    pDeck->cards[0].rank = 5; // First card
    pDeck->cards[1].rank = 5; // Second card
    pDeck->cards[2].rank = 1; // Third card

    pDeck->cards[3].rank = 10; // Fourth card

    initPlayer(&player, 1000);
   

    // Clear previous hand data
    initHand(&player.hand);

    // Deal initial hands
    addCardToHand(&player.hand, dealCard(&deck));
    addCardToHand(&player.hand, dealCard(&deck));

    // Print initial hand value
    printf("Initial Hand Value: %d\n", getHandValue(&player.hand));
    // check booleans
    printf("Is hand doubleable? %d\n", canDouble(&player.hand));
    printf("Is hand splittable? %d\n", canSplit(&player.hand));
    printf("Is hand surrenderable? %d\n", canSurrender(&player.hand));
    printf("Is hand soft? %d\n", hasSoftAce(&player.hand));

    // Deal new card
    addCardToHand(&player.hand, dealCard(&deck));
    
    // Print new hand value
    printf("New Hand Value: %d\n", getHandValue(&player.hand));
    // Check booleans
    printf("Is hand doubleable? %d\n", canDouble(&player.hand));
    printf("Is hand splittable? %d\n", canSplit(&player.hand));
    printf("Is hand surrenderable? %d\n", canSurrender(&player.hand));
    printf("Is hand soft? %d\n", hasSoftAce(&player.hand));

    return 0;
}