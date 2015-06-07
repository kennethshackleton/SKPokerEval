#include "SevenEval.h"
#include "FiveEval.h"

SevenEval::SevenEval() : mRankPtr(new short unsigned[CIRCUMFERENCE_SEVEN]),
    mFlushRankPtr(new short unsigned[MAX_SEVEN_FLUSH_KEY_INT+1]) {
  int const face[13] = {ACE, KING, QUEEN, JACK, TEN, NINE, EIGHT, SEVEN, SIX,
    FIVE, FOUR, THREE, TWO};
  int const face_flush[13] = {ACE_FLUSH, KING_FLUSH, QUEEN_FLUSH, JACK_FLUSH,
    TEN_FLUSH, NINE_FLUSH, EIGHT_FLUSH, SEVEN_FLUSH, SIX_FLUSH, FIVE_FLUSH,
    FOUR_FLUSH, THREE_FLUSH, TWO_FLUSH};
  
  for (int n = 0; n < 13; n++) {
    int const N = n<<2;
    long unsigned const start = face[n] << NON_FLUSH_BIT_SHIFT;
    mDeckcardsKey[N] = start + SPADE;
    mDeckcardsKey[N+1] = start + HEART;
    mDeckcardsKey[N+2] = start + DIAMOND;
    mDeckcardsKey[N+3] = start + CLUB;
    
    mDeckcardsFlush[N] = (short unsigned) face_flush[n];
    mDeckcardsFlush[N+1] = (short unsigned) face_flush[n];
    mDeckcardsFlush[N+2] = (short unsigned) face_flush[n];
    mDeckcardsFlush[N+3] = (short unsigned) face_flush[n];
    
    mDeckcardsSuit[N] = SPADE;
    mDeckcardsSuit[N+1] = HEART;
    mDeckcardsSuit[N+2] = DIAMOND;
    mDeckcardsSuit[N+3] = CLUB;
  }
  
  int count = 0;
  
  // Generate seven-ranks from five-ranks.
  FiveEval five_card_evaluator;
  
  // Non-flush ranks.
  for (int i = 1; i < 13; ++i) {
    for (int j = 1; j <= i; ++j) {
      for (int k = 1; k <= j; ++k) {
        for (int l = 0; l <= k; ++l) {
          for (int m = 0; m <= l; ++m) {
            for (int n = 0; n <= m; ++n) {
              for (int p = 0; p <= n; ++p) {
                if (i != m && j != n && k != p) {
                  int const key = face[i] + face[j] + face[k] + face[l] +
                      face[m] + face[n] + face[p];
                  // The (4*i)+0 and (4*m)+1 trick prevents
                  // flushes.
                  short unsigned const rank =
                  five_card_evaluator.GetRank(i<<2, j<<2, k<<2, l<<2, (m<<2)+1,
                                              (n<<2)+1, (p<<2)+1);
                  mRankPtr[key < CIRCUMFERENCE_SEVEN ?
                           key : key - CIRCUMFERENCE_SEVEN] = rank;
                  ++count;
                }
              }
            }
          }
        }
      }
    }
  }
  
  // Flush ranks, all seven of the same suit.
  for (int i = 6; i < 13; ++i) {
    for (int j = 5; j < i; ++j) {
      for(int k = 4; k < j; ++k) {
        for(int l = 3; l < k; ++l) {
          for(int m = 2; m < l; ++m) {
            for(int n = 1; n < m; ++n) {
              for(int p = 0; p < n; ++p) {
                int const key = face_flush[i] + face_flush[j] + face_flush[k] +
                    face_flush[l] + face_flush[m] + face_flush[n] +
                    face_flush[p];
                mFlushRankPtr[key] =
                  five_card_evaluator.GetRank(i<<2, j<<2, k<<2, l<<2, m<<2,
                                              n<<2, p<<2);
                ++count;
              }
            }
          }
        }
      }
    }
  }
  
  // Only six cards of the same suit.
  for (int i = 5; i < 13; ++i) {
    for (int j = 4; j < i; ++j) {
      for (int k = 3; k < j; ++k) {
        for (int l = 2; l < k; ++l) {
          for (int m = 1; m < l; ++m) {
            for (int n = 0; n < m; ++n) {
              int const key = face_flush[i] + face_flush[j] + face_flush[k] +
                  face_flush[l] + face_flush[m] + face_flush[n];
              // The Two of Clubs is the card at index 51; the other six
              // cards all have the spade suit.
              mFlushRankPtr[key] =
              five_card_evaluator.GetRank(i<<2, j<<2, k<<2, l<<2, m<<2, n<<2,
                                          51);
              ++count;
            }
          }
        }
      }
    }
  }
  
  // Only five cards of one suit.
  for (int i = 4; i < 13; ++i) {
    for (int j = 3; j < i; ++j) {
      for (int k = 2; k < j; ++k) {
        for (int l = 1; l < k; ++l) {
          for (int m = 0; m < l; ++m) {
            int const key = face_flush[i] + face_flush[j] + face_flush[k] +
                face_flush[l] + face_flush[m];
            mFlushRankPtr[key] = five_card_evaluator.GetRank(i<<2, j<<2, k<<2,
                                                             l<<2, m<<2);
            ++count;
          }
        }
      }
    }
  }
  
  // Initialise flush checks.
  short suit_count = 0, flush_suit_index = -1, cards_matched_so_far = 0;
  short suit_key = SPADE;
  short const suits[4] = {SPADE, HEART, DIAMOND, CLUB};
  
  // Initialise all entries of mFlushCheck[] to UNVERIFIED, as yet unchecked.
  for (int i = 0 ; i < MAX_FLUSH_CHECK_SUM+1; ++i) {
    mFlushCheck[i] = UNVERIFIED;
  }
  
  // Seven-card flush.
  for (int i = 0; i < NUMBER_OF_SUITS; ++i) {
    for (int j = 0; j <= i; ++j) {
      for (int k = 0; k <= j; ++k) {
        for (int l = 0; l <= k; ++l) {
          for (int m = 0; m <= l; ++m) {
            for (int n = 0; n <= m; ++n) {
              for (int p = 0; p <= n; ++p) {
                flush_suit_index = -1;
                cards_matched_so_far = 0;
                suit_key = suits[i] + suits[j] + suits[k] +
                suits[l] + suits[m] + suits[n] +
                suits[p];
                if (UNVERIFIED == mFlushCheck[suit_key]) {
                  do {
                    ++flush_suit_index;
                    suit_count = (suits[i] == suits[flush_suit_index]) +
                    (suits[j] == suits[flush_suit_index]) +
                    (suits[k] == suits[flush_suit_index]) +
                    (suits[l] == suits[flush_suit_index]) +
                    (suits[m] == suits[flush_suit_index]) +
                    (suits[n] == suits[flush_suit_index]) +
                    (suits[p] == suits[flush_suit_index]);
                    cards_matched_so_far += suit_count;
                  } while (cards_matched_so_far < 3 && flush_suit_index < 4);
                  // A count of 5 or more means we have a
                  // flush. We place the value of the flush
                  // suit here.
                  if (suit_count > 4) {
                    mFlushCheck[suit_key] = suits[flush_suit_index];
                  } else {
                    // Otherwise this is a non-flush hand.
                    mFlushCheck[suit_key] = NOT_A_FLUSH;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

SevenEval::~SevenEval() {
  delete mRankPtr;
  delete mFlushRankPtr;
}

short unsigned SevenEval::GetRank(int const i, int const j, int const k,
                                  int const l, int const m, int const n,
                                  int const p) const {
  // Create a 7-card hand key by adding up each of the card keys.
  long unsigned key = mDeckcardsKey[i] + mDeckcardsKey[j] + mDeckcardsKey[k] +
  mDeckcardsKey[l] + mDeckcardsKey[m] + mDeckcardsKey[n] + mDeckcardsKey[p];
  // Tear off the flush check strip.
  int const flush_check_key = (const int) (key & SUIT_BIT_MASK);
  short flush_suit = mFlushCheck[flush_check_key];
  if (flush_suit == NOT_A_FLUSH) {
    // Tear off the non-flush key strip, and look up the rank.
    key = (key >> NON_FLUSH_BIT_SHIFT);
    // Take key modulo the circumference.
    return mRankPtr[(key < CIRCUMFERENCE_SEVEN ? key :
                     key - CIRCUMFERENCE_SEVEN)];
  }
  // Generate a flush key, and look up the rank.
  int const flush_key =
    (mDeckcardsSuit[i] == flush_suit ? mDeckcardsFlush[i] : 0) +
    (mDeckcardsSuit[j] == flush_suit ? mDeckcardsFlush[j] : 0) +
    (mDeckcardsSuit[k] == flush_suit ? mDeckcardsFlush[k] : 0) +
    (mDeckcardsSuit[l] == flush_suit ? mDeckcardsFlush[l] : 0) +
    (mDeckcardsSuit[m] == flush_suit ? mDeckcardsFlush[m] : 0) +
    (mDeckcardsSuit[n] == flush_suit ? mDeckcardsFlush[n] : 0) +
    (mDeckcardsSuit[p] == flush_suit ? mDeckcardsFlush[p] : 0);
  return mFlushRankPtr[flush_key];
}
