#pragma once

#include "cbc/common.h"

char* cbc_load_file(char const* filename, size_t* len);


/* implementation */
char* cbc_load_file(char const* filename, size_t* len)
{
  // load file
  FILE* f = fopen(filename, "r");
  if (NULL == f) { DIE("Unable to open '%s'", filename); }

  // determine length
  fseek(f, 0, SEEK_END);
  *len = (size_t)ftell(f);
  rewind(f);

  // read into buffer
  char* buffer = malloc(*len);
  ASSERT(*len == fread(buffer, 1, *len, f));
  fclose(f);
  return buffer;
}
