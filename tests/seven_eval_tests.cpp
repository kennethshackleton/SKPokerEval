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

#include <gtest/gtest.h>
#include <atomic>
#include "Parallel.h"
#include "FiveEval.h"
#include "../src/SevenEval.h"

class SevenEvalTest : public ::testing::Test {
protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
  FiveEval const five_eval;
};

TEST_F(SevenEvalTest, CompareWithFiveEval) {
  std::atomic<long> count(0);
  auto const outer = [&](int const& i) {
    long inner_count = 0;
    for (int j = i+1; j < 47; ++j) {
      for (int k = j+1; k < 48; ++k) {
        for (int l = k+1; l < 49; ++l) {
          for (int m = l+1; m < 50; ++m) {
            for (int n = m+1; n < 51; ++n) {
              for (int p = n+1; p < 52; ++p) {
                auto const five_rank = five_eval.GetRank(i, j, k, l, m, n, p);
                auto const seven_rank = SevenEval::GetRank(i, j, k, l, m, n, p);
                ASSERT_EQ(five_rank, seven_rank) << "Ranks at "
                                                 << i << ", "
                                                 << j << ", "
                                                 << k << ", "
                                                 << l << ", "
                                                 << m << ", "
                                                 << n << ", "
                                                 << p << " after "
                                                 << inner_count
                                                 << " differ.";
                ++inner_count;
              }
            }
          }
        }
      }
    }
    count += inner_count;
  };
  ParallelFor(0, 46, outer);
  ASSERT_EQ(133784560, count) << "Invalid number of seven card hands tested.";
}
