#ifndef DECK_H
#define DECK_H

/**
 * @file: deck.h
 * @author: fcalve4
 * 
 * This file contains the declarations for the Deck struct and its functions,
 * as well as the Card struct.
 */


/**
 * @struct Deck
 * @brief Represents a deck of cards.
 * 
 * The deck is implemented as a dynamically allocated array of cards. The deck (shoe)
 * is initialized with a certain number of decks, and the cards are shuffled
 * before being dealt. 
 */
typedef struct {
    Card *cards; // Dynamically allocated array of cards
    int num_cards; // Current number of cards
    int capacity; // Maximum capacity of the array
    int top;
} Deck;

/**
 * @struct Card
 * @brief Represents a single card in a deck.
 * 
 * The card is represented by its rank, which is an integer from 1 to 13.
 * 10-13 represents face cards (10-value), while 1 represents an Ace.
 */
typedef struct {
    int rank; // 1-13 (1 = Ace, 11 = Jack, 12 = Queen, 13 = King)
} Card;

// Deck functions
void init_deck(Deck *deck, int numDecks);
void shuffle_deck(Deck *deck);
Card deal_card(Deck *deck);
void free_deck(Deck *deck);

#endif
