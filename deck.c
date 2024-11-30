#include "deck.h"
#include <stdio.h>
#include <stdlib.h>

// "Deck" & "Shoe" are interchangeable in this program
void initDeck(Deck *deck, int numDecks) {
    deck->capacity = 52 * numDecks;
    deck->numCards = deck->capacity;
    deck->cards = (Card *)malloc(deck->capacity * sizeof(Card));
    deck->top = 0;
    
    if (deck->cards == NULL) {
        fprintf(stderr, "Error: Memory allocation for deck failed.\n");
        exit(1);
    }
    // ********************************SHOE VS DECK????
    // Populate the deck with cards
    int index = 0;
    for (int d = 0; d < numDecks; d++) {
        for (int rank = 1; rank <= 13; rank++) {
            for (int count = 0; count < 4; count++) {
                deck->cards[index].rank = rank;
                index++;
            }
        }
    }
}

void freeDeck(Deck *deck) {
    free(deck->cards); // Free the dynamically allocated memory
    deck->cards = NULL; // Avoid dangling pointer
    deck->numCards = 0;
    deck->capacity = 0;
    deck->top = 0; // Reset the top index after freeing memory
}

void shuffleDeck(Deck *deck) {
    for (int i = deck->capacity - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = temp;
    }
    deck->top = 0; // Reset the top index after shuffle
}

Card dealCard(Deck *deck) {
    if (deck->cards == NULL || deck->top >= deck->capacity) {
        fprintf(stderr, "Error: No more cards in the deck.\n");
        exit(1);
    }
    printf("Dealing card #%d\n", deck->top);
    return deck->cards[deck->top++];
}
