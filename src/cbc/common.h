#pragma once

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define ASSERT(check, ...) \
        do { \
          if (!(check)) { \
            DIE("Assertion '" #check "' failed. " __VA_ARGS__); \
          } \
        } while (false)

#define BZERO(ptr) do { void* ptr_ = (ptr); memset(ptr_, 0, sizeof(*(ptr))); } while (false)

#define DIE(...) \
        do { \
          fprintf(stderr, "%s:%d ", __FILE__, __LINE__); \
          fprintf(stderr, __VA_ARGS__); \
          fprintf(stderr, "\n"); \
          abort(); \
        } while (false)


// attributes
#define CONST_FCN     __attribute__((const))
#define NOINLINE_FCN  __attribute__((noinline))
#define PURE_FCN      __attribute__((pure))
