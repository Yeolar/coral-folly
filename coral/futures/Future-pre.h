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

// included by Future.h, do not include directly.

namespace coral {

template <class> class Promise;

template <typename T>
struct isFuture : std::false_type {
  using Inner = typename Unit::Lift<T>::type;
};

template <typename T>
struct isFuture<Future<T>> : std::true_type {
  typedef T Inner;
};

template <typename T>
struct isTry : std::false_type {};

template <typename T>
struct isTry<Try<T>> : std::true_type {};

namespace detail {

template <class> class Core;
template <class...> struct CollectAllVariadicContext;
template <class...> struct CollectVariadicContext;
template <class> struct CollectContext;

template<typename F, typename... Args>
using resultOf = decltype(std::declval<F>()(std::declval<Args>()...));

template <typename...>
struct ArgType;

template <typename Arg, typename... Args>
struct ArgType<Arg, Args...> {
  typedef Arg FirstArg;
};

template <>
struct ArgType<> {
  typedef void FirstArg;
};

template <bool isTry, typename F, typename... Args>
struct argResult {
  using Result = resultOf<F, Args...>;
};

template<typename F, typename... Args>
struct callableWith {
    template<typename T,
             typename = detail::resultOf<T, Args...>>
    static constexpr std::true_type
    check(std::nullptr_t) { return std::true_type{}; };

    template<typename>
    static constexpr std::false_type
    check(...) { return std::false_type{}; };

    typedef decltype(check<F>(nullptr)) type;
    static constexpr bool value = type::value;
};

template<typename T, typename F>
struct callableResult {
  typedef typename std::conditional<
    callableWith<F>::value,
    detail::argResult<false, F>,
    typename std::conditional<
      callableWith<F, T&&>::value,
      detail::argResult<false, F, T&&>,
      typename std::conditional<
        callableWith<F, T&>::value,
        detail::argResult<false, F, T&>,
        typename std::conditional<
          callableWith<F, Try<T>&&>::value,
          detail::argResult<true, F, Try<T>&&>,
          detail::argResult<true, F, Try<T>&>>::type>::type>::type>::type Arg;
  typedef isFuture<typename Arg::Result> ReturnsFuture;
  typedef Future<typename ReturnsFuture::Inner> Return;
};

template <typename L>
struct Extract : Extract<decltype(&L::operator())> { };

template <typename Class, typename R, typename... Args>
struct Extract<R(Class::*)(Args...) const> {
  typedef isFuture<R> ReturnsFuture;
  typedef Future<typename ReturnsFuture::Inner> Return;
  typedef typename ReturnsFuture::Inner RawReturn;
  typedef typename ArgType<Args...>::FirstArg FirstArg;
};

template <typename Class, typename R, typename... Args>
struct Extract<R(Class::*)(Args...)> {
  typedef isFuture<R> ReturnsFuture;
  typedef Future<typename ReturnsFuture::Inner> Return;
  typedef typename ReturnsFuture::Inner RawReturn;
  typedef typename ArgType<Args...>::FirstArg FirstArg;
};

} // detail


class Timekeeper;

} // namespace
