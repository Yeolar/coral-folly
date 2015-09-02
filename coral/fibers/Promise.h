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
#pragma once

#include <coral\/fibers/traits.h>
#include <coral/futures/Try.h>

namespace coral { namespace fibers {

class Baton;

template <typename F>
typename FirstArgOf<F>::type::value_type
inline await(F&& func);

template <typename T>
class Promise {
 public:
  typedef T value_type;

  ~Promise();

  // not copyable
  Promise(const Promise&) = delete;
  Promise& operator=(const Promise&) = delete;

  // movable
  Promise(Promise&&) noexcept;
  Promise& operator=(Promise&&);

  /** Fulfill this promise (only for Promise<void>) */
  void setValue();

  /** Set the value (use perfect forwarding for both move and copy) */
  template <class M>
  void setValue(M&& value);

  /**
   * Fulfill the promise with a given try
   *
   * @param t
   */
  void setTry(coral::Try<T>&& t);

  /** Fulfill this promise with the result of a function that takes no
    arguments and returns something implicitly convertible to T.
    Captures exceptions. e.g.

    p.setWith([] { do something that may throw; return a T; });
  */
  template <class F>
  void setWith(F&& func);

  /** Fulfill the Promise with an exception_wrapper, e.g.
    auto ew = coral::try_and_catch<std::exception>([]{ ... });
    if (ew) {
      p.setException(std::move(ew));
    }
    */
  void setException(coral::exception_wrapper);

 private:
  template <typename F>
  friend typename FirstArgOf<F>::type::value_type await(F&&);

  Promise(coral::Try<T>& value, Baton& baton);
  coral::Try<T>* value_;
  Baton* baton_;

  void throwIfFulfilled() const;

  template <class F>
  typename std::enable_if<
    std::is_convertible<typename std::result_of<F()>::type, T>::value &&
    !std::is_same<T, void>::value>::type
  fulfilHelper(F&& func);

  template <class F>
  typename std::enable_if<
    std::is_same<typename std::result_of<F()>::type, void>::value &&
    std::is_same<T, void>::value>::type
  fulfilHelper(F&& func);
};

}}

#include <coral\/fibers/Promise-inl.h>
