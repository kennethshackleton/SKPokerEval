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

template <class T>
inline void doNotOptimiseAway(T&& datum) {
  asm volatile("" : "+r"(datum));
}

class Profiler {
public:
  static double RandomAccessProfile(unsigned const count) {
    std::default_random_engine gen;
    std::uniform_int_distribution<uint8_t> dist(0, 51);
    int const length = 28*count;
    uint8_t * const buffer = (uint8_t *) malloc(length * sizeof(uint8_t));
    for (int i = 0; i < length; i += 7) {
      uint8_t j = 0;
      while (j < 7) {
        uint8_t const r = dist(gen);
        bool accept = true;
        for (uint8_t k = 0; k < j; ++k) {
          if (buffer[i+k] == r) {
            accept = false;
            break;
          }
        }
        if (accept) buffer[i+(j++)] = r;
      }
    }
    auto const start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < length; i += 28) {
      doNotOptimiseAway(
        SevenEval::GetRank(buffer[i+21], buffer[i+22], buffer[i+23],
          buffer[i+24], buffer[i+25], buffer[i+26], buffer[i+27])
      );
      doNotOptimiseAway(
        SevenEval::GetRank(buffer[i], buffer[i+1], buffer[i+2],
          buffer[i+3], buffer[i+4], buffer[i+5], buffer[i+6])
      );
      doNotOptimiseAway(
        SevenEval::GetRank(buffer[i+7], buffer[i+8], buffer[i+9],
          buffer[i+10], buffer[i+11], buffer[i+12], buffer[i+13])
      );
      doNotOptimiseAway(
        SevenEval::GetRank(buffer[i+14], buffer[i+15], buffer[i+16],
          buffer[i+17], buffer[i+18], buffer[i+19], buffer[i+20])
      );
    }
    auto const end = std::chrono::high_resolution_clock::now();
    delete buffer;
    return 1e-6 * std::chrono::duration_cast<std::chrono::nanoseconds>(
      end - start).count();
  }
};

int main() {
  std::cout << "Profiling SevenEval random access..." << std::endl;
  long const numberOfHands = 50000000L;
  auto fastest = std::numeric_limits<double>::max();
  for (int i = 0; i < 20; ++i) {
    auto const profile = Profiler::RandomAccessProfile(numberOfHands / 4);
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
