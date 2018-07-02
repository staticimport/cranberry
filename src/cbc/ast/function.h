#pragma once

#include "cbc/ast/types.h"

struct cbc_token* cbc_ast_function_parse(struct cbc_ast_function*, struct cbc_token* tokens,
                                         struct cbc_token* tokens_end);


/* implementation */
struct cbc_token* cbc_ast_function_parse(struct cbc_ast_function* fcn, struct cbc_token* tokens,
                                         struct cbc_token* tokens_end)
{
  struct cbc_token* tk = tokens;

  // fcn keyword, should have already been checked before now
  ASSERT(tk < tokens_end && tk->type == CBC_TOKEN_KEYWORD && tk->subtype == CBC_KEYWORD_FCN);
  ++tk;

  // function name
  ASSERT(tk < tokens_end && tk->type == CBC_TOKEN_IDENTIFIER);
  fcn->function_name = tk;
  ++tk;

  // operators, no args supported now
  ASSERT(tk < tokens_end && tk->type == CBC_TOKEN_OPERATOR && tk->subtype == CBC_OPERATOR_PAREN_OPEN);
  ++tk;
  ASSERT(tk < tokens_end && tk->type == CBC_TOKEN_OPERATOR && tk->subtype == CBC_OPERATOR_PAREN_CLOSE);
  ++tk;
  ASSERT(tk < tokens_end && tk->type == CBC_TOKEN_OPERATOR && tk->subtype == CBC_OPERATOR_COLON);
  ++tk;

  // return type
  ASSERT(tk < tokens_end && (tk->type == CBC_TOKEN_IDENTIFIER ||
                            (tk->type == CBC_TOKEN_KEYWORD && cbc_keyword_is_type(tk->subtype))));
  fcn->return_type = tk;
  ++tk;

  // open brace
  ASSERT(tk < tokens_end && tk->type == CBC_TOKEN_OPERATOR && tk->subtype == CBC_OPERATOR_BRACE_OPEN);
  ++tk;

  // statements, only single return supported now
  ASSERT(tk < tokens_end && tk->type == CBC_TOKEN_KEYWORD && tk->subtype == CBC_KEYWORD_RETURN);
  ++tk;

  // return value
  ASSERT(tk < tokens_end && tk->type == CBC_TOKEN_INTEGER);
  fcn->return_value = tk;
  ++tk;

  // operators
  ASSERT(tk < tokens_end && tk->type == CBC_TOKEN_OPERATOR && tk->subtype == CBC_OPERATOR_SEMICOLON);
  ++tk;
  ASSERT(tk < tokens_end && tk->type == CBC_TOKEN_OPERATOR && tk->subtype == CBC_OPERATOR_BRACE_CLOSE);
  ++tk;

  return tk;
}
