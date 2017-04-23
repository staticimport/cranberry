#pragma once

#include <ctype.h>
#include <iso646.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define ASSERT(check, ...) \
        do { \
          if (!(check)) { \
            fprintf(stderr, "Assertion `" #check "' failed. " __VA_ARGS__); \
            fputc('\n', stderr); \
            abort(); \
          } \
        } while (false) 

#define BZERO(ptr) memset(ptr, 0, sizeof(*(ptr)))

#define CONST_FCN __attribute__((const))
#define PURE_FCN  __attribute__((pure))

enum cbc_operator_type
{
  CBC_OP_INVALID,
  CBC_OP_ASSIGNMENT,
  CBC_OP_BRACKET_OPEN,
  CBC_OP_BRACKET_CLOSE,
  CBC_OP_COMP_EQ,
  CBC_OP_COMP_GE,
  CBC_OP_COMP_GT,
  CBC_OP_COMP_LE,
  CBC_OP_COMP_LT,
  CBC_OP_DOT,
  CBC_OP_FWDSLASH,
  CBC_OP_FWDSLASH_EQ,
  CBC_OP_MINUS,
  CBC_OP_MINUS_EQ,
  CBC_OP_PAREN_OPEN,
  CBC_OP_PAREN_CLOSE,
  CBC_OP_PLUS,
  CBC_OP_PLUS_EQ,
  CBC_OP_STAR,
  CBC_OP_STAR_EQ,
};


/* Functionality */
bool cbc_isnum(char) CONST_FCN;
bool cbc_isop(char)  CONST_FCN;
enum cbc_operator_type cbc_determine_op(char const*, size_t) PURE_FCN;


/* Implementation */
bool cbc_isnum(char c)
{
  return ((c >= '0') & (c <= '9'));
}

bool cbc_isop(char c)
{
  return (!isalpha(c)) & (!cbc_isnum(c)) & (!isspace(c)) &
         (c != '$') & (c != '@') & (c != '"') & (c != '\'');
}

enum cbc_operator_type cbc_determine_op(char const* s, size_t len)
{
  if (len == 1) {
    switch (s[0]) {
    case '=':   return CBC_OP_ASSIGNMENT;
    case '[':   return CBC_OP_BRACKET_OPEN;
    case ']':   return CBC_OP_BRACKET_CLOSE;
    case '>':   return CBC_OP_COMP_GT;
    case '<':   return CBC_OP_COMP_LT;
    case '.':   return CBC_OP_DOT;
    case '/':   return CBC_OP_FWDSLASH;
    case '-':   return CBC_OP_MINUS;
    case '(':   return CBC_OP_PAREN_OPEN;
    case ')':   return CBC_OP_PAREN_CLOSE;
    case '+':   return CBC_OP_PLUS;
    case '*':   return CBC_OP_STAR;
    default:    return CBC_OP_INVALID;
    }
  }
  else if (len == 2) {
    char const c1 = s[0]; char const c2 = s[1];
    if ((c1 == '=') & (c2 == '='))  { return CBC_OP_COMP_EQ; }
    if ((c1 == '>') & (c2 == '='))  { return CBC_OP_COMP_GE; }
    if ((c1 == '<') & (c2 == '='))  { return CBC_OP_COMP_LE; }
    if ((c1 == '/') & (c2 == '='))  { return CBC_OP_FWDSLASH_EQ; }
    if ((c1 == '-') & (c2 == '='))  { return CBC_OP_MINUS_EQ; }
    if ((c2 == '+') & (c2 == '='))  { return CBC_OP_PLUS_EQ; }
    if ((c2 == '*') & (c2 == '='))  { return CBC_OP_STAR_EQ; }
  }
  return CBC_OP_INVALID;
} 

