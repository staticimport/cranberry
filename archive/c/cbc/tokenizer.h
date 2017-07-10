#pragma once

#include "file_reader.h"

enum cbc_token_type
{
  CBC_TOKEN_INVALID    = '?',
  CBC_TOKEN_IDENTIFIER = 'I',
  CBC_TOKEN_NUMBER     = 'N',
  CBC_TOKEN_OPERATOR   = 'O',
  CBC_TOKEN_STRING     = 'S',
};

struct cbc_token
{
  char const*             text;
  int                     textlen;
  enum cbc_token_type     type;
  // TODO: here? enum cbc_operator_type  optype;  // iff type == CBC_TOKEN_OPERATOR
};

struct cbc_tokenizer
{
  struct cbc_file_reader* file_reader;
  char*  buffer;
  int    next_colnum;
  struct cbc_token token;
  bool   has_retreated;
};



/* Functionality */
void    cbc_tokenizer_init(struct cbc_tokenizer*);
void    cbc_tokenizer_uninit(struct cbc_tokenizer*);
void    cbc_tokenizer_start(struct cbc_tokenizer*, struct cbc_file_reader*);
struct  cbc_token* cbc_tokenizer_next(struct cbc_tokenizer*);
struct  cbc_token* cbc_tokenizer_next_or_die(struct cbc_tokenizer*, char const* error_msg);
void    cbc_tokenizer_retreat(struct cbc_tokenizer*);


/* Implementation */
#define CBC_MAX_TOKEN_LEN 8192
void cbc_tokenizer_init(struct cbc_tokenizer* tz)
{
  tz->file_reader   = NULL;
  tz->buffer        = (char*)malloc(CBC_MAX_TOKEN_LEN);
  tz->has_retreated = false;
}

void cbc_tokenizer_uninit(struct cbc_tokenizer* tz)
{
  free(tz->buffer);
}

void cbc_tokenizer_start(struct cbc_tokenizer* tz, struct cbc_file_reader* file_reader)
{
  tz->file_reader   = file_reader;
  tz->next_colnum   = 1;
  tz->has_retreated = false;
}

void cbc_tokenizer_next_identifier(struct cbc_tokenizer* tz, struct cbc_token* out)
{
  char c; char* tail = tz->buffer;
  cbc_file_reader_peek_and_pop(tz->file_reader, tail++);
  while (cbc_file_reader_peek(tz->file_reader, &c) and (isalpha(c) | (c == '_') | (c == '?')))
  {
    *(tail++) = c;
    cbc_file_reader_pop(tz->file_reader);
  }

  out->text     = tz->buffer;
  out->textlen  = (int)(tail - tz->buffer);
  out->type     = CBC_TOKEN_IDENTIFIER;
}

void cbc_tokenizer_next_number(struct cbc_tokenizer* tz, struct cbc_token* out)
{
  char c; char* tail = tz->buffer;
  cbc_file_reader_peek_and_pop(tz->file_reader, tail++);
  while (cbc_file_reader_peek(tz->file_reader, &c) and 
        (cbc_isnum(c) | (c == 'e') | (c == 'x') | (c == '.') | (c == '-') |
        ((c >= 'a') & (c <= 'f')) | ((c >= 'A') & (c <= 'F'))))
  {
    *(tail++) = c;
    cbc_file_reader_pop(tz->file_reader);
  }

  out->text     = tz->buffer;
  out->textlen  = (int)(tail - tz->buffer);
  out->type     = CBC_TOKEN_NUMBER;
}

void cbc_tokenizer_next_string(struct cbc_tokenizer* tz, struct cbc_token* out)
{
  char terminator = '?'; char c; bool is_escaped = false; char* tail = tz->buffer;
  cbc_file_reader_peek_and_pop(tz->file_reader, &terminator);
  *(tail++) = terminator;
  while (true)
  {
    ASSERT(cbc_file_reader_peek_and_pop(tz->file_reader, &c));
    *(tail++) = c;
    if ((c == terminator) & (!is_escaped)) { break; }
    is_escaped = (c == '\\') ? !is_escaped : false;
  }

  out->text     = tz->buffer;
  out->textlen  = (int)(tail - tz->buffer);
  out->type     = CBC_TOKEN_STRING;
}

void cbc_tokenizer_next_operator(struct cbc_tokenizer* tz, struct cbc_token* out)
{
  char c; char* tail = tz->buffer;
  while (cbc_file_reader_peek(tz->file_reader, &c) and cbc_isop(c))
  {
    *(tail++) = c;
    cbc_file_reader_pop(tz->file_reader);
  }

  out->text     = tz->buffer;
  out->textlen  = (int)(tail - tz->buffer);
  out->type     = CBC_TOKEN_OPERATOR;
  DEBUG_ASSERT(out->textlen > 0);
}

void cbc_tokenizer_skip_line_comment(struct cbc_tokenizer* tz)
{
  cbc_file_reader_pop(tz->file_reader); // '#'
  char c;
  while (cbc_file_reader_peek(tz->file_reader, &c) and (c != '\n')) 
  { 
    cbc_file_reader_pop(tz->file_reader);
  }
}

struct cbc_token* cbc_tokenizer_next(struct cbc_tokenizer* tz)
{
  bool const previously_retreated = tz->has_retreated;
  tz->has_retreated = false;
  if (previously_retreated) { return &tz->token; }

  char c; bool peek_ok;
  CURRENT_COLNUM = tz->next_colnum;

next: // <-- used for comments
  // find first non-whitespace char
  c = '?'; 
  while (true) {
    peek_ok = cbc_file_reader_peek(tz->file_reader, &c);
    if (peek_ok & (isspace(c) != 0))
    { 
      cbc_file_reader_pop(tz->file_reader);
      if (c == '\n') { CURRENT_LINENUM += 1; CURRENT_COLNUM = 1; }
      else           { CURRENT_COLNUM  += 1; }
    }
    else { break; }
  }
  if (!peek_ok) { return NULL; }
  DEBUG_ASSERT(!isspace(c));
  
  // first char should dictate token type
  if (isalpha(c) | (c == '@') | (c == '$')) { cbc_tokenizer_next_identifier(tz, &tz->token); }
  else if (cbc_isnum(c))                    { cbc_tokenizer_next_number(tz, &tz->token); }
  else if ((c == '"') | (c == '\''))        { cbc_tokenizer_next_string(tz, &tz->token); }
  else if (c == '#')                        { cbc_tokenizer_skip_line_comment(tz); goto next; }
  else                                      { cbc_tokenizer_next_operator(tz, &tz->token); }
  tz->next_colnum = CURRENT_COLNUM + tz->token.textlen;
  return &tz->token;
}

struct cbc_token* cbc_tokenizer_next_or_die(struct cbc_tokenizer* tz, char const* error_msg)
{
  struct cbc_token* const t = cbc_tokenizer_next(tz);
  ASSERT(t != NULL, error_msg);
  return t;
}

void cbc_tokenizer_retreat(struct cbc_tokenizer* tz)
{
  ASSERT(!tz->has_retreated, "cannot retreat tokenizer repeatedly");
  tz->has_retreated = true;
}

