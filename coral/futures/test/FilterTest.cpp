/*
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

#include <gtest/gtest.h>

#include <coral/futures/Future.h>

using namespace coral;

TEST(Filter, alwaysTrye) {
  EXPECT_EQ(42, makeFuture(42).filter([](int){ return true; }).get());
}

TEST(Filter, alwaysFalse) {
  EXPECT_THROW(makeFuture(42).filter([](int){ return false; }).get(),
               coral::PredicateDoesNotObtain);
}

TEST(Filter, moveOnlyValue) {
  EXPECT_EQ(42,
    *makeFuture(coral::make_unique<int>(42))
     .filter([](std::unique_ptr<int> const&) { return true; })
     .get());
}
