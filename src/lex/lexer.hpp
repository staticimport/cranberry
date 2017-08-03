#pragma once

#include "lex/token.hpp"

#include <unordered_map>

namespace CBC {

/* functionality */
class Lexer
{
public:
  TokenStream Lex(CharStream&);
private:
  typedef std::vector<Token> Tokens;
  static bool DetermineKeyword(std::string const&, TokenType* out);

  void NextToken(CharStream&, Tokens&);
  void ParseKeywordOrIdentifier(CharStream&, Tokens&);
  void ParseNumber(CharStream&, Tokens&);
  void ParseString(CharStream&, char terminator, Tokens&);
  void SkipToNextLine(CharStream&);

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
  case '(':   tokens.emplace_back(TokenType::LEFT_PAREN,    mLineNum, mColNum); break;
  case ')':   tokens.emplace_back(TokenType::RIGHT_PAREN,   mLineNum, mColNum); break;
  case '{':   tokens.emplace_back(TokenType::LEFT_BRACE,    mLineNum, mColNum); break;
  case '}':   tokens.emplace_back(TokenType::RIGHT_BRACE,   mLineNum, mColNum); break;
  case '[':   tokens.emplace_back(TokenType::LEFT_BRACKET,  mLineNum, mColNum); break;
  case ']':   tokens.emplace_back(TokenType::RIGHT_BRACKET, mLineNum, mColNum); break;
  case ',':   tokens.emplace_back(TokenType::COMMA,         mLineNum, mColNum); break;
  case '.':   tokens.emplace_back(TokenType::DOT,           mLineNum, mColNum); break;
  case ':':   tokens.emplace_back(TokenType::COLON,         mLineNum, mColNum); break;
  case '#':   SkipToNextLine(cs); break;

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

  // whitespace
  case ' ':   break;
  case '\t':  break;
  case '\r':  break;
  case '\n':  mLineNum++; mColNum = 0; break;

  // misc
  case '"':   ParseString(cs, '"',  tokens); break;
  case '\'':  ParseString(cs, '\'', tokens); break;
  default:
    cs.Push(); --mColNum;
    if      (isdigit(c)) { ParseNumber(cs, tokens); break; }
    else if (isalpha(c) | (c == '@') | (c == '$')) { ParseKeywordOrIdentifier(cs, tokens); break; };
    DIE("invalid token at %d:%d 0x%02x = '%c'", mLineNum, mColNum, c, c); // TODO: filename
  }
}

void Lexer::ParseKeywordOrIdentifier(CharStream& cs, Tokens& tokens)
{
  std::ostringstream oss;
  int const startColNum = mColNum + 1;
  char c;
  while (cs.Peek(&c))
  {
    if (isalpha(c) | (c == '_') | (c == '?') |
       (startColNum == (mColNum + 1) & ((c == '@') | (c == '$')))) // starting char only
    {
      oss << c;
      cs.Pop();
      ++mColNum;
      if (c == '?') { break; } // ? can only be at end of identifier
    }
    else { break; }
  }

  std::string text = oss.str();
  TokenType keywordTokenType;
  if (DetermineKeyword(text, &keywordTokenType))
  {
    tokens.emplace_back(keywordTokenType, mLineNum, startColNum);
  }
  else
  {
    tokens.emplace_back(TokenType::IDENTIFIER, std::move(text), mLineNum, startColNum);
  }
}

void Lexer::ParseString(CharStream& cs, char terminator, Tokens& tokens)
{
  bool isEscaped = false;
  std::ostringstream oss;
  int const startColNum = mColNum;
  while (cs.CanPop())
  {
    char c = cs.Pop(); ++mColNum;
    if ((c == terminator) & (!isEscaped))
    {
      tokens.emplace_back(TokenType::STRING, std::move(oss.str()), mLineNum, startColNum);
      return;
    }
    else if (c == '\\') { isEscaped = !isEscaped; }
    else { isEscaped = false; }
    oss << c;
  }
  DIE("Unterminated string at %d:%d", mLineNum, mColNum); // TODO: needs filename...
}

void Lexer::ParseNumber(CharStream& cs, Tokens& tokens)
{
  std::ostringstream oss;
  bool foundDecimal = false;
  int const startColNum = mColNum + 1; // first char was pushed back onto stream
  while (cs.CanPop())
  {
    char c = cs.Pop(); ++mColNum;
    if (isdigit(c)) { oss << c; continue; }
    else if ((c == '.') & (!foundDecimal))
    {
      // we will support opertions like 100.each, so we need to peek ahead
      // to see if the . is an operator or part of a floating point
      char next_c;
      if (cs.Peek(&next_c) && isdigit(next_c))
      {
        foundDecimal = true;
        oss << c;
        continue;
      }
      else { cs.Push(); --mColNum; break; }
    }
    else { cs.Push(); --mColNum; break; }
  }
  tokens.emplace_back(foundDecimal ? TokenType::FLOAT : TokenType::INTEGER, std::move(oss.str()),
                      mLineNum, startColNum);
}

void Lexer::SkipToNextLine(CharStream& cs)
{
 while (cs.CanPop() && '\n' != cs.Pop()) {}
 ++mLineNum;
 mColNum = 0;
}

bool Lexer::DetermineKeyword(std::string const& s, TokenType* out)
{
  static std::unordered_map<std::string, TokenType> KEYWORD_MAP = {
    { "and", TokenType::AND },
    { "bool", TokenType::BOOL },
    { "catch", TokenType::CATCH },
    { "def", TokenType::DEF },
    { "else", TokenType::ELSE },
    { "elif", TokenType::ELSE_IF },
    { "fcn", TokenType::FCN },
    { "flt", TokenType::FLT },
    { "if", TokenType::IF },
    { "int", TokenType::INT },
    { "next", TokenType::NEXT },
    { "NIL", TokenType::NIL },
    { "no", TokenType::NO },
    { "not", TokenType::NOT },
    { "or", TokenType::OR },
    { "self", TokenType::SELF },
    { "str", TokenType::STR },
    { "super", TokenType::SUPER },
    { "try", TokenType::TRY },
    { "unless", TokenType::UNLESS },
    { "until", TokenType::UNTIL },
    { "yes", TokenType::YES },
    { "yield", TokenType::YIELD },
    { "val", TokenType::VAL },
    { "var", TokenType::VAR },
  };

  auto iter = KEYWORD_MAP.find(s);
  if (iter != KEYWORD_MAP.end())  { *out = iter->second; return true; }
  else                            { return false; }
}

}

