#include <stdio.h>
#include <stdlib.h>

#include "deck.h"

// "Deck" & "Shoe" are interchangeable in this program
void init_deck(Deck *deck, int num_decks) {
    deck->cards = (Card *)malloc(52 * num_decks * sizeof(Card));
    deck->num_cards = deck->capacity;
    deck->capacity = 52 * num_decks;
    deck->top = 0;
    
    if (deck->cards == NULL) {
        fprintf(stderr, "Error: Memory allocation for deck failed.\n");
        exit(1);
    }
    // ********************************SHOE VS DECK????
    // Populate the deck with cards
    int index = 0;
    for (int d = 0; d < num_decks; d++) {
        for (int rank = 1; rank <= 13; rank++) {
            for (int count = 0; count < 4; count++) {
                // adjust to only add tens to the deck and not 11,12,13
                if (rank <= 10) {
                    deck->cards[index].rank = rank;
                } else {
                    deck->cards[index].rank = 10;
                }
                index++;
            }
        }
    }
}

void free_deck(Deck *deck) {
    free(deck->cards); // Free the dynamically allocated memory
    deck->cards = NULL; // Avoid dangling pointer
    deck->num_cards = 0;
    deck->capacity = 0;
    deck->top = 0; // Reset the top index after freeing memory
}

void shuffle_deck(Deck *deck) {
    for (int i = deck->capacity - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = temp;
    }
    deck->top = 0; // Reset the top index after shuffle
}

Card deal_card(Deck *deck) {
    if (deck->cards == NULL || deck->top >= deck->capacity) {
        fprintf(stderr, "Error: No more cards in the deck.\n");
        exit(1);
    }
    deck->top++;
    Card card = deck->cards[deck->top];
    return card;
}
