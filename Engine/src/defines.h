#pragma once


//TODO define engine specific types


//Static Assertions


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
