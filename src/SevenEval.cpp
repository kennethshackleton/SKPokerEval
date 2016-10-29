#include "SevenEval.h"
#include "FiveEval.h"
#include "RankHash.h"
#include "RankOffsets.h"
#include "FlushRanks.h"
#include "FlushCheck.h"
#include <cstddef>

SevenEval::SevenEval() {
  int const face[13] = {ACE, KING, QUEEN, JACK, TEN, NINE, EIGHT, SEVEN, SIX,
    FIVE, FOUR, THREE, TWO};
  int const face_flush[13] = {ACE_FLUSH, KING_FLUSH, QUEEN_FLUSH, JACK_FLUSH,
    TEN_FLUSH, NINE_FLUSH, EIGHT_FLUSH, SEVEN_FLUSH, SIX_FLUSH, FIVE_FLUSH,
    FOUR_FLUSH, THREE_FLUSH, TWO_FLUSH};
  for (size_t n = 0; n < 13; ++n) {
    size_t const N = n<<2;
    int unsigned const start = face[n] << NON_FLUSH_BIT_SHIFT;

    mDeckcardsKey[N  ] = start + SPADE;
    mDeckcardsKey[N+1] = start + HEART;
    mDeckcardsKey[N+2] = start + DIAMOND;
    mDeckcardsKey[N+3] = start + CLUB;
    
    mDeckcardsFlush[N  ] = face_flush[n];
    mDeckcardsFlush[N+1] = face_flush[n];
    mDeckcardsFlush[N+2] = face_flush[n];
    mDeckcardsFlush[N+3] = face_flush[n];
    
    mDeckcardsSuit[N  ] = SPADE;
    mDeckcardsSuit[N+1] = HEART;
    mDeckcardsSuit[N+2] = DIAMOND;
    mDeckcardsSuit[N+3] = CLUB;
  }
}

SevenEval::~SevenEval() {}

uint16_t SevenEval::GetRank(int const i, int const j, int const k,
    int const l, int const m, int const n, int const p) const {
  // Create a 7-card hand key by adding up each of the card keys.
  uint_fast32_t key = mDeckcardsKey[i] + mDeckcardsKey[j] + mDeckcardsKey[k] +
    mDeckcardsKey[l] + mDeckcardsKey[m] + mDeckcardsKey[n] + mDeckcardsKey[p];
  // Tear off the flush check strip.
  int_fast8_t const flush_suit = flush_check[key & SUIT_BIT_MASK];
  if (NOT_A_FLUSH == flush_suit) {
    // Tear off the non-flush key strip, and look up the rank.
    key >>= NON_FLUSH_BIT_SHIFT;
    return rank_hash[offsets[key >> RANK_OFFSET_SHIFT] +
      (key & RANK_HASH_MOD)];
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
  return flush_ranks[flush_key];
}
