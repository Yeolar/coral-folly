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

#include <coral/Bits.h>

#include <coral/CpuId.h>
#include <coral/Portability.h>

// None of this is necessary if we're compiling for a target that supports
// popcnt, which includes MSVC
#if !defined(__POPCNT__) && !defined(_MSC_VER)
namespace {

int popcount_builtin(unsigned int x) {
  return __builtin_popcount(x);
}

int popcountll_builtin(unsigned long long x) {
  return __builtin_popcountll(x);
}

#if CORAL_HAVE_IFUNC && !defined(CORAL_SANITIZE_ADDRESS)

// Strictly speaking, these versions of popcount are usable without ifunc
// support. However, we would have to check, via CpuId, if the processor
// implements the popcnt instruction first, which is what we use ifunc for.
int popcount_inst(unsigned int x) {
  int n;
  asm ("popcntl %1, %0" : "=r" (n) : "r" (x));
  return n;
}

int popcountll_inst(unsigned long long x) {
  unsigned long long n;
  asm ("popcntq %1, %0" : "=r" (n) : "r" (x));
  return n;
}

typedef decltype(popcount_builtin) Type_popcount;
typedef decltype(popcountll_builtin) Type_popcountll;

// This function is called on startup to resolve coral::detail::popcount
extern "C" Type_popcount* coral_popcount_ifunc() {
  return coral::CpuId().popcnt() ?  popcount_inst : popcount_builtin;
}

// This function is called on startup to resolve coral::detail::popcountll
extern "C" Type_popcountll* coral_popcountll_ifunc() {
  return coral::CpuId().popcnt() ?  popcountll_inst : popcountll_builtin;
}

#endif  // CORAL_HAVE_IFUNC && !defined(CORAL_SANITIZE_ADDRESS)

}  // namespace

namespace coral {
namespace detail {

// Call coral_popcount_ifunc on startup to resolve to either popcount_inst
// or popcount_builtin
int popcount(unsigned int x)
#if CORAL_HAVE_IFUNC && !defined(CORAL_SANITIZE_ADDRESS)
  __attribute__((__ifunc__("coral_popcount_ifunc")));
#else
{  return popcount_builtin(x); }
#endif

// Call coral_popcount_ifunc on startup to resolve to either popcountll_inst
// or popcountll_builtin
int popcountll(unsigned long long x)
#if CORAL_HAVE_IFUNC && !defined(CORAL_SANITIZE_ADDRESS)
  __attribute__((__ifunc__("coral_popcountll_ifunc")));
#else
{  return popcountll_builtin(x); }
#endif

}  // namespace detail
}  // namespace coral

#endif  /* !__POPCNT__ */
