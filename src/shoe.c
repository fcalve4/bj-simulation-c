#include "shoe.h"

#include <stdio.h>
#include <stdlib.h>

// "Deck" & "Shoe" are interchangeable in this program
void init_shoe(Shoe* shoe, int num_decks) {
    // Dynamically allocate memory for shoe
    shoe->cards = (int*)malloc(52 * num_decks * sizeof(int));
    shoe->num_cards = shoe->capacity;
    shoe->capacity = 52 * num_decks;
    shoe->top = 0; // Current index of the shoe

    if (shoe->cards == NULL) {
        fprintf(stderr, "Error: Memory allocation for shoe failed.\n");
        exit(1);
    }
    // Populate the shoe with cards
    int index = 0;
    // For every deck in the shoe
    for (int deck = 0; deck < num_decks; deck++) {
        // For every type of card in a deck 
        for (int rank = 1; rank <= 13; rank++) {
            // For every suit for every card in a deck
            for (int count = 0; count < 4; count++) {
                // Add the card to the shoe including 4 tens
                if (rank <= 10) {
                    shoe->cards[index] = rank;
                } else {
                    shoe->cards[index] = 10;
                }
                index++;
            }
        }
    }
}

void free_shoe(Shoe* shoe) {
    free(shoe->cards);   // Free the dynamically allocated memory
    shoe->cards = NULL;  // Avoid dangling pointer
    shoe->num_cards = 0;
    shoe->capacity = 0;
    shoe->top = 0;  // Reset the top index after freeing memory
}

void shuffle_shoe(Shoe* shoe) {
    // Fischer-Yates shuffle algorithmn to shuffle the order of cards
    for (int i = shoe->capacity - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = shoe->cards[i];
        shoe->cards[i] = shoe->cards[j];
        shoe->cards[j] = temp;
    }
    shoe->top = 0;  // Reset the top index after shuffle
}

int deal_card(Shoe* shoe) {
    if (shoe->cards == NULL || shoe->top >= shoe->capacity) {
        fprintf(stderr, "Error: No more cards in the shoe.\n");
        exit(1); // Exit the function if there are no more cards in the shoe
    }
    return shoe->cards[shoe->top++];
}
