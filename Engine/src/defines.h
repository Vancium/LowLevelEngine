#pragma once


//Engine Types

//Unsigned integer values
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

//Signed integer values
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

//Floating point types
typedef float f32;
typedef double f64;

//Boolean types
typedef int b32;
typedef char b8;


//Static Assertions
#if defined(__clang__) || defined(__gcc__)
#define STATIC_ASSERT _Static_assert
#else
#define STATIC_ASSERT static_assert
#endif


//Make sure custom types are of right length

STATIC_ASSERT(sizeof(u8) == 1, "u8 should be 1 byte long.");
STATIC_ASSERT(sizeof(u16) == 2, "u16 should be 2 bytes long.");
STATIC_ASSERT(sizeof(u32) == 4, "u32 should be 4 bytes long.");
STATIC_ASSERT(sizeof(u64) == 8, "u64 should be 8 bytes long.");

STATIC_ASSERT(sizeof(i8) == 1, "i8 should be 1 byte long.");
STATIC_ASSERT(sizeof(i16) == 2, "i16 should be 2 bytes long.");
STATIC_ASSERT(sizeof(i32) == 4, "i32 should be 4 bytes long.");
STATIC_ASSERT(sizeof(i64) == 8, "i64 should be 8 bytes long.");

STATIC_ASSERT(sizeof(f32) == 4, "f32 should be 4 bytes long");
STATIC_ASSERT(sizeof(f64) == 8, "f64 should be 8 bytes long");

#define TRUE 1
#define FALSE 0

#ifdef LLE_EXPORT
// Exports
#ifdef _MSC_VER
#define LLAPI __declspec(dllexport)
#else
#define LLAPI __attribute__((visibility("default")))
#endif
#else
// Imports
#ifdef _MSC_VER
#define LLAPI __declspec(dllimport)
#else
#define LLAPI
#endif
#endif
