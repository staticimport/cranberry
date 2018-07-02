#pragma once

#include "ast/ast.hpp"
#include "ast/type_reference.hpp"

namespace CBC {

class Argument : public ASTNode
{
public:
  ~Argument(void);
  void Parse(TokenStream&) override;
  void Transpile(std::ostream&) const override;
private:
  std::string mName;
  TypeReference mTypeReference;
};


Argument::~Argument(void) {}

void Argument::Parse(TokenStream& tokens)
{
  mName = TokenUtils::PopType(tokens, TokenType::IDENTIFIER, "argument name");
  TokenUtils::PopType(tokens, TokenType::COLON, "argument colon");
  mTypeReference.Parse(tokens);
}

void Argument::Transpile(std::ostream& out) const
{
  out << "Argument::Transpile TODO!\n";
}

}

