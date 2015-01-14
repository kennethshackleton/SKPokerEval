# SpecialKEval

SpecialKEval is a lightweight Texas Hold'em 5- and 7-card hand evaluator 
written in C++.

## Example

```
#include <iostream>
#include "SevenEval.h"

int main() {
  SevenEval const* eval = new SevenEval();
  // Get the rank of the seven-card spade flush, ace high.
  std::cout << eval->GetRank(0, 4, 8, 12, 16, 20, 24) << std::endl;
  delete eval;
  return 0;
}
```
