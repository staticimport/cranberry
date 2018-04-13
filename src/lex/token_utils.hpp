#pragma once

#include "lex/token.hpp"

namespace CBC {

class TokenUtils
{
public:
  static void PopKeyword(TokenStream&, char const* keyword, char const* context);
  static std::string const& PopType(TokenStream&, TokenType, char const* context);
private:
  static Token const* Take(TokenStream&, char const* context);
  TokenUtils(void) = delete;
};


void TokenUtils::PopKeyword(TokenStream& stream, char const* keyword, char const* context)
{
  Token const* token = Take(stream, context);
  if (token->mType != TokenType::IDENTIFIER or token->mText != keyword)
  {
    // TODO: add filename
    throw ParseError(CBC::FormatStr("%d:%d %s -> failed to find keyword '%s'",
          token->mLineNum, token->mColumn, context, keyword));
  }
}

std::string const& TokenUtils::PopType(TokenStream& stream, TokenType type, char const* context)
{
  Token const* token = Take(stream, context);
  if (token->mType != type)
  {
    // TODO: add filename
    // TODO: add text for which token was expected
    throw ParseError(CBC::FormatStr("%d:%d %s -> failed to find next expected token",
          token->mLineNum, token->mColumn, context));
  }
  return token->mText;
}

Token const* TokenUtils::Take(TokenStream& stream, char const* context)
{
  Token const* token = stream.TryPop();
    // TODO: add filename
  if (!token) { throw ParseError(CBC::FormatStr("%s -> end of stream reached prematurely", context)); }
  return token;
}

}
