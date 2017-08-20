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
#include <chrono>
#include <limits>
#include <vector>
#include <array>

template <class T>
inline void doNotOptimiseAway(T&& datum) {
  asm volatile("" : "+r"(datum));
}

class Profiler {
public:
  static double RandomAccessProfile(unsigned const count) {
    std::default_random_engine gen;
    std::uniform_int_distribution<int> dist(0, 51);
    std::vector<std::array<unsigned, 7>> buffer(count);
    std::array<unsigned, 7> hand;
    for (int i = 0; i < count; ++i) {
      int j = 0;
      while (j < 7) {
        unsigned const r = dist(gen);
        bool accept = true;
        for (int k = 0; k < j; ++k) {
          if (hand[k] == r) {
            accept = false;
            break;
          }
        }
        if (accept) hand[j++] = r;
      }
      buffer.push_back(hand);
    }
    auto const start = std::chrono::high_resolution_clock::now();
    for (auto const hand : buffer) {
      doNotOptimiseAway(
        SevenEval::GetRank(hand[0], hand[1], hand[2], hand[3], hand[4],
          hand[5], hand[6])
      );
    }
    auto const end = std::chrono::high_resolution_clock::now();
    return 1e-6 * std::chrono::duration_cast<std::chrono::nanoseconds>(
      end - start).count();
  }
};

int main() {
  std::cout << "Profiling SevenEval random access..." << std::endl;
  long const numberOfHands = 50000000L;
  auto fastest = std::numeric_limits<double>::max();
  for (int i = 0; i < 20; ++i) {
    auto const profile = Profiler::RandomAccessProfile(numberOfHands);
    fastest = std::min(fastest, profile);
    std::cout << i << ": " << profile << " ms" << std::endl;
  }
  std::cout << "Best random access time: " << fastest << " ms" << std::endl;
  if (fastest > 0.0) {
      double const handsPerSecond = (numberOfHands * 1000L) / fastest;
      std::cout << "Best random access rate: " << handsPerSecond
        << " hands/sec" << std::endl;
  }
}

#endif
