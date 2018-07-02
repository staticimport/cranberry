#pragma once

#include "cbc/tokens.h"

struct cbc_ast_function
{
  struct cbc_token* function_name;
  struct cbc_token* return_type;
  struct cbc_token* return_value;
};

struct cbc_ast
{
  struct cbc_ast_function main_function;
};
