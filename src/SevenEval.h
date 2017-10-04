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

#include "RankHash.h"
#include "RankOffsets.h"
#include "FlushRanks.h"
#include "FlushCheck.h"
#include "Deckcards.h"
#include "Constants.h"
#include <cstdint>

class SevenEval final {
public:
  // Get the rank of a hand comprising seven cards, each represented by an
  // integer from 0 (resp. Ace of Spades) to 51 (resp. Two of Clubs) inclusive.
  // Two such integers of the same residue modulo 4 correspond to the same suit.
  // The higher the rank the better the hand. Two hands of equal rank tie.
  template<bool TDoFlushCheck = true>
  static inline uint16_t GetRank(uint8_t i, uint8_t j, uint8_t k, uint8_t m,
      uint8_t n, uint8_t p, uint8_t q) {
    auto const key = card[i] + card[j] + card[k] + card[m] + card[n] +
      card[p] + card[q];
    if (TDoFlushCheck) {
      auto const suit = flush_check[key >> FLUSH_BIT_SHIFT];
      if (NOT_A_SUIT != suit) {
        auto const * const s = suit_kronecker[suit];
        return flush_ranks[s[i] | s[j] | s[k] | s[m] | s[n] | s[p] | s[q]];
      }
    }
    auto const hash = FACE_BIT_MASK & (uint32_t)(31 * (uint64_t) key);
    return rank_hash[offsets[hash >> RANK_OFFSET_SHIFT] +
      (hash & RANK_HASH_MOD)];
  }
private:
  SevenEval() {}
};

#endif // SKPOKEREVAL_SEVENEVAL_H
