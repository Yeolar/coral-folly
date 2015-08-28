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

#ifndef CORAL_DETAIL_FUNCTIONAL_EXCEPT_H
#define CORAL_DETAIL_FUNCTIONAL_EXCEPT_H

#include <coral/Portability.h>

#if !CORAL_HAVE_BITS_FUNCTEXCEPT_H

CORAL_NAMESPACE_STD_BEGIN

CORAL_NORETURN void __throw_length_error(const char* msg);
CORAL_NORETURN void __throw_logic_error(const char* msg);
CORAL_NORETURN void __throw_out_of_range(const char* msg);

#ifdef _MSC_VER
CORAL_NORETURN void __throw_bad_alloc();
#endif

CORAL_NAMESPACE_STD_END

#else
#error This file should never be included if CORAL_HAVE_BITS_FUNCTEXCEPT_H is set
#endif

#endif
