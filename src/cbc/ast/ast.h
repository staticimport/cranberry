#pragma once

#include "cbc/ast/function.h"
#include "cbc/ast/types.h"

void cbc_ast_parse(struct cbc_ast*, struct cbc_token* tokens, struct cbc_token* tokens_end);


/* implementation */
void cbc_ast_parse(struct cbc_ast* ast, struct cbc_token* tokens, struct cbc_token* tokens_end)
{
  ASSERT(tokens < tokens_end, "No tokens to parse");
  BZERO(ast);
  ASSERT(tokens[0].type == CBC_TOKEN_KEYWORD && tokens[0].subtype == CBC_KEYWORD_FCN);
  tokens = cbc_ast_function_parse(&ast->main_function, tokens, tokens_end);
  ASSERT(tokens == tokens_end, "Unexpected tokens found after main function");
}
