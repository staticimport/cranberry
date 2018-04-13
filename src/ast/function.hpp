#pragma once

#include "ast/ast.hpp"
#include "ast/argument.hpp"

namespace CBC {

class Function : public ASTNode
{
public:
  ~Function();
  void Parse(TokenStream&) override;
  void Transpile(std::ostream&) const override;
private:
  std::string mName;
  std::vector<Argument> mArguments;
  TypeReference mReturnTypeReference;
  // TODO: statements
};

Function::~Function() {}

void Function::Parse(TokenStream& tokens)
{
  TokenUtils::PopKeyword(tokens, "fcn", "function definition");
  mName = TokenUtils::PopType(tokens, TokenType::IDENTIFIER, "function name");
  TokenUtils::PopType(tokens, TokenType::LEFT_PAREN, "function args open paren");
  for (Token const* t = tokens.Peek(); t && t->mType != TokenType::RIGHT_PAREN; t = tokens.Peek())
  {
    Argument arg;
    arg.Parse(tokens);
    mArguments.emplace_back(arg);
  }
  TokenUtils::PopType(tokens, TokenType::RIGHT_PAREN, "function args close paren");
  TokenUtils::PopType(tokens, TokenType::COLON, "function args return type colon");
  mReturnTypeReference.Parse(tokens);
  // TODO: statements
  TokenUtils::PopKeyword(tokens, "end", "function end");
}

void Function::Transpile(std::ostream& out) const
{
  out << "Function::Transpile TODO!\n";
}

}
