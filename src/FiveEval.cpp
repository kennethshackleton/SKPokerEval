#include "FiveEval.h"

FiveEval::FiveEval() :
mRankPtr(new short unsigned[MAX_FIVE_NONFLUSH_KEY_INT+1]),
mFlushRankPtr(new short unsigned[MAX_FIVE_FLUSH_KEY_INT+1]) {
  int unsigned const face[13] = {TWO_FIVE, THREE_FIVE, FOUR_FIVE, FIVE_FIVE,
    SIX_FIVE, SEVEN_FIVE, EIGHT_FIVE, NINE_FIVE, TEN_FIVE, JACK_FIVE,
    QUEEN_FIVE, KING_FIVE, ACE_FIVE};
  int unsigned const face_flush[13] = {TWO_FLUSH, THREE_FLUSH, FOUR_FLUSH,
    FIVE_FLUSH, SIX_FLUSH, SEVEN_FLUSH, EIGHT_FLUSH, NINE_FLUSH, TEN_FLUSH,
    JACK_FLUSH, QUEEN_FLUSH, KING_FLUSH, ACE_FLUSH};
  
  for (int n = 0; n < 13; ++n) {
    int const N = n<<2;
    mDeckcardsSuit[N] = SPADE;
    mDeckcardsSuit[N+1] = HEART;
    mDeckcardsSuit[N+2] = DIAMOND;
    mDeckcardsSuit[N+3] = CLUB;
    
    mDeckcardsFace[N] = face[12-n];
    mDeckcardsFace[N+1] = face[12-n];
    mDeckcardsFace[N+2] = face[12-n];
    mDeckcardsFace[N+3] = face[12-n];
    
    mDeckcardsFlush[N] = (short unsigned) face_flush[12-n];
    mDeckcardsFlush[N+1] = (short unsigned) face_flush[12-n];
    mDeckcardsFlush[N+2] = (short unsigned) face_flush[12-n];
    mDeckcardsFlush[N+3] = (short unsigned) face_flush[12-n];
  }
  
  // n increments as rank.
  short unsigned n = 1;
  
  // High card.
  for (int i = 5; i < NUMBER_OF_FACES; ++i) {
    for (int j = 3; j < i; ++j) {
      for (int k = 2; k < j; ++k) {
        for (int l = 1; l < k; ++l) {
          // No straights.
          for (int m = 0; m < l && !(i-m == 4 || (i == 12 && j == 3)); ++m) {
            mRankPtr[face[i] + face[j] + face[k] + face[l] + face[m]] = n++;
          }
        }
      }
    }
  }
  
  // Pair.
  for (int i = 0; i < NUMBER_OF_FACES; ++i) {
    for (int j = 2; j < NUMBER_OF_FACES; ++j) {
      for (int k = 1; k < j; ++k) {
        for (int l = 0; l < k; ++l) {
          if (i != j && i != k && i != l) {
            mRankPtr[(face[i]<<1) + face[j] + face[k] + face[l]] = n++;
          }
        }
      }
    }
  }
  
  // Two pair.
  for (int i = 1; i < NUMBER_OF_FACES; ++i) {
    for (int j = 0; j < i; ++j) {
      for (int k = 0; k < NUMBER_OF_FACES; ++k) {
        // No full-houses.
        if (k != i && k != j) {
          mRankPtr[(face[i]<<1) + (face[j]<<1) + face[k]] = n++;
        }
      }
    }
  }
  
  // Triple.
  for (int i = 0; i < NUMBER_OF_FACES; ++i) {
    for (int j = 1; j < NUMBER_OF_FACES; ++j) {
      for (int k = 0; k < j; ++k) {
        // No quads.
        if (i != j && i != k) {
          mRankPtr[(3*face[i]) + face[j] + face[k]] = n++;
        }
      }
    }
  }
  
  // Low straight non-flush.
  mRankPtr[face[12] + face[0] + face[1] + face[2] + face[3]] = n++;
  
  // Usual straight non-flush.
  for (int i = 0; i < 9; ++i) {
    mRankPtr[face[i] + face[i+1] + face[i+2] + face[i+3] + face[i+4]] = n++;
  }
  
  // Flush not a straight.
  for (int i = 5; i < NUMBER_OF_FACES; ++i) {
    for (int j = 3; j < i; ++j) {
      for (int k = 2; k < j; ++k) {
        for (int l = 1; l < k ; ++l) {
          for (int m = 0; m < l; ++m) {
            if (!(i-m == 4 || (i == 12 && j == 3))) {
              mFlushRankPtr[face_flush[i] + face_flush[j] + face_flush[k] +
                            face_flush[l] + face_flush[m]] = n++;
            }
          }
        }
      }
    }
  }
  
  // Full house.
  for (int i = 0; i < NUMBER_OF_FACES; ++i) {
    for (int j = 0; j < NUMBER_OF_FACES; ++j) {
      if (i != j) {
        mRankPtr[(3 * face[i]) + (face[j]<<1)] = n++;
      }
    }
  }
  
  // Quad.
  for (int i = 0; i < NUMBER_OF_FACES; ++i) {
    for (int j = 0; j < NUMBER_OF_FACES; ++j) {
      if (i != j) {
        mRankPtr[(face[i]<<2) + face[j]] = n++;
      }
    }
  }
  
  // Low straight flush.
  mFlushRankPtr[face_flush[0] + face_flush[1] + face_flush[2] +
                face_flush[3] + face_flush[12]] = n++;
  
  // Usual straight flush.
  for (int i = 0; i < 9; ++i) {
    mFlushRankPtr[face_flush[i] + face_flush[i+1] + face_flush[i+2] +
                  face_flush[i+3] + face_flush[i+4]] = n++;
  }
}

FiveEval::~FiveEval() {
  delete mRankPtr;
  delete mFlushRankPtr;
}

short unsigned FiveEval::GetRank(int const card_one, int const card_two,
                                 int const card_three, int const card_four,
                                 int const card_five) const {
  if ((mDeckcardsSuit[card_one] == mDeckcardsSuit[card_two]) &&
      (mDeckcardsSuit[card_one] == mDeckcardsSuit[card_three]) &&
      (mDeckcardsSuit[card_one] == mDeckcardsSuit[card_four]) &&
      (mDeckcardsSuit[card_one] == mDeckcardsSuit[card_five])) {
    return mFlushRankPtr[mDeckcardsFlush[card_one] +
                         mDeckcardsFlush[card_two] +
                         mDeckcardsFlush[card_three] +
                         mDeckcardsFlush[card_four] +
                         mDeckcardsFlush[card_five]];
  }
  return mRankPtr[mDeckcardsFace[card_one] +
                  mDeckcardsFace[card_two] +
                  mDeckcardsFace[card_three] +
                  mDeckcardsFace[card_four] +
                  mDeckcardsFace[card_five]];
}

short unsigned FiveEval::GetRank(int const card_one, const int card_two,
                                 const int card_three, const int card_four,
                                 const int card_five, const int card_six,
                                 const int card_seven) const {
  int seven_cards[7] = {card_one, card_two, card_three, card_four, card_five,
    card_six, card_seven};
  int temp[5];
  
  short unsigned best_rank_so_far = 0, current_rank = 0;
  int m = 0;
  
  for (int i = 1; i < 7; ++i) {
    for (int j = 0; j < i; ++j) {
      m = 0;
      for (int k = 0; k < 7; ++k) {
        if (k != i && k !=j) {
          temp[m++] = seven_cards[k];
        }
      }
      current_rank = GetRank(temp[0], temp[1], temp[2], temp[3], temp[4]);
      if (best_rank_so_far < current_rank) {
        best_rank_so_far = current_rank;
      }
    }
  }
  return best_rank_so_far;
}
