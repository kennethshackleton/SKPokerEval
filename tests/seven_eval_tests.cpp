#include <gtest/gtest.h>
#include <atomic>
#include "Parallel.h"
#include "../src/FiveEval.h"
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
