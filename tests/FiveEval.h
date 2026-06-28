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

#ifndef SKPOKEREVAL_FIVEEVAL_H
#define SKPOKEREVAL_FIVEEVAL_H

#include "../src/Constants.h"
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
