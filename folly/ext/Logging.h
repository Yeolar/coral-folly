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

#ifndef FOLLY_EXT_LOGGING_H_
#define FOLLY_EXT_LOGGING_H_

#include <atomic>
#include <chrono>

#include <folly/Portability.h>

#if FOLLY_HAVE_LIBGLOG && FOLLY_USE_GLOG

#include <glog/logging.h>

#else

#include <cassert>
#include <csignal>
#include <cstdlib>
#include <cstring>

#include <folly/ext/FixedStream.h>
#include <folly/ext/Time.h>

#ifndef __FILENAME__
# define __FILENAME__ ((strrchr(__FILE__, '/') ?: __FILE__ - 1) + 1)
#endif

namespace folly {
namespace logging {

// The FOLLY_LOG_ABORT macro terminates the application
// and generates a core dump
#ifndef FOLLY_LOG_ABORT
# if FOLLY_LOG_USE_ABORT
#   define FOLLY_LOG_ABORT abort()
# else
#   define FOLLY_LOG_ABORT raise(SIGABRT)
# endif
#endif

/**
 * Output levels modelled after syslog
 */
enum class Level {
  EMERG  = 0,
  FATAL  = 0,
  ALERT  = 1,
  CRIT   = 2,
  ERROR  = 3,
  WARN   = 4,
  NOTICE = 5,
  INFO   = 6,
  DEBUG  = 7,
  TRACE  = 8,
  NOTSET = 9
};

#ifndef NDEBUG
constexpr Level DEFAULT_LOGLEVEL = Level::DEBUG;
#else
constexpr Level DEFAULT_LOGLEVEL = Level::WARN;
#endif

constexpr size_t BUF_SIZE = 1024;

inline Level getLevel(const char label) {
  const char* levelLabels = "FACEWNIDT";
  const char* p = strchr(levelLabels, label);
  if (!p) return DEFAULT_LOGLEVEL;
  return static_cast<Level>(p - levelLabels);
}

inline const char* getLevelName(Level level) {
  static const char* levelName[] = {
    "FATAL",
    "ALERT",
    "CRIT",
    "ERROR",
    "WARN",
    "NOTICE",
    "INFO",
    "DEBUG",
    "TRACE",
    "NOTSET"
  };
  return levelName[static_cast<int>(level)];
}

class LogWriter {
public:
  LogWriter(Level level, const char* name = nullptr);
  ~LogWriter();

  void setLevel(Level level) {
    level_ = level;
  }

  Level getLevel() const {
    return level_;
  }

  bool isEnabled(Level level) const {
    return level <= level_;
  }

  void log(Level level, const char* file, int line, const char* format, ...);

  // Hexadecimal dump in the canonical hex + ascii display
  // See -C option in man hexdump
  void logHexdump(const char* file,
                  int line,
                  char* data,
                  int datalen,
                  const char* format, ...);

  void logString(const char* message, size_t len) {
    fwrite(message, sizeof(char), len, file_);
  }

private:
  Level level_;         // The current level (everything above is filtered)
  FILE *file_;          // The output file descriptor
};

/**
 * Public initialization function - creates a singleton instance of LogWriter
 */
extern void initialize(Level level, const char* name = nullptr);

#ifndef SET_LOGLEVEL
#define SET_LOGLEVEL(severity) \
  ::folly::logging::initialize(::folly::logging::getLevel(#severity[0]))
#endif

/**
 * Accessor for the LogWriter singleton instance
 */
extern LogWriter* get();

class LogMessage {
public:
  LogMessage(Level level, const char* file, int line, bool abort)
  : out_(buf_, BUF_SIZE)
  , abort_(abort) {
    out_ << "[" << getLevelName(level)[0]
#if FOLLY_LOG_TIMESTAMP
    << timeNowPrintf(" %y%m%d %T ")
#else
    << " "
#endif
    << file << ":" << line << "] ";
  }

  ~LogMessage() {
    out_ << std::endl;
    std::string message = out_.str();
    get()->logString(message.c_str(), message.size());
    if (abort_)
      FOLLY_LOG_ABORT;
  }

  FixedOstream& stream() { return out_; }

private:
  char buf_[BUF_SIZE];
  FixedOstream out_;
  bool abort_;
};

// This class is used to explicitly ignore values in the conditional
// logging macros.  This avoids compiler warnings like "value computed
// is not used" and "statement has no effect".
class LogMessageVoidify {
public:
  LogMessageVoidify() { }
  // This has to be an operator with a precedence lower than << but
  // higher than ?:
  void operator&(std::ostream&) { }
};

} // namespace logging
} // namespace folly

// printf interface macro helper; do not use directly

#ifndef LOG_FORMAT
#define LOG_FORMAT(severity, ...) do {                    \
  if (::folly::logging::get()->isEnabled(severity)) {     \
    ::folly::logging::get()->log(                         \
        severity, __FILENAME__, __LINE__, __VA_ARGS__);   \
  }                                                       \
  } while (0)
#endif

#ifndef LOG_STREAM
#define LOG_STREAM(severity, abort)                       \
  (!::folly::logging::get()->isEnabled(severity)) ?       \
  (void) 0 :                                              \
  ::folly::logging::LogMessageVoidify() &                 \
  ::folly::logging::LogMessage(                           \
      severity, __FILENAME__, __LINE__, abort).stream()
#endif

///////////////////////////////////////////////////////////////////////////
// Logging macros interface starts here

#ifndef LOG_HEXDUMP
#define LOG_HEXDUMP(data, datalen, ...) do {                              \
  ::folly::logging::get()->logHexdump(                                    \
      __FILENAME__, __LINE__, (char*)data, (int)datalen, __VA_ARGS__);    \
  } while (0)
#endif

#ifndef NDEBUG
# define LOG_DEBUG(...) LOG_FORMAT(::folly::logging::Level::DEBUG, __VA_ARGS__)
#else
# define LOG_DEBUG(...)
#endif

#define LOG_INFO(...)   LOG_FORMAT(::folly::logging::Level::INFO, __VA_ARGS__)
#define LOG_NOTICE(...) LOG_FORMAT(::folly::logging::Level::NOTICE, __VA_ARGS__)
#define LOG_WARN(...)   LOG_FORMAT(::folly::logging::Level::WARN, __VA_ARGS__)
#define LOG_WARNING(...) LOG_FORMAT(::folly::logging::Level::WARN, __VA_ARGS__)
#define LOG_ERROR(...)  LOG_FORMAT(::folly::logging::Level::ERROR, __VA_ARGS__)
#define LOG_CRIT(...)   LOG_FORMAT(::folly::logging::Level::CRIT, __VA_ARGS__)
#define LOG_ALERT(...)  LOG_FORMAT(::folly::logging::Level::ALERT, __VA_ARGS__)
#define LOG_EMERG(...)  LOG_FORMAT(::folly::logging::Level::EMERG, __VA_ARGS__)

#define LOG_FATAL(...) do {                                 \
  LOG_FORMAT(::folly::logging::Level::FATAL, __VA_ARGS__);  \
  FOLLY_LOG_ABORT;                                          \
  } while (0)

#ifndef NDEBUG
# define LOG_STREAM_DEBUG  LOG_STREAM(::folly::logging::Level::DEBUG, false)
# define LOG_STREAM_TRACE  LOG_STREAM(::folly::logging::Level::TRACE, false) \
                           << __func__ << " "
# define LOG_STREAM_DFATAL LOG_STREAM(::folly::logging::Level::FATAL, true)
#else
# define LOG_STREAM_DEBUG  LOG_STREAM(::folly::logging::Level::NOTSET, false)
# define LOG_STREAM_TRACE  LOG_STREAM(::folly::logging::Level::NOTSET, false)
# define LOG_STREAM_DFATAL LOG_STREAM(::folly::logging::Level::NOTSET, true)
#endif

#define LOG_STREAM_INFO   LOG_STREAM(::folly::logging::Level::INFO, false)
#define LOG_STREAM_NOTICE LOG_STREAM(::folly::logging::Level::NOTICE, false)
#define LOG_STREAM_WARN   LOG_STREAM(::folly::logging::Level::WARN, false)
#define LOG_STREAM_WARNING LOG_STREAM(::folly::logging::Level::WARN, false)
#define LOG_STREAM_ERROR  LOG_STREAM(::folly::logging::Level::ERROR, false)
#define LOG_STREAM_CRIT   LOG_STREAM(::folly::logging::Level::CRIT, false)
#define LOG_STREAM_ALERT  LOG_STREAM(::folly::logging::Level::ALERT, false)
#define LOG_STREAM_EMERG  LOG_STREAM(::folly::logging::Level::EMERG, false)
#define LOG_STREAM_FATAL  LOG_STREAM(::folly::logging::Level::FATAL, true)

#define LOG(severity)   LOG_STREAM_ ## severity
#define PLOG(severity)  LOG_STREAM_ ## severity << strerror(errno) << " "

// currently just alias to LOG(DEBUG)
#define VLOG(i)         LOG_STREAM_DEBUG

#define LOG_FIRST_N(severity, n)                  \
  static int LOG_OCCURRENCES = 0;               \
  if (LOG_OCCURRENCES <= n) ++LOG_OCCURRENCES;  \
  if (LOG_OCCURRENCES <= n) LOG(severity)

#ifndef NDEBUG
# define DCHECK(condition) \
    (condition) ? (void) 0 : LOG(FATAL) << "Check `" # condition "' failed. "
#else
# define DCHECK(condition)
#endif
#define DCHECK_OP(op, val1, val2) DCHECK((val1) op (val2))
#define DCHECK_EQ(val1, val2) DCHECK_OP(==, val1, val2)
#define DCHECK_NE(val1, val2) DCHECK_OP(!=, val1, val2)
#define DCHECK_LE(val1, val2) DCHECK_OP(<=, val1, val2)
#define DCHECK_LT(val1, val2) DCHECK_OP(< , val1, val2)
#define DCHECK_GE(val1, val2) DCHECK_OP(>=, val1, val2)
#define DCHECK_GT(val1, val2) DCHECK_OP(> , val1, val2)

#define CHECK(condition) \
  (condition) ? (void) 0 : LOG(FATAL) << "Check `" # condition "' failed. "
#define CHECK_OP(op, val1, val2) CHECK((val1) op (val2))
#define CHECK_EQ(val1, val2) CHECK_OP(==, val1, val2)
#define CHECK_NE(val1, val2) CHECK_OP(!=, val1, val2)
#define CHECK_LE(val1, val2) CHECK_OP(<=, val1, val2)
#define CHECK_LT(val1, val2) CHECK_OP(< , val1, val2)
#define CHECK_GE(val1, val2) CHECK_OP(>=, val1, val2)
#define CHECK_GT(val1, val2) CHECK_OP(> , val1, val2)

#define PCHECK(condition) \
  (condition) ? (void) 0 : PLOG(FATAL) << "Check `" # condition "' failed. "
#define PCHECK_OP(op, val1, val2) PCHECK((val1) op (val2))
#define PCHECK_EQ(val1, val2) PCHECK_OP(==, val1, val2)
#define PCHECK_NE(val1, val2) PCHECK_OP(!=, val1, val2)
#define PCHECK_LE(val1, val2) PCHECK_OP(<=, val1, val2)
#define PCHECK_LT(val1, val2) PCHECK_OP(< , val1, val2)
#define PCHECK_GE(val1, val2) PCHECK_OP(>=, val1, val2)
#define PCHECK_GT(val1, val2) PCHECK_OP(> , val1, val2)

#define CHECK_ERR(invocation) \
  ((invocation) != -1) ? (void) 0 : LOG(FATAL) << # invocation

#endif /* FOLLY_HAVE_LIBGLOG && FOLLY_USE_GLOG */

#endif /* FOLLY_EXT_LOGGING_H_ */

