### Change Log

#### 1.9

* Reduce memory footprint by 40%, to under 140kB.

#### 1.8.1

* Simplify flush rank key and look-up.
* Accept smaller non-flush rank offset table.
* Observed performance gains of about 6%.

#### 1.8

* Index cards by bytes.
* Observed performance gains of about 23%.

#### 1.7.1

* Reduce rank hash table by 9%.
* Slight performance gains.

#### 1.7.0

* Reduce required memory footprint by 40%.
* Slight performance gains.

#### 1.6.0

* Optimise ranking flushes by removing branching in this case.

#### 1.5.0

* Static interface.
* Include scripts for key generation and hashing.
* Include profiler.

#### 1.4.2

* CHM perfect hash of seven rank keys, optimisation that also reduces footprint.

#### 1.4.1

* Minor optimisation when ranking flushes.

#### 1.4

* Optimisation when ranking flushes.
* Optimisation for initialising seven-card non-flush ranks.
* Extended test coverage.

#### 1.3

* Complete renaming to "SKPokerEval".
* Add version number.
* Minor corrections.

#### 1.2

* Rename to SKPokerEval.

#### 1.1

* Travis CI.

#### 1.0

* Settle on publishing only in C++.
