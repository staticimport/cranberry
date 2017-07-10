#pragma once

#include "tokenizer.h"
#include "parse/primitives.h"

struct cbc_expression;

struct cbc_unary_expression
{
  struct cbc_operator_type  operator;
  struct cbc_expression*    sub_expression;
};

struct cbc_binary_expression
{
  enum   cbc_operator_type  operator;
  struct cbc_expression*    left;
  struct cbc_expression*    right;
};

struct cbc_fcn_invoc_expression
{
  struct cbc_string fcn_name;
  CBC_DYN_ARRAY(args, struct cbc_expression*);
};

enum cbc_expression_type
{
  CBC_EXPR_TYPE_UNARY,
  CBC_EXPR_TYPE_BINARY,
  CBC_EXPR_TYPE_IDENTIFIER,
  CBC_EXPR_TYPE_FLT,
  CBC_EXPR_TYPE_INT,
  CBC_EXPR_TYPE_STR,
  CBC_EXPR_TYPE_FCN_INVOC,
};

struct cbc_expression
{
  union {
    struct cbc_unary_expression     unary;
    struct cbc_binary_expression    binary;
    struct cbc_string               identifier_token;
    struct cbc_string               flt_token;
    struct cbc_string               int_token;
    struct cbc_string               str_token;
    struct cbc_fcn_invoc_expression fcn_invoc;
  } types;
  enum cbc_expression_type type;
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


/* functionality */
void cbc_expression_parse(struct cbc_expression*, struct cbc_tokenizer*);


/* implementation */
void cbc_expression_parse(struct cbc_expression* exp, struct cbc_tokenizer* tzr)
{
  /*struct cbc_token* t;

  t = cbc_tokenizer_next_or_die(tzr, "missing expression (1)");
  switch (t->type)
  {
  case CBC_TOKEN_IDENTIFIER:
  }*/
}

