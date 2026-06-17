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

TEST_F(SevenEvalTest, BatchMatchesScalarFlushImpossible) {
  uint8_t const c0 = 0;
  uint8_t const c1 = 5;
  uint8_t const c2 = 10;
  uint8_t const c3 = 15;
  uint8_t const c4 = 16;
  uint8_t const c5 = 21;
  uint8_t const c6_start = 22;
  uint8_t const c6_end = 31;

  std::vector<uint16_t> batch_ranks;
  SevenEval::GetRankBatch(
      c0, c1, c2, c3, c4, c5, c6_start, c6_end,
      [&batch_ranks](size_t idx, uint16_t rank) {
        if (batch_ranks.size() <= idx) {
          batch_ranks.resize(idx + 1);
        }
        batch_ranks[idx] = rank;
      });

  ASSERT_EQ(static_cast<size_t>(c6_end - c6_start), batch_ranks.size());
  for (uint8_t q = c6_start; q < c6_end; ++q) {
    auto const expected = SevenEval::GetRank(c0, c1, c2, c3, c4, c5, q);
    ASSERT_EQ(expected, batch_ranks[q - c6_start]) << "Mismatch at q=" << static_cast<int>(q);
  }
}

TEST_F(SevenEvalTest, BatchMatchesScalarFlushPossible) {
  uint8_t const c0 = 0;
  uint8_t const c1 = 4;
  uint8_t const c2 = 8;
  uint8_t const c3 = 12;
  uint8_t const c4 = 17;
  uint8_t const c5 = 22;
  uint8_t const c6_start = 24;
  uint8_t const c6_end = 33;

  std::vector<uint16_t> batch_ranks;
  SevenEval::GetRankBatch(
      c0, c1, c2, c3, c4, c5, c6_start, c6_end,
      [&batch_ranks](size_t idx, uint16_t rank) {
        if (batch_ranks.size() <= idx) {
          batch_ranks.resize(idx + 1);
        }
        batch_ranks[idx] = rank;
      });

  ASSERT_EQ(static_cast<size_t>(c6_end - c6_start), batch_ranks.size());
  for (uint8_t q = c6_start; q < c6_end; ++q) {
    auto const expected = SevenEval::GetRank(c0, c1, c2, c3, c4, c5, q);
    ASSERT_EQ(expected, batch_ranks[q - c6_start])
        << "Mismatch at q=" << static_cast<int>(q);
  }
}

TEST_F(SevenEvalTest, BatchMatchesScalarExhaustive) {
  std::atomic<long> count(0);
  auto const outer = [&](int const& c0) {
    long inner_count = 0;
    for (int c1 = c0 + 1; c1 < 47; ++c1) {
      for (int c2 = c1 + 1; c2 < 48; ++c2) {
        for (int c3 = c2 + 1; c3 < 49; ++c3) {
          for (int c4 = c3 + 1; c4 < 50; ++c4) {
            for (int c5 = c4 + 1; c5 < 51; ++c5) {
              auto const c6_start = static_cast<uint8_t>(c5 + 1);
              auto const c6_end = static_cast<uint8_t>(52);

              uint16_t batch_ranks[52] = {};
              size_t produced = 0;
              SevenEval::GetRankBatch(
                  static_cast<uint8_t>(c0), static_cast<uint8_t>(c1),
                  static_cast<uint8_t>(c2), static_cast<uint8_t>(c3),
                  static_cast<uint8_t>(c4), static_cast<uint8_t>(c5),
                  c6_start, c6_end,
                  [&batch_ranks, &produced](size_t idx, uint16_t rank) {
                    batch_ranks[idx] = rank;
                    ++produced;
                  });

              auto const expected_count = static_cast<size_t>(c6_end - c6_start);
              ASSERT_EQ(expected_count, produced)
                  << "Invalid batch output count at "
                  << c0 << ", " << c1 << ", " << c2 << ", "
                  << c3 << ", " << c4 << ", " << c5;

              for (uint8_t c6 = c6_start; c6 < c6_end; ++c6) {
                auto const expected = SevenEval::GetRank(
                    static_cast<uint8_t>(c0), static_cast<uint8_t>(c1),
                    static_cast<uint8_t>(c2), static_cast<uint8_t>(c3),
                    static_cast<uint8_t>(c4), static_cast<uint8_t>(c5), c6);
                ASSERT_EQ(expected, batch_ranks[c6 - c6_start])
                    << "Mismatch at "
                    << c0 << ", " << c1 << ", " << c2 << ", "
                    << c3 << ", " << c4 << ", " << c5 << ", "
                    << static_cast<int>(c6);
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
  ASSERT_EQ(133784560, count) << "Invalid number of seven card hands tested in batch exhaustive test.";
}
