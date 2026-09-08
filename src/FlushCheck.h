// SKPokerEval
//
// Copyright 2010 Kenneth J. Shackleton
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SKPOKEREVAL_FLUSHCHECK_H
#define SKPOKEREVAL_FLUSHCHECK_H

#include <cstdint>

int8_t const flush_check_data[256] = {
  0, 0, 0, -1, -1, 1, 1, 1, 0, 0, -1, -1, -1, 1, 1, -1, 0, -1, -1, -1,
  -1, 1, -1, -1, -1, -1, -1, -1, -1, 3, 3, 3, -1, -1, -1, -1, -1, 3, 3, -1,
  2, 2, 2, -1, -1, 3, -1, -1, 2, 2, -1, -1, -1, -1, -1, -1, 2, 0, 0, -1,
  -1, -1, 1, 1, -1, 0, -1, -1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, 3, 3, -1, -1, -1, -1, -1, -1, 3, -1, -1, 2, 2, -1,
  -1, -1, -1, -1, -1, 2, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, 1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 2, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

class FlushCheck final {
public:
  inline int8_t operator[](uint16_t key) const {
    return flush_check_data[static_cast<uint8_t>(key)];
  }
};

FlushCheck const flush_check{};

#endif // SKPOKEREVAL_FLUSHCHECK_H
