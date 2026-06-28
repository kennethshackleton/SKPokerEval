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
