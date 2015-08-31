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

#if CORAL_HAVE_LIBONIG

#include <coral/Regex.h>
#include <coral/Conv.h>

namespace coral {

namespace {

__attribute__((constructor)) void setupOnigSyntax() {
  static OnigSyntaxType syntax;
  onig_copy_syntax(&syntax, ONIG_SYNTAX_RUBY);
  ONIG_OPTION_OFF(syntax.options, ONIG_OPTION_ASCII_RANGE); // enable Unicode
  onig_set_syntax_options(&syntax, syntax.options);
  onig_set_default_syntax(&syntax);
}

void region_free(OnigRegion* r) {
  onig_region_free(r, 1);
}

} // namespace anon

bool regexValidate(StringPiece pattern, std::string* error) {
  OnigRegex tmp = NULL;
  OnigErrorInfo err;
  int r = onig_new(&tmp,
                   (OnigUChar*)pattern.data(),
                   (OnigUChar*)pattern.data() + pattern.size(),
                   ONIG_OPTION_CAPTURE_GROUP,
                   ONIG_ENCODING_UTF8,
                   ONIG_SYNTAX_DEFAULT,
                   &err);
  if (r != ONIG_NORMAL) {
    OnigUChar s[ONIG_MAX_ERROR_MESSAGE_LEN];
    onig_error_code_to_str(s, r, &err);
    *error = (const char*)s;
  }
  if (tmp) {
    onig_free(tmp);
  }
}

void Regex::Regex(StringPiece pattern, OnigOptionType options) {
  pattern_ = pattern.str();

  OnigRegex tmp = NULL;
  OnigErrorInfo err;
  if (!(options & ONIG_OPTION_DONT_CAPTURE_GROUP)) {
    options |= ONIG_OPTION_CAPTURE_GROUP;
  }
  int r = onig_new(&tmp,
                   (OnigUChar*)pattern.begin(),
                   (OnigUChar*)pattern.end(),
                   options,
                   ONIG_ENCODING_UTF8,
                   ONIG_SYNTAX_DEFAULT,
                   &err);
  if (r != ONIG_NORMAL) {
    OnigUChar s[ONIG_MAX_ERROR_MESSAGE_LEN];
    onig_error_code_to_str(s, r, &err);
    LOG(ERROR) << "regex error: " << s << " (" << pattern_ << ")";
    if (tmp) {
      onig_free(tmp);
    }
  } else {
    re_.reset(tmp, onig_free);
  }
}

#ifndef ONIG_OPTION_NOTGPOS
#define ONIG_OPTION_NOTGPOS (ONIG_OPTION_MAXBIT << 1)
#endif

bool regexSearch(StringPiece sp, Match& match, const Regex& regex,
                 const char* from, const char* to,
                 OnigOptionType options) {
  if (regex.isValid()) {
    OnigRegionPtr region(onig_region_new(), region_free);
    from = from ?: sp.begin();
    to = to ?: sp.end();
    const char* gpos = (options & ONIG_OPTION_NOTGPOS) ? nullptr : from;
    options &= ~ONIG_OPTION_NOTGPOS;

    if (ONIG_MISMATCH != onig_search_gpos(regex.get(),
                                          (OnigUChar*)sp.begin(),
                                          (OnigUChar*)sp.end(),
                                          (OnigUChar*)gpos,
                                          (OnigUChar*)from,
                                          (OnigUChar*)to,
                                          region.get(),
                                          options)) {
      match = Match(regex.regex(), region);
      return true;
    }
  }
  return false;
}

namespace {

struct UData {
  const char* buffer;
  const OnigRegion* match;
  std::map<std::string, std::string>* out;
  std::multimap<std::string, SizeRange>* idxOut;
};

int copyNamedCaptures(const OnigUChar* name,
                      const OnigUChar* nameEnd,
                      int n,
                      int* it,
                      OnigRegex re,
                      void* udata) {
  const UData& data = *(UData*)udata;
  std::string capture;
  bool has = false;
  while (n > 0) {
    if (data.match->beg[*it] != -1) {
      has = true;
      capture.append(
        data.buffer + data.match->beg[*it],
        data.buffer + data.match->end[*it]);
    }
    ++it;
    --n;
  }
  if (has) {
    data.out->emplace(std::string(name, nameEnd), capture);
  }
  return 0;
}

int copyNamedCaptureIndices(const OnigUChar* name,
                            const OnigUChar* nameEnd,
                            int n,
                            int* it,
                            OnigRegex re,
                            void* udata) {
  const UData& data = *(UData*)udata;
  while (n > 0) {
    if (data.match->beg[*it] != -1) {
      data.idxOut->emplace(
        std::string(name, nameEnd),
        SizeRange(data.match->beg[*it], data.match->end[*it]));
    }
    ++it;
    --n;
  }
  return 0;
}

} // namespace anon

std::map<std::string, std::string> Match::captures() const {
  std::map<std::string, std::string> out;
  const char* buffer = buffer_.data();
  OnigRegion* match = region_.get();
  OnigRegexType* re = re_.get();
  for (size_t i = 0; i < match->num_regs; ++i) {
    if (match->beg[i] != -1)
      out.emplace(
        to<std::string>(i),
        std::string(buffer + match->beg[i], buffer + match->end[i]));
  }
  UData udata = { buffer, match, &out, nullptr };
  onig_foreach_name(re, &copyNamedCaptures, &udata);
}

std::multimap<std::string, SizeRange> Match::captureIndices() const {
  std::multimap<std::string, SizeRange> out;
  const char* buffer = buffer_.data();
  OnigRegion* match = region_.get();
  OnigRegexType* re = re_.get();
  for (size_t i = 0; i < match->num_regs; ++i) {
    if (match->beg[i] != -1)
      out.emplace(to<std::string>(i), SizeRange(match->beg[i], match->end[i]));
  }
  UData udata = { buffer, match, nullptr, &out };
  onig_foreach_name(re, &copyNamedCaptureIndices, &udata);
}

} // namespace coral

#endif

