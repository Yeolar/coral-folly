/*
 * Copyright 2015 Yeolar
 * Copyright 2015 Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <folly/ext/String.h>

namespace folly {

LineEnding estimateLineEnding(StringPiece sp) {
  size_t nCR = std::count(sp.begin(), sp.end(), '\r');
  size_t nLF = std::count(sp.begin(), sp.end(), '\n');

  if (nCR == 0 && nLF == 0) return LineEnding::NONE;
  if (nCR == 0)             return LineEnding::LF;
  if (nLF == 0)             return LineEnding::CR;
  if (nCR == nLF)           return LineEnding::CRLF;
  return LineEnding::MIXED;
}

}  // namespace folly

