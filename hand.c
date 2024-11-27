#include "hand.h"
#include <stdio.h>
#include <stdlib.h>


void initHand(Hand *hand) {
    hand->capacity = 5; // Initial capacity
    hand->numCards = 0;
    hand->cards = (Card *)malloc(hand->capacity * sizeof(Card));

    if (hand->cards == NULL) {
        fprintf(stderr, "Error: Memory allocation for hand failed.\n");
        exit(1);
    }
}

void addCardToHand(Hand *hand, Card card) {
    if (hand->numCards >= hand->capacity) {
        hand->capacity *= 2; // Double the capacity
        hand->cards = (Card *)realloc(hand->cards, hand->capacity * sizeof(Card));

        if (hand->cards == NULL) {
            fprintf(stderr, "Error: Memory reallocation for hand failed.\n");
            exit(1);
        }
    }
    hand->cards[hand->numCards++] = card;
}

void freeHand(Hand *hand) {
    free(hand->cards); // Free the dynamically allocated memory
    hand->cards = NULL; // Avoid dangling pointer
    hand->numCards = 0;
}

int getHandValue(const Hand *hand) {
    int value = 0;
    int aceCount = 0;

    for (int i = 0; i < hand->numCards; i++) {
        int rank = hand->cards[i].rank;
        if (rank > 10) {
            value += 10; // Face cards are worth 10
        } else if (rank == 1) {
            value += 1;  // Aces initially count as 1
            aceCount++;
        } else {
            value += rank;
        }
    }

    // Convert Aces from 1 to 11 if it doesn't cause a bust
    while (aceCount > 0 && value + 10 <= 21) {
        value += 10;
        aceCount--;
    }

    return value;
}

int isBust(const Hand *hand) {
    return getHandValue(hand) > 21;
}
