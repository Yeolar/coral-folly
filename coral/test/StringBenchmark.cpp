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

#include <coral/Benchmark.h>
#include <coral/String.h>

BENCHMARK(libc_tolower, iters) {
  static const size_t kSize = 256;
  // This array is static to keep the compiler from optimizing the
  // entire function down to a no-op if it has an inlined impl of
  // tolower and thus is able to tell that there are no side-effects.
  // No side-effects + no writes to anything other than local variables
  // + no return value = no need to run any of the code in the function.
  // gcc, for example, makes that optimization with -O2.
  static char input[kSize];
  for (size_t i = 0; i < kSize; i++) {
    input[i] = (char)(i & 0xff);
  }
  for (auto i = iters; i > 0; i--) {
    for (size_t offset = 0; offset < kSize; offset++) {
      input[offset] = tolower(input[offset]);
    }
  }
}

BENCHMARK(coral_toLowerAscii, iters) {
  static const size_t kSize = 256;
  static char input[kSize];
  for (size_t i = 0; i < kSize; i++) {
    input[i] = (char)(i & 0xff);
  }
  for (auto i = iters; i > 0; i--) {
    coral::toLowerAscii(input, kSize);
  }
}

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  coral::runBenchmarks();
  if (FLAGS_benchmark) {
    coral::runBenchmarks();
  }
  return 0;
}

/*
Results on x86_64:
============================================================================
coral/test/StringBenchmark.cpp                  relative  time/iter  iters/s
============================================================================
libc_tolower                                                 1.30us  767.50K
coral_toLowerAscii                                         115.21ns    8.68M
============================================================================
*/
