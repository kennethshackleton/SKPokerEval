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

#ifndef SKPOKEREVAL_PARALLEL_H
#define SKPOKEREVAL_PARALLEL_H

#include <vector>
#include <thread>
#include <future>
#include <atomic>

template <class F>
void ParallelFor(int const begin, int const end, F const fn) {
  std::atomic<int> idx(0);
  idx = begin;
  int const num_cpus = std::thread::hardware_concurrency();
  std::vector<std::future<void>> futures(num_cpus);
  for (int cpu = 0; cpu < num_cpus; ++cpu) {
    futures[cpu] = std::async(
      std::launch::async,
      [&idx, end, &fn]() {
        while (true) {
          int const i = idx++;
          if (i >= end) {
            break;
          }
          fn(i);
        }
      }
    );
  }
  for (int cpu = 0; cpu < num_cpus; ++cpu) {
    futures[cpu].get();
  }
}

#endif
