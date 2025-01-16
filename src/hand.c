#include "hand.h"

#include <stdio.h>
#include <stdlib.h>

#include "metadata.h"
#include "shoe.h"

void init_hand(Hand* hand) {
    hand->cards = (int*)malloc(5 * sizeof(int));  // Initial size is 5 for what should it be?
    if (hand->cards == NULL) {
        fprintf(stderr, "Error: Memory allocation for hand failed.\n");
        exit(1);
    }
    hand->num_cards = 0;
    hand->capacity = 5;  // Initial capacity
}

int has_soft_ace(Hand* hand) {
    int value = 0;
    int aceCount = 0;
    for (int i = 0; i < hand->num_cards; i++) {
        if (hand->cards[i] == 1) {
            aceCount++;
            value += 11;
        } else {
            value += hand->cards[i];
        }
    }
    return aceCount > 0 && value <= 21;
}

int can_double(Hand* hand) { return hand->num_cards == 2; }

int can_split(Hand* hand, Metadata* metadata) {
    if (metadata->num_times_split >= metadata->max_splits) {
        return 0;
    }

    return hand->num_cards == 2 && hand->cards[0] == hand->cards[1];
}
int can_surrender(Hand* hand) { return hand->num_cards == 2; }

void add_card_to_hand(Hand* hand, int card) {
    // Sanity check for the Hand pointer
    if (hand == NULL) {
        fprintf(stderr, "Error: Hand pointer is NULL.\n");
        exit(1);
    }

    // Sanity check for the Cards pointer
    if (hand->cards == NULL) {
        fprintf(stderr,
                "Error: Hand->cards pointer is NULL. Ensure hand is "
                "initialized properly.\n");
        exit(1);
    }

    // Check if resizing is necessary
    if (hand->num_cards >= hand->capacity) {
        hand->capacity *= 2;  // Double the capacity
        int* newCards = realloc(hand->cards, hand->capacity * sizeof(int));

        // Check if realloc failed
        if (newCards == NULL) {
            fprintf(stderr, "Error: Memory reallocation for hand failed.\n");
            free(hand->cards);  // Free existing memory before exiting
            exit(1);
        }

        hand->cards = newCards;  // Update the pointer to the reallocated memory
    }

    // Add the card to the hand
    hand->cards[hand->num_cards++] = card;
}

void free_hand(Hand* hand) {
    // Free the dynamically allocated memory and avoid dangling pointer
    free(hand->cards);
    hand->cards = NULL;
    hand->num_cards = 0;
}

void free_hands(Hand* player_hand, Hand* dealer_hand) {
    free_hand(player_hand);
    free_hand(dealer_hand);
}

int get_hand_value(Hand* hand) {
    int value = 0;
    int ace_count = 0;

    for (int i = 0; i < hand->num_cards; i++) {
        int rank = hand->cards[i];
        if (rank > 10) {
            value += 10;  // Face cards are worth 10
        } else if (rank == 1) {
            value += 1;  // Aces initially count as 1
            ace_count++;
        } else {
            value += rank;
        }
    }

    // Convert Aces from 1 to 11 if it doesn't cause a bust
    while (ace_count > 0 && value + 10 <= 21) {
        value += 10;
        ace_count--;
    }

    return value;
}

int is_bust(Hand* hand) { return get_hand_value(hand) > 21; }
