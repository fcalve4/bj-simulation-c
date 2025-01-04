#ifndef DECK_H
#define DECK_H

#include "card.h"

typedef struct {
    Card *cards; // Dynamically allocated array of cards
    int numCards; // Current number of cards
    int capacity; // Maximum capacity of the array
    int top;
} Deck;

// Deck functions
void initDeck(Deck *deck, int numDecks);
void shuffleDeck(Deck *deck);
Card dealCard(Deck *deck);
void freeDeck(Deck *deck);

#endif
