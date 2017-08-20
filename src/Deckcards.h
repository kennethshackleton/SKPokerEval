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

#ifndef SKPOKEREVAL_DECKCARDS_H
#define SKPOKEREVAL_DECKCARDS_H

#include "Constants.h"
#include <cstdint>

uint_fast32_t const card[DECK_SIZE] = {
  ACE + (SPADE << FLUSH_BIT_SHIFT),
  ACE + (HEART << FLUSH_BIT_SHIFT),
  ACE + (DIAMOND << FLUSH_BIT_SHIFT),
  ACE + (CLUB << FLUSH_BIT_SHIFT),

  KING + (SPADE << FLUSH_BIT_SHIFT),
  KING + (HEART << FLUSH_BIT_SHIFT),
  KING + (DIAMOND << FLUSH_BIT_SHIFT),
  KING + (CLUB << FLUSH_BIT_SHIFT),

  QUEEN + (SPADE << FLUSH_BIT_SHIFT),
  QUEEN + (HEART << FLUSH_BIT_SHIFT),
  QUEEN + (DIAMOND << FLUSH_BIT_SHIFT),
  QUEEN + (CLUB << FLUSH_BIT_SHIFT),

  JACK + (SPADE << FLUSH_BIT_SHIFT),
  JACK + (HEART << FLUSH_BIT_SHIFT),
  JACK + (DIAMOND << FLUSH_BIT_SHIFT),
  JACK + (CLUB << FLUSH_BIT_SHIFT),

  TEN + (SPADE << FLUSH_BIT_SHIFT),
  TEN + (HEART << FLUSH_BIT_SHIFT),
  TEN + (DIAMOND << FLUSH_BIT_SHIFT),
  TEN + (CLUB << FLUSH_BIT_SHIFT),

  NINE + (SPADE << FLUSH_BIT_SHIFT),
  NINE + (HEART << FLUSH_BIT_SHIFT),
  NINE + (DIAMOND << FLUSH_BIT_SHIFT),
  NINE + (CLUB << FLUSH_BIT_SHIFT),

  EIGHT + (SPADE << FLUSH_BIT_SHIFT),
  EIGHT + (HEART << FLUSH_BIT_SHIFT),
  EIGHT + (DIAMOND << FLUSH_BIT_SHIFT),
  EIGHT + (CLUB << FLUSH_BIT_SHIFT),

  SEVEN + (SPADE << FLUSH_BIT_SHIFT),
  SEVEN + (HEART << FLUSH_BIT_SHIFT),
  SEVEN + (DIAMOND << FLUSH_BIT_SHIFT),
  SEVEN + (CLUB << FLUSH_BIT_SHIFT),

  SIX + (SPADE << FLUSH_BIT_SHIFT),
  SIX + (HEART << FLUSH_BIT_SHIFT),
  SIX + (DIAMOND << FLUSH_BIT_SHIFT),
  SIX + (CLUB << FLUSH_BIT_SHIFT),

  FIVE + (SPADE << FLUSH_BIT_SHIFT),
  FIVE + (HEART << FLUSH_BIT_SHIFT),
  FIVE + (DIAMOND << FLUSH_BIT_SHIFT),
  FIVE + (CLUB << FLUSH_BIT_SHIFT),

  FOUR + (SPADE << FLUSH_BIT_SHIFT),
  FOUR + (HEART << FLUSH_BIT_SHIFT),
  FOUR + (DIAMOND << FLUSH_BIT_SHIFT),
  FOUR + (CLUB << FLUSH_BIT_SHIFT),

  THREE + (SPADE << FLUSH_BIT_SHIFT),
  THREE + (HEART << FLUSH_BIT_SHIFT),
  THREE + (DIAMOND << FLUSH_BIT_SHIFT),
  THREE + (CLUB << FLUSH_BIT_SHIFT),

  TWO + (SPADE << FLUSH_BIT_SHIFT),
  TWO + (HEART << FLUSH_BIT_SHIFT),
  TWO + (DIAMOND << FLUSH_BIT_SHIFT),
  TWO + (CLUB << FLUSH_BIT_SHIFT)
};

uint_fast8_t const suit[DECK_SIZE] = {
  INDEX_SPADE, INDEX_HEART, INDEX_DIAMOND, INDEX_CLUB,
  INDEX_SPADE, INDEX_HEART, INDEX_DIAMOND, INDEX_CLUB,
  INDEX_SPADE, INDEX_HEART, INDEX_DIAMOND, INDEX_CLUB,
  INDEX_SPADE, INDEX_HEART, INDEX_DIAMOND, INDEX_CLUB,
  INDEX_SPADE, INDEX_HEART, INDEX_DIAMOND, INDEX_CLUB,
  INDEX_SPADE, INDEX_HEART, INDEX_DIAMOND, INDEX_CLUB,
  INDEX_SPADE, INDEX_HEART, INDEX_DIAMOND, INDEX_CLUB,
  INDEX_SPADE, INDEX_HEART, INDEX_DIAMOND, INDEX_CLUB,
  INDEX_SPADE, INDEX_HEART, INDEX_DIAMOND, INDEX_CLUB,
  INDEX_SPADE, INDEX_HEART, INDEX_DIAMOND, INDEX_CLUB,
  INDEX_SPADE, INDEX_HEART, INDEX_DIAMOND, INDEX_CLUB,
  INDEX_SPADE, INDEX_HEART, INDEX_DIAMOND, INDEX_CLUB,
  INDEX_SPADE, INDEX_HEART, INDEX_DIAMOND, INDEX_CLUB
};

uint_fast16_t const suit_kronecker[NUMBER_OF_SUITS][DECK_SIZE] = {
  {
    ACE_FLUSH,   0, 0, 0,
    KING_FLUSH,  0, 0, 0,
    QUEEN_FLUSH, 0, 0, 0,
    JACK_FLUSH,  0, 0, 0,
    TEN_FLUSH,   0, 0, 0,
    NINE_FLUSH,  0, 0, 0,
    EIGHT_FLUSH, 0, 0, 0,
    SEVEN_FLUSH, 0, 0, 0,
    SIX_FLUSH,   0, 0, 0,
    FIVE_FLUSH,  0, 0, 0,
    FOUR_FLUSH,  0, 0, 0,
    THREE_FLUSH, 0, 0, 0,
    TWO_FLUSH,   0, 0, 0
  },
  {
    0, ACE_FLUSH,   0, 0,
    0, KING_FLUSH,  0, 0,
    0, QUEEN_FLUSH, 0, 0,
    0, JACK_FLUSH,  0, 0,
    0, TEN_FLUSH,   0, 0,
    0, NINE_FLUSH,  0, 0,
    0, EIGHT_FLUSH, 0, 0,
    0, SEVEN_FLUSH, 0, 0,
    0, SIX_FLUSH,   0, 0,
    0, FIVE_FLUSH,  0, 0,
    0, FOUR_FLUSH,  0, 0,
    0, THREE_FLUSH, 0, 0,
    0, TWO_FLUSH,   0, 0
  },
  {
    0, 0, ACE_FLUSH,   0,
    0, 0, KING_FLUSH,  0,
    0, 0, QUEEN_FLUSH, 0,
    0, 0, JACK_FLUSH,  0,
    0, 0, TEN_FLUSH,   0,
    0, 0, NINE_FLUSH,  0,
    0, 0, EIGHT_FLUSH, 0,
    0, 0, SEVEN_FLUSH, 0,
    0, 0, SIX_FLUSH,   0,
    0, 0, FIVE_FLUSH,  0,
    0, 0, FOUR_FLUSH,  0,
    0, 0, THREE_FLUSH, 0,
    0, 0, TWO_FLUSH,   0
  },
  {
    0, 0, 0, ACE_FLUSH,
    0, 0, 0, KING_FLUSH,
    0, 0, 0, QUEEN_FLUSH,
    0, 0, 0, JACK_FLUSH,
    0, 0, 0, TEN_FLUSH,
    0, 0, 0, NINE_FLUSH,
    0, 0, 0, EIGHT_FLUSH,
    0, 0, 0, SEVEN_FLUSH,
    0, 0, 0, SIX_FLUSH,
    0, 0, 0, FIVE_FLUSH,
    0, 0, 0, FOUR_FLUSH,
    0, 0, 0, THREE_FLUSH,
    0, 0, 0, TWO_FLUSH
  }
};

#endif
