# SKPokerEval

A lightweight 32-bit Texas Hold'em 7-card hand evaluator written in C++.

## Travis status

[![Build Status](https://travis-ci.org/kennethshackleton/SKPokerEval.svg)](https://travis-ci.org/kennethshackleton/SKPokerEval)

## Build instructions

From the root directory of the project execute:

```
./configure && cmake . && make && ctest
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
