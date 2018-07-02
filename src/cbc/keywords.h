#pragma once

#include "cbc/common.h"

enum cbc_keyword_type
{
  CBC_KEYWORD_FCN,
  CBC_KEYWORD_I32,
  CBC_KEYWORD_RETURN,
};

bool cbc_determine_keyword(char const* s, size_t len, enum cbc_keyword_type* type_out);
bool cbc_keyword_is_type(enum cbc_keyword_type);


/* implementation */
bool cbc_determine_keyword(char const* s, size_t len, enum cbc_keyword_type* type_out)
{
  switch (len)
  {
  case 3:
    if      (0 == memcmp(s, "fcn", 3))  { *type_out = CBC_KEYWORD_FCN;    return true; }
    else if (0 == memcmp(s, "i32", 3))  { *type_out = CBC_KEYWORD_I32;    return true; }
    return false;
  case 6:
    if (0 == memcmp(s, "return", 6))    { *type_out = CBC_KEYWORD_RETURN; return true; }
    return false;
  default:
    return false;
  }
}

bool cbc_keyword_is_type(enum cbc_keyword_type type)
{
  switch (type)
  {
  case CBC_KEYWORD_I32:
    return true;
  case CBC_KEYWORD_FCN:
  case CBC_KEYWORD_RETURN:
    return false;
  default:
    abort();
  };
}
