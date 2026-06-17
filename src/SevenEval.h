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
#include <cstddef>
#include <cstdint>

class SevenEval final {
public:
  // Get the rank of a hand comprising seven cards, each represented by an
  // integer from 0 (resp. Ace of Spades) to 51 (resp. Two of Clubs) inclusive.
  // Two such integers of the same residue modulo 4 correspond to the same suit.
  // The higher the rank the better the hand. Two hands of equal rank tie.
  template<bool TDoFlushCheck = true>
  static inline uint16_t GetRank(uint8_t c0, uint8_t c1, uint8_t c2,
      uint8_t c3, uint8_t c4, uint8_t c5, uint8_t c6) {
    auto const key = card[c0] + card[c1] + card[c2] + card[c3] + card[c4] +
      card[c5] + card[c6];
    if (TDoFlushCheck) {
      auto const suit = flush_check[key >> FLUSH_BIT_SHIFT];
      if (NOT_A_SUIT != suit) [[unlikely]] {
        auto const& s = suit_kronecker[suit];
        return flush_ranks[s[c0] | s[c1] | s[c2] | s[c3] | s[c4] | s[c5] | s[c6]];
      }
    }
    return GetNonFlushRankFromKey(key);
  }

  // Get the rank of batches of hands comprising six fixed cards and a
  // varying seventh card.
  template<bool TDoFlushCheck = true, typename Callback>
  static inline void GetRankBatch(uint8_t c0, uint8_t c1,
      uint8_t c2, uint8_t c3, uint8_t c4, uint8_t c5,
      uint8_t c6_start, uint8_t c6_end, Callback&& cb) {
    unsigned start = c6_start;
    unsigned end = c6_end;
    if (end > 52u) end = 52u;
    if (start >= end) return;

    uint8_t const start_u8 = static_cast<uint8_t>(start);
    uint8_t const end_u8 = static_cast<uint8_t>(end);

    auto const base_key = card[c0] + card[c1] + card[c2] + card[c3] + card[c4] + card[c5];
    if constexpr (TDoFlushCheck) {
      auto const suit6 = flush_check_6[base_key >> FLUSH_BIT_SHIFT];
      if (NOT_A_SUIT == suit6) [[likely]] {
        GetRankBatch<false>(
          c0, c1, c2, c3, c4, c5, start_u8, end_u8,
          static_cast<Callback&&>(cb));
        return;
      }
      if (suit6 >= GUARANTEED_FLUSH_SUIT) [[unlikely]] {
        auto const& s = suit_kronecker[suit6 - GUARANTEED_FLUSH_SUIT];
        auto const base_bits = s[c0] | s[c1] | s[c2] | s[c3] | s[c4] | s[c5];
        size_t idx = 0;
        for (unsigned q = start; q < end; ++q, ++idx)
          cb(idx, flush_ranks[base_bits | s[static_cast<uint8_t>(q)]]);
        return;
      }
    }
    auto const eval_from_key_and_q = [&](uint32_t key, uint8_t q) -> uint16_t {
      if (TDoFlushCheck) {
        auto const suit = flush_check[key >> FLUSH_BIT_SHIFT];
        if (NOT_A_SUIT != suit) [[unlikely]] {
          auto const& s = suit_kronecker[suit];
          return flush_ranks[s[c0] | s[c1] | s[c2] | s[c3] | s[c4] | s[c5] | s[q]];
        }
      }
      return GetNonFlushRankFromKey(key);
    };

    size_t idx = 0;

    for (; start + 7 < end; start += 8, idx += 8) {
      uint8_t const q0 = static_cast<uint8_t>(start);
      uint8_t const q1 = static_cast<uint8_t>(start + 1);
      uint8_t const q2 = static_cast<uint8_t>(start + 2);
      uint8_t const q3 = static_cast<uint8_t>(start + 3);
      uint8_t const q4 = static_cast<uint8_t>(start + 4);
      uint8_t const q5 = static_cast<uint8_t>(start + 5);
      uint8_t const q6 = static_cast<uint8_t>(start + 6);
      uint8_t const q7 = static_cast<uint8_t>(start + 7);

      auto const key0 = base_key + card[q0];
      auto const key1 = base_key + card[q1];
      auto const key2 = base_key + card[q2];
      auto const key3 = base_key + card[q3];
      auto const key4 = base_key + card[q4];
      auto const key5 = base_key + card[q5];
      auto const key6 = base_key + card[q6];
      auto const key7 = base_key + card[q7];

      auto const r0 = eval_from_key_and_q(key0, q0);
      auto const r1 = eval_from_key_and_q(key1, q1);
      auto const r2 = eval_from_key_and_q(key2, q2);
      auto const r3 = eval_from_key_and_q(key3, q3);
      auto const r4 = eval_from_key_and_q(key4, q4);
      auto const r5 = eval_from_key_and_q(key5, q5);
      auto const r6 = eval_from_key_and_q(key6, q6);
      auto const r7 = eval_from_key_and_q(key7, q7);

      cb(idx, r0);
      cb(idx + 1, r1);
      cb(idx + 2, r2);
      cb(idx + 3, r3);
      cb(idx + 4, r4);
      cb(idx + 5, r5);
      cb(idx + 6, r6);
      cb(idx + 7, r7);
    }

    for (; start + 2 < end; start += 3, idx += 3) {
      uint8_t const q0 = static_cast<uint8_t>(start);
      uint8_t const q1 = static_cast<uint8_t>(start + 1);
      uint8_t const q2 = static_cast<uint8_t>(start + 2);

      auto const key0 = base_key + card[q0];
      auto const key1 = base_key + card[q1];
      auto const key2 = base_key + card[q2];

      auto const r0 = eval_from_key_and_q(key0, q0);
      auto const r1 = eval_from_key_and_q(key1, q1);
      auto const r2 = eval_from_key_and_q(key2, q2);

      cb(idx, r0);
      cb(idx + 1, r1);
      cb(idx + 2, r2);
    }

    for (; start < end; ++start, ++idx) {
      uint8_t const q = static_cast<uint8_t>(start);
      auto const key = base_key + card[q];
      auto const r0 = eval_from_key_and_q(key, q);
      cb(idx, r0);
    }
  }
private:
  SevenEval() {}

  static inline uint16_t GetNonFlushRankFromKey(uint32_t key) {
    auto const hash = FACE_BIT_MASK & (uint32_t)(6019237u * (uint64_t)key + 3215237u);
    return rank_hash[offsets[hash >> RANK_OFFSET_SHIFT] + (hash & RANK_HASH_MOD)];
  }
};

#endif // SKPOKEREVAL_SEVENEVAL_H
