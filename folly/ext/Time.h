/*
 * Copyright (C) 2015, Yeolar
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

#ifndef FOLLY_EXT_TIME_H_
#define FOLLY_EXT_TIME_H_

#include <cmath>
#include <ctime>
#include <string>
#include <boost/operators.hpp>

namespace folly {

/**
 * Time represents absolute time.
 *
 * absolute time is the time interval since the reference date,
 * the reference date (epoch) is 00:00:00 1 January 2001.
 *
 * APPLE:
 *  typedef double CFTimeInterval;
 *  typedef CFTimeInterval CFAbsoluteTime;
 */
class Time : private boost::totally_ordered<Time> {
public:
  static const double kTimeIntervalSince1970;
  static const double kInvalidTime;

  Time(double t = kInvalidTime)
    : t_(t) { }

  Time(time_t t)
    : t_(t - kTimeIntervalSince1970) { }

  Time(const char* s);

  Time(const std::string& str)
    : Time(str.c_str()) { }

  double value() const { return t_; }

  time_t timeValue() const {
    return std::round(t_) + kTimeIntervalSince1970;
  }

  static Time now();

  explicit operator bool() const { return t_ != kInvalidTime; }

  Time operator+(double d) const { return Time(t_ + d); }
  Time operator-(double d) const { return Time(t_ - d); }

private:
  double t_;
};

inline bool operator==(const Time& lhs, const Time& rhs) {
  return lhs.value() == rhs.value();
}

inline bool operator<(const Time& lhs, const Time& rhs) {
  return lhs.value() < rhs.value();
}

inline double operator-(const Time& lhs, const Time& rhs) {
  return lhs.value() - rhs.value();
}

/**
 * timePrintf is much like strftime but deposits its result into a
 * string. Two signatures are supported: the first simply returns the
 * resulting string, and the second appends the produced characters to
 * the specified string and returns a reference to it.
 */
std::string timePrintf(const char *format, const struct tm *tm);

void timeAppendf(std::string *out, const char *format, const struct tm *tm);

/**
 * For Time.
 */
inline std::string timePrintf(const char *format, const Time& tm) {
  time_t t = tm.timeValue();
  return timePrintf(format, std::localtime(&t));
}

inline void timeAppendf(std::string *out, const char *format, const Time& tm) {
  time_t t = tm.timeValue();
  timeAppendf(out, format, std::localtime(&t));
}

/**
 * timeNowPrintf is a specialized timePrintf for print current time.
 */
inline std::string timeNowPrintf(const char *format) {
  time_t t = std::time(nullptr);
  return timePrintf(format, std::localtime(&t));
}

inline void timeNowAppendf(std::string *out, const char *format) {
  time_t t = std::time(nullptr);
  timeAppendf(out, format, std::localtime(&t));
}

/*
 * Some common time format.
 * About time zone: currently can handle +dddd, UTC, GMT.
 */
const char *const EMAIL_TIME_FORMAT = "%a, %d %b %Y %T GMT";
const char *const APPLE_TIME_FORMAT = "%F %T +0000";

} // namespace folly

#endif /* FOLLY_EXT_TIME_H_ */

