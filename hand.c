#include "hand.h"
#include <stdio.h>
#include <stdlib.h>

#include "card.h"


void initHand(Hand *hand) {
    hand->capacity = 5; // Initial capacity
    hand->numCards = 0;
    hand->cards = (Card *)malloc(hand->capacity * sizeof(Card));

    // init Booleans
    hand->softOrHard = 0;
    hand->doubleable = 0;
    hand->splittable = 0;
    hand->surrenderable = 0;

    if (hand->cards == NULL) {
        fprintf(stderr, "Error: Memory allocation for hand failed.\n");
        exit(1);
    }
}

int hasSoftAce(const Hand *hand) {
    int value, aceCount;
    for (int i = 0; i < hand->numCards; i++) {
        if (hand->cards[i].rank == 1) {
            aceCount++;
            value += 11;
        } else {
            value += hand->cards[i].rank;
        }
    }
    return aceCount > 0 && value <= 21;
}

int canDouble(const Hand *hand) {
    return hand->numCards == 2;
}

int canSplit(const Hand *hand) {
    return hand->numCards == 2 && hand->cards[0].rank == hand->cards[1].rank;
}
int canSurrender(const Hand *hand) {
    return hand->numCards == 2;
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

    // Update soft/hard status
    hand->softOrHard = hasSoftAce(hand);
    hand->doubleable = canDouble(hand);
    hand->splittable = canSplit(hand);
    hand->surrenderable = canSurrender(hand);
}

void freeHand(Hand *hand) {
    // Free the dynamically allocated memory and avoid dangling pointer
    free(hand->cards); 
    hand->cards = NULL;
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
