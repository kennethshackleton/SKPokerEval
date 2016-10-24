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

#ifndef SKPOKEREVAL_FIVEEVAL_H
#define SKPOKEREVAL_FIVEEVAL_H

#include "Constants.h"
#include <cstdint>

class FiveEval {
public:
  FiveEval();
  ~FiveEval();
  // Get the rank of a hand comprising five cards, each represented by an
  // integer from 0 (resp. Ace of Spades) to 51 (resp. Two of Clubs) inclusive.
  // The higher the rank the better the hand. Two hands of equal rank tie.
  uint16_t GetRank(int, int, int, int, int) const;
  // Get the rank of a hand comprising seven cards, each represented by an
  // integer from 0 (resp. Ace of Spades) to 51 (resp. Two of Clubs) inclusive.
  // The higher the rank the better the hand. Two hands of equal rank tie.
  uint16_t GetRank(int, int, int, int, int, int, int) const;
private:
  uint16_t *mRankPtr;
  uint16_t *mFlushRankPtr;
  uint32_t mDeckcardsFace[DECK_SIZE];
  uint16_t mDeckcardsFlush[DECK_SIZE];
  uint16_t mDeckcardsSuit[DECK_SIZE];
};

#endif // SKPOKEREVAL_FIVEEVAL_H
