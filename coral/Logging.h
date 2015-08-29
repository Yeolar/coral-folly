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

#ifndef CORAL_LOGGING_H_
#define CORAL_LOGGING_H_

#include <atomic>
#include <chrono>

#include <coral/Portability.h>

#if CORAL_HAVE_LIBGLOG && CORAL_USE_GLOG

#include <glog/logging.h>

#else

#include <cassert>
#include <csignal>
#include <cstdlib>
#include <cstring>

#include <coral/FixedStream.h>
#include <coral/Time.h>

#ifndef __FILENAME__
# define __FILENAME__ ((strrchr(__FILE__, '/') ?: __FILE__ - 1) + 1)
#endif

namespace coral {
namespace logging {

// The CORAL_LOG_ABORT macro terminates the application
// and generates a core dump
#ifndef CORAL_LOG_ABORT
# if CORAL_LOG_USE_ABORT
#   define CORAL_LOG_ABORT abort()
# else
#   define CORAL_LOG_ABORT raise(SIGABRT)
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
  ::coral::logging::initialize(::coral::logging::getLevel(#severity[0]))
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
#if CORAL_LOG_TIMESTAMP
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
      CORAL_LOG_ABORT;
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
} // namespace coral

// printf interface macro helper; do not use directly

#ifndef LOG_FORMAT
#define LOG_FORMAT(severity, ...) do {                    \
  if (::coral::logging::get()->isEnabled(severity)) {     \
    ::coral::logging::get()->log(                         \
        severity, __FILENAME__, __LINE__, __VA_ARGS__);   \
  }                                                       \
  } while (0)
#endif

#ifndef LOG_STREAM
#define LOG_STREAM(severity, abort)                       \
  (!::coral::logging::get()->isEnabled(severity)) ?       \
  (void) 0 :                                              \
  ::coral::logging::LogMessageVoidify() &                 \
  ::coral::logging::LogMessage(                           \
      severity, __FILENAME__, __LINE__, abort).stream()
#endif

///////////////////////////////////////////////////////////////////////////
// Logging macros interface starts here

#ifndef LOG_HEXDUMP
#define LOG_HEXDUMP(data, datalen, ...) do {                              \
  ::coral::logging::get()->logHexdump(                                    \
      __FILENAME__, __LINE__, (char*)data, (int)datalen, __VA_ARGS__);    \
  } while (0)
#endif

#ifndef NDEBUG
# define LOG_DEBUG(...) LOG_FORMAT(::coral::logging::Level::DEBUG, __VA_ARGS__)
#else
# define LOG_DEBUG(...)
#endif

#define LOG_INFO(...)   LOG_FORMAT(::coral::logging::Level::INFO, __VA_ARGS__)
#define LOG_NOTICE(...) LOG_FORMAT(::coral::logging::Level::NOTICE, __VA_ARGS__)
#define LOG_WARN(...)   LOG_FORMAT(::coral::logging::Level::WARN, __VA_ARGS__)
#define LOG_WARNING(...) LOG_FORMAT(::coral::logging::Level::WARN, __VA_ARGS__)
#define LOG_ERROR(...)  LOG_FORMAT(::coral::logging::Level::ERROR, __VA_ARGS__)
#define LOG_CRIT(...)   LOG_FORMAT(::coral::logging::Level::CRIT, __VA_ARGS__)
#define LOG_ALERT(...)  LOG_FORMAT(::coral::logging::Level::ALERT, __VA_ARGS__)
#define LOG_EMERG(...)  LOG_FORMAT(::coral::logging::Level::EMERG, __VA_ARGS__)

#define LOG_FATAL(...) do {                                 \
  LOG_FORMAT(::coral::logging::Level::FATAL, __VA_ARGS__);  \
  CORAL_LOG_ABORT;                                          \
  } while (0)

#ifndef NDEBUG
# define LOG_STREAM_DEBUG  LOG_STREAM(::coral::logging::Level::DEBUG, false)
# define LOG_STREAM_TRACE  LOG_STREAM(::coral::logging::Level::TRACE, false) \
                           << __func__ << " "
# define LOG_STREAM_DFATAL LOG_STREAM(::coral::logging::Level::FATAL, true)
#else
# define LOG_STREAM_DEBUG  LOG_STREAM(::coral::logging::Level::NOTSET, false)
# define LOG_STREAM_TRACE  LOG_STREAM(::coral::logging::Level::NOTSET, false)
# define LOG_STREAM_DFATAL LOG_STREAM(::coral::logging::Level::NOTSET, true)
#endif

#define LOG_STREAM_INFO   LOG_STREAM(::coral::logging::Level::INFO, false)
#define LOG_STREAM_NOTICE LOG_STREAM(::coral::logging::Level::NOTICE, false)
#define LOG_STREAM_WARN   LOG_STREAM(::coral::logging::Level::WARN, false)
#define LOG_STREAM_WARNING LOG_STREAM(::coral::logging::Level::WARN, false)
#define LOG_STREAM_ERROR  LOG_STREAM(::coral::logging::Level::ERROR, false)
#define LOG_STREAM_CRIT   LOG_STREAM(::coral::logging::Level::CRIT, false)
#define LOG_STREAM_ALERT  LOG_STREAM(::coral::logging::Level::ALERT, false)
#define LOG_STREAM_EMERG  LOG_STREAM(::coral::logging::Level::EMERG, false)
#define LOG_STREAM_FATAL  LOG_STREAM(::coral::logging::Level::FATAL, true)

#define LOG(severity)   LOG_STREAM_ ## severity
#define PLOG(severity)  LOG_STREAM_ ## severity << strerror(errno) << " "

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

#endif /* CORAL_HAVE_LIBGLOG && CORAL_USE_GLOG */

///////////////////////////////////////////////////////////////////////////

#ifndef CR_LOG_EVERY_MS
/**
 * Issues a LOG(severity) no more often than every
 * milliseconds. Example:
 *
 * CR_LOG_EVERY_MS(INFO, 10000) << "At least ten seconds passed"
 *   " since you last saw this.";
 *
 * The implementation uses for statements to introduce variables in
 * a nice way that doesn't mess surrounding statements.  It is thread
 * safe.  Non-positive intervals will always log.
 */
#define CR_LOG_EVERY_MS(severity, milli_interval)                            \
  for (decltype(milli_interval) CR_LEM_once = 1,                             \
                                CR_LEM_interval = (milli_interval);          \
       CR_LEM_once; )                                                        \
    for (::std::chrono::milliseconds::rep CR_LEM_prev, CR_LEM_now =          \
             CR_LEM_interval <= 0 ? 0 :                                      \
             ::std::chrono::duration_cast< ::std::chrono::milliseconds>(     \
                 ::std::chrono::system_clock::now().time_since_epoch()       \
                 ).count();                                                  \
         CR_LEM_once; )                                                      \
      for (static ::std::atomic< ::std::chrono::milliseconds::rep>           \
               CR_LEM_hist; CR_LEM_once; CR_LEM_once = 0)                    \
        if (CR_LEM_interval > 0 &&                                           \
            (CR_LEM_now - (CR_LEM_prev =                                     \
                           CR_LEM_hist.load(std::memory_order_acquire)) <    \
                                                          CR_LEM_interval || \
             !CR_LEM_hist.compare_exchange_strong(CR_LEM_prev,CR_LEM_now))) {\
        } else                                                               \
          LOG(severity)

#endif

#endif /* CORAL_LOGGING_H_ */

