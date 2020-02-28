#pragma once

#include "my_ffi.h"

/*
 * Here goes the implementation of the methods and such that we define in
 * cbindgen.toml.
 */

namespace my_ffi {

template <typename T>
void OwnedSlice<T>::Clear() {
  // This assumes that your C++ code and your Rust code share allocators,
  // otherwise it should do something else (call into rust to call drop_in_place
  // or something).
  if (!len)
    return;
  for (auto& val : AsSpan())
    val.~T();
  free(ptr);
  ptr = (T*)alignof(T);
  len = 0;
}

template <typename T>
void OwnedSlice<T>::CopyFrom(const OwnedSlice& other) {
  Clear();

  auto other_span = other.AsSpan();
  if (other_span.empty())
    return;

  ptr = (T*)malloc(sizeof(T) * other_span.size());
  len = other_span.size();

  size_t i = 0;
  for (auto& elem : other_span)
    new (ptr + i++) T(elem);
}

template <typename T>
OwnedSlice<T>::OwnedSlice()
  : ptr((T*)alignof(T))
  , len(0)
{}

template <typename T>
OwnedSlice<T>::~OwnedSlice() {
  Clear();
}

template <typename T>
OwnedSlice<T>::OwnedSlice(const OwnedSlice& other) : OwnedSlice() {
  CopyFrom(other);
}

template <typename T>
OwnedSlice<T>& OwnedSlice<T>::operator=(const OwnedSlice& other) {
  if (this != &other)
    CopyFrom(other);
  return *this;
}

template <typename T>
bool OwnedSlice<T>::operator==(const OwnedSlice& other) const {
  // Apparently span<T> doesn't implement operator==?
  auto self_span = AsSpan();
  auto other_span = other.AsSpan();
  if (self_span.size() != other_span.size())
    return false;
  for (size_t i = 0; i < self_span.size(); ++i)
    if (self_span[i] != other_span[i])
      return false;
  return true;
}

template <typename T>
bool OwnedSlice<T>::operator!=(const OwnedSlice& other) const {
  return !(*this == other);
}

}
