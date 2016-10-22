// SKPokerEval
//
// Copyright 2010 Kenneth J. Shackleton
//
// This program gives you software freedom; you can copy, convey, propagate,
// redistribute and/or modify this program under the terms of the GNU General
// Public License (GPL) as published by the Free Software Foundation (FSF),
// either version 3 of the License, or (at your option) any later version of
// the GPL published by the FSF.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program in a file in the top-level directory called "GPLv3". If
// not, see http://www.gnu.org/licenses/.

#ifndef SKPOKEREVAL_SEVENEVAL_H
#define SKPOKEREVAL_SEVENEVAL_H

#include <cstdint>
#include "Constants.h"

class SevenEval {
public:
  SevenEval();
  ~SevenEval();
  // Get the rank of a hand comprising seven cards, each represented by an
  // integer from 0 (resp. Ace of Spades) to 51 (resp. Two of Clubs) inclusive.
  // The higher the rank the better the hand. Two hands of equal rank tie.
  inline short unsigned GetRank(int const i, int const j, int const k,
      int const l, int const m, int const n, int const p) const {
    // Create a 7-card hand key by adding up each of the card keys.
    uint_fast32_t key = mDeckcardsKey[i] + mDeckcardsKey[j] + mDeckcardsKey[k] +
      mDeckcardsKey[l] + mDeckcardsKey[m] + mDeckcardsKey[n] + mDeckcardsKey[p];
    // Tear off the flush check strip.
    int_fast16_t const flush_suit = mFlushCheck[key & SUIT_BIT_MASK];
    if (flush_suit == NOT_A_FLUSH) {
      // Tear off the non-flush key strip, and look up the rank.
      key >>= NON_FLUSH_BIT_SHIFT;
      // Take key modulo the circumference.
      return mRankPtr[(key < CIRCUMFERENCE_SEVEN ? key :
                       key - CIRCUMFERENCE_SEVEN)];
    }
    // Generate a flush key, and look up the rank.
    int flush_key = 0;
    if (mDeckcardsSuit[i] == flush_suit) flush_key  = mDeckcardsFlush[i];
    if (mDeckcardsSuit[j] == flush_suit) flush_key += mDeckcardsFlush[j];
    if (mDeckcardsSuit[k] == flush_suit) flush_key += mDeckcardsFlush[k];
    if (mDeckcardsSuit[l] == flush_suit) flush_key += mDeckcardsFlush[l];
    if (mDeckcardsSuit[m] == flush_suit) flush_key += mDeckcardsFlush[m];
    if (mDeckcardsSuit[n] == flush_suit) flush_key += mDeckcardsFlush[n];
    if (mDeckcardsSuit[p] == flush_suit) flush_key += mDeckcardsFlush[p];
    return mFlushRankPtr[flush_key];
  }
private:
  uint16_t *mRankPtr;
  uint16_t *mFlushRankPtr;
  uint32_t mDeckcardsKey[DECK_SIZE];
  uint16_t mDeckcardsFlush[DECK_SIZE];
  uint16_t mDeckcardsSuit[DECK_SIZE];
  int16_t mFlushCheck[MAX_FLUSH_CHECK_SUM+1];
};

#endif // SKPOKEREVAL_SEVENEVAL_H
