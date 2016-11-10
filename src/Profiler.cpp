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

#ifndef SKPOKEREVAL_PROFILER_H
#define SKPOKEREVAL_PROFILER_H

#include "SevenEval.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <ctime>
#include <limits>

template <class T>
inline void doNotOptimiseAway(T&& datum) {
  asm volatile("" : "+r"(datum));
}

class Profiler {
public:
  static clock_t Profile(unsigned const count) {
    std::default_random_engine gen;
    std::uniform_int_distribution<int> dist(0, 51);
    int const length = 7*count;
    unsigned * const buffer = (unsigned *) malloc(length * sizeof(unsigned));
    for (int i = 0; i < length; i += 7) {
      int j = 0;
      while (j < 7) {
        unsigned const r = dist(gen);
        bool accept = true;
        for (int k = 0; k < j; ++k) {
          if (buffer[i+k] == r) {
            accept = false;
            break;
          }
        }
        if (accept) buffer[i+(j++)] = r;
      }
    }
    std::clock_t const start = std::clock();
    for (int i = 0; i < length; i += 7) {
      doNotOptimiseAway(
        SevenEval::GetRank(buffer[i], buffer[i+1], buffer[i+2],
          buffer[i+3], buffer[i+4], buffer[i+5], buffer[i+6])
      );
    }
    std::clock_t const end = std::clock();
    delete buffer;
    return end-start;
  }
};

int main() {
  clock_t fastest = std::numeric_limits<clock_t>::max();
  for (int i = 0; i < 20; ++i) {
    fastest = std::min(fastest, Profiler::Profile(50000000));
  }
  std::cout << 1000.0f * fastest / CLOCKS_PER_SEC << " ms" << std::endl;
}

#endif
