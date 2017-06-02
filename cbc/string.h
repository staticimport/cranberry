#pragma once

#include "common.h"

/* types */
struct cbc_string
{
  union {
    char  local[15];
    char* ptr;
  } data;
  bool is_local;
};

/* functionality */
struct cbc_string cbc_string_create(char const* text, size_t len);
void  cbc_string_destroy(struct cbc_string);
char* cbc_str2cstr(struct cbc_string s);

/* implementation */
struct cbc_string cbc_string_create(char const* text, size_t len)
{
  struct cbc_string s;
  if (len < sizeof(s.data.local))
  {
    memcpy(s.data.local, text, len);
    s.data.local[len] = '\0';
    s.is_local = true;
  }
  else
  {
    s.data.ptr = strndup(text, len);
    s.is_local = false;
  }
}

void cbc_string_destroy(struct cbc_string s)
{
  if (!s.is_local) { free(s.data.ptr); }
}

char* cbc_str2cstr(struct cbc_string s);
