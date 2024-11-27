#ifndef HAND_H
#define HAND_H

#include "card.h"

typedef struct {
    Card *cards;  // Dynamically allocated array for cards
    int numCards; // Number of cards in the hand
    int capacity; // Capacity of the hand

    // Booleans
    int softOrHard;
    int doubleable;
    int splittable;
    int surrenderable;
} Hand;

// Hand functions
void initHand(Hand *hand);
void addCardToHand(Hand *hand, Card card);
int getHandValue(const Hand *hand);
int isBust(const Hand *hand);
void freeHand(Hand *hand);

int hasSoftAce(const Hand *hand);
int canDouble(const Hand *hand);
int canSplit(const Hand *hand);
int canSurrender(const Hand *hand);

#endif
