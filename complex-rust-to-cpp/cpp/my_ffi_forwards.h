#pragma once

#include <memory>
#include <cassert>
#include <cstdlib>
#include <span>

/*
 * Here go definitions that cbindgen-generated code needs, for one reason or
 * another
 */

namespace my_ffi {

// A layout-compatible (but not ABI-compatible!) version of Rust's Box.
//
// Basically, a non-null unique_ptr, where operator== and operator!= have value
// semantics.
template <typename T>
struct Box {
  Box() = delete; // For good measure.

  ~Box() {
    assert(m_raw);
    m_raw->~T();
    free(m_raw);
  }

  Box(const Box& other) {
    m_raw = (T*)malloc(sizeof(T));
    new (m_raw) T(*other);
  }

  Box& operator=(const Box& other) const {
    if (this != &other) {
      this->~Box();
      new (this) Box(other);
    }
    return *this;
  }

  const T* operator->() const {
    assert(m_raw);
    return m_raw;
  }

  const T& operator*() const {
    assert(m_raw);
    return *m_raw;
  }

  T* operator->() {
    assert(m_raw);
    return m_raw;
  }

  T& operator*() {
    assert(m_raw);
    return *m_raw;
  }

  bool operator==(const Box& other) const { return *(*this) == *other; }
  bool operator!=(const Box& other) const { return *(*this) != *other; }

 private:
  T* m_raw;
};

} // namespace my_ffi
