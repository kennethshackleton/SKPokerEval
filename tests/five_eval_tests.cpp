#include <gtest/gtest.h>
#include <climits>
#include "../src/FiveEval.h"

TEST(FiveEval, HighCard) {
  FiveEval const eval;
  long rank = LONG_MIN;
  for (int i = 5; i <= 12; ++i) {
    int const I = ((12-i)<<2) + 1; // No flushes.
    for (int j = 3; j < i; ++j) {
      int const J = (12-j)<<2;
      for (int k = 2; k < j; ++k) {
        int const K = (12-k)<<2;
        for (int l = 1; l < k; ++l) {
          int const L = (12-l)<<2;
          for (int m = 0; m < l &&
              !(i-m==4 || (i==12 && j==3)); ++m) {
            int const M = (12-m)<<2;
            int const next_rank = eval.GetRank(I, J, K, L, M);
            ASSERT_GT(next_rank, rank) << "Rank " << next_rank << " at "
                                       << I << ", "
                                       << J << ", "
                                       << K << ", "
                                       << L << ", "
                                       << M << " is invalid.";
            rank = next_rank;
          }
        }
      }
    }
  }
}

TEST(FiveEval, WorstPairBeatsBestHighCard) {
  FiveEval const eval;
  int const rank_of_best_high_card = eval.GetRank(0, 4, 8, 12, 21);
  int const rank_of_worst_pair = eval.GetRank(36, 40, 44, 48, 49);
  ASSERT_LT(rank_of_best_high_card, rank_of_worst_pair);
}

