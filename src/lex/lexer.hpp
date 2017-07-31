#pragma once

#include "lex/token.hpp"

namespace CBC {

/* functionality */
class Lexer
{
public:
  TokenStream Lex(CharStream&);
private:
  typedef std::vector<Token> Tokens;

  void NextToken(CharStream&, Tokens&);

  int mLineNum;
  int mColNum;
};


/* implementation */
TokenStream Lexer::Lex(CharStream& cs)
{
  Tokens tokens; tokens.reserve(cs.TotalSize() / 3); // approximation
  mLineNum = 1; mColNum = 0;
  while (cs.CanPop()) { NextToken(cs, tokens); }
  return TokenStream(std::move(tokens));
}

void Lexer::NextToken(CharStream& cs, Tokens& tokens)
{
  char const c = cs.Pop(); ++mColNum;
  switch (c)
  {
  // single-char operators
  case '(':   tokens.emplace_back(TokenType::LEFT_PAREN,  mLineNum, mColNum); break;
  case ')':   tokens.emplace_back(TokenType::RIGHT_PAREN, mLineNum, mColNum); break;
  case '{':   tokens.emplace_back(TokenType::LEFT_BRACE,  mLineNum, mColNum); break;
  case '}':   tokens.emplace_back(TokenType::RIGHT_BRACE, mLineNum, mColNum); break;
  case ',':   tokens.emplace_back(TokenType::COMMA,       mLineNum, mColNum); break;
  case '.':   tokens.emplace_back(TokenType::DOT,         mLineNum, mColNum); break;

  // multi-char operators
  case '!':   if (cs.TryPop('=')) {
                tokens.emplace_back(TokenType::BANG_EQUAL,  mLineNum, mColNum);
                ++mColNum;
              } else { tokens.emplace_back(TokenType::BANG, mLineNum, mColNum); }
              break;
  case '=':   if (cs.TryPop('=')) {
                tokens.emplace_back(TokenType::EQUAL_EQUAL,  mLineNum, mColNum);
                ++mColNum;
              } else { tokens.emplace_back(TokenType::EQUAL, mLineNum, mColNum); }
              break;
  case '>':   if (cs.TryPop('=')) {
                tokens.emplace_back(TokenType::GREATER_EQUAL,  mLineNum, mColNum);
                ++mColNum;
              } else if (cs.TryPop('>')) {
                tokens.emplace_back(TokenType::RIGHT_SHIFT,    mLineNum, mColNum);
                ++mColNum;
              } else { tokens.emplace_back(TokenType::GREATER, mLineNum, mColNum); }
              break;
  case '<':   if (cs.TryPop('=')) {
                if (cs.TryPop('>')) {
                  tokens.emplace_back(TokenType::COMPARE,  mLineNum, mColNum);
                  mColNum += 2;
                } else {
                  tokens.emplace_back(TokenType::LESS_EQUAL,  mLineNum, mColNum);
                  ++mColNum;
                }
              } else if (cs.TryPop('<')) {
                tokens.emplace_back(TokenType::LEFT_SHIFT,    mLineNum, mColNum);
                ++mColNum;
              } else { tokens.emplace_back(TokenType::LESS, mLineNum, mColNum); }
              break;
  case '-':   if (cs.TryPop('=')) {
                tokens.emplace_back(TokenType::MINUS_EQUAL,  mLineNum, mColNum);
                ++mColNum;
              } else if (cs.TryPop('>')) {
                tokens.emplace_back(TokenType::ARROW,    mLineNum, mColNum);
                ++mColNum;
              } else { tokens.emplace_back(TokenType::MINUS, mLineNum, mColNum); }
              break;
  case '+':   if (cs.TryPop('=')) {
                tokens.emplace_back(TokenType::PLUS_EQUAL,  mLineNum, mColNum);
                ++mColNum;
              } else { tokens.emplace_back(TokenType::PLUS, mLineNum, mColNum); }
              break;
  case '*':   if (cs.TryPop('=')) {
                tokens.emplace_back(TokenType::STAR_EQUAL,  mLineNum, mColNum);
                ++mColNum;
              } else { tokens.emplace_back(TokenType::STAR, mLineNum, mColNum); }
              break;
  case '/':   if (cs.TryPop('=')) {
                tokens.emplace_back(TokenType::SLASH_EQUAL,  mLineNum, mColNum);
                ++mColNum;
              } else { tokens.emplace_back(TokenType::SLASH, mLineNum, mColNum); }
              break;
  default:    DIE("invalid token 0x%02x = '%c'", c, c);
  }
}

}

