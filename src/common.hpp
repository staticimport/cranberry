#pragma once

#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

#define ASSERT(check, ...) \
        do { \
          if (!(check)) { \
            throw std::runtime_error(CBC::FormatStr("Assertion `" #check "' failed. " __VA_ARGS__)); \
          } \
        } while (false)
#define DIE(...) do { throw std::runtime_error(CBC::FormatStr(__VA_ARGS__)); } while (false)

#ifndef NDEBUG
#define DEBUG_ASSERT(...) ASSERT(__VA_ARGS__)
#endif

#define BZERO(ptr) memset(ptr, 0, sizeof(*(ptr)))

#define CONST_FCN __attribute__((const))
#define PURE_FCN  __attribute__((pure))



namespace CBC
{
/* types */
struct ParseError : public std::runtime_error
{
  ParseError(std::string const& what) : std::runtime_error(what.c_str()) {}
};

/* functionality */
std::string FormatStr(char const* fmt, ...);


/* implementation */
std::string FormatStr(char const* fmt, ...)
{
  char* buf;
  va_list ap;
  va_start(ap, fmt);
  if (vasprintf(&buf, fmt, ap) <= 0) { abort(); } // can't use ASSERT here
  va_end(ap);
  std::string const s(buf);
  free(buf);
  return s;
}

}; // namespace CBC

