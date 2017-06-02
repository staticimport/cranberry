#pragma once

#include "tokenizer.h"
#include "parse/primitives.h"

enum cbc_expression_component_type
{
  CBC_EXPR_COMP_PRIMITIVE,
  CBC_EXPR_COMP_OPERATOR,
  CBC_EXPR_COMP_FCN_INVOC,
};

struct cbc_expression_component
{
  union {
    struct cbc_primitive primitive;
    struct cbc_string    operator;
    struct cbc_function_invocation fcn_invocation;
  } value;
  enum cbc_expression_component_type type;
};

struct cbc_expression
{
  CBC_DYN_ARRAY(components, struct cbc_expression_component);
};


/* functionality */
void cbc_expression_parse(struct cbc_expression*, struct cbc_tokenizer*);


/* implementation */
void cbc_expression_parse(struct cbc_expression* exp, struct cbc_tokenizer* tzr)
{
}

