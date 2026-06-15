#include <gtest/gtest.h>
#include <climits>
#include <atomic>
#include "Parallel.h"
#include "FiveEval.h"

class FiveEvalTest : public ::testing::Test {
protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
  FiveEval const eval;
};

TEST_F(FiveEvalTest, HighCard) {
  long rank = LONG_MIN;
  for (int i = 5; i < 13; ++i) {
    int const I = ((12-i)<<2) + 1; // No flushes.
    for (int j = 3; j < i; ++j) {
      int const J = (12-j)<<2;
      for (int k = 2; k < j; ++k) {
        int const K = (12-k)<<2;
        for (int l = 1; l < k; ++l) {
          int const L = (12-l)<<2;
          for (int m = 0; m < l; ++m) {
            if (i-m == 4 || (i == 12 && j == 3)) {
              continue;
            }
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

TEST_F(FiveEvalTest, WorstPairBeatsBestHighCard) {
  // Ace through to Jack inclusive, plus a Nine. No flush.
  int const rank_of_best_high_card = eval.GetRank(0, 4, 8, 12, 21);
  // Pair of Twos with Five high.
  int const rank_of_worst_pair = eval.GetRank(36, 40, 44, 48, 49);
  ASSERT_LT(rank_of_best_high_card, rank_of_worst_pair);
}

TEST_F(FiveEvalTest, Pair) {
  long rank = LONG_MIN;
  for (int i = 0; i < 13; ++i) {
    int const I = (12-i)<<2;
    for (int j = 2; j < 13; ++j) {
      if (j == i) {
        continue;
      }
      int const J = (12-j)<<2;
      for (int k = 1; k < j; ++k) {
        if (k == i) {
          continue;
        }
        int const K = (12-k)<<2;
        for (int l = 0; l < k; ++l) {
          if (l == i) {
            continue;
          }
          int const L = (12-l)<<2;
          int const next_rank = eval.GetRank(I, I+1, J, K, L);
          ASSERT_GT(next_rank, rank) << "Rank " << next_rank << " at "
                                     << I << ", "
                                     << I+1 << ", "
                                     << J << ", "
                                     << K << ", "
                                     << L << " is invalid.";
          rank = next_rank;
        }
      }
    }
  }
}

TEST_F(FiveEvalTest, WorstTwoPairBeatsBestPair) {
  // Pair of Aces with King, Queen and Jack.
  int const rank_of_best_pair = eval.GetRank(0, 1, 4, 8, 12);
  // Pair of Threes, Pair of Twos, Four high.
  int const rank_of_worst_two_pair = eval.GetRank(44, 45, 48, 49, 40);
  ASSERT_LT(rank_of_best_pair, rank_of_worst_two_pair);
}

TEST_F(FiveEvalTest, TwoPair) {
  long rank = LONG_MIN;
  for (int i = 1; i < 13; ++i) {
    int const I = (12-i)<<2;
    for (int j = 0; j < i; ++j) {
      int const J = (12-j)<<2;
      for (int k = 0; k < 13; ++k) {
        if (k == i || k == j) {
          continue;
        }
        int const K = (12-k)<<2;
        int const next_rank = eval.GetRank(I, I+1, J, J+1, K);
        ASSERT_GT(next_rank, rank) << "Rank " << next_rank << " at "
                                   << I << ", "
                                   << I+1 << ", "
                                   << J << ", "
                                   << J+1 << ", "
                                   << K << " is invalid.";
        rank = next_rank;
      }
    }
  }
}

TEST_F(FiveEvalTest, WorstTripleBeatsBestTwoPair) {
  // Pair of Aces, Pair of Kings, Queen high.
  int const rank_of_best_two_pair = eval.GetRank(0, 1, 4, 5, 8);
  // Triple Twos, Four high.
  int const rank_of_worst_triple = eval.GetRank(48, 49, 50, 40, 44);
  ASSERT_LT(rank_of_best_two_pair, rank_of_worst_triple);
}

TEST_F(FiveEvalTest, Triple) {
  long rank = LONG_MIN;
  for (int i = 0; i < 13; ++i) {
    int const I = (12-i)<<2;
    for (int j = 1; j < 13; ++j) {
      if (j == i) {
        continue;
      }
      int const J = (12-j)<<2;
      for (int k = 0; k < j; ++k) {
        if (k == i) {
          continue;
        }
        int const K = (12-k)<<2;
        int const next_rank = eval.GetRank(I, I+1, I+2, J, K);
        ASSERT_GT(next_rank, rank) << "Rank " << next_rank << " at "
                                   << I << ", "
                                   << I+1 << ", "
                                   << I+2 << ", "
                                   << J << ", "
                                   << K << " is invalid.";
        rank = next_rank;
      }
    }
  }
}

TEST_F(FiveEvalTest, WorstStraightBeatsBestTriple) {
  // Triple Aces, King, Queen.
  int const rank_of_best_triple = eval.GetRank(0, 1, 2, 4, 8);
  // Straight Ace low.
  int const rank_of_worst_straight = eval.GetRank(0, 48, 45, 42, 39);
  ASSERT_LT(rank_of_best_triple, rank_of_worst_straight);
}

TEST_F(FiveEvalTest, Straight) {
  int rank = eval.GetRank(37, 40, 44, 48, 0); // Straight, Ace low.
  for (int i = 0; i < 9; ++i) {
    int const I = (12-i)<<2;
    int const next_rank = eval.GetRank(I-15, I-12, I-8, I-4, I);
    ASSERT_GT(next_rank, rank) << "Rank " << next_rank << " at "
                               << I-15 << ", "
                               << I-12 << ", "
                               << I-8 << ", "
                               << I-4 << ", "
                               << I << " is invalid.";
    rank = next_rank;
  }
}

TEST_F(FiveEvalTest, WorstFlushBeatsBestStraight) {
  // Straight Ace high.
  int const rank_of_best_straight = eval.GetRank(0, 4, 8, 12, 17);
  // Seven, Five, Four, Three, Two.
  int const rank_of_worst_flush = eval.GetRank(28, 36, 40, 44, 48);
  ASSERT_LT(rank_of_best_straight, rank_of_worst_flush);
}

TEST_F(FiveEvalTest, Flush) {
  long rank = LONG_MIN;
  for (int i = 5; i < 13; ++i) {
    int const I = (12-i)<<2;
    for (int j = 3; j < i; ++j) {
      int const J = (12-j)<<2;
      for (int k = 2; k < j; ++k) {
        int const K = (12-k)<<2;
        for (int l = 1; l < k; ++l) {
          int const L = (12-l)<<2;
          for (int m = 0; m < l; ++m) {
            if (i-m == 4 || (i == 12 && j == 3)) {
              continue;
            }
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

TEST_F(FiveEvalTest, WorstFullHouseBeatsBestFlush) {
  // Ace, King, Queen, Jack, Nine
  int const rank_of_best_flush = eval.GetRank(0, 4, 8, 12, 20);
  // Triple Twos, Pair Threes.
  int const rank_of_worst_full_house = eval.GetRank(48, 49, 50, 44, 45);
  ASSERT_LT(rank_of_best_flush, rank_of_worst_full_house);
}

TEST_F(FiveEvalTest, FullHouse) {
  long rank = LONG_MIN;
  for (int i = 0; i < 13; ++i) {
    int const I = (12-i)<<2;
    for (int j = 0; j < 13; ++j) {
      if (j == i) {
        continue;
      }
      int const J = (12-j)<<2;
      int const next_rank = eval.GetRank(I, I+1, I+2, J, J+1);
      ASSERT_GT(next_rank, rank) << "Rank " << next_rank << " at "
                                 << I << ", "
                                 << I+1 << ", "
                                 << I+2 << ", "
                                 << J << ", "
                                 << J+1 << " is invalid.";
      rank = next_rank;
    }
  }
}

TEST_F(FiveEvalTest, WorstQuadBeatsBestFullHouse) {
  // Triple Aces, Pair Kings.
  int const rank_of_best_full_house = eval.GetRank(0, 1, 2, 4, 5);
  // Quad Twos, Three high.
  int const rank_of_worst_quad = eval.GetRank(48, 49, 50, 51, 44);
  ASSERT_LT(rank_of_best_full_house, rank_of_worst_quad);
}

TEST_F(FiveEvalTest, Quad) {
  long rank = LONG_MIN;
  for (int i = 0; i < 13; ++i) {
    int const I = (12-i)<<2;
    for (int j = 0; j < 13; ++j) {
      if (j == i) {
        continue;
      }
      int const J = (12-j)<<2;
      int const next_rank = eval.GetRank(I, I+1, I+2, I+3, J);
      ASSERT_GT(next_rank, rank) << "Rank " << next_rank << " at "
                                 << I << ", "
                                 << I+1 << ", "
                                 << I+2 << ", "
                                 << I+3 << ", "
                                 << J << " is invalid.";
      rank = next_rank;
    }
  }
}

TEST_F(FiveEvalTest, WorstStraightFlushBeatsBestQuad) {
  // Quad Aces, King high.
  int const rank_of_best_quad = eval.GetRank(0, 1, 2, 3, 4);
  // Straight flush Ace low.
  int const rank_of_worst_straight_flush = eval.GetRank(36, 40, 44, 48, 0);
  ASSERT_LT(rank_of_best_quad, rank_of_worst_straight_flush);
}

TEST_F(FiveEvalTest, StraightFlush) {
  int rank = eval.GetRank(36, 40, 44, 48, 0); // Straight flush, Ace low.
  for (int i = 0; i < 9; ++i) {
    int const I = (12-i)<<2;
    int const next_rank = eval.GetRank(I-16, I-12, I-8, I-4, I);
    ASSERT_GT(next_rank, rank) << "Rank " << next_rank << " at "
                               << I-16 << ", "
                               << I-12 << ", "
                               << I-8 << ", "
                               << I-4 << ", "
                               << I << " is invalid.";
    rank = next_rank;
  }
}

TEST_F(FiveEvalTest, SevenCardHand) {
  std::atomic<long> count(0);
  auto const outer = [&](int const& i) {
    long inner_count = 0;
    uint8_t five[5];
    uint8_t seven[7];
    seven[0] = i;
    for (int j = 5; j < i; ++j) {
      seven[1] = j;
      for (int k = 4; k < j; ++k) {
        seven[2] = k;
        for (int l = 3; l < k; ++l) {
          seven[3] = l;
          for (int m = 2; m < l; ++m) {
            seven[4] = m;
            for (int n = 1; n < m; ++n) {
              seven[5] = n;
              for (int p = 0; p < n; ++p) {
                seven[6] = p;
                int rbf = 0;
                uint8_t v = 0;
                for (uint8_t x = 1; x < 7; ++x) {
                  for (uint8_t y = 0; y < x; ++y) {
                    v = 0;
                    for (uint8_t z = 0; z < 7; ++z) {
                      if (z != x && z != y) {
                        five[v++] = seven[z];
                      }
                    }
                    int const rank = eval.GetRank(five[0], five[1],
                        five[2], five[3], five[4]);
                    if (rbf < rank) {
                      rbf = rank;
                    }
                  }
                }
                auto const rbs = eval.GetRank(i, j, k, l, m, n, p);
                ASSERT_EQ(rbf, rbs) << "Rank " << rbs << " at "
                                    << i << ", "
                                    << j << ", "
                                    << k << ", "
                                    << l << ", "
                                    << m << ", "
                                    << n << ", "
                                    << p << " is invalid.";
                ++inner_count;
              }
            }
          }
        }
      }
    }
    count += inner_count;
  };
  ParallelFor(6, 52, outer);
  ASSERT_EQ(133784560, count) << "Invalid number of seven card hands tested.";
}
