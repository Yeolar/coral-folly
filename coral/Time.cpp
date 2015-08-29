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

#include <coral/Time.h>
#include <cfloat>
#include <cstring>
#include <stdexcept>
#include <sys/time.h>
#include <coral/Conv.h>
#include <glog/logging.h>

namespace coral {

// APPLE: = kCFAbsoluteTimeIntervalSince1970
const double Time::kTimeIntervalSince1970 = 978307200.0;
const double Time::kInvalidTime = DBL_MIN;

Time::Time(const char* s) {
  if (!s) {
    t_ = kInvalidTime;
    return;
  }
  struct tm t;
  if (strptime(s, "%F %T %z", &t)) {
    // APPLE alternative: CFCalendarComposeAbsoluteTime
    t_ = std::mktime(&t) - kTimeIntervalSince1970;
  } else {
    t_ = kInvalidTime;
    LOG(WARNING) << "failed to parse time: " << s;
  }
}

Time Time::now() {
#if __APPLE__
  return CFAbsoluteTimeGetCurrent();
#else
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  return tv.tv_sec + tv.tv_usec / 1000000.0;
#endif
}

namespace {

// Rule:
//  1. time(nullptr) get as UTC.
//  2. mktime(localtime(&t)) == t

const struct tm* timeZoneOffset(const char* format, const struct tm* tm) {
  size_t n = strlen(format);
  const char* p = format;
  if (n >= 5) {
    p += n - 5;
    if ((*p == '+' || *p == '-') && std::all_of(p+1, p+5, isdigit)) {
      struct tm tztm = *tm;
      time_t t = std::mktime(&tztm);
      int tz = coral::to<int>(p+1);
      t += (*p == '+' ? 1 : -1) * (tz / 100 * 60 + tz % 100) * 60;
      return std::gmtime(&t);
    }
  }
  if (n >= 3) {
    p += 2;
    if (strcasecmp(p, "GMT") == 0 || strcasecmp(p, "UTC") == 0) {
      struct tm tztm = *tm;
      time_t t = std::mktime(&tztm);
      return std::gmtime(&t);
    }
  }
  return tm;
}

inline void timePrintfImpl(std::string& output, const char *format,
                           const struct tm *tm) {
  tm = timeZoneOffset(format, tm);

  const auto writePoint = output.size();
  size_t formatLen = strlen(format);
  size_t remaining = std::max(std::max(32UL, output.capacity() - writePoint),
                              formatLen * 2);
  output.resize(writePoint + remaining);
  size_t bytesUsed = 0;

  do {
    bytesUsed = strftime(&output[writePoint], remaining, format, tm);
    if (bytesUsed == 0) {
      remaining *= 2;
      if (remaining > formatLen * 16) {
        throw std::invalid_argument(
            coral::to<std::string>("Maybe a non-output format given: ",
                                   format));
      }
      output.resize(writePoint + remaining);
    } else {  // > 0, there was enough room
      break;
    }
  } while (bytesUsed == 0);

  output.resize(writePoint + bytesUsed);
}

} // namespace anon

std::string timePrintf(const char *format, const struct tm *tm) {
  std::string ret;
  timePrintfImpl(ret, format, tm);
  return ret;
}

void timeAppendf(std::string *out, const char *format, const struct tm *tm) {
  timePrintfImpl(*out, format, tm);
}

} // namespace coral

