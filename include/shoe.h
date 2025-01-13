#ifndef SHOE_H
#define SHOE_H

/**
 * @file: shoe.h
 * @author: fcalve4
 * 
 * This file contains the declarations for the Shoe struct and its functions,
 * as well as the Card struct.
 */

/**
 * @struct Card
 * @brief Represents a single playing card.
 * 
 * The card is represented by its rank, which is an integer from 1 to 13.
 * 10-13 represents face cards (10-value), while 1 represents an Ace.
 */
typedef struct {
    int rank; // "Rank" of the card, 1-13 (1 = Ace, 11 = Jack, 12 = Queen, 13 = King)
} Card;

/**
 * @struct Shoe
 * @brief Represents the shoe containing n decks of cards.
 * 
 * The Shoe is implemented as a dynamically allocated array of cards. The shoe
 * is initialized with a certain number of decks, and the cards are shuffled
 * before being dealt.
 */
typedef struct {
    Card *cards; // Dynamically allocated array of Cards
    int num_cards; // Current number of cards in card array
    int capacity; // Maximum capacity of the array
    int top; // Index of the top card in the shoe
} Shoe;

/**
 * @brief Initializes a shoe with a certain number of decks.
 * 
 * This function dynamically allocates memory for the shoe and 
 * populates it with cards based on how many decks are specified. The other
 * attributes are initialized as well.
 * 
 * @param shoe: Pointer to the deck to initialize
 * @param numDecks: Number of decks to use for the shoe
 */
void init_shoe(Shoe *shoe, int numDecks);
void shuffle_shoe(Shoe *shoe);
Card deal_card(Shoe *shoe);
void free_shoe(Shoe *shoe);

#endif
