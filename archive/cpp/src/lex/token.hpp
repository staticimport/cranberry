#pragma once

#include "common.hpp"
#include <vector>

namespace CBC {

/* functionality */
enum class TokenType
{
  // single-char tokens
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET,
  COMMA, DOT, COLON,

  // multi-char tokens
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL, RIGHT_SHIFT,
  LESS, LESS_EQUAL, LEFT_SHIFT, COMPARE,
  MINUS, MINUS_EQUAL, ARROW,
  PLUS, PLUS_EQUAL,
  STAR, STAR_EQUAL,
  SLASH, SLASH_EQUAL,

  // literals
  IDENTIFIER, STRING, INTEGER, FLOAT,

  // keywords
  AND, BOOL, CATCH, DEF, ELSE, ELSE_IF, FCN, FLT, IF, INT, NEXT, NIL,
  NO, NOT, OR, SELF, STR, SUPER, TRY, UNLESS, UNTIL, WHILE, YES, YIELD, VAL, VAR,
};

struct Token
{
  TokenType const   mType;
  std::string const mText;
  int const mLineNum;
  int const mColumn;

  Token(TokenType, int linenum, int col);
  Token(TokenType, std::string&& text, int linenum, int col);
};

struct TokenStream
{
public:
  TokenStream(std::vector<Token>&&);

  Token const* Peek(void) const;
  Token const* Pop(void);
  void         Push(void);
  Token const* TryPop(void); // TODO: do we need normal Pop?
private:
  std::vector<Token> mTokens;
  size_t mIndex;
};


/* implementation */
Token::Token(TokenType type, int linenum, int col)
  : mType(type), mText(), mLineNum(linenum), mColumn(col)
{}

Token::Token(TokenType type, std::string&& text, int linenum, int col)
  : mType(type), mText(text), mLineNum(linenum), mColumn(col)
{}

TokenStream::TokenStream(std::vector<Token>&& tokens)
  : mTokens(tokens), mIndex(0)
{}

PURE_FCN
Token const* TokenStream::Peek(void) const
{
  return mIndex < mTokens.size() ? &mTokens[mIndex] : nullptr;
}

Token const* TokenStream::Pop(void)
{
  return &mTokens.at(mIndex++);
}

void TokenStream::Push(void)
{
  ASSERT(mIndex > 0);
  mIndex -= 1;
}

Token const* TokenStream::TryPop(void)
{
  return mIndex < mTokens.size() ? &mTokens[mIndex++] : nullptr;
}

}

