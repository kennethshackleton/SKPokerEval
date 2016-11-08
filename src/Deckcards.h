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

uint32_t const deckcards_key[DECK_SIZE] = {
    (ACE << NON_FLUSH_BIT_SHIFT) + SPADE,
    (ACE << NON_FLUSH_BIT_SHIFT) + HEART,
    (ACE << NON_FLUSH_BIT_SHIFT) + DIAMOND,
    (ACE << NON_FLUSH_BIT_SHIFT) + CLUB,

    (KING << NON_FLUSH_BIT_SHIFT) + SPADE,
    (KING << NON_FLUSH_BIT_SHIFT) + HEART,
    (KING << NON_FLUSH_BIT_SHIFT) + DIAMOND,
    (KING << NON_FLUSH_BIT_SHIFT) + CLUB,

    (QUEEN << NON_FLUSH_BIT_SHIFT) + SPADE,
    (QUEEN << NON_FLUSH_BIT_SHIFT) + HEART,
    (QUEEN << NON_FLUSH_BIT_SHIFT) + DIAMOND,
    (QUEEN << NON_FLUSH_BIT_SHIFT) + CLUB,

    (JACK << NON_FLUSH_BIT_SHIFT) + SPADE,
    (JACK << NON_FLUSH_BIT_SHIFT) + HEART,
    (JACK << NON_FLUSH_BIT_SHIFT) + DIAMOND,
    (JACK << NON_FLUSH_BIT_SHIFT) + CLUB,

    (TEN << NON_FLUSH_BIT_SHIFT) + SPADE,
    (TEN << NON_FLUSH_BIT_SHIFT) + HEART,
    (TEN << NON_FLUSH_BIT_SHIFT) + DIAMOND,
    (TEN << NON_FLUSH_BIT_SHIFT) + CLUB,

    (NINE << NON_FLUSH_BIT_SHIFT) + SPADE,
    (NINE << NON_FLUSH_BIT_SHIFT) + HEART,
    (NINE << NON_FLUSH_BIT_SHIFT) + DIAMOND,
    (NINE << NON_FLUSH_BIT_SHIFT) + CLUB,

    (EIGHT << NON_FLUSH_BIT_SHIFT) + SPADE,
    (EIGHT << NON_FLUSH_BIT_SHIFT) + HEART,
    (EIGHT << NON_FLUSH_BIT_SHIFT) + DIAMOND,
    (EIGHT << NON_FLUSH_BIT_SHIFT) + CLUB,

    (SEVEN << NON_FLUSH_BIT_SHIFT) + SPADE,
    (SEVEN << NON_FLUSH_BIT_SHIFT) + HEART,
    (SEVEN << NON_FLUSH_BIT_SHIFT) + DIAMOND,
    (SEVEN << NON_FLUSH_BIT_SHIFT) + CLUB,

    (SIX << NON_FLUSH_BIT_SHIFT) + SPADE,
    (SIX << NON_FLUSH_BIT_SHIFT) + HEART,
    (SIX << NON_FLUSH_BIT_SHIFT) + DIAMOND,
    (SIX << NON_FLUSH_BIT_SHIFT) + CLUB,

    (FIVE << NON_FLUSH_BIT_SHIFT) + SPADE,
    (FIVE << NON_FLUSH_BIT_SHIFT) + HEART,
    (FIVE << NON_FLUSH_BIT_SHIFT) + DIAMOND,
    (FIVE << NON_FLUSH_BIT_SHIFT) + CLUB,

    (FOUR << NON_FLUSH_BIT_SHIFT) + SPADE,
    (FOUR << NON_FLUSH_BIT_SHIFT) + HEART,
    (FOUR << NON_FLUSH_BIT_SHIFT) + DIAMOND,
    (FOUR << NON_FLUSH_BIT_SHIFT) + CLUB,

    (THREE << NON_FLUSH_BIT_SHIFT) + SPADE,
    (THREE << NON_FLUSH_BIT_SHIFT) + HEART,
    (THREE << NON_FLUSH_BIT_SHIFT) + DIAMOND,
    (THREE << NON_FLUSH_BIT_SHIFT) + CLUB,

    (TWO << NON_FLUSH_BIT_SHIFT) + SPADE,
    (TWO << NON_FLUSH_BIT_SHIFT) + HEART,
    (TWO << NON_FLUSH_BIT_SHIFT) + DIAMOND,
    (TWO << NON_FLUSH_BIT_SHIFT) + CLUB
};

uint16_t const deckcards_flush[DECK_SIZE] = {
    ACE_FLUSH,   ACE_FLUSH,   ACE_FLUSH,   ACE_FLUSH,
    KING_FLUSH,  KING_FLUSH,  KING_FLUSH,  KING_FLUSH,
    QUEEN_FLUSH, QUEEN_FLUSH, QUEEN_FLUSH, QUEEN_FLUSH,
    JACK_FLUSH,  JACK_FLUSH,  JACK_FLUSH,  JACK_FLUSH,
    TEN_FLUSH,   TEN_FLUSH,   TEN_FLUSH,   TEN_FLUSH,
    NINE_FLUSH,  NINE_FLUSH,  NINE_FLUSH,  NINE_FLUSH,
    EIGHT_FLUSH, EIGHT_FLUSH, EIGHT_FLUSH, EIGHT_FLUSH,
    SEVEN_FLUSH, SEVEN_FLUSH, SEVEN_FLUSH, SEVEN_FLUSH,
    SIX_FLUSH,   SIX_FLUSH,   SIX_FLUSH,   SIX_FLUSH,
    FIVE_FLUSH,  FIVE_FLUSH,  FIVE_FLUSH,  FIVE_FLUSH,
    FOUR_FLUSH,  FOUR_FLUSH,  FOUR_FLUSH,  FOUR_FLUSH,
    THREE_FLUSH, THREE_FLUSH, THREE_FLUSH, THREE_FLUSH,
    TWO_FLUSH,   TWO_FLUSH,   TWO_FLUSH,   TWO_FLUSH
};

uint16_t const deckcards_suit[DECK_SIZE] = {
    SPADE, HEART, DIAMOND, CLUB,
    SPADE, HEART, DIAMOND, CLUB,
    SPADE, HEART, DIAMOND, CLUB,
    SPADE, HEART, DIAMOND, CLUB,
    SPADE, HEART, DIAMOND, CLUB,
    SPADE, HEART, DIAMOND, CLUB,
    SPADE, HEART, DIAMOND, CLUB,
    SPADE, HEART, DIAMOND, CLUB,
    SPADE, HEART, DIAMOND, CLUB,
    SPADE, HEART, DIAMOND, CLUB,
    SPADE, HEART, DIAMOND, CLUB,
    SPADE, HEART, DIAMOND, CLUB,
    SPADE, HEART, DIAMOND, CLUB
};

#endif
