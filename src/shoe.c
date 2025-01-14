#include "shoe.h"

#include <stdio.h>
#include <stdlib.h>

// "Deck" & "Shoe" are interchangeable in this program
void init_shoe(Shoe* shoe, int num_decks) {
  shoe->cards = (Card*)malloc(52 * num_decks * sizeof(Card));
  shoe->num_cards = shoe->capacity;
  shoe->capacity = 52 * num_decks;
  shoe->top = 0;

  if (shoe->cards == NULL) {
    fprintf(stderr, "Error: Memory allocation for deck failed.\n");
    exit(1);
  }
  // ********************************SHOE VS DECK????
  // Populate the deck with cards
  int index = 0;
  for (int d = 0; d < num_decks; d++) {
    for (int rank = 1; rank <= 13; rank++) {
      for (int count = 0; count < 4; count++) {
        // adjust to only add tens to the deck and not 11,12,13 
        if (rank <= 10) {
          shoe->cards[index].rank = rank;
        } else {
          shoe->cards[index].rank = 10;
        }
        index++;
      }
    }
  }
}

void free_shoe(Shoe* shoe) {
  free(shoe->cards);   // Free the dynamically allocated memory
  shoe->cards = NULL;  // Avoid dangling pointer
  shoe->num_cards = 0;
  shoe->capacity = 0;
  shoe->top = 0;  // Reset the top index after freeing memory
}

void shuffle_shoe(Shoe* shoe) {
  for (int i = shoe->capacity - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    Card temp = shoe->cards[i];
    shoe->cards[i] = shoe->cards[j];
    shoe->cards[j] = temp;
  }
  shoe->top = 0;  // Reset the top index after shuffle
}

Card deal_card(Shoe* shoe) {
  if (shoe->cards == NULL || shoe->top >= shoe->capacity) {
    printf("shoe->top: %d\n", shoe->top);
    printf("shoe->capacity: %d\n", shoe->capacity);
    fprintf(stderr, "Error: No more cards in the shoe.\n");
    exit(1);
  }
  return shoe->cards[shoe->top++];
}
