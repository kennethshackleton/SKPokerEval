#include <iostream>
#include "HandEval.h"
#include "SevenEval.h"

using namespace std;

int main (int argc, char *const argv[]) {
  HandEval *eval = new HandEval();
  int holeCards[4] = {0, 11, 37, 38}; // Two pairs of hole cards, the first
                                      // player has the Ace of Spades and the
                                      // Queen of Clubs, while the second
                                      // player has the 5 of Hearts and the
                                      // 5 of Diamonds.
  
  // The equity should be approximately 45.33% versus 54.67%.
  printf("\n%s\n", eval->computePreFlopEquityForSpecificHoleCards(holeCards, 2));
  eval->timeRankMethod();
  delete eval;
  
  SevenEval *sEval = new SevenEval();
  printf("%i\n", sEval->getRankOfSeven(0, 1, 2, 3, 4, 5, 6)); // Ace quad with kings, 7452
  printf("%i\n", sEval->getRankOfSeven(5, 6, 2, 4, 3, 0, 1)); // Shuffled ace quad with kings, again 7452
  printf("%i\n", sEval->getRankOfSeven(0, 4, 8, 12, 16, 20, 24)); // Spade royal flush, 7462
  printf("%i\n", sEval->getRankOfSeven(51, 47, 43, 39, 30, 26, 22)); // 49, the worst hand.
  delete sEval;
  
  return 0;
}
