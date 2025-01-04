#ifndef DECK_H
#define DECK_H

#include "card.h"

typedef struct {
    Card *cards; // Dynamically allocated array of cards
    int num_cards; // Current number of cards
    int capacity; // Maximum capacity of the array
    int top;
} Deck;

// Deck functions
void init_deck(Deck *deck, int numDecks);
void shuffle_deck(Deck *deck);
Card deal_card(Deck *deck);
void free_deck(Deck *deck);

#endif
