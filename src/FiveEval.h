// SpecialKEval
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

#ifndef SPECIALKEVAL_FIVEEVAL_H
#define SPECIALKEVAL_FIVEEVAL_H

#include "Constants.h"

#ifdef __cplusplus
extern "C" {
#endif
class FiveEval {
public:
  FiveEval();
  ~FiveEval();
  // Get the rank of a hand comprising five cards, each represented by an
  // integer from 0 (resp. Ace of Spades) to 51 (resp. Two of Clubs) inclusive.
  // The higher the rank the better the hand. Two hands of equal rank tie.
  short unsigned GetRank(int const, int const, int const, int const,
                         int const) const;
  // Get the rank of a hand comprising seven cards, each represented by an
  // integer from 0 (resp. Ace of Spades) to 51 (resp. Two of Clubs) inclusive.
  // The higher the rank the better the hand. Two hands of equal rank tie.
  short unsigned GetRank(int const, int const, int const, int const,
                         int const, const int, int const) const;
private:
  short unsigned *mRankPtr;
  short unsigned *mFlushRankPtr;
  int unsigned mDeckcardsFace[DECK_SIZE];
  short unsigned mDeckcardsFlush[DECK_SIZE];
  short unsigned mDeckcardsSuit[DECK_SIZE];
};
#ifdef __cplusplus
}
#endif

#endif // SPECIALKEVAL_FIVEEVAL_H
