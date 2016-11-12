# SKPokerEval

A lightweight 32-bit Texas Hold'em 7-card hand evaluator written in C++.

## Travis status

[![Build Status](https://travis-ci.org/kennethshackleton/SKPokerEval.svg)](https://travis-ci.org/kennethshackleton/SKPokerEval)

## Example

```
#include <iostream>
#include "SevenEval.h"

int main() {
  // Get the rank of the seven-card spade flush, ace high.
  std::cout << SevenEval::GetRank(0, 4, 8, 12, 16, 20, 24) << std::endl;
  return 0;
}
```
