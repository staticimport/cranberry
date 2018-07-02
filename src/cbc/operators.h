#pragma once

#include "cbc/common.h"

enum cbc_operator_type
{
  CBC_OPERATOR_BRACE_OPEN,
  CBC_OPERATOR_BRACE_CLOSE,
  CBC_OPERATOR_COLON,
  CBC_OPERATOR_PAREN_OPEN,
  CBC_OPERATOR_PAREN_CLOSE,
  CBC_OPERATOR_SEMICOLON,
};

bool cbc_determine_operator(char const* s, size_t len, enum cbc_operator_type* type_out);


/* implementation */
bool cbc_determine_operator1(char c, enum cbc_operator_type* type_out)
{
  switch (c)
  {
  case '{': *type_out = CBC_OPERATOR_BRACE_OPEN;  return true;
  case '}': *type_out = CBC_OPERATOR_BRACE_CLOSE; return true;
  case ':': *type_out = CBC_OPERATOR_COLON;       return true;
  case '(': *type_out = CBC_OPERATOR_PAREN_OPEN;  return true;
  case ')': *type_out = CBC_OPERATOR_PAREN_CLOSE; return true;
  case ';': *type_out = CBC_OPERATOR_SEMICOLON;   return true;
  default:  return false;
  }
}

bool cbc_determine_operator(char const* s, size_t len, enum cbc_operator_type* type_out)
{
  if (1 == len) { return cbc_determine_operator1(*s, type_out); }
  return false;
}
