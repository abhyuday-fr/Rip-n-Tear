#ifndef BASE_H
#define BASE_H

/*
 * base.h uses short, intuitive type names used throughout the project
 */

#include <cstddef>
#include <cstdint>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;

using usize = size_t;
using isize = ptrdiff_t;

// catch a weird platform/ABI before it causes a silent miscompile somewhere
// deep in the interpreter
static_assert(sizeof(u8) == 1 && sizeof(i8) == 1, "byte size mismatch");
static_assert(sizeof(u16) == 2 && sizeof(i16) == 2, "halfword size mismatch");
static_assert(sizeof(u32) == 4 && sizeof(i32) == 4, "word size mismatch");
static_assert(sizeof(u64) == 8 && sizeof(i64) == 8, "doubleword size mismatch");
static_assert(sizeof(f32) == 4, "float must be 32-bit (IEEE 754 single)");
static_assert(sizeof(f64) == 8, "double must be 64-bit (IEEE 754 double)");

// Typical x86-64/ARM64 cache line size
constexpr usize CACHE_LINE_SIZE = 64;

#endif
