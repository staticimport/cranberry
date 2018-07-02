#pragma once

#include "lex/token_utils.hpp"

namespace CBC {

class ASTNode
{
public:
  virtual ~ASTNode(void) {}
  virtual void Parse(TokenStream&) = 0;
  virtual void Transpile(std::ostream&) const = 0;
};

}

