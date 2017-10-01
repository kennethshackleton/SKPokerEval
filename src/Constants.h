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

#ifndef SKPOKEREVAL_CONSTANTS_H_
#define SKPOKEREVAL_CONSTANTS_H_

#define DECK_SIZE 52

#define NUMBER_OF_SUITS 4
#define NUMBER_OF_FACES 13

#define SPADE 0
#define HEART 1
#define DIAMOND 8
#define CLUB 57

#define INDEX_SPADE 0
#define INDEX_HEART 1
#define INDEX_DIAMOND 2
#define INDEX_CLUB 3
#define NOT_A_SUIT (-1)

#define TWO_FIVE 0
#define THREE_FIVE 1
#define FOUR_FIVE 5
#define FIVE_FIVE 22
#define SIX_FIVE 94
#define SEVEN_FIVE 312
#define EIGHT_FIVE 992
#define NINE_FIVE 2422
#define TEN_FIVE 5624
#define JACK_FIVE 12522
#define QUEEN_FIVE 19998
#define KING_FIVE 43258
#define ACE_FIVE 79415

#define TWO_FLUSH 1
#define THREE_FLUSH TWO_FLUSH<<1
#define FOUR_FLUSH THREE_FLUSH<<1
#define FIVE_FLUSH FOUR_FLUSH<<1
#define SIX_FLUSH FIVE_FLUSH<<1
#define SEVEN_FLUSH SIX_FLUSH<<1
#define EIGHT_FLUSH SEVEN_FLUSH<<1
#define NINE_FLUSH EIGHT_FLUSH<<1
#define TEN_FLUSH NINE_FLUSH<<1
#define JACK_FLUSH TEN_FLUSH<<1
#define QUEEN_FLUSH JACK_FLUSH<<1
#define KING_FLUSH QUEEN_FLUSH<<1
#define ACE_FLUSH KING_FLUSH<<1

// _SEVEN tag suppressed
#define TWO 0
#define THREE 1
#define FOUR 5
#define FIVE 22
#define SIX 98
#define SEVEN 453
#define EIGHT 2031
#define NINE 8698
#define TEN 22854
#define JACK 83661
#define QUEEN 262349
#define KING 636345
#define ACE 1479181
// end of _SEVEN tag suppressed

#define MAX_FIVE_NONFLUSH_KEY_INT ((4*ACE_FIVE)+KING_FIVE)
#define MAX_FIVE_FLUSH_KEY_INT (ACE_FLUSH|KING_FLUSH|QUEEN_FLUSH|JACK_FLUSH|\
                                TEN_FLUSH)
#define MAX_SEVEN_FLUSH_KEY_INT (ACE_FLUSH|KING_FLUSH|QUEEN_FLUSH|JACK_FLUSH|\
                                 TEN_FLUSH|NINE_FLUSH|EIGHT_FLUSH)

#define RANK_OFFSET_SHIFT 9
#define RANK_HASH_MOD ((1<<RANK_OFFSET_SHIFT) - 1)

#define MAX_FLUSH_CHECK_SUM (7*CLUB)

// Bit masks
#define FLUSH_BIT_SHIFT 23
#define FACE_BIT_MASK ((1 << FLUSH_BIT_SHIFT) - 1)

#endif // SKPOKEREVAL_CONSTANTS_H_
