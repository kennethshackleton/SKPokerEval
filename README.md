# SKPokerEval

A lightweight 32-bit Texas Hold'em 7-card hand evaluator written in C++.

## Travis status

[![Build Status](https://travis-ci.org/kennethshackleton/SKPokerEval.svg)](https://travis-ci.org/kennethshackleton/SKPokerEval)

## How do I use it?

```
#include <iostream>
#include "SevenEval.h"

int main() {
  // Get the rank of the seven-card spade flush, ace high.
  std::cout << SevenEval::GetRank(0, 4, 8, 12, 16, 20, 24) << std::endl;
  return 0;
}
```

## Why does it work?

We exploit a key-scheme that gives us just enough uniqueness to correctly identify the integral rank of any 7-card hand, where the greater this rank is the better the hand we hold and two hands of the same rank always draw. Typically we require six additions and a memory footprint just shy of 400kB.

To start with we computed by brute force the first thirteen non-negative integers such that the sum of exactly seven with each taken at most four times is unique among all such sums: 0, 1, 5, 22, 98, 453, 2031, 8698, 22854, 83661, 262349, 636345 and 1479181. A valid sum might be 0+0+1+1+1+1+5 = 9 or 0+98+98+453+98+98+1 = 846, but invalid sum expressions include 0+262349+0+0+0+1 (too few summands), 1+1+5+22+98+453+2031+8698 (too many summands), 0+1+5+22+98+453+2031+8698 (again too many summands, although 1+5+22+98+453+2031+8698 is a legitimate expression) and 1+1+1+1+1+98+98 (too many 1's). We assign these integers as the card face values and add these together to generate a key for any non-flush 7-card hand. The largest non-flush key we see is 7825759, corresponding to any of the four quad-of-aces-full-of-kings.

Similarly, we assign the integer values 0, 1, 8 and 57 for spade, heart, diamond and club respectively. Any sum of exactly seven values taken from {0, 1, 8, 57} is unique among all such sums. We add up the suits of a 7-card hand to produce a "flush check" key and use this to find a pre-calculated flush suit value (in the case we're looking at a flush) or otherwise a defined non-flush constant. The largest flush key we see is 7999, corresponding to any of the four 7-card straight flushes with ace high.

The extraordinarily lucky aspect of this is that the maximum non-flush key we have, 7825759, is a 23-bit integer (note 2^23 = 8388608) and the largest suit key we find, 57*7 = 399, is a 9-bit integer (note 2^9 = 512). If we bit-shift a card's non-flush face value and add to this its flush check to make a card key in advance, when we aggregate the resulting card keys over a given 7-card hand we generate a 23+9 = 32-bit integer key for the whole hand. This integer key can only just be accommodated on a 32-bit machine and yet still carries enough information to decide if we're looking at a flush and if not to then look up the rank of the hand.

## How might I profile my contribution?

The project contains a [profiler](src/Profiler.cpp) which might be used to help benchmark your changes.

```
g++ -c -std=c++11 -O3 Profiler.cpp
g++ -o profile Profiler.o
./profile
```

Crudely, the lower the result the more efficiently the ranks were computed. This starts to be convincing with consistent gains of, say, 30% or better.
