#ifndef HAND_H
#define HAND_H

#include "metadata.h"
#include "shoe.h"

/**
 *
 */
typedef struct {
  Card* cards;    // Dynamically allocated array for cards
  int num_cards;  // Number of cards in the hand
  int capacity;   // Capacity of the hand
} Hand;

// Hand functions
void init_hand(Hand* hand);
void add_card_to_hand(Hand* hand, Card card);
int get_hand_value(Hand* hand);
int is_bust(Hand* hand);
void free_hand(Hand* hand);
void free_hands(Hand* player_hand, Hand* dealer_hand);

int has_soft_ace(Hand* hand);
int can_double(Hand* hand);
int can_split(Hand* hand, Metadata* metadata);
int can_surrender(Hand* hand);

#endif
