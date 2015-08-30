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

#include <coral/io/FsUtil.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <coral/Exception.h>

namespace bsys = ::boost::system;

namespace coral {
namespace fs {

namespace detail {
extern const char shellEscapeTable[] =
  "EEEEEEEEEEnEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE,-./iiiiiiiiii:EEEEE"
  "@AAAAAAAAAAAAAAAAAAAAAAAAAAE\\EE_EaaaaaaaaaaaaaaaaaaaaaaaaaaEEEEE"
  "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR"
  "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR";
}  // namespace detail

path normalize(const path& pth) {
  path tmp(pth);
  tmp.normalize();

  if (tmp.string().find("//") == 0) {
    tmp = tmp.string().substr(1);
  }

  path p;
  auto it = tmp.begin();
  if (*it++ == "/" && *it == "..") {
    // NOTE: ++it is same as it++ for path
    ++it;
  } else {
    --it;
  }
  for (; it != tmp.end(); ++it) {
    if (*it != ".") {
      p /= *it;
    }
  }
  return p;
}

namespace {
bool skipPrefix(const path& pth, const path& prefix, path::const_iterator& it) {
  it = pth.begin();
  for (auto& p : prefix) {
    if (it == pth.end()) {
      return false;
    }
    if (p == ".") {
      // Should only occur at the end, if prefix ends with a slash
      continue;
    }
    if (*it++ != p) {
      return false;
    }
  }
  return true;
}
}  // namespace

bool starts_with(const path& pth, const path& prefix) {
  path::const_iterator it;
  return skipPrefix(pth, prefix, it);
}

path remove_prefix(const path& pth, const path& prefix) {
  path::const_iterator it;
  if (!skipPrefix(pth, prefix, it)) {
    throw filesystem_error(
        "Path does not start with prefix",
        pth, prefix,
        bsys::errc::make_error_code(bsys::errc::invalid_argument));
  }

  path p;
  for (; it != pth.end(); ++it) {
    p /= *it;
  }

  return p;
}

path canonical_parent(const path& pth, const path& base) {
  return canonical(pth.parent_path(), base) / pth.filename();
}

path executable_path() {
  return read_symlink("/proc/self/exe");
}

dev_t device(const path& p) {
  struct stat path_stat;
  if (stat(p.c_str(), &path_stat) == 0) {
    return path_stat.st_dev;
  }
  return -1;
}

const passwd* passwd_entry() {
  return getpwuid(getuid());
}

path home_path() {
  const passwd* entry = passwd_entry();
  if (entry && entry->pw_dir) {
    path p(entry->pw_dir);
    if (exists(p)) {
      return p;
    }
  }
  return path();
}

path home_relative_path(const path& pth) {
  path home(home_path());
  if (pth == home) {
    return "~";
  }
  path::const_iterator it;
  if (!skipPrefix(pth, home, it)) {
    return pth;
  }
  path p("~");
  for (; it != pth.end(); ++it) {
    p /= *it;
  }
  return p;
}

path relative_path(const path& pth, const path& base) {
  path relative;
  path::const_iterator it = pth.begin();
  for (auto& p : base) {
    if (it == pth.end()) {
      break;
    }
    if (p == ".") {
      // Should only occur at the end, if prefix ends with a slash
      continue;
    }
    if (*it != p) {
      relative /= "..";
    } else {
      ++it;
    }
  }
  for (; it != pth.end(); ++it) {
    relative /= *it;
  }
  return relative;
}

}  // namespace fs
}  // namespace coral
