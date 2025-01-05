#include "metadata.h"

void init_metadata(Metadata *metadata, int num_decks, int h17, int ls, int enhc, int bj_pay, int pen, int num_simulations, int bankroll, int wager)
{
    metadata->num_decks = num_decks;
    metadata->h17 = h17;
    metadata->ls = ls;
    metadata->enhc = enhc;
    metadata->bj_pay = bj_pay;
    metadata-> pen = pen;

    metadata-> num_simulations = num_simulations;
    metadata->bankroll = bankroll;
    metadata->wager = wager;
    
}