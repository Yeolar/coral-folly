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

#include <folly/ext/Logging.h>

#if !(FOLLY_HAVE_LIBGLOG && FOLLY_USE_GLOG)

#include <cerrno>
#include <cstdarg>
#include <system_error>

namespace folly {
namespace logging {

namespace {

/**
 * A countable version of snprintf/vsnprintf.
 */
int vscnprintf(char* buf, size_t size, const char* format, va_list args) {
  int n = vsnprintf(buf, size, format, args);

  //
  // The return value is the number of characters which would be written
  // into buf not including the trailing '\0'. If size is == 0 the
  // function returns 0.
  //
  // On error, the function also returns 0. This is to allow idiom such
  // as len += _vscnprintf(...)
  //
  // See: http://lwn.net/Articles/69419/
  //
  if (n <= 0)
    return 0;
  if (n < (int)size)
    return n;
  return (int)size - 1;
}

int scnprintf(char* buf, size_t size,
              FOLLY_PRINTF_FORMAT const char* format, ...)
  FOLLY_PRINTF_FORMAT_ATTR(3, 4);

int scnprintf(char* buf, size_t size, const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  int n = vscnprintf(buf, size, format, ap);
  va_end(ap);
  return n;
}

static LogWriter* logger = nullptr;

} // namespace anon

LogWriter::LogWriter(Level level, const char* name)
  : level_(level) {
  if (name && strlen(name) > 0) {
    file_ = fopen(name, "a+");
    if (!file_) {
      throw std::system_error(
          errno, std::system_category(), "open log file failed");
    }
  } else {
    file_ = stderr;
  }
}

LogWriter::~LogWriter() {
  if (file_ != nullptr && file_ != stderr) {
    fclose(file_);
  }
}

void LogWriter::log(Level level,
                    const char* file,
                    int line,
                    const char* format, ...) {
  char buf[BUF_SIZE];
  size_t len = 0;
  size_t size = BUF_SIZE;

  len += scnprintf(buf + len, size - len, "[%c", getLevelName(level)[0]);
#if FOLLY_LOG_TIMESTAMP
  time_t t = time(nullptr);
  len += strftime (buf + len, size - len, " %y%m%d %T", localtime(&t));
#endif
  len += scnprintf(buf + len, size - len, " %s:%d] ", file, line);

  va_list args;
  va_start(args, format);
  len += vscnprintf(buf + len, size - len, format, args);
  va_end(args);
  buf[len++] = '\n';
  logString(buf, len);
}

// Hexadecimal dump in the canonical hex + ascii display
// See -C option in man hexdump
void LogWriter::logHexdump(const char* file,
                           int line,
                           char* data,
                           int datalen,
                           const char* format, ...) {
  char buf[8 * BUF_SIZE];
  int off = 0;    // data offset
  size_t len = 0;
  size_t size = 8 * BUF_SIZE;
  int i;

#if FOLLY_LOG_TIMESTAMP
  time_t t = time(nullptr);
  len += strftime (buf + len, size - len, "[- %y%m%d %T", localtime(&t));
  len += scnprintf(buf + len, size - len, " %s:%d] ", file, line);
#else
  len += scnprintf(buf + len, size - len, "[- %s:%d] ", file, line);
#endif

  va_list args;
  va_start(args, format);
  len += vscnprintf(buf + len, size - len, format, args);
  va_end(args);
  buf[len++] = '\n';

  while (datalen != 0 && (len < size - 1)) {
    char* save;
    int savelen;
    const char* str;
    unsigned char c;

    len += scnprintf(buf + len, size - len, "%08x  ", off);
    save = data;
    savelen = datalen;
    for (i = 0; datalen != 0 && i < 16; data++, datalen--, i++) {
      c = *data;
      str = (i == 7) ? "  " : " ";
      len += scnprintf(buf + len, size - len, "%02x%s", c, str);
    }
    for ( ; i < 16; i++) {
      str = (i == 7) ? "  " : " ";
      len += scnprintf(buf + len, size - len, "  %s", str);
    }
    data = save;
    datalen = savelen;
    len += scnprintf(buf + len, size - len, "  |");
    for (i = 0; datalen != 0 && i < 16; data++, datalen--, i++) {
      c = isprint(*data) ? *data : '.';
      len += scnprintf(buf + len, size - len, "%c", c);
    }
    len += scnprintf(buf + len, size - len, "|\n");
    off += 16;
  }
  logString(buf, len);
}

void initialize(Level level, const char* name) {
  if (!logger) {
    logger = new LogWriter(level, name);
  }
}

LogWriter* get() {
  if (!logger) {
    logger = new LogWriter(DEFAULT_LOGLEVEL);
    LOG(WARN) << "Logger uninitialized, use "
      << getLevelName(DEFAULT_LOGLEVEL)
      << " level and output to stderr by default.";
  }
  return logger;
}

} // namespace logging
} // namespace folly

#endif
