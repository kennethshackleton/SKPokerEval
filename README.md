# SpecialKEval

SpecialKEval is a lightweight Texas Hold'em 5- and 7-card hand evaluator 
written in C++.

## Travis status

![Build Status](https://travis-ci.org/kennethshackleton/SpecialKEval.svg)

## Build instructions

From the root directory of the project execute:

```
./configure && make && ctest && rm eval_tests
```

This will build the library and run unit tests.

## Example

```
#include <iostream>
#include "SevenEval.h"

int main() {
  SevenEval const eval;
  // Get the rank of the seven-card spade flush, ace high.
  std::cout << eval.GetRank(0, 4, 8, 12, 16, 20, 24) << std::endl;
  return 0;
}
```
