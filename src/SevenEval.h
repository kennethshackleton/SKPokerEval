// SKPokerEval
//
// Copyright 2010 Kenneth J. Shackleton
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
      if (NOT_A_SUIT != suit) [[unlikely]] {
        auto const& s = suit_kronecker[suit];
        return flush_ranks[s[i] | s[j] | s[k] | s[m] | s[n] | s[p] | s[q]];
      }
    }
    auto const hash = FACE_BIT_MASK &
      static_cast<uint32_t>(6019237u * static_cast<uint64_t>(key) + 3215237u);
    return rank_hash[offsets[hash >> RANK_OFFSET_SHIFT] +
      (hash & RANK_HASH_MOD)];
  }
private:
  SevenEval() {}
};

#endif // SKPOKEREVAL_SEVENEVAL_H
