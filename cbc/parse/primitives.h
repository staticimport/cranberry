#pragma once

#include "string.h"

enum cbc_primitive_type
{
  CBC_PRIMITIVE_FLT,
  CBC_PRIMITIVE_INT,
  CBC_PRIMITIVE_STRING,
  CBC_PRIMITIVE_VAR,
};

struct cbc_primitive
{
  struct cbc_string         text;
  enum   cbc_primitive_type type;
};

