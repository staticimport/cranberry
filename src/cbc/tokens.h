#pragma once

#include "cbc/keywords.h"
#include "cbc/operators.h"

enum cbc_token_type
{
  CBC_TOKEN_IDENTIFIER,
  CBC_TOKEN_INTEGER,
  CBC_TOKEN_KEYWORD,
  CBC_TOKEN_OPERATOR,
};

struct cbc_token
{
  enum cbc_token_type type;
  int                 subtype; // cbc_keyword_type or cbc_operator_type, if applicable
  char const*         text;
  size_t              textlen;
};

struct cbc_token* cbc_tokenize(char const* source, size_t source_len, size_t* num_tokens_out);


/* implementation */
struct cbc_token_list
{
  struct cbc_token* tokens;
  struct cbc_token* next_token;
  struct cbc_token* tokens_end;
};

struct cbc_token_list cbc_token_list_create(void)
{
  static size_t const INITIAL_TOKENS_CAPACITY = 1024;
  struct cbc_token_list tkl;
  tkl.tokens      = malloc(sizeof(struct cbc_token) * INITIAL_TOKENS_CAPACITY);
  tkl.next_token  = tkl.tokens;
  tkl.tokens_end  = tkl.tokens + INITIAL_TOKENS_CAPACITY;
  return tkl;
}

__attribute__((noinline))
void cbc_token_list_expand(struct cbc_token_list* tkl)
{
  size_t const old_capacity = tkl->tokens_end - tkl->tokens;
  size_t const new_capacity = old_capacity * 2;
  size_t const length       = tkl->next_token - tkl->tokens;
  tkl->tokens               = (struct cbc_token*)realloc(tkl->tokens, sizeof(struct cbc_token) * new_capacity);
  tkl->next_token           = tkl->tokens + length;
  tkl->tokens_end           = tkl->tokens + new_capacity;
}

struct cbc_token* cbc_token_list_next(struct cbc_token_list* tkl)
{
  if (tkl->next_token == tkl->tokens_end) { cbc_token_list_expand(tkl); }
  return tkl->next_token++;
}

char const* cbc_tokenize_integer(struct cbc_token_list* tkl, char const* source, char const* source_end)
{
  struct cbc_token* const tk = cbc_token_list_next(tkl);
  tk->type    = CBC_TOKEN_INTEGER;
  tk->subtype = -1;
  tk->text    = source;
  tk->textlen = 0;
  while (source < source_end && isdigit(*source)) { ++source; ++tk->textlen; }
  return source;
}

char const* cbc_tokenize_identifier_or_keyword(struct cbc_token_list* tkl, char const* source, char const* source_end)
{
  struct cbc_token* const tk = cbc_token_list_next(tkl);
  tk->type    = CBC_TOKEN_IDENTIFIER;
  tk->subtype = -1;
  tk->text    = source;
  tk->textlen = 0;
  while (source < source_end && isalnum(*source)) { ++source; ++tk->textlen; }

  // check if it's actually a keyword
  if (cbc_determine_keyword(tk->text, tk->textlen, (enum cbc_keyword_type*)&tk->subtype))
  {
    tk->type = CBC_TOKEN_KEYWORD;
  }

  return source;
}

char const* cbc_tokenize_operator(struct cbc_token_list* tkl, char const* source, char const* source_end)
{
  struct cbc_token* const tk = cbc_token_list_next(tkl);
  tk->type    = CBC_TOKEN_OPERATOR;
  tk->subtype = -1;
  tk->text    = source;
  tk->textlen = 0;
  while (source < source_end && isprint(*source) && !isalnum(*source)) { ++source; ++tk->textlen; }
  size_t const max_textlen = tk->textlen;

  // attempt to match longest operator
  while (tk->textlen > 0 && !cbc_determine_operator(tk->text, tk->textlen, (enum cbc_operator_type*)&tk->subtype))
  {
    --tk->textlen;
    --source;
  }

  (void)max_textlen; // TODO
  if (tk->textlen == 0) { DIE("failed to parse operator from '%.*s'", (int)max_textlen, tk->text); }

  return source;
}

struct cbc_token* cbc_tokenize(char const* source, size_t source_len, size_t* num_tokens_out)
{
  struct cbc_token_list tkl = cbc_token_list_create();
  char const*       source_next = source;
  char const* const source_end  = source + source_len;
  while (source_next < source_end)
  {
    char const c = *source_next;
    if      (isspace(c)) { ++source_next; }
    else if (isdigit(c)) { source_next = cbc_tokenize_integer(&tkl, source_next, source_end); }
    else if (isalpha(c)) { source_next = cbc_tokenize_identifier_or_keyword(&tkl, source_next, source_end); }
    else                 { source_next = cbc_tokenize_operator(&tkl, source_next, source_end); }
  }

  // return
  *num_tokens_out = tkl.next_token - tkl.tokens;
  return tkl.tokens;
}

