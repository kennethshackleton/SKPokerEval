#include <gtest/gtest.h>
#include "../src/FiveEval.h"
#include "../src/SevenEval.h"

TEST(SevenEval, Regression) {
  FiveEval const* five_eval = new FiveEval();
  SevenEval const* seven_eval = new SevenEval();
  for (int i=0; i<46; ++i) {
    for (int j=i+1; j<47; ++j) {
      for (int k=j+1; k<48; ++k) {
        for (int l=k+1; l<49; ++l) {
          for (int m=l+1; m<50; ++m) {
            for (int n=m+1; n<51; ++n) {
              for (int p=n+1; p<52; ++p) {
                int const five_rank = five_eval->GetRank(i, j, k, l, m, n, p);
                int const seven_rank = seven_eval->GetRank(i, j, k, l, m, n, p);
                ASSERT_EQ(five_rank, seven_rank) << "Ranks at "
                                                 << i << ", "
                                                 << j << ", "
                                                 << k << ", "
                                                 << l << ", "
                                                 << m << ", "
                                                 << n << ", "
                                                 << p
                                                 << " differ.";
              }
            }
          }
        }
      }
    }
  }
}
