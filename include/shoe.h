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

/**
 * @brief Shuffles the cards in the shoe.
 * 
 * This function uses a Fisher-Yates shuffle to randomize the order of
 * the cards in the shoe. The top index is reset to 0 after shuffling.
 * 
 * @param shoe: Pointer to the shoe to shuffle
 */
void shuffle_shoe(Shoe *shoe);

/**
 * @brief Deals a card from the shoe.
 * 
 * This function returns the card at the top of the shoe and increments
 * the top index. If the shoe is empty,/null an error message is printed 
 * and the program exits.
 * 
 * @param shoe: Pointer to the shoe to deal from
 * @return Card: The card dealt from the shoe
 */
Card deal_card(Shoe *shoe);

/**
 * @brief Frees the memory allocated for the shoe.
 * 
 * This function frees the dynamically allocated memory for the shoe and
 * resets the attributes to 0 or NULL to avoid dangling pointers.
 * 
 * @param shoe: Pointer to the shoe to free
 */
void free_shoe(Shoe *shoe);

#endif
