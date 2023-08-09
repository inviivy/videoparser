#pragma once

#include <stddef.h>
#include <stdint.h>

#include <type_traits>

namespace base {
template <typename T, size_t N>
constexpr size_t size(const T (&array)[N]) noexcept {
  return N;
}

namespace bits {
#define LIKELY(x) (x)

// CountLeadingZeroBits(value) returns the number of zero bits following the
// most significant 1 bit in |value| if |value| is non-zero, otherwise it
// returns {sizeof(T) * 8}.
// Example: 00100010 -> 2
//
// CountTrailingZeroBits(value) returns the number of zero bits preceding the
// least significant 1 bit in |value| if |value| is non-zero, otherwise it
// returns {sizeof(T) * 8}.
// Example: 00100010 -> 1
//
// C does not have an operator to do this, but fortunately the various
// compilers have built-ins that map to fast underlying processor instructions.
//
// TODO(pkasting): When C++20 is available, replace with std::countl_zero() and
// similar.

// __builtin_clz has undefined behaviour for an input of 0, even though there's
// clearly a return value that makes sense, and even though some processor clz
// instructions have defined behaviour for 0. We could drop to raw __asm__ to
// do better, but we'll avoid doing that unless we see proof that we need to.
template <typename T, int bits = sizeof(T) * 8>
constexpr
    typename std::enable_if<std::is_unsigned_v<T> && sizeof(T) <= 8, int>::type
    CountLeadingZeroBits(T value) {
  static_assert(bits > 0, "invalid instantiation");
  return LIKELY(value)
             ? bits == 64 ? __lzcnt64(static_cast<uint64_t>(value))
                          : __lzcnt(static_cast<uint32_t>(value)) - (32 - bits)
             : bits;
}

// Returns the integer i such as 2^i <= n < 2^(i+1).
//
// There is a common `BitLength` function, which returns the number of bits
// required to represent a value. Rather than implement that function,
// use `Log2Floor` and add 1 to the result.
//
// TODO(pkasting): When C++20 is available, replace with std::bit_xxx().
constexpr int Log2Floor(uint32_t n) { return 31 - CountLeadingZeroBits(n); }

// Returns the integer i such as 2^(i-1) < n <= 2^i.
constexpr int Log2Ceiling(uint32_t n) {
  // When n == 0, we want the function to return -1.
  // When n == 0, (n - 1) will underflow to 0xFFFFFFFF, which is
  // why the statement below starts with (n ? 32 : -1).
  return (n ? 32 : -1) - CountLeadingZeroBits(n - 1);
}

}; // namespace bits
} // namespace base
