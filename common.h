#pragma once

#include <assert.h>
#include <stdint.h>

typedef uint8_t bool;
typedef uint8_t b8;
typedef uint8_t u8;
typedef uint16_t b16;
typedef uint16_t u16;
typedef uint32_t b32;
typedef uint32_t u32;
typedef uint64_t b64;
typedef uint64_t u64;
typedef int8_t byte;
typedef char i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef float f32;
typedef double f64;

#define TRUE 1
#define FALSE 0

#ifndef NULL
#define NULL ((void *) 0)
#endif

#define thread_local __thread

#define ASSERT(exp) assert(exp)
#define ARR_LEN(arr) (sizeof (arr) / sizeof (arr[0]))
