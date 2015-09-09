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

#include <folly/dynamic.h>

namespace folly {

//////////////////////////////////////////////////////////////////////

#define DEF_TYPE(T, str, typen)                                   \
  template<> char const dynamic::TypeInfo<T>::name[] = str;       \
  template<> dynamic::Type const dynamic::TypeInfo<T>::type = typen

DEF_TYPE(void*,               "null",    dynamic::NULLT);
DEF_TYPE(bool,                "boolean", dynamic::BOOL);
DEF_TYPE(fbstring,            "string",  dynamic::STRING);
#if FOLLY_DYNAMIC_EXTEND_DATA
DEF_TYPE(byte_string,         "data",    dynamic::DATA);
#endif
DEF_TYPE(dynamic::Array,      "array",   dynamic::ARRAY);
DEF_TYPE(double,              "double",  dynamic::DOUBLE);
DEF_TYPE(int64_t,             "int64",   dynamic::INT64);
DEF_TYPE(dynamic::ObjectImpl, "object",  dynamic::OBJECT);

#undef DEF_TYPE

const char* dynamic::typeName() const {
  return typeName(type_);
}

TypeError::TypeError(const std::string& expected, dynamic::Type actual)
  : std::runtime_error(to<std::string>("TypeError: expected dynamic "
      "type `", expected, '\'', ", but had type `",
      dynamic::typeName(actual), '\''))
{}

TypeError::TypeError(const std::string& expected,
    dynamic::Type actual1, dynamic::Type actual2)
  : std::runtime_error(to<std::string>("TypeError: expected dynamic "
      "types `", expected, '\'', ", but had types `",
      dynamic::typeName(actual1), "' and `", dynamic::typeName(actual2),
      '\''))
{}

TypeError::~TypeError() = default;

// This is a higher-order preprocessor macro to aid going from runtime
// types to the compile time type system.
#define CR_DYNAMIC_APPLY(type, apply) do {         \
  switch ((type)) {                             \
  case NULLT:   apply(void*);          break;   \
  case ARRAY:   apply(Array);          break;   \
  case BOOL:    apply(bool);           break;   \
  case DOUBLE:  apply(double);         break;   \
  case INT64:   apply(int64_t);        break;   \
  case OBJECT:  apply(ObjectImpl);     break;   \
  case STRING:  apply(fbstring);       break;   \
  default:      CHECK(0); abort();              \
  }                                             \
} while (0)

bool dynamic::operator<(dynamic const& o) const {
  if (UNLIKELY(type_ == OBJECT || o.type_ == OBJECT)) {
    throw TypeError("object", type_);
  }
  if (type_ != o.type_) {
    return type_ < o.type_;
  }

#define CR_X(T) return CompareOp<T>::comp(*getAddress<T>(),   \
                                          *o.getAddress<T>())
  CR_DYNAMIC_APPLY(type_, CR_X);
#undef CR_X
}

bool dynamic::operator==(dynamic const& o) const {
  if (type() != o.type()) {
    if (isNumber() && o.isNumber()) {
      auto& integ = isInt() ? *this : o;
      auto& doubl = isInt() ? o     : *this;
      return integ.asInt() == doubl.asDouble();
    }
    return false;
  }

#define CR_X(T) return *getAddress<T>() == *o.getAddress<T>();
  CR_DYNAMIC_APPLY(type_, CR_X);
#undef CR_X
}

dynamic& dynamic::operator=(dynamic const& o) {
  if (&o != this) {
    if (type_ == o.type_) {
#define CR_X(T) *getAddress<T>() = *o.getAddress<T>()
      CR_DYNAMIC_APPLY(type_, CR_X);
#undef CR_X
    } else {
      destroy();
#define CR_X(T) new (getAddress<T>()) T(*o.getAddress<T>())
      CR_DYNAMIC_APPLY(o.type_, CR_X);
#undef CR_X
      type_ = o.type_;
    }
  }
  return *this;
}

dynamic& dynamic::operator=(dynamic&& o) noexcept {
  if (&o != this) {
    if (type_ == o.type_) {
#define CR_X(T) *getAddress<T>() = std::move(*o.getAddress<T>())
      CR_DYNAMIC_APPLY(type_, CR_X);
#undef CR_X
    } else {
      destroy();
#define CR_X(T) new (getAddress<T>()) T(std::move(*o.getAddress<T>()))
      CR_DYNAMIC_APPLY(o.type_, CR_X);
#undef CR_X
      type_ = o.type_;
    }
  }
  return *this;
}

dynamic& dynamic::operator[](dynamic const& k) & {
  if (!isObject() && !isArray()) {
    throw TypeError("object/array", type());
  }
  if (isArray()) {
    return at(k);
  }
  auto& obj = get<ObjectImpl>();
  auto ret = obj.insert({k, nullptr});
  return ret.first->second;
}

dynamic dynamic::getDefault(const dynamic& k, const dynamic& v) const& {
  auto& obj = get<ObjectImpl>();
  auto it = obj.find(k);
  return it == obj.end() ? v : it->second;
}

dynamic dynamic::getDefault(const dynamic& k, dynamic&& v) const& {
  auto& obj = get<ObjectImpl>();
  auto it = obj.find(k);
  // Avoid clang bug with ternary
  if (it == obj.end()) {
    return std::move(v);
  } else {
    return it->second;
  }
}

dynamic dynamic::getDefault(const dynamic& k, const dynamic& v) && {
  auto& obj = get<ObjectImpl>();
  auto it = obj.find(k);
  // Avoid clang bug with ternary
  if (it == obj.end()) {
    return v;
  } else {
    return std::move(it->second);
  }
}

dynamic dynamic::getDefault(const dynamic& k, dynamic&& v) && {
  auto& obj = get<ObjectImpl>();
  auto it = obj.find(k);
  return std::move(it == obj.end() ? v : it->second);
}

const dynamic* dynamic::get_ptr(dynamic const& idx) const& {
  if (auto* parray = get_nothrow<Array>()) {
    if (!idx.isInt()) {
      throw TypeError("int64", idx.type());
    }
    if (idx < 0 || idx >= parray->size()) {
      return nullptr;
    }
    return &(*parray)[idx.asInt()];
  } else if (auto* pobject = get_nothrow<ObjectImpl>()) {
    auto it = pobject->find(idx);
    if (it == pobject->end()) {
      return nullptr;
    }
    return &it->second;
  } else {
    throw TypeError("object/array", type());
  }
}

dynamic const& dynamic::at(dynamic const& idx) const& {
  if (auto* parray = get_nothrow<Array>()) {
    if (!idx.isInt()) {
      throw TypeError("int64", idx.type());
    }
    if (idx < 0 || idx >= parray->size()) {
      throw std::out_of_range("out of range in dynamic array");
    }
    return (*parray)[idx.asInt()];
  } else if (auto* pobject = get_nothrow<ObjectImpl>()) {
    auto it = pobject->find(idx);
    if (it == pobject->end()) {
      throw std::out_of_range(to<std::string>(
          "couldn't find key ", idx.asString(), " in dynamic object"));
    }
    return it->second;
  } else {
    throw TypeError("object/array", type());
  }
}

bool dynamic::get(const dynamic& k, dynamic& v) const {
  if (auto* pobject = get_nothrow<ObjectImpl>()) {
    auto it = pobject->find(k);
    if (it != pobject->end()) {
      v = it->second;
      return true;
    }
  }
  return false;
}

bool dynamic::get(const dynamic& k, fbstring& v) const {
  dynamic d = nullptr;
  if (get(k, d) && d.isString()) {
    v = d.getString();
    return true;
  }
  return false;
}

#if FOLLY_DYNAMIC_EXTEND_DATA
bool dynamic::get(const dynamic& k, byte_string& v) const {
  dynamic d = nullptr;
  if (get(k, d) && d.isData()) {
    v = d.getData();
    return true;
  }
  return false;
}
#endif

bool dynamic::get(const dynamic& k, double& v) const {
  dynamic d = nullptr;
  if (get(k, d) && d.isDouble()) {
    v = d.getDouble();
    return true;
  }
  return false;
}

bool dynamic::get(const dynamic& k, int64_t& v) const {
  dynamic d = nullptr;
  if (get(k, d) && d.isInt()) {
    v = d.getInt();
    return true;
  }
  return false;
}

bool dynamic::get(const dynamic& k, bool& v) const {
  dynamic d = nullptr;
  if (get(k, d) && d.isBool()) {
    v = d.getBool();
    return true;
  }
  return false;
}

bool dynamic::getByKeyPath(const StringPiece& kpath, dynamic& v) const {
  std::vector<fbstring> ks;
  split('.', kpath, ks);

  const dynamic* d = this;
  for (auto& k : ks) {
    if (!d->isObject()) {
      return false;
    }
    auto it = d->find(k);
    if (it == d->items().end()) {
      return false;
    }
    d = &it->second;
  }
  v = *d;
  return true;
}

bool dynamic::getByKeyPath(const StringPiece& kpath, fbstring& v) const {
  dynamic d = nullptr;
  if (getByKeyPath(kpath, d) && d.isString()) {
    v = d.getString();
    return true;
  }
  return false;
}

#if FOLLY_DYNAMIC_EXTEND_DATA
bool dynamic::getByKeyPath(const StringPiece& kpath, byte_string& v) const {
  dynamic d = nullptr;
  if (getByKeyPath(kpath, d) && d.isData()) {
    v = d.getData();
    return true;
  }
  return false;
}
#endif

bool dynamic::getByKeyPath(const StringPiece& kpath, double& v) const {
  dynamic d = nullptr;
  if (getByKeyPath(kpath, d) && d.isDouble()) {
    v = d.getDouble();
    return true;
  }
  return false;
}

bool dynamic::getByKeyPath(const StringPiece& kpath, int64_t& v) const {
  dynamic d = nullptr;
  if (getByKeyPath(kpath, d) && d.isInt()) {
    v = d.getInt();
    return true;
  }
  return false;
}

bool dynamic::getByKeyPath(const StringPiece& kpath, bool& v) const {
  dynamic d = nullptr;
  if (getByKeyPath(kpath, d) && d.isBool()) {
    v = d.getBool();
    return true;
  }
  return false;
}

std::size_t dynamic::size() const {
  if (auto* ar = get_nothrow<Array>()) {
    return ar->size();
  }
  if (auto* obj = get_nothrow<ObjectImpl>()) {
    return obj->size();
  }
  if (auto* str = get_nothrow<fbstring>()) {
    return str->size();
  }
#if FOLLY_DYNAMIC_EXTEND_DATA
  if (auto* str = get_nothrow<byte_string>()) {
    return str->size();
  }
#endif
  throw TypeError("array/object", type());
}

dynamic::const_iterator
dynamic::erase(const_iterator first, const_iterator last) {
  auto& arr = get<Array>();
  return get<Array>().erase(
    arr.begin() + (first - arr.begin()),
    arr.begin() + (last - arr.begin()));
}

std::size_t dynamic::hash() const {
  switch (type()) {
  case OBJECT:
  case ARRAY:
  case NULLT:
    throw TypeError("not null/object/array", type());
  case INT64:
    return std::hash<int64_t>()(asInt());
  case DOUBLE:
    return std::hash<double>()(asDouble());
  case BOOL:
    return std::hash<bool>()(asBool());
  case STRING:
    return std::hash<fbstring>()(asString());
#if FOLLY_DYNAMIC_EXTEND_DATA
  case DATA:
    return std::hash<byte_string>()(asData());
#endif
  default:
    CHECK(0); abort();
  }
}

char const* dynamic::typeName(Type t) {
#define CR_X(T) return TypeInfo<T>::name
  CR_DYNAMIC_APPLY(t, CR_X);
#undef CR_X
}

void dynamic::destroy() noexcept {
  // This short-circuit speeds up some microbenchmarks.
  if (type_ == NULLT) return;

#define CR_X(T) detail::Destroy::destroy(getAddress<T>())
  CR_DYNAMIC_APPLY(type_, CR_X);
#undef CR_X
  type_ = NULLT;
  u_.nul = nullptr;
}

//////////////////////////////////////////////////////////////////////

}
